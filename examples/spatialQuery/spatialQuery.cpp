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
	This file shows an example of how to use the database interface to do some
	spatial queries involving objects with points, lines and polygon geometries.

	Authors: 
		Karine Reis Ferreira  
		Lubia Vinhas
*/

#include "../../src/terralib/drivers/MySQL/TeMySQL.h"
#include "../../src/terralib/drivers/shapelib/TeDriverSHPDBF.h"

int main()
{
 	// Datatabase server parameters
	string host = "localhost";
	string dbname = "DB320RC1";
	string user = "root";
	string password = "vinhas";

	// Open a connection to the DB320RC1 MySQL database 
	TeDatabase* db = new TeMySQL();
	if (!db->connect(host, user, password, dbname))
	{
		cout << "Error: " << db->errorMessage() << endl;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}
	cout << "Connection successful to the database \"" << dbname << "\" on MySQL server \"" << host << "\" !\n";;

	// Check whether there is a layer of polygons called "Mapa_Distritos_SP".
	// In negative case, import it from the Mapa_Distritos_SP.shp file
	string layerName = "Mapa_Distritos_SP";

	TeDatum sad69 = TeDatumFactory::make("SAD69");
	TePolyconic* proj = new TePolyconic(sad69, -54*TeCDR);

	if (db->layerExist(layerName) == false)
	{
		// Create a new layer in the database
		TeLayer* layer = new TeLayer(layerName, db, proj);
		string filename = "../data/Mapa_Distritos_SP.shp";	// Shapefile path
		string tablename = "Mapa_Distritos_SP";	// Name of the attribute table

		if (TeImportShape(layer, filename, tablename))
			cout << "The shapefile \"Mapa_Distritos_SP.shp\" was imported successfully into the TerraLib database!\n" << endl;
		else
			cout << "Error: Fail to import the shapefile \"Mapa_Distritos_SP.shp\"!\n" << endl;
	}	

	// Check whether there is a layer of lines called "Mapa_Drenagem_SP".
	// In negative case, import it from the Mapa_Drenagem_SP.shp file
	layerName = "Mapa_Drenagem_SP";

	if (db->layerExist(layerName) == false)
	{
		// Create a new layer in the database
		TeLayer* layer = new TeLayer(layerName, db, proj);
		string filename = "../data/Mapa_Drenagem_SP.shp";	// Shapefile path
		string tablename = "Mapa_Drenagem_SP";	// Name of the attribute table

		if (TeImportShape(layer, filename, tablename))
			cout << "The shapefile \"Mapa_Drenagem_SP.shp\" was imported successfully into the TerraLib database!\n" << endl;
		else
			cout << "Error: Fail to import the shapefile \"Mapa_Drenagem_SP.shp\"!\n" << endl;
	}

	// Check whether there is a layer of points called "Mapa_Bairros_SP".
	// In negative case, import it from the Mapa_Bairros_SP.shp file
	layerName = "Mapa_Industrias_SP";

	if (db->layerExist(layerName) == false)
	{
		// Create a new layer in the database
		TeLayer* layer = new TeLayer(layerName, db, proj);
		string filename = "../data/Mapa_Industrias_SP.shp";	// Shapefile path
		string tablename = "Mapa_Industrias_SP";	// Name of the attribute table

		if (TeImportShape(layer, filename, tablename))
			cout << "The shapefile \"Mapa_Industrias_SP.shp\" was imported successfully into the TerraLib database!\n" << endl;
		else
			cout << "Error: Fail to import the shapefile \"Mapa_Industrias_SP.shp\"!\n" << endl;
	}

	// Retrieve from the database a layer of polygons, a layer of lines and a layer of points
    TeLayer* regions = new TeLayer("Mapa_Distritos_SP");		// regions have polygons
    if (!db->loadLayer(regions))                
    {                                                                                               
        cout << "Fail to load layer \"Mapa_Distritos_SP\": " << db->errorMessage() << endl;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
    }

    TeLayer* rivers = new TeLayer("Mapa_Drenagem_SP");	// rivers have lines
    if (!db->loadLayer(rivers))                
    {                                                                                               
        cout << "Fail to load layer \"Mapa_Drenagem_SP\": " << db->errorMessage() << endl;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
    }

	TeLayer* industries = new TeLayer("Mapa_Industrias_SP");	// districts have points
    if (!db->loadLayer(industries))                
    {                                                                                               
        cout << "Fail to load layer \"Mapa_Industrias_SP\": " << db->errorMessage() << endl;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
    }

	//get precision from projection
	TePrecision::instance().setPrecision(TeGetPrecision(proj));	
	
	vector<string> objsOut;	// holds the identification of the resulting objects
	
	vector<string> objsIn;  // objects to be queried		
	objsIn.push_back("48");

	// Retrieve the regions that are adjacent or touch the district 48
	bool res = db->spatialRelation(regions->tableName(TePOLYGONS), TePOLYGONS, objsIn,
		                      objsOut, TeTOUCHES);
	if (res)
	{
		cout << "Regions that touch the region \"48\": \n";
		unsigned int i;
		for (i=0; i<objsOut.size(); i++)
			cout << "Regions: " << objsOut[i] << endl;

	}

	// Retrieve the rivers that cross the district 48
	res = db->spatialRelation(regions->tableName(TePOLYGONS), TePOLYGONS, objsIn,
		                            rivers->tableName(TeLINES), TeLINES, objsOut, TeCROSSES);
	if (res)
	{
		cout << "\nRivers that cross the region \"48\": \n";
		unsigned int i;
		for (i=0; i<objsOut.size(); i++)
			cout << "River: " << objsOut[i] << endl;
	}

	// Retrieve the industries that are within the district 48
	res = db->spatialRelation(regions->tableName(TePOLYGONS), TePOLYGONS, objsIn,
		                      industries->tableName(TePOINTS), TePOINTS, objsOut, TeWITHIN);
	if (res)
	{
		cout << "\nIndustries that are within the region \"48\": \n";
		unsigned int i;
		for (i=0; i<objsOut.size(); i++)
			cout << "Industry: " << objsOut[i] << endl;

	}

	db->close();
	cout << endl << "Press Enter\n";
	cout.flush();
	getchar();
	return 0;
}

