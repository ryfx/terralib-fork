/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright  2001-2007 INPE and Tecgraf/PUC-Rio.
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
#include <TeVectorRemap.h>
#include <TeWaitCursor.h>
#include <TeAsciiFile.h>
#include <qfiledialog.h>


void GeoOpAssignDataLocationDistribute::init()
{
	help_ = 0;
	erro_ = false;
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	db_ = mainWindow_->currentDatabase();
	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	if(db_ == 0 || curViewItem == 0)
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

	fromTheme_ = 0;
	toTheme_ = 0;
	vector<TeGeomRep> reps;
	vector<string> toThemeCandidates;
	unsigned int i, j, k = -1, n, nt;

	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	for (i = j = 0; i < themeItemVec.size(); ++i)
	{
		TeTheme* theme = (TeTheme*)(themeItemVec[i]->getAppTheme())->getTheme();
		if(theme->type() != TeTHEME)
			continue;

		TeGeomRep rep = (TeGeomRep)(theme->visibleGeoRep() & ~TeTEXT); // TeTEXT not is considered

		// step 1: filter themes that has only one representation
		if(rep == TePOLYGONS || rep == TeLINES || rep == TePOINTS || rep == TeCELLS)
		{
			// step 2: filter themes that use only static or event table
			TeAttrTableVector themeTablesVector; 
			theme->getAttTables(themeTablesVector);
			nt = themeTablesVector.size();
		
			for (n=0; n<nt; ++n)
			{
				if(!(themeTablesVector[n].tableType() == TeAttrStatic || themeTablesVector[n].tableType() == TeAttrEvent))
					break;
			}

			if (n == nt)
			{
				toThemeCandidates.push_back(theme->name());
				reps.push_back(rep);
				++j;
			}
		}
	}
	// step 3: check the compatibility between to and from themes
	j = 0;
	k = 0;
	for (i=0; i<reps.size();++i)
	{
		for (nt=0; nt<reps.size();++nt)
		{
			if (nt == i)
				continue;
			if (reps[i] == TeLINES)
			{
				if (reps[nt] == TeLINES)
				{
					toThemeComboBox->insertItem(toThemeCandidates[i].c_str());
					if (curTheme != 0 && (toThemeCandidates[i] == curTheme->name()))
						k = j;
					++j;
					break;
				}
			}
			else
			{
				if (reps[nt] == TePOLYGONS || reps[nt] == TeCELLS)
				{
					toThemeComboBox->insertItem(toThemeCandidates[i].c_str());
					if (curTheme != 0 && (toThemeCandidates[i] == curTheme->name()))
						k = j;
					++j;
					break;
				}
			}
		}
	}

	reps.clear();
	toThemeCandidates.clear();

	if (j < 1)
	{
		erro_ = true;
		QString msg = tr("View has to have at least two themes with only one visible vectorial representation.") + "\n";
		msg += tr("Themes that distribute data have to have polygons or cells.") + "\n";
		msg += tr("Themes that receive the data distributes have to have polygons, cells, lines or points.");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

	toThemeComboBox->setCurrentItem(k);
	toThemeComboBox_activated(k);
}


void GeoOpAssignDataLocationDistribute::toThemeComboBox_activated(int /*item*/)
{
	unsigned int i;
	string toThemeName = toThemeComboBox->currentText().latin1();

	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	for (i = 0; i < themeItemVec.size(); ++i)
	{
		TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
		TeTheme* theme = (TeTheme*)appTheme->getTheme();
		if(theme->type() != TeTHEME)
			continue;

		if(theme->name() == toThemeName)
		{
			toTheme_ = appTheme;
			break;
		}
	}
	TeGeomRep rep1 = (TeGeomRep)(((TeTheme*)toTheme_->getTheme())->visibleGeoRep() & ~TeTEXT); // TeTEXT not is considered
	map<string, int> mapaux;
	TeGeomRep rep2;
	for (i = 0; i < themeItemVec.size(); ++i)
	{
		TeTheme* theme = (TeTheme*)(themeItemVec[i]->getAppTheme())->getTheme();
		if(theme->type() != TeTHEME)
			continue;

		if (theme->name() == toThemeName)
			continue;

		rep2 = (TeGeomRep)(theme->visibleGeoRep() & ~TeTEXT); // TeTEXT not is considered
		if (rep2 == TePOLYGONS || rep2 == TeCELLS || (rep1 == TeLINES && rep2 == TeLINES))
			mapaux[theme->name()] = i;
	}
	fromThemeComboBox->clear();
	map<string, int>::const_iterator it = mapaux.begin();
	while (it != mapaux.end())
	{
		fromThemeComboBox->insertItem(it->first.c_str());
		++it;
	}
	mapaux.clear();
	if(fromThemeComboBox->count() > 0)
	{
		fromThemeComboBox->setCurrentItem(0);
		fromThemeComboBox_activated(0);
	}
}


void GeoOpAssignDataLocationDistribute::fromThemeComboBox_activated( int )
{
	unsigned int i, j;
	string name = fromThemeComboBox->currentText().latin1();

	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	for (i = 0; i < themeItemVec.size(); ++i)
	{
		TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
		TeTheme* theme = (TeTheme*)appTheme->getTheme();
		if(theme->type() != TeTHEME)
			continue;
		if(theme->name() == name)
		{
			fromTheme_ = appTheme;
			break;
		}
	}
	string tableName = getNewTableName(((TeTheme*)fromTheme_->getTheme())->layer()->name());
	tableNameLineEdit->setText(tableName.c_str());

	attribute_llistBox->clear();
	
	TeAttrTableVector attrTables = ((TeTheme*)fromTheme_->getTheme())->attrTables();
	for(i=0; i<attrTables.size(); ++i)
	{
		if (attrTables[i].tableType() == TeAttrStatic || attrTables[i].tableType() == TeAttrEvent)
		{
			TeAttributeList& attrList = attrTables[i].attributeList();
			for (j=0; j<attrList.size(); ++j)
			{
				string tableCol = attrTables[i].name()+"."+attrList[j].rep_.name_;
				attribute_llistBox->insertItem(tableCol.c_str());
			}
		}
	}
	attribute_llistBox->selectAll(true);

	TeGeomRep rep1 = (TeGeomRep)(((TeTheme*)toTheme_->getTheme())->visibleGeoRep() & ~TeTEXT); 
	TeGeomRep rep2 = (TeGeomRep)(((TeTheme*)fromTheme_->getTheme())->visibleGeoRep() & ~TeTEXT); 
	if (rep1 == TeLINES || rep1 == TePOINTS)
	{
		insideOrCoveredByRadioButton->setEnabled(false);
		if (rep2 == TeLINES)
		{
			interceptsRadioButton->setEnabled(true);
			insideRadioButton->setEnabled(false);
			spatialOperationButtonGroup->setButton(spatialOperationButtonGroup->id(interceptsRadioButton));
		}
		else
		{
			insideRadioButton->setEnabled(true);
			interceptsRadioButton->setEnabled(false);
			spatialOperationButtonGroup->setButton(spatialOperationButtonGroup->id(insideRadioButton));
		}
	}
	else
	{
		interceptsRadioButton->setEnabled(false);
		insideRadioButton->setEnabled(true);
		insideOrCoveredByRadioButton->setEnabled(true);
		spatialOperationButtonGroup->setButton(spatialOperationButtonGroup->id(insideOrCoveredByRadioButton));
	}
}

void GeoOpAssignDataLocationDistribute::okPushButton_clicked()
{
	if (toTheme_ == 0)
	{
		QString msg = tr("Select a theme to assign data to!");
		QMessageBox::warning(this, tr("Error"), msg);
		return;
	}

	if (fromTheme_ == 0)
	{
		QString msg = tr("Select a theme to distribute data!");
		QMessageBox::warning(this, tr("Error"), msg);
		return;
	}

	TeBox box1 = toTheme_->boundingBox();
	TeBox bx = fromTheme_->boundingBox();
	TeBox box2 = TeRemapBox(bx, ((TeTheme*)fromTheme_->getTheme())->layer()->projection(), ((TeTheme*)toTheme_->getTheme())->layer()->projection());
	TeBox bboxIntersection;
	if (!TeIntersection(box1,box2,bboxIntersection))
	{
		QString msg = tr("The two themes do not intersect.");
		QMessageBox::warning(this, tr("Error"), msg);
		return;
	}

	string tableName = tableNameLineEdit->text().latin1();
	string errorMessage;
	bool changed;
	string newName = TeCheckName(tableName, changed, errorMessage); 
	if(changed)
	{
		QString mess = tr("The table name is invalid: ") + errorMessage.c_str();
		mess += "\n" + tr("Change current name and try again.");
		QMessageBox::warning(this, tr("Warning"), mess);
		tableNameLineEdit->setText(newName.c_str());
		return;
	}
	tableName = newName;
	if(db_->tableExist(tableName) == true)
	{
		QMessageBox::warning(this, tr("Warning"), tr("The table already exists! Type other table name and try again"));
		return;
	}

	int spatialRelation = TeUNDEFINEDREL;

	if(insideRadioButton->isOn())
		spatialRelation = TeWITHIN;
	else if(insideOrCoveredByRadioButton->isOn())
		spatialRelation = (TeWITHIN | TeCOVEREDBY);
	else if(interceptsRadioButton->isOn())
		spatialRelation = TeINTERSECTS;
	
	vector<string> attsToDistribute;
	for (unsigned int ni=0; ni<attribute_llistBox->count(); ++ni)
	{
		if (attribute_llistBox->item(ni)->isSelected())
			attsToDistribute.push_back(attribute_llistBox->item(ni)->text().latin1());
	}

	if (attsToDistribute.empty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select at least one attribute to be distributed!"));
		return;
	}

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
			std::string fileName = qfileName.latin1();
			std::string::size_type k = fileName.find(".txt", 0, 4);
			if (k == string::npos)
				fileName += ".txt";

			logFile = new TeAsciiFile(fileName,"w");
		}
		catch(...)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Could not open the log file."));
			return;
		}
	}

	TeWaitCursor wait;
	bool res = TeGeoOpAssignDataLocationDistribute((TeTheme*)toTheme_->getTheme(), (TeTheme*)fromTheme_->getTheme(), tableName, spatialRelation, attsToDistribute, logFile);

	delete logFile;
	logFile = 0;

	wait.resetWaitCursor();
 	if(res)
	{	
		QString msg1 = tr("Spatial Join executed!");
		QString msg2 = tr("The table:") + " ";
		msg2 += tableName.c_str();
		msg2 += " " + tr("was created with success!") + "\n";
		msg2 +=	tr("Do you want to display the data?");
		int response = QMessageBox::question(this, msg1, msg2, tr("Yes"), tr("No"));

		if (response != 0)
			return;

		string themeName = ((TeTheme*)toTheme_->getTheme())->name();
		TeQtViewsListView* viewsListView = mainWindow_->getViewsListView();
		TeQtViewItem* viewItem = viewsListView->currentViewItem();
		TeQtThemeItem* themeItem = (TeQtThemeItem*)viewItem->firstChild();
		TeQtThemeItem* newActiveThemeItem;
		while(themeItem)
		{
			if (((TeTheme*)themeItem->getAppTheme()->getTheme())->name() == themeName)
				newActiveThemeItem = themeItem;

			viewsListView->setOn(themeItem, false);
			((TeTheme*)themeItem->getAppTheme()->getTheme())->visibility(false);
			themeItem = (TeQtThemeItem*)themeItem->nextSibling();
		}

		if(newActiveThemeItem)
		{
			TeTable table(tableName);
			db_->loadTableInfo(table);
			((TeTheme*)toTheme_->getTheme())->addThemeTable(table);
			db_->updateThemeTable((TeTheme*)toTheme_->getTheme());
			mainWindow_->getGrid()->clear();
			if (!((TeTheme*)toTheme_->getTheme())->createCollectionAuxTable() || !((TeTheme*)toTheme_->getTheme())->populateCollectionAux())
			{
				QMessageBox::critical(this, tr("Error"),
				tr("Fail to mount the auxiliary table of the collection!"));
				reject();
			}
			mainWindow_->getGrid()->init(toTheme_);
			viewsListView->setOn(newActiveThemeItem, true);
			((TeTheme*)newActiveThemeItem->getAppTheme()->getTheme())->visibility(true);
			viewsListView->selectItem((TeQtViewItem*)newActiveThemeItem);
		}

		mainWindow_->resetAction_activated();
		accept();
	}
	else
	{
		((TeTheme*)toTheme_->getTheme())->layer()->removeAttributeTable(tableName);

		db_->deleteTable(tableName);
		{
			QString msg = tr("Spatial join not executed!");
			QMessageBox::warning(this, tr("Error"), msg);
		}
		reject();
	}
}



void GeoOpAssignDataLocationDistribute::reject()
{
	hide();
}


void GeoOpAssignDataLocationDistribute::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("GeoOpAssignDataLocation.htm");
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


string GeoOpAssignDataLocationDistribute::getNewTableName( string name )
{
	int	i = 0;
	string tableName = name + "_Join";
	string sname = tableName;

	while(db_->tableExist(sname))
		sname = tableName + Te2String(++i);

	return sname;
}


void GeoOpAssignDataLocationDistribute::selAll_pushButton_clicked()
{
	if (attribute_llistBox->numRows() == 0)
		return;

	attribute_llistBox->selectAll(true);
}


void GeoOpAssignDataLocationDistribute::unselAll_pushButton_clicked()
{
	if (attribute_llistBox->numRows() == 0)
		return;
	attribute_llistBox->selectAll(false);
}
