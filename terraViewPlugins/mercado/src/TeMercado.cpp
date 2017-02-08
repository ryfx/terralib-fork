#include "TeMercado.h"
#include <TeGeometryAlgorithms.h>
#include <TeVectorRemap.h>
#include <TeAppTheme.h>
#include <TeApplicationUtils.h>
#include <TeDriverCSV.h>
#include <TeTable.h>
#include <TeUtils.h>
#include <TeAsciiFile.h>

TeMercado::TeMercado(TeDatabase* database):currentDatabase_(database)
{
};

TeMercado::~TeMercado()
{
};

bool TeMercado::listCandidateFlowDataTables(vector<string>& tables)
{
	if(!currentDatabase_)
	{
		return false;
	}

	string sql = "SELECT table_id, attr_table FROM te_layer_table WHERE attr_table_type = ";
		   sql += Te2String(TeAttrExternal);
		   sql += " AND attr_table NOT IN (SELECT external_table_name FROM te_tables_relation)";		   

	TeDatabasePortal* portal = currentDatabase_->getPortal();

	if(!portal)
	{
		return false;
	}

	if(!portal->query(sql))
	{
		portal->freeResult();
		delete portal;
		return false;
	}

	while(portal->fetchRow())
		tables.push_back(portal->getData("attr_table"));

	portal->freeResult();
	delete portal;

	return true;
}

vector<string> TeMercado::getDistinctObjects(const string& tableName, const string& columnName)
{
	vector<string> objVec;

  	string sql = "SELECT DISTINCT ";
		   sql+= columnName;
		   sql+= " FROM ";
		   sql+= tableName;

	TeDatabasePortal* portal = currentDatabase_->getPortal();


	if(!portal->query(sql))
	{
		delete portal;
	}

	while(portal->fetchRow())
	{
		objVec.push_back(portal->getData(0));
	}

	return objVec;
}

vector<string> TeMercado::getDistinctLabelObjects(const string& tableName, const string& columnName, const string& columnLabelName)
{
	vector<string> objVec;

	string sql = "SELECT DISTINCT ";
		   sql+= columnName;
		   sql+= ", ";
		   sql+= columnLabelName;
		   sql+= " FROM ";
		   sql+= tableName;

	TeDatabasePortal* portal = currentDatabase_->getPortal();


	if(!portal->query(sql))
	{
		delete portal;
	}

	while(portal->fetchRow())
	{
		objVec.push_back(portal->getData(1));
	}

	sort(objVec.begin(), objVec.end());

	return objVec;
}

vector<string> TeMercado::getDistinctTableObjects(const string& fileTableName, const string& layerName, const string& layerTableName, const string& columnName, const string& columnLabelName)
{
	vector<string> objVec;

	MercadoMap map = this->getMercadoMap(fileTableName);


	MercadoMap::iterator it = map.begin();

	while(it!= map.end())
	{
		std::string value = it->first;

		string sql = "SELECT ";
		sql+= columnLabelName;
		sql+= " FROM ";
		sql+= layerTableName;
		sql+= " WHERE ";
		sql+= columnName;
		sql+= " = '";
		sql+= value;
		sql+= "'";

		TeDatabasePortal* portal = currentDatabase_->getPortal();

		if(!portal->query(sql))
		{
			delete portal;
		}

		while(portal->fetchRow())
		{
			objVec.push_back(portal->getData(0));
		}

		++it;
	}

	sort(objVec.begin(), objVec.end());

	return objVec;
}

vector<string> TeMercado::getLabelObjects(const string& tableName)
{
	vector<string> objVec;

	if(!tableName.empty() && currentDatabase_->tableExist(tableName))
	{
		string sql = "SELECT * ";
		sql+= " FROM ";
		sql+= tableName;

		TeDatabasePortal* portal = currentDatabase_->getPortal();

		if(portal->query(sql))
		{
			while(portal->fetchRow())
			{
				objVec.push_back(portal->getData(1));
			}

			sort(objVec.begin(), objVec.end());
		}

		delete portal;
	}

	return objVec;
}

string TeMercado::getIdFromLabel(const string& tableName, const string& labelName)
{
	string id = "";

	if(currentDatabase_->tableExist(tableName))
	{
		string sql = "SELECT obj_id ";
		sql+= " FROM ";
		sql+= tableName;
		sql+= " WHERE label = '";
		sql+= labelName;
		sql+= "'";

		TeDatabasePortal* portal = currentDatabase_->getPortal();

		if(portal->query(sql) && portal->fetchRow())
		{
			id = portal->getData(0);
		}

		delete portal;
	}

	return id;
}

string TeMercado::getLabelFromId(const string& tableName, const string& id)
{
	string label = "";

	if(currentDatabase_->tableExist(tableName))
	{
		string sql = "SELECT label ";
		sql+= " FROM ";
		sql+= tableName;
		sql+= " WHERE obj_id = '";
		sql+= id;
		sql+= "'";

		TeDatabasePortal* portal = currentDatabase_->getPortal();

		if(portal->query(sql) && portal->fetchRow())
		{
			label = portal->getData(0);
		}

		delete portal;
	}

	return label;
}

