/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroEdgeFunctions.h
*
*******************************************************************************
*
* $Rev: 7857 $:
*
* $Author: eric $:
*
* $Date: 2009-05-14 17:39:02 +0300 (to, 14 touko 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Functions Group.
	@defgroup hidroFunctions The Group for Plugin Functions Classes.
*/

#ifndef HIDRO_EDGEFUNCTIONS_H_
#define HIDRO_EDGEFUNCTIONS_H_

/*
** ----------------------------------------------------------------------------
** Includes:
*/
#include <string>

class TeDatabase;

/** 
  * \file HidroEdgeFunctions.h
  *
  * \class HidroFunctions
  *
  * \brief This file is class to manipulate a edge from graph
  *
  * This class allow create attributes from edges, such as,
  * distance...
  * The new attribute was insert into database as new column
  * into hidro edge attribute table.
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroFunctions
 */

class HidroEdgeFunctions
{

public:

	/** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor with parameters
	  * \param database		Input parameter - current database pointer from app
	*/
	HidroEdgeFunctions(TeDatabase* database);

	/** \brief Virtual destructor.
	*/
	HidroEdgeFunctions();

	//@}
	
	/** @name Behavioral Methods
		* Methods related to hidro graph persistance.
		*/
	//@{

	bool createDistanceAttribute(const std::string& graphName, const std::string& attrName);

public:
	
	/** \brief Function used to get the error message from this class
        \return		String with the error message, if was not error, a empty string will be returned
      */
	std::string getErrorMessage() const;

	//@}

protected:

	std::string		_errorMessage;		//!< Attribute used to inform about an error generated by internal functions

	TeDatabase*		_db;				//!< Attribute used to access current database from application
};


/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_EDGEFUNCTIONS_H_