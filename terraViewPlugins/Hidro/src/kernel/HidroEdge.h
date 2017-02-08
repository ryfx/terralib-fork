/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroEdge.h
*
*******************************************************************************
*
* $Rev: 7965 $:
*
* $Author: eric $:
*
* $Date: 2009-07-29 19:59:07 +0300 (ke, 29 heinä 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Kernel Group.
	@defgroup hidroKernel The Group for Plugin Base Classes.
*/

#ifndef HIDRO_EDGE_H_
#define HIDRO_EDGE_H_

/*
** ----------------------------------------------------------------------------
** Includes:
*/
#include <TeGeometry.h>

class HidroAttr;
class HidroVertex;


/** 
  * \file HidroEdge.h
  *
  * \class HidroEdge
  *
  * \brief This file is a base class to define a graph Edge
  *
  * A edge from a graph indicate the flow into the graph, represents
  * the link between two vertex. An edge was represent by a terraLib line
  * and has a geometry location, identification and a vector of attributes.
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroKernel
 */

class HidroEdge
{

public:

	/** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor.
	*/
	HidroEdge();

	/** \brief Constructor with parameters
	  * \param id		Input parameter id, set the edge identification
	*/
	HidroEdge(const std::string& id);

	/** \brief Virtual destructor.
	*/
	~HidroEdge();

	//@}


	/** @name Behavioral Methods
		* Methods related to hidro edge behavior.
		*/
	//@{

public:

    /** \brief Function used to set the edge identification
        \param id	Input parameter, value used to set the edge identification
      */
	void setEdgeId(const std::string& id);

	/** \brief Function used to get the edge identification
        \return String values with the edge identification
      */
	std::string getEdgeId() const;

	/** \brief Function used to get the edge geometry
        \return terraLib line value used to define the line geometry
      */
	TeLine2D getEdgeLine() const;

	/** \brief Function used to set the edge initial vertex
	  * IMPORTANT: This function also add this edge in vertex map edge out
      * \param vertex		Input parameter, hidro vertex used to indicate the flow origin
      */
	void setEdgeVertexFrom(HidroVertex* vertex);

	/** \brief Function used to get the edge initial vertex
        \return Hidro vertex that indicate the flow origin
      */
	HidroVertex* getEdgeVertexFrom() const;

	/** \brief Function used to set the edge final vertex
	  * IMPORTANT: This function also add this edge in vertex map edge in
	  * \param vertex		Input parameter, hidro vertex used to indicate the flow destiny
      */
	void setEdgeVertexTo(HidroVertex* vertex);

	/** \brief Function used to get the edge final vertex
        \return Hidro vertex that indicate the flow destiny
      */
	HidroVertex* getEdgeVertexTo() const;

	/** \brief Function used to add an attribute to edge
        \param Input parameter, hidro attribute value
		\return	True if the attribute was added correctly and false in other case
      */
	bool addEdgeAttr(HidroAttr* attr);

	/** \brief Function used to remove an attribute from edge
        \param Input parameter, hidro attribute value name
		\return	True if the attribute was removed correctly and false in other case
      */
	bool removeEdgeAttr(const std::string& attrName);

	/** \brief Function used to get the edge attribute by name
		\param attrName		Input parameter, hidro attribute value name
     	\return				Return a valid pointer if the vertex was founded and NULL in other case
							False in other case
	  */
	HidroAttr* getEdgeAttr(const std::string& attrName);

	/** \brief Function used to get the edge attribute vector
		\return		The attribute vector from the edge
      */
	std::vector<HidroAttr*>& getEdgeVecAttr();



protected:

	/** \brief Function used to get the edge attribute by name, internal function
		\param attrName	Input parameter, hidro attribute value name
        \return			A valid iterator if the attr was found and a invalid iterator in the other case
      */
	std::vector<HidroAttr*>::iterator getEdgeAttrItem(const std::string& attrName);

	//@}


protected:

	std::string				_edgeId;			//!< Attribute used to define the edge identification

	HidroVertex*			_edgeVertexFrom;	//!< Attribute used to define the initial vertex from edge

	HidroVertex*			_edgeVertexTo;		//!< Attribute used to define the final vertex from edge

	std::vector<HidroAttr*>	_edgeAttrVec;		//!< Vector Attribute used to safe the edge attributes

};

/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_EDGE_H_