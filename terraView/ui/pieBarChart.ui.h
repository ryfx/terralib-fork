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

#include <TeQtViewsListView.h>
#include <TeQtThemeItem.h>
#include <TeQtChartItem.h>
#include <TeWaitCursor.h>
#include <TeDatabaseUtils.h>
#include <qcolordialog.h>
#include <TeExternalTheme.h>

#include <TeApplicationUtils.h>
#include <TeThemeFunctions.h>

void PieBarChart::init()
{
	help_ = 0;

	int i;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();

	db_ = mainWindow_->currentDatabase();
	canvas_ = mainWindow_->getCanvas();
	theme_ = mainWindow_->currentTheme();
	TeTheme* curTheme = (TeTheme*)theme_->getTheme();

	// Check whether there are numeric attributes to edit the bar or pie chart
	TeAttributeList numAttrVector = curTheme->sqlNumAttList();
	if (numAttrVector.size() == 0)
	{
		QMessageBox::warning(this, tr("Warning"),
				tr("There are no numeric attributes to edit the Pie or Bar Chart!"));
		hide();
		return;		
	}

	// Initialize the chart type of the theme in the interface
	chartTypeButtonGroup->setButton(theme_->chartType());
	chartType_ = theme_->chartType();

	//Insert the attributes in the "Proportional to" combobox for the pie chart
	TeAttributeList attributesVec = curTheme->sqlNumAttList();
	proportionalToComboBox->insertItem("NONE");
	string dimAttr = theme_->pieDimAttribute();
	int isel = -1;
	for(i = 0; i < (int)attributesVec.size(); ++i)
	{
		string name(attributesVec[i].rep_.name_.c_str());
		theme_->concatTableName(name);
		proportionalToComboBox->insertItem(name.c_str());
		chartAttributeComboBox->insertItem(name.c_str());
		if(dimAttr == name)
			isel = i;
	}

	// Fill the minimum height combobox of the bar chart
	for(i = 1; i <= 49; ++i)
		minHeightComboBox->insertItem(Te2String(i).c_str());

	// Fill the maximum height of the bar chart
	for(i = 5; i <= 50; ++i)
		maxHeightComboBox->insertItem(Te2String(i).c_str());

	// Fill the width of the bar chart
	for(i = 1; i <= 20; ++i)
		widthComboBox->insertItem(Te2String(i).c_str());

	// Fill the minimum diameter for the pie chart
	for(i = 1; i <= 49; ++i)
		minDiameterComboBox->insertItem(Te2String(i).c_str());

	// Fill the maximum diameter for the pie chart	
	for(i = 5; i <= 50; ++i)
		maxDiameterComboBox->insertItem(Te2String(i).c_str());

	// Fill the  diameter for the pie chart	
	for(i = 5; i <= 50; ++i)
		diameterComboBox->insertItem(Te2String(i).c_str());
	
	
	// Enable or disable the items associated to the dimensions according
	// the chart type, as well as set the current item for each one
	if (chartType_ == TeBarChart)
	{
		proportionalToComboBox->insertItem("");
		proportionalToComboBox->setCurrentItem(proportionalToComboBox->count() - 1);
		proportionalToComboBox->setEnabled(false);

		minDiameterComboBox->insertItem("");
		minDiameterComboBox->setCurrentItem(minDiameterComboBox->count() - 1);
		minDiameterComboBox->setEnabled(false);

		maxDiameterComboBox->insertItem("");
		maxDiameterComboBox->setCurrentItem(maxDiameterComboBox->count() - 1);
		maxDiameterComboBox->setEnabled(false);

		diameterComboBox->insertItem("");
		diameterComboBox->setCurrentItem(diameterComboBox->count() - 1);
		diameterComboBox->setEnabled(false);

		int barMinHeight = (int)theme_->barMinHeight();
		if(theme_->keepDimension() == false)
			barMinHeight = canvas_->mapDWtoV((double)barMinHeight);
		if (barMinHeight == 0 || barMinHeight == -1)
			minHeightComboBox->setCurrentItem(9);
		else
			minHeightComboBox->setCurrentText(Te2String(barMinHeight + 1).c_str());

		int barMaxHeight = (int)theme_->barMaxHeight();
		if(theme_->keepDimension() == false)
			barMaxHeight = canvas_->mapDWtoV((double)barMaxHeight);
		if (barMaxHeight == 0 || barMaxHeight == -1)
			maxHeightComboBox->setCurrentItem(35);
		else
			maxHeightComboBox->setCurrentText(Te2String(barMaxHeight + 1).c_str());
	
		int barWidth = (int)theme_->barWidth();
		if(theme_->keepDimension() == false)
			barWidth = canvas_->mapDWtoV((double)barWidth);
		if (barWidth == 0 || barWidth == -1)
			widthComboBox->setCurrentItem(5);
		else
			widthComboBox->setCurrentText(Te2String(barWidth + 1).c_str());
	
	
	}
	else if (chartType_ == TePieChart)
	{
		minHeightComboBox->insertItem("");
		minHeightComboBox->setCurrentItem(minHeightComboBox->count() - 1);
		minHeightComboBox->setEnabled(false);

		maxHeightComboBox->insertItem("");
		maxHeightComboBox->setCurrentItem(maxHeightComboBox->count() - 1);
		maxHeightComboBox->setEnabled(false);

		widthComboBox->insertItem("");
		widthComboBox->setCurrentItem(widthComboBox->count() - 1);
		widthComboBox->setEnabled(false);

		if(isel > -1)
		{
			proportionalToComboBox->setCurrentItem(isel+1);

			diameterComboBox->insertItem("");
			diameterComboBox->setCurrentItem(diameterComboBox->count() - 1);
			diameterComboBox->setEnabled(false);

			int pieMinDiameter = (int)theme_->pieMinDiameter();
			if(theme_->keepDimension() == false)
				pieMinDiameter = canvas_->mapDWtoV((double)pieMinDiameter);
			if (pieMinDiameter == 0 || pieMinDiameter == -1)
				minDiameterComboBox->setCurrentItem(9);
			else
				minDiameterComboBox->setCurrentText(Te2String(pieMinDiameter + 1).c_str());

			int pieMaxDiameter = (int)theme_->pieMaxDiameter();
			if(theme_->keepDimension() == false)
				pieMaxDiameter = canvas_->mapDWtoV((double)pieMaxDiameter);
			if (pieMaxDiameter == 0 || pieMaxDiameter == -1)
				maxDiameterComboBox->setCurrentItem(35);
			else
				maxDiameterComboBox->setCurrentText(Te2String(pieMaxDiameter + 1).c_str());

		}
		else
		{
			proportionalToComboBox->setCurrentItem(0);

			minDiameterComboBox->insertItem("");
			minDiameterComboBox->setCurrentItem(minDiameterComboBox->count() - 1);
			minDiameterComboBox->setEnabled(false);

			maxDiameterComboBox->insertItem("");
			maxDiameterComboBox->setCurrentItem(maxDiameterComboBox->count() - 1);
			maxDiameterComboBox->setEnabled(false);

			int pieDiameter = (int)theme_->pieDiameter();
			if(theme_->keepDimension() == false)
				pieDiameter = canvas_->mapDWtoV((double)pieDiameter);
			if (pieDiameter == 0 || pieDiameter == -1)
				diameterComboBox->setCurrentItem(15);
			else
				diameterComboBox->setCurrentText(Te2String(pieDiameter + 1).c_str());

		}
	}

	// Set the check box that indicates if the chart will follow or 
	// not the zoom in the canvas
	if(theme_->keepDimension())
		fixedSizeCheckBox->setChecked(true);
	else
		fixedSizeCheckBox->setChecked(false);

	// Set the chart aggregation function
	TeAttrTableVector tablesVec;
	if (curTheme->getAttTables(tablesVec, TeAttrEvent) ||
		curTheme->getAttTables(tablesVec, TeFixedGeomDynAttr) ||
		curTheme->getAttTables(tablesVec, TeAttrExternal))
	{
		chartFunctionComboBox->insertItem("AVG");
		chartFunctionComboBox->insertItem("MAX");
		chartFunctionComboBox->insertItem("MIN");
		chartFunctionComboBox->insertItem("SUM");

		if (theme_->chartFunction().empty())
			chartFunctionComboBox->setCurrentItem(0);
		else
			chartFunctionComboBox->setCurrentText(theme_->chartFunction().c_str());
	}
	else
		chartFunctionComboBox->setEnabled(false);

	for(i = 0; i < (int)theme_->chartAttributes_.size(); ++i)
	{
		chartAttributesVec_.push_back(theme_->chartAttributes_[i]);
		chartColorsVec_.push_back(theme_->chartColors_[i]);

		int	ww = 21;
		int	pixh = 16;

		QPixmap	 pixmap(ww, pixh);
		QPainter p(&pixmap);
		QBrush	 brush;
		QColor	 cor;

		int	r = theme_->chartColors_[i].red_;
		int	g = theme_->chartColors_[i].green_;
		int	b = theme_->chartColors_[i].blue_;
		cor.setRgb(r, g, b);
		brush.setColor(cor);
		brush.setStyle(Qt::SolidPattern);

		p.fillRect (1,1,ww-2,pixh-2,brush);
		p.end();

		chartTable->setPixmap(i, 0, pixmap);
		chartTable->setText(i, 1, theme_->chartAttributes_[i].c_str());
	}

	//Set the horizontal header labels of the chart table
	chartTable->horizontalHeader()->setLabel(0, tr("Color"));
	chartTable->horizontalHeader()->setLabel(1, tr("Attribute"));
	chartTable->adjustColumn(0);
	chartTable->adjustColumn(1);

	tableRow_ = theme_->chartAttributes_.size();
	colorIndex_ = 0;
	getChartColors();

	if (chartAttributesVec_.size() != 0)
	{
		removeChartAttributePushButton->setEnabled(true);
		okPushButton->setEnabled(true);
	}
	else
	{
		removeChartAttributePushButton->setEnabled(false);
		okPushButton->setEnabled(false);
	}

	inputComboBox->clear();
	inputComboBox->insertItem(tr("All"));
	inputComboBox->insertItem(tr("Selected by Pointing"));
	inputComboBox->insertItem(tr("Not Selected by Pointing"));
	inputComboBox->insertItem(tr("Selected by Query"));
	inputComboBox->insertItem(tr("Not Selected by Query"));
	inputComboBox->insertItem(tr("Grouped"));
	inputComboBox->insertItem(tr("Not Grouped"));

	if(theme_->chartAttributes_.empty())
		inputComboBox->setCurrentItem(0);
	else
		inputComboBox->setCurrentItem(theme_->chartObjects());

	diameterComboBox->setEditable(true);
	minDiameterComboBox->setEditable(true);
	maxDiameterComboBox->setEditable(true);
	minHeightComboBox->setEditable(true);
	maxHeightComboBox->setEditable(true);

	connect(chartTable, SIGNAL(doubleClicked(int, int, int, const QPoint&)),
			this, SLOT(chartTable_doubleClicked(int, int, int, const QPoint&)));
}


