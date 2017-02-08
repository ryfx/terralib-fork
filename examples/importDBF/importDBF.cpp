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
	This file shows an example of how to import a DBF table as
	an attribute table in a TerraLib database. This attribute 
	table will be registered in the database as an external table.

	Author: Lubia Vinhas  
*/

#include "../../src/terralib/drivers/shapelib/TeDriverSHPDBF.h"
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

	// Import the dbf file
	string filename = "../data/SOCEC.dbf";
	if (db->tableExist("SOCEC"))
		cout << "There is already a table named \"SOCEC\" in the TerraLib database!\n\n";
	else
	{
		if (TeImportDBFTable(filename,db)) 
			cout << "The dbf file \"SOCEC.dbf\" was imported successfully as an external table in the TerraLib database!\n\n";
		else
		{
			db->close();
			delete db;
			cout << "Fail to import the dbf file!\n";
			cout << "Press Enter\n";
			getchar();
			return 1;
		}
	}

	TeAttrTableVector tableVec;
	if (db->getAttrTables(tableVec, TeAttrExternal))
	{
		cout << "External tables in the database: " << tableVec.size() << endl;
		TeAttrTableVector::iterator it = tableVec.begin();
		while (it != tableVec.end())
		{
			cout << (*it).name() << " primary key column: " << (*it).uniqueName() << endl << endl;
			++it;
		}
	}

	db->close();
	delete db;
	cout << "Press Enter\n";
	getchar();
	return 0;
}
 
