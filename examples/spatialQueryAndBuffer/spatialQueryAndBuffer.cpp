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
   This file shows an example of how to use the database interface to do some
   spatial queries involving objects with points, lines and polygon geometries,
   and to generate a buffer operation.

   Author: Karine Reis   
*/

#include "../../src/terralib/drivers/MySQL/TeMySQL.h"

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
		
	// Retrieve from the database the layer "Mapa_Distritos_SP"
    TeLayer* regions = new TeLayer("Mapa_Distritos_SP");		
    if (!db->loadLayer(regions))                
    {                                                                                               
        cout << "Fail to load layer \"Mapa_Distritos_SP\": " << db->errorMessage() << endl;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
    }

    // Retrieve from the database the layer "Mapa_Industrias_SP"
	TeLayer* industries = new TeLayer("Mapa_Industrias_SP");	
    if (!db->loadLayer(industries))                
    {                                                                                               
        cout << "Fail to load layer \"Mapa_Industrias_SP\": " << db->errorMessage() << endl;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
    }

	TePrecision::instance().setPrecision(TeGetPrecision(regions->projection()));

	// holds the identification of the resulting objects
	vector<string> objsOut;	
	vector<string> objsIn;  // objects to be queried		
	objsIn.push_back("48");
	
	// Generate a buffer (polygon) around 1000 meters distance from the element "48"
	TePolygonSet bufferPol;
	if (!db->buffer(regions->tableName(TePOLYGONS), TePOLYGONS,
                     objsIn, bufferPol, 1000))
	{
		cout << "Fail to generate buffer!" << endl;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}
	
	// Retrieve the industries that are within the buffer generated
	if(!db->spatialRelation(industries->tableName(TePOINTS), TePOINTS, &bufferPol, objsOut, TeWITHIN)) 
	{
		cout << "Fail to retrieve districts!" << endl;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}

	//Shows the result
	cout << " Retrieved industries:" << endl << endl;
	for(unsigned int i=0; i<objsOut.size(); i++)
		cout << "      " << objsOut[i] << endl;

	
	db->close();
	cout << endl << "Press Enter\n";
	getchar();
	return 0;
};