void PieBarChart::getChartColors()
{
	colorsVec_.clear();
	TeColor c;
	c.init(230, 153, 230);
	colorsVec_.push_back(c);
	c.init(153, 230, 230);
	colorsVec_.push_back(c);
	c.init(230, 230, 153);
	colorsVec_.push_back(c);
	c.init(153, 230, 153);
	colorsVec_.push_back(c);
	c.init(153, 153, 230);
	colorsVec_.push_back(c);
	c.init(230, 153, 153);
	colorsVec_.push_back(c);
	c.init(50, 100, 200);
	colorsVec_.push_back(c);
	c.init(50, 200, 100);
	colorsVec_.push_back(c);
	c.init(100, 50, 200);
	colorsVec_.push_back(c);
	c.init(100, 200, 50);
	colorsVec_.push_back(c);
	c.init(200, 50, 100);
	colorsVec_.push_back(c);
	c.init(200, 100, 50);
	colorsVec_.push_back(c);
	c.init(80, 150, 200);
	colorsVec_.push_back(c);
	c.init(80, 200, 150);
	colorsVec_.push_back(c);
	c.init(150, 80, 200);
	colorsVec_.push_back(c);
	c.init(150, 200, 80);
	colorsVec_.push_back(c);
	c.init(200, 80, 150);
	colorsVec_.push_back(c);
	c.init(200, 150, 80);
	colorsVec_.push_back(c);
	c.init(30, 150, 100);
	colorsVec_.push_back(c);
	c.init(30, 100, 150);
	colorsVec_.push_back(c);
	c.init(150, 30, 100);
	colorsVec_.push_back(c);
	c.init(150, 100, 30);
	colorsVec_.push_back(c);
	c.init(100, 30, 150);
	colorsVec_.push_back(c);
	c.init(100, 150, 30);
	colorsVec_.push_back(c);
	c.init(230, 150, 100);
	colorsVec_.push_back(c);
	c.init(230, 100, 150);
	colorsVec_.push_back(c);
	c.init(150, 230, 100);
	colorsVec_.push_back(c);
	c.init(150, 100, 230);
	colorsVec_.push_back(c);
	c.init(100, 230, 150);
	colorsVec_.push_back(c);
	c.init(100, 150, 230);
	colorsVec_.push_back(c);
	colorMaxIndex_ = (int)colorsVec_.size();
}