bool TeMercado::createMercadoTable(const string& extTableName, const string& originColumn, const string& destinyColumn, const string& linkTable, const string& linkColumn, TeTable& mercTable)
{
	map<string, map<string, int> > mercadoMap = getMercadoMap(extTableName, originColumn, destinyColumn);

	if(mercadoMap.size() == 0)
		return false;

	vector<string> allObj = getDistinctObjects(linkTable, linkColumn);

	if(allObj.size() == 0)
		return false;

		
	TeAttributeList attList;

	TeAttribute attLink;
	attLink.rep_.name_ = originColumn;
	attLink.rep_.type_ = TeSTRING;
	attLink.rep_.numChar_ = 50;
	attLink.rep_.isAutoNumber_ = false;
	attLink.rep_.isPrimaryKey_ = true;
	attList.push_back(attLink);


	MercadoMap::iterator itMercMap = mercadoMap.begin();

	while(itMercMap != mercadoMap.end())
	{
		TeAttribute att;
		att.rep_.name_ = "obj_" + itMercMap->first ;
		att.rep_.type_ = TeINT;
		att.rep_.isAutoNumber_ = false;
		att.rep_.isPrimaryKey_ = false;
		attList.push_back(att);

		++itMercMap;
	}

	mercTable.setAttributeList(attList);
	mercTable.setLinkName(originColumn);
	mercTable.setUniqueName(originColumn);
	mercTable.setTableType(TeAttrExternal);

	if (TeProgress::instance())
	{
		TeProgress::instance()->setCaption("Mercado");
		TeProgress::instance()->setMessage("Generating Table. Please, wait!");
		TeProgress::instance()->setTotalSteps(allObj.size());
	}

	int count = 0;
	
	for(unsigned int i = 0; i < allObj.size(); i++)
	{
		TeTableRow row;
		row.push_back(allObj[i]);
		
		itMercMap = mercadoMap.begin();

		while(itMercMap != mercadoMap.end())
		{
			OriginMap::iterator itOrigin = (*itMercMap).second.find(allObj[i]);
			if(itOrigin != (*itMercMap).second.end())
			{
				row.push_back(Te2String(itOrigin->second));
			}
			else
			{
				row.push_back("0");
			}
			++itMercMap;
		}

		mercTable.add(row);

		TeProgress::instance()->setProgress(count);

		count ++;
	}

	if (TeProgress::instance())
		TeProgress::instance()->reset();

    return true;
}

bool TeMercado::createMercadoTable(const string& fileTableName, const string& originColumn, const string& linkTable, const string& linkColumn, TeTable& mercTable)
{
	map<string, map<string, int> > mercadoMap = getMercadoMap(fileTableName);

	if(mercadoMap.size() == 0)
		return false;

	vector<string> allObj = getDistinctObjects(linkTable, linkColumn);

	if(allObj.size() == 0)
		return false;

	TeAttributeList attList;

	TeAttribute attLink;
	attLink.rep_.name_ = originColumn;
	attLink.rep_.type_ = TeSTRING;
	attLink.rep_.numChar_ = 50;
	attLink.rep_.isAutoNumber_ = false;
	attLink.rep_.isPrimaryKey_ = true;
	attList.push_back(attLink);


	MercadoMap::iterator itMercMap = mercadoMap.begin();

	while(itMercMap != mercadoMap.end())
	{
		TeAttribute att;
		att.rep_.name_ = "obj_" + itMercMap->first ;
		att.rep_.type_ = TeINT;
		att.rep_.isAutoNumber_ = false;
		att.rep_.isPrimaryKey_ = false;
		attList.push_back(att);

		++itMercMap;
	}

	mercTable.setAttributeList(attList);
	mercTable.setLinkName(originColumn);
	mercTable.setUniqueName(originColumn);
	mercTable.setTableType(TeAttrExternal);

	if (TeProgress::instance())
	{
		TeProgress::instance()->setCaption("Mercado");
		TeProgress::instance()->setMessage("Generating Table. Please, wait!");
		TeProgress::instance()->setTotalSteps(allObj.size());
	}

	int count = 0;

	for(unsigned int i = 0; i < allObj.size(); i++)
	{
		TeTableRow row;
		row.push_back(allObj[i]);

		itMercMap = mercadoMap.begin();

		while(itMercMap != mercadoMap.end())
		{
			OriginMap::iterator itOrigin = (*itMercMap).second.find(allObj[i]);
			if(itOrigin != (*itMercMap).second.end())
			{
				row.push_back(Te2String(itOrigin->second));
			}
			else
			{
				row.push_back("0");
			}
			++itMercMap;
		}

		mercTable.add(row);

		TeProgress::instance()->setProgress(count);

		count ++;
	}

	if (TeProgress::instance())
		TeProgress::instance()->reset();

	return true;
}

bool TeMercado::createMercadoLabelTable(const string& extTableName, const string& originColumn, const string& labelColumn, TeTable& labelTable)
{
	string sql = "SELECT DISTINCT ";
		   sql+= originColumn;
		   sql+= ", ";
		   sql+= labelColumn;
		   sql+= " FROM ";
		   sql+= extTableName;

   TeDatabasePortal* portal = currentDatabase_->getPortal();

   if(!portal->query(sql))
	{
		delete portal;
		return false;
	}

	TeAttributeList attList;

	TeAttribute attObjId;
	attObjId.rep_.name_ = "obj_Id";
	attObjId.rep_.type_ = TeSTRING;
	attObjId.rep_.numChar_ = 255;
	attObjId.rep_.isAutoNumber_ = false;
	attObjId.rep_.isPrimaryKey_ = true;
	attList.push_back(attObjId);

	TeAttribute attLabel;
	attLabel.rep_.name_ = "label";
	attLabel.rep_.type_ = TeSTRING;
	attLabel.rep_.numChar_ = 255;
	attLabel.rep_.isAutoNumber_ = false;
	attLabel.rep_.isPrimaryKey_ = false;
	attList.push_back(attLabel);


	labelTable.setAttributeList(attList);
	labelTable.setTableType(TeAttrExternal);


	while(portal->fetchRow())
	{
		string origin = portal->getData(0);
		string label = portal->getData(1);

		TeTableRow row;
		row.push_back(origin);
		row.push_back(label);

		labelTable.add(row);
	}

	delete portal;

	return true;
}

