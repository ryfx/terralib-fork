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
	This file illustrates how to execute some operations relative to attribute tables
	in a MySQL TerraLib database:
	 - how to create an attribute table;
	 - how to add some columns;
	 - how to insert same data into it;
	 - how to retrieve some rows using an SQL.

	Author: Lubia Vinhas  
*/

#include "../../src/terralib/kernel/TeTable.h"
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

	// Create a table called "TesteTable"
	string tableName = "TesteTable";
	if (db->tableExist(tableName))
	{
		cout << "There is already a table named \"TesteTable\" in the TerraLib database!\n";
		cout << "Some data will be retrieved from the table already created:\n\n";
	}
	else
	{
		// 1) Create a table

		TeAttributeList attList;	
		TeAttribute at;
		at.rep_.name_= "IntCol";	
		at.rep_.type_ = TeINT;	
		at.rep_.isPrimaryKey_ = true;

		attList.push_back(at);

		if (!db->createTable(tableName, attList))
		{
			cout << "Fail to create the table: " << db->errorMessage() << endl;
			db->close();
			cout << endl << "Press Enter\n";
			getchar();
			return 1;
		}

		//2)  Add new columns to the table

		TeAttribute atr;
		atr.rep_.name_= "TimeCol";
		atr.rep_.type_ = TeDATETIME;
		atr.dateTimeFormat_ = "DDsMMsYYYYsHHsmmsSS";
		atr.dateChronon_ = TeSECOND;
		atr.timeSeparator_ = ":";
		atr.dateSeparator_ = "/";
		attList.push_back(atr);

		if (!db->addColumn(tableName, atr.rep_))
		{
			cout << "Fail to add the column \"TimeCol\" to the table: " << db->errorMessage() << endl;
			db->close();
			cout << endl << "Press Enter\n";
			getchar();
			return 1;
		}

		atr.rep_.name_= "RealCol";
		atr.rep_.type_ = TeREAL;
		atr.rep_.numChar_ = 15;
		atr.rep_.decimals_ = 4;
		attList.push_back(atr);

		if (!db->addColumn(tableName, atr.rep_))
		{
			cout << "Fail to add the column \"RealCol\" to the table: " << db->errorMessage() << endl;
			db->close();
			cout << endl << "Press Enter\n";
			getchar();
			return 1;
		}

		atr.rep_.name_= "StringCol";
		atr.rep_.type_ = TeSTRING;
		atr.rep_.numChar_ = 10;
		attList.push_back(atr);

		if (!db->addColumn(tableName, atr.rep_))
		{
			cout << "Fail to add the column \"StringCol\" to the table: " << db->errorMessage() << endl;
			db->close();
			cout << endl << "Press Enter\n";
			getchar();
			return 1;
		}
		
		//3) Add two new rows to the table 

		TeTable table(tableName, attList, "");		// creates a table in memory

		TeTableRow row1;							// fill some rows
		row1.push_back("1");
		row1.push_back("25/4/2003 07:14");
		row1.push_back("15.23");
		row1.push_back("object1");
		table.add(row1);

		TeTableRow row2;
		row2.push_back("2");
		row2.push_back("01/03/1975 21:30:15");
		row2.push_back("56.87");
		row2.push_back("object2");
		table.add(row2);

		if (!db->insertTable(table))				// save the table into the database
		{
			cout << "Fail to save the table: " << db->errorMessage() << endl;
			db->close();
			cout << endl << "Press Enter\n";
			getchar();
			return 1;
		}
	}

	//	4) Retrieve some data from the table using a SQL query 
	TeDatabasePortal* portal = db->getPortal();
	if (!portal)
	{
		cout << "Fail to get a portal fom the database: " << db->errorMessage() << endl;
		db->close();
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}

	string sql = "SELECT * FROM " + tableName;

	if (portal->query(sql) == false)
	{
		cout << "Fail to submit the query: " << db->errorMessage() << endl;
		delete portal;
		db->close();
		cout << endl << "Press Enter\n";
		getchar();
		return 1;
	}

	// Retrieve the attributes from the table
	int count = 1;
	while (portal->fetchRow())
	{
		cout << "Row : " << count << endl;
		int intAttribute = atoi(portal->getData(0));
		cout << "Integer attribute: " << intAttribute << endl;

		TeTime t = portal->getDate(1);
		cout << "Time attribute: " ;
		cout << t.getDateTime("DDsMMsYYYYsHHsmmsSS") << " or " << t.getDateTime("YYYYsMMsDDsHHsmmsSS") << endl;

		cout << "Double attribute: " ;
		double d = portal->getDouble(2);
		cout << d << endl;

		cout << "String attribute: ";
		string x = portal->getData(3);
		cout << x << endl << endl;
		count++;
	}

	delete portal;
	db->close();
	cout << endl << "Press Enter\n";
	getchar();
	return 0;
}
