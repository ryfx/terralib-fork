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
#include <TeQtViewsListView.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <TeQtChartItem.h>
#include <TeKernelParams.h>
#include <TeQtTerraStat.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qbuttongroup.h>
#include <graphic.h>
#include <qstatusbar.h>
#include <insertText.h>
#include <qlineedit.h>
#include <visual.h>
#include <insertPoint.h>
#include <check.xpm>
#include <uncheck.xpm>
#include <animation.h>
#include <insertSymbol.h>
#include <qpaintdevice.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <geographicalGrid.h>
#include <float.h>
#include <qpaintdevicemetrics.h>
#include <TeQtLegendItem.h>
#include <displaySize.h>
#include <TeColorUtils.h>

#include <addressLocatorWindow.h>
#include <graphic3DWindow.h>

#include <PluginsSignal.h>

void DisplayWindow::init()
{
	graphicScaleVisible_ = false;
	graphicScaleOffsetX_ = 0.;
	graphicScaleOffsetY_ = 0.;
	displaySelection_ = false;
	hGraphCursor_ = 60;
	wGraphCursor_ = 100;
	circleRadius_ = 50;
	spatialCursorAngle_ = 0.;
	drawSetOnCenter_ = true;
	zoomFactor_ = 0.5;
	contrastPixmap_ = 0;
	connectedPixmap_ = 0;
	symbolSize_ = 12;
	symbolFamily_ = "Verdana";
	symbolFixedSize_ = false;
	symbolColor_ = QColor(255, 0, 255);
	connectedRect_ = QRect(0, 0 , 128, 128);
	isSGUnitSet_ = false;
	SGUnit_= 2;

	QWidget *ws = (QWidget*)parent();
	mainWindow_ = (TerraViewBase*)ws->parent();

	canvas_ = new TeQtCanvas(this);
	canvas_->installEventFilter(this);

	popupCanvas_ = canvas_->popupCanvas();
	
	QPopupMenu *popupVisual = new QPopupMenu;
	popupCanvas_->insertItem(tr("Visual"), popupVisual);
	popupVisual->insertItem(tr("Default..."), this, SLOT(setDefaultVisual()));
	popupVisual->insertItem(tr("Pointing..."), this, SLOT(setPointingVisual()));
	popupVisual->insertItem(tr("Query..."), this, SLOT(setQueryVisual()));
	popupVisual->insertItem(tr("Pointing and Query..."), this, SLOT(setPointingQueryVisual()));
	popupVisual->insertSeparator();
	popupVisual->insertItem(tr("Default Visual of the Text..."), mainWindow_, SLOT(textVisual()));

	popupOwnVisual_ = new QPopupMenu;
	popupCanvas_->insertItem(tr("Own Visual"), popupOwnVisual_);
	popupOwnVisual_->insertItem(tr("Set the Own Visual of the Object..."), this, SLOT(ownVisualSlot()));
	popupOwnVisual_->insertItem(tr("Remove the Own Visual of the Object"), this, SLOT(removeOwnVisualSlot()));
	popupOwnVisual_->insertItem(tr("Remove the Own Visual of All of the Objects"), this, SLOT(removeAllOwnVisualsSlot()));

	popupCanvas_->insertItem(tr("Change Legend Visual..."), this, SLOT(changeLegendVisualSlot()));
	popupCanvas_->insertSeparator();

	QPopupMenu *popupMidias = new QPopupMenu;
	popupCanvas_->insertItem(tr("Medias"), popupMidias);
	popupMidias->insertItem(tr("Show Default Media..."), mainWindow_, SLOT(popupShowDefaultMediaSlot()));
	popupMidias->insertSeparator();
	popupMidias->insertItem(tr("Insert Media..."), mainWindow_, SLOT(popupInsertMediaSlot()));
	popupMidias->insertItem(tr("Insert URL..."), mainWindow_, SLOT(popupInsertURLSlot()));
	popupMidias->insertSeparator();
	popupMidias->insertItem(tr("Default Media Description..."), mainWindow_, SLOT(popupDefaultMediaDescriptionSlot()));
	popupMidias->insertItem(tr("Attributes and Media..."), mainWindow_, SLOT(popupAttributesAndMediaSlot()));
	popupCanvas_->insertSeparator();
		
	QPopupMenu *popupSpatialOperation = new QPopupMenu;
	popupCanvas_->insertItem(tr("Spatial Operation"), popupSpatialOperation);

	popupSpatialCursorType_ = new QPopupMenu;
	popupCanvas_->insertItem(tr("Spatial Cursor Type"), popupSpatialCursorType_);
	popupSpatialCursorType_->insertItem(tr("Rectangle"), this, SLOT(setSquareCursorTypeSlot()));
	popupSpatialCursorType_->insertItem(tr("Ellipse"), this, SLOT(setCircleCursorTypeSlot()));
	popupSpatialCursorType_->setItemChecked(popupSpatialCursorType_->idAt(0), true);
	popupCanvas_->insertItem(tr("Automatic Promotion on the Grid"), mainWindow_, SLOT(autoPromoteSlot()));

	popupCanvas_->insertSeparator();
	QPopupMenu *popupPrint = new QPopupMenu;
	popupCanvas_->insertItem(tr("Save Display"), popupPrint);
	popupPrint->insertItem(tr("Printer..."), mainWindow_, SLOT(saveDisplayWithPrinterAction_activated()));
	popupPrint->insertItem(tr("File..."), mainWindow_, SLOT(saveDisplayAsFileAction_activated()));

	popupCanvas_->insertSeparator();
	popupCanvas_->insertItem(tr("Graphic Scale Visual..."), this, SLOT(popupGraphicScaleVisualSlot()));
	popupCanvas_->insertItem(tr("Show Graphic Scale"), this, SLOT(showGraphicScaleSlot()));
	popupGraphicScaleUnit_ = new QPopupMenu;
	popupGraphicScaleUnit_->insertItem(tr("Decimal Degrees"), this, SLOT(setGraphicScaleUnit_DecimalDegreesSlot()));
	popupGraphicScaleUnit_->insertItem(tr("Kilometers"), this, SLOT(setGraphicScaleUnit_KilometersSlot()));
	popupGraphicScaleUnit_->insertItem(tr("Meters"), this, SLOT(setGraphicScaleUnit_MetersSlot()));
	popupGraphicScaleUnit_->insertItem(tr("Feets"), this, SLOT(setGraphicScaleUnit_FeetsSlot()));
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(0), true);
	popupCanvas_->insertItem(tr("Graphic Scale Units"), popupGraphicScaleUnit_);
	popupCanvas_->insertSeparator();
	popupCanvas_->insertItem(tr("Geographical Grid Visual..."), this, SLOT(popupGeographicalCoordinatesVisualSlot()));
	popupCanvas_->insertItem(tr("Show Gegraphical coordinates"), this, SLOT(showGeographicalCoordinatesSlot()));

	popupCanvas_->insertSeparator();
	popupCanvas_->insertItem(tr("Visibility of the Representations..."), mainWindow_, SLOT(setCurrentThemeVisibility()));

	popupCanvas_->insertSeparator();
	popupCanvas_->insertItem(tr("Insert Symbol..."), this, SLOT(popupInsertSymbolSlot()));
	popupCanvas_->insertItem(tr("Insert Text..."), this, SLOT(insertTextSlot()));
	popupCanvas_->insertItem(tr("Edit Text..."), this, SLOT(editTextSlot()));
	popupCanvas_->insertItem(tr("Visual of the Text..."), this, SLOT(textEditVisualSlot()));

	popupCanvas_->insertSeparator();
	popupCanvas_->insertItem(tr("Insert Point..."), this, SLOT(popupInsertPointSlot()));
	popupCanvas_->insertItem(tr("Delete Pointed Objects..."), this, SLOT(popupDeletePointedObjectsSlot())); // remove pointed points

	popupCanvas_->insertSeparator();
	popupCanvas_->insertItem(tr("Centralize the Drawing of the Selected Objects"), this, SLOT(popupDrawSelectedOnCenterSlot()));
	popupCanvas_->insertSeparator();
	popupCanvas_->insertItem(tr("Animation..."), this, SLOT(popupAnimationSlot()));
	popupCanvas_->insertSeparator();
	popupCanvas_->insertItem(tr("Remove Raster Contrast..."), this, SLOT(removeRasterContrastSlot()));
	popupCanvas_->insertSeparator();
	popupCanvas_->insertItem(tr("Display Size..."), this, SLOT(displaySizeSlot()));

	popupZoom_ = new QPopupMenu;
	popupZoom_->insertItem(tr(".25x"), this, SLOT(changeZoomFactorSlot(int)), 0, 0);
	popupZoom_->insertItem(tr(".5x"), this, SLOT(changeZoomFactorSlot(int)), 0, 1);
	popupZoom_->insertItem(tr(".75x"), this, SLOT(changeZoomFactorSlot(int)), 0, 2);

	popupZoom_->setItemChecked(popupZoom_->idAt(1), true);

    QGridLayout *spatialOperationLayout = new QGridLayout( popupSpatialOperation, 1, 1, 2, 2, "spatialOperationLayout"); 

    QButtonGroup *spatialOperationButtonGroup = new QButtonGroup( popupSpatialOperation, "spatialOperationButtonGroup" );
    spatialOperationButtonGroup->setColumnLayout(0, Qt::Vertical );
    spatialOperationButtonGroup->layout()->setSpacing( 6 );
    spatialOperationButtonGroup->layout()->setMargin( 11 );

    QGridLayout *spatialOperationButtonGroupLayout = new QGridLayout( spatialOperationButtonGroup->layout() );
    spatialOperationButtonGroupLayout->setAlignment( Qt::AlignTop );

    QGridLayout *layout = new QGridLayout( 0, 1, 1, 0, 4, "layout"); 

	QRadioButton *disjointRadioButton = new QRadioButton( spatialOperationButtonGroup, "disjointRadioButton" );
    layout->addWidget( disjointRadioButton, 0, 0 );

    QRadioButton *crossesRadioButton = new QRadioButton( spatialOperationButtonGroup, "crossesRadioButton" );
    layout->addWidget( crossesRadioButton, 1, 0 );

    QRadioButton *whithinRadioButton = new QRadioButton( spatialOperationButtonGroup, "whithinRadioButton" );
    layout->addWidget( whithinRadioButton, 2, 0 );

    QRadioButton *intersectsRadioButton = new QRadioButton( spatialOperationButtonGroup, "intersectsRadioButton" );
    layout->addWidget( intersectsRadioButton, 3, 0 );

    QRadioButton *noSpatialOperationRadioButton = new QRadioButton( spatialOperationButtonGroup, "noSpatialOperationRadioButton" );
    layout->addWidget( noSpatialOperationRadioButton, 4, 0 );
	spatialOperationButtonGroupLayout->addLayout( layout, 0, 0 );

    spatialOperationLayout->addWidget( spatialOperationButtonGroup, 0, 0 );
    
    disjointRadioButton->setText( tr( "Disjoint" ) );
    crossesRadioButton->setText( tr( "Crosses" ) );
    whithinRadioButton->setText( tr( "Within" ) );
    intersectsRadioButton->setText( tr( "Intersects" ) );
    noSpatialOperationRadioButton->setText( tr( "No Spatial Operation" ) );

    popupSpatialOperation->resize( QSize(123, 305).expandedTo(popupSpatialOperation->minimumSizeHint()) );
    clearWState( WState_Polished );

	popupSpatialOperation->insertItem(spatialOperationButtonGroup);

	// signals and slots connections
    connect( spatialOperationButtonGroup, SIGNAL( clicked(int) ), this, SLOT( spatialOperationButtonGroup_clicked(int) ) );

    // tab order
	setTabOrder( disjointRadioButton, crossesRadioButton );
    setTabOrder( crossesRadioButton, whithinRadioButton );
    setTabOrder( whithinRadioButton, intersectsRadioButton );
    setTabOrder( intersectsRadioButton, noSpatialOperationRadioButton );

	connect(canvas_, SIGNAL(mousePressed(TeCoord2D&, int, QPoint&)),
		this, SLOT(mousePressedOnCanvas(TeCoord2D&, int, QPoint&)));
	connect(canvas_, SIGNAL(mouseRightButtonPressed(TeCoord2D&, int, QPoint&)),
		this, SLOT(mouseRightButtonPressedOnCanvas(TeCoord2D&, int, QPoint&)));
	connect(canvas_, SIGNAL(mouseMoved(TeCoord2D&, int, QPoint&)),
		this, SLOT(mouseMovedOnCanvas(TeCoord2D&, int, QPoint&)));
	connect(canvas_, SIGNAL(mouseReleased(TeCoord2D&, int, QPoint&)),
		this, SLOT(mouseReleasedOnCanvas(TeCoord2D&, int, QPoint&)));
	connect(canvas_, SIGNAL(mouseDoublePressed(TeCoord2D&, int, QPoint&)),
		this, SLOT(mouseDoublePressedOnCanvas(TeCoord2D&, int, QPoint&)));
	connect(canvas_, SIGNAL(mouseLeave()),
		this, SLOT(mouseLeaveCanvas()));
	connect(canvas_, SIGNAL(popupCanvasSignal(QMouseEvent*)),
		this, SLOT(popupCanvasSlot(QMouseEvent*)));
	connect(canvas_, SIGNAL(keyPressed(QKeyEvent*)),
		this, SLOT(keyPressedOnCanvas(QKeyEvent*)));
	connect(canvas_, SIGNAL(keyReleased(QKeyEvent*)),
		this, SLOT(keyReleasedOnCanvas(QKeyEvent*)));
	connect(canvas_, SIGNAL(paintEvent()),
		this, SLOT(paintEventSlot()));

	intersectsRadioButton->setChecked(true);
	spatialOperation_ = TeINTERSECTS;
}


TeQtCanvas* DisplayWindow::getCanvas()
{
	 return canvas_;
}


void DisplayWindow::resizeEvent(QResizeEvent *)
{
	canvas_->resize(width(), height());
}


void DisplayWindow::closeEvent(QCloseEvent *)
{
	mainWindow_->displayAction->setOn(false);
}


void DisplayWindow::mousePressedOnCanvas(TeCoord2D& pt, int state, QPoint& m)
{
	canvas_->plotOnPixmap0();
	QPoint offset;
	if(state == Qt::RightButton || state == Qt::MidButton)
		return;

	if((mainWindow_->currentView() == 0) || (mainWindow_->currentTheme() == 0))
		return;

	if(canvas_->getWorld().isValid() == false)
		return;

	TeAppTheme* appTheme = mainWindow_->currentTheme();
	TeAbstractTheme* absTheme = appTheme->getTheme();
	TeProjection* themeProjection = 0;
	if (absTheme)
		themeProjection = absTheme->getThemeProjection();
	else 
		return;

	TeCoord2D pdw;
	double	resolution;
	double	delta;

	if(canvas_->projection())
	{
		if(themeProjection && !(*themeProjection == *(canvas_->projection())))
		{
			resolution = canvas_->pixelSize();
			TeCoord2D pl(pt.x()-resolution/2., pt.y()-resolution/2.);
			TeCoord2D pu(pt.x()+resolution/2., pt.y()+resolution/2.);
			TeBox	box(pl, pu);
			box = TeRemapBox(box, canvas_->projection(), themeProjection);
			resolution = box.x2_ - box.x1_;
			canvas_->projection()->setDestinationProjection(themeProjection);
			pdw = canvas_->projection()->PC2LL(pt);
			pdw = themeProjection->LL2PC(pdw);
			delta = 3.*resolution;
		}
		else
		{
			delta = 3.*canvas_->pixelSize();
			pdw = pt;
		}
	}
	else
	{
		delta = 3.*canvas_->pixelSize();
		pdw = pt;
	}

	// Pan, zooming In and Zooming Out
	if (canvas_->getCursorMode() == TeQtCanvas::Pan)
		panStart_ = pt;
	else if (canvas_->getCursorMode() == TeQtCanvas::ZoomIn)
	{
		TeWaitCursor wait;
		TeBox box;
	
		box = canvas_->getWorld();
		TeCoord2D c = box.center();
		double dx = pt.x() - c.x();
		double dy = pt.y() - c.y();
		box.x1_ += dx;
		box.x2_ += dx;
		box.y1_ += dy;
		box.y2_ += dy;
		zoomIn(box, zoomFactor_);	
		mainWindow_->updateBoxesVector(box);
		canvas_->setWorld(box, mainWindow_->cvContentsW_, mainWindow_->cvContentsH_);
		plotData();
		int hmin = canvas_->horizontalScrollBar()->minValue();
		int hmax = canvas_->horizontalScrollBar()->maxValue();
		canvas_->horizontalScrollBar()->setValue((hmax-hmin)/2);
		int vmin = canvas_->verticalScrollBar()->minValue();
		int vmax = canvas_->verticalScrollBar()->maxValue();
		canvas_->verticalScrollBar()->setValue((vmax-vmin)/2);
	}
	else if (canvas_->getCursorMode() == TeQtCanvas::ZoomOut)
	{
		TeWaitCursor wait;
		TeBox box;
	
		box = canvas_->getWorld();
		TeCoord2D c = box.center();
		double dx = pt.x() - c.x();
		double dy = pt.y() - c.y();
		box.x1_ += dx;
		box.x2_ += dx;
		box.y1_ += dy;
		box.y2_ += dy;
		zoomOut(box, zoomFactor_);	
		mainWindow_->updateBoxesVector(box);
		canvas_->setWorld(box, mainWindow_->cvContentsW_, mainWindow_->cvContentsH_);
		plotData();
		int hmin = canvas_->horizontalScrollBar()->minValue();
		int hmax = canvas_->horizontalScrollBar()->maxValue();
		canvas_->horizontalScrollBar()->setValue((hmax-hmin)/2);
		int vmin = canvas_->verticalScrollBar()->minValue();
		int vmax = canvas_->verticalScrollBar()->maxValue();
		canvas_->verticalScrollBar()->setValue((vmax-vmin)/2);
	}

	editOn_ = mainWindow_->editionIsActivated();
	graphCursorOn_ = mainWindow_->graphicCursorIsActivated();
	set<string> objectIdSet, uniqueIdSet;

	TeTheme* theme = 0;
	TeLayer* layer = 0;
	if (absTheme->type() == TeTHEME || absTheme->type() == TeEXTERNALTHEME)
	{
		theme = appTheme->getSourceTheme();
		if (theme == 0)
			return;
		layer = theme->layer();
	}

	bool traster = false;
	if (absTheme->visibleGeoRep() & TeRASTER || absTheme->visibleGeoRep() & TeRASTERFILE)
		traster = true;

	bool bb = false;
	if (!traster && (canvas_->getCursorMode() == TeQtCanvas::Pointer) || 
	    (canvas_->getCursorMode() == TeQtCanvas::Information))
	{
		if(graphCursorOn_ == false) // do pointing selection
		{
			if(absTheme->type()>= 3)
			{
				if (canvas_->getCursorMode() == TeQtCanvas::Information)
				{
					PluginsSignal sigrefresh(PluginsSignal::S_POINT_QUERY);
					sigrefresh.signalData_.store(PluginsSignal::D_QT_CANVAS, canvas_);
					sigrefresh.signalData_.store(PluginsSignal::D_ABSTRACT_THEME, absTheme);
					sigrefresh.signalData_.store(PluginsSignal::D_COORD2D, pt);
					PluginsEmitter::instance().send(sigrefresh);
				}
				return;
			}

			string pointingMode = "togglePointing";
			if(layer->hasGeometry(TePOINTS))
			{
				TePoint point;
				if(bb = theme->locatePoint(pdw, point, delta))
					objectIdSet.insert(point.objectId());
			}
			if(bb==false && layer->hasGeometry(TeLINES))
			{
				TeLine2D line;
				if(bb = theme->locateLine(pdw, line, delta))
					objectIdSet.insert(line.objectId());
			}
			if(bb==false && layer->hasGeometry(TeCELLS))
			{
				TeCell cel;
				if(bb = theme->locateCell(pdw, cel, 0.))
					objectIdSet.insert(cel.objectId());
			}
			if(bb==false && layer->hasGeometry(TePOLYGONS))
			{
				TePolygon polygon;
				if(bb = theme->locatePolygon(pdw, polygon, 0.))
					objectIdSet.insert(polygon.objectId());
			}
			if (canvas_->getCursorMode() == TeQtCanvas::Information)
			{
				if(!traster)
				{
					string obj;
					set<string>::iterator it = objectIdSet.begin();
					if(it != objectIdSet.end())
					{
						obj = (*it);

						mainWindow_->showObjectInformation(obj);

						// blink object on display
						mainWindow_->getGrid()->putColorOnGrid(uniqueIdSet, objectIdSet, pointingMode);
						mainWindow_->getGrid()->putColorOnGrid(uniqueIdSet, objectIdSet, pointingMode);
					}
				}
				else
					rasterDataToStatusBar(layer, pdw);
			}
			else if(!traster)
				mainWindow_->getGrid()->putColorOnGrid(uniqueIdSet, objectIdSet, pointingMode);
		}
		else if(graphCursorOn_) // do spatial selection
		{
			if (appTheme->getTheme()->getProductId() == TeEXTERNALTHEME || 
				appTheme->getTheme()->getProductId() == TeTHEME)
			{
				TeLayer* layer  = 0;
				TeTheme* theme = (TeTheme*)appTheme->getTheme();
				
				if(theme->type() == TeTHEME)
					layer = theme->layer();
				else
					layer = ((TeExternalTheme*)theme)->getRemoteTheme()->layer();

				int i;
				if(m != cGraphCursor_) // return if canvasPopup (menu popup) is visible
					return;

				if(xorPointArrayVec_.size() == 0)
					return;

				TeWaitCursor wait;
				vector<QPointArray> pVec = rotateXorPoints();
				QPointArray pa = pVec[0];
				TeLine2D line;
				TeCoord2D p;
				for(i=0; i < (int)pa.count(); i++)
				{
					p = canvas_->mapVtoDW(pa.point(i));
					line.add(p);
				}
				TeLinearRing ring(line);
				TePolygon poly;
				poly.add ( ring );

				set<string> objectIdSet;

				TePrecision::instance().setPrecision(TeGetPrecision(layer->projection()));

				TeDatabase* dba = layer->database();
				TeDatabasePortal* portal = dba->getPortal();

				if(theme->layer()->hasGeometry(TePOINTS)
					&& dba->spatialRelation(layer->tableName(TePOINTS),  
					TePOINTS, (TePolygon*)&poly, portal, spatialOperation_, theme->collectionTable()))
				{
					do
						objectIdSet.insert(portal->getData(1));
					while (portal->fetchRow());
				}
				portal->freeResult();
				if(bb==false && layer->hasGeometry(TeLINES)
					&& dba->spatialRelation(layer->tableName(TeLINES), 
						TeLINES, (TePolygon*)&poly, portal, spatialOperation_, theme->collectionTable()))
				{
					do
						objectIdSet.insert(portal->getData(1));
					while (portal->fetchRow());
				}
				portal->freeResult();
				if(bb==false && layer->hasGeometry(TePOLYGONS) 
					&& dba->spatialRelation(layer->tableName(TePOLYGONS), 
						TePOLYGONS, (TePolygon*)&poly, portal, spatialOperation_, theme->collectionTable()))
				{
					do
						objectIdSet.insert(portal->getData(1));
					while (portal->fetchRow());
				}
				portal->freeResult();
				if(bb==false && layer->hasGeometry(TeCELLS) 
					&& dba->spatialRelation(layer->tableName(TeCELLS), 
						TeCELLS, (TePolygon*)&poly, portal, spatialOperation_, theme->collectionTable()))
				{
					do
						objectIdSet.insert(portal->getData(1));
					while (portal->fetchRow());
				}
				delete portal;
				plotXor(false);
				displaySelection_ = true;
				if (!traster)
				{
					if(state == 0) // no key or Shift key ... new
						mainWindow_->getGrid()->putColorOnGrid(uniqueIdSet, objectIdSet, "newPointing");
					if(state == Qt::ShiftButton) // Shift key ... add
						mainWindow_->getGrid()->putColorOnGrid(uniqueIdSet, objectIdSet, "addPointing");
				}
				displaySelection_ = false;
			}
			else
				QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		}
	}
	else if(canvas_->getCursorMode() == TeQtCanvas::Edit) // find text for edition
	{
		if(appTheme->getTheme()->type() == TeTHEME)
		{
			TeText text;
			bb = appTheme->locateText(pdw, text, delta);
			if (bb == true)
			{
				appTheme->editRep(TeTEXT); // edit text
				if((state & Qt::ShiftButton) == 0)
					mtEdit_.endEdit();

				TeQtTextEdit tedit;
				TeVisual visual = appTheme->getTextVisual(text);
				tedit.init(text, visual, canvas_);
				mtEdit_.push(tedit);
			}
		}
		else
			QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
	}
	else if(canvas_->getCursorMode() == TeQtCanvas::Distance) // distance meter
	{
		QPoint c = m; 
		if(xorPointArrayVec_.size() == 0)
		{
			QPointArray par(2);
			par.setPoint(0, c);
			par.setPoint(1, c);
			xorPointArrayVec_.push_back(par);
		}
		else
		{
			plotXor(false);
			xorPointArrayVec_.clear();

			mainWindow_->statusBar()->message("");
		}
	}
	else if(canvas_->getCursorMode() == TeQtCanvas::Hand)
	{
		if(appTheme->getTheme()->type() == TeTHEME)
		{
			if(appTheme->editRep() == (TeGeomRep)0x20000000) // init legend movement
			{
				plotXor(false);
				xorMtEdit_.endEdit();
				mtEdit_.endEdit();
				vector<string> s;
				rectLegendOnCanvas_ = getRectLegendOnCanvas(appTheme, s);
				pLeg_ = TeCoord2D((double)m.x(), (double)m.y());
			}
			else if(appTheme->editRep() == (TeGeomRep)0x80000000) // init move piechart and barchart
			{
				plotXor(false);
				xorMtEdit_.endEdit();
				mtEdit_.endEdit();
				appTheme->chartPoint_ = pdw;
				TeBox box = getPieBarBox(appTheme);
				if(box.isValid() == false)
				{
					appTheme->chartSelected_.clear();
					pieBarBoxEdit_ = TeBox();
				}
				else
					pieBarBoxEdit_ = box;

				box = pieBarBoxEdit_;
				QPaintDevice* dev = canvas_->getPainter()->device();
				canvas_->plotOnPixmap0();
				canvas_->mapDWtoV(box);
				if(dev)
				{
					canvas_->getPainter()->end();
					canvas_->getPainter()->begin(dev);
				}
			}
			else if(appTheme->editRep() == (TeGeomRep)0x4) // init move TePoint
			{
				TeCoord2D p = appTheme->tePoint_.location();
				double d = 3 * delta;
				pointBoxEdit_ = TeBox(p.x()-d, p.y()-d, p.x()+d, p.y()+d);
				plotXor(false);
				xorMtEdit_.endEdit();
				mtEdit_.endEdit();
			}
			else if(appTheme->editRep() == 0x10000000) // init move graphic scale
			{
				plotXor(false);
				xorMtEdit_.endEdit();
				mtEdit_.endEdit();
				pGraphicScale_ = m;
			}
		}
		else
		{
			if(appTheme->editRep() == 0x10000000) // init move graphic scale
			{
				plotXor(false);
				xorMtEdit_.endEdit();
				mtEdit_.endEdit();
				pGraphicScale_ = m;
			}
		}
	}
}


void DisplayWindow::mouseRightButtonPressedOnCanvas(TeCoord2D &pt, int, QPoint&)
{
	rightButtonPressed_ = pt;
}


