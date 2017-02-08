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
#include <TeColorUtils.h>
#include <TeQtProgress.h>
#include <TeQtViewsListView.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <TeQtLegendItem.h>
#include <TeQtChartItem.h>
#include <TeDatabaseUtils.h>
#include <TePlotTheme.h>
#include <display.h>
#include <qpopupmenu.h>
#include <changeMinMax.h>
#include <saveAndLoadColorBarWindow.h>
#include <visual.h>

#define MAXSLICES 200


void LegendWindow::init()
{
	// Get variables from the main window
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	db_ = mainWindow_->currentDatabase();
	canvas_ = mainWindow_->getCanvas();

	theme_ = mainWindow_->currentTheme();
	TeTheme* curTheme = (TeTheme*)theme_->getTheme();
	themeItem_ = (TeQtThemeItem*)mainWindow_->getViewsListView()->currentThemeItem();

	if(!(curTheme->type() == TeTHEME || curTheme->type() == TeEXTERNALTHEME))
		return;

	help_ = 0;
	invertColor_ = false;

	TeWaitCursor wait;
	int i;

	//********************* Initialization of Variables *************** 
	//Mounting the brushStyleMap
	brushStyleMap_[TePolyTypeTransparent] = Qt::NoBrush;
	brushStyleMap_[TePolyTypeFill] = Qt::SolidPattern;
	brushStyleMap_[TePolyTypeHorizontal] = Qt::HorPattern;
	brushStyleMap_[TePolyTypeVertical] = Qt::VerPattern;
	brushStyleMap_[TePolyTypeFDiagonal] = Qt::FDiagPattern;
	brushStyleMap_[TePolyTypeBDiagonal] = Qt::BDiagPattern;
	brushStyleMap_[TePolyTypeCross] = Qt::CrossPattern;
	brushStyleMap_[TePolyTypeDiagonalCross] = Qt::DiagCrossPattern;

	//Mounting the penStyleMap
	penStyleMap_[TeLnTypeContinuous] = Qt::SolidLine;
	penStyleMap_[TeLnTypeDashed] = Qt::DashLine;
	penStyleMap_[TeLnTypeDotted] = Qt::DotLine;
	penStyleMap_[TeLnTypeDashDot] = Qt::DashDotLine;
	penStyleMap_[TeLnTypeDashDotDot] = Qt::DashDotDotLine;
	penStyleMap_[TeLnTypeNone] = Qt::NoPen;
	

	TeGrouping& groupingStruct = curTheme->grouping();
	//*********** Grouping Modes ******************************
	// Set the grouping modes
	groupingModeComboBox->clear();
	groupingModeComboBox->insertItem(tr("Equal Steps"));
	groupingModeComboBox->insertItem(tr("Quantil"));
	groupingModeComboBox->insertItem(tr("Standard Deviation"));
	groupingModeComboBox->insertItem(tr("Unique Value"));

	// Set the current grouping mode
	if (groupingStruct.groupMode_ == TeNoGrouping)
		groupingMode_ = TeEqualSteps;
	else
		groupingMode_ = groupingStruct.groupMode_;

	groupingModeComboBox->setCurrentItem((int)(groupingMode_));

	//*********** Grouping Attributes ******************************
	//	Insert the grouping attributes
	setGroupingAttributes();

	// Set the current grouping attribute
	if (groupingStruct.groupMode_ != TeNoGrouping)
	{
		groupingAttr_ = groupingStruct.groupAttribute_.name_;
		for(i = 0; i < attributesComboBox->count(); ++i)
		{
			string s = attributesComboBox->text(i).latin1();
			if(s == groupingAttr_)
			{
				attributesComboBox->setCurrentItem(i);
				break;
			}
		}
	}
	else
	{
		attributesComboBox->setCurrentItem(0);
		groupingAttr_ = attributesComboBox->currentText().latin1();
	}

	// Get the table type of the grouping attribute
	TeAttrTableType attrTableType = getAttrTableType();

	// Set the the grouping attribute representation
	TeAttributeList attrList = curTheme->sqlAttList();
	for (i = 0; i < (int)attrList.size(); ++i)
	{
		string attrName = attrList[i].rep_.name_;
		size_t pos = attrName.find(".");
		string fullAttrName;
		
		if (pos != string::npos)
			fullAttrName = attrName;
		else
			fullAttrName = curTheme->getTableName(attrName) + "." + attrName;

		if (fullAttrName == groupingAttr_)
		{
			attrRep_ = attrList[i].rep_;
			break;
		} 
	}

	//*********** Chronon *******************************************
	// Fill the chronon combobox and the interval chrono combobox
	// Check whether there is a temporary table in the theme
	chrononComboBox->clear();
	TeAttrTableVector tablesVec;
	if (curTheme->getAttTables(tablesVec, TeAttrEvent) ||
		curTheme->getAttTables(tablesVec, TeFixedGeomDynAttr))
	{
		chrononComboBox->insertItem(tr("Day"));
		chrononComboBox->insertItem(tr("Month"));
		chrononComboBox->insertItem(tr("Year"));
		chrononComboBox->insertItem(tr("Month of Year"));

		// Enable the chronon combobox if the grouping attribute belongs
		// to a table whose type is TeFixedGeomDynAttr or TeAttrEvent
		if ((attrTableType == TeAttrEvent) || (attrTableType == TeFixedGeomDynAttr))
		{
			TeChronon chr = curTheme->grouping().groupChronon_;
			int chrIndex;
			if (chr == TeDAY)
				chrIndex = 0;
			else if (chr == TeMONTH)
				chrIndex = 1;
			else if (chr == TeYEAR)
				chrIndex = 2;
			else if (chr == TeMONTHOFYEAR)
				chrIndex = 3;
			else
				chrIndex = -1;

			if (chrIndex > 0)
				chrononComboBox->setCurrentItem(chrIndex);
			chronon_ = chr;
		}
		else
		{
			chrononComboBox->insertItem("");
			chrononComboBox->setCurrentItem(chrononComboBox->count() - 1);
			chrononComboBox->setEnabled(false);
			chronon_ = TeNOCHRONON;
		}

		for(i = 0; i < 60; ++i)
			string s = Te2String(i+1);
	}
	else
	{
		chronon_ = TeNOCHRONON;
		chrononComboBox->setEnabled(false);
	}

	//*********** Slices *******************************************
	// Fill the slices combobox according to the grouping mode
	slicesComboBox->clear();
	for(i = 0; i < MAXSLICES; ++i)
	{
		string s = Te2String(i+1);
		slicesComboBox->insertItem(s.c_str());
	}

	if (groupingMode_ == TeEqualSteps || groupingMode_ == TeQuantil)
	{
		// Set the current slice
		if (groupingStruct.groupMode_ != TeNoGrouping)
		{
			int slices = groupingStruct.groupNumSlices_;
			slicesComboBox->setCurrentText(Te2String(slices).c_str());
		}
		else
			slicesComboBox->setCurrentItem(4);  // number of slices = 5
	}
	else
	{
		slicesComboBox->insertItem("");
		slicesComboBox->setCurrentItem(slicesComboBox->count() - 1);
		slicesComboBox->setEnabled(false);
	}

	//************************** Standard Deviation ********************
	// Fill the standard deviation combobox
	stdDevComboBox->clear();
	stdDevComboBox->insertItem("1");
	stdDevComboBox->insertItem("0.5");
	stdDevComboBox->insertItem("0.25");

	if (groupingMode_ == TeStdDeviation)
	{
		// Set the current standard deviation
		if (groupingStruct.groupMode_ != TeNoGrouping)
		{
			double stdDev = curTheme->grouping().groupStdDev_;
			for (i = 0; i < stdDevComboBox->count(); ++i)
			{
				if (stdDevComboBox->text(i).toDouble() == stdDev)
				{
					stdDevComboBox->setCurrentItem(i);
					break;
				}
			}
		}
	}
	else
	{
		stdDevComboBox->insertItem("");
		stdDevComboBox->setCurrentItem(stdDevComboBox->count() - 1);
		stdDevComboBox->setEnabled(false);
	}

	//******************************* Precision *********************************
	// Fill the precision combobox
	precisionComboBox->clear();
	for(i = 0; i < 15; ++i)
	{
		string s = Te2String(i+1);
		precisionComboBox->insertItem(s.c_str());
	}

	if (attrRep_.type_ == TeINT || attrRep_.type_ == TeREAL)
	{
		//Set the group precision
		if (groupingStruct.groupMode_ == TeNoGrouping)
			precisionComboBox->setCurrentItem(5); // precision = 6
		else
			precisionComboBox->setCurrentItem(curTheme->grouping().groupPrecision_ - 1);
	}
	else
	{
		precisionComboBox->insertItem("");
		precisionComboBox->setCurrentItem(precisionComboBox->count() - 1);
		precisionComboBox->setEnabled(false);
	}

	//******************************* Function *********************************
	// Fill the function combobox
	functionComboBox->insertItem("MIN");
	functionComboBox->insertItem("MAX");
	functionComboBox->insertItem("AVG");
	functionComboBox->insertItem("COUNT");
	functionComboBox->insertItem("SUM");

	// Set the function combobox according the type of the
	// table which the grouping attribute belongs
	if ((attrTableType == TeAttrExternal || attrTableType == TeFixedGeomDynAttr) &&
		(attrRep_.type_ == TeINT || attrRep_.type_ == TeREAL))
	{
		if (groupingStruct.groupMode_ == TeNoGrouping)
			functionComboBox->setCurrentItem(0);
		else
		{
			string func = curTheme->grouping().groupFunction_;
			for(i = 0; i < functionComboBox->count(); ++i)
			{
				string s = functionComboBox->text(i).latin1();
				if (s == func)
				{
					functionComboBox->setCurrentItem(i);
					break;
				}
			}
			if(i == functionComboBox->count())
				functionComboBox->setCurrentItem(0);
		}
	}
	else if (attrTableType == TeAttrStatic || attrTableType == TeAttrEvent)
	{
		functionComboBox->insertItem("");
		functionComboBox->setCurrentItem(functionComboBox->count() - 1);
		functionComboBox->setEnabled(false);
	}
	
	//************************ Colors of the Grouping ***************************
	if (groupingStruct.groupMode_ != TeNoGrouping)
		QString groupingColors = theme_->groupColor().c_str();

	//************************ Legend Grid ****************************
	// Copy the theme contents to the legend source theme
	legendSource_.copyAppThemeContents(*theme_);
	legendTable->setDataSource(&legendSource_);

	// Set the number of rows and colums of the table
	if (groupingStruct.groupMode_ != TeNoGrouping)
	{
		legendTable->setNumRows(legendSource_.numRows());
		legendTable->setNumCols(legendSource_.numCols());
	}
	else
	{
		legendTable->setNumRows(10);
		legendTable->setNumCols(5);
	}

	legendTable->setLeftMargin(0);
	legendTable->verticalHeader()->hide();

	// Set and adjust the columns of the table
	setAndAdjustTableColumns();

	okPushButton->setEnabled(false);

	string groupColor = theme_->groupColor();

	frameTeQtColorBar->setVerticalBar(false);
	frameBTeQtColorBar->setVerticalBar(false);

	if(groupingMode_ == TeStdDeviation)
	{
		frameTeQtColorBar->setColorBar(getColorBarVector(groupColor, true));
		frameTeQtColorBar->drawColorBar();

		frameBTeQtColorBar->setColorBar(getColorBarVector(groupColor, false));
		frameBTeQtColorBar->drawColorBar();
	}
	else
	{
		frameTeQtColorBar->setColorBar(getColorBarVector(groupColor, true));
		frameTeQtColorBar->drawColorBar();
		frameBTeQtColorBar->hide();
	}

	importPushButton->setEnabled(false);
	
	loadNamesComboBox->clear();
	loadNamesComboBox->insertItem("");
	TeDatabasePortal* portal = db_->getPortal();
	if(portal->query("SELECT name FROM tv_colorbar ORDER BY name"))
	{
		while(portal->fetchRow())
			loadNamesComboBox->insertItem(portal->getData(0));
	}
	delete portal;
	loadNamesComboBox->setCurrentItem(0);

	connect(&legendSource_, SIGNAL(dataChanged()), this, SLOT(legendTableChanged()));

	popup_ = new QPopupMenu(this);
	popup_->insertItem(QString(tr("Change Min/Max Legend Limits...")), this, SLOT(popupChangeMinAndMaxSlot()));
	connect(legendTable, SIGNAL(contextMenuRequested(int, int, const QPoint&)),
			this, SLOT(legendTable_contextMenuRequested(int, int, const QPoint&)));
	connect(legendTable, SIGNAL(doubleClicked(int, int, int, const QPoint&)),
			this, SLOT(legendTable_doubleClicked(int, int, int, const QPoint&)));

	connect(frameTeQtColorBar, SIGNAL(colorChangedSignal()), this, SLOT(colorChangedSlot()));
	connect(frameBTeQtColorBar, SIGNAL(colorChangedSignal()), this, SLOT(colorChangedSlot()));
}