bool TeMercado::createMercadoLabelTable(const string& fileTableName, const string& layerName, const string& layerTableName, const string& linkColumn, const string& labelColumn, TeTable& labelTable)
{
	TeAttributeList attList;

	TeAttribute attObjId;
	attObjId.rep_.name_ = "obj_Id";
	attObjId.rep_.type_ = TeSTRING;
	attObjId.rep_.numChar_ = 255;
	attObjId.rep_.isAutoNumber_ = false;
	attObjId.rep_.isPrimaryKey_ = true;
	attList.push_back(attObjId);

	TeAttribute attLabel;
	attLabel.rep_.name_ = "label";
	attLabel.rep_.type_ = TeSTRING;
	attLabel.rep_.numChar_ = 255;
	attLabel.rep_.isAutoNumber_ = false;
	attLabel.rep_.isPrimaryKey_ = false;
	attList.push_back(attLabel);


	labelTable.setAttributeList(attList);
	labelTable.setTableType(TeAttrExternal);

	MercadoMap map = this->getMercadoMap(fileTableName);
	
	MercadoMap::iterator it = map.begin();

	while(it!= map.end())
	{
		std::string value = it->first;

		string sql = "SELECT ";
		sql+= linkColumn;
		sql+= ", ";
		sql+= labelColumn;
		sql+= " FROM ";
		sql+= layerTableName;
		sql+= " WHERE ";
		sql+= linkColumn;
		sql+= " = '";
		sql+= value;
		sql+= "'";

		TeDatabasePortal* portal = currentDatabase_->getPortal();

		if(!portal->query(sql))
		{
			delete portal;
		}

		while(portal->fetchRow())
		{
			string origin = portal->getData(0);
			string label = portal->getData(1);

			TeTableRow row;
			row.push_back(origin);
			row.push_back(label);

			labelTable.add(row);
		}

		delete portal;

		++it;
	}
	

	return true;
}

bool TeMercado::createPointRepresentation(TeLayer* layer)
{
	if(!layer->hasGeometry(TePOLYGONS))
		return false;

	TePolygonSet polSet;
	
	if(!layer->getPolygons(polSet))
		return false;

	TePolygonSet::iterator itPol = polSet.begin();

	TePointSet pointSet;
	while(itPol != polSet.end())
	{
		TePoint point( TeFindCentroid ( ( *itPol ) ) );

		point.objectId(itPol->objectId());
		
		pointSet.add(point);

		++itPol;
	}

	if(!layer->addPoints(pointSet))
		return false;

	return true;
}

TeRaster* TeMercado::createMercadoRaster(vector<TeRaster*> vecRas, vector<string> vecObj, vector<TeColor> vecColor, int levelMin, int levelMax)
{
	if(vecRas.size() == 0)
		return 0;

	double levMin = (double)levelMin / 100.;
	double levMax = (double)levelMax / 100.;

	TeRasterParams param = vecRas[0]->params();
	param.mode_ = 'c';
	param.setDummy(0);
	param.setDataType(TeUNSIGNEDCHAR);
	param.setPhotometric(TeRasterParams::TePallete);
	param.status_ = TeRasterParams::TeReadyToWrite;

	createRasterDomLut(param, vecColor);

	TeRaster* raster = new TeRaster(param);

	if(!raster->init())
		return 0;

	if (TeProgress::instance())
	{
		TeProgress::instance()->setCaption("Mercado");
		TeProgress::instance()->setMessage("Generating Raster. Please, wait!");
		TeProgress::instance()->setTotalSteps(raster->params().nlines_);
	}

	int count = 0;

	for (int lin = 0; lin < raster->params().nlines_; lin++)
	{
		for (int col = 0; col < raster->params().ncols_; col++)
		{
			vector<double> vecValues;
			double val;
			double total = 0.0;
			
			for (unsigned int vecPos = 0; vecPos < vecRas.size(); vecPos++)
			{
				vecRas[vecPos]->getElement(col, lin, val);
				vecValues.push_back(val);
				total += val;
			}
			
			if (total == 0)
			{
				if(!raster->setElement(col, lin, 0))
					return 0;
				continue;
			}

			//generate individual kernel maps -> each raster / total

			vector<double>::iterator itValues = vecValues.begin();
			int count = 0;
			bool check = false;
			double secundary = 0.;
			while (itValues != vecValues.end())
			{
				double result = *itValues / total;

				//evitar a interseccao entre os mapas de mercado
				if(result > levMax)
				{
					check = false;
					break;
				}

				if (result > levMin && result <=levMax && result > secundary)
				{
					secundary = result;

					raster->setElement(col, lin, count + 1);
				
					check = true;
				}

				if(result == levMax && levMax == 1)
				{
					secundary = result;

					raster->setElement(col, lin, count + 1);
				
					check = true;
				}

				++itValues;
				++count;
			}
			
			if(!check)
				if(!raster->setElement(col, lin, 0))
					return 0;
		}

		TeProgress::instance()->setProgress(count);

		count ++;
	}

	if (TeProgress::instance())
		TeProgress::instance()->reset();

	return raster;
}