void DisplayWindow::mouseMovedOnCanvas(TeCoord2D &pt, int state, QPoint& m)
{
	if(state == Qt::RightButton || state == Qt::MidButton)
		return;

	if(mainWindow_->currentView() == 0 || canvas_->getWorld().isValid() == false)
		return;	
	
	editOn_ = mainWindow_->editionIsActivated();
	graphCursorOn_ = mainWindow_->graphicCursorIsActivated();

	if(graphCursorOn_ == false && mainWindow_->currentView()->connectedId() != 0 &&
		mainWindow_->getTooltip().empty() && 
		canvas_->getCursorMode() == TeQtCanvas::Pointer)
	{
		canvas_->plotOnWindow();
		QPaintDevice *device = canvas_->viewport();
		canvas_->viewport()->setFocus(); // to receive keyboard events
		QPoint of(canvas_->contentsX (), canvas_->contentsY ());
		QPoint p = m;
		QRect rect(connectedRect_);
		if(state == Qt::AltButton)
		{
			QPoint c = rect.center();
			int dx = p.x() - c.x();
			int dy = p.y() - c.y();
			int w = rect.width() + dx;
			if(w < 4)
				w = 4;
			int h = rect.height() + dy;
			if(h < 4)
				h = 4;

			rect.setWidth(w);
			rect.setHeight(h);
		}
		rect.moveCenter(p);
		int sx = rect.left();
		int sy = rect.top();
		int ix = sx + of.x();
		int iy = sy + of.y();
		int w = rect.width();
		int h = rect.height();

		restoreConnectedRect(rect);
		bitBlt (device,sx,sy,connectedPixmap_,ix,iy,w,h,Qt::CopyROP,true);
		canvas_->plotOnWindow();
		canvas_->setLineColor (255, 0, 0);
		canvas_->setLineStyle (TeLnTypeContinuous);
		canvas_->plotRect(rect);

		connectedRect_ = rect;
	}

	if (!mainWindow_->currentDatabase())
		return;

	TeAbstractTheme* absTheme = 0;
	TeProjection* themeProjection = 0;
	TeAppTheme* appTheme = mainWindow_->currentTheme();
	if (appTheme)
	{
		absTheme = appTheme->getTheme();
		themeProjection = absTheme->getThemeProjection();
	}
	else
		return;

	TeCoord2D pdw, pan;
	double	resolution;
	double	delta;
	if(canvas_->projection())
	{
		if(themeProjection && !(*(themeProjection) == *(canvas_->projection())))
		{
			resolution = canvas_->pixelSize();
			TeCoord2D pl(pt.x()-resolution/2., pt.y()-resolution/2.);
			TeCoord2D pu(pt.x()+resolution/2., pt.y()+resolution/2.);
			TeBox	box(pl, pu);
			box = TeRemapBox(box, canvas_->projection(), themeProjection);
			resolution = box.x2_ - box.x1_;
			canvas_->projection()->setDestinationProjection(themeProjection);
			pdw = canvas_->projection()->PC2LL(pt);
			pdw = themeProjection->LL2PC(pdw);
			pan = canvas_->projection()->PC2LL(panStart_);
			pan = themeProjection->LL2PC(pan);
			delta = 3.*resolution;
		}
		else
		{
			delta = 3.*canvas_->pixelSize();
			pdw = pt;
			pan = panStart_;
		}
	}
	else
	{
		delta = 3.*canvas_->pixelSize();
		pdw = pt;
		pan = panStart_;
	}

	if(canvas_->getCursorMode() == TeQtCanvas::Pan && state == Qt::LeftButton)
	{
		if((state & Qt::LeftButton)) // mouse drag
		{
			canvas_->plotOnPixmap0();
			QPoint qps = canvas_->mapDWtoV(pan);
			QPoint qpa = canvas_->mapDWtoV(pdw);
			int x = qps.x() - qpa.x();
			int y = qps.y() - qpa.y();
			canvas_->copyPanArea(x, y);
		}
	}

	TeTheme* theme = 0;
	TeLayer* layer = 0;
	string TC;
	if (absTheme && (absTheme->getProductId() == TeTHEME || absTheme->getProductId() == TeEXTERNALTHEME))
	{
		theme = appTheme->getSourceTheme();
		if (theme == 0)
			return;
		layer = theme->layer();
		TC = theme->collectionTable();
	}

	if(canvas_->getCursorMode() == TeQtCanvas::Area)
	{
	}
	else if(canvas_->getCursorMode() == TeQtCanvas::Pointer)
	{
		// Put coordinates on the statusbar
		double  cx, cy;			
		char	buf[512];
		short	xg,xm,yg,ym;	
		float	xs, ys;

		memset(buf, 0, 512);
		if(canvas_->projection()->name() == "NoProjection")
		{
			sprintf (buf, "x:%.2f  y:%.2f", pt.x(), pt.y());
		}
		else
		{
			canvas_->projection()->setDestinationProjection(canvas_->projection());
			TeCoord2D spt = canvas_->projection()->PC2LL (pt);
			if (spt.x() > TePI)
			{
				buf[0]=0;
			}
			else
			{
				cx = spt.x()*TeCRD;
				cy = spt.y()*TeCRD;

				xg = (short)cx;
				yg = (short)cy;

				xm = (short)((cx-(float)xg)*60.);
				ym = (short)((cy-(float)yg)*60.);


				xs = (float)(((cx*60.-(float)xg*60.-(float)xm)*60.));

				ys = (float)(((cy*60.-(float)yg*60.-(float)ym)*60.));

				QString scaleGraphUnit = mainWindow_->currentView()->projection()->units().c_str();
				if(canvas_->projection()->name() == "LatLong")
				{
					if (scaleGraphUnit == "DecimalDegrees" && unit_ == "Decimal Degrees")
					{
						if (cx < 0 && xg == 0 && cy < 0 && yg == 0)
							sprintf (buf, "Long: -%02d:%02d:%.2f Lat: -%02d:%02d:%.2f", xg, ABS(xm), ABS(xs), yg, ABS(ym), ABS(ys));
						else if (cx < 0 && xg == 0)
							sprintf (buf, "Long: -%02d:%02d:%.2f Lat: %02d:%02d:%.2f", xg, ABS(xm), ABS(xs), yg, ABS(ym), ABS(ys));
						else if (cy < 0 && yg == 0)
							sprintf (buf, "Long: %02d:%02d:%.2f Lat: -%02d:%02d:%.2f", xg, ABS(xm), ABS(xs), yg, ABS(ym), ABS(ys));
						else
							sprintf (buf, "Long: %02d:%02d:%.2f Lat: %02d:%02d:%.2f", xg, ABS(xm), ABS(xs), yg, ABS(ym), ABS(ys));

					}
					else
					{
						if (cx < 0 && xg == 0 && cy < 0 && yg == 0)
							sprintf (buf, "Long: -%02d:%02d:%.2f Lat: -%02d:%02d:%.2f", xg, ABS(xm), ABS(xs), yg, ABS(ym), ABS(ys));
						else if (cx < 0 && xg == 0)
							sprintf (buf, "Long: -%02d:%02d:%.2f Lat: %02d:%02d:%.2f", xg, ABS(xm), ABS(xs), yg, ABS(ym), ABS(ys));
						else if (cy < 0 && yg == 0)
							sprintf (buf, "Long: %02d:%02d:%.2f Lat: -%02d:%02d:%.2f", xg, ABS(xm), ABS(xs), yg, ABS(ym), ABS(ys));
						else
							sprintf (buf, "Long: %02d:%02d:%.2f Lat: %02d:%02d:%.2f", xg, ABS(xm), ABS(xs), yg, ABS(ym), ABS(ys));
					}
				}
				else
				{
					if (scaleGraphUnit == "DecimalDegrees" && unit_ == "Decimal Degrees")
					{
						if (cx < 0 && xg == 0 && cy < 0 && yg == 0)
							sprintf (buf, "Long: -%02d:%02d:%.2f Lat: -%02d:%02d:%.2f  x: %.6f (%s)  y: %.6f (%s)", xg, ABS(xm), ABS(xs), yg, ABS(ym), ABS(ys), cx/unitConv_, unit_.c_str(), cy/unitConv_, unit_.c_str());
						else if (cx < 0 && xg == 0)
							sprintf (buf, "Long: -%02d:%02d:%.2f Lat: %02d:%02d:%.2f  x:%.6f (%s)  y:%.6f (%s)", xg, ABS(xm), ABS(xs), yg, ABS(ym), ABS(ys), cx/unitConv_, unit_.c_str(), cy/unitConv_, unit_.c_str());
						else if (cy < 0 && yg == 0)
							sprintf (buf, "Long: %02d:%02d:%.2f Lat: -%02d:%02d:%.2f  x:%.6f (%s)  y:%.6f (%s)", xg, ABS(xm), ABS(xs), yg, ABS(ym), ABS(ys), cx/unitConv_, unit_.c_str(), cy/unitConv_, unit_.c_str());
						else
							sprintf (buf, "Long: %02d:%02d:%.2f Lat: %02d:%02d:%.2f  x:%.6f (%s)  y:%.6f (%s)", xg, ABS(xm), ABS(xs), yg, ABS(ym), ABS(ys), cx/unitConv_, unit_.c_str(), cy/unitConv_, unit_.c_str());

					}
					else
					{
						if (cx < 0 && xg == 0 && cy < 0 && yg == 0)
							sprintf (buf, "Long: -%02d:%02d:%.2f Lat: -%02d:%02d:%.2f  x:%.3f (%s)  y:%.3f (%s)", xg, ABS(xm), ABS(xs), yg, ABS(ym), ABS(ys), pt.x()/unitConv_, unit_.c_str(), pt.y()/unitConv_, unit_.c_str());
						else if (cx < 0 && xg == 0)
							sprintf (buf, "Long: -%02d:%02d:%.2f Lat: %02d:%02d:%.2f  x:%.3f (%s)  y:%.3f (%s)", xg, ABS(xm), ABS(xs), yg, ABS(ym), ABS(ys), pt.x()/unitConv_, unit_.c_str(), pt.y()/unitConv_, unit_.c_str());
						else if (cy < 0 && yg == 0)
							sprintf (buf, "Long: %02d:%02d:%.2f Lat: -%02d:%02d:%.2f  x:%.3f (%s)  y:%.3f (%s)", xg, ABS(xm), ABS(xs), yg, ABS(ym), ABS(ys), pt.x()/unitConv_, unit_.c_str(), pt.y()/unitConv_, unit_.c_str());
						else
							sprintf (buf, "Long: %02d:%02d:%.2f Lat: %02d:%02d:%.2f  x:%.3f (%s)  y:%.3f (%s)", xg, ABS(xm), ABS(xs), yg, ABS(ym), ABS(ys), pt.x()/unitConv_, unit_.c_str(), pt.y()/unitConv_, unit_.c_str());
					}
				}
			}
		}
		if(graphCursorOn_ == false)
			mainWindow_->statusBar()->message(buf);
		if(absTheme->visibleRep() == 0)
			return;

		appTheme->chartSelected_.clear();
	
		// TODO: how to handle other types of themes
		if(graphCursorOn_ == false && mainWindow_->currentView()->connectedId() == 0)		// tooltip
		{
			if (!theme || !layer)
				return;

			if(mainWindow_->getTooltip().empty() == false)
			{
				static TeBox sbox;
				static string sfname;
				string oid;
				bool bb = false;
				if(layer->hasGeometry(TePOINTS))
				{
					TePoint point;
					bb = theme->locatePoint(pdw, point, delta);
					if (bb == true)
						oid = point.objectId();
				}
				else if(layer->hasGeometry(TeLINES))
				{
					TeLine2D line;
					bb = theme->locateLine(pdw, line, delta);
					if (bb == true)
						oid = line.objectId();
				}
				else if(bb==false && layer->hasGeometry(TeCELLS))
				{
					TeCell cel;
					bb = theme->locateCell(pdw, cel, 0.);
					if (bb == true)
						oid = cel.objectId();
				}
				else if(bb==false && layer->hasGeometry(TePOLYGONS))
				{
					TePolygon polygon;
					bb = theme->locatePolygon(pdw, polygon, 0.);
					if (bb == true)
						oid = polygon.objectId();
				}
				if(oid.empty() == false)
				{
					string fname = mainWindow_->getTooltip();
					if(!(oid == tipId_) || !(sbox == canvas_->getWorld()) || !(sfname == fname))
					{
						TeVisual visual;
						double th;
						double x;
						double y;
						int	nn = 1;
						string TC = theme->collectionTable();
						string sel, csel;
						int ff = fname.find("(");
						TeDatabasePortal *portal= layer->database()->getPortal();

						if(ff >= 0)
						{
							sel = "SELECT " + fname + ", MIN(" + TC + ".label_x), MIN(";
							sel += TC + ".label_y)" + theme->sqlFrom();
							sel += " WHERE " + TC + ".c_object_id = '" + oid + "'";
							sel += " GROUP BY " + TC + ".c_object_id";
						}
						else
						{
							sel = "SELECT " + fname + ", " + TC + ".label_x, ";
							sel += TC + ".label_y" + theme->sqlFrom();
							sel += " WHERE " + TC + ".c_object_id = '" + oid + "'";
							sel += " ORDER BY " + fname;

							string cname = "COUNT(" + fname + ")";

							csel = "SELECT " + cname + theme->sqlFrom();
							csel += " WHERE " + TC + ".c_object_id = '" + oid + "'";
							csel += " GROUP BY " + TC + ".c_object_id";
							if(portal->query(csel) && portal->fetchRow())
								nn = portal->getInt(0);
						}

						xorOn_ = false;
						canvas_->copyPixmapToWindow();
						canvas_->plotOnWindow();
						visual = *(theme->defaultLegend().visual(TeTEXT));
						TeColor cor = visual.color();
						canvas_->setTextColor(cor.red_, cor.green_, cor.blue_);
						string fam = visual.family();
						canvas_->setTextStyle (fam, visual.size(), visual.bold(), visual.italic());
						int tsize = visual.size();
						th = canvas_->mapVtoCW(tsize);
						int	the = (int)(th * 1.3);

						portal->freeResult();
						if(portal->query(sel) && portal->fetchRow())
						{
							x = portal->getDouble(1);
							y = portal->getDouble(2);
							y = y + (nn-1) * the / 2.;
						}

						portal->freeResult();
						if(portal->query(sel))
						{
							nn = 0;
							while(portal->fetchRow())
							{
								string val = portal->getData(0);

								TeText tx;
								TeCoord2D tp(x, y - (nn++ * the));
								tx.add(tp);
								tx.setTextValue(val);
								tx.setHeight(th);
								canvas_->plotText(tx, visual);
							}
							plotGeographicalGrid();
							plotGraphicScale();

							tipId_ = oid;
							sfname = fname;
							sbox = canvas_->getWorld();
						}
						delete portal;
					}
				}
			}
			else
			{
				tipId_.clear();
				xorOn_ = false;
				plotGeographicalGrid();
				plotGraphicScale();
				canvas_->copyPixmapToWindow();
			}
		}
		else if(graphCursorOn_==true) // draw rubber band cursor for spatial selection
		{
			plotXor(false);
			QPoint p = m;
			static QPoint pc(p);
			int w, h;
			w = h = 1000; // max width and height

			if(state & Qt::AltButton) //change cursor dimension
			{
				QPoint dif = p - cGraphCursor_;

				wGraphCursor_ += 2 * dif.x();
				hGraphCursor_ += 2 * dif.y();
				if(wGraphCursor_ <= 0)
					wGraphCursor_ = 1;
				if(hGraphCursor_ <= 0)
					hGraphCursor_ = 1;
				if(wGraphCursor_ >= w)
					wGraphCursor_ = w;
				if(hGraphCursor_ >= h)
					hGraphCursor_ = h;

				circleRadius_ = (wGraphCursor_ + hGraphCursor_) / 2;
			}
			if(state & Qt::ControlButton) //change cursor angle
			{
				double dx = p.x() - pc.x();
				double dy = p.y() - pc.y();
				spatialCursorAngle_ = atan(dy/dx) * 180. / TePI;
				if(dx<0)
					spatialCursorAngle_ = 180. + spatialCursorAngle_;
				else if(dx>=0 && dy<0)
					spatialCursorAngle_ = 360. + spatialCursorAngle_;
			}
			else
				pc = p;

			string s = buf;
			memset(buf, 0, 512);
			double ww = canvas_->mapVtoCW(wGraphCursor_);
			double hh = canvas_->mapVtoCW(hGraphCursor_);
			s += "  ";
			if(popupSpatialCursorType_->isItemChecked(popupSpatialCursorType_->idAt(0))) // rectangle
			{
				s += tr("size:").ascii();
				sprintf(buf, "%s%.3f (%s) x %.3f (%s)", s.c_str(), ww/unitConv_, unit_.c_str(), hh/unitConv_, unit_.c_str());
			}
			else
			{
				s += tr("radius:").ascii();
				sprintf(buf, "%s%.3f (%s) x %.3f (%s)", s.c_str(), ww/unitConv_/2., unit_.c_str(), hh/unitConv_/2., unit_.c_str());
			}
			mainWindow_->statusBar()->message(buf);

			cGraphCursor_ = p;
			plotXor(true);
		}
	}
	else if(canvas_->getCursorMode() == TeQtCanvas::Edit)
	{
		xorPointArrayVec_.clear();
		QPoint qp = m;
		TeCoord2D pt((double)qp.x(), (double)qp.y());
		TeCoord2D p = appTheme->canvasLegUpperLeft();
		double w = appTheme->canvasLegWidth();
		double h = appTheme->canvasLegHeight();

		if(appTheme->getTheme()->type() == TeTHEME)
		{
			if((theme->visibleRep() & 0x20000000) && p.x() < pt.x() && pt.x() < p.x()+w &&
			p.y() < pt.y() && pt.y() < p.y()+h)
			{
				appTheme->editRep((TeGeomRep)0x20000000); // sel legend
				canvas_->setMode (TeQtCanvas::Hand);
				canvas_->viewport()->setCursor( QCursor( PointingHandCursor ) );
			}
			else if(appTheme->chartAttributes_.size() && (theme->visibleRep() & 0x80000000) && locatePieBar(mainWindow_->currentDatabase(), appTheme, pdw, delta))
			{
				appTheme->editRep((TeGeomRep)0x80000000); // sel piebar
				canvas_->setMode (TeQtCanvas::Hand);
				canvas_->viewport()->setCursor( QCursor( PointingHandCursor ) );
			}
			else if((theme->visibleRep() == 0x4) && theme->locatePoint (pdw, appTheme->tePoint_, delta))
			{
				appTheme->editRep((TeGeomRep)0x4); // sel TePoint
				canvas_->setMode (TeQtCanvas::Hand);
				canvas_->viewport()->setCursor( QCursor( PointingHandCursor ) );
			}
			else if(graphicScaleVisible_ && canvas_->locateGraphicScale(m)) // sel graphic scale
			{
				appTheme->editRep((TeGeomRep)0x10000000); // sel graphic scale
				canvas_->setMode (TeQtCanvas::Hand);
				canvas_->viewport()->setCursor( QCursor( PointingHandCursor ) );
			}
			else // text can be selected or not selected
			{
				if(state == Qt::NoButton || state == Qt::AltButton) // altButton for text 45 degrees
				{
					canvas_->plotOnPixmap0();
					if(mtEdit_.isHotPoint(qp))
					{
						appTheme->editRep(TeTEXT);
						int mode = mtEdit_.mode();
						if(mode == 5)
						{
							canvas_->setMode (TeQtCanvas::Hand);
							canvas_->viewport()->setCursor( QCursor( PointingHandCursor ) );
						}
						else
						{
							canvas_->setMode (TeQtCanvas::UpArrow);
							canvas_->viewport()->setCursor( QCursor( UpArrowCursor ) );
						}
					}
				}
				else if((state & Qt::LeftButton)) // draging for area cursor selection
				{
					if((state & Qt::ShiftButton) == 0) // if not is shiftButton for add texts
					{
						mtEdit_.endEdit();				// initialize
						appTheme->editRep((TeGeomRep)0);
					}

					mtEdit_.selMode(true);
					canvas_->setMode (TeQtCanvas::Area);
					canvas_->viewport()->setCursor( QCursor( CrossCursor ) );
					canvas_->initCursorArea(qp);
				}
			}
		}
		else
		{
			if(graphicScaleVisible_ && canvas_->locateGraphicScale(m)) // sel graphic scale
			{
				appTheme->editRep((TeGeomRep)0x10000000); // sel graphic scale
				canvas_->setMode (TeQtCanvas::Hand);
				canvas_->viewport()->setCursor( QCursor( PointingHandCursor ) );
			}
		}
	}
	else if(canvas_->getCursorMode() == TeQtCanvas::Distance)
	{
		canvas_->plotOnPixmap0();
		QPoint c = m; 
		distancePoint_ = c;
		plotXor(true);

		if(xorPointArrayVec_.size())
		{
			QPoint a = xorPointArrayVec_[0].point(0);
			QPoint b = xorPointArrayVec_[0].point(1);
			TeCoord2D pa = canvas_->mapVtoCW(a);
			TeCoord2D pb = canvas_->mapVtoCW(b);
			if(state & Qt::ControlButton)
				emit distanceMeterSignal(pa, pb);

			double distance = sqrt((pa.x()-pb.x()) * (pa.x()-pb.x()) + (pa.y()-pb.y()) * (pa.y()-pb.y()));
			QString s = tr("Distance:") + " " + Te2String(distance/unitConv_,6).c_str() + "(" + unit_.c_str() + ")";
			double kernelDistance = distance;	
		
			if(!(*(canvas_->projection()) == *(themeProjection)))
			{
				TeCoord2D pa = canvas_->mapVtoDW(a);
				TeCoord2D pb = canvas_->mapVtoDW(b);

				double distance = sqrt((pa.x()-pb.x()) * (pa.x()-pb.x()) + (pa.y()-pb.y()) * (pa.y()-pb.y()));
				kernelDistance = distance;	
			}
			mainWindow_->statusBar()->message(s);
		}
	}
	else if(canvas_->getCursorMode() == TeQtCanvas::Hand ||
		canvas_->getCursorMode() == TeQtCanvas::UpArrow) // edition progress
	{
		if(appTheme->getTheme()->type() == TeTHEME)
		{
			if((state & Qt::LeftButton) == 0) // mouse is not draging
			{
				canvas_->plotOnPixmap0();
				if(layer->hasGeometry(TeTEXT) && mtEdit_.edit() && appTheme->editRep() == TeTEXT) // if text on edition
				{
					QPoint p = m;
					if(!mtEdit_.isHotPoint(p))
					{
						mainWindow_->enableEditAction_toggled(true);
					}
					else
					{
						if(mtEdit_.mode() == 5)
						{
							canvas_->setMode (TeQtCanvas::Hand);
							canvas_->viewport()->setCursor( QCursor( PointingHandCursor ) );
						}
						else
						{
							canvas_->setMode (TeQtCanvas::UpArrow);
							canvas_->viewport()->setCursor( QCursor( UpArrowCursor ) );
						}
					}
				}
				else if(appTheme->editRep() == (TeGeomRep)0x80000000 &&
					canvas_->getCursorMode() == TeQtCanvas::Hand) // if piebar selected
				{
					if(appTheme->chartAttributes_.size())
					{
						if(locatePieBar(mainWindow_->currentDatabase(), appTheme, pdw, delta) == false)
						{
							appTheme->editRep((TeGeomRep)0); // unselect piebar
							mainWindow_->enableEditAction_toggled(true);
						}
					}
				}
				else if(appTheme->editRep() == (TeGeomRep)0x4 &&
					canvas_->getCursorMode() == TeQtCanvas::Hand) // if TePoint selected
				{
					if(theme->locatePoint(pdw, appTheme->tePoint_, delta) == false)
					{
						appTheme->editRep((TeGeomRep)0); // unselect TePoint
						mainWindow_->enableEditAction_toggled(true);
					}
				}
				else if(appTheme->editRep() == (TeGeomRep)0x20000000) // if legend selected
				{
					QPoint qp = m;
					TeCoord2D pt((double)qp.x(), (double)qp.y());
					TeCoord2D p = appTheme->canvasLegUpperLeft();
					double w = appTheme->canvasLegWidth();
					double h = appTheme->canvasLegHeight();
					if(!(p.x() < pt.x() && pt.x() < p.x()+w &&
					p.y() < pt.y() && pt.y() < p.y()+h))
					{
						appTheme->editRep((TeGeomRep)0); // unselect legend
						mainWindow_->enableEditAction_toggled(true);
					}
				}
				else if(graphicScaleVisible_) // if graphic scale selected
				{
					if(canvas_->locateGraphicScale(m) == false)
					{
						appTheme->editRep((TeGeomRep)0); // unselect legend
						mainWindow_->enableEditAction_toggled(true);
					}
				}
			}
			else if((state & Qt::LeftButton)) // mouse drag
			{
				canvas_->plotOnWindow();
				if(mtEdit_.edit()) // drag text
				{
					canvas_->plotOnPixmap0();
					bool apxAngle = false;
	#ifdef WIN32
					if((state & Qt::AltButton))
	#else
					if((state & (Qt::AltButton | Qt::ControlButton)))
	#endif
						apxAngle = true;
					QPoint p = m;
					if(mtEdit_.mode() > 0)
						mtEdit_.change(p, apxAngle);
					else
					{
						mtEdit_.isHotPoint(p);
						mtEdit_.change(p, apxAngle);
					}
					plotXor(true);
				}
				else if(appTheme->editRep() == (TeGeomRep)0x20000000) // drag legend
				{
					TeCoord2D p = appTheme->canvasLegUpperLeft();
					QPoint p1((int)p.x(), (int)p.y());
					QPoint p2((int)pLeg_.x(), (int)pLeg_.y());
					QPoint p3 = p1 - p2;
					p2 = m - p2;
					p1 += p2;
					p.x(p1.x());
					p.y(p1.y());
					appTheme->canvasLegUpperLeft(p);
					p2 = p1 - p3;
					pLeg_.x(p2.x());
					pLeg_.y(p2.y());
					plotXor(true);
				}
				else if(appTheme->editRep() == (TeGeomRep)0x10000000) // drag graphic scale
				{
					mGraphicScale_ = m;
					plotXor(true);
				}
				else if(appTheme->editRep() == (TeGeomRep)0x80000000) // drag piechart or barchart
				{
					appTheme->chartPoint_ = pdw;
					plotXor(true);
				}
				else if(appTheme->editRep() == (TeGeomRep)0x4) // drag TePoint
				{
					appTheme->tePoint_.add(pdw);
					plotXor(true);
				}
			}
		}
		else
		{
			if((state & Qt::LeftButton) == 0) // mouse is not draging
			{
				canvas_->plotOnPixmap0();

				if(graphicScaleVisible_) // if graphic scale selected
				{
					if(canvas_->locateGraphicScale(m) == false)
					{
						appTheme->editRep((TeGeomRep)0); // unselect legend
						mainWindow_->enableEditAction_toggled(true);
					}
				}
			}
			else if((state & Qt::LeftButton)) // mouse drag
			{
				canvas_->plotOnWindow();

				if(appTheme->editRep() == (TeGeomRep)0x10000000) // drag graphic scale
				{
					mGraphicScale_ = m;
					plotXor(true);
				}
			}
		}
	}
	else if(canvas_->getCursorMode() == TeQtCanvas::Information)
	{
		if (absTheme->visibleGeoRep() & TeRASTER)
			rasterDataToStatusBar(layer, pdw);
	}
}