void LegendWindow::groupingModeComboBox_activated(int groupingMode)
{
	int i;

	TeGroupingMode newGroupingMode = (TeGroupingMode)groupingMode;

	if (newGroupingMode == groupingMode_)
		return;

	TeTheme* curTheme = (TeTheme*)theme_->getTheme();
	if (newGroupingMode != TeUniqueValue)
	{
		if (groupingMode_ == TeUniqueValue)
		{
			setGroupingAttributes();

			if (attrRep_.type_ == TeINT || attrRep_.type_ == TeREAL)
			{
				// Set the new position of the attribute in the combobox
				for(i = 0; i < attributesComboBox->count(); ++i)
				{
					string s = attributesComboBox->text(i).latin1();
					if(s == groupingAttr_)
					{
						attributesComboBox->setCurrentItem(i);
						break;
					}
				}
			}
			else
			{
				// Set the current attribute as the first one 
				attributesComboBox->setCurrentItem(0);
				groupingAttr_ = attributesComboBox->currentText().latin1();
				
				// Update the attribute representation
				TeAttributeList attrList = curTheme->sqlAttList();
				attrRep_ = attrList[0].rep_;
			}
		}
	}
	else 
	{
		setGroupingAttributes();
		// Set the new position of the attribute in the combobox
		for(i = 0; i < attributesComboBox->count(); ++i)
		{
			string s = attributesComboBox->text(i).latin1();
			if(s == groupingAttr_)
			{
				attributesComboBox->setCurrentItem(i);
				break;
			}
		}
		if(importPushButton->isEnabled() == false && attributesComboBox->currentText().isEmpty() == false)
			groupingAttr_ = attributesComboBox->currentText().latin1();

		// Set the enabling or not of the function combobox
		TeAttrTableType attrTableType = getAttrTableType();
		if (attrTableType == TeAttrStatic || attrTableType == TeAttrEvent)
		{
			if (functionComboBox->isEnabled() == true)
			{
				functionComboBox->insertItem("");
				functionComboBox->setCurrentItem(functionComboBox->count() - 1);
				functionComboBox->setEnabled(false);
			}
		}
		else if (attrTableType == TeAttrExternal || attrTableType == TeFixedGeomDynAttr)
		{
			if (functionComboBox->isEnabled() == true)
			{
				if (attrRep_.type_ != TeINT && attrRep_.type_ != TeREAL)
				{
					functionComboBox->insertItem("");
					functionComboBox->setCurrentItem(functionComboBox->count() - 1);
					functionComboBox->setEnabled(false);
				}
			}
			else 
			{
				if (attrRep_.type_ == TeINT || attrRep_.type_ == TeREAL)
				{
					functionComboBox->removeItem(functionComboBox->count() - 1);
					functionComboBox->setCurrentItem(0);
					functionComboBox->setEnabled(true);
				}
			}
		}
	}

	groupingMode_ = newGroupingMode;
	checkWidgetEnabling();
	applyPushButton->setEnabled(true);
	okPushButton->setEnabled(false);


	if(groupingMode_ == TeStdDeviation)
	{	
		frameBTeQtColorBar->show();
		frameBTeQtColorBar->setColorBar(frameBTeQtColorBar->getInputColorVec());
		frameBTeQtColorBar->drawColorBar();
	}
	else if(frameBTeQtColorBar->isShown())
	{
		frameBTeQtColorBar->hide();
	}
}