void PieBarChart::chartTypeButtonGroup_clicked(int chartType)
{
	if (chartType_ == (TeChartType)chartType)
		return;

	chartType_ = (TeChartType)chartType;

	if (chartType_ == TeBarChart)
	{
		proportionalToComboBox->insertItem("");
		proportionalToComboBox->setCurrentItem(proportionalToComboBox->count() - 1);
		proportionalToComboBox->setEnabled(false);

		minHeightComboBox->removeItem(minHeightComboBox->count() - 1);
		int barMinHeight = (int)theme_->barMinHeight();
		if(theme_->keepDimension() == false)
			barMinHeight = canvas_->mapDWtoV((double)barMinHeight);
		if (barMinHeight == 0 || barMinHeight == -1)
			minHeightComboBox->setCurrentItem(9);
		else
			minHeightComboBox->setCurrentText(Te2String(barMinHeight + 1).c_str());
		minHeightComboBox->setEnabled(true);

		maxHeightComboBox->removeItem(maxHeightComboBox->count() - 1);
		int barMaxHeight = (int)theme_->barMaxHeight();
		if(theme_->keepDimension() == false)
			barMaxHeight = canvas_->mapDWtoV((double)barMaxHeight);
		if (barMaxHeight == 0 || barMaxHeight == -1)
			maxHeightComboBox->setCurrentItem(35);
		else
			maxHeightComboBox->setCurrentText(Te2String(barMaxHeight + 1).c_str());
		maxHeightComboBox->setEnabled(true);

		widthComboBox->removeItem(widthComboBox->count() - 1);
		int barWidth = (int)theme_->barWidth();
		if(theme_->keepDimension() == false)
			barWidth = canvas_->mapDWtoV((double)barWidth);
		if (barWidth == 0 || barWidth == -1)
			widthComboBox->setCurrentItem(5);
		else
			widthComboBox->setCurrentText(Te2String(barWidth).c_str());
		widthComboBox->setEnabled(true);

		if (minDiameterComboBox->isEnabled() == true)
		{
			minDiameterComboBox->insertItem("");
			minDiameterComboBox->setCurrentItem(minDiameterComboBox->count() - 1);
			minDiameterComboBox->setEnabled(false);
		}

		if (maxDiameterComboBox->isEnabled() == true)
		{
			maxDiameterComboBox->insertItem("");
			maxDiameterComboBox->setEnabled(false);
			maxDiameterComboBox->setCurrentItem(maxDiameterComboBox->count() - 1);
		}

		if (diameterComboBox->isEnabled() == true)
		{
			diameterComboBox->insertItem("");
			diameterComboBox->setEnabled(false);
			diameterComboBox->setCurrentItem(diameterComboBox->count() - 1);
		}
	}
	else if (chartType_ == TePieChart)
	{
		proportionalToComboBox->removeItem(proportionalToComboBox->count() - 1);
		proportionalToComboBox->setCurrentItem(0);
		proportionalToComboBox->setEnabled(true);

		minHeightComboBox->insertItem("");
		minHeightComboBox->setCurrentItem(minHeightComboBox->count() - 1);
		minHeightComboBox->setEnabled(false);

		maxHeightComboBox->insertItem("");
		maxHeightComboBox->setCurrentItem(maxHeightComboBox->count() - 1);
		maxHeightComboBox->setEnabled(false);

		widthComboBox->insertItem("");
		widthComboBox->setCurrentItem(widthComboBox->count() - 1);
		widthComboBox->setEnabled(false);

		diameterComboBox->removeItem(diameterComboBox->count() - 1);
		int pieDiameter = (int)theme_->pieDiameter();
		if(theme_->keepDimension() == false)
			pieDiameter = canvas_->mapDWtoV((double)pieDiameter);
		if (pieDiameter == 0 || pieDiameter == -1)
			diameterComboBox->setCurrentItem(15);
		else
			diameterComboBox->setCurrentText(Te2String(pieDiameter).c_str());
		diameterComboBox->setEnabled(true);
	}

//	chartAttributesVec_.clear();
//	chartColorsVec_.clear();
//	tableRow_ = 0;
//	clearChartTable();
//	colorIndex_ = 0;

//	removeChartAttributePushButton->setEnabled(false);
//	okPushButton->setEnabled(false);
}