void DisplayWindow::mouseReleasedOnCanvas(TeCoord2D& pt, int state, QPoint& m)
{
	canvas_->plotOnPixmap0();
	if(state == Qt::RightButton || state == Qt::MidButton)
		return;

	if(!mainWindow_->currentView() || !canvas_->getWorld().isValid() || !mainWindow_->currentTheme())
		return;

	TeAbstractTheme* absTheme = 0;
	TeProjection* themeProjection = 0;
	TeAppTheme* appTheme = mainWindow_->currentTheme();
	if (appTheme)
	{
		absTheme = appTheme->getTheme();
		themeProjection = absTheme->getThemeProjection();
	}
	else
		return;

	if (themeProjection)
		canvas_->setDataProjection(themeProjection);

	TeQtProgress progress(this, "ProgressDialog", true);

	TeCoord2D pdw, pan;
	double	resolution;
	double	delta;
	if(canvas_->projection())
	{
		if(themeProjection && !(*(themeProjection) == *(canvas_->projection())))
		{
			resolution = canvas_->pixelSize();
			TeCoord2D pl(pt.x()-resolution/2., pt.y()-resolution/2.);
			TeCoord2D pu(pt.x()+resolution/2., pt.y()+resolution/2.);
			TeBox	box(pl, pu);
			box = TeRemapBox(box, canvas_->projection(), themeProjection);
			resolution = box.x2_ - box.x1_;
			canvas_->projection()->setDestinationProjection(themeProjection);
			pdw = canvas_->projection()->PC2LL(pt);
			pdw = themeProjection->LL2PC(pdw);
			pan = canvas_->projection()->PC2LL(panStart_);
			pan = themeProjection->LL2PC(pan);
			delta = 3.*resolution;
		}
		else
		{
			delta = 3.*canvas_->pixelSize();
			pdw = pt;
			pan = panStart_;
		}
	}
	else
	{
		delta = 3.*canvas_->pixelSize();
		pdw = pt;
		pan = panStart_;
	}

	if(canvas_->getCursorMode() == TeQtCanvas::Pointer && state == Qt::LeftButton)
	{
		if(connectedPixmap_)
		{
			int ulx = connectedRect_.left();
			int uly = connectedRect_.top();
			int w = connectedRect_.width();
			int h = connectedRect_.height();
			QPoint of(canvas_->contentsX (), canvas_->contentsY ());
			QPaintDevice *device = canvas_->viewport();

			bitBlt (device,ulx,uly,connectedPixmap_,ulx+of.x(),uly+of.y(),w,h,Qt::CopyROP,true);
			canvas_->plotOnWindow();
			canvas_->setLineColor (255, 0, 0);
			canvas_->setLineStyle (TeLnTypeContinuous);
			canvas_->plotRect(connectedRect_);
		}
	}
	else if (canvas_->getCursorMode() == TeQtCanvas::Pan && state == Qt::LeftButton)
	{
		double	dx = panStart_.x() - pt.x();
		double	dy = panStart_.y() - pt.y();

		TeBox box = canvas_->getWorld();
		box.x1_ = box.x1_ + dx;
		box.x2_ = box.x2_ + dx;
		box.y1_ = box.y1_ + dy;
		box.y2_ = box.y2_ + dy;

		canvas_->setWorld(box, mainWindow_->cvContentsW_, mainWindow_->cvContentsH_);
		plotData();
		mainWindow_->updateBoxesVector(box);
	}

	TeTheme* theme = 0;
	TeLayer* layer = 0;
	if (absTheme->getProductId() == TeTHEME || absTheme->getProductId() == TeEXTERNALTHEME)
	{
		theme = appTheme->getSourceTheme();
		if (theme == 0)
			return;
		layer = theme->layer();
	}

	if (canvas_->getCursorMode() == TeQtCanvas::Edit)
	{
		if(appTheme->getTheme()->type() == TeTHEME)
		{
			if(mtEdit_.edit()) // text edition
			{
				TeText text;
				bool bb = appTheme->locateText(pdw, text, delta);
				if (bb == true)
				{
					mtEdit_.change();
					plotXor(true);
					canvas_->setMode (TeQtCanvas::Hand);
					canvas_->viewport()->setCursor( QCursor( PointingHandCursor ) );
				}
				else
				{
					if((state & Qt::ShiftButton) == 0)
					{
						mtEdit_.endEdit();
						appTheme->editRep((TeGeomRep)0); // unselect text
						plotXor(false);
					}
				}
				textRectSelected_ = mtEdit_.getRect();
			}
		}
		else
		{
			QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
			canvas_->copyPixmapToWindow();
		}
	}
	else if (canvas_->getCursorMode() == TeQtCanvas::Area)
	{
		if(appTheme->getTheme()->type() == TeTHEME || appTheme->getTheme()->type() == TeEXTERNALTHEME)
		{
			if(mainWindow_->zoomCursorAction->isOn()==false && (theme->visibleRep() & TeTEXT) == 0)
			{
				mainWindow_->enableEditAction_toggled(true);
				canvas_->copyPixmapToWindow();
				return;
			}
			if (editOn_ && mainWindow_->zoomCursorAction->isOn() == false) // sel texts from area cursor
			{
				canvas_->plotOnPixmap0();
				mtEdit_.selMode(false);
				TeBox box = canvas_->getCursorBox ();
				if(!(*(canvas_->projection()) == *(layer->projection())))
					box = TeRemapBox (box, canvas_->projection(), layer->projection());
				if(box.x1_ > box.x2_)
				{
					double x = box.x1_;
					box.x1_ = box.x2_;
					box.x2_ = x;
				}
				if(box.y1_ > box.y2_)
				{
					double y = box.y1_;
					box.y1_ = box.y2_;
					box.y2_ = y;
				}
				mtEdit_.mode(0);
				mainWindow_->enableEditAction_toggled(true);
				TeDatabasePortal* portal  = mainWindow_->currentDatabase()->getPortal();
				string table = appTheme->textTable();
				string CT = theme->collectionTable();
				string sel = "SELECT * FROM " + table + ", " + CT;
				sel += " WHERE x > " + Te2String(box.x1_, 9);
				sel += " AND x < " + Te2String(box.x2_, 9);
				sel += " AND y > " + Te2String(box.y1_, 9);
				sel += " AND y < " + Te2String(box.y2_, 9);
				sel += " AND " + CT + ".c_object_id = " + table + ".object_id";
				portal->freeResult();
				if(portal->query(sel) && portal->fetchRow())
				{
					TeWaitCursor wait;
					bool flag;
					do
					{
						TeQtTextEdit tedit;
						TeText tx;
						flag = portal->fetchGeometry(tx);
						TeVisual visual = appTheme->getTextVisual(tx);
						tedit.init(tx, visual, canvas_);
						mtEdit_.push(tedit);
					} while (flag);
				}

				sel = "SELECT * FROM " + table;
				sel += " WHERE x > " + Te2String(box.x1_, 9);
				sel += " AND x < " + Te2String(box.x2_, 9);
				sel += " AND y > " + Te2String(box.y1_, 9);
				sel += " AND y < " + Te2String(box.y2_, 9);
				sel += " AND object_id = 'TE_NONE_ID'";
				portal->freeResult();
				if(portal->query(sel) && portal->fetchRow())
				{
					TeWaitCursor wait;
					bool flag;
					do
					{
						TeQtTextEdit tedit;
						TeText tx;
						flag = portal->fetchGeometry(tx);
						TeVisual visual = appTheme->getTextVisual(tx);
						tedit.init(tx, visual, canvas_);
						mtEdit_.push(tedit);
					} while (flag);
				}
				delete portal;

				if(mtEdit_.size())
					appTheme->editRep(TeTEXT); // select text
				else
					appTheme->editRep((TeGeomRep)0); // unselect text

				mtEdit_.change();
				plotXor(true);
				plotLegendOnCanvas (appTheme);
				canvas_->copyPixmapToWindow();
				textRectSelected_ = mtEdit_.getRect();
			}
		}
	}
	else if(canvas_->getCursorMode() == TeQtCanvas::Hand ||
		canvas_->getCursorMode() == TeQtCanvas::UpArrow)
	{
		if(appTheme->getTheme()->type() == TeTHEME || appTheme->getTheme()->type() == TeEXTERNALTHEME)
		{
			if(appTheme->editRep() == (TeGeomRep)0x80000000 &&
				canvas_->getCursorMode() == TeQtCanvas::Hand) // update piebar position
			{
				TeBox box = pieBarBoxEdit_;
				canvas_->plotOnPixmap0();
				canvas_->mapDWtoV(box);
				TeBox rb((int)(box.x1()-2), (int)(box.y1()-2), (int)(box.x2()+2), (int)(box.y2()+2));
				canvas_->copyPixmap1ToPixmap0((int)rb.x1(), (int)rb.y1(), (int)rb.width(), (int)rb.height());
				updatePieBarLocation(mainWindow_->currentDatabase(), appTheme, pdw);
				box = getPieBarBox(appTheme);
				canvas_->mapDWtoV(box);
				canvas_->copyPixmap1ToPixmap0((int)(box.x1()-2), (int)(box.y1()-2), (int)(box.width()+4), (int)(box.height()+4));
				canvas_->plotOnPixmap0();
				canvas_->mapVtoCW(rb);
				canvas_->mapCWtoDW(rb);
				plotPieBars(appTheme, canvas_, &progress, rb);
				TePoint tePoint(pdw);
				if(TeIntersects(tePoint, rb) == false)
				{
					TeBox rbb = rb;
					TeCoord2D center(rbb.x1_ + rbb.width() / 2., rbb.y1_ + rbb.height() / 2.);
					TeCoord2D ncenter = tePoint.location();
					double dx = ncenter.x() - center.x();
					double dy = ncenter.y() - center.y();
					rbb.x1_ = rbb.x1_ + dx;
					rbb.x2_ = rbb.x2_ + dx;
					rbb.y1_ = rbb.y1_ + dy;
					rbb.y2_ = rbb.y2_ + dy;
					plotPieBars(appTheme, canvas_, &progress, rbb);
				}
				TePlotTextWV(appTheme, canvas_, &progress, rb);
				plotLegendOnCanvas (appTheme);
				plotGeographicalGrid();
				plotGraphicScale();
				canvas_->copyPixmapToWindow();
				xorPointArrayVec_.clear();
				canvas_->setMode (TeQtCanvas::Hand);
				canvas_->viewport()->setCursor( QCursor( PointingHandCursor ) );
			}
			else if(appTheme->editRep() == (TeGeomRep)0x4 &&
				canvas_->getCursorMode() == TeQtCanvas::Hand) // update TePoint position
			{
				TeBox box = pointBoxEdit_;
				appTheme->tePoint_.add(pdw);
				updatePointLocation(mainWindow_->currentDatabase(), appTheme, pdw);

				plotData(box, true);
				if(TeIntersects(appTheme->tePoint_, box) == false)
				{
					TeCoord2D center(box.x1_ + box.width() / 2., box.y1_ + box.height() / 2.);
					TeCoord2D ncenter = appTheme->tePoint_.location();
					double dx = ncenter.x() - center.x();
					double dy = ncenter.y() - center.y();
					box.x1_ = box.x1_ + dx;
					box.x2_ = box.x2_ + dx;
					box.y1_ = box.y1_ + dy;
					box.y2_ = box.y2_ + dy;
					plotData(box, false);
				}
				plotXor(false);
				xorPointArrayVec_.clear();
				canvas_->setMode (TeQtCanvas::Hand);
				canvas_->viewport()->setCursor( QCursor( PointingHandCursor ) );
			}
			else if(appTheme->editRep() == (TeGeomRep)0x20000000) // update legend position
			{
				plotXor(false);
				if(!(absTheme->visibleGeoRep() & TeRASTER || absTheme->visibleGeoRep() & TeRASTERFILE))
				{
					QRect r = rectLegendOnCanvas_;
					r = QRect(r.x()-2, r.y()-4, r.width()+20, r.height()+8);
					TeBox rb(r.x(), r.y(), r.right(), r.bottom());
					canvas_->copyPixmap1ToPixmap0(r.x(), r.y(), r.width(), r.height());
					TeCoord2D p = appTheme->canvasLegUpperLeft();
					canvas_->plotOnPixmap0();
					canvas_->mapVtoCW(rb);
					canvas_->mapCWtoDW(rb);
					plotPieBars(appTheme, canvas_, &progress, rb);
					TePlotTextWV(appTheme, canvas_, &progress, rb);
					plotLegendOnCanvas (appTheme);
					plotGeographicalGrid();
					plotGraphicScale();
					canvas_->copyPixmapToWindow();
					string up = "UPDATE te_theme_application SET canvas_leg_x = " + Te2String(p.x(), 9);
					up += ", canvas_leg_y = " + Te2String(p.y(), 9);
					up += " WHERE theme_id = " + Te2String(theme->id());
					mainWindow_->currentDatabase()->execute(up);
					xorPointArrayVec_.clear();
					canvas_->setMode (TeQtCanvas::Hand);
					canvas_->viewport()->setCursor( QCursor( PointingHandCursor ) );
				}
				else
					plotData();
			}
			else if(mtEdit_.edit())
			{
				TeQtCanvas::CursorMode cmode = canvas_->getCursorMode();
				if(layer->hasGeometry(TeTEXT))
				{
					string table = appTheme->textTable();
					int i, j;
					TeWaitCursor wait;

					i = 0;
					while(i < mtEdit_.size())
					{
						string ids = "(";
						j = 0;
						while(j < 200 && i < mtEdit_.size())
						{
							ids += Te2String(mtEdit_.text(i).geomId()) + ",";
							i++;
							j++;
						}
						ids = ids.substr(0, ids.size()-1);
						ids += ")";

						if(mtEdit_.mode() == 5)
						{
							double px, py;
							TeText tx = mtEdit_.text(i-1);
							int geoid = tx.geomId();
							string sel = "SELECT x, y FROM " + appTheme->textTable();
							sel += " WHERE geom_id = " + Te2String(geoid);
							TeDatabasePortal* portal = mainWindow_->currentDatabase()->getPortal();
							portal->freeResult();
							if(portal->query(sel))
							{
								if(portal->fetchRow())
								{
									px = portal->getDouble(0);
									py = portal->getDouble(1);
									portal->freeResult();

									string dx = Te2String(px - tx.location().x());
									string dy = Te2String(py - tx.location().y());
									string up = "UPDATE " + appTheme->textTable() + " SET";
									up += " x = x - " + dx;
									up += ", y = y - " + dy;
									up += " WHERE geom_id IN " + ids;
									mainWindow_->currentDatabase()->execute(up);
								}
							}

							TeBox tBox;
							string textBox = "SELECT MIN(x), MIN(y), MAX(x), MAX(y) FROM ";
							textBox += appTheme->textTable();
							portal->freeResult();
							if(portal->query(textBox))
							{
								if(portal->fetchRow())
								{
									double x1 = portal->getDouble(0);
									double y1 = portal->getDouble(1);
									double x2 = portal->getDouble(2);
									double y2 = portal->getDouble(3);
									TeBox box(x1, y1, x2, y2);
									tBox = box;
								}
							}
							delete portal;

							TeBox lBox = layer->box();
							updateBox(lBox, tBox);
							layer->setLayerBox(lBox);
							string up = "UPDATE te_layer SET lower_x = " + Te2String(lBox.x1_);
							up += ", lower_y = " + Te2String(lBox.y1_);
							up += ", upper_x = " + Te2String(lBox.x2_);
							up += ", upper_y = " + Te2String(lBox.y2_);
							up += " WHERE layer_id = " + Te2String(layer->id());
							mainWindow_->currentDatabase()->execute(up);

							up = "UPDATE te_representation SET lower_x = " + Te2String(tBox.x1_);
							up += ", lower_y = " + Te2String(tBox.y1_);
							up += ", upper_x = " + Te2String(tBox.x2_);
							up += ", upper_y = " + Te2String(tBox.y2_);
							up += " WHERE layer_id = " + Te2String(layer->id());
							up += " AND geom_table = '" + appTheme->textTable() + "'";
							mainWindow_->currentDatabase()->execute(up);
						}
						else
						{
							string angle = Te2String(mtEdit_.text(i-1).angle(), 3);
							string height = Te2String(mtEdit_.text(i-1).height(), 6);

							string up = "UPDATE " + appTheme->textTable() + " SET";
							if(mtEdit_.mode() == 1 || mtEdit_.mode() == 2)
								up += " angle = " + angle;
							else
								up += " height = " + height;
							up += " WHERE geom_id IN " + ids;
							mainWindow_->currentDatabase()->execute(up);
						}
					}
				}
				plotXor(false);
				if(!(absTheme->visibleGeoRep() & TeRASTER || absTheme->visibleGeoRep() & TeRASTERFILE))
				{
					textRectSelected_.moveCenter(textRectSelected_.center());
					canvas_->copyPixmap1ToPixmap0(textRectSelected_.left(), textRectSelected_.top(),
						textRectSelected_.width(), textRectSelected_.height());
					canvas_->plotOnPixmap0();
					TeBox rb(textRectSelected_.left(), textRectSelected_.top(), textRectSelected_.right(), textRectSelected_.bottom());
					canvas_->mapVtoCW(rb);
					canvas_->mapCWtoDW(rb);
					plotPieBars(appTheme, canvas_, &progress, rb);
					TePlotTextWV(appTheme, canvas_, &progress, rb);
					textRectSelected_ = mtEdit_.getRect();
					textRectSelected_.moveCenter(textRectSelected_.center());
					TeBox nb(textRectSelected_.left(), textRectSelected_.top(), textRectSelected_.right(), textRectSelected_.bottom());
					canvas_->mapVtoCW(nb);
					canvas_->mapCWtoDW(nb);
					TePlotTextWV(appTheme, canvas_, &progress, nb);
					plotLegendOnCanvas (appTheme);
					plotGeographicalGrid();
					plotGraphicScale();
					plotXor(true);
					canvas_->copyPixmapToWindow();
					textRectSelected_ = mtEdit_.getRect();
					if(cmode == TeQtCanvas::Hand)
					{
						canvas_->setMode (TeQtCanvas::Hand);
						canvas_->viewport()->setCursor( QCursor( PointingHandCursor ) );
					}
					else if(cmode == TeQtCanvas::UpArrow)
					{
						canvas_->setMode (TeQtCanvas::UpArrow);
						canvas_->viewport()->setCursor( QCursor( UpArrowCursor ) );
					}
				}
				else
					plotData();
			}
			else if(graphicScaleVisible_ && canvas_->getCursorMode() == TeQtCanvas::Hand) // update graphic scale position
			{
				plotXor(false);
				canvas_->plotOnPixmap0();
				QRect r = canvas_->graphicScaleRect();
				canvas_->copyPixmap1ToPixmap0(r.x(), r.y(), r.width(), r.height());
				TeBox rb(r.x(), r.y(), r.right(), r.bottom());
				canvas_->mapVtoCW(rb);
				canvas_->mapCWtoDW(rb);
				plotPieBars(appTheme, canvas_, &progress, rb);
				TePlotTextWV(appTheme, canvas_, &progress, rb);
				plotLegendOnCanvas (appTheme);
				plotGeographicalGrid();
				graphicScaleOffsetX_ = (double)(m.x() - pGraphicScale_.x())/(double)canvas_->getPixmap0()->width();
				graphicScaleOffsetY_ = (double)(m.y() - pGraphicScale_.y())/(double)canvas_->getPixmap0()->height();
				plotGraphicScale();
				canvas_->copyPixmapToWindow();
				xorPointArrayVec_.clear();
				canvas_->setMode (TeQtCanvas::Hand);
				canvas_->viewport()->setCursor( QCursor( PointingHandCursor ) );
				graphicScaleOffsetX_ = 0.;
				graphicScaleOffsetY_ = 0.;
			}
		}
		else
		{
			if(graphicScaleVisible_ && canvas_->getCursorMode() == TeQtCanvas::Hand) // update graphic scale position
			{
				plotXor(false);
				canvas_->plotOnPixmap0();
				QRect r = canvas_->graphicScaleRect();
				canvas_->copyPixmap1ToPixmap0(r.x(), r.y(), r.width(), r.height());
				TeBox rb(r.x(), r.y(), r.right(), r.bottom());
				canvas_->mapVtoCW(rb);
				canvas_->mapCWtoDW(rb);
				plotPieBars(appTheme, canvas_, &progress, rb);
				TePlotTextWV(appTheme, canvas_, &progress, rb);
				plotLegendOnCanvas (appTheme);
				plotGeographicalGrid();
				graphicScaleOffsetX_ = (double)(m.x() - pGraphicScale_.x())/(double)canvas_->getPixmap0()->width();
				graphicScaleOffsetY_ = (double)(m.y() - pGraphicScale_.y())/(double)canvas_->getPixmap0()->height();
				plotGraphicScale();
				canvas_->copyPixmapToWindow();
				xorPointArrayVec_.clear();
				canvas_->setMode (TeQtCanvas::Hand);
				canvas_->viewport()->setCursor( QCursor( PointingHandCursor ) );
				graphicScaleOffsetX_ = 0.;
				graphicScaleOffsetY_ = 0.;
			}
		}
	}
}

void DisplayWindow::mouseDoublePressedOnCanvas(TeCoord2D& pt, int state, QPoint&)
{
	if(state == Qt::RightButton || state == Qt::MidButton)
		return;

	if(mainWindow_->currentView() == 0 || mainWindow_->currentDatabase() == 0 || mainWindow_->currentTheme() == 0)
		return;

	TeAbstractTheme* absTheme = 0;
	TeProjection* themeProjection = 0;
	TeAppTheme* appTheme = mainWindow_->currentTheme();
	if (appTheme)
	{
		absTheme = appTheme->getTheme();
		themeProjection = absTheme->getThemeProjection();
	}
	else
		return;

	TeTheme* theme = 0;
	TeLayer* layer = 0;
	if (absTheme->getProductId() == TeTHEME || absTheme->getProductId() == TeEXTERNALTHEME)
	{
		theme = appTheme->getSourceTheme();
		layer = theme->layer();
	}
	if (!theme || !layer)
		return;

	if (canvas_->getCursorMode() == TeQtCanvas::Pointer)
	{
		TeCoord2D pdw;
		double	resolution;
		double	delta;

		canvas_->setDataProjection(themeProjection);
		TeBox wcbox = canvas_->getWorld();
		if(wcbox.isValid() == false)
			return;
		if(canvas_->projection())
		{
			if(!(*(themeProjection) == *(canvas_->projection())))
			{
				resolution = canvas_->pixelSize();
				TeCoord2D pl(pt.x()-resolution/2., pt.y()-resolution/2.);
				TeCoord2D pu(pt.x()+resolution/2., pt.y()+resolution/2.);
				TeBox	box(pl, pu);
				box = TeRemapBox(box, canvas_->projection(), themeProjection);
				resolution = box.x2_ - box.x1_;
				canvas_->projection()->setDestinationProjection(themeProjection);
				pdw = canvas_->projection()->PC2LL(pt);
				pdw = themeProjection->LL2PC(pdw);
				delta = 3.*resolution;
			}
			else
			{
				delta = 3.*canvas_->pixelSize();
				pdw = pt;
			}
		}
		else
		{
			delta = 3.*canvas_->pixelSize();
			pdw = pt;
		}

		object_id_.clear();
		bool bb = false;
		if(appTheme->editRep() == (TeGeomRep)0)
		{
			if(layer->hasGeometry(TePOINTS))
			{
				TePoint point;
				bb = theme->locatePoint(pdw, point, delta);
				if (bb == true)
					object_id_ = point.objectId();
			}
			if(bb==false && layer->hasGeometry(TeLINES))
			{
				TeLine2D line;
				bb = theme->locateLine(pdw, line, delta);
				if (bb == true)
					object_id_ = line.objectId();
			}
			if(bb==false && layer->hasGeometry(TeCELLS))
			{
				TeCell cel;
				bb = theme->locateCell(pdw, cel, 0.);
				if (bb == true)
					object_id_ = cel.objectId();
			}
			if(bb==false && layer->hasGeometry(TePOLYGONS))
			{
				TePolygon polygon;
				bb = theme->locatePolygon(pdw, polygon, 0.);
				if (bb == true)
					object_id_ = polygon.objectId();
			}
			if(bb==false && layer->hasGeometry(TeRASTER))
			{
				string values;
				double val;
				TeCoord2D ij = layer->raster()->coord2Index(pdw);
				for (int nb=0; nb <layer->raster()->nBands(); nb++)
				{
					if (!values.empty())
						values += ", ";
					if (layer->raster()->getElement((int)(ij.x_),(int)(ij.y_),val,nb))
						values += Te2String(val,3);
					else
						values += "NoData";
				}
				char mess[1024];
				sprintf(mess,"X:%.2f Y:%.2f ( %s )",pdw.x_,pdw.y_,values.c_str());
				mainWindow_->statusBar()->message(mess);
			}
		}
		if(object_id_.empty() == false)
		{
			if(absTheme->type() != TeTHEME)
			{
				QMessageBox::information(this, tr("Information"), tr("Display of media is not allowed for this current theme type!"));	
				return;
			}

			mainWindow_->setMediaId(object_id_);
			mainWindow_->popupShowDefaultMediaSlot();
		}
	}
}

void DisplayWindow::mouseLeaveCanvas()
{
	if (canvas_->getCursorMode() == TeQtCanvas::Pointer)
	{
		if(mainWindow_->getTooltip().empty()==false)
		{
			tipId_.clear();
			canvas_->copyPixmapToWindow();
		}
		if(mtEdit_.size() == 0)
		{
			if(mainWindow_->distanceMeterAction->isOn() == false)
			{
				plotXor(false);
				xorPointArrayVec_.clear();
			}
		}
		if(connectedPixmap_)
		{
			QRect r;
			restoreConnectedRect(r);
		}
	}
}

void DisplayWindow::keyPressedOnCanvas(QKeyEvent* e)
{
	if(mainWindow_->currentView() == 0)
		return;

	editOn_ = mainWindow_->editionIsActivated();
	graphCursorOn_ = mainWindow_->graphicCursorIsActivated();

	if(editOn_ == false)
	{
		if(graphCursorOn_ == false && canvas_->getCursorMode() == TeQtCanvas::Pointer)
		{
			if(e->key() == Qt::Key_Control)
			{
				QRect r;
				restoreConnectedRect(r);
			}
		}
	}

	TeAppTheme* appTheme = mainWindow_->currentTheme();
	if (appTheme == 0)
		return;
	
	TeTheme* theme = 0;
	TeLayer* layer = 0;
	if (appTheme->getTheme()->getProductId() == TeTHEME || appTheme->getTheme()->getProductId() == TeEXTERNALTHEME)
	{
		theme = appTheme->getSourceTheme();
		if (theme == 0)
			return;
		layer = theme->layer();
	}
	if (!theme || !layer)
		return;

	if (mainWindow_->currentDatabase() == 0)
		return;

	if(e->key() == Qt::Key_Delete)
	{
		if(mtEdit_.size())
		{
			int response = QMessageBox::question(this, tr("Delete Selected Texts"),
					 tr("Do you really want to delete the selected texts?"),
					 tr("Yes"), tr("No"));

			if (response != 0)
				return;

			vector<string> geomIdVec;
			int	i;
			for(i=0; i<mtEdit_.size(); i++)
			{
				TeText tx = mtEdit_.text(i);
				geomIdVec.push_back(Te2String(tx.geomId()));
			}
			plotXor(false);
			mtEdit_.endEdit();
			vector<string>::iterator itB = geomIdVec.begin();
			vector<string>::iterator itE = geomIdVec.end();
			vector<string> vec = generateInClauses(itB,itE, mainWindow_->currentDatabase(), false);

			string table = appTheme->textTable();
			for(i=0; i<(int)vec.size(); i++)
			{
				string del = "DELETE FROM " + table + " WHERE geom_id IN " + vec[i];
				mainWindow_->currentDatabase()->execute(del);
			}

			int conta = 1;	
			string sel = "SELECT COUNT(*) FROM " + table;
			TeDatabasePortal* portal = mainWindow_->currentDatabase()->getPortal();
			if(portal->query(sel))
			{
				if(portal->fetchRow())
					conta = portal->getInt(0);
			}
			portal->freeResult();

			TeBox rb(textRectSelected_.left(), textRectSelected_.top(), textRectSelected_.right(), textRectSelected_.bottom());
			canvas_->copyPixmap1ToPixmap0((int)rb.x1(), (int)rb.y1(), (int)rb.width(), (int)rb.height());
			canvas_->plotOnPixmap0();
			canvas_->setDataProjection (layer->projection());
			TeQtProgress progress(this, "ProgressDialog", true);
			canvas_->mapVtoCW(rb);
			canvas_->mapCWtoDW(rb);
			plotPieBars(appTheme, canvas_, &progress, rb);
			TePlotTextWV(appTheme, canvas_, &progress, rb);
			plotLegendOnCanvas (appTheme);
			plotGeographicalGrid();
			plotGraphicScale();
			updateTextEdit();
			canvas_->copyPixmapToWindow();
			mainWindow_->enableEditAction_toggled(true);

			if(conta == 0)
			{
				QString msg = tr("The text representation is empty!") + "\n";
				msg += tr("Do you really want to delete it?");
				int response = QMessageBox::question(this, tr("Delete Text Representation"),
									msg, tr("Yes"), tr("No"));

				if (response == 0)
				{
					string tabv = table + "_txvisual";
					string drop = "DROP TABLE " + tabv;
					mainWindow_->currentDatabase()->execute(drop);

					if(layer->removeGeometry(TeTEXT, table))
					{
						appTheme->textTable("");
						string s = "UPDATE te_theme_application SET text_table = null";
						s += " WHERE theme_id = " + Te2String(theme->id());
						mainWindow_->currentDatabase()->execute(s);

						int v = theme->visibleRep() & (TeTEXT ^ 0xffffffff);
						theme->visibleRep(v);
						s = "UPDATE te_theme SET visible_rep = " + Te2String(v);
						s += " WHERE theme_id = " + Te2String(theme->id());
						mainWindow_->currentDatabase()->execute(s);
					}
					else
					{
						msg = tr("The text representation table could not be removed!") + "\n";
						msg += tr("Check whether it is being used by another application.");
						QMessageBox::warning(this, tr("Warning"), msg);
					}
				}
			}
			else
			{
				TeBox tBox;
				string textBox = "SELECT MIN(x), MIN(y), MAX(x), MAX(y) FROM ";
				textBox += appTheme->textTable();
				portal->freeResult();
				if(portal->query(textBox))
				{
					if(portal->fetchRow())
					{
						double x1 = portal->getDouble(0);
						double y1 = portal->getDouble(1);
						double x2 = portal->getDouble(2);
						double y2 = portal->getDouble(3);
						TeBox box(x1, y1, x2, y2);
						tBox = box;
					}
				}
				portal->freeResult();

				TeBox lBox = layer->box();
				updateBox(lBox, tBox);
				layer->setLayerBox(lBox);
				string up = "UPDATE te_layer SET lower_x = " + Te2String(lBox.x1_);
				up += ", lower_y = " + Te2String(lBox.y1_);
				up += ", upper_x = " + Te2String(lBox.x2_);
				up += ", upper_y = " + Te2String(lBox.y2_);
				up += " WHERE layer_id = " + Te2String(layer->id());
				mainWindow_->currentDatabase()->execute(up);

				up = "UPDATE te_representation SET lower_x = " + Te2String(tBox.x1_);
				up += ", lower_y = " + Te2String(tBox.y1_);
				up += ", upper_x = " + Te2String(tBox.x2_);
				up += ", upper_y = " + Te2String(tBox.y2_);
				up += " WHERE layer_id = " + Te2String(layer->id());
				up += " AND geom_table = '" + appTheme->textTable() + "'";
				mainWindow_->currentDatabase()->execute(up);
			}
			delete portal;
		}
	}
}

void DisplayWindow::keyReleasedOnCanvas(QKeyEvent* e)
{
	editOn_ = mainWindow_->editionIsActivated();
	graphCursorOn_ = mainWindow_->graphicCursorIsActivated();

	TeView*	view = mainWindow_->currentView();
	if(view == 0)
		return;

	if(editOn_ == false && graphCursorOn_ == false && canvas_->getCursorMode() == TeQtCanvas::Pointer)
	{
		if(connectedPixmap_)
		{
			if(e->key() == Qt::Key_Control || e->key() == Qt::Key_Alt)
			{
				int ulx = connectedRect_.left();
				int uly = connectedRect_.top();
				int w = connectedRect_.width();
				int h = connectedRect_.height();
				QPoint of(canvas_->contentsX (), canvas_->contentsY ());
				QPaintDevice *device = canvas_->viewport();

				bitBlt (device,ulx,uly,connectedPixmap_,ulx+of.x(),uly+of.y(),w,h,Qt::CopyROP,true);
				canvas_->plotOnWindow();
				canvas_->setLineColor (255, 0, 0);
				canvas_->setLineStyle (TeLnTypeContinuous);
				canvas_->plotRect(connectedRect_);
			}
			else if(e->key() == Qt::Key_Escape)
			{
				int response = QMessageBox::question(this, tr("Disconnect View"),
						 tr("Do you really want to disconnect the View?"),
						 tr("Yes"), tr("No"));

				if (response != 0)
					return;

				view->connectedId(0);
				resetConnectedPixmap();
			}
		}
	}
}