void LegendWindow::attributesComboBox_activated( const QString &attr)
{
	if (groupingAttr_ == attr.latin1())
		return;

	groupingAttr_ = attr.latin1();

	// Set the attribute rep of the new grouping attribute
	TeTheme* curTheme = (TeTheme*)theme_->getTheme();
	TeAttributeList attrList = curTheme->sqlAttList();
	for (unsigned int i = 0; i < attrList.size(); ++i)
	{
		string attrName = attrList[i].rep_.name_;
		size_t pos = attrName.find(".");
		string fullAttrName;
		
		if (pos != string::npos)
			fullAttrName = attrName;
		else
			fullAttrName = curTheme->getTableName(attrName) + "." + attrName;

		if (fullAttrName == groupingAttr_)
		{
			attrRep_ = attrList[i].rep_;
			break;
		} 
	}

	TeAttrTableType attrTableType = getAttrTableType();

	// Set the enabling or not of the function combobox
	if (attrTableType == TeAttrStatic || attrTableType == TeAttrEvent)
	{
		if (functionComboBox->isEnabled() == true)
		{
			functionComboBox->insertItem("");
			functionComboBox->setCurrentItem(functionComboBox->count() - 1);
			functionComboBox->setEnabled(false);
		}
	}
	else if (attrTableType == TeAttrExternal || attrTableType == TeFixedGeomDynAttr)
	{
		if (functionComboBox->isEnabled() == true)
		{
			if (attrRep_.type_ != TeINT && attrRep_.type_ != TeREAL)
			{
				functionComboBox->insertItem("");
				functionComboBox->setCurrentItem(functionComboBox->count() - 1);
				functionComboBox->setEnabled(false);
			}
		}
		else 
		{
			if (attrRep_.type_ == TeINT || attrRep_.type_ == TeREAL)
			{
				functionComboBox->removeItem(functionComboBox->count() - 1);
				functionComboBox->setCurrentItem(0);
				functionComboBox->setEnabled(true);
			}
		}
	}

	// Set the enabling or not of the chronon combobox
	if (attrTableType == TeFixedGeomDynAttr || attrTableType == TeAttrEvent)
	{
		if (chrononComboBox->isEnabled() == false)
		{
			chrononComboBox->removeItem(chrononComboBox->count() - 1);
			chrononComboBox->setCurrentItem(0);
			chrononComboBox->setEnabled(true);
			chronon_ = TeDAY;
		}
	}
	else
	{
		if (chrononComboBox->isEnabled() == true)
		{
			chrononComboBox->insertItem("");
			chrononComboBox->setCurrentItem(chrononComboBox->count() - 1);
			chrononComboBox->setEnabled(false);
			chronon_ = TeNOCHRONON;
		}
	}

	checkWidgetEnabling();
	applyPushButton->setEnabled(true);
	okPushButton->setEnabled(false);
}