std::vector<TeRaster*> TeMercado::createMercadoRaster(std::vector<TeRaster*> vecRasIn)
{
	std::vector<TeRaster*> vecRasterOut;

	if(vecRasIn.empty())
	{
		return vecRasterOut;
	}
	

	TeRasterParams params = vecRasIn[0]->params();
	params.mode_ = 'c';
	params.setDummy(0);
	params.setDataType(TeDOUBLE);
	
	int size = vecRasIn.size();

	for(int i = 0; i < size; ++i)
	{
		TeRaster* raster = new TeRaster(params);

		if(!raster->init())
		{
			return vecRasterOut;
		}
		
		vecRasterOut.push_back(raster);
	}

	
	if (TeProgress::instance())
	{
		TeProgress::instance()->setCaption("Mercado");
		TeProgress::instance()->setMessage("Generating Mercado Raster. Please, wait!");
		TeProgress::instance()->setTotalSteps(params.nlines_);
	}

	for (int lin = 0; lin < params.nlines_; lin++)
	{
		for (int col = 0; col < params.ncols_; col++)
		{
			vector<double> vecValues;
			double val;
			double total = 0.0;
			
			for (unsigned int vecPos = 0; vecPos < vecRasIn.size(); vecPos++)
			{
				vecRasIn[vecPos]->getElement(col, lin, val);
				vecValues.push_back(val);
				total += val;
			}

			for (unsigned int vecPos = 0; vecPos < vecValues.size(); vecPos++)
			{
				double value = 0.;
				
				if (total != 0)
				{
					value = vecValues[vecPos] / total;
				}
				
				vecRasterOut[vecPos]->setElement(col, lin, value);
				
			}
		}
			
		TeProgress::instance()->setProgress(lin);
	}

	if (TeProgress::instance())
		TeProgress::instance()->reset();


	return vecRasterOut;
}

bool TeMercado::createMercadoRelation(TeRaster* raster, TeLayer* layer, std::string columnName, vector<string> vecObj, vector<TeColor> vecColor)
{
//add a new column in attribute table for information of mercado relation.
	std::string tableName = layer->attrTables()[0].name();
	std::string colNameDom = columnName + "_Dom";
	std::string colNameRelation = columnName + "_Value";

	TeAttributeRep attr1;
	attr1.name_ = colNameDom;
	attr1.isPrimaryKey_ = false;
	attr1.numChar_ = 255;
	attr1.isAutoNumber_ = false;
	attr1.type_ = TeSTRING;

	TeAttributeRep attr2;
	attr2.name_ = colNameRelation;
	attr2.isPrimaryKey_ = false;
	attr2.isAutoNumber_ = false;
	attr2.type_ = TeREAL;

	if(!layer->database()->addColumn(tableName, attr1))
	{
		return false;
	}

	if(!layer->database()->addColumn(tableName, attr2))
	{
		return false;
	}

//parcour each polygon to get class relation
	TePolygonSet ps;
	layer->getPolygons(ps);

	if(TeProgress::instance())
	{
		TeProgress::instance()->setCaption("Mercado");
		TeProgress::instance()->setMessage("Generating Mercado Relation");
		TeProgress::instance()->setTotalSteps(ps.size());
	}

	std::map< std::string, std::map<int, int> > polClass;

	for (unsigned int p = 0; p < ps.size(); ++p)
	{
		std::string objId = ps[p].objectId();

		std::map< std::string, std::map<int, int> >::iterator itPol = polClass.find(objId);

		if(itPol == polClass.end())
		{
			std::map<int, int> classRelation;
			polClass.insert(std::map< std::string, std::map<int, int> >::value_type(objId, classRelation));
			itPol = polClass.find(objId);

			if(itPol == polClass.end())
			{
				return false;
			}
		}

		TeRaster::iteratorPoly itParcour = raster->begin(ps[p],TeBoxPixelIn);
		while (!itParcour.end())
		{
			double val;
			if(raster->getElement(itParcour.currentColumn(),itParcour.currentLine(),val,0))
			{
				std::map<int, int>::iterator it = itPol->second.find((int)val);

				if(it == itPol->second.end())
				{
					itPol->second.insert(std::map<int, int>::value_type((int)val, 1));
				}
				else
				{
					++it->second;
				}
			}
			
			++itParcour;
		}
		
		if(TeProgress::instance())
		{
			if(TeProgress::instance()->wasCancelled())
			{
				TeProgress::instance()->reset();
				break;
			}
			TeProgress::instance()->setProgress(p);
		}

	}
	if(TeProgress::instance())
		TeProgress::instance()->reset();

//parcour the map if dominance information to set in database table
	std::map< std::string, std::map<int, int> >::iterator it = polClass.begin();

	while(it != polClass.end())
	{
		double relation = 0.0;
		
		int value = getClassDom(it->second, relation);

		if(value != -1)
		{
			TeColor color(raster->params().lutr_[value], raster->params().lutg_[value], raster->params().lutb_[value]);

			std::string className = getClassInfo(color, vecObj, vecColor);

			addClassInfo(tableName, columnName, className, relation, it->first, layer->attrTables()[0].linkName());
		}

		++it;
	}

	return true;
}


map<string, map<string, int> > TeMercado::getMercadoMap(const string& tableName, const string& originColumn, const string& destinyColumn)
{
	map<string, map<string, int> > mercadoMap;

	string sql = "SELECT ";
		   sql+= originColumn;
		   sql+= ", ";
		   sql+= destinyColumn;
		   sql+= " FROM ";
		   sql+= tableName;

   TeDatabasePortal* portal = currentDatabase_->getPortal();

   if(!portal->query(sql))
	{
		delete portal;
	}

	MercadoMap::iterator it;
	OriginMap::iterator itInter;

	if (TeProgress::instance())
	{
		TeProgress::instance()->setCaption("Mercado");
		TeProgress::instance()->setMessage("Generating Map. Please, wait!");
		TeProgress::instance()->setTotalSteps(portal->numRows());
	}

	int count = 0;

	while(portal->fetchRow())
	{
		string origin = portal->getData(0);
		string destiny = portal->getData(1);

		it = mercadoMap.find(destiny);

		if(it == mercadoMap.end())
		{
			//destino ainda nao se encontra no MAP
			OriginMap auxMap;
			auxMap.insert(OriginMap::value_type(origin, 1));
			mercadoMap.insert(MercadoMap::value_type(destiny, auxMap));

		}
		else
		{
			//destino ja esta no MAP
			itInter	 = (*it).second.find(origin);

			if(itInter == (*it).second.end())
			{
				//origem ainda nao se encontra no MAP
				(*it).second.insert(OriginMap::value_type(origin, 1));
			}
			else
			{
				//origem ja esta no MAP
				++(*itInter).second;
			}
		}

		TeProgress::instance()->setProgress(count);

		count ++;
	}

	if (TeProgress::instance())
		TeProgress::instance()->reset();

	return mercadoMap;
}