bool DisplayWindow::plotXor( bool mode )
{
	TeView*	view = mainWindow_->currentView();
	if(view == 0)
		return xorOn_;

	TeAppTheme* appTheme = mainWindow_->currentTheme();
	if(appTheme == 0)
		return xorOn_;

	TeTheme* theme = 0;
	TeLayer* layer = 0;
	TeAbstractTheme* absTheme = appTheme->getTheme();
	if (absTheme->type() == TeTHEME || absTheme->type() == TeEXTERNALTHEME)
	{
		theme = appTheme->getSourceTheme();
		if (theme == 0)
			return false;
		layer = theme->layer();
	}

	canvas_->plotOnPixmap0();
	editOn_ = mainWindow_->editionIsActivated();
	graphCursorOn_ = mainWindow_->graphicCursorIsActivated();

	int i;

	if(xorOn_ && xorPointArrayVec_.size())
	{
		if(graphCursorOn_)
		{
			vector<QPointArray> pVec = rotateXorPoints();
			for(i=0; i < (int)pVec.size(); i++)
				canvas_->plotXorPolyline(pVec[i]);
		}
		else
		{
			bool cdev = true;
			if(appTheme->editRep() == (TeGeomRep)0x4)
				cdev = false;
			for(i=0; i < (int)xorPointArrayVec_.size(); i++)
				canvas_->plotXorPolyline(xorPointArrayVec_[i], cdev);

			if(mainWindow_->distanceMeterAction->isOn())
				canvas_->plotXorTextDistance(xorPointArrayVec_, unitConv_, unit_);
			//if(xorPointArrayVec_.empty()==false && mainWindow_->distanceMeterAction->isOn())
			//{
			//	QPoint a = xorPointArrayVec_[0].point(0);
			//	QPoint b = xorPointArrayVec_[0].point(1);
			//	TeCoord2D ppa = canvas_->mapVtoCW(a);
			//	TeCoord2D ppb = canvas_->mapVtoCW(b);
			//	double distance = sqrt((ppa.x()-ppb.x()) * (ppa.x()-ppb.x()) + (ppa.y()-ppb.y()) * (ppa.y()-ppb.y()));
			//	string s;
			//	if(unit_ == "Decimal Degrees")
			//		 s = Te2String(distance/unitConv_,6) + "(" + unit_ + ")";
			//	else
			//		 s = Te2String(distance/unitConv_,1) + "(" + unit_ + ")";

			//	double dy = (double)(b.y() - a.y());
			//	double dx = (double)(b.x() - a.x());
			//	distance = sqrt(dx * dx + dy * dy);
			//	double txHeight = -7.;
			//	double dd = distance / 2.;
			//	double alfa = atan(dy/dx);
			//	if(dx<0 && dy<0)
			//		alfa = alfa - TePI;
			//	else if(dy>0 && dx<0)
			//		alfa = TePI + alfa;
			//	double beta = atan(txHeight/dd);
			//	double c = dd * cos(beta);
			//	double x = c * cos(alfa+beta);
			//	x += a.x();
			//	double y = c * sin(alfa+beta);
			//	y += a.y();
			//	QPoint p((int)x, (int)y);
			//	TeCoord2D pp = canvas_->mapVtoDW(p);
			//	alfa = alfa * 180. / TePI;
			//	canvas_->plotText(pp, s, -alfa);
			//}
		}
	}
	else if(xorOn_ && xorMtEdit_.size())
	{
		for(i=0; i < xorMtEdit_.size(); i++)
		{
			TeText txt = xorMtEdit_.text(i);
			canvas_->plotTextRects(txt, xorMtEdit_.visual(i));
		}
	}
	xorOn_ = false;
	if(mode == false)
		return xorOn_;

	if(editOn_)
	{
		if(mtEdit_.size() && layer)
		{
			if(layer->hasGeometry(TeTEXT) && mtEdit_.edit())
			{
				for(i=0; i < mtEdit_.size(); i++)
				{
					TeText txt = mtEdit_.text(i);
					canvas_->plotTextRects(txt, mtEdit_.visual(i));
				}
				xorMtEdit_ = mtEdit_;
				xorOn_ = true;
			}
		}
		else if(appTheme->editRep() == (TeGeomRep)0x20000000) // legend
		{
			TeCoord2D p = appTheme->canvasLegUpperLeft();
			QPoint qp((int)p.x(), (int)p.y());
			double w = appTheme->canvasLegWidth();
			double h = appTheme->canvasLegHeight();
			xorPointArrayVec_.clear();
			QPointArray par(5);
			par.setPoint(0, qp);
			par.setPoint(1, qp.x(), (int)(qp.y() + h));
			par.setPoint(2, (int)(qp.x() + w), (int)(qp.y() + h));
			par.setPoint(3, (int)(qp.x() + w), qp.y());
			par.setPoint(4, qp);
			xorPointArrayVec_.push_back(par);

			for(i=0; i < (int)xorPointArrayVec_.size(); i++)
				canvas_->plotXorPolyline(xorPointArrayVec_[i]);
			xorOn_ = true;
		}
		else if(appTheme->editRep() == (TeGeomRep)0x10000000) // graphic scale
		{
			QRect r = canvas_->graphicScaleRect();
			QPoint dif = mGraphicScale_ - pGraphicScale_;

			xorPointArrayVec_.clear();
			QPointArray par(5);
			par.setPoint(0, r.left()+dif.x(), r.top()+dif.y());
			par.setPoint(1, r.left()+dif.x(), r.bottom()+dif.y());
			par.setPoint(2, r.right()+dif.x(), r.bottom()+dif.y());
			par.setPoint(3, r.right()+dif.x(), r.top()+dif.y());
			par.setPoint(4, r.left()+dif.x(), r.top()+dif.y());
			xorPointArrayVec_.push_back(par);

			for(i=0; i < (int)xorPointArrayVec_.size(); i++)
				canvas_->plotXorPolyline(xorPointArrayVec_[i]);
			xorOn_ = true;
		}
		else if(appTheme->editRep() == (TeGeomRep)0x80000000 && layer) // piechart and barchart
		{
			if(appTheme->chartSelected_.empty() == false)
			{
				double dx, dy;
				TeBox box = pieBarBoxEdit_;
				TeChartType chartType = (TeChartType)appTheme->chartType();
				if(chartType == TePieChart)
				{
					TeCoord2D center(box.x1_ + box.width() / 2., box.y1_ + box.height() / 2.);
					TeCoord2D ncenter = appTheme->chartPoint_;
					dx = ncenter.x() - center.x();
					dy = ncenter.y() - center.y();
				}
				else
				{
					TeCoord2D bcenter(box.x1_ + box.width() / 2., box.y1_);
					TeCoord2D nbcenter = appTheme->chartPoint_;
					dx = nbcenter.x() - bcenter.x();
					dy = nbcenter.y() - bcenter.y();
				}
				box.x1_ = box.x1_ + dx;
				box.x2_ = box.x2_ + dx;
				box.y1_ = box.y1_ + dy;
				box.y2_ = box.y2_ + dy;

				if(box.isValid() == false)
					appTheme->chartSelected_.clear();
				else
				{
					xorPointArrayVec_.clear();
					QPointArray par(5);
					TeCoord2D ll = box.lowerLeft();
					TeCoord2D ul(box.x1_, box.y2_);
					TeCoord2D ur(box.upperRight());
					TeCoord2D lr(box.x2_, box.y1_);

					canvas_->setDataProjection(layer->projection());
					QPoint offset(0, 0);
					if(canvas_->getPainter()->device() == canvas_->viewport())
						offset = canvas_->offset();
					par.setPoint(0, canvas_->mapDWtoV(ll) + offset);
					par.setPoint(1, canvas_->mapDWtoV(ul) + offset);
					par.setPoint(2, canvas_->mapDWtoV(ur) + offset);
					par.setPoint(3, canvas_->mapDWtoV(lr) + offset);
					par.setPoint(4, canvas_->mapDWtoV(ll) + offset);
					xorPointArrayVec_.push_back(par);

					for(i=0; i < (int)xorPointArrayVec_.size(); i++)
						canvas_->plotXorPolyline(xorPointArrayVec_[i]);
					xorOn_ = true;
				}
			}
		}
		else if(appTheme->editRep() == (TeGeomRep)0x4 && layer) // TePoint
		{
			double dx, dy;
			TeBox box = pointBoxEdit_;
			TeCoord2D center(box.x1_ + box.width() / 2., box.y1_ + box.height() / 2.);
			TeCoord2D ncenter = appTheme->tePoint_.location();
			dx = ncenter.x() - center.x();
			dy = ncenter.y() - center.y();
			box.x1_ = box.x1_ + dx;
			box.x2_ = box.x2_ + dx;
			box.y1_ = box.y1_ + dy;
			box.y2_ = box.y2_ + dy;

			xorPointArrayVec_.clear();
			QPointArray par(5);
			TeCoord2D ll = box.lowerLeft();
			TeCoord2D ul(box.x1_, box.y2_);
			TeCoord2D ur(box.upperRight());
			TeCoord2D lr(box.x2_, box.y1_);

			canvas_->setDataProjection(layer->projection());
			QPoint offset(0, 0);
			if(canvas_->getPainter()->device() == canvas_->viewport())
				offset = canvas_->offset();
			par.setPoint(0, canvas_->mapDWtoV(ll) + offset);
			par.setPoint(1, canvas_->mapDWtoV(ul) + offset);
			par.setPoint(2, canvas_->mapDWtoV(ur) + offset);
			par.setPoint(3, canvas_->mapDWtoV(lr) + offset);
			par.setPoint(4, canvas_->mapDWtoV(ll) + offset);
			xorPointArrayVec_.push_back(par);

			for(i=0; i < (int)xorPointArrayVec_.size(); i++)
				canvas_->plotXorPolyline(xorPointArrayVec_[i], false);
			xorOn_ = true;
		}
	}
	else
	{
		if(graphCursorOn_)
		{
			QPoint c(cGraphCursor_);
			xorPointArrayVec_.clear();
			if(popupSpatialCursorType_->isItemChecked(popupSpatialCursorType_->idAt(0)))
			{
				int w = wGraphCursor_;
				int h = hGraphCursor_;
				QPointArray par(5);
				par.setPoint(0, c.x()-w/2, c.y()-h/2);
				par.setPoint(1, c.x()+w/2, c.y()-h/2);
				par.setPoint(2, c.x()+w/2, c.y()+h/2);
				par.setPoint(3, c.x()-w/2, c.y()+h/2);
				par.setPoint(4, c.x()-w/2, c.y()-h/2);
				xorPointArrayVec_.push_back(par);
			}
			else
			{
				QPoint a = c;
				a.setX(a.x() + circleRadius_);
				int rx = (int)((double)wGraphCursor_ / 2. +.5);
				int ry = (int)((double)hGraphCursor_ / 2. +.5);
				QPointArray pa = getElipse(c, rx, ry);
				xorPointArrayVec_.push_back(pa);
			}
			vector<QPointArray> pVec = rotateXorPoints();
			for(i=0; i < (int)pVec.size(); i++)
				canvas_->plotXorPolyline(pVec[i]);
		}
		else if(xorPointArrayVec_.size() && mainWindow_->distanceMeterAction->isOn())
		{
			QPoint a = xorPointArrayVec_[0].point(0);
			xorPointArrayVec_.clear();
			QPointArray par(2);
			par.setPoint(0, a);
			par.setPoint(1, distancePoint_);
			xorPointArrayVec_.push_back(par);
			QPointArray pa = getCircle(a, distancePoint_);
			xorPointArrayVec_.push_back(pa);

			for(i=0; i < (int)xorPointArrayVec_.size(); i++)
				canvas_->plotXorPolyline(xorPointArrayVec_[i]);

			if(mainWindow_->distanceMeterAction->isOn())
				canvas_->plotXorTextDistance(xorPointArrayVec_, unitConv_, unit_);
		//	QPoint b = xorPointArrayVec_[0].point(1);
		//	TeCoord2D ppa = canvas_->mapVtoCW(a);
		//	TeCoord2D ppb = canvas_->mapVtoCW(b);
		//	double distance = sqrt((ppa.x()-ppb.x()) * (ppa.x()-ppb.x()) + (ppa.y()-ppb.y()) * (ppa.y()-ppb.y()));
		//	string s;
		//	if(unit_ == "Decimal Degrees")
		//			s = Te2String(distance/unitConv_,6) + "(" + unit_ + ")";
		//	else
		//			s = Te2String(distance/unitConv_,1) + "(" + unit_ + ")";

		//	double dy = (double)(b.y() - a.y());
		//	double dx = (double)(b.x() - a.x());
		//	distance = sqrt(dx * dx + dy * dy);
		//	double txHeight = -7.;
		//	double dd = distance / 2.;
		//	double alfa = atan(dy/dx);
		//	if(dx<0 && dy<0)
		//		alfa = alfa - TePI;
		//	else if(dy>0 && dx<0)
		//		alfa = TePI + alfa;
		//	double beta = atan(txHeight/dd);
		//	double c = dd * cos(beta);
		//	double x = c * cos(alfa+beta);
		//	x += a.x();
		//	double y = c * sin(alfa+beta);
		//	y += a.y();
		//	QPoint p((int)x, (int)y);
		//	TeCoord2D pp = canvas_->mapVtoDW(p);
		//	alfa = alfa * 180. / TePI;
		//	canvas_->plotText(pp, s, -alfa);
		//
		}
		xorOn_ = true;
	}
	canvas_->plotOnPixmap0();
	return xorOn_;
}


void DisplayWindow::plotGraphicScale()
{
	TeView*	view = mainWindow_->currentView();
	if(view == 0)
		return;

	TeDatabase* db = mainWindow_->currentDatabase();
	
	if(!db->tableExist("graphic_scale"))
	{
		return;
	}

	TeVisual visual;

	TeDatabasePortal* portal  = db->getPortal();

	std::string sel = "SELECT * FROM graphic_scale WHERE view_id = ";
	sel += Te2String(view->id());

	portal->freeResult();
	if(portal->query(sel) == false)
	{
		delete portal;
		return;
	}
	if(portal->fetchRow() == false)
	{
		portal->freeResult();

		string ins = "INSERT INTO graphic_scale (view_id, font_family, red, green, blue, bold, italic, font_size, visible)";
		ins += " VALUES (" + Te2String(view->id()) + ", 'verdana', 0, 0, 0, 0, 0, 10, 1)";
		if (db->execute(ins) == false)
		{
			QMessageBox::critical(this, tr("Error"), 
				tr("Fail to insert a value into the graphic_scale table!"));
			delete portal;
			return;
		}

		visual.family("verdana");
		TeColor cor(0, 0, 0);
		visual.color(cor);
		visual.bold(false);
		visual.italic(false);
		visual.size(10);
	}
	else
	{
		bool visible = (bool)(portal->getInt(8));
		graphicScaleVisible_ = visible;

		string family = portal->getData(1);
		int red = portal->getInt(2);
		int green = portal->getInt(3);
		int blue = portal->getInt(4);
		bool bold = (bool)(portal->getInt(5));
		bool italic = (bool)(portal->getInt(6));
		int size = portal->getInt(7);

		visual.family(family);
		TeColor cor(red, green, blue);
		visual.color(cor);
		visual.bold(bold);
		visual.italic(italic);
		visual.size(size);
	}
	delete portal;
	canvas_->plotOnPixmap0();
	double scaleGraphUnitConvFactor = 1.;
	QString scaleGraphUnit = view->projection()->units().c_str();
	if (scaleGraphUnit == "DecimalDegrees")  // From decimal degrees to...
	{
		if (SGUnit_==1)	// kilometers
		{
			scaleGraphUnit = tr("Kilometers");
			scaleGraphUnitConvFactor = 0.00900901;
		}
		else if (SGUnit_==2) // meters
		{
			scaleGraphUnit = tr("Meters");
			scaleGraphUnitConvFactor = 0.00000900901;
		}
		else if (SGUnit_== 3) // feets
		{
			scaleGraphUnit = tr("Feets");
			scaleGraphUnitConvFactor = 1./364173.2289;
		}
		else				// decimal degrees
		{
			scaleGraphUnit = tr("Decimal Degrees");
			scaleGraphUnitConvFactor = 1.;
		}
	}
	else if (scaleGraphUnit == "Meters")// From Meters to...
	{
		if (SGUnit_ == 0) // Decimal degrees
		{
			scaleGraphUnit = tr("Decimal Degrees");
			scaleGraphUnitConvFactor = 111000;
		}
		if (SGUnit_ == 1)	// Kilometers
		{
			scaleGraphUnit = tr("Kilometers");
			scaleGraphUnitConvFactor = 1000;
		}
		else if (SGUnit_ == 2) // Meters
		{
			scaleGraphUnit = tr("Meters");
			scaleGraphUnitConvFactor = 1;
		}
		else if (SGUnit_ == 3) // 
		{
			scaleGraphUnit = tr("Feets");
			scaleGraphUnitConvFactor = 1./3.2808399;
		}
	}
    
  if( graphicScaleVisible_ )
  {  
    canvas_->plotGraphicScale(visual, graphicScaleOffsetX_, graphicScaleOffsetY_,scaleGraphUnitConvFactor,scaleGraphUnit.latin1());
  }
	QRect &rec = canvas_->graphicScaleRect();
	QRect recp = canvas_->getPixmap0()->rect();
	recp = recp.intersect(rec);
	if(recp.width() < 20 || recp.height() < 20)
	{
		canvas_->copyPixmap1ToPixmap0(rec.x(), rec.y(), rec.width(), rec.height());
		TeBox rb(rec.x(), rec.y(), rec.right(), rec.bottom());
		canvas_->mapVtoCW(rb);
		canvas_->mapCWtoDW(rb);
		TeAppTheme* currentTheme = mainWindow_->currentTheme();
		TeQtProgress progress(this, "ProgressDialog", true);
		if(currentTheme)
		{
			plotPieBars(currentTheme, canvas_, &progress, rb);
			TePlotTextWV(currentTheme, canvas_, &progress, rb);
			plotLegendOnCanvas (currentTheme);
		}
		graphicScaleOffsetX_ = 0.;
		graphicScaleOffsetY_ = 0.;
		rec = QRect();
		
		if( graphicScaleVisible_ )
		{
		  canvas_->plotGraphicScale(visual, graphicScaleOffsetX_, graphicScaleOffsetY_,scaleGraphUnitConvFactor,scaleGraphUnit.latin1());
		}
	}
	unit_ = scaleGraphUnit.latin1();
	unitConv_ = scaleGraphUnitConvFactor;
}

void DisplayWindow::graphicScaleVisualSlot( int view_id )
{
	TeDatabase* db = mainWindow_->currentDatabase();
	TeVisual scaleVisual;

	TeDatabasePortal* portal  = db->getPortal ();
	string sel = "SELECT * FROM graphic_scale WHERE view_id = ";
	sel += Te2String(view_id);
	if(portal->query(sel) == false)
	{
		delete portal;
		return;
	}
	if(portal->fetchRow() == false)
	{
		delete portal;
		return;
	}
	else
	{
		scaleVisual.family(portal->getData(1));
		int r = portal->getInt(2);
		int g = portal->getInt(3);
		int b = portal->getInt(4);
		TeColor cor(r, g, b);
		scaleVisual.color(cor);
		scaleVisual.bold((bool)(portal->getInt(5)));
		scaleVisual.italic((bool)(portal->getInt(6)));
		scaleVisual.size(portal->getInt(7));
	}
	delete portal;

	VisualWindow *visualWindow = new VisualWindow(this, "visualWindow",true);
	visualWindow->setCaption(tr("Graphic Scale Visual"));
	TeLegendEntry graphicScaleLegend;

	TeGeomRepVisualMap& vmap = graphicScaleLegend.getVisualMap();
	vmap[TeTEXT] = scaleVisual.copy();
	visualWindow->loadLegend(graphicScaleLegend, 0);

	visualWindow->exec();
	delete visualWindow;

	TeVisual* visual = vmap[TeTEXT];
	TeColor cor = visual->color();
	string upd = "UPDATE graphic_scale  SET ";
	upd += "font_family = '" + visual->family() + "', ";
	upd += "red = " + Te2String(cor.red_) + ", ";
	upd += "green = " + Te2String(cor.green_) + ", ";
	upd += "blue = " + Te2String(cor.blue_) + ", ";
	upd += "bold = " + Te2String(visual->bold()) + ", ";
	upd += "italic = " + Te2String(visual->italic()) + ", ";
	upd += "font_size = " + Te2String(visual->size());
	upd += " WHERE view_id = " + Te2String(view_id);
	db->execute(upd);
	plotData();
}