void LegendWindow::chrononComboBox_activated( int i)
{
	switch (i)
	{
		case 0:
			chronon_ = TeDAY;
			break;
		case 1:
			chronon_ = TeMONTH;
			break;
		case 2:
			chronon_ = TeYEAR;
			break;
		case 3:
			chronon_ = TeMONTHOFYEAR;
			break;
		default:
			chronon_ = TeNOCHRONON;
			break;
	}
	applyPushButton->setEnabled(true);
	okPushButton->setEnabled(false);
}


void LegendWindow::slicesComboBox_activated( const QString & )
{
	applyPushButton->setEnabled(true);
	okPushButton->setEnabled(false);
}

void LegendWindow::stdDevComboBox_activated( const QString & )
{
	applyPushButton->setEnabled(true);
	okPushButton->setEnabled(false);
}

void LegendWindow::precisionComboBox_activated( const QString & )
{
	applyPushButton->setEnabled(true);
	okPushButton->setEnabled(false);
}

void LegendWindow::functionComboBox_activated( int )
{
	applyPushButton->setEnabled(true);
	okPushButton->setEnabled(false);
}

void LegendWindow::clearColorsPushButton_clicked()
{
	frameTeQtColorBar->clearColorBar();
	if(frameBTeQtColorBar->isShown())
		frameBTeQtColorBar->clearColorBar();
}

void LegendWindow::invertColorsPushButton_clicked()
{
	frameTeQtColorBar->invertColorBar();
	if(frameBTeQtColorBar->isShown())
		frameBTeQtColorBar->invertColorBar();

	invertColor_ = !invertColor_;
}


void LegendWindow::applyPushButton_clicked()
{
	TeWaitCursor wait;

	// Set the grouping parameters and store them in a grouping structure 
	TeGrouping groupingParams;
	groupingParams.groupMode_ = TeGroupingMode(groupingModeComboBox->currentItem());
	groupingParams.groupAttribute_ = attrRep_;
	groupingParams.groupAttribute_.name_ = groupingAttr_;
	groupingParams.groupNumSlices_ = slicesComboBox->currentText().toInt();
	groupingParams.groupStdDev_ = stdDevComboBox->currentText().toFloat();
	groupingParams.groupPrecision_ = precisionComboBox->currentText().toInt();
	groupingParams.groupChronon_ = chronon_;
	if (functionComboBox->count() != 0)
		groupingParams.groupFunction_ = functionComboBox->currentText().latin1();
	groupingParams.groupNullAttr_ = true;  //Show missing data

	// Generate the legends
	legendSource_.generateLegends(groupingParams);
	legendTable->setNumRows(legendSource_.numRows());
	legendTable->setNumCols(legendSource_.numCols());

	// Put color on legends
	vector<ColorBar> colorVec = frameTeQtColorBar->getInputColorVec();
	vector<ColorBar> colorBVec = frameBTeQtColorBar->getInputColorVec();
	legendSource_.putColorOnLegend(colorVec, colorBVec);

	// Set and adjust the columns of the table
	setAndAdjustTableColumns();
	if(groupingParams.groupMode_ != TeUniqueValue && attributesComboBox->currentText().isEmpty())
	{
		wait.resetWaitCursor();
		QMessageBox::warning(this, tr("Warning"),
			tr("Fail to apply legend! The theme don�t have numerical attribute."));
		return;
	}
	okPushButton->setEnabled(true);
}

void LegendWindow::getColorNameVector(string& scor, vector<string>& colorNameVec)
{
	if(scor.empty())
		scor = tr("R").latin1();

	QString s = scor.c_str();
	QStringList ss = QStringList::split("-",s,true);

	for(unsigned int i = 0; i < ss.size(); i++)
	{
		QString a = ss[i];
		if(tr("R") == a)
			colorNameVec.push_back("RED");
		else if(tr("G") == a)
			colorNameVec.push_back("GREEN");
		else if(tr("B") == a)
			colorNameVec.push_back("BLUE");
		else if(tr("Cy") == a)
			colorNameVec.push_back("CYAN");
		else if(tr("Or") == a)
			colorNameVec.push_back("ORANGE");
		else if(tr("Mg") == a)
			colorNameVec.push_back("MAGENTA");
		else if(tr("Y") == a)
			colorNameVec.push_back("YELLOW");
		else
			colorNameVec.push_back("GRAY");
	}
}


void LegendWindow::okPushButton_clicked()
{
	TeWaitCursor wait;
	string objId, valStr;
	double val;
	unsigned int i;

	map<string, TeGeomRepVisualMap*> objVisualMap;

	legendTable->clearSelection(true);

	// Delete the legends in the database and memory
	TeTheme* curTheme = (TeTheme*)theme_->getTheme();
	db_->deleteLegend(curTheme->id()); 
	curTheme->legend().clear();

	// Copy the legend vector, the grouping parameters, and the grouping colors 
	// of the legend source
	vector<TeLegendEntry>& legVec = curTheme->legend();
	legVec = ((TeTheme*)legendSource_.getAppTheme().getTheme())->legend();
	for (i = 0; i < legVec.size(); ++i)
	{
		legVec[i].id(0);
		legVec[i].theme(curTheme->id());
	}

	theme_->groupColor(legendSource_.getAppTheme().groupColor());
	curTheme->grouping() = ((TeTheme*)legendSource_.getAppTheme().getTheme())->grouping();
	theme_->concatTableName(curTheme->grouping().groupAttribute_.name_);
	
	if (chronon_ != TeNOCHRONON)
	{
		vector<map<string, string> >& mapObjValVec = legendSource_.mapObjValVec();
		map<string, string>::iterator it;
		unsigned int k;
		double from, to;
		for (int frame = 0; frame < (int)mapObjValVec.size(); ++frame)
		{
			map<string, string>& objValMap = mapObjValVec[frame];
			for (it = objValMap.begin(); it != objValMap.end(); ++it)
			{
				objId = it->first;
				val = atof(it->second.c_str());
				for (k = 0; k < legVec.size(); ++k)
				{
					TeLegendEntry& leg = legVec[k];
					from = atof(leg.slice().from_.c_str());
					to = atof(leg.slice().to_.c_str());
					if(val >= from && val < to)
					{
						TeGeomRepVisualMap& vm = leg.getVisualMap();
						objVisualMap[objId] = &vm;
						break;
					}
				}
			}
			createFrame(objVisualMap, frame);
		}	
	}

	if (updateDatabase() == false)
	{
		wait.resetWaitCursor();
		QMessageBox::warning(this, tr("Warning"),
			tr("Fail to insert legends into the database!"));
		return;
	}

	mainWindow_->groupChanged();
	accept();
}


