#include <HidroBoostBidirectionGraph.h>

#include <HidroBoostUtils.h>

#include <HidroEdge.h>
#include <HidroVertex.h>

#include <stack>

#include <boost/graph/adjacency_list.hpp>

HidroBoostBidirectionalGraph::HidroBoostBidirectionalGraph()
{
}

HidroBoostBidirectionalGraph::~HidroBoostBidirectionalGraph()
{
}

std::string HidroBoostBidirectionalGraph::getBGLalgorithmDescription()
{
	std::string desc = "The BidirectionalGraph concept refines IncidenceGraph and adds the requirement";
				desc+= " for efficient access to the in-edges of each vertex. This concept is separated";
				desc+= " from IncidenceGraph because for directed graphs efficient access to in-edges";
				desc+= " typically requires more storage space, and many algorithms do not require access";
				desc+= " to in-edges. For undirected graphs this is not an issue, since the in_edges()";
				desc+= " and out_edges() functions are the same, they both return the edges incident to the vertex.";

	return desc;
}

bool HidroBoostBidirectionalGraph::checkInternalParameters()
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

	if(_internalParams._endVertexId.empty())
	{
		_errorMessage = "End vertex information not defined.";
		return false;
	}


	return true;
}

bool HidroBoostBidirectionalGraph::execute()
{
	_errorMessage = "";

	std::map<std::string, HidroEdge*>::iterator it = _internalParams._inputHidroGraph->getGraphMapEdge().begin();

	typedef boost::adjacency_list
	< 
		boost::listS,											// Store out_in_edges of each vertex in a std::list
		boost::vecS,											// Store vertex set in a std::vector
		boost::bidirectionalS,									// The graph is directed, with both out-edges and in-edges
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

	HidroVertex* endVertex = _internalParams._inputHidroGraph->getGraphVertex(_internalParams._endVertexId);

	if(!endVertex)
	{
		_errorMessage = "End vertex not found.";
		return false;
	}

	int endV = endVertex->getVertexIdx();

	vertex_descriptor e = vertex(endV, g);

	int idx = 0;

	std::stack<vertex_descriptor> stack;

	stack.push(e);

	while(!stack.empty())
	{
		vertex_descriptor vertexDesc = stack.top();
		stack.pop();

		boost::graph_traits < graph_type >::in_edge_iterator ei, ei_end;

		for (tie(ei, ei_end) = in_edges(vertexDesc, g); ei != ei_end; ++ei)
		{
			boost::graph_traits < graph_type >::edge_descriptor e = *ei;
			boost::graph_traits < graph_type >::vertex_descriptor u = source(e, g);
			boost::graph_traits < graph_type >::vertex_descriptor v = target(e, g);

			stack.push(u);

			if(!createGraphComponent(_internalParams._inputHidroGraph, _internalParams._outputHidroGraph, u, v, idx))
			{
				_errorMessage = "Error creating output graph component.";
				return false;
			}
		}
	}

	return true;
}