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
	This file shows an example of how to create a point representation
	(centroid of polygons) to a layer in a TerraLib database.

	Author: Lubia Vinhas  
*/

#include "../../src/terralib/drivers/MySQL/TeMySQL.h"

using std::string;

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

	TeLayer* distritos = new TeLayer("Distritos");
	if (!db->loadLayer(distritos))
	{
		cout << "Fail to load layer \"Distritos\": " << db->errorMessage() << endl << endl;
		db->close();
		delete db;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	// Check whether the table of points to be generated 
	// already exists in the database
	string pointsTableName = distritos->tableName(TePOINTS);
	if (pointsTableName.empty() == false)
	{
		if (db->tableExist(pointsTableName))
		{
			cout << "The table of points \"" << pointsTableName << "\" already exists in the database!\n\n";  
			db->close();
			delete db;
			cout << "Press Enter\n";
			getchar();
			return 1;
		}
	}

	TeRepresentation* repPol = distritos->getRepresentation(TePOLYGONS);
	TePointSet centroids; 	//generate centroids
	if (db->centroid(repPol->tableName_, TePOLYGONS, centroids) == false)
	{
		cout << "Fail to create centroids: " << db->errorMessage();  
		db->close();
		delete db;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	 //  Add new representation to the layer
	distritos->addPoints(centroids);
	cout << "Centroids created!\n\n";  

	db->close();
	delete db;
	cout << "Press Enter\n";
	getchar();
	return 0;
}