bool LegendWindow::updateDatabase()
{
	TeTheme* curTheme = (TeTheme*)theme_->getTheme();

	// Associate each object to its group in the collection table
	if(!curTheme->saveGrouping())
		return false;

	// Save legend in the collection table
	if(curTheme->grouping().groupMode_ != TeRasterSlicing)
	{
		if(curTheme->saveLegendInCollection() == false)
			return false;
	}

	// Update the te_theme_application table
	string update = "UPDATE te_theme_application SET ";
	update += "grouping_color='" + theme_->groupColor() + "'";
	update += " WHERE theme_id = " + Te2String(curTheme->id());
	if(db_->execute(update) == false)
		return false;

	// Update the theme item interface
	mainWindow_->updateThemeItemInterface(themeItem_);

	return true;
}


void LegendWindow::createFrame(map<string, TeGeomRepVisualMap*>& objVisualMap, int n)
{
	map<int, RepMap>& layerRepMap = mainWindow_->layerId2RepMap_;
	DisplayWindow* display = mainWindow_->getDisplayWindow();

	int i;
	TeBox box;
	TeLayer* layer = ((TeTheme*)theme_->getTheme())->layer();

	if (canvas_->getWorld().isValid() == false)
		mainWindow_->resetAction_activated();

	if(n == 0)
		plotBackground();

	int w = canvas_->viewport()->width();
	int h = canvas_->viewport()->height();
	QPixmap pix(w, h);

	QPainter* painter = canvas_->getPainter();
	QPaintDevice* dev = painter->device();
	if(dev)
		painter->end();
	painter->begin(&pix);
	painter->eraseRect(pix.rect());
	bitBlt (&pix, 0, 0, &backPixmap_, 0, 0, w, h, Qt::CopyROP, true);

	TeView* curview = mainWindow_->currentView();
	canvas_->setProjection(curview->projection());
	canvas_->setDataProjection (layer->projection());
	box = canvas_->getDataWorld();

	// If there is intersection between layer and canvas, plot the theme
	if (TeIntersects(box, layer->box()) == true)
	{
		TePlotFrame(theme_, canvas_, layerRepMap, objVisualMap);
		plotPieBars(theme_, canvas_, (TeQtProgress*)0);
		TePlotTextWV(theme_, canvas_, (TeQtProgress*)0);
	}
	display->plotLegendOnCanvas (theme_);

	QString num = Te2String(n).c_str();
	for(i = (int)num.length(); i < 5; i++)
		num.prepend("0");

	QDir animaQDir = QDir :: currentDirPath();
	QString path = animaQDir.currentDirPath() + "/animation";
	animaQDir.setPath(path);
	if(n == 0)
	{
		if(animaQDir.exists() == false)
			animaQDir.mkdir(path);
		else
		{
			for(i=0; i<(int)animaQDir.count(); i++)
			{
				QString file = animaQDir[i];
				if(file.isEmpty() || file == "." || file == "..")
					continue;
				animaQDir.remove(file, false);
			}
		}
	}

	QString name = path + "/image_" + num + ".jpeg";
	pix.save(name, "JPEG", 100);

	canvas_->plotOnPixmap0();
}


void LegendWindow::plotBackground()
{
	TeBox box;
	TeLayer* layer;
    TeQtProgress *progress = mainWindow_->getProgress();

	TeView*	view = mainWindow_->currentView();
	if(view == 0)
		return;

	map<int, RepMap>& layerRepMap = mainWindow_->layerId2RepMap_;
	
	canvas_->setProjection(view->projection());
	int w = canvas_->width();
	int h = canvas_->height();
	backPixmap_.resize(w, h);
	backPixmap_.fill();

	QPainter* painter = canvas_->getPainter();
	QPaintDevice* dev = painter->device();
	if(dev)
		painter->end();
	painter->begin(&backPixmap_);

	QPixmap* pix = canvas_->getPixmap1();
	bitBlt (&backPixmap_, 0, 0, pix, 0, 0, w, h, Qt::CopyROP, true);

	TeQtViewItem* viewItem = mainWindow_->getViewsListView()->getViewItem(view);
	vector<TeQtThemeItem*> themeItemVec = viewItem->getThemeItemVec();
	for (int i = (int)(themeItemVec.size()-1); i >= 0; --i)
	{
		TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
		TeTheme* theme = (TeTheme*)appTheme->getTheme();
		if(theme_ == appTheme)
			continue;

		if (theme->visibility() && theme->visibleRep())
		{
			if((theme->visibleRep() & TeRASTER) == 0)
			{
				layer = theme->layer();
				canvas_->setDataProjection (layer->projection());
				box = canvas_->getDataWorld();

				// If there is intersection between layer and canvas, plot the theme
				if (TeIntersects(box, layer->box()) == true)
				{
					TePlotObjects(appTheme, canvas_, layerRepMap, progress);
					plotPieBars(appTheme, canvas_, progress);
					TePlotTextWV(appTheme, canvas_, progress);
				}
			}
		}
	}

}

