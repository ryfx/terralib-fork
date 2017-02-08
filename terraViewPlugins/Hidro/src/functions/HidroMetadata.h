/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroMetadata.h
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

#ifndef HIDRO_METADATA_H_
#define HIDRO_METADATA_H_

/*
** ----------------------------------------------------------------------------
** Includes:
*/
#include <vector>

class TeDatabase;
class TeLayer;
class TeProjection;

/** 
  * \file HidroMetadata.h
  *
  * \class HidroMetadata
  *
  * \brief This file is class to handle a graph table into database
  *
  * To handle a graph table into database we need methods to create,
  * and get information for this table. The new table to be created
  * in database will be called te_hidro_graph.
  * This table have this attributes
  *		graph_id: unique identification for the graph
  *		graph_name: name to identificate the graph (defined by user)
  *		layer_points_id: vertex layer identification
  *		layer_lines_id: edge layer identification
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroFunctions
 */

typedef struct
{
	std::string attrName;
	std::string layerName;
	std::string layerAttrName;
} GraphAttr;

class HidroMetadata
{

public:

	/** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor with parameters
	  * \param database		Input parameter - current database pointer from app
	*/
	HidroMetadata(TeDatabase* database);

	/** \brief Virtual destructor.
	*/
	~HidroMetadata();

	//@}

	/** @name Behavioral Methods
		* Methods related to hidro graph metadata.
		*/
	//@{

public:

	/** \brief Function used to get the error message from this class
        \return		String with the error message, if was not error, a empty string will be returned
      */
	std::string getErrorMessage() const;

	/** \brief Function used add a new entry in database graph table
		\param graphName		Input parameter, attribute used to define the graph name
		\param layerPointsId	Input parameter, attribute used to identify the layer used to store the points from graph
		\param layerLinesId		Input parameter, attribute used to identify the layer used to store the lines from graph
		\param res				Input parameter, attribute used to define the graph resolution (used in case of regular grid)
        \return					Return true if the record was saved correctly in table, and false in other case
      */
	bool addGraphEntry(const std::string& graphName, const int& layerPointsId, const int& layerLinesId, const double& res = 0.);

	/** \brief Function used remove a entry in database graph table
		\param graphName		Input parameter, attribute used to define the graph name
        \return					Return true if the record was removed correctly in table, and false in other case
      */
	bool removeGraphEntry(const std::string& graphName);

	/** \brief Function used get pointer from graph layer with point representation
		\param graphName		Input parameter, attribute used to define the graph name
        \return					Return a valid pointer if the layer was founded and false in other case
      */
	TeLayer* getGraphLayerPoints(const std::string& graphName);

	/** \brief Function used get pointer from graph layer with line representation
		\param graphName		Input parameter, attribute used to define the graph name
        \return					Return a valid pointer if the layer was founded and false in other case
      */
	TeLayer* getGraphLayerLines(const std::string& graphName);

	/** \brief Function used get the attribute list associated to layer with point representation
		\param graphName		Input parameter, attribute used to define the graph name
        \return					Return a string vector with attribute names, or a empty vector
      */
	std::vector<std::string> getLayerPointsAttributes(const std::string& graphName, const bool& allAttrs = true);

	/** \brief Function used get the attribute list associated to layer with line representation
		\param graphName		Input parameter, attribute used to define the graph name
        \return					Return a string vector with attribute names, or a empty vector
      */
	std::vector<std::string> getLayerLinesAttributes(const std::string& graphName, const bool& allAttrs = true);

	/** \brief Function used get the attribute list associated to graph with integer or double attribute
		\param graphName		Input parameter, attribute used to define the graph name
        \return					Return a string vector with attribute names, or a empty vector
      */
	std::vector<std::string> getGraphCostsAttributes(const std::string& graphName);

	/** \brief Function used get the projection from a hidro graph (the projection returned is a copy from vertex layer)
		\param graphName		Input parameter, attribute used to define the graph name
        \return					Return projection copy from vertex layer
      */
	TeProjection* getGraphProjection(const std::string& graphName);

	/** \brief Function used get all graphs saved in database
        \return		Return a string vector with graphs names
      */
	std::vector<std::string> getGraphNameListFromDB();

	/** \brief Function used check if exist in database a graph with the input name
		\param graphName		Input parameter, attribute used to define the graph name
        \return					Return true if the graph with this names exist in database and false in other case
      */
	bool existGraph(const std::string& graphName);

