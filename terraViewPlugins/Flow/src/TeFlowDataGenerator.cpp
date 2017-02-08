/************************************************************************************
TerraLib - a library for developing GIS applications.
Copyright © 2001-2004 INPE and Tecgraf/PUC-Rio.

This code is part of the TerraLib library.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

You should have received a copy of the GNU Lesser General Public
License along with this library.

The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The library provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use
of this library and its documentation.
*************************************************************************************/

#include "TeFlowMetadata.h"
#include "TeFlowDataGenerator.h"
#include "TeDatabase.h"
#include "TeQuerier.h"
#include "TeQuerierParams.h"
#include "TeBufferRegion.h"
#include <math.h>                    // for trigonometry functions


// converte radianos em graus decimal
inline double fromRadianosToSegundosDecimal(const double& rd)
{
	return ((rd * 180.0 * 3600.0) / TePI);
}

// converte segundos decimais em radianos
inline double fromSegundosDecimalToRadianos(const double& sd)
{
	return ((sd * TePI) / (3600.0 * 180.0));
}

// cA e cB em radianos
double TeDistanciaGeodesica(const TeCoord2D& cA, const TeCoord2D& cB, const TeDatum& datum)
{
	// Longitudes entre -180 e +180
	if(fabs(cA.x_) >= TePI)
		return 0.0;
	
	if(fabs(cB.x_) >= TePI)
		return 0.0;

	// Latitudes entre -90 e +90
	if(fabs(cA.y_) >= (TePI / 2.0))
		return 0.0;
	
	if(fabs(cB.y_) >= (TePI / 2.0))
		return 0.0;


	// Informações do datum
	double rd = datum.radius();			// Raio equatorial da Terra no elipsóide 
	double flt = datum.flattening();	// Coeficiente de achatamento (admensional)

	// Latitude dos pontos em radianos
	double latRdA = cA.y_;
	double latRdB = cB.y_;

	// Longitude dos pontos em radianos
	double longRdA = cA.x_;
	double longRdB = cB.x_;

	// Latitudes e longitudes em segundos decimal
	double latSdA = fromRadianosToSegundosDecimal(latRdA);
	double latSdB = fromRadianosToSegundosDecimal(latRdB);
	double longSdA = fromRadianosToSegundosDecimal(longRdA);
	double longSdB = fromRadianosToSegundosDecimal(longRdB);

	double e2 = (2.0 * flt) - (flt * flt);	// Excentricidade ao quadrado

	// deltaLat e deltaLong em segundos
	double deltaLat  = latSdB - latSdA;
	double deltaLong = longSdB - longSdA;


	// Senos e cossenos auxiliares
	double cosLatB = cos(latRdB);
	double senLatB = sin(latRdB);
	double tgLatB  = tan(latRdB);
	double sen1Segundo = sin(fromSegundosDecimalToRadianos(1.0));

	// Grande Normal em relação ao ponto B
	double denominadorNb2 = 1.0 - (e2 * (senLatB * senLatB));	// denominador da grande normal ao quadrado
	double Nb = rd / sqrt(denominadorNb2);

	double denominadorAb = Nb * sen1Segundo;
	//double Ab = 1 / denominadorAb;

	double X = deltaLong * cosLatB * denominadorAb;

	// Raio da seção meridiana
	double Mb = (rd * (1.0 - e2)) / pow(denominadorNb2, 1.5);

	double denominadorBb = Mb * sen1Segundo;
	//double Bb = 1 / denominadorBb;

	double Cb = tgLatB / (2.0 * Mb * Nb * sen1Segundo);	

	double Eb = (1.0 + 3.0 * (tgLatB * tgLatB)) / (6.0 * Nb * Nb);	

	double Db = (3.0 * e2 * cosLatB * senLatB * sen1Segundo) / (2.0 * denominadorNb2);

	double Y = denominadorBb * (deltaLat - (Cb * X * X) + (deltaLat * X * X * Eb) -( deltaLat * deltaLat * Db));

	// Azimute
	double A = atan2(X, Y);

	double distanciaGeodesica = 0.0;
	
	if(deltaLong != 0.0)
		distanciaGeodesica  = X / sin(A); 
	else
		distanciaGeodesica  = Y / cos(A);

	return fabs(distanciaGeodesica);

}