void LegendWindow::setGroupingAttributes()
{
	unsigned int i;

	attributesComboBox->clear();

	TeGroupingMode groupingMode = (TeGroupingMode)(groupingModeComboBox->currentItem());

	TeTheme* curTheme = (TeTheme*)theme_->getTheme();
	TeAttributeList	attrVector =  curTheme->sqlAttList();
	TeAttributeList numAttrVector = curTheme->sqlNumAttList();

	if (groupingMode != TeUniqueValue)
	{
		for (i = 0; i < numAttrVector.size(); ++i)
		{
			string attrName = numAttrVector[i].rep_.name_;
			if (attrName.find(".") != string::npos)
				attributesComboBox->insertItem(attrName.c_str());
			else
			{
				string s = curTheme->getTableName(attrName) + "." + attrName;
				attributesComboBox->insertItem(s.c_str());
			}
		}
	}
	else
	{
		for (i = 0; i < attrVector.size(); ++i)
		{
			string attrName = attrVector[i].rep_.name_;
			if (attrName.find(".") != string::npos)
				attributesComboBox->insertItem(attrName.c_str());
			else
			{
				string s = curTheme->getTableName(attrName) + "." + attrName;
				attributesComboBox->insertItem(s.c_str());
			}
		}
	}

	if(attributesComboBox->count()==0)
		attributesComboBox->insertItem("");
}


void LegendWindow::checkWidgetEnabling()
{
	if (groupingMode_ == TeEqualSteps || groupingMode_ == TeQuantil)
	{
		if(slicesComboBox->isEnabled() == false)
		{
			slicesComboBox->removeItem(slicesComboBox->count() - 1);
			slicesComboBox->setCurrentItem(4);  // number of slices = 5
			slicesComboBox->setEnabled(true);
		}

		if(stdDevComboBox->isEnabled() == true)
		{
			stdDevComboBox->insertItem("");
			stdDevComboBox->setCurrentItem(stdDevComboBox->count() - 1);
			stdDevComboBox->setEnabled(false);
		}

		if(precisionComboBox->isEnabled() == false)
		{
			precisionComboBox->removeItem(precisionComboBox->count() - 1);
			precisionComboBox->setCurrentItem(5);  // precision = 6
			precisionComboBox->setEnabled(true);
		}
	}
	else if (groupingMode_ == TeStdDeviation)
	{
		if(slicesComboBox->isEnabled() == true)
		{
			slicesComboBox->insertItem("");
			slicesComboBox->setCurrentItem(slicesComboBox->count() - 1);
			slicesComboBox->setEnabled(false);
		}

		if(stdDevComboBox->isEnabled() == false)
		{
			stdDevComboBox->removeItem(stdDevComboBox->count() - 1);
			stdDevComboBox->setCurrentItem(0);  // std deviation = 1
			stdDevComboBox->setEnabled(true);
		}

		if(precisionComboBox->isEnabled() == false)
		{
			precisionComboBox->removeItem(precisionComboBox->count() - 1);
			precisionComboBox->setCurrentItem(5);  // precision = 6
			precisionComboBox->setEnabled(true);
		}
	}
	else if (groupingMode_ == TeUniqueValue)
	{
		if(slicesComboBox->isEnabled() == true)
		{
			slicesComboBox->insertItem("");
			slicesComboBox->setCurrentItem(slicesComboBox->count() - 1);
			slicesComboBox->setEnabled(false);
		}

		if(stdDevComboBox->isEnabled() == true)
		{
			stdDevComboBox->insertItem("");
			stdDevComboBox->setCurrentItem(stdDevComboBox->count() - 1);
			stdDevComboBox->setEnabled(false);
		}

		if (attrRep_.type_ == TeINT || attrRep_.type_ == TeREAL)
		{
			if(precisionComboBox->isEnabled() == false)
			{
				precisionComboBox->removeItem(precisionComboBox->count() - 1);
				precisionComboBox->setCurrentItem(5);  // precision = 6
				precisionComboBox->setEnabled(true);
			}
		}
		else
		{
			if(precisionComboBox->isEnabled() == true)
			{
				precisionComboBox->insertItem("");
				precisionComboBox->setCurrentItem(precisionComboBox->count() - 1);
				precisionComboBox->setEnabled(false);
			}
		}
	}
}


void LegendWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("legendWindow.htm");
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


void LegendWindow::importCheckBox_toggled(bool status)
{
	unsigned int i, j;
	TeAbstractTheme* curTheme = theme_->getTheme();

	if(status)
	{
		importFromViewComboBox->clear();
		importFromThemeComboBox->clear();
		
		vector<TeQtViewItem*> viewItemVec = mainWindow_->getViewsListView()->getViewItemVec();
		for (i = 0; i < viewItemVec.size(); ++i)
		{
			TeView* view = viewItemVec[i]->getView();
			vector<TeQtThemeItem*> themeItemVec = viewItemVec[i]->getThemeItemVec();
			for (j = 0; j < themeItemVec.size(); ++j)
			{
				TeAbstractTheme* absTheme = themeItemVec[j]->getAppTheme()->getTheme();
				if(absTheme->type() != TeTHEME)
					continue;

				if ((absTheme->grouping().groupMode_ == TeEqualSteps ||
					 absTheme->grouping().groupMode_ == TeQuantil) &&
					 absTheme->visibleGeoRep() == curTheme->visibleGeoRep() &&
					 absTheme->id() != curTheme->id())
				{
					string impGroupingAttr = absTheme->grouping().groupAttribute_.name_;
					for (int k = 0; k < attributesComboBox->count(); ++k)
					{
						if(impGroupingAttr == attributesComboBox->text(k).latin1())
						{
							viewThemeMap_.insert(make_pair(view->name(), (TeAppTheme*)themeItemVec[j]->getAppTheme()));
							importFromThemeComboBox->insertItem(absTheme->name().c_str());
							break;
						}
					}
				}				
			}

			multimap<string, TeAppTheme*>::iterator pos;
			pos = viewThemeMap_.find(view->name());
			if (pos != viewThemeMap_.end())
				importFromViewComboBox->insertItem(view->name().c_str());
		}

		if(importFromViewComboBox->count())
		{
			importFromViewComboBox->setCurrentItem(0);
			importFromViewComboBox->setEnabled(true);

			// Set the impTheme as the first one in the combo box
			string viewName = importFromViewComboBox->currentText().latin1();
			string themeName = importFromThemeComboBox->currentText().latin1();

			vector<TeQtViewItem*> viewItemVec = mainWindow_->getViewsListView()->getViewItemVec();
			for (i = 0; i < viewItemVec.size(); ++i)
			{
				TeView* view = viewItemVec[i]->getView();
				if (view->name() == viewName)
				{
					vector<TeQtThemeItem*> themeItemVec = viewItemVec[i]->getThemeItemVec();
					for (j = 0; j < themeItemVec.size(); ++j)
					{
						TeAppTheme* appTheme = themeItemVec[j]->getAppTheme();
						if (appTheme->getTheme()->name() == themeName)
							importTheme_ = appTheme;
						break;	
					}
				}
			}

			groupingParamsGroupBox->setEnabled(false);
			colorGroupBox->setEnabled(false);
			applyPushButton->setEnabled(false);
			okPushButton->setEnabled(false);
			importFromViewComboBox->setEnabled(true);
			importFromThemeComboBox->setEnabled(true);
			importPushButton->setEnabled(true);
		}
		else
		{
			QMessageBox::information (this, tr("Import Theme Grouping"),
				tr("There is no theme with grouping by Equal Steps or Quantil to be imported!"));
			importCheckBox->setChecked(false);
			importPushButton->setEnabled(false);
			groupingParamsGroupBox->setEnabled(true);
			colorGroupBox->setEnabled(true);
			applyPushButton->setEnabled(true);
			okPushButton->setEnabled(false);
		}
	}
	else
	{
		importCheckBox->setChecked(false);
		importFromViewComboBox->clear();
		importFromThemeComboBox->clear();
		importFromViewComboBox->setEnabled(false);
		importFromThemeComboBox->setEnabled(false);
		importPushButton->setEnabled(false);
		groupingParamsGroupBox->setEnabled(true);
		colorGroupBox->setEnabled(true);
		applyPushButton->setEnabled(true);
		okPushButton->setEnabled(false);
	}
}

