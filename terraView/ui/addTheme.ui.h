/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright � 2001-2007 INPE and Tecgraf/PUC-Rio.
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
#include <TeQtViewsListView.h>


void AddTheme::init() //
{
	help_ = 0;
	TeWaitCursor wait;
	unsigned int i;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();

	//Fill the database큦 lineEdit with the name of the current database
	db_ = mainWindow_->currentDatabase();
	currentDatabaseLineEdit->setText(QString(db_->databaseName().c_str()));

	// Fill the view큦 combobox with the list of views of the current database
	TeViewMap& viewMap = db_->viewMap();
	TeViewMap::iterator viewIt;
	for (viewIt = viewMap.begin(); viewIt != viewMap.end(); ++viewIt)
	{
		QString viewName = viewIt->second->name().c_str();
		viewComboBox->insertItem(viewName);
	}
	
	//Initially select the view that will contain the theme to be added
	//as the current view in the main window
	themeView_ = mainWindow_->currentView();
	if (themeView_ != 0)
	{
		i = 0;
		for (viewIt = viewMap.begin(); viewIt != viewMap.end(); ++viewIt)
		{
			if(viewIt->second->id() == themeView_->id())
			{
				viewComboBox->setCurrentItem(i);
				break;
			}
			i++;
		}
	}
	else
		viewComboBox->setCurrentItem(0);

	//update the view to be used (themeView_)
	viewComboBox_activated(viewComboBox->currentText());

	// Fill the layer큦 combobox with the list of layers of the current database
	TeLayerMap& layerMap = db_->layerMap();
	TeLayerMap::iterator layerIt;
	for (layerIt = layerMap.begin(); layerIt != layerMap.end(); ++layerIt)
	{
		TeLayer* l = layerIt->second;
		if (l)
			layerComboBox->insertItem(QString(layerIt->second->name().c_str()));
	}

	//If there is a current layer, set it as the current item on the layer큦 combobox;
	themeLayer_ = mainWindow_->currentLayer();
	if (themeLayer_ != 0)
	{
		i = 0;
		for (layerIt = layerMap.begin(); layerIt != layerMap.end(); ++layerIt)
		{
			if(layerIt->second->id() == themeLayer_->id())
			{
				layerComboBox->setCurrentItem(i);
				break;
			}
			i++;
		}
	}
	else
		layerComboBox->setCurrentItem(0);

	layerComboBox_activated(layerComboBox->currentText());
}


void AddTheme::layerTablesListBox_clicked( QListBoxItem *item )
{
	if (!item)
		return;

	unsigned int i, j;
	string s = item->text().latin1();

	bool staticItemType = false;
	bool eventItemType = false;
	bool dynAttrItemType = false;
	bool dynGeomAttrItemType = false;


	TeAttrTableVector layerTablesVector;
	themeLayer_->getAttrTables(layerTablesVector);

	for(i = 0; i < layerTablesVector.size(); ++i)
	{
		if(layerTablesVector[i].name() == s)
		{
			if (layerTablesVector[i].tableType() == TeAttrStatic)
			{
				tableTypeLineEdit->setText(tr("Static"));
				staticItemType = true;
			}
			else if (layerTablesVector[i].tableType() == TeAttrEvent)
			{
				tableTypeLineEdit->setText(tr("Event"));
				eventItemType = true;
			}
			else if (layerTablesVector[i].tableType() == TeFixedGeomDynAttr)
			{
				tableTypeLineEdit->setText(tr("Dynamic Attribute"));
				dynAttrItemType = true;
			}
			else if (layerTablesVector[i].tableType() == TeDynGeomDynAttr)
			{
				tableTypeLineEdit->setText(tr("Dynamic Geometry and Dynamic Attribute"));
				dynGeomAttrItemType = true;
			}
			break;
		}
	}
	removeTablePushButton->setEnabled(false);

	if (themeTablesListBox->count() == 0)
		addTablePushButton->setEnabled(true);
	else
	{
		bool staticType = false;
		bool eventType = false;
		bool dynAttrType = false;
		bool dynGeomAttrType = false;

		for (i = 0; i < themeTablesListBox->count(); ++i)
		{
			string tableName = themeTablesListBox->text(i).latin1();
			for(j = 0; j < layerTablesVector.size(); ++j)
			{
				if(layerTablesVector[j].name() == tableName)
				{
					if (layerTablesVector[j].tableType() == TeAttrStatic)
						staticType = true;
					else if (layerTablesVector[j].tableType() == TeAttrEvent)
						eventType = true;
					else if (layerTablesVector[j].tableType() == TeFixedGeomDynAttr)
						dynAttrType = true;
					else if (layerTablesVector[j].tableType() == TeDynGeomDynAttr)
						dynGeomAttrType = true;
				}
			}
		}

		if (eventType)
			addTablePushButton->setEnabled(false);
		else if (staticType && !dynAttrType && !dynGeomAttrType)
		{
			if (eventItemType)
				addTablePushButton->setEnabled(false);
			else
				addTablePushButton->setEnabled(true);
		}
		else if (staticType && (dynAttrType || dynGeomAttrType))
		{
			if (staticItemType)
				addTablePushButton->setEnabled(true);
			else
				addTablePushButton->setEnabled(false);
		}
		else if (dynAttrType || dynGeomAttrType)
		{
			if (staticItemType)
				addTablePushButton->setEnabled(true);
			else
				addTablePushButton->setEnabled(false);
		}
	}
}