// Funcao auxiliar que dado uma coluna, verifica se ela existe e se o nome ja existir, tenta criar um novo nome
void validColumnName2(TeAttributeRep& rep, TeAttributeList& attrList)
{
	unsigned int nStep = attrList.size();

	string repName = TeConvertToUpperCase(rep.name_);

	bool alter = true;

	while(alter)
	{
		alter = false;

		for(unsigned int i = 0; i < nStep; ++i)
		{
			if(TeConvertToUpperCase(attrList[i].rep_.name_) == repName)
			{
				rep.name_ += "_";
				alter = true;
				repName = TeConvertToUpperCase(rep.name_);
			}
		}
	}
}


//TeFlowDataGenerator class implementation
TeFlowDataGenerator::TeFlowDataGenerator(TeTheme* theme)
	: referenceTheme_(theme)
{			
}

TeLayer* TeFlowDataGenerator::buildFlowData(const string& flowTableName,
										    const string& strOrigin,
										    const string& strDestiny,
											const string& referenceThemeLinkColumnName,
										    const string& newLayerName)
{
// checa se o tema de refencia esta carregado
	errorMessage_ = "";

	if(!referenceTheme_)
	{
		errorMessage_ = "No reference theme defined!";
		return 0;
	}	

// recupera as informacoes de metadado da tabela externa que contem os fluxos: vamos utilizar
// principalmente a coluna de ligacao
// e tambem faz a validacao para ver se a tabela nao esta associada a outra tabela no banco
	TeTable flowTable(flowTableName);

	if(!getFlowTableInfo(flowTable))
		return 0;

// carrega o mapa com os centroides dos objetos do tema de referencia, para os quais podera haver fluxos
	TeFlowDataGenerator::TeCentroidMap centerMap;
	
	if(!loadCentroidMap(referenceThemeLinkColumnName, centerMap))
		return 0;

// adiciona um campo para armazenar a distancia na tabela de fluxo
	string newDistanceColumnName;
	
	if(!addDistanceColumn(flowTableName, newDistanceColumnName))
		return 0;

// cria um novo layer
	TeDatabase* db = referenceTheme_->layer()->database();

	TeLayer* newLayer = new TeLayer(newLayerName, db, referenceTheme_->layer()->projection());

	if(!newLayer)
	{
		errorMessage_ = "Could not create a new layer (" + newLayerName + ")!";
		return 0;
	}

// cria as linhas de fluxo e grava no novo layer
	TeFlowMetadata flowMetadata(db);
	flowMetadata.flowTableDestinyColumnName_ = strDestiny;
	flowMetadata.flowTableOriginColumnName_  = strOrigin;
	flowMetadata.referenceColumnName_ = TeGetExtension(referenceThemeLinkColumnName.c_str());

	if(!createFlow(newLayer, flowTable, newDistanceColumnName, flowMetadata, centerMap))
	{
// remove o layer recem criado
		db->deleteLayer(newLayer->id());

// remove a coluna de distancia recem criada
		db->deleteColumn(flowTableName, newDistanceColumnName);

		return 0;
	}

// atualiza as informacoes da tabela externa de fluxo: registra a tabela no novo layer
	if(!registerFlowTable(newLayer, flowTable))
	{
		string errorMsgAux = errorMessage_;

		if(!rollbackFlowTableInfo(flowTable))
			return 0;	

		errorMessage_ += errorMsgAux;

// remove o layer recem criado
		db->deleteLayer(newLayer->id());

// remove a coluna de distancia recem criada
		db->deleteColumn(flowTableName, newDistanceColumnName);

		return 0;
	}
  	
// salva os metadados de fluxo da camada criada
	flowMetadata.flowLayerId_ = newLayer->id();
	flowMetadata.flowTableId_ = flowTable.id();
	flowMetadata.referenceLayerId_ = referenceTheme_->layer()->id();

	string refTableName = referenceTheme_->getTableName(referenceThemeLinkColumnName);

    if(!saveFlowLayerMetadata(flowMetadata, refTableName))
	{
		string errorMsgAux = errorMessage_;

		if(!rollbackFlowTableInfo(flowTable))
			return 0;	

		errorMessage_ += errorMsgAux;

// remove o layer recem criado
		db->deleteLayer(newLayer->id());

// remove a coluna de distancia recem criada
		db->deleteColumn(flowTableName, newDistanceColumnName);

		return 0;
	}	

	return newLayer;
}