void DisplayWindow::plotGeographicalGrid()
{
	TeView*	view = mainWindow_->currentView();
	if(view == 0)
		return;

	TeDatabase* db = mainWindow_->currentDatabase();
	if(!db->tableExist("app_geogrid"))
	{
		return;
	}

	int		 MAXLINES = 100; // draw max lines
	int		 precision = 1;
	int		 j;
	int		 i;
	double	 dx, dy;
	bool	 deltaCalc = true;
	bool	 txLatHorizontal = true;
	bool	 txLonHorizontal = true;
	bool	 vis, leftVis, rightVis, topVis, bottomVis;
	TeVisual visual;
	QColor	 lineCor;
	int		 xGrausOffset = 0;
	int		 xMinsOffset = 0;
	double	 xSecsOffset = 0.;
	int		 yGrausOffset = 0;
	int		 yMinsOffset = 0;
	double	 ySecsOffset = 0.;
	int		 deltaGraus = 0;
	int		 deltaMins = 0;
	double	 deltaSecs = 0.;

	vis = false;

	
	TeDatabasePortal* portal  = db->getPortal();
	string sel = "SELECT visible FROM app_geogrid WHERE view_id = " + Te2String(view->id());
	if(portal->query(sel))
	{
		if(portal->fetchRow())
		{
			vis = portal->getInt(0);
			if(vis == false)
			{
				delete portal;
				return;
			}
		}
	}	

	TeQtViewItem* viewItem = mainWindow_->getViewsListView()->getViewItem(view);
	vector<TeQtThemeItem*> themeItemVec = viewItem->getThemeItemVec();
	for (i = (int)themeItemVec.size() - 1; i > 0 ; --i)
	{
		TeTheme* theme = (TeTheme*)(themeItemVec[i]->getAppTheme())->getTheme();
		if(theme->visibility() != 0)
		{
			vis = true;
			break;
		}
	}

	if(vis == false)
	{
		delete portal;
		return;
	}

	sel = "SELECT * FROM app_geogrid WHERE view_id = ";
	sel += Te2String(view->id());
	portal->freeResult();
	if(portal->query(sel) && portal->fetchRow())
	{
		bool visible = (bool)(portal->getInt(8));
		if(visible == false)
		{
			portal->freeResult();
			delete portal;
			return;
		}

		string family = portal->getData(1);
		int red = portal->getInt(2);
		int green = portal->getInt(3);
		int blue = portal->getInt(4);
		bool bold = (bool)(portal->getInt(5));
		bool italic = (bool)(portal->getInt(6));
		int size = portal->getInt(7);

		visual.family(family);
		TeColor cor(red, green, blue);
		visual.color(cor);
		visual.bold(bold);
		visual.italic(italic);
		visual.size(size);

		leftVis = (bool)(portal->getInt(9));
		rightVis = (bool)(portal->getInt(10));
		topVis = (bool)(portal->getInt(11));
		bottomVis = (bool)(portal->getInt(12));
		lineCor = QColor(portal->getInt(13), portal->getInt(14), portal->getInt(15));
		deltaCalc = (bool)(portal->getInt(16));
		deltaGraus = portal->getInt(17);
		deltaMins = portal->getInt(18);
		deltaSecs = portal->getDouble(19);
		xGrausOffset = portal->getInt(20);
		xMinsOffset = portal->getInt(21);
		xSecsOffset = portal->getDouble(22);
		yGrausOffset = portal->getInt(23);
		yMinsOffset = portal->getInt(24);
		ySecsOffset = portal->getDouble(25);
		txLatHorizontal = (bool)(portal->getInt(26));
		txLonHorizontal = (bool)(portal->getInt(27));
		precision = portal->getInt(28);
	}
	else
	{
		delete portal;
		return;
	}

	delete portal;

	TeBox viewBox = canvas_->getWorld();

	TeProjection* proj = canvas_->projection();
	if(proj->name() == "NoProjection")
	{
		QString msg = tr("Without projection! Grid will not be ploted!") + "\n";
		msg += tr("Do you want to turn off it or continue?");
		int ret = QMessageBox::question(this, tr("Question"), msg, "Turn Off", "Continue");

		if(ret == 0) // turn off
		{
			string up = "UPDATE app_geogrid SET visible = 0 WHERE view_id = " + Te2String(view->id());
			db->execute(up);
		}
		return;
	}
	canvas_->projection()->setDestinationProjection(canvas_->projection());
 
	TeCoord2D lowerLeft = viewBox.lowerLeft();
	TeCoord2D p1 = canvas_->projection()->PC2LL(lowerLeft);
	TeCoord2D upperRight = viewBox.upperRight();
	TeCoord2D p2 = canvas_->projection()->PC2LL(upperRight);

	double dh = (viewBox.upperRight().x() - viewBox.lowerLeft().x()) / 100;
	double dv = (viewBox.upperRight().y() - viewBox.lowerLeft().y()) / 100;
	TeCoord2D pv = viewBox.lowerLeft();

	int wcount = 0;
#ifdef WIN32
	while(_isnan(p1.x()) != 0 || _isnan(p1.y()) != 0)
	{
		pv.x(pv.x() + dh);
		pv.y(pv.y() + dv);
		p1 = canvas_->projection()->PC2LL(pv);
		if(++wcount == 1000)
			return;
	}

	wcount = 0;
	pv = viewBox.upperRight();
	while(_isnan(p2.x()) != 0 || _isnan(p2.y()) != 0)
	{
		pv.x(pv.x() - dh);
		pv.y(pv.y() - dv);
		p2 = canvas_->projection()->PC2LL(pv);
		if(++wcount == 1000)
			return;
	}
#else
	while(isnan(p1.x()) != 0 || isnan(p1.y()) != 0)
	{
		pv.x(pv.x() + dh);
		pv.y(pv.y() + dv);
		p1 = canvas_->projection()->PC2LL(pv);
		if(++wcount == 1000)
			return;
	}

	wcount = 0;
	pv = viewBox.upperRight();
	while(isnan(p2.x()) != 0 || isnan(p2.y()) != 0)
	{
		pv.x(pv.x() - dh);
		pv.y(pv.y() - dv);
		p2 = canvas_->projection()->PC2LL(pv);
		if(++wcount == 1000)
			return;
	}
#endif

	if(p1.x() <= -TePI)
		p1.x(-TePI);
	if(p1.y() <= -TePI/2.)
		p1.y(-TePI/2.);

	if(p2.x() >= TePI)
		p2.x(TePI);
	if(p2.y() >= TePI/2.)
		p2.y(TePI/2.);

	double width = (p2.x() - p1.x()) * TeCRD;
	double height = (p2.y() - p1.y()) * TeCRD;

	if(deltaCalc)
	{
		int idx;
		dx = width / 10;
		if(log10(dx) > 0)
			idx = (int)(log10(dx) +.5);
		else
			idx = (int)(log10(dx) -.5);
		if(idx < 0)
			idx--;
		dx = dx / pow(10., idx);

		if(dx > 5)
			dx = 10 * pow(10., idx);
		else if(dx > 4 && dx <= 5)
			dx = 5 * pow(10., idx);
		else if(dx > 2 && dx <= 4)
			dx = 4 * pow(10., idx);
		else if(dx > 1 && dx <= 2)
			dx = 2 * pow(10., idx);
		else
			dx = pow(10., idx);
		while((fabs(width)/dx) > 10)
			dx *= 2;
		while((fabs(width)/dx) < 10  && dx > 1.0e-50)
			dx /= 2;
		if(dx <= 1.0e-50)
		{
			QString msg = tr("Geographical grid has too many lines and will not be ploted!") + "\n";
			msg += tr("Do you want to turn off it or continue?");
			int ret = QMessageBox::question(this, tr("Question"), msg, "Turn Off", "Continue");

			if(ret == 0) // turn off
			{
				string up = "UPDATE app_geogrid SET visible = 0 WHERE view_id = " + Te2String(view->id());
				db->execute(up);
			}
			return;
		}

		int xgraus = int(dx);
		int xmins = (int)((dx - (double)xgraus) * 60);
		double xsecs = (((dx - (double)xgraus) * 60) - (double)xmins) * 60;

		if(xgraus > 0)
		{
			if(xgraus <= 1)
				xgraus = 1;
			else if(xgraus <= 2)
				xgraus = 2;
			else if(xgraus <= 3)
				xgraus = 3;
			else if(xgraus <= 4)
				xgraus = 4;
			else if(xgraus <= 5)
				xgraus = 5;
			else if(xgraus <= 6)
				xgraus = 6;
			else if(xgraus <= 10)
				xgraus = 10;
			else if(xgraus <= 20)
				xgraus = 20;
			else
				xgraus = 30;
			xmins = 0;
			xsecs = 0;
		}
		else if(xmins > 0)
		{
			if(xmins <= 1)
				xmins = 1;
			else if(xmins <= 2)
				xmins = 2;
			else if(xmins <= 3)
				xmins = 3;
			else if(xmins <= 4)
				xmins = 4;
			else if(xmins <= 5)
				xmins = 5;
			else if(xmins <= 6)
				xmins = 6;
			else if(xmins <= 10)
				xmins = 10;
			else if(xmins <= 20)
				xmins = 20;
			else
				xmins = 30;
			xsecs = 0;
		}
		else if(xsecs > 1)
		{
			if(xsecs <= 2)
				xsecs = 2;
			else if(xsecs <= 3)
				xsecs = 3;
			else if(xsecs <= 4)
				xsecs = 4;
			else if(xsecs <= 5)
				xsecs = 5;
			else if(xsecs <= 6)
				xsecs = 6;
			else if(xsecs <= 10)
				xsecs = 10;
			else if(xsecs <= 20)
				xsecs = 20;
			else
				xsecs = 30;
		}
		else
		{
			double a = -log10(xsecs);
			double b = xsecs * pow(10., a+1.);

			xsecs = 10. / pow(10., a+1.);
			if(fabs(5.-b) < fabs(10.-b))
				xsecs = 5. / pow(10., a+1.);
			if(fabs(2.5-b) < fabs(5.-b))
				xsecs = 2.5 / pow(10., a+1.);
			if(fabs(2.-b) < fabs(2.5-b))
				xsecs = 2. / pow(10., a+1.);
			if(fabs(1.-b) < fabs(2.-b))
				xsecs = 1. / pow(10., a+1.);
		}

		dx = (double)xgraus + (double)xmins / 60. + xsecs / 3600.;
	}
	else
		dx = (double)deltaGraus + (double)deltaMins / 60. + deltaSecs / 3600.;

	dy = dx;

	double xini = (int)(p1.x() * TeCRD /dx) * dx;
	double xxini = xini;
	double xOffset =  (double)xGrausOffset + (double)xMinsOffset / 60. + xSecsOffset / 3600.;
	xini += xOffset;
	while(xini < xxini)
		xini += dx;
	xini -= dx;
	double xfinal = p2.x() * TeCRD;


	double yini = (int)(p1.y() * TeCRD /dy) * dy;
	double yyini = yini;
	double yOffset =  (double)yGrausOffset + (double)yMinsOffset / 60. + ySecsOffset / 3600.;
	yini += yOffset;
	while(yini < yyini)
		yini += dy;
	yini -= dy;
	double yfinal = p2.y() * TeCRD;

	if(((height) / dy) > MAXLINES || ((width) / dx) > MAXLINES)
	{
		if(deltaCalc)
		{
			QString msg = tr("Geographical grid has too many lines and will not be ploted!") + "\n";
			msg += tr("Do you want to turn off it or continue?");
			int ret = QMessageBox::question(this, tr("Question"), msg, "Turn Off", "Continue");

			if(ret == 0) // turn off
			{
				string up = "UPDATE app_geogrid SET visible = 0 WHERE view_id = " + Te2String(view->id());
				db->execute(up);
			}
			return;
		}

		QString msg = tr("Geographical grid has too many lines!") + "\n";
		msg += tr("Do you want to turn off it or change to calculate it automatically or continue?");
		int ret = QMessageBox::question(this, tr("Question"), msg, "Turn Off", "Change to Auto", "Continue");

		if(ret == 0) // turn off
		{
			string up = "UPDATE app_geogrid SET visible = 0 WHERE view_id = " + Te2String(view->id());
			db->execute(up);
			return;
		}
		else if(ret == 1) // change to auto
		{
			string up = "UPDATE app_geogrid SET delta_calc = 1 WHERE view_id = " + Te2String(view->id());
			db->execute(up);
			plotGeographicalGrid();
			return;
		}
		else //continue
		{
			if(((height) / dy) > (100*MAXLINES) || ((width) / dx) > (100*MAXLINES))
			{
				QMessageBox::information(this, tr("Geographical Grid"), tr("Geographical grid has too many lines! Grid will not be ploted"));
				return;
			}
		}
	}

	canvas_->plotOnPixmap0();
	QPainter *painter = canvas_->getPainter();
	QColor txCor(visual.color().red_, visual.color().green_, visual.color().blue_);

	int txHeight = visual.size();
	txHeight = (int)((double)txHeight * canvas_->printerFactor() + .5);

	QFont font(visual.family().c_str(), txHeight);
	font.setBold(visual.bold());
	font.setItalic(visual.italic());
	painter->setFont(font);
	QFontMetrics fm(font);
	QRect rect;

	int nn = 20;
	QPointArray pa(nn);
	QPoint p;

	vector<QRect> rectVec;
	double x = xini;
	bool xskip = false;
	painter->setPen(lineCor);
	double idx = dx;
	while(xskip == false)
	{
		while(x<xfinal)
		{
			if(x >= p1.x() * TeCRD)
			{
				QPoint pi, pf;
				for(j=0; j<nn; ++j)
				{
					double y = p1.y() * TeCRD + (height/(nn-1)) * j;
					if(y > yfinal)
						y = yfinal;
					TeCoord2D c(x * TeCDR, y * TeCDR);
					c = canvas_->projection()->LL2PC(c);
					p = canvas_->mapCWtoV(c);
					if(j == 0)
						pi = p;
					pf = p;
					pa.setPoint(j, p);
					if(y == yfinal)
					{
						painter->drawPolyline(pa, 0, j+1);
						break;
					}
				}
				if(j == nn)
					painter->drawPolyline(pa);

				if(txLonHorizontal && xskip == false)
				{
					int xgraus = int(x);
					int xmins = (int)((x - (double)xgraus) * 60);
					double xsecs = (((x - (double)xgraus) * 60) - (double)xmins) * 60;
					xsecs = atof(Te2String(xsecs, precision).c_str());
					if(abs(xmins) == 60)
					{
						xmins = 0;
						if(xgraus > 0)
							xgraus++;
						else
							xgraus--;
					}
					if(fabs(xsecs) == 60)
					{
						xsecs = 0;
						if(xmins > 0)
							xmins++;
						else
							xmins--;

						if(abs(xmins) == 60)
						{
							xmins = 0;
							if(xgraus > 0)
								xgraus++;
							else
								xgraus--;
						}
					}

					QString ss;
					if(xgraus >= 0 && xmins >= 0 && xsecs >= 0)
						ss += tr("E") + Te2String(xgraus).c_str();
					else
						ss += tr("W") + Te2String(-xgraus).c_str();
					ss += ":";
					ss += Te2String(abs(xmins)).c_str();
					ss += ":";
					ss += Te2String(fabs(xsecs), precision).c_str();

					rect = fm.boundingRect(ss);
					pi.setY(pi.y() - (txHeight+1));
					rect.moveCenter(pi);
					QRect rec = rect;
					rec.setWidth((int)(rec.width() * 1.3));
					rec.moveCenter(pi);
					for(i=0; i<(int)rectVec.size(); ++i)
					{
						if(rec.intersects(rectVec[i]))
						{
							xskip = true;
							break;
						}
					}
					rectVec.push_back(rect);

					pf.setY(pf.y() + (txHeight-1));
					rect.moveCenter(pf);
					rec = rect;
					rec.setWidth((int)(rec.width() * 1.3));
					rec.moveCenter(pf);
					for(i=0; i<(int)rectVec.size(); ++i)
					{
						if(rec.intersects(rectVec[i]))
						{
							xskip = true;
							break;
						}
					}
					rectVec.push_back(rect);
				}
			}

			x += dx;
			if(((xfinal - x) / dx) > 10000)
				return;
		}

		if(xskip)
		{
			dx += idx;
			x = xini;
			xskip = false;
			rectVec.clear();
		}
		else
			break;
	}


	rectVec.clear();

	double y = yini;
	double idy = dy;
	bool yskip = false;
	while(yskip == false)
	{
		while(y<yfinal)
		{
			if(y >= p1.y() * TeCRD)
			{
				QPoint pi, pf;
				painter->setPen(lineCor);
				for(j=0; j<nn; ++j)
				{
					double x = p1.x() * TeCRD + (width/(nn-1)) * j;
					if(x > xfinal)
						x = xfinal;
					TeCoord2D c(x * TeCDR, y * TeCDR);
					c = canvas_->projection()->LL2PC(c);
					p = canvas_->mapCWtoV(c);
					if(j == 0)
						pi = p;
					pf = p;
					pa.setPoint(j, p);
					if(x == xfinal)
					{
						painter->drawPolyline(pa, 0, j+1);
						break;
					}
				}
				if(j == nn)
					painter->drawPolyline(pa);

				if(txLatHorizontal==false && yskip == false)
				{
					int ygraus = int(y);
					int ymins = (int)((y - (double)ygraus) * 60);
					double ysecs = (((y - (double)ygraus) * 60) - (double)ymins) * 60;
					ysecs = atof(Te2String(ysecs, precision).c_str());
					if(abs(ymins) == 60)
					{
						ymins = 0;
						if(ygraus > 0)
							ygraus++;
						else
							ygraus--;
					}
					if(fabs(ysecs) == 60)
					{
						ysecs = 0;
						if(ymins > 0)
							ymins++;
						else
							ymins--;

						if(abs(ymins) == 60)
						{
							ymins = 0;
							if(ygraus > 0)
								ygraus++;
							else
								ygraus--;
						}
					}

					QString ss;
					if(ygraus >= 0 && ymins >= 0 && ysecs >= 0)
						ss += tr("N") + Te2String(ygraus).c_str();
					else
						ss += tr("S") + Te2String(-ygraus).c_str();
					ss += ":";
					ss += Te2String(abs(ymins)).c_str();
					ss += ":";
					ss += Te2String(fabs(ysecs), precision).c_str();

					rect = fm.boundingRect(ss);

					pi.setX((int)(pi.x() + rect.height()/1.3));
					rect.moveCenter(pi);
					QRect r(0, pi.y()-rect.width()/2, (int)(rect.height()/1.3), rect.width());
					QPoint pc = r.center();
					QRect rr(r);
					rr.setHeight((int)(r.height()*1.1));
					rr.moveCenter(pc);
					for(i=0; i<(int)rectVec.size(); ++i)
					{
						if(rr.intersects(rectVec[i]))
						{
							yskip = true;
							break;
						}
					}
					rectVec.push_back(rr);
					
					rect.moveCenter(pf);
					r = QRect((int)(pf.x()-rect.height()/1.3), pf.y()-rect.width()/2, (int)(rect.height()/1.3), rect.width());
					pc = r.center();
					rr = r;
					rr.setHeight((int)(r.height()*1.1));
					rr.moveCenter(pc);
					for(i=0; i < (int)rectVec.size(); ++i)
					{
						if(rr.intersects(rectVec[i]))
						{
							yskip = true;
							break;
						}
					}
					rectVec.push_back(rr);
				}
			}

			y += dy;
			if(((yfinal - y) / dy) > 10000)
				return;
		}

		if(yskip)
		{
			dy += idy;
			y = yini;
			yskip = false;
			rectVec.clear();
		}
		else
			break;
	}

	rectVec.clear();

	painter->setPen(txCor);
	x = xini;;
	while(x<xfinal)
	{
		if(x >= p1.x() * TeCRD)
		{
			QPoint pi, pf;
			double y = p1.y() * TeCRD;
			TeCoord2D c(x * TeCDR, y * TeCDR);
			c = canvas_->projection()->LL2PC(c);
			pi = canvas_->mapCWtoV(c);

			y = p2.y() * TeCRD;
			c = TeCoord2D(x * TeCDR, y * TeCDR);
			c = canvas_->projection()->LL2PC(c);
			pf = canvas_->mapCWtoV(c);

			int xgraus = int(x);
			int xmins = (int)((x - (double)xgraus) * 60);
			double xsecs = (((x - (double)xgraus) * 60) - (double)xmins) * 60;
			xsecs = atof(Te2String(xsecs, precision).c_str());
			if(abs(xmins) == 60)
			{
				xmins = 0;
				if(xgraus > 0)
					xgraus++;
				else
					xgraus--;
			}
			if(fabs(xsecs) == 60)
			{
				xsecs = 0;
				if(xmins > 0)
					xmins++;
				else
					xmins--;

				if(abs(xmins) == 60)
				{
					xmins = 0;
					if(xgraus > 0)
						xgraus++;
					else
						xgraus--;
				}
			}

			QString ss;
			if(xgraus >= 0 && xmins >= 0 && xsecs >= 0)
				ss += tr("E") + Te2String(xgraus).c_str();
			else
				ss += tr("W") + Te2String(-xgraus).c_str();
			ss += ":";
			ss += Te2String(abs(xmins)).c_str();
			ss += ":";
			ss += Te2String(fabs(xsecs), precision).c_str();

			rect = fm.boundingRect(ss);
			pi.setY(pi.y() - (txHeight+1));
			pf.setY(pf.y() + (txHeight-1));
			if(txLonHorizontal)
			{
				if(bottomVis)
				{
					rect.moveCenter(pi);
					painter->drawText (rect.left(), rect.bottom(), ss);
					rectVec.push_back(rect);
				}

				if(topVis)
				{
					rect.moveCenter(pf);
					painter->drawText (rect.left(), rect.bottom(), ss);
					rectVec.push_back(rect);
				}
			}
			else
			{
				QRect r;
				if(bottomVis)
				{
					pi.setX(pi.x() + rect.width()/2);
					rect.moveCenter(pi);
					painter->save();
					painter->translate (rect.left(), rect.bottom());
					painter->rotate(-90);
					painter->drawText (0, -2, ss);
					painter->restore();
					r = QRect(rect.x() - (txHeight+3), (int)(rect.y() - rect.width()/1.3), txHeight+1, rect.width());
					rectVec.push_back(r);
				}

				if(topVis)
				{
					pf.setX(pf.x() + rect.width()/2);
					rect.moveCenter(pf);
					painter->save();
					painter->translate (rect.left(), rect.bottom());
					painter->rotate(-90);
					painter->drawText ((int)(-rect.width()/1.2), -2, ss);
					painter->restore();
					r = QRect(rect.x() - (txHeight+3), rect.y(), txHeight+1, (int)(rect.width()*1.1));
					rectVec.push_back(r);
				}
			}
		}

		x += dx;
	}

	y = yini;
	while(y<yfinal)
	{
		if(y >= p1.y() * TeCRD)
		{
			QPoint pi, pf;
			for(j=0; j<nn; ++j)
			{
				double x = p1.x() * TeCRD + (width/(nn-1)) * j;
				if(x > xfinal)
					x = xfinal;
				TeCoord2D c(x * TeCDR, y * TeCDR);
				c = canvas_->projection()->LL2PC(c);
				p = canvas_->mapCWtoV(c);
				if(j == 0)
					pi = p;
				pf = p;
				pa.setPoint(j, p);
				if(x == xfinal)
					break;
			}

			int ygraus = int(y);
			int ymins = (int)((y - (double)ygraus) * 60);
			double ysecs = (((y - (double)ygraus) * 60) - (double)ymins) * 60;
			ysecs = atof(Te2String(ysecs, precision).c_str());
			if(abs(ymins) == 60)
			{
				ymins = 0;
				if(ygraus > 0)
					ygraus++;
				else
					ygraus--;
			}
			if(fabs(ysecs) == 60)
			{
				ysecs = 0;
				if(ymins > 0)
					ymins++;
				else
					ymins--;

				if(abs(ymins) == 60)
				{
					ymins = 0;
					if(ygraus > 0)
						ygraus++;
					else
						ygraus--;
				}
			}

			QString ss;
			if(ygraus >= 0 && ymins >= 0 && ysecs >= 0)
				ss += tr("N") + Te2String(ygraus).c_str();
			else
				ss += tr("S") + Te2String(-ygraus).c_str();
			ss += ":";
			ss += Te2String(abs(ymins)).c_str();
			ss += ":";
			ss += Te2String(fabs(ysecs), precision).c_str();

			rect = fm.boundingRect(ss);
			if(txLatHorizontal)
			{
				if(leftVis)
				{
					pi.setX(pi.x() + rect.width()/2);
					pi.setY(pi.y() - (txHeight-1));
					rect.moveCenter(pi);
					for(i=0; i<(int)rectVec.size(); ++i)
					{
						if(rect.intersects(rectVec[i]))
							break;
					}
					if(i == (int)rectVec.size())
						painter->drawText (rect.left(), rect.bottom(), ss);
				}

				if(rightVis)
				{
					pf.setX(pf.x() - rect.width()/2);
					pf.setY(pf.y() - (txHeight-1));
					rect.moveCenter(pf);
					for(i=0; i<(int)rectVec.size(); ++i)
					{
						if(rect.intersects(rectVec[i]))
							break;
					}
					if(i == (int)rectVec.size())
						painter->drawText (rect.left(), rect.bottom(), ss);
				}
			}
			else
			{
				if(leftVis)
				{
					pi.setX((int)(pi.x() + rect.height()/1.3));
					rect.moveCenter(pi);
					painter->save();
					painter->translate(pi.x(), pi.y());
					painter->rotate(-90);
					QRect r(0, pi.y()-rect.width()/2, (int)(rect.height()/1.3), rect.width());
					for(i=0; i<(int)rectVec.size(); ++i)
					{
						if(r.intersects(rectVec[i]))
							break;
					}
					if(i == (int)rectVec.size())
						painter->drawText (-rect.width()/2, 0, ss);
					painter->restore();
				}

				if(rightVis)
				{
					rect.moveCenter(pf);
					painter->save();
					painter->translate(pf.x(), pf.y());
					painter->rotate(-90);
					QRect r((int)(pf.x()-rect.height()/1.3), pf.y()-rect.width()/2, (int)(rect.height()/1.3), rect.width());
					for(i=0; i<(int)rectVec.size(); ++i)
					{
						if(r.intersects(rectVec[i]))
							break;
					}
					if(i == (int)rectVec.size())
						painter->drawText (-rect.width()/2, 0, ss);
					painter->restore();
				}
			}
		}

		y += dy;
	}

	canvas_->copyPixmap0ToWindow();
}


void DisplayWindow::updateTextEdit()
{
	mtEdit_.change();
	xorMtEdit_.change();
	textRectSelected_ = mtEdit_.getRect();
}


void DisplayWindow::plotData()
{
	TeView*	view = mainWindow_->currentView();
	if(view == 0 || canvas_->getWorld().isValid() == false)
		return;

	canvas_->plotOnPixmap0();
	if(connectedPixmap_)
		delete connectedPixmap_;
	connectedPixmap_ = 0;

	map<int, RepMap>& layerRepMap = mainWindow_->layerId2RepMap_;

	TeAppTheme* currentAppTheme = mainWindow_->currentTheme();
	TeAbstractTheme* currentTheme = (currentAppTheme ? currentAppTheme->getTheme() : 0);

	TeQtGrid* grid = mainWindow_->getGrid();
	grid->refresh();

	int i;
	TeBox box;
	TeQtProgress progress(this, "ProgressDialog", true);
	canvas_->clearAll();	
	canvas_->setProjection(view->projection());

	xorOn_ = false;

	// first step: plot all except current theme when it is vectorial type. Plot in order when current theme is a RASTER.
	TeQtViewItem* viewItem = mainWindow_->getViewsListView()->getViewItem(view);
	vector<TeQtThemeItem*> themeItemVec = viewItem->getThemeItemVec();
	for (i = (int)themeItemVec.size() - 1; i >= 0 ; --i)
	{
		TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
		TeAbstractTheme* theme = appTheme->getTheme(); 

		if(currentAppTheme == appTheme && !(theme->visibleGeoRep() & TeRASTER || theme->visibleGeoRep() & TeRASTERFILE))
			continue;

		if (theme->visibility() && theme->visibleRep())
		{
			canvas_->setDataProjection(theme->getThemeProjection());

			bool draw = false;
			if(canvas_->canvasAndDataProjectionEqual())
			{
				if(TeIntersects(canvas_->getWorld(), theme->box()) == true)
					draw = true;
			}
			else
			{
				TeBox canvasBox = TeRemapBoxPC2Geodetic(canvas_->getWorld(), canvas_->projection());
				TeBox layerBox = TeRemapBoxPC2Geodetic(theme->box(), theme->getThemeProjection());
				if (TeIntersects(canvasBox, layerBox) == true)
					draw = true;

			}
			
			// If there is intersection between layer and canvas, plot the theme
			if(draw)
			{
				TePlotObjects(appTheme, canvas_, layerRepMap, &progress);
				plotPieBars(appTheme, canvas_, &progress);
				TePlotTextWV(appTheme, canvas_, &progress);
			}
			plotLegendOnCanvas(appTheme);
			canvas_->copyPixmap0ToPixmap1();
			canvas_->copyPixmap1ToPixmap2();
		}
	}

	// second step: plot current theme
	if (currentTheme && currentTheme->visibility() && currentTheme->visibleRep())
	{
		if(!(currentTheme->visibleGeoRep() & TeRASTER || currentTheme->visibleGeoRep() & TeRASTERFILE))
		{
			canvas_->setDataProjection(currentTheme->getThemeProjection());

			bool draw = false;
			if(canvas_->canvasAndDataProjectionEqual())
			{
				if(TeIntersects(canvas_->getWorld(), currentTheme->box()) == true)
					draw = true;
			}
			else
			{
				TeBox canvasBox = TeRemapBoxPC2Geodetic(canvas_->getWorld(), canvas_->projection());
				TeBox layerBox = TeRemapBoxPC2Geodetic(currentTheme->box(), currentTheme->getThemeProjection());
				if (TeIntersects(canvasBox, layerBox) == true)
					draw = true;

			}
			
			// If there is intersection between layer and canvas, plot the theme
			if(draw)
			{
				TePlotObjects(currentAppTheme, canvas_, layerRepMap, &progress);
			}
			canvas_->copyPixmap0ToPixmap1();
		}

		// third step: plot only editable elements
		if(currentTheme && currentTheme->visibility())
		{
			plotPieBars(currentAppTheme, canvas_, &progress);
			TePlotTextWV(currentAppTheme, canvas_, &progress);
			plotLegendOnCanvas (currentAppTheme);
		}
	}

//fourth step: plot view graphic scale
	plotGeographicalGrid();
	createGraphicScaleTable();
	plotGraphicScale();

	updateTextEdit();
	if(editOn_ || (graphCursorOn_ && displaySelection_))
		plotXor(true);

	if(mainWindow_->getAddressLocatorWindow() && mainWindow_->getAddressLocatorWindow()->isVisible())
		mainWindow_->getAddressLocatorWindow()->plotPoint();

	canvas_->copyPixmapToWindow();

	plotConnectedView();
	mainWindow_->scaleComboBox->setEnabled(true);
	double scale = (canvas_->scaleApx() + .5);
	char name [ 500 ];
	sprintf ( name, "%.0f", scale );

	mainWindow_->scaleComboBox->setCurrentText(name);

	TeBox wb = canvas_->getWorld();
	view->setCurrentBox(wb);
}

void DisplayWindow::plotConnectedView()
{
	TeView*	curview = mainWindow_->currentView();
	if(curview == 0 || canvas_->getWorld().isValid() == false)
		return;

	int connectId = curview->connectedId();
	if(connectId == 0)
		return;

	TeDatabase* db = mainWindow_->currentDatabase();
	TeViewMap& viewMap = db->viewMap();
	TeView* view = viewMap[connectId];

	map<int, RepMap>& layerRepMap = mainWindow_->layerId2RepMap_;

	int i;
	TeBox box;
	TeLayer* layer;
	TeQtProgress progress(this, "ProgressDialog", true);

	if(connectedPixmap_)
		delete connectedPixmap_;
//	connectedPixmap_ = new QPixmap(canvas_->viewport()->width(),canvas_->viewport()->height());
	connectedPixmap_ = new QPixmap(canvas_->getPixmap1()->width(), canvas_->getPixmap1()->height());
	connectedPixmap_->fill(paletteBackgroundColor());
	QPaintDevice* dev = canvas_->getPainter()->device();
	if(dev)
		canvas_->getPainter()->end();
	canvas_->getPainter()->begin(connectedPixmap_);
	canvas_->setProjection(curview->projection());

	// prepare pixmap2 to plotPolygon with transparency
	QPixmap tmp(canvas_->getPixmap2()->width(), canvas_->getPixmap2()->height());
	if(tmp.width() == 0 || tmp.height() == 0)
	{
		QMessageBox::warning(this, tr("Warning"), 
			tr("There is no memory available for this operation!"));

		delete connectedPixmap_;
		connectedPixmap_ = new QPixmap(1, 1);
		return;
	}

	bitBlt(&tmp, 0, 0, canvas_->getPixmap2(), 0, 0, tmp.width(), tmp.height(), Qt::CopyROP, true);
	int w = connectedPixmap_->width();
	int h = connectedPixmap_->height();

	TeQtViewItem* viewItem = mainWindow_->getViewsListView()->getViewItem(view);
	vector<TeQtThemeItem*> themeItemVec = viewItem->getThemeItemVec();
	for (i = (int)themeItemVec.size() - 1; i >= 0 ; --i)
	{
		TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
		TeTheme* theme = (TeTheme*)appTheme->getTheme();
		if (theme->visibility() && theme->visibleRep())
		{
			if(theme->visibleRep())
			{
				layer = theme->layer();
				canvas_->setDataProjection(layer->projection());

				bool draw = false;
				if(canvas_->canvasAndDataProjectionEqual())
				{
					if(TeIntersects(canvas_->getWorld(), layer->box()) == true)
						draw = true;
				}
				else
				{
					TeBox canvasBox = TeRemapBoxPC2Geodetic(canvas_->getWorld(), canvas_->projection());
					TeBox layerBox = TeRemapBoxPC2Geodetic(layer->box(), layer->projection());
					if (TeIntersects(canvasBox, layerBox) == true)
						draw = true;

				}
				
				// If there is intersection between layer and canvas, plot the theme
				if(draw)
				{
					if((theme->visibleRep() & TePOLYGONS) || (theme->visibleRep() & TeCELLS))
						bitBlt (canvas_->getPixmap2(), 0, 0, connectedPixmap_, 0, 0, w, h, Qt::CopyROP, true);
					TePlotObjects(appTheme, canvas_, layerRepMap, &progress);
					plotPieBars(appTheme, canvas_, &progress);
					TePlotTextWV(appTheme, canvas_, &progress);
				}
				plotLegendOnCanvas(appTheme);
			}
		}
	}

	//vector<TeViewNode*>& themeVector = view->themes();
	//for (i = themeVector.size() - 1; i >= 0; --i)
	//{
	//	TeAppTheme *theme = (TeAppTheme*)themeVector[i];
	//	if (theme->visibility() && theme->visibleRep())
	//	{
	//		if(theme->visibleRep())
	//		{
	//			layer = theme->layer();
	//			canvas_->setDataProjection (layer->projection());
	//			box = canvas_->getDataWorld();

	//			// If there is intersection between layer and canvas, plot the theme
	//			if (TeIntersects(box, layer->box()) == true)
	//			{
	//				if((theme->visibleRep() & TePOLYGONS) || (theme->visibleRep() & TeCELLS))
	//					bitBlt (canvas_->getPixmap2(), 0, 0, connectedPixmap_, 0, 0, w, h, Qt::CopyROP, true);
	//				TePlotObjects(theme, canvas_, layerRepMap, &progress);
	//				plotPieBars(theme, canvas_, &progress);
	//				TePlotTextWV(theme, canvas_, &progress);
	//			}
	//			plotLegendOnCanvas (theme);
	//		}
	//	}
	//}

	// restore pixmap2 
	bitBlt(canvas_->getPixmap2(), 0, 0, &tmp, 0, 0, tmp.width(), tmp.height(), Qt::CopyROP, true);

	canvas_->getPainter()->end();
	if(dev)
		canvas_->getPainter()->begin(dev);

}

void DisplayWindow::plotData(TeBox box, bool docopy) // box in Data World coordinates
{
	TeView*	view = mainWindow_->currentView();
	if(view == 0 || canvas_->getWorld().isValid() == false)
		return;

	map<int, RepMap>& layerRepMap = mainWindow_->layerId2RepMap_;

	TeAppTheme* currentAppTheme = mainWindow_->currentTheme();
	TeTheme* currentTheme = 0;
	if (currentAppTheme)
		currentTheme = (TeTheme*)currentAppTheme->getTheme();

	TeQtGrid* grid = mainWindow_->getGrid();
	grid->refresh();

	int i;
	TeLayer* layer;
	TeQtProgress progress(this, "ProgressDialog", true);

	if(editOn_ || (graphCursorOn_ && displaySelection_))
		plotXor(false);
	xorOn_ = false;

	canvas_->setProjection(view->projection());

	// image recovery from pixmap2
	TeBox bv = box;
	canvas_->mapDWtoCW(bv);
	canvas_->mapCWtoV(bv);
	if(docopy)
	{
//		canvas_->copyPixmap3ToPixmap2((int)bv.x1(), (int)bv.y1(), (int)bv.width(), (int)bv.height());
		canvas_->copyPixmap2ToPixmap1((int)bv.x1(), (int)bv.y1(), (int)bv.width(), (int)bv.height());
		canvas_->copyPixmap1ToPixmap0((int)bv.x1(), (int)bv.y1(), (int)bv.width(), (int)bv.height());
	}
	canvas_->plotOnPixmap0();

	//  plot only vectorial representations
	if (docopy)
	{
		TeQtViewItem* viewItem = mainWindow_->getViewsListView()->getViewItem(view);
		vector<TeQtThemeItem*> themeItemVec = viewItem->getThemeItemVec();
		for (i = (int)themeItemVec.size() - 1; i > 0 ; --i)
		{
			TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
			TeTheme* theme = (TeTheme*)appTheme->getTheme();
			if(currentAppTheme == appTheme && !(theme->visibleGeoRep() & TeRASTER || theme->visibleGeoRep() & TeRASTERFILE))
				continue;

			if (theme->visibility() && theme->visibleRep())
			{
				if((theme->visibleRep() & TeRASTER) == 0)
				{
					layer = theme->layer();
					canvas_->setDataProjection (layer->projection());

					TePlotObjects(appTheme, canvas_, layerRepMap, &progress, box);
					plotPieBars(appTheme, canvas_, &progress, box);
					TePlotTextWV(appTheme, canvas_, &progress, box);
					plotLegendOnCanvas(appTheme);
					canvas_->copyPixmap0ToPixmap1((int)bv.x1(), (int)bv.y1(), (int)bv.width(), (int)bv.height());
					canvas_->copyPixmap1ToPixmap2((int)bv.x1(), (int)bv.y1(), (int)bv.width(), (int)bv.height());
				}
			}
		}
	}


	//vector<TeViewNode*>& themeVector = view->themes();
	//if(docopy)
	//{
	//	for (i = themeVector.size() - 1; i >= 0; --i)
	//	{
	//		TeAppTheme *theme = (TeAppTheme*)themeVector[i];
	//		if(currentTheme == theme)
	//			continue;

	//		if (theme->visibility() && theme->visibleRep())
	//		{
	//			if((theme->visibleRep() & TeRASTER) == 0)
	//			{
	//				layer = theme->layer();
	//				canvas_->setDataProjection (layer->projection());

	//				TePlotObjects(theme, canvas_, layerRepMap, &progress, box);
	//				plotPieBars(theme, canvas_, &progress, box);
	//				TePlotTextWV(theme, canvas_, &progress, box);
	//				plotLegendOnCanvas (theme);
	//				canvas_->copyPixmap0ToPixmap1((int)bv.x1(), (int)bv.y1(), (int)bv.width(), (int)bv.height());
	//				canvas_->copyPixmap1ToPixmap2((int)bv.x1(), (int)bv.y1(), (int)bv.width(), (int)bv.height());
	//			}
	//		}
	//	}
	//}

	if (currentTheme && currentTheme->visibility() && currentTheme->visibleRep())
	{
		if((currentTheme->visibleRep() & TeRASTER) == 0)
		{
			layer = currentTheme->layer();
			canvas_->setDataProjection (layer->projection());

			TePlotObjects(currentAppTheme, canvas_, layerRepMap, &progress, box);
		}
	}
	canvas_->copyPixmap0ToPixmap1((int)bv.x1(), (int)bv.y1(), (int)bv.width(), (int)bv.height());

	// third step: plot only editable elements
	if(currentTheme && currentTheme->visibility())
	{
		if((currentTheme->visibleRep() & TeRASTER) == 0)
		{
			plotPieBars(currentAppTheme, canvas_, &progress, box);
			TePlotTextWV(currentAppTheme, canvas_, &progress, box);
			plotLegendOnCanvas(currentAppTheme);
		}
	}

	//fourth step: plot view graphic scale
	plotGeographicalGrid();
	createGraphicScaleTable();
	plotGraphicScale();

	updateTextEdit();
	if(editOn_ || (graphCursorOn_ && displaySelection_))
		plotXor(true);

	canvas_->copyPixmap0ToWindow((int)bv.x1(), (int)bv.y1(), (int)bv.width(), (int)bv.height());

	TeBox wb = canvas_->getWorld();
	view->setCurrentBox(wb);
}

