#include <HidroEdgeEdition.h>
#include <HidroGraphEdition.h>
#include <HidroMetadata.h>

#include <TeGeometryAlgorithms.h>
#include <TeDatabase.h>

#include <TeQtGrid.h>


HidroEdgeEdition::HidroEdgeEdition(TeTheme* edgeTheme, TeTheme* vertexTheme, TeQtGrid* grid) : 
_edgeTheme(edgeTheme), 
_vertexTheme(vertexTheme), 
_grid(grid)
{
}

HidroEdgeEdition::~HidroEdgeEdition()
{
}

std::string HidroEdgeEdition::getErrorMessage() const
{
	return _errorMessage;
}

bool HidroEdgeEdition::addEdge(const std::string& vertexFromId, const std::string& vertexToId, const std::vector<std::string>& attrs, const std::string& name)
{
	_errorMessage = "";

//get layer of points
	TeLayer* layer = _edgeTheme->layer();

	if(!layer->hasGeometry(TeLINES))
	{
		_errorMessage = "This theme does not have Line Representation.";
		return false;
	}

//get coords
	HidroGraphEdition gVertexEdit(_vertexTheme);
	TeCoord2D origin = gVertexEdit.getVertexCoord(vertexFromId);
	TeCoord2D destiny = gVertexEdit.getVertexCoord(vertexToId);

//get new object id
	HidroGraphEdition gEdit(_edgeTheme);
	std::string objId = gEdit.getEdgeNewObjId();

	if(objId.empty())
	{
		_errorMessage = "Error getting new object id for edge.";
		return false;
	}

//create new line
	TeLine2D line;
	line.add(origin);
	line.add(destiny);
	
	line.objectId(objId);

	TeLineSet ls;
	ls.add(line);

//create attr table entry
	TeTable& table = layer->attrTables()[0];
	table.clear();


	TeTableRow row;

	row.push_back(vertexFromId);
	row.push_back(vertexToId);
	row.push_back(name);
	row.push_back(objId);

	for(unsigned int i = 0; i < attrs.size(); ++i)
	{
		row.push_back(attrs[i]);
	}


	table.add(row);

//add line in layer
	if(!layer->addLines(ls))
	{
		_errorMessage = "Error adding line geometry in layer.";
		return false;
	}

//add attribute in layer attr table
	if(!layer->database()->insertTable(table))
	{
		_errorMessage = "Error adding attribute entry in layer.";
		return false;
	}

//update theme collection
	if(!gEdit.insertCollectionEntry(TeFindCentroid(line), line.objectId(), _grid))
	{
		_errorMessage = gEdit.getErrorMessage();
		return false;
	}

	return true;
}

bool HidroEdgeEdition::changeEdge(const std::string& edgeId, const std::string& vertexId, const bool& changeVertexFrom)
{
	_errorMessage = "";

	TeDatabase* db = _edgeTheme->layer()->database();

//get coord
	HidroGraphEdition gVertexEdit(_vertexTheme);
	TeCoord2D coord = gVertexEdit.getVertexCoord(vertexId);

//get line
	HidroGraphEdition gEdit(_edgeTheme);
	TeLine2D line = gEdit.getEdgeLine(edgeId);

	if(changeVertexFrom)
	{
		line[0] = coord;
	}
	else
	{
		line[1] = coord;
	}

//update box line
	TeBox box;
	updateBox(box, line[0]);
	updateBox(box, line[1]);
	line.setBox(box);

//update line
	std::string table = _edgeTheme->layer()->tableName(TeLINES);
	if(!db->updateLine(table, line))
	{
		_errorMessage = "Error updating geometry line";
		return false;
	}

//update attrs
	HidroMetadata metadata(db);

	std::string tableName = _edgeTheme->layer()->attrTables()[0].name();

	std::string sql = "UPDATE ";
				sql+= tableName;
				sql+= " SET ";

	if(changeVertexFrom)
	{
				sql+= metadata.getEdgeVertexFromAttrName();
	}
	else
	{
				sql+= metadata.getEdgeVertexToAttrName();
	}
				sql+= " = '";
				sql+= vertexId;
				sql+= "' WHERE ";
				sql+= metadata.getEdgeNameAttrName();
				sql+= " = '";
				sql+= edgeId;
				sql+= "'";

	if(!db->execute(sql))
	{
		_errorMessage = "Error updating edge attributes.";
		return false;
	}

//update theme collection
	if(!gEdit.changeCollectionEntry(TeFindCentroid(line), line.objectId()))
	{
		_errorMessage = gEdit.getErrorMessage();
		return false;
	}

	return true;
}

bool HidroEdgeEdition::removeEdge(const std::string& edgeId)
{
	_errorMessage = "";

//getting object id
	HidroGraphEdition gEdit(_edgeTheme);
	std::string objId = gEdit.getObjIdFromEdge(edgeId);

	if(objId.empty())
	{
		_errorMessage = "Error getting object id from edge.";
		return false;
	}

//get layer of points
	TeLayer* layer = _edgeTheme->layer();

	if(!layer->hasGeometry(TeLINES))
	{
		_errorMessage = "This theme does not have Line Representation.";
		return false;
	}

//remove point
	std::string tablePoints = layer->tableName(TeLINES);

	std::string sqlGeom = "DELETE FROM " + tablePoints;
				sqlGeom+= " WHERE object_id = '" + objId + "'";

	if(!layer->database()->execute(sqlGeom))
	{
		_errorMessage = "Error removing geometry line.";
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