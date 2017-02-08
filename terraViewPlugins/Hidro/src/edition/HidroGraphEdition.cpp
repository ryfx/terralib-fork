#include <HidroGraphEdition.h>
#include <HidroMetadata.h>

#include <TeDatabase.h>

#include <TeQtGrid.h>

HidroGraphEdition::HidroGraphEdition(TeTheme* theme) : _theme(theme)
{
}

HidroGraphEdition::~HidroGraphEdition()
{
}

std::string HidroGraphEdition::getErrorMessage() const
{
	return _errorMessage;
}

bool HidroGraphEdition::insertCollectionEntry(const TeCoord2D& coord, const std::string& objId, TeQtGrid* grid)
{
	_errorMessage = "";

//get variables
	TeDatabase* db		= _theme->layer()->database();
	std::string CT		= _theme->collectionTable();
	std::string CAT		= _theme->collectionAuxTable();

//insert entry in collection
	std::string sqlCT = "INSERT INTO ";
				sqlCT+= CT;
				sqlCT+= " (c_object_id, label_x, label_y, c_legend_own, c_object_status)";
				sqlCT+= " VALUES ('";
				sqlCT+= objId;
				sqlCT+= "', ";
				sqlCT+= Te2String(coord.x());
				sqlCT+= ", ";
				sqlCT+= Te2String(coord.y());
				sqlCT+= ", 0, 1)";

	if(!db->execute(sqlCT))
	{
		_errorMessage = "Error adding entry in collection table.";
		return false;
	}

//insert entry in collection aux
	int uniqueId = atoi(objId.c_str()) + 1;

	std::string sqlCAT = "INSERT INTO ";
				sqlCAT+= CAT;
				sqlCAT+= " (object_id, unique_id, grid_status)";
				sqlCAT+= " VALUES ('";
				sqlCAT+= objId;
				sqlCAT+= "', ";
				sqlCAT+= Te2String(uniqueId);
				sqlCAT+= ", 1)";

	if(!db->execute(sqlCAT))
	{
		_errorMessage = "Error adding entry in collection aux table.";
		return false;
	}

//insert object in grid
	if(grid)
	{
		grid->getPortal()->freeResult();
		grid->insertObjectIntoCollection(objId);
		grid->updatePortalContents();
	}

	return true;
}

bool HidroGraphEdition::changeCollectionEntry(const TeCoord2D& coord, const std::string& objId)
{
	_errorMessage = "";

//get variables
	TeDatabase* db		= _theme->layer()->database();
	std::string CT		= _theme->collectionTable();
	std::string CAT		= _theme->collectionAuxTable();

//update collection table
	std::string sqlCT = "UPDATE ";
				sqlCT+= CT;
				sqlCT+= " SET ";
				sqlCT+= "label_x = ";
				sqlCT+= Te2String(coord.x());
				sqlCT+= ", label_y = ";
				sqlCT+= Te2String(coord.y());
				sqlCT+= " WHERE c_object_id = '";
				sqlCT+= objId;
				sqlCT+= "'";

	if(!db->execute(sqlCT))
	{
		_errorMessage = "Error updating entry in collection table.";
		return false;
	}

	return true;
}

bool HidroGraphEdition::revemoCollectionEntry(const std::string& objId, TeQtGrid* grid)
{
	_errorMessage = "";

//get variables
	TeDatabase* db		= _theme->layer()->database();
	std::string CT		= _theme->collectionTable();
	std::string CAT		= _theme->collectionAuxTable();

//remove from collection table
	std::string sqlCT = "DELETE FROM ";
				sqlCT+= CT;
				sqlCT+= " WHERE c_object_id = '";
				sqlCT+= objId;
				sqlCT+= "'";

	if(!db->execute(sqlCT))
	{
		_errorMessage = "Error removing entry from collection table.";
		return false;
	}

//remove from collection aut table
	std::string sqlCAT = "DELETE FROM ";
				sqlCAT+= CAT;
				sqlCAT+= " WHERE object_id = '";
				sqlCAT+= objId;
				sqlCAT+= "'";

	if(!db->execute(sqlCAT))
	{
		_errorMessage = "Error removing entry from collection aux table.";
		return false;
	}

//remove object from grid
	if(grid)
	{
		grid->deletePointedLines();
		grid->updatePortalContents();
	}

	return true;
}

