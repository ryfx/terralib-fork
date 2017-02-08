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
#include <display.h>
#include <TeView.h>
#include <TeQtViewsListView.h>
#include <TeGeoProcessingFunctions.h>
#include <TeQtDatabaseItem.h>
#include <TeQtDatabasesListView.h>
#include <TeQtLayerItem.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <TeQtGrid.h>
#include <help.h>
#include <TeAsciiFile.h>
#include <qfiledialog.h>
#include <TeWaitCursor.h>

void GeoOpOverlayUnion::init()
{
	int i, j, k = -1, n;
	help_ = 0;
	erro_ = false;
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	db_ = mainWindow_->currentDatabase();
	if(db_ == 0 || mainWindow_->currentView() == 0)
	{
		QString msg = tr("You must select, before, a view !") + "\n";
		msg += tr("Select a view and try again.");
		QMessageBox::warning(this, tr("Warning"), msg);
		erro_ = true;;
		return;
	}

	TeAppTheme* curAppTheme = mainWindow_->currentTheme();
	TeTheme* curTheme;
	if (curAppTheme)
		curTheme = (TeTheme*)curAppTheme->getTheme();
	else
		curTheme = 0;

	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	for (i = j = 0; i < (int)themeItemVec.size(); ++i)
	{
		TeTheme* theme = (TeTheme*)(themeItemVec[i]->getAppTheme())->getTheme();
		TeGeomRep rep = (TeGeomRep)(theme->visibleGeoRep() & ~TeTEXT); // TeTEXT not is considered

		if(rep == TePOLYGONS)
		{
			// filter theme that use only static or event table
			TeAttrTableVector themeTablesVector; 
			theme->getAttTables(themeTablesVector);
		
			for(n=0; n<(int)themeTablesVector.size(); n++)
			{
				TeTable table = themeTablesVector[n];
				if(!(table.tableType() == TeAttrStatic || table.tableType() == TeAttrEvent))
					break;
			}
			if(n == (int)themeTablesVector.size())
			{
				if(curTheme != 0 && (curTheme->id() == theme->id()))
					k = j;
				string name = theme->name();
				themeInputComboBox->insertItem(name.c_str());
				j++;
			}
		}
	}
	if(j <= 1)
	{
		erro_ = true;
		QString msg = tr("You must have, on the view, two or more themes with polygon representation!") + "\n";
		msg += tr("The themes must have only one visible representation.") + "\n";
		msg += tr("Add other themes, on the view, and try again.");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

	if(k >= 0)
		themeInputComboBox->setCurrentItem(k);
	else
		themeInputComboBox->setCurrentItem(0);

	themeInputComboBox_activated(themeInputComboBox->currentItem());
}


void GeoOpOverlayUnion::themeInputComboBox_activated( int )
{
	int	i, n;
	QString s = themeOverlayComboBox->currentText();
	QString ss = themeInputComboBox->currentText();

	themeOverlayComboBox->clear();

	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	for (i = 0; i < (int)themeItemVec.size(); ++i)
	{
		TeTheme* theme = (TeTheme*)(themeItemVec[i]->getAppTheme())->getTheme();
		TeGeomRep rep = (TeGeomRep)(theme->visibleGeoRep() & ~TeTEXT); // TeTEXT not is considered

		if(rep == TePOLYGONS)
		{
			// filter theme that use only static or event table
			TeAttrTableVector themeTablesVector; 
			theme->getAttTables(themeTablesVector);
		
			for(n=0; n<(int)themeTablesVector.size(); n++)
			{
				TeTable table = themeTablesVector[n];
				if(!(table.tableType() == TeAttrStatic || table.tableType() == TeAttrEvent))
					break;
			}

			if(n == (int)themeTablesVector.size())
			{
				QString sss = theme->name().c_str();
				if(sss != ss)
					themeOverlayComboBox->insertItem(sss);
			}
		}
	}
	if(s != ss)
	{
		for(i=0; i<(int)themeOverlayComboBox->count(); ++i)
		{
			QString sss = themeOverlayComboBox->text(i);
			if(sss == s)
			{
				themeOverlayComboBox->setCurrentItem(i);
				break;
			}
		}
	}
	else
		themeOverlayComboBox->setCurrentItem(0);

	for (i=0; i<(int)themeItemVec.size(); ++i)
	{
		theme_ = themeItemVec[i]->getAppTheme();
		QString name = ((TeTheme*)theme_->getTheme())->name().c_str();
		if(name == ss)
			break;
	}

	s = themeOverlayComboBox->currentText();
	for (i=0; i<(int)themeItemVec.size(); ++i)
	{
		themeOverlay_ = themeItemVec[i]->getAppTheme();
		QString name = ((TeTheme*)themeOverlay_->getTheme())->name().c_str();
		if(name == s)
			break;
	}

	string layerName = getNewLayerName(((TeTheme*)theme_->getTheme())->layer());
	layerNameLineEdit->setText(layerName.c_str());
}

void GeoOpOverlayUnion::themeOverlayComboBox_activated( int )
{
	unsigned int i;
	QString s = themeOverlayComboBox->currentText();

	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	for (i = 0; i < themeItemVec.size(); ++i)
	{
		themeOverlay_ = themeItemVec[i]->getAppTheme();
		QString name = ((TeTheme*)themeOverlay_->getTheme())->name().c_str();
		if(name == s)
			break;
	}
}

void GeoOpOverlayUnion::oKPushButton_clicked()
{
	string layerName = layerNameLineEdit->text().latin1();
	if(layerName.empty())
	{
		QMessageBox::warning(this,tr("Warning"),
		tr("Enter output layer name and try again!"));
		return;
	}
	string errorMessage;
	bool changed;
	string newName = TeCheckName(layerName, changed, errorMessage); 
	if(changed)
	{
		QString mess = tr("The output layer name is invalid: ") + errorMessage.c_str();
		mess += "\n" + tr("Change current name and try again.");
		QMessageBox::warning(this, tr("Warning"), mess);
		layerNameLineEdit->setText(newName.c_str());
		return;
	}
	layerName = newName;
	if (db_->layerExist(layerName))
	{
		QMessageBox::warning(this,tr("Warning"),
		tr("Output layer name already exist, replace and try again!"));
		return;
	}
	TeSelectedObjects selOb = TeAll;
	if(pointedRadioButton->isOn())
		selOb = TeSelectedByPointing;
	else if(queriedRadioButton->isOn())
		selOb = TeSelectedByQuery;

	TeSelectedObjects selObOverlay = TeAll;
	if(pointedTrimRadioButton->isOn())
		selObOverlay = TeSelectedByPointing;
	else if(queriedTrimRadioButton->isOn())
		selObOverlay = TeSelectedByQuery;

	TeBox bInvalid;
	TeLayer* newLayer = new TeLayer(layerName, db_, bInvalid, ((TeTheme*)theme_->getTheme())->layer()->projection());
	QString qlog = tr("Do you wish to have an error log file for this operation?");
	QString logc = tr("Log file");
	TeAsciiFile* logFile = 0;
	int resp = QMessageBox::question(this, logc, qlog, tr("Yes"), tr("No"));
	if ( resp == 0)
	{
		
		QString qfileName = QFileDialog::getSaveFileName(".","Log (*.txt)",this,
                    "Save log file"
                    "Choose a name for the error log file");
		if (qfileName.isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Please indicate a valid filename for the log file."));
			return;
		}
		try
		{
			logFile = new TeAsciiFile(qfileName.latin1(),"w");
		}
		catch(...)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Couldn't not open the log file."));
			return;
		}
	}
	TeWaitCursor wait;
	bool res = TeGeoOpOverlayUnion(newLayer, (TeTheme*)theme_->getTheme(), (TeTheme*)themeOverlay_->getTheme(), selOb, selObOverlay);
	wait.resetWaitCursor();
	if(res)
	{	
		TeQtDatabasesListView *databasesListView = mainWindow_->getDatabasesListView();
		TeQtDatabaseItem *databaseItem = databasesListView->currentDatabaseItem();

		TeQtLayerItem *layerItem = new TeQtLayerItem(databaseItem, newLayer->name().c_str(), newLayer);
		layerItem->setEnabled(true);
		if (databasesListView->isOpen(layerItem->parent()) == false)
			databasesListView->setOpen(layerItem->parent(),true);
		mainWindow_->checkWidgetEnabling();

		QString msg1 = tr("Overlay union executed!");
		QString msg2 = tr("The layer:") + " ";
		msg2 += layerName.c_str();
		msg2 += " " + tr("was created with success!") + "\n";
		msg2 +=	tr("Do you want to display the data?");
		int response = QMessageBox::question(this, msg1, msg2, tr("Yes"), tr("No"));

		if (response != 0)
			return;

		string themeName = db_->getNewThemeName(layerName);
		TeView* view = mainWindow_->currentView();
		TeAttrTableVector tablesVector;
		newLayer->getAttrTables(tablesVector);

		mainWindow_->createTheme(themeName, view, newLayer, "", tablesVector);

		TeQtViewsListView* viewsListView = mainWindow_->getViewsListView();
		TeQtViewItem* viewItem = viewsListView->currentViewItem();
		TeQtThemeItem* themeItem = (TeQtThemeItem*)viewItem->firstChild();
		TeQtThemeItem* newActiveThemeItem;
		while(themeItem)
		{
			if (((TeTheme*)themeItem->getAppTheme()->getTheme())->name() == themeName)
			{
				newActiveThemeItem = themeItem;
				themeItem->setState(QCheckListItem::On);
				((TeTheme*)themeItem->getAppTheme()->getTheme())->visibility(true);
			}
			else
			{
				themeItem->setState(QCheckListItem::Off);
				((TeTheme*)themeItem->getAppTheme()->getTheme())->visibility(false);
			}

			themeItem = (TeQtThemeItem*)themeItem->nextSibling();
		}
		if(newActiveThemeItem)
			viewsListView->selectItem((TeQtViewItem*)newActiveThemeItem);

		mainWindow_->resetAction_activated();
		accept();
	}
	else
	{
		db_->deleteLayer(newLayer->id());
		{
			QString msg = tr("Overlay union not executed!");
			QMessageBox::warning(this, tr("Error"), msg);
		}
		reject();
	}
}


