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
   This example creates a theme with spatial and attribute restrictions and fills 
   a STElementSet from this created theme with all attributes and geometries.

   Author: Karine Reis   
*/

#include "../../src/terralib/kernel/TeSTElementSet.h"
#include "../../src/terralib/kernel/TeSTEFunctionsDB.h"
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
	
	// Loads the layer
	TeLayer* bairrosPA =  new TeLayer("BairrosPoA");
	if (!db->loadLayer(bairrosPA))
	{
	    cout << "Fail to load the layer!" << db->errorMessage() << endl;
		db->close();
		cout << endl << "Press Enter\n";
		getchar();
        return 1;
	}

	// Loads the geometry of Santo Antonio district (id = 48)
	TePolygonSet ps;
	bairrosPA->loadGeometrySet("48", ps);
		
	// Loads a layer named OcorrenciaPoA 
	TeLayer* OcorrenciaPoA =  new TeLayer("OcorrenciasPoA");
	if (!db->loadLayer(OcorrenciaPoA))
	{
	    cout << "Fail to load the layer!" << db->errorMessage() << endl;
		db->close();
		cout << endl << "Press Enter\n";
		getchar();
        return 1;
	}

	// Create a theme with retrictions from Ocorrencias layer, in memory
	TeTheme* Ocorrencias = new TeTheme("Ocorrencias", OcorrenciaPoA);
	TeAttrTableVector attrTables;
	OcorrenciaPoA->getAttrTables(attrTables);
	Ocorrencias->setAttTables(attrTables);
	
	//spatial restriction: within Santo Antonio district
	Ocorrencias->setSpatialRest (&ps, TePOINTS, TeWITHIN);
	//attribute restriction: type "ameaça"
	Ocorrencias->attributeRest(" EVENTO = 'Ameaça' ");

	// Creates a elementSet from theme 
	TeSTElementSet  steSet(Ocorrencias);

	// Builds the elementSet with geometries and all attributes	
	bool loadGeometries = true;
	bool loadAllAttributes = true;
	vector<string>  attrs;
	if(!TeSTOSetBuildDB(&steSet, loadGeometries, loadAllAttributes, attrs))
	{
		cout << "Error! " << endl;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}
	
	// Shows how many elements the elementSet has
	cout << "Number of elements: " <<  steSet.numElements() << endl;

	// Traverse all instances using iterator 
	TeSTElementSet::iterator it = steSet.begin();
	while ( it != steSet.end())
	{
		TeSTInstance st = (*it);

		//Gets attribute value
		string event;
		st.getPropertyValue("EVENTO", event);
		
		cout << " Object Identifier :  " << st.objectId() << endl; 
		cout << " Event	  :  " << event  << endl ; 
				
		//Gets geometry
		if(st.hasPoints())
		{
			TePointSet pset;
			st.getGeometry (pset);
			cout<< "     point id: "<< pset[0].objectId ()	 << endl;
			for(unsigned int j=0; j<pset.size (); ++j)
			{
				string point =  Te2String(pset[j].location().x(), 7) +";"+  Te2String(pset[j].location().y(), 7);
				cout<< "		point: " << " ("+ point + ") " <<endl << endl;
			}
		}
		++it;
	}

	db->close();
	cout << endl << "Press Enter\n";
	getchar();
	return 0;
}
