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
	This file shows an example of how to build a mosaic of two images  
	in GeoTiff format in a layer of a TerraLib database.

	Author: Lubia Vinhas  
*/

#include "../../src/terralib/kernel/TeDatabase.h"
#include "../../src/terralib/drivers/MySQL/TeMySQL.h"
#include "../../src/terralib/kernel/TeInitRasterDecoders.h"
#include "../../src/terralib/kernel/TeImportRaster.h"

int main()
{
	// Initialize the raster decoder tool
	TeInitRasterDecoders();

	// Access the input images
	TeRaster img1("../data/nat1.tif");
	if (!img1.init())
	{
		cout << "Cannot access the first input image!" << endl << endl;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	TeRaster img2("../data/nat2.tif");
	if (!img2.init())
	{
		cout << "Cannot access the second input image!" << endl << endl;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

    // Database server parameters
     string host = "localhost";
     string dbname = "DB320RC1";
     string user = "root";
     string password = "vinhas";

     // Connect to the database
     TeDatabase* db = new TeMySQL();
     if (!db->connect(host, user, password, dbname))
     {
		cout << "Error: " << db->errorMessage() << endl << endl;
		cout << "Press Enter\n";
		return 1;
     }

	// Creates a layer to receive the raster geometry (same projection as raster data)
	string layerName = "NatividadeMosaic";

	// Check whether there is a layer with this name in the database
	if (db->layerExist(layerName))
	{
		db->close();
		cout << "The database already has an infolayer with the name \"";
		cout << layerName << "\"!" << endl << endl;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	TeLayer* layer = new TeLayer(layerName, db, img1.projection());
	if (layer->id() <= 0)
	{
		db->close();
		cout << "The destination layer could not be created!\n" << db->errorMessage() << endl;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	// Import the first image to the layer
	if (!TeImportRaster(layer,&img1,256,256,TeRasterParams::TeNoCompression,"",255,true,TeRasterParams::TeExpansible))
	{
		db->close();
		cout << "Fail to import the first image\n\n!";
		cout << "Press Enter\n";
		getchar();
		return 1;
	}
	else
		cout << "The first image was imported successfully!\n\n"; 

	delete layer;
	layer = 0;

	layer = new TeLayer(layerName, db);

	// Mosaic the second raster to the same layer
	if (!TeImportRaster(layer, &img2, 256, 256, TeRasterParams::TeNoCompression, "", 255, true, TeRasterParams::TeExpansible))
	{
		db->close();
		cout << "Fail to import the second image\n\n!";
		cout << "Press Enter\n";
		getchar();
		return 1;
	}
	else
		cout << "The second image was added successfully!" << endl << endl;

	// Close the database
	db->close();
	cout << "\nPress enter...";
	getchar();
	return 0;
}
 