void DisplayWindow::plotData(TeAppTheme* theme, set<string>& drawSet )
{
	TeQtProgress progress(this, "ProgressDialog", true);
	TeBox objBox;
	double tol = 0.0000001;
	if(drawSetOnCenter_)
	{
		if (objBoxInCanvas(theme, canvas_, drawSet, (TeGeomRep)(((TeTheme*)theme->getTheme())->layer()->geomRep()), objBox, tol) == false)
		{
//			canvas_->setWorld(objBox);
			canvas_->setWorld(objBox, mainWindow_->cvContentsW_, mainWindow_->cvContentsH_);
			plotData();
			return;
		}
	}

	map<int, RepMap>& layerRepMap = mainWindow_->layerId2RepMap_;

	TeQtGrid* grid = mainWindow_->getGrid();
	grid->refresh();
//	canvas_->plotOnPixmap1();
	canvas_->plotOnPixmap0();

// restore legend area
	vector<string> kernelHeader;
	QRect grect = getRectLegendOnCanvas(theme, kernelHeader);
//	bitBlt(canvas_->getPixmap0(), grect.x(), grect.y()-2*printerFactor_, canvas_->getPixmap1(), grect.x(), grect.y()-2*printerFactor_, grect.width()+8*printerFactor_, grect.height()+2*printerFactor_);
	bitBlt(canvas_->getPixmap0(), grect.x(), grect.y()-2, canvas_->getPixmap1(), grect.x(), grect.y()-2, grect.width()+8, grect.height()+2);

	TeBox boxRet = TePlotSelectedObjects(theme, canvas_, drawSet, layerRepMap, &progress);
	TeBox boxw = boxRet;
	canvas_->mapDWtoCW(boxRet);
	canvas_->mapCWtoV(boxRet);
	canvas_->copyPixmap0ToPixmap1((int)boxRet.x1(), (int)boxRet.y1(), (int)boxRet.width(), (int)boxRet.height());
//	canvas_->copyPixmap1ToPixmap0((int)boxRet.x1(), (int)boxRet.y1(), (int)boxRet.width(), (int)boxRet.height());
//	canvas_->plotOnPixmap0();
	plotPieBars(theme, canvas_, &progress, boxw);

//	canvas_->plotOnPixmap0();
	TeCoord2D c = boxw.center();
	double w = boxw.width() / 6;
	double h = boxw.height() / 6;
	boxw.x1_ = c.x() - w;
	boxw.x2_ = c.x() + w;
	boxw.y1_ = c.y() - h;
	boxw.y2_ = c.y() + h;

	TePlotTextWV(theme, canvas_, &progress, boxw);
	plotLegendOnCanvas (theme);
	plotGeographicalGrid();
	createGraphicScaleTable();
	plotGraphicScale();
	updateTextEdit();
	if(editOn_ || (graphCursorOn_ && displaySelection_))
		plotXor(true);

	if(mainWindow_->getAddressLocatorWindow() && mainWindow_->getAddressLocatorWindow()->isVisible())
		mainWindow_->getAddressLocatorWindow()->plotPoint();

	canvas_->copyPixmapToWindow();

	GraphicWindow* graphWindow = mainWindow_->getGraphicWindow();
	if(graphWindow->isVisible())
		graphWindow->applyGraphic();

	Graphic3DWindow* graph3DWindow = mainWindow_->getGraphic3DWindow();
	if(graph3DWindow && graph3DWindow->isVisible())
		graph3DWindow->updateSelection();

	TeBox wb = canvas_->getWorld();
	TeView*	view = mainWindow_->currentView();
	view->setCurrentBox(wb);
}


bool DisplayWindow::legendFromKernel(TeAppTheme *appTheme, vector<string>& kHeader)
{
	TeTheme* theme = (TeTheme*)appTheme->getTheme();
	TeDatabase* db = theme->layer()->database();

	//Verifica se a legenda vem de kernel e monta o string
	TeKernelParams kp;
	bool isKernel = FALSE;

	if (theme->layer()->hasGeometry(TeRASTER) &&
	loadKernelParams(db, theme->layerId(), kp)) 
	{
		isKernel = TRUE;
	}
	else if (!theme->layer()->hasGeometry(TeRASTER)) 
	{
		TeAttributeRep	rep; 
		if(theme->grouping().groupMode_ != TeNoGrouping)
			rep = theme->grouping().groupAttribute_;

		string field = rep.name_;
		string table = field;
		int f = table.find(".");
		if(f >= 0)
		{
			table = table.substr(0, f);
			field = field.substr(f+1);
			if (loadKernelParams(db, theme->id(), table, field, kp))  {
			isKernel = TRUE;
			}
		} 
	}

	//Monta string do titulo do Kernel
	if (isKernel)
	{
   		getKernelLegend(kp,kHeader);
	}
	return isKernel;
}

QRect DisplayWindow::plotLegendKernel(QPoint& pIni, vector<string>& s) {
  
  QRect kRect(pIni, pIni);

  string sTxt;
	if(s.size()) {
     int i;
     for(i=0; i<(int)s.size(); i++) {
						string trTxt;
						string txt;
						int ff = s[i].find(":");
						if(ff == -1)
							trTxt = s[i];
						else
						{
							trTxt = s[i];
							trTxt = trTxt.substr(0, ff+1);
							txt = s[i];
							txt = txt.substr(ff+1);
						}
						
						sTxt = tr(trTxt.c_str()).latin1() + txt;
            canvas_->plotLegend(pIni, (QPixmap*)0, sTxt);
	          kRect |= canvas_->getLegendRect(pIni, (QPixmap*)0, sTxt);
            pIni = kRect.bottomLeft();
			}
    }
  return kRect;
}

QRect DisplayWindow::getRectLegendOnCanvas(TeAppTheme* appTheme,
                                           vector<string>& s)
{
	TeTheme* theme = (TeTheme*)appTheme->getTheme();
	bool isKernel = legendFromKernel(appTheme, s);

	TeCoord2D pp = appTheme->canvasLegUpperLeft();
	QPoint pIni((int)pp.x(), (int)pp.y());
	QPoint ph(0, 16);
	if(canvas_->printerFactor() != 1)
	{
		ph *= canvas_->printerFactor();
		pIni *= canvas_->printerFactor();
	}

	QRect kRect;
	if (isKernel) 
	{
		kRect = plotLegendKernel(pIni,s);
		kRect.setBottom(kRect.bottom() + 8);
		pIni = kRect.bottomLeft();
	}

	QRect rect(pIni, pIni);

	TeQtThemeItem *themeItem = mainWindow_->getViewsListView()->getThemeItem(appTheme);
	vector<QListViewItem*> itemVector = themeItem->getChildren();
	for (int i = 0; i < (int)itemVector.size(); ++i)
	{
		string txt;
		const QPixmap* pix = 0;
		TeQtCheckListItem *item = (TeQtCheckListItem*)itemVector[i];
		if((theme->visibleRep() & 0x40000000))
		{
			if (item->getType() == TeQtCheckListItem::LEGENDTITLE)
			{
				txt = item->text().latin1();
				int f = txt.find(tr("Variable: ").latin1());
				if(f >= 0)
				{
					string s = tr("Variable: ").latin1();
					txt = txt.substr(s.size());
				}
			}
			else if (item->getType() == TeQtCheckListItem::LEGEND)
			{
				txt = item->text().latin1();
				pix = item->pixmap(0);
			}
		}
		if((theme->visibleRep() & 0x80000000))
		{
			if (item->getType() == TeQtCheckListItem::CHARTTITLE)
			{
				pIni.setY((int)(pIni.y() + 8 * canvas_->printerFactor()));
				txt = item->text().latin1();
			}
			else if (item->getType() == TeQtCheckListItem::CHART)
			{
				txt = item->text().latin1();
				pix = item->pixmap(0);
			}
		}
		if(pix || txt.empty()==false)
		{
			rect |= canvas_->getLegendRect(pIni, pix, txt);
			pIni += ph;
		}
	}

	int rep = theme->layer()->geomRep();
	if(rep & TePOLYGONS || rep & TeCELLS)
		rect.setRight(rect.right() + 20);
	if(rep & TeLINES)
		rect.setRight(rect.right() + 20);
	if(rep & TePOINTS)
		rect.setRight(rect.right() + 20);

	rect |= kRect;
	return rect;
}
	

void DisplayWindow::plotLegendOnCanvas(TeAppTheme *appTheme)
{
	TeTheme* theme = (TeTheme*)appTheme->getTheme();

	if(appTheme->showCanvasLegend() == false)
		return;

	if ((theme->visibleRep() & 0x20000000) == 0)
		return;

	TeDatabase* db = theme->layer()->database(); 

	vector<string> kernelHeader;

	QRect grect = getRectLegendOnCanvas(appTheme, kernelHeader);
	QRect vrect = canvas_->viewport()->rect();
	QPoint offset = canvas_->offset();

	if(canvas_->printerFactor() != 1)
	{
		vrect = canvas_->getPixmap0()->rect();
		offset *= canvas_->printerFactor();
	}

	QPoint ppp = vrect.bottomRight() + offset;
	vrect.setBottom(ppp.y());
	vrect.setRight(ppp.x());

	if(vrect.intersects(grect) == false)
	{
		QPoint pc = vrect.center();
		TeCoord2D p(pc.x(), pc.y());
		appTheme->canvasLegUpperLeft(p);
		grect = getRectLegendOnCanvas(appTheme, kernelHeader);

		string up = "UPDATE te_theme_application SET canvas_leg_x = " + Te2String(p.x(), 9);
		up += ", canvas_leg_y = " + Te2String(p.y(), 9);
		up += " WHERE theme_id = " + Te2String(theme->id());
		db->execute(up);
	}

	TeCoord2D pp = appTheme->canvasLegUpperLeft();
	QPoint pIni((int)pp.x(), (int)pp.y());
	if(canvas_->printerFactor() != 1)
		pIni *= canvas_->printerFactor();
	else
	{
		appTheme->canvasLegWidth(grect.width());
		appTheme->canvasLegHeight(grect.height());
	}
	canvas_->setRectColor(240, 240, 240);

	if (!kernelHeader.empty()) {
		QRect	krect = plotLegendKernel(pIni, kernelHeader);
		pIni = krect.bottomLeft();
		pIni.setY(pIni.y() + 8);
	}
	QPoint ph(0, 16);
	if(canvas_->printerFactor() != 1)
		ph *= canvas_->printerFactor();
	int i;

	canvas_->plotRect(grect.left(), grect.bottom(), grect.width(), grect.height(), 60, true);

//	QPoint pKernelBar1, pKernelBar2;

	TeQtThemeItem *themeItem = mainWindow_->getViewsListView()->getThemeItem(appTheme);
	vector<QListViewItem*> itemVector = themeItem->getChildren();
	for (i = 0; i < (int)itemVector.size(); ++i)
	{
		string txt;
		const QPixmap* pix = 0;
		TeQtCheckListItem *item = (TeQtCheckListItem*)itemVector[i];
		if((theme->visibleRep() & 0x40000000))
		{
			if (item->getType() == TeQtCheckListItem::LEGENDTITLE)
			{
				txt = item->text().latin1();
				int f = txt.find(tr("Variable: ").latin1());
				if(f >= 0)
				{
					string s = tr("Variable: ").latin1();
					txt= txt.substr(s.size());
				}
			}
			else if (item->getType() == TeQtCheckListItem::LEGEND)
			{
				txt = item->text().latin1();
				pix = item->pixmap(0);
			}
		}
		if((theme->visibleRep() & 0x80000000))
		{
			if (item->getType() == TeQtCheckListItem::CHARTTITLE)
			{
				pIni.setY((int)(pIni.y() + 8.*canvas_->printerFactor()));
				txt = item->text().latin1();
			}
			else if (item->getType() == TeQtCheckListItem::CHART)
			{
				txt = item->text().latin1();
				pix = item->pixmap(0);
			}
		}

		if(pix || txt.empty()==false)
		{
			if(item->getType() == TeQtCheckListItem::CHARTTITLE || item->getType() == TeQtCheckListItem::LEGENDTITLE)
				canvas_->plotLegend(pIni, (TeLegendEntry*)0, txt);
			else if(item->getType() == TeQtCheckListItem::LEGEND)
				canvas_->plotLegend(pIni, ((TeQtLegendItem*)(item))->legendEntry(), txt);
			else
				canvas_->plotLegend(pIni, ((TeQtChartItem*)(item))->color(), txt);

			pIni += ph;

//			if (!kernelHeader.empty() && item->getType() == TeQtCheckListItem::LEGENDTITLE)
//				pKernelBar1 = pIni;
		}
	}
/*	if (!kernelHeader.empty())
	{
		pKernelBar2 = pIni;
		int w = (int)(21. * canvas_->printerFactor() + .5);
		string scor = theme->groupColor();
		vector<string> colorNameVec;

		QString s = scor.c_str();
		QStringList ss = QStringList::split("-",s,true);

		for(i = 0; i < (int)ss.size(); i++)
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

		vector<TeColor> colorVec;

		int nColors = pKernelBar2.y() - pKernelBar1.y();

		getColors(colorNameVec, nColors, colorVec);
		if (colorNameVec.size() == 1 && theme->groupColorDir() == 1) // arrumar o codigo......
		{
			vector<TeColor> cVec;
			for (int k = colorVec.size() - 1; k >= 0; --k)
				cVec.push_back(colorVec[k]);
			colorVec = cVec;
		}

		QPainter* painter = canvas_->getPainter();
		QPaintDevice* dev = painter->device();
		canvas_->plotOnPixmap0();
		int x2 = pKernelBar1.x() + w;
		for(i=0; i < (int)colorVec.size(); ++i)
		{
			QColor cor(colorVec[i].red_, colorVec[i].green_, colorVec[i].blue_);
			painter->setPen(cor);
			painter->moveTo(pKernelBar1.x(), pKernelBar1.y() + i);
			painter->lineTo(x2, pKernelBar1.y() + i);
		}

		canvas_->plotOnWindow();
		x2 = pKernelBar1.x() + w - canvas_->offset().x();
		for(i=0; i < (int)colorVec.size(); ++i)
		{
			QColor cor(colorVec[i].red_, colorVec[i].green_, colorVec[i].blue_);
			painter->setPen(cor);
			painter->moveTo(pKernelBar1.x(), pKernelBar1.y() + i - canvas_->offset().y());
			painter->lineTo(x2, pKernelBar1.y() + i - canvas_->offset().y());
		}
		if(dev)
		{
			painter->end();
			painter->begin(dev);
		}
	}*/
}


void DisplayWindow::resetEditions()
{
	plotXor(false);
	xorMtEdit_.endEdit();
	mtEdit_.endEdit();
	xorPointArrayVec_.clear();
}


void DisplayWindow::spatialOperationButtonGroup_clicked(int i)
{
	TeLayer* layer = mainWindow_->currentLayer();
	if(layer == 0)
	{
		spatialOperation_ = TeUNDEFINEDREL;
		return;
	}

	switch(i)
	{
		case 0:
			spatialOperation_ = TeDISJOINT;
			break;
		case 1:
			if (layer->hasGeometry(TePOLYGONS))
				spatialOperation_ = TeOVERLAPS;
			else if (layer->hasGeometry(TeLINES))
 				spatialOperation_ = TeCROSSES;
			else
				spatialOperation_ = TeUNDEFINEDREL;
			break;
		case 2:
			spatialOperation_ = TeWITHIN;
			break;
		case 3:
			spatialOperation_ = TeINTERSECTS;
			break;
		case 4:
			spatialOperation_ = TeUNDEFINEDREL;
			break;
		default:
			break;
	}
}


void DisplayWindow::popupCanvasSlot(QMouseEvent* m)
{
	QIconSet iconCheck((QPixmap)check);
	QIconSet iconUncheck((QPixmap)uncheck);

	QPoint p = m->pos();
	QPoint o (canvas_->contentsX(), canvas_->contentsY());
	p += o;

	TeCoord2D pt = canvas_->mapVtoCW(p);

	TeView*	view = mainWindow_->currentView();
	if(view == 0)
		return;

	TeAppTheme* appTheme = mainWindow_->currentTheme();
	if(appTheme == 0)
		return;

	TeTheme* theme = (TeTheme*)appTheme->getTheme();

	TeAttrTableVector attrTables; 
	TeLayer* layer = 0;
	if(theme->type() == TeTHEME || theme->type() == TeEXTERNALTHEME)	
	{
		layer = theme->layer();
		theme->getAttTables(attrTables); 
	}

	unsigned int i;
	bool external = false;
	for(i=0; i<attrTables.size(); i++)
	{
		if(attrTables[i].tableType() == TeAttrExternal)
		{
			external = true;
			break;
		}
	}

	TeDatabase* db = mainWindow_->currentDatabase();
	editOn_ = mainWindow_->editionIsActivated();
	graphCursorOn_ = mainWindow_->graphicCursorIsActivated();

	TeCoord2D pdw;
	double	resolution;
	double	delta;

	if(canvas_->getCursorMode() == TeQtCanvas::ZoomIn || 
	   canvas_->getCursorMode() == TeQtCanvas::ZoomOut)
	{	
		QPoint	mp(m->globalPos().x(), m->globalPos().y());
		popupZoom_->exec(mp);
	}
	else
	{	
		canvas_->setDataProjection(theme->getThemeProjection());
		if(canvas_->projection())
		{
			if(!(*(theme->getThemeProjection()) == *(canvas_->projection())))
			{
				resolution = canvas_->pixelSize();
				TeCoord2D pl(pt.x()-resolution/2., pt.y()-resolution/2.);
				TeCoord2D pu(pt.x()+resolution/2., pt.y()+resolution/2.);
				TeBox	box(pl, pu);
				box = TeRemapBox(box, canvas_->projection(), theme->getThemeProjection());
				resolution = box.x2_ - box.x1_;
				canvas_->projection()->setDestinationProjection(theme->getThemeProjection());
				pdw = canvas_->projection()->PC2LL(pt);
				pdw = theme->getThemeProjection()->LL2PC(pdw);
				delta = 3.*resolution;
			}
			else
			{
				delta = 3.*canvas_->pixelSize();
				pdw = pt;
			}
		}
		else
		{
			delta = 3.*canvas_->pixelSize();
			pdw = pt;
		}
		textInsertion_ = pdw;
		appTheme->tePoint_.add(pdw);

		pointedObjId_.clear();
		if(layer && editOn_ == false && graphCursorOn_ == false) // do pointing selection
		{
			if(layer->hasGeometry(TePOINTS))
			{
				TePoint point;
				if(theme->locatePoint(pdw, point, delta))
					pointedObjId_ = point.objectId();
			}
			if(pointedObjId_.empty() && layer->hasGeometry(TeLINES))
			{
				TeLine2D line;
				if(theme->locateLine(pdw, line, delta))
					pointedObjId_ = line.objectId();
			}
			if(pointedObjId_.empty() && layer->hasGeometry(TeCELLS))
			{
				TeCell cel;
				if(theme->locateCell(pdw, cel, 0.))
					pointedObjId_ = cel.objectId();
			}
			if(pointedObjId_.empty() && layer->hasGeometry(TePOLYGONS))
			{
				TePolygon polygon;
				if(theme->locatePolygon(pdw, polygon, 0.))
					pointedObjId_ = polygon.objectId();
			}
		}
		if(pointedObjId_.empty())
		{
			popupCanvas_->setItemEnabled(popupCanvas_->idAt(1), false); // own visual
			popupCanvas_->setItemEnabled(popupCanvas_->idAt(4), false); // media
			popupCanvas_->setItemEnabled(popupCanvas_->idAt(2), false); // legend visual
		}
		else
		{
			mainWindow_->setMediaId(pointedObjId_);

			popupCanvas_->setItemEnabled(popupCanvas_->idAt(2), false); // legend visual
			popupCanvas_->setItemEnabled(popupCanvas_->idAt(4), true); // media

			int	legId, ownId, resId;
			bool owns = false;

			TeDatabasePortal* portal = db->getPortal();
			string TC = theme->collectionTable();
			string sel = "SELECT c_legend_own FROM " + TC + " WHERE c_legend_own > 0";
			if(portal->query(sel) && portal->fetchRow())
				owns = true;

			sel = "SELECT c_legend_id, c_legend_own, c_object_status FROM " + TC;
			sel += " WHERE c_object_id = '" + pointedObjId_ + "'";
			portal->freeResult();
			if(portal->query(sel) && portal->fetchRow())
			{
				legId = portal->getInt(0);
				ownId = portal->getInt(1);
				resId = portal->getInt(2);

				if(legId)
				{
					if(theme->visibleRep() & 0x40000000)
					{
						popupCanvas_->setItemEnabled(popupCanvas_->idAt(2), true); // legend visual
						popupCanvas_->setItemEnabled(popupCanvas_->idAt(1), false); // own visual
					}
					else
					{
						popupCanvas_->setItemEnabled(popupCanvas_->idAt(1), true); // own visual
						popupCanvas_->setItemEnabled(popupCanvas_->idAt(2), false); // legend visual
						if(ownId)
						{
							popupOwnVisual_->setItemEnabled(popupOwnVisual_->idAt(1), true); // reset own visual
							popupOwnVisual_->setItemEnabled(popupOwnVisual_->idAt(2), true); // reset all own visual
						}
						else
						{
							popupOwnVisual_->setItemEnabled(popupOwnVisual_->idAt(1), false); // reset own visual
							if(owns)
								popupOwnVisual_->setItemEnabled(popupOwnVisual_->idAt(2), true); // reset all own visual
							else
								popupOwnVisual_->setItemEnabled(popupOwnVisual_->idAt(2), false); // reset all own visual
						}
					}
				}
				else
				{
					popupCanvas_->setItemEnabled(popupCanvas_->idAt(1), true); // own visual
					popupCanvas_->setItemEnabled(popupCanvas_->idAt(2), false); // legend visual
					if(ownId)
					{
						popupOwnVisual_->setItemEnabled(popupOwnVisual_->idAt(1), true); // reset own visual
						popupOwnVisual_->setItemEnabled(popupOwnVisual_->idAt(2), true); // reset all own visual
					}
					else
					{
						popupOwnVisual_->setItemEnabled(popupOwnVisual_->idAt(1), false); // reset own visual
						if(owns)
							popupOwnVisual_->setItemEnabled(popupOwnVisual_->idAt(2), true); // reset all own visual
						else
							popupOwnVisual_->setItemEnabled(popupOwnVisual_->idAt(2), false); // reset all own visual
					}
				}
			}
			delete portal;
		}

		if(editOn_)
		{
			mtEditInd_ = mtEdit_.getTextIndex(p);
			if(mtEditInd_ >= 0)
				popupCanvas_->setItemEnabled(popupCanvas_->idAt(23), true); // edit Text
			else
				popupCanvas_->setItemEnabled(popupCanvas_->idAt(23), false); // edit Text
			if(mtEdit_.size())
				popupCanvas_->setItemEnabled(popupCanvas_->idAt(24), true); // text visual
			else
				popupCanvas_->setItemEnabled(popupCanvas_->idAt(24), false); // text visual
		}
		else
		{
			popupCanvas_->setItemEnabled(popupCanvas_->idAt(23), false); // edit text
			popupCanvas_->setItemEnabled(popupCanvas_->idAt(24), false); // text visual
		}
		popupCanvas_->setItemEnabled(popupCanvas_->idAt(21), true); // insert Symbol
		popupCanvas_->setItemEnabled(popupCanvas_->idAt(22), true); // insert Text

		bool scaleVis=false, geoGridVis=false;
		TeDatabasePortal* portal  = db->getPortal();
		string sel = "SELECT visible FROM graphic_scale WHERE view_id = " + Te2String(view->id());
		if(portal->query(sel) && portal->fetchRow())
			scaleVis = portal->getInt(0);

		portal->freeResult();
		if(db->tableExist("app_geogrid"))
		{
			sel = "SELECT visible FROM app_geogrid WHERE view_id = " + Te2String(view->id());
			if(portal->query(sel) && portal->fetchRow())
				geoGridVis = portal->getInt(0);			
		}
		delete portal;

		if(scaleVis)
		{
			popupCanvas_->setItemEnabled(popupCanvas_->idAt(12), true); // graphic scale visual
			popupCanvas_->changeItem(popupCanvas_->idAt(13), iconCheck, tr("Show Graphic Scale")); // show graphic scale
			popupCanvas_->setItemEnabled(popupCanvas_->idAt(14), true);// graphic scale units
		}
		else
		{
			popupCanvas_->setItemEnabled(popupCanvas_->idAt(12), false); // graphic scale visual
			popupCanvas_->changeItem(popupCanvas_->idAt(13), iconUncheck, tr("Show Graphic Scale"));
			popupCanvas_->setItemEnabled(popupCanvas_->idAt(14), false);// graphic scale units
		}

		if(geoGridVis)
		{
			popupCanvas_->setItemEnabled(popupCanvas_->idAt(16), true); // geographical Grid visual
			popupCanvas_->changeItem(popupCanvas_->idAt(17), iconCheck, tr("Show Geographical Grid")); // show gegraphical grid
		}
		else
		{
			popupCanvas_->setItemEnabled(popupCanvas_->idAt(16), false); // geographical Grid visual
			popupCanvas_->changeItem(popupCanvas_->idAt(17), iconUncheck, tr("Show Geographical Grid")); // show gegraphical grid
		}

		popupCanvas_->setItemEnabled(popupCanvas_->idAt(6), graphCursorOn_); // spatial operation
		popupCanvas_->setItemEnabled(popupCanvas_->idAt(7), graphCursorOn_); // spatial cursor type
		if(mainWindow_->getGrid()->autoPromote_ == true)
			popupCanvas_->changeItem(popupCanvas_->idAt(8), iconCheck, tr("Automatic Promotion on the Grid"));
		else
			popupCanvas_->changeItem(popupCanvas_->idAt(8), iconUncheck, tr("Automatic Promotion on the Grid"));

		if(external == false)
		{
			if(layer && layer->hasGeometry(TePOINTS) && (theme->visibleRep() & TePOINTS))
			{
				if(editOn_)
					popupCanvas_->setItemEnabled(popupCanvas_->idAt(26), true); // insert point
			}
			else
				popupCanvas_->setItemEnabled(popupCanvas_->idAt(26), false); // insert point

			if(mainWindow_->getGrid()->getPortal() && mainWindow_->getGrid()->hasPointedLine())
				popupCanvas_->setItemEnabled(popupCanvas_->idAt(27), true); // remove pointed points
			else
				popupCanvas_->setItemEnabled(popupCanvas_->idAt(27), false); // remove pointed points		
		}

		if(drawSetOnCenter_)
			popupCanvas_->changeItem(popupCanvas_->idAt(29), iconCheck, tr("Centralize the Drawing of the Selected Objects"));
		else
			popupCanvas_->changeItem(popupCanvas_->idAt(29), iconUncheck, tr("Centralize the Drawing of the Selected Objects"));

		popupCanvas_->setItemEnabled(popupCanvas_->idAt(30), false); // remove raster contrast
		if(theme->visibleRep() & TeRASTER)
		{
			if (theme->rasterVisual() &&
			   (theme->rasterVisual()->getContrastM() != 1.0 ||
				theme->rasterVisual()->getContrastR() != 1.0 ||
				theme->rasterVisual()->getContrastG() != 1.0 ||
				theme->rasterVisual()->getContrastB() != 1.0 ))
			{
				popupCanvas_->setItemEnabled(popupCanvas_->idAt(33), true); // remove raster contrast
//				popupCanvas_->changeItem(popupCanvas_->idAt(33), iconCheck, tr("Remove Raster Contrast"));
			}
			else
				popupCanvas_->setItemEnabled(popupCanvas_->idAt(33), false); // remove raster contrast
		}
		else
			popupCanvas_->setItemEnabled(popupCanvas_->idAt(33), false); // remove raster contrast

		QPoint	mp(m->globalPos().x(), m->globalPos().y());
		popupCanvas_->exec(mp);
	}
}


void DisplayWindow::ownVisualSlot()
{
	VisualWindow *visualWindow = new VisualWindow(this, "visualWindow",true);
	visualWindow->setCaption(tr("Own Visual"));

	int legId = 0;
	TeDatabase* db = mainWindow_->currentDatabase();

	TeAppTheme* appTheme = mainWindow_->currentTheme();
	TeTheme* theme = (TeTheme*)appTheme->getTheme();

	string TC = theme->collectionTable();
	string sel = "SELECT c_legend_own FROM " + TC;
	sel += " WHERE c_object_id = '" + pointedObjId_ + "'";
	TeDatabasePortal* portal = db->getPortal();
	if(portal->query(sel) && portal->fetchRow())
		legId = portal->getInt(0);
	delete portal;

	TeLegendEntry leg = theme->defaultLegend();
	TeLegendEntry* ownLegend;
	if(legId == 0)
		ownLegend = &leg;
	else
		ownLegend = db->legendMap()[legId];

	TeVisual polygonVisual, lineVisual, pointVisual, textVisual;
	TeGeomRepVisualMap::iterator it = ownLegend->getVisualMap().begin();
	for (; it != ownLegend->getVisualMap().end(); ++it)
	{
		TeGeomRep rep = it->first;
		if (rep == TePOLYGONS || rep == TeCELLS)
			polygonVisual = *(ownLegend->visual(rep));
		else if (rep == TeLINES)
			lineVisual = *(ownLegend->visual(rep));
		else if (rep == TePOINTS)
			pointVisual = *(ownLegend->visual(rep));
		else if (rep == TeTEXT)
			textVisual = *(ownLegend->visual(rep));
	}

	visualWindow->loadLegend(*ownLegend, theme);

	visualWindow->exec();
	delete visualWindow;

	for (it = ownLegend->getVisualMap().begin(); it != ownLegend->getVisualMap().end(); ++it)
	{
		TeGeomRep rep = it->first;
		if (rep == TePOLYGONS || rep == TeCELLS)
		{
			if(!(polygonVisual == *(ownLegend->visual(rep))))
				break;
		}
		else if (rep == TeLINES)
		{
			if(!(lineVisual == *(ownLegend->visual(rep))))
				break;
		}
		else if (rep == TePOINTS)
		{
			if(!(pointVisual == *(ownLegend->visual(rep))))
				break;
		}
		else if (rep == TeTEXT)
		{
			if(!(textVisual == *(ownLegend->visual(rep))))
				break;
		}
	}

	if(it == ownLegend->getVisualMap().end())
	{
		QMessageBox::warning(this, tr("Warning"), 
			tr("No visual was modified!"));
		return;
	}

	if(ownLegend->id() == theme->defaultLegend().id())
	{
		TeLegendEntry *leg = new TeLegendEntry(*ownLegend);
		leg->id(0);
		leg->group(-10);
		db->insertLegend(leg);
		theme->getObjOwnLegendMap()[pointedObjId_] = leg->id();

		string up = "UPDATE " + TC + " SET c_legend_own = " + Te2String(leg->id());
		up += " WHERE c_object_id = '" + pointedObjId_ + "'";
		db->execute(up);
	}
	else
		db->updateLegend(ownLegend);

	
	plotData();
}