void LegendWindow::importFromViewComboBox_activated(const QString& vName)
{
	int i;
	string viewName = vName.latin1();

	importFromThemeComboBox->clear();

	i = 0;
	multimap<string, TeAppTheme*>::iterator it = viewThemeMap_.lower_bound(viewName);

	while(it != viewThemeMap_.upper_bound(viewName))
	{
		TeAppTheme* theme = it->second;
		if(theme->getTheme()->type() == TeTHEME)
		{
			importFromThemeComboBox->insertItem(theme->getTheme()->name().c_str());
			if (i == 0)
			{
				importTheme_ = theme;
				++i;
			}
		}
		++it;
	}

	importFromThemeComboBox->setCurrentItem(0);
}


void LegendWindow::importFromThemeComboBox_activated(const QString &themeName)
{
	string viewName = importFromViewComboBox->currentText().latin1();

	multimap<string, TeAppTheme*>::iterator it = viewThemeMap_.lower_bound(viewName);
	while (it != viewThemeMap_.upper_bound(viewName))
	{
		TeAppTheme* theme = it->second;
		if (theme->getTheme()->name() == themeName.latin1())
		{
			importTheme_ = theme;
			break;
		}
		++it;
	}
}


void LegendWindow::importPushButton_clicked()
{
	TeWaitCursor wait;
	int i;

	QString importThemeName = importFromThemeComboBox->currentText();
	importFromThemeComboBox_activated(importThemeName);
	if(importTheme_ == 0)
		return;

	TeGroupingMode groupingMode = importTheme_->getTheme()->grouping().groupMode_;
	groupingModeComboBox->setCurrentItem((int)groupingMode);
	groupingModeComboBox_activated((int)groupingMode);

	// Check if the grouping attribute of the imported theme
	// is present in the current theme
	string impGroupingAttr = importTheme_->getTheme()->grouping().groupAttribute_.name_;
	string groupingAttr;
	for (i = 0; i < (int)attributesComboBox->count(); ++i)
	{
		if(impGroupingAttr == attributesComboBox->text(i).latin1())
		{
			attributesComboBox->setCurrentItem(i);
			groupingAttr = impGroupingAttr;
			break;
		}
	}

	if (groupingAttr.empty() == true)
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("The corresponding grouping attribute doesn�t exist in the current theme!"));
		return;
	}

	// Copy the legends from the imported theme to the theme in the legend source
	legendSource_.setLegends(importTheme_->getTheme()->legend());

	// Set the grouping params of the theme in the legend source
	TeGrouping gParams = importTheme_->getTheme()->grouping();
	legendSource_.setGroupingParams(gParams);

	legendTable->setNumRows(legendSource_.numRows());
	legendTable->setNumCols(legendSource_.numCols());

	// Set and adjust the columns of the table
	setAndAdjustTableColumns();

	// Update the legend interface
	precisionComboBox->setCurrentItem(gParams.groupPrecision_ - 1);

	string groupColor = importTheme_->groupColor();
	legendSource_.setGroupingColors(groupColor.c_str());
	frameBTeQtColorBar->hide();
	frameTeQtColorBar->setColorBar(getColorBarVector(groupColor, true));
	frameTeQtColorBar->drawColorBar();

	okPushButton->setEnabled(true);
}


TeAttrTableType LegendWindow::getAttrTableType()
{
	// Find the type of the table which the grouping attribute belongs
	TeTable attrTable;
	string attrTableName = groupingAttr_.substr(0,groupingAttr_.find("."));
	TeAttrTableVector& attrTablesVec = ((TeTheme*)theme_->getTheme())->attrTables();
	for (unsigned int i = 0; i < attrTablesVec.size(); ++i)
	{
		if (attrTablesVec[i].name() == attrTableName)
		{
			attrTable = attrTablesVec[i];
			break;
		}
	}

	return attrTable.tableType();
}


void LegendWindow::legendTableChanged()
{
	okPushButton->setEnabled(true);
}

void LegendWindow::legendTable_contextMenuRequested(int, int, const QPoint& p)
{
	if (groupingMode_ == TeEqualSteps)
		popup_->exec(p);
}

