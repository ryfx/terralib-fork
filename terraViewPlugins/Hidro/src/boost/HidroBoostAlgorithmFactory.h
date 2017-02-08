/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroBoostAlgorithmFactory.h
*
*******************************************************************************
*
* $Rev: 7786 $:
*
* $Author: eric $:
*
* $Date: 2009-04-14 19:41:27 +0300 (ti, 14 huhti 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Boost Group.
	@defgroup hidroBoost The Group for Boost Algorithms.
*/


#ifndef HIDRO_BOOST_FACTORIES_REGISTERED
#define HIDRO_BOOST_FACTORIES_REGISTERED

#include <HidroBoostConnectedComponentsFactory.h>
#include <HidroBoostStronglyConnectedComponentsFactory.h>
#include <HidroBoostDijkstraFactory.h>
#include <HidroBoostBreadthFirstSearchFactory.h>
#include <HidroBoostBidirectionGraphFactory.h>

#endif //HIDRO_BOOST_FACTORIES_REGISTERED

#ifndef HIDRO_BOOST_ALGORITHM_FACTORY_H
#define HIDRO_BOOST_ALGORITHM_FACTORY_H

/** 
  * \file HidroBoostAlgorithmFactory.h
  *
  * \class HidroBoostAlgorithmFactory
  *
  * \brief This file is a base class to define a graph boost algorithm factory
  *
  * A factory class for boost algorithms.
  *
  * \sa HidroBoost_Algorithm.h
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroBoost
 */

/*
** ----------------------------------------------------------------------------
** Includes:
*/

#include <HidroBoostAlgorithms.h>
#include <HidroBoostParams.h>

#include <TeFactory.h>

class HidroBoostAlgorithmFactory : public TeFactory< HidroBoostAlgorithms, HidroBoostParams >
{
public:
	/**
	* Default Destructor
	*/
	virtual ~HidroBoostAlgorithmFactory();

protected:
	/**
	* Default constructor
	*
	* @param factoryName Factory name.
	*/
	HidroBoostAlgorithmFactory( const std::string& factoryName );
};

/*
** ---------------------------------------------------------------
** End:
*/

#endif //HIDRO_BOOST_ALGORITHM_FACTORY_H
