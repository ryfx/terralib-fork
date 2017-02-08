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
   Set (STElementSet) from a shapefile. 

   Author: Karine Reis Ferreira 
*/


#include "../../src/terralib/kernel/TeSTElementSet.h"
#include "../../src/terralib/drivers/shapelib/TeSTEFunctionsSHP.h"

int
main()
{
	
	// shape file name with its path 
	string filename = "../../examples/data/EstadosBrasil.shp";
	
	TeSTElementSet  steSet;

	// fills the element set from shape file
	if (!TeSTOSetBuildSHP(steSet,filename))
	{
		cout << "Erro\n";
		return 1;
	}
	
	// prints the number of elements of the element set
	cout << "Number of elements: " << steSet.numElements() << endl;

	TeSTElementSet::iterator it = steSet.begin();
	while ( it != steSet.end())
	{
		TeSTInstance st = (*it);

		string desc;
		// returns the attributes
		TePropertyVector vectp = st.getPropertyVector();

		cout << "Id: " << st.objectId() << "\nProperties:\n "; 
		for (unsigned int i=0; i<vectp.size(); i++)
		{
			cout << vectp[i].attr_.rep_.name_ << "  =  ";
			cout << vectp[i].value_ << endl;
		}

		if (st.hasPolygons())
		{
			const TePolygonSet& ps = st.getPolygons();
			cout << "Geometry of " << ps.size() << " polygon(s).\n";
		}
		if (st.hasLines())
		{
			const TeLineSet& ls = st.getLines();
			cout << "Geometry of " << ls.size() << " line(s).\n";
		}
		if (st.hasPoints())
		{
			const TePointSet& pts = st.getPoints();
			cout << "Geometry of " << pts.size() << " point(s).\n";
		}
		cout << endl;
		++it;
	}
	cout << "End\n";
	cout.flush();
	getchar();
	return 0;
}
