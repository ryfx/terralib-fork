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
	This file shows an example of how to import some geographical data
	in MID/MIF format into a TerraLib database.

	Author: Lubia Vinhas  
*/

#include "../../src/terralib/functions/TeDriverMIDMIF.h"
#include "../../src/terralib/drivers/MySQL/TeMySQL.h"

int main()
{
     // Datatabase server parameters
     string host = "localhost";
     string dbname = "DB320RC1";
     string user = "root";
     string password = "vinhas";

     // Connects to database
     TeDatabase* db = new TeMySQL();
     if (!db->connect(host, user,password, dbname))
     {
		cout << "Error: " << db->errorMessage() << endl;
		cout << "Press Enter\n";
		getchar();
		delete db;
		return 1;
     }

	TeLayer *layer = 0;
	string filename = "../data/Distritos.mif";
	if (db->layerExist("Distritos"))
	{
		cout << "Warning: Layer \"Distritos\" already exists in the database!\n" << endl;
		if (db->loadLayerSet() == true)
		{
			// Get layer named "Distritos" from the cache
			TeLayerMap& layerMap = db->layerMap();
			map<int, TeLayer*>::iterator it;
			for (it = layerMap.begin(); it != layerMap.end(); ++it)
			{
				layer = it->second;
				if (layer->name() == "Distritos")
					break;
			}
		}
	}
	else
	{
		// Import the MID/MIF file
		layer = TeImportMIF(filename, db);                          
		if (layer) 
			cout << "MID/MIF file imported to the TerraLib database successfully!\n" << endl;
		else
		{
			cout << "Fail to import the MID/MIF file!\n";
			db->close();
			delete db;
			cout << "Press Enter\n";
			getchar();
			return 1;
		}
	}

	// Show some information about the layer associated to the MID/MIF file
	cout << "Layer: " << layer->name() << ", id: " << layer->id() << endl;

	// Geometries information
	cout << "Projection: " << layer->projection()->name();              
	cout << "/" << layer->projection()->datum().name();          
	TeBox bb = layer->box();
	char message[255];
	sprintf(message,"[%.15f,%.15f,%.15f,%.15f]\n",bb.x1_, bb.y1_, bb.x2_, bb.y2_);
	cout << "\nBounding box: " << message;
	cout << "Number of polygons: " << layer->nGeometries(TePOLYGONS) << endl;
	cout << "Number of lines: "    << layer->nGeometries(TeLINES) << endl;
	cout << "Number of points: "   << layer->nGeometries(TePOINTS) << endl;

	// Information of the Tables of Attributes of the Layer
	TeAttrTableVector tableVec;
	layer->getAttrTables(tableVec);
	cout << "Number of Attribute Tables: " << tableVec.size() << endl;
	
	for (unsigned int i = 0; i < tableVec.size(); ++i)
	{
		cout << "Attribute Table " << "\"" << tableVec[i].name() << "\": ";
		cout << layer->nObjects(tableVec[i].name()) << " objects" << endl << endl;
	}
	
	db->close();
	delete db;
	cout << "Press Enter\n";
	getchar();
	return 0;
}

 