void GeoOpOverlayUnion::reject()
{
	hide();
}


void GeoOpOverlayUnion::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("geoOpOverlayUnion.htm");
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

string GeoOpOverlayUnion::getNewLayerName( TeLayer *layer )
{
	string name = layer->name();
	size_t f = name.find("_union");
	if(f == string::npos)
		name = name + "_union";
	
	return(db_->getNewLayerName(name));
}


bool GeoOpOverlayUnion::isValidName( QString s )
{
	if(s.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Specify the output layer name and try again!"));
		return false;
	}
	else // verify if name is valid
	{
		QString msg;
		string layerName = s.latin1();
		int i = layerName.find(" ");
		if(i >= 0)
		{
			int response = QMessageBox::question(this, tr("Question"),
				tr("The layer name cannot have blank spaces!\nDo you wish to bypass them automatically?"),
				tr("Yes"), tr("No"));

			if(response == 0)
			{
				while((i = layerName.find(" ")) >= 0)
					layerName[i] = '_';
				layerNameLineEdit->setText(layerName.c_str());
			}
			else
				return false;
		}

		if(layerName[0] >= 0x30 && layerName[0] <= 0x39)
		{
			msg = tr("The layer name cannot begin with a numeric character.");
			msg += "\n" + tr("Specify other layer name and try again!");			
			QMessageBox::warning(this, tr("Warning"), msg);
			return false;
		}
		int f1 = layerName.find("*");
		int f2 = layerName.find("/");
		int f3 = layerName.find("-");
		int f4 = layerName.find("+");
		int f5 = layerName.find("=");
		int f6 = layerName.find("%");
		int f7 = layerName.find(">");
		int f8 = layerName.find("<");
		if(f1>=0 || f2>=0 || f3>=0 || f4>=0 || f5>=0 || f6>=0 || f7>=0 || f8>=0)
		{
			msg = tr("The layer name cannot have mathematical symbols like: *, /, +, -, =, %, >, <");
			msg += "\n" + tr("Specify other layer name and try again!");			
			QMessageBox::warning(this, tr("Warning"), msg);
			return false;
		}
		string u = TeConvertToUpperCase(layerName);
		if(u=="OR" || u=="AND" || u=="NOT" || u=="LIKE")
		{
			msg = tr("The layer name cannot be logical name operators like: OR, AND, NOT, LIKE");
			msg += "\n" + tr("Specify other layer name and try again!");
			QMessageBox::warning(this, tr("Warning"), msg);

			return false;
		}

		if(u=="SELECT" || u=="FROM" || u=="UPDATE" || u=="DELETE" ||u=="BY" || u=="GROUP" || u=="ORDER" ||
		   u=="DROP" || u=="INTO" || u=="VALUE" || u=="IN" || u=="ASC" || u=="DESC"|| u=="COUNT" || u=="JOIN" ||
		   u=="LEFT" || u=="RIGHT" || u=="INNER" || u=="UNION" || u=="IS" || u=="NULL" || u=="WHERE" ||
		   u=="BETWEEN" || u=="DISTINCT" || u=="TRY" || u=="IT" || u=="INSERT" || u=="ALIASES" || u=="CREATE" ||
		   u=="ALTER" || u=="TABLE" || u=="INDEX" || u=="ALL" || u=="HAVING")
		{
			msg = "'";
			msg += layerName.c_str() + tr("' is a SQL reserved word!") + "\n";
			msg += tr("Enter with another layer name, don´t using SQL reserved words like:") + "\n";
			msg += "SELECT, FROM, UPDATE, DELETE, BY, GROUP, ORDER, DROP, INTO, VALUE, IN, ASC, DESC,\n";
			msg += "COUNT, JOIN, LEFT, RIGHT, INNER, UNION, IS, NULL, WHERE, BETWEEN, DISTINCT,\n";
			msg += "TRY, IT, INSERT, ALIASES, CREATE, ALTER, TABLE, INDEX, ALL, HAVING...";
			msg += "\n" + tr("Specify other layer name and try again!");
			QMessageBox::warning(this, tr("Warning"), msg);
			return false;
		}

		if(db_->tableExist(layerName))
		{
			msg = tr("The specified layer already exist.");
			msg += "\n" + tr("Specify other layer name and try again!");
			QMessageBox::warning(this, tr("Warning"), msg);
			return false;
		}
		return true;
	}
}

