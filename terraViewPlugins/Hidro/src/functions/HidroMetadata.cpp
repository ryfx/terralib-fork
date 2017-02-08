#include <HidroMetadata.h>

#include <TeDatabase.h>
#include <TeLayer.h>

HidroMetadata::HidroMetadata(TeDatabase* database) : _db(database), _errorMessage("")
{
	_hidroGraphTableName	= "te_hidro_graph";
	_vertexNameAttrName		= "vertex_name";
	_edgeNameAttrName		= "edge_name";
	_edgeVertexFromAttrName	= "edge_attr_vertex_from";
	_edgeVertexToAttrName	= "edge_attr_vertex_to";
	_objectId				= "object_id_";
}

HidroMetadata::~HidroMetadata()
{
}

std::string HidroMetadata::getErrorMessage() const
{
	return _errorMessage;
}

bool HidroMetadata::addGraphEntry(const std::string& graphName, const int& layerPointsId, const int& layerLinesId, const double& res)
{
	_errorMessage = "";

//verify if the graph name is empty
	if(graphName.empty())
	{
		_errorMessage = "Graph name is empty.";
		return false;
	}
   
//create hidro graph table if not exist yet
	if(!createGraphMetadataTable())
	{
		return false;
	}

//create attribute tables for vertex and edge
	std::string tableVertexName	= graphName + "_VertexAttrTable";
	if(!createAttrMetadataTable(tableVertexName))
	{
		return false;
	}

	std::string tableEdgeName = graphName + "_EdgeAttrTable";
	if(!createAttrMetadataTable(tableEdgeName))
	{
		return false;
	}

//create new table entry sql
	std::string sql = "INSERT INTO ";
				sql+= _hidroGraphTableName;
				sql+= " (graph_name, layer_points_id, layer_lines_id, vertex_attr_table, edge_attr_table, graph_res)";
				sql+= " VALUES ('";
				sql+= graphName;
				sql+= "', ";
				sql+= Te2String(layerPointsId);
				sql+= ", ";
				sql+= Te2String(layerLinesId);
				sql+= ", '" + tableVertexName + "', ";
				sql+= "'" + tableEdgeName + "'";
				sql+= ", " + Te2String(res);
				sql+= ")";

//save entry in database table
	if(!_db->execute(sql))
	{
		_errorMessage = _db->errorMessage();
		return false;
	}

	return true;
}

bool HidroMetadata::removeGraphEntry(const std::string& graphName)
{
	_errorMessage = "";

//verify if the graph name is empty
	if(graphName.empty())
	{
		_errorMessage = "Graph name is empty.";
		return false;
	}

//create remove table sql
	std::string sql = "DELETE FROM ";
				sql+= _hidroGraphTableName;
				sql+= " WHERE ";
				sql+= "graph_name";
				sql+= " = '";
				sql+= graphName;
				sql+= "'";

//remove entry from database table
	if(!_db->execute(sql))
	{
		_errorMessage = _db->errorMessage();
		return false;
	}

	return true;
}

TeLayer* HidroMetadata::getGraphLayerPoints(const std::string& graphName)
{
	_errorMessage = "";

//get database portal
	TeDatabasePortal* portal = _db->getPortal();

	if(!portal)
	{
		_errorMessage = "Database Portal Internal Error.";
		return NULL;
	}

//create sql to get layer id
	std::string sql = "SELECT ";
				sql+= "layer_points_id";
				sql+= " FROM ";
				sql+= _hidroGraphTableName;
				sql+= " WHERE ";
				sql+= "graph_name";
				sql+= " = '";
				sql+= graphName;
				sql+= "'";

//verify if exist the graph name
	if(!portal->query(sql) || !portal->fetchRow())
	{
		delete portal;

		_errorMessage = "Error getting layer points from this graph.";
		return NULL;
	}

//get layer id
	int layerId = portal->getInt(0);

	delete portal;

//get layer
	TeLayer* layer = getLayerById(layerId);

	return layer;
}

