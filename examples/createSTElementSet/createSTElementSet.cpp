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
   Set (STElementSet) from a theme. A Spatial Temporal Element Set can be created 
   from a layer or from a theme.
   In this example the STElementSet is created from a layer and is filled only
   with attributes.

   Author: Karine Reis   
*/

#include "../../src/terralib/kernel/TeQuerierParams.h"
#include "../../src/terralib/kernel/TeQuerier.h"
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
	
	// Load a previously created theme named DistritosSaoPaulo.
	// This theme was created on a layer of the metropolitan districts of Sao Paulo city
	TeTheme* trmsp = new TeTheme("DistritosSaoPaulo");
	if (!db->loadTheme(trmsp))
	{
		cout << "Fail to load the theme \"DistritosSaoPaulo\": " << db->errorMessage() << endl;
		db->close();
		cout << endl << "Press Enter\n";
		getchar();
	        return 1;
	}

	// Defines the attributes that will be kept in the STElementSet. 
	// The attribute name must be in the format "table_name.attribute_name"
	vector<string> attrs;
	attrs.push_back("Distritos.Nome_Distrito");
	attrs.push_back("Distritos.Pop2000");

	// Define the parameters of the querier
	TeQuerierParams qpar(false, attrs);
	qpar.setParams(trmsp);

	// Define a querier to the database
	TeQuerier qdb(qpar);
	qdb.loadInstances();

	// Show how many instances the querier found
	cout << "Number of instances: " << qdb.numElemInstances() << endl;

	// Loop through all of the instances
	TeSTInstance steInstance;
	while (qdb.fetchInstance(steInstance))
	{
		string nome, pop;
		steInstance.getPropertyValue("Nome_distrito", nome);
		steInstance.getPropertyValue("Pop2000", pop);
		
		cout << " Object Identifier :  "<< steInstance.objectId() << endl; 
		cout << " Nome Distrito     :  "<< nome  << endl; 
		cout << " Pop2000           :  "<< pop  << endl << endl; 
	}

	db->close();
	cout << endl << "Press Enter\n";
	getchar();
	return 0;
}