	/** \brief Function used to get the vertex name from a graph given the vertex object id
		\param graphName		Input parameter, attribute used to define the graph name
		\param vertexObjId		Input parameter, attribute used to define the vertex object identification
        \return					Return a string with the vertex name if the vertex objId was found in graph
      */
	std::string getVertexName(const std::string& graphName, const std::string& vertexObjId);

	/** \brief Function used to get the edge name from a graph given the edge object id
		\param graphName		Input parameter, attribute used to define the graph name
		\param edgeObjId		Input parameter, attribute used to define the edge object identification
        \return					Return a string with the edge name if the edge objId was found in graph
      */
	std::string getEdgeName(const std::string& graphName, const std::string& edgeObjId);

	/** \brief Function used to get the hidro graph table name (defined hardcode)
        \return		Return a string with the hidro graph table name
      */
	std::string getHidroTableName();

	/** \brief Function used to get the vertex name attribute from vertex hidro table attr (defined hardcode)
        \return		Return a string with the vertex name attribute name
      */
	std::string getVertexNameAttrName();

	/** \brief Function used to get the edge name attribute from edge hidro table attr (defined hardcode)
        \return		Return a string with the edge name attribute name
      */
	std::string getEdgeNameAttrName();

	/** \brief Function used to get the edge vertex from attribute from edge hidro table attr (defined hardcode)
        \return		Return a string with the edge vertex from attribute name
      */
	std::string getEdgeVertexFromAttrName();

	/** \brief Function used to get the edge vertex to attribute from edge hidro table attr (defined hardcode)
        \return		Return a string with the edge vertex to attribute name
      */
	std::string getEdgeVertexToAttrName();
	
	/** \brief Function used to get the attribute name used to define the object id from vertex and edge object (defined hardcode)
        \return		Return a string with the attribute object id name
      */
	std::string getObjectIdAttrName();

	std::string getGraphVertexAttrTableName(const std::string& graphName);

	std::string getGraphEdgeAttrTableName(const std::string& graphName);

	bool addGraphVertexAttrEntry(const std::string& graphName, const std::string& attrName, const std::string& layerName = "", const std::string& layerAttrName = "");

	bool addGraphEdgeAttrEntry(const std::string& graphName, const std::string& attrName, const std::string& layerName = "", const std::string& layerAttrName = "");

	bool existGraphAttrEntry(const std::string& graphName, const std::string& attrName, const bool& vertexAttr);

	bool removeGraphAttrEntry(const std::string& graphName, const std::string& attrName, const bool& vertexAttr);

	bool removeGraphAttr(const std::string& graphName, const std::string& attrName, const bool& vertexAttr);

	bool addGraphAttr(const std::string& graphName, const std::string& attrName, const bool& stringType, const bool& vertexAttr);

	std::vector<GraphAttr> getGraphAttrList(const std::string graphName, const bool& vertexAttr);

	double getGraphResolution(const std::string& graphName);


protected:

	/** \brief Function used to create the hidro graph table in database
        \return		Return true if the table was created correctly and false in other case
      */
	bool createGraphMetadataTable();

	/** \brief Function used to create the hidro graph attr table in database
	    \param	tableName	Input parameter, define the table name
        \return				Return true if the table was created correctly and false in other case
      */
	bool createAttrMetadataTable(const std::string& tableName);

	/** \brief Function used to get a layer from database by id
		\param id	Input parameter - attribute used to identify the layer
        \return		Return a valid layer pointer if the layer id was found in database
					and a NULL pointer if the layer was not found
      */
	TeLayer* getLayerById(const int& id);

	//@}

protected:

	std::string		_errorMessage;			//!< Attribute used to inform about an error generated by internal functions

	TeDatabase*		_db;					//!< Attribute used to access current database from application

	std::string		_hidroGraphTableName;	//!< Attribute used to define the hidro graph table name in database

	std::string		_vertexNameAttrName;	//!< Attribute used to define the column name for vertex name attribute

	std::string		_edgeNameAttrName;		//!< Attribute used to define the column name for edge name attribute

	std::string		_edgeVertexFromAttrName;//!< Attribute used to define the column name for edge vertex from attribute

	std::string		_edgeVertexToAttrName;	//!< Attribute used to define the column name for edge vertex to attribute

	std::string		_objectId;				//!< Attribute used to define the column name for object id (vertex and edge)

};


/*
** ---------------------------------------------------------------
** End:
*/

#endif // HIDRO_METADATA_H_