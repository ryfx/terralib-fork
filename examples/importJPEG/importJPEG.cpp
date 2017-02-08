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
	This file shows an example of how to import a raster data (an image in JPEG
	format) to a TerraLib database.

	Author: Lubia Vinhas  
*/

#include "../../src/terralib/kernel/TeDatabase.h"
#include "../../src/terralib/drivers/MySQL/TeMySQL.h"
#include "../../src/terralib/kernel/TeImportRaster.h"
#include "../../src/terralib/kernel/TeDecoderJPEG.h"

int main()
{

	// Creates an instance of the JPEG raster decoder
	static TeDecoderJPEGFactory theDecoderJPEGFactory("JPEG");
	TeDecoderFactory::instanceName2Dec()["jpg"]  = "JPEG";	
	TeDecoderFactory::instanceName2Dec()["jpeg"] = "JPEG";
	TeDecoderFactory::instanceName2Dec()["JPG"]  = "JPEG";	
	TeDecoderFactory::instanceName2Dec()["JPEG"] = "JPEG";

	// Acess input image
	TeRaster image("../data/sampa.jpg");
	if (!image.init())
	{
		cout << "Cannot access input image!" << endl << endl;
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
		image.clear();
		cout << "Error: " << db->errorMessage() << endl << endl;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	string layerName = "SampaJPEG";

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
	TeLayer* layer = new TeLayer(layerName, db, image.projection());
	if (layer->id() <= 0)
	{
		image.clear();
		db->close();
		delete db;
		cout << "The destination layer could not be created!\n" << db->errorMessage() << endl << endl;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	// Import raster to the layer
	if (!TeImportRaster(layer, &image, 128, 128))
	{
		image.clear();
		db->close();
		delete db;
		cout << "Fail to import image!\n" << endl;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	// Close database
	db->close();
	delete db;
	cout << "The JPEG image was imported successfully into the TerraLib database!\n\n";
	cout << "Press Enter\n";
	getchar();
	return 0;
}
 
