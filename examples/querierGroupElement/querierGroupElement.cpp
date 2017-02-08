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
   This example shows how to use a querier from theme, grouping  
   spatiotemporal instances by a element. 
   
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
     string dbname = "saudavel";
     string user = "root";
     string pass = "";

	 // Connects to database
     TeDatabase* db_ = new TeMySQL();
     if (!db_->connect(host,user,pass,dbname))
     {
         cout << "Error: " << db_->errorMessage() << endl;
         return 1;
     }

	// Load the layer 
	TeTheme* coletas = new TeTheme("Coletas");
	if (!db_->loadTheme(coletas))
    {
        cout << "Error: " << db_->errorMessage() << endl; 
		db_->close(); 
        return 1;
    }
		
	//Attributes to group 
	bool loadGeometries = false;
	TeGroupingAttr attributes;
	
	pair<TeAttributeRep, TeStatisticType> attr1(TeAttributeRep("Coletas.NRO_OVOS_PAL1"), TeSUM);
	attributes.push_back(attr1);

	pair<TeAttributeRep, TeStatisticType> attr2(TeAttributeRep("Coletas.NRO_OVOS_PAL2"), TeSUM);
	attributes.push_back(attr2);

	pair<TeAttributeRep, TeStatisticType> attr3(TeAttributeRep("Coletas.NRO_OVOS_PAL3"), TeSUM);
	attributes.push_back(attr3);

	pair<TeAttributeRep, TeStatisticType> attr4(TeAttributeRep("Coletas.NRO_OVOS"), TeSUM);
	attributes.push_back(attr4);
	
	// Set querier parameters - load only the grouped attributes 
	TeQuerierParams querierParams(loadGeometries, attributes);
	querierParams.setParams(coletas);

	TeQuerier  querier(querierParams);

	// Load instances from layer based in the querier parameters 
	if(!querier.loadInstances())
	{
		return 1;
	}

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




