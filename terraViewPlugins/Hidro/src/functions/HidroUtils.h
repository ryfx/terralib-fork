/******************************************************************************
* Hidro development team
* 
* Terra View Hidro Plugin
* 
* @(#) HidroUtils.h
*
*******************************************************************************
*
* $Rev: 8063 $:
*
* $Author: fernanda $:
*
* $Date: 2009-10-22 16:16:18 +0300 (to, 22 loka 2009) $:
*
******************************************************************************/

/*!
	\brief This is the Hidro Functions Group.
	@defgroup hidroFunctions The Group for Plugin Functions Classes.
*/

#ifndef HIDRO_UTILS_H
#define HIDRO_UTILS_H

/*
** ----------------------------------------------------------------------------
** Includes:
*/

#include <TeDatabase.h>

class TeLayer;
class TeView;
class TeTheme;
class TeRaster;
class TeProjection;
class TeCoord2D;

class HidroGraph;
class HidroVertex;
class HidroEdge;

/** 
  * \file HidroUtils.h
  *
  * \class HidroUtils
  *
  * \brief This file is class to give support for other classes
  * This class is most used inside interfaces classes that needs
  * help to get information about terralib containers and objects.
  *
  * \author Eric Silva Abreu <eric.abreu@funcate.org.br>
  * \version 1.0
  *
/*!
  @ingroup hidroFunctions
 */

class HidroUtils
{

public:

	/** @name Constructor and Destructor Methods
		* Methods related to instantiation and destruction.
		*/
	//@{		
	
	/** \brief Default Constructor with parameters
	  * \param database		Input parameter - current database pointer from app
	*/
	HidroUtils(TeDatabase* database);

	/** \brief Virtual destructor.
	*/
	~HidroUtils();

	//@}

	/** @name Behavioral Methods
		* Methods related to hidro graph utils.
		*/
	//@{

public:

	/** \brief Function used to list all layer names from current database
	  * \param onlyRaster		Input parameter, used to define if only raster layers will be listed
	  * \param onlyPolygons		Input parameter, used to define if only polygons layers will be listed
	  * \return					Return string vector with all layers names
    */
	std::vector<std::string> listLayers(const bool& onlyRasters, const bool& onlyPolygons);

	/** \brief Function used to list all layer names from current database that has a raster pallete
	  * \return					Return string vector with all layers names
    */
	std::vector<std::string> listLayersLUT();

	/** \brief Function used to list all themes belongs to selected graph 
	  * \param layerName		Input parameter, used to define the theme layers to be listed
	  * \return					Return string vector with all themes belongs to defined layer name
    */
	std::vector<std::string> listLayerThemes(const std::string& layerName);

	/** \brief Function used to check a layer name defined by user in interface
	  * the name can not be exist in database and has to be a valid name.
	  * \param layerName		Input parameter, used to define the layer name
      * \return					True if the name is valid and False in other case
    */
	bool checkLayerName(const std::string& layerName, std::string& errorMessage);

	/** \brief Function used to get a layer pointer from current database by layer name
	  * \param layerName		Input parameter, attribute used to define the layer name
	  * \Return					Return the terralib layer pointer if the layer was founded and NULL in other case
    */
	TeLayer* getLayerByName(const std::string& layerName);

	/** \brief Function used to get a theme from database givin a theme name
	  * \param themeName		Input parameter, attribute used to define the theme name
	  * \return					Return a terralib Theme attribute pointer, valid if found and NULL in other case
    */
	TeTheme* getThemeByName(const std::string& themeName);

	/** \brief Function used to import to database a raster created, also will be created a new layer
	* \param layerName		Input parameter, attribute used to define the layer name
	* \param raster			Input parameter, terralib raster attribute, object to be imported
	* \return				Return true if the raster was imported correctly and false in other case
    */
	bool saveOutputRaster(const std::string& layerName, TeRaster* raster);

	/** \brief Function used to import to database a raster created, also will be created a new layer
	* \param layerName		Input parameter, attribute used to define the layer name
	* \param raster			Input parameter, terralib raster attribute, object to be imported
	* \param view
	* \return				Return true if the raster was imported correctly and false in other case
    */
	bool saveOutputRasterCreatingTheme(const std::string& layerName, TeRaster* raster, TeView* view);

