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
	defined by a geometry (polygons, lines, cells and points) from another theme. 
	This spatial restriction is associated to querier. 
   
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

	// Load the themes 
	TeTheme* ocorrencias = new TeTheme("Ocorrencias");
	if (!db_->loadTheme(ocorrencias))
    {
        cout << "Error: " << db_->errorMessage() << endl; 
		db_->close(); 
        return 1;
    }

	TeTheme* bairros = new TeTheme("Tema_BairroBH");  
	if (!db_->loadTheme(bairros))
    {
        cout << "Error: " << db_->errorMessage() << endl; 
		db_->close(); 
        return 1;
    }
		
		
	// ------------ querier to theme "bairros"
	bool loadGeometries = true;
	vector<string> attrs;
	attrs.push_back ("Bairrobh.nobai");

	TeQuerierParams querParamsBair(loadGeometries, attrs);
	querParamsBair.setParams(bairros);
	
	TeQuerier  querBairros(querParamsBair); 

	if(!querBairros.loadInstances())
	{
		cout << " No data!!! " << endl; 
		return 1;
	}

	//Load each element and pass to querier as spatial restriction
	TeSTInstance bairro;
	while(querBairros.fetchInstance (bairro))
	{
		string name;
		bairro.getPropertyValue(name, 0);

		cout << " Bairro: " << name << endl;
		
		TePolygonSet setP;
		
		if(!bairro.getGeometry (setP))  //get the polygon set
			continue;

		// ------------ querier to theme coletas
		loadGeometries = false;
		vector<string> attributes;
		attributes.push_back ("ocorrenciasbh.Nrbo");

		TeQuerierParams querParamsOcr(loadGeometries, attributes);
		querParamsOcr.setParams(ocorrencias);
		
		//spatial restriction
		querParamsOcr.setSpatialRest(&setP);

		TeQuerier  querOcorrencias(querParamsOcr);
		
		if(!querOcorrencias.loadInstances())
		{
			cout << " No data!!! " << endl; 
			continue;
		}

		TeSTInstance sto;
		while(querOcorrencias.fetchInstance(sto)) 
		{
			// Plot each attribute, its name and value
			TePropertyVector vec = sto.getPropertyVector();
			for(unsigned int i=0; i<vec.size(); ++i)
			{
				string  attrName = vec[i].attr_.rep_.name_;
				string  attrValue = vec[i].value_;

				cout << attrName  << " : " << attrValue << endl;
			}

			//Plot the time
			cout << endl << " Data : " << sto.getInitialDateTime() << endl;
		}
	}

	cout << " End " << endl;
	getchar(); 
	db_->close ();
	return 0;
}




