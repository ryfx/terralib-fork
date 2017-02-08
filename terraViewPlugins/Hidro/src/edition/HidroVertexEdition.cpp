#include <HidroVertexEdition.h>
#include <HidroEdgeEdition.h>
#include <HidroGraphEdition.h>

#include <TeDatabase.h>
#include <TeTheme.h>

#include <TeQtGrid.h>

HidroVertexEdition::HidroVertexEdition(TeTheme* theme, TeQtGrid* grid) : _theme(theme), _grid(grid)
{
}

HidroVertexEdition::~HidroVertexEdition()
{
}

std::string HidroVertexEdition::getErrorMessage() const
{
	return _errorMessage;
}

bool HidroVertexEdition::addVertex(const double& xCoord, const double& yCoord, const std::vector<std::string>& attrs, const std::string& name)
{
	_errorMessage = "";

//get layer of points
	TeLayer* layer = _theme->layer();

	if(!layer->hasGeometry(TePOINTS))
	{
		_errorMessage = "This theme does not have Point Representation.";
		return false;
	}

//get new object id
	HidroGraphEdition gEdit(_theme);
	std::string objId = gEdit.getVertexNewObjId();

	if(objId.empty())
	{
		_errorMessage = "Error getting new object id for vertex.";
		return false;
	}

//create new point
	TePoint pt(TeCoord2D(xCoord, yCoord));
	pt.objectId(objId);

	TePointSet ps;
	ps.add(pt);

//create attr table entry
	TeTable& table = layer->attrTables()[0];
	table.clear();

	TeTableRow row;

	row.push_back(name);
	row.push_back(objId);

	for(unsigned int i = 0; i < attrs.size(); ++i)
	{
		row.push_back(attrs[i]);
	}

	table.add(row);

//add point in layer
	if(!layer->addPoints(ps))
	{
		_errorMessage = "Error adding point geometry in layer.";
		return false;
	}

//add attribute in layer attr table
	if(!layer->database()->insertTable(table))
	{
		_errorMessage = "Error adding attribute entry in layer.";
		return false;
	}

//update theme collection
	if(!gEdit.insertCollectionEntry(pt.elem(), pt.objectId(), _grid))
	{
		_errorMessage = gEdit.getErrorMessage();
		return false;
	}

	return true;
}

bool HidroVertexEdition::removeVertex(const std::string& vertexId, const std::string& edgeTable)
{
	_errorMessage = "";

//removes edges in and out from this vertex
	HidroGraphEdition gEdit(_theme);
	std::vector<std::string> edges = gEdit.getEdgesFromVertex(vertexId, edgeTable);

	if(!edges.empty())
	{
		_errorMessage = "This vertex has edges.";
		return false;
	}


//getting object id
	std::string objId = gEdit.getObjIdFromVertex(vertexId);

	if(objId.empty())
	{
		_errorMessage = "Error getting object id from vertex.";
		return false;
	}

//get layer of points
	TeLayer* layer = _theme->layer();

	if(!layer->hasGeometry(TePOINTS))
	{
		_errorMessage = "This theme does not have Point Representation.";
		return false;
	}

//remove point
	std::string tablePoints = layer->tableName(TePOINTS);

	std::string sqlGeom = "DELETE FROM " + tablePoints;
				sqlGeom+= " WHERE object_id = '" + objId + "'";

	if(!layer->database()->execute(sqlGeom))
	{
		_errorMessage = "Error removing geometry point.";
		return false;
	}

//remove attr point
	TeTable& table = layer->attrTables()[0];

	std::string sqlAttr = "DELETE FROM " + table.name();
				sqlAttr+= " WHERE " + table.linkName();
				sqlAttr+= " = '" + objId + "'";

	if(!layer->database()->execute(sqlAttr))
	{
		_errorMessage = "Error removing attribute point.";
		return false;
	}

//update theme collection
	if(!gEdit.revemoCollectionEntry(objId, _grid))
	{
		_errorMessage = gEdit.getErrorMessage();
		return false;
	}

	return true;
}