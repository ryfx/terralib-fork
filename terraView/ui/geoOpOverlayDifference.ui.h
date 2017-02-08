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
#include <help.h>
#include <qfiledialog.h>
#include <TeAsciiFile.h>
#include <qfile.h>
#include <TeWaitCursor.h>

void GeoOpOverlayDifference::init()
{
	help_ = 0;
	erro_ = false;
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	db_ = mainWindow_->currentDatabase();
	if(db_ == 0 || mainWindow_->currentView() == 0)
	{
		QString msg = tr("You must select, before, a view !") + "\n";
		msg += tr("Select a view and try again.");
		QMessageBox::warning(this, tr("Warning"), msg);
		erro_ = true;
		return;
	}
	TeAppTheme* curAppTheme = mainWindow_->currentTheme();
	TeTheme* curTheme;
	if (curAppTheme)
		curTheme = (TeTheme*)curAppTheme->getTheme();
	else
		curTheme = 0;

	if(curTheme && curTheme->type() != TeTHEME)
		curTheme = 0;

	unsigned int i, j, n, k;
	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	for (i = j = 0; i < themeItemVec.size(); ++i)
	{
		TeTheme* theme = (TeTheme*)(themeItemVec[i]->getAppTheme())->getTheme();
		if(theme->type() != TeTHEME)
			continue;

		TeGeomRep rep = (TeGeomRep)(theme->visibleGeoRep() & ~TeTEXT); // TeTEXT not is considered

		// filter themes that use layers with TePolygon representation
		if (rep == TePOLYGONS)
		{
			// filter theme that use only static or event table
			TeAttrTableVector themeTablesVector; 
			theme->getAttTables(themeTablesVector);
		
			for (n=0; n<themeTablesVector.size(); n++)
			{
				TeTable table = themeTablesVector[n];
				if(!(table.tableType() == TeAttrStatic || table.tableType() == TeAttrEvent))
					break;
			}

			if (n == themeTablesVector.size())
			{
				if(curTheme != 0 && (curTheme->id() == theme->id()))
					k = j;
				theme1ComboBox->insertItem(theme->name().c_str());
				theme2ComboBox->insertItem(theme->name().c_str());
				j++;
			}
		}
	}
	if (theme1ComboBox->count() < 2)
	{
		QString msg = tr("You must have at least two themes with polygon representation!") + "\n";
		QMessageBox::warning(this, tr("Warning"), msg);
		erro_ = true;
		return;
	}
	theme1ComboBox->setCurrentItem(k);
	theme2ComboBox->setCurrentItem(1);
}

void GeoOpOverlayDifference::okPushButton_clicked()
{
	if (theme1ComboBox->currentItem() == theme2ComboBox->currentItem())
	{
		QString msg = tr("You have to choose two different themes to make the difference!") + "\n";
		QMessageBox::warning(this, tr("Warning"), msg);
		erro_ = true;
		return;
	}

	TeTheme *theme1=0, *theme2=0;
	string t1 = theme1ComboBox->currentText().latin1();
	string t2 = theme2ComboBox->currentText().latin1();
	unsigned int i;
	vector<TeViewNode*>& themeVec = mainWindow_->currentView()->themes();
	for (i=0; i<themeVec.size(); ++i)
	{
		if (((TeTheme*)themeVec[i])->name() == t1)
			theme1 = (TeTheme*)themeVec[i];
		if (((TeTheme*)themeVec[i])->name() == t2)
			theme2 = (TeTheme*)themeVec[i];
		if (theme1 != 0 && theme2 != 0)
			break;
	}

	if (theme1 == 0 || theme2 == 0 ||
		(theme1 == theme2))
	{
		QString msg = tr("You have to choose two different themes to make the difference!") + "\n";
		QMessageBox::warning(this, tr("Warning"), msg);
		erro_ = true;
		return;
	}
	string layerName = specifyOutputLayerLineEdit->text().latin1();
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
		specifyOutputLayerLineEdit->setText(newName.c_str());
		return;
	}
	layerName = newName;
	if (db_->layerExist(layerName))
	{
		QMessageBox::warning(this,tr("Warning"),
		tr("Output layer name already exist, replace and try again!"));
		return;
	}
	TeBox bInvalid;
	TeLayer* newLayer = new TeLayer(layerName, db_, bInvalid, theme1->layer()->projection());
	if (!newLayer || newLayer->id() <= 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Couldn't not create the new layer."));
		return;
	}
	// create a temporary log file
	TeAsciiFile* logFile=0;
	QDir adir;
	string dirname = adir.absPath().latin1();
	string dirfile;
	int f = dirname.find("\\");
	if(f >= 0)
		dirfile = dirname + "\\" + "tv_geodiff_error.txt";
	else
		dirfile = dirname + "/" + "tv_geodiff_error.txt";
	try
	{
		logFile = new TeAsciiFile(dirfile,"w");
	}
	catch(...)
	{
		logFile = 0;
	}
	TeWaitCursor wait;
	bool res = TeGeoOpOverlayDifference(newLayer, theme1, theme2, static_cast<TeSelectedObjects>(theme1SelObjButtonGroup->selectedId()),
								static_cast<TeSelectedObjects>(theme2SelObjButtonGroup->selectedId()),logFile);
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

		string logfilename;
		if (logFile)
		{
			logfilename = logFile->name();
			fclose(logFile->FilePtr());
			
	        QFile file(logfilename.c_str());
	        file.open( IO_ReadOnly ); 
			if (file.size() <= 0)
			{
				file.remove();
				logfilename = "";
			}
    	}

		QString msg1 = tr("Difference executed!");
		QString msg2 = tr("The layer:") + " ";
		msg2 += layerName.c_str();
		msg2 += " " + tr("was created with success!");
		if (!logfilename.empty())
		{
			msg2 += "\n";
			msg2 += tr("(Some geometrical errors were detected. Check the file: \"");
			msg2 += dirfile.c_str();
			msg2 += "\")";
		}
		msg2 += "\n";
		msg2 +=	tr("Do you want to display the data?");
		if (QMessageBox::question(this, msg1, msg2, tr("Yes"), tr("No")) != 0)
		{
			accept();
			return;
		}

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
			QString msg = tr("Error executing difference...");
			QMessageBox::warning(this, tr("Error"), msg);
		}
		reject();
	}
}


void GeoOpOverlayDifference::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("GeoOpDifference.htm");
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


void GeoOpOverlayDifference::reject()
{
	hide();
}



string GeoOpOverlayDifference::getNewLayerName(TeLayer *layer)
{
	string name = layer->name();
	size_t f = name.find("_diff");
	if(f == string::npos)
		name = name + "_diff";
	
	return(db_->getNewLayerName(name));
}

