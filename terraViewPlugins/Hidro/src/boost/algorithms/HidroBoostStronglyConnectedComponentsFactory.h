#ifndef HIDRO_BOOST_STRONGLYCONNECTEDCOMPONENTS_FACTORY_H
#define HIDRO_BOOST_STRONGLYCONNECTEDCOMPONENTS_FACTORY_H

#include <HidroBoostAlgorithmFactory.h>

class HidroBoostStronglyConnectedComponentsFactory : public HidroBoostAlgorithmFactory
{
public :

	/**
	* @brief Default constructor
	*/
	HidroBoostStronglyConnectedComponentsFactory();      

	/**
	* @brief Default Destructor
	*/
	~HidroBoostStronglyConnectedComponentsFactory();

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
	static HidroBoostStronglyConnectedComponentsFactory HidroBoostStronglyConnectedComponentsFactory_instance;
};

#endif //HIDRO_BOOST_STRONGLYCONNECTEDCOMPONENTS_FACTORY_H