map<string, map<string, int> > TeMercado::getMercadoMap(const string& fileTableName)
{
  map<string, map<string, int> > mercadoMap;

  //open file
  TeAsciiFile* csvFile = new TeAsciiFile(fileTableName);

  //read the number of entries
  unsigned int totalSteps = 0;
  while(csvFile->isNotAtEOF())
  {
    csvFile->findNewLine();
    ++totalSteps;
  }
  csvFile->rewind();

  //get header info
  std::vector<std::string> rowHeader;

  if(csvFile->isNotAtEOF())
  {
    csvFile->readStringListCSV(rowHeader,';');
    csvFile->findNewLine();
  }

  for(unsigned int i = 1; i < rowHeader.size(); ++i)
  {
    std::string value = rowHeader[i];

    std::string newValue = value;

    std::vector<std::string> vecOut;

    TeSplitString(value, "_", vecOut);

    if(!vecOut.empty() && (TeConvertToUpperCase(vecOut[0]) != TeConvertToUpperCase("Total")))
	{
      newValue = vecOut[0];
    }
    
    rowHeader[i] = newValue;
  }

  //get body info

  bool endOfFile = false;
  while(!endOfFile)
  {
    vector<string> row;

    //read line
    csvFile->readStringListCSV(row, ';');

    //fix obj name
    if(!row.empty())
    {
      std::string value = row[0];

      std::string newValue = value;

      std::vector<std::string> vecOut;

      TeSplitString(value, " ", vecOut);

      if(!vecOut.empty())
      {
        newValue = vecOut[0];
      }

      row[0] = newValue;

	  std::string origin = row[0];

	  MercadoMap::iterator it;
	  OriginMap::iterator itInter;

	  if(TeConvertToUpperCase(origin) != TeConvertToUpperCase("Total"))
	  {
		  for(unsigned int i = 1; i < row.size() - 1; ++i)
		  {
			  int value = atoi(row[i].c_str());

			  std::string destiny = rowHeader[i];

				OriginMap auxMap;
				auxMap.insert(OriginMap::value_type(origin, value));

				it = mercadoMap.find(destiny);
				
				if(it == mercadoMap.end())
				{
					//destino ainda nao se encontra no MAP
					OriginMap auxMap;
					auxMap.insert(OriginMap::value_type(origin, value));
					mercadoMap.insert(MercadoMap::value_type(destiny, auxMap));

				}
				else
				{
					//destino ja esta no MAP
					itInter	 = (*it).second.find(origin);

					if(itInter == (*it).second.end())
					{
						//origem ainda nao se encontra no MAP
						(*it).second.insert(OriginMap::value_type(origin, value));
					}
				}

		  }
	  }
    }
       
    csvFile->findNewLine();

	endOfFile = !csvFile->isNotAtEOF();
  }


  return mercadoMap;
}

void TeMercado::createRasterDomLut(TeRasterParams& params, vector<TeColor> colorVec)
{
	if(colorVec.size() == 0)
		return;

	if(params.lutr_.size() != 0 )
		return ;

	params.lutr_.push_back(255);
	params.lutg_.push_back(255);
	params.lutb_.push_back(255);
	params.lutClassName_.push_back("");

	for (unsigned int i = 0; i < colorVec.size(); i++)
	{
		params.lutr_.push_back(colorVec[i].red_);
		params.lutg_.push_back(colorVec[i].green_);
		params.lutb_.push_back(colorVec[i].blue_);
		params.lutClassName_.push_back("");
	}

	return;
}

int TeMercado::getClassDom(std::map<int, int> classMap, double& relation)
{
	int index = -1;
	double total = 0.0;

	std::map<int, int>::iterator it = classMap.begin();

	while(it != classMap.end())
	{
		total+= it->second;

		++it;
	}

	it = classMap.begin();

	while(it != classMap.end())
	{
		double value = it->second;

		double res = value / total;

		if(res >= 0.5)
		{
			relation = res;
			index = it->first;
			break;		
		}

		++it;
	}

	return index;
}

std::string TeMercado::getClassInfo(TeColor color, vector<string> vecObj, vector<TeColor> vecColor)
{
	std::string className = "";

	if(vecObj.size() != vecColor.size())
	{
		return className;
	}

	for(unsigned int i = 0; i < vecObj.size(); ++i)
	{
		if(color.red_ == vecColor[i].red_ && color.green_ == vecColor[i].green_ && color.blue_ == vecColor[i].blue_)
		{
			className = vecObj[i];
			break;
		}
	}

	return className;
}

bool TeMercado::addClassInfo(std::string tableName, std::string colName, std::string className, double relation, std::string objId, std::string linkColumn)
{
	std::string colNameDom = colName + "_Dom";
	std::string colNameRelation = colName + "_Value";

	std::string insert  = "UPDATE " + tableName;
				insert += " SET " + colNameDom + "  = '" + className+ "'"; 
				insert += ", " + colNameRelation + " = " + Te2String(relation, 2);
				insert += " WHERE " + linkColumn + " = '" + objId + "'";

	if(!currentDatabase_->execute(insert))
	{
		return false;
	}

	return true;
}

