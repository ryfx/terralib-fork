#include <FillCellUtils.h>
#include <TeUtils.h>

FillCellUtils::FillCellUtils(TeDatabase* db)
{
	_database = db;
}

FillCellUtils::~FillCellUtils()
{
}

std::vector<std::string> FillCellUtils::getCellLayerList()
{
	std::vector<std::string> layerList;

	if(_database)
	{
		TeLayerMap& layerMap = _database->layerMap();
		TeLayerMap::iterator layerIt;

		for(layerIt = layerMap.begin(); layerIt != layerMap.end(); ++layerIt)
		{
			if ((layerIt->second) && (layerIt->second->geomRep() & TeCELLS))
			{
				layerList.push_back(layerIt->second->name());
			}
		}
	}

	return layerList;
}

std::vector<std::string> FillCellUtils::getThemeList()
{
	std::vector<std::string> themeList;

	if(_database)
	{
		TeThemeMap& themeMap = _database->themeMap();
		TeThemeMap::iterator themeIt;

		for (themeIt = themeMap.begin(); themeIt != themeMap.end(); ++themeIt)
		{
			themeList.push_back(themeIt->second->name());
		}
	}

	return themeList;
}

TeTheme* FillCellUtils::getTheme(const std::string& themeName)
{
	if(themeName.empty())
	{
		return NULL;
	}

	if(_database)
	{
		TeThemeMap& themeMap = _database->themeMap();
		TeThemeMap::iterator themeIt;

		for (themeIt = themeMap.begin(); themeIt != themeMap.end(); ++themeIt)
		{
			if(themeIt->second->name() == themeName)
			{
				TeTheme* t = dynamic_cast<TeTheme*>(themeIt->second);
				
				if(t)
				{
					return t;
				}
			}
		}
	}

	return NULL;
}

TeLayer* FillCellUtils::getLayer(const std::string& layerName)
{
	if(layerName.empty())
	{
		return NULL;
	}

	if(_database)
	{
		TeLayerMap& layerMap = _database->layerMap();
		TeLayerMap::iterator layerIt;

		for (layerIt = layerMap.begin(); layerIt != layerMap.end(); ++layerIt)
		{
			if(layerIt->second->name() == layerName)
			{
				TeLayer* l = layerIt->second;
				
				return l;
			}
		}
	}
	return NULL;
}

std::vector<std::string> FillCellUtils::getThemeAttrTables(const std::string& themeName)
{
	std::vector<std::string> attrVec;

	TeTheme* theme = getTheme(themeName);

	if(theme)
	{
		if(theme->layer())
		{
			attrVec = getLayerAttrTables(theme->layer()->name());
		}
	}

	return attrVec;
}

std::vector<std::string> FillCellUtils::getLayerAttrTables(const std::string& layerName)
{
	std::vector<std::string> attrVec;

	TeLayer* layer = getLayer(layerName);

	if(layer) 
	{	
		for(unsigned int i = 0; i < layer->attrTables().size(); ++i)
		{
			if(layer->attrTables()[i].tableType() != TeAttrMedia)
			{
				attrVec.push_back(layer->attrTables()[i].name());
			}
		}
	}

	return attrVec;
}

TeAttributeList FillCellUtils::getThemeAttrList(const std::string& themeName, const std::string& tableName)
{
	TeAttributeList attrList;

	TeTheme* theme = getTheme(themeName);

	if(theme)
	{
		if(theme->layer())
		{
			attrList = getLayerAttrList(theme->layer()->name(), tableName);
		}
	}

	return attrList;
}

TeAttributeList FillCellUtils::getLayerAttrList(const std::string& layerName, const std::string& tableName)
{
	TeAttributeList attrList;

	TeLayer* layer = getLayer(layerName);

	if(layer)
	{
		TeTable table;

		if(layer->getAttrTablesByName(tableName, table))
		{
			attrList = table.attributeList();
		}
	}

	return attrList;
}

bool FillCellUtils::tableIsTemporal(const std::string& layerName, const std::string& tableName)
{
	TeLayer* layer = getLayer(layerName);

	if(layer)
	{
		for(unsigned int i = 0; i < layer->attrTables().size(); ++i)
		{
			if(layer->attrTables()[i].name() == tableName)
			{
				if(layer->attrTables()[i].tableType() == TeFixedGeomDynAttr)
				{
					return true;
				}
			}
		}
	}
	
	return false;
}

