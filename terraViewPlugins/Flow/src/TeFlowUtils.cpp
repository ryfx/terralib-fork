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

#include <TeDatabase.h>
#include<TeFlowUtils.h>
//#include <qapplication.h>
//#include <qmessagebox.h>
//#include <terraViewBase.h>


bool TeFlowUtils::listCandidateFlowDataTables(vector<string>& tables)
{
	errorMessage_ = "";

	if(!db_)
	{
		errorMessage_ = "No database defined!";
		return false;
	}

	string sql = "SELECT table_id, attr_table FROM te_layer_table WHERE attr_table_type = ";
		   sql += Te2String(TeAttrExternal);
		   sql += " AND attr_table NOT IN (SELECT external_table_name FROM te_tables_relation)";		   

	TeDatabasePortal* portal = db_->getPortal();

	if(!portal)
	{
		errorMessage_ = "Couldn't get a database portal!";
		return false;
	}

	if(!portal->query(sql))
	{
		errorMessage_ = "Couldn't read TE_LAYER_TABLE in order to list candidate flow tables!";

		if(!portal->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += portal->errorMessage();
		}

		portal->freeResult();
		delete portal;
		return false;
	}

	while(portal->fetchRow())
		tables.push_back(portal->getData("attr_table"));

	portal->freeResult();
	delete portal;

	return true;
}

bool TeFlowUtils::listReferenceThemes(const int& layerId, vector<string>& themes)
{
	errorMessage_ = "";

	if(!db_)
	{
		errorMessage_ = "No database defined!";
		return false;
	}

	TeDatabasePortal* portal = db_->getPortal();

	if(!portal)
	{
		errorMessage_ = "Couldn't get a database portal!";

		if(!portal->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += portal->errorMessage();
		}
		return false;
	}
	
// listar os possiveis temas
	string sql  = "SELECT name FROM te_theme WHERE layer_id = ";
	sql += Te2String(layerId);

	if(!portal->query(sql))
	{
		errorMessage_ = "Couldn't read table te_theme!";

		if(!portal->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += portal->errorMessage();
		}

		portal->freeResult();
		delete portal;
		return false;
	}

	while(portal->fetchRow())
		themes.push_back(portal->getData(0));	

	portal->freeResult();
	delete portal;

	return true;
}

TeTheme* TeFlowUtils::getReferenceTheme(const string& dominanceThemeName)
{
// Descobre o tema que contem as informacoes de dominancia
	errorMessage_ = "";

	if(!db_)
	{
		errorMessage_ = "No database defined!";
		return false;
	}

	TeThemeMap& themes = db_->themeMap();
	TeThemeMap::iterator it = themes.begin();

	TeTheme* referenceTheme = 0;

	while(it != themes.end())
	{
		if(it->second->name() == dominanceThemeName)
		{
			referenceTheme = (TeTheme*)it->second;
			break;
		}

		++it;
	}

	if(!referenceTheme)
		errorMessage_ = "Couldn't find the specified theme: " + dominanceThemeName + "!";	

	return referenceTheme;
}

string TeFlowUtils::getNewLayerName(TeLayer* newLayer)
{
	
	errorMessage_ = "";
	
	int	i = 0;
	string name = newLayer->name();

	size_t f = name.find("_flow");

	if(f != string::npos)
		name.erase(f, name.size() - f);

	string newLayerName = name + "_flow";

	while(db_->layerExist(newLayerName))
		newLayerName = name + Te2String(i++) + "_flow";

	return newLayerName;
}

bool TeFlowUtils::getLeftString(const string flowTableName, const string columnTableName, const int& length)
{
	string sqlUpdate;

	sqlUpdate  = "UPDATE ";
	sqlUpdate += flowTableName;
	sqlUpdate += " SET ";
	sqlUpdate += columnTableName;
	sqlUpdate += " = ";
	sqlUpdate += db_->leftString(columnTableName, length);
	
	if(!db_->execute(sqlUpdate))
    	return false;

	return true;
}