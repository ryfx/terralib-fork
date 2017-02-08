#ifndef HIDRO_BOOST_STRONGLYCONNECTEDCOMPONENTS_H
#define HIDRO_BOOST_STRONGLYCONNECTEDCOMPONENTS_H

#include <HidroBoostAlgorithms.h>

#include <boost/graph/adjacency_list.hpp>

class HidroBoostStronglyConnectedComponents : public HidroBoostAlgorithms
{
public:
	/**
	* @brief Default Constructor.
	*/
	HidroBoostStronglyConnectedComponents();

	/**
	* @brief Default Destructor
	*/
	~HidroBoostStronglyConnectedComponents();

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

	boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> createDirectAdjacencyList(HidroGraph* graph);
};

#endif //HIDRO_BOOST_STRONGLYCONNECTEDCOMPONENTS_H
