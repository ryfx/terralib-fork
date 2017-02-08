/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright ? 2001-2007 INPE and Tecgraf/PUC-Rio.
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
#include <TeVectorRemap.h>

void GeoOpOverlayIntersection::init()
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

	if(curTheme && curTheme->type() != TeTHEME)
		curTheme = 0;

	allRadioButton->setEnabled(true);
	pointedRadioButton->setEnabled(true);
	queriedRadioButton->setEnabled(true);
	backValueTextLabel->setEnabled(false);
	backValueLineEdit->clear();
	backValueLineEdit->setEnabled(false);

	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	for (i = j = 0; i < (int)themeItemVec.size(); ++i)
	{
		TeTheme* theme = (TeTheme*)(themeItemVec[i]->getAppTheme())->getTheme();
		if(theme->type() != TeTHEME)
			continue;

		TeGeomRep rep = (TeGeomRep)(theme->visibleGeoRep() & ~TeTEXT); // TeTEXT not is considered

		// filter theme that use layer with only one type of geometric representation
		if(rep == TePOLYGONS || rep == TeLINES || rep == TePOINTS || rep == TeCELLS)
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
		else if (rep == TeRASTER)
		{
		    themeInputComboBox->insertItem(theme->name().c_str());
		    j++;
		}
	}

	if(j <= 1)
	{
		erro_ = true;
		QString msg = tr("You must have, on the view, two or more themes that can be processed!") + "\n";
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


void GeoOpOverlayIntersection::themeInputComboBox_activated( int )
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
		if(theme->type() != TeTHEME)
			continue;

		TeGeomRep rep = (TeGeomRep)(theme->visibleGeoRep() & ~TeTEXT); // TeTEXT not is considered

		// filter theme that use layer with only POLYGON representation
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
		if(theme_->getTheme()->type() != TeTHEME)
			continue;

		if(((TeTheme*)theme_->getTheme())->name().c_str() == ss)
		{	
			if (((TeTheme*)theme_->getTheme())->visibleGeoRep()== TeRASTER)
			{
				TeRasterParams& par = ((TeTheme*)theme_->getTheme())->layer()->raster()->params();
				if (par.useDummy_)
				{
					QString myValue;
					if (par.dataType_[0]==TeFLOAT || par.dataType_[0]==TeDOUBLE)
						myValue.sprintf("%.6f",par.dummy_[0]);
					else
						myValue.sprintf("%.0f",par.dummy_[0]);
					backValueLineEdit->setText(myValue);
					backValueLineEdit->setCursorPosition(0);
				}
				else
					backValueLineEdit->setText("255");
			}
			break;
		}
	}

	s = themeOverlayComboBox->currentText();
	for (i=0; i<(int)themeItemVec.size(); ++i)
	{
		themeTrim_ = themeItemVec[i]->getAppTheme();
		if(themeTrim_->getTheme()->type() != TeTHEME)
			continue;

		QString name = ((TeTheme*)themeTrim_->getTheme())->name().c_str();
		if(name == s)
			break;
	}

	TeGeomRep rep = (TeGeomRep)(((TeTheme*)theme_->getTheme())->visibleGeoRep() & ~TeTEXT);
	if (rep == TeRASTER)
	{
		allRadioButton->setEnabled(false);
		pointedRadioButton->setEnabled(false);
		queriedRadioButton->setEnabled(false);
		backValueTextLabel->setEnabled(true);
		backValueLineEdit->setEnabled(true);
		putOverlayThemeAttributesCheckBox->setEnabled(false);
	}
	else
	{
		allRadioButton->setEnabled(true);
		pointedRadioButton->setEnabled(true);
		queriedRadioButton->setEnabled(true);
		backValueTextLabel->setEnabled(false);
		backValueLineEdit->clear();
		backValueLineEdit->setEnabled(false);
		putOverlayThemeAttributesCheckBox->setEnabled(true);
	}
	string layerName = getNewLayerName(((TeTheme*)theme_->getTheme())->layer());
	layerNameLineEdit->setText(layerName.c_str());
}


