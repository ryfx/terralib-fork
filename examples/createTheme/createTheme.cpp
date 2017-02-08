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
	This file illustrates how to create themes in a TerraLib database

	Authors: Karine Reis Ferreira and Lubia Vinhas  
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

	// Load the layer "Distritos" which contains data of the districts of the Sao Paulo city
	TeLayer* dist = new TeLayer("Distritos");
	if (!db->loadLayer(dist))
	{
		cout << "Fail to load the layer \"Distritos\": " << db->errorMessage() << endl;
		db->close();
		delete db;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}

	TeProjection* proj = TeProjectionFactory::make(dist->projection()->params()); 

	// Create a view with the same projection of the layer
	string viewName = "SaoPaulo";

	// Check whether there is a view with this name in the datatabase
	if (db->viewExist(viewName) == true)
	{
		cout << "There is already a view named \"SaoPaulo\" in the database\n";
		db->close();
		delete db;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}

	TeView* view = new TeView(viewName, user); 
	view->projection(proj);
	if (!db->insertView(view))		// save the view in the database
	{
		cout << "Fail to insert the view \"SaoPaulo\" into the database: " << db->errorMessage() << endl;
		db->close();
		delete db;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}

	// Create a theme that will contain all of the objects of the layer (no restrictions applied)
	TeTheme* theme = new TeTheme("DistritosSaoPaulo", dist);
	view->add(theme);
		
	// Set a default visual for the geometries of the objects of the layer 
	// Polygons will be set with the blue color
	TeColor color;
	color.init(0,0,255);
	TeVisual* polygonVisual = TeVisualFactory::make("tevisual");
	polygonVisual->color(color); 

	// Points will be set with the red color
	color.init(255,0,0);
	TeVisual* pointVisual = TeVisualFactory::make("tevisual");
	pointVisual->color(color);
   	pointVisual->style(TePtTypeX);

    theme->setVisualDefault(polygonVisual, TePOLYGONS);
    theme->setVisualDefault(pointVisual, TePOINTS);

	// Set all of the geometrical representations to be visible
	int allRep = dist->geomRep();
	theme->visibleRep(allRep);

	// Set the attribute tables of the theme equal the tables of the layer 
	theme->setAttTables(dist->attrTables());

	// Save the theme in the database
    if (!theme->save())	
    {
		cout << "Fail to save the theme \"DistritosSaoPaulo\" in the database: " << db->errorMessage() << endl;
		db->close();
		delete db;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}

	// Build the collection of objects associated to the theme
	if (!theme->buildCollection())
	{
		cout << "Fail to build the theme \"DistritosSaoPaulo\": " << db->errorMessage() << endl;
		db->close();
		delete db;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}
	cout << "The theme \"DistritosSaoPaulo\" was created without restrictions!\n";

	// Create a theme with the following attribute restriction:
	// Districts with population higher than 100,000 people
	TeTheme* themeRest = new TeTheme("Pop91GT100000", dist);
	themeRest->setAttTables (dist->attrTables());
	
	// Set the attribute restriction
	string restAttr = " Pop91 > 100000 ";
	themeRest->attributeRest(restAttr);

	// Set all of the geometrical representations to be visible
	themeRest->visibleRep(allRep);

	TeVisual* polygonVisual2 = TeVisualFactory::make("tevisual");
	polygonVisual2->color(color); 

   	TeVisual* pointVisual2 = TeVisualFactory::make("tevisual");
	pointVisual2->color(color);
   	pointVisual2->style(TePtTypeX);

	// Set the visual
	themeRest->setVisualDefault(polygonVisual2, TePOLYGONS);
	themeRest->setVisualDefault(pointVisual2, TePOINTS);

	// Insert the theme into the view
	view->add(themeRest);

	// Save the theme in the database
	if (!themeRest->save())
    {
		cout << "Fail to save the theme \"Pop91GT100000\" in the database: " << db->errorMessage() << endl;
		db->close();
		delete db;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}

	// Build the collection of objects associated to the theme
	if (!themeRest->buildCollection())
	{
		cout << "Fail to build the theme \"Pop91GT100000\": " << db->errorMessage() << endl;
		db->close();
		delete db;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}

	cout << "The theme  \"Pop91GT100000\" was created with attribute restrictions!\n\n";
	delete db;
	cout << endl << "Press Enter\n";
   	getchar();
	return 0;
}


