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
   This file illustrates an example of how to create a proximity matrix from a 
   Spatial Temporal Element Set (STElementSet). 
   In this example, the distance strategy is used to construct the matrix.   

   Author: Karine Reis   
*/

#include "../../src/terralib/kernel/TeSTElementSet.h"
#include "../../src/terralib/kernel/TeSTEFunctionsDB.h"
#include "../../src/terralib/kernel/TeGeneralizedProxMatrix.h"
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
		
	
	// Opens a connection to a TerraLib database 
    // Loads a layer named Distritos
	TeLayer* DistritosSP =  new TeLayer("Distritos");
	if (!db->loadLayer(DistritosSP))
	{
	    cout << "Fail to load the layer!" << db->errorMessage() << endl;
		db->close();
		cout << endl << "Press Enter\n";
		getchar();
        return 1;
	}

    
	// Creates a STElementSet from DistritosSP layer 
	TeSTElementSet steSet(DistritosSP);

	// Builds the STElementSet with geometries
	vector<string> attrs;
	if(!TeSTOSetBuildDB(&steSet, true, false, attrs))
	{
		cout << "Error! " << endl;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}
	
	// Shows how many elements the elementSet has
	cout << "Number of elements: " <<  steSet.numElements() << endl;

	
	//Create prox matrix strategies		
	TeProxMatrixLocalDistanceStrategy<TeSTElementSet>   sc_dist (&steSet, TePOLYGONS, 12000.00);
	TeGeneralizedProxMatrix<TeSTElementSet> proxMat(&sc_dist); 

	// Builds the proximity matrix
	if(!proxMat.constructMatrix())
	{
		cout << "Error building proximity matrix! " << endl;
		getchar();
		db->close ();
		return 0;
	}

	// Shows the neighbours of each object from proxMat
	TeSTElementSet::iterator it = steSet.begin();
	while ( it != steSet.end())
	{
		cout<< " The neighbours of the element  "<< (*it).objectId() << " are: " << endl;
			
		// Gets the neighbours of an element
		TeNeighboursMap neighbors = proxMat.getMapNeighbours((*it).objectId());
		TeNeighboursMap::iterator itN = neighbors.begin();
		while (itN != neighbors.end())
		{
			cout<< "         "<<(*itN).first << endl;
			++itN;
		}
		cout << endl;
		++it;
	}

	
	db->close();
	cout << endl << "Press Enter\n";
	getchar();
	return 0;
}
