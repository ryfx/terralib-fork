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
    This example shows how to use a querier from theme, using a spatial restriction 
	defined by a rectangle (TeBOX). This spatial restriction is associated to querier. 
   
   Author: Karine Reis 
*/


#include "../../src/terralib/drivers/MySQL/TeMySQL.h"
#include "../../src/terralib/kernel/TeQuerier.h"
#include "../../src/terralib/kernel/TeQuerierParams.h"

#include <time.h>
#include <iostream>

using std::cout;

int 
main()
{	
	
	// Datatabase server parameters
     string host = "localhost";
     string dbname = "DB320RC1";
     string user = "root";
     string pass = "";

	 // Connects to database
     TeDatabase* db_ = new TeMySQL();
     if (!db_->connect(host,user,pass,dbname))
     {
         cout << "Error: " << db_->errorMessage() << endl;
         return 1;
     }

	// Load the theme 
	TeTheme* ocorrencias = new TeTheme("Ocorrencias");
	if (!db_->loadTheme(ocorrencias))
    {
        cout << "Error: " << db_->errorMessage() << endl; 
		db_->close(); 
        return 1;
    }
		
	//All attributes and geometries
	bool loadGeometries = false;
	bool loadAllAttributes = true;
	
	// Set querier parameters - load all attributes and geometries 
	TeQuerierParams querierParams(loadGeometries, loadAllAttributes);
	querierParams.setParams(ocorrencias);

	//Set spatial restriction
	TeBox box(609033.62, 794723.35, 613455.34, 800417.99); 
	querierParams.setSpatialRest(box, TeWITHIN); 		
	
	TeQuerier  querier(querierParams);

	// Load instances from layer based in the querier parameters 
	querier.loadInstances();

	// Traverse all the instances 
	TeSTInstance sti;
	while(querier.fetchInstance(sti))
	{
		cout << " Object: " << sti.objectId() << " --------------------------- " << endl << endl;
	
		// Plot each attribute, its name and value
		TePropertyVector vec = sti.getPropertyVector();
		for(unsigned int i=0; i<vec.size(); ++i)
		{
			string  attrName = vec[i].attr_.rep_.name_;
			string  attrValue = vec[i].value_;

			cout << attrName  << " : " << attrValue << endl;

		}
		
		cout << endl << endl;
	}

	cout << " End " << endl;
	getchar(); 
	db_->close ();
	return 0;
}