void PieBarChart::proportionalToComboBox_activated(const QString& propTo)
{
	if(propTo == "NONE")
	{
		if (minDiameterComboBox->isEnabled() == true)
		{
			minDiameterComboBox->insertItem("");
			minDiameterComboBox->setCurrentItem(minDiameterComboBox->count() - 1);
			minDiameterComboBox->setEnabled(false);
		}

		if (maxDiameterComboBox->isEnabled() == true)
		{
			maxDiameterComboBox->insertItem("");
			maxDiameterComboBox->setCurrentItem(maxDiameterComboBox->count() - 1);
			maxDiameterComboBox->setEnabled(false);
		}

		if (diameterComboBox->isEnabled() == false)
		{
			diameterComboBox->removeItem(diameterComboBox->count() - 1);
			int pieDiameter = (int)theme_->pieDiameter();
			if(theme_->keepDimension() == false)
				pieDiameter = canvas_->mapDWtoV((double)pieDiameter);
			if (pieDiameter == 0 || pieDiameter == -1)
				diameterComboBox->setCurrentItem(15);
			else
				diameterComboBox->setCurrentText(Te2String(pieDiameter).c_str());
			diameterComboBox->setEnabled(true);
		}
	}
	else
	{
		if (minDiameterComboBox->isEnabled() == false)
		{
			minDiameterComboBox->removeItem(minDiameterComboBox->count() - 1);
			int pieMinDiameter = (int)theme_->pieMinDiameter();
			if(theme_->keepDimension() == false)
				pieMinDiameter = canvas_->mapDWtoV((double)pieMinDiameter);
			if (pieMinDiameter == 0 || pieMinDiameter == -1)
				minDiameterComboBox->setCurrentItem(9);
			else
				minDiameterComboBox->setCurrentText(Te2String(pieMinDiameter).c_str());
			minDiameterComboBox->setEnabled(true);
		}

		if (maxDiameterComboBox->isEnabled() == false)
		{
			maxDiameterComboBox->removeItem(maxDiameterComboBox->count() - 1);
			int pieMaxDiameter = (int)theme_->pieMaxDiameter();
			if(theme_->keepDimension() == false)
				pieMaxDiameter = canvas_->mapDWtoV((double)pieMaxDiameter);
			if (pieMaxDiameter == 0 || pieMaxDiameter == -1)
				maxDiameterComboBox->setCurrentItem(35);
			else
				maxDiameterComboBox->setCurrentText(Te2String(pieMaxDiameter).c_str());
			maxDiameterComboBox->setEnabled(true);
		}

		if (diameterComboBox->isEnabled() == true)
		{
			diameterComboBox->insertItem("");
			diameterComboBox->setCurrentItem(diameterComboBox->count() - 1);
			diameterComboBox->setEnabled(false);
		}
	}
}