void LegendWindow::popupChangeMinAndMaxSlot()
{
	ChangeMinMax w(this, "", true);
	int ret = w.exec();

	if(ret == QDialog::Rejected)
		return;

	TeWaitCursor wait;

	double minValue = atof(w.minLineEdit->text().latin1());
	double maxValue = atof(w.maxLineEdit->text().latin1());

	// Set the grouping parameters and store them in a grouping structure 
	TeGrouping groupingParams;
	groupingParams.groupMode_ = TeGroupingMode(groupingModeComboBox->currentItem());
	groupingParams.groupAttribute_ = attrRep_;
	groupingParams.groupAttribute_.name_ = groupingAttr_;
	groupingParams.groupNumSlices_ = slicesComboBox->currentText().toInt();
	groupingParams.groupStdDev_ = stdDevComboBox->currentText().toFloat();
	groupingParams.groupPrecision_ = precisionComboBox->currentText().toInt();
	groupingParams.groupChronon_ = chronon_;
	if (functionComboBox->count() != 0)
		groupingParams.groupFunction_ = functionComboBox->currentText().latin1();
	groupingParams.groupNullAttr_ = true;  //Show missing data

	// Generate the legends
	legendSource_.generateLegends(groupingParams, minValue, maxValue);
	legendTable->setNumRows(legendSource_.numRows());
	legendTable->setNumCols(legendSource_.numCols());

	// Set and adjust the columns of the table
	setAndAdjustTableColumns();
	okPushButton->setEnabled(true);
}


void LegendWindow::legendTable_doubleClicked(int row, int col, int, const QPoint&)
{
	if (col == 0)
	{
		TeTheme* theme = (TeTheme*)legendSource_.getAppTheme().getTheme();
		TeLegendEntry& leg = theme->legend()[row];

		VisualWindow visualWindow(this);
		visualWindow.setCaption(tr("Legend Visual"));
		visualWindow.loadLegend(leg, theme);
		visualWindow.exec();

		legendSource_.setCell(row, col, 0);
	}
}

void LegendWindow::setAndAdjustTableColumns()
{
	legendTable->horizontalHeader()->setLabel(0, tr("Color"));
	if (groupingMode_ == TeUniqueValue)
	{
		legendTable->horizontalHeader()->setLabel(1, tr("Value"));
		legendTable->horizontalHeader()->setLabel(2, tr("Label"));
		legendTable->horizontalHeader()->setLabel(3, tr("Count"));
		legendTable->setColumnReadOnly (1, true);		// Value
		legendTable->setColumnReadOnly (2, false);		// Label
		legendTable->setColumnReadOnly (3, true);		// Count
	}
	else
	{
		legendTable->horizontalHeader()->setLabel(1, tr("From"));
		legendTable->horizontalHeader()->setLabel(2, tr("To"));
		legendTable->horizontalHeader()->setLabel(3, tr("Label"));
		legendTable->horizontalHeader()->setLabel(4, tr("Count"));
		legendTable->setColumnReadOnly (1, false);		// From
		legendTable->setColumnReadOnly (2, false);		// To
		legendTable->setColumnReadOnly (3, false);		// Label
		legendTable->setColumnReadOnly (4, true);		// Count
	}

	// Adjust the columns of the table
	legendTable->adjustColumn(0);
	legendTable->adjustColumn(1);
	legendTable->adjustColumn(2);
	legendTable->adjustColumn(3);
	if (groupingMode_ != TeUniqueValue)
		legendTable->adjustColumn(4);
}

void LegendWindow::colorChangedSlot()
{
	if(legendSource_.numRows() > 0)
	{
		vector<ColorBar> colorVec = frameTeQtColorBar->getInputColorVec();
		vector<ColorBar> colorBVec = frameBTeQtColorBar->getInputColorVec();
		legendSource_.putColorOnLegend(colorVec, colorBVec);
		setAndAdjustTableColumns();
		okPushButton->setEnabled(true);
	}
}


void LegendWindow::equalSpacePushButton_clicked()
{
	frameTeQtColorBar->setEqualSpace();
	if(frameBTeQtColorBar->isShown())
		frameBTeQtColorBar->setEqualSpace();
	colorChangedSlot();
}


void LegendWindow::saveColorPushButton_clicked()
{
	SaveAndLoadColorBarWindow* w = new SaveAndLoadColorBarWindow(this, "SaveAndLoadColorBarWindow", true);
	vector<ColorBar> inputColorVec = frameTeQtColorBar->getInputColorVec();
	vector<ColorBar> inputBColorVec = frameBTeQtColorBar->getInputColorVec();
	string colors = getColors(inputColorVec, inputBColorVec, groupingMode_);
	w->setColor(colors.c_str());
	w->exec();
	QString item = w->getColorName();
	delete w;
	loadNamesComboBox->clear();
	loadNamesComboBox->insertItem("");
	TeDatabasePortal* portal = db_->getPortal();
	if(portal->query("SELECT name FROM tv_colorbar ORDER BY name"))
	{
		while(portal->fetchRow())
			loadNamesComboBox->insertItem(portal->getData(0));
	}
	delete portal;
	loadNamesComboBox->setCurrentText(item);
}


void LegendWindow::loadNamesComboBox_activated( int )
{
	QString colorBarName = loadNamesComboBox->currentText();
	if(colorBarName.isEmpty())
		return;
	string color_;
	TeDatabasePortal* portal = db_->getPortal();
	string name =  colorBarName.latin1();
	string s = "SELECT colorbar FROM tv_colorbar WHERE name = '" + name + "'";
	if(portal->query(s))
	{
		if(portal->fetchRow())
			color_ = portal->getData(0);
	}
	delete portal;
	frameTeQtColorBar->setColorBar(getColorBarVector(color_, true));
	if(groupingMode_ == TeStdDeviation)
	{
		frameTeQtColorBar->drawColorBar();
		frameBTeQtColorBar->setColorBar(getColorBarVector(color_, false));
		frameBTeQtColorBar->drawColorBar();
	}
	else
		frameTeQtColorBar->drawColorBar();
	colorChangedSlot();
}


void LegendWindow::show()
{
	TeTheme* theme = (TeTheme*)mainWindow_->currentTheme()->getTheme();
	if(theme->type() == TeTHEME || theme->type() == TeEXTERNALTHEME)
		QDialog::show();
	else
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		reject();
		hide();
	}
}
