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
#include <qfile.h>
#include <TeAsciiFile.h>
#include <TeWaitCursor.h>

void GeoOpAdd::init()
{
	theme_ = 0;
	int	i, j;
	help_ = 0;
	erro_ = false;
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	db_ = mainWindow_->currentDatabase();

	TeAppTheme* curAppTheme = mainWindow_->currentTheme();
	TeTheme* curTheme;
	if (curAppTheme)
		curTheme = (TeTheme*)curAppTheme->getTheme();
	else
		curTheme = 0;

	if(curTheme && curTheme->type() != TeTHEME)
		curTheme = 0;

	map<TeGeomRep, int> repsMap;
	repsMap[TePOLYGONS] = repsMap[TeLINES] = repsMap[TePOINTS] = repsMap[TeCELLS] = 0;

	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	for (i = j = 0; i < (int)themeItemVec.size(); ++i)
	{
		TeTheme* theme = (TeTheme*)(themeItemVec[i]->getAppTheme())->getTheme();
		if(theme->type() != TeTHEME)
			continue;

		TeGeomRep gType = (TeGeomRep)(theme->visibleGeoRep() & ~TeTEXT);

		// filter theme that use only static or event table
		TeAttrTableVector themeTablesVector; 
		theme->getAttTables(themeTablesVector);
		
		for(j=0; j<(int)themeTablesVector.size(); j++)
		{
			TeTable table = themeTablesVector[j];
			if(!(table.tableType() == TeAttrStatic || table.tableType() == TeAttrEvent))
				break;
		}
		if(j == (int)themeTablesVector.size())
		{
			if(gType == TePOLYGONS || gType == TeLINES || gType == TePOINTS || gType == TeCELLS)
				repsMap[gType]++;
		}
	}

	// try to get the default representation from the current theme
	TeGeomRep tRep = TeGEOMETRYNONE;
	bool ok=false;
	if(curTheme)
	{
		tRep = (TeGeomRep)(curTheme->visibleGeoRep() & ~TeTEXT); 
		if(tRep == TePOLYGONS || tRep == TeLINES || tRep == TePOINTS || tRep == TeCELLS)
		{
			if(repsMap[tRep] > 1)
			{
				if(tRep == TePOLYGONS)
					representationButtonGroup->setButton(representationButtonGroup->id(polygonsRadioButton));
				else if(tRep == TeLINES)
					representationButtonGroup->setButton(representationButtonGroup->id(linesRadioButton));
				else if(tRep == TePOINTS)
					representationButtonGroup->setButton(representationButtonGroup->id(pointsRadioButton));
				else if(tRep == TeCELLS)
					representationButtonGroup->setButton(representationButtonGroup->id(cellsRadioButton));
				ok = true;
			}
		}
	}
	if (!ok) // otherwise choose first possible one
	{
		if(repsMap[TePOLYGONS] > 1 || repsMap[TeLINES] > 1 || repsMap[TePOINTS] > 1 || repsMap[TeCELLS] > 1)
		{
			if(repsMap[TePOLYGONS] > 1)
				representationButtonGroup->setButton(representationButtonGroup->id(polygonsRadioButton));
			else if(repsMap[TeLINES] > 1)
				representationButtonGroup->setButton(representationButtonGroup->id(linesRadioButton));
			else if(repsMap[TePOINTS] > 1)
				representationButtonGroup->setButton(representationButtonGroup->id(pointsRadioButton));
			else if(repsMap[TeCELLS] > 1)
				representationButtonGroup->setButton(representationButtonGroup->id(cellsRadioButton));
		}
		else
		{
			QString msg = tr("You must have, on the view, two or more themes with the same vectorial representation!") + "\n";
			msg += tr("The themes must have only one type of representation.") + "\n";
			msg += tr("Add other themes on the view and try again.");
			QMessageBox::warning(this, tr("Warning"), msg);
			erro_ = true;
			return;
		}
	}

	erro_ = false;
	representationButtonGroup_clicked(representationButtonGroup->selectedId());
	if (erro_)
		return;
}