void AddTheme::themeTablesListBox_clicked( QListBoxItem *item )
{
	if (!item)
		return;

	string s = item->text().latin1();

	TeAttrTableVector layerTablesVector;
	themeLayer_->getAttrTables(layerTablesVector);

	for(unsigned int i = 0; i < layerTablesVector.size(); ++i)
	{
		if(layerTablesVector[i].name() == s)
		{
			if (layerTablesVector[i].tableType() == TeAttrStatic)
				tableTypeLineEdit->setText(tr("Static"));
			else if (layerTablesVector[i].tableType() == TeAttrEvent)
				tableTypeLineEdit->setText(tr("Event"));
			else if (layerTablesVector[i].tableType() == TeFixedGeomDynAttr)
				tableTypeLineEdit->setText(tr("Dynamic Attribute"));
			else if (layerTablesVector[i].tableType() == TeDynGeomDynAttr)
				tableTypeLineEdit->setText(tr("Dynamic Geometry and Dynamic Attribute"));
			break;
		}
	}
	addTablePushButton->setEnabled(false);
	removeTablePushButton->setEnabled(true);
}


void AddTheme::layerComboBox_activated(const QString& layerName)
{
	if (layerName.isEmpty())
		return;

	//Set the layer according the selection made in the layer큦 comboBox
	TeLayerMap& layerMap = db_->layerMap();
	TeLayerMap::iterator it;
	TeLayer *layer;
	for (it = layerMap.begin(); it != layerMap.end(); ++it)
	{
		layer = it->second;
		if (layer->name() == layerName.latin1())
			break;
	}
	themeLayer_ = layer;
//	setThemeName(themeLayer_->name());
	string themeName = db_->getNewThemeName(themeLayer_->name());
	themeNameLineEdit->setText(themeName.c_str());

	// Clear the list boxes associated to the layer and theme tables 
	themeTablesListBox->clear();
	layerTablesListBox->clear();

	// Disable the add, remove and OK pushButtons,
	// and the selectAllObjects checkBox
	addTablePushButton->setEnabled(false);
	removeTablePushButton->setEnabled(false);
	okPushButton->setEnabled(true);
	selectAllObjectsCheckBox->setEnabled(false);

	//Get the layer tables
	TeAttrTableVector layerTablesVector;
	if(themeLayer_->getAttrTables(layerTablesVector) == false)
	{
		if(themeLayer_->hasGeometry(TeRASTER) || themeLayer_->hasGeometry(TeRASTERFILE))
		{
			okPushButton->setEnabled(true);
			selectAllObjectsCheckBox->setChecked(true);
			selectAllObjectsCheckBox_toggled(true);
			return;
		}

		QMessageBox::critical(this, tr("Error"),
			tr("Fail to get the infolayer tables!"));
		hide();
	}

	selectAllObjectsCheckBox->setChecked(true);
	selectAllObjectsCheckBox_toggled(true);
	selectAllObjectsCheckBox->setEnabled(true);
	okPushButton->setEnabled(true);

	// Insert the name of the tables in the list boxes
	if (layerTablesVector.size() == 1)
	{
		themeTablesListBox->insertItem(layerTablesVector[0].name().c_str());
		removeTablePushButton->setEnabled(false);
	}
	else
	{
		addTablePushButton->setEnabled(true);
		for(unsigned int i = 0; i < layerTablesVector.size(); ++i)
		{
			if(layerTablesVector[i].tableType() != TeAttrMedia)
				layerTablesListBox->insertItem(layerTablesVector[i].name().c_str());
		}
	}

	tableTypeLineEdit->setText("");
	addTablePushButton->setEnabled(false);
	removeTablePushButton->setEnabled(false);

	TeTheme *t = new TeTheme("", themeLayer_);
	TeAppTheme a(t);
	dummyTheme_ = a;
}


void AddTheme::viewComboBox_activated(const QString& viewName)
{
	if (viewName.isEmpty())
		return;

	TeViewMap& viewMap = db_->viewMap();
	TeViewMap::iterator it;
	TeView *view;
	for (it = viewMap.begin(); it != viewMap.end(); ++it)
	{
		view = it->second;
		if (view->name() == viewName.latin1())
			break;
	}
	themeView_ = view;
}


