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
	This file shows an example of how to duplicate a layer changing its projection.

	Author: Lubia Vinhas  
*/

#include "../../src/terralib/drivers/MySQL/TeMySQL.h"
#include "../../src/terralib/functions/TeLayerFunctions.h"

int main()
{
	// Datatabase server parameters
	string host = "localhost";
	string dbname = "DB320RC1";
	string user = "root";
	string password = "vinhas";

	// Open a connection to a MySQL database 
	TeDatabase* db = new TeMySQL();
	if (!db->connect(host, user, password, dbname))
	{
		cout << "Error: " << db->errorMessage() << endl << endl;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	cout << "Database \"" << dbname << "\" connected on MySQL \"localhost\" server!" << endl << endl;

	// Load the layer Distritos by its name 
	TeLayer* layer1 = new TeLayer("Distritos");
	if (!db->loadLayer(layer1))                
	{                                                                                               
		cout << "The layer \"Distritos\" could not be loaded from the database!\n\n";
		db->close();
		cout << "Press Enter\n";
		getchar();
		return 1;
	}
	cout << "Layer \"" << layer1->name() << "\" retrieved!" << endl;

	// Show the layer projection
	TeProjection* proj1 = layer1->projection();
	cout << "Projection/Datum of the layer \"Distritos\": " << proj1->name();
	cout << "/" << proj1->datum().name() << endl <<endl; 

	// Create a different projection
	TeDatum sad69 = TeDatumFactory::make("SAD69");
	TeProjection* proj2 = new TeLatLong(sad69);

	// Create a layer with a different projection
	TeLayer* layer2 = new TeLayer("Distritos_LL", db, proj2);

	// Copy the layer doing a remapping of its geometries
	bool res = TeCopyLayerToLayer(layer1, layer2);

	if (res)
		cout << "Layer \"Distritos\" was successfully copied to layer \"Distritos_LL\"\n\n";
	else
		cout << "Fail to remap layer!\n\n";

	db->close();
	cout << "Press Enter\n";
	getchar();
	return 0;
} 
