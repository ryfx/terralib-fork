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

#include "TeFlowMetadata.h"
#include "TeDatabase.h"

TeFlowMetadata::TeFlowMetadata(TeDatabase* db)
	: flowLayerId_(-1), flowTableId_(-1), referenceLayerId_(-1), referenceTableId_(-1), db_(db)
{
}

int TeFlowMetadata::createFlowMetadataTable()
{
	errorMessage_ = "";

	if(!db_)
	{
		errorMessage_ = "No database defined!";
		return false;
	}

	if(db_->tableExist("te_flow_data_layer"))
	{
		errorMessage_ = "The table te_flow_data_layer already exist!";
		return -1;
	}
	
	TeAttributeList attList;

	TeAttribute at1;
	at1.rep_.type_ = TeINT;
	at1.rep_.name_ = "flow_layer_id";
	at1.rep_.isPrimaryKey_ = true;
	at1.rep_.isAutoNumber_ = false;
	attList.push_back(at1);	

	TeAttribute at2;
	at2.rep_.type_ = TeINT;
	at2.rep_.name_ = "flow_table_id";
	at2.rep_.isPrimaryKey_ = false;
	at2.rep_.isAutoNumber_ = false;		
	attList.push_back(at2);	

	TeAttribute at3;
	at3.rep_.type_ = TeSTRING;
	at3.rep_.name_ = "origin_column";
	at3.rep_.numChar_ = 255;
	attList.push_back(at3);	

	TeAttribute at4;
	at4.rep_.type_ = TeSTRING;
	at4.rep_.name_ = "destiny_column";
	at4.rep_.numChar_ = 255;
	attList.push_back(at4);	

	TeAttribute at5;
	at5.rep_.type_ = TeINT;
	at5.rep_.name_ = "reference_layer_id";
	at5.rep_.isPrimaryKey_ = true;
	at5.rep_.isAutoNumber_ = false;
	attList.push_back(at5);	

	TeAttribute at6;
	at6.rep_.type_ = TeINT;
	at6.rep_.name_ = "reference_table_id";
	at6.rep_.isPrimaryKey_ = true;
	at6.rep_.isAutoNumber_ = false;
	attList.push_back(at6);	

	TeAttribute at7;
	at7.rep_.type_ = TeSTRING;
	at7.rep_.name_ = "reference_column";
	at7.rep_.numChar_ = 255;
	attList.push_back(at7);		

	if(!db_->createTable("te_flow_data_layer", attList))
	{
		errorMessage_ = "Could not create table te_flow_data_layer!";
		
		if(!db_->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += db_->errorMessage();
		}

		return 0;
	}

	return 1;
}

int TeFlowMetadata::dropFlowMetadataTable()
{
	errorMessage_ = "";

	if(!db_)
	{
		errorMessage_ = "No database defined!";
		return false;
	}

	if(!db_->tableExist("te_flow_data_layer"))
	{
		errorMessage_ = "The table te_flow_data_layer doesn't exist!";
		return -1;
	}

	if(!db_->deleteTable("te_flow_data_layer"))
	{
		errorMessage_ = "Could not drop the table te_flow_data_layer!";

		if(!db_->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += db_->errorMessage();
		}

		return 0;
	}

	return 1;
}

bool TeFlowMetadata::insertMetadata(void)
{
	errorMessage_ = "";
	
	if(!db_)
	{
		errorMessage_ = "No database defined!";

		return false;
	}

	string  sql  = "INSERT INTO te_flow_data_layer VALUES(";
			sql += Te2String(flowLayerId_);
			sql += ", ";
			sql += Te2String(flowTableId_);
			sql += ", '";
			sql += flowTableOriginColumnName_;
			sql += "', '";
			sql += flowTableDestinyColumnName_;
			sql += "', ";
			sql += Te2String(referenceLayerId_);
			sql += ", ";
			sql += Te2String(referenceTableId_);
			sql += ", '";
			sql += referenceColumnName_;
			sql += "')";

	if(!db_->execute(sql))
	{
		errorMessage_ = "Couldn't register flow layer metadata in te_flow_data_layer!";

		if(!db_->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += db_->errorMessage();
		}

		return false;
	}

	return true;
}

bool TeFlowMetadata::loadMetadata(const int& flowLayerId)
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
		return false;
	}
	
	string sql  = "SELECT * FROM te_flow_data_layer WHERE flow_layer_id = ";
	       sql += Te2String(flowLayerId);

	if(!portal->query(sql) || !portal->fetchRow())
	{
		errorMessage_ = "Couldn't find flow layer metadata in te_flow_data_layer!";

		if(!portal->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += portal->errorMessage();
		}

		portal->freeResult();
		delete portal;
		return false;
	}

	flowLayerId_ = portal->getInt("flow_layer_id");
	flowTableOriginColumnName_ = portal->getData("origin_column");
	flowTableDestinyColumnName_ = portal->getData("destiny_column");
	flowTableId_ = portal->getInt("flow_table_id");
	referenceColumnName_ = portal->getData("reference_column");
	referenceLayerId_ = portal->getInt("reference_layer_id");
	referenceTableId_ = portal->getInt("reference_table_id");

	portal->freeResult();
	delete portal;

	return true;
}

bool TeFlowMetadata::deleteMetadata(const int& flowLayerId)
{
	errorMessage_ = "";

	if(!db_)
	{
		errorMessage_ = "No database defined!";
		return false;
	}

	string  sql  = "DELETE FROM te_flow_data_layer WHERE flow_layer_id = ";
			sql += Te2String(flowLayerId);

	if(!db_->execute(sql))
	{
		errorMessage_  = "Couldn't delete flow layer metadata for layer id = '";
		errorMessage_ += Te2String(flowLayerId);
		errorMessage_ += "'!";

		if(!db_->errorMessage().empty())
		{
			errorMessage_ += " \n";
			errorMessage_ += db_->errorMessage();
		}

		return false;
	}

	return true;

}


