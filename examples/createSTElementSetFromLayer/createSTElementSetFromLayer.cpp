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
   This file illustrates an example of how to create a new Spatial Temporal Element 
   Set (STElementSet) from a layer. A Spatial Temporal Element Set can be created 
   from a layer or from a theme.
   In this example the STElementSet is created from a layer and is filled only
   with some attributes.

   Author: Karine Reis   
*/

#include "../../src/terralib/kernel/TeSTElementSet.h"
#include "../../src/terralib/kernel/TeSTEFunctionsDB.h"
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
	
	//Verify if there is the layer "EstadosBrasil"
	string layerName = "EstadosBrasil";
	if (!db->layerExist(layerName))
	{
		// Create a new layer in the database
		TeDatum sad69 = TeDatumFactory::make("SAD69");
		TePolyconic* proj = new TePolyconic(sad69, -54);

		TeLayer* layer = new TeLayer(layerName, db, proj);
		string filename = "../data/EstadosBrasil.shp";	// Shapefile path
		string tablename = "EstadosBrasil";	// Name of the attribute table

		if (TeImportShape(layer, filename, tablename))
			cout << "The shapefile \"EstadosBrasil.shp\" was imported successfully into the TerraLib database!\n" << endl;
		else
			cout << "Error: Fail to import the shapefile \"EstadosBrasil.shp\"!\n" << endl;

	}
	
	// Loads the layer
	TeLayer* estados = new TeLayer("EstadosBrasil");
	if (!db->loadLayer(estados))
	{
	    cout << "Fail to load the layer \"EstadosBrasil\": " << db->errorMessage() << endl;
		db->close();
		cout << endl << "Press Enter\n";
		getchar();
        return 1;
	}

	// Creates a elementSet from the layer "estados"
	TeSTElementSet steSet (estados);

	// Defines what attributes will be kept in the elementSet. 
	// The attribute name must be in the format "table_name.attribute_name"
	vector<string> attrs;
	attrs.push_back("EstadosBrasil.NOME_UF");
	attrs.push_back("EstadosBrasil.CAPITAL");

	// Fills the elementSet without geometries and with the 
	// attributes NOME_UF and CAPITAL 
	bool loadGeometries = false;
	bool loadAllAttributes = false;
	if(!TeSTOSetBuildDB(&steSet, loadGeometries, loadAllAttributes, attrs))
	{
		cout << "Error! " << endl;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}

	// Prints the number of elements of the elementSet
	cout << "Number of elements: " << steSet.numElements() << endl;

	//Traverse all instances of the set using iterator
	TeSTElementSet::iterator it = steSet.begin();
	while ( it != steSet.end())
	{
		TeSTInstance st = (*it);

		string desc;
		// returns the attributes
		TePropertyVector vectp = st.getPropertyVector();

		cout << "Id: " << st.objectId() << " ---------------- " << endl;
		for (unsigned int i=0; i<vectp.size(); i++)
		{
			cout << vectp[i].attr_.rep_.name_ << "  =  ";
			cout << vectp[i].value_ << endl;
		}
		cout << endl;
		++it;
	}

	db->close();
	cout << endl << "Press Enter\n";
	getchar();
	return 0;
}