TeLayer* HidroMetadata::getGraphLayerLines(const std::string& graphName)
{
	_errorMessage = "";

//get database portal
	TeDatabasePortal* portal = _db->getPortal();

	if(!portal)
	{
		_errorMessage = "Database Portal Internal Error.";
		return NULL;
	}

//create sql to get layer id
	std::string sql = "SELECT ";
				sql+= "layer_lines_id";
				sql+= " FROM ";
				sql+= _hidroGraphTableName;
				sql+= " WHERE ";
				sql+= "graph_name";
				sql+= " = '";
				sql+= graphName;
				sql+= "'";

//verify if exist the graph name
	if(!portal->query(sql) || !portal->fetchRow())
	{
		delete portal;

		_errorMessage = "Error getting layer lines from this graph.";
		return NULL;
	}

//get layer id
	int layerId = portal->getInt(0);

	delete portal;

//get layer
	TeLayer* layer = getLayerById(layerId);

	return layer;
}

std::vector<std::string> HidroMetadata::getLayerPointsAttributes(const std::string& graphName, const bool& allAttrs)
{
	_errorMessage = "";

	std::vector<std::string> vecAttrOut;

	if(!graphName.empty())
	{
//getting layer points from this graph
		TeLayer* layer = this->getGraphLayerPoints(graphName);

		if(layer && !layer->attrTables().empty())
		{
//getting layer attribute names
			std::vector<std::string> vecAttr;
			if(!layer->attrTables()[0].attributeNames(vecAttr))
			{
				_errorMessage = "Error getting attribute names from layer.";
			}

			if(allAttrs)
			{
				return vecAttr;
			}
			else
			{
				for(unsigned int i = 0; i < vecAttr.size(); ++ i)
				{
					if(vecAttr[i] == this->getObjectIdAttrName() ||
						vecAttr[i] == this->getVertexNameAttrName())
					{
						continue;
					}

					vecAttrOut.push_back(vecAttr[i]);
				}
			}
		}
	}

	return vecAttrOut;
}

std::vector<std::string> HidroMetadata::getLayerLinesAttributes(const std::string& graphName, const bool& allAttrs)
{
	_errorMessage = "";

	std::vector<std::string> vecAttrOut;

	if(!graphName.empty())
	{
//getting layer lines from this graph
		TeLayer* layer = this->getGraphLayerLines(graphName);

		if(layer && !layer->attrTables().empty())
		{
//getting layer attribute names
			std::vector<std::string> vecAttr;
			if(!layer->attrTables()[0].attributeNames(vecAttr))
			{
				_errorMessage = "Error getting attribute names from layer.";
			}

			if(allAttrs)
			{
				return vecAttr;
			}
			else
			{
				for(unsigned int i = 0; i < vecAttr.size(); ++ i)
				{
					if(vecAttr[i] == this->getObjectIdAttrName() ||
						vecAttr[i] == this->getEdgeNameAttrName() ||
						vecAttr[i] == this->getEdgeVertexFromAttrName() ||
						vecAttr[i] == this->getEdgeVertexToAttrName())
					{
						continue;
					}

					vecAttrOut.push_back(vecAttr[i]);
				}
			}
		}
	}

	return vecAttrOut;
}

std::vector<std::string> HidroMetadata::getGraphCostsAttributes(const std::string& graphName)
{
	_errorMessage = "";

	std::vector<std::string> vecAttrOut;

	if(!graphName.empty())
	{
//getting layer lines from this graph
		TeLayer* layer = this->getGraphLayerLines(graphName);

		if(layer && !layer->attrTables().empty())
		{
//getting layer attribute names
			TeAttributeList list = layer->attrTables()[0].attributeList();
			
			for(unsigned int i = 0; i < list.size(); ++ i)
			{
				TeAttribute attr = list[i];
				
				if(attr.rep_.type_ == TeREAL || attr.rep_.type_ == TeINT)
				{
					vecAttrOut.push_back(attr.rep_.name_);
				}
			}
		}
	}

	return vecAttrOut;
}