bool TeFlowDataGenerator::loadCentroidMap(const string& referenceThemeLinkColumnName, TeFlowDataGenerator::TeCentroidMap& centerMap)
{
	//----- This is used for progress bar
	int dt = CLOCKS_PER_SEC/4;
	int dt2 = CLOCKS_PER_SEC * 5;
	clock_t	t0, t1, t2;
	t2 = clock();
	t0 = t1 = t2;
	//-----
	
// checa se o tema de refencia esta carregado
	errorMessage_ = "";

	if(!referenceTheme_)
	{
		errorMessage_ = "No reference theme defined!";
		return false;
	}

// tenta carregar as geometrias do tema de referencia, usando o querier
	bool loadGeometries = true;

	vector<string> attrs;
	attrs.push_back(referenceThemeLinkColumnName);

    TeQuerierParams querierParams(loadGeometries, attrs);
	querierParams.setParams(referenceTheme_);
    TeQuerier querier(querierParams);

	if(!querier.loadInstances())
	{
		errorMessage_ = "Could not load instances from reference theme!";
		return false;
	}

// Percorre as geometria, carregando o MAP de centroides
	string keyValue;		// data 0 map
	TeCoord2D coordValue;	// data 1 map
	
	unsigned int steps = querier.numElemInstances();
	unsigned int steps_aux = 0;
		
	if(TeProgress::instance())
	{
		TeProgress::instance()->setTotalSteps(steps);
		TeProgress::instance()->setCaption("Loading reference theme centroids");
		TeProgress::instance()->setMessage("Processing...      ");
	}

	TeSTInstance sti;

    while(querier.fetchInstance(sti))
    {
		if(TeProgress::instance()->wasCancelled())
		{
			TeProgress::instance()->reset();
			errorMessage_ = "The process was canceled by user!";

			return false;
		}
		
		TeProperty prop;
		sti.getProperty(prop, 0);
		keyValue = prop.value_;
		
		sti.centroid(coordValue);
		centerMap.insert(pair<string, TeCoord2D>(keyValue, coordValue));
		steps_aux++;

		if(TeProgress::instance())
		{
			t2 = clock();
			if(int(t2-t1) > dt)
			{
				t1 = t2;
				
				if((int)(t2-t0) > dt2)
					TeProgress::instance()->setProgress(steps_aux);
			}
		}
    }
	querier.clear();

	if(TeProgress::instance())
		TeProgress::instance()->reset();

	return true;
}

bool TeFlowDataGenerator::addDistanceColumn(const string& flowTableName, string& distanceColumnName)
{
// checa se o tema de refencia esta carregado
	errorMessage_ = "";

	if(!referenceTheme_)
	{
		errorMessage_ = "No reference theme defined!";
		return false;
	}

	TeDatabase* db = referenceTheme_->layer()->database();

	TeAttributeRep at1;
	at1.type_ = TeREAL;
	at1.isPrimaryKey_ = false;
	at1.name_ = "DISTANCE";

	TeAttributeList attrList;

// Recupera a lista de atributos da tabela com o fluxo
	if(!db->getAttributeList(flowTableName, attrList))
	{
		errorMessage_ = "Couldn't add distance column to flow table!";

		if(!db->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += db->errorMessage();
		}

		return false;
	}

// valida o nome da coluna at1 a ser adicionada na tabela de fluxo
	validColumnName2(at1, attrList); 

// tenta adicionar a coluna de distancia
	if(!db->addColumn(flowTableName, at1))
	{
		errorMessage_ = "Couldn't add distance column to flow table!";

		if(!db->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += db->errorMessage();
		}

		return false;
	}

// atualiza a coluna de distancia com valores padroes
	string value = "0";
	string sql  = "UPDATE ";
	sql += flowTableName;
	sql += " SET ";
	sql += at1.name_;
	sql += " = '";
	sql += value;
	sql += "'";

	if(!db->execute(sql))
	{
		errorMessage_ = "Couldn't update distance on flow table!";

		if(!db->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += db->errorMessage();
		}

		return false;
	}

	distanceColumnName = at1.name_;

	return true;
}

