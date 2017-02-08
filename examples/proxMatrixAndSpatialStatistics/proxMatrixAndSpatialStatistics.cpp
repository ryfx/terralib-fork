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
   Spatial Temporal Element Set (STElementSet) and calculate spatial statistics.  
   In this example, the adjacency strategy is used to construct the matrix.   

   Author: Karine Reis   
*/

#include "../../src/terralib/kernel/TeSTElementSet.h"
#include "../../src/terralib/kernel/TeSTEFunctionsDB.h"
#include "../../src/terralib/kernel/TeGeneralizedProxMatrix.h"
#include "../../src/terralib/stat/TeSpatialStatistics.h"
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
    	// Loads a layer named DistritosSaoPaulo 
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

	// Fills the STElementSet only with the attribute "Populac" to calculate statistics 
	//and without geometries
	vector<string> attrs;
	attrs.push_back ("Distritos.Pop91");

	if(!TeSTOSetBuildDB(&steSet, false, false, attrs))
	{
		cout << "Error! " << endl;
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}
	
	// Shows how many elements the elementSet has
	cout << "Number of elements: " <<  steSet.numElements() << endl;

	// Constructs the proximity matrix
	TeProxMatrixLocalAdjacencyStrategy			sc_adj (&steSet, TePOLYGONS);
	TeGeneralizedProxMatrix<TeSTElementSet>		proxMat(&sc_adj);
	
	cout << "Calculating the proximity matrix..." << endl;
	if(!proxMat.constructMatrix())
	{
		cout << "Error building proximity matrix! " << endl;
		return 1;
	}

	// Calculates the global mean
	double mean = TeFirstMoment (steSet.begin(), steSet.end(), 0); 
	cout << "Global mean " << mean << endl << endl;

	// Calculates the desviations (Z) to each object and keeps them
	// in the STElementSet
	if(!TeDeviation (steSet.begin(), steSet.end(), mean))
	{
		cout << "Error calculating Z! " << endl;
		return 1;
	}   
	
	// Index of the Z (desviations) attribute in the STElementSet
	int indexZ = 1;  

	// Calculates the local mean (WZ) to each object and keeps them
	// in the STElementSet
	if(!TeLocalMean (&steSet, &proxMat, indexZ))
	{
		cout << "Error calculating Local Mean! " << endl;
		return 1;
	}

	// Index of the WZ (local mean) attribute in the STElementSet
	int indexWZ = 2;

	// Shows the Z and WZ generated for each object
	TeSTElementSet::iterator it = steSet.begin();
	while ( it != steSet.end())
	{
		TeSTInstance obj = (*it);
		
      //Gets attribute value
		string valZ, valWZ;
		obj.getPropertyValue(valZ, indexZ);
		obj.getPropertyValue(valWZ, indexWZ);
		cout << " Object Identifier :  " << obj.objectId() << endl; 
		cout << " Z		:  " << valZ   << endl; 
		cout << " WZ	:  " << valWZ  << endl << endl; 
		++it;
	}

	// Inserts the generated attributes (Z and WZ) in the DistritosSP 
	// table 
	if(!TeUpdateDBFromSet (&steSet, "Distritos"))
	{
		cout << "Error updating database! " << endl;
		return 1;
	}
	
	db->close();
	cout << endl << "Press Enter\n";
	getchar();
	return 0;
}
