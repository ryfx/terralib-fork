#include <HidroBoostStronglyConnectedComponents.h>

#include <HidroGraph.h>
#include <HidroEdge.h>
#include <HidroVertex.h>

#include <boost/graph/strong_components.hpp>

HidroBoostStronglyConnectedComponents::HidroBoostStronglyConnectedComponents()
{
}

HidroBoostStronglyConnectedComponents::~HidroBoostStronglyConnectedComponents()
{
}

std::string HidroBoostStronglyConnectedComponents::getBGLalgorithmDescription()
{
	std::string desc = "A directed graph is called strongly connected if there is a path";
				desc+= " from each vertex in the graph to every other vertex. The strongly";
				desc+= " connected components (SCC) of a directed graph are its maximal";
				desc+= " strongly connected subgraphs. If each strongly connected component";
				desc+= " is contracted to a single vertex, the resulting graph is a directed acyclic graph.";

	return desc;
}

bool HidroBoostStronglyConnectedComponents::checkInternalParameters()
{
	_errorMessage = "";

	if(!_internalParams._inputHidroGraph->isGraphValid())
	{
		_errorMessage = "Hidro Graph is invalid.";
		return false;
	}

	return true;
}

bool HidroBoostStronglyConnectedComponents::execute()
{
	_errorMessage = "";

	boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> adjList = createDirectAdjacencyList(_internalParams._inputHidroGraph);

	int numVertices = num_vertices(adjList);

	std::vector<int> component(numVertices);

	_internalParams._returnIntValue = 
		strong_components(adjList, boost::make_iterator_property_map(component.begin(), get(boost::vertex_index, adjList), component[0]));

	return true;
}

boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> HidroBoostStronglyConnectedComponents::createDirectAdjacencyList(HidroGraph* graph)
{
	std::map<std::string, HidroEdge*>::iterator it = graph->getGraphMapEdge().begin();

	boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> gBoost;

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