bool TeFlowDataGenerator::getFlowTableInfo(TeTable& flowTable)
{
// checa se o tema de refencia esta carregado
	errorMessage_ = "";

	if(!referenceTheme_)
	{
		errorMessage_ = "No reference theme defined!";
		return false;
	}

	TeDatabase* db = referenceTheme_->layer()->database();	

	if(!db->loadTableInfo(flowTable))
	{
		errorMessage_ = "Couldn't load flow table info from te_layer_table!";

		if(!db->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += db->errorMessage();
		}

		return false;
	}

// verificar se a tabela nao esta associada a alguma outra e se ela e externa
	if(flowTable.tableType() != TeAttrExternal)
	{
		errorMessage_ = "The informed flow table is not an external table!";
		return false;
	}

	if(flowTable.relatedTableId() >= 0)	// se existe uma tabela ligada
	{
		errorMessage_  = "The informed flow table is linked to " + flowTable.relatedTableName() + "!\n";
		errorMessage_ += "Please, unlink table before calling flow data generator!";
		return false;
	}

	return true;
}

bool TeFlowDataGenerator::createFlow(TeLayer* newLayer, TeTable& flowTable, const string& distanceColumnName, const TeFlowMetadata& flowMetadata, TeFlowDataGenerator::TeCentroidMap& centerMap)
{
// checa se o tema de refencia esta carregado
	errorMessage_ = "";

	if(!referenceTheme_)
	{
		errorMessage_ = "No reference theme defined!";
		return false;
	}	

//----- This is used for progress bar
	int dt = CLOCKS_PER_SEC/4;
	int dt2 = CLOCKS_PER_SEC * 5;
	clock_t	t0, t1, t2;
	t2 = clock();
	t0 = t1 = t2;
	int steps_aux = 0;
//-----	

	if(TeProgress::instance())
	{
		TeProgress::instance()->setTotalSteps(NULL);
		TeProgress::instance()->setCaption("Creating flow lines...");
		TeProgress::instance()->setMessage("Processing...      ");
	}	

// verifica se e possivel obter as coordenadas em latitude/longitude, isto fara diferenca para o calculo da distancia
// entre as extremidades dos fluxos
	TeProjection* currentProj = referenceTheme_->layer()->projection();

	bool doRemap = true;

	if(currentProj->name() == "NoProjection")
		doRemap = false;

	double bufferRadius = 0.08;

// abre um portal para a tabela com os fluxos
	TeDatabase* db = referenceTheme_->layer()->database();

	TeDatabasePortal* portal = db->getPortal();

	if(!portal)
    {
		errorMessage_ = "Error getting a portal to table: " + flowTable.name() + "!";

		if(!portal->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += portal->errorMessage();
		}

		return false;
    }
	
	string sql_flow;
    sql_flow  = "SELECT ";
//	sql_flow += flowTable.linkName();
	sql_flow += "unique_id_";
	sql_flow += ", ";
	sql_flow += flowMetadata.flowTableOriginColumnName_;
	sql_flow += ", ";
	sql_flow += flowMetadata.flowTableDestinyColumnName_;
	sql_flow += " FROM ";
	sql_flow += flowTable.name();
	sql_flow += " ORDER BY ";
	sql_flow += "unique_id_";
//	sql_flow += flowTable.linkName();

	if(!portal->query(sql_flow))
	{
		errorMessage_ = "Error quering table: " + flowTable.name() + "!";

		if(!portal->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += portal->errorMessage();
		}

		portal->freeResult();
		delete portal;
		return false;
	}

	TeLineSet lSet;
	TePolygonSet pSet;

	unsigned int i = 0;
	double distFlow = 0.0;

// navega pelos registros da tabela de fluxo gerando as linhas e poligonos
	while(portal->fetchRow())
	{
		if(TeProgress::instance()->wasCancelled())
		{	
			TeProgress::instance()->reset();

			errorMessage_ = "The process was canceled by user!";

			portal->freeResult();
			delete portal;
			return false;
		}

		++i;
		++steps_aux;

		TeLine2D l_flow;
	
// recupera os valores de: id do fluxo (deve existir na tabela de entrada), id do objeto de origem e id do objeto de destino
		string objectId_flow =   portal->getData(0);	// Id do fluxo
		string originVal_flow  = portal->getData(1);	// origem
		string destinyVal_flow = portal->getData(2);	// destino
		l_flow.objectId(objectId_flow);

// verifica se exite as coordenadas de origem e destino no MAP
		TeCentroidMap::iterator centerOrigin = centerMap.find(originVal_flow);
		TeCentroidMap::iterator centerDestiny = centerMap.find(destinyVal_flow);

		if((centerOrigin == centerMap.end()) || (centerDestiny == centerMap.end()))
		{
			if (TeProgress::instance())
				TeProgress::instance()->reset();

			errorMessage_ = "Could not find an origin for object: " + originVal_flow + ", or a destiny object: " + destinyVal_flow +  "!";

			portal->freeResult();
			delete portal;
			return false;
		}

// verifica se a origem e destio sao diferentes, para tracar uma linha
		if(originVal_flow != destinyVal_flow) 
		{   
			drawArrow(l_flow, centerOrigin->second, centerDestiny->second);

			unsigned int val = l_flow.size();
			//l_flow.add(centerOrigin->second);
			//l_flow.add(centerDestiny->second);

			if(doRemap)
			{
				TeCoord2D cOriginRad = currentProj->PC2LL(centerOrigin->second);
				TeCoord2D cDestinyRad = currentProj->PC2LL(centerDestiny->second);

				distFlow = TeDistanciaGeodesica(cOriginRad, cDestinyRad, currentProj->datum()) / 1000.0;
			}
			else
			{
				distFlow = TeDistance(l_flow[0], l_flow[1]) / 1000.0;
			}
		}
		else	// gera um poligono para representar um fluxo interno
		{
			TePolygon circle_flow;
			TeBUFFERREGION::TeBufferRegion(centerOrigin->second, bufferRadius, 30, circle_flow);
			circle_flow.objectId(objectId_flow);
			pSet.add(circle_flow);

			l_flow.add(centerOrigin->second);
			l_flow.add(centerDestiny->second);
			
			distFlow = 0;
		}
	
		if(TeProgress::instance())
		{
			t2 = clock();
			if(int(t2-t1) > dt)
			{
				t1 = t2;
		
				if((int)(t2-t0) > dt2)
					TeProgress::instance()->setProgress(steps_aux);
			}
		}
		
		lSet.add(l_flow);
		
		if(!updateColumn(distanceColumnName, distFlow, flowTable, objectId_flow))
		{
			portal->freeResult();
			delete portal;			
			return false;
		}

// verifica se e hora de atualizar o layer com as geometrias
		if(i == 100)
		{
			if(!newLayer->addLines(lSet))
			{
				errorMessage_ = "Could not add flow lines!";

				portal->freeResult();
				delete portal;
				return false;
			}

			if(!newLayer->addPolygons(pSet))
			{
				errorMessage_ = "Could not add flow polygons!";

				portal->freeResult();
				delete portal;
				return false;
			}

			lSet.clear();
			pSet.clear();

			i = 0;
		}
	}	

	portal->freeResult();
	delete portal;

// restaram linhas e poligonos?
	if(i != 0)
	{
		if(!newLayer->addLines(lSet))
		{
			errorMessage_ = "Could not add flow lines!";
			return false;
		}

		if(!newLayer->addPolygons(pSet))
		{
			errorMessage_ = "Could not add flow polygons!";
			return false;
		}

		lSet.clear();
		pSet.clear();
	}

	if (TeProgress::instance())
		TeProgress::instance()->reset();

// nao fez nenhum fetchrow => erro!
	if(steps_aux == 0)
	{
		errorMessage_ = "The specified table (" + flowTable.name() + " is empty!";		
		return false;
	}

	return true;
}