void DisplayWindow::removeOwnVisualSlot()
{
	int ownId;
	TeDatabase* db = mainWindow_->currentDatabase();

	TeAppTheme* appTheme = mainWindow_->currentTheme();
	TeTheme* theme = (TeTheme*)appTheme->getTheme();

	string TC = theme->collectionTable();
	string sel = "SELECT c_legend_own FROM " + TC;
	sel += " WHERE c_object_id = '" + pointedObjId_ + "'";

	TeDatabasePortal* portal = db->getPortal();
	if(portal->query(sel) && portal->fetchRow())
		ownId = portal->getInt(0);
	delete portal;

	db->legendMap().erase(ownId);

	string del = "DELETE FROM te_legend WHERE legend_id = " + Te2String(ownId);
	db->execute(del);
	del = "DELETE FROM te_visual WHERE legend_id = " + Te2String(ownId);
	db->execute(del);

	string up = "UPDATE " + TC + " SET c_legend_own = 0 WHERE c_object_id = '" + pointedObjId_ + "'";
	db->execute(up);

	theme->getObjOwnLegendMap().erase(pointedObjId_);
	plotData();
}


void DisplayWindow::removeAllOwnVisualsSlot()
{
	TeDatabase* db = mainWindow_->currentDatabase();

	TeAppTheme* appTheme = mainWindow_->currentTheme();
	TeTheme* theme = (TeTheme*)appTheme->getTheme();

	string TC = theme->collectionTable();
	int	i;
	vector<string> idVec, sVec;

	string sel = "SELECT c_legend_own FROM " + TC + " WHERE c_legend_own <> 0";
	TeDatabasePortal* portal = db->getPortal();
	if(portal->query(sel))
	{
		while(portal->fetchRow())
			idVec.push_back(portal->getData(0));
	}
	delete portal;
	vector<string>::iterator itB = idVec.begin();
	vector<string>::iterator itE = idVec.end();
	sVec = generateInClauses(itB, itE, db, false);

	for(i=0; i<(int)sVec.size(); i++)
	{
		string del = "DELETE FROM te_legend WHERE legend_id IN " + sVec[i];
		db->execute(del);

		del = "DELETE FROM te_visual WHERE legend_id IN " + sVec[i];
		db->execute(del);
	}

	for(i=0; i<(int)idVec.size(); i++)
		db->legendMap().erase(atoi(idVec[i].c_str()));

	string up = "UPDATE " + TC + " SET c_legend_own = 0 WHERE c_legend_own <> 0";
	db->execute(up);

	theme->getObjOwnLegendMap().clear();
	plotData();
}

void DisplayWindow::popupGraphicScaleVisualSlot()
{
	TeView*	view = mainWindow_->currentView();
	if(view == 0)
		return;
	graphicScaleVisualSlot(view->id());
}

void DisplayWindow::showGraphicScaleSlot()
{
	TeView*	view = mainWindow_->currentView();
	if(view == 0)
		return;

	bool vis;
	TeDatabase* db = mainWindow_->currentDatabase();

	TeDatabasePortal* portal  = db->getPortal();
	string sel = "SELECT visible FROM graphic_scale WHERE view_id = " + Te2String(view->id());
	if(portal->query(sel) && portal->fetchRow())
		vis = portal->getInt(0);
	delete portal;

	string up;
	if(vis)
		up = "UPDATE graphic_scale SET visible = 0 WHERE view_id = " + Te2String(view->id());
	else

		up = "UPDATE graphic_scale SET visible = 1 WHERE view_id = " + Te2String(view->id());

	db->execute(up);
	plotData();
}

void DisplayWindow::showGeographicalCoordinatesSlot()
{
	TeView*	view = mainWindow_->currentView();
	if(view == 0)
		return;

	bool vis;
	TeDatabase* db = mainWindow_->currentDatabase();

	if(db->tableExist("app_geogrid") == false)
	{
		TeAttributeList attr;
		TeAttribute		at;

		at.rep_.type_ = TeINT;				// 0
		at.rep_.name_ = "view_id";
		at.rep_.numChar_ = 0;
		at.rep_.isPrimaryKey_ = true;
		attr.push_back(at);

		at.rep_.isPrimaryKey_ = false;		// 1
		at.rep_.type_ = TeSTRING;
		at.rep_.name_ = "font_family";
		at.rep_.numChar_ = 50;
		attr.push_back(at);

		at.rep_.numChar_ = 0;				// 2
		at.rep_.type_ = TeINT;
		at.rep_.name_ = "red";
		attr.push_back(at);

		at.rep_.name_ = "green";			// 3
		attr.push_back(at);

		at.rep_.name_ = "blue";				// 4
		attr.push_back(at);

		at.rep_.name_ = "bold";				// 5
		attr.push_back(at);

		at.rep_.name_ = "italic";			// 6
		attr.push_back(at);

		at.rep_.name_ = "font_size";		// 7
		attr.push_back(at);

		at.rep_.name_ = "visible";			// 8
		attr.push_back(at);

		at.rep_.name_ = "left_vis";			// 9
		attr.push_back(at);

		at.rep_.name_ = "right_vis";		// 10
		attr.push_back(at);

		at.rep_.name_ = "top_vis";			// 11
		attr.push_back(at);

		at.rep_.name_ = "bottom_vis";		// 12
		attr.push_back(at);

		at.rep_.name_ = "line_red";			// 13
		attr.push_back(at);

		at.rep_.name_ = "line_green";		// 14
		attr.push_back(at);

		at.rep_.name_ = "line_blue";		// 15
		attr.push_back(at);
 
		at.rep_.name_ = "delta_calc";		// 16
		attr.push_back(at);

		at.rep_.name_ = "delta_graus";			// 17
		attr.push_back(at);

		at.rep_.name_ = "delta_mins";			// 18
		attr.push_back(at);

		at.rep_.type_ = TeREAL;
		at.rep_.name_ = "delta_secs";			// 19
		attr.push_back(at);

		at.rep_.type_ = TeINT;
		at.rep_.name_ = "dx_graus_offset";	// 20
		attr.push_back(at);

		at.rep_.name_ = "dx_mins_offset";	// 21
		attr.push_back(at);

		at.rep_.type_ = TeREAL;
		at.rep_.name_ = "dx_secs_offset";	// 22
		attr.push_back(at);

		at.rep_.type_ = TeINT;
		at.rep_.name_ = "dy_graus_offset";	// 23
		attr.push_back(at);

		at.rep_.name_ = "dy_mins_offset";	// 24
		attr.push_back(at);

		at.rep_.type_ = TeREAL;
		at.rep_.name_ = "dy_secs_offset";	// 25
		attr.push_back(at);

		at.rep_.type_ = TeINT;
		at.rep_.name_ = "tx_lat_horizontal";// 26
		attr.push_back(at);

		at.rep_.name_ = "tx_lon_horizontal";// 27
		attr.push_back(at);

		at.rep_.name_ = "sec_precision";	// 28
		attr.push_back(at);

		if(db->createTable ("app_geogrid", attr) == false)
		{
			QMessageBox::critical(this, tr("Error"), 
				tr("Fail to create the app_geogrid table!"));
			return;
		}
		if (db->createRelation("fk_geogrid", "app_geogrid", "view_id", "te_view", "view_id", true) == false)
		{
			QMessageBox::critical(this, tr("Error"), 
				tr("Fail to create the integrity among the app_geogrid and the te_view tables!"));
			return;
		}
	}

	string sel = "SELECT visible FROM app_geogrid WHERE view_id = ";
	sel += Te2String(view->id());
	TeDatabasePortal* portal  = db->getPortal();
	if(portal->query(sel) == false)
	{
		delete portal;
		return;
	}
	if(portal->fetchRow() == false)
	{
		portal->freeResult();

		string ins = "INSERT INTO app_geogrid (view_id, font_family, red, green, blue, bold, italic, font_size, visible,";
		ins += " left_vis, right_vis, top_vis, bottom_vis, line_red, line_green, line_blue, delta_calc";
		ins += ", delta_graus, delta_mins, delta_secs, dx_graus_offset, dx_mins_offset";
		ins += ", dx_secs_offset, dy_graus_offset, dy_mins_offset, dy_secs_offset, tx_lat_horizontal, tx_lon_horizontal, sec_precision)";
		ins += " VALUES (" + Te2String(view->id()) + ", 'Tahoma', 60, 30, 255, 0, 0, 8, 1, 1, 1, 1, 1, 255, 0, 255,";
		ins += " 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1)";
		if (db->execute(ins) == false)
		{
			QMessageBox::critical(this, tr("Error"), 
				tr("Fail to insert a value into the app_geogrid table!"));
			delete portal;
			return;
		}
		plotData();
		return;
	}
	else
		vis = (bool)portal->getInt(0);

	delete portal;

	string up;
	if(vis)
		up = "UPDATE app_geogrid SET visible = 0 WHERE view_id = " + Te2String(view->id());
	else

		up = "UPDATE app_geogrid SET visible = 1 WHERE view_id = " + Te2String(view->id());

	db->execute(up);
	plotData();
}

void DisplayWindow::popupGeographicalCoordinatesVisualSlot()
{
	TeView*	view = mainWindow_->currentView();
	if(view == 0)
		return;
	GeographicalGrid w(this, "Geographical Grid", true);
	if(w.error_ == false)
		w.exec();
}

void DisplayWindow::changeLegendVisualSlot()
{
	int	legId;
	TeDatabase* db = mainWindow_->currentDatabase();
	TeDatabasePortal* portal = db->getPortal();

	TeAppTheme* appTheme = mainWindow_->currentTheme();
	TeTheme* theme = (TeTheme*)appTheme->getTheme();

	string sel = "SELECT c_legend_id FROM " + theme->collectionTable();
	sel += " WHERE c_object_id = '" + pointedObjId_ + "'";

	if(portal->query(sel))
	{
		if(portal->fetchRow())
			legId = portal->getInt(0);
	}
	delete portal;

	TeLegendEntry* leg = db->legendMap()[legId];
	mainWindow_->changeLegendVisual(leg);
}



QPointArray DisplayWindow::getCircle( QPoint pa, QPoint pb )
{
	int r = (int)(sqrt(double((pa.x()-pb.x()) * (pa.x()-pb.x()) + (pa.y()-pb.y()) * (pa.y()-pb.y()))) + .5);
	int	i, j, x, y;
	int n = (int)((double)r * sqrt(2.) / 2. + .5) + 1;
	QPointArray	qa(8*n), qb(n), qc(n), qd(n), qe(n), qf(n), qg(n), qh(n);

	int rr = r * r;
	for(i=0; i<n; i++)
	{
		x = i;
		y = (int)(sqrt(rr - ((double)x * (double)x)) + .5);
		double xx = y;
		double yy = x;

		QPoint a(x, y);
		QPoint b(x, -y);
		QPoint c(-x, -y);
		QPoint d(-x, y);
		QPoint e((int)xx, (int)yy);
		QPoint f((int)xx, (int)-yy);
		QPoint g((int)-xx, (int)-yy);
		QPoint h((int)-xx, (int)yy);

		qa.setPoint(i, a+pa);
		qb.setPoint(i, b+pa);
		qc.setPoint(i, c+pa);
		qd.setPoint(i, d+pa);
		qe.setPoint(i, e+pa);
		qf.setPoint(i, f+pa);
		qg.setPoint(i, g+pa);
		qh.setPoint(i, h+pa);
	}

	for(j=0; j<n; j++)
		qa.setPoint(i++, qe.point(n-(j+1)));
	for(j=0; j<n; j++)
		qa.setPoint(i++, qf.point(j));
	for(j=0; j<n; j++)
		qa.setPoint(i++, qb.point(n-(j+1)));
	for(j=0; j<n; j++)
		qa.setPoint(i++, qc.point(j));
	for(j=0; j<n; j++)
		qa.setPoint(i++, qg.point(n-(j+1)));
	for(j=0; j<n; j++)
		qa.setPoint(i++, qh.point(j));
	for(j=0; j<n; j++)
		qa.setPoint(i++, qd.point(n-(j+1)));
	return qa;
}

QPointArray DisplayWindow::getElipse( QPoint pc, int rx, int ry )
{
	int	i, j, x, y, n;
	if(rx >= ry)
		n = rx + 1;
	else
		n = ry + 1;

	QPointArray	qa(4*n), qb(n), qc(n), qd(n);

	int rrx = rx * rx;
	int rry = ry * ry;
	for(i=0; i<n; i++)
	{
		if(rx >= ry)
		{
			x = i;
			double xx = (double)x * (double)x;
			y = (int)(sqrt(rry - xx * rry / rrx) + .5);
		}
		else
		{
			y = i;
			double yy = (double)y * (double)y;
			x = (int)(sqrt(rrx - yy * rrx / rry) + .5);
		}

		QPoint a(x, y);
		QPoint b(x, -y);
		QPoint c(-x, -y);
		QPoint d(-x, y);

		qa.setPoint(i, a+pc);
		qb.setPoint(i, b+pc);
		qc.setPoint(i, c+pc);
		qd.setPoint(i, d+pc);
	}

	if(rx >= ry)
	{
		for(j=0; j<n; j++)
			qa.setPoint(i++, qb.point(n-(j+1)));
		for(j=0; j<n; j++)
			qa.setPoint(i++, qc.point(j));
		for(j=0; j<n; j++)
			qa.setPoint(i++, qd.point(n-(j+1)));
	}
	else
	{
		for(j=0; j<n; j++)
			qa.setPoint(i++, qd.point(n-(j+1)));
		for(j=0; j<n; j++)
			qa.setPoint(i++, qc.point(j));
		for(j=0; j<n; j++)
			qa.setPoint(i++, qb.point(n-(j+1)));
	}
	return qa;
}

void DisplayWindow::setSquareCursorTypeSlot()
{
	popupSpatialCursorType_->setItemChecked(popupSpatialCursorType_->idAt(0), true);
	popupSpatialCursorType_->setItemChecked(popupSpatialCursorType_->idAt(1), false);
}


void DisplayWindow::setCircleCursorTypeSlot()
{
	popupSpatialCursorType_->setItemChecked(popupSpatialCursorType_->idAt(0), false);
	popupSpatialCursorType_->setItemChecked(popupSpatialCursorType_->idAt(1), true);
}


vector<QPointArray> DisplayWindow::rotateXorPoints()
{
	if(spatialCursorAngle_ == 0.)
		return xorPointArrayVec_;
	
	vector<QPointArray> pVec;
	QPoint c = cGraphCursor_;
	int	i, j, x, y;

	for(j=0; j<(int)xorPointArrayVec_.size(); j++)
	{
		QPointArray qpa(xorPointArrayVec_[j].count());
		for(i=0; i<(int)xorPointArrayVec_[j].count(); i++)
		{
			QPoint a = xorPointArrayVec_[j].point(i);
			double dx = a.x() - c.x();
			double dy = a.y() - c.y();
			double alfa = atan(dy/dx) * 180. / TePI;
			if(dx<0)
				alfa = 180. + alfa;
			else if(dx>=0 && dy<0)
				alfa = 360. + alfa;
			double beta = alfa + spatialCursorAngle_;
			double d = sqrt(double((a.x()-c.x()) * (a.x()-c.x()) + (a.y()-c.y()) * (a.y()-c.y())));

			x = c.x() + TeRound(cos(beta*TePI/180.) * d);
			y = c.y() + TeRound(sin(beta*TePI/180.) * d);
			a = QPoint(x, y);
			qpa.setPoint(i, a);
		}
		pVec.push_back(qpa);
	}

	return pVec;
}


void DisplayWindow::insertTextSlot()
{
	TeView*	view = mainWindow_->currentView();
	if(view == 0)
		return;

	TeAppTheme* appTheme = mainWindow_->currentTheme();
	if(appTheme == 0)
		return;

	TeTheme* theme = (TeTheme*)appTheme->getTheme();

	TeLayer* layer = 0;
	if(theme->type() == TeTHEME)
		layer = theme->layer();

	TeDatabase* db = mainWindow_->currentDatabase();

	InsertTextWindow* insText = new InsertTextWindow(this, "insert Text", true);
	if (!insText->cancel_)
        insText->exec();
	if(insText->cancel_ == true)
	{
		delete insText;
		return;
	}

	QString s = insText->insertLineEdit->text();
	delete insText;

	if(s.isEmpty())
		return;

	string table = appTheme->textTable();
	string ss = s.latin1();

	TeBox dataBox = canvas_->getDataWorld();
	if(dataBox.isValid() == false)
	{
		canvas_->setProjection(mainWindow_->currentView()->projection());
		canvas_->setDataProjection(theme->getThemeProjection());
//		canvas_->setWorld(layer->box());
		canvas_->setWorld(layer->box(), mainWindow_->cvContentsW_, mainWindow_->cvContentsH_);
		dataBox = canvas_->getDataWorld();
	}
	TeCoord2D pc = dataBox.center();
	TeText tx(pc, "qP");
	TeVisual visual = *(theme->defaultLegend().visual(TeTEXT));
	int tsize = visual.size();
	TeColor cor = visual.color();
	int rgb = cor.blue_ | (cor.green_ << 8) | (cor.red_ << 16);
	double th;
	if(visual.fixedSize() == false)
		th = canvas_->mapVtoCW(tsize) * 1.8;
	else
	{
		QRect rect = canvas_->textRect (tx, visual);
		th = canvas_->mapVtoCW(rect.height());
	}

	string ins = "INSERT INTO " + table;
	ins += " ( object_id, x, y, text_value, angle, height, alignment_vert, alignment_horiz) VALUES (";
	ins += "'TE_NONE_ID'";
	ins += ", " + Te2String(textInsertion_.x());
	ins += ", " + Te2String(textInsertion_.y());
	ins += ", '" + db->escapeSequence(ss) + "'";
	ins += ", 0";
	ins += ", " + Te2String(th);
	ins += ", 0";
	ins += ", 0)";
	db->execute(ins);

	string get = "SELECT * FROM " + table + " WHERE object_id = 'TE_NONE_ID'";
	get += " AND text_value = '" + db->escapeSequence(ss) + "'";
	get += " AND x = " + Te2String(textInsertion_.x());
	get += " AND y = " + Te2String(textInsertion_.y());

	TeDatabasePortal* portal = db->getPortal();

	if(portal->query(get) && portal->fetchRow())
		portal->fetchGeometry(tx);

	string vtable = table + "_txvisual";
	ins = "INSERT INTO " + vtable;
	ins += " ( geom_id, dot_height, fix_size, color, family, bold, italic) VALUES (";
	ins += " " + Te2String(tx.geomId());
	ins += ", " + Te2String(tsize);
	ins += ", " + Te2String(visual.fixedSize());
	ins += ", " + Te2String(rgb);
	ins += ", '" + visual.family() + "'";
	ins += ", " + Te2String(visual.bold());
	ins += ", " + Te2String(visual.italic()) + ")";
	db->execute(ins);

	mainWindow_->cursorActionGroup_selected(mainWindow_->enableEditAction);

	mtEdit_.endEdit();
	TeQtTextEdit tedit;
	visual = appTheme->getTextVisual(tx);
	tedit.init(tx, visual, canvas_);
	mtEdit_.push(tedit);
	textRectSelected_ = mtEdit_.getRect();

	TeBox tBox;
	string textBox = "SELECT MIN(x), MIN(y), MAX(x), MAX(y) FROM ";
	textBox += appTheme->textTable();
	portal->freeResult();
	if(portal->query(textBox))
	{
		if(portal->fetchRow())
		{
			double x1 = portal->getDouble(0);
			double y1 = portal->getDouble(1);
			double x2 = portal->getDouble(2);
			double y2 = portal->getDouble(3);
			TeBox box(x1, y1, x2, y2);
			tBox = box;
		}
	}
	delete portal;

	TeBox lBox = layer->box();
	updateBox(lBox, tBox);
	layer->setLayerBox(lBox);
	string up = "UPDATE te_layer SET lower_x = " + Te2String(lBox.x1_);
	up += ", lower_y = " + Te2String(lBox.y1_);
	up += ", upper_x = " + Te2String(lBox.x2_);
	up += ", upper_y = " + Te2String(lBox.y2_);
	up += " WHERE layer_id = " + Te2String(layer->id());
	db->execute(up);

	up = "UPDATE te_representation SET lower_x = " + Te2String(tBox.x1_);
	up += ", lower_y = " + Te2String(tBox.y1_);
	up += ", upper_x = " + Te2String(tBox.x2_);
	up += ", upper_y = " + Te2String(tBox.y2_);
	up += " WHERE layer_id = " + Te2String(layer->id());
	up += " AND geom_table = '" + appTheme->textTable() + "'";
	db->execute(up);

	TeBox rb(textRectSelected_.left(), textRectSelected_.top(), textRectSelected_.right(), textRectSelected_.bottom());
	canvas_->copyPixmap1ToPixmap0((int)rb.x1(), (int)rb.y1(), (int)rb.width(), (int)rb.height());
	canvas_->plotOnPixmap0();
	TeQtProgress progress(this, "ProgressDialog", true);
	canvas_->mapVtoCW(rb);
	canvas_->mapCWtoDW(rb);
	plotPieBars(appTheme, canvas_, &progress, rb);
	TePlotTextWV(appTheme, canvas_, &progress, rb);
	plotLegendOnCanvas (appTheme);
	plotGeographicalGrid();
	plotGraphicScale();
	canvas_->copyPixmapToWindow();
	plotXor(true);
}


void DisplayWindow::editTextSlot()
{
	TeView*	view = mainWindow_->currentView();
	if(view == 0)
		return;

	TeAppTheme* appTheme = mainWindow_->currentTheme();
	if(appTheme == 0)
		return;

	TeTheme* theme = (TeTheme*)appTheme->getTheme();

	TeLayer* layer = theme->layer();
	if (layer == 0)
		return;

	TeDatabase* db = mainWindow_->currentDatabase();
	string table = appTheme->textTable();

	TeText text = mtEdit_.text(mtEditInd_);

	InsertTextWindow* insText = new InsertTextWindow(this, "insert Text", true);
	insText->insertLineEdit->setText(text.textValue().c_str());
	insText->exec();

	if(insText->cancel_ == true)
		return;
	QString s = insText->insertLineEdit->text();
	delete insText;

	if(s.isEmpty())
		return;

	plotXor(false);
	string ss = s.latin1();

	string up = "UPDATE " + table + " SET text_value = '" + db->escapeSequence(ss) + "' WHERE";
	up += " geom_id = " + Te2String(text.geomId());

	db->execute(up);

	mtEdit_.updateText(mtEditInd_, ss);
	QRect tr = textRectSelected_;
	textRectSelected_ = mtEdit_.getRect();
	TeBox rb(tr.left(), tr.top(), tr.right(), tr.bottom());
	canvas_->copyPixmap1ToPixmap0((int)rb.x1(), (int)rb.y1(), (int)rb.width(), (int)rb.height());
	canvas_->plotOnPixmap0();
	TeQtProgress progress(this, "ProgressDialog", true);
	canvas_->mapVtoCW(rb);
	canvas_->mapCWtoDW(rb);
	plotPieBars(appTheme, canvas_, &progress, rb);
	TePlotTextWV(appTheme, canvas_, &progress, rb);
	plotLegendOnCanvas(appTheme);
	plotGeographicalGrid();
	plotGraphicScale();
	canvas_->copyPixmapToWindow();
	plotXor(true);
}


void DisplayWindow::textEditVisualSlot()
{
	TeView*	view = mainWindow_->currentView();
	if(view == 0)
		return;

	TeAppTheme* appTheme = mainWindow_->currentTheme();
	if(appTheme == 0)
		return;

	TeTheme* theme = (TeTheme*)appTheme->getTheme();

	TeLayer* layer = theme->layer();
	if (layer == 0)
		return;

	TeDatabase* db = mainWindow_->currentDatabase();
	string table = appTheme->textTable() + "_txvisual";
	TeText text;
	if(mtEditInd_ < 0)
		text = mtEdit_.text(0);
	else
		text = mtEdit_.text(mtEditInd_);

	TeVisual visual = appTheme->getTextVisual(text);
	TeLegendEntry legend;
	legend.setVisual(visual.copy(), TeTEXT);

	VisualWindow *visualWindow = new VisualWindow(this, "visualWindow",true);
	visualWindow->setCaption(tr("Text Visual"));
	visualWindow->loadLegend(legend, theme);
	visualWindow->exec();
	delete visualWindow;

	plotXor(false);
	int	dot_height;
	string family;
	bool fix_size, bold, italic;

	TeVisual* newVisual = legend.visual(TeTEXT);
	dot_height = newVisual->size();
	fix_size = newVisual->fixedSize();
	TeColor cor = newVisual->color();
	int rgb = cor.blue_ | (cor.green_ << 8) | (cor.red_ << 16);
	family = newVisual->family();
	bold = newVisual->bold();
	italic = newVisual->italic();

	vector<string> geomIdVec;
	int	i;
	for(i=0; i<mtEdit_.size(); i++)
		geomIdVec.push_back(Te2String(mtEdit_.text(i).geomId()));

	vector<string>::iterator itB = geomIdVec.begin();
	vector<string>::iterator itE = geomIdVec.end();
	vector<string> inClauseVector = generateInClauses(itB,itE, db, false);

	string up = "UPDATE " + table + " SET dot_height = " + Te2String(dot_height);
	up += ", fix_size = " + Te2String(fix_size);
	up += ", color = " + Te2String(rgb);
	up += ", family = '" + family + "'";
	up += ", bold = " + Te2String(bold);
	up += ", italic = " + Te2String(italic);
	up += " WHERE geom_id IN ";

	for (i = 0; i < (int)inClauseVector.size(); ++i)
	{
		string upd = up + inClauseVector[i];
		db->execute(upd);
	}

	mtEdit_.visual(*newVisual);
	mtEdit_.change();
	QRect tr = textRectSelected_;
	textRectSelected_ = mtEdit_.getRect();
	canvas_->plotOnPixmap0();
	TeQtProgress progress(this, "ProgressDialog", true);
	TeBox rb(tr.left(), tr.top(), tr.right(), tr.bottom());
	canvas_->copyPixmap1ToPixmap0((int)rb.x1(), (int)rb.y1(), (int)rb.width(), (int)rb.height());
	canvas_->mapVtoCW(rb);
	canvas_->mapCWtoDW(rb);
	plotPieBars(appTheme, canvas_, &progress, rb);
	TePlotTextWV(appTheme, canvas_, &progress, rb);
	plotLegendOnCanvas(appTheme);
	plotGeographicalGrid();
	plotGraphicScale();
	canvas_->copyPixmapToWindow();
	plotXor(true);
}


void DisplayWindow::popupDrawSelectedOnCenterSlot()
{
	drawSetOnCenter_ = !drawSetOnCenter_;
}


void DisplayWindow::setDefaultVisual()
{
	TeQtThemeItem *themeItem = mainWindow_->getViewsListView()->currentThemeItem();
	mainWindow_->setDefaultVisual(themeItem);
}


void DisplayWindow::setPointingVisual()
{
	TeQtThemeItem *themeItem = mainWindow_->getViewsListView()->currentThemeItem();
	mainWindow_->setPointingVisual(themeItem);
}

void DisplayWindow::setQueryVisual()
{
	TeQtThemeItem *themeItem = mainWindow_->getViewsListView()->currentThemeItem();
	mainWindow_->setQueryVisual(themeItem);
}

void DisplayWindow::setPointingQueryVisual()
{
	TeQtThemeItem *themeItem = mainWindow_->getViewsListView()->currentThemeItem();
	mainWindow_->setPointingQueryVisual(themeItem);
}



void DisplayWindow::changeZoomFactorSlot( int zoomFactor )
{
	popupZoom_->setItemChecked(popupZoom_->idAt(0), false);
	popupZoom_->setItemChecked(popupZoom_->idAt(1), false);
	popupZoom_->setItemChecked(popupZoom_->idAt(2), false);

	popupZoom_->setItemChecked(popupZoom_->idAt(zoomFactor), true);

	if (zoomFactor == 0)
		zoomFactor_ = 0.25;
	else if (zoomFactor == 1)
		zoomFactor_ = 0.5;
	else
		zoomFactor_ = 0.75;
}


