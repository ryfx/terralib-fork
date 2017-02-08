/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright  2001-2004 INPE and Tecgraf/PUC-Rio.
This file is part of TerraView. TerraView is free software; you can redistribute it 
and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

You should have received a copy of the GNU General Public License
along with TerraView.
The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The software provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use of
this program and its documentation.
*************************************************************************************/

#ifndef FILLCELLUTILS_H
#define FILLCELLUTILS_H

#include <TeDatabase.h>
#include <TeTimeInterval.h>

/**
* @brief A class for fill cell plugin utils.
* @author Eric Silva Abreu <eric.abreu@funcate.org.br>
*/   
class FillCellUtils
{
public:

	/**
	* Default constructor.
	*/      
	FillCellUtils(TeDatabase* db);

	/**
	* Default destructor.
	*/    
	~FillCellUtils();

public:

	/**
	* Function to return a list of layers names with cell representation
	*
	* @return List of layers name from current database
	*/  
	std::vector<std::string> getCellLayerList();

	/**
	* Function to return a list of themes names
	*
	* @return List of theme name from current database
	*/  
	std::vector<std::string> getThemeList();

	/**
	* Function to get a theme given a theme name.
	*
	* @param	themeName	String that defines the theme name selected as input theme in fill cell operation
	* @return	A valid theme pointer if a theme with this name exist in database.
	*/ 
	TeTheme* getTheme(const std::string& themeName);

	/**
	* Function to get a layer given a layer name.
	*
	* @param	layerName	String that defines the layer name selected as layer cell used in fill cell operation
	* @return	A valid layer pointer if a layer with this name exist in database.
	*/
	TeLayer* getLayer(const std::string& layerName);

	/**
	* Function to get a list of tables from a theme
	* call function getLayerAttrTables
	* @param	themeName	String that defines the theme name selected as input theme in fill cell operation
	*
	* @return	A list of table names from a theme
	*/
	std::vector<std::string> getThemeAttrTables(const std::string& themeName);

	/**
	* Function to get a list of tables from a layer
	* @param	layerName	String that defines the layer name selected as input layer in fill cell operation
	*
	* @return	A list of table names from a layer
	*/
	std::vector<std::string> getLayerAttrTables(const std::string& layerName);

	/**
	* Function to get a list of atrributes for a specific table from a theme
	* call function getLayerAttrList
	* @param	themeName	String that defines the theme name selected as input theme in fill cell operation
	* @param	tableName	String that defines the table selected from input theme
	*
	* @return	A list of attributes names
	*/
	TeAttributeList getThemeAttrList(const std::string& themeName, const std::string& tableName);

	/**
	* Function to get a list of atrributes for a specific table from a layer
	* @param	layerName	String that defines the layer name selected as input layer in fill cell operation
	* @param	tableName	String that defines the table selected from input theme
	*
	* @return	A list of attributes names
	*/
	TeAttributeList getLayerAttrList(const std::string& layerName, const std::string& tableName);

	/**
	* Function used to verify if a selected table is temporal
	* @param	layerName	String that defines the layer name selected as input layer in fill cell operation
	* @param	tableName	String that defines the table selected from input theme
	*
	* @return	True if table is temporal.
	*/
	bool tableIsTemporal(const std::string& layerName, const std::string& tableName);

	/**
	* Function used to get all times from a temporal table
	* These times a returned as a string such as (timeInitial/timeFinal).
	* @param	layerName	String that defines the layer name selected as input layer in fill cell operation
	* @param	tableName	String that defines the table selected from input theme
	*
	* @return	A list of times(timeInitial/timeFinal) from a temporal table
	*/
	std::vector<std::string> getTableTimes(const std::string& layerName, const std::string& tableName);

	/**
	* Function used to get the time interval from a temporal table given a string time (timeInitial/timeFinal)
	* @param	layerName		String that defines the layer name selected as input layer in fill cell operation
	* @param	tableName		String that defines the table selected from input theme
	* @param	strTimeInterval	String that defines a time interval.
	*
	* @return	terralib struct that defines a time interval
	*/
	TeTimeInterval getTimeInterval(const std::string& layerName, const std::string& tableName, const std::string& strTimeInterval);

	/**
	* Function used to get the attribute data type for a atrribute
	* @param	layerName		String that defines the layer name selected as input layer in fill cell operation
	* @param	tableName		String that defines the table selected from input theme
	* @param	attrName		String that defines the attribute selected from theme table
	*
	* @return	terralib struct that defines attribute data type
	*/
	TeAttrDataType getAttrDataType(const std::string& layerName, const std::string& tableName, const std::string& attrName);

	/**
	* Function used to check with the new attribute name is valid or already exist in selected table
	* @param	layerName		String that defines the layer name selected as input layer in fill cell operation
	* @param	tableName		String that defines the table selected from input theme
	* @param	attrName		String that defines the new attribute name
	*
	* @return	True if the name is valid and false if the attribute name already exist
	*/
	bool checkAttributeName(const std::string& layerName, const std::string& tableName, const std::string& attrName, std::string& errorMessage);

	/**
	* Function used to update the collection aux table from theme
	* @param	layerName		String that defines the layer name selected as input layer cell in fill cell operation
	*
	* @return	True if the theme was update correctly and false in other case
	*/
	bool updateTheme(const std::string& layerName);

protected:

	/**
	* The current database pointer.
	*/    
	TeDatabase* _database;
};


#endif //FILLCELLUTILS_H