TeRaster* TeMercado::mercadoRasterClipping(TeRaster* whole, TePolygonSet& mask, TeProjection* geomProj, const string& clipName, double background, const string& decId)
{
  if( decId.empty() ) {
    throw TeException( UNKNOWN_DECODER_TECHNOLOGY_TYPE, "Invalid decoder", 
      false );
  }
  
  if( whole == 0 ) {
    throw TeException( UNKNOWN_ERROR_TYPE, "Invalid raster pointer", 
      false );
  }  

  if( mask.empty() ) {
    throw TeException( UNKNOWN_ERROR_TYPE, "Invalid polygon set mask", 
      false );
  }  
  
  if( ( decId != "MEM" ) && clipName.empty() ) {
    throw TeException( UNKNOWN_ERROR_TYPE, "Invalid clipname", 
      false );
  }    
    
  if( geomProj == 0 ) {
    throw TeException( UNKNOWN_ERROR_TYPE, "Invalid projection pointer", 
      false );
  }    

	if (whole->params().status_ != TeRasterParams::TeReadyToRead && 
		whole->params().status_ != TeRasterParams::TeReadyToWrite )
		return 0;
	
	bool isEqualProj = (*whole->projection() == *geomProj);
	if (!isEqualProj &&
		(geomProj->name() == "NoProjection" || whole->projection()->name() == "NoProjection"))
		return 0;

	TePolygonSet mask2;
	if (!isEqualProj)
		TeVectorRemap(mask,geomProj,mask2,whole->projection());
	else
		mask2 = mask;

	if (!TeIntersects(mask2.box(),whole->params().boundingBox()))
		return 0;

	TeRasterParams par = whole->params();
	if (!decId.empty())
	{
		par.decoderIdentifier_ = decId;
		if (decId == "DB")
		{
			if (!par.database_)
				return 0;
		}
	}
	par.fileName_ = clipName;
	par.mode_ = 'c';
	TeCoord2D bll = whole->coord2Index(mask2.box().lowerLeft());
	TeCoord2D bur = whole->coord2Index(mask2.box().upperRight());
	
	bll = whole->index2Coord(TeCoord2D(TeRoundRasterIndex(bll.x_),TeRoundRasterIndex(bll.y_)));
	bur = whole->index2Coord(TeCoord2D(TeRoundRasterIndex(bur.x_),TeRoundRasterIndex(bur.y_)));

	par.boxResolution(bll.x_, bll.y_, bur.x_, bur.y_,par.resx_, par.resy_);
	par.setDummy(background);
	TeRaster*  clip = new TeRaster(par);
	clip->init();
	if (clip->params().status_ != TeRasterParams::TeReadyToWrite)
		return 0;
	
	TeCoord2D cd = TeCoord2D(0,0);
	TeCoord2D ul = clip->params().index2Coord(cd);
	TeCoord2D dxdy = whole->params().coord2Index(ul);
	int j = TeRoundRasterIndex(dxdy.x_); // column delta
	int i = TeRoundRasterIndex(dxdy.y_); // line delta
	unsigned int np = mask2.size();
	unsigned int nb = whole->params().nBands();
	unsigned int band;
	double val;

	for (unsigned int p=0; p<np; ++p)
	{
		TeRaster::iteratorPoly it = whole->begin(mask2[p],TeBoxPixelIn);
		while (!it.end())
		{
			for(band = 0; band < nb; ++band)
			{
				if (whole->getElement(it.currentColumn(),it.currentLine(),val,band))
					clip->setElement(it.currentColumn()-j,it.currentLine()-i,val,band);
			}
			++it;
		}
	}


	return clip;
}

bool TeMercado::createMercadoVetorial(	TeTheme* themeIn, const std::string& mercadoTable, const std::string& outputThemeName, TeTheme*& theme,
							std::vector<std::string>& vecObj, std::vector<std::string>& vecObjLabel, std::vector<TeColor>& vecColor, 
							const int& levelMin, const int& levelMax)
{

	TeLayer* vecLayer = themeIn->layer();

	double levMin = (double)levelMin / 100.;
	double levMax = (double)levelMax / 100.;

	if(!vecLayer || !vecLayer->hasGeometry(TePOLYGONS))
	{
		return false;
	}

	if(mercadoTable.empty() || !currentDatabase_->tableExist(mercadoTable))
	{
		return false;
	}

	if(outputThemeName.empty() || currentDatabase_->themeExist(outputThemeName))
	{
		return false;
	}

	if(vecObj.empty() || vecColor.empty() || vecObj.size() != vecColor.size())
	{
		return false;
	}

	if(levelMax <= levelMin)
	{
		return false;
	}

	theme = createMercadoVetorialTheme(vecLayer, outputThemeName, themeIn->view(), mercadoTable);

	if(!theme)
	{
		return false;
	}

	vecLayer->loadLayerTables();

	TeAttrTableVector layerTablesVector;

	vecLayer->getAttrTables(layerTablesVector);

	std::string linkColumnName = "";

	for(unsigned int i = 0; i < layerTablesVector.size(); i++)
	{
		TeTable t = layerTablesVector[i];

		if(t.name() == mercadoTable)
		{
			linkColumnName = t.linkName();
			break;
		}		
	}

	if(linkColumnName.empty())
	{
		return false;
	}


//percorre todas as geometrias do layer para descobrir o mercado de cada uma
	std::string sql = "SELECT object_id from " + vecLayer->tableName(TePOLYGONS);

	std::string count = "SELECT count(*) from " + vecLayer->tableName(TePOLYGONS);

	TeDatabasePortal* portal = currentDatabase_->getPortal();

	if (!portal)
		return false;

	if(!portal->query(count) && !portal->fetchRow())
	{
		delete portal;
		return false;
	}

	int countValue = portal->getInt(0);

	portal->freeResult();

	if(!portal->query(sql))
	{
		delete portal;
		return false;
	}

	if (TeProgress::instance())
	{
		TeProgress::instance()->setCaption("Mercado");
		TeProgress::instance()->setMessage("Generating Mercado Vectorial. Please, wait!");
		TeProgress::instance()->setTotalSteps(countValue);
	}

	int countStep = 0;

	while (portal->fetchRow())
	{
		std::string objId = portal->getData(0);

//descobre de qual objeto essa geometria eh dominada
		
		std::string sqlMerc = "SELECT ";

		for(unsigned int i = 0; i < vecObj.size(); ++i)
		{
			if( i != 0)
			{
				sqlMerc += ", ";
			}

			sqlMerc += vecObj[i];
		}

		sqlMerc += " FROM " + mercadoTable + " WHERE " + linkColumnName  + " = '" + objId + "'";

		TeDatabasePortal* portalMerc = currentDatabase_->getPortal();

		if (!portalMerc)
			return false;

		if(!portalMerc->query(sqlMerc))
		{
			delete portalMerc;
			return false;
		}

		if(portalMerc->fetchRow())
		{
			double total = 0.;

			std::vector<double> values;

			for(unsigned int i = 0; i < vecObj.size(); ++i)
			{
				double num = portalMerc->getDouble(i);
				values.push_back(num);

				total += num;
			}

			if(!values.empty() && total != 0.)
			{
				for(unsigned int i = 0; i < values.size(); ++i)
				{
					if(values[i] != 0.)
					{
						double val = values[i] / total;

						//evitar a interseccao entre os mapas de mercado
						if(val > levMax)
						{
							break;
						}

						if(val > levMin && val <= levMax)
						{
							TeColor cor		= vecColor[i];

							createVetorialMercadoObjectVisual(theme, objId, cor);

							break;
						}
					}
				}
			}
		}

		delete portalMerc;

		TeProgress::instance()->setProgress(countStep);

		countStep ++;
	}

	delete portal;

	if (TeProgress::instance())
		TeProgress::instance()->reset();

	createVetorialThemeLegend(theme, vecObjLabel, vecColor);
	
	return true;
}