TeProjection* HidroMetadata::getGraphProjection(const std::string& graphName)
{
	_errorMessage = "";

	if(!graphName.empty())
	{
//getting layer lines from this graph
		TeLayer* layer = this->getGraphLayerPoints(graphName);

		if(layer)
		{
			TeProjection* proj = TeProjectionFactory::make(layer->projection()->params());

			return proj;
		}
	}

	return NULL;
}

std::vector<std::string> HidroMetadata::getGraphNameListFromDB()
{
	_errorMessage = "";

	std::vector<std::string> vec;

//get database portal
	TeDatabasePortal* portal = _db->getPortal();

	if(!portal)
	{
		_errorMessage = "Database Portal Internal Error.";
		return vec;
	}

//create sql to get all graph names
	std::string sql = "SELECT ";
				sql+= "graph_name";
				sql+= " FROM ";
				sql+= _hidroGraphTableName;

//verify if exist the graph names
	if(!portal->query(sql))
	{
		delete portal;

		_errorMessage = "Error getting graph names from database.";
		return vec;
	}

//get graph names
	while(portal->fetchRow())
	{
		vec.push_back(portal->getData(0));
	}

	delete portal;

	return vec;
}

bool HidroMetadata::existGraph(const std::string& graphName)
{
	if(graphName.empty())
	{
		return false;
	}

//get graph names from database
	std::vector<std::string> graphNames = getGraphNameListFromDB();

	for(unsigned int i = 0; i < graphNames.size(); ++i)
	{
		if(graphNames[i] == graphName)
		{
//graph found
			return true;
		}
	}

//graph not found
	return false;
}

std::string HidroMetadata::getVertexName(const std::string& graphName, const std::string& vertexObjId)
{
	_errorMessage = "";

	std::string name = "";

	TeLayer* layer = this->getGraphLayerPoints(graphName);

	if(layer)
	{
//get database portal
		TeDatabasePortal* portal = _db->getPortal();

		if(!portal)
		{
			_errorMessage = "Database Portal Internal Error.";
			return NULL;
		}

//create sql to get vertex name from id
		std::string table = layer->attrTables()[0].name();

		std::string sql = "SELECT ";
					sql+= this->getVertexNameAttrName();
					sql+= " FROM ";
					sql+= table;
					sql+= " WHERE ";
					sql+= this->getObjectIdAttrName();
					sql+= " = '";
					sql+= vertexObjId;
					sql+= "'";


//get vertex name
		if(portal->query(sql) && portal->fetchRow())
		{
			name = portal->getData(0);
		}

		delete portal;
	}

	return name;
}

std::string HidroMetadata::getEdgeName(const std::string& graphName, const std::string& edgeObjId)
{
	_errorMessage = "";

	std::string name = "";

	TeLayer* layer = this->getGraphLayerLines(graphName);

	if(layer)
	{
//get database portal
		TeDatabasePortal* portal = _db->getPortal();

		if(!portal)
		{
			_errorMessage = "Database Portal Internal Error.";
			return NULL;
		}

//create sql to get edge name from id
		std::string table = layer->attrTables()[0].name();

		std::string sql = "SELECT ";
					sql+= this->getEdgeNameAttrName();
					sql+= " FROM ";
					sql+= table;
					sql+= " WHERE ";
					sql+= this->getObjectIdAttrName();
					sql+= " = '";
					sql+= edgeObjId;
					sql+= "'";


//get edge name
		if(portal->query(sql) && portal->fetchRow())
		{
			name = portal->getData(0);
		}

		delete portal;
	}

	return name;
}

std::string HidroMetadata::getHidroTableName()
{
	return _hidroGraphTableName;
}

