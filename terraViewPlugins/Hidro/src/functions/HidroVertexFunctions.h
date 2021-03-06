/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroVertexFunctions.h
*
*******************************************************************************
*
* $Rev: 7861 $:
*
* $Author: eric $:
*
* $Date: 2009-05-15 23:36:18 +0300 (pe, 15 touko 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Functions Group.
	@defgroup hidroFunctions The Group for Plugin Functions Classes.
*/

#ifndef HIDRO_VERTEXFUNCTIONS_H_
#define HIDRO_VERTEXFUNCTIONS_H_

/*
** ----------------------------------------------------------------------------
** Includes:
*/
#include <string>

class TeDatabase;
class TeLayer;

/** 
  * \file HidroVertexFunctions.h
  *
  * \class HidroFunctions
  *
  * \brief This file is class to manipulate a vertex from graph
  *
  * This class allow create attributes from vertex from layers
  * with raster or polygons representations.
  * The new attribute was insert into database as new column
  * into hidro vertex attribute table.
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroFunctions
 */

class HidroVertexFunctions
{

public:

	/** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor with parameters
	  * \param database		Input parameter - current database pointer from app
	*/
	HidroVertexFunctions(TeDatabase* database);

	/** \brief Virtual destructor.
	*/
	HidroVertexFunctions();

	//@}
	
	/** @name Behavioral Methods
		* Methods related to hidro graph persistance.
		*/
	//@{

public:
	
	/** \brief Function used to get the error message from this class
        \return		String with the error message, if was not error, a empty string will be returned
      */
	std::string getErrorMessage() const;

	bool createAttributeFromRaster(const std::string& graphName, TeLayer* layer, const std::string& attrName);

	bool createAttributeFromPolygons(const std::string& graphName, TeLayer* layer, const std::string& className, const std::string& attrName);

	//@}

protected:

	std::string		_errorMessage;		//!< Attribute used to inform about an error generated by internal functions

	TeDatabase*		_db;				//!< Attribute used to access current database from application
};


/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_VERTEXFUNCTIONS_H_