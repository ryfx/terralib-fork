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
	This file shows an example of how to create a TerraLib database called "DB320RC1"
	in an MySQL DBMS Server running in the local machine, supposing there is an
	user called "root" with no password.

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

	// Creates a new database
	TeDatabase* db = new TeMySQL();
	if (db->newDatabase(dbname, user, password, host) == false)
	{
		cout << "Error: " << db->errorMessage() << endl;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}
	cout << "The database \"" << dbname;
	cout << "\" was created successfully in the MySQL server located in \"" << host;
	cout << "\" for the user named \"" << user << "\" !"  << endl;

	// Close database
	db->close();
	delete db;
	cout << endl << "Press Enter\n";
	getchar();
	return 0;
}



 
