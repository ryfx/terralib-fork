#include <HidroEdgeFunctions.h>
#include <HidroEdge.h>
#include <HidroVertex.h>
#include <HidroMetadata.h>
#include <HidroGraph.h>
#include <HidroPersister.h>

#include <TeGeometryAlgorithms.h>
#include <TeDatabase.h>

HidroEdgeFunctions::HidroEdgeFunctions(TeDatabase* database) : _db(database), _errorMessage("")
{
}

HidroEdgeFunctions::~HidroEdgeFunctions()
{
}

bool HidroEdgeFunctions::createDistanceAttribute(const std::string& graphName, const std::string& attrName)
{
	_errorMessage = "";

//check input paramters
	if(graphName.empty() || attrName.empty())
	{
		_errorMessage = "Invalid input parameters.";
		return false;
	}

	HidroMetadata metadata(_db);

	if(!metadata.existGraph(graphName))
	{
		_errorMessage = "Graph does not exist.";
		return false;
	}

	if(metadata.existGraphAttrEntry(graphName, attrName, false))
	{
		_errorMessage = "Graph Attribute already exist.";
		return false;
	}

//load graph from database
	HidroPersister persister(_db);

	HidroGraph graph;

	if(!persister.loadGraphFromDatabase(graphName, graph))
	{
		_errorMessage = persister.getErrorMessage();
		return false;
	}

//create the attribute into table
	if(!metadata.addGraphAttr(graphName, attrName, false, false))
	{
		_errorMessage = metadata.getErrorMessage();
		return false;
	}

//get attribute table name
	TeLayer* layer = metadata.getGraphLayerLines(graphName);
	
	if(!layer || layer->attrTables().empty())
	{
		_errorMessage = metadata.getErrorMessage();
		return false;
	}

	std::string tableName = layer->attrTables()[0].name();


//get edges and generate distance attribute
	std::map<std::string, HidroEdge*>::iterator itEdge = graph.getGraphMapEdge().begin();

	if(TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Creating Edge Distance Attribute");
		TeProgress::instance()->setTotalSteps(graph.getGraphMapEdge().size());
	}

	int count = 0;

	while(itEdge != graph.getGraphMapEdge().end())
	{
		HidroEdge* edge = itEdge->second;
		
		std::string edgeId = edge->getEdgeId();

		double distance = TeDistance(edge->getEdgeVertexFrom()->getVertexCoord(), edge->getEdgeVertexTo()->getVertexCoord());

//insert new entry in table
		std::string sql = "UPDATE ";
				sql+= tableName;
				sql+= " SET ";
				sql+= attrName;
				sql+= " = ";
				sql+= Te2String(distance, 6);
				sql+= " WHERE ";
				sql+= metadata.getEdgeNameAttrName();
				sql+= " = '";
				sql+= edgeId;
				sql+= "'";

//save entry in database table
		if(!_db->execute(sql))
		{
			_errorMessage = _db->errorMessage();
			return false;
		}

		if(TeProgress::instance())
		{
			TeProgress::instance()->setProgress(count);
		}

		++count;
		++itEdge;
	}

	if(TeProgress::instance())
	{
		TeProgress::instance()->reset();
	}

	return true;
}

std::string HidroEdgeFunctions::getErrorMessage() const
{
	return _errorMessage;
}