#ifndef HIDRO_BOOST_CONNECTEDCOMPONENTS_FACTORY_H
#define HIDRO_BOOST_CONNECTEDCOMPONENTS_FACTORY_H

#include <HidroBoostAlgorithmFactory.h>

class HidroBoostConnectedComponentsFactory : public HidroBoostAlgorithmFactory
{
public :

	/**
	* @brief Default constructor
	*/
	HidroBoostConnectedComponentsFactory();      

	/**
	* @brief Default Destructor
	*/
	~HidroBoostConnectedComponentsFactory();

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
	static HidroBoostConnectedComponentsFactory HidroBoostConnectedComponentsFactory_instance;
};

#endif //HIDRO_BOOST_CONNECTEDCOMPONENTS_FACTORY_H
