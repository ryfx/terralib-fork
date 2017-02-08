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
	This file shows an example of how to create a layer in a MySQL TerraLib database,
	and insert some vectorial data in this new layer. 
	
	Author: Lubia Vinhas  
*/

#include "../../src/terralib/drivers/MySQL/TeMySQL.h"


int main()
{
	// Datatabase server parameters
	string host = "localhost";
	string dbname = "DB320RC1";
	string user = "root";
	string password = "vinhas";

	// Opens a connection to a database accessible though ADO
	TeDatabase* db = new TeMySQL();
	if (!db->connect(host, user, password, dbname))
	{
		cout << "Error: " << db->errorMessage() << endl << endl;
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	cout << "Connection successful to the database \"" << dbname << "\" on MySQL server \"" << host << "\" !\n";

	// Creates a projection
	TeDatum mDatum = TeDatumFactory::make("SAD69");
	TeProjection* pUTM = new TeUtm(mDatum,0.0);

	// Create a new layer called "TesteLayer"
	string layerName = "TesteLayer";
	if (db->layerExist(layerName))
	{
		cout << "There is already a layer named \"TesteLayer\" in the TerraLib database!\n\n";
		db->close();
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	TeLayer* layer = new TeLayer(layerName, db, pUTM);
	if (layer->id() <= 0)      // layer wasn´t created properly
	{
		cout << "Error: " << db->errorMessage() << endl;
		db->close();
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	// Add the geometry representation LINES and store it in a table called "TesteLayerLines"
	if (!layer->addGeometry(TeLINES, "TesteLayerLines"))
	{
		cout << "Error: " << db->errorMessage() << endl;
		db->close();
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	// Create a set of lines
	TeLine2D reta;
	reta.add(TeCoord2D(500,500));
	reta.add(TeCoord2D(600,500));
	reta.add(TeCoord2D(700,500));
	reta.objectId("reta");

	TeLine2D ele;
	ele.add(TeCoord2D(500,600));
	ele.add(TeCoord2D(600,600));
	ele.add(TeCoord2D(700,700));
	ele.add(TeCoord2D(800,600));
	ele.add(TeCoord2D(900,600));
	ele.objectId("ele");

	TeLineSet ls;
	ls.add(reta);
	ls.add(ele);

	// Add the line set to the layer
	if (!layer->addLines(ls))
	{
		cout << "Error: " << db->errorMessage() << endl;
		db->close();
		cout << "Press Enter\n";
		getchar();
		return 1;
	}
	else
		cout << "Lines inserted to the new layer!\n";

	// Creates a set of  polygons
	// A simple polygon
	TeLine2D line;
	line.add(TeCoord2D(900,900));
	line.add(TeCoord2D(900,1000));
	line.add(TeCoord2D(1000,1000));
	line.add(TeCoord2D(1000,900));
	line.add(TeCoord2D(900,900));

	TeLinearRing r1(line);
	TePolygon poly1;
	poly1.add(r1);
	poly1.objectId("spoli");

	// A polygon with a hole
	TeLine2D line2;
	line2.add(TeCoord2D(200,200));
	line2.add(TeCoord2D(200,400));
	line2.add(TeCoord2D(400,400));
	line2.add(TeCoord2D(400,200));
	line2.add(TeCoord2D(200,200));

	TeLinearRing r2(line2);

	TeLine2D line3;
	line3.add(TeCoord2D(250,250));
	line3.add(TeCoord2D(250,300));
	line3.add(TeCoord2D(300,300));
	line3.add(TeCoord2D(300,250));
	line3.add(TeCoord2D(250,250));

	TeLinearRing r3(line3);

	TePolygon poly2;
	poly2.add(r2);
	poly2.add(r3);
	poly2.objectId("cpoli");

	TePolygonSet ps;
	ps.add(poly1);
	ps.add(poly2);

	// Add the polygon set to the layer
	// As addGeometry wasn´t called before, the table of polygons will have a default name
	if (!layer->addPolygons(ps))
	{
		cout << "Error: " << db->errorMessage() << endl;
		db->close();
		cout << "Press Enter\n";
		getchar();
		return 1;
	}
	else
		cout << "Polygons inserted in the new layer!\n";

	// Create a set of points
	TePoint p1(40,40);
	p1.objectId("ponto1");
	TePoint p2(65,65);
	p2.objectId("ponto2");

	TePointSet pos;
	pos.add(p1);
	pos.add(p2);

	// Add the point set to the layer
	// As addGeometry wasn´t called before, the table of points will have a default name
	if (!layer->addPoints(pos))
	{
		cout << "Error: " << db->errorMessage() << endl;
		db->close();
		cout << "Press Enter\n";
		getchar();
		return 1;
	}
	else
		cout << "Points inserted in the new layer!\n";

	// Create an attribute table
	// Define a list of attributes
	TeAttributeList attList;
	TeAttribute at;
	at.rep_.type_ = TeSTRING;
	at.rep_.numChar_ = 16;
	at.rep_.name_ = "object_id";
	at.rep_.isPrimaryKey_ = true;
	attList.push_back(at);

	at.rep_.type_ = TeSTRING;       // a string attribute
	at.rep_.numChar_ = 255;
	at.rep_.name_ = "nome";
	at.rep_.isPrimaryKey_ = false;
	attList.push_back(at);  

	at.rep_.type_ = TeREAL; // a float attribute
	at.rep_.name_ = "val1";
	at.rep_.decimals_ = 3;
	at.rep_.isPrimaryKey_ = false;
	attList.push_back(at);  

	at.rep_.type_ = TeINT;          // an integer attribute
	at.rep_.name_ = "val2";
	at.rep_.isPrimaryKey_ = false;
	attList.push_back(at);  

	// Create an attribute table associated to the new layer, called "TesteLayerAttr"
	TeTable attTable("TesteLayerAttr", attList, "object_id", "object_id");		// creates a table in memory
	if (!layer->createAttributeTable(attTable))
	{
		cout << "Error: " << db->errorMessage() << endl << endl;
		db->close();
		cout << "Press Enter\n";
		getchar();
		return 1;
	}

	// Each row is related to one of the geometries by its objetc_id
	TeTableRow row;
	row.push_back("reta");
	row.push_back("An straight line");
	row.push_back("11.1");
	row.push_back("11");
	attTable.add(row);
	row.clear();

	row.push_back("ele");
	row.push_back("A mountain shaped line");
	row.push_back("22.2");
	row.push_back("22");
	attTable.add(row);
	row.clear();

	row.push_back("spoli");
	row.push_back("A simple polygon");
	row.push_back("33.3");
	row.push_back("33");
	attTable.add(row);

	row.clear();
	row.push_back("ponto1");
	row.push_back("A point");
	row.push_back("55.5");
	row.push_back("55");
	attTable.add(row);
	row.clear();

	row.push_back("ponto2");
	row.push_back("Another point");
	row.push_back("66.6");
	row.push_back("66");
	attTable.add(row);
	row.clear();

	row.push_back("cpoli");
	row.push_back("A polygon with hole");
	row.push_back("44.4");
	row.push_back("44");
	attTable.add(row);
	row.clear();

	// Saves the table in the database
	if (!layer->saveAttributeTable( attTable ))
	{
		cout << "Error: " << db->errorMessage() << endl << endl;
		db->close();
		cout << "Press Enter\n";
		getchar();
		return 1;
	}
	else
		cout << "Objects inserted in the new layer!\n\n";

	// Close database
	db->close();
	cout << "Press Enter\n";
	cout.flush();
	getchar();
	return 0;
}

