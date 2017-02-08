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
#include <TeApplicationUtils.h>
#include <TeQtDatabaseItem.h>
#include <TeQtDatabasesListView.h>
#include <TeQtLayerItem.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <help.h>
#include <qdialog.h>
#include <TeAsciiFile.h>
#include <TeWaitCursor.h>

void GeoOpAggregation::init()
{
	int i, j, k = -1, n;
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

	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	if(db_ == 0 || curViewItem == 0)
	{
		QString msg = tr("This operation requires an active view!");
		QMessageBox::warning(this, tr("Warning"), msg);
		erro_ = true;;
		return;
	}
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
		
			for(n=0; n<(int)themeTablesVector.size(); ++n)
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
				themeComboBox->insertItem(name.c_str());
				j++;
			}
		}
	}
	if(j == 0)
	{
		erro_ = true;
		QString msg = tr("Selected view should have at least one theme with vectorial representation!") + "\n";
		msg += tr("The themes must have only one visible representation.") + "\n";
		msg += tr("Add other themes to the view and try again.");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

	if(k >= 0)
		themeComboBox->setCurrentItem(k);
	else
		themeComboBox->setCurrentItem(0);

	themeComboBox_activated(themeComboBox->currentItem());

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
	selectAllComboBox->insertItem("VALIDCOUNT");
	unselectAllComboBox->insertItem("VALIDCOUNT");
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
}


void GeoOpAggregation::themeComboBox_activated( int /* item */)
{
	if (themeComboBox->currentText().isEmpty())
		return;

	string s = themeComboBox->currentText().latin1();
	unsigned int i;
	TeQtViewItem* viewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = viewItem->getThemeItemVec();
	for (i = 0; i < themeItemVec.size(); ++i)
	{
		TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
		TeTheme* theme = (TeTheme*)appTheme->getTheme();
		if(theme->name() == s)
		{
			theme_ = appTheme;
			break;
		}
	}
	attributeListBox->clear();
	TeAttributeList	aList = ((TeTheme*)theme_->getTheme())->sqlAttList();
	if (((TeTheme*)theme_->getTheme())->visibleRep() & 0x40000000)
		attributeListBox->insertItem("CURRENT LEGEND");

	for(i = 0; i < aList.size(); ++i)
	{
		string name = aList[i].rep_.name_;
		attributeListBox->insertItem(name.c_str());
	}

	string lName = getNewLayerName(((TeTheme*)theme_->getTheme())->layer());
	layerNameLineEdit->setText(lName.c_str());

	outListBox->clear();

	for(i = 0; i < aList.size(); ++i)
	{
		string ss;
		string name = aList[i].rep_.name_;
		TeAttrDataType type = aList[i].rep_.type_;

		if(type == TeREAL || type == TeINT)
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
			ss = name + ": VALIDCOUNT";
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
			ss = name + ": VALIDCOUNT";
			outListBox->insertItem(ss.c_str());
			outListBox->insertItem("");
		}
	}	
}

void GeoOpAggregation::okPushButton_clicked()
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
	int	j, k;
	unsigned int i;
	TeAttributeRep	atRep;
	TeGroupingAttr attrMM;
	TeAttributeList	aList = ((TeTheme*)theme_->getTheme())->sqlAttList();
	vector<string>	agregAttrVec;
	for(i = 0; i<attributeListBox->count(); ++i)
	{
		if(attributeListBox->isSelected(i))
			agregAttrVec.push_back(attributeListBox->text(i).latin1());
	}
	if(agregAttrVec.size() == 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select aggregation attribute and try again"));
		return;
	}

	QString s;
	for(i=0; i<outListBox->count(); ++i)
	{
		if(outListBox->isSelected(i))
		{
			s = outListBox->text(i);
			j = s.find(": ");
			QString a = s;
			a.remove(j, (int)a.length() - j);
			s.remove(0, j + 2);
			s.prepend("Te");

			for(k=0; k<(int)aList.size(); ++k)
			{
				string b = a.latin1();
				if(b == aList[k].rep_.name_)
				{
					atRep = aList[k].rep_;
					break;
				}
			}

			string st = s.latin1();
			TeStatisticType sType;
			sType = getStatisticFromString(st);
			attrMM.push_back(make_pair<TeAttributeRep, TeStatisticType>(atRep, sType));
		}
	}

	TeSelectedObjects selOb = TeAll;
	if(pointedRadioButton->isOn())
		selOb = TeSelectedByPointing;
	else if(queriedRadioButton->isOn())
		selOb = TeSelectedByQuery;

	TeBox bInvalid;
	TeLayer* newLayer = new TeLayer(layerName, db_, bInvalid, ((TeTheme*)theme_->getTheme())->layer()->projection());
	// create a temporary log file
	TeAsciiFile* logFile=0;
	QDir adir;
	string dirname = adir.absPath().latin1();
	string dirfile;
	int f = dirname.find("\\");
	if(f >= 0)
		dirfile = dirname + "\\" + "tv_geoaggr_error.txt";
	else
		dirfile = dirname + "/" + "tv_geoaggr_error.txt";
	try
	{
		logFile = new TeAsciiFile(dirfile,"w");
	}
	catch(...)
	{
		logFile = 0;
	}
	TeWaitCursor wait;
	bool res = TeGeoOpAggregation(newLayer, (TeTheme*)theme_->getTheme(), agregAttrVec, attrMM, selOb, logFile);
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
		QString msg1 = tr("Agqregation executed!");
		QString msg2 = tr("The layer:") + " ";
		msg2 += layerName.c_str();
		msg2 += " " + tr("was created successfully!");
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
			QString msg = tr("Aggregation was not executed!");
			QMessageBox::warning(this, tr("Error"), msg);
		}
		reject();
	}
}

void GeoOpAggregation::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("geoOpAggregation.htm");
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


void GeoOpAggregation::reject()
{
	if(help_)
		delete(help_);
	hide();
}


void GeoOpAggregation::outListBox_clicked( QListBoxItem *item )
{
	QString s = item->text();
	if(s.isEmpty())
		outListBox->setSelected(item, false);
}



string GeoOpAggregation::getNewLayerName( TeLayer* layer)
{
	string name = layer->name();
	size_t f = name.find("_agreg");
	if(f == string::npos)
		name = name + "_agreg";
	
	return(db_->getNewLayerName(name));
}


void GeoOpAggregation::selectAllComboBox_activated( int)
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


void GeoOpAggregation::unselectAllComboBox_activated( int)
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




void GeoOpAggregation::attributeListBox_selectionChanged()
{
/*	int i, j;
	string ss;
	
	outListBox->clear();
	TeAttributeList	aList = theme_->sqlAttList();

	for(i = 0; i < (int)aList.size(); ++i)
	{
		string name = aList[i].rep_.name_;
		
		for(j=0; j<(int)attributeListBox->count(); ++j)
		{
			if(attributeListBox->isSelected(j))
			{
				string aname = attributeListBox->text(j).latin1();
				if(name == aname)
					break;
			}
		}
		if(j < (int)attributeListBox->count())
			continue;

		TeAttrDataType type = aList[i].rep_.type_;

		if(type == TeREAL || type == TeINT)
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
			ss = name + ": VALIDCOUNT";
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
			ss = name + ": VALIDCOUNT";
			outListBox->insertItem(ss.c_str());
			outListBox->insertItem("");
		}
	}	*/
}