std::vector<std::string> HidroGraphEdition::getEdgesFromVertex(const std::string& vertexId, const std::string& tableEdgeName)
{
	std::vector<std::string> edges;

//get variables
	TeDatabase* db = _theme->layer()->database();

	HidroMetadata metadata(db);

//create sql to get edges belongs to this vertex
	std::string sql = "SELECT ";
				sql+= metadata.getEdgeNameAttrName();
				sql+= " FROM ";
				sql+= tableEdgeName;
				sql+= " WHERE ";
				sql+= metadata.getEdgeVertexFromAttrName();
				sql+= " = '";
				sql+= vertexId;
				sql+= "'";
				sql+= " OR ";
				sql+= metadata.getEdgeVertexToAttrName();
				sql+= " = '";
				sql+= vertexId;
				sql+= "'";

	TeDatabasePortal* portal = db->getPortal();

//verify if edge was found
	if(portal && portal->query(sql))
	{
		while(portal->fetchRow())
		{
			edges.push_back(portal->getData(0));
		}
	}

	return edges;
}

TeCoord2D HidroGraphEdition::getVertexCoord(const std::string& vertexId)
{
	TePointSet ps;

	std::string objId = this->getObjIdFromVertex(vertexId);

//create where clause to get specific point
	std::string whereClause = "object_id = '" + objId + "'";

	TeLayer* layer = _theme->layer();

//search the point in point set
	if(layer->getPoints(ps, whereClause))
	{
		return ps[0].elem();
	}

	return TeCoord2D();
}

TeLine2D HidroGraphEdition::getEdgeLine(const std::string& edgeId)
{
	TeLineSet ls;

	std::string objId = this->getObjIdFromEdge(edgeId);

//create where clause to get specific point
	std::string whereClause = "object_id = '" + objId + "'";

	TeLayer* layer = _theme->layer();

//search the point in point set
	TeLine2D line;

	if(layer->getLines(ls, whereClause))
	{
		line = ls[0];
	}

	return line;
}

std::string HidroGraphEdition::getObjIdFromVertex(const std::string& vertexName)
{
	_errorMessage = "";

	TeDatabase* db = _theme->layer()->database();
	
	std::string tableName = _theme->layer()->attrTables()[0].name();

	HidroMetadata metadata(db);

	std::string sql = "SELECT " + metadata.getObjectIdAttrName() + " FROM ";
				sql+= tableName + " WHERE " + metadata.getVertexNameAttrName() + " ='";
				sql+= vertexName + "'";

	TeDatabasePortal* portal = db->getPortal();

	std::string objId = "";

	if(!portal)
	{
		return "";
	}

	if(portal->query(sql) && portal->fetchRow())
	{
		objId = portal->getData(0);
	}

	delete portal;

	return objId;
}

std::string HidroGraphEdition::getObjIdFromEdge(const std::string& edgeName)
{
	_errorMessage = "";

	TeDatabase* db = _theme->layer()->database();
	
	std::string tableName = _theme->layer()->attrTables()[0].name();

	HidroMetadata metadata(db);

	std::string sql = "SELECT " + metadata.getObjectIdAttrName() + " FROM ";
				sql+= tableName + " WHERE " + metadata.getEdgeNameAttrName() + " ='";
				sql+= edgeName + "'";

	TeDatabasePortal* portal = db->getPortal();

	std::string objId = "";

	if(!portal)
	{
		return "";
	}

	if(portal->query(sql) && portal->fetchRow())
	{
		objId = portal->getData(0);
	}

	delete portal;

	return objId;
}

std::string HidroGraphEdition::getVertexNewObjId()
{
	_errorMessage = "";

	TeDatabase* db				= _theme->layer()->database();
	TeDatabasePortal* portal	= db->getPortal();
	
	std::string tableName = _theme->layer()->tableName(TePOINTS);

	std::string sql = "SELECT MAX (geom_id) FROM " + tableName;

	std::string objId = "";

	if(!portal)
	{
		return "";
	}

	if(portal->query(sql) && portal->fetchRow())
	{
		int value = portal->getInt(0);
		value += 1;

		objId = Te2String(value);
	}

	delete portal;

	return objId;
}

std::string HidroGraphEdition::getEdgeNewObjId()
{
	_errorMessage = "";

	TeDatabase* db				= _theme->layer()->database();
	TeDatabasePortal* portal	= db->getPortal();
	
	std::string tableName = _theme->layer()->tableName(TeLINES);

	std::string sql = "SELECT MAX (geom_id) FROM " + tableName;

	std::string objId = "";

	if(!portal)
	{
		return "";
	}

	if(portal->query(sql) && portal->fetchRow())
	{
		int value = portal->getInt(0);
		value += 1;

		objId = Te2String(value);
	}

	delete portal;

	return objId;
}