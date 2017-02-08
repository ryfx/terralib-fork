/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroGraph.h
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

#ifndef HIDRO_GRAPH_H_
#define HIDRO_GRAPH_H_

/*
** ----------------------------------------------------------------------------
** Includes:
*/
#include <vector>
#include <map>

#include <TeBox.h>

class HidroVertex;
class HidroEdge;


/** 
  * \file HidroGraph.h
  *
  * \class HidroGraph
  *
  * \brief This file is a base class to define a graph
  *
  * A graph is a struct organized by vertex and edges to 
  * describe a flow. In this application we study the water
  * flow, knhow as hidrography modeling. The graph that we
  * build will be directional, labeled and unciclic.
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroKernel
 */

class HidroGraph
{

public:

	/** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor.
	*/
	HidroGraph();

	/** \brief Constructor with parameters
	  * \param name		Input parameter name, set the graph name
	*/
	HidroGraph(const std::string& name);

	/** \brief Virtual destructor.
	*/
	~HidroGraph();

	//@}

	/** @name Behavioral Methods
		* Methods related to hidro graph behavior.
		*/
	//@{

public:

    /** \brief Function used to set the graph name
        \param name	Input parameter, value used to set the graph name
      */
	void setGraphName(const std::string& name);

	/** \brief Function used to get the graph name
        \return	String that defines the graph name
      */
	std::string getGraphName() const;

	/** \brief Function used to add a new vertex inside the graph
        \param vertex	Input parameter, hidro vertex that will be added into the graph
		\return			True if the vertex is not alredy in the graph and false in other case
      */
	bool addGraphVertex(HidroVertex* vertex);

	/** \brief Function used to remove a vertex from graph
        \param vertexId		Input parameter, hidro vertex identification from graph vertex
		\return				True if the vertex was found in the graph and false in other case
      */
	bool removeGraphVertex(const std::string& vertexId);

	/** \brief Function used to get a vertex from graph
        \param vertexId		Input parameter, hidro vertex that will be find in the graph
		\param vertex		Output parameter, hidro vertex that will be getted from graph
		\return				True if the vertex was found in the graph and false in other case
      */
	HidroVertex* getGraphVertex(const std::string& vertexId);

	/** \brief Function used to get a vertex from graph
        \param vertexId		Input parameter, index value from vertex
		\param vertex		Output parameter, hidro vertex that will be getted from graph
		\return				True if the vertex was found in the graph and false in other case
      */
	HidroVertex* getGraphVertexByIdx(const int& vertexIdx);

	/** \brief Function used to get the graph vertex vector
		\return				Vector with the graph vertex
      */
	std::map<std::string, HidroVertex*>& getGraphMapVertex();

	/** \brief Function used to add a new edge inside the graph
        \param edge	Input parameter, hidro edge that will be added into the graph
		\return		True if the edge is not alredy in the graph and false in other case
      */
	bool addGraphEdge(HidroEdge* edge);

	/** \brief Function used to remove an edge from graph
        \param edgeId		Input parameter, hidro edge identification from graph edges
		\return				True if the edge was found in the graph and false in other case
      */
	bool removeGraphEdge(const std::string& edgeId);

	/** \brief Function used to get an edge from graph
        \param edgeId	Input parameter, hidro edge that will be find in the graph
		\param edge		Output parameter, hidro edge that will be getted from graph
		\return			True if the edge was found in the graph and false in other case
      */
	HidroEdge* getGraphEdge(const std::string& edgeId);

	/** \brief Function used to get the graph edge vector
		\return				Vector with the graph edges
      */
	std::map<std::string, HidroEdge*>& getGraphMapEdge();

	/** \brief Function used to khnow if a graph was valid
		\return		True if the graph was valid and false in other case
      */
	bool isGraphValid();

	TeBox getGrahBox();

	//@}

protected:

	std::string							_graphName;			//!< Attribute used to define the graph name

	std::map<std::string, HidroVertex*>	_graphMapVertex;	//!< Vector attribute used to store the graph vertex

	std::map<int, std::string>			_graphMapVertexIdx;	//!< Vector attribute used to store the graph vertex idx

	std::map<std::string, HidroEdge*>	_graphMapEdge;		//!< Vector attribute used to store the graph edges

	TeBox								_graphBox;
};


/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_GRAPH_H_