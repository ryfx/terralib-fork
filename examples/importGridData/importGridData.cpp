/************************************************************************************
TerraLib - a library for developing GIS applications.
Copyright © 2001-2004 INPE and Tecgraf/PUC-Rio.

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
	This file shows an example of how to import, to a TerraLib database, a raster data: 
	a binary raw grid data. Each point of the grid is a float value that represents the 
	elevation over an area 

	Author: Lubia Vinhas  
*/

#include "../../src/terralib/kernel/TeDatabase.h"
#include "../../src/terralib/drivers/MySQL/TeMySQL.h"
#include "../../src/terralib/kernel/TeDecoderMemoryMap.h"
#include "../../src/terralib/kernel/TeImportRaster.h"

int main()
{
	// Initialize the raster decoder tool
	static TeDecoderMemoryMapFactory theDecoderMemoryMapFactory("MEMMAP");
	TeDecoderFactory::instanceName2Dec()["raw"]  = "MEMMAP";	
	TeDecoderFactory::instanceName2Dec()["RAW"]  = "MEMMAP";	

	TeRasterParams parRaw;
	parRaw.nBands(1);
	parRaw.setDataType(TeFLOAT);
	parRaw.fileName_ = "../data/elevation.raw";
	parRaw.mode_ = 'r';
	TeProjection* proj = new TeNoProjection();
	parRaw.projection(proj);
	parRaw.lowerLeftResolutionSize(183557.0,8246277.0,30,30,382,422,false);

	// Acess input image
	TeRaster elev(parRaw);
	if (!elev.init())
	{
		cout << "Cannot access input grid!" << endl << endl;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

 	// Datatabase server parameters
	string host = "localhost";
	string dbname = "DB320RC1";
	string user = "root";
	string password = "vinhas";
	TeDatabase* db = new TeMySQL();
	if (!db->connect(host, user, password, dbname))
	{
		elev.clear();
		cout << "Error: " << db->errorMessage() << endl << endl;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	string layerName = "Elevation";

	// Check whether there is a layer with this name in the database
	if (db->layerExist(layerName))
	{
		cout << "The database already has an infolayer with the name \"";
		cout << layerName << "\"!" << endl << endl;
		db->close();
		delete db;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	// Create a layer to receive the raster geometry (same projection as raster data)
	TeLayer* layer = new TeLayer(layerName, db, elev.projection());
	if (layer->id() <= 0)
	{
		elev.clear();
		db->close();
		delete db;
		cout << "The destination layer could not be created!\n" << db->errorMessage() << endl << endl;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	// Import raster to the layer
	if (!TeImportRaster(layer, &elev, parRaw.ncols_, 2))
	{
		elev.clear();
		db->close();
		delete db;
		cout << "Fail to import grid!\n" << endl;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	// Close database
	db->close();
	delete db;
	cout << "The GRID data was imported successfully into the TerraLib database!\n\n";
	cout << "Press Enter\n";
	getchar();
	return 0;
}
 
