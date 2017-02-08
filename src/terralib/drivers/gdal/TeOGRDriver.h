/************************************************************************************
TerraLib - a library for developing GIS applications.
Copyright  2001-2007 INPE and Tecgraf/PUC-Rio.

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

/** \file  TeOGRDriver.h
    \brief This file contains a concrete geo data driver based on OGR Library.
    \author Douglas Uba <douglas@dpi.inpe.br>
   */

#ifndef _TEOGRDRIVER_H
#define _TEOGRDRIVER_H

// Defines
#include "TeGDALDefines.h"
#include "ogr_core.h"

// TerraLib include files
#include "../../kernel/TeGeoDataDriver.h"
#include "../../kernel/TeBox.h"
#include "../../kernel/TeDataTypes.h"
#include "../../kernel/TeAttribute.h"
#include "../../kernel/TeTable.h"
#include "../../kernel/TeAttribute.h"

// STL include files
#include <vector>
#include <string>

// forward declarations;
class TeProjection;
class TeSTElementSet;
class OGRDataSource;
class OGRLayer;
class TeLayer;

class TEGDAL_DLL TeOGRDriver : public TeGeoDataDriver 
{
public:
  
	TeOGRDriver(const std::string& fileName, const bool &writing=false, const std::string &OGRDriverName="GML");

    ~TeOGRDriver();

	//! Returns the complete access path associated to this driver
	virtual std::string getFileName();

	//! Returns true or false whether the data file is accessible
	virtual bool isDataAccessible();

	//! Returns the spatial reference associated to the data as an instance of TeProjection 
	virtual TeProjection * getDataProjection();

	//! Loads the minimum metadata information about the data
	virtual bool getDataInfo(unsigned int& nObjects, TeBox& ext, TeGeomRep& repres);

	//! Loads the list of descriptive attributes of the objetcts represented in the file
	virtual bool getDataAttributesList(TeAttributeList& attList);

	//! Loads the data into an TeSTElementSet structure in memory
	virtual bool loadData(TeSTElementSet* dataSet);

	//! Gets the next OGR Element
	virtual bool loadNextElement(TeSTElementSet* dataSet);

	//! Gets the names of registered OGR drivers.
	virtual void getDriversNames(std::vector<std::string>& dnames);

	//! Create a new layer to OGR format
	virtual bool createLayer(TeLayer* layer, const int& rep = 255); 

	//! Create a new table to OGR Layer
	virtual bool createAttributeTable(TeTable& table);

	//! Sets geographic elements to ogr layer
	virtual bool addElement(TeSTInstance* e);

protected:

	OGRDataSource* ogrDS_;  //!< A pointer to OGR Data Source.
	OGRLayer* ogrLayer_;	//!< A pointer to first OGR Layer on DataSource. // TODO: revision is necessary.
	std::string fileName_;  //!< Complete access path.
	TeBox box_;				//!< Data set box.
	unsigned int numObjs_;  //!< Number of features in data set.
	TeGeomRep geomType_;    //!< Geometric representation of data set.

private:
  
    TeOGRDriver(const TeOGRDriver& source);

    TeOGRDriver & operator=(const TeOGRDriver & source);
};

#endif // _TEOGRDRIVER_H