void AddTheme::addTablePushButton_clicked()
{
	// Insert the table in the list of theme tables 
	themeTablesListBox->insertItem(layerTablesListBox->currentText());
	layerTablesListBox->removeItem(layerTablesListBox->currentItem());
	if(layerTablesListBox->count() == 0)
		addTablePushButton->setEnabled(false);
	addTablePushButton->setEnabled(false);
	removeTablePushButton->setEnabled(false);
	tableTypeLineEdit->setText("");
}


void AddTheme::removeTablePushButton_clicked()
{
	layerTablesListBox->insertItem(themeTablesListBox->currentText());
	themeTablesListBox->removeItem(themeTablesListBox->currentItem());
	if(themeTablesListBox->count() == 0)
		removeTablePushButton->setEnabled(false);
	addTablePushButton->setEnabled(false);
	removeTablePushButton->setEnabled(false);
	tableTypeLineEdit->setText("");
}


void AddTheme::selectAllObjectsCheckBox_toggled(bool isAll)
{
	if (isAll)
	{
		if (extension() != 0)
			showExtension(false);
	}
	else
	{
		if (extension() != 0)
			setExtension(0);

		queryWindow_ = new QueryWindow();
		queryWindow_->newQueryPushButton->setEnabled(false);
		queryWindow_->filterQueryPushButton->setEnabled(false);
		queryWindow_->addToQueryPushButton->setEnabled(false);
		queryWindow_->cancelPushButton->setEnabled(false);
		queryWindow_->helpPushButton->setEnabled(false);
		setExtension(queryWindow_);
		setOrientation(Qt::Horizontal);
		showExtension(true);

		// Get the layer tables
		TeAttrTableVector layerTablesVector;
		themeLayer_->getAttrTables(layerTablesVector);

		TeTheme* dummyTheme = (TeTheme*)dummyTheme_.getTheme();
		dummyTheme->layer(themeLayer_);

		// Set the tables of the dummy theme
		dummyTheme->clearAttTableVector();
		for (unsigned i = 0; i < themeTablesListBox->count(); ++i)
		{
			string tableName = themeTablesListBox->item(i)->text().latin1();
			// Given the table name, get the table from the vector of layer tables
			for (unsigned j = 0; j < layerTablesVector.size(); ++j)
			{
				TeTable& table = layerTablesVector[j];
				if (table.name() == tableName)
				{
					table.setOrder(i);
					dummyTheme->addThemeTable(table);
					break;
				}
			}
		}
		queryWindow_->setTheme(&dummyTheme_);	
	}
}


void AddTheme::okPushButton_clicked()
{
	TeWaitCursor wait;
	unsigned int i, j;
	string tableName;

	if(themeLayer_ == 0)
	{
		wait.resetWaitCursor();
		QMessageBox::critical(this, tr("Error"),
		tr("Invalid null layer name!"));
		return;
	}

	if(!themeLayer_->hasGeometry(TeRASTER) && !themeLayer_->hasGeometry(TeRASTERFILE))
	{
		if(themeTablesListBox->count() == 0)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"),
			tr("There is no tables in the theme!"));
			return;
		}
	}

	if (themeNameLineEdit->text().isEmpty())
	{
		wait.resetWaitCursor();
		QMessageBox::warning(this,tr("Warning"),
		tr("Give the name of the theme to be added!"));
		return;
	}

	themeName_ = themeNameLineEdit->text().latin1();

	if(selectAllObjectsCheckBox->isOn() == false)
		whereClause_ = queryWindow_->queryTextEdit->text().latin1();

	//Get the layer tables
	TeAttrTableVector layerTablesVector;
	themeLayer_->getAttrTables(layerTablesVector);

	//Fill the vector of theme tables
	for (i = 0; i < themeTablesListBox->count(); ++i)
	{
		tableName = themeTablesListBox->item(i)->text().latin1();
		// Given the table name, get the table from the vector of layer tables
		for (j = 0; j < layerTablesVector.size(); ++j)
		{
			TeTable& table = layerTablesVector[j];
			if (table.name() == tableName)
			{
				table.setOrder(i);
				themeTablesVector_.push_back(table);
				break;
			}
		}
	}

	accept();
}



TeAttrTableVector& AddTheme::themeTablesVector()
{
	return themeTablesVector_;
}

string AddTheme::themeName()
{
	return themeName_;
}

string AddTheme::whereClause()
{
	return whereClause_;
}

TeView* AddTheme::themeView()
{
	return themeView_;
}

TeLayer* AddTheme::themeLayer()
{
	return themeLayer_;
}

void AddTheme::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("addTheme.htm");
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

