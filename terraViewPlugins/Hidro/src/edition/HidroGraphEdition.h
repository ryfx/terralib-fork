/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroGraphEdition.h
*
******************************************************************************
*
* $Rev: 7694 $:
*
* $Author: eric $:
*
* $Date: 2009-03-23 14:37:58 +0200 (ma, 23 maalis 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Edition Group.
	@defgroup hidroEdition The Group for Plugin Edition Classes.
*/

#ifndef HIDRO_GRAPH_EDITION_H_
#define HIDRO_GRAPH_EDITION_H_

/** 
  * \file HidroGraphEdition.h
  *
  * \class HidroGraphEdition
  *
  * \brief This file is a class for graph edition, works like a util class for graph edition
  *
  * This class manipulates the collections from graph themes, add, change and removes 
  * collections entry for graph operations.
  *
  * Also implements auxiliar functions to help vertex and edge editions.
  *
  * \sa HidroEdgeEdition.h, HidroVertexEdition.h
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

#include <TeGeometry.h>

class TeTheme;
class TeCoord2D;

class TeQtGrid;

class HidroGraphEdition
{

public:

	/** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor.
	  * \param theme	Input Parameter, attribute used to accees the layer from graph
	*/
	HidroGraphEdition(TeTheme* theme);

	/** \brief Virtual destructor.
	*/
	~HidroGraphEdition();

	//@}

public:

	/** @name Behavioral Methods
		* Methods related to hidro edge behavior.
		*/
	//@{

    /** \brief Function used to get the error messagem from graph edition
        \return		Return the string with the error message from graph edition operation
      */
	std::string getErrorMessage() const;

	/** \brief Function used to insert a new entry in collection table
	  * \param				Input parameter, attribute coord used to define the label coordenate
	  * \param				Input parameter, attribute used to define the object identification
	  * \param				Input parameter, attribute used to access the grid application
      * \return				Return True if the collection entry was added correctly and false in other case
      */
	bool insertCollectionEntry(const TeCoord2D& coord, const std::string& objId, TeQtGrid* grid);

	/** \brief Function used to change a collection entry
	  * \param				Input parameter, attribute coord used to define the label coordenate
	  * \param				Input parameter, attribute used to define the object identification
      * \return				Return True if the collection entry was changed correctly and false in other case
      */
	bool changeCollectionEntry(const TeCoord2D& coord, const std::string& objId);

	/** \brief Function used to remove a collection entry
	  * \param				Input parameter, attribute used to define the object identification
	  * \param				Input parameter, attribute used to access the grid application
      * \return				Return True if the collection entry was removed correctly and false in other case
      */
	bool revemoCollectionEntry(const std::string& objId, TeQtGrid* grid);
	
	/** \brief Function used to get all edges that come or out from a defined vertex
	  * \param				Input parameter, attribute used to define a vertex identification
	  * \param				Input parameter, attribute used to inform the edge attribute table
      * \return				Return String vector with all edges ids finded
      */
	std::vector<std::string> getEdgesFromVertex(const std::string& vertexId, const std::string& tableEdgeName);

	/** \brief Function used to get a vertex coordenate
	  * \param				Input parameter, attribute that define the vertex identifaction
      * \return				Return terralib attribute that defines the vertex geometry
      */
	TeCoord2D getVertexCoord(const std::string& vertexId);

	/** \brief Function used to get a edge line
	  * \param				Input parameter, attribute that define the edge identifaction
      * \return				Return terralib attribute that defines the edge geometry
      */
	TeLine2D getEdgeLine(const std::string& edgeId);

	/** \brief Function used to get the object identification from a vertex
	  * \param				Input parameter, attribute that defines the vertex identification
      * \return				Return a string with the vertex object identification
      */
	std::string getObjIdFromVertex(const std::string& vertexName);

	/** \brief Function used to get the object identification from a edge
	  * \param				Input parameter, attribute that defines the edge identification
      * \return				Return a string with the edge object identification
      */
	std::string getObjIdFromEdge(const std::string& edgeName);

	/** \brief Function used to get a new object id for a new vertex, this function executes the
	  *        sql to get the max value from geom_id and defines that value to a new object id entry
      * \return		Return a string with a object id
      */
	std::string getVertexNewObjId();

	/** \brief Function used to get a new object id for a edge vertex, this function executes the
	  *        sql to get the max value from geom_id and defines that value to a new object id entry
      * \return		Return a string with a object id
      */
	std::string getEdgeNewObjId();

protected:

	TeTheme*		_theme;				//!< Attribute used to define the theme from graph

	std::string		_errorMessage;		//!< Attribute used to define a error message in vertex edition
};


/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_GRAPH_EDITION_H_