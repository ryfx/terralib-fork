/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroPersister.h
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
	\brief This is the Hidro Functions Group.
	@defgroup hidroFunctions The Group for Plugin Functions Classes.
*/

#ifndef HIDRO_PERSISTER_H_
#define HIDRO_PERSISTER_H_

/*
** ----------------------------------------------------------------------------
** Includes:
*/
#include <string>

#include <TeAttribute.h>

class TeDatabase;
class TeProjection;
class TeTable;
class TeLayer;
class TeView;

class HidroGraph;
class HidroMetadata;


/** 
  * \file HidroPersister.h
  *
  * \class HidroPersister
  *
  * \brief This file is class to handle a graph into database
  *
  * To handle a graph into database we need methods to load,
  * save and delete a graph from database. 
  * This class also be used to create a view in database to
  * show the graph in terraView. It will be necessary two layers
  * to save the graph information, one layer with point representation
  * for vertex and one layer with line representation for edges. This
  * layers will be associated with attribute tables.
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroFunctions
 */

class HidroPersister
{

public:

	/** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor with parameters
	  * \param database		Input parameter - current database pointer from app
	*/
	HidroPersister(TeDatabase* database);

	/** \brief Virtual destructor.
	*/
	~HidroPersister();

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

	/** \brief Function used to load a hidro graph from database
		\param graphName		Input parameter, attribute used to define the graph name
		\param graph			Output parameter, hidro attribute used to define a graph struct
        \return					Return true if the graph was loaded correctly and false in other case
      */
	bool loadGraphFromDatabase(const std::string& graphName, HidroGraph& graph);

	/** \brief Function used to save a hidro graph in database
		\param graph			Input parameter, hidro attribute used to define a graph struct
		\param proj				Input parameter, projection from current graph
		\param res				Input parameter, resolution from current graph (used in case of regular grid)
        \return					Return true if the graph was saved correctly and false in other case
      */
	bool saveGraphInDatabase(HidroGraph& graph, TeProjection* proj, const double& res = 0.);

	/** \brief Function used to remove a hidro graph from database
		\param graphName		Input parameter, attribute used to define the graph name
        \return					Return true if the graph was removed correctly and false in other case
      */
	bool deleteGraphFromDatabase(const std::string& graphName);

	/** \brief	Function used to create a graph visualization in application, a new view was created in database
				one theme was created to visualize the layer with points representation and one theme for layer with
				lines representation, the object names was created from graph name
		\param graph				Input parameter, hidro attribute used to define a graph struct
		\param proj					Input parameter, terralib projection used to create the view
        \return						Return true if a view and themes for graph visualization was created correctly
									and false in other case
      */
	TeView* createGraphView(HidroGraph* graph, TeProjection* proj);

		/** \brief Function used to save the layer and layer attriubte table in database
		\param layer		Input parameter, attribute used to define layer that attribute table belongs
		\param table		Input parameter, attribute used to define the table created
		\param rep			Input parameter, attribute used to define if the layer has points or lines representations
        \return				Return true if the layer was saved correctly and false in other case
      */
	bool saveLayer(TeLayer* layer, TeTable& table, const TeGeomRep& rep);

	/** \brief Function used create a new theme insede a view from a input layer
		\param themeName		Input parameter, attribute used to define the theme name
		\param whereClauseIN	Input parameter, attribute used to create a theme with restriction
		\param view				Input parameter, attribute used to inform the view to insert a new theme
		\param layer			Input parameter, attribute used to define the layer to be generate the theme
        \return					Return true if the theme was created correctly and false in other case
      */
	bool createTheme(const std::string& themeName, const std::string& whereClauseIN, TeView* view, TeLayer* layer);

	bool updateGraphAttributes(HidroGraph* inputGraph, HidroGraph* outputGraph);


protected:

	/** \brief Function used to get the vertex attribute list from graph. The first vertex is used to get the attribute list;
		\param graph	Input parameter, hidro graph used to get vertex attributes
        \return			Return attribute list from first vertex founded in graph
      */
	TeAttributeList getVertexAttributeList(HidroGraph& graph);

	/** \brief Function used to get the edge attribute list from graph. The first edge is used to get the attribute list;
		\param graph	Input parameter, hidro graph used to get edge attributes
        \return			Return attribute list from first edge founded in graph
      */
	TeAttributeList getEdgeAttributeList(HidroGraph& graph);

	/** \brief Function used to create a new attribute table in current database
		\param tableName	Input parameter, attribute used to define the table name
		\param attrList		Input parameter, attribute list used to define the table fields
        \return				Return true if the table was created correctly in database and false in other case
      */
	bool createAttributeTable(const std::string& tableName, TeAttributeList& attrList);

	/** \brief Function used to create a new table in memory, this function also define the link table
		\param tableName	Input parameter, attribute used to define the table name
		\param attrList		Input parameter, attribute list used to define the table fields
		\param table		Output parameter, attribute used to define the table created
		\param edgTable		Input parameter, atrribute used to indicate if the table to be created is from edge objects
        \return				Return true if the table was created correctly in memory and false in other case
      */
	bool createTable(const std::string& tableName, TeAttributeList& attrList, TeTable& table, const bool& edgeTable);

	bool copyAttributeEntry(HidroGraph* inputGraph, HidroGraph* outputGraph, const std::string& attrName, const bool& isVertexAttr);
	//@}

protected:

	std::string		_errorMessage;		//!< Attribute used to inform about an error generated by internal functions

	TeDatabase*		_db;				//!< Attribute used to access current database from application

	HidroMetadata*	_hidroMetadata;		//!< Attribute used to handle with hidro metadata information

};


/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_PERSISTER_H_