bool TeMercado::createFullMercadoVetorial(	TeTheme* themeIn, const std::string& mercadoTable, const std::string& outputThemeName, TeTheme*& theme,
								std::vector<std::string>& vecObj, std::vector<std::string>& vecObjLabel, std::vector<TeColor>& vecColor, 
								const int& levelMin, const int& levelMax, std::vector<std::string>& vecAllObj)
{
	TeLayer* vecLayer = themeIn->layer();

	double levMin = (double)levelMin / 100.;
	double levMax = (double)levelMax / 100.;

	if(!vecLayer || !vecLayer->hasGeometry(TePOLYGONS))
	{
		return false;
	}

	if(mercadoTable.empty() || !currentDatabase_->tableExist(mercadoTable))
	{
		return false;
	}

	if(outputThemeName.empty() || currentDatabase_->themeExist(outputThemeName))
	{
		return false;
	}

	if(vecObj.empty() || vecColor.empty() || vecObj.size() != vecColor.size())
	{
		return false;
	}

	if(vecAllObj.empty())
	{
		return false;
	}

	if(levelMax <= levelMin)
	{
		return false;
	}

	theme = createMercadoVetorialTheme(vecLayer, outputThemeName, themeIn->view(), mercadoTable);

	if(!theme)
	{
		return false;
	}

	vecLayer->loadLayerTables();

	TeAttrTableVector layerTablesVector;

	vecLayer->getAttrTables(layerTablesVector);

	std::string linkColumnName = "";

	for(unsigned int i = 0; i < layerTablesVector.size(); i++)
	{
		TeTable t = layerTablesVector[i];

		if(t.name() == mercadoTable)
		{
			linkColumnName = t.linkName();
			break;
		}		
	}

	if(linkColumnName.empty())
	{
		return false;
	}


//percorre todas as geometrias do layer para descobrir o mercado de cada uma
	std::string sql = "SELECT object_id from " + vecLayer->tableName(TePOLYGONS);

	std::string count = "SELECT count(*) from " + vecLayer->tableName(TePOLYGONS);

	TeDatabasePortal* portal = currentDatabase_->getPortal();

	if (!portal)
		return false;

	if(!portal->query(count) && !portal->fetchRow())
	{
		delete portal;
		return false;
	}

	int countValue = portal->getInt(0);

	portal->freeResult();

	if(!portal->query(sql))
	{
		delete portal;
		return false;
	}

	if (TeProgress::instance())
	{
		TeProgress::instance()->setCaption("Mercado");
		TeProgress::instance()->setMessage("Generating Mercado Vectorial. Please, wait!");
		TeProgress::instance()->setTotalSteps(countValue);
	}

	int countStep = 0;

	while (portal->fetchRow())
	{
		std::string objId = portal->getData(0);

//total for this objId
		double total = 0.;

		for(unsigned int i = 0; i < vecAllObj.size(); ++i)
		{
			std::string obj = vecAllObj[i];
		
			std::string sqlMerc = "SELECT " + obj + " FROM " + mercadoTable + " WHERE " + linkColumnName  + " = '" + objId + "'";

			TeDatabasePortal* portalMerc = currentDatabase_->getPortal();
		
			if (!portalMerc)
			{
				delete portal;
				return false;
			}

			if(!portalMerc->query(sqlMerc))
			{
				delete portalMerc;
				delete portal;

				continue;
			}
		
			if(portalMerc->fetchRow())
			{
				double num = portalMerc->getDouble(0);
			
				total = total + num;
			}
		
			delete portalMerc;
		}
	
		//find mercado
		for(unsigned int i = 0; i < vecObj.size(); ++i)
		{
			std::string obj = vecObj[i];
		
			std::string sqlMerc = "SELECT " + obj + " FROM " + mercadoTable + " WHERE " + linkColumnName  + " = '" + objId + "'";

			TeDatabasePortal* portalMerc = currentDatabase_->getPortal();
		
			if (!portalMerc)
			{
				delete portal;
				return false;
			}

			if(!portalMerc->query(sqlMerc))
			{
				delete portalMerc;
				delete portal;

				continue;
			}
		
			if(portalMerc->fetchRow())
			{
				double num = portalMerc->getDouble(0);
			
				double val = num / total;

				//evitar a interseccao entre os mapas de mercado
				if(val > levMax)
				{
					break;
				}

				if(val > levMin && val <= levMax)
				{
					TeColor cor = vecColor[i];

					createVetorialMercadoObjectVisual(theme, objId, cor);

					break;
				}
			}
		
			delete portalMerc;
		}

		TeProgress::instance()->setProgress(countStep);

		countStep ++;
	}

	delete portal;

	if (TeProgress::instance())
		TeProgress::instance()->reset();

	createVetorialThemeLegend(theme, vecObjLabel, vecColor);
	
	return true;
}

