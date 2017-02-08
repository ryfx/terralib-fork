#include <HidroBoostConnectedComponents.h>

#include <HidroGraph.h>
#include <HidroEdge.h>
#include <HidroVertex.h>

#include <boost/graph/connected_components.hpp>

HidroBoostConnectedComponents::HidroBoostConnectedComponents()
{
}

HidroBoostConnectedComponents::~HidroBoostConnectedComponents()
{
}

std::string HidroBoostConnectedComponents::getBGLalgorithmDescription()
{
	std::string desc = "Connected Component of an undirected graph is a subgraph in"; 
				desc+= " which any two vertices are connected to each other by paths,";
				desc+= " and to which no more vertices or edges can be added while";
				desc+= " preserving its connectivity. That is, it is a maximal connected";
				desc+= " subgraph.";

	return desc;
}

bool HidroBoostConnectedComponents::checkInternalParameters()
{
	_errorMessage = "";

	if(!_internalParams._inputHidroGraph->isGraphValid())
	{
		_errorMessage = "Hidro Graph is invalid.";
		return false;
	}

	return true;
}

bool HidroBoostConnectedComponents::execute()
{
	_errorMessage = "";

	boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> adjList = createUndirectAdjacencyList(_internalParams._inputHidroGraph);

	int numVertices = num_vertices(adjList);

	std::vector<int> component(numVertices);

	_internalParams._returnIntValue = connected_components(adjList, &component[0]);

	return true;
}

boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> HidroBoostConnectedComponents::createUndirectAdjacencyList(HidroGraph* graph)
{
	std::map<std::string, HidroEdge*>::iterator it = graph->getGraphMapEdge().begin();

	boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> gBoost;

	while(it != graph->getGraphMapEdge().end())
	{
		HidroEdge* edge = it->second;

		int idxFrom = edge->getEdgeVertexFrom()->getVertexIdx();
		int idxTo = edge->getEdgeVertexTo()->getVertexIdx();

		boost::add_edge(idxFrom, idxTo, gBoost);
	
		++it;
	}

	return gBoost;
}