void GeoOpAdd::representationButtonGroup_clicked( int item)
{
	TeView* view = mainWindow_->currentView();
	TeGeomRep selType;
	int	i, j;

	inputThemesListBox->clear();
	useFieldsFromComboBox->clear();
	QButton* button = representationButtonGroup->find(item);
	if(button->text() == tr("Polygons"))
		selType = TePOLYGONS;
	else if(button->text() == tr("Lines"))
		selType = TeLINES;
	else if(button->text() == tr("Points"))
		selType = TePOINTS;
	else if(button->text() == tr("Cells"))
		selType = TeCELLS;

	vector<TeViewNode*>& themeVec = view->themes();

	for (i = 0; i < (int)themeVec.size(); ++i)
	{
		TeTheme* t = (TeTheme*)themeVec[i];
		if(t->type() != TeTHEME)
			continue;

		TeGeomRep gType = (TeGeomRep)(t->visibleGeoRep() & ~TeTEXT); // TeTEXT is not considered;

		// filter theme that use layer with only one type of geometric representation
		if(gType == selType)
		{
			// filter theme that use only static or event table
			TeAttrTableVector themeTablesVector; 
			t->getAttTables(themeTablesVector);
		
			for(j=0; j<(int)themeTablesVector.size(); j++)
			{
				TeTable table = themeTablesVector[j];
				if(!(table.tableType() == TeAttrStatic || table.tableType() == TeAttrEvent))
					break;
			}
			if(j == (int)themeTablesVector.size())
			{
				string name = t->name();
				inputThemesListBox->insertItem(name.c_str());
			}
		}
	}

	if(inputThemesListBox->count() == 0)
	{
		QString msg = tr("You must have two or more themes with") + " ";
		msg += tr(button->text()) + " " + tr("representation!") + "\n";
		msg += tr("Select other representation and try again.");
		QMessageBox::warning(this, tr("Warning"), msg);
		erro_ = true;
	}
}

void GeoOpAdd::okPushButton_clicked()
{
	TeView* view = mainWindow_->currentView();
	vector<TeViewNode*>& tthemeVec = view->themes();
	themeVec_.clear();
	unsigned int i, j;
	for(i=0; i<inputThemesListBox->count(); ++i)
	{
		if(inputThemesListBox->isSelected(i))
		{
			for (j = 0; j < tthemeVec.size(); ++j)
			{
				if(tthemeVec[j]->name() == inputThemesListBox->text(i).latin1())
				{
					themeVec_.push_back((TeTheme*)tthemeVec[j]);
					break;
				}
			}
		}
	}

	if (themeVec_.size() < 2)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select at least two themes to be added."));
		return;
	}

	if(useFieldsFromComboBox->currentItem() == 0)
	{
		QString mess = tr("Select the theme that will keep the attributes.");
		QMessageBox::warning(this, tr("Warning"), mess);
		return;
	}

	if (!theme_)
		useFieldsFromComboBox_activated(useFieldsFromComboBox->currentText());

	if(specifyOutputLayerLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this,tr("Warning"),
		tr("Enter output layer name and try again!"));
		return;
	}
	string layerName = specifyOutputLayerLineEdit->text().latin1();
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

	TeAttributeRep	atRep;
	QString s = useFieldsFromComboBox->currentText();
	QString ss = specifyOutputLayerLineEdit->text();
//	TeThemeVector themeVec;
	vector<TeTheme*> themeVec;
	for(i=0; i<themeVec_.size(); ++i)
	{
		TeTheme* t = (TeTheme*)themeVec_[i];
		QString tName = t->name().c_str();
		if(tName == s)
			continue;
		themeVec.push_back(t);
	}

	TeBox bInvalid;
	TeTheme* ttt = ((TeTheme*)theme_->getTheme());
	TeLayer* ll = ttt->layer();
	TeProjection* prj = ll->projection();
	TeLayer* newLayer = new TeLayer(layerName, db_, bInvalid, prj);

	TeSelectedObjects selOb;
	if(allRadioButton->isOn())
		selOb = TeAll;
	else if(pointedRadioButton->isOn())
		selOb = TeSelectedByPointing;
	else
		selOb = TeSelectedByQuery;

	// create a temporary log file
	TeAsciiFile* logFile=0;
	QDir adir;
	string dirname = adir.absPath().latin1();
	string dirfile;
	int f = dirname.find("\\");
	if(f >= 0)
		dirfile = dirname + "\\" + "tv_geoadd_error.txt";
	else
		dirfile = dirname + "/" + "tv_geoadd_error.txt";
	try
	{
		logFile = new TeAsciiFile(dirfile,"w");
	}
	catch(...)
	{
		logFile = 0;
	}
	TeWaitCursor wait;
	bool res = TeGeoOpAdd(newLayer, (TeTheme*)theme_->getTheme(), themeVec, selOb, logFile);
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
		
		QString msg1 = tr("Add executed!");
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
			if(((TeTheme*)themeItem->getAppTheme()->getTheme())->name() == themeName)
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
			QString msg = tr("Add not executed!");
			QMessageBox::warning(this, tr("Error"), msg);
		}
		reject();
	}
}


