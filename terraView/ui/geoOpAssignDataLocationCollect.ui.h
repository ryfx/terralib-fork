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
#include <qapplication.h>
#include <qmessagebox.h>

#include <terraViewBase.h>
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
#include <TeWaitCursor.h>

void GeoOpAssignDataLocationCollect::selectAllComboBox_highlighted( int )
{
	int	i;
	help_ = 0;

	QString s = selectAllComboBox->currentText();
	if(s.isEmpty())
		return;

	QString ss = ": " + s;

	for(i=0; i<(int)outListBox->count(); ++i)
	{
		s = outListBox->text(i);
		if(s.find(ss) > 0)
			outListBox->setSelected(i, true);
	}
}


void GeoOpAssignDataLocationCollect::unselectAllComboBox_highlighted( int )
{
	int	i;
	QString s = unselectAllComboBox->currentText();
	if(s.isEmpty())
		return;

	QString ss = ": " + s;

	for(i=0; i<(int)outListBox->count(); ++i)
	{
		s = outListBox->text(i);
		if(s.find(ss) > 0)
			outListBox->setSelected(i, false);
	}
}


void GeoOpAssignDataLocationCollect::outListBox_clicked( QListBoxItem * item )
{
	if(item->text().isEmpty())
		outListBox->setSelected(item, false);
}


void GeoOpAssignDataLocationCollect::okPushButton_clicked()
{
	if (!toTheme_)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select a theme to assign data to."));
		return;
	}

	if (!fromTheme_)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select a theme to collect data from."));
		return;
	}


	TeGeomRep repFrom = (TeGeomRep)(((TeTheme*)fromTheme_->getTheme())->visibleGeoRep() & ~TeTEXT);
	TeGeomRep repTo = (TeGeomRep)(((TeTheme*)toTheme_->getTheme())->visibleGeoRep() & ~TeTEXT);
	if (repFrom == TeRASTER &&
		repTo != TePOINTS)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Collection of raster data is implemented only for points."));
		return;
	}

	unsigned int i, j;
	QString s;

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

	TeAttributeRep	atRep;
	TeGroupingAttr measuresColl;
	for(i=0; i<outListBox->count(); ++i)
	{
		if(outListBox->isSelected(i))
		{
			s = outListBox->text(i);
			j = s.find(": ");
			string cname = s.left(j).latin1();
			atRep = attrReps_[cname];
			QString a = s;
			atRep.name_ = cname;
			a.remove(j, (int)a.length() - j);
			s.remove(0, j + 2);
			s.prepend("Te");
			TeStatisticType sType;
			sType = getStatisticFromString(s.latin1());
			measuresColl.push_back(make_pair<TeAttributeRep, TeStatisticType>(atRep, sType));
		}
	}

	TeTheme* fromTheme = (TeTheme*)fromTheme_->getTheme();
	TeTheme* toTheme = (TeTheme*)toTheme_->getTheme();

	if (measuresColl.empty())
	{
		TeGeomRep rep = (TeGeomRep)(fromTheme->visibleGeoRep() & ~TeTEXT);
		if (rep == TeRASTER)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Select at least one band to collect the measures."));
			return;
		}
		TeAttrTableVector attrv;
		fromTheme->getAttTables(attrv,TeAttrStatic);
		string defCol;
		if (!attrv.empty())
			defCol = attrv[0].linkName();
		else
		{
			fromTheme->getAttTables(attrv,TeAttrEvent);
			if (!attrv.empty())
				defCol = attrv[0].uniqueName();
		}
		if (!defCol.empty())
		{
			atRep.name_ = attrv[0].name() + "." + defCol;
			atRep.type_ = TeSTRING;
			measuresColl.push_back(make_pair<TeAttributeRep, TeStatisticType>(atRep, TeCOUNT));
		}
	}

	int spatialRelation = TeUNDEFINEDREL;

	if(insideRadioButton->isOn())
		spatialRelation = TeWITHIN;
	else if(insideOrCoveredByRadioButton->isOn())
		spatialRelation = (TeWITHIN | TeCOVEREDBY);
	else if(overlapsRadioButton->isOn())
		spatialRelation = TeOVERLAPS | TeWITHIN | TeCOVEREDBY | TeCROSSES;
	else if(crossesRadioButton->isOn())
		spatialRelation = TeCROSSES;
	
	TeWaitCursor wait;
	bool res = TeGeoOpAssignByLocationCollect(toTheme, fromTheme,tableName, measuresColl, spatialRelation);
	wait.resetWaitCursor();
 	if(res)
	{	
		QString msg1 = tr("Data Collect operation successfully executed!");
		QString msg2 = tr("The table:") + " ";
		msg2 += tableName.c_str();
		msg2 += " " + tr("was created!") + "\n";
		msg2 +=	tr("Do you want to display this table?");
		int response = QMessageBox::question(this, msg1, msg2, tr("Yes"), tr("No"));

		if (response != 0)
			return;

		string themeName = toTheme->name();
		TeQtViewsListView* viewsListView = mainWindow_->getViewsListView();
		TeQtViewItem* viewItem = viewsListView->currentViewItem();
		TeQtThemeItem* themeItem = (TeQtThemeItem*)viewItem->firstChild();
		TeQtThemeItem* newActiveThemeItem;
		while(themeItem)
		{
			if(((TeTheme*)themeItem->getAppTheme()->getTheme())->name() == themeName)
				newActiveThemeItem = themeItem;

			themeItem->setState(QCheckListItem::Off);
			((TeTheme*)themeItem->getAppTheme()->getTheme())->visibility(false);
			themeItem = (TeQtThemeItem*)themeItem->nextSibling();
		}
		if(newActiveThemeItem)
		{
			TeTable table(tableName);
			db_->loadTableInfo(table);
			toTheme->addThemeTable(table);
			db_->updateThemeTable(toTheme);
			mainWindow_->getGrid()->clear();
			if (!toTheme->createCollectionAuxTable() || !toTheme->populateCollectionAux())
			{
				QMessageBox::critical(this, tr("Error"),
				tr("Fail to mount the auxiliary table of the collection!"));
				reject();
			}
			mainWindow_->getGrid()->init(toTheme_);
			newActiveThemeItem->setState(QCheckListItem::On);
			((TeTheme*)newActiveThemeItem->getAppTheme()->getTheme())->visibility(true);
			viewsListView->selectItem((TeQtViewItem*)newActiveThemeItem);
		}

		mainWindow_->resetAction_activated();
		accept();
	}
	else
	{
		toTheme->layer()->removeAttributeTable(tableName);

		db_->deleteTable(tableName);
		
		QString msg = tr("Data Collect operation failed!");
		QMessageBox::warning(this, tr("Error"), msg);
		
		reject();
	}
}