void GeoOpOverlayIntersection::themeOverlayComboBox_activated( int )
{
	unsigned int i;
	QString s = themeOverlayComboBox->currentText();

	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	for (i = 0; i < themeItemVec.size(); ++i)
	{
		themeTrim_ = themeItemVec[i]->getAppTheme();
		if(themeTrim_->getTheme()->type() != TeTHEME)
			continue;
		if(((TeTheme*)themeTrim_->getTheme())->name().c_str() == s)
			break;
	}
}



void GeoOpOverlayIntersection::OKPushButton_clicked()
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

	bool isEqualProj = (*((TeTheme*)theme_->getTheme())->layer()->projection() == *((TeTheme*)themeTrim_->getTheme())->layer()->projection());
	if (!isEqualProj &&
		(((TeTheme*)theme_->getTheme())->layer()->projection()->name() == "NoProjection" || 
		 ((TeTheme*)themeTrim_->getTheme())->layer()->projection()->name() == "NoProjection"))
	{
		QMessageBox::warning(this,tr("Error"),
		tr("Themes have different projections that can not be combined: one of them is NoProjection."));
		return;
	}
	TeBox bClip = theme_->boundingBox();
	if (!isEqualProj)
		bClip = TeRemapBox (bClip, ((TeTheme*)theme_->getTheme())->layer()->projection(), ((TeTheme*)themeTrim_->getTheme())->layer()->projection());
	if (!TeIntersects(bClip,themeTrim_->boundingBox()))
	{
		QMessageBox::warning(this,tr("Error"),
		tr("Theme to be clipped and theme with the mask don't intercept."));
		return;
	}

	TeSelectedObjects selOb = TeAll;
	if(pointedRadioButton->isOn())
		selOb = TeSelectedByPointing;
	else if(queriedRadioButton->isOn())
		selOb = TeSelectedByQuery;

	TeSelectedObjects selObTrim = TeAll;
	if(pointedTrimRadioButton->isOn())
		selObTrim = TeSelectedByPointing;
	else if(queriedTrimRadioButton->isOn())
		selObTrim = TeSelectedByQuery;

	bool attrTrim = putOverlayThemeAttributesCheckBox->isChecked();
	TeBox bInvalid;
	TeLayer* newLayer = new TeLayer(layerName, db_, bInvalid, ((TeTheme*)theme_->getTheme())->layer()->projection());
	// create a temporary log file
	TeAsciiFile* logFile=0;
	QDir adir;
	string dirname = adir.absPath().latin1();
	string dirfile;
	int f = dirname.find("\\");
	if(f >= 0)
		dirfile = dirname + "\\" + "tv_geointersec_error.txt";
	else
		dirfile = dirname + "/" + "tv_geointersec_error.txt";
	try
	{
		logFile = new TeAsciiFile(dirfile,"w");
	}
	catch(...)
	{
		logFile = 0;
	}

	TeWaitCursor wait;
	bool result;
	if (backValueLineEdit->isEnabled())
		result = TeGeoOpOverlayIntersection(newLayer, (TeTheme*)theme_->getTheme(), (TeTheme*)themeTrim_->getTheme(), selObTrim, backValueLineEdit->text().toDouble(),logFile);
	else
		result = TeGeoOpOverlayIntersection(newLayer, (TeTheme*)theme_->getTheme(), (TeTheme*)themeTrim_->getTheme(), selOb, selObTrim, attrTrim,logFile);
	wait.resetWaitCursor();
	if(result)
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

		QString msg1 = tr("Overlay Intersection executed!");
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
			QString msg = tr("Overlay Intersection not executed!");
			QMessageBox::warning(this, tr("Error"), msg);
		}
		reject();
	}
}


void GeoOpOverlayIntersection::reject()
{
	hide();
}


void GeoOpOverlayIntersection::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("geoOpOverlayIntersection.htm");
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

string GeoOpOverlayIntersection::getNewLayerName( TeLayer *layer )
{
	string name = layer->name();
	size_t f = name.find("_clip");
	if(f == string::npos)
		name = name + "_clip";
	
	return(db_->getNewLayerName(name));
}



