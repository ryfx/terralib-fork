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
#include <TeQtViewsListView.h>
#include <TeQtDatabaseItem.h>
#include <TeQtDatabasesListView.h>
#include <TeQtLayerItem.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <display.h>
#include <terraViewBase.h>
#include <qstring.h>
#include <qlineedit.h>
#include <qdialog.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <help.h>

#include <TeApplicationUtils.h>
#include <TeDatabase.h>
#include <TeAsciiFile.h>
#include <TeGeoProcessingFunctions.h>
#include <TeUtils.h>
#include <TeGUIUtils.h>


void CreateBuffersWindow::init()
{
	help_ = 0;
	erro_ = false;
	polyPixmap_ = 0;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	displayWindow_ = mainWindow_->getDisplayWindow();
	db_ = mainWindow_->currentDatabase();

	TeTheme* curTheme = 0;
	TeAppTheme* appTheme = mainWindow_->currentTheme();
	if (appTheme)
		curTheme = (TeTheme*)appTheme->getTheme();

	if(curTheme && curTheme->type() != TeTHEME)
		curTheme = 0;

	unsigned int i, index=0, nthemes=0, aux1;
	int aux2=-1;
	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	for (i=0; i<themeItemVec.size(); ++i)
	{
		TeTheme* theme = (TeTheme*)(themeItemVec[i]->getAppTheme())->getTheme();
		if(theme->type() != TeTHEME)
			continue;

		TeGeomRep gType = (TeGeomRep)(theme->visibleGeoRep() & ~TeTEXT);
		if (gType == TePOLYGONS || gType == TeCELLS || gType == TePOINTS || gType == TeLINES)
		{
			themeComboBox->insertItem(theme->name().c_str());
			if (curTheme && curTheme->name() == theme->name())
			{	
				aux2 = i;
				index = nthemes;
			}
			aux1 = i;
			++nthemes;
		}
	}
	if (nthemes==0)
	{
    erro_ = true;
		QString msg = tr("Selected view should have at least one theme with vectorial representation!") + "\n";
		msg += tr("The themes must have only one visible representation.") + "\n";
		msg += tr("Add other themes to the view and try again.");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}
	if (aux2 > -1)
	{
		themeComboBox->setCurrentItem(index);
		theme_ = themeItemVec[aux2]->getAppTheme();
	}
	else
	{
		themeComboBox->setCurrentItem(nthemes);
		theme_ = themeItemVec[aux1]->getAppTheme();
	}

	useObjectsSelectedByButtonGroup_clicked(useObjectsSelectedByButtonGroup->id(allRadioButton));
	distanceButtonGroup_clicked(distanceButtonGroup->id(fixAtRadioButton));

	QString ss;
	for(i=2; i <= 10; ++i)
	{
		ss = ss.setNum(i);
		numberOfRingsComboBox->insertItem(ss);
	}
	numberOfRingsComboBox->setCurrentItem(0);
	generateMultipleRingsCheckBox_toggled(generateMultipleRingsCheckBox->isChecked());
	int extId = ruleToPolygonCaseButtonGroup->id(onlyOutsideRadioButton);
	ruleToPolygonCaseButtonGroup_clicked(extId);
}

