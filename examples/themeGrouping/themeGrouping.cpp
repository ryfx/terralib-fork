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
	This file illustrates how to group objects of a theme. Each group of objects 
	is associated to a legend.

	Authors: Karine Reis Ferreira 
*/

#include "../../src/terralib/kernel/TeLegendEntry.h"
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
	
	// Load a previously created theme named T_dist.
	// This theme was created on a layer of the metropolitan districts of Sao Paulo city
	TeTheme* trmsp = new TeTheme("DistritosSaoPaulo");
	if (!db->loadTheme(trmsp))
	{
	    cout << "Fail to load the theme \"T_dist\": " << db->errorMessage() << endl;
		db->close();
		cout << endl << "Press Enter\n";
		getchar();
        return 1;
	}
	trmsp->resetGrouping();		// clear any grouping previously created
	
	TeAttributeRep rep;			// grouping will be based on the attribute named "area_km2"
	rep.name_ = " area_km2 ";	// that contains the area of each metropolitan district
	rep.type_ = TeREAL;

	TeGrouping equalstep3;					// define a particular way of grouping objects
	equalstep3.groupAttribute_ = rep;
	equalstep3.groupMode_ = TeEqualSteps;	// equal steps in 3 groups
	equalstep3.groupNumSlices_ = 3;
	if (!trmsp->buildGrouping(equalstep3))	// generate groups based on the grouping definition
	{
	    cout << "Fail to generate the groups: " << db->errorMessage() << endl;
		db->close();
		cout << endl << "Press Enter\n";
		getchar();
        return 1;
	}
    
	// Associate a different visual to each group

	TeVisual visual(TePOLYGONS);		// group 1: dark green
	TeColor color(0,200,0);
	visual.color(color);
	trmsp->setGroupingVisual(1,&visual,TePOLYGONS);

	color.init(0,150,0);
	visual.color(color);		// group 2: medium green
	trmsp->setGroupingVisual(2,&visual,TePOLYGONS);

	color.init(0,100,0);	
	visual.color(color);		// group 3: light red
	trmsp->setGroupingVisual(3,&visual,TePOLYGONS);	
	
	if (!trmsp->saveGrouping())			// save grouping definition
	{
	    cout << "Fail to save the grouping: " << db->errorMessage() << endl;
		db->close();
		cout << endl << "Press Enter\n";
		getchar();
        return 1;
	}
		
	if (!trmsp->saveLegendInCollection())			// save grouping definition
	{
	    cout << "Fail to save the grouping: " << db->errorMessage() << endl;
		db->close();
		cout << endl << "Press Enter\n";
		getchar();
        return 1;
	}
	db->close();
	cout << "Theme was successfully grouped! " << endl;
	cout << endl << "Press Enter\n";
    getchar();
	return 0;
}

