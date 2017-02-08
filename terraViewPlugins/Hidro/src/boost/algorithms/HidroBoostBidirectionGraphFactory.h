#ifndef HIDRO_BOOST_BIDIRECTIONALGRAPH_FACTORY_H
#define HIDRO_BOOST_BIDIRECTIONALGRAPH_FACTORY_H

#include <HidroBoostAlgorithmFactory.h>

class HidroBoostBidirectionalGraphFactory : public HidroBoostAlgorithmFactory
{
public :

	/**
	* @brief Default constructor
	*/
	HidroBoostBidirectionalGraphFactory();      

	/**
	* @brief Default Destructor
	*/
	~HidroBoostBidirectionalGraphFactory();

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
	static HidroBoostBidirectionalGraphFactory HidroBoostBidirectionalGraphFactory_instance;
};

#endif //HIDRO_BOOST_BIDIRECTIONALGRAPH_FACTORY_H
