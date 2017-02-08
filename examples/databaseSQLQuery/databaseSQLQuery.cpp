/************************************************************************************
TerraLib - a library for developing GIS applications.
Copyright � 2001-2004 INPE and Tecgraf/PUC-Rio.

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
    This file shows an example of how retrieve geometries (polygons) using an SQL query 

	Author: Lubia Vinhas  
*/

#include "../../src/terralib/drivers/MySQL/TeMySQL.h"

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
	cout << "Connection successful to the database \"" << dbname << "\" on MySQL server \"" << host << "\" !";
	cout << endl << endl;

	// Load a layer by its name 
	TeLayer *layer = new TeLayer("Distritos");
	if (!db->loadLayer(layer))                
	{                                                                                               
		cout << "Error: " << db->errorMessage() << endl << endl;
		db->close();
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	// Retrieve the information about the polygons representation
	TeRepresentation* rep = layer->getRepresentation(TePOLYGONS);
	if (!rep)
	{                                                                                               
		cout << "Layer has no polygons representation!" << endl;
		db->close();
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	string geomTableName = rep->tableName_;

	// Build the SQL to get the polygons that are inside a given box
	TeBox bb(320100,7390100,337900,7599000);
    string q;
    q = "SELECT * FROM " + geomTableName + " WHERE ";
	q +=  db->getSQLBoxWhere (bb, TePOLYGONS, geomTableName);

    // Get a portal to the database
    TeDatabasePortal *portal = db->getPortal();
    if (!portal)
    {                                                                                               
		cout << "Fail to get a database portal!\n\n";
		cout << "Press Enter\n";
		db->close();
		getchar();
		return 1;
    }

    // Submit the query and go to the first position of the portal 
    if (!portal->query(q) || !portal->fetchRow())
    {       
		cout << "Fail to submit the query or no records returned\n";
		delete portal;
		db->close();
		cout << "Press Enter\n";
		getchar();
		return 1;
   }

    // Process the polygons found
    TePolygonSet ps;
    bool flag = true;
    do
    {
        TePolygon poly;
        flag = portal->fetchGeometry(poly);
        ps.add(poly);
    }
    while (flag);   // while there is a record to be read

    delete portal;

	// Show the result
	unsigned int i, j;

	cout << "Number of polygons found: " << ps.size() << endl;
	for (i=0; i < ps.size(); ++i)
    {
		TePolygon pol = ps[i];
		cout << "Geometry id: " << pol[0].geomId() << ", object id: " << pol.objectId() << endl;
		for (j = 1; j < pol.size(); ++j)
			 cout << "\t Child id: " << pol[j].geomId() << endl;
	}


	db->close();
	cout << endl << "Press Enter\n";
	cout.flush();
	getchar();
	return 0;
}



