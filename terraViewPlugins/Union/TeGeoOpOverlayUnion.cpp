#include "TeGeoOpOverlayUnion.h"

#include "TeQuerier.h"
#include "TeQuerierParams.h"
#include "TeOverlay.h"
#include "TeProgress.h"
#include "TeVectorRemap.h"
#include "TeDatabase.h"

#include "TeAsciiFile.h"

inline bool changeAttrList(TeAttributeList& attrList)
{
	bool change = false;
	
	for(int i=0; i<(int)attrList.size(); ++i) 
	{
		string& nameOr = attrList[i].rep_.name_;
	
		attrList[i].rep_.isAutoNumber_ = false;
		attrList[i].rep_.isPrimaryKey_ = false;
		
		//replace "." for "_"
		string::size_type f = nameOr.find(".");
		if(f != string::npos)
		{
			nameOr.replace(f, 1, "_");
			change = true;
		}

		string nameTemp = TeConvertToUpperCase(nameOr);

		int count = 1;
		int j = i+1;
		while(j<(int)attrList.size())
		{
			if((nameTemp == TeConvertToUpperCase(attrList[j].rep_.name_)) && (i!=j))
			{
				nameTemp = nameTemp +"_"+ Te2String(count);
				nameOr = nameOr +"_"+ Te2String(count);

				j = 0;
				change = true;
				++count;
			}
			++j;
		}
	}

	return change;
}

