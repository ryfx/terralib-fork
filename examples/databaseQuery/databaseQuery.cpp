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
        This file shows an example of how to retrieve an existing layer from a
        TerraLib database, and execute some queries on its polygon geometry table.

        Author: Lubia Vinhas  
*/

#include "../../src/terralib/drivers/MySQL/TeMySQL.h"

int main()
{
	unsigned int i, j;
	// Datatabase server parameters
	string host = "localhost";
	string dbname = "DB320RC1";
	string user = "root";
	string password = "vinhas";

	// Open a connection to a MySQL database
	TeDatabase* db = new TeMySQL();
	if (!db->connect(host, user, password, dbname))
	{
		cout << "Error: " << db->errorMessage() << endl;
		cout << endl << "Press Enter\n";
		cout.flush();
		getchar();
		return 1;
	}
	cout << "Connection successful to the database \"" << dbname << "\" on MySQL server \"" << host << "\"!";
	cout << endl << endl;

	// Load a layer by its name 
	TeLayer *layer = new TeLayer("Distritos");
	if (!db->loadLayer(layer))                
	{                                                                                               
		cout << "Error: " << db->errorMessage() << endl;
		db->close();
		cout << endl << "Press Enter\n";
		cout.flush();
		getchar();
		return 1;
	}

	// Retrieve the information about the polygons representation
	TeRepresentation* rep = layer->getRepresentation(TePOLYGONS);
	if (!rep)
	{                                                                                               
		cout << "Layer has no polygons representation!" << endl;
		db->close();
		cout << endl << "Press Enter\n";
		cout.flush();
		getchar();
		return 0;
	}

	string polyTableName = rep->tableName_;
	TePolygonSet ps;
	// Retrieve all polygons related to the object with id "33"
	if (db->loadPolygonSet(polyTableName, "33", ps) && ps.size() > 0)
	{
		cout << "Polygon identifiers related to the object id \'33\': " << endl;
		for (i = 0; i < ps.size(); ++i)
		{
			TePolygon pol = ps[i];
			cout << "Parent id: " << pol[0].geomId() << endl;
			for (j = 1; j < pol.size(); ++j)
				cout << "\t Child id: " << pol[j].geomId() << endl;
		}
	}
	else
	{
		cout << "No polygons related to the object \'33\' were found!" << endl << endl;
	}
	ps.clear();

	// Retrieve all polygons that intersect a certain bounding box
	// given in the same projection coordinates of the data
	TeBox box(320000,7390000,338000,7400000);
	if (db->loadPolygonSet(polyTableName, box, ps) && ps.size() > 0)
	{
		cout << "\nPolygons that intersect the bounding box: (320000,7390000,338000,7400000)" << endl;
		cout << "Number of polygons found: " << ps.size() << endl;
		for (i = 0; i < ps.size(); ++i)
		{
			TePolygon pol = ps[i];
			cout << "Parent id: " << pol[0].geomId() << ", object id: " << pol.objectId() << endl;
			for (j = 1; j < pol.size(); ++j)
				cout << "\t Child id: " << pol[j].geomId() << endl;
		}
	}
	else
		cout << "No polygons intersect the bounding box (320000,7390000,338000,7400000)!" << endl << endl;

	//! Retrieving polygons that contain a certain point
	TePolygon poly;
	cout << endl << "Polygons that contain the point (350000,7395000):" << endl;
	TeCoord2D pt(350000,7395000);
	if (db->locatePolygon(polyTableName, pt,poly,0))
		cout << "Polygon id: " << poly.geomId() << ", object id: " << poly.objectId() << endl;
	else
	{
		cout << "\nNo polygons found!\n\n";
		db->close();
		cout << endl << "Press Enter\n";
		cout.flush();
		getchar();
		return 0;
	}

	db->close();
	cout << endl << "Press Enter\n";
	cout.flush();
	getchar();
	return 0;
}



 
