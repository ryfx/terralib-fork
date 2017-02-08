#include <HidroBoostDijkstra.h>

#include <HidroBoostUtils.h>

#include <HidroEdge.h>
#include <HidroVertex.h>
#include <HidroAttrDouble.h>
#include <HidroAttrInt.h>

#include <boost/graph/adjacency_list.hpp>

HidroBoostDijkstra::HidroBoostDijkstra()
{
}

HidroBoostDijkstra::~HidroBoostDijkstra()
{
}

std::string HidroBoostDijkstra::getBGLalgorithmDescription()
{
	std::string desc = "Dijkstra's algorithm is a graph search algorithm that solves the single-source shortest";
				desc+= " path problem for a graph with nonnegative edge path costs, producing a shortest path tree.";
				desc+= " For a given source vertex (node) in the graph, the algorithm finds the path with lowest cost";
				desc+= " between that vertex and every other vertex. It can also be used for finding costs of shortest ";
				desc+= " paths from a single vertex to a single destination vertex by stopping the algorithm once the";
				desc+= " shortest path to the destination vertex has been determined.";

	return desc;
}

bool HidroBoostDijkstra::checkInternalParameters()
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

	if(_internalParams._edgeCostAttribute.empty())
	{
		_errorMessage = "Edge Cost attribute not defined.";
		return false;
	}

	if(_internalParams._startVertexId.empty())
	{
		_errorMessage = "Start vertex information not defined.";
		return false;
	}

	return true;
}

bool HidroBoostDijkstra::execute()
{
	_errorMessage = "";

	typedef boost::adjacency_list < boost::listS, boost::vecS, boost::directedS, boost::no_property, 
									boost::property < boost::edge_weight_t, int > > graph_t;

	typedef boost::graph_traits < graph_t >::vertex_descriptor vertex_descriptor;

	typedef boost::graph_traits < graph_t >::edge_descriptor edge_descriptor;

	typedef std::pair<int, int> bEdge;

	int num_nodes = _internalParams._inputHidroGraph->getGraphMapVertex().size();

	int num_edges = _internalParams._inputHidroGraph->getGraphMapEdge().size();

	std::vector<bEdge> edge_array(num_edges);

	std::vector<double> weights(num_edges);

	std::map<std::string, HidroEdge*>::iterator it = _internalParams._inputHidroGraph->getGraphMapEdge().begin();

	int i = 0;

	while(it != _internalParams._inputHidroGraph->getGraphMapEdge().end())
	{
		HidroEdge* edge = it->second;

		int startIdx = edge->getEdgeVertexFrom()->getVertexIdx();
		int endIdx	 = edge->getEdgeVertexTo()->getVertexIdx();

		edge_array[i] = bEdge(startIdx, endIdx);

		double costValue = 0.;
		
		HidroAttr* attr = edge->getEdgeAttr(_internalParams._edgeCostAttribute);

		if(!attr)
		{
			_errorMessage = "Error getting edge attribute.";
			return false;
		}

		if(attr->getAttrType() == TeINT)
		{
			HidroAttrInt* attrInt = (HidroAttrInt*)attr;
			costValue = (double)attrInt->getAttrValue();
		}
		else if(attr->getAttrType() == TeREAL)
		{
			HidroAttrDouble* attrDouble = (HidroAttrDouble*)attr;
			costValue = attrDouble->getAttrValue();
		}

		weights[i] = costValue;

		++i;

		++it;
	}

	graph_t g(num_nodes);

	boost::property_map<graph_t, boost::edge_weight_t>::type weightmap = get(boost::edge_weight, g);

	for(int i = 0; i < num_edges; ++i)
	{
		edge_descriptor e; 

		bool inserted;

		tie(e, inserted) = boost::add_edge(edge_array[i].first, edge_array[i].second, g);
		weightmap[e] = weights[i];
	}

	std::vector<vertex_descriptor> p(boost::num_vertices(g));
	std::vector<double> d(boost::num_vertices(g));

	int start, end;

	HidroVertex* startVertex = _internalParams._inputHidroGraph->getGraphVertex(_internalParams._startVertexId);

	if(!startVertex)
	{
		_errorMessage = "Start vertex not found.";
		return false;
	}

	start = startVertex->getVertexIdx();

	if(!_internalParams._endVertexId.empty())
	{
		HidroVertex* endVertex = _internalParams._inputHidroGraph->getGraphVertex(_internalParams._endVertexId);

		if(!endVertex)
		{
			_errorMessage = "Start vertex not found.";
			return false;
		}

		end = endVertex->getVertexIdx();
	}

	vertex_descriptor s = vertex(start, g);

	if(_internalParams._endVertexId.empty())
	{
		boost::property_map<graph_t, boost::vertex_index_t>::type indexmap = get(boost::vertex_index, g);

		try
		{
			dijkstra_shortest_paths(g, s, &p[0], &d[0], weightmap, indexmap, 
								std::less<double>(), boost::closed_plus<double>(), 
								(std::numeric_limits<double>::max)(), 0,
								boost::default_dijkstra_visitor());
		}
		catch(...)
		{
			_errorMessage = "Internal Error, Boost Algorithm.";
			return false;
		}
	}
	else 
	{
		vertex_descriptor e = vertex(end, g);

		try
		{
			dijkstra_shortest_paths(g, s, boost::predecessor_map(&p[0]).distance_map(&d[0]).visitor(target_visit(e, boost::on_examine_vertex()))); 
		}
		catch(...)
		{
			//end point found
		}
	}

	boost::graph_traits < graph_t >::edge_iterator ei, ei_end;

	int idx = 0;

	for(tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
	{
		boost::graph_traits < graph_t >::edge_descriptor e = *ei;
		boost::graph_traits < graph_t >::vertex_descriptor u = source(e, g);
		boost::graph_traits < graph_t >::vertex_descriptor v = target(e, g);

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