std::string HidroMetadata::getVertexNameAttrName()
{
	return _vertexNameAttrName;
}

std::string HidroMetadata::getEdgeNameAttrName()
{
	return _edgeNameAttrName;
}

std::string HidroMetadata::getEdgeVertexFromAttrName()
{
	return _edgeVertexFromAttrName;
}

std::string HidroMetadata::getEdgeVertexToAttrName()
{
	return _edgeVertexToAttrName;
}

std::string HidroMetadata::getObjectIdAttrName()
{
	return _objectId;
}

std::string HidroMetadata::getGraphVertexAttrTableName(const std::string& graphName)
{
	std::string vertexAttrName = "";

	if(graphName.empty() || !existGraph(graphName))
	{
		return vertexAttrName;
	}

//get database portal
	TeDatabasePortal* portal = _db->getPortal();

	if(!portal)
	{
		return vertexAttrName;
	}

//create sql to verify if the attr exist
	std::string sql = "SELECT vertex_attr_table";
				sql+= " FROM ";
				sql+= this->getHidroTableName();
				sql+= " WHERE ";
				sql+= "graph_name";
				sql+= " = '";
				sql+= graphName;
				sql+= "'";

//verify
	if(portal->query(sql) && portal->fetchRow())
	{
		vertexAttrName = portal->getData(0);
	}

	delete portal;

	return vertexAttrName;
}

std::string HidroMetadata::getGraphEdgeAttrTableName(const std::string& graphName)
{
	std::string edgeAttrName = "";

	if(graphName.empty() || !existGraph(graphName))
	{
		return edgeAttrName;
	}

//get database portal
	TeDatabasePortal* portal = _db->getPortal();

	if(!portal)
	{
		return edgeAttrName;
	}

//create sql to verify if the attr exist
	std::string sql = "SELECT edge_attr_table";
				sql+= " FROM ";
				sql+= this->getHidroTableName();
				sql+= " WHERE ";
				sql+= "graph_name";
				sql+= " = '";
				sql+= graphName;
				sql+= "'";

//verify
	if(portal->query(sql) && portal->fetchRow())
	{
		edgeAttrName = portal->getData(0);
	}

	delete portal;

	return edgeAttrName;
}

bool HidroMetadata::addGraphVertexAttrEntry(const std::string& graphName, const std::string& attrName, const std::string& layerName, const std::string& layerAttrName)
{
	_errorMessage = "";

	if(graphName.empty() || !existGraph(graphName))
	{
		return false;
	}

	if(attrName.empty())
	{
		_errorMessage = "Invalid input parameters.";
		return false;
	}

	if(existGraphAttrEntry(graphName, attrName, true))
	{
		return false;
	}

	std::string table = getGraphVertexAttrTableName(graphName);

//create new table entry sql
	std::string sql = "INSERT INTO ";
				sql+= table;
				sql+= " (attribute, origin_layer, origin_layer_attr)";
				sql+= " VALUES ('";
				sql+= attrName;
				sql+= "', '";
				sql+= layerName;
				sql+= "', '";
				sql+= layerAttrName;
				sql+= "')";

//save entry in database table
	if(!_db->execute(sql))
	{
		_errorMessage = _db->errorMessage();
		return false;
	}


	return true;
}

bool HidroMetadata::addGraphEdgeAttrEntry(const std::string& graphName, const std::string& attrName, const std::string& layerName, const std::string& layerAttrName)
{
	_errorMessage = "";

	if(graphName.empty() || !existGraph(graphName))
	{
		return false;
	}

	if(attrName.empty())
	{
		_errorMessage = "Invalid input parameters.";
		return false;
	}

	if(existGraphAttrEntry(graphName, attrName, false))
	{
		return false;
	}

	std::string table = getGraphEdgeAttrTableName(graphName);

//create new table entry sql
	std::string sql = "INSERT INTO ";
				sql+= table;
				sql+= " (attribute, origin_layer, origin_layer_attr)";
				sql+= " VALUES ('";
				sql+= attrName;
				sql+= "', '";
				sql+= layerName;
				sql+= "', '";
				sql+= layerAttrName;
				sql+= "')";

//save entry in database table
	if(!_db->execute(sql))
	{
		_errorMessage = _db->errorMessage();
		return false;
	}

	return true;
}

