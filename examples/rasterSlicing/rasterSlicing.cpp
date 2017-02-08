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
	This file shows an example of how to create a legend over a raster data,
	stored in a layer TerraLib.

	Author: Lubia Vinhas  
*/

#include "../../src/terralib/kernel/TeRaster.h"
#include "../../src/terralib/drivers/MySQL/TeMySQL.h"

int main()
{
 	// Datatabase server parameters
	string host = "localhost";
	string dbname = "DB320RC1";
	string user = "root";
	string password = "vinhas";
	TeDatabase* db = new TeMySQL();
	if (!db->connect(host, user, password, dbname))
	{
		cout << "Error: " << db->errorMessage() << endl << endl;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}


	// Retrieve a layer with a raster representation
	TeLayer* layer = new TeLayer("Elevation",db);
	if (layer->id() < 1)
	{
		cout << "Cannot access layer Elevation " << endl;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}

	// Create a new view with the same projection as the layer
	TeView* view = new TeView("ViewElevation", user);
	view->projection(layer->projection());
	if (!db->insertView(view)) 
	{
		 cout << "Could not insert view into the database: " << db->errorMessage() << endl;
		 db->close();
		 cout << endl << "Press Enter\n";
		 getchar();
		 return 1;
	}

	// Create a new theme and a legend over it
	TeAbstractTheme* rstTheme = new TeTheme("Elevation", layer);

	rstTheme->visibleRep(TeRASTER | 0x40000000);	// make visible: the raster representation
													// and the legend (0x4000000)
	view->add(rstTheme);							// add theme to the view
	if (!rstTheme->save())							// save theme definition to the database
	{
		 cout << "Error saving theme: " << db->errorMessage() << endl;
		 db->close();
		 cout << endl << "Press Enter\n";
		 getchar();
		 return 1;
	}

	// Define red, green and blue components to build a 10 color pallete
	short red[10] =   { 0, 45, 99, 160, 230, 232, 235, 237, 240, 242 };
	short green[10] = { 166, 182, 198, 214, 230, 195, 178, 180, 201, 242 };
	short blue[10]  = { 0, 0, 0, 0, 0, 46, 94, 142, 192, 242 };

	// Define a slicing mode: 10 slices
	TeGrouping group1;
	group1.groupMode_ = TeRasterSlicing;
	group1.groupNumSlices_ = 10;
	group1.groupPrecision_ = 8;

	
	// Default way of slicing a raster representation: slices of equal size
	if (!rstTheme->buildGrouping(group1)) 
	{
		 cout << "Error grouping theme: " << db->errorMessage() << endl;
		 db->close();
		 cout << endl << "Press Enter\n";
		 getchar();
		 return 1;
	}

	// Assign to each slice one color of the pallete
	int ng;
	for (ng=0; ng<rstTheme->grouping().groupNumSlices_; ng++)
	{
		TeColor color(red[ng],green[ng],blue[ng]);
		TeVisual* visual = TeVisualFactory::make("tevisual");            
		visual->color(color);
		visual->transparency(rstTheme->defaultLegend().visual(TePOLYGONS)->transparency());
		visual->contourStyle(rstTheme->defaultLegend().visual(TePOLYGONS)->contourStyle());
		visual->contourWidth(rstTheme->defaultLegend().visual(TePOLYGONS)->contourWidth());
		visual->contourColor(rstTheme->defaultLegend().visual(TePOLYGONS)->contourColor());	
		rstTheme->setGroupingVisual(ng+1,visual,TePOLYGONS);
	}

	// Saves the theme legend to the database
	if (!rstTheme->saveGrouping())
	{
		 cout << "Error saving theme slicing: " << db->errorMessage() << endl;
		 db->close();
		 cout << endl << "Press Enter\n";
		 getchar();
		 return 1;
	}
	cout << "Created first theme...\n";
	delete rstTheme;
	
	// Generates a new theme with slices arbitrarily defined
	
	rstTheme = new TeTheme("Elevation4Slices", layer);
	rstTheme->visibleRep(TeRASTER | 0x40000000);	// make visible: the raster representation
													// and the legend (0x4000000)
	view->add(rstTheme);							// add theme to the view
	if (!rstTheme->save())							// save theme definition to the database
	{
		 cout << "Error saving theme: " << db->errorMessage() << endl;
		 db->close();
		 cout << endl << "Press Enter\n";
		 getchar();
		 return 1;
	}

	vector<TeSlice> mySlices;
	TeSlice sl1("980","1000"); 
	mySlices.push_back(sl1);
	TeSlice sl2("1000","1020"); 
	mySlices.push_back(sl2);
	TeSlice sl3("1020","1100"); 
	mySlices.push_back(sl3);
	TeSlice sl4("1100","1200"); 
	mySlices.push_back(sl4);
	 
	TeGrouping group2;
	group2.groupMode_ = TeRasterSlicing;
	group2.groupNumSlices_ = 4;
	group2.groupPrecision_ = 8;

	if (!rstTheme->buildGrouping(group2,mySlices)) 
	{
		 cout << "Error grouping theme: " << db->errorMessage() << endl;
		 db->close();
		 cout << endl << "Press Enter\n";
		 getchar();
		 return 1;
	}

	for (ng=0; ng<rstTheme->grouping().groupNumSlices_; ng++)
	{
		TeColor color(red[ng*2],green[ng*2],blue[ng*2]);
		TeVisual* visual2 = TeVisualFactory::make("tevisual");            
		visual2->color(color);
		visual2->transparency(rstTheme->defaultLegend().visual(TePOLYGONS)->transparency());
		visual2->contourStyle(rstTheme->defaultLegend().visual(TePOLYGONS)->contourStyle());
		visual2->contourWidth(rstTheme->defaultLegend().visual(TePOLYGONS)->contourWidth());
		visual2->contourColor(rstTheme->defaultLegend().visual(TePOLYGONS)->contourColor());	
		rstTheme->setGroupingVisual(ng+1,visual2,TePOLYGONS);
	}

	if (!rstTheme->saveGrouping())
	{
		 cout << "Error saving theme slicing: " << db->errorMessage() << endl;
		 db->close();
		 cout << endl << "Press Enter\n";
		 getchar();
		 return 1;
	}
	
	db->close();
	cout << "Created second theme...\n";
	cout << "Sucess. Press enter...\n";
	getchar();
	return 0;
}
