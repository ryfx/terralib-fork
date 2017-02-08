/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright © 2001-2007 INPE and Tecgraf/PUC-Rio.
This file is part of TerraView. TerraView is free software; you can redistribute it 
and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

You should have received a copy of the GNU General Public License
along with TerraView.
The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The software provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use of
this program and its documentation.
*************************************************************************************/

#include <TeWaitCursor.h>
#include <TeDatabase.h>
#include <TeAppTheme.h>
#include <TeQtDatabasesListView.h>
#include <TeQtLayerItem.h>

void CreateTable::init()
{
	help_ = 0;

	mainWindow_= (TerraViewBase*)qApp->mainWidget();
    TeQtDatabasesListView *dbListView = mainWindow_->getDatabasesListView();
	TeQtLayerItem *layerItem = (TeQtLayerItem*)dbListView->popupItem();
	TeLayer* layer = layerItem->getLayer();

	staticTableRadioButton->setChecked(true);
	layerLineEdit->setText(layer->name().c_str());

	initTimeAttrNamelineEdit->setEnabled(false);
	finalTimeAttrNamelineEdit->setEnabled(false);
	insertObjectsCheckBox->setChecked(true);
}


void CreateTable::tableTypeButtonGroup_clicked( int )
{
	initTimeAttrNamelineEdit->setText("");
	finalTimeAttrNamelineEdit->setText("");
	if (staticTableRadioButton->isOn())
	{
		initTimeAttrNamelineEdit->setEnabled(false);
		finalTimeAttrNamelineEdit->setEnabled(false);
	}
	else
	{
		initTimeAttrNamelineEdit->setEnabled(true);
		finalTimeAttrNamelineEdit->setEnabled(true);
	}
}