bool TeFlowDataGenerator::registerFlowTable(TeLayer* newLayer, TeTable& flowTable)
{
// checa se o tema de refencia esta carregado
	errorMessage_ = "";

	if(!referenceTheme_)
	{
		errorMessage_ = "No reference theme defined!";
		return false;
	}

	TeDatabase* db = referenceTheme_->layer()->database();	

// tenta registrar a tabela selecionada no metadado do novo layer	
// mas primeiro remove o metadado dela com tabela de fluxo na te_layer_table
	string deleteFlowTable  = "DELETE FROM te_layer_table WHERE (layer_id IS NULL) AND table_id = ";
           deleteFlowTable += Te2String(flowTable.id());

	if(!db->execute(deleteFlowTable))
	{
		errorMessage_ = "Couldn't remove flow table metadata from te_layer_table!";

		if(!db->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += db->errorMessage();
		}

		return false;
	}

	flowTable.setTableType(TeAttrStatic);
	flowTable.setId(-1);
	flowTable.setLinkName("unique_id_");

// atualiza os metadados da tabela do layer (te_layer_table)
	if(!newLayer->createAttributeTable(flowTable))
	{
		errorMessage_ = "Couldn't register flow attribute table in te_layer_table!";

		if(!db->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += db->errorMessage();
		}

		return false;
	}	

// o load e importante porque adicionamos a coluna de distancia que nao existia
	if(!db->loadTableInfo(newLayer->attrTables()[0]))
	{
		errorMessage_ = "Couldn't load table info for new attribute layer table!";

		if(!db->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += db->errorMessage();
		}

		return false;
	}

	flowTable = newLayer->attrTables()[0];

	return true;
}