	/** \brief Function used to get the vertex identification giver a coord over vertex theme
	  * \param graphName	Input parameter, attribute used to define the selected hidro graph
	  * \param vertexTheme	Input parameter, attribute used to define the theme that have the vertex from graph
	  * \param projCanvas	Input parameter, attribute used to define the projection from canvas (used in tolerance rotine)
	  * \param pixelSize	Input parameter, attribute used to define the size of a pixel in canvas (used in tolerance rotine)
	  * \param coord		Input parameter, terralib attribute used to define a coordenate
	  * \return				Return a string with vertex name if the vertex was found
    */
	std::string getPointName(const std::string& graphName, TeTheme* vertexTheme, TeProjection* projCanvas, double& pixelSize, TeCoord2D& coord);

	TeCoord2D getPointCoord(const std::string& graphName, TeTheme* vertexTheme, TeProjection* projCanvas, double& pixelSize, TeCoord2D& coord);

	/** \brief Function used to get the edge identification giver a coord over edge theme
	  * \param graphName	Input parameter, attribute used to define the selected hidro graph
	  * \param edgeTheme	Input parameter, attribute used to define the theme that have the edges from graph
	  * \param projCanvas	Input parameter, attribute used to define the projection from canvas (used in tolerance rotine)
	  * \param pixelSize	Input parameter, attribute used to define the size of a pixel in canvas (used in tolerance rotine)
	  * \param coord		Input parameter, terralib attribute used to define a coordenate
	  * \return				Return a string with edge name if the edge was found
    */
	std::string getLineName(const std::string& graphName, TeTheme* edgeTheme, TeProjection* projCanvas,  double& pixelSize, TeCoord2D& coord);


public:

	/** \brief Function used to created a vertex from a raster cell, given by a coord
    \param	raster		Input parameter, terralib raster attribute, this raster is a flow grid
	\param	line		Input parameter, attribute used to define the x coord to get raster cell
	\param	col			Input parameter, attribute used to define the y coord to get raster cell
	\param	idx			Input parameter, attribute used to index value for this vertex
	\return				Return a valid hidro vertex pointer if the vertex was created correctly and false in other case
    */
	HidroVertex* createVertex(TeRaster* raster, const int& line, const int& col, const int& idx, HidroGraph* graph = NULL, const bool& checkVertex = true);

	/** \brief Function used to created a edge from two hidro vertex
		\param	vertexFrom	Input parameter, hidro vertex attribute, indicate the origin node
		\param	vertexTo	Input parameter, hidro vertex attribute, indicate the destiny node
		\return				Return a valid hidro edge pointer if the edge was created correctly and false in other case
		*/
	HidroEdge* createEdge(HidroVertex* vertexFrom, HidroVertex* vertexTo);

	/** \brief Generates a graph 
		\param	inputRaster		Input parameter, terralib raster attribute, this raster is a flow grid
		\param	graph			Output parameter, hidro graph attribute, generated from raster flow
		\return					True if the graph was generated correctly and false in other case
		*/
	bool generateGraph(TeRaster* inputRaster, HidroGraph& graph, const bool& checkVertex = true);


protected:

	/** \brief Function used to get the tolerance values to get the a coord location in current theme
	  * \param projTheme	Input parameter, attribute used to define the projection from current theme
	  * \param projCanvas	Input parameter, attribute used to define the projection from canvas
	  * \param pixelSize	Input parameter, attribute used to define the size of a pixel in canvas
	  * \param pt			Input parameter, function used to defined the point that we want to get the tolerance
	  * \return				Return a double value that defines the tolerance.
    */
	double getToleranceValue(TeProjection* projTheme, TeProjection* projCanvas, double& pixelSize, TeCoord2D& pt);

	//@}

protected:

	TeDatabase*	_database;		//!< Attribute used to access current database from application

};

/*
** ---------------------------------------------------------------
** End:
*/


#endif //HIDRO_UTILS_H