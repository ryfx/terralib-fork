/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroEdgeEdition.h
*
*******************************************************************************
*
* $Rev: 7900 $:
*
* $Author: eric $:
*
* $Date: 2009-06-10 16:05:59 +0300 (ke, 10 kesä 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Edition Group.
	@defgroup hidroEdition The Group for Plugin Edition Classes.
*/

#ifndef HIDRO_EDGE_EDITION_H_
#define HIDRO_EDGE_EDITION_H_

/** 
  * \file HidroEdgeEdition.h
  *
  * \class HidroEdgeEdition
  *
  * \brief This file is a class for graph edge edition
  *
  * The edition operations defined for hidro edge are:
  *		Add: Insert a new edge in database (layer of lines from defined graph)
  *          Its necessary define two vertex, also needs to define the edge attributes
  *
  *		Change: Change a vertex from a selected edge.
  *				Its necessary selec an edge, a new vertex and indicate if the vertex changed will
  *				be the vertex from or vertex to from selected edge.
  *
  *		Remove:	Remove a selected edge from graph and database;
  *				Its necessary select a edge.
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

class HidroEdgeEdition
{
public:

	/** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor.
	  * \param edgeTheme	Input Parameter, attribute used to access the layer of lines from graph
	  * \param vertexTheme	Input Parameter, attribute used to accees the layer of points from graph
	  * \param grid			Input Parameter, attribute used to access the grid from app
	*/
	HidroEdgeEdition(TeTheme* edgeTheme, TeTheme* vertexTheme, TeQtGrid* grid);

	/** \brief Virtual destructor.
	*/
	~HidroEdgeEdition();

	//@}

public:

	/** @name Behavioral Methods
		* Methods related to hidro edge behavior.
		*/
	//@{

    /** \brief Function used to get the error messagem from edge edition
        \return		Return the string with the error message from edge edition operation
      */
	std::string getErrorMessage() const;

	/** \brief Function used to add a new edge in graph database
	  * \param vertexFromId		Input parameter, attribute used to define the origin vertex from new edge
	  * \param vertexToId		Input parameter, attribute used to define the destiny vertex from new edge
	  * \param attrs			Input parameter, vector attribute with the edges attributes 
	  * \param name				Input parameter, attribute used to define the edge name
      * \return					Return true if the new edge was added correctly and false in other case
      */
	bool addEdge(const std::string& vertexFromId, const std::string& vertexToId, const std::vector<std::string>& attrs, const std::string& name);

	/** \brief Function used to change a vertex from a existing edge in graph database
	  * \param edgeId				Input parameter, attribute used to define the edge changed
	  * \param vertexId				Input parameter, attribute used to define the new vertex from edge
	  * \param changeVertexFrom		Input parameter, attribute used to inform if the vertex changed will be from or to
      * \return						Return true if the edge was changed correctly and false in other case
      */
	bool changeEdge(const std::string& edgeId, const std::string& vertexId, const bool& changeVertexFrom);

	/** \brief Function used to remove a edge from graph database
	  * \param edgeId		Input parameter, attribute used to define the edge selected
      * \return				Return true if the edge was removed correctly and false in other case
      */
	bool removeEdge(const std::string& edgeId);

	//@}

protected:

	TeTheme*		_edgeTheme;			//!< Attribute used to define the theme with lines from graph

	TeTheme*		_vertexTheme;		//!< Attribute used to define the theme with points from graph

	TeQtGrid*		_grid;				//!< Attribute used to define the grid from tv app

	std::string		_errorMessage;		//!< Attribute used to define a error message in edge edition
};

/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_EDGE_EDITION_H_