void GeoOpAssignDataLocationCollect::init()
{
	insideRadioButton->setChecked(true);
	fromTheme_ = 0;
	toTheme_ = 0;
	help_ = 0;
	int  k = -1;
	unsigned int j,i;
	erro_ = false;
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	db_ = mainWindow_->currentDatabase();
	TeTheme* curTheme = 0;
	TeAppTheme* appTheme = mainWindow_->currentTheme();
	if (appTheme)
		curTheme = (TeTheme*)appTheme->getTheme();

	if(curTheme && curTheme->type() != TeTHEME)
		curTheme = 0;

	// filter the candidate themes: the ones that use only static or event table and some representations
	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	for (i = j = 0; i < themeItemVec.size(); ++i)
	{
		TeTheme* theme = (TeTheme*)(themeItemVec[i]->getAppTheme())->getTheme();
		if(theme->type() != TeTHEME)
			continue;

		TeGeomRep gType = (TeGeomRep)(theme->visibleGeoRep() & ~TeTEXT);
		if (gType == TeRASTER)
		{
			srcThemes_.push_back(theme->name());
			continue;
		}
		TeAttrTableVector themeTablesVector; 
		theme->getAttTables(themeTablesVector);
		for(j=0; j<themeTablesVector.size(); ++j)
		{
			TeTable table = themeTablesVector[j];
			if(table.tableType() == TeAttrStatic || table.tableType() == TeAttrEvent)
			{
				if(gType == TePOLYGONS || gType == TeCELLS || gType == TePOINTS)
				{
					restrThemes_.push_back(theme->name());
					srcThemes_.push_back(theme->name());
					toThemeComboBox->insertItem(theme->name().c_str());
					if(curTheme != 0 && (curTheme->id() == theme->id()))
						k = j;
					++j;
				}	
				else if (gType == TeLINES)
					srcThemes_.push_back(theme->name());
				break;
			}
		}
	}

	if (restrThemes_.empty())
	{
		erro_ = true;
		QString msg = tr("Selected view should contain at least one theme with polygons or cell representation") + "\n";
		msg += tr("to be used as a spatial restricition.") + "\n";
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

	if (srcThemes_.empty() || 
		(srcThemes_.size() == 1 && restrThemes_.size() == 1 && srcThemes_[0] == restrThemes_[0])  )
	{
		erro_ = true;
		QString msg = tr("Selected view should contain at least a theme to provide the data to be collected.");
		msg += "\n" + tr("This theme should be different of the theme with the spatial restriction.");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}
	
	if(k >= 0)
		toThemeComboBox->setCurrentItem(k);
	else
		toThemeComboBox->setCurrentItem(0);

	selectAllComboBox->insertItem("");
	unselectAllComboBox->insertItem("");
	selectAllComboBox->insertItem("MINVALUE");
	unselectAllComboBox->insertItem("MINVALUE");
	selectAllComboBox->insertItem("MAXVALUE");
	unselectAllComboBox->insertItem("MAXVALUE");
	selectAllComboBox->insertItem("MEAN");
	unselectAllComboBox->insertItem("MEAN");
	selectAllComboBox->insertItem("SUM");
	unselectAllComboBox->insertItem("SUM");
	selectAllComboBox->insertItem("MEDIAN");
	unselectAllComboBox->insertItem("MEDIAN");
	selectAllComboBox->insertItem("COUNT");
	unselectAllComboBox->insertItem("COUNT");
	selectAllComboBox->insertItem("MODE");
	unselectAllComboBox->insertItem("MODE");
	selectAllComboBox->insertItem("STANDARDDEVIATION");
	unselectAllComboBox->insertItem("STANDARDDEVIATION");
	selectAllComboBox->insertItem("VARIANCE");
	unselectAllComboBox->insertItem("VARIANCE");
	selectAllComboBox->insertItem("VARCOEFF");
	unselectAllComboBox->insertItem("VARCOEFF");
	selectAllComboBox->insertItem("SKEWNESS");
	unselectAllComboBox->insertItem("SKEWNESS");
	selectAllComboBox->insertItem("KURTOSIS");
	unselectAllComboBox->insertItem("KURTOSIS");
	selectAllComboBox->insertItem("AMPLITUDE");
	unselectAllComboBox->insertItem("AMPLITUDE");
	selectAllComboBox->insertItem("KERNEL");
	unselectAllComboBox->insertItem("KERNEL");

	toThemeComboBox_activated(toThemeComboBox->currentItem());
}


string GeoOpAssignDataLocationCollect::getNewTableName( string name )
{
	int	i = 0;
	string tableName = name + "_Collect";
	string sname = tableName;

	while(db_->tableExist(sname))
		sname = tableName + Te2String(++i);
	return sname;
}



void GeoOpAssignDataLocationCollect::selectAllComboBox_activated( int )
{
	int	i;
	QString s = selectAllComboBox->currentText();
	if(s.isEmpty())
		return;

	QString ss = ": " + s;

	for(i=0; i<(int)outListBox->count(); ++i)
	{
		s = outListBox->text(i);
		if(s.find(ss) > 0)
			outListBox->setSelected(i, true);
	}
}


void GeoOpAssignDataLocationCollect::unselectAllComboBox_activated( int )
{
	int	i;
	QString s = unselectAllComboBox->currentText();
	if(s.isEmpty())
		return;

	QString ss = ": " + s;

	for(i=0; i<(int)outListBox->count(); ++i)
	{
		s = outListBox->text(i);
		if(s.find(ss) > 0)
			outListBox->setSelected(i, false);
	}
}


void GeoOpAssignDataLocationCollect::fromThemeComboBox_activated( int )
{
	string name = fromThemeComboBox->currentText().latin1();

	TeQtViewItem* viewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = viewItem->getThemeItemVec();

	unsigned int i;
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
	TeTheme* fromTheme = (TeTheme*)fromTheme_->getTheme();
	string tableName = getNewTableName(fromTheme->layer()->name());
	tableNameLineEdit->setText(tableName.c_str());

	outListBox->clear();
	if (!fromTheme_)
		return;

	string ss;
	TeGeomRep rep = (TeGeomRep)(fromTheme->visibleGeoRep() & ~TeTEXT);
	if (rep == TeRASTER)
	{
		spatialOperationButtonGroup->setEnabled(false);
		selectAllComboBox->setEnabled(false);
		unselectAllComboBox->setEnabled(false);
		for (int nb=0; nb <fromTheme->layer()->raster()->params().nBands(); ++nb)
		{
			ss = "Band" + Te2String(nb) + ": VALUE";
			outListBox->insertItem(ss.c_str());
		}
		outListBox->insertItem("");
		return;
	}
	else
	{
		spatialOperationButtonGroup->setEnabled(true);
		selectAllComboBox->setEnabled(true);
		unselectAllComboBox->setEnabled(true);
	}

	if (rep == TePOINTS)
	{
		insideOrCoveredByRadioButton->setEnabled(false);
		overlapsRadioButton->setEnabled(false);
	}
	else
	{
		insideOrCoveredByRadioButton->setEnabled(true);
		overlapsRadioButton->setEnabled(true);
	}

	if (rep == TeLINES)
		crossesRadioButton->setEnabled(true);
	else
		crossesRadioButton->setEnabled(false);

	TeAttrTableVector themeTables;
	fromTheme->getAttTables(themeTables);
	unsigned int ntables = themeTables.size();
	if (ntables == 0)
		return;

	attrReps_.clear();
	for (i=0; i<ntables; ++i)
	{
		TeAttributeList& attList = themeTables[i].attributeList();
		for (unsigned int j = 0; j <attList.size(); ++j)
		{
			string name = themeTables[i].name() + "." + attList[j].rep_.name_;
			attrReps_[name] = attList[j].rep_;
			if (attList[j].rep_.type_ == TeREAL || attList[j].rep_.type_ == TeINT)
			{
				ss = name + ": MINVALUE";
				outListBox->insertItem(ss.c_str());
				ss = name + ": MAXVALUE";
				outListBox->insertItem(ss.c_str());
				ss = name + ": MEAN";
				outListBox->insertItem(ss.c_str());
				ss = name + ": SUM";
				outListBox->insertItem(ss.c_str());
				ss = name + ": MEDIAN";
				outListBox->insertItem(ss.c_str());
				ss = name + ": COUNT";
				outListBox->insertItem(ss.c_str());
				ss = name + ": MODE";
				outListBox->insertItem(ss.c_str());
				ss = name + ": STANDARDDEVIATION";
				outListBox->insertItem(ss.c_str());
				ss = name + ": VARIANCE";
				outListBox->insertItem(ss.c_str());
				ss = name + ": VARCOEFF";
				outListBox->insertItem(ss.c_str());
				ss = name + ": SKEWNESS";
				outListBox->insertItem(ss.c_str());
				ss = name + ": KURTOSIS";
				outListBox->insertItem(ss.c_str());
				ss = name + ": AMPLITUDE";
				outListBox->insertItem(ss.c_str());
				ss = name + ": KERNEL";
				outListBox->insertItem(ss.c_str());
				outListBox->insertItem("");
			}
			else
			{
				ss = name + ": MINVALUE";
				outListBox->insertItem(ss.c_str());
				ss = name + ": MAXVALUE";
				outListBox->insertItem(ss.c_str());
				ss = name + ": COUNT";
				outListBox->insertItem(ss.c_str());
				outListBox->insertItem("");
			}
		}
	}
}


void GeoOpAssignDataLocationCollect::toThemeComboBox_activated( int )
{
	string toThemeName = toThemeComboBox->currentText().latin1();

	TeQtViewItem* viewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = viewItem->getThemeItemVec();
	unsigned int i;
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
	fromThemeComboBox->clear();
	for (i=0; i<srcThemes_.size(); ++i)
	{
		if (srcThemes_[i] == toThemeName)
			continue;
		fromThemeComboBox->insertItem(srcThemes_[i].c_str());	
	}
	if (fromThemeComboBox->count() > 0)
	{
		fromThemeComboBox->setCurrentItem(0);
		fromThemeComboBox_activated(0);
	}
}


void GeoOpAssignDataLocationCollect::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("geoOpAssignDataLocation.htm");
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