void GeoOpAdd::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("geoOpAdd.htm");
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


void GeoOpAdd::reject()
{
	hide();
}

string GeoOpAdd::getNewLayerName( TeLayer *layer )
{
	string name = layer->name();
	size_t f = name.find("_add");
	if(f == string::npos)
		name = name + "_add";
	
	return(db_->getNewLayerName(name));
}

void GeoOpAdd::inputThemesListBox_clicked( QListBoxItem * )
{
	int	i, cc = 0;
	useFieldsFromComboBox->clear();
	useFieldsFromComboBox->insertItem("");

	for(i=0; i<(int)inputThemesListBox->count(); ++i)
	{
		if(inputThemesListBox->isSelected(i))
		{
			useFieldsFromComboBox->insertItem(inputThemesListBox->text(i));
			++cc;
		}
	}

	if(cc > 1)
		useFieldsFromComboBox->setCurrentItem(useFieldsFromComboBox->count()-1);
	else
		useFieldsFromComboBox->setCurrentItem(0);
}


void GeoOpAdd::useFieldsFromComboBox_activated( const QString &s )
{
	TeGeomRep selType;
	unsigned int i, j;

	int item = representationButtonGroup->selectedId();
	QButton* button = representationButtonGroup->find(item);
	if(button->text() == tr("Polygons"))
		selType = TePOLYGONS;
	else if(button->text() == tr("Lines"))
		selType = TeLINES;
	else if(button->text() == tr("Points"))
		selType = TePOINTS;
	else if(button->text() == tr("Cells"))
		selType = TeCELLS;


	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	for (i = 0; i < themeItemVec.size(); ++i)
	{
		TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
		TeTheme* theme = (TeTheme*)appTheme->getTheme();
		TeGeomRep gType = (TeGeomRep)(theme->visibleGeoRep() & ~TeTEXT); // TeTEXT is not considered;

		// filter theme that use layer with only one type of geometric representation
		if(gType == selType)
		{
			// filter theme that use only static or event table
			TeAttrTableVector themeTablesVector; 
			theme->getAttTables(themeTablesVector);
		
			for(j = 0; j < themeTablesVector.size(); ++j)
			{
				TeTable table = themeTablesVector[j];
				if(!(table.tableType() == TeAttrStatic || table.tableType() == TeAttrEvent))
					break;
			}

			if(j == themeTablesVector.size())
			{
				string name = theme->name();
				string sname = s.latin1();
				if(name == sname)
				{
					theme_ = appTheme;
					string newLayerName = getNewLayerName(theme->layer());
					if(specifyOutputLayerLineEdit->text().isEmpty())
						specifyOutputLayerLineEdit->setText(newLayerName.c_str());
					break;
				}
			}
		}
	}

	//vector<TeViewNode*>& themeVec = view->themes();
	//for (i = 0; i < (int)themeVec.size(); ++i)
	//{
	//	TeTheme* t = (TeTheme*)themeVec[i];
	//	TeGeomRep gType = (TeGeomRep)(t->visibleGeoRep() & ~TeTEXT); // TeTEXT is not considered;

	//	// filter theme that use layer with only one type of geometric representation
	//	if(gType == selType)
	//	{
	//		// filter theme that use only static or event table
	//		TeAttrTableVector themeTablesVector; 
	//		t->getAttTables(themeTablesVector);
	//	
	//		for(j=0; j<(int)themeTablesVector.size(); j++)
	//		{
	//			TeTable table = themeTablesVector[j];
	//			if(!(table.tableType() == TeAttrStatic || table.tableType() == TeAttrEvent))
	//				break;
	//		}
	//		if(j == (int)themeTablesVector.size())
	//		{
	//			string name = t->name();
	//			string sname = s.latin1();
	//			if(name == sname)
	//			{
	//				theme_ = (TeAppTheme*)t;
	//				string newLayerName = getNewLayerName(t->layer());
	//				if(specifyOutputLayerLineEdit->text().isEmpty())
	//					specifyOutputLayerLineEdit->setText(newLayerName.c_str());
	//				break;
	//			}
	//		}
	//	}
	//}
}


void GeoOpAdd::inputThemesListBox_selectionChanged()
{
	inputThemesListBox_clicked(0);
}
