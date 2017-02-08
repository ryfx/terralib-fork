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
	This file shows an example of importing a shapefile that will create
	an attribute and a geometry table in a TerraLib database.

	Author: Lubia Vinhas  
*/

#include "../../src/terralib/drivers/shapelib/TeDriverSHPDBF.h"
#include "../../src/terralib/drivers/MySQL/TeMySQL.h"

using namespace std;
#include <string>

int main()
{
	// Open a connection to the "DB320RC1" TerraLib database
	// Datatabase server parameters
	string host = "localhost";
	string dbname = "DB320RC1";
	string user = "root";
	string password = "vinhas";

	TeMySQL* db = new TeMySQL();
	if (!db->connect(host, user, password, dbname))
	{
		cout << "Error: " << db->errorMessage() << endl;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	string layerName = "Brasil";

	// Check whether there is a layer with the name Brasil in the database
	TeLayer *layer = 0;
	if (db->layerExist(layerName))
	{
		cout << "The database already has an infolayer with the name \"";
		cout << layerName << "\" !" << endl << endl;
		if (db->loadLayerSet() == true)
		{
			// Get the layer named "Brasil" from the cache
			TeLayerMap& layerMap = db->layerMap();
			map<int, TeLayer*>::iterator it;
			for (it = layerMap.begin(); it != layerMap.end(); ++it)
			{
				layer = it->second;
				if (layer->name() == layerName)
					break;
			}
		}
	}
	else
	{
		// Import the shape file
		// Define a projection. It cannot be decoded from a shapefile
		TeDatum sad69 = TeDatumFactory::make("SAD69");
		TePolyconic* proj = new TePolyconic(sad69, -54.0*TeCDR);
		layer = new TeLayer(layerName, db, proj);
	
		string filename = "../data/EstadosBrasil.shp";	// Shapefile path
		string tablename = "BrasilIBGE";	// Name of the attribute table
		string linkcolumn = "SPRROTULO";	// Column that will link the attributes and geometry tables                 
	
		if (TeImportShape(layer, filename, tablename, linkcolumn))
			cout << "The shapefile was imported successfully into the TerraLib database!\n" << endl;
		else
		{
			cout << "Error: Fail to import the shapefile!\n" << endl;
			db->close();
			delete db;
			cout << "Press Enter\n";
			getchar();
			return 1;		
		}
	}

	// Show some information about the layer associated to the shape file
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

 