void PieBarChart::minHeightComboBox_activated(const QString& qmin)
{
	string min = qmin.latin1();
	string max = maxHeightComboBox->currentText().latin1();

	int vmin = atoi(min.c_str());
	int vmax = atoi(max.c_str());

	if(vmax <= vmin)
		vmax = vmin + 1;
	maxHeightComboBox->setCurrentText(Te2String(vmax).c_str());
}


void PieBarChart::maxHeightComboBox_activated(const QString& qmax)
{
	string min = minHeightComboBox->currentText().latin1();
	string max = qmax.latin1();

	int vmin = atoi(min.c_str());
	int vmax = atoi(max.c_str());

	if(vmin >= vmax)
		vmin = vmax - 1;
	minHeightComboBox->setCurrentText(Te2String(vmin).c_str());
}


void PieBarChart::minDiameterComboBox_activated(const QString& qmin)
{
	string min = qmin.latin1();
	string max = maxDiameterComboBox->currentText().latin1();

	int vmin = atoi(min.c_str());
	int vmax = atoi(max.c_str());

	if(vmax <= vmin)
		vmax = vmin + 1;
	maxDiameterComboBox->setCurrentText(Te2String(vmax).c_str());
}


void PieBarChart::maxDiameterComboBox_activated( const QString& qmax)
{
	string min = minDiameterComboBox->currentText().latin1();
	string max = qmax.latin1();

	int vmin = atoi(min.c_str());
	int vmax = atoi(max.c_str());

	if(vmin >= vmax)
		vmin = vmax - 1;
	minDiameterComboBox->setCurrentText(Te2String(vmin).c_str());
}


void PieBarChart::insertChartAttributePushButton_clicked()
{
	TeWaitCursor wait;

	string name = chartAttributeComboBox->currentText().latin1();
	
	unsigned int i;
	for(i = 0; i < chartAttributesVec_.size(); ++i)
		if(chartAttributesVec_[i] == name)
			return;
	if(tableRow_ == 10)
		return;

	int	ww = 21;
	int	pixh = 16;

	QPixmap		pixmap(ww, pixh);
	QPainter	p(&pixmap);
	QBrush		brush;
	QColor		cor;
	TeColor		tcor;

	while(1)
	{
		int rr = colorsVec_[colorIndex_].red_;
		int gg = colorsVec_[colorIndex_].green_;
		int bb = colorsVec_[colorIndex_].blue_;

		for(i = 0; i < chartColorsVec_.size(); ++i)
		{
			int r = chartColorsVec_[i].red_;
			int g = chartColorsVec_[i].green_;
			int b = chartColorsVec_[i].blue_;
			if(rr == r && gg == g && bb == b)
				break;
		}
		if(i == chartColorsVec_.size())
			break;
		else
			colorIndex_++;
	}
		
	int	c = colorIndex_;
	cor.setRgb(colorsVec_[c].red_, colorsVec_[c].green_, colorsVec_[c].blue_);
	tcor = colorsVec_[c];
	brush.setColor(cor);
	brush.setStyle(Qt::SolidPattern);

	p.fillRect (1,1,ww-2,pixh-2,brush);
	p.end();

	chartTable->setPixmap(tableRow_, 0, pixmap);
	chartTable->setText(tableRow_, 1, name.c_str());
	chartTable->adjustColumn(0);
	chartTable->adjustColumn(1);

	chartAttributesVec_.push_back(name);
	chartColorsVec_.push_back(tcor);

	colorIndex_++;
	if(colorIndex_ >= colorMaxIndex_)
		colorIndex_ = 0;
	tableRow_++;

	removeChartAttributePushButton->setEnabled(true);
	okPushButton->setEnabled(true);
}