void CreateBuffersWindow::accept() // OK Button
{
	if(fixAtRadioButton->isOn() && fixAtLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this,tr("Warning"),
		tr("Enter with distance and try again!"));
		return;
	}
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

	bool copyColumns = copyColumnsCheckBox->isChecked();

	double distance = -1.;
    string fieldName = "";
	string tableName = "";
	if(fixAtRadioButton->isOn())
		distance = fixAtLineEdit->text().toDouble();
	else
	{
		string fieldNameForDistance = specifiedFromAttributeFieldComboBox->currentText().latin1();
		fieldName = TeGetExtension(fieldNameForDistance.c_str());
		tableName = TeGetName(fieldNameForDistance.c_str());
	}

	int numberOfRings = 1;
	if(generateMultipleRingsCheckBox->isChecked())
		numberOfRings = numberOfRingsComboBox->currentText().toInt();

	// Buffer type
	TeBUFFERREGION::TeBufferType bufferType = TeBUFFERREGION::TeOUTSIDEBUFFER;
	if(insideAndOutsideRadioButton->isChecked())
		bufferType = TeBUFFERREGION::TeINSIDEOUTSIDEBUFFER;
	else if (onlyInsideRadioButton->isChecked())
		bufferType = TeBUFFERREGION::TeINSIDEBUFFER;

	// Unin and dissolve
	bool doUnion = true;
	if(DissolveNoRadioButton->isChecked())
		doUnion = false;
	
	// Create a temporary log file
	TeAsciiFile* logFile=0;
	QDir adir;
	string dirname = adir.absPath().latin1();
	string dirfile;
	int f = dirname.find("\\");
	if(f >= 0)
		dirfile = dirname + "\\" + "tv_buffer_error.txt";
	else
		dirfile = dirname + "/" + "tv_buffer_error.txt";
	try
	{
		logFile = new TeAsciiFile(dirfile,"w");
	}
	catch(...)
	{
		logFile = 0;
	}
	
	//Create a new layer to store the buffer
	TeLayer* newLayer = new TeLayer(layerName, db_, ((TeTheme*)theme_->getTheme())->layer()->projection());

	//number of points to generate the buffer
	int numPoints = 10;
	if(((TeTheme*)theme_->getTheme())->visibleGeoRep()==TePOINTS)
		numPoints = 32;

	//Execute buffer opertation
	TeWaitCursor wait;
	bool res = TeGeoOpBuffer(newLayer, (TeTheme*)theme_->getTheme(), (TeSelectedObjects)inputSel_, bufferType, distance, 
						numberOfRings, numPoints, doUnion, tableName, fieldName, logFile, copyColumns);
	wait.resetWaitCursor();
	if(res)
	{
		//insert the created layer  
		TeQtDatabasesListView *databasesListView = mainWindow_->getDatabasesListView();
		TeQtDatabaseItem *databaseItem = databasesListView->currentDatabaseItem();

		TeQtLayerItem *layerItem = new TeQtLayerItem(databaseItem, newLayer->name().c_str(), newLayer);
		layerItem->setEnabled(true);
		if (databasesListView->isOpen(layerItem->parent()) == false)
			databasesListView->setOpen(layerItem->parent(),true);
		mainWindow_->checkWidgetEnabling();

		//Log file
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
		QString msg1 = tr("Buffer operation executed!");
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
			//delete newLayer;
			QDialog::accept();
			return;
		}
		
		//create a theme to display the buffer
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
		//delete newLayer;
		QDialog::accept();
	}
	else
	{
		db_->deleteLayer(newLayer->id());

		if (logFile)
		{
			fflush(logFile->FilePtr());
			fclose(logFile->FilePtr());
		}

		QString msg = tr("Buffer operation was not executed!\n");
		msg += tr("Check the created log file: \n"); 
		msg +=  dirfile.c_str();
			
		QMessageBox::warning (this, tr("Error"), msg);
		reject();
	}
}


void CreateBuffersWindow::reject() // cancel button
{
	QDialog::reject();
}


void CreateBuffersWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("createBuffersWindow.htm");
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

void CreateBuffersWindow::themeComboBox_activated(const QString& themeName)
{
	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	
	for (unsigned int i=0; i<themeItemVec.size(); ++i)
	{
		TeTheme* theme = (TeTheme*)(themeItemVec[i]->getAppTheme())->getTheme();
		if(theme->type() != TeTHEME)
			continue;

		if (theme->name() == themeName.latin1())
		{
			theme_ = themeItemVec[i]->getAppTheme();
			break;
		}
	}

	TeGeomRep rep = (TeGeomRep)(theme_->getTheme()->visibleGeoRep()& ~TeTEXT);
	if(rep == TePOLYGONS)
	{
		ruleToPolygonCaseButtonGroup->show();
		QPaintEvent e(rect());
		paintEvent(&e);
	}
	else
		ruleToPolygonCaseButtonGroup->hide();

	if(fixAtRadioButton->isOn())
		distanceButtonGroup_clicked(distanceButtonGroup->id(fixAtRadioButton));
	else if(specifiedFromAttributeFieldRadioButton->isOn())
		distanceButtonGroup_clicked(distanceButtonGroup->id(specifiedFromAttributeFieldRadioButton));

	useObjectsSelectedByButtonGroup_clicked(useObjectsSelectedByButtonGroup->selectedId());
}


