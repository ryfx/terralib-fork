/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroBoostAlgorithms.h
*
*******************************************************************************
*
* $Rev: 7712 $:
*
* $Author: eric $:
*
* $Date: 2009-03-27 03:52:16 +0200 (pe, 27 maalis 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Boost Group.
	@defgroup hidroBoost The Group for Boost Algorithms.
*/

#ifndef HIDRO_BOOST_ALGORITHMS_H
#define HIDRO_BOOST_ALGORITHMS_H

/** 
  * \file HidroBoostAlgorithms.h
  *
  * \class HidroBoostAlgorithms
  *
  * \brief This file is a base class to define a graph algorithm
  *
  * To execute any algorithm from boost in necessary extend this class.
  * This class is a base class for any boost operation. It's necessary
  * define any parameters used in operation using HidroBoostParams.
  *
  * \sa HidroBoostParams.h
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

#include <HidroBoostParams.h>

class HidroBoostAlgorithms
{
public:
	/**
	* @brief Default Destructor
	*/
	virtual ~HidroBoostAlgorithms();

	/**
	* @brief Reset the current algorithm following the new supplied 
	* parameters.
	*
	* @param newParams The new parameters.
	* @return true if OK, false on errors.
	*/      
	bool reset( const HidroBoostParams& newParams );

	/**
	* @brief Returns a copy of the current internal algorithm parameters.
	*
	* @param params The internal parameters copy.
	*/        
	void getParameters( HidroBoostParams& params ) const;

	/**
	* @brief Returns a default object.
	*
	* @return A default object.
	*/
	static HidroBoostAlgorithms* DefaultObject(const HidroBoostParams&)
	{
		return 0;
	}

	/**
	* @brief Function used to execute the operation
	*
	*
	* @return True the operation is successfuly and false in other case.
	*/
	bool executeAlgorithm();

	/**
	* @brief Function used to get description from current algorithm selected
	*
	*
	* @return String with BGL algorithm description.
	*/
	virtual std::string getBGLalgorithmDescription() = 0;

protected:
	/**
	* @brief Default Constructor.
	*/
	HidroBoostAlgorithms();


	/**
	* @brief Function used to check parameters.
	*
	*
	* @return True if parameters is correctly and false in other case.
	*/
	virtual bool checkParameters();

	/**
	* @brief Function used to check internal parameters..
	*
	*
	* @return True if parameters is correctly and false in other case.
	*/
	virtual bool checkInternalParameters() = 0;

	/**
	* @brief Function used to execute boost algorithm, pure virtual function.
	*
	*
	* @return True if the execution is correctly and false in other case.
	*/
	virtual bool execute() = 0;

private:
	/**
	* @brief Operator = overload.
	*
	* @param external External instance reference.
	*
	* @return The external instance reference.
	*/        
	const HidroBoostAlgorithms& operator=(const HidroBoostAlgorithms&)
	{ 
		return *this; 
	};

protected:
	/**
	* @brief The current internal parameters.
	*/
	HidroBoostParams _internalParams;

public:
	/**
	* @brief Attribute used to inform about an error in operation.
	*/
	std::string _errorMessage;
};

/*
** ---------------------------------------------------------------
** End:
*/

#endif //HIDRO_BOOST_ALGORITHMS_H
