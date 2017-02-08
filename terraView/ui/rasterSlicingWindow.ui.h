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
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <TeQtLegendItem.h>
#include <TeColorUtils.h>
#include <TeLayer.h>
#include <terraViewBase.h>
#include <TeRasterTransform.h>
#include <TeWaitCursor.h>
#include <TeGroupingAlgorithms.h>
#include <TeAppTheme.h>
#include <TeDatabaseUtils.h>
#include <saveAndLoadColorBarWindow.h>
#include <visual.h>
#include <TeApplicationUtils.h>

#include <qmessagebox.h>
#include <qbitmap.h>
#include <qpainter.h>
#include <qapplication.h>
#include <qcolordialog.h>

#define MAXSLICES 200

void RasterSlicingWindow::init()
{
	help_ = 0;
	invertColor_ = false;

	TeWaitCursor wait;
	int i;

	// Get variables from the main window
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	TeQtThemeItem* themeItem_ = (TeQtThemeItem*)mainWindow_->getViewsListView()->currentThemeItem();
	if(importCheckBox->isChecked() && importTheme_)
		theme_ = importTheme_;
	else
		theme_ = mainWindow_->currentTheme();

	TeTheme* baseTheme = (TeTheme*)theme_->getTheme();
	themeItem_ = (TeQtThemeItem*)mainWindow_->getViewsListView()->currentThemeItem();

	// check if theme comes from a layer with raster representation
	raster_ = baseTheme->layer()->raster();
	if (!raster_)
	{
		QMessageBox::warning(this,tr("Warning"),
					tr("Current layer has no raster representation!"));
		reject();
		return;
	}
	// Fill the slices combobox according to the grouping mode
	slicesComboBox->clear();
	for(i = 0; i < MAXSLICES; ++i)
	{
		string s = Te2String(i+1);
		slicesComboBox->insertItem(s.c_str());
	}

	// Fill the interface according to previously set groupings
	TeGrouping& groupingStruct = baseTheme->grouping();
	if (groupingStruct.groupMode_ != TeNoGrouping)
	{
		// if there is a grouping defined, make sure it refers to a raster slicing
		if (groupingStruct.groupMode_ != TeRasterSlicing)
		{
			QMessageBox::warning(this,tr("Warning"),tr("Wrong grouping mode!"));

			reject();
			return;
		}
	}

	// Insert bands that can be grouped
	bandComboBox->clear();
	for (i=0; i < raster_->nBands(); i++)
		bandComboBox->insertItem(QString("%1").arg(i));
	
	if (groupingStruct.groupMode_ != TeNoGrouping)
	{
		for(i = 0; i < bandComboBox->count(); ++i)
		{
			string s = bandComboBox->text(i).latin1();
			if(s == groupingStruct.groupAttribute_.name_)
			{
				bandComboBox->setCurrentItem(i);
				break;
			}
		}
	}
	else
		bandComboBox->setCurrentItem(0);

	// Check number of slices
	if (groupingStruct.groupMode_ != TeNoGrouping)
	{
		int slices = groupingStruct.groupNumSlices_;
		slicesComboBox->setCurrentItem(slices - 1);
	}
	else
		slicesComboBox->setCurrentItem(4);  // number of slices = 5

	//Set the group precision
	if (groupingStruct.groupMode_ == TeNoGrouping)
	{
		TeDataType dt = raster_->params().dataType_[0];
		if (dt == TeFLOAT)
			precisionComboBox->setCurrentItem(6);
		else if (dt == TeDOUBLE) 
			precisionComboBox->setCurrentItem(14);
		else
			precisionComboBox->setCurrentItem(0);
	}
	else
		precisionComboBox->setCurrentItem(groupingStruct.groupPrecision_ - 1);

	int precision_= precisionComboBox->currentItem();


	// Colors of the Grouping 
	string groupColor = theme_->groupColor();
	colorBarVec_ = getColorBarVector(groupColor, true);

	frameTeQtColorBar->setVerticalBar(false);
	frameTeQtColorBar->setColorBar(colorBarVec_);
	frameTeQtColorBar->drawColorBar();
	
	// vmin and vmax
	double vmin, vmax;
	rasterSlices_.clear();
	TeLegendEntryVector& legendVector = baseTheme->legend();
	if (legendVector.empty() == false && groupingStruct.groupMode_ != TeNoGrouping)
	{
		legendTable->setNumRows(legendVector.size());
		i=0;
		while (i < (int)legendVector.size())
		{
			TeLegendEntry& leg = legendVector[i];
			rasterSlices_.push_back(leg);
			++i;
		}
		vmax = atof(legendVector[i-1].slice().to_.c_str());
		vmin = atof(legendVector[0].slice().from_.c_str());
	}
	else
	{
		vmin = raster_->params().vmin_[0];
		vmax = raster_->params().vmax_[0];
	}

	string fmt = "%." + Te2String(precision_) + "f";
	QString txt;
	txt.sprintf(fmt.c_str(),vmin);
	minValueLineEdit->setText(txt);
	txt.sprintf(fmt.c_str(),vmax);
	maxValueLineEdit->setText(txt);
	maxValueLineEdit->setCursorPosition(0);
	minValueLineEdit->setCursorPosition(0);

	//************************ Legend Grid ****************************
	legendTable->setLeftMargin(0);
	legendTable->verticalHeader()->hide();
	legendTable->setNumCols(4);
	legendTable->horizontalHeader()->setLabel(0, tr("Color"));
	legendTable->horizontalHeader()->setLabel(1, tr("From"));
	legendTable->horizontalHeader()->setLabel(2, tr("To"));
	legendTable->horizontalHeader()->setLabel(3, tr("Label"));
	legendTable->adjustColumn(0);
	legendTable->adjustColumn(1);
	legendTable->adjustColumn(2);
	legendTable->adjustColumn(3);

	okPushButton->setEnabled(false);

	if (groupingStruct.groupMode_ != TeNoGrouping)
	{
		putColorOnLegend();
		showLegend();
		if (!rasterSlices_.empty())
			okPushButton->setEnabled(true);
	}
	importCheckBox->setChecked(false);
	importCheckBox_toggled(false);

	loadNamesComboBox->clear();
	loadNamesComboBox->insertItem("");
	TeDatabase* db = mainWindow_->currentDatabase();
	TeDatabasePortal* portal = db->getPortal();
	if(portal->query("SELECT name FROM tv_colorbar ORDER BY name"))
	{
		while(portal->fetchRow())
			loadNamesComboBox->insertItem(portal->getData(0));
	}
	delete portal;
	loadNamesComboBox->setCurrentItem(0);
	connect(frameTeQtColorBar, SIGNAL(colorChangedSignal()), this, SLOT(colorChangedSlot()));
}

