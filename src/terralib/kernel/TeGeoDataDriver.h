/************************************************************************************
 TerraLib - a library for developing GIS applications.
Copyright � 2001-2007 INPE and Tecgraf/PUC-Rio.

This code is part of the TerraLib library.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

You should have received a copy of the GNU Lesser General Public
License along with this library.

The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The library provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use
of this library and its documentation.
*************************************************************************************/
/* 
	\file TeGeoDataDriver.h
	\brief This file contains contains the abstract definition of a driver to geographical data files with vectorial representation.
*/

#ifndef __TERRALIB_INTERNAL_GEODATADRIVER_H
#define __TERRALIB_INTERNAL_GEODATADRIVER_H

class TeProjection;
class TeSTElementSet;
class TeLayer;
class TeTable;
class TeSTInstance;

#include "TeDataTypes.h"
#include "TeAttribute.h"
#include "TeBox.h"

#include "TeDefines.h"

#include <string>

//! Abstract definition of a driver to geographical data files with vectorial representation
class TL_DLL TeGeoDataDriver
{
public:

	//! Destructor
	virtual ~TeGeoDataDriver() {}

	//! Returns the complete access path associated to this driver
	virtual std::string getFileName() = 0;

	//! Returns true or false whether the data file is accessible
	virtual bool isDataAccessible() = 0;

	//! Returns the spatial reference associated to the data as an instance of TeProjection 
	virtual TeProjection* getDataProjection() = 0;

	//! Loads the minimum metadata information about the data
	/*
		\param nObjects to return the number of objects accessible in the data set
		\param ext to return the spatial extension of the geometries in the data set
		\param repres to return the type of geometries in the file
		\return true if or false whether it can retrieve the information or not
	*/
	virtual bool getDataInfo(unsigned int& nObjects , TeBox& ext, TeGeomRep& repres) = 0;

	//! Loads the list of descriptive attributes of the objetcts represented in the file
	virtual bool getDataAttributesList(TeAttributeList& attList) = 0;

	//! Loads the data into an TeSTElementSet structure in memory
	/*
		\param dataSet to return data set
		\return true if or false whether it can retrieve the information or not
	*/
	virtual bool loadData(TeSTElementSet* dataSet) = 0;

	//! Loads the next TeSTInstance and adds it to given TeSTElementSet - not loading all objects to memory.
	virtual bool loadNextElement(TeSTElementSet* dataSet) { return false; }

	//! Creates a new TerraLib layer from a concrete geo data driver.
	/*! 
		\param layer TerraLib Layer
		\param rep TerraLib Representation: Polygon, Line, Point or Text
		\return true if the layer was successfully created and false otherwise
	*/
	virtual bool createLayer(TeLayer* layer, const int& rep) { return false; }

	//! Creates a TerraLib Attribute Table from a concrete geo data driver.
	/*! 
		\param table TerraLib Table
		\return true if the table was successfully created and false otherwise
	*/
	virtual bool createAttributeTable(TeTable& table) { return false; }

	//! Adds a TeSTInstance (TerraLib feature) to set of features handled by a concrete geo data driver.
	/*!
		\param feature A TerraLib feature.
		\return true if the feature was successfully added and false otherwise
	*/
	virtual bool addElement(TeSTInstance* e) { return false; }

	static TeGeoDataDriver*	DefaultObject() { return 0; }
};

#endif //__TERRALIB_INTERNAL_GEODATADRIVER_H