bool HidroMetadata::existGraphAttrEntry(const std::string& graphName, const std::string& attrName, const bool& vertexAttr)
{
	_errorMessage = "";

	if(graphName.empty() || !existGraph(graphName))
	{
		return false;
	}

	if(attrName.empty())
	{
		_errorMessage = "Invalid input parameters.";
		return false;
	}

//get database portal
	TeDatabasePortal* portal = _db->getPortal();

	if(!portal)
	{
		_errorMessage = "Database Portal Internal Error.";
		return false;
	}

//create sql to verify if the attr exist
	std::string table = "";

	if(vertexAttr)
	{
		table = getGraphVertexAttrTableName(graphName);
	}
	else
	{
		table = getGraphEdgeAttrTableName(graphName);
	}

	std::string sql = "SELECT *";
				sql+= " FROM ";
				sql+= table;
				sql+= " WHERE ";
				sql+= " attribute = '";
				sql+= attrName;
				sql+= "'";

//verify
	if(portal->query(sql) && portal->fetchRow())
	{
		delete portal;
		return true;
	}

	delete portal;
	return false;
}

bool HidroMetadata::removeGraphAttrEntry(const std::string& graphName, const std::string& attrName, const bool& vertexAttr)
{
	_errorMessage = "";

	if(graphName.empty() || !existGraph(graphName))
	{
		return false;
	}

	if(attrName.empty())
	{
		_errorMessage = "Invalid input parameters.";
		return false;
	}

//remove attr table entry sql

	std::string table = "";

	if(vertexAttr)
	{
		table = getGraphVertexAttrTableName(graphName);
	}
	else
	{
		table = getGraphEdgeAttrTableName(graphName);
	}

	std::string sql = "DELETE FROM ";
				sql+= table;
				sql+= " WHERE ";
				sql+= "attribute";
				sql+= " = '";
				sql+= attrName;
				sql+= "'";

//remove entry from database table
	if(!_db->execute(sql))
	{
		_errorMessage = _db->errorMessage();
		return false;
	}

	return true;
}

bool HidroMetadata::removeGraphAttr(const std::string& graphName, const std::string& attrName, const bool& vertexAttr)
{
	_errorMessage = "";

	if(graphName.empty() || !existGraph(graphName))
	{
		return false;
	}

	if(attrName.empty())
	{
		_errorMessage = "Invalid input parameters.";
		return false;
	}

//get layer
	TeLayer* layer = NULL;

	if(vertexAttr)
	{
		layer = this->getGraphLayerPoints(graphName);
	}
	else
	{
		layer = this->getGraphLayerLines(graphName);
	}

	if(!layer || layer->attrTables().empty())
	{
		return false;
	}

//get attribute table name
	std::string tableName = layer->attrTables()[0].name();

	std::string sql = "ALTER TABLE ";
				sql+= tableName;
				sql+= " DROP ";
				sql+= attrName;

//remove column from database table
	if(!_db->execute(sql))
	{
		_errorMessage = _db->errorMessage();
		return false;
	}

	return true;
}