TeTheme* TeMercado::createMercadoVetorialTheme(TeLayer* vecLayer, const std::string& outputThemeName, const int& viewId, const std::string& mercadoTable)
{
	TeViewMap& viewMap = currentDatabase_->viewMap();
	TeView* currentView = viewMap[viewId];

	TeTheme* theme = new TeTheme(outputThemeName, vecLayer);

	theme->layer (vecLayer);
	theme->visibleRep(TePOLYGONS);
	theme->visibility(true);

	currentView->add(theme);

	TeAttrTableVector layerTablesVector;

	vecLayer->getAttrTables(layerTablesVector);

	for (unsigned int i = 0; i < layerTablesVector.size(); ++i)
	{
		theme->addThemeTable(layerTablesVector[i]);
	}

	TeAppTheme* themeApp = new TeAppTheme(theme);

	if(!insertAppTheme(currentDatabase_, themeApp))
	{
		delete theme;
		return 0;
	}

	if (!theme->save())
    {
		currentDatabase_->deleteTheme(theme->id());
		return 0;
	}

	if (!theme->buildCollection())
	{
		currentDatabase_->deleteTheme(theme->id());
		return 0;
	}

	if(!theme->generateLabelPositions())
	{
		currentDatabase_->deleteTheme(theme->id());
		return 0;
	}

	TeBox tbox = currentDatabase_->getThemeBox(theme);
	theme->setThemeBox(tbox);

	currentDatabase_->updateTheme(theme);

    std::string themeId = Te2String(theme->id()),
                sql = "UPDATE te_theme SET parent_id = " + themeId + " WHERE theme_id = " + themeId;   

    if(!currentDatabase_->execute(sql))
    {
		currentDatabase_->deleteTheme(theme->id());
		return 0;
    }
	
	return theme;
}

void TeMercado::createVetorialThemeLegend(TeTheme* theme, std::vector<std::string>& vecObj, std::vector<TeColor>& vecColor)
{
//defaul visual;
	TeVisual* visual = new TeVisual(TePOLYGONS);

	visual->color(TeColor(255,255,255));
	visual->transparency(theme->defaultLegend().visual(TePOLYGONS)->transparency());
	visual->contourStyle(theme->defaultLegend().visual(TePOLYGONS)->contourStyle());
	visual->contourWidth(theme->defaultLegend().visual(TePOLYGONS)->contourWidth());
	visual->contourColor(TeColor(0, 0, 0));

	theme->setVisualDefault(visual, TePOLYGONS);

//legendas
	for(unsigned int i = 0; i < vecObj.size(); ++i)
	{
		TeVisual* visual = new TeVisual(TePOLYGONS);            
		visual->color(vecColor[i]);
		visual->transparency(theme->defaultLegend().visual(TePOLYGONS)->transparency());
		visual->contourStyle(theme->defaultLegend().visual(TePOLYGONS)->contourStyle());
		visual->contourWidth(theme->defaultLegend().visual(TePOLYGONS)->contourWidth());
		visual->contourColor(theme->defaultLegend().visual(TePOLYGONS)->contourColor());	

		TeLegendEntry legend;
		legend.label(vecObj[i]);
		legend.setVisual(visual, TePOLYGONS);
		legend.group(i);
		legend.theme(theme->id());
		theme->legend().push_back(legend);
	}

	currentDatabase_->updateTheme(theme);

	return;
}

bool TeMercado::createVetorialMercadoObjectVisual(TeTheme* theme, const std::string& objId, TeColor cor)
{
	std::string TC = theme->collectionTable();

	TeLegendEntry leg = theme->defaultLegend();

	TeVisual* visual = new TeVisual(TePOLYGONS);            
	visual->color(cor);
	visual->transparency(theme->defaultLegend().visual(TePOLYGONS)->transparency());
	visual->contourStyle(theme->defaultLegend().visual(TePOLYGONS)->contourStyle());
	visual->contourWidth(theme->defaultLegend().visual(TePOLYGONS)->contourWidth());
	visual->contourColor(theme->defaultLegend().visual(TePOLYGONS)->contourColor());

	leg.setVisual(visual, TePOLYGONS);
	leg.id(0);
	leg.group(-10);
	
	if(!currentDatabase_->insertLegend(&leg))
	{
		return false;
	}

	theme->getObjOwnLegendMap()[objId] = leg.id();

	std::string up  = "UPDATE " + TC + " SET c_legend_own = " + Te2String(leg.id());
				up += " WHERE c_object_id = '" + objId + "'";
	
	if(!currentDatabase_->execute(up))
	{
		return false;
	}

	return true;
}