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
#include <TeDatabaseUtils.h>
#include <TeQtViewsListView.h>
#include <TeQtViewItem.h>
#include <algorithm>


void SelectThemeTables::init()
{
	help_ = 0;

	unsigned int i;
	vector<string> themeTablesNamesVector;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	theme_ = mainWindow_->currentTheme();
	if(theme_->getTheme()->type() != TeTHEME)
		return;

	TeTheme* curBaseTheme = (TeTheme*)theme_->getTheme();
	TeLayer* layer = curBaseTheme->layer();
	db_ = mainWindow_->currentDatabase();
	TeAttrTableVector themeTablesVector; 
	curBaseTheme->getAttTables(themeTablesVector);
	string viewName = mainWindow_->getViewsListView()->currentViewItem()->text().latin1();

	themeLineEdit->setText(curBaseTheme->name().c_str());

	viewLineEdit->setText(viewName.c_str());

	TeAttrTableVector layerTablesVector;
	if(layer->getAttrTables(layerTablesVector) == false)
	{
		QMessageBox::critical(this, tr("Error"),
			tr("Fail to get the infolayer tables!"));
		return;
	}

	for (i = 0; i < themeTablesVector.size(); ++i)
	{
		if (themeTablesVector[i].tableType() == TeAttrStatic || themeTablesVector[i].tableType() == TeFixedGeomDynAttr)
		{
			themeTablesNamesVector.push_back(themeTablesVector[i].name());
			themeTablesListBox->insertItem(themeTablesVector[i].name().c_str());
		}
	}

	for(i = 0; i < layerTablesVector.size(); ++i)
	{
		if(layerTablesVector[i].tableType() == TeAttrStatic || layerTablesVector[i].tableType() == TeFixedGeomDynAttr)
		{
			string tableName = layerTablesVector[i].name();
			if (std::find(themeTablesNamesVector.begin(), themeTablesNamesVector.end(),
					tableName) == themeTablesNamesVector.end())
				layerTablesListBox->insertItem(tableName.c_str());
		}
	}

	addTablePushButton->setEnabled(false);
	removeTablePushButton->setEnabled(false);
	if(themeTablesListBox->numRows() > 0)
		removeTablePushButton->setEnabled(true);
	if(layerTablesListBox->numRows() > 0)
		addTablePushButton->setEnabled(true);
}



void SelectThemeTables::addTablePushButton_clicked()
{
	themeTablesListBox->insertItem(layerTablesListBox->currentText());
	layerTablesListBox->removeItem(layerTablesListBox->currentItem());
	if(layerTablesListBox->count() == 0)
		addTablePushButton->setEnabled(false);
	removeTablePushButton->setEnabled(true);
}


void SelectThemeTables::removeTablePushButton_clicked()
{
	layerTablesListBox->insertItem(themeTablesListBox->currentText());
	themeTablesListBox->removeItem(themeTablesListBox->currentItem());
	if(themeTablesListBox->count() == 0)
		removeTablePushButton->setEnabled(false);
	addTablePushButton->setEnabled(true);
}


void SelectThemeTables::okPushButton_clicked()
{
	TeWaitCursor wait;
	unsigned int i, j, order = 0;
	TeTable table;
	string tableName;
	TeAttrTableVector newTablesVector;

	// Get the infolayer tables
	TeAttrTableVector layerTablesVector;
	TeTheme* curBaseTheme = (TeTheme*)theme_->getTheme();
	curBaseTheme->layer()->getAttrTables(layerTablesVector);

	// mount new tables vector
	bool newUsaStatic = false;
	bool newUsaTemporal = false;
	string tempTableName;
	order = 0;
	for (i = 0; i < themeTablesListBox->count(); ++i)
	{
		tableName = themeTablesListBox->item(i)->text().latin1();
		for (j = 0; j < layerTablesVector.size(); ++j)
		{
			if (layerTablesVector[j].name() == tableName)
			{
				table = layerTablesVector[j];
				if(table.tableType() == TeAttrStatic)
				{
					table.setOrder(order++);
					newTablesVector.push_back(table);
					newUsaStatic = true;
				}
				break;
			}
		}
	}

	for (i = 0; i < themeTablesListBox->count(); ++i)
	{
		tableName = themeTablesListBox->item(i)->text().latin1();
		for (j = 0; j < layerTablesVector.size(); ++j)
		{
			if (layerTablesVector[j].name() == tableName)
			{
				table = layerTablesVector[j];
				if(table.tableType() == TeFixedGeomDynAttr)
				{
					if(newUsaTemporal == true)
					{
						wait.resetWaitCursor();
						QMessageBox::warning(this, tr("Error"),
						tr("Only one temporal table can be used!"));
						return;
					}
					table.setOrder(order++);
					newTablesVector.push_back(table);
					tempTableName = table.name();
					newUsaTemporal = true;
				}
				break;
			}
		}
	}

	// Get the old theme tables
	TeAttrTableVector themeTablesVector;
	curBaseTheme->getAttTables(themeTablesVector);
	bool usaExternal = false;
	for (i = 0; i < themeTablesVector.size(); ++i)
	{
		TeTable t =	themeTablesVector[i];
		if (t.tableType() == TeAttrExternal)
		{
			usaExternal = true;
			newTablesVector.push_back(themeTablesVector[i]);
		}
	}

	if(usaExternal && newUsaTemporal)
	{
		wait.resetWaitCursor();
		int response = QMessageBox::question(this, tr("External table linked"),
			tr("Do you really want to Unlink the external table and link the temporal table:") + tempTableName.c_str() + tr("?"),
				tr("Yes"), tr("No"));

		if (response != 0)
			return;
		else
		{
			i = newTablesVector.size() - 1;
			while(newTablesVector[i].tableType() == TeAttrExternal)
			{
				newTablesVector.pop_back();
				i = newTablesVector.size() - 1;
			}
			usaExternal = false;
		}
	}

	if(usaExternal) // verify if external tables can be linked. If not, remove it.
	{
		vector<TeTable>::iterator it = newTablesVector.begin();
		while(it != newTablesVector.end())
		{
			TeTable t = *it;
			if(t.tableType() == TeAttrExternal)
			{
				string s = TeConvertToUpperCase(t.relatedTableName());
				vector<TeTable>::iterator it2 = newTablesVector.begin();
				while(it2 != newTablesVector.end())
				{
					TeTable t2 = *it2;
					string s2 = TeConvertToUpperCase(t2.name());
					if(s == s2)
						break;
					it2++;
				}
				if(it2 == newTablesVector.end())
				{
					newTablesVector.erase(it);
					it = newTablesVector.begin();
				}
				else
					it++;
			}
			else
				it++;
		}
	}

	// Set the new vector of theme tables
	themeTablesVector.clear ();
	curBaseTheme->clearAttTableVector();
	for (i = 0; i < newTablesVector.size(); ++i)
		curBaseTheme->addThemeTable(newTablesVector[i]);

	db_->updateThemeTable(curBaseTheme);

	mainWindow_->getGrid()->clear();
	if (!curBaseTheme->createCollectionAuxTable() || !curBaseTheme->populateCollectionAux())
	{
		wait.resetWaitCursor();
		QMessageBox::critical(this, tr("Error"),
		tr("Fail to mount the auxiliary table of the collection!"));
		return;
	}
	mainWindow_->getGrid()->init(theme_);
	hide();

}


void SelectThemeTables::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("selThemeTables.htm");
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




void SelectThemeTables::show()
{
	if(theme_->getTheme()->type() == TeTHEME)
		QDialog::show();
	else
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		reject();
		hide();
	}

}