void DisplayWindow::popupInsertPointSlot()
{
	TeAppTheme* theme = mainWindow_->currentTheme();
	if(theme->getTheme()->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	InsertPoint* insPoint = new InsertPoint(this, "insert Point", true);
	if(insPoint->init(rightButtonPressed_))
		insPoint->exec();
	delete insPoint;
}


void DisplayWindow::popupDeletePointedObjectsSlot()
{
	TeAppTheme* theme = mainWindow_->currentTheme();
	if(theme->getTheme()->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	int response = QMessageBox::question(this, tr("Delete Pointed Objects"),
			 tr("Do you really want to delete the pointed objects?"),
			 tr("Yes"), tr("No"));

	if (response != 0)
		return;

	TeQtGrid* grid = mainWindow_->getGrid();
	TeBox box;

	grid->getPortal()->freeResult();
	string erro = deletePointedObjects(((TeTheme*)theme->getTheme()), box);
	if(erro.empty() == false)
	{
		QMessageBox::critical(this, tr("Error"), erro.c_str());
		return;
	}

	grid->deletePointedLines();
	grid->updatePortalContents();

	double d = canvas_->mapVtoDW(6);

	box.x1_ = box.x1_ - d;
	box.x2_ = box.x2_ + d;
	box.y1_ = box.y1_ - d;
	box.y2_ = box.y2_ + d;
	plotData(box, true);
}


QPixmap* DisplayWindow::getContrastPixmap()
{
	return contrastPixmap_;
}


void DisplayWindow::setContrastPixmap(QPixmap* p)
{
	if (p == 0 && contrastPixmap_ != 0)
	{
		delete contrastPixmap_;
		contrastPixmap_ = 0;
	}
	else
		contrastPixmap_ = p;
}


void DisplayWindow::popupAnimationSlot()
{
	Animation anima(this);
	anima.exec();
}


void DisplayWindow::resetConnectedPixmap()
{
	if(connectedPixmap_)
		delete connectedPixmap_;
	connectedPixmap_ = 0;
}




void DisplayWindow::restoreConnectedRect( QRect &rect)
{
	if(connectedPixmap_ == 0)
		return;

	QPoint of(canvas_->contentsX (), canvas_->contentsY ());

	int ulx, uly, w, h;
	QPaintDevice *device = canvas_->viewport();

	if(connectedRect_.contains(rect.topLeft()))
	{
		QRect r(connectedRect_.left(), connectedRect_.top(), connectedRect_.width(), rect.top()-connectedRect_.top()+1);
		ulx = r.left();
		uly = r.top();
		w = r.width()+1;
		h = r.height()+1;
		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx+of.x(),uly+of.y(),w,h,Qt::CopyROP,true);
//		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx,uly,w,h,Qt::CopyROP,true);

		QRect rr(connectedRect_.left(), rect.top(), rect.left()-connectedRect_.left()+1, connectedRect_.bottom()-rect.top()+1);
		ulx = rr.left();
		uly = rr.top();
		w = rr.width()+1;
		h = rr.height()+1;
		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx+of.x(),uly+of.y(),w,h,Qt::CopyROP,true);
//		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx,uly,w,h,Qt::CopyROP,true);
	}
	else if(connectedRect_.contains(rect.bottomRight()))
	{
		QRect r(connectedRect_.left(), rect.bottom(), connectedRect_.width(), connectedRect_.bottom()-rect.bottom()+1);
		ulx = r.left();
		uly = r.top();
		w = r.width()+1;
		h = r.height()+1;
		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx+of.x(),uly+of.y(),w,h,Qt::CopyROP,true);
//		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx,uly,w,h,Qt::CopyROP,true);

		QRect rr(rect.right(), connectedRect_.top(), connectedRect_.right()-rect.right()+1, rect.bottom()-connectedRect_.top()+1);
		ulx = rr.left();
		uly = rr.top();
		w = rr.width()+1;
		h = rr.height()+1;
		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx+of.x(),uly+of.y(),w,h,Qt::CopyROP,true);
//		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx,uly,w,h,Qt::CopyROP,true);
	}
	else if(connectedRect_.contains(rect.topRight()))
	{
		QRect r(connectedRect_.left(), connectedRect_.top(), connectedRect_.width(), rect.top()-connectedRect_.top()+1);
		ulx = r.left();
		uly = r.top();
		w = r.width()+1;
		h = r.height()+1;
		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx+of.x(),uly+of.y(),w,h,Qt::CopyROP,true);
//		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx,uly,w,h,Qt::CopyROP,true);

		QRect rr(rect.right(), rect.top(), connectedRect_.right()-rect.right()+1, connectedRect_.bottom()-rect.top()+1);
		ulx = rr.left();
		uly = rr.top();
		w = rr.width()+1;
		h = rr.height()+1;
		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx+of.x(),uly+of.y(),w,h,Qt::CopyROP,true);
//		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx,uly,w,h,Qt::CopyROP,true);
	}
	else if(connectedRect_.contains(rect.bottomLeft()))
	{
		QRect r(connectedRect_.left(), connectedRect_.top(), rect.left()-connectedRect_.left()+1, connectedRect_.height());
		ulx = r.left();
		uly = r.top();
		w = r.width()+1;
		h = r.height()+1;
		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx+of.x(),uly+of.y(),w,h,Qt::CopyROP,true);
//		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx,uly,w,h,Qt::CopyROP,true);

		QRect rr(rect.left(), rect.bottom(), connectedRect_.right()-rect.left()+1, connectedRect_.bottom()-rect.bottom()+1);
		ulx = rr.left();
		uly = rr.top();
		w = rr.width()+1;
		h = rr.height()+1;
		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx+of.x(),uly+of.y(),w,h,Qt::CopyROP,true);
//		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx,uly,w,h,Qt::CopyROP,true);
	}
	else
	{
		ulx = connectedRect_.left();
		uly = connectedRect_.top();
		w = connectedRect_.width()+1;
		h = connectedRect_.height()+1;
		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx+of.x(),uly+of.y(),w,h,Qt::CopyROP,true);
//		bitBlt (device,ulx,uly,canvas_->getPixmap0(),ulx,uly,w,h,Qt::CopyROP,true);
	}
}


void DisplayWindow::paintEventSlot()
{
	if(connectedPixmap_)
	{
		if(canvas_->getDataWorld().isValid() == false)
			return;
		QPainter* painter = canvas_->getPainter();
		QPixmap *pix = (QPixmap*)painter->device();
//		if(pix==canvas_->getPixmap0() || pix==canvas_->getPixmap1() ||
//			pix==canvas_->getPixmap2() || pix==canvas_->getPixmap3())
		if(pix==canvas_->getPixmap0() || pix==canvas_->getPixmap1())
			return;

		QPaintDevice* device = canvas_->viewport();
		int ulx = connectedRect_.left();
		int uly = connectedRect_.top();
		int w = connectedRect_.width();
		int h = connectedRect_.height();
		QPoint of(canvas_->contentsX (), canvas_->contentsY ());

		bitBlt (device,ulx,uly,connectedPixmap_,ulx+of.x(),uly+of.y(),w,h,Qt::CopyROP,true);
		canvas_->plotOnWindow();
		canvas_->setLineColor (255, 0, 0);
		canvas_->setLineStyle (TeLnTypeContinuous);
		canvas_->plotRect(connectedRect_);
	}
}


void DisplayWindow::popupInsertSymbolSlot()
{
	TeView*	view = mainWindow_->currentView();
	if(view == 0)
		return;

	TeAppTheme* appTheme = mainWindow_->currentTheme();
	if(appTheme == 0)
		return;

	TeTheme* theme = (TeTheme*)appTheme->getTheme();

	if(theme->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	TeLayer* layer = theme->layer();
	if (layer == 0)
		return;

	TeDatabase* db = mainWindow_->currentDatabase();

	int flag = QDialog::Rejected;
	InsertSymbol w(this, "insert symbol", true);
	if(w.erro_ == false)
	{
		w.fontSizeComboBox->setCurrentText(Te2String(symbolSize_).c_str());
		w.fontComboBox_activated(symbolFamily_);
		w.setFixedSize(symbolFixedSize_);
		w.setColor(symbolColor_);
		flag = w.exec();
	}

	if(flag == QDialog::Rejected)
		return;

	symbolSize_ = w.fontSizeComboBox->currentText().toInt();
	symbolFamily_ = w.fontComboBox->currentText();
	symbolColor_ = w.color_;

	QString s = w.selString_;
	QFont font = w.font_;
	symbolFixedSize_ = false;
	if(w.fixedSizeCheckBox->isChecked())
		symbolFixedSize_ = true;

	QColor wcor = w.color_;
	TeColor cor(wcor.red(), wcor.green(), wcor.blue());
	int tsize = w.fontSizeComboBox->currentText().toInt();

	if(s.isEmpty())
		return;

	string table = appTheme->textTable();
	string ss = s.latin1();

	TeBox dataBox = canvas_->getDataWorld();
	if(dataBox.isValid() == false)
	{
		canvas_->setProjection(mainWindow_->currentView()->projection());
		canvas_->setDataProjection(layer->projection());
//		canvas_->setWorld(layer->box());
		canvas_->setWorld(layer->box(), mainWindow_->cvContentsW_, mainWindow_->cvContentsH_);
		dataBox = canvas_->getDataWorld();
	}
	TeCoord2D pc = dataBox.center();
	TeText tx(pc, "qP");
	TeVisual visual = *(theme->defaultLegend().visual(TeTEXT));
	visual.family(font.family().latin1());
	visual.size(tsize);
	visual.color(cor);
	int rgb = cor.blue_ | (cor.green_ << 8) | (cor.red_ << 16);
	double th;
	if(symbolFixedSize_ == false)
		th = canvas_->mapVtoCW(tsize) * 1.8;
	else
	{
		QRect rect = canvas_->textRect (tx, visual);
		th = canvas_->mapVtoCW(rect.height());
	}

	string ins = "INSERT INTO " + table;
	ins += " ( object_id, x, y, text_value, angle, height, alignment_vert, alignment_horiz) VALUES (";
	ins += "'TE_NONE_ID'";
	ins += ", " + Te2String(textInsertion_.x());
	ins += ", " + Te2String(textInsertion_.y());
	ins += ", '" + db->escapeSequence(ss) + "'";
	ins += ", 0";
	ins += ", " + Te2String(th);
	ins += ", 0";
	ins += ", 0)";
	db->execute(ins);

	string get = "SELECT * FROM " + table + " WHERE object_id = 'TE_NONE_ID'";
	get += " AND text_value = '" + db->escapeSequence(ss) + "'";
	get += " AND x = " + Te2String(textInsertion_.x());
	get += " AND y = " + Te2String(textInsertion_.y());

	TeDatabasePortal* portal = db->getPortal();

	if(portal->query(get) && portal->fetchRow())
		portal->fetchGeometry(tx);

	string vtable = table + "_txvisual";
	ins = "INSERT INTO " + vtable;
	ins += " ( geom_id, dot_height, fix_size, color, family, bold, italic) VALUES (";
	ins += " " + Te2String(tx.geomId());
	ins += ", " + Te2String(tsize);
	ins += ", " + Te2String(symbolFixedSize_);
	ins += ", " + Te2String(rgb);
	ins += ", '" + visual.family() + "'";
	ins += ", " + Te2String(visual.bold());
	ins += ", " + Te2String(visual.italic()) + ")";
	db->execute(ins);

	mainWindow_->cursorActionGroup_selected(mainWindow_->enableEditAction);

	mtEdit_.endEdit();
	TeQtTextEdit tedit;
	visual = appTheme->getTextVisual(tx);
	tedit.init(tx, visual, canvas_);
	mtEdit_.push(tedit);
	textRectSelected_ = mtEdit_.getRect();

	TeBox tBox;
	string textBox = "SELECT MIN(x), MIN(y), MAX(x), MAX(y) FROM ";
	textBox += appTheme->textTable();
	portal->freeResult();
	if(portal->query(textBox))
	{
		if(portal->fetchRow())
		{
			double x1 = portal->getDouble(0);
			double y1 = portal->getDouble(1);
			double x2 = portal->getDouble(2);
			double y2 = portal->getDouble(3);
			TeBox box(x1, y1, x2, y2);
			tBox = box;
		}
	}
	delete portal;

	TeBox lBox = layer->box();
	updateBox(lBox, tBox);
	layer->setLayerBox(lBox);
	string up = "UPDATE te_layer SET lower_x = " + Te2String(lBox.x1_);
	up += ", lower_y = " + Te2String(lBox.y1_);
	up += ", upper_x = " + Te2String(lBox.x2_);
	up += ", upper_y = " + Te2String(lBox.y2_);
	up += " WHERE layer_id = " + Te2String(layer->id());
	db->execute(up);

	up = "UPDATE te_representation SET lower_x = " + Te2String(tBox.x1_);
	up += ", lower_y = " + Te2String(tBox.y1_);
	up += ", upper_x = " + Te2String(tBox.x2_);
	up += ", upper_y = " + Te2String(tBox.y2_);
	up += " WHERE layer_id = " + Te2String(layer->id());
	up += " AND geom_table = '" + appTheme->textTable() + "'";
	db->execute(up);

	TeBox rb(textRectSelected_.left(), textRectSelected_.top(), textRectSelected_.right(), textRectSelected_.bottom());
	canvas_->copyPixmap1ToPixmap0((int)rb.x1(), (int)rb.y1(), (int)rb.width(), (int)rb.height());
	canvas_->plotOnPixmap0();
	TeQtProgress progress(this, "ProgressDialog", true);
	canvas_->mapVtoCW(rb);
	canvas_->mapCWtoDW(rb);
	plotPieBars(appTheme, canvas_, &progress, rb);
	TePlotTextWV(appTheme, canvas_, &progress, rb);
	plotLegendOnCanvas(appTheme);
	plotGeographicalGrid();
	plotGraphicScale();
	canvas_->copyPixmapToWindow();
	plotXor(true);
}


void DisplayWindow::removeRasterContrastSlot()
{
	if(mainWindow_->currentTheme() == 0)
		return;

	TeTheme* theme = (TeTheme*)mainWindow_->currentTheme()->getTheme();
	theme->rasterVisual()->setContrastB(1.0);
	theme->rasterVisual()->setContrastG(1.0);
	theme->rasterVisual()->setContrastR(1.0);
	theme->rasterVisual()->setContrastM(1.0);
	plotData();

}

void DisplayWindow::plotOnPrinter(QString fileName)
{
	TeView*	view = mainWindow_->currentView();
	if(view == 0 || canvas_->getWorld().isValid() == false)
		return;

	TeBox canvasBox = canvas_->getWorld();
	QPrinter printer(QPrinter::PrinterResolution);
	if(fileName.isEmpty())
	{
		if(printer.setup() == false)
			return;
		canvas_->widthRef2PrinterFactor(canvas_->getPixmap0()->width());
		canvas_->numberOfPixmaps(1);
		if(canvas_->setWorld(canvasBox, 0, 0, &printer) == false)
		{
			canvas_->numberOfPixmaps(3);
			QString erro = tr("It was not possible to allocate memory for these dimensions");
			erro += "\n" + tr("Change the printer resolution and try again");
			QMessageBox::warning(this, tr("Warning"), erro);
			canvas_->setWorld(canvasBox, mainWindow_->cvContentsW_, mainWindow_->cvContentsH_);
			plotOnPrinter("");
			return;
		}
	}
	else
	{
		canvas_->setWorld(canvasBox, canvas_->getPixmap0()->width(), canvas_->getPixmap0()->height(), &printer);
	}

	canvas_->clearRaster();
//	canvas_->plotOnPixmap0();
	canvas_->getPixmap0()->fill();

	map<int, RepMap>& layerRepMap = mainWindow_->layerId2RepMap_;

	TeAppTheme* currentAppTheme = mainWindow_->currentTheme();
	TeTheme* currentTheme = 0;
	
	if (currentAppTheme)
		currentTheme = (TeTheme*)currentAppTheme->getTheme();

	TeQtGrid* grid = mainWindow_->getGrid();
	grid->refresh();

	int i;
	TeBox box;
	//TeLayer* layer;
	TeQtProgress progress(this, "ProgressDialog", true);
	canvas_->setProjection(view->projection());

	// first step: plot all except current theme
	TeQtViewItem* viewItem = mainWindow_->getViewsListView()->getViewItem(view);
	vector<TeQtThemeItem*> themeItemVec = viewItem->getThemeItemVec();
	for (i = (int)themeItemVec.size() - 1; i > 0 ; --i)
	{
		TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
		TeTheme* theme = (TeTheme*)appTheme->getTheme();
		if(currentAppTheme == appTheme)
			continue;

		if (theme->visibility() && theme->visibleRep())
		{
			//layer = theme->layer();
			canvas_->setDataProjection (theme->getThemeProjection());
			box = canvas_->getDataWorld();

			// If there is intersection between layer and canvas, plot the theme
			if (TeIntersects(box, theme->box()) == true)
			{
				TePlotObjects(appTheme, canvas_, layerRepMap, &progress);
				plotPieBars(appTheme, canvas_, &progress);
				TePlotTextWV(appTheme, canvas_, &progress);
			}
			plotLegendOnCanvas(appTheme);
		}
	}

	// second step: plot current theme
	if (currentAppTheme && currentTheme->visibility() && currentTheme->visibleRep())
	{
		//layer = currentTheme->layer();
		canvas_->setDataProjection (currentTheme->getThemeProjection());
		box = canvas_->getDataWorld();

		// If there is intersection between layer and canvas, plot the theme
		if (TeIntersects(box, currentTheme->box()) == true)
		{
			TePlotObjects(currentAppTheme, canvas_, layerRepMap, &progress);
		}
	}

// third step: plot only editable elements
	if(currentTheme && currentTheme->visibility())
	{
		plotPieBars(currentAppTheme, canvas_, &progress);
		TePlotTextWV(currentAppTheme, canvas_, &progress);
		plotLegendOnCanvas(currentAppTheme);
	}

//fourth step: plot view graphic scale
	plotGeographicalGrid();
	plotGraphicScale();

//	mainWindow_->saveDisplayAsFileAction_activated();

	if(fileName.isEmpty())
	{
		QPainter* p = canvas_->getPainter();
		p->end();
		p->begin(&printer);
		QPaintDeviceMetrics pdm(&printer);
//		if(pdm.depth() < 32)
		if(pdm.numColors() < 3)
		{
			QImage ima;
			ima = *canvas_->getPixmap0();
			ima = ima.convertDepth(pdm.depth());
			QPixmap pix(ima);
			p->drawPixmap(0, 0, pix);
		}
		else
			p->drawPixmap(0, 0, *canvas_->getPixmap0());
		p->flush();

		canvas_->numberOfPixmaps(3);
		canvas_->clearRaster();
		canvas_->plotOnWindow();
		canvas_->setWorld(canvasBox, mainWindow_->cvContentsW_, mainWindow_->cvContentsH_);
		plotData();
	}
	else
	{
		QImageIO imo;
		QImage   im(canvas_->getPixmap0()->width(), canvas_->getPixmap0()->height(), canvas_->getPixmap0()->depth(),
					Qt::AutoColor | Qt::DiffuseDither | Qt::DiffuseAlphaDither);;
		im = *(canvas_->getPixmap0()); // convert to image
		imo.setImage(im);
		imo.setFileName(fileName);

		QString format = fileName;
		string sss = fileName.latin1();
		int p = format.findRev(".");
		format.remove(0, p+1);
		format = format.upper();
		imo.setFormat(format);
		string ssss = format.latin1();

		bool b = imo.write();
		canvas_->numberOfPixmaps(3);
		canvas_->clearRaster();
		canvas_->plotOnWindow();
		canvas_->setWorld(canvasBox, mainWindow_->cvContentsW_, mainWindow_->cvContentsH_);
		plotData();

		if(b)
			QMessageBox::information(this, tr("Save Display Contents as File"),
				tr("Save operation executed!"));
		else
			QMessageBox::warning(this, tr("Warning"),
				tr("Fail to save the file!"));
	}

}

void DisplayWindow::displaySizeSlot()
{
	DisplaySize* w = new DisplaySize(this, "DisplaySize");
	if(w->exec() == QDialog::Rejected)
	{
		delete w;
		return;
	}

	int width = w->widthComboBox->currentText().toInt();
	int height = w->heightComboBox->currentText().toInt();
	if(w->defaultCheckBox->isChecked())
	{
		width = 0;
		height = 0;
	}

	TeBox b = canvas_->getWorld();
	if(canvas_->setWorld(b, width, height) == true)
	{
		mainWindow_->cvContentsW_ = width;
		mainWindow_->cvContentsH_ = height;
		mainWindow_->resetAction_activated();
	}
	else
	{
		QString erro = tr("It was not possible to allocate memory for these dimensions");
		QMessageBox::warning(this, tr("Warning"), erro);
		delete w;
		displaySizeSlot();
	}
}



TeBox DisplayWindow::getPieBarBox( TeAppTheme* appTheme)
{
	TeTheme* theme = (TeTheme*)appTheme->getTheme();
	TeBox	boxOut;

	if(appTheme->chartAttributes_.size() == 0)
		return boxOut;

	string objectId = appTheme->chartSelected_;

	string	TS = theme->collectionTable();
	string	sel = "SELECT * FROM " + TS + " WHERE ";

	sel += TS + ".c_object_id = '" + objectId + "'";

	TeDatabasePortal *portal = theme->layer()->database()->getPortal();
	if (!portal)
		return false;

	if (!portal->query(sel) || !portal->fetchRow())
	{
		delete portal;
		return boxOut;
	}

	canvas_->setDataProjection(theme->layer()->projection());

	double	width = appTheme->barWidth();
	double	maxh = appTheme->barMaxHeight();
	double	minh = appTheme->barMinHeight();
	double	maxd = appTheme->pieMaxDiameter();
	double	mind = appTheme->pieMinDiameter();
	double	diameter = appTheme->pieDiameter();


	if(appTheme->keepDimension() != 0)
	{
		width = canvas_->mapVtoCW((int)width);
		maxh = canvas_->mapVtoCW((int)maxh);
		minh = canvas_->mapVtoCW((int)minh);
		maxd = canvas_->mapVtoCW((int)maxd);
		mind = canvas_->mapVtoCW((int)mind);
		diameter = canvas_->mapVtoCW((int)diameter);
	}

	unsigned int i;

	TeChartType chartType = (TeChartType)appTheme->chartType();
	if(chartType == TePieChart)
	{
		double delta = diameter / 2.;
		if(!(appTheme->pieDimAttribute() == "NONE"))
			delta = maxd / 2.;

		string attrs;
		for(i=0; i<appTheme->chartAttributes_.size(); i++)
			attrs += appTheme->chartAttributes_[i] + ",";
		attrs += "label_x,label_y";
		if(!(appTheme->pieDimAttribute() == "NONE"))
			attrs += "," + appTheme->pieDimAttribute();

		string q = "SELECT " + attrs + theme->sqlFrom() + " WHERE " + TS + ".c_object_id = '" + objectId + "'";
		portal->freeResult();
		if(portal->query(q))
		{
			if(portal->fetchRow())
			{
				double tot = 0.;
				for(i=0; i<appTheme->chartAttributes_.size(); i++)
				{
					string v = portal->getData(i);
					if(v.empty())
					{
						delete portal;
						return boxOut;
					}
					tot += atof(v.c_str());
				}
				if(tot == 0.)
				{
					delete portal;
					return boxOut;
				}

				if(!(appTheme->pieDimAttribute() == "NONE"))
				{
					if(appTheme->chartMaxVal() - appTheme->chartMinVal() == 0)
						diameter = 0.;
					else
					{
						double	adim = portal->getDouble(appTheme->chartAttributes_.size()+2);
						double a = (adim - appTheme->chartMinVal()) / (appTheme->chartMaxVal() - appTheme->chartMinVal());
						diameter = a * (maxd - mind) + mind;
					}
				}
				double	x = appTheme->chartPoint_.x();
				double	y = appTheme->chartPoint_.y();
				boxOut.x1_ = x - diameter / 2.;
				boxOut.y1_ = y - diameter / 2.;
				boxOut.x2_ = x + diameter / 2.;
				boxOut.y2_ = y + diameter / 2.;
			}
		}
	}
	else
	{
		double	n = appTheme->chartAttributes_.size();
		double	maxv = appTheme->chartMaxVal();
		double	minv = appTheme->chartMinVal();

		string attrs;
		for(i=0; i<appTheme->chartAttributes_.size(); i++)
			attrs += appTheme->chartAttributes_[i] + ",";
		attrs += "label_x,label_y";

		string q = "SELECT " + attrs + theme->sqlFrom() + " WHERE " + TS + ".c_object_id = '" + objectId + "'";

		vector<double> dvec;
		portal->freeResult();
		if(portal->query(q))
		{
			if(portal->fetchRow())
			{
				double	x = appTheme->chartPoint_.x();
				double	xmin = x - n * width / 2.;
				double	xmax = x + n * width / 2.;
				double	y = appTheme->chartPoint_.y();

				dvec.clear();
				for(i=0; i<appTheme->chartAttributes_.size(); i++)
				{
					string val = portal->getData(i);
					if(val.empty())
					{
						delete portal;
						return boxOut;
					}
					dvec.push_back(atof(val.c_str()));
				}

				double mheight = -10;
				for(i=0; i<dvec.size(); i++)
				{
					double height;
					double v = dvec[i];
					if(maxv - minv == 0)
						height = 0.;
					else
						height = (v - minv) * (maxh - minh) / (maxv - minv) + minh;
					mheight = MAX(height, mheight);
				}
				boxOut.x1_ = xmin;
				boxOut.y1_ = y;
				boxOut.x2_ = xmax;
				boxOut.y2_ = y + mheight;
			}
		}
	}
	delete portal;
	return boxOut;
}


void DisplayWindow::setGraphicScaleUnit_DecimalDegreesSlot()
{
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(0), true);
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(1), false);
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(2), false);
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(3), false);
	isSGUnitSet_ = true;
	SGUnit_ = 0;
	unit_ = tr("Decimal Degrees").latin1();
	plotData();
}


void DisplayWindow::setGraphicScaleUnit_KilometersSlot()
{
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(0), false);
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(1), true);
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(2), false);
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(3), false);
	isSGUnitSet_ = true;
	SGUnit_ = 1;
	unit_ = tr("Kilometers").latin1();
	plotData();
}


void DisplayWindow::setGraphicScaleUnit_MetersSlot()
{
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(0), false);
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(1), false);
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(2), true);
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(3), false);
	isSGUnitSet_ = true;
	SGUnit_ = 2;
	unit_ = tr("Meters").latin1();
	plotData();
}


void DisplayWindow::setGraphicScaleUnit_FeetsSlot()
{
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(0), false);
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(1), false);
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(2), false);
	popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(3), true);
	isSGUnitSet_ = true;
	SGUnit_ = 3;
	unit_ = tr("Feets").latin1();
	plotData();
}


void DisplayWindow::setScaleGraphicUnit( int n )
{
	//for (int i=0; i<4; ++i)
	//{
	//	if (i==n)
	//		popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(n), true);
	//	else
	//		popupGraphicScaleUnit_->setItemChecked(popupGraphicScaleUnit_->idAt(n), false);
	//}
	//SGUnit_ = n;
	if(n == 0)
		setGraphicScaleUnit_DecimalDegreesSlot();
	else if(n == 1)
		setGraphicScaleUnit_KilometersSlot();
	else if(n == 2)
		setGraphicScaleUnit_MetersSlot();
	else if(n == 3)
		setGraphicScaleUnit_FeetsSlot();
}


bool DisplayWindow::getGUScaleSetting()
{
	return  isSGUnitSet_;
}


int DisplayWindow::getSGUnit()
{
	return SGUnit_;
}


void DisplayWindow::rasterDataToStatusBar(TeLayer* layer, TeCoord2D pdw)
{
	TeRaster *r = layer->raster();
	TeDecoder *d = r->decoder();
	double val;
	TeCoord2D c = r->coord2Index(pdw);
	int col = TeRound(c.x());
	int lin = TeRound(c.y());
	int	i;

	QString	s = "lin=" + QString::number(lin) + " ";
	s += "col=" + QString::number(col) + " (";

	for(i=0; i<r->nBands(); ++i)
	{
		if(d->getElement(col, lin, val, i))
			s += QString::number(val) + " ";
		else
			s += tr("NoData") + " ";
	}
	s += ")";
	mainWindow_->statusBar()->message(s);
}


void DisplayWindow::createGraphicScaleTable()
{
	TeDatabase* db = mainWindow_->currentDatabase();

	TeAttribute		at;
	if(db->tableExist("graphic_scale") == false)
	{
		TeAttributeList attr;

		at.rep_.type_ = TeINT;
		at.rep_.name_ = "view_id";
		at.rep_.isPrimaryKey_ = true;
		attr.push_back(at);

		at.rep_.isPrimaryKey_ = false;
		at.rep_.type_ = TeSTRING;
		at.rep_.name_ = "font_family";
		at.rep_.numChar_ = 50;
		attr.push_back(at);

		at.rep_.numChar_ = 0;
		at.rep_.type_ = TeINT;
		at.rep_.name_ = "red";
		attr.push_back(at);

		at.rep_.type_ = TeINT;
		at.rep_.name_ = "green";
		attr.push_back(at);

		at.rep_.type_ = TeINT;
		at.rep_.name_ = "blue";
		attr.push_back(at);

		at.rep_.type_ = TeINT;
		at.rep_.name_ = "bold";
		attr.push_back(at);

		at.rep_.type_ = TeINT;
		at.rep_.name_ = "italic";
		attr.push_back(at);

		at.rep_.type_ = TeINT;
		at.rep_.name_ = "font_size";
		attr.push_back(at);

		at.rep_.type_ = TeINT;
		at.rep_.name_ = "visible";
		attr.push_back(at);

		if(db->createTable ("graphic_scale", attr) == false)
		{
			QMessageBox::critical(this, tr("Error"), 
				tr("Fail to create the graphic_scale table!"));
			return;
		}
		if (db->createRelation("fk_gscale", "graphic_scale", "view_id", "te_view", "view_id", true) == false)
		{
			QMessageBox::critical(this, tr("Error"), 
				tr("Fail to create the integrity among the graphic_scale and the te_view tables!"));
			return;
		}
	}
}

bool DisplayWindow::drawSetOnCenter()
{
    return drawSetOnCenter_;
}