bool HidroMetadata::addGraphAttr(const std::string& graphName, const std::string& attrName, const bool& stringType, const bool& vertexAttr)
{
	_errorMessage = "";

	if(graphName.empty() || !existGraph(graphName))
	{
		return false;
	}

	if(attrName.empty())
	{
		_errorMessage = "Invalid input parameters.";
		return false;
	}

//get layer
	TeLayer* layer = NULL;

	if(vertexAttr)
	{
		layer = this->getGraphLayerPoints(graphName);
	}
	else
	{
		layer = this->getGraphLayerLines(graphName);
	}

	if(!layer || layer->attrTables().empty())
	{
		return false;
	}

//get attribute table name
	std::string tableName = layer->attrTables()[0].name();

//add column into database table
	TeAttributeRep attr;
	attr.name_ = attrName;
	attr.isPrimaryKey_ = false;

	if(stringType)
	{
		attr.type_ = TeSTRING;
		attr.numChar_ = 100;
	}
	else
	{
		attr.type_ = TeREAL;
		attr.isAutoNumber_ = false;
	}
	
	if(!_db->addColumn(tableName, attr))
	{
		_errorMessage = _db->errorMessage();
		return false;
	}

	return true;
}

std::vector<GraphAttr> HidroMetadata::getGraphAttrList(const std::string graphName, const bool& vertexAttr)
{
	std::vector<GraphAttr> graphList;

	if(!graphName.empty() && existGraph(graphName))
	{
//get database portal
		TeDatabasePortal* portal = _db->getPortal();

		if(!portal)
		{
			_errorMessage = "Database Portal Internal Error.";
			return graphList;
		}

//create sql to get attr list
		std::string table = "";

		if(vertexAttr)
		{
			table = getGraphVertexAttrTableName(graphName);
		}
		else
		{
			table = getGraphEdgeAttrTableName(graphName);
		}

		std::string sql = "SELECT *";
					sql+= " FROM ";
					sql+= table;

//get list
		if(portal->query(sql))
		{
			while(portal->fetchRow())
			{
				GraphAttr gAttr;

				gAttr.attrName = portal->getData(0);
				gAttr.layerName = portal->getData(1);
				gAttr.layerAttrName = portal->getData(2);

				graphList.push_back(gAttr);
			}
		}

		delete portal;
	}

	return graphList;
}

double HidroMetadata::getGraphResolution(const std::string& graphName)
{
	_errorMessage = "";

//get database portal
	TeDatabasePortal* portal = _db->getPortal();

	if(!portal)
	{
		_errorMessage = "Database Portal Internal Error.";
		return NULL;
	}

//create sql to get layer id
	std::string sql = "SELECT ";
				sql+= "graph_res";
				sql+= " FROM ";
				sql+= _hidroGraphTableName;
				sql+= " WHERE ";
				sql+= "graph_name";
				sql+= " = '";
				sql+= graphName;
				sql+= "'";

//verify if exist the graph name
	if(!portal->query(sql) || !portal->fetchRow())
	{
		delete portal;

		_errorMessage = "Error getting layer points from this graph.";
		return 0.;
	}

//get graph resolution
	double res = portal->getDouble(0);

	delete portal;

	return res;
}

