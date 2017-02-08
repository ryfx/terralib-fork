#include "TeDisolve.h"

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

bool TeOpDisolve(TeLayer* newLayer, TeTheme* theme, TeSelectedObjects selObj){
		
		// verifica se os parametros de entrada são validos
		if((!newLayer) || (!theme))
			return false;

		//TeLayer* layer = theme->layer();
		TeGeomRep geomRepTheme = (TeGeomRep)(theme->visibleRep() & ~TeTEXT);
			

		// Querier Theme
		TeQuerierParams paramsTheme(true, true);
		paramsTheme.setParams(theme);
		paramsTheme.setSelecetObjs(selObj);
		TeQuerier querierTheme(paramsTheme); 

		if(!querierTheme.loadInstances()) // nao ha objetos em theme
			return false;

		/******** início da criacao da tabela para o novo layer ********/
		TeAttributeList attrList;
		TeAttribute at;
		at.rep_.type_ = TeSTRING;
		at.rep_.name_ = "object_id_" + Te2String(newLayer->id());
		at.rep_.numChar_ = 255;
		at.rep_.isPrimaryKey_ = true;
		at.rep_.isAutoNumber_ = false;
		attrList.push_back(at);

		// recupera a lista de atributos da querierTheme
		TeAttributeList attribs = querierTheme.getAttrList();
		TeAttributeList::iterator it = attribs.begin();
		while(it != attribs.end()){
			attrList.push_back(*it);
			++it;
		}

		changeAttrList(attrList);
		attrList[0].rep_.isPrimaryKey_ = true;

		TeTable attrTable(newLayer->name(), attrList,  attrList[0].rep_.name_, attrList[0].rep_.name_);

		if(!newLayer->createAttributeTable(attrTable))
			return false;
		/******** fim da criacao da tabela para o novo layer ********/

		
		bool insertAttr = false;
		int dt = CLOCKS_PER_SEC/4;
		int dt2 = CLOCKS_PER_SEC * 5;
		clock_t	t0, t1, t2;

		int numPolygonsTheme = querierTheme.numElemInstances();
		// set progress bar
		if(TeProgress::instance()){
			string caption = "Disolve";
			string msg = "Executing Disolve. Please, wait!";
			TeProgress::instance()->setCaption(caption.c_str());
			TeProgress::instance()->setMessage(msg);
			TeProgress::instance()->setTotalSteps(numPolygonsTheme);
			t2 = clock();
			t0 = t1 = t2;
		}

		TeSTInstance STheme;
		TePolygonSet ps, polySet;;
		TeLineSet ls, lineSet;
		TePointSet pts, pointSet;
		int newId = 0;

		int steps = 0;
		while(querierTheme.fetchInstance(STheme)){ // para cada objeto de Theme
			
			
			if(geomRepTheme == TePOLYGONS){
				if(!STheme.getGeometry(polySet))
					continue;
			}else{
				if(geomRepTheme == TeLINES){
					if(!STheme.getGeometry(lineSet))
						continue;
				}else{
					if(!STheme.getGeometry(pointSet))
						continue;
				}
			}

			TeTableRow rowTheme;
			rowTheme.push_back("");

			TePropertyVector vecTheme = STheme.getPropertyVector();

			// escreve na linha os atributos do objeto corrente de theme
			for(int p = 0; p < (int)vecTheme.size(); p++)
				rowTheme.push_back(vecTheme[p].value_);
			
			if(geomRepTheme == TePOLYGONS){
				for(int i = 0; i < (int)polySet.size(); i++){ // para cada POLIGONO do objeto corrente
					TePolygon polyTheme = polySet[i];
					newId++;
					polyTheme.objectId(Te2String(newId));
					ps.add(polyTheme);
					newLayer->addPolygons(ps);
					rowTheme[0] = Te2String(newId);
					attrTable.add(rowTheme);
					ps.clear();
				}
			}else{
				if(geomRepTheme == TeLINES){
					for(int i = 0; i < (int)lineSet.size(); i++){ // para cada LINHA do objeto corrente
						TeLine2D lineTheme = lineSet[i];
						newId++;
						lineTheme.objectId(Te2String(newId));
						ls.add(lineTheme);
						newLayer->addLines(ls);
						rowTheme[0] = Te2String(newId);
						attrTable.add(rowTheme);
						ls.clear();
					}
				}else{
					for(int i = 0; i < (int)pointSet.size(); i++){ // para cada PONTO do objeto corrente
						TePoint pointTheme = pointSet[i];
						newId++;
						pointTheme.objectId(Te2String(newId));
						pts.add(pointTheme);
						newLayer->addPoints(pts);
						rowTheme[0] = Te2String(newId);
						attrTable.add(rowTheme);
						pts.clear();
					}
				}
			}
			rowTheme.clear();

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
		}

		if(TeProgress::instance())
			TeProgress::instance()->reset();

		newLayer->saveAttributeTable(attrTable);
		if(attrTable.size()>0)
			insertAttr = true;
		attrTable.clear();

		if(!newLayer->box().isValid())
			return false;

		if(!insertAttr)
			return false; 
		
		return true;
}