void CreateBuffersWindow::useObjectsSelectedByButtonGroup_clicked( int s)
{
	string where;
	inputSel_ = TeAll;

	if(s == useObjectsSelectedByButtonGroup->id(pointedRadioButton))
	{
		inputSel_ = TeSelectedByPointing;
		where = " WHERE c_object_status = 1 OR c_object_status = 3";
	}
	else if(s == useObjectsSelectedByButtonGroup->id(queriedRadioButton))
	{
		inputSel_ = TeSelectedByQuery;
		where = " WHERE c_object_status = 2 OR c_object_status = 3";
	}

	string sel = "SELECT COUNT(*) FROM " + ((TeTheme*)theme_->getTheme())->collectionTable();
	if(where.empty() == false)
		sel += where;

	int n = 0;
	TeDatabasePortal* portal = db_->getPortal();
	if(portal->query(sel) && portal->fetchRow())
		n = atoi(portal->getData(0));

	delete portal;
	QString num;
	num = num.setNum(n);
	numberOfObjectsLineEdit->setText(num);
}


void CreateBuffersWindow::distanceButtonGroup_clicked( int s)
{
	if(s == distanceButtonGroup->id(fixAtRadioButton))
	{
		fixAtLineEdit->setEnabled(true);
		specifiedFromAttributeFieldComboBox->setEnabled(false);
	}
	else if(s == distanceButtonGroup->id(specifiedFromAttributeFieldRadioButton))
	{
		fixAtLineEdit->setEnabled(false);
		specifiedFromAttributeFieldComboBox->setEnabled(true);
		vector<TeAttrDataType> dataTypes;
		dataTypes.push_back(TeREAL);
		dataTypes.push_back(TeINT);

		fillColumnCombo(dataTypes, (TeTheme*)theme_->getTheme(), specifiedFromAttributeFieldComboBox);

		if(specifiedFromAttributeFieldComboBox->count() == 0)
		{
			QMessageBox::warning(this,tr("Warning"),
			tr("The selected theme don´t have numeric attribute!"));
			fixAtRadioButton->setDown(true);
			distanceButtonGroup_clicked(distanceButtonGroup->id(fixAtRadioButton));
			return;
		}
		else
			specifiedFromAttributeFieldComboBox->setCurrentItem(0);
	}
}


void CreateBuffersWindow::ruleToPolygonCaseButtonGroup_clicked( int s)
{
	if(polyPixmap_)
		delete polyPixmap_;

	if(s == ruleToPolygonCaseButtonGroup->id(insideAndOutsideRadioButton))
		polyPixmap_ = new QPixmap(QPixmap::fromMimeSource("insideAndOutside.png"));
	else if(s == ruleToPolygonCaseButtonGroup->id(onlyOutsideRadioButton))
		polyPixmap_ = new QPixmap(QPixmap::fromMimeSource("onlyOutside.png"));
	else if(s == ruleToPolygonCaseButtonGroup->id(onlyInsideRadioButton))
		polyPixmap_ = new QPixmap(QPixmap::fromMimeSource("onlyInside.png"));

	ruleToPolygonCaseFrame->setBackgroundPixmap(*polyPixmap_);
	ruleToPolygonCaseFrame->erase();
}


void CreateBuffersWindow::generateMultipleRingsCheckBox_toggled( bool b)
{
	if(b)
	{
		numberOfRingsComboBox->setCurrentItem(0);
		numberOfRingsComboBox->setEnabled(true);
	}
	else
	{
		numberOfRingsComboBox->setCurrentItem(0);
		numberOfRingsComboBox->setEnabled(false);
	}
}



void CreateBuffersWindow::unionPushButton_clicked()
{
    DissolveYesRadioButton->setChecked(true);
    DissolveNoRadioButton->setChecked(false);

}


void CreateBuffersWindow::noUnionPushButton_clicked()
{
    DissolveNoRadioButton->setChecked(true);
    DissolveYesRadioButton->setChecked(false);
}