void PieBarChart::removeChartAttributePushButton_clicked()
{
	unsigned int size = chartAttributesVec_.size();
	unsigned int i;

	if (chartTable->currentSelection() == -1)
	{
		QMessageBox::warning(this, tr("Warning"),
		tr("Select the grid row to be removed!"));
		return;
	}

	for(i = 0; i < size; ++i)
	{
		if(chartTable->isRowSelected(i, true))
		{
			string label = chartTable->text(i, 1).latin1();
			vector<string>::iterator it = chartAttributesVec_.begin();
			vector<TeColor>::iterator cit = chartColorsVec_.begin();

			for (it = chartAttributesVec_.begin(), cit = chartColorsVec_.begin();
			     it != chartAttributesVec_.end(); ++it, ++cit)
			{
				if((*it) == label)
				{
					chartAttributesVec_.erase(it);
					chartColorsVec_.erase(cit);
					break;
				}
			}
		}
	}
	clearChartTable();
	
	int		ww = 21;
	int		pixh = 16;
	QPixmap		pixmap(ww, pixh);
	QPainter	p(&pixmap);
	QBrush		brush;
	QColor		cor;

	brush.setStyle(Qt::SolidPattern);
	tableRow_ = 0;
	for(i = 0; i < chartAttributesVec_.size(); ++i)
	{
		cor.setRgb(chartColorsVec_[i].red_, chartColorsVec_[i].green_, chartColorsVec_[i].blue_);
		brush.setColor(cor);

		p.fillRect (1,1,ww-2,pixh-2,brush);

		chartTable->setPixmap(tableRow_, 0, pixmap);
		chartTable->setText(tableRow_, 1, chartAttributesVec_[i].c_str());

		tableRow_++;
	}
	chartTable->adjustColumn(0);
	chartTable->adjustColumn(1);

	if (chartAttributesVec_.size() != 0)
	{
		removeChartAttributePushButton->setEnabled(true);
		okPushButton->setEnabled(true);
	}
	else
	{
		removeChartAttributePushButton->setEnabled(false);
		okPushButton->setEnabled(false);
	}
}


void PieBarChart::clearChartTable()
{
	int	i, j;

	for(i = 0; i < chartTable->numRows(); ++i)
	{
		for (j = 0; j < chartTable->numCols(); ++j)
			chartTable->clearCell(i, j);
	}
	chartTable->clearSelection(true);
}