void RasterSlicingWindow::slicingPushButton_clicked()
{
	rasterSlices_.clear();
	calculateSlices();
	putColorOnLegend();
	showLegend();
	if (!rasterSlices_.empty())
		okPushButton->setEnabled(true);
}


void RasterSlicingWindow::okPushButton_clicked()
{
	if(legendTable->numRows() == 0)
		return;

	if (!getEditChanges())
		return;

	unsigned int i;

	TeQtThemeItem* themeItem_ = (TeQtThemeItem*)mainWindow_->getViewsListView()->currentThemeItem();
	TeAppTheme* theme_ = (TeAppTheme*)themeItem_->getAppTheme();
	TeTheme* baseTheme = (TeTheme*)theme_->getTheme();
	baseTheme->deleteGrouping();				// remove previously defined grouping 
	themeItem_->removeLegends();			// remove previously defined legends

	TeDatabase* db_ = mainWindow_->currentDatabase();

	vector<ColorBar> cbVec;
	vector<ColorBar> inputColorVec = frameTeQtColorBar->getInputColorVec();
	string colors = getColors(inputColorVec, cbVec, TeEqualSteps);
	theme_->groupColor(colors);
	theme_->groupColorDir(TeColorAscSatBegin);

	TeAttributeRep rep;
	rep.type_ = TeREAL;
	rep.name_ = bandComboBox->currentText().latin1();
	TeGrouping slicing(rep,"",TeRasterSlicing,rasterSlices_.size());
	slicing.groupPrecision_ = precisionComboBox->currentItem();
	baseTheme->grouping(slicing);
	for (i=0; i<rasterSlices_.size(); i++)
	{
		rasterSlices_[i].theme(baseTheme->id());
		rasterSlices_[i].group(i);
		rasterSlices_[i].id(0); // to insert legend in the database
		baseTheme->legend().push_back(rasterSlices_[i]);
	}
	if (baseTheme->legend().size())
		baseTheme->visibleRep(baseTheme->visibleRep() | 0x40000000);
	else
		baseTheme->visibleRep(baseTheme->visibleRep() & 0xbfffffff);

	updateAppTheme(db_, theme_);
	rasterSlices_.clear();
	baseTheme->removeRasterVisual();

	// Update the theme item interface
	mainWindow_->updateThemeItemInterface(themeItem_);

	// Draw the legend on the canvas
	mainWindow_->drawAction_activated();
	accept();
}


