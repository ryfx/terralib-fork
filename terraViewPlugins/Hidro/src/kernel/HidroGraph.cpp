#include <HidroGraph.h>
#include <HidroVertex.h>
#include <HidroEdge.h>

HidroGraph::HidroGraph() : _graphName(""), _graphBox(TeBox())
{
}

HidroGraph::HidroGraph(const std::string& name): _graphBox(TeBox())
{
	_graphName = name;
}

HidroGraph::~HidroGraph()
{
	std::map<std::string, HidroVertex*>::iterator itVertex = _graphMapVertex.begin();

	while(itVertex != _graphMapVertex.end())
	{
		delete itVertex->second;

		++itVertex;
	}

	_graphMapVertex.clear();

	_graphMapVertexIdx.clear();


	std::map<std::string, HidroEdge*>::iterator itEdge = _graphMapEdge.begin();

	while(itEdge != _graphMapEdge.end())
	{
		delete itEdge->second;

		++itEdge;
	}
	
	_graphMapEdge.clear();
}

void HidroGraph::setGraphName(const std::string& name)
{
	_graphName = name;
}

std::string HidroGraph::getGraphName() const
{
	return _graphName;
}

bool HidroGraph::addGraphVertex(HidroVertex* vertex)
{
	//find the vertex from the graph vector
	std::map<std::string, HidroVertex*>::iterator it = _graphMapVertex.find(vertex->getVertexId());

	if(it != _graphMapVertex.end())
	{
		return true;
	}

	//if not found, the new vertex will be added into the graph vector
	_graphMapVertex.insert(std::map<std::string, HidroVertex*>::value_type(vertex->getVertexId(), vertex));

	_graphMapVertexIdx.insert(std::map<int, std::string>::value_type(vertex->getVertexIdx(), vertex->getVertexId()));

	updateBox(_graphBox, vertex->getVertexCoord());

	return true;
}

bool HidroGraph::removeGraphVertex(const std::string& vertexId)
{
	//find the vertex from the graph vector
	std::map<std::string, HidroVertex*>::iterator it = _graphMapVertex.find(vertexId);

	if(it == _graphMapVertex.end())
	{
		return false;
	}

	std::map<int, std::string>::iterator itIdx = _graphMapVertexIdx.find(it->second->getVertexIdx());

	if(itIdx == _graphMapVertexIdx.end())
	{
		return false;
	}

	//if found, the vertex will be removed from the graph vector
	_graphMapVertexIdx.erase(itIdx);

	_graphMapVertex.erase(it);

	return true;
}

HidroVertex* HidroGraph::getGraphVertex(const std::string& vertexId)
{
	//find the vertex from the graph vector
	std::map<std::string, HidroVertex*>::iterator it = _graphMapVertex.find(vertexId);

	if(it == _graphMapVertex.end())
	{
		return NULL;
	}

	//if found, the get the vertex from the graph vector
	return it->second;
}

HidroVertex* HidroGraph::getGraphVertexByIdx(const int& vertexIdx)
{
	std::map<int, std::string>::iterator itIdx = _graphMapVertexIdx.find(vertexIdx);

	if(itIdx == _graphMapVertexIdx.end())
	{
		return NULL;
	}

	std::string vertexId = itIdx->second;

	return getGraphVertex(vertexId);
}

std::map<std::string, HidroVertex*>& HidroGraph::getGraphMapVertex()
{
	return _graphMapVertex;
}

bool HidroGraph::addGraphEdge(HidroEdge* edge)
{
	//find the edge from the graph vector
	std::map<std::string, HidroEdge*>::iterator it = _graphMapEdge.find(edge->getEdgeId());

	if(it != _graphMapEdge.end())
	{
		return true;
	}

	//if not found, the new edge will be added into the graph vector
	_graphMapEdge.insert(std::map<std::string, HidroEdge*>::value_type(edge->getEdgeId(), edge));

	return true;
}

bool HidroGraph::removeGraphEdge(const std::string& edgeId)
{
	//find the edge from the graph vector
	std::map<std::string, HidroEdge*>::iterator it = _graphMapEdge.find(edgeId);

	if(it == _graphMapEdge.end())
	{
		return false;
	}

	//if found, the edge will be removed from the graph vector
	_graphMapEdge.erase(it);

	return true;
}

HidroEdge* HidroGraph::getGraphEdge(const std::string& edgeId)
{
	//find the edge from the graph vector
	std::map<std::string, HidroEdge*>::iterator it = _graphMapEdge.find(edgeId);

	if(it == _graphMapEdge.end())
	{
		return false;
	}

	//if found, the get the edge from the graph vector
	return it->second;
}

std::map<std::string, HidroEdge*>& HidroGraph::getGraphMapEdge()
{
	return _graphMapEdge;
}

bool HidroGraph::isGraphValid()
{
	if(_graphName.empty())
	{
		return false;
	}

	if(_graphMapVertex.empty())
	{
		return false;
	}

	if(_graphMapEdge.empty())
	{
		return false;
	}

	std::map<std::string, HidroEdge*>::iterator it = _graphMapEdge.begin();

	while(it != _graphMapEdge.end())
	{
		if(it->second->getEdgeVertexFrom() == NULL)
		{
			return false;
		}

		if(it->second->getEdgeVertexTo() == NULL)
		{
			return false;
		}

		++it;
	}

	return true;
}

TeBox HidroGraph::getGrahBox()
{
	return _graphBox;
}