bool TeFlowDataGenerator::saveFlowLayerMetadata(TeFlowMetadata& flowMetadata, const string& referenceAttTableName)
{
// checa se o tema de refencia esta carregado
	errorMessage_ = "";

	if(!referenceTheme_)
	{
		errorMessage_ = "No reference theme defined!";
		return false;
	}

	TeDatabase* db = referenceTheme_->layer()->database();	

// Carrega o metadado da tabela de atributo do tema de referencia, para obtermos o
// id do layer de referencia
	TeTable auxTable(referenceAttTableName);

	if(!db->loadTableInfo(auxTable))
	{
		errorMessage_ = "Couldn't load table info for reference attribute layer table!";

		if(!db->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += db->errorMessage();
		}

		return false;
	}

    flowMetadata.referenceTableId_ = auxTable.id();

// salva o metadado da camada de fluxo
	if(!flowMetadata.insertMetadata())
	{
		errorMessage_ = flowMetadata.errorMessage();
		return false;
	}

	return true;
}

bool TeFlowDataGenerator::updateColumn(const string& columnName, const double& dist, TeTable& flowTable, const string& objId)
{
// checa se o tema de refencia esta carregado
	errorMessage_ = "";

	if(!referenceTheme_)
	{
		errorMessage_ = "No reference theme defined!";
		return false;
	}

	TeDatabase* db = referenceTheme_->layer()->database();	

// atualiza a coluna de distancia
	string sql  = "UPDATE ";
	       sql += flowTable.name();
		   sql += " SET ";
		   sql += columnName;
		   sql += " = ";
		   sql += Te2String(dist);
		   sql += " WHERE ";
		   sql += "unique_id_";
		   sql += " = '";
		   sql += objId;
		   sql += "'";

	if(!db->execute(sql))
	{
		errorMessage_ = "Couldn't update distance on flow table!";

		if(!db->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += db->errorMessage();
		}

		return false;
	}		   

	return true;
}

bool TeFlowDataGenerator::rollbackFlowTableInfo(TeTable& flowTable)
{
// checa se o tema de refencia esta carregado
	errorMessage_ = "";

	if(!referenceTheme_)
	{
		errorMessage_ = "No reference theme defined!";
		return false;
	}

	TeDatabase* db = referenceTheme_->layer()->database();	

// remove as informacoes de metadado da tabela de atributos em te_layer_table
	string deleteFlowTable  = "DELETE FROM te_layer_table WHERE table_id = ";
		   deleteFlowTable += Te2String(flowTable.id());

	if(!db->execute(deleteFlowTable))
	{
		errorMessage_ = "Couldn't remove flow table metadata from te_layer_table during rollback!";

		if(!db->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += db->errorMessage();
		}

		return false;
	}

// seta as informacoes necessarias para voltar a tabela como externa
	flowTable.setId(-1);
	flowTable.setTableType(TeAttrExternal);

	if(!db->insertTableInfo(-1, flowTable))
	{
		errorMessage_ = "Couldn't rollback flow table info!";

		if(!db->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += db->errorMessage();
		}

		return false;
	}
	
	return true;
}

void TeFlowDataGenerator::drawArrow(TeLine2D& line, TeCoord2D coordInit, TeCoord2D coordEnd)
{
	double slopy, cosy, siny;

	double size = 0.08;

	slopy = atan2((double)( coordInit.y() - coordEnd.y() ),(double)( coordInit.x() - coordEnd.x() ) );
	cosy = cos( slopy );
	siny = sin( slopy ); 

	line.add((TeCoord2D(coordInit.x(),coordInit.y())));
	line.add((TeCoord2D(coordEnd.x(),coordEnd.y())));
	line.add((TeCoord2D(coordEnd.x() -(-size*cosy-(size/2.0*siny)),coordEnd.y() -(-size*siny+(size/2.0*cosy )))));
	line.add((TeCoord2D(coordEnd.x() -(-size*cosy+(size/2.0*siny)),coordEnd.y() +(size/2.0*cosy+size*siny))));
	line.add((TeCoord2D(coordEnd.x(),coordEnd.y())));
}