std::vector<std::string> FillCellUtils::getTableTimes(const std::string& layerName, const std::string& tableName)
{
	std::vector<std::string> vecTimes;

	if(tableIsTemporal(layerName, tableName))
	{
		TeLayer* layer = getLayer(layerName);

		for(unsigned int i = 0; i < layer->attrTables().size(); ++i)
		{
			if(layer->attrTables()[i].name() == tableName)
			{
				std::string tInit  = layer->attrTables()[i].attInitialTime();
				std::string tFinal = layer->attrTables()[i].attFinalTime();

				std::string sql = " SELECT DISTINCT "+ tInit +", "+ tFinal + " FROM "+  layer->attrTables()[i].name();

				TeDatabasePortal* portal = _database->getPortal();

				if(portal->query(sql))
				{
					while(portal->fetchRow())
					{		
						std::string strTimeInit = portal->getDate(0).getDateTime();
						std::string strTimeFinal =  portal->getDate(1).getDateTime(); 

						std::string strTime = strTimeInit + "/" + strTimeFinal;
						
						vecTimes.push_back(strTime);
					}				
				}

				portal->freeResult();
				delete portal;
			}
		}
	}

	return vecTimes;
}

TeTimeInterval FillCellUtils::getTimeInterval(const std::string& layerName, const std::string& tableName, const std::string& strTimeInterval)
{
	TeTimeInterval t;

	TeLayer* layer = getLayer(layerName);

	for(unsigned int i = 0; i < layer->attrTables().size(); ++i)
	{
		if(layer->attrTables()[i].name() == tableName)
		{
			std::string tInit  = layer->attrTables()[i].attInitialTime();
			std::string tFinal = layer->attrTables()[i].attFinalTime();

			std::string sql = " SELECT DISTINCT "+ tInit +", "+ tFinal + " FROM "+  layer->attrTables()[i].name();

			TeDatabasePortal* portal = _database->getPortal();

			if(portal->query(sql))
			{
				while(portal->fetchRow())
				{		
					std::string strTimeInit = portal->getDate(0).getDateTime();
					std::string strTimeFinal =  portal->getDate(1).getDateTime(); 

					std::string strTime = strTimeInit + "/" + strTimeFinal;
					
					if(strTime == strTimeInterval)
					{
						TeTime t1, t2;
						t1 = portal->getDate(0);
						t2 = portal->getDate(1);

						portal->freeResult();
						delete portal;
						
						TeTimeInterval tVal(t1, t2);
						
						return tVal;
					}
				}				
			}

			portal->freeResult();
			delete portal;
		}
	}


	return t;
}

TeAttrDataType FillCellUtils::getAttrDataType(const std::string& layerName, const std::string& tableName, const std::string& attrName)
{
	TeAttributeList attrList = getLayerAttrList(layerName, tableName);

	TeAttributeList::iterator it = attrList.begin();
	
	while (it != attrList.end())
	{
		if(it->rep_.name_ == attrName)
		{
			return it->rep_.type_;
		}
		
		++it;
	}

	return TeSTRING;
}

bool FillCellUtils::checkAttributeName(const std::string& layerName, const std::string& tableName, const std::string& attrName, std::string& errorMessage)
{
	TeAttributeList attrList = getLayerAttrList(layerName, tableName);

	TeAttributeList::iterator it = attrList.begin();
	
	while (it != attrList.end())
	{
		if(it->rep_.name_ == attrName)
		{
			errorMessage = "Attribute already exist.";
			return false;
		}

		++it;
	}

	bool changed = false;
	std::string errorMess = "";

	TeCheckName(attrName, changed, errorMess);

	if(changed)
	{
		errorMessage = errorMess;
		return false;
	}

	return true;
}

bool FillCellUtils::updateTheme(const std::string& layerName)
{
	if(!_database)
	{
		return false;
	}
	
	TeThemeMap& themeMap = _database->themeMap();
	TeThemeMap::iterator themeIt;

	for (themeIt = themeMap.begin(); themeIt != themeMap.end(); ++themeIt)
	{
		TeTheme* t = dynamic_cast<TeTheme*>(themeIt->second);
		
		if(t && t->layer()->name() == layerName)
		{
			if(!t->createCollectionAuxTable())
			{
				return false;
			}

			if(!t->populateCollectionAux())
			{
				return false;
			}
		}
	}

	return true;
}