void RasterSlicingWindow::drawPolygonRep( int w, int h, TeColor & color, QPixmap *pixmap )
{
	QPainter p(pixmap);
	QBrush	 brush;
	QColor	 cor;
	cor.setRgb(color.red_, color.green_, color.blue_);

	brush.setStyle(Qt::SolidPattern);
	brush.setColor(cor);

	QRect trect(1, 1, w-2, h-2);
	QRect rect(0, 0, w-1, h-1);

	int width = rect.width();
	int height = rect.height();

	int r = width%8;
	if(r)
		width += (8-r);
	r = height%8;
	if(r)
		height += (8-r);

	if(width == 0)
		width = 8;
	if(height == 0)
		height = 8;

	QBitmap	bm;
	bm.resize(width, height);
	//Fill bitmap with 0-bits: clipping region
	bm.fill(Qt::color0);
	QPainter maskPainter(&bm);

	// Draw bitmap with 1-bits: drawing region
	QBrush bs(Qt::color1, Qt::SolidPattern);
	QPen pen(Qt::color1, 1);
	maskPainter.setPen(pen);
 	maskPainter.fillRect(trect, bs);
	maskPainter.end();

	QRegion clipRegion(bm);
	p.setClipRegion(clipRegion);
	p.fillRect(trect, brush);
	p.setClipping(false);

	uint		pwidth=1;
	pen.setColor(cor);
	pen.setStyle(Qt::SolidLine);
	p.setPen(pen);
	p.drawRect (1+pwidth/2,1+pwidth/2,w-2-pwidth/2,h-2-pwidth/2);
	p.end();
}


bool RasterSlicingWindow::getEditChanges()
{
	int i;
	for(i=0; i<legendTable->numRows(); i++)
	{
		if ((legendTable->text(i,1).isEmpty() || legendTable->text(i,1).isNull() ||
			legendTable->text(i,2).isEmpty() || legendTable->text(i,2).isNull()) ||
			legendTable->text(i,1).toDouble() > legendTable->text(i,2).toDouble())
		{
			QString mess = tr("Bad defined slice: ");
			mess += i;
			QMessageBox::warning(this,tr("Warning"),mess);
			return false;
		}
		
		string s = legendTable->text(i,1).latin1();
		rasterSlices_[i].from(s);
		s = legendTable->text(i,2).latin1();
		rasterSlices_[i].to(s);
		s = legendTable->text(i,3).latin1();
		rasterSlices_[i].label(s);
	}
	return true;
}

void RasterSlicingWindow::calculateSlices()
{
	// calculate slices
	unsigned int nslices = slicesComboBox->currentText().toUInt();
	if (minValueLineEdit->text().isEmpty() || maxValueLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Inform min and max values to be used."));
		return;
	}

	// generate a legend to each slice	
	double vmin = minValueLineEdit->text().toDouble();
	double vmax = maxValueLineEdit->text().toDouble();
	int prec = precisionComboBox->currentText().toInt();
	vector<TeSlice> result;
	TeGroupByEqualStep(vmin,vmax,nslices,result,prec);
	for (unsigned int ns=0;ns<result.size();ns++)
	{
		TeLegendEntry leg(result[ns]);
		leg.group(ns+1);
		rasterSlices_.push_back(leg);
	}
}



void RasterSlicingWindow::precisionComboBox_activated( const QString & )
{
	bandComboBox_activated(bandComboBox->currentItem());
}


void RasterSlicingWindow::slicesComboBox_activated( const QString & )
{
	legendTable->setNumRows(0);
	rasterSlices_.clear();
	okPushButton->setEnabled(false);
}

void RasterSlicingWindow::bandComboBox_activated(int b)
{
	if (!raster_)
		return; 

	string fmt = "%." + Te2String(precisionComboBox->currentItem()+1) + "f";
	char txt[40];
	sprintf(txt,fmt.c_str(),raster_->params().vmin_[b]);
	minValueLineEdit->setText(QString(txt));
	sprintf(txt,fmt.c_str(),raster_->params().vmax_[b]);
	maxValueLineEdit->setText(QString(txt));
	maxValueLineEdit->setCursorPosition(0);
	minValueLineEdit->setCursorPosition(0);
	legendTable->setNumRows(0);
	rasterSlices_.clear();
	okPushButton->setEnabled(false);
}


