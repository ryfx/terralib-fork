/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroVertex.h
*
*******************************************************************************
*
* $Rev: 7952 $:
*
* $Author: eric $:
*
* $Date: 2009-07-07 17:17:02 +0300 (ti, 07 heinä 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Kernel Group.
	@defgroup hidroKernel The Group for Plugin Base Classes.
*/

#ifndef HIDRO_VERTEX_H_
#define HIDRO_VERTEX_H_

/*
** ----------------------------------------------------------------------------
** Includes:
*/
#include <map>
#include <vector>

#include <TeCoord2D.h>

class HidroAttr;
class HidroEdge;

/** 
  * \file HidroVertex.h
  *
  * \class HidroVertex
  *
  * \brief This file is a base class to define a graph vertex
  *
  * A vertex from a graph is like as a node in a tree. The vertex
  * has a geometry location, identification and a vector of attributes.
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroKernel
 */

class HidroVertex
{

public:
		
    /** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor.
	*/
	HidroVertex();

	/** \brief Constructor with parameters
	  * \param id		Input parameter id, set the vertex identification
	  * \param coord	Input parameter coord, set the vertex location
	*/
	HidroVertex(const std::string& id, const TeCoord2D& coord);

	/** \brief Virtual destructor.
	*/
	~HidroVertex();

	//@}

	/** @name Behavioral Methods
		* Methods related to hidro vertex behavior.
		*/
	//@{

public:

    /** \brief Function used to set the vertex identification
        \param id	Input parameter, value used to set the vertex identification
      */
	void setVertexId(const std::string& id);

	/** \brief Function used to get the vertex identification
        \return String values with the vertex identification
      */
	std::string getVertexId() const;

	/** \brief Function used to set the vertex idx
        \param idx	Input parameter, value used to set the vertex index
      */
	void setVertexIdx(const int& idx);

	/** \brief Function used to get the vertex index
        \return Integer value with the vertex index
      */
	int getVertexIdx() const;

	/** \brief Function used to set the vertex location
        \param coord	Input parameter, terralib coord used to set the vertex location
      */
	void setVertexCoord(const TeCoord2D& coord);

	/** \brief Function used to get the vertex location
        \return terraLib coord value used to define the vertex location
      */
	TeCoord2D getVertexCoord() const;

	/** \brief Function used to add an attribute to vertex
        \param attr		Input parameter, hidro attribute value
		\return			True if the attribute was added correctly and false in other case
      */
	bool addVertexAttr(HidroAttr* attr);

	/** \brief Function used to remove an attribute from vertex
        \param attrName		Input parameter, hidro attribute value name
		\return				True if the attribute was removed correctly and false in other case
      */
	bool removeVertexAttr(const std::string& attrName);

	/** \brief Function used to get the vertex attribute by name
		\param attrName		Input parameter, hidro attribute value name
		\return				Return a valid pointer if the vertex was founded and NULL in other case
							False in other case
      */
	HidroAttr* getVertexAttr(const std::string& attrName);

	/** \brief Function used to get the vertex attribute vector
		\return		The attribute vector from the vertex
      */
	std::vector<HidroAttr*>& getVertexVecAttr();

	/** \brief Function used to get the map used to save all edges that leaving this vertex
		\return		The attribute set with all hidro edges that leaving this vertex
      */
	std::map<std::string, HidroEdge*>& getMapEdgeOut();

	/** \brief Function used to add a new edge to map of edges out, only will be added if this edge is not alredy in map
		\param edge		Input parameter, hidro edge attribute that leaving this vertex
      */
	void addEdgeOut(HidroEdge* edge);

	/** \brief Function used to get the map used to save all edges that coming to this vertex
		\return		The attribute set with all hidro edges that coming to this vertex
      */
	std::map<std::string, HidroEdge*>& getMapEdgeIn();

	/** \brief Function used to add a new edge to map of edges in, only will be added if this edge is not alredy in map
		\param edge		Input parameter, hidro edge attribute that coming to this vertex
      */
	void addEdgeIn(HidroEdge* edge);

protected:

	/** \brief Function used to get the vertex attribute by name, internal function
		\param attrName		Input parameter, hidro attribute value name
        \return				A valid iterator if the attr was found and a invalid iterator in the other case
      */
	std::vector<HidroAttr*>::iterator getVertexAttrItem(const std::string& attrName);

	//@}

protected:

	std::string							_vertexId;			//!< Attribute used to define the vertex identification

	int									_vertexIdx;			//!< Attribute used to define an index to this vertex

	TeCoord2D							_vertexCoord;		//!< Attribute used to define the vertex location

	std::vector<HidroAttr*>				_vertexAttrVec;		//!< Vector Attribute used to save the vertex attributes

	std::map<std::string, HidroEdge*>	_mapEdgeOut;		//!< Map attribute used to save all edges that leaving this vertex

	std::map<std::string, HidroEdge*>	_mapEdgeIn;			//!< Map attribute used to save all edges that coming to this vertex
};

/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_VERTEX_H_