bool TeGeoOpOverlayUnion(TeLayer* newLayer, TeTheme* theme, TeTheme* themeOverlay, 
				         TeSelectedObjects selOb, TeSelectedObjects selObOverlay, TeAsciiFile* logFile){

		// verifica se os parametros de entrada são validos
		if((!newLayer) || (!theme) || (!themeOverlay))
			return false;

		TeLayer* layer = theme->layer();
		TeLayer* layerOverlay = themeOverlay->layer();

		TeGeomRep geomRepTheme = (TeGeomRep)(theme->visibleRep() & ~TeTEXT);
		TeGeomRep geomRepThemeOverlay = (TeGeomRep)(themeOverlay->visibleRep() & ~TeTEXT); 

		TeProjection* projTheme = layer->projection();
		TeProjection* projOverlay = layerOverlay->projection();

		// Querier Theme
		TeQuerierParams paramsTheme(true, true);
		paramsTheme.setParams(theme);
		paramsTheme.setSelecetObjs(selOb);
		TeQuerier querierTheme(paramsTheme); 

		if(!querierTheme.loadInstances()) // nao ha objetos em theme
			return  false;

		bool insertAttr = false;
		int dt = CLOCKS_PER_SEC/4;
		int dt2 = CLOCKS_PER_SEC * 5;
		clock_t	t0, t1, t2;

		/******** início da criacao da tabela para o novo layer ********/
		TeAttributeList attrList;
		TeAttribute at;
		at.rep_.type_ = TeSTRING;
		at.rep_.name_ = "object_id_" + Te2String(newLayer->id());
		at.rep_.numChar_ = 255;
		at.rep_.isPrimaryKey_ = true;
		at.rep_.isAutoNumber_ = false;
		attrList.push_back(at);

		// Querier Overlay
		TeQuerierParams paramsOverlay(true, true);
		paramsOverlay.setParams(themeOverlay);
		paramsOverlay.setSelecetObjs(selObOverlay);
		TeQuerier querierOverlay(paramsOverlay); 

		if(!querierOverlay.loadInstances()) // nao ha objetos em themeOverlay
			return false;

		// recupera a lista de atributos da querierTheme
		TeAttributeList attribs = querierTheme.getAttrList();
		int numberAtributtesTheme = attribs.size(); // usado na diferença (Overlay - Theme) - look!
		TeAttributeList::iterator it = attribs.begin();
		while(it != attribs.end()){
			attrList.push_back(*it);
			++it;
		}

		// recupera a lista de atributos da querierOverlay
		attribs.clear();
		attribs = querierOverlay.getAttrList(); 
		int numberAtributtesOverlay = attribs.size(); // usado na diferença (Theme - Overlay) - look!
		it = attribs.begin();
		while(it != attribs.end()){
			attrList.push_back(*it);
			++it;
		}
		
		// todos os campos da nova tabela podem ser null 
		it = attrList.begin();
		while(it != attrList.end()){
			(*it).rep_.null_ = true;
			++it;
		}

		changeAttrList(attrList);
		attrList[0].rep_.isPrimaryKey_ = true;

		TeTable attrTable(newLayer->name(), attrList,  attrList[0].rep_.name_, attrList[0].rep_.name_);
		
		if(!newLayer->createAttributeTable(attrTable)) // cria tabela na memoria
			return false;
		/******** fim da criacao da tabela para o novo layer ********/

		int numPolygonsTheme = querierTheme.numElemInstances();
		int numPolygonsThemeOverlay = querierOverlay.numElemInstances();
		// set progress bar
		if(TeProgress::instance()){
			string caption = "Overlay Union";
			string msg = "Executing Overlay Union. Please, wait!";
			TeProgress::instance()->setCaption(caption.c_str());
			TeProgress::instance()->setMessage(msg);
			TeProgress::instance()->setTotalSteps(numPolygonsTheme + numPolygonsThemeOverlay);
			t2 = clock();
			t0 = t1 = t2;
		}

		double t = TeGetPrecision(layer->projection());
		TePrecision::instance().setPrecision(t);	

		TeSTInstance Theme, Overlay;
		TePolygonSet ps; // resultado final para as geometrias
		int newId = 0;

		int steps = 0;

		/******************** inicio do primeiro passo ********************/
		while(querierTheme.fetchInstance(Theme)){
			
			TePolygonSet polySet;
			if(!Theme.getGeometry(polySet))
				continue;

			TeTableRow rowTheme;
			TePropertyVector vecTheme = Theme.getPropertyVector();

			// escreve na linha os atributos de theme
			for(int p = 0; p < (int)vecTheme.size(); p++)
				rowTheme.push_back(vecTheme[p].value_);

			for(int i = 0; i < (int)polySet.size(); i++){ // para cada polígono de theme
				TePolygon polyTheme = polySet[i];
				
				if(!((*projTheme) == (*projOverlay))){
					TePolygon pOut;
					TeVectorRemap(polyTheme, projTheme, pOut, projOverlay);
					pOut.objectId(polyTheme.objectId());
					polyTheme = pOut;
				}
				
				TePolygonSet polyThemeSet;
				polyThemeSet.add(polyTheme);
				
				TeBox boxTheme = polyTheme.box(); // retangulo envolvente do poligono atual de theme

				/* Querier Candidate
				   Recupera os candidatos de themeOverlay para interceptarem theme*/
				TeQuerierParams params(true, true);
				params.setParams(themeOverlay);
				params.setSelecetObjs(selObOverlay);
				params.setSpatialRest(boxTheme, TeINTERSECTS, geomRepThemeOverlay);

				TeQuerier querierCandidate(params);
				
				if(!querierCandidate.loadInstances()) // nao existe candidatos... 
					continue;

				/* Candidatos a intersecao
				   Serao unidos posteriormente para realizacao da diferenca:
				   (theme - themeOverlay) */
				TePolygonSet intersect;
				
				TeSTInstance sti;
				TeTableRow row;
				while(querierCandidate.fetchInstance(sti)){
					TePropertyVector vect = sti.getPropertyVector();
					row.push_back("");
					
					// insere os valores de theme
					for(int i = 0; i < (int)rowTheme.size(); i++)
						row.push_back(rowTheme[i]);
					
					// insere os valores de themeOverlay 
					for(int p = 0; p < (int)vect.size(); p++)
						row.push_back(vect[p].value_);

					if(sti.hasPolygons() && (geomRepThemeOverlay == TePOLYGONS)){
						
						TePolygonSet polySetOverlay;
						sti.getGeometry(polySetOverlay);

						/* Agrupamento dos polígonos para realização da diferença: 
						   (theme - themeOverlay) */
						for(int m = 0; m < (int)polySetOverlay.size(); m++)
							intersect.add(polySetOverlay[m]);
						
						for(int j = 0; j < (int)polySetOverlay.size(); j++){
							TePolygonSet psAux, resultIntersec;
							psAux.add(polySetOverlay[j]);
							
							// interseção entre os polígonos
							if(!TeOVERLAY::TeIntersection(polyThemeSet, psAux, resultIntersec)){
								if (logFile){
									// write log
								}
							}

							// insere a linha na tabela memoria
							if(!resultIntersec.empty()){
								for(int c = 0; c < (int)resultIntersec.size(); c++){
									newId++;
									TePolygon pAux = resultIntersec[c];
									pAux.objectId(Te2String(newId));
									ps.add(pAux);
									row[0] = Te2String(newId);
									attrTable.add(row);
								}
							}
						}
					}
					row.clear();
				}//while fetchInstance de querierCandidate
				
				// Uniao de todos os objetos do themeOverlay que inteceptam objetos do theme
				TePolygonSet unionOverlay, resultDif;
				if(!TeOVERLAY::TeUnion(intersect, unionOverlay)){
					if(logFile){
						// write log
					}
				}
				
				// realiza diferenca entre theme e a uniao dos candidatos de themeOverlay
				if(!TeOVERLAY::TeDifference(polyThemeSet, unionOverlay, resultDif)){
					if (logFile){
						// write log
					}
				}
				
				row.push_back("");
				for(int i = 0; i < (int)rowTheme.size(); i++)
						row.push_back(rowTheme[i]);
				
				for(int p = 0; p < numberAtributtesOverlay; p++) 
					row.push_back("");
				
				//insere a linha na tabela memoria   
				if (!resultDif.empty()){
					for(int c = 0; c < (int)resultDif.size(); c++){
						newId++;
						TePolygon pAux = resultDif[c];
						pAux.objectId(Te2String(newId));
						ps.add(pAux);
						row[0] = Te2String(newId);
						attrTable.add(row);
					}
				}

			} // para cada poligono de theme

			if(geomRepTheme == TePOLYGONS && ps.size()){ // insere as geometrias no layer
				newLayer->addPolygons(ps);
				ps.clear();
			}

			// barra de progresso
			steps++;
			t2 = clock();
			if ((TeProgress::instance()) && (int(t2-t1) > dt))
			{
				t1 = t2;
				if(TeProgress::instance()->wasCancelled())
				{
					TeProgress::instance()->reset();
					return false;
				}
				if((int)(t2-t0) > dt2)
					TeProgress::instance()->setProgress(steps);
			}

			if((steps%100)==0){
				newLayer->saveAttributeTable(attrTable); //save 100 records!!!
				if(attrTable.size()>0)
					insertAttr = true;
				attrTable.clear();
			}
		
		} //while fetchInstance querierTheme
		/********************* fim do primeiro passo *********************
			Operacoes realizadas:
			* Intersecao Theme - ThemeOverlay
			* Diferenca  Theme - ThemeOverlay  
		******************************************************************/

		newLayer->saveAttributeTable(attrTable);
		if(attrTable.size()>0)
			insertAttr = true;
		attrTable.clear();
		ps.clear(); // novas geometrias serao geradas
		
		/******************** inicio do segundo passo ********************/
		while(querierOverlay.fetchInstance(Overlay)){
			
			TePolygonSet polySet;
			if(!Overlay.getGeometry(polySet))
				continue;

			TeTableRow rowOverlay;
			TePropertyVector vecOverlay = Overlay.getPropertyVector();

			// escreve na linha os atributos de themeOverlay
			for(int p = 0; p < (int)vecOverlay.size(); p++)
				rowOverlay.push_back(vecOverlay[p].value_);

			for(int i = 0; i < (int)polySet.size(); i++){ // para cada poligono de themeOverlay
				TePolygon polyOverlay = polySet[i];
				
				if(!((*projTheme) == (*projOverlay))){
					TePolygon pOut;
					TeVectorRemap(polyOverlay, projOverlay, pOut, projTheme);
					pOut.objectId(polyOverlay.objectId());
					polyOverlay = pOut;
				}
				
				TePolygonSet polyOverlaySet;
				polyOverlaySet.add(polyOverlay);
				
				TeBox boxOverlay = polyOverlay.box();

				// Querier Candidate
				TeQuerierParams params(true, true);
				params.setParams(theme);
				params.setSelecetObjs(selOb);
				params.setSpatialRest(boxOverlay, TeINTERSECTS, geomRepTheme);

				TeQuerier querierCandidate(params);
				
				if(!querierCandidate.loadInstances())
					continue;

				// Candidatos a interseção. Serão unidos para realização da diferença
				TePolygonSet intersect;

				TeSTInstance sti;
				while(querierCandidate.fetchInstance(sti)){
					if(sti.hasPolygons() && (geomRepTheme == TePOLYGONS)){
						TePolygonSet polySetTheme;
						sti.getGeometry(polySetTheme);

						// agrupamentos dos candidatos para realização da diferença
						for(int m = 0; m < (int)polySetTheme.size(); m++)
							intersect.add(polySetTheme[m]);
					}
				}//while fetchInstance querierTheme
				
				// Faz a união de todos os objetos do theme que inteceptam objetos do themeOverlay
				TePolygonSet unionTheme, resultDif;
				if(!TeOVERLAY::TeUnion(intersect, unionTheme)){
					if(logFile){
						// write log
					}
				}
				if(!TeOVERLAY::TeDifference(polyOverlaySet, unionTheme, resultDif)){
					if (logFile){
						// write log
					}
				}
				
				TeTableRow row;
				row.push_back("");
				
				/*  A tabela do novo layer tem os atributos de theme = null 
					para a diferenca: (themeOverlay - theme) */
				for(int p = 0; p < numberAtributtesTheme; p++) 
					row.push_back("");

				for(int i = 0; i < (int)rowOverlay.size(); i++)
					row.push_back(rowOverlay[i]);
				
				if (!resultDif.empty()){
					for(int c = 0; c < (int)resultDif.size(); ++c){
						newId++;
						TePolygon pAux = resultDif[c];
						pAux.objectId(Te2String(newId));
						ps.add(pAux);
						row[0] = Te2String(newId);
						attrTable.add(row);
					}
				}
				rowOverlay.clear();
				row.clear();
			}// para cada poligono de themeOverlay	
			
			if(geomRepThemeOverlay == TePOLYGONS && ps.size()) // insere as geometrias no layer
				newLayer->addPolygons(ps);

			// progresso!
			steps++;
			t2 = clock();
			if ((TeProgress::instance()) && (int(t2-t1) > dt))
			{
				t1 = t2;
				if(TeProgress::instance()->wasCancelled())
				{
					TeProgress::instance()->reset();
					return false;
				}
				if((int)(t2-t0) > dt2)
					TeProgress::instance()->setProgress(steps);
			}

			if((steps%100)==0){
				newLayer->saveAttributeTable(attrTable); //save 100 records!!!
				if(attrTable.size()>0)
					insertAttr = true;
				attrTable.clear();
			}
		
		}//while fetchInstance querierOverlay
		/********************* fim do segundo passo *********************
			Operacao realizada:
			* Diferenca  ThemeOverlay - Theme
		******************************************************************/
		
		if(TeProgress::instance())
			TeProgress::instance()->reset();

		// salva a tablela no banco 
		newLayer->saveAttributeTable(attrTable);
		if(attrTable.size()>0)
			insertAttr = true;
		
		if(!newLayer->box().isValid())
			return false;

		if(!insertAttr)
			return false; 
		
		return true; // Sorria! =)
}	
