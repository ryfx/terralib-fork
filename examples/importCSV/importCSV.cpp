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
	This file shows how to import a MID/MIF data file into a new layer
	and a CSV file as a second attribute table of the layer

	Author: Lubia Vinhas  
*/

#include "../../src/terralib/kernel/TeAsciiFile.h"
#include "../../src/terralib/functions/TeDriverMIDMIF.h"
#include "../../src/terralib/drivers/MySQL/TeMySQL.h"

int main()
{
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
		getchar();
		return 1;
     }

	string filename = "../data/BairrosRecife.mif";
	// Check whether there is a layer with the name BairrosRecife in the database
	TeLayer *layer = 0;
	if (db->layerExist("BairrosRecife"))
	{
		cout << "The MID/MIF file was not imported because it was previously\n";
		cout << "imported into the database!\n\n";
		if (db->loadLayerSet() == true)
		{
			// Get the layer named "BairrosRecife" from the cache
			TeLayerMap& layerMap = db->layerMap();
			map<int, TeLayer*>::iterator it;
			for (it = layerMap.begin(); it != layerMap.end(); ++it)
			{
				layer = it->second;
				if (layer->name() == "BairrosRecife")
					break;
			}
		}
	}
	else
	{
		// Import the MID/MIF file
		layer = new TeLayer("BairrosRecife", db, 0);

		// Select the filed ID_ to be the identifier of each neighbourhood
		if (TeImportMIF(layer, filename,"BairrosRecife", "ID_")) 
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


	// Import the csv file as a second attribute table of the layer
	filename = "../data/BairrosRecife2.csv";
	if (db->tableExist("BairrosRecife2"))
	{
		cout << "The csv file was not imported because it was previously\n";
		cout << "imported as an attribute table into the database!\n\n";
	}
	else
	{
		cout << "Importing a second attribute table from a CSV file...\n";
	
		// Creates a definition of the columns contained in the CSV file
		TeAttributeList attList;
		TeAttribute column1;
		column1.rep_.name_  = "BAIRRO_ID";
		column1.rep_.type_ = TeSTRING;
		column1.rep_.isPrimaryKey_ = true;
		column1.rep_.numChar_ = 32;
		attList.push_back(column1);
	
		TeAttribute column2;
		column2.rep_.name_  = "ORDEM";
		column2.rep_.type_ = TeINT;
		attList.push_back(column2);
	
	
		TeAttribute column3;
		column3.rep_.name_  = "NOME";
		column3.rep_.type_ = TeSTRING;
		column3.rep_.numChar_ = 32;
		attList.push_back(column3);
	
		TeAttribute column4;
		column4.rep_.name_  = "NOME_PADRAO";
		column4.rep_.type_ = TeSTRING;
		column4.rep_.numChar_ = 32;
		attList.push_back(column4);
	
		TeTable attTable2("BairrosRecife2",attList, "BAIRRO_ID","BAIRRO_ID", TeAttrStatic);
		if (!layer->createAttributeTable(attTable2))
		{
			cout << "Fail to create the attribute table from the csv file in the TerraLib database!\n\n";
			db->close();
			delete db;
			cout << "Press Enter\n";
			getchar();
			return 1;
		}
	
		TeAsciiFile csvFile(filename);
		while (csvFile.isNotAtEOF())
		{
			TeTableRow trow;
			csvFile.readNStringCSV (trow, 4, ';');
			if (trow.empty())
				break;
	
			csvFile.findNewLine();
			attTable2.add(trow);
		}

		if (attTable2.size() > 0)
		{
			if (layer->saveAttributeTable(attTable2) == false)
			{
				cout << "Fail to save the attribute table generated\n";
				cout << "through the csv file in the database!\n\n";
				cout << "Press Enter\n";
				db->close();
				delete db;
				getchar();
				return 1;
			}
			attTable2.clear();
		}
		cout << "Table successfully imported!\n";
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
 
