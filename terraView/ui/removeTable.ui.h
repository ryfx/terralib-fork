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

#include <terraViewBase.h>
#include <TeWaitCursor.h>
#include <TeQtViewsListView.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <TeDatabaseUtils.h>

void RemoveTable::init()
{
	help_ = 0;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	layer_ = 0;
}

void RemoveTable::showTables(TeDatabase *db, TeLayer* layer)
{
	db_ = db;
	if (layer != 0)		// operation is to remove a layer table
	{
		tableTextLabel->setText(tr("Infolayer Tables:"));
		layer_ = layer;
		TeAttrTableVector tablesVector;
		layer->getAttrTables(tablesVector);
		if (tablesVector.empty() == true)
		{
			QMessageBox::warning(this,tr("Warning"),
				tr("There is no infolayer tables to remove from the database!"));
			reject();
		}
		for(unsigned int i=0; i < tablesVector.size(); i++)
			tableComboBox->insertItem(tablesVector[i].name().c_str());
	}
	else				// operation is to remove an external table
	{
		int tableId;
		string tableName;

		tableTextLabel->setText(tr("External Tables:"));
		int tableType = (int)TeAttrExternal;
		string q = "SELECT table_id, attr_table FROM te_layer_table";
		q += " WHERE attr_table_type = " + Te2String(tableType);

		TeDatabasePortal *portal = db_->getPortal();
		if (portal->query(q) == false)
		{
			delete portal;
			reject();
			return;
		}

		while(portal->fetchRow())
		{
			tableId = portal->getInt(0);
			tableName = portal->getData(1);
			extTableMap_[tableName] = tableId;
			tableComboBox->insertItem(tableName.c_str());
		}
		delete portal;

		if (extTableMap_.empty() == true)
		{
			QMessageBox::warning(this,tr("Warning"),
				tr("There is no external tables to remove from the database!"));
			reject();
			return;
		}
	}
	accept();
}


void RemoveTable::okPushButton_clicked()
{
	TeWaitCursor wait;
	unsigned int i, j, k, order;
	TeTable table;
	TeViewMap::iterator it;
	vector<string> affectedThemeNamesVector;
	vector<TeAppTheme*> affectedThemesVector;
	TeView *view;
	TeAppTheme *appTheme;
	int tableId;				// id of the table to be deleted
	QString msg;

	string tableName = tableComboBox->currentText().latin1();

	// Get the id of the table to be deleted
	if (layer_ != 0)		// operation is to remove a layer table
	{
		// Get the table instance from its name
		TeAttrTableVector tablesVector;
		layer_->getAttrTables(tablesVector);
		for (i = 0; i < tablesVector.size(); ++i)
		{
			if (tablesVector[i].name() == tableName)
			{
				tableId = tablesVector[i].id();
				break;
			}
		}				
	}
	else	// operation is to remove an external table
		tableId = extTableMap_[tableName];

	// Check the themes that will be affected due the deletion of this table
	vector<TeQtViewItem*> viewItemVec = mainWindow_->getViewsListView()->getViewItemVec();
	for (i = 0; i < viewItemVec.size(); ++i)
	{
		view = viewItemVec[i]->getView();
		vector<TeQtThemeItem*> themeItemVec = viewItemVec[i]->getThemeItemVec();
		for (j = 0; j < themeItemVec.size(); ++j)
		{
			TeAppTheme* appTheme = themeItemVec[j]->getAppTheme();
			TeTheme* theme = (TeTheme*)appTheme->getTheme();
			if(theme->type() == TeTHEME)
			{
				if (theme->isThemeTable(tableId) == true)
				{
					string themeViewName = theme->name() + " (" + view->name() + ")";
					affectedThemeNamesVector.push_back(themeViewName);
					affectedThemesVector.push_back(appTheme);
				}
			}
		}
	}

	wait.resetWaitCursor();
	if (affectedThemeNamesVector.empty() == false)
	{
		msg = tr("The following themes(views) will be affected by the deletion of the table") + " " + tableName.c_str() + ":\n";
		for (i = 0; i < affectedThemeNamesVector.size(); ++i)
		{
			msg += " - ";
			msg += affectedThemeNamesVector[i].c_str();
			msg += "\n";
		}
		msg += "\n";
		msg += tr("Do you really want to delete the table") + " " + tableName.c_str() + "?";
	}
	else
		msg = tr("Do you really want to delete the table") + " " + tableName.c_str() + "?";

	int response = QMessageBox::question(this, tr("Table Deletion"),msg,
		tr("Yes"), tr("No"));
	if(response == 1)
	{
		hide();
		return;
	}

	wait.setWaitCursor();
	// Remove the table from the themes that are affected by the deletion of the table
	for (i = 0; i < affectedThemesVector.size(); ++i)
	{
		appTheme = affectedThemesVector[i];
		TeTheme* theme = (TeTheme*)appTheme->getTheme();
		TeAttrTableVector tablesVector; 
		theme->getAttTables(tablesVector);

		// Insert in the new tables vector only the tables that are
		// non external, excluding the one to be deleted
		order = 0;
		TeAttrTableVector newTablesVector;
		for (j = 0; j < tablesVector.size(); ++j)
		{
			table = tablesVector[j];
			if (table.id() != tableId && table.tableType() != TeAttrExternal)
			{
				table.setOrder(order++);
				newTablesVector.push_back(table);
			}
		}

		// Insert the external tables in the new vector of theme tables 
		// which have a relation with tables that remain as theme tables
		for (j = 0; j < tablesVector.size(); ++j)
		{
			table = tablesVector[j];
			if (table.tableType() == TeAttrExternal)
			{
				for (k = 0; k < newTablesVector.size(); ++k)
				{
					if (table.id() != tableId && newTablesVector[k].id() == table.relatedTableId())
					{
						table.setOrder(order++);
						newTablesVector.push_back(table);
						break;
					}
				}
			}
		}

		// Set the new vector of theme tables
		tablesVector.clear();
		theme->clearAttTableVector();
		for (j = 0; j < newTablesVector.size(); ++j)
			theme->addThemeTable(newTablesVector[j]);

		db_->updateThemeTable(theme);

		if (appTheme == mainWindow_->currentTheme())
			mainWindow_->getGrid()->clear();

		if (!theme->createCollectionAuxTable() || !theme->populateCollectionAux())
		{
			QMessageBox::information(this, tr("Error"),
			tr("Fail to mount the auxiliary table of the collection!"));
			return;
		}
		if (appTheme == mainWindow_->currentTheme())
			mainWindow_->getGrid()->init(appTheme);
	}

	// If the operation is to remove a layer table, update the vector of layer tables
	if (layer_ != 0)
	{
		TeAttrTableVector& layerTablesVector = layer_->attrTables();
		for (i = 0; i < layerTablesVector.size(); ++i)
		{
			if (layerTablesVector[i].id() == tableId)
			{
				TeAttrTableVector::iterator it;
				it = layerTablesVector.begin() + i;
				layerTablesVector.erase(it);
			}
		}
	}

	// Delete the record corresponding to the table to be deleted in the te_layer_table
	string del = "DELETE FROM te_layer_table WHERE table_id = " + Te2String(tableId);
	db_->execute(del);

	wait.resetWaitCursor();
	if(db_->deleteTable(tableName) == true)
		msg = tr("The table") + " " + tableName.c_str() + " " + tr("was removed successfully!");
	else
		msg = tr("The table") + " " + tableName.c_str() + " " + tr("could not be removed!");
	QMessageBox::information(this, tr("Information"), msg);

	hide();
}


void RemoveTable::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("removeTable.htm");
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