bool HidroMetadata::createGraphMetadataTable()
{
	_errorMessage = "";

//verify if the table already exist
	if(_db->tableExist(_hidroGraphTableName))
	{
		return true;	
	}

//start database transaction
	if(!_db->beginTransaction())
	{
		_errorMessage = "Database internal error.";
		return false;
	}

//create attribute list
	TeAttributeList attrList;
	
	TeAttribute attr1;
	attr1.rep_.name_ = "graph_id";
	attr1.rep_.isPrimaryKey_= true;
	attr1.rep_.isAutoNumber_ = true;
	attr1.rep_.type_ = TeINT;
	attrList.push_back(attr1);

	TeAttribute attr2;
	attr2.rep_.name_ = "graph_name";
	attr2.rep_.numChar_ = 255;
	attr2.rep_.isPrimaryKey_= false;
	attr2.rep_.isAutoNumber_ = false;
	attr2.rep_.type_ = TeSTRING;
	attrList.push_back(attr2);

	TeAttribute attr3;
	attr3.rep_.name_ = "layer_points_id";
	attr3.rep_.isPrimaryKey_= false;
	attr3.rep_.isAutoNumber_ = false;
	attr3.rep_.type_ = TeINT;
	attrList.push_back(attr3);

	TeAttribute attr4;
	attr4.rep_.name_ = "layer_lines_id";
	attr4.rep_.isPrimaryKey_= false;
	attr4.rep_.isAutoNumber_ = false;
	attr4.rep_.type_ = TeINT;
	attrList.push_back(attr4);

	TeAttribute attr5;
	attr5.rep_.name_ = "vertex_attr_table";
	attr5.rep_.numChar_ = 255;
	attr5.rep_.isPrimaryKey_= false;
	attr5.rep_.isAutoNumber_ = false;
	attr5.rep_.type_ = TeSTRING;
	attrList.push_back(attr5);

	TeAttribute attr6;
	attr6.rep_.name_ = "edge_attr_table";
	attr6.rep_.numChar_ = 255;
	attr6.rep_.isPrimaryKey_= false;
	attr6.rep_.isAutoNumber_ = false;
	attr6.rep_.type_ = TeSTRING;
	attrList.push_back(attr6);

	TeAttribute attr7;
	attr7.rep_.name_ = "graph_res";
	attr7.rep_.isPrimaryKey_= false;
	attr7.rep_.isAutoNumber_ = false;
	attr7.rep_.type_ = TeREAL;
	attrList.push_back(attr7);

//create attr table
	if(!_db->createTable(_hidroGraphTableName, attrList))
	{
		_db->rollbackTransaction();

		_errorMessage = _db->errorMessage();
		return false;
	}

//commit and finish transaction
	if(!_db->commitTransaction())
	{
		_db->rollbackTransaction();

		_errorMessage = "Database internal error.";
		return false;
	}

	return true;
}

bool HidroMetadata::createAttrMetadataTable(const std::string& tableName)
{
	_errorMessage = "";

//verify if the table already exist
	if(_db->tableExist(tableName))
	{
		_errorMessage = "Table already exist.";
		return false;	
	}

//start database transaction
	if(!_db->beginTransaction())
	{
		_errorMessage = "Database internal error.";
		return false;
	}

//create attribute list
	TeAttributeList attrList;
	
	TeAttribute attr1;
	attr1.rep_.name_ = "attribute";
	attr1.rep_.isPrimaryKey_= true;
	attr1.rep_.isAutoNumber_ = false;
	attr1.rep_.type_ = TeSTRING;
	attr1.rep_.numChar_ = 255;
	attrList.push_back(attr1);

	TeAttribute attr2;
	attr2.rep_.name_ = "origin_layer";
	attr2.rep_.numChar_ = 255;
	attr2.rep_.isPrimaryKey_= false;
	attr2.rep_.isAutoNumber_ = false;
	attr2.rep_.type_ = TeSTRING;
	attrList.push_back(attr2);

	TeAttribute attr3;
	attr3.rep_.name_ = "origin_layer_attr";
	attr3.rep_.isPrimaryKey_= false;
	attr3.rep_.isAutoNumber_ = false;
	attr3.rep_.type_ = TeSTRING;
	attr3.rep_.numChar_ = 255;
	attrList.push_back(attr3);

//create attr table
	if(!_db->createTable(tableName, attrList))
	{
		_db->rollbackTransaction();

		_errorMessage = _db->errorMessage();
		return false;
	}

//commit and finish transaction
	if(!_db->commitTransaction())
	{
		_db->rollbackTransaction();

		_errorMessage = "Database internal error.";
		return false;
	}

	return true;
}

TeLayer* HidroMetadata::getLayerById(const int& id)
{
//find the layer in database layer map
	TeLayerMap::iterator it = _db->layerMap().begin();

	while(it != _db->layerMap().end())
	{
		if(it->second->id() == id)
		{
//layer found, return a valid pointer
			return it->second;
		}

		++it;
	}

//layer not found, retur invalid pointer
	return NULL;
}