/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroVertexEdition.h
*
*******************************************************************************
*
* $Rev: 7813 $:
*
* $Author: eric $:
*
* $Date: 2009-04-24 17:53:15 +0300 (pe, 24 huhti 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Edition Group.
	@defgroup hidroEdition The Group for Plugin Edition Classes.
*/

#ifndef HIDRO_VERTEX_EDITION_H_
#define HIDRO_VERTEX_EDITION_H_

/** 
  * \file HidroVertexEdition.h
  *
  * \class HidroVertexEdition
  *
  * \brief This file is a class for graph vertex edition
  *
  * The edition operations defined for hidro vertex are:
  *		Add: Insert a new vertex in database (layer of points from defined graph)
  *          Its necessary define two coordenates (x, y), also needs to define the vertex attributes
  *
  *		Remove:	Remove a selected vertex from graph and database;
  *				Its necessary select a vertex. This vertex only will be deleted if none edge come or out
  *				from this vertex.
  *
  *	All operations even changed the theme collection.
  *
  * \sa HidroGraphEdition.h
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroEdition
 */


/*
** ----------------------------------------------------------------------------
** Includes:
*/

#include <string>
#include <vector>

class TeTheme;

class TeQtGrid;

class HidroVertexEdition
{

public:

	/** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor.
	  * \param theme	Input Parameter, attribute used to accees the layer of points from graph
	  * \param grid		Input Parameter, attribute used to access the grid from app
	*/
	HidroVertexEdition(TeTheme* theme, TeQtGrid* grid);

	/** \brief Virtual destructor.
	*/
	~HidroVertexEdition();

	//@}

public:

	/** @name Behavioral Methods
		* Methods related to hidro edge behavior.
		*/
	//@{

    /** \brief Function used to get the error messagem from vertex edition
        \return		Return the string with the error message from vertex edition operation
      */
	std::string getErrorMessage() const;

	/** \brief Function used to add a new vertex in graph database
	  * \param xCoord		Input parameter, attribute used to define the x coord from new vertex
	  * \param yCoord		Input parameter, attribute used to define the y coord from new vertex
	  * \param attrs		Input parameter, vector attribute with the vertex attributes 
	  * \param name			Input parameter, attribute used to define the vertex name
      * \return				Return true if the new vertex was added correctly and false in other case
      */
	bool addVertex(const double& xCoord, const double& yCoord, const std::vector<std::string>& attrs, const std::string& name);

	/** \brief Function used to remove a vertex from graph database
	  * \param vertexId		Input parameter, attribute used to define the vertex selected
	  * \param edgeTable	Input parameter, attribute used to indicate edge table name
      * \return				Return true if the vertex was removed correctly and false in other case
      */
	bool removeVertex(const std::string& vertexId, const std::string& edgeTable);

protected:

	TeTheme*		_theme;				//!< Attribute used to define the theme with points from graph

	TeQtGrid*		_grid;				//!< Attribute used to define the grid from tv apps

	std::string		_errorMessage;		//!< Attribute used to define a error message in vertex edition
};

/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_VERTEX_EDITION_H_