#ifndef HIDRO_BOOST_DIJKSTRA_FACTORY_H
#define HIDRO_BOOST_DIJKSTRA_FACTORY_H

#include <HidroBoostAlgorithmFactory.h>

class HidroBoostDijkstraFactory : public HidroBoostAlgorithmFactory
{
public :

	/**
	* @brief Default constructor
	*/
	HidroBoostDijkstraFactory();      

	/**
	* @brief Default Destructor
	*/
	~HidroBoostDijkstraFactory();

protected :  

	/**
	* @brief Implementation for the abstract TeFactory::build.
	*
	* @param arg A const reference to the used parameters.
	* @return A pointer to the new generated instance.
	*/
	HidroBoostAlgorithms* build( const HidroBoostParams& arg );

};

namespace
{  
	/** @brief A concrete factory instance for registering into the factory dictionary */
	static HidroBoostDijkstraFactory HidroBoostDijkstraFactory_instance;
};

#endif //HIDRO_BOOST_DIJKSTRA_FACTORY_H
