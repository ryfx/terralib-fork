#include <HidroBoostBreadthFirstSearch.h>

#include <HidroBoostUtils.h>

#include <HidroEdge.h>
#include <HidroVertex.h>
#include <HidroAttrDouble.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>

HidroBoostBreadthFirstSearch::HidroBoostBreadthFirstSearch()
{
}

HidroBoostBreadthFirstSearch::~HidroBoostBreadthFirstSearch()
{
}

std::string HidroBoostBreadthFirstSearch::getBGLalgorithmDescription()
{
	std::string desc = "Breadth-First Search (BFS) is a graph search algorithm that begins";
				desc+= " at the root node and explores all the neighboring nodes.";
				desc+= " BFS is an uninformed search method that aims to expand and examine";
				desc+= " all nodes of a graph or combinations of sequence by systematically";
				desc+= " searching through every solution. In other words, it exhaustively";
				desc+= " searches the entire graph or sequence without considering the goal until it finds it.\n";
				desc+= "Breadth-first search can be used to solve many problems in graph theory, for example:\n";
				desc+= "\t* Finding all connected components in a graph.\n";
				desc+= "\t* Finding all nodes within one connected component.\n";
				desc+= "\t* Finding the shortest path between two nodes u and v (in an unweighted graph).\n";
				desc+= "\t* Testing a graph for bipartiteness.\n";

	return desc;
}

bool HidroBoostBreadthFirstSearch::checkInternalParameters()
{
	_errorMessage = "";

	if(!_internalParams._inputHidroGraph->isGraphValid())
	{
		_errorMessage = "Hidro Graph is invalid.";
		return false;
	}

	if(!_internalParams._outputHidroGraph)
	{
		_errorMessage = "Output Hidro Graph is invalid.";
		return false;
	}

	if(_internalParams._startVertexId.empty())
	{
		_errorMessage = "Start vertex information not defined.";
		return false;
	}

	return true;
}

bool HidroBoostBreadthFirstSearch::execute()
{
	_errorMessage = "";

	typedef boost::adjacency_list<	boost::mapS, boost::vecS, boost::directedS,
									boost::property<boost::vertex_color_t, boost::default_color_type, 
										boost::property<boost::vertex_degree_t, int,
											boost::property<boost::vertex_in_degree_t, int,
												boost::property<boost::vertex_out_degree_t, int> > > > 
								 > Graph;

	typedef boost::graph_traits < Graph >::vertex_descriptor vertex_descriptor;

	int numVertices = _internalParams._inputHidroGraph->getGraphMapVertex().size();
  
	Graph G(numVertices);

	std::map<std::string, HidroEdge*>::iterator it = _internalParams._inputHidroGraph->getGraphMapEdge().begin();

	while(it != _internalParams._inputHidroGraph->getGraphMapEdge().end())
	{
		HidroEdge* edge = it->second;

		int idxFrom = edge->getEdgeVertexFrom()->getVertexIdx();
		int idxTo = edge->getEdgeVertexTo()->getVertexIdx();

		boost::add_edge(idxFrom, idxTo, G);
	
		++it;
	}

	HidroVertex* startVertex = _internalParams._inputHidroGraph->getGraphVertex(_internalParams._startVertexId);

	if(!startVertex)
	{
		_errorMessage = "Start vertex not found.";
		return false;
	}

	int start = startVertex->getVertexIdx();
	std::vector<vertex_descriptor> p(boost::num_vertices(G));
	p[vertex(start, G)] = vertex(start, G);

	boost::breadth_first_search(G, vertex(start, G), boost::visitor(
		boost::make_bfs_visitor(boost::record_predecessors(&p[0], boost::on_tree_edge()))));

	boost::graph_traits < Graph >::edge_iterator ei, ei_end;

	int idx = 0;

	for(tie(ei, ei_end) = edges(G); ei != ei_end; ++ei)
	{
		boost::graph_traits < Graph >::edge_descriptor e = *ei;
		boost::graph_traits < Graph >::vertex_descriptor u = source(e, G);
		boost::graph_traits < Graph >::vertex_descriptor v = target(e, G);

		if (p[v] == u)
		{
			if(!createGraphComponent(_internalParams._inputHidroGraph, _internalParams._outputHidroGraph, u, v, idx))
			{
				_errorMessage = "Error creating output graph component.";
				return false;
			}
		}
	}

	return true;
}