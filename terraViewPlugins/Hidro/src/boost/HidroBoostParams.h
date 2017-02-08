/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroBoostParams.h
*
*******************************************************************************
*
* $Rev: 7694 $:
*
* $Author: eric $:
*
* $Date: 2009-03-23 14:37:58 +0200 (ma, 23 maalis 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Boost Group.
	@defgroup hidroBoost The Group for Boost Algorithms.
*/

#ifndef HIDRO_BOOST_PARAMS_H
#define HIDRO_BOOST_PARAMS_H

/** 
  * \file HidroBoostParams.h
  *
  * \class HidroBoostParams
  *
  * \brief This file is a base class to define a graph boost params
  *
  * To execute any algorithm from boost is necessary
  * define any parameters used in operation using this class.
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

#include <string>

#include <HidroGraph.h>

class HidroBoostParams
{
public:

	/**
	* @brief Boost Algorithm Name.
	*
	* @note Default value = "".
	*/
	std::string _boostAlgorithmName;

	/** 
	* @brief Input Hidro Graph Struct used to modeling the graph in terraView application, algorithm parameter
	*
	* @note Default value = NULL.    
	*/
	HidroGraph* _inputHidroGraph;

	/** 
	* @brief Output Hidro Graph Struct used to modeling the graph in terraView application, algorithm result
	*
	* @note Default value = NULL.    
	*/
	HidroGraph* _outputHidroGraph;

	/** 
	* @brief String attribute used to define the attribute used for edge cost in boost algorithm
	*
	* @note Default value = empty.    
	*/
	std::string _edgeCostAttribute;

	/** 
	* @brief String attribute used to define the start vertex used in boost algorithm
	*
	* @note Default value = empty.    
	*/
	std::string _startVertexId;

	/** 
	* @brief String attribute used to define the end vertex used in boost algorithm
	*
	* @note Default value = empty.    
	*/
	std::string _endVertexId;

	/** 
	* @brief int attribute used to define a integer parameter
	*
	* @note Default value = empty.    
	*/
	int _returnIntValue;

	/**
	* @brief Default constructor
	*/
	HidroBoostParams();

	/**
	* @brief Alternative constructor
	* @param external External reference.
	*/
	HidroBoostParams( const HidroBoostParams& external );      

	/**
	* @brief Default Destructor
	*/
	~HidroBoostParams();

	/**
	* @brief operator= implementation.
	*
	* @param external External reference.
	* @return A const reference to the external object instance.
	*/
	const HidroBoostParams& operator=(const HidroBoostParams& external );

	/**
	* @brief This is for TeFactory compatibility.
	*/
	std::string decName() const;

	/**
	* @brief Reset to the default parameters.
	*/
	void reset();
};

/*
** ---------------------------------------------------------------
** End:
*/

#endif //HIDRO_BOOST_PARAMS_H
