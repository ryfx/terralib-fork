#include <HidroBoostIncidenceGraph.h>

#include <HidroBoostUtils.h>

#include <HidroEdge.h>
#include <HidroVertex.h>

#include <stack>
#include <set>

#include <boost/graph/adjacency_list.hpp>

HidroBoostIncidenceGraph::HidroBoostIncidenceGraph()
{
}

HidroBoostIncidenceGraph::~HidroBoostIncidenceGraph()
{
}

std::string HidroBoostIncidenceGraph::getBGLalgorithmDescription()
{
	std::string desc = "The IncidenceGraph concept provides an interface for efficient access to the out-edges of each vertex in the graph";

	return desc;
}

bool HidroBoostIncidenceGraph::checkInternalParameters()
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

bool HidroBoostIncidenceGraph::execute()
{
	_errorMessage = "";

	std::map<std::string, HidroEdge*>::iterator it = _internalParams._inputHidroGraph->getGraphMapEdge().begin();

	typedef boost::adjacency_list
	< 
		boost::listS,											// Store out_in_edges of each vertex in a std::list
		boost::vecS,											// Store vertex set in a std::vector
		boost::directedS,										// The graph is directed
		boost::property < boost::vertex_name_t, std::string >	// Add a vertex property
	>graph_type;

	typedef boost::graph_traits < graph_type >::vertex_descriptor vertex_descriptor;

	graph_type g;

	while(it != _internalParams._inputHidroGraph->getGraphMapEdge().end())
	{
		HidroEdge* edge = it->second;

		int idxFrom = edge->getEdgeVertexFrom()->getVertexIdx();
		int idxTo = edge->getEdgeVertexTo()->getVertexIdx();

		boost::add_edge(idxFrom, idxTo, g);
	
		++it;
	}

	boost::graph_traits < graph_type >::vertex_iterator i, end;
	tie(i, end) = boost::vertices(g);

	HidroVertex* startVertex = _internalParams._inputHidroGraph->getGraphVertex(_internalParams._startVertexId);

	if(!startVertex)
	{
		_errorMessage = "Start vertex not found.";
		return false;
	}

	int startV = startVertex->getVertexIdx();

	vertex_descriptor e = vertex(startV, g);

	int idx = 0;

	std::stack<vertex_descriptor> stack;
	std::set<int> vertexSet;

	stack.push(e);

	vertexSet.insert(e);

	while(!stack.empty())
	{
		vertex_descriptor vertexDesc = stack.top();
		stack.pop();

		boost::graph_traits < graph_type >::out_edge_iterator ei, ei_end;

		for (tie(ei, ei_end) = out_edges(vertexDesc, g); ei != ei_end; ++ei)
		{
			boost::graph_traits < graph_type >::edge_descriptor e = *ei;
			boost::graph_traits < graph_type >::vertex_descriptor u = source(e, g);
			boost::graph_traits < graph_type >::vertex_descriptor v = target(e, g);

			std::set<int>::iterator it = vertexSet.find(v);

			if(it == vertexSet.end())
			{
				stack.push(v);
			}

			if(!createGraphComponent(_internalParams._inputHidroGraph, _internalParams._outputHidroGraph, u, v, idx))
			{
				_errorMessage = "Error creating output graph component.";
				return false;
			}
		}
	}

	vertexSet.clear();

	return true;
}