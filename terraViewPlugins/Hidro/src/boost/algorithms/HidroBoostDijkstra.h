#ifndef HIDRO_BOOST_DIJKSTRA_H
#define HIDRO_BOOST_DIJKSTRA_H

#include <HidroBoostAlgorithms.h>

#include <boost/graph/dijkstra_shortest_paths.hpp>

template <class Vertex, class Tag>
struct target_visitor : public boost::default_dijkstra_visitor
{
	target_visitor(Vertex u) : v(u) 
	{ 
	}

	template <class Graph> void examine_vertex(Vertex u, Graph& g)
	{
		if( u == v ) 
		{
			throw(-1);
		}
	}

private:

	Vertex v;
};

template <class Vertex, class Tag> target_visitor<Vertex, Tag> target_visit(Vertex u, Tag) 
{
	return target_visitor<Vertex, Tag>(u);
}

class HidroBoostDijkstra : public HidroBoostAlgorithms
{
public:
	/**
	* @brief Default Constructor.
	*/
	HidroBoostDijkstra();

	/**
	* @brief Default Destructor
	*/
	~HidroBoostDijkstra();

	/**
	* @brief Function used to get description from current algorithm selected
	*
	*
	* @return String with BGL algorithm description.
	*/
	virtual std::string getBGLalgorithmDescription();

protected:

	/**
	* @brief Function used to check internal parameters..
	*
	*
	* @return True if parameters is correctly and false in other case.
	*/
	virtual bool checkInternalParameters();

	/**
	* @brief Function used to measure objects, pure virtual function implementation.
	*
	*
	* @return True if the measure is correctly and false in other case.
	*/
	virtual bool execute();
};

#endif //HIDRO_BOOST_DIJKSTRA_H