void RasterSlicingWindow::putColorOnLegend()
{
	if (rasterSlices_.size() == 0)
		return;

	vector<TeColor> colorVec = getColors(colorBarVec_, rasterSlices_.size());
	for (unsigned int j = 0; j < rasterSlices_.size(); j++)
	{
		TeColor	cor = colorVec[j];
		TeVisual* v = (((TeTheme*)theme_->getTheme())->defaultLegend().visual(TePOLYGONS))->copy();	
		v->color(cor);
		rasterSlices_[j].getVisualMap()[TePOLYGONS] = v;
	}
	showLegend();
}


void RasterSlicingWindow::showLegend()
{
	if (rasterSlices_.size() == 0)
		return;

	unsigned int i;
	legendTable->setNumRows(0);
	legendTable->setNumRows(rasterSlices_.size());
	for (i = 0; i < rasterSlices_.size(); ++i)
	{
		QPixmap	pixmap(21, 16);
		pixmap.fill();
		drawPolygonRep(21, 16, rasterSlices_[i].getVisualMap()[TePOLYGONS]->color(), &pixmap);
		legendTable->setPixmap(i, 0, pixmap);
		legendTable->setText(i, 1, rasterSlices_[i].from().c_str());
		legendTable->setText(i, 2, rasterSlices_[i].to().c_str());
		legendTable->setText(i, 3, rasterSlices_[i].label().c_str());
	}
	legendTable->adjustColumn(1);
	legendTable->adjustColumn(2);
	legendTable->adjustColumn(3);
}


void RasterSlicingWindow::importPushButton_clicked()
{
	init();
	theme_ = mainWindow_->currentTheme();
	okPushButton->setEnabled(true);
	applyPushButton->setEnabled(true);
}


void RasterSlicingWindow::importFromViewComboBox_activated( const QString & s)
{
	if(s.isEmpty())
		return;

	string name = s.latin1();
	TeViewMap& viewMap = mainWindow_->currentDatabase()->viewMap();
	TeViewMap::iterator it;
	importView_ = 0;
	for (it = viewMap.begin(); it != viewMap.end(); ++it)
	{
		if(it->second->name() == name)
		{ 		
			importView_ = it->second;
			break;
		}
	}

	importFromThemeComboBox->clear();
	if(importView_)
	{
		TeQtViewItem* importViewItem = mainWindow_->getViewsListView()->getViewItem(importView_);
		vector<TeQtThemeItem*> themeItemVec = importViewItem->getThemeItemVec();
		TeTheme* baseTheme = (TeTheme*)theme_->getTheme();
		for (unsigned int i = 0; i < themeItemVec.size(); ++i)
		{
			TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
			TeTheme* theme = (TeTheme*)appTheme->getTheme();
			if (theme->id() != baseTheme->id() &&
				theme->grouping().groupMode_ == TeRasterSlicing)
					importFromThemeComboBox->insertItem(theme->name().c_str());
		}
	}

	if(importFromThemeComboBox->count())
	{
		importFromThemeComboBox->setCurrentItem(0);
		importFromThemeComboBox->setEnabled(true);
		importFromThemeComboBox_activated(importFromThemeComboBox->currentText());
	}
	else
	{
		importFromThemeComboBox->setEnabled(false);
		importPushButton->setEnabled(false);
	}
}


void RasterSlicingWindow::importFromThemeComboBox_activated( const QString & s)
{
	if(s.isEmpty())
		return;

	importTheme_ = 0;
	if(importView_)
	{
		TeQtViewItem* importViewItem = mainWindow_->getViewsListView()->getViewItem(importView_);
		vector<TeQtThemeItem*> themeItemVec = importViewItem->getThemeItemVec();
		TeTheme* baseTheme = (TeTheme*)theme_->getTheme();
		for (unsigned int i = 0; i < themeItemVec.size(); ++i)
		{
			TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
			TeTheme* theme = (TeTheme*)appTheme->getTheme();
			if (theme->id() != baseTheme->id() && 
				theme->grouping().groupMode_ == TeRasterSlicing &&
				theme->name() == s.latin1())
			{
				importTheme_ = appTheme;
				break;
			}
		}
	}
	if(importTheme_)
		importPushButton->setEnabled(true);
}


