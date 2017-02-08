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
   This example shows how to use a querier from theme.
   
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
	string password = "vinhas";

	 // Connects to database
     TeDatabase* db_ = new TeMySQL();
     if (!db_->connect(host,user,password,dbname))
     {
         cout << "Error: " << db_->errorMessage() << endl;
         return 1;
     }

	// Load the layer 
	TeTheme* bairros = new TeTheme("DistritosSaoPaulo");
	if (!db_->loadTheme(bairros))
    {
        cout << "Error: " << db_->errorMessage() << endl; 
		db_->close(); 
        return 1;
    }
		
	//All attributes and geometries
	bool loadGeometries = true;
	bool loadAllAttributes = true;
	
	// Set querier parameters - load all attributes and geometries 
	TeQuerierParams querierParams(loadGeometries, loadAllAttributes);
	querierParams.setParams(bairros);

	TeQuerier  querier(querierParams);

	// Load instances from layer based in the querier parameters 
	if(!querier.loadInstances())
		return 1;

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

		//Get geometries
		if(sti.hasPolygons())
		{
			TePolygonSet polSet;
			sti.getGeometry(polSet);

			for(unsigned int i=0; i<polSet.size(); ++i)
			{
				TeCoord2D centroid = TeFindCentroid(polSet[i]);
				string p = "( "+ Te2String(centroid.x(), 7) +", "+  Te2String(centroid.y(), 7) +")"; 
				cout << " Centroid of the Polygon : " << p << endl; 
			}
		}

		if(sti.hasPoints())
		{
			TePointSet ponSet;
			sti.getGeometry(ponSet);

			for(unsigned int i=0; i<ponSet.size(); ++i)
			{
				string p = "( "+ Te2String(ponSet[i].location().x(), 7) +", "+  Te2String(ponSet[i].location().y(), 7) +")"; 
				cout << " Point : " << p << endl; 
			}
		}

		cout << endl << endl;
	}

	cout << " Press enter! " << endl;
	getchar(); 
	db_->close ();
	return 0;
}