void PieBarChart::okPushButton_clicked()
{
	TeWaitCursor wait;
	unsigned int i;

	// Save the chart parameters in the current theme
	theme_->keepDimension(fixedSizeCheckBox->isChecked());

	if(chartType_ == TeBarChart)
	{
		theme_->chartType(TeBarChart);
		theme_->barMaxHeight(maxHeightComboBox->currentText().toFloat());
		theme_->barMinHeight(minHeightComboBox->currentText().toFloat());
		theme_->barWidth(widthComboBox->currentText().toInt());
		theme_->pieDiameter(-1);
		theme_->pieMaxDiameter(-1);
		theme_->pieMinDiameter(-1);
		theme_->pieDimAttribute("NONE");

		if(theme_->keepDimension() == false)
		{
//			theme_->barWidth(canvas_->mapVtoCW((int)theme_->barWidth()));
//			theme_->barMaxHeight(canvas_->mapVtoCW((int)theme_->barMaxHeight()));
//			theme_->barMinHeight(canvas_->mapVtoCW((int)theme_->barMinHeight()));
			theme_->barWidth(canvas_->mapVtoDW((int)theme_->barWidth()));
			theme_->barMaxHeight(canvas_->mapVtoDW((int)theme_->barMaxHeight()));
			theme_->barMinHeight(canvas_->mapVtoDW((int)theme_->barMinHeight()));
		}
	}
	else if(chartType_ == TePieChart)
	{
		theme_->chartType(TePieChart);
		theme_->barMaxHeight(-1);
		theme_->barMinHeight(-1);
		theme_->barWidth(-1);

		theme_->pieDimAttribute(proportionalToComboBox->currentText().latin1());
		if(theme_->pieDimAttribute() == "NONE")
		{
			theme_->pieDiameter(diameterComboBox->currentText().toFloat());
			theme_->pieMaxDiameter(-1);
			theme_->pieMinDiameter(-1);
			if(theme_->keepDimension() == false)
				theme_->pieDiameter(canvas_->mapVtoDW((int)theme_->pieDiameter()));
//				theme_->pieDiameter(canvas_->mapVtoCW((int)theme_->pieDiameter()));
		}
		else
		{
			theme_->pieDiameter(-1);
			theme_->pieMaxDiameter(maxDiameterComboBox->currentText().toFloat());
			theme_->pieMinDiameter(minDiameterComboBox->currentText().toFloat());

			if(theme_->keepDimension() == false)
			{
//				theme_->pieMaxDiameter(canvas_->mapVtoCW((int)theme_->pieMaxDiameter()));
//				theme_->pieMinDiameter(canvas_->mapVtoCW((int)theme_->pieMinDiameter()));
				theme_->pieMaxDiameter(canvas_->mapVtoDW((int)theme_->pieMaxDiameter()));
				theme_->pieMinDiameter(canvas_->mapVtoDW((int)theme_->pieMinDiameter()));
			}
		}
	}

	string FUNC = "MIN";
	if (chartFunctionComboBox->count() != 0)
		FUNC = chartFunctionComboBox->currentText().latin1();
	theme_->chartFunction(FUNC);

	theme_->chartColors_.clear();
	theme_->chartAttributes_.clear();

	for(i = 0; i < chartAttributesVec_.size(); ++i)
	{
		theme_->chartColors_.push_back(chartColorsVec_[i]);
		theme_->chartAttributes_.push_back(chartAttributesVec_[i]);
	}

	TeTheme* curTheme = (TeTheme*)theme_->getTheme();
	TeDatabase* db;
	string sqlfrom, CT, CA;
	if(curTheme->getProductId() == TeTHEME)
	{
		db = curTheme->layer()->database();
		sqlfrom = curTheme->sqlGridFrom();
		CT = curTheme->collectionTable();
		CA = curTheme->collectionAuxTable();
	}
	else if(curTheme->getProductId() == TeEXTERNALTHEME)
	{
		db = ((TeExternalTheme*)curTheme)->getRemoteTheme()->layer()->database();
		sqlfrom = ((TeExternalTheme*)curTheme)->getRemoteTheme()->sqlGridFrom();
		CT = ((TeExternalTheme*)curTheme)->getRemoteTheme()->collectionTable();
		CA = ((TeExternalTheme*)curTheme)->getRemoteTheme()->collectionAuxTable();
	}

	TeDatabasePortal* portal = db->getPortal();
	
	canvas_->setDataProjection (curTheme->getThemeProjection());
	int curitem = inputComboBox->currentItem();
	theme_->chartObjects((TeSelectedObjects)curitem);


	if(curTheme->getProductId() == TeTHEME)
	{
		if(curitem == 1)
		{
			sqlfrom += " WHERE (grid_status = 1 OR grid_status = 3";
			sqlfrom += " OR (grid_status is null AND (c_object_status = 1 OR c_object_status = 3)))";
		}
		else if(curitem == 2)
		{
			sqlfrom += " WHERE (grid_status = 0 OR grid_status = 2";
			sqlfrom += " OR (grid_status is null AND (c_object_status = 0 OR c_object_status = 2)))";
		}
		else if(curitem == 3)
			sqlfrom += " WHERE (grid_status >= 2 OR (grid_status is null AND c_object_status >= 2))";
		else if(curitem == 4)
			sqlfrom += " WHERE (grid_status < 2 OR (grid_status is null AND c_object_status < 2))";
		else if(curitem == 5)
			sqlfrom += " WHERE (c_legend_id <> 0)";
		else if(curitem == 6)
			sqlfrom += " WHERE (c_legend_id = 0)";

		if(chartType_ == TeBarChart)
		{
			string q = "SELECT";
			for (i = 0; i < theme_->chartAttributes_.size(); ++i)
				q += " " + FUNC + "(" + theme_->chartAttributes_[i] + ") AS F" + Te2String(i) + ",";

			q = q.substr(0, q.size() - 1);
			q += sqlfrom;
			q += " GROUP BY " + CT + ".c_object_id";

			string qq = "SELECT";
			for (i = 0; i < theme_->chartAttributes_.size(); ++i)
				qq += " MAX(T.F" + Te2String(i) + "), MIN(T.F" + Te2String(i) + "),";
			qq = qq.substr(0, qq.size() - 1);
			
			if((db->dbmsName()!="OracleAdo") && (db->dbmsName()!="OracleSpatial"))
				qq += " FROM (" + q + ") AS T";  
			else
				qq += " FROM (" + q + ") T";  

			if(portal->query(qq))
			{
				if(portal->fetchRow())
				{
					double max = -TeMAXFLOAT;
					double min = TeMAXFLOAT;
					for (i = 0; i < theme_->chartAttributes_.size(); i++)
					{
						double valmax = atof(portal->getData(i*2));
						double valmin = atof(portal->getData(i*2+1));
						max = MAX(valmax, max);
						min = MIN(valmin, min);
					}
					theme_->chartMaxVal(max);
					theme_->chartMinVal(min);
				}
			}
		}	
		else
		{
			if(theme_->pieDimAttribute() != "NONE")
			{
				string q = "SELECT " + FUNC + "(" + theme_->pieDimAttribute() + ") AS F";
				q += sqlfrom;
				q += " GROUP BY " + CT + ".c_object_id";

				string qq;
				if((db->dbmsName()!="OracleAdo") && (db->dbmsName()!="OracleSpatial"))
					qq = "SELECT MAX(T.F), MIN(T.F) FROM (" + q + ") AS T";
				else
					qq = "SELECT MAX(T.F), MIN(T.F) FROM (" + q + ") T";

				if(portal->query(qq))
				{
					if(portal->fetchRow())
					{
						theme_->chartMaxVal(atof(portal->getData(0)));
						theme_->chartMinVal(atof(portal->getData(1)));
					}
				}
			}
		}
	}
	else if(curTheme->getProductId() == TeEXTERNALTHEME)
	{
		vector<string>::iterator it;
		vector<string> itenVec;
		if(curitem == 0)
			itenVec = getItems(curTheme, TeAll);
		else if(curitem == 1)
			itenVec = getItems(curTheme, TeSelectedByPointing);
		else if(curitem == 2)
			itenVec = getItems(curTheme, TeNotSelectedByPointing);
		else if(curitem == 3)
			itenVec = getItems(curTheme, TeSelectedByQuery);
		else if(curitem == 4)
			itenVec = getItems(curTheme, TeNotSelectedByQuery);
		else if(curitem == 5)
			itenVec = getItems(curTheme, TeGrouped);
		else if(curitem == 6)
			itenVec = getItems(curTheme, TeNotGrouped);

		double max = -TeMAXFLOAT;
		double min = TeMAXFLOAT;
		std::vector<std::string>::iterator it_begin = itenVec.begin();
		std::vector<std::string>::iterator it_end = itenVec.end();

		vector<string> sv = generateItemsInClauses(it_begin, it_end, curTheme);
		if(chartType_ == TeBarChart)
		{
			for(it=sv.begin(); it!=sv.end(); ++it)
			{
				string q = "SELECT";
				for (i = 0; i < theme_->chartAttributes_.size(); ++i)
					q += " " + FUNC + "(" + theme_->chartAttributes_[i] + ") AS F" + Te2String(i) + ",";

				q = q.substr(0, q.size() - 1);
				q += sqlfrom + " WHERE " + CA + ".unique_id IN " + *it;
				q += " GROUP BY " + CT + ".c_object_id";

				string qq = "SELECT";
				for (i = 0; i < theme_->chartAttributes_.size(); ++i)
					qq += " MAX(T.F" + Te2String(i) + "), MIN(T.F" + Te2String(i) + "),";
				qq = qq.substr(0, qq.size() - 1);
				
				if((db->dbmsName()!="OracleAdo") && (db->dbmsName()!="OracleSpatial"))
					qq += " FROM (" + q + ") AS T";  
				else
					qq += " FROM (" + q + ") T";  

				portal->freeResult();
				if(portal->query(qq))
				{
					if(portal->fetchRow())
					{
						for (i = 0; i < theme_->chartAttributes_.size(); i++)
						{
							double valmax = atof(portal->getData(i*2));
							double valmin = atof(portal->getData(i*2+1));
							max = MAX(valmax, max);
							min = MIN(valmin, min);
						}
					}
				}
			}
			theme_->chartMaxVal(max);
			theme_->chartMinVal(min);
		}	
		else
		{
			if(theme_->pieDimAttribute() != "NONE")
			{
				for(it=sv.begin(); it!=sv.end(); ++it)
				{
					string q = "SELECT " + FUNC + "(" + theme_->pieDimAttribute() + ") AS F";
					q += sqlfrom + " WHERE " + CA + ".unique_id IN " + *it;
					q += " GROUP BY " + CT + ".c_object_id";

					string qq;
					if((db->dbmsName()!="OracleAdo") && (db->dbmsName()!="OracleSpatial"))
						qq = "SELECT MAX(T.F), MIN(T.F) FROM (" + q + ") AS T";
					else
						qq = "SELECT MAX(T.F), MIN(T.F) FROM (" + q + ") T";

					portal->freeResult();
					if(portal->query(qq))
					{
						if(portal->fetchRow())
						{
							max = MAX(max, atof(portal->getData(0)));
							min = MIN(min, atof(portal->getData(1)));
						}
					}
				}
				theme_->chartMaxVal(max);
				theme_->chartMinVal(min);
			}
		}
	}

	delete portal;
	deleteChartParams(db_, theme_);
	insertChartParams(db_, theme_);
	if(theme_->chartAttributes_.size())
		curTheme->visibleRep(curTheme->visibleRep() | 0x80000000);
	else
		curTheme->visibleRep(curTheme->visibleRep() & 0x7fffffff);

	updateAppTheme (db_, theme_);
//	accept();

	// Insert the new chart items in the theme interface
	mainWindow_->updateThemeItemInterface(mainWindow_->getViewsListView()->currentThemeItem());

	// Draw the chart on the canvas
	mainWindow_->drawAction_activated();
}



void PieBarChart::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("pieBarChart.htm");
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


void PieBarChart::chartTable_doubleClicked(int row, int col, int, const QPoint&)
{
	if (col == 0)
	{
		if(row >= (int)chartColorsVec_.size())
			return;

		TeColor cor = chartColorsVec_[row];
		QColor color(cor.red_, cor.green_, cor.blue_);
		bool isOK = false;

		QColor	qc = QColorDialog::getRgba (color.rgb(), &isOK, this );
		if (isOK)
		{
			cor.init(qc.red(), qc.green(), qc.blue());
			chartColorsVec_[row] = cor;

			chartTable->clearCell(row, col);
			int		ww = 21;
			int		pixh = 16;
			QPixmap		pixmap(ww, pixh);
			QPainter	p(&pixmap);
			QBrush		brush;

			brush.setStyle(Qt::SolidPattern);
			brush.setColor(qc);
			p.fillRect (1,1,ww-2,pixh-2,brush);

			chartTable->setPixmap(row, 0, pixmap);
			chartTable->adjustColumn(0);
			chartTable->adjustColumn(1);

			repaint();
		}
	}
}