void RasterSlicingWindow::importCheckBox_toggled( bool v )
{
	importFromViewComboBox->clear();
	importFromThemeComboBox->clear();
	if (!v)
	{
		importCheckBox->setChecked(false);
		importFromViewComboBox->clear();
		importFromThemeComboBox->clear();
		importFromViewComboBox->setEnabled(false);
		importFromThemeComboBox->setEnabled(false);
		importPushButton->setEnabled(false);
		okPushButton->setEnabled(true);
		applyPushButton->setEnabled(true);
		return;
	}

	unsigned int i, j;
	TeTheme* curTheme = (TeTheme*)theme_->getTheme();
	vector<TeQtViewItem*> viewItemVec = mainWindow_->getViewsListView()->getViewItemVec();
	for (i = 0; i < viewItemVec.size(); ++i)
	{
		TeView* view = viewItemVec[i]->getView();
		vector<TeQtThemeItem*> themeItemVec = viewItemVec[i]->getThemeItemVec();
		for (j = 0; j < themeItemVec.size(); ++j)
		{
			TeTheme* theme = (TeTheme*)(themeItemVec[j]->getAppTheme())->getTheme();
			if (theme->grouping().groupMode_ == TeRasterSlicing &&	theme->id() != curTheme->id())
			{
				importFromViewComboBox->insertItem(view->name().c_str());
				break;
			}				
		}
	}

	if(importFromViewComboBox->count())
	{
		importFromViewComboBox->setCurrentItem(0);
		importFromViewComboBox->setEnabled(true);
		importFromViewComboBox_activated(importFromViewComboBox->currentText());
		okPushButton->setEnabled(false);
		applyPushButton->setEnabled(false);
	}
	else
	{
		QMessageBox::information (this, tr("Import Theme Grouping"),
			tr("There are no themes with raster slicing to be imported!"));
		importCheckBox->setChecked(false);
		importCheckBox_toggled(false);
	}
}


void RasterSlicingWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("rasterSlicingWindow.htm");
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


void RasterSlicingWindow::clearColorsPushButton_clicked()
{
	frameTeQtColorBar->clearColorBar();
}


void RasterSlicingWindow::invertColorsPushButton_clicked()
{
	frameTeQtColorBar->invertColorBar();
}


void RasterSlicingWindow::equalSpacePushButton_clicked()
{
	frameTeQtColorBar->setEqualSpace();
}


void RasterSlicingWindow::saveColorPushButton_clicked()
{
	SaveAndLoadColorBarWindow* w = new SaveAndLoadColorBarWindow(this, "SaveAndLoadColorBarWindow", true);

	vector<ColorBar> cbVec;
	vector<ColorBar> inputColorVec = frameTeQtColorBar->getInputColorVec();	
	string colors = getColors(inputColorVec, cbVec, TeEqualSteps);
	w->setColor(colors.c_str());
	w->exec();
	QString item = w->getColorName();
	delete w;
	loadNamesComboBox->clear();
	loadNamesComboBox->insertItem("");
	TeDatabasePortal* portal = mainWindow_->currentDatabase()->getPortal();
	if(portal->query("SELECT name FROM tv_colorbar ORDER BY name"))
	{
		while(portal->fetchRow())
			loadNamesComboBox->insertItem(portal->getData(0));
	}
	delete portal;
	loadNamesComboBox->setCurrentText(item);
}

void RasterSlicingWindow::colorChangedSlot()
{
	colorBarVec_ = frameTeQtColorBar->getInputColorVec();
	putColorOnLegend();
}

void RasterSlicingWindow::loadNamesComboBox_activated( int )
{
	QString colorBarName = loadNamesComboBox->currentText();
	if(colorBarName.isEmpty())
		return;

	string color_;
	TeDatabasePortal* portal = mainWindow_->currentDatabase()->getPortal();
	string name =  colorBarName.latin1();
	string s = "SELECT colorbar FROM tv_colorbar WHERE name = '" + name + "'";
	if(portal->query(s))
	{
		if(portal->fetchRow())
			color_ = portal->getData(0);
	}
	delete portal;
	frameTeQtColorBar->setColorBar(getColorBarVector(color_, true));
	frameTeQtColorBar->drawColorBar();

	colorChangedSlot();
}


void RasterSlicingWindow::legendTable_valueChanged( int row, int col )
{
	if (row < (int)rasterSlices_.size()&& (col ==1 || col == 2) )
	{
		QString label;
		label = legendTable->text(row,1);
		label += " ~ ";
		label += legendTable->text(row,2);
		legendTable->setText(row,3,label);
	}
}


void RasterSlicingWindow::legendTable_doubleClicked( int row, int col, int, const QPoint & )
{
	if (row < (int)rasterSlices_.size() && col == 0)
	{
		QColor	qc = QColorDialog::getColor();
		if (qc.isValid())
		{
			TeColor cor;
			cor.init(qc.red(), qc.green(), qc.blue());
			QPixmap	pixmap(21, 16);
			pixmap.fill();
			drawPolygonRep(21, 16, cor, &pixmap);
			legendTable->setPixmap(row, 0, pixmap);
			rasterSlices_[row].getVisualMap()[TePOLYGONS]->color(cor);
		}	
	}
}