void CreateTable::okPushButton_clicked()
{
	TeWaitCursor wait;
	QString qs = tableNameLineEdit->text();
	if(qs.isEmpty())
	{
		wait.resetWaitCursor();
		QMessageBox::warning(this, tr("Warning"), tr("Give the table name!"));
		return;
	}
	string nome = qs.latin1();

    TeQtDatabasesListView *dbListView = mainWindow_->getDatabasesListView();
	TeQtLayerItem *layerItem = (TeQtLayerItem*)dbListView->popupItem();
	TeLayer* layer = layerItem->getLayer();
	//TeDatabase* db = mainWindow_->currentDatabase();
	TeDatabase* db = layer->database();

	string errorMessage;
	bool changed;

	string newName = TeCheckName(nome, changed, errorMessage); 

	if(changed)
	{
		QString mess = tr("The table name is invalid: ") + errorMessage.c_str();
				mess += "\n" + tr("Change current name and try again.");
				QMessageBox::warning(this, tr("Warning"), mess);

			wait.resetWaitCursor();
			return;
	}

	nome = newName;

	if(db->tableExist(nome) == true)
	{
		wait.resetWaitCursor();
		QMessageBox::warning(this, tr("Warning"), tr("The table already exists!"));
		return;
	}

	TeAttributeList attr;
	TeAttribute		at;
	at.rep_.type_ = TeSTRING;
	at.rep_.name_ = "object_id";
	at.rep_.numChar_ = 100;

	if (eventTableRadioButton->isOn() || 
		dynAttrTableRadioButton->isOn() ||
		dynGeomAndAttrTableRadioButton->isOn())
	{
		QString qs = initTimeAttrNamelineEdit->text();
		if (qs.isEmpty())
		{
			wait.resetWaitCursor();
			QMessageBox::warning(this, tr("Warning"),
				tr("Enter with the attribute name of the initial time!"));
			return;
		}
	}

	TeTable attTable;
	QString s, ss;

	if (staticTableRadioButton->isOn())
	{
		at.rep_.isPrimaryKey_ = true;
		attr.push_back(at);
		attTable = TeTable(nome, attr, "object_id", "object_id", TeAttrStatic);
	}
	else if (eventTableRadioButton->isOn())
	{
		at.rep_.isPrimaryKey_ = true;
		attr.push_back(at);
		at.rep_.isPrimaryKey_ = false;
		s = initTimeAttrNamelineEdit->text();
		at.rep_.name_ = s.latin1();
		at.rep_.type_ = TeDATETIME;
		attr.push_back(at);
		ss = finalTimeAttrNamelineEdit->text();
		if (ss.isEmpty() == false && ss != s)
		{
			at.rep_.name_ = ss.latin1();
			attr.push_back(at);
		}
		attTable = TeTable(nome, attr, "object_id", "object_id", TeAttrEvent);
	}
 	else
	{
		at.rep_.isPrimaryKey_ = false;
		attr.push_back(at);
		at.rep_.isPrimaryKey_ = true;
		at.rep_.name_ = "unique_id";
		at.rep_.isAutoNumber_ = true;
		at.rep_.type_ = TeINT;
		attr.push_back(at);
		at.rep_.isAutoNumber_ = false;
		at.rep_.isPrimaryKey_ = false;
		s = initTimeAttrNamelineEdit->text();
		at.rep_.name_ = s.latin1();
		at.rep_.type_ = TeDATETIME;
		attr.push_back(at);
		ss = finalTimeAttrNamelineEdit->text();
		if (ss.isEmpty() == false && ss != s)
		{
			at.rep_.name_ = ss.latin1();
			attr.push_back(at);
		}

		if (dynAttrTableRadioButton->isOn())
			attTable = TeTable(nome, attr, "unique_id", "object_id", TeFixedGeomDynAttr);
		else
			attTable = TeTable(nome, attr, "unique_id", "object_id", TeDynGeomDynAttr);
	}

	if (eventTableRadioButton->isOn() || dynAttrTableRadioButton->isOn() || dynGeomAndAttrTableRadioButton->isOn())
	{
		attTable.attInitialTime(s.latin1());
		if (ss.isEmpty() == false && ss != s)
			attTable.attFinalTime(ss.latin1());
	}

	if(layer->createAttributeTable(attTable) == false)
	{
		wait.resetWaitCursor();
		QMessageBox::critical(this, tr("Error"), tr("Fail to create the table!"));
		return;
	}
	string atribLinkName = attTable.linkName();
	layer->addAttributeTable(attTable);

	if(eventTableRadioButton->isOn())
	{
		wait.resetWaitCursor();
		QMessageBox::information(this, tr("Information"), tr("The table was created successfully!"));
		hide();
		return;
	}

	if(insertObjectsCheckBox->isChecked() == false)
	{
		if (staticTableRadioButton->isOn() == false)
		{
			at.rep_.name_ = "unique_id";
			at.rep_.type_ = TeSTRING;
			at.rep_.numChar_ = 100;
			at.rep_.isAutoNumber_ = false;
			at.rep_.isPrimaryKey_ = false;
			if(db->alterTable(nome, at.rep_) == false)
			{
				wait.resetWaitCursor();
				QMessageBox::critical(this, tr("Error"), tr("Fail to alter column to VARCHAR!"));
				return;
			}
		}

		wait.resetWaitCursor();
		QMessageBox::information(this, tr("Information"), tr("The table was created successfully!"));
		hide();
		return;
	}

	attr.clear();
	at.rep_.name_ = "tempCol";
	at.rep_.type_ = TeSTRING;
	at.rep_.numChar_ = 100;
	at.rep_.isAutoNumber_ = false;
	at.rep_.isPrimaryKey_ = false;
	attr.push_back(at);

	if(layer->database()->tableExist("tempTable"))
	{
		if(db->execute("DROP TABLE tempTable") == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"), tr("Fail to drop the temporary table!"));
			return;
		}
	}
	if(layer->database()->createTable("tempTable", attr) == false)
	{
		wait.resetWaitCursor();
		QMessageBox::critical(this, tr("Error"), tr("Fail to create the temporary table!"));
		return;
	}

	string popule, geo;
	if(layer->hasGeometry(TePOLYGONS))
	{
		geo = layer->tableName(TePOLYGONS);
		popule = "INSERT INTO tempTable (tempCol) SELECT object_id FROM " + geo;
		if(db->execute(popule) == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"), tr("Fail to insert the objects in the temporary table!"));
			if(db->tableExist("tempTable"))
			{
				if(db->execute("DROP TABLE tempTable") == false)
				{
					wait.resetWaitCursor();
					QMessageBox::critical(this, tr("Error"), tr("Fail to drop the temporary table!"));
					return;
				}
			}
			return;
		}
	}
	if(layer->hasGeometry(TeLINES))
	{
		geo = layer->tableName(TeLINES);
		popule = "INSERT INTO tempTable (tempCol) SELECT object_id FROM " + geo;
		if(db->execute(popule) == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"), tr("Fail to insert the objects in the temporary table!"));
			if(db->tableExist("tempTable"))
			{
				if(db->execute("DROP TABLE tempTable") == false)
				{
					wait.resetWaitCursor();
					QMessageBox::critical(this, tr("Error"), tr("Fail to drop the temporary table!"));
					return;
				}
			}
			return;
		}
	}
	if(layer->hasGeometry(TePOINTS))
	{
		geo = layer->tableName(TePOINTS);
		popule = "INSERT INTO tempTable (tempCol) SELECT object_id FROM " + geo;
		if(db->execute(popule) == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"), tr("Fail to insert the objects in the temporary table!"));
			if(db->tableExist("tempTable"))
			{
				if(db->execute("DROP TABLE tempTable") == false)
				{
					wait.resetWaitCursor();
					QMessageBox::critical(this, tr("Error"), tr("Fail to drop the temporary table!"));
					return;
				}
			}
			return;
		}
	}
	if(layer->hasGeometry(TeCELLS))
	{
		geo = layer->tableName(TeCELLS);
		popule = "INSERT INTO tempTable (tempCol) SELECT object_id FROM " + geo;
		if(db->execute(popule) == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"), tr("Fail to insert the objects in the temporary table!"));
			if(db->tableExist("tempTable"))
			{
				if(db->execute("DROP TABLE tempTable") == false)
				{
					wait.resetWaitCursor();
					QMessageBox::critical(this, tr("Error"), tr("Fail to drop the temporary table!"));
					return;
				}
			}
			return;
		}
	}

	popule = "INSERT INTO " + nome + " ("+atribLinkName+") SELECT DISTINCT tempCol FROM tempTable";
	if(db->execute(popule) == false)
	{
		wait.resetWaitCursor();
		QMessageBox::critical(this, tr("Error"), tr("Fail to insert the objects in the table!"));
		if(db->tableExist("tempTable"))
		{
			if(db->execute("DROP TABLE tempTable") == false)
			{
				wait.resetWaitCursor();
				QMessageBox::critical(this, tr("Error"), tr("Fail to drop the temporary table!"));
				return;
			}
		}
		return;
	}

	if(db->tableExist("tempTable"))
	{
		if(db->execute("DROP TABLE tempTable") == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"), tr("Fail to drop the temporary table!"));
			return;
		}
	}

	if (staticTableRadioButton->isOn() == false)
	{
		at.rep_.name_ = "unique_id";
		at.rep_.type_ = TeSTRING;
		at.rep_.numChar_ = 100;
		at.rep_.isAutoNumber_ = false;
		at.rep_.isPrimaryKey_ = false;
		if(db->alterTable(nome, at.rep_) == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"), tr("Fail to alter column to VARCHAR!"));
			return;
		}
	}
	
	wait.resetWaitCursor();
	QMessageBox::information(this, tr("Information"), tr("The table was created successfully!"));
	hide();
}



void CreateTable::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("createTable.htm");
	if(help_->erro_ == false)
	{
		help_->show();
		help_->raise();
	}
	else
	{
		delete help_;
		help_ = 0;
	}
}


