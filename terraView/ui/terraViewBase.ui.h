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
#ifndef _MSVS
	#include <teConfig.h>
#endif

#include <qlineedit.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qpaintdevicemetrics.h>
#include <qstrlist.h>
#include <qfiledialog.h>

#include <TeDriverSHPDBF.h>
#include <visual.h>
#include <databaseProp.h>
#include <layerProp.h>
#include <viewProp.h>
#include <themeProp.h>
#include <TeDataTypes.h>
#include <TeTable.h>
#include <TeRasterRemap.h>
#include <TeVectorRemap.h>
#include <TeGeometryAlgorithms.h>
#include <TeDBConnectionsPool.h>
#include <query.h>
#include <TeQtViewsListView.h>
#include <TeQtLayerItem.h>
#include <TeQtThemeItem.h>
#include <TeQtViewItem.h>
#include <TeQtDatabaseItem.h>
#include <TeFileTheme.h>
#include <addView.h>
#include <addTheme.h>
#include <projection.h>
#include <qcolordialog.h>
#include <qtabwidget.h>
#include <TePlotTheme.h>
#include <legendWindow.h>
#include <TeQtLegendItem.h>
#include <TeQtChartItem.h>
#include <TeQtProgress.h>
#include <TeDatabaseUtils.h>
#include <themeVis.h>
#include <graphic.h>
#include <edit.xpm>
#include <nonEdit.xpm>
#include <import.xpm>
#include <importDisabled.xpm>
#include <view.xpm>
#include <viewDisabled.xpm>
#include <theme.xpm>
#include <themeDisabled.xpm>
#include <check.xpm>
#include <uncheck.xpm>
#include <cursor_edit.xpm>
#include <cursor_edit_mask.xpm>
#include <cursor_distance.xpm>
#include <cursor_distance_mask.xpm>
#include <cursor_zoom_in.xpm>
#include <cursor_zoom_in_mask.xpm>
#include <cursor_zoom_out.xpm>
#include <cursor_zoom_out_mask.xpm>
#include <cursor_info.xpm>
#include <cursor_info_mask.xpm>
#include <createCells.h>
#include <textRep.h>
#include <TeInitRasterDecoders.h>
#include <importTbl.h>
#include <qfiledialog.h>
#include <qcombobox.h>
#include <exportWindow.h>
#include <statistic.h>
#include <graphicParams.h>
#include <selThemeTables.h>
#include <createTable.h>
#include <removeTable.h>
#include <addColumn.h>
#include <changeColData.h>
#include <kernelWindow.h>
#include <kernelRatioWindow.h>
#include <TeQtProgress.h>
#include <createLayerFromTheme.h>
#include <createThemeFromTheme.h>
#include <saveRaster.h>
#include <saveAsTable.h>
#include <saveTableAsTxt.h>
#include <countComputing.h>
#include <importRasterSimple.h>
#include <contrast.h>
#include <connectView.h>
#include <geoOpAggregation.h>
#include <geoOpAdd.h>
#include <geoOpOverlayIntersection.h>
#include <geoOpOverlayDifference.h>
#include <geoOpOverlayUnion.h>
#include <geoOpAssignDataLocationCollect.h>
#include <geoOpAssignDataLocationDistribute.h>
#include <createBuffersWindow.h>
#include <semivar.h>
#include <spatializationWindow.h>
#include <addressLocatorWindow.h>
#include <rasterSlicingWindow.h>
#include <animation.h>
#include <skaterWindow.h>
#include <bayesWindow.h>
#include <localBayesWindow.h>
#include <rasterColorCompositionWindow.h>
#include <pieBarChart.h>
#include <changeColumnName.h>
#include <changeColumnType.h>
#include <histogramBuildForm.h> 
#include <createProxMatrix.h>
#include <selectProxMatrix.h>
#include <rasterTransparency.h>
#include <urlWindow.h>
#include <printFileResolution.h>
#include <PluginSupportFunctions.h>
#include <graphic3DWindow.h>
#include <rasterImportWizard.h>
#include <generateSamplePoints.h>
#include <saveThemeToFileWindow.h>
#include <qinputdialog.h>
#include <exportLayersThemes.h>
#include <telayer_metadata.h>
#include <dlgDropDatabase.h>
#include <TeInitDatabases.h>
#ifdef ADO_FOUND
#include <TeInitAdoDatabase.h>
#endif // ADO_FOUND
#include <TeDatabaseFactory.h>

#include <TeUpdateDBVersion.h>


std::vector<std::string> vecPluginFileNames;
std::vector<std::string> vecPluginNames;
std::vector<std::string> vecTranslatedNames;

void TerraViewBase::init()
{

	TeInitDatabases();
	#ifdef ADO_FOUND
		TeInitAdoDatabase();
	#endif
	

	help_ = 0;
	cvContentsW_ = 0;
	cvContentsH_ = 0;
	printFileResolution_ = 200;
	graphic3DWindow_ = 0;
	semivar_ = 0;
	qapplication_ptr_ = 0;
	pluginMetadataSetted_ = false;

	QDir dir;
	string absPath = dir.absPath().latin1();
	lastOpenDir_ = dir.absPath();

	cursorSelected_ = pointerCursorAction;
	origDecimalSepIsPoint_ = true; 
	columnTooltip_ = -1;
	linkTable_ = 0;
	showMediaWindow_ = 0;
	createProxMatrix_ = 0;
	selectProxMatrix_ = 0;
    ws_ = new QWorkspace(this);
    ws_->setScrollBarsEnabled( TRUE );
    setCentralWidget( ws_ );

	//Instantiate the QDockWindow that will contain the Databases Tree
    databasesDock_ = new QDockWindow(QDockWindow::InDock, ws_, "databasesDock");
    databasesDock_->setCloseMode(QDockWindow::Always);    
    addDockWindow(databasesDock_, Qt::DockLeft);
    moveDockWindow(databasesDock_, Qt::DockLeft);
    databasesDock_->setFixedExtentWidth(200);
    databasesDock_->setVerticallyStretchable(TRUE);
    databasesDock_->setResizeEnabled(TRUE);

	connect(databasesDock_,SIGNAL(visibilityChanged(bool)),
			this, SLOT(databasesTreeAction_toggled(bool)));

	//Instantiate the widget that will contain the list of databases 
    databasesListView_ = new TeQtDatabasesListView(databasesDock_, "databasesListView" );
	databasesListView_->addColumn( tr( "Databases" ) );
	databasesDock_->setWidget(databasesListView_);

	connect(databasesListView_,SIGNAL(itemChanged(QListViewItem*)),this, SLOT(itemChanged(QListViewItem*)));
	connect(databasesListView_,SIGNAL(itemRenamed(QListViewItem*,int,const QString&)),
			this,SLOT(itemRenamed(QListViewItem*, int,const QString&)));
	connect(databasesListView_,SIGNAL(popupSignal()), this, SLOT(execPopupFromDatabasesListViewSlot()));	

	//Instantiate the QDockWindow that will contain the Views Tree
    viewsDock_ = new QDockWindow(QDockWindow::InDock, ws_, "viewsDock");
    viewsDock_->setCloseMode(QDockWindow::Always);    
    addDockWindow(viewsDock_, Qt::DockLeft);
    moveDockWindow(viewsDock_, Qt::DockLeft);
    viewsDock_->setFixedExtentWidth(200);
    viewsDock_->setVerticallyStretchable(TRUE);
    viewsDock_->setResizeEnabled(TRUE);

	connect(viewsDock_,SIGNAL(visibilityChanged(bool)),this, SLOT(viewsTreeAction_toggled(bool)));

	//Instantiate the widget that will contain the list of views 
    viewsListView_ = new TeQtViewsListView(viewsDock_, "viewsListView" );
	viewsListView_->addColumn( tr( "Views/Themes" ) );
	viewsDock_->setWidget(viewsListView_);

	connect(viewsListView_,SIGNAL(itemChanged(QListViewItem*)),this, SLOT(itemChanged(QListViewItem*)));
	connect(viewsListView_,SIGNAL(itemRenamed(QListViewItem*,int,const QString&)),
			this,SLOT(itemRenamed(QListViewItem*, int,const QString&)));
	connect(viewsListView_, SIGNAL(checkListItemVisChanged(QCheckListItem*, bool)),
			this, SLOT(checkListItemVisChanged(QCheckListItem*, bool)));
	connect(viewsListView_, SIGNAL(dragDropItems(TeQtThemeItem*, TeQtViewItem*, TeQtViewItem*)),
			this, SLOT(updateThemesPriority(TeQtThemeItem*, TeQtViewItem*, TeQtViewItem*)));
	connect(viewsListView_,SIGNAL(popupSignal()), this, SLOT(execPopupFromViewsListViewSlot()));	

	//Instantiate the QDockWindow that will contain the grid
    gridDock_ = new QDockWindow(QDockWindow::InDock, ws_, "gridDock");
    gridDock_->setCloseMode(QDockWindow::Always);    
    addDockWindow(gridDock_, Qt::DockBottom);
    moveDockWindow(gridDock_, Qt::DockBottom);
    gridDock_->setFixedExtentHeight(170);
    gridDock_->setVerticallyStretchable(TRUE);
    gridDock_->setResizeEnabled(TRUE);
 
	connect(gridDock_,SIGNAL(visibilityChanged(bool)),this, SLOT(gridAction_toggled(bool)));

	//Instantiate the window that will contain the display area
	displayWindow_ = new DisplayWindow(ws_, 0, WStyle_SysMenu);

	canvas_ = displayWindow_->getCanvas();

	connect(canvas_, SIGNAL(zoomArea()), this, SLOT(zoomAreaSlot()));

	//Instantiate the grid
	grid_ = new TeQtGrid(gridDock_, "grid" );
	gridDock_->setWidget(grid_);

	//Instantiate the graphic window
	graphic_ = new GraphicWindow(ws_, 0, WStyle_SysMenu);

	connect(grid_,SIGNAL(putColorOnObject(TeAppTheme*, set<string>&)),
			graphic_, SLOT(putColorOnObjectSlot(TeAppTheme*, set<string>&)));
	connect(grid_,SIGNAL(putColorOnObject(TeAppTheme*, set<string>&)),
			displayWindow_, SLOT(plotData(TeAppTheme*, set<string>&)));
	connect(grid_,SIGNAL(vertHeaderLeftDblClickSignal(int)),
			this, SLOT(popupVerticalHeaderDblClickSlot(int)));
		
	//insert items on the grid popupHorizHeader
	QPopupMenu *popupHorizHeader = grid_->popupHorizHeader();
	popupHorizHeader->insertItem(tr("Sort in Ascendent Order"), this, SLOT(popupGridColumnsSlot(int)), 0, 7);
	popupHorizHeader->insertItem(tr("Sort in Descendent Order"), this, SLOT(popupGridColumnsSlot(int)), 0, 8);
	popupHorizHeader->insertSeparator();
	popupHorizHeader->insertItem(tr("Statistic..."), this, SLOT(popupStatisticSlot()));

	QPopupMenu *popupHistogram = new QPopupMenu;
	popupHorizHeader->insertItem(tr("Histogram"), popupHistogram);
	popupHistogram->insertItem(tr("All"), this, SLOT(popupHistogramSlot(int)), 0, 0);
	popupHistogram->insertSeparator();
	popupHistogram->insertItem(tr("Selected by Query"), this, SLOT(popupHistogramSlot(int)), 0, 1);
	popupHistogram->insertItem(tr("Not Selected by Query"), this, SLOT(popupHistogramSlot(int)), 0, 2);
	popupHistogram->insertSeparator();
	popupHistogram->insertItem(tr("Grouped"), this, SLOT(popupHistogramSlot(int)), 0, 3);
	popupHistogram->insertItem(tr("Not Grouped"), this, SLOT(popupHistogramSlot(int)), 0, 4);

	QPopupMenu *popupNormalProb = new QPopupMenu;
	popupHorizHeader->insertItem(tr("Normal Probability"), popupNormalProb);
	popupNormalProb->insertItem(tr("All"), this, SLOT(popupNormalProbSlot(int)), 0, 0);
	popupNormalProb->insertSeparator();
	popupNormalProb->insertItem(tr("Selected by Query"), this, SLOT(popupNormalProbSlot(int)), 0, 1);
	popupNormalProb->insertItem(tr("Not Selected by Query"), this, SLOT(popupNormalProbSlot(int)), 0, 2);
	popupNormalProb->insertSeparator();
	popupNormalProb->insertItem(tr("Grouped"), this, SLOT(popupNormalProbSlot(int)), 0, 3);
	popupNormalProb->insertItem(tr("Not Grouped"), this, SLOT(popupNormalProbSlot(int)), 0, 4);
	popupHorizHeader->insertSeparator();
	
	popupTooltip_ = new QPopupMenu;
	popupHorizHeader->insertItem(tr("Tooltip"), popupTooltip_);
	popupTooltip_->insertItem("All", this, SLOT(popupTooltipSlot(int)), 0, 0);
	popupTooltip_->insertItem("MIN", this, SLOT(popupTooltipSlot(int)), 0, 1);
	popupTooltip_->insertItem("MAX", this, SLOT(popupTooltipSlot(int)), 0, 2);
	popupTooltip_->insertItem("COUNT", this, SLOT(popupTooltipSlot(int)), 0, 3);
	popupTooltip_->insertItem("AVG", this, SLOT(popupTooltipSlot(int)), 0, 4);
	popupTooltip_->insertItem("SUM", this, SLOT(popupTooltipSlot(int)), 0, 5);
	popupTooltip_->insertItem("Hide", this, SLOT(popupTooltipSlot(int)), 0, 6);
	popupHorizHeader->insertSeparator();

	popupHorizHeader->insertItem(tr("Add Column..."), this, SLOT(popupAddColumnSlot()));
	popupHorizHeader->insertItem(tr("Delete Column..."), this, SLOT(popupDeleteColumnSlot()));
	popupHorizHeader->insertItem(tr("Change Column Data..."), this, SLOT(popupChangeColumnDataSlot()));
	popupHorizHeader->insertItem(tr("Change Column Name..."), this, SLOT(popupChangeColumnNameSlot()));
	popupHorizHeader->insertItem(tr("Change Column Type..."), this, SLOT(popupChangeColumnTypeSlot()));
	popupHorizHeader->insertSeparator();
	popupHorizHeader->insertItem(tr("Unlink External Table"), this, SLOT(popupUnlinkExternalTableSlot()));
			
	//insert items on the grid popupVertHeader
	QPopupMenu *popupVertHeader = grid_->popupVertHeader();

	QPopupMenu *popupPromote = new QPopupMenu;
	popupVertHeader->insertItem(tr("Promote"), popupPromote);
	popupPromote->insertItem(tr("Lines Selected by Pointing"), this, SLOT(popupPromoteSlot(int)), 0, 0);
	popupPromote->insertItem(tr("Lines Selected by Query"), this, SLOT(popupPromoteSlot(int)), 0, 1);

	QPopupMenu *popupScrollGrid = new QPopupMenu;
	popupVertHeader->insertItem(tr("Scroll Grid"), popupScrollGrid);
	popupScrollGrid->insertItem(tr("Next Pointed Object       Ctrl+PgDown"), this, SLOT(popupScrollGridSlot(int)), 0, 0);
	popupScrollGrid->insertItem(tr("Previous Pointed Object   Ctrl+PgUp"), this, SLOT(popupScrollGridSlot(int)), 0, 1);
	popupScrollGrid->insertItem(tr("Next Queried Object       Alt+PgDown"), this, SLOT(popupScrollGridSlot(int)), 0, 2);
	popupScrollGrid->insertItem(tr("Previous Queried Object   Alt+PgUp"), this, SLOT(popupScrollGridSlot(int)), 0, 3);

	popupVertHeader->insertSeparator();
	popupVertHeader->insertItem(tr("Insert Media..."), this, SLOT(popupInsertMediaSlot()));
	popupVertHeader->insertItem(tr("Insert URL..."), this, SLOT(popupInsertURLSlot()));
	popupVertHeader->insertSeparator();
	popupVertHeader->insertItem(tr("Show Default Media..."), this, SLOT(popupShowDefaultMediaSlot()));
	popupVertHeader->insertItem(tr("Default Media Description..."), this, SLOT(popupDefaultMediaDescriptionSlot()));
	popupVertHeader->insertItem(tr("Attributes and Media..."), this, SLOT(popupAttributesAndMediaSlot()));
	
	//insert items on the grid popupViewport
	QPopupMenu *popupViewport = grid_->popupViewport();

	popupViewport->insertItem(tr("Adjust Columns"), this, SLOT(popupGridColumnsSlot(int)), 0, 0);
	popupViewport->insertSeparator();
	popupViewport->insertItem(tr("Sort in Ascendent Order"), this, SLOT(popupGridColumnsSlot(int)), 0, 1);
	popupViewport->insertItem(tr("Sort in Descendent Order"), this, SLOT(popupGridColumnsSlot(int)), 0, 2);
	popupViewport->insertSeparator();
	popupViewport->insertItem(tr("Swap Columns"), this, SLOT(popupGridColumnsSlot(int)), 0, 3);
	popupViewport->insertItem(tr("Hide Column(s)"), this, SLOT(popupGridColumnsSlot(int)), 0, 4);
	popupViewport->insertItem(tr("Show All Column(s)"), this, SLOT(popupGridColumnsSlot(int)), 0, 5);
	popupViewport->insertItem(tr("Delete Column(s)"), this, SLOT(popupGridColumnsSlot(int)), 0, 6);
	popupViewport->insertSeparator();

	QPopupMenu *popupVisual = new QPopupMenu;
	popupViewport->insertItem(tr("Visual"), popupVisual);
	popupVisual->insertItem(tr("Default..."), displayWindow_, SLOT(setDefaultVisual()));
	popupVisual->insertItem(tr("Pointing..."), displayWindow_, SLOT(setPointingVisual()));
	popupVisual->insertItem(tr("Query..."), displayWindow_, SLOT(setQueryVisual()));
	popupVisual->insertItem(tr("Pointing and Query..."), displayWindow_, SLOT(setPointingQueryVisual()));
	popupVisual->insertSeparator();
	popupVisual->insertItem(tr("Text..."), this, SLOT(textVisual()));

	popupViewport->insertItem(tr("Visibility of the Representations..."), this, SLOT(setCurrentThemeVisibility()));

	QPopupMenu *popupRemoveColors = new QPopupMenu;
	popupViewport->insertItem(tr("Remove Colors"), popupRemoveColors);
	popupRemoveColors->insertItem(tr("Pointed Objects"), this, SLOT(removePointingColor()));
	popupRemoveColors->insertItem(tr("Queried Objects"), this, SLOT(removeQueryColor()));
	popupViewport->insertSeparator();

	popupTable_ = new QPopupMenu;
	popupViewport->insertItem(tr("Table"), popupTable_);
	popupTable_->insertItem(tr("Select Theme Tables..."), this, SLOT(selectThemeTables()));
	popupTable_->insertItem(tr("Link External Table..."), this, SLOT(linkExternalTable()));
	popupTable_->insertSeparator();
	popupTable_->insertItem(tr("Edit in the Vertical Direction"), this, SLOT(popupEditInVerticalDirection()));

	QIconSet iconCheck((QPixmap)check);
	QIconSet iconUncheck((QPixmap)uncheck);
	popupTable_->setItemChecked(popupTable_->idAt(3), true); // vertical
	popupTable_->changeItem(popupTable_->idAt(3), iconCheck, tr("Edit in the Vertical Direction"));

	popupTable_->insertSeparator();
	popupTable_->insertItem(tr("Export Table..."), this, SLOT(popupExportTableSlot()));

	popupTable_->insertSeparator();
	popupTable_->insertItem(tr("Export Table As..."), this, SLOT(popupExportTableAsTxtSlot()));

	popupViewport->insertSeparator();

	QPopupMenu *popupDispersion = new QPopupMenu;
	popupViewport->insertItem(tr("Dispersion"), popupDispersion);
	popupViewport->insertItem(tr("Dispersion 3D..."), this, SLOT(popupDispersion3DSlot()));
	popupDispersion->insertSeparator();
	popupDispersion->insertItem(tr("All"), this, SLOT(popupDispersionSlot(int)), 0, 0);
	popupDispersion->insertItem(tr("Selected by Query"), this, SLOT(popupDispersionSlot(int)), 0, 1);
	popupDispersion->insertItem(tr("Not Selected by Query"), this, SLOT(popupDispersionSlot(int)), 0, 2);
	popupDispersion->insertSeparator();
	popupDispersion->insertItem(tr("Grouped"), this, SLOT(popupDispersionSlot(int)), 0, 3);
	popupDispersion->insertItem(tr("Not Grouped"), this, SLOT(popupDispersionSlot(int)), 0, 4);

	popupViewport->insertSeparator();
	popupViewport->insertItem(tr("Calculate Area"), this, SLOT(popupCalculateAreaSlot()));
	popupViewport->insertItem(tr("Calculate Perimeter"), this, SLOT(popupCalculatePerimeterSlot()));

	QPopupMenu *popupCountComputing = new QPopupMenu;
	popupViewport->insertItem(tr("Count Computing"), popupCountComputing);
	popupCountComputing->insertItem(tr("Collection..."), this, SLOT(popupCalculateCountSlot(int)), 0, 0);
	popupCountComputing->insertItem(tr("Selected by Query..."), this, SLOT(popupCalculateCountSlot(int)), 0, 1);
	popupCountComputing->insertItem(tr("Selected by Pointing..."), this, SLOT(popupCalculateCountSlot(int)), 0, 2);
	popupCountComputing->insertItem(tr("Not Selected by Query..."), this, SLOT(popupCalculateCountSlot(int)), 0, 3);
	popupCountComputing->insertItem(tr("Not Selected by Pointing..."), this, SLOT(popupCalculateCountSlot(int)), 0, 4);
	popupCountComputing->insertItem(tr("Selected by Query or Pointing..."), this, SLOT(popupCalculateCountSlot(int)), 0, 5);
	popupCountComputing->insertItem(tr("Selected by Query and Pointing..."), this, SLOT(popupCalculateCountSlot(int)), 0, 6);

	popupViewport->insertSeparator();
	popupViewport->insertItem(tr("Display Grid Information"), this, SLOT(popupShowGridInformationSlot()));
	popupViewport->changeItem(popupViewport->idAt(23), iconCheck, tr("Display Grid Information"));

	//connect the popups signals from the grid
	connect(grid_,SIGNAL(horizHeaderSignal(int)), this, SLOT(popupHorizHeaderSlot(int)));
	connect(grid_,SIGNAL(vertHeaderSignal(int)), this, SLOT(popupVertHeaderSlot(int)));
	connect(grid_,SIGNAL(viewportSignal()), this, SLOT(popupViewportSlot()));
	connect(grid_,SIGNAL(displayGridInformation(QString)), this, SLOT(displayGridInformationSlot(QString)));
	connect(grid_,SIGNAL(clearGridInformation()), this, SLOT(clearGridInformationSlot()));

	//Initialize the ToolBarAction and StatusBarAction with the check pixmap
	showToolBarAction->setIconSet((QPixmap)check);
	showStatusBarAction->setIconSet((QPixmap)check);
	
	currentDatabase_ = 0;
    currentLayer_ = 0;
	currentView_ = 0;
	currentTheme_ = 0;
	databaseProp_ = 0;
	layerProp_ = 0;
	viewProp_ = 0;
	themeProp_ = 0;
	graphicParams_ = 0;
	query_ = 0;
	kernel_ = 0;
  	kernelRatio_ = 0;
	addressLocatorWindow_ = 0;

	if (getenv("GEOTIFF_CSV") == NULL)
	{
//		string geotifcsv = "GEOTIFF_CSV=" + absPath + "/csv";
		string geotifcsv = "GEOTIFF_CSV=";
		geotifcsv +=  ".";
#ifdef WIN32
		geotifcsv += "\\csv";
#else
		geotifcsv += "/csv";
#endif
		putenv((char*)(geotifcsv.c_str()));
	}

	//Oracle var
	if (getenv("NLS_NUMERIC_CHARACTERS") == NULL)
	{
		string numericChar = "NLS_NUMERIC_CHARACTERS=.,";
		putenv((char*)(numericChar.c_str()));
	}

	TeInitRasterDecoders();
	TeDBConnectionsPool::instance().clear();

	progress_ = new TeQtProgress(this, "ProgressDialog", true);
	progress_->setCancelButtonText("Cancelar");
	TeProgress::setProgressInterf((TeProgressBase*)progress_);

	canvas_->setWorld(TeBox(), cvContentsW_, cvContentsH_);
	resetBoxesVector();
	graphicAction_toggled(false);	

	scaleComboBox->insertItem("1000");
	scaleComboBox->insertItem("2000");
	scaleComboBox->insertItem("2500");
	scaleComboBox->insertItem("5000");
	scaleComboBox->insertItem("10000");
	scaleComboBox->insertItem("20000");
	scaleComboBox->insertItem("25000");
	scaleComboBox->insertItem("50000");
	scaleComboBox->insertItem("100000");
	scaleComboBox->insertItem("200000");
	scaleComboBox->insertItem("250000");
	scaleComboBox->insertItem("500000");
	scaleComboBox->insertItem("1000000");
	scaleComboBox->insertItem("2000000");
	scaleComboBox->insertItem("2500000");
	scaleComboBox->insertItem("5000000");
	scaleComboBox->insertItem("10000000");
	scaleComboBox->insertItem("20000000");
	scaleComboBox->insertItem("25000000");
	scaleComboBox->insertItem("50000000");
	scaleComboBox->insertItem("100000000");
	scaleComboBox->setEnabled(false);			
	scaleComboBox->setCurrentText("");
	scaleComboBox->setMaxCount(scaleComboBox->count()+1);
	vectorialExportAction->setVisible(false);
	infolayerMultiLayer_Vectorial_ExportAction->setText(tr("Vectorial Export..."));
	appCaption_ = caption();
}


void TerraViewBase::execPopupFromDatabasesListViewSlot()
{
	TeQtCheckListItem *checkItem = (TeQtCheckListItem*)databasesListView_->popupItem();
	QPopupMenu* popupMenu = databasesListView_->getPopupMenu();
	popupMenu->clear();

	if (checkItem == 0)
		popupMenu->insertItem(QString(tr("Open Database...")), this, SLOT(openDatabaseAction_activated()));
	else if (checkItem->getType() == TeQtCheckListItem::DATABASE)
	{
		TeQtDatabaseItem* dbItem = (TeQtDatabaseItem*)(databasesListView_->popupItem());
		TeDatabase* db = dbItem->getDatabase();

		popupMenu->insertItem(QString(tr("Remove External Table...")), this, SLOT(popupRemoveExternalTable()));
		popupMenu->insertSeparator();
		popupMenu->insertItem(QString(tr("Reconnect Database")), this, SLOT(popupDatabaseRefreshSlot()));
		popupMenu->insertSeparator();

		QPopupMenu* popupDatabaseInfo = new QPopupMenu;
		popupMenu->insertItem(QString(tr("General Info")), popupDatabaseInfo);
		QPopupMenu* popupDatabaseInfoInsert = new QPopupMenu;
		popupDatabaseInfo->insertItem(QString(tr("Insert")), popupDatabaseInfoInsert);
		popupDatabaseInfoInsert->insertItem(QString(tr("File")), this, SLOT(popupInsertFileDatabaseInfo()));
		popupDatabaseInfoInsert->insertItem(QString(tr("URL")), this, SLOT(popupInsertURLDatabaseInfo()));
		popupDatabaseInfo->insertItem(QString(tr("Retrieve")), this, SLOT(popupRetrieveDatabaseInfo()));
		popupDatabaseInfo->insertItem(QString(tr("Remove")), this, SLOT(popupRemoveDatabaseInfo()));
		popupMenu->insertSeparator();
		popupMenu->insertItem(QString(tr("Properties...")), this, SLOT(popupDatabaseProperties()));

		if (db == currentDatabase_)
			popupMenu->setItemEnabled(popupMenu->idAt(2), true);	// reconnect database
		else
			popupMenu->setItemEnabled(popupMenu->idAt(2), false);

		string file = "";
		if(db->tableExist("tv_generalinfo"))
		{
			TeDatabasePortal *portal = db->getPortal();
			string query = "SELECT name_ FROM tv_generalinfo WHERE layer_id = -1";
			if(portal->query(query) && portal->fetchRow())
				file = portal->getData(0);

			delete portal;
		}

		if(file.empty())
		{
			popupDatabaseInfo->setItemEnabled(popupDatabaseInfo->idAt(0), true); // insert information 
			popupDatabaseInfo->setItemEnabled(popupDatabaseInfo->idAt(1), false); // retrieve information 
			popupDatabaseInfo->setItemEnabled(popupDatabaseInfo->idAt(2), false); // remove information 
		}
		else
		{
			popupDatabaseInfo->setItemEnabled(popupDatabaseInfo->idAt(0), false); // insert information 
			popupDatabaseInfo->setItemEnabled(popupDatabaseInfo->idAt(1), true); // retrieve information 
			popupDatabaseInfo->setItemEnabled(popupDatabaseInfo->idAt(2), true); // remove information 
		}
	}
	else if (checkItem->getType() == TeQtCheckListItem::INFOLAYER)
	{
		TeLayer* layer = ((TeQtLayerItem*)(checkItem))->getLayer();
		TeDatabase *db = layer->database();

		popupMenu->insertItem(QString(tr("Rename")), this, SLOT(popupRenameDatabasesListViewItem()));
		popupMenu->insertItem(QString(tr("Remove")), this, SLOT(popupRemoveDatabasesListViewItem()));
		popupMenu->insertSeparator();
		popupMenu->insertItem(QString(tr("Create Cells...")), this, SLOT(createCellsAction_activated()));
		popupMenu->insertSeparator();
		popupMenu->insertItem(QString(tr("Export Layer...")), this, SLOT(vectorialExportAction_activated()));
		popupMenu->insertSeparator();
		popupMenu->insertItem(tr("Create Layer Table..."), this, SLOT(createLayerTable()));
		popupMenu->insertItem(QString(tr("Delete Layer Table...")), this, SLOT(deleteLayerTable()));
		popupMenu->insertSeparator();
		popupMenu->insertItem(QString(tr("Add Centroid Representation")), this, SLOT(popupAddCentroidRepDatabasesListViewItem()));
		popupMenu->insertItem(QString(tr("Create Layer of Centroids")), this, SLOT(popupCreateCentroidLayerDatabasesListViewItem()));
		popupMenu->insertItem(QString(tr("Create Layer of Centroids Counting")), this, SLOT(popupCreateCountLayerDatabasesListViewItem()));
		popupMenu->insertSeparator();
		popupMenu->insertItem(QString(tr("Load Memory")), this, SLOT(popupLayerLoadMemory()));
		popupMenu->insertItem(QString(tr("Clear Memory")), this, SLOT(popupLayerClearMemory()));
		popupMenu->insertSeparator();
		popupMenu->insertItem(QString(tr("Projection...")), this, SLOT(popupLayerProjection()));
		popupMenu->insertSeparator();

		QPopupMenu* popupLayerInfo = new QPopupMenu;
		popupMenu->insertItem(QString(tr("General Info")), popupLayerInfo);
		QPopupMenu* popupLayerInfoInsert = new QPopupMenu;
		popupLayerInfo->insertItem(QString(tr("Insert")), popupLayerInfoInsert);
		popupLayerInfoInsert->insertItem(QString(tr("File")), this, SLOT(popupInsertFileLayerInfo()));
		popupLayerInfoInsert->insertItem(QString(tr("URL")), this, SLOT(popupInsertURLLayerInfo()));
		popupLayerInfo->insertItem(QString(tr("Retrieve")), this, SLOT(popupRetrieveLayerInfo()));
		popupLayerInfo->insertItem(QString(tr("Remove")), this, SLOT(popupRemoveLayerInfo()));
		popupMenu->insertSeparator();
		popupMenu->insertItem(QString(tr("Properties...")), this, SLOT(layerProperties()));

		if(layer->geomRep() & TePOLYGONS || layer->geomRep() & TeLINES)
		{
			map<int, RepMap>::iterator it = layerId2RepMap_.find(layer->id());
			if(it == layerId2RepMap_.end())
			{
				popupMenu->setItemEnabled(popupMenu->idAt(12), true);	// load memory
				popupMenu->setItemEnabled(popupMenu->idAt(13), false);	// clear memory
			}
			else
			{
				popupMenu->setItemEnabled(popupMenu->idAt(12), false);	// load memory
				popupMenu->setItemEnabled(popupMenu->idAt(13), true);	// clear memory
			}
		}

		if(layer->geomRep() & TePOLYGONS || layer->geomRep() & TeLINES || layer->geomRep() & TeCELLS )
		{
			popupMenu->setItemEnabled(popupMenu->idAt(9), true);		// create layer of centroids
			if((layer->geomRep() & TePOINTS) == false)
				popupMenu->setItemEnabled(popupMenu->idAt(8), true);	// add centroid representation
			else
				popupMenu->setItemEnabled(popupMenu->idAt(8), false);	// add centroid representation
		}
		else
		{
			popupMenu->setItemEnabled(popupMenu->idAt(9), false);	// create layer of centroids
			popupMenu->setItemEnabled(popupMenu->idAt(8), false);	// add centroid representation
		}

		if(layer->geomRep() & TePOINTS)
		{
			string sel = "SELECT COUNT(*) FROM " + layer->tableName(TePOINTS);
			sel += " GROUP BY x, y ORDER BY COUNT(*) DESC";
			int max = 1;
			TeDatabasePortal *portal = db->getPortal();
			if(portal->query(sel))
			{
				if(portal->fetchRow())
					max = portal->getInt(0);
			}
			delete portal;
			if(max > 1)
				popupMenu->setItemEnabled(popupMenu->idAt(10), true);		// create layer of centroids counting
			else
				popupMenu->setItemEnabled(popupMenu->idAt(10), false);		// create layer of centroids counting
		}
		else
			popupMenu->setItemEnabled(popupMenu->idAt(10), false);			// create layer of centroids counting

		std::string file = "";

		if(db->tableExist("tv_generalinfo"))
		{
			TeDatabasePortal *portal = db->getPortal();
			string query = "SELECT name_ FROM tv_generalinfo WHERE layer_id = " + Te2String(layer->id());
			if(portal->query(query) && portal->fetchRow())
				file = portal->getData(0);
			delete portal;
		}

		if(file.empty())
		{
			popupLayerInfo->setItemEnabled(popupLayerInfo->idAt(0), true); // insert information 
			popupLayerInfo->setItemEnabled(popupLayerInfo->idAt(1), false); // retrieve information 
			popupLayerInfo->setItemEnabled(popupLayerInfo->idAt(2), false); // remove information 
		}
		else
		{
			popupLayerInfo->setItemEnabled(popupLayerInfo->idAt(0), false); // insert information 
			popupLayerInfo->setItemEnabled(popupLayerInfo->idAt(1), true); // retrieve information 
			popupLayerInfo->setItemEnabled(popupLayerInfo->idAt(2), true); // remove information 
		}
	}

	popupMenu->exec();
}


void TerraViewBase::execPopupFromViewsListViewSlot()
{
	unsigned int i;
	TeQtCheckListItem *checkItem = (TeQtCheckListItem*)viewsListView_->popupItem();
	if (checkItem == 0)
		return;

	if(checkItem->isEnabled() == false)
		return;

	QPopupMenu* popupMenu = viewsListView_->getPopupMenu();
	popupMenu->clear();

	if (checkItem->getType() == TeQtCheckListItem::VIEW)
	{
		TeQtViewItem* popupViewItem = (TeQtViewItem*)viewsListView_->popupItem();
		popupMenu->insertItem(QString(tr("Rename")), this, SLOT(popupRenameViewsListViewItem()));
		popupMenu->insertItem(QString(tr("Remove")), this, SLOT(popupRemoveViewsListViewItem()));
		popupMenu->insertItem(QString(tr("Export Themes")), this, SLOT(popupExportThemesViewListViewItem()));
		popupMenu->insertSeparator();
		if (popupViewItem == viewsListView_->currentViewItem())
		{
			popupMenu->insertItem(QString(tr("Projection...")), this, SLOT(popupViewProjection()));
			popupMenu->insertSeparator();

			if (currentDatabase_->viewMap().size() > 1)
			{
				popupMenu->insertItem(QString(tr("Connect...")), this, SLOT(popupViewConnect()));
				popupMenu->insertSeparator();
			}

			QPopupMenu *popupGeoprocessing = new QPopupMenu;
			popupMenu->insertItem(QString(tr("Geoprocessing")), popupGeoprocessing);
			popupGeoprocessing->insertItem(QString(tr("Aggregation...")), this, SLOT(popupGeoprocessingSlot(int)), 0, 0);
			popupGeoprocessing->insertItem(QString(tr("Add...")), this, SLOT(popupGeoprocessingSlot(int)), 0, 1);
			popupGeoprocessing->insertItem(QString(tr("Intersection...")), this, SLOT(popupGeoprocessingSlot(int)), 0, 2);
			popupGeoprocessing->insertItem(QString(tr("Difference...")), this, SLOT(popupGeoprocessingSlot(int)), 0, 3);
			QPopupMenu *popupGeoprocessingAssignData = new QPopupMenu;
			popupGeoprocessing->insertItem(QString(tr("Assign Data By Location")), popupGeoprocessingAssignData);
			popupGeoprocessingAssignData->insertItem(QString(tr("Distribute...")), this, SLOT(popupGeoprocessingSlot(int)), 0, 5);
			popupGeoprocessingAssignData->insertItem(QString(tr("Collect...")), this, SLOT(popupGeoprocessingSlot(int)), 0, 6);
 			popupGeoprocessing->insertItem(QString(tr("Create Buffers...")), this, SLOT(popupGeoprocessingSlot(int)), 0, 7);
			popupMenu->insertSeparator();
		}

		popupMenu->insertItem(QString(tr("Properties...")), this, SLOT(viewProperties()));
	}
	else if (checkItem->getType() == TeQtCheckListItem::THEME)
	{
		TeQtThemeItem* popupThemeItem = (TeQtThemeItem*)viewsListView_->popupItem();
		TeAppTheme *popupAppTheme = popupThemeItem->getAppTheme();

		if (popupThemeItem->isThemeItemInvalid() == true)
		{
			popupMenu->insertItem(tr("Remove"), this, SLOT(popupRemoveViewsListViewItem()));	// Remove popup
			popupMenu->insertItem(tr("Properties..."), this, SLOT(themeProperties()));
			popupMenu->exec();
			return;
		}

		if(popupAppTheme->getTheme()->type() > 3)	// WMS theme builds its own popumenu...
		{
			TeCoord2D menuLocation(popupMenu->x(), popupMenu->y());
			PluginsSignal sigpopup(PluginsSignal::S_SHOW_THEME_POPUP);
			sigpopup.signalData_.store(PluginsSignal::D_QT_THEMEITEM, popupThemeItem);
			sigpopup.signalData_.store(PluginsSignal::D_THEMETYPE, popupThemeItem->getAppTheme()->getTheme()->type());
			sigpopup.signalData_.store(PluginsSignal::D_COORD2D, menuLocation);
			PluginsEmitter::instance().send(sigpopup);
			return;
		}
		else if ( popupAppTheme->getTheme()->type() == 3) // File theme
		{
			popupMenu->insertItem(tr("Rename"), this, SLOT(popupRenameViewsListViewItem()));	// Rename popup
			popupMenu->insertItem(tr("Remove"), this, SLOT(popupRemoveViewsListViewItem()));	// Remove popup
			popupMenu->insertSeparator();
			QPopupMenu *popupThemeVisual = new QPopupMenu;
			popupMenu->insertItem(tr("Visual"), popupThemeVisual);									// Visual popup
			popupThemeVisual->insertItem(tr("Default..."), this, SLOT(popupDefaultVisual()));
			popupThemeVisual->insertItem(tr("Pointing..."), this, SLOT(popupPointingVisual()));
			popupMenu->insertItem(tr("Visibility of the Representations..."), this, SLOT(popupThemeVisibility()));	// Rep. visibility
			popupMenu->insertSeparator();
			popupMenu->insertItem(tr("Properties..."), this, SLOT(themeProperties()));
			popupMenu->exec();
			return;
		}

		bool enableVect = false;
		bool enableRst = false;

		int tRep = popupAppTheme->getTheme()->visibleGeoRep();
		enableVect = tRep & TePOLYGONS || tRep & TeLINES ||  tRep & TePOINTS || tRep & TeCELLS;
		enableRst = tRep & TeRASTER || tRep & TeRASTERFILE;

		bool kernelIsVisible = true;
		if ((kernel_==0) || (kernel_ && kernel_->isHidden()))
			kernelIsVisible = false;

		bool themeIsCurrent = false;
		if (popupThemeItem == viewsListView_->currentThemeItem())
			themeIsCurrent = true;

		bool insertSep = false;
		if (kernelIsVisible == false)
		{
			popupMenu->insertItem(tr("Rename"), this, SLOT(popupRenameViewsListViewItem()));	// Rename popup
			popupMenu->insertItem(tr("Remove"), this, SLOT(popupRemoveViewsListViewItem()));	// Remove popup
			insertSep = true;
		}

		if (insertSep == true)
		{
			popupMenu->insertSeparator();
			insertSep = false;
		}

		if (popupAppTheme->getTheme()->type() < 3 )  
		{
			QPopupMenu *popupThemeVisual = new QPopupMenu;
			popupMenu->insertItem(tr("Visual"), popupThemeVisual);									// Visual popup
			popupThemeVisual->insertItem(tr("Default..."), this, SLOT(popupDefaultVisual()));
			popupThemeVisual->insertItem(tr("Pointing..."), this, SLOT(popupPointingVisual()));
			popupThemeVisual->insertItem(tr("Query..."), this, SLOT(popupQueryVisual()));
			popupThemeVisual->insertItem(tr("Pointing and Query..."), this, SLOT(popupPointingQueryVisual()));
			popupThemeVisual->insertSeparator();
			popupThemeVisual->insertItem(tr("Text..."), this, SLOT(textVisual()));
			popupMenu->insertItem(tr("Visibility of the Representations..."), this, SLOT(popupThemeVisibility()));	// Rep. visibility
		}

		if (enableVect == true && enableRst == false)
		{
			QPopupMenu *popupThemeRemoveColors = new QPopupMenu;
			popupMenu->insertItem(tr("Remove Colors"), popupThemeRemoveColors);				// Remove Colors
			popupThemeRemoveColors->insertItem(tr("Pointed Objects"), this, SLOT(popupRemovePointingColor()));
			popupThemeRemoveColors->insertItem(tr("Queried Objects"), this, SLOT(popupRemoveQueryColor()));
		}

		popupMenu->insertSeparator();

		if (enableVect == true && enableRst == false && themeIsCurrent == true)
		{
			popupMenu->insertItem(tr("Attribute Query..."), this, SLOT(attributesQuery()));		// Attribute Query
			popupMenu->insertItem(tr("Spatial Query..."), this, SLOT(spatialQuery()));			// Spatial Query
			insertSep = true;
		}

		if (insertSep == true)
		{
			popupMenu->insertSeparator();
			insertSep = false;
		}

		if (themeIsCurrent == true)
		{
			popupMenu->insertItem(tr("Edit Legend..."), this, SLOT(editLegend()));					// Edit Legend
			insertSep = true;
		}

		if (enableVect == true && enableRst == false && themeIsCurrent == true)
		{
			popupMenu->insertItem(tr("Edit Bar or Pie Chart..."), this, SLOT(editPieBarChart()));	// Edit Bar or Pie Chart
			insertSep = true;
		}

		if (themeIsCurrent == true)
		{
			popupMenu->insertItem(tr("Text Representation..."), this, SLOT(textRepresentation()));	// Text Representation
			insertSep = true;
		}

		if (enableVect == true && enableRst == false && themeIsCurrent == true)
		{
				popupMenu->insertItem(tr("Graphic Parameters..."), this, SLOT(setGraphicParams()));	// Graphic Parameters
				insertSep = true;
		}

		if (insertSep == true)
		{
			popupMenu->insertSeparator();
			insertSep = false;
		}

		if (enableVect == true && enableRst == false && themeIsCurrent == true && kernelIsVisible == false)
		{
			popupMenu->insertItem(tr("Select Theme Tables..."), this, SLOT(selectThemeTables())); //Select Theme Tables 
			insertSep = true;
		}

		if (enableVect == true && enableRst == false && themeIsCurrent == true && kernelIsVisible == false)
		{
			TeAttrTableVector tableVec; 
			TeTheme* th = (TeTheme*)currentTheme_->getTheme();
			if(th->type() == TeTHEME || th->type() == TeEXTERNALTHEME)
			{
				th->getAttTables(tableVec);

				for (i = 0; i < tableVec.size(); ++i)
				{
					TeTable t = tableVec[i];
					if(t.tableType() == TeFixedGeomDynAttr || t.tableType() == TeDynGeomDynAttr)
						break;
				}

				if(i >= tableVec.size())
					popupMenu->insertItem(tr("Link External Table..."), this, SLOT(linkExternalTable())); // Link Ext. Table

				insertSep = true;
			}
		}

		if (insertSep == true)
		{
			popupMenu->insertSeparator();
			insertSep = false;
		}

		popupMenu->insertItem(tr("Create Layer from Theme..."), this, SLOT(popupCreateLayerFromTheme()));
		popupMenu->insertItem(tr("Create Theme from Theme..."), this, SLOT(popupCreateThemeFromTheme()));
		popupMenu->insertItem(tr("Save Theme to File..."), this, SLOT(popupSaveThemeToFile()));

		popupMenu->insertSeparator();

		if (enableVect == false && enableRst == true && themeIsCurrent == true)
		{
			popupMenu->insertItem(tr("Image Enhancement..."), this, SLOT(popupImageContrast()));
			popupMenu->insertItem(tr("Raster Transparency..."),this,SLOT(popuRasterTransparencySlot()));
			popupMenu->insertItem(tr("Raster Display Properties..."), this, SLOT(popupRasterDisplaySlot()));

			/* Image processing Pop-up group */

			QPopupMenu *popupImgProc = new QPopupMenu;
			popupMenu->insertItem(tr("Image Processing" ), popupImgProc);

			popupImgProc->insertItem(tr("Histogram"), this, SLOT(popupRasterHistogramSlot()));
			showRasterDisplayProperties();
			insertSep = true;
		}

		if (insertSep == true)
		{
			popupMenu->insertSeparator();
			insertSep = false;
		}

		if (enableVect == true && enableRst == false && themeIsCurrent == true)
		{
			popupMenu->insertItem(tr("Locate Address..."), this, SLOT(popupLocateAddress()));
			popupMenu->insertSeparator();
		}

		popupMenu->insertItem(tr("Remove All Aliases"), this, SLOT(popupThemeRemoveAllAlias()));
		popupMenu->insertSeparator();
		popupMenu->insertItem(tr("Properties..."), this, SLOT(themeProperties()));
	}
	else if (checkItem->getType() == TeQtCheckListItem::LEGENDTITLE)
	{
		TeQtThemeItem* themeItem = (TeQtThemeItem*)checkItem->parent();
		if(themeItem->isThemeItemInvalid() == false)
		{
			TeAppTheme* appTheme = (TeAppTheme*)themeItem->getAppTheme();
			TeTheme* theme = (TeTheme*)appTheme->getTheme();
			TeDatabase* database = theme->layer()->database();
			TeAttributeRep attrRep; 
			if(theme->grouping().groupMode_ != TeNoGrouping)
				attrRep = theme->grouping().groupAttribute_;
			
			if(currentTheme_ && ((TeTheme*)currentTheme_->getTheme())->id() == theme->id())
				popupMenu->insertItem(tr("Edit Legend..."), this, SLOT(editLegend()));					// Edit Legend
		
			popupMenu->insertItem(tr("Remove Legend"), this, SLOT(popupRemoveViewsListViewItem()));		// Remove Legend
			popupMenu->insertSeparator();
			popupMenu->insertItem(QString(tr("Set Alias")), this, SLOT(popupRenameViewsListViewItem()));		// Set Alias

			string text = attrRep.name_;
			map<int, map<string, string> >::iterator it = database->mapThemeAlias().find(theme->id());
			if(it != database->mapThemeAlias().end())
			{
				map<string, string>& m = it->second;
				map<string, string>::iterator tit = m.find(text);
				if(tit != m.end())
					popupMenu->insertItem(QString(tr("Remove Alias")), this, SLOT(popupRemoveAliasViewsListViewItem())); // Remove Alias
			}
		}
	}
	else if (checkItem->getType() == TeQtCheckListItem::LEGEND)
	{
		TeQtThemeItem* themeItem = (TeQtThemeItem*)checkItem->parent();
		if(themeItem->isThemeItemInvalid() == false)
		{
			TeQtLegendItem *legendItem = (TeQtLegendItem*)viewsListView_->popupItem();

			popupMenu->insertItem(tr("Rename Legend..."), this, SLOT(popupRenameViewsListViewItem()));

			if (legendItem->parent() == viewsListView_->currentThemeItem())
				popupMenu->insertItem(tr("Change Visual..."), this, SLOT(popupChangeLegendVisualSlot()));			
		}
	}
	else if (checkItem->getType() == TeQtCheckListItem::CHARTTITLE)
	{
		TeQtThemeItem* themeItem = (TeQtThemeItem*)checkItem->parent();
		if(themeItem->isThemeItemInvalid() == false)
		{
			popupMenu->insertItem(tr("Edit Bar or Pie Chart..."), this, SLOT(editPieBarChart()));
			popupMenu->insertItem(tr("Remove Chart"), this, SLOT(popupRemoveViewsListViewItem()));
		}
	}
	else if (checkItem->getType() == TeQtCheckListItem::CHART)
	{
		TeQtThemeItem* themeItem = (TeQtThemeItem*)checkItem->parent();
		if(themeItem->isThemeItemInvalid() == false)
		{
			TeQtThemeItem* themeItem = (TeQtThemeItem*)checkItem->parent();
			TeAppTheme* appTheme = themeItem->getAppTheme();
			TeTheme* theme = (TeTheme*)appTheme->getTheme();
			TeDatabase* database = theme->layer()->database();
			int item = -1;
			TeQtCheckListItem* citem = checkItem;

			popupMenu->insertItem(tr("Change Color..."), this, SLOT(popupChangeChartColorSlot()));
			popupMenu->insertItem(QString(tr("Set Alias")), this, SLOT(popupRenameViewsListViewItem()));

			while(citem->getType() == TeQtCheckListItem::CHART)
			{
				++item;
				citem = (TeQtCheckListItem*)citem->itemAbove();
			}

			string text = appTheme->chartAttributes_[item];
			map<int, map<string, string> >::iterator it = database->mapThemeAlias().find(theme->id());
			if(it != database->mapThemeAlias().end())
			{
				map<string, string>& m = it->second;
				map<string, string>::iterator tit = m.find(text);
				if(tit != m.end())
					popupMenu->insertItem(QString(tr("Remove Alias")), this, SLOT(popupRemoveAliasViewsListViewItem()));
			}
		}
	}

	popupMenu->exec();
}


void TerraViewBase::popupHorizHeaderSlot(int itemCol)
{
	if(currentTheme_ == NULL)
	{
		return;
	}

	columnTooltip_ = itemCol;
	gridColumn_ = itemCol; // column swapped
	bool index = grid_->isIndex(itemCol);
	bool attrTemp = grid_->isDateTimeRegistered(itemCol);
	TeTheme* baseTheme = (TeTheme*)currentTheme_->getTheme();
	if(baseTheme->sqlAttList()[itemCol].rep_.type_ == TeREAL || 
	   baseTheme->sqlAttList()[itemCol].rep_.type_ == TeINT)
	{
		grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(0+3), true); // statistic
		grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(1+3), true); // histogram
		grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(2+3), true); // normal prob
		popupTooltip_->setItemEnabled(popupTooltip_->idAt(4), true); // AVG
		popupTooltip_->setItemEnabled(popupTooltip_->idAt(5), true); // SUM
	}
	else
	{
		grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(0+3), false); // statistic 
		grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(1+3), false); // histogram 
		grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(2+3), false); // normal prob 
		popupTooltip_->setItemEnabled(popupTooltip_->idAt(4), false); // AVG
		popupTooltip_->setItemEnabled(popupTooltip_->idAt(5), false); // SUM
	}

	if((kernel_==0) || (kernel_ && kernel_->isHidden())) // kernel hidden
	{
		grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(6), true); // add col

		if(index || attrTemp)
		{
			grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(7+3), false); // delete col
			grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(9+3), false); // change col Name
			grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(10+3), false); // change col Type
			if(attrTemp)
				grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(8), true); // change col data
			else
				grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(8), false); // change col data
		}
		else
		{
			grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(7+3), true); // delete col
			grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(8+3), true); // change col data
			grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(9+3), true); // change col Name
			grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(10+3), true); // change col Type
		}

		if(grid_->isExternalTable(itemCol) == true)
			grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(12+3), true); // external table 
		else
			grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(12+3), false); // external table 
	}
	else
	{
		grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(6+3), false); // add col
		grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(7+3), false); // delete col
		grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(8+3), false); // change col data
		grid_->popupHorizHeader()->setItemEnabled(grid_->popupHorizHeader()->idAt(12+3), false); // unlink external table 
	}

	grid_->popupHorizHeader()->exec();
}


void TerraViewBase::popupVertHeaderSlot(int rowPointed)
{
	mediaId_ = grid_->getObject(rowPointed);
	grid_->popupVertHeader()->exec();		
}

void TerraViewBase::popupVerticalHeaderDblClickSlot( int row )
{
	mediaId_ = grid_->getObject(row);
	popupShowDefaultMediaSlot();
}


void TerraViewBase::popupViewportSlot()
{
	QIconSet iconCheck((QPixmap)check);
	QIconSet iconUncheck((QPixmap)uncheck);
	unsigned int i, n, nc = grid_->numCols();
	vector<int>	cols;

	for(n = 0; n < nc; ++n)
		if (grid_->isColumnSelected(n, true) == true)
			cols.push_back(n);

	if(cols.size() == 0)
	{
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(2), false); // sort asc	
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(3), false); // sort desc	
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(5), false); // swap	
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(6), false); // hide
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(8), false); // delete
	}
	else if(cols.size() == 1)
	{
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(2), true); // sort asc	
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(3), true); // sort desc	
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(5), false); // swap	
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(6), true); // hide
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(8), true); // delete
	}
	else
	{
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(2), true); // sort asc	
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(3), true); // sort desc	
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(5), true); // swap	
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(6), true); // hide
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(8), true); // delete
	}

	for(n = 0; n < nc; ++n)
		if(grid_->getVisColumn(n) == false)
			break;
	if(n < nc)
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(7), true); // show all columns
	else
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(7), false); // show all columns

	if((kernel_==0) || (kernel_ && kernel_->isHidden())) // kernel hidden
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(14), true); // table
	else
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(14), false); // table


	if(currentTheme_ == NULL)
	{
		return;
	}

	int disp = 0;
	TeTheme* baseTheme = (TeTheme*)currentTheme_->getTheme();
	for(n = 0; n < cols.size(); ++n)
	{
		if(baseTheme->sqlAttList()[cols[n]].rep_.type_ == TeREAL ||
		   baseTheme->sqlAttList()[cols[n]].rep_.type_ == TeINT)
			disp++;
	}
	if(disp >= 2)
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(16), true); // dispersion
	else
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(16), false); // dispersion	

	if(disp >= 3)
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(17), true); // dispersion 3D
	else
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(17), false); // dispersion 3D

	TeAttrTableVector attrTableVector; 
	baseTheme->getAttTables(attrTableVector);
	TeTable& table = attrTableVector[0];

	bool area = true, perimeter = true;
	TeAttributeList AL = table.attributeList();
	for(i=0; i<AL.size(); i++)
	{
		string s = TeConvertToUpperCase(AL[i].rep_.name_);
		if(s == "AREA" || s == "?EA")
			area = false;
		if(s == "PERIMETER" || s == "PERIMETRO" || s == "PER?ETRO")
			perimeter = false;
	}
	TeLayer* layer = baseTheme->layer();
	if(layer->hasGeometry(TePOLYGONS) == false)
		area = false;
	if(layer->hasGeometry(TePOLYGONS) == false && layer->hasGeometry(TeLINES) == false)
		perimeter = false;

	grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(19), area); // calc area
	grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(20), perimeter); // calc perimeter 
	bool b = grid_->showGridInformation();
	if(b)
		grid_->popupViewport()->changeItem(grid_->popupViewport()->idAt(23), iconCheck, tr("Display Grid Information"));
	else
		grid_->popupViewport()->changeItem(grid_->popupViewport()->idAt(23), iconUncheck, tr("Display Grid Information"));

	bool external = false, bEvent = false, bTemp = false;
	TeAttrTableVector tableVec; 
	baseTheme->getAttTables(tableVec);

	for(i=0; i<tableVec.size(); i++)
	{
		TeTable t = tableVec[i];
		if(t.tableType() == TeAttrExternal)
			external = true;
		else if (t.tableType() == TeAttrEvent)
			bEvent = true;
		else if (t.tableType() == TeFixedGeomDynAttr || t.tableType() == TeDynGeomDynAttr)
			bTemp = true;
	}
	if (external == true)
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(21), true);	//calc counting
	else
		grid_->popupViewport()->setItemEnabled(grid_->popupViewport()->idAt(21), false);	//calc counting

	if (bEvent)
		popupTable_->setItemEnabled(popupTable_->idAt(0), false);
	else
		popupTable_->setItemEnabled(popupTable_->idAt(0), true);

	if (bTemp || bEvent)
		popupTable_->setItemEnabled(popupTable_->idAt(1), false);
	else
		popupTable_->setItemEnabled(popupTable_->idAt(1), true);

	grid_->popupViewport()->exec();		
}

void TerraViewBase::dropDatabaseAction_activated()
{
	dlgDropDatabase dropDatabase(this);

	dropDatabase.loadDatabase(currentDatabase_);
	dropDatabase.exec();
}

void TerraViewBase::closeDatabaseAction_activated()
{
	int response = QMessageBox::question(this, tr("Close Database"),
             tr("Do you really want to close the database?"),
			 tr("Yes"), tr("No"));

	if (response != 0)
		return;

	grid_->clear();
	canvas_->clearAll();

	if(currentDatabase_->projectMap().empty() == false)
	{
		TeProject* project = currentDatabase_->projectMap().begin()->second;
		if(currentView_)
			project->setCurrentViewId(currentView_->id());
		else
			project->setCurrentViewId(-1);

		currentDatabase_->updateProject(project);
	}

	TeViewMap& vm = currentDatabase_->viewMap();
	TeViewMap::iterator it = vm.begin();
	while(it != vm.end())
	{
		currentDatabase_->updateView(it->second);
		it++;
	}

	QString dname = currentDatabase_->databaseName().c_str();
	emit(dbChanged(dname));
	currentDatabase_->close();
	delete currentDatabase_;
	currentDatabase_ = 0;
	currentLayer_ = 0;
	currentView_ = 0;
	currentTheme_ = 0;

	//TeDBConnectionsPool::instance().clear();

	databasesListView_->removeItem((QListViewItem*)databasesListView_->currentDatabaseItem());
	viewsListView_->removeItem((QListViewItem*)viewsListView_->currentViewItem());
	viewsListView_->clear();
	checkWidgetEnabling();
}


void TerraViewBase::closeEvent( QCloseEvent *ce )
{
  int response = QMessageBox::question(this, tr("Exit"),
             tr("Do you really want to exit from the application?"),
			 tr("Yes"), tr("No"));

  if (response == 0)
  {
	TeWaitCursor wait;

	ce->accept();
#ifdef WIN32
	// delete TerraView_TEMP directory
	char wdir[100];
	GetWindowsDirectory((LPTSTR)wdir, 100);
	string swdir = wdir;
	int f = swdir.find(":\\");
	if(f >= 0)
		swdir = swdir.substr(0, f);
	swdir.append(":/TERRAVIEW_TEMP");

	//restore the decimal separator if it was changed
	if (origDecimalSepIsPoint_ == false)
	{
		HKEY    hk;
		DWORD	DataSize = 2;
		DWORD   Type = REG_SZ;
		char    buf[2];

		string key = "Control Panel\\International";
		string sepDecimal = "sDecimal";	
		string sepDecimalResult = "";

		if (RegOpenKeyExA(HKEY_CURRENT_USER, key.c_str(), 0, KEY_SET_VALUE, &hk) == ERROR_SUCCESS)
		{
			memset (buf, 0, 2);
			buf[0] = ',';
			DataSize = 2;
				
			RegSetValueExA(hk, sepDecimal.c_str(), NULL, Type, (LPBYTE)buf, DataSize);  			
			RegCloseKey (hk);
		}
	}

	if(currentDatabase_)
	{
		databasesListView_->removeItem((QListViewItem*)databasesListView_->currentDatabaseItem());
		viewsListView_->removeItem((QListViewItem*)viewsListView_->currentViewItem());

		if(currentDatabase_->projectMap().empty() == false)
		{
			TeProject* project = currentDatabase_->projectMap().begin()->second;
			if(currentView_)
				project->setCurrentViewId(currentView_->id());
			else
				project->setCurrentViewId(-1);

			currentDatabase_->updateProject(project);
		}

		TeViewMap& vm = currentDatabase_->viewMap();
		TeViewMap::iterator it = vm.begin();
		while(it != vm.end())
		{
			currentDatabase_->updateView(it->second);
			it++;
		}

		currentDatabase_->close();
		delete currentDatabase_;
		currentDatabase_ = 0;
	}
	
	TeDBConnectionsPool::instance().clear();

    /* Unloading all plugins */     
    plugins_manager_.setDefaultPluginsParameters( getPluginParameters() );    
    plugins_manager_.unloadPlugins();

	vecPluginFileNames.clear();
	vecPluginNames.clear();
	vecTranslatedNames.clear();

#else
    string swdir = "/tmp/TERRAVIEW_TEMP";
#endif

	QDir dir(swdir.c_str());
	if(dir.exists() == false)
		return;

	unsigned int i;
	for(i=0; i<dir.count(); i++)
	{
		QString file = dir[i];
		if(file.isEmpty() || file == "." || file == "..")
			continue;
		dir.remove(file, false);
	}
	dir.rmdir(swdir.c_str());
  }
  else
  {
	ce->ignore();
  }
}


void TerraViewBase::cursorActionGroup_selected(QAction *action)
{
	cursorSelected_ = action;

	if(currentTheme_ == 0)
	{
		if(action == distanceMeterAction || action == infoCursorAction || action == graphicCursorAction)
		{
			pointerCursorAction->setOn(true);
			return;
		}
	}
	cursorSelected_->setOn(true);

	if (action == pointerCursorAction)
	{
		if(enableEditAction->isOn())
		{
			enableEditAction->setIconSet((QPixmap)edit_xpm);
			QPixmap pm1(cursor_edit);
			QBitmap bm;
			bm = pm1;

			QPixmap pm2(cursor_edit_mask);
			QBitmap bmMask;
			bmMask = pm2;

			QCursor cursor(bm, bmMask, 7, 4);
			canvas_->viewport()->setCursor(cursor);
			canvas_->setMode (TeQtCanvas::Edit);
		}
		else
		{
			canvas_->setMode (TeQtCanvas::Pointer);
			canvas_->viewport()->setCursor( QCursor( ArrowCursor ) );
		}
	}
	else if (action == zoomCursorAction)
	{
		canvas_->setMode (TeQtCanvas::Area);
	    canvas_->viewport()->setCursor( QCursor( CrossCursor ) );
	}
	else if (action == zoomInAction)
	{
		QPixmap pm1(cursor_zoom_in);
		QBitmap bm;
		bm = pm1;

		QPixmap pm2(cursor_zoom_in_mask);
		QBitmap bmMask;
		bmMask = pm2;

		QCursor cursor(bm, bmMask, 12, 12);
		canvas_->setMode (TeQtCanvas::ZoomIn);

	    canvas_->viewport()->setCursor( QCursor( cursor ) );
	}
	else if (action == zoomOutAction)
	{
		QPixmap pm1(cursor_zoom_out);
		QBitmap bm;
		bm = pm1;

		QPixmap pm2(cursor_zoom_out_mask);
		QBitmap bmMask;
		bmMask = pm2;

		QCursor cursor(bm, bmMask, 12, 12);
		canvas_->setMode (TeQtCanvas::ZoomOut);
	    canvas_->viewport()->setCursor( QCursor( cursor ) );
	}
	else if (action == panCursorAction)
	{
		canvas_->setMode (TeQtCanvas::Pan);
	    canvas_->viewport()->setCursor( QCursor( SizeAllCursor ) );
	}
	else if(action == distanceMeterAction)
	{
		enableEditAction_toggled(false);

		QPixmap pm1(cursor_distance);
		QBitmap bm;
		bm = pm1;

		QPixmap pm2(cursor_distance_mask);
		QBitmap bmMask;
		bmMask = pm2;

		QCursor cursor(bm, bmMask, 7, 4);
		canvas_->viewport()->setCursor(cursor);
		canvas_->setMode (TeQtCanvas::Distance);
	}
	else if(action == infoCursorAction)
	{
		enableEditAction_toggled(false);

		QPixmap pm1(cursor_info);
		QBitmap bm;
		bm = pm1;

		QPixmap pm2(cursor_info_mask);
		QBitmap bmMask;
		bmMask = pm2;

		QCursor cursor(bm, bmMask, 7, 4);
		canvas_->viewport()->setCursor(cursor);
		canvas_->setMode (TeQtCanvas::Information);
	}
	else if(action == enableEditAction)
	{
		enableEditAction->setOn(true);
		enableEditAction->setIconSet((QPixmap)edit_xpm);
		QPixmap pm1(cursor_edit);
		QBitmap bm;
		bm = pm1;

		QPixmap pm2(cursor_edit_mask);
		QBitmap bmMask;
		bmMask = pm2;

		QCursor cursor(bm, bmMask, 7, 4);
		canvas_->viewport()->setCursor(cursor);
		canvas_->setMode (TeQtCanvas::Edit);
	}
	else
	{
		enableEditAction_toggled(false);

		canvas_->setMode (TeQtCanvas::Pointer);
		canvas_->viewport()->setCursor( QCursor( ArrowCursor ) );
	}

	if(action == graphicCursorAction && graphicCursorAction->isEnabled())
		emit graphicCursorActivated(true);
	else
		emit graphicCursorActivated(false);
}


void TerraViewBase::enableEditAction_toggled( bool b)
{
	if(currentTheme_ == 0)
	{
		enableEditAction->setOn(false);
		return;
	}
	//else
	//{
	//	TeAbstractTheme* theme = currentTheme_->getTheme();

	//	if((theme->type() != TeTHEME) && (theme->type() != TeEXTERNALTHEME))
	//	{
	//		QMessageBox::warning(this, tr("Warning"),
	//		tr("Can not execute edition operations on this kind of theme!"));
	//	}
	//}

	enableEditAction->setOn(b);
	if(b)
		cursorActionGroup_selected(pointerCursorAction);
	else
	{
		displayWindow_->resetEditions();
		enableEditAction->setIconSet((QPixmap)nonEdit_xpm);
		if (cursorSelected_ == pointerCursorAction)
		{
			canvas_->setMode (TeQtCanvas::Pointer);
			canvas_->viewport()->setCursor( QCursor( ArrowCursor ) );
		}
		else if (cursorSelected_ == zoomCursorAction)
		{
			canvas_->setMode (TeQtCanvas::Area);
			canvas_->viewport()->setCursor( QCursor( CrossCursor ) );
		}
		else if (cursorSelected_ == zoomInAction)
		{
			QPixmap pm1(cursor_zoom_in);
			QBitmap bm;
			bm = pm1;

			QPixmap pm2(cursor_zoom_in_mask);
			QBitmap bmMask;
			bmMask = pm2;

			QCursor cursor(bm, bmMask, 12, 12);
			canvas_->setMode (TeQtCanvas::ZoomIn);
			canvas_->viewport()->setCursor( QCursor( cursor ) );
		}
		else if (cursorSelected_ == zoomOutAction)
		{
			QPixmap pm1(cursor_zoom_out);
			QBitmap bm;
			bm = pm1;

			QPixmap pm2(cursor_zoom_out_mask);
			QBitmap bmMask;
			bmMask = pm2;

			QCursor cursor(bm, bmMask, 12, 12);
			canvas_->setMode (TeQtCanvas::ZoomOut);
			canvas_->viewport()->setCursor( QCursor( cursor ) );
		}
		else if (cursorSelected_ == panCursorAction)
		{
			canvas_->setMode (TeQtCanvas::Pan);
			canvas_->viewport()->setCursor( QCursor( SizeAllCursor ) );
		}
		else if(cursorSelected_ == distanceMeterAction)
		{
			QPixmap pm1(cursor_distance);
			QBitmap bm;
			bm = pm1;

			QPixmap pm2(cursor_distance_mask);
			QBitmap bmMask;
			bmMask = pm2;

			QCursor cursor(bm, bmMask, 7, 4);
			canvas_->viewport()->setCursor(cursor);
			canvas_->setMode (TeQtCanvas::Distance);
		}
		else if(cursorSelected_ == infoCursorAction)
		{
			QPixmap pm1(cursor_info);
			QBitmap bm;
			bm = pm1;

			QPixmap pm2(cursor_info_mask);
			QBitmap bmMask;
			bmMask = pm2;

			QCursor cursor(bm, bmMask, 7, 4);
			canvas_->viewport()->setCursor(cursor);
			canvas_->setMode (TeQtCanvas::Information);
		}
		else
		{
			canvas_->setMode (TeQtCanvas::Pointer);
			canvas_->viewport()->setCursor( QCursor( ArrowCursor ) );
		}
	}
}


void TerraViewBase::openDatabaseAction_activated()
{
	DatabaseWindow *database = new DatabaseWindow(this, "database", true);
	database->exec();
	delete database;

// emit signal to plugins
	if(databasesListView_->currentDatabaseItem() && databasesListView_->currentDatabaseItem()->getDatabase())
	{
		PluginsSignal sigrefresh(PluginsSignal::S_SET_DATABASE);
		sigrefresh.signalData_.store(PluginsSignal::D_DATABASE, databasesListView_->currentDatabaseItem()->getDatabase());
		PluginsEmitter::instance().send(sigrefresh);
	}

	drawAction_activated();
	databasesTreeAction_toggled(true);
	viewsTreeAction_toggled(true);
	checkWidgetEnabling();
}


void TerraViewBase::popupDatabaseProperties()
{
	TeQtDatabaseItem *dbItem = (TeQtDatabaseItem*)databasesListView_->popupItem();
	databaseProperties(dbItem);
}


void TerraViewBase::databaseProperties(TeQtDatabaseItem *dbItem)
{
	if (databaseProp_ == 0)
	{
		databaseProp_ = new DatabaseProperties(this, "databaseProp", false);

		databaseProp_->databasePropertiesTable->horizontalHeader()->setLabel(0, tr("Description"));
		databaseProp_->databasePropertiesTable->horizontalHeader()->setLabel(1, tr("Value"));

//		databaseProp_->databasePropertiesTable->setColumnStretchable(0, true);
//		databaseProp_->databasePropertiesTable->setColumnStretchable(1, true);
		databaseProp_->databasePropertiesTable->setLeftMargin(0);
	}

	TeDatabase *database = dbItem->getDatabase();
	vector<QString> params;
	vector<QString> vals;

	TeDatabasePortal *portal = database->getPortal();
	if(portal->query("SELECT table_id, attr_table, unique_id FROM te_layer_table WHERE attr_table_type = 3"))
	{
		while(portal->fetchRow())
		{
			params.push_back(tr("external table:"));
			vals.push_back(portal->getData(1));
			params.push_back(tr("table id:"));
			vals.push_back(portal->getData(0));
			params.push_back(tr("unique name:"));
			vals.push_back(portal->getData(2));
		}
	}
	delete portal;

	string dbName = database->databaseName();
	int m;

	if (database->dbmsName() == "Ado")
	{
		// extract the directory
		unsigned int position = dbName.rfind('/');
		if (position == std::string::npos)
			position = dbName.rfind('\\');
		string directory = dbName.substr(0,position);
		string name = dbName.substr(position + 1, dbName.length() - position);

		databaseProp_->databasePropertiesTable->setNumRows(3+(int)params.size());
		databaseProp_->databasePropertiesTable->setText(0, 0, tr("Database Name:"));
//		databaseProp_->databasePropertiesTable->setText(0, 1, dbItem->text(1));
		databaseProp_->databasePropertiesTable->setText(0, 1, name.c_str());
		databaseProp_->databasePropertiesTable->setText(1, 0, tr("Database Type:"));
		databaseProp_->databasePropertiesTable->setText(1, 1, tr("Microsoft Access-Ado"));
		databaseProp_->databasePropertiesTable->setText(2, 0, tr("Directory Name:"));
		databaseProp_->databasePropertiesTable->setText(2, 1, directory.c_str());
		m = 3;
	}
	else
	{
		databaseProp_->databasePropertiesTable->setNumRows(5+(int)params.size());

		databaseProp_->databasePropertiesTable->setText(0, 0, tr("Database Name:"));
		databaseProp_->databasePropertiesTable->setText(0, 1, dbName.c_str());

		databaseProp_->databasePropertiesTable->setText(1, 0, tr("Database Type:"));
		databaseProp_->databasePropertiesTable->setText(1, 1, database->dbmsName().c_str());

		databaseProp_->databasePropertiesTable->setText(2, 0, tr("Host:"));
		string host = database->host();
		if (host.empty())
			host = tr("localhost").latin1();
		databaseProp_->databasePropertiesTable->setText(2, 1, host.c_str());

		databaseProp_->databasePropertiesTable->setText(3, 0, tr("Port:"));
		string port;
		if (database->portNumber() != 0)
			port = Te2String(database->portNumber());
		databaseProp_->databasePropertiesTable->setText(3, 1, port.c_str());

		databaseProp_->databasePropertiesTable->setText(4, 0, tr("User:"));
		string user = database->user();
		databaseProp_->databasePropertiesTable->setText(4, 1, user.c_str());	
		m = 5;
	}

	int i;
	for(i=0; i<(m+(int)params.size()); ++i)
	{
		if(i >= m)
		{
			databaseProp_->databasePropertiesTable->setText(i, 0, params[i-m]);
			databaseProp_->databasePropertiesTable->setText(i, 1, vals[i-m]);
		}
		databaseProp_->databasePropertiesTable->setRowReadOnly(i, true);
	}

	databaseProp_->databasePropertiesTable->adjustColumn(0);
	databaseProp_->databasePropertiesTable->adjustColumn(1);

	databaseProp_->show();
	databaseProp_->raise();
}


void TerraViewBase::importDataAction_activated()
{
	ImportWindow *importWindow = new ImportWindow(this, "importWindow", true);
	if (importWindow->exec() == QDialog::Accepted)
	{
		TeLayer *layer = importWindow->getLayer();
		if (layer != 0)
		{
			QString msg = tr("The data was imported successfully!") + "\n";
			msg += tr("Do you want to display the data?");
			int response = QMessageBox::question(this, tr("Display the Data"),
				msg, tr("Yes"), tr("No"));
			if(response == 0)
			{
				// find a valid name for a view
				QString viewName = layer->name().c_str();
				bool flag = true;
				int count = 1;
				while (flag)
				{
					TeViewMap& viewMap = currentDatabase_->viewMap();
					TeViewMap::iterator it;
					for (it = viewMap.begin(); it != viewMap.end(); ++it)
					{
						if (it->second->name() == viewName.latin1())
							break;
					}
					if (it == viewMap.end())
						flag = false;

					else
					{
						char vnumber[5];
						sprintf(vnumber,"_%d",count);
						viewName += vnumber;
						count++;
					}
				}
				
				// create a projection to the view the parameters of the imported layer
				TeProjection *viewProjection = TeProjectionFactory::make(layer->projection()->params());

				// create a view and a theme on the main window for data visualization
				// add the new view to the database and insert it on the map of Views
				TeView *view = new TeView (viewName.latin1(), currentDatabase_->user());
				TeBox b;
				view->setCurrentBox(b);
				view->setCurrentTheme(-1);
				view->projection(viewProjection);
				if (!currentDatabase_->insertView(view))
				{
					QString msg = tr("The view") + " \"" + view->name().c_str() + "\" " + tr("could not be saved into the database!");
					QMessageBox::critical( this, tr("Error"), msg);
					delete importWindow;
					return;
				}
				if(currentDatabase_->projectMap().empty() == false)
				{
					TeProjectMap& pm = currentDatabase_->projectMap();
					TeProject* project = pm.begin()->second;
					project->addView(view->id());
				}
				currentDatabase_->insertProjectViewRel(1, view->id());

				checkWidgetEnabling();
				
				// create the view item and set it as the current one
				TeQtViewItem *viewItem = new TeQtViewItem(viewsListView_, viewName, view);

				// create the theme that will be associated to this view
				TeAttrTableVector ATV;
				layer->getAttrTables(ATV, TeAttrStatic);
				QString themeName = currentDatabase_->getNewThemeName(layer->name()).c_str();
				if(createTheme(themeName.latin1(), view, layer, "", ATV) == false)
				{
					QString msg = tr("The theme") + " \"" + themeName + "\" ";
					msg += tr("could not be inserted into the view") + " \"" + view->name().c_str() + "\" !";
					QMessageBox::critical( this, tr("Error"), msg);
					delete importWindow;
					return;
				}

				vector<TeQtThemeItem*> themeItemVec = viewItem->getThemeItemVec();
				TeAppTheme* newTheme = themeItemVec[0]->getAppTheme();

				grid_->clear();
				QListViewItem* item = (QListViewItem*)viewsListView_->getThemeItem(newTheme);
				viewsListView_->setOpen(viewItem, true);
				viewsListView_->setOn((QCheckListItem*)item, true);
				viewsListView_->selectItem(item);

				resetAction_activated();
			}
		}
	}
	delete importWindow;
}


bool TerraViewBase::createTheme(string themeName, TeView *view, TeLayer *layer, string whereClause, TeAttrTableVector& tablesVector)
{
	viewsListView_->selectViewItem(view);

	// check the view if there is already a theme with the given name
	unsigned int i;
	vector<TeViewNode*>& themeVector = view->themes();
	for (i = 0; i < themeVector.size(); ++i)
	{
		if (themeVector[i]->name() == themeName)
		{
			QMessageBox::warning(this, tr("Warning"),
			tr("There is already a theme with this name!"));
			return false;
		}
	}

	// Add a theme
	TeWaitCursor wait;
	QString msg;
	
	TeTheme* theme = new TeTheme(themeName, layer, 0, view->id());
	theme->visibleRep(layer->geomRep());
	view->add(theme);
	theme->visibility(true);
	if(whereClause.empty() == false)
		theme->attributeRest(whereClause);
	
	TeAppTheme* appTheme = new TeAppTheme(theme);
	
	if((!layer->hasGeometry(TeRASTER) && !layer->hasGeometry(TeRASTERFILE)) && tablesVector.size() > 0)
	{
		appTheme->canvasLegUpperLeft(layer->box().center());
		theme->attributeRest(whereClause);

		// Add the tables list to the theme tables list
		for (i = 0; i < tablesVector.size(); ++i)
			theme->addThemeTable(tablesVector[i]);

		// Insert the theme into the database (te_theme and te_theme_application)
		if(!insertAppTheme(currentDatabase_, appTheme))
		{
			QMessageBox::critical(this, tr("Error"),
			tr("Fail to insert the theme into the database!"));
			return false;
		}

		// Save the theme informations (save the theme tables, create the collection table
		// and the auxiliary table for the collection)
 		if(!theme->save())
		{
			msg = tr("Fail to save the theme!") + "\n";
			msg += currentDatabase_->errorMessage().c_str();
			QMessageBox::critical(this, tr("Error"), msg);
			return false;	
		}

		// Build the collection (populate it and the auxiliary table for the collection)
		if(!theme->buildCollection("", false))
		{
			msg = tr("Fail to build the collection table!") + "\n";
			msg += currentDatabase_->errorMessage().c_str();
			QMessageBox::critical(this, tr("Error"), msg);
			currentDatabase_->deleteTheme (theme->id());  // delete the theme
			return false;	
		}

		// Generate label_x and label_y in the collection table
		if(!theme->generateLabelPositions())
		{
			msg = tr("Fail to build the label positions of the objects in the collection table!") + "\n";
			msg += currentDatabase_->errorMessage().c_str();
			QMessageBox::warning(this, tr("Error"), msg);
			//return false;	
		}
		
		//Put the new theme on the interface and set it as the current theme
		TeQtThemeItem* themeItem = new TeQtThemeItem (viewsListView_->getViewItem(view), theme->name().c_str(),appTheme);
		themeItem->setOn(true);
	}
	else if (layer->hasGeometry(TeRASTER) || layer->hasGeometry(TeRASTERFILE))
	{
		insertAppTheme(currentDatabase_, appTheme);

		//Put the new theme on the interface and set it as the current theme
		TeQtThemeItem* themeItem = new TeQtThemeItem (viewsListView_->getViewItem(view), theme->name().c_str(),appTheme);
		themeItem->setOn(true);
	}

	TeBox tbox = currentDatabase_->getThemeBox(theme);
	theme->setThemeBox(tbox);
	currentDatabase_->updateTheme(theme);

	checkWidgetEnabling();

	// Make the view projection to inherit the projection of this theme's layer
	// if the theme is the first one 
	if(view->size() == 1)
	{
		int viewId = view->projection()->id();
		delete view->projection();
		int layerId = layer->projection()->id();
		TeProjection *proj = currentDatabase_->loadProjection(layerId);
		proj->id(viewId);
		view->projection(proj);
		currentDatabase_->updateProjection(proj);
	}

    std::string themeId = Te2String(theme->id()),
                sql = "UPDATE te_theme SET parent_id = " + themeId + " WHERE theme_id = " + themeId;

    if(!currentDatabase_->execute(sql))
    {
        return false;
    }

    return true;
}


void TerraViewBase::showToolBarAction_activated()
{
	if (toolBar->isVisible())
	{
		toolBar->hide();
		showToolBarAction->setIconSet((QPixmap)uncheck);
		showToolBarAction->setOn(false);
	}
	else
	{
		toolBar->show();
		showToolBarAction->setIconSet((QPixmap)check);
		showToolBarAction->setOn(true);
	}
}


void TerraViewBase::showStatusBarAction_activated()
{
	if (statusBar()->isVisible())
	{
		statusBar()->hide();
		showStatusBarAction->setIconSet((QPixmap)uncheck);
		showStatusBarAction->setOn(false);
	}
	else
	{
		statusBar()->show();
		showStatusBarAction->setIconSet((QPixmap)check);
		showStatusBarAction->setOn(true);
	}
}


void TerraViewBase::popupRenameDatabasesListViewItem()
{
	databasesListView_->popupItem()->startRename(0);
}

void TerraViewBase::popupThemeRemoveAllAlias()
{
	if(currentTheme_->getTheme()->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));
		return;
	}

	TeQtThemeItem* themeItem = (TeQtThemeItem*)viewsListView_->popupItem();
	TeAppTheme* theme = themeItem->getAppTheme();
	theme->removeAllAlias();
	themeItem->updateAlias();
}

void TerraViewBase::popupRemoveAliasViewsListViewItem()
{
	if(currentTheme_->getTheme()->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	string attr, nattr;
	TeQtCheckListItem *checkListItem = (TeQtCheckListItem*)viewsListView_->popupItem();
	TeQtThemeItem* themeItem = (TeQtThemeItem*) checkListItem->parent();
	TeAppTheme* appTheme = themeItem->getAppTheme();
	TeTheme* baseTheme = (TeTheme*)appTheme->getTheme();

	if (checkListItem->getType() == TeQtCheckListItem::LEGENDTITLE)
	{
		if(baseTheme->grouping().groupMode_ != TeNoGrouping)
		{
			attr = baseTheme->grouping().groupAttribute_.name_;
			nattr = baseTheme->grouping().groupNormAttribute_;
			appTheme->concatTableName(attr);
			appTheme->concatTableName(nattr);
		}
		if(nattr.empty() == false)
			attr += "/" + nattr;
	}
	else if (checkListItem->getType() == TeQtCheckListItem::CHART)
	{
		attr = ((TeQtChartItem*)checkListItem)->name();
		appTheme->concatTableName(attr);
	}

	appTheme->removeAlias(attr);
	themeItem->updateAlias();
}

void TerraViewBase::popupRenameViewsListViewItem()
{
	TeQtCheckListItem *item = (TeQtCheckListItem*)viewsListView_->popupItem();
	if(item->getType() == TeQtCheckListItem::LEGENDTITLE || item->getType() == TeQtCheckListItem::CHART)
	{
		TeQtThemeItem* themeItem = (TeQtThemeItem*)item->parent();
        if(themeItem->getAppTheme()->getTheme()->type() != TeTHEME)
		{
			QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
			return;
		}
	}

	viewsListView_->popupItem()->startRename(0);
}


void TerraViewBase::itemRenamed(QListViewItem* item, int, const QString& newName)
{
	static bool flag = false;
	string oldName;
	unsigned int	i;

	//Verify the name
	bool changed = false;
	std::string invalidChar = "";
	TeCheckName(newName.latin1(), changed, invalidChar);	
	//End

	if(newName.isEmpty() || changed)
	{
		if(changed)
		{
			QMessageBox::warning(this, tr("Warning"), tr("New Layer name invalid!"));
		}

		TeQtCheckListItem *checkListItem = (TeQtCheckListItem*)item;
		if (checkListItem->getType() == TeQtCheckListItem::INFOLAYER)
		{
			TeQtLayerItem *layerItem = (TeQtLayerItem*)item;
			TeLayer *layer = layerItem->getLayer();
			oldName = layer->name();
		}
		else if (checkListItem->getType() == TeQtCheckListItem::VIEW)
		{
			TeQtViewItem *viewItem = (TeQtViewItem*)item;
			TeView *view = viewItem->getView();
			oldName = view->name();
		}
		else if (checkListItem->getType() == TeQtCheckListItem::THEME)
		{
			TeQtThemeItem *themeItem = (TeQtThemeItem*)item;
			TeAppTheme *appTheme = themeItem->getAppTheme();
			oldName = appTheme->getTheme()->name();
		}
		else if (checkListItem->getType() == TeQtCheckListItem::LEGEND)
		{
			TeQtLegendItem *legendItem = (TeQtLegendItem*)item;
			oldName = legendItem->legendEntry()->label();
		}
		else if (checkListItem->getType() == TeQtCheckListItem::LEGENDTITLE)
		{
			TeQtThemeItem* themeItem = (TeQtThemeItem*) checkListItem->parent();
			TeAppTheme* appTheme = themeItem->getAppTheme();
			TeTheme* baseTheme = (TeTheme*)appTheme->getTheme();
			oldName = appTheme->getLegendTitleAlias();
			if(oldName.empty())
			{
				string attr, nattr; 
				if(baseTheme->grouping().groupMode_ != TeNoGrouping)
				{
					attr = baseTheme->grouping().groupAttribute_.name_;
					nattr = baseTheme->grouping().groupNormAttribute_;
					appTheme->concatTableName(attr);
					appTheme->concatTableName(nattr);		
				}		
				
				if(nattr.empty() == false)
					attr += "/" + nattr;
				oldName = attr;
			}
		}
		else if (checkListItem->getType() == TeQtCheckListItem::CHART)
		{
			TeQtThemeItem* themeItem = (TeQtThemeItem*) checkListItem->parent();
			TeAppTheme* appTheme = themeItem->getAppTheme();

			string attr = ((TeQtChartItem*)checkListItem)->name();
			appTheme->concatTableName(attr);
			oldName = appTheme->getPieBarAlias(attr);
			if(oldName.empty())
				oldName = attr;
		}
		if(flag)
		{
			((TeQtCheckListItem*)item)->cancelRename(0);
			item->setText(0, oldName.c_str());
			item->repaint();
		}
		else
			item->startRename(0);

		flag = !flag;
		return;
	}

	string name = newName.latin1();	

	TeQtCheckListItem *checkListItem = (TeQtCheckListItem*)item;
	if (checkListItem->getType() == TeQtCheckListItem::INFOLAYER)
	{
		TeQtLayerItem *layerItem = (TeQtLayerItem*)item;
		TeLayer *layer = layerItem->getLayer();
		TeLayerMap& layerMap = currentDatabase_->layerMap();
		TeLayerMap::iterator layerIt = layerMap.begin();
		while (layerIt != layerMap.end())
		{
			if(layerIt->first != layer->id() && layerIt->second->name() == name)
				break;
			layerIt++;
		}
		if(layerIt == layerMap.end())
		{
			layer->name(newName.latin1());
			layer->database()->updateLayer(layer);
			emit layerChanged(layer->id());
		}
		else
		{
			QMessageBox::warning(this, tr("Warning"),
			tr("There is already a layer with this name!") + "\n" + tr("Enter with another name."));
			item->setText(0, "");
			item->startRename(0);
			return;
		}
	}
	else if (checkListItem->getType() == TeQtCheckListItem::VIEW)
	{
		TeQtViewItem *viewItem = (TeQtViewItem*)item;
		TeView *view = viewItem->getView();
		TeViewMap& viewMap = currentDatabase_->viewMap();
		TeViewMap::iterator it;
		for (it = viewMap.begin(); it != viewMap.end(); ++it)
		{
			if (it->first != view->id() && it->second->name() == name)
				break;
		}
		if(it == viewMap.end())
		{
			view->name(name);
			currentDatabase()->updateView(view);
			emit viewChanged(view->id());
		}
		else
		{
			QMessageBox::warning(this, tr("Warning"),
			tr("There is already a view with this name!") + "\n" + tr("Enter with another name."));
			item->setText(0, "");
			item->startRename(0);
			return;
		}
	}
	else if (checkListItem->getType() == TeQtCheckListItem::THEME)
	{
		TeQtThemeItem *themeItem = (TeQtThemeItem*)item;
		TeAppTheme *appTheme = themeItem->getAppTheme();
		TeAbstractTheme* baseTheme = appTheme->getTheme();
		TeQtViewItem *viewItem = (TeQtViewItem*)item->parent();
		vector<TeQtThemeItem*> themeItemVec = viewItem->getThemeItemVec();
		for(i=0; i<themeItemVec.size(); i++)
		{
			TeAppTheme *appThemeItem = themeItemVec[i]->getAppTheme();
			if(baseTheme->name() != name && (appThemeItem->getTheme())->name() == name)
			{
				QMessageBox::warning(this, tr("Warning"),
				tr("There is already a theme with this name!") + "\n" + tr("Enter with another name."));
				item->setText(0, "");
				item->startRename(0);
				return;
			}
		}

		baseTheme->name(newName.latin1());
		currentDatabase_->updateTheme(baseTheme);
		emit themeChanged(baseTheme->id());
	}
	else if (checkListItem->getType() == TeQtCheckListItem::LEGEND)
	{
		TeQtLegendItem *legendItem = (TeQtLegendItem*)item;
		TeQtThemeItem *themeItem = (TeQtThemeItem*)item->parent();
		TeAppTheme *appTheme = themeItem->getAppTheme();
		TeTheme *baseTheme = (TeTheme*)appTheme->getTheme();

		unsigned int i;
		TeLegendEntryVector& legendEntryVector = baseTheme->legend();
		for (i = 0; i < legendEntryVector.size(); ++i)
		{
			if (legendEntryVector[i].label() == legendItem->legendEntry()->label())
			{
				string label = newName.latin1();
				legendEntryVector[i].label(label);		
				baseTheme->layer()->database()->updateTheme(baseTheme);
				break;
			}
		}
	}
	else if (checkListItem->getType() == TeQtCheckListItem::LEGENDTITLE)
	{
		string alias = newName.latin1();
		TeQtThemeItem* themeItem = (TeQtThemeItem*) checkListItem->parent();
		TeAppTheme* appTheme = themeItem->getAppTheme();
		TeTheme *baseTheme = (TeTheme*)appTheme->getTheme();

		string attr, nattr;
		if(baseTheme->grouping().groupMode_ != TeNoGrouping)
		{
			attr = baseTheme->grouping().groupAttribute_.name_;		
			nattr = baseTheme->grouping().groupNormAttribute_;	
			appTheme->concatTableName(attr);
			appTheme->concatTableName(nattr);
		}
		
		if(nattr.empty() == false)
			attr += "/" + nattr;
		if(appTheme->setAlias(attr, alias))
		{
			themeItem->updateAlias();
			baseTheme->layer()->database()->updateTheme(baseTheme);
		}
		else
		{
			QMessageBox::warning(this, tr("Warning"),
			tr("There is already an alias with this name!") + "\n" + tr("Enter with another alias."));
			item->setText(0, "");
			item->startRename(0);
			return;
		}
	}
	else if (checkListItem->getType() == TeQtCheckListItem::CHART)
	{
		string alias = newName.latin1();
		TeQtThemeItem* themeItem = (TeQtThemeItem*) checkListItem->parent();
		TeAppTheme* appTheme = themeItem->getAppTheme();
		TeTheme *baseTheme = (TeTheme*)appTheme->getTheme();

		string attr = ((TeQtChartItem*)checkListItem)->name();
		appTheme->concatTableName(attr);
		if(appTheme->setAlias(attr, alias))
		{
			themeItem->updateAlias();
			baseTheme->layer()->database()->updateTheme(baseTheme);
		}
		else
		{
			QMessageBox::warning(this, tr("Warning"),
			tr("There is already an alias with this name!") + "\n" + tr("Enter with another alias."));
			item->setText(0, "");
			item->startRename(0);
			return;
		}
	}
}


void TerraViewBase::popupRemoveDatabasesListViewItem()
{
	TeQtCheckListItem *checkItem = (TeQtCheckListItem*)databasesListView_->popupItem();
	removeItem(checkItem);
}


void TerraViewBase::popupRemoveViewsListViewItem()
{
	TeQtCheckListItem *checkItem = (TeQtCheckListItem*)viewsListView_->popupItem();
	removeItem(checkItem);
}


void TerraViewBase::popupExportThemesViewListViewItem()
{

    if(!currentDatabase_)
	{
		return;
	}

	TeQtCheckListItem *checkItem = (TeQtCheckListItem*)viewsListView_->popupItem();
	TeQtViewItem *viewItem = (TeQtViewItem*)checkItem;
	TeView *view = viewItem->getView();

	if (view->themes().empty())
	{
	    QMessageBox::critical(this, tr("Error"),tr("The view has no themes!"));
	    return;
	}

	ExportLayersThemes  clsELT(this, "Export Layers Themes", 0);

	clsELT.init(currentDatabase_, view, lastOpenDir_);

	clsELT.exec();

}

void TerraViewBase::removeItem(TeQtCheckListItem *checkItem)
{
	QString itemType;
	QString itemName = checkItem->text();

	if (checkItem->getType() == TeQtCheckListItem::INFOLAYER)
		itemType = tr("the infolayer");
	else if (checkItem->getType() == TeQtCheckListItem::VIEW)
		itemType = tr("the view");
	else if (checkItem->getType() == TeQtCheckListItem::THEME)
		itemType = tr("the theme");
	else if (checkItem->getType() == TeQtCheckListItem::LEGENDTITLE)
		itemType = tr("the legend");
	else if (checkItem->getType() == TeQtCheckListItem::CHARTTITLE)
		itemType = tr("the pie/chart");

	QString question = tr("Do you really want to remove") + " " + itemType + " \"" + itemName + "\" ?";

	int response = QMessageBox::question(this, tr("Remove Item"),
					question, tr("Yes"), tr("No"));

	if (response != 0)
		return;

	TeWaitCursor wait;
	if (checkItem->getType() == TeQtCheckListItem::INFOLAYER)
	{
		TeQtLayerItem *layerItem = (TeQtLayerItem*)checkItem;
		TeLayer *layer = layerItem->getLayer();
		TeDatabase* db = layer->database();
		if(db->tableExist("tv_generalinfo"))
		{
			string  del = "DELETE FROM tv_generalinfo WHERE layer_id = " + Te2String(layer->id());
			db->execute(del);
		}

		emit layerChanged(layer->id());

		//Remove the theme items associated to this popup layer
		vector<TeQtThemeItem*> themeItemVec = viewsListView_->getThemeItemVec();
		bool vis = false;
		for (unsigned int i = 0; i < themeItemVec.size(); ++i)
		{
			TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
			if (appTheme->getTheme()->type() >= 3)
				continue;

			TeTheme* theme = (TeTheme*)appTheme->getTheme();
			if (theme->layer() != layer)
				continue;

			TeQtViewItem* viewItem = viewsListView_->getViewItem(themeItemVec[i]);
			if (viewItem->getView() == currentView_)
				currentView_ = 0;

			if (appTheme == currentTheme_)
			{
				currentTheme_ = 0;
				grid_->clear();
			}

			if (theme->visibility() & 1)
				vis = true;

			viewsListView_->removeThemeItem(appTheme);
			TeAbstractTheme* absTheme = (TeAbstractTheme*)appTheme->getTheme();
			deleteAppTheme(db, absTheme);
		}
	
		deleteTextVisual(layer);
		db->deleteLayer(layer->id());
		databasesListView_->removeItem(layerItem);
		if (currentLayer_ == layer)
		{
			canvas_->clearAll();
			currentLayer_ = 0;
		}
		if(vis)
			displayWindow_->plotData();	
	}
	else if (checkItem->getType() == TeQtCheckListItem::VIEW)
	{
		TeQtViewItem *viewItem = (TeQtViewItem*)checkItem;
		TeView *view = viewItem->getView();
		emit viewChanged(view->id());

		viewsListView_->removeItem(viewItem);

		if (currentView_ == view)
		{
			grid_->clear();
			canvas_->clearAll();
			currentView_ = 0;
			currentTheme_ = 0;
		}
		if(currentDatabase_->projectMap().empty() == false)
		{
			TeProjectMap& pm = currentDatabase_->projectMap();
			TeProject* project = pm.begin()->second;
			project->deleteView(view->id());
		}
		else
			this->currentDatabase()->deleteView(view->id());
		checkWidgetEnabling();
	}
	else if (checkItem->getType() == TeQtCheckListItem::THEME)
	{
		TeQtThemeItem *themeItem = (TeQtThemeItem*)checkItem;
		TeAppTheme *appTheme = themeItem->getAppTheme();
//		TeTheme* baseTheme = (TeTheme*)appTheme->getTheme();
		TeAbstractTheme* baseTheme = (TeAbstractTheme*)appTheme->getTheme();
		emit themeChanged(baseTheme->id());

		TeDatabase *db = currentDatabase_;
		viewsListView_->removeItem(themeItem);

		if (currentTheme_ == appTheme)
		{
			grid_->clear();
			canvas_->clearAll();
			currentTheme_ = 0;
			if(currentView_->getCurrentTheme() == baseTheme->id())
			{
				currentView_->setCurrentTheme(-1);
				db->updateView(currentView_);
			}
		}
		deleteAppTheme(db, baseTheme);
	}
	else if (checkItem->getType() == TeQtCheckListItem::LEGENDTITLE)
	{
		TeQtThemeItem *themeItem = (TeQtThemeItem*)checkItem->parent();
		TeAppTheme *appTheme = themeItem->getAppTheme();
		TeTheme* baseTheme = (TeTheme*)appTheme->getTheme();

		baseTheme->deleteGrouping();
		themeItem->removeLegends();

		unsigned long rep = baseTheme->visibleRep();
		rep &= 0xbfffffff;
		baseTheme->visibleRep((TeGeomRep)rep);

		if(themeItem->isVisible() && themeItem->parent()->isSelected())
			displayWindow_->plotData();	
	}
	else if (checkItem->getType() == TeQtCheckListItem::CHARTTITLE)
	{
		TeQtThemeItem *themeItem = (TeQtThemeItem*)checkItem->parent();
		TeAppTheme *appTheme = themeItem->getAppTheme();
		TeTheme* baseTheme = (TeTheme*)appTheme->getTheme();
		themeItem->removeCharts();

		string del = "DELETE FROM te_chart_params WHERE theme_id = " + Te2String(baseTheme->id());
		currentDatabase_->execute(del);

		appTheme->chartAttributes_.clear();
		appTheme->chartColors_.clear();		

		unsigned long rep = baseTheme->visibleRep();
		rep &= 0x7fffffff;
		baseTheme->visibleRep((TeGeomRep)rep);

		if(themeItem->isVisible() && themeItem->parent()->isSelected())
			displayWindow_->plotData();	
	}

	checkWidgetEnabling();
}


void TerraViewBase::popupLayerLoadMemory()
{
	TeWaitCursor wait;

	TeQtLayerItem *layerItem = (TeQtLayerItem*)databasesListView_->popupItem();
	TeLayer* layer = layerItem->getLayer();
	mountLayerRepMap(layer);
}


void TerraViewBase::popupLayerClearMemory()
{
	TeQtLayerItem *layerItem = (TeQtLayerItem*)databasesListView_->popupItem();
	TeLayer* layer = layerItem->getLayer();
	layerId2RepMap_.erase(layer->id());
}

void TerraViewBase::layerProperties()
{	
	TeQtLayerItem *layerItem = (TeQtLayerItem*)databasesListView_->popupItem();
	layerProperties(layerItem);
}

void TerraViewBase::layerProperties(TeQtLayerItem* layerItem)
{
    if (layerProp_)
		delete layerProp_;

	layerProp_ = new LayerProperties(this, "Layer Properties",true);
	layerProp_->init(layerItem);
	layerProp_->exec();
}

void TerraViewBase::viewProperties()
{
	TeQtViewItem *viewItem = (TeQtViewItem*)viewsListView_->popupItem();
    viewProperties(viewItem);
}

void TerraViewBase::viewProperties(TeQtViewItem* viewItem)
{
    unsigned int i;
	TeView *view = viewItem->getView();

	if (viewProp_ == 0)
	{
		viewProp_ = new ViewProperties();
		viewProp_->viewPropertiesTable->horizontalHeader()->setLabel(0, tr("Parameter"));
		viewProp_->viewPropertiesTable->horizontalHeader()->setLabel(1, tr("Value"));
	}

	vector<string> param;
	vector<string> val;

	param.push_back(tr("View Name:").latin1());
	val.push_back(viewItem->text(0).latin1());
	param.push_back(tr("Projection:").latin1());
	val.push_back(view->projection()->name());
	
	viewProp_->viewPropertiesTable->setNumRows(param.size());
	for(i = 0; i < param.size(); ++i)
	{
		viewProp_->viewPropertiesTable->setText(i, 0, param[i].c_str());
		viewProp_->viewPropertiesTable->setText(i, 1, val[i].c_str());
	}
	viewProp_->viewPropertiesTable->adjustColumn(0);
	viewProp_->viewPropertiesTable->adjustColumn(1);

	viewProp_->show();
	viewProp_->raise();
}


void TerraViewBase::themeProperties()
{
	TeQtThemeItem *themeItem = (TeQtThemeItem*)viewsListView_->popupItem();
    themeProperties(themeItem);
}

void TerraViewBase::themeProperties(TeQtThemeItem* themeItem)
{
    unsigned int i;
    TeAppTheme *appTheme = themeItem->getAppTheme();
	TeAbstractTheme* baseTheme = appTheme->getTheme();

	TeTheme* theme = 0;
	if(baseTheme->type() == TeTHEME)
		theme = (TeTheme*)(baseTheme);
	else if(baseTheme->type() == TeEXTERNALTHEME)
		theme = ((TeExternalTheme*)(baseTheme))->getRemoteTheme();

	if (themeProp_ == 0)
		themeProp_ = new ThemeProperties();

	themeProp_->setCaption(tr("Theme Properties"));
	themeProp_->themePropertiesTable->setNumCols(2);
	themeProp_->themePropertiesTable->horizontalHeader()->setLabel(0, tr("Parameter"));
	themeProp_->themePropertiesTable->horizontalHeader()->setLabel(1, tr("Value"));

	vector<QString> param;
	vector<QString> val;
	string s;

	param.push_back(tr("Theme Name:"));
	val.push_back(themeItem->text(0));
	TeLayer* layer = 0;
	TeDatabase* db = 0;
	if (theme)
	{
		layer = theme->layer();
		param.push_back(tr("Infolayer:"));
		val.push_back(layer->name().c_str());

		param.push_back(tr("Creation Time:"));
		val.push_back(theme->getCreationTime().getDateTime().c_str());

		if(theme->layer() != NULL)
		{
			param.push_back(tr("Layer Has Been Updated:"));
            //if(theme->layer()->getEditionTime() <= theme->getCreationTime())
			if(theme->isUpdated())
			{
				val.push_back(tr("false"));
			}
			else
			{
				val.push_back(tr("true"));
			}
		}

		db = layer->database();
		TeDatabasePortal* portal = db->getPortal();
		int rep = layer->geomRep();
		if((rep & TePOLYGONS) || (rep & TeLINES) || (rep & TePOINTS))
		{
			param.push_back(tr("Number of polygons:"));
			if(layer->hasGeometry(TePOLYGONS))
			{
				string count = "SELECT COUNT(*) FROM " + layer->tableName(TePOLYGONS);
				count += ", " + theme->collectionTable() + " WHERE ";
				count += layer->tableName(TePOLYGONS) + ".object_id = ";
				count += theme->collectionTable() + ".c_object_id";
				portal->freeResult();
				if(portal->query(count))
				{
					if(portal->fetchRow())
					{
						count = portal->getData(0);			
						val.push_back(count.c_str());
					}
				}
				else
				{
					val.push_back("0");
				}
			}
			else
				val.push_back("0");
		
			param.push_back(tr("Number of Lines:"));
			if(layer->hasGeometry(TeLINES))
			{
				string count = "SELECT COUNT(*) FROM " + layer->tableName(TeLINES);
				count += ", " + theme->collectionTable() + " WHERE ";
				count += layer->tableName(TeLINES) + ".object_id = ";
				count += theme->collectionTable() + ".c_object_id";
				portal->freeResult();
				if(portal->query(count))
				{
					if(portal->fetchRow())
					{
						count = portal->getData(0);			
						val.push_back(count.c_str());
					}
				}
				else
				{
					val.push_back("0");
				}
			}
			else
				val.push_back("0");
		
			param.push_back(tr("Number of Points:"));
			if(layer->hasGeometry(TePOINTS))
			{
				string count = "SELECT COUNT(*) FROM " + layer->tableName(TePOINTS);
				count += ", " + theme->collectionTable() + " WHERE ";
				count += layer->tableName(TePOINTS) + ".object_id = ";
				count += theme->collectionTable() + ".c_object_id";
				portal->freeResult();
				if(portal->query(count))
				{
					if(portal->fetchRow())
					{
						count = portal->getData(0);			
						val.push_back(count.c_str());
					}
				}
				else
				{
					val.push_back("0");
				}
			}
			else
				val.push_back("0");
		}
		if(rep&TeRASTER)
		{
		}
		delete portal;

		param.push_back(tr("Collection:"));
		if(theme->attributeRest().empty())
			val.push_back(tr("Without Restriction"));
		else
			val.push_back(theme->attributeRest().c_str());
	}
	else
	{
		if(baseTheme->type() == TeFILETHEME)
		{
			param.push_back(tr("File name"));
			val.push_back(static_cast<TeFileTheme*>(baseTheme)->getFileName().c_str());
		}
	}

	char fmt[100];
	if (appTheme->getTheme()->getThemeProjection() != 0)
	{
		if (appTheme->getTheme()->getThemeProjection()->name() == "NoProjection" ||
			appTheme->getTheme()->getThemeProjection()->name() == "LatLong")
				strcpy(fmt,"%.10f");
		else
			strcpy(fmt,"%.3f");
	}
	else
		strcpy(fmt,"%.10f");

	QString myValue;
	param.push_back(tr("Initial Coordinate(X)"));
	myValue.sprintf(fmt,baseTheme->getThemeBox().x1_);
	val.push_back(myValue);
    param.push_back(tr("Initial Coordinate(Y)"));
	myValue.sprintf(fmt,baseTheme->getThemeBox().y1_);
	val.push_back(myValue);
	param.push_back(tr("Final Coordinate(X)"));
	myValue.sprintf(fmt,baseTheme->getThemeBox().x2_);
	val.push_back(myValue);
	param.push_back(tr("Final Coordinate(Y)"));
	myValue.sprintf(fmt,baseTheme->getThemeBox().y2_);
	val.push_back(myValue);
	if (appTheme->getTheme()->getThemeProjection())
	{
		param.push_back(tr("Projection"));
		string proj = appTheme->getTheme()->getThemeProjection()->name(); 
		proj += " / "+ appTheme->getTheme()->getThemeProjection()->datum().name();
		val.push_back(proj.c_str());
	}

	//external theme
	if(baseTheme->getProductId() == TeEXTERNALTHEME)
	{
		TeExternalTheme* extTheme = (TeExternalTheme*)baseTheme;

		int remoteThemeId = 0;
		int remoteDBId = 0;
		if(extTheme->getRemoteThemeInfo(remoteThemeId, remoteDBId))
		{
			TeDatabase* remotedb = TeDBConnectionsPool::instance().getDatabase(remoteDBId);
			if(remotedb != NULL)
			{
				TeDatabase* extConn = remotedb;
				param.push_back(tr("Database type:"));
				val.push_back(QString(extConn->dbmsName().c_str()));
				param.push_back(tr("Database name:"));
				val.push_back(QString(extConn->databaseName().c_str()));
				param.push_back(tr("Host name:"));
				val.push_back(QString(extConn->host().c_str()));
				param.push_back(tr("Port:"));
				val.push_back(QString(Te2String(extConn->portNumber()).c_str()));
				param.push_back(tr("User:"));
				val.push_back(QString(extConn->user().c_str()));
			}
		}

		if(currentDatabase_ != NULL)
		{
			TeThemeMap::iterator itTheme = currentDatabase_->invalidThemeMap().find(extTheme->id());
			bool isInvalid = (itTheme != currentDatabase_->invalidThemeMap().end());
	        
			param.push_back(tr("Theme Status:"));
			if(isInvalid == true)
			{
				val.push_back(tr("Invalid"));
			}
			else 
			{
				val.push_back(tr("Valid"));

				if(extTheme->getRemoteTheme() != NULL)
				{
					param.push_back(tr("Remote Name:"));
					val.push_back(extTheme->getRemoteTheme()->name().c_str());
				}
			}
		}
	}

	themeProp_->themePropertiesTable->setNumRows(param.size());
	for(i = 0; i < param.size(); ++i)
	{
		themeProp_->themePropertiesTable->setText(i, 0, param[i]);
		themeProp_->themePropertiesTable->setText(i, 1, val[i]);
	}
	themeProp_->themePropertiesTable->adjustColumn(0);
	themeProp_->themePropertiesTable->adjustColumn(1);
	themeProp_->show();
	themeProp_->raise();
}

void TerraViewBase::showObjectInformation(string objId)
{
	unsigned int i;
	string CT, from;

	TeTheme* theme = (TeTheme*)currentTheme_->getTheme();

	if(theme->type() == TeTHEME)
	{
		CT = theme->collectionTable();
		from = theme->sqlGridFrom();
	}
	else if(theme->type() == TeEXTERNALTHEME)
	{
		CT = ((TeExternalTheme*)theme)->getRemoteTheme()->collectionTable();
		from = ((TeExternalTheme*)theme)->getRemoteTheme()->sqlGridFrom();
	}
	else
		return;
		
	TeDatabase* db = theme->layer()->database();
	TeDatabasePortal* portal = db->getPortal();

	string sel = "SELECT * " + from + " WHERE " + CT + ".c_object_id = '" + objId + "'";

	int offset = 9;
	TeAttributeList atl;
	vector<vector<string> > svec;
	if(portal->query(sel))
	{
		atl = portal->getAttributeList();
		while(portal->fetchRow())
		{
			vector<string> v;
			for(i=offset; i<atl.size(); ++i)
			{
				string s = portal->getData(i);
				v.push_back(s);
			}
			svec.push_back(v);
		}
	}
	delete portal;

	vector<string>::iterator it;

	if (themeProp_ == 0)
		themeProp_ = new ThemeProperties();

	themeProp_->setCaption(tr("Object Information"));

	int j, n, m;
	if(svec.size() > 1)
	{
		n = (int)svec[0].size();
		themeProp_->themePropertiesTable->setNumCols(n);
		for(i=0; i<(unsigned int)n; ++i)
			themeProp_->themePropertiesTable->horizontalHeader()->setLabel(i, atl[i+offset].rep_.name_.c_str());

		m = (int)svec.size();
		themeProp_->themePropertiesTable->setNumRows(m);
		for(i=0; i<(unsigned int)m; ++i)
		{
			for(j=0; j<n; ++j)
			{
				string s = svec[i][j].c_str();
				themeProp_->themePropertiesTable->setText(i, j, s.c_str());
			}
		}
		for(j=0; j<n; ++j)
			themeProp_->themePropertiesTable->adjustColumn(j);
	}
	else
	{
		themeProp_->themePropertiesTable->setNumCols(2);
		themeProp_->themePropertiesTable->horizontalHeader()->setLabel(0, tr("Parameter"));
		themeProp_->themePropertiesTable->horizontalHeader()->setLabel(1, tr("Value"));
		m = (int)svec[0].size();
		themeProp_->themePropertiesTable->setNumRows(m);
		for(i=0; i<(unsigned int)m; ++i)
		{
			string s = atl[i+offset].rep_.name_;
			themeProp_->themePropertiesTable->setText(i, 0, s.c_str());
			s = svec[0][i];
			themeProp_->themePropertiesTable->setText(i, 1, s.c_str());
		}
		themeProp_->themePropertiesTable->adjustColumn(0);
		themeProp_->themePropertiesTable->adjustColumn(1);
	}

	themeProp_->show();
	themeProp_->raise();
}

void TerraViewBase::drawAction_activated()
{
	TeWaitCursor wait;
	TeBox box;

	if(currentView_ == 0)
		return;

	vector<TeViewNode*>& themeVector = currentView_->themes();
	if (themeVector.size() == 0)
		return;

	if(currentView_->getCurrentBox().isValid() == false)
	{
		resetAction_activated();
		return;
	}
	else
	{
		canvas_->setWorld(currentView_->getCurrentBox());
		box = canvas_->getWorld();
	}
	displayWindow_->plotData();

	if(curBoxIndex_ >= 0)
	{
		TeBox bbox = boxesVector_[curBoxIndex_];
		if(bbox.x1_ == box.x1_ && bbox.y1_ == box.y1_ &&
			(bbox.width() == box.width() || bbox.height() == box.height()))
			return;
	}

	++curBoxIndex_;
	boxesVector_.push_back(box);
	checkWidgetEnabling();
}


void TerraViewBase::previousDisplayAction_activated()
{
	TeWaitCursor wait;
	TeBox box;

	--curBoxIndex_;

	if (curBoxIndex_ < 0)
	{
		curBoxIndex_ = 0;
		return;
	}
	
	box = boxesVector_[curBoxIndex_];
	canvas_->setWorld(box, cvContentsW_, cvContentsH_);

	checkWidgetEnabling();
	displayWindow_->plotData();
}


void TerraViewBase::nextDisplayAction_activated()
{
	TeWaitCursor wait;
	TeBox box;

	++curBoxIndex_;

	if (curBoxIndex_ == (int)boxesVector_.size())
	{
		--curBoxIndex_;
		return;
	}

	box = boxesVector_[curBoxIndex_];
	canvas_->setWorld(box, cvContentsW_, cvContentsH_);

	checkWidgetEnabling();
	displayWindow_->plotData();
}


void TerraViewBase::resetAction_activated()
{
	TeWaitCursor wait;
	TeBox box;
	unsigned int i;
	if (currentView_ == 0)
		return;
	TeProjection *viewProjection = currentView_->projection();
	vector<TeQtThemeItem*> themeItemVec = viewsListView_->currentViewItem()->getThemeItemVec();

	curBoxIndex_ = 0;

	for (i = 0 ; i < themeItemVec.size(); ++i)
	{
		TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
		TeAbstractTheme* baseTheme = appTheme->getTheme();
		if (baseTheme->visibility() && baseTheme->visibleRep())
		{
			TeBox themeBox;
			themeBox = baseTheme->getThemeBox();
			if(themeBox.isValid() == false)
				return;
			if(baseTheme->visibleGeoRep() & TePOINTS)
				zoomIn(themeBox, 1.1);
			TeProjection *themeProjection = baseTheme->getThemeProjection();
			themeBox = TeRemapBox(themeBox, themeProjection, viewProjection);
			updateBox (box, themeBox);
		}
	}
	canvas_->setWorld(box, cvContentsW_, cvContentsH_);
	currentView_->setCurrentBox(box);

	if(boxesVector_.size())
		boxesVector_[0] = box;
	else
		boxesVector_.push_back(box);
	canvas_->clearRaster();
	checkWidgetEnabling();
	displayWindow_->setContrastPixmap(0);
	displayWindow_->plotData();
}



void TerraViewBase::tileWindowsAction_activated()
{
    ws_->tile();
}


void TerraViewBase::databasesTreeAction_toggled(bool isVisible)
{
	if (isVisible)
		databasesDock_->show();
	else
		databasesDock_->hide();
	
	databasesTreeAction->setOn(isVisible);
}


void TerraViewBase::viewsTreeAction_toggled(bool isVisible)
{
	if (isVisible)
		viewsDock_->show();
	else
		viewsDock_->hide();
	
	viewsTreeAction->setOn(isVisible);
}

void TerraViewBase::gridAction_toggled(bool isVisible)
{
	if (isVisible)
		gridDock_->show();
	else
		gridDock_->hide();
	
	gridAction->setOn(isVisible);
}


void TerraViewBase::displayAction_toggled(bool isVisible)
{
	if (isVisible)
	{
		displayWindow_->show();
		if (graphic_->isVisible())
			tileWindowsAction_activated();
		else
			displayWindow_->showMaximized();
	}
	else
	{
		displayWindow_->hide();
		if (graphic_->isVisible())
			graphic_->showMaximized();
	}

	displayAction->setOn(isVisible);
}



TeQtDatabasesListView* TerraViewBase::getDatabasesListView()
{
	 return databasesListView_;
}


TeQtViewsListView* TerraViewBase::getViewsListView()
{
	return viewsListView_;
}



void TerraViewBase::itemChanged(QListViewItem *item)
{
	TeWaitCursor wait;
	bool themeItemChanged = false;

	TeQtCheckListItem *checkListItem = (TeQtCheckListItem*)item;

	if (checkListItem->getType() == TeQtCheckListItem::DATABASE)
	{
		TeQtDatabaseItem *dbItem = (TeQtDatabaseItem*)item;
		bool draw = false;
		bool otherDB = true;
		if(!currentDatabase_ || currentDatabase_==dbItem->getDatabase())
			otherDB = false;

		if(currentDatabase_)
		{
			draw = true;
			emit(dbChanged(currentDatabase_->databaseName().c_str()));
		}
        
		currentDatabase_ = dbItem->getDatabase();
		currentTheme_ = 0;
		currentLayer_ = 0;
		currentView_ = 0;
		
		grid_->clear();
		viewsListView_->reset();
				
		if(!currentDatabase_)
			return;

		//verify if it must load all database metadata
		if(otherDB)
		{
            //TeDBConnectionsPool::instance().clear();
            TeDBConnectionsPool::instance().loadExternalDBConnections(currentDatabase_);

            std::vector<int> dbIds = TeDBConnectionsPool::instance().getConnectionsIdVector();

			if(dbIds.size()>0)
			{
				currentDatabase_->clear();
				// Get layers from the database
				if (!currentDatabase_->loadLayerSet())
				{
					wait.resetWaitCursor();
					QString msg = tr("Fail to get the layers from the database!") + "\n";
					msg += currentDatabase_->errorMessage().c_str();
					QMessageBox::critical(this, tr("Error"), msg);
					return;		
				}

				// Insert layers into the tree
				string dbName = TeGetBaseName (currentDatabase_->databaseName().c_str());
				databasesListView_->removeItem((QListViewItem*)dbItem);
				dbItem = new TeQtDatabaseItem(databasesListView_, dbName.c_str(), currentDatabase_);

				TeLayerMap& layerMap = currentDatabase_->layerMap();
				TeLayerMap::iterator itlay = layerMap.begin();
				TeQtLayerItem *layerItem;
				while ( itlay != layerMap.end() )
				{
					TeLayer *layer = (*itlay).second;
					string name = layer->name();
					layerItem = new TeQtLayerItem(dbItem, name.c_str(), layer);
					if (databasesListView_->isOpen(dbItem) == false)
						databasesListView_->setOpen(dbItem,true);
					itlay++;
				}

				getDatabasesListView()->selectItem(dbItem);

				if (!currentDatabase_->loadViewSet(currentDatabase_->user()))
				{
					wait.resetWaitCursor();
					QString msg = tr("Fail to get the views from the database!") + "\n";
					msg += currentDatabase_->errorMessage().c_str();
					QMessageBox::critical(this, tr("Error"), msg);
					return;
				}

				if(!currentDatabase_->loadProjectSet())
				{
					QMessageBox::critical(this, tr("Error"), tr("Fail to load project!")); 
					return;
				}
			}
		}
		
		loadViewsTree(currentDatabase_);
		
		if (currentDatabase_->themeMap().size() < 20)
			viewsListView_->openViewItems();
		resetBoxesVector();
		canvas_->setWorld(TeBox(), cvContentsW_, cvContentsH_);
		if(graphicParams_)
			graphicParams_->hide();
		if(graphic_)
			graphic_->hide();
		if(graphic3DWindow_)
			graphic3DWindow_->hide();
		if (query_)
			query_->hide();
		if(addressLocatorWindow_)
			addressLocatorWindow_->hide();
		
		if(currentDatabase_->projectMap().empty() == false)
		{
			TeProject* project = currentDatabase_->projectMap().begin()->second;
			int currentViewId = project->getCurrentViewId();
			if(currentViewId > 0)
			{
				TeViewMap& vMap = currentDatabase_->viewMap();
				currentView_ = vMap[currentViewId];
				if(currentView_)
				{
					if(currentView_->getCurrentBox().isValid())
						canvas_->setWorld(currentView_->getCurrentBox());

					TeQtViewItem* viewItem = (TeQtViewItem*)viewsListView_->getViewItem(currentView_);
					viewsListView_->setOpen((QCheckListItem*)viewItem, true);
					viewsListView_->setOn((QCheckListItem*)viewItem, true);
					viewsListView_->selectItem((QListViewItem*)viewItem);

					int currentThemeId = currentView_->getCurrentTheme();
					if(currentThemeId > 0)
					{
						vector<TeQtThemeItem*> themeItemVec = viewItem->getThemeItemVec();
						if (themeItemVec.size() != 0)
						{
							unsigned int i;
							for (i = 0; i < themeItemVec.size(); ++i)
							{
								TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
								if (((TeTheme*)appTheme->getTheme())->id() == currentThemeId)
								{
									currentTheme_ = appTheme;
									break;
								}
							}
						}
						if(currentTheme_)
						{
							TeQtThemeItem* themeItem = (TeQtThemeItem*)viewsListView_->getThemeItem(currentTheme_);
							viewsListView_->setOpen((QCheckListItem*)themeItem, true);
							viewsListView_->setOn((QCheckListItem*)themeItem, true);
							viewsListView_->selectItem((QListViewItem*)themeItem);

							if(currentTheme_->getTheme()->type() == TeTHEME)
							{
								TeTheme *theme = static_cast<TeTheme*> (currentTheme_->getTheme());
								currentLayer_ = theme->layer();
								if(currentLayer_)
									grid_->init(currentTheme_);
							}
						}
					}
				}
			}
			if(draw)
				drawAction_activated();
		}
	}
	else if (checkListItem->getType() == TeQtCheckListItem::INFOLAYER)
	{
		if(currentLayer_)
			emit(layerChanged(currentLayer_->id()));
		TeQtLayerItem *layerItem = (TeQtLayerItem*)item;
		currentLayer_ = layerItem->getLayer();
		currentDatabase_ = currentLayer_->database();
	}
	else if (checkListItem->getType() == TeQtCheckListItem::VIEW)
	{
		enableEditAction_toggled(false);
		int oldViewId = -1;
		if(currentView_)
			oldViewId = currentView_->id();

		TeQtViewItem *viewItem = (TeQtViewItem*)item;
		if (viewItem)
			currentView_ = viewItem->getView();
		else
			currentView_ = 0;
		currentTheme_ = 0;
		grid_->clear();
		canvas_->clearAll();
		canvas_->setProjection(currentView_->projection());
		resetBoxesVector();
		displayWindow_->resetEditions();
		if(graphicParams_)
			graphicParams_->hide();
		if(graphic_)
			graphic_->hide();
		if(graphic3DWindow_)
			graphic3DWindow_->hide();
		if (query_)
			query_->hide();
		if(addressLocatorWindow_)
			addressLocatorWindow_->hide();
		if (!displayWindow_->getGUScaleSetting())
		{
			if (currentView_->projection()->units() == "DecimalDegrees")
				displayWindow_->setScaleGraphicUnit(0);
			else
				displayWindow_->setScaleGraphicUnit(2);
		}

		if(currentDatabase_)
		{
			if(currentDatabase_->projectMap().empty() == false)
			{
				TeProject* project = currentDatabase_->projectMap().begin()->second;
				if(currentView_)
					project->setCurrentViewId(currentView_->id());
				else
					project->setCurrentViewId(-1);
			}

			if(currentView_)
			{
				currentTheme_ = 0;

				if(currentView_->getCurrentTheme() > 0)
				{
					int i;
					vector<TeViewNode*>& vt = currentView_->themes();
					for(i=0; i<(int)vt.size(); ++i)
					{
						TeViewNode* vn = vt[i];
						if(vn->id() == currentView_->getCurrentTheme())
						{
							TeThemeMap& tm = currentDatabase_->themeMap();
							if(currentView_->getCurrentTheme() > 0)
							{
								TeAbstractTheme* t = tm[currentView_->getCurrentTheme()];
								if(t != NULL)
								{
									int tId = t->id();
									TeQtThemeItem* ti = viewsListView_->getThemeItem(tId);
									viewsListView_->selectItem(ti);
								}
								else
								{
									int tId = currentView_->getCurrentTheme();
									TeQtThemeItem* ti = viewsListView_->getThemeItem(tId);
									viewsListView_->selectItem(ti);
								}
							}
							break;
						}
					}
					if(i == (int)vt.size())
					{
						currentView_->setCurrentTheme(0);
						currentDatabase_->updateView(currentView_);
					}
				}

				if(currentView_->getCurrentBox().isValid())
					canvas_->setWorld(currentView_->getCurrentBox());
			}
		}
		emit(viewChanged(oldViewId));
	}
	else if (checkListItem->getType() == TeQtCheckListItem::THEME)
	{	
		TeQtThemeItem *themeItem = (TeQtThemeItem*)item;
		if(currentTheme_ != themeItem->getAppTheme())
		{
			themeItemChanged = true;
			if(currentTheme_)
				emit(themeChanged(((TeTheme*)currentTheme_->getTheme())->id()));

			if(graphicParams_)
				graphicParams_->hide();

			if(graphic_)
				graphic_->hide();

			if(graphic3DWindow_)
				graphic3DWindow_->hide();

			if (query_)
				query_->hide();

			if(addressLocatorWindow_)
				addressLocatorWindow_->hide();

			enableEditAction_toggled(false);
		}

		if (themeItem)
		{
			currentTheme_ = themeItem->getAppTheme();
			if(currentTheme_->getTheme()->type() == TeTHEME)
			{
				TeTheme* baseTheme = (TeTheme*)currentTheme_->getTheme();
				databasesListView_->selectLayerItem(baseTheme->layer());
				if (baseTheme->layer() != currentLayer_)
					currentLayer_ = baseTheme->layer();

				if (currentLayer_->hasGeometry(TeRASTER) ||  currentLayer_->hasGeometry(TeRASTERFILE))
					gridAction_toggled(false);
				else
					gridAction_toggled(true);
			}
			else 
			{
				databasesListView_->selectLayerItem((TeLayer*)0);
				vector<TeQtDatabaseItem*>  dbItemVec = databasesListView_->getDatabaseItemVec();
				vector<TeQtDatabaseItem*>::iterator it;
				for(it=dbItemVec.begin(); it!=dbItemVec.end(); ++it)
				{
					if(currentDatabase_ == (*it)->getDatabase())
					{
						databasesListView_->setSelected((QListViewItem*)*it, true);
						break;
					}
				}

				if(currentTheme_->getTheme()->type() == TeFILETHEME) 
					gridAction_toggled(false);
				else
					gridAction_toggled(true);
			}

			TeQtViewItem *viewItem = viewsListView_->currentViewItem();
			currentView_ = viewItem->getView();
		}
		else
			currentTheme_ = 0;

		if(currentDatabase_ && currentView_)
		{
			if(currentTheme_)
			{
				currentView_->setCurrentTheme(currentTheme_->getTheme()->id());
				viewsListView_->selectItem(themeItem);
			}
			else
				currentView_->setCurrentTheme(-1);
		}

		showTooltip_ = "HIDE";
		columnTooltip_ = -1;
		displayWindow_->resetEditions();
	}

	checkWidgetEnabling();

	if(currentDatabase_ && themeItemChanged)
	{
		if(currentDatabase_->projectMap().empty() == false)
		{
			TeProject* project = currentDatabase_->projectMap().begin()->second;
			if(currentView_)
				project->setCurrentViewId(currentView_->id());
			else
				project->setCurrentViewId(-1);

			currentDatabase_->updateProject(project);
		}

//		if(currentView_)
//			currentDatabase_->updateView(currentView_);
	}

	if (checkListItem->getType() != TeQtCheckListItem::THEME)
	{
	     wait.resetWaitCursor();
		return;
	    }

	canvas_->setMode(TeQtCanvas::Pointer);
	canvas_->viewport()->setCursor(QCursor(ArrowCursor));
	pointerCursorAction->setOn(true);

	if (currentTheme_ == 0)
	{
		wait.resetWaitCursor();
	    grid_->clear();
		return;
	}
	grid_->init(currentTheme_);
	wait.resetWaitCursor();
}


void TerraViewBase::loadViewsTree(TeDatabase *database)
{
	TeView *view;
	TeQtViewItem *viewItem;
	TeQtThemeItem *themeItem;
	unsigned int i;

	// Get Views from the database
	TeViewMap& viewMap = database->viewMap();

	// Fill views and tree view
	viewsListView_->clear();

	TeViewMap::iterator viewIt;
	for (viewIt = viewMap.begin(); viewIt != viewMap.end(); ++viewIt)
	{
		view = viewIt->second;
		viewItem = new TeQtViewItem(viewsListView_, view->name().c_str(), view);

		TeTheme* theme;
		TeAppTheme* appTheme;
		vector<TeViewNode*>& themeVector = view->themes();
		for (i = 0; i < themeVector.size(); ++i)
		{
			TeViewNode* viewNode = themeVector[i];
			if (viewNode->type() == 1)		//means TeTREE=GRUPO no vipe
				continue;
			theme = (TeTheme*)themeVector[i];
			appTheme = new TeAppTheme(theme);
			loadAppTheme(database, appTheme); 
			appTheme->editRep((TeGeomRep)0);

			themeItem = new TeQtThemeItem (viewItem, theme->name().c_str(), appTheme);

			if(theme->type() > 3)
			{
				PluginsSignal sigseticon(PluginsSignal::S_SET_QTTHEMEICON);
				sigseticon.signalData_.store(PluginsSignal::D_QT_THEMEITEM, themeItem);
				sigseticon.signalData_.store(PluginsSignal::D_THEMETYPE, themeItem->getAppTheme()->getTheme()->type());
				PluginsEmitter::instance().send(sigseticon);
			}			
			
			viewsListView_->setOn(themeItem, theme->visibility());
			updateThemeItemInterface(themeItem);
		}

		TeThemeMap tMap = database->invalidThemeMap();
		TeThemeMap::iterator it;
		for(it=tMap.begin(); it!=tMap.end(); ++it)
		{
			theme = (TeTheme*)it->second;
			if(theme->view() == view->id())
			{
				appTheme = new TeAppTheme(theme);
				loadAppTheme(database, appTheme); 
				appTheme->editRep((TeGeomRep)0);

				themeItem = new TeQtThemeItem (viewItem, theme->name().c_str(), appTheme);
				themeItem->setThemeItemAsInvalid();
				themeItem->setOn(false);
				theme->visibility(0);
				updateThemeItemInterface(themeItem);
			}
		}
	}
	canvas_->clearAll();
	cursorActionGroup_selected(pointerCursorAction);
}

void TerraViewBase::updateThemeItemInterface(TeQtThemeItem* themeItem)
{
	if (!themeItem)
		return;

	themeItem->removeLegends();
	themeItem->removeCharts();

	unsigned int j;
	TeAppTheme* appTheme = themeItem->getAppTheme();
	TeTheme* baseTheme = (TeTheme*)appTheme->getTheme();
	TeDatabase* database = appTheme->getLocalDatabase();
	TeLegendEntryVector& legendVector = baseTheme->legend();
	if (legendVector.size() > 0)
	{
		//insert the grouping attribute name
		TeAttributeRep rep; 
		string nattr;
		if(baseTheme->grouping().groupMode_ != TeNoGrouping)
		{
			rep = baseTheme->grouping().groupAttribute_;
			nattr = baseTheme->grouping().groupNormAttribute_;
		}
		string text = rep.name_;
		map<int, map<string, string> >::iterator it = database->mapThemeAlias().find(baseTheme->id());
		if(it != database->mapThemeAlias().end())
		{
			map<string, string>& m = it->second;
			map<string, string>::iterator tit = m.find(text);
			if(tit != m.end())
				text = tit->second;
			if(!(nattr.empty() || nattr == "NONE"))
			{
				tit = m.find(nattr);
				if(tit != m.end())
					text += "/" + tit->second;
				else
					text += "/" + nattr;
			}
		}
		new TeQtLegendTitleItem(themeItem, text.c_str());				

		//insert the legend items
		for (j = 0; j < legendVector.size(); ++j)
		{
			TeLegendEntry& legendEntry = legendVector[j];
			string txt = legendEntry.label();
			new TeQtLegendItem(themeItem, txt.c_str(), &legendEntry);
		}
	}

	if(appTheme->chartAttributes_.size())
	{
		themeItem->removeCharts();
		TeChartType chartType = (TeChartType)appTheme->chartType();
		if(chartType == TeBarChart)
			new TeQtChartTitleItem (themeItem, tr("Bar Chart"));
		else
			new TeQtChartTitleItem (themeItem, tr("Pie Chart"));

		for(j = 0; j < appTheme->chartAttributes_.size(); ++j)
		{
			string label = appTheme->chartAttributes_[j];
			TeColor color = appTheme->chartColors_[j];
			new TeQtChartItem(themeItem, label.c_str(), color);
			themeItem->updateAlias();
		}
	}
}

TeDatabase* TerraViewBase::currentDatabase()
{
	return currentDatabase_;
}

TeLayer* TerraViewBase::currentLayer()
{
	return currentLayer_;
}


TeView* TerraViewBase::currentView()
{
	return currentView_;
}


TeAppTheme* TerraViewBase::currentTheme()
{
	return currentTheme_;
}


void TerraViewBase::setCurrentAppTheme(TeAppTheme * appTheme)
{
	TeQtThemeItem* themeItem = viewsListView_->getThemeItem(appTheme);
	viewsListView_->selectItem(themeItem);
	currentTheme_ = appTheme;
}


TeQtGrid* TerraViewBase::getGrid()
{
	return grid_;
}


void TerraViewBase::attributesQuery()
{
    if(currentTheme() == NULL)
    {
        return;
    }

	TeTheme* theme = (TeTheme*)currentTheme()->getTheme();

	if((theme->type() != TeTHEME) && (theme->type() != TeEXTERNALTHEME))
	{
		QMessageBox::information(this, tr("Information"), 
            tr("This operation is not allowed for this current theme type!"));	
	}

    if (query_ == 0)
    {
		query_ = new QueryWindow(this, "query", false);
    }

    query_->setTheme(currentTheme_);
	query_->show();
}


void TerraViewBase::spatialQuery()
{
    SpatialQueryWindow* spatialQuery = new SpatialQueryWindow(this, "spatialquery");
    bgAndBoxChanged_ = TeBox();
	spatialQuery->Init();
}

void TerraViewBase::popupPromoteSlot(int i)
{
	if (i == 0)
		grid_->promotePointedObjectsSlot();
	else
		grid_->promoteQueriedObjectsSlot();
}


void TerraViewBase::popupGridColumnsSlot(int i)
{
	vector<int> cols;
	if(i == 7 || i == 8)
	{
		int n;
		for(n=0; n<grid_->numCols(); ++n)
		{
			int col = grid_->getColumn(n);
			if(col == gridColumn_)
			{
				cols.push_back(n);
				break;
			}
		}
		if(cols.size() == 0)
			return;
	}

	switch(i)
	{
		case 0:
			grid_->adjustColumns();
			break;
		case 1:
			grid_->sortAscOrderSlot();
			break;
		case 2:
			grid_->sortDescOrderSlot();
			break;
		case 3:
			grid_->swapColumns();
			break;
		case 4:
			grid_->setVisColumn(false);
			break;
		case 5:
			grid_->setVisColumn(true);
			break;
		case 6:
			deleteSelectedColumns();
			break;
		case 7:
			grid_->sort(cols, "ASC");
			break;
		case 8:
			grid_->sort(cols, "DESC");
			break;
		default:
			break;
	}
}


void TerraViewBase::checkWidgetEnabling()
{
	if (currentDatabase_ == 0)
	{
		//fileMenu
		databasePropertiesAction->setEnabled(false);
		closeDatabaseAction->setEnabled(false);
		dropDatabaseAction->setEnabled(false);
		importDataAction->setEnabled(false);
		importDataAction->setIconSet((QPixmap)importDisabled);
		importRasterAction->setEnabled(false);
		importRasterSimpleAction->setEnabled(false);
		importTableAction->setEnabled(false);
		importTablePointsAction->setEnabled(false);
		vectorialExportAction->setEnabled(false);
		infolayerMultiLayer_Vectorial_ExportAction->setEnabled(false);
		addressGeocodingAction->setEnabled(false);
		fileMenu->setItemEnabled(fileMenu->idAt(15), false); //saveDisplayAction
		fileMenu->setItemEnabled(fileMenu->idAt(16), false); //saveGraphicAction

		//infolayerMenu
		createCellsAction->setEnabled(false);
		
		//viewMenu
		addViewAction->setEnabled(false);			
		addViewAction->setIconSet((QPixmap)viewDisabled);
		
		//themeMenu
		addThemeAction->setEnabled(false);			
		addThemeAction->setIconSet((QPixmap)themeDisabled);

		//analysisMenu
		analysisMenu->setItemEnabled(analysisMenu->idAt(0), false); //proximity matrix
		createProxMatrixAction->setEnabled(false);
		selectProxMatrixAction->setEnabled(false);
		spatialStatisticsAction->setEnabled(false);			
		kernelMapAction->setEnabled(false);
		kernelRatioAction->setEnabled(false);
		semivarAction->setEnabled(false);
		skaterAction->setEnabled(false);
		globalEmpiricalBayesAction->setEnabled(false);
		localEmpiricalBayesAction->setEnabled(false);
		generateSamplePointsAction->setEnabled(false);
		
		//operationMenu
		pointerCursorAction->setEnabled(false);
		zoomCursorAction->setEnabled(false);
		zoomInAction->setEnabled(false);			
		zoomOutAction->setEnabled(false);			
		scaleComboBox->setEnabled(false);			
		scaleComboBox->setCurrentText("");			
		panCursorAction->setEnabled(false);
		graphicCursorAction->setEnabled(false);
		enableEditAction->setEnabled(false);
		distanceMeterAction->setEnabled(false);
		infoCursorAction->setEnabled(false);
		drawAction->setEnabled(false);			
		previousDisplayAction->setEnabled(false);			
		nextDisplayAction->setEnabled(false);			
		resetAction->setEnabled(false);			
		unselectObjectsAction->setEnabled(false);
		invertSelectionAction->setEnabled(false);
	}
	else
	{
		//fileMenu
		databasePropertiesAction->setEnabled(true);
		closeDatabaseAction->setEnabled(true);
		std::string dbmsName;
		dbmsName=currentDatabase_->dbmsName();
		if( (dbmsName == "PostGIS") || (dbmsName == "PostgreSQL") || (dbmsName == "SqlServerAdo")  \
			|| (dbmsName == "SqlServerAdoSpatial") || (dbmsName == "MySQL") )	dropDatabaseAction->setEnabled(true);
		else																	dropDatabaseAction->setEnabled(false);	

		importDataAction->setEnabled(true);
		importDataAction->setIconSet((QPixmap)import);
		importRasterSimpleAction->setEnabled(true);
		importRasterAction->setEnabled(true);
		importTableAction->setEnabled(true);
		importTablePointsAction->setEnabled(true);
		vectorialExportAction->setEnabled(true);
		infolayerMultiLayer_Vectorial_ExportAction->setEnabled(true);
		addressGeocodingAction->setEnabled(true);
		fileMenu->setItemEnabled(fileMenu->idAt(15), true); //saveDisplayAction
		fileMenu->setItemEnabled(fileMenu->idAt(16), true); //saveGraphicAction

		//infolayerMenu
		if (currentDatabase_->layerMap().size() != 0)
			createCellsAction->setEnabled(true);
		else
			createCellsAction->setEnabled(false);

		//viewMenu
		addViewAction->setEnabled(true);
		addViewAction->setIconSet((QPixmap)view_xpm);

		if (currentView_ != 0)
		{		
			analysisMenu->setItemEnabled(analysisMenu->idAt(0), true); //proximity matrix
		    createProxMatrixAction->setEnabled(true);	
			selectProxMatrixAction->setEnabled(true);	
			addViewAction->setEnabled(true);
			addViewAction->setIconSet((QPixmap)view_xpm);
		}

		//themeMenu
		if (currentDatabase_->layerMap().size() != 0 && currentDatabase_->viewMap().size() != 0)
		{
			addThemeAction->setEnabled(true);
			addThemeAction->setIconSet((QPixmap)theme_xpm);
		}
		else
		{
			addThemeAction->setEnabled(false);
			addThemeAction->setIconSet((QPixmap)themeDisabled);
		}

		//analysisMenu
		if (currentTheme_ != 0)
		{
			spatialStatisticsAction->setEnabled(true);
			TeGeomRep rep = static_cast<TeGeomRep>(currentTheme_->getTheme()->visibleGeoRep() & ~TeTEXT); // TeTEXT not is considered

			if(rep == TePOINTS)
				semivarAction->setEnabled(true);

			skaterAction->setEnabled(true);
			globalEmpiricalBayesAction->setEnabled(true);
			localEmpiricalBayesAction->setEnabled(true);
			generateSamplePointsAction->setEnabled(true);
		}
		
		//operationMenu
		if (currentView_ != 0)
		{
			analysisMenu->setItemEnabled(analysisMenu->idAt(0), true); //proximity matrix
		    createProxMatrixAction->setEnabled(true);	
			selectProxMatrixAction->setEnabled(true);	
		    kernelMapAction->setEnabled(true);
			kernelRatioAction->setEnabled(true);			
			vector<TeQtThemeItem*> themeItemVec = viewsListView_->currentViewItem()->getThemeItemVec();
			if (themeItemVec.size() != 0)
			{
				unsigned int i;
				for (i = 0; i < themeItemVec.size(); ++i)
				{
					TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
					if (appTheme->getTheme()->visibility())
					{
						drawAction->setEnabled(true);
						break;
					}
				}
				if (i >= themeItemVec.size())
					drawAction->setEnabled(false);
			}

			if (drawAction->isEnabled())
			{
				if (curBoxIndex_ < 0)
				{
					pointerCursorAction->setEnabled(false);
					zoomCursorAction->setEnabled(false);
					zoomInAction->setEnabled(false);
					zoomOutAction->setEnabled(false);
					scaleComboBox->setEnabled(false);			
					scaleComboBox->setCurrentText("");			
					panCursorAction->setEnabled(false);
					graphicCursorAction->setEnabled(false);
					enableEditAction->setEnabled(false);
					distanceMeterAction->setEnabled(false);
					infoCursorAction->setEnabled(false);
					previousDisplayAction->setEnabled(false);
					nextDisplayAction->setEnabled(false);
					resetAction->setEnabled(false);
					invertSelectionAction->setEnabled(false);
					unselectObjectsAction->setEnabled(false);
				}
				else if (curBoxIndex_ >= 0)
				{
					pointerCursorAction->setEnabled(true);
					zoomCursorAction->setEnabled(true);
					zoomInAction->setEnabled(true);
					zoomOutAction->setEnabled(true);
					panCursorAction->setEnabled(true);
					graphicCursorAction->setEnabled(true);
					
					if(currentTheme_ != 0)
					{
						TeAbstractTheme* theme = currentTheme_->getTheme();
						bool enable = !((theme->type() != TeTHEME) && (theme->type() != TeEXTERNALTHEME));

						enableEditAction->setEnabled(enable);
					}

					distanceMeterAction->setEnabled(true);
					infoCursorAction->setEnabled(true);
					resetAction->setEnabled(true);
					if(currentTheme_ && ((currentTheme_->getTheme()->visibleGeoRep() & TeRASTER) ||
					   (currentTheme_->getTheme()->visibleGeoRep() & TeRASTERFILE)))
					{
						invertSelectionAction->setEnabled(false);
						unselectObjectsAction->setEnabled(false);
					}
					else
					{
						invertSelectionAction->setEnabled(true);
						unselectObjectsAction->setEnabled(true);
					}

					if (curBoxIndex_ == 0)
						previousDisplayAction->setEnabled(false);
					else 
						previousDisplayAction->setEnabled(true);

					if (curBoxIndex_ >= (int)(boxesVector_.size() - 1))
						nextDisplayAction->setEnabled(false);
					else
						nextDisplayAction->setEnabled(true);
				}
			}
			else
			{
				pointerCursorAction->setEnabled(false);
				zoomCursorAction->setEnabled(false);
				zoomInAction->setEnabled(false);
				zoomOutAction->setEnabled(false);
				scaleComboBox->setEnabled(false);			
				scaleComboBox->setCurrentText("");			
				panCursorAction->setEnabled(false);
				graphicCursorAction->setEnabled(false);
				enableEditAction->setEnabled(false);
				distanceMeterAction->setEnabled(false);
				infoCursorAction->setEnabled(false);
				resetAction->setEnabled(false);
				previousDisplayAction->setEnabled(false);
				nextDisplayAction->setEnabled(false);
				invertSelectionAction->setEnabled(false);
				unselectObjectsAction->setEnabled(false);
			}
		}
	}
}

void TerraViewBase::popupRemovePointingColor()
{
	TeQtThemeItem *themeItem = (TeQtThemeItem*)viewsListView_->popupItem();
	TeAppTheme* appTheme = themeItem->getAppTheme();
	removePointingColor((TeTheme*)appTheme->getTheme());
}

void TerraViewBase::removePointingColor()
{
	if (currentTheme_->getTheme()->type() >= 3)
		return;
	removePointingColor((TeTheme*)currentTheme_->getTheme());
}

void TerraViewBase::removePointingColor(TeTheme* theme)
{
	if(!(theme->type() == TeTHEME || theme->type() == TeEXTERNALTHEME))
		return;

	if(theme == (TeTheme*)currentTheme_->getTheme())
		grid_->removePointingColorSlot();
	else
	{
		TeDatabase* db;
		if(theme->getProductId() == TeTHEME)
			db = theme->layer()->database();
		else if(theme->getProductId() == TeEXTERNALTHEME)
			db = ((TeExternalTheme*)theme)->getSourceDatabase();

		string s = "UPDATE " + theme->collectionTable();
		s += " SET c_object_status = 0 WHERE c_object_status = 1";
		db->execute(s);

		s = "UPDATE " + theme->collectionTable();
		s += " SET c_object_status = 2 WHERE c_object_status = 3";
		db->execute(s);

		s = "UPDATE " + theme->collectionAuxTable();
		s += " SET grid_status = 0 WHERE grid_status = 1";
		db->execute(s);

		s = "UPDATE " + theme->collectionAuxTable();
		s += " SET grid_status = 2 WHERE grid_status = 3";
		db->execute(s);

		if(theme->visibility())
			displayWindow_->plotData();

		theme->removePointingColor();
	}
}


void TerraViewBase::popupRemoveQueryColor()
{
	TeQtThemeItem *themeItem = (TeQtThemeItem*)viewsListView_->popupItem();
	TeAppTheme *theme = themeItem->getAppTheme();
	removeQueryColor(theme);
}

void TerraViewBase::removeQueryColor()
{
	removeQueryColor(currentTheme_);
}

void TerraViewBase::removeQueryColor(TeAppTheme* appTheme)
{
	TeTheme* baseTheme = (TeTheme*)appTheme->getTheme();

	if(!(baseTheme->type() == TeTHEME || baseTheme->type() == TeEXTERNALTHEME))
		return;

	appTheme->refineAttributeWhere("");
	appTheme->refineSpatialWhere("");
	appTheme->refineTemporalWhere("");

	if(appTheme == currentTheme_)
	{
		grid_->removeQueryColorSlot();
		queryChanged();
	}
	else
	{
		TeDatabase* db = appTheme->getLocalDatabase();

		string s = "UPDATE " + baseTheme->collectionTable();
		s += " SET c_object_status = 0 WHERE c_object_status = 2";
		db->execute(s);

		s = "UPDATE " + baseTheme->collectionTable();
		s += " SET c_object_status = 1 WHERE c_object_status = 3";
		db->execute(s);

		s = "UPDATE " + baseTheme->collectionAuxTable();
		s += " SET grid_status = 0 WHERE grid_status = 2";
		db->execute(s);

		s = "UPDATE " + baseTheme->collectionAuxTable();
		s += " SET grid_status = 1 WHERE grid_status = 3";
		db->execute(s);

		if(baseTheme->visibility())
			displayWindow_->plotData();

		baseTheme->removeQueryColor();
	}
}


void TerraViewBase::popupScrollGridSlot(int i)
{
	switch (i)
	{
		case 0:
			grid_->scrollToNextPointedObjectSlot();
			break;
		case 1:
			grid_->scrollToPrevPointedObjectSlot();
			break;
		case 2:
			grid_->scrollToNextQueriedObjectSlot();
			break;
		case 3:
			grid_->scrollToPrevQueriedObjectSlot();
			break;
		default:
			break;
	}
}


void TerraViewBase::popupEditInVerticalDirection()
{
	if(currentTheme_->getTheme()->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	QIconSet iconCheck((QPixmap)check);
	QIconSet iconUncheck((QPixmap)uncheck);

	if (grid_->isVerticalEdition() == false)
	{
		popupTable_->setItemChecked(popupTable_->idAt(3), true); // vertical
		popupTable_->changeItem(popupTable_->idAt(3), iconCheck, tr("Edit in the &Vertical Direction"));
		grid_->setVerticalEdition(true);
	}
	else
	{
		popupTable_->setItemChecked(popupTable_->idAt(3), false); // horizontal
		popupTable_->changeItem(popupTable_->idAt(3), iconUncheck, tr("Edit in the &Vertical Direction"));
		grid_->setVerticalEdition(false);
	}
}



TeQtCanvas* TerraViewBase::getCanvas()
{
    return canvas_;
}


void TerraViewBase::addViewAction_activated()
{
	AddView *addView = new AddView(this, "addView", true);
	addView->exec();
	delete addView;
}



void TerraViewBase::popupViewProjection()
{
	TeQtViewItem *viewItem = (TeQtViewItem*)viewsListView_->popupItem();
    viewProjection(viewItem);
}

void TerraViewBase::viewProjection(TeQtViewItem* viewItem)
{
    TeProjection *viewProjection = viewItem->getView()->projection();

	ProjectionWindow *projectionWindow = new ProjectionWindow(this, "projection", true);	
	projectionWindow->loadProjection(viewProjection);

	if (projectionWindow->exec() == QDialog::Accepted)
	{
		TeProjection *projection = projectionWindow->getProjection();
		if (projection == 0)
		{
			QMessageBox::critical(this, tr("Error"),
			tr("Fail to set the new projection for the view!"));
			return;
		}
		projection->id(viewProjection->id());

		TeProjection *proj = currentView_->projection();
		if(*proj == *projection)
		{
			delete projectionWindow;
			return;
		}
		currentView_->projection(projection);
		canvas_->setProjection(projection);
		currentDatabase_->updateProjection(projection);
		if (projection->units() == "DecimalDegrees")
			displayWindow_->setScaleGraphicUnit(0);
		else
			displayWindow_->setScaleGraphicUnit(2);
		unsigned int i;
		for(i = 0; i < boxesVector_.size(); i++)
			boxesVector_[i] = TeRemapBox(boxesVector_[i], proj, projection);

		delete viewProjection;
		scaleComboBox_activated(scaleComboBox->currentText());
//		drawAction_activated();
	}
	delete projectionWindow;
}


void TerraViewBase::addThemeAction_activated()
{
	AddTheme *addTheme = new AddTheme(this, "addTheme", true);
	if (addTheme->exec() == QDialog::Accepted)
	{
		if(createTheme(addTheme->themeName(), addTheme->themeView(), addTheme->themeLayer(),
			addTheme->whereClause(), addTheme->themeTablesVector()) == false)
		{
			QString msg = tr("The theme") + " \"" + addTheme->themeName().c_str();
			msg += "\" " + tr("could not be inserted into the view") + " \"" + addTheme->themeView()->name().c_str() + "\" !";
			QMessageBox::critical( this, tr("Error"), msg);
		}
	}
	delete addTheme;
}


void TerraViewBase::popupDefaultVisual()
{
	TeQtThemeItem *themeItem = (TeQtThemeItem*)viewsListView_->popupItem();
	setDefaultVisual(themeItem);
}


void TerraViewBase::setDefaultVisual(TeQtThemeItem *themeItem)
{
	TeAppTheme *appTheme = themeItem->getAppTheme();
	TeAbstractTheme* theme = appTheme->getTheme();
	TeLegendEntry& defaultLegend = theme->defaultLegend();

	VisualWindow *visualWindow = new VisualWindow(this, "visualWindow",true);
	visualWindow->setCaption(tr("Default Visual"));


	// saves the visual defined
	TeVisual polygonVisual, lineVisual, pointVisual, textVisual;
	TeGeomRepVisualMap repVisualMap = defaultLegend.getVisualMap();
	TeGeomRepVisualMap::iterator it;
	for (it = repVisualMap.begin(); it != repVisualMap.end(); ++it)
	{
		TeGeomRep rep = it->first;
		if (rep == TePOLYGONS || rep == TeCELLS)
			polygonVisual = *(defaultLegend.visual(rep));
		else if (rep == TeLINES)
			lineVisual = *(defaultLegend.visual(rep));
		else if (rep == TePOINTS)
			pointVisual = *(defaultLegend.visual(rep));
		else if (rep == TeTEXT)
			textVisual = *(defaultLegend.visual(rep));
	}

	visualWindow->loadLegend(defaultLegend, theme);
	visualWindow->exec();
	delete visualWindow;

	// check if there is any modification
	vector<TeGeomRep> aux;
	for (it = repVisualMap.begin(); it != repVisualMap.end(); ++it)
	{
		TeGeomRep rep = it->first;
		if (rep == TePOLYGONS || rep == TeCELLS)
		{
			if(!(polygonVisual == *(defaultLegend.visual(rep))))
				aux.push_back(rep);
		}
		else if (rep == TeLINES)
		{
			if(!(lineVisual == *(defaultLegend.visual(rep))))
				aux.push_back(rep);
		}
		else if (rep == TePOINTS)
		{
			if(!(pointVisual == *(defaultLegend.visual(rep))))
				aux.push_back(rep);
		}
		else if (rep == TeTEXT)
		{
			if(!(textVisual == *(defaultLegend.visual(rep))))
			{
				if(editionIsActivated())
					cursorActionGroup_selected(pointerCursorAction);
				aux.push_back(rep);
			}
		}
	}
	if(aux.empty())
		return;

	// saves the modified visual default 
	currentDatabase_->updateLegend(&defaultLegend); 

	// passes the modification of visual default to pointing visual
	// passes the modification of visual default to query visual
	// passes the modification of visual default to query and point visual


	TeLegendEntry& pointingLegend = theme->pointingLegend();
	TeLegendEntry& queryLegend = theme->queryLegend();
	TeLegendEntry& pointingQueryLegend = theme->queryAndPointingLegend();

	TeColor color;

	for (unsigned int a=0; a<aux.size(); ++a)
	{
		TeVisual defaultLegVisual = *(defaultLegend.visual(aux[a]));
		
		TeVisual* pointingVisual = pointingLegend.visual(aux[a]);
		color = pointingVisual->color();
		defaultLegVisual.color(color);
		pointingLegend.setVisual(defaultLegVisual.copy(), aux[a]);
		
		TeVisual* queryVisual = queryLegend.visual(aux[a]);
		color = queryVisual->color();
		defaultLegVisual.color(color);
		queryLegend.setVisual(defaultLegVisual.copy(), aux[a]);
		
		TeVisual* pointingQueryVisual = pointingQueryLegend.visual(aux[a]);
		color = pointingQueryVisual->color();
		defaultLegVisual.color(color);
		pointingQueryLegend.setVisual(defaultLegVisual.copy(), aux[a]);
	}
	currentDatabase_->updateLegend(&pointingLegend); 
	currentDatabase_->updateLegend(&queryLegend); 
	currentDatabase_->updateLegend(&pointingQueryLegend); 
	
	canvas_->clearAll();
	displayWindow_->plotData();
}


void TerraViewBase::popupPointingVisual()
{
	TeQtThemeItem *themeItem = (TeQtThemeItem*)viewsListView_->popupItem();
	setPointingVisual(themeItem);
}


void TerraViewBase::setPointingVisual(TeQtThemeItem *themeItem)
{
	TeAppTheme *appTheme = themeItem->getAppTheme();
	TeTheme* theme = (TeTheme*)appTheme->getTheme();
	TeLegendEntry& pointingLegend = theme->pointingLegend();
	bool isOK = false;
	TeColor color;

	color = pointingLegend.visual(TePOLYGONS)->color();
	QColor inputColor = QColor(color.red_, color.green_, color.blue_);

	QColor	outputColor = QColorDialog::getRgba (inputColor.rgb(), &isOK, this );
	if (isOK)
	{
		TeColor newColor = TeColor(outputColor.red(), outputColor.green(), outputColor.blue());
		if(color == newColor)
			return;

		// Set the new color of the pointing visual
		TeGeomRepVisualMap& repVisualMap = pointingLegend.getVisualMap();
		TeGeomRepVisualMap::iterator it;
		for (it = repVisualMap.begin(); it != repVisualMap.end(); ++it)
		{
			TeGeomRep rep = it->first;
			TeVisual* pointingVisual = pointingLegend.visual(rep);
			pointingVisual->color(newColor);
		}

		currentDatabase_->updateLegend(&pointingLegend);
		canvas_->clearAll();
		displayWindow_->plotData();
	}
}


void TerraViewBase::popupQueryVisual()
{
	TeQtThemeItem *themeItem = (TeQtThemeItem*)viewsListView_->popupItem();
	setQueryVisual(themeItem);
}


void TerraViewBase::setQueryVisual(TeQtThemeItem *themeItem)
{
	TeAppTheme *appTheme = themeItem->getAppTheme();
	TeTheme* theme = (TeTheme*)appTheme->getTheme();
	TeLegendEntry& queryLegend = theme->queryLegend();
	bool isOK = false;
	TeColor color;

	color = queryLegend.visual(TePOLYGONS)->color();
	QColor inputColor = QColor(color.red_, color.green_, color.blue_);

	QColor	outputColor = QColorDialog::getRgba (inputColor.rgb(), &isOK, this );
	if (isOK)
	{
		TeColor newColor = TeColor(outputColor.red(), outputColor.green(), outputColor.blue());
		if(color == newColor)
			return;

		// Set the new color of the pointing visual
		TeGeomRepVisualMap& repVisualMap = queryLegend.getVisualMap();
		TeGeomRepVisualMap::iterator it;
		for (it = repVisualMap.begin(); it != repVisualMap.end(); ++it)
		{
			TeGeomRep rep = it->first;
			TeVisual* queryVisual = queryLegend.visual(rep);
			queryVisual->color(newColor);
		}

		currentDatabase_->updateLegend(&queryLegend);
		canvas_->clearAll();
		displayWindow_->plotData();
	}
}


void TerraViewBase::popupPointingQueryVisual()
{
	TeQtThemeItem *themeItem = (TeQtThemeItem*)viewsListView_->popupItem();
	setPointingQueryVisual(themeItem);
}


void TerraViewBase::setPointingQueryVisual(TeQtThemeItem *themeItem)
{
	TeAppTheme *appTheme = themeItem->getAppTheme();
	TeTheme* theme = (TeTheme*)appTheme->getTheme();
	TeLegendEntry& pointingQueryLegend = theme->queryAndPointingLegend();
	bool isOK = false;
	TeColor color;

	color = pointingQueryLegend.visual(TePOLYGONS)->color();
	QColor inputColor = QColor(color.red_, color.green_, color.blue_);

	QColor	outputColor = QColorDialog::getRgba (inputColor.rgb(), &isOK, this );
	if (isOK)
	{
		TeColor newColor = TeColor(outputColor.red(), outputColor.green(), outputColor.blue());
		if(color == newColor)
			return;

		// Set the new color of the pointing visual
		TeGeomRepVisualMap& repVisualMap = pointingQueryLegend.getVisualMap();
		TeGeomRepVisualMap::iterator it;
		for (it = repVisualMap.begin(); it != repVisualMap.end(); ++it)
		{
			TeGeomRep rep = it->first;
			TeVisual* pointingQueryVisual = pointingQueryLegend.visual(rep);
			pointingQueryVisual->color(newColor);
		}

		currentDatabase_->updateLegend(&pointingQueryLegend);
		canvas_->clearAll();
		displayWindow_->plotData();
	}
}


void TerraViewBase::textVisual()
{
	VisualWindow *visualWindow = new VisualWindow(this, "visualWindow",true);
	visualWindow->setCaption(tr("Text Visual"));
	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	TeLegendEntry& defaultLegend = curTheme->defaultLegend();

	TeVisual* textVisual = defaultLegend.visual(TeTEXT);

	TeLegendEntry auxLegend;
	auxLegend.setVisual(textVisual->copy(), TeTEXT); 
	//auxLegend.getVisualMap()[TeTEXT] = textVisual;

	visualWindow->loadLegend(auxLegend, 0);
	visualWindow->exec();
	delete visualWindow;

	if(*textVisual == *(auxLegend.visual(TeTEXT)))
		return;
	if(editionIsActivated())
		cursorActionGroup_selected(pointerCursorAction);

	defaultLegend.getVisualMap()[TeTEXT] = auxLegend.visual(TeTEXT)->copy();

	currentDatabase_->updateLegend(&defaultLegend);
//	currentDatabase_->loadLegend(currentTheme_);
	if(curTheme->layer()->hasGeometry(TeTEXT) && (curTheme->visibleRep() & TeTEXT))
	{
		canvas_->clearAll();
		displayWindow_->plotData();
	}
}


void TerraViewBase::checkListItemVisChanged(QCheckListItem *checkListItem, bool visible)
{
	TeQtThemeItem *themeItem = (TeQtThemeItem*)checkListItem;
	if (themeItem->isThemeItemInvalid() == true)
	{
		TeAbstractTheme* absTheme = (TeAbstractTheme*)themeItem->getAppTheme()->getTheme();
		absTheme->visibility(0);
		return;
	}

	TeAppTheme *appTheme = themeItem->getAppTheme();
	TeTheme* theme = (TeTheme*)appTheme->getTheme();
	theme->visibility(visible);
	checkWidgetEnabling();

	//check if the theme item whose visibility was changed belongs to the current view item;
	//in positive case reset the boxes vector and the current box index.
//	if (viewsListView_->currentViewItem()->isChild(themeItem) == true)
//		resetBoxesVector();

	string q = "UPDATE te_theme SET enable_visibility = " + Te2String(theme->visibility());
	q += " WHERE theme_id = " + Te2String(theme->id());
	if (currentDatabase_->execute(q) == false)
	{
		QMessageBox::warning(this, tr("Warning"),
		tr("The visibility of this theme could not be updated in the database!"));
		return;
	}
}

void TerraViewBase::updateThemesPriority(TeQtThemeItem *pressedThemeItem, TeQtViewItem *pressedViewItem, TeQtViewItem *dropViewItem)
{
	//if null parameters, the theme name already it exist on destination view
	if(pressedThemeItem == 0 && pressedViewItem == 0 && dropViewItem == 0)
	{
		QString msg = tr("The theme could not be dragged because the theme name") + "\n";
		msg += tr("already exists in the destination view!");
		msg += "\n" + tr("Rename the theme before dragging it!");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

	unsigned int i;
	string updateStr;

	TeView *pressedView = pressedViewItem->getView();
	int pressedViewCurrentTheme = pressedView->getCurrentTheme();
	TeView *dropView = dropViewItem->getView();

	emit viewChanged(pressedView->id());
	if (pressedViewItem != dropViewItem)
		emit viewChanged(dropView->id());

	TeQtThemeItem *themeItem;
	TeTheme *theme;

	if (pressedViewItem != dropViewItem)
	{		
		//update the theme priorities
		TeAppTheme* appTheme = pressedThemeItem->getAppTheme();
		theme = (TeTheme*)appTheme->getTheme();
		pressedView->remove(theme->id());
		//save the priority of the theme to be added because the add operation
		//always insert it in the vector of nodes with the least priority
		int prior = pressedThemeItem->order();
		dropView->add(theme);
		theme->priority(prior);

		vector<QListViewItem*> themeItemVector = dropViewItem->getChildren();
		for (i = 0; i < themeItemVector.size(); ++i)
		{
			themeItem = (TeQtThemeItem*)themeItemVector[i];
			TeAppTheme* appTheme = themeItem->getAppTheme();
			theme = (TeTheme*)appTheme->getTheme();
			theme->priority(themeItem->order());
			theme->view(dropView->id());

			//update the priorities in the database
			updateStr = "UPDATE te_theme SET priority = " + Te2String(theme->priority());
			updateStr += ", view_id = " + Te2String(dropView->id());
			updateStr += " WHERE theme_id = " + Te2String(theme->id());
			if (currentDatabase_->execute(updateStr) == false)
			{
				QMessageBox::warning(this, tr("Warning"),
					tr("Fail to update the priorities of the themes in the database!"));
				return;
			}
		}

		//sort the vector of themes of the drop view according their priorities
		dropView->sort();

		if(pressedViewCurrentTheme == theme->id())
			pressedView->setCurrentTheme(-1);
		if(currentTheme_ == appTheme)
		{
			currentView_ = dropView;
			currentView_->setCurrentTheme(theme->id());
		}
	}

	//update the theme priorities of the pressed item view
	vector<QListViewItem*> themeItemVector = pressedViewItem->getChildren();

	for (i = 0; i < themeItemVector.size(); ++i)
	{
		themeItem = (TeQtThemeItem*)themeItemVector[i];
		TeAppTheme* appTheme = themeItem->getAppTheme();
		theme = (TeTheme*)appTheme->getTheme();
		theme->priority(themeItem->order());

		//update the priorities in the database
		updateStr = "UPDATE te_theme SET priority = " + Te2String(theme->priority());
		updateStr += " WHERE theme_id = " + Te2String(theme->id());
		if (currentDatabase_->execute(updateStr) == false)
		{
			QMessageBox::warning(this, tr("Warning"),
				tr("Fail to update the priorities of the themes in the database!"));
			return;
		}
		//sort the vector of themes of the pressed item view according their priorities
		pressedView->sort();
	}
}


void TerraViewBase::textRepresentation()
{
	TextRep *textRep = new TextRep(this, "textRep", true);
	textRep->exec();
	delete textRep;
}



void TerraViewBase::editLegend()
{
	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	if(!(curTheme->type() == TeTHEME || curTheme->type() == TeEXTERNALTHEME))
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	if (curTheme->layer()->hasGeometry(TeRASTER) || curTheme->layer()->hasGeometry(TeRASTERFILE))  // launch raster slicing editor
	{
		RasterSlicingWindow* w = new RasterSlicingWindow(this,"w",true);
		w->exec();
		delete w;
	}
	// theme doesn't have a raster representation or there is a vector grouping 
	// defined, launch vetorial legend editor
	else
	{
		LegendWindow legendWindow(this);
		if (legendWindow.exec())
		{
			viewsListView_->setOpen(viewsListView_->currentThemeItem(), true);
			if (curTheme->grouping().groupChronon_ == TeNOCHRONON)
				drawAction_activated();
			else
			{
				Animation anima(this);
				QDir animaQDir = QDir :: currentDirPath();
				QString path = animaQDir.currentDirPath() + "/animation";
				anima.setAnimaParams(path, canvas_);
				anima.directoryPushButton->setEnabled(false);
				anima.exec();
			}
		}
	}
}


void TerraViewBase::editPieBarChart()
{
	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	if(!(curTheme->type() == TeTHEME || curTheme->type() == TeEXTERNALTHEME))
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}
  
  if (curTheme->sqlNumAttList().empty())
	{
		QMessageBox::warning(this, tr("Warning"),
				tr("Theme has no numeric attributes to create Pie or Bar charts."));
		return;		
	}

	TeQtLegendTitleItem *popupLegendTitleItem = (TeQtLegendTitleItem*)viewsListView_->popupItem();
	TeQtThemeItem *themeItem = (TeQtThemeItem*)popupLegendTitleItem->parent();
	viewsListView_->selectItem(themeItem);

	PieBarChart *pieBarChart = new PieBarChart(this, "PieBarChart", false);
	if (pieBarChart->exec() == QDialog::Rejected)
		delete pieBarChart; 
}


void TerraViewBase::changeLegendVisual( TeLegendEntry * leg )
{
	VisualWindow *visualWindow = new VisualWindow(this, "visualWindow",true);
	visualWindow->setCaption(tr("Legend Visual"));

	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	visualWindow->loadLegend(*leg, curTheme);

	visualWindow->exec();
	delete visualWindow;

	currentDatabase_->updateLegend(leg);
//	currentDatabase_->loadLegend(currentTheme_);
	if (curTheme->grouping().groupMode_ == TeRasterSlicing)
		curTheme->removeRasterVisual();

	TeQtThemeItem *themeItem = viewsListView_->currentThemeItem();
	TeQtLegendItem *legendItem = (TeQtLegendItem*)themeItem->getLegendItem(leg);
	legendItem->changeVisual(leg);
	canvas_->clearAll();
	displayWindow_->plotData();		
}

void TerraViewBase::popupChangeLegendVisualSlot()
{
	TeQtLegendItem *legendItem = (TeQtLegendItem*)viewsListView_->popupItem();
	TeLegendEntry* leg = legendItem->legendEntry();
	changeLegendVisual(leg);
}


void TerraViewBase::popupChangeChartColorSlot()
{
  	bool isOK = false;
	TeColor color;
	unsigned int i;

	TeQtChartItem *chartItem = (TeQtChartItem*) viewsListView_->popupItem();
	TeQtThemeItem *themeItem = (TeQtThemeItem*)chartItem->parent();
	TeAppTheme *appTheme = themeItem->getAppTheme();

	color = chartItem->color();
	QColor inputColor = QColor(color.red_, color.green_, color.blue_);

	for(i = 0; i < appTheme->chartAttributes_.size(); ++i)
	{
		string label = appTheme->chartAttributes_[i];
		if (chartItem->text().latin1() == label)
			break;
	}

	QColor outputColor = QColorDialog::getRgba (inputColor.rgb(), &isOK, this);
	if (isOK)
	{
		color.init(outputColor.red(), outputColor.green(), outputColor.blue());
		appTheme->chartColors_[i].red_ = color.red_;
		appTheme->chartColors_[i].green_ = color.green_;
		appTheme->chartColors_[i].blue_ = color.blue_;

		chartItem->changeColor(color);

		updateAppTheme(currentDatabase_, appTheme);
		canvas_->clearAll();
		displayWindow_->plotData();
	}
}


void TerraViewBase::popupLegendOrChartPropertiesSlot()
{

}


void TerraViewBase::popupThemeVisibility()
{
	TeQtThemeItem *themeItem = (TeQtThemeItem*)viewsListView_->popupItem();
	themeVisibility(themeItem);
}

void TerraViewBase::setCurrentThemeVisibility()
{
	TeQtThemeItem *themeItem = viewsListView_->currentThemeItem();
	themeVisibility(themeItem);	
}

void TerraViewBase::themeVisibility(TeQtThemeItem *themeItem)
{
	TeAppTheme *appTheme = themeItem->getAppTheme();
	TeAbstractTheme* abstractTheme = appTheme->getTheme();
	
	int visOld = abstractTheme->visibleRep();
	ThemeVisibility themeVis(this, "themeVis", true);
	themeVis.loadTheme(appTheme);
	if (themeVis.exec() != QDialog::Accepted)
		return;

	int vis = abstractTheme->visibleRep();
	int smin = 0, smax = 0;
	if(themeVis.minimumScaleComboBox->currentText().isEmpty() == false)
		smin = themeVis.minimumScaleComboBox->currentText().toInt();
	if(themeVis.maximumScaleComboBox->currentText().isEmpty() == false)
		smax = themeVis.maximumScaleComboBox->currentText().toInt();

	if((int)abstractTheme->maxScale() == smax && (int)abstractTheme->minScale() == smin && vis == visOld)
		return;

	abstractTheme->minScale((double)smin);
	abstractTheme->maxScale((double)smax);


	TeWaitCursor wait;

	TeDatabase* db = NULL;

	TeTheme* theme = dynamic_cast<TeTheme*>(abstractTheme);	
	if(theme != NULL)
	{		
		db = theme->layer()->database();
	}
	else
	{
		db = currentDatabase_;
	}
	if(db == NULL)
	{
		return;
	}

	displayWindow_->plotData();
	
	string up = "UPDATE te_theme SET visible_rep = " + Te2String(vis);
	if(smin != 0 && & smax != 0)
	{
		up += ", min_scale = " + Te2String(smin);
		up += ", max_scale = " + Te2String(smax);
	}
	else
	{
		up += ", min_scale = null";
		up += ", max_scale = null";
	}
	up += " WHERE theme_id = " + Te2String(abstractTheme->id());
	if(db->execute(up) == false)
	{
		wait.resetWaitCursor();
		QMessageBox::warning(this, tr("Warning"),
		tr("Fail to update the visibility of the representations of the theme!"));
		return;
	}
	
	up = "UPDATE te_theme_application SET ";
	up += " show_canvas_leg = " + Te2String(appTheme->showCanvasLegend());
	up += ", canvas_leg_x = " + Te2String(appTheme->canvasLegUpperLeft().x());
	up += ", canvas_leg_y = " + Te2String(appTheme->canvasLegUpperLeft().y());
	up += " WHERE theme_id = " + Te2String(abstractTheme->id());
	if(db->execute(up) == false)
	{
		wait.resetWaitCursor();
		QMessageBox::warning(this, tr("Warning"),
		tr("Fail to update the te_theme_application table!"));
		return;
	}
	wait.resetWaitCursor();
}




bool TerraViewBase::editionIsActivated()
{
	return (enableEditAction->isOn());
}


void TerraViewBase::graphicAction_toggled(bool status)
{
	if(status)
	{
		graphic_->show();
		if (displayWindow_->isVisible())
			tileWindowsAction_activated();
		else
			graphic_->showMaximized();

	}
	else
	{
		graphic_->hide();
		if (displayWindow_->isVisible())
			displayWindow_->showMaximized();

		if(boxesVector_.empty() == false)
		{
			TeBox box = canvas_->getWorld();
			TeBox box2 = boxesVector_[0];
			if(box2.x1_ != box.x1_ || box2.y1_ != box.y1_)
			{
				canvas_->setWorld(box, cvContentsW_, cvContentsH_);
				displayWindow_->plotData();
				updateBoxesVector(box);
			}
		}
	}

	graphicAction->setOn(status);
}



bool TerraViewBase::graphicCursorIsActivated()
{
	return (graphicCursorAction->isOn());
}

void TerraViewBase::setGraphicParams()
{
	if (graphicParams_ == 0)
		graphicParams_ = new GraphicParams(this, "graphicParams", false);
	graphicParams_->show();
}


TeBox& TerraViewBase::getChangedBox()
{
	return bgAndBoxChanged_;
}


void TerraViewBase::createCellsAction_activated()
{
	bgAndBoxChanged_ = TeBox();
	CreateCells *createCells = new CreateCells(this,"createCells",true);
	if (createCells->exec() == QDialog::Accepted)
	{
		TeLayer *layer = createCells->getNewLayer();
		if (layer)
		{
			new TeQtLayerItem(databasesListView_->currentDatabaseItem(),
				layer->name().c_str(), layer);		
			QMessageBox::information( this, tr("Information"),
				tr("The infolayer was created successfully!"));
		}
		else
			QMessageBox::critical(this, tr("Error"),
				tr("Fail to create the infolayer with cells!"));
	}	
	delete createCells ;
}


void TerraViewBase::popupStatisticSlot()
{
	StatisticWindow* statistic = new StatisticWindow(this, "statistic", false);
	statistic->exec();
	delete statistic;
}


void TerraViewBase::popupHistogramSlot(int i)
{
	if(graphicParams_ == 0)
		setGraphicParams();
	else
		graphicParams_->reinit();
	graphicParams_->hide();
	
	string none;
	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	string fname = curTheme->sqlAttList()[gridColumn_].rep_.name_;

	graphicParams_->applyGraphic(i, fname, none, Histogram);
	graphicAction_toggled(true);
}

void TerraViewBase::popupNormalProbSlot(int i)
{
	if(graphicParams_ == 0)
		setGraphicParams();
	else
		graphicParams_->reinit();
	graphicParams_->hide();

	string none;
	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	string fname = curTheme->sqlAttList()[gridColumn_].rep_.name_;
	graphicParams_->applyGraphic(i, fname, none, NormalProbability);
}

void TerraViewBase::popupDispersionSlot(int i)
{
	if(graphicParams_ == 0)
		setGraphicParams();
	else
		graphicParams_->reinit();
	graphicParams_->hide();

	int n, nc = grid_->numCols();
	vector<int>	cols;

	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	for(n = 0; n < nc; ++n)
	{
		if (grid_->isColumnSelected(n, true) == true)
		{
			int col = grid_->getColumn(n);
			if(curTheme->sqlAttList()[col].rep_.type_ == TeREAL ||
				curTheme->sqlAttList()[col].rep_.type_ == TeINT)
				cols.push_back(col);
		}
	}

	string fnamex = curTheme->sqlAttList()[cols[0]].rep_.name_;
	string fnamey = curTheme->sqlAttList()[cols[1]].rep_.name_;
	graphicParams_->applyGraphic(i, fnamex, fnamey, Dispersion);
}


void TerraViewBase::popupTooltipSlot(int i)
{
	switch(i)
	{
		case 0:
		showTooltip_ = "ALL";
		break;
		case 1:
		showTooltip_ = "MIN";
		break;
		case 2:
		showTooltip_ = "MAX";
		break;
		case 3:
		showTooltip_ = "COUNT";
		break;
		case 4:
		showTooltip_ = "AVG";
		break;
		case 5:
		showTooltip_ = "SUM";
		break;
		case 6:
		showTooltip_ = "HIDE";
		break;
	}
}


void TerraViewBase::popupAddColumnSlot()
{
	TeAttrTableVector atvec; 
	((TeTheme*)currentTheme_->getTheme())->getAttTables(atvec);
	if(atvec.size() == 0)
	{
		QMessageBox::warning(this, tr("Warning"),
		tr("There is no table of static type to append a column!"));
		return;
	}

	AddColumn *addColumn = new AddColumn(this, "addColumn", true);
	addColumn->exec();
	delete addColumn;   
}


void TerraViewBase::popupDeleteColumnSlot()
{
	if(currentTheme_->getTheme()->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	QString msg;

	if(grid_->getPortal() == 0)
		return;

	bool sortBy = grid_->isSortBy(gridColumn_);

	string colName = grid_->getPortal()->getAttribute(gridColumn_).rep_.name_;
	if(grid_->isIndex(gridColumn_)) // column swaped
	{
		msg = tr("The column") + " \"";
		msg += colName.c_str();
		msg += "\" " + tr("could not be deleted because it is an index!");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

	msg = tr("Do you really want to delete the column") + " \"";
	msg += colName.c_str();
	msg += "\" ?";
	int response = QMessageBox::question(this, tr("Delete Column"), msg,
		tr("Yes"), tr("No"));

	if(response == 1)
		return;

	TeWaitCursor wait;

	// Get the index of the table whose column is to be deleted
	int tableIndex = currentTheme_->getTableIndexFromField(colName);

	// Get the table from the vector of theme tables through its index
	TeAttrTableVector attrTableVector; 
	((TeTheme*)currentTheme_->getTheme())->getAttTables(attrTableVector);
	TeTable& table = attrTableVector[tableIndex];
	string tableName = table.name();
	string tableId = Te2String(table.id());

	unsigned int position = colName.find(".");
	if (position != string::npos)
		colName = colName.substr(position + 1);

	int x = grid_->contentsX();
	int y = grid_->contentsY();

	if(sortBy)
		grid_->clear();
	else
		grid_->clearPortal();

	// Try to delete the column from the table
	if (!currentDatabase_->deleteColumn(tableName, colName))
	{
		wait.resetWaitCursor();
	    msg = tr("Error deleting column of the table!");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

	// Update all the themes that uses this table
	vector<TeQtViewItem*> viewItemVec = viewsListView_->getViewItemVec();
	for (unsigned int i = 0; i < viewItemVec.size(); ++i)
	{
		vector<TeQtThemeItem*> themeItemVec = viewItemVec[i]->getThemeItemVec();
		for (unsigned int j = 0; j < themeItemVec.size(); ++j)
		{
			TeAppTheme* appTheme = themeItemVec[j]->getAppTheme();
			TeTheme* theme = (TeTheme*)appTheme->getTheme();
			if(theme->type() == TeTHEME)
			{
				if (theme->isThemeTable(table.id()) == true)
				{
					theme->layer()->loadLayerTables();
					theme->loadThemeTables();
				}
			}
		}
	}

	if(sortBy)
		grid_->init(currentTheme_);
	else
		grid_->initPortal();
	grid_->setContentsPos(x, y);
	grid_->refresh();
	wait.resetWaitCursor();
}


void TerraViewBase::deleteSelectedColumns()
{
	QString msg;
	int	i, j;

	if(grid_->getPortal() == 0)
		return;

	vector<int> cVec = grid_->getSelectedColumns();
	if(cVec.size() == 0)
	{
		msg = tr("Select the columns to be deleted!");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

	vector<string> colVec;
	vector<int> indexVec;
	bool sortBy = false;

	for(i=0; (unsigned int)i<cVec.size(); i++)
	{
		j = cVec[i];
		if(grid_->isIndex(j))
			indexVec.push_back(j);
		else
			colVec.push_back(grid_->getPortal()->getAttribute(j).rep_.name_);

		if(sortBy == false)
			sortBy = grid_->isSortBy(j);
	}

	string indexNames;
	for(i=0; (unsigned int)i<indexVec.size(); i++)
	{
		j = indexVec[i];
		indexNames += grid_->getPortal()->getAttribute(j).rep_.name_ + ", ";
	}
	if(indexNames.empty() == false)
		indexNames = indexNames.substr(0, indexNames.size()-2);

	string cNames;
	for(i=0; (unsigned int)i<cVec.size(); i++)
	{
		j = cVec[i];
		cNames += grid_->getPortal()->getAttribute(j).rep_.name_ + ", ";
	}
	if(cNames.empty() == false)
		cNames = cNames.substr(0, cNames.size()-2);

	msg = tr("Do you really want to delete the column(s):") + " ";
	msg += cNames.c_str();
	msg += " ?";
	int response = QMessageBox::question(this, tr("Delete Column"), msg,
		tr("Yes"), tr("No"));

	if(response == 1)
		return;

	TeWaitCursor wait;

	if(sortBy)
		grid_->clear();
	else
		grid_->clearPortal();

	set<int> tableIdSet;
	for(i=0; (unsigned int)i<colVec.size(); i++)
	{
		string colName = colVec[i];
		TeTable table = grid_->findTable(colName);
		tableIdSet.insert(table.id());
		string tableName = table.name();

		unsigned int position = colName.find(".");
		if (position != string::npos)
			colName = colName.substr(position + 1);
		currentDatabase_->deleteColumn(tableName, colName);
	}

	int x = grid_->contentsX();
	int y = grid_->contentsY();

	// Update all the themes that uses these tables
	set<int>::iterator tit = tableIdSet.begin();
	while(tit != tableIdSet.end())
	{
		int tableId = (*tit);
		vector<TeQtViewItem*> viewItemVec = viewsListView_->getViewItemVec();
		for (unsigned int i = 0; i < viewItemVec.size(); ++i)
		{
			vector<TeQtThemeItem*> themeItemVec = viewItemVec[i]->getThemeItemVec();
			for (unsigned int j = 0; j < themeItemVec.size(); ++j)
			{
				TeAppTheme* appTheme = themeItemVec[j]->getAppTheme();
				TeTheme* theme = (TeTheme*)appTheme->getTheme();
				if(theme->type() == TeTHEME)
				{
					if (theme->isThemeTable(tableId) == true)
					{
						theme->layer()->loadLayerTables();
						theme->loadThemeTables();
					}
				}
			}
		}
		++tit;
	}

	if(sortBy)
		grid_->init(currentTheme_);
	else
		grid_->initPortal();
	grid_->setContentsPos(x, y);
	grid_->refresh();

	if(indexVec.size() == 1)
	{
		msg = tr("The column:") + " ";
		msg += indexNames.c_str();
		msg += " " + tr("could not be deleted because it is index!");
		QMessageBox::warning(this, tr("Warning"), msg);
		wait.resetWaitCursor();
	}
	else if(indexVec.size() > 1)
	{
		msg = tr("The columns:") + " ";
		msg += indexNames.c_str();
		msg += " " + tr("could not be deleted because they are indexes!");
		QMessageBox::warning(this, tr("Warning"), msg);
		wait.resetWaitCursor();
	}
	wait.resetWaitCursor();
}

void TerraViewBase::popupChangeColumnDataSlot()
{
	ChangeColumnData *changeColumnData = new ChangeColumnData(this, "changeColumnData", true);
	changeColumnData->exec();
	delete changeColumnData;   
}

void TerraViewBase::popupChangeColumnNameSlot()
{
	ChangeColumnName *changeColumnName = new ChangeColumnName(this, "changeColumnName", true);
	changeColumnName->exec();
	delete changeColumnName;   
}

void TerraViewBase::popupChangeColumnTypeSlot()
{
	ChangeColumnType *changeColumnType = new ChangeColumnType(this, "changeColumnType", true);
	changeColumnType->exec();
	delete changeColumnType;   
}


void TerraViewBase::popupAdjustColumnWidthSlot()
{

}

void TerraViewBase::popupShowDefaultMediaSlot()
{
	if(currentTheme_->getTheme()->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	if(showMediaWindow_ == 0)
	{
		QPoint	p(300, 200);
		showMediaWindow_ = new TeQtShowMedia;
		showMediaWindow_->reparent(this, Qt::WType_TopLevel, p, true);
	}
	showMediaWindow_->init(mediaId_, currentTheme_, grid_);
	showMediaWindow_->hide();
	showMediaWindow_->row_ = -1;
	showMediaWindow_->slotShow();
}


void TerraViewBase::popupInsertMediaSlot()
{
	if(currentTheme_->getTheme()->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	if(showMediaWindow_ == 0)
	{
		QPoint	p(300, 200);
		showMediaWindow_ = new TeQtShowMedia;
		showMediaWindow_->reparent(this, Qt::WType_TopLevel, p, true);
	}
	showMediaWindow_->init(mediaId_, currentTheme_, grid_);
	showMediaWindow_->hide();
	showMediaWindow_->slotInsert(lastOpenDir_);
}


void TerraViewBase::popupInsertURLSlot()
{
	if(currentTheme_->getTheme()->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	if(showMediaWindow_ == 0)
	{
		QPoint	p(300, 200);
		showMediaWindow_ = new TeQtShowMedia;
		showMediaWindow_->reparent(this, Qt::WType_TopLevel, p, true);
	}
	showMediaWindow_->init(mediaId_, currentTheme_, grid_);
	showMediaWindow_->hide();
	showMediaWindow_->slotInsertURL();
}


void TerraViewBase::popupDefaultMediaDescriptionSlot()
{
	if(currentTheme_->getTheme()->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	if(showMediaWindow_ == 0)
	{
		QPoint	p(300, 200);
		showMediaWindow_ = new TeQtShowMedia;
		showMediaWindow_->reparent(this, Qt::WType_TopLevel, p, true);
	}
	showMediaWindow_->init(mediaId_, currentTheme_, grid_);
	showMediaWindow_->hide();
	showMediaWindow_->slotDescription();
}


void TerraViewBase::popupAttributesAndMediaSlot()
{
	if(currentTheme_->getTheme()->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	if(showMediaWindow_ == 0)
	{
		QPoint	p(300, 200);
		showMediaWindow_ = new TeQtShowMedia;
		showMediaWindow_->reparent(this, Qt::WType_TopLevel, p, true);
	}

	showMediaWindow_->init(mediaId_, currentTheme_, grid_);
	if(showMediaWindow_->isHidden())
		showMediaWindow_->show();
//	showMediaWindow_->raise();
}

void TerraViewBase::importRasterAction_activated()
{
	RasterImportWizard *importWindow = new RasterImportWizard(this, "RasterImportWizard", true);
	importWindow->setParams(currentDatabase_,lastOpenDir_);
	if (importWindow->exec() == QDialog::Accepted)
	{
    //if (!importWindow->isMosaic())
	if(true)
    {
		  TeLayer *layer = importWindow->getLayer();
		  if (layer != 0)
		  {
			  TeQtDatabaseItem *databaseItem = databasesListView_->currentDatabaseItem();
			  TeQtLayerItem *layerItem = new TeQtLayerItem((QListViewItem*)databaseItem, layer->name().c_str(), layer);
			  layerItem->setEnabled(true);
			  if (databasesListView_->isOpen(layerItem->parent()) == false)
				  databasesListView_->setOpen(layerItem->parent(),true);
			  checkWidgetEnabling();
			  QString msg = tr("The raster data was imported successfully!") + "\n";
			  msg += tr("Do you want to display the data?");
			  int response = QMessageBox::question(this, tr("Display Raster Data"), msg, tr("Yes"), tr("No"));

			  if(response == 0)
			  {
				  // find a valid name for a view
				  QString viewName = layer->name().c_str();
				  bool flag = true;
				  int count = 1;
				  while (flag)
				  {
					  TeViewMap& viewMap = currentDatabase_->viewMap();
					  TeViewMap::iterator it;
					  for (it = viewMap.begin(); it != viewMap.end(); ++it)
					  {
						  if (it->second->name() == viewName.latin1())
							  break;
					  }
					  if (it == viewMap.end())
						  flag = false;
					  else
					  {
						  char vnumber[5];
						  sprintf(vnumber,"_%d",count);
						  viewName += vnumber;
						  count++;
					  }
				  }
  				
				  // create a projection to the view the parameters of the imported layer
				  TeProjection *viewProjection = TeProjectionFactory::make(layer->projection()->params());

				  // create a view and a theme on the main window for data visualization
				  // add the new view to the database and insert it on the map of Views
				  TeView *view = new TeView (viewName.latin1(), currentDatabase_->user());
				  view->projection(viewProjection);
				  TeBox b;
				  view->setCurrentBox(b);
				  view->setCurrentTheme(-1);
				  currentDatabase_->insertView(view);
				  if(currentDatabase_->projectMap().empty() == false)
				  {
					  TeProjectMap& pm = currentDatabase_->projectMap();
					  TeProject* project = pm.begin()->second;
					  project->addView(view->id());
				  }
				  currentDatabase_->insertProjectViewRel(1, view->id());

				  checkWidgetEnabling();
  				
				  // create the view item and set it as the current one
				  TeQtViewItem *viewItem = new TeQtViewItem(viewsListView_, viewName, view);

				  // create the theme that will be associated to this view
				  TeAttrTableVector ATV;
				  layer->getAttrTables(ATV, TeAttrStatic);
				  QString themeName = currentDatabase_->getNewThemeName(layer->name()).c_str();
				  if(createTheme(themeName.latin1(), view, layer, "", ATV) == false)
				  {
					  QString msg = tr("The theme") + " \"" + themeName;
					  msg += "\" " + tr("could not be inserted into the view") + " \"" + view->name().c_str() + "\" !";
					  QMessageBox::critical(this, tr("Error"), msg);
				  }

				  vector<TeQtThemeItem*> themeItemVec = viewItem->getThemeItemVec();
				  TeAppTheme* newTheme = themeItemVec[0]->getAppTheme();

				  grid_->clear();
				  QListViewItem* item = (QListViewItem*)viewsListView_->getThemeItem(newTheme);
				  viewsListView_->setOpen(viewItem, true);
				  viewsListView_->setOn((QCheckListItem*)item, true);
				  viewsListView_->selectItem(item);

				  resetAction_activated();
			  }
		  }
    }
    else
    {
    }
	}
	lastOpenDir_ = importWindow->getLastOpenDir();
	delete importWindow;
}



void TerraViewBase::vectorialExportAction_activated()
{
	if (currentDatabase_->layerMap().empty())
	{
	    QMessageBox::critical(this, tr("Error"),tr("The database has no layers!"));
	    return;
	}
	TeDatabasePortal* portal = currentDatabase_->getPortal();
	string sql;
  sql = "SELECT COUNT(te_layer.layer_id) FROM (te_layer INNER JOIN te_representation ON te_layer.layer_id = te_representation.layer_id) INNER JOIN te_layer_table ON te_layer.layer_id = te_layer_table.layer_id WHERE ";
	sql += " ((te_layer_table.attr_table_type=1 Or te_layer_table.attr_table_type=3) AND (te_representation.geom_type=1 Or te_representation.geom_type=2 Or te_representation.geom_type=4 Or te_representation.geom_type=8)) ";
	if (portal->query(sql) && portal->fetchRow())
  {
	  if (portal->getInt(0) <= 0)
    {
	    QMessageBox::critical(this, tr("Error"),   
			tr("The database has no layers with static table and vectorial representation!"));
		  delete portal;
      return;
		}
	}
  else
  {
	    QMessageBox::critical(this, tr("Error"),   
			tr("Error accessing the database."));
		  delete portal;
      return;
  }
  delete portal;
	ExportWindow *exportWindow = new ExportWindow(this, "exportWindow", true);
	exportWindow->setParams(currentDatabase_,lastOpenDir_);
	exportWindow->init(currentLayer_);
	exportWindow->exec();
	lastOpenDir_ = exportWindow->getLastOpenDir();
	delete exportWindow;	
}


void TerraViewBase::saveDisplayAsFileAction_activated()
{
	if(canvas_->getPixmap0() == 0)
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("There is no display contents to be saved!"));
		return;
	}
	QString out = tr("Images (");
	QImageIO imaio;
	QStrList list = imaio.outputFormats();
	char* c = list.first();
	while(c != 0)
	{
		out += "*.";
		out += c;
		out += " ";
		c = list.next();
	}
	out += ")";


    QString s = QFileDialog::getSaveFileName(
                    "",
                    out,
                    this,
                    tr("Save File Dialog"),
                    tr("Choose a filename to save"));
	if(s.isNull() || s.isEmpty())
		return;
	int p = s.findRev(".");
	if(p == -1)
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Select the file format and try again!"));
		saveDisplayAsFileAction_activated();
	}
	else
	{
		QString ss = s;
		ss.remove(0, p);
		ss = ss.upper();
		ss.insert(0, "*");
		ss.append(" ");
		if(out.find(ss) == -1)
		{
			QMessageBox::warning(this, tr("Warning"),
				tr("File format is not valid, change and try again!"));
			saveDisplayAsFileAction_activated();
		}
	}

	printFileResolutionFunc(s);
}


void TerraViewBase::saveDisplayWithPrinterAction_activated()
{
	displayWindow_->plotOnPrinter("");
}


void TerraViewBase::importTableAction_activated()
{
	ImportTbl *importTable = new ImportTbl(this, "importTable", true);
	importTable->setParams(currentDatabase_,lastOpenDir_);
	if (importTable->exec()== QDialog::Accepted)
	{
		QMessageBox::information(this,tr("Information"),tr("The table was successfully imported!"));
	}
	lastOpenDir_ = importTable->getLastOpenDir();
	delete importTable;
}

void TerraViewBase::importTablePointsAction_activated()
{
	ImportTblPoints *importPoints = new ImportTblPoints(this,"importTableOfPoints",true);
	if (importPoints->exec() == QDialog::Accepted)
	{
		TeLayer *layer = importPoints->getLayer();
		if (layer != 0)
		{
			QString msg = tr("The data was imported successfully!") + "\n";
			msg += tr("Do you want to display the data?");
			int response = QMessageBox::question(this, tr("Display the Data"),
				msg, tr("Yes"), tr("No"));
			if(response == 0)
			{
				// find a valid name for a view
				QString viewName = layer->name().c_str();
				bool flag = true;
				int count = 1;
				while (flag)
				{
					TeViewMap& viewMap = currentDatabase_->viewMap();
					TeViewMap::iterator it;
					for (it = viewMap.begin(); it != viewMap.end(); ++it)
					{
						if (it->second->name() == viewName.latin1())
							break;
					}
					if (it == viewMap.end())
						flag = false;

					else
					{
						char vnumber[5];
						sprintf(vnumber,"_%d",count);
						viewName += vnumber;
						count++;
					}
				}
				
				// create a projection to the view the parameters of the imported layer
				TeProjection *viewProjection = TeProjectionFactory::make(layer->projection()->params());

				// create a view and a theme on the main window for data visualization
				// add the new view to the database and insert it on the map of Views
				TeView *view = new TeView (viewName.latin1(), currentDatabase_->user());
				TeBox b;
				view->setCurrentBox(b);
				view->setCurrentTheme(-1);
				view->projection(viewProjection);

				if (!currentDatabase_->insertView(view))
				{
					QString msg = tr("The view") + " \"" + view->name().c_str() + "\" " + tr("could not be saved into the database!");
					QMessageBox::critical( this, tr("Error"), msg);
					delete importPoints;
					return;
				}
				if(currentDatabase_->projectMap().empty() == false)
				{
					TeProjectMap& pm = currentDatabase_->projectMap();
					TeProject* project = pm.begin()->second;
					project->addView(view->id());
				}
				currentDatabase_->insertProjectViewRel(1, view->id());

				checkWidgetEnabling();
				
				// create the view item and set it as the current one
				TeQtViewItem *viewItem = new TeQtViewItem(viewsListView_, viewName, view);

				// create the theme that will be associated to this view
				TeAttrTableVector ATV;
				layer->getAttrTables(ATV);
				QString themeName = currentDatabase_->getNewThemeName(layer->name()).c_str();
				if(createTheme(themeName.latin1(), view, layer, "", ATV) == false)
				{
					QString msg = tr("The theme") + " \"" + themeName + "\" ";
					msg += tr("could not be inserted into the view") + " \"" + view->name().c_str() + "\" !";
					QMessageBox::critical( this, tr("Error"), msg);
				}

				vector<TeQtThemeItem*> themeItemVec = viewItem->getThemeItemVec();
				TeAppTheme* newTheme = themeItemVec[0]->getAppTheme();

				grid_->clear();
				QListViewItem* item = (QListViewItem*)viewsListView_->getThemeItem(newTheme);
				viewsListView_->setOpen(viewItem, true);
				if(item)
				{
					viewsListView_->setOn((QCheckListItem*)item, true);
					viewsListView_->selectItem(item);
				}

				resetAction_activated();
			}
		}
	}
	delete importPoints;
}

void TerraViewBase::createProxMatrixAction_activated()
{
	if (currentDatabase_ && currentView_)
	{ 
		if(createProxMatrix_ == 0)
			createProxMatrix_ = new CreateProxMatrix(this, "createproxMatrix", false);
		else
			createProxMatrix_->init();
	}
	else
	{
		QString msg;
		if(currentDatabase_ == 0)
			msg = tr("Select a database!");
		else if(currentView_ == 0)
			msg = tr("Select a view!");
		QMessageBox::warning(this, tr("Warning"), msg);
	}
}

void TerraViewBase::selectProxMatrixAction_activated()
{
	if (currentDatabase_ && currentView_)
	{ 
		if(selectProxMatrix_ == 0)
			selectProxMatrix_ = new SelectProxMatrix(this, "selectProxMatrix", false);
		else
			selectProxMatrix_->init();
	}
	else
	{
		QString msg;
		if(currentDatabase() == 0)
			msg = tr("Select a database!");
		else if(currentView() == 0)
			msg = tr("Select a view!");
		
		QMessageBox::warning(this, tr("Warning"), msg);
	}
}

void TerraViewBase::spatialStatisticsAction_activated()
{
	if (currentDatabase() && currentView() && currentLayer() && currentTheme())
	{
	  if(currentTheme()->getTheme()->type() != TeTHEME)
	  {
		  QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		  return;
	  }
		Anaesp anaesp(this, "anaesp", true);
		anaesp.exec();
	}
	else
	{
		QString msg;
		if(currentDatabase() == 0)
			msg = tr("Select a database!");
		else if(currentView() == 0)
			msg = tr("Select a view!");
		else if(currentTheme()  == 0)
			msg = tr("Select a theme!");
		else if(currentLayer() == 0)
			msg = tr("Select a layer!");
		
		QMessageBox::warning(this, tr("Warning"), msg);
	}
}


void TerraViewBase::kernelMapAction_activated()
{
	if (!currentDatabase())
		return;

	// open the interface only when there is a view selected
	if (!currentView())	
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select a view!"));
		return;
	}

	// to disable widgets
	kernelEnableControl(false); 
	bool res;
 	if (kernel_ == NULL) 
	{
   		kernel_ = new KernelWindow(this, "kernel", false);
   		res = kernel_->loadView(true);
	}
  	else 
	{
    	res = kernel_->loadView(false);
	}
	if(res)
		kernel_->show();
	else
		kernelEnableControl(true); // to enable widgets
}

void TerraViewBase::kernelEnableControl( bool b )
{
	if(b)
	{
		openDatabaseAction->setEnabled(true); // icon open database
		importDataAction->setEnabled(true); // icon import vector
		importDataAction->setIconSet((QPixmap)import);
		addViewAction->setEnabled(true); // icon add view
		addViewAction->setIconSet((QPixmap)view_xpm);
		addThemeAction->setEnabled(!currentDatabase_->layerMap().empty());  // icon add theme
		addThemeAction->setIconSet((QPixmap)theme_xpm);
		fileMenu->setItemEnabled(fileMenu->idAt(0), true); // open database	
		fileMenu->setItemEnabled(fileMenu->idAt(2), true); // close database	
		fileMenu->setItemEnabled(fileMenu->idAt(4), true); // import vector	
		fileMenu->setItemEnabled(fileMenu->idAt(5), true); // import raster	
		fileMenu->setItemEnabled(fileMenu->idAt(6), true); // import table	
		fileMenu->setItemEnabled(fileMenu->idAt(7), true); // import points	
		menubar->setItemEnabled(menubar->idAt(3), true); // layer	
		menubar->setItemEnabled(menubar->idAt(4), true); // view	
		menubar->setItemEnabled(menubar->idAt(5), true); // theme	
		menubar->setItemEnabled(menubar->idAt(6), true); // analysis
		databasesListView_->setEnabled(true); // database tree

		TeViewMap& viewMap = currentDatabase_->viewMap();
		TeViewMap::iterator it;
		for (it = viewMap.begin(); it != viewMap.end(); ++it)
		{
			TeView* view = it->second;
			TeQtViewItem* viewItem = viewsListView_->getViewItem(view);
			viewItem->setEnabled(true);
		}
	}
	else
	{
		openDatabaseAction->setEnabled(false); // icon open database
		importDataAction->setEnabled(false); // icon import vector
		importDataAction->setIconSet((QPixmap)importDisabled);
		addViewAction->setEnabled(false); // icon add view
		addViewAction->setIconSet((QPixmap)viewDisabled);
		addThemeAction->setEnabled(false);  // icon add theme
		addThemeAction->setIconSet((QPixmap)themeDisabled);
		fileMenu->setItemEnabled(fileMenu->idAt(0), false); // open database	
		fileMenu->setItemEnabled(fileMenu->idAt(2), false); // close database	
		fileMenu->setItemEnabled(fileMenu->idAt(4), false); // import vector	
		fileMenu->setItemEnabled(fileMenu->idAt(5), false); // import raster	
		fileMenu->setItemEnabled(fileMenu->idAt(6), false); // import table	
		fileMenu->setItemEnabled(fileMenu->idAt(7), false); // import points	
		menubar->setItemEnabled(menubar->idAt(3), false); // layer	
		menubar->setItemEnabled(menubar->idAt(4), false); // view	
		menubar->setItemEnabled(menubar->idAt(5), false); // theme	
		menubar->setItemEnabled(menubar->idAt(6), false); // analysis	
		databasesListView_->setEnabled(false); // database tree

		TeViewMap& viewMap = currentDatabase_->viewMap();
		TeViewMap::iterator it;
		for (it = viewMap.begin(); it != viewMap.end(); ++it)
		{
			TeView* view = it->second;
			TeQtViewItem* viewItem = viewsListView_->getViewItem(view);
			if(view == currentView_)
				viewItem->setEnabled(true);
			else
			{
				viewItem->setOpen(false);
				viewItem->setEnabled(false);
			}
		}
	}
}


void TerraViewBase::selectThemeTables()
{
	SelectThemeTables *selThemeTables = new SelectThemeTables(this, "selThemeTables", true);
	selThemeTables->exec();
	delete selThemeTables;   
}


void TerraViewBase::createLayerTable()
{
	CreateTable *createTable = new CreateTable(this, "createTable", true);
	createTable->exec();
	delete createTable; 
}


void TerraViewBase::popupRemoveExternalTable()
{
    TeQtDatabaseItem *dbItem = (TeQtDatabaseItem*)databasesListView_->popupItem();
    removeExternalTable(dbItem);
}

void TerraViewBase::removeExternalTable(TeQtDatabaseItem* dbItem)
{
    RemoveTable *removeTable = new RemoveTable(this, "removeTable", true);	
	TeDatabase *db = dbItem->getDatabase();
	removeTable->showTables(db, 0);
	int ret = removeTable->result();
	if (ret != QDialog::Rejected)
		removeTable->exec();
	delete removeTable;
}

void TerraViewBase::deleteLayerTable()
{
	TeQtLayerItem *layerItem = (TeQtLayerItem*)databasesListView_->popupItem();
	TeLayer *layer = layerItem->getLayer();

	RemoveTable *removeTable = new RemoveTable(this, "removeTable", true);
	removeTable->showTables(layer->database(), layer);
	int ret = removeTable->result();
	if (ret != QDialog::Rejected)
		removeTable->exec();
	delete removeTable;
}


void TerraViewBase::resetBoxesVector()
{
	curBoxIndex_ = -1;
	boxesVector_.clear();
	checkWidgetEnabling();
//	canvas_->setWorld(TeBox());
	canvas_->setWorld(TeBox(), cvContentsW_, cvContentsH_);
}


void TerraViewBase::linkExternalTable()
{
	if(linkTable_ == 0)
		linkTable_ = new LinkExtTable(this, "LinkExtTable");
	
	if(linkTable_->init(currentTheme_) == false)
		return;

	linkTable_->show();
}


void TerraViewBase::popupUnlinkExternalTableSlot()
{
	int col = gridColumn_;
	string fieldName = grid_->horizontalHeader()->label(col).latin1();

	TeAttrTableVector attrTableVector;
	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	curTheme->getAttTables(attrTableVector);

	// Get the index of the table to be detached in the vector of theme tables
	unsigned int index = currentTheme_->getTableIndexFromField(fieldName);

	// Get the table corresponding to the index to be detached
	TeTable& table = attrTableVector[index];
	string tableNameAlias = curTheme->aliasVector()[index];

	// Check if there is at least another table which could be linked with 
	// the one that is to be detached. In positive case, send a warning
	// to say that this table cannot be detached because there is a table
	// that is attached to it.
	if (index != (attrTableVector.size() - 1))
	{
		TeTable nextTable = attrTableVector[index+1];
		string nextTableNameAlias = curTheme->aliasVector()[index+1];
		if (nextTable.relatedTableId() == table.id())
		{
			QString msg = tr("The table") + " \"" + tableNameAlias.c_str() + "\" " + tr("is used to link the external table") + " \"";
			msg += nextTableNameAlias.c_str();
			msg += "\" !\n" + tr("First, unlink the table") + " \"" + nextTableNameAlias.c_str() + "\".";
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}
	}

	QString msg = tr("Do you really want to unlink the external table") + " \"" + tableNameAlias.c_str() + "\" ?";
	int response = QMessageBox::question(this, tr("Unlink External Table"),
				   msg, tr("Yes"), tr("No"));
	if (response != 0)
			return;

	TeWaitCursor wait;

	// Remove the table from the memory and the database
	curTheme->removeThemeTable(index);
	currentDatabase_->removeThemeTable(curTheme, attrTableVector[index].getOrder());

	// Mount the new collection auxiliary table and reinitialize the grid
	grid_->clear();
	if (!curTheme->createCollectionAuxTable() || !curTheme->populateCollectionAux())
	{
		wait.resetWaitCursor();
	    QMessageBox::critical(this, tr("Error"),
		tr("Fail to mount the auxiliary table of the collection!"));
		return;
	}
	grid_->init(currentTheme_);
	wait.resetWaitCursor();
}


DisplayWindow* TerraViewBase::getDisplayWindow()
{
	return displayWindow_;
}


void TerraViewBase::updateBoxesVector( TeBox b )
{
	++curBoxIndex_;
	boxesVector_.push_back(b);
	checkWidgetEnabling();
}


string TerraViewBase::getTooltip()
{
	if (!currentTheme_)
		return "";

	string s;
	if(showTooltip_ == "HIDE" || columnTooltip_ == -1)
		return s;

	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	if(currentTheme_ && (int)curTheme->sqlAttList().size() > columnTooltip_)
	{
		if(showTooltip_ == "ALL")
			s = curTheme->sqlAttList()[columnTooltip_].rep_.name_;
		else
			s = showTooltip_ + "(" + curTheme->sqlAttList()[columnTooltip_].rep_.name_ + ")";
	}
	return s;
}


int TerraViewBase::gridColumn()
{
	return gridColumn_;
}


TeQtProgress* TerraViewBase::getProgress()
{
	return progress_;
}


GraphicParams* TerraViewBase::getGraphicParams()
{
	return graphicParams_;
}


GraphicWindow* TerraViewBase::getGraphicWindow()
{
	return graphic_;
}


void TerraViewBase::setMediaId( string id )
{
	mediaId_ = id;
}


void TerraViewBase::autoPromoteSlot()
{
	grid_->autoPromote_ = !(grid_->autoPromote_);
}


void TerraViewBase::createCountLegends( TeAppTheme * appTheme, string table, string field )
{
	TeTheme* theme = (TeTheme*)appTheme->getTheme();
	if(generateCountLegends(appTheme, table, field))
	{
		unsigned int i;
		string text;
		TeQtThemeItem *themeItem = viewsListView_->getThemeItem(appTheme);

		themeItem->removeCharts();
		themeItem->removeLegends();

		TeLegendEntryVector& legendVector = theme->legend();
		if (legendVector.size() > 0)
		{
			//insert the grouping attribute name
			TeAttributeRep rep;  
			string nattr;
			if(theme->grouping().groupMode_ != TeNoGrouping)
			{
				rep = theme->grouping().groupAttribute_;
				nattr = theme->grouping().groupNormAttribute_;
			}
			text = rep.name_;
			TeDatabase* database = theme->layer()->database();
			map<int, map<string, string> >::iterator it = database->mapThemeAlias().find(theme->id());
			if(it != database->mapThemeAlias().end())
			{
				map<string, string>& m = it->second;
				map<string, string>::iterator tit = m.find(text);
				if(tit != m.end())
					text = tit->second;
				if(!(nattr.empty() || nattr == "NONE"))
				{
					tit = m.find(nattr);
					if(tit != m.end())
						text += "/" + tit->second;
					else
						text += "/" + nattr;
				}
			}
			new TeQtLegendTitleItem(themeItem, text.c_str());				

			//insert the legend items
			for (i = 0; i < legendVector.size(); ++i)
			{
				TeLegendEntry& legendEntry = legendVector[i];
				string txt = legendEntry.label();
				new TeQtLegendItem(themeItem, txt.c_str(), &legendEntry);
			}
		}

		if(appTheme->chartAttributes_.size())
		{
			TeChartType chartType = (TeChartType)appTheme->chartType();
			if(chartType == TeBarChart)
				new TeQtChartTitleItem (themeItem, tr("Bar Chart"));
			else
				new TeQtChartTitleItem (themeItem, tr("Pie Chart"));

			for(i = 0; i < appTheme->chartAttributes_.size(); ++i)
			{
				string label = appTheme->chartAttributes_[i];
				TeColor color = appTheme->chartColors_[i];
				new TeQtChartItem(themeItem, label.c_str(), color);
				themeItem->updateAlias();
			}
		}
	}
}

void TerraViewBase::popupAddCentroidRepDatabasesListViewItem()
{
	TeQtLayerItem* layerItem = (TeQtLayerItem*)(databasesListView_->popupItem());
	TeLayer* layer = layerItem->getLayer();
	TeGeomRep geomRep;
	if(layer->hasGeometry (TePOLYGONS))
		geomRep = TePOLYGONS;
	else if (layer->hasGeometry (TeLINES))
		geomRep = TeLINES;
	else if (layer->hasGeometry (TeCELLS))
		geomRep = TeCELLS;
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("Layer doesn't have a valid representation to calculate centroids (polygons, cells our lines)"));
		return;
	}
	if(TeProgress::instance())
	{
		string caption = tr("Adding Centroid Representation").latin1();
		TeProgress::instance()->setCaption(caption.c_str());
		string msg = tr("Generating centroids. Please, wait!").latin1();
		TeProgress::instance()->setMessage(msg);
	}	
	
	TeWaitCursor wait;
	TePointSet centroids;
    vector<string> keysVec;
	if(!layer->database()->centroid(layer->tableName(geomRep), geomRep, centroids, keysVec))
	{
		wait.resetWaitCursor();
		QString msg = tr("The new representation could not be created!");
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}
	if(layer->addPoints(centroids))
	{
		if(!layer->database()->insertMetadata(layer->tableName(TePOINTS),layer->database()->getSpatialIdxColumn(TePOINTS), 0.005,0.005,layer->box()))
		{
			wait.resetWaitCursor();
			QString msg = tr((currentDatabase_->errorMessage()).c_str());
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}
		
		if(!layer->database()->createSpatialIndex (layer->tableName(TePOINTS),layer->database()->getSpatialIdxColumn(TePOINTS), (TeSpatialIndexType)TeRTREE))
		{
			wait.resetWaitCursor();
			QString msg = tr((currentDatabase_->errorMessage()).c_str());
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}
	}
	else
	{
		wait.resetWaitCursor();
		QString msg = tr("The new representation could not be created!");
		QMessageBox::critical(this, tr("Error"), msg);

	}
	wait.resetWaitCursor();
	if (layer->database()->dbmsName() == "PostGIS" || layer->database()->dbmsName() == "OracleSpatial")
	{
		QString msg = tr("The new representation was created successfully!");
		QMessageBox::information(this, tr("Information"), msg);
		return;
	}

	int response = QMessageBox::question(this, tr("Adding Centroid Representation"),
				tr("Do you wish to insert the centroids locations in an attribute table?"),
				tr("Yes"), tr("No"));

	if (response != 0)
	{
		QString msg = tr("The new representation was created successfully!");
		QMessageBox::information(this, tr("Information"), msg);
		return;
	}

	bool doRefresh= false;
	if (this->getGrid()->getTheme() && this->getGrid()->getTheme()->getTheme()->type() == TeTHEME)
	{		
		TeTheme* at = (TeTheme*) (this->getGrid()->getTheme()->getTheme());
		if (at->layer()->id() == layer->id())
		{
			this->getGrid()->clearPortal();
			doRefresh= true;
		}
	}

	TeAttrTableVector atts;
	if (!layer->getAttrTables(atts,TeAttrStatic))
	{
		QString msg = tr("Layer doesn't have static attribute tables!");
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}
	string tablename;
	int ntab = 0;
	if (atts.size() == 1)
		tablename = atts[0].name();
	else
	{
		QStringList lst;
		unsigned int nn;
		for (nn=0; nn<atts.size(); ++nn)
			lst << atts[nn].name().c_str();
		QString tname;
		bool ok;
		QString text = QInputDialog::getItem(tr("Adding Centroid Representation"),  
			           tr("Choose a table to add the centroids location:"), lst, 0, false, &ok, this );
		if ( ok ) 
			tablename = text.latin1();
		else
			return;
		for (nn=0; nn<atts.size(); ++nn)
		{
			if (tablename == atts[nn].name())
			{
				ntab = nn;
				break;
			}
		}
	}

	string colname = "CentrX";
	unsigned int aux1 = atts[ntab].attributeList().size();
	unsigned int aux2 = 1;
	unsigned int aux3 = 0;
	while (aux3<aux1)
	{
		if (atts[ntab].attributeList()[aux3].rep_.name_ == colname)
		{
			colname = "CentrX_" + Te2String(aux2);
			aux3 = 0;
			++aux2;
		}
		else
			++aux3;
	}
	TeAttributeRep rep;
	rep.name_ = colname;
	rep.type_ = TeREAL;
	if (layer->projection()->units() == "Meters")
		rep.decimals_ = 3;
	else
		rep.decimals_ = 10;
	rep.isAutoNumber_ = false;
	rep.isPrimaryKey_ = false;
	if (!layer->database()->addColumn(tablename,rep))
	{
		QString msg = tr("Fail to add column to store centroids X location.");
		msg += layer->database()->errorMessage().c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}
	string sql = "UPDATE " + tablename;
	sql += " INNER JOIN ";
	sql += layer->tableName(TePOINTS);
	sql += " ON ";
	sql += tablename;
	sql += ".";
	sql += atts[ntab].linkName();
	sql += " = ";
	sql += layer->tableName(TePOINTS);
	sql += ".object_id SET " + colname + " = ";
	sql += layer->tableName(TePOINTS);
	sql += ".x";
	if (!layer->database()->execute(sql))
	{
		QString msg = tr("Fail to update centroids X location attribute!");
		msg += layer->database()->errorMessage().c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}
		colname = "CentrY";
	aux2 = 1;
	aux3 = 0;
	while (aux3<aux1)
	{
		if (atts[ntab].attributeList()[aux3].rep_.name_ == colname)
		{
			colname = "CentrY_" + Te2String(aux2);
			aux3 = 0;
			++aux2;
		}
		else
			++aux3;
	}
	rep.name_ = colname;
	if (!layer->database()->addColumn(tablename,rep))
	{
		QString msg = tr("Fail to add column to store centroids Y location.");
		msg += layer->database()->errorMessage().c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}
	sql = "UPDATE " + tablename;
	sql += " INNER JOIN ";
	sql += layer->tableName(TePOINTS);
	sql += " ON ";
	sql += tablename;
	sql += ".";
	sql += atts[ntab].linkName();
	sql += " = ";
	sql += layer->tableName(TePOINTS);
	sql += ".object_id SET " + colname + " = ";
	sql += layer->tableName(TePOINTS);
	sql += ".y";
	if (!layer->database()->execute(sql))
	{
		QString msg = tr("Fail to update the centroids Y location attribute!");
		msg += layer->database()->errorMessage().c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}
	QString msg = tr("The new representation was created successfully!");
	QMessageBox::information(this, tr("Information"), msg);

	vector<TeQtViewItem*> viewItemVec = viewsListView_->getViewItemVec();
	for (unsigned int i = 0; i < viewItemVec.size(); ++i)
	{
		vector<TeQtThemeItem*> themeItemVec = viewItemVec[i]->getThemeItemVec();
		for (unsigned int j = 0; j < themeItemVec.size(); ++j)
		{
			TeAppTheme* appTheme = themeItemVec[j]->getAppTheme();
			if(appTheme->getTheme()->type() == TeTHEME)
			{
				TeTheme* theme = (TeTheme*)appTheme->getTheme();
				if (theme->isThemeTable(atts[ntab].name()))
				{
					theme->layer()->loadLayerTables();
					theme->loadThemeTables();
				}
			}
		}
	}
	if (doRefresh)
		grid_->init(currentTheme_);
	return;
}

void TerraViewBase::popupCreateCentroidLayerDatabasesListViewItem()
{
	TeQtLayerItem* layerItem = (TeQtLayerItem*)(databasesListView_->popupItem());
	TeLayer* layer = layerItem->getLayer();

	// check if layer has a representation where to calculate centroid
	TeGeomRep geomRep;
	if(layer->hasGeometry (TePOLYGONS))
		geomRep = TePOLYGONS;
	else if (layer->hasGeometry (TeLINES))
		geomRep = TeLINES;
	else if (layer->hasGeometry (TeCELLS))
		geomRep = TeCELLS;
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("Layer doesn't have a valid representation to calculate centroids (polygons, cells our lines)"));
		return;
	}

	// find a valid name for the new layer
	string name = layer->name() +"_Centr";
	unsigned int	j = 0;
	TeLayerMap& layerMap = currentDatabase_->layerMap();
	TeLayerMap::iterator layerIt = layerMap.begin();
	while (layerIt != layerMap.end())
	{
		if(layerIt->second->name() == name)
		{
			++j;
			name = layer->name() +"_Centr" + Te2String(j);
			layerIt != layerMap.begin();
		}
		else
			++layerIt;
	}
	TeLayer* layerOut = new TeLayer(name, layer->database(), layer->projection());
	if(TeProgress::instance())
	{
		string caption = tr("Centroid Generation").latin1();
		TeProgress::instance()->setCaption(caption.c_str());
		string msg = tr("Generating centroids. Please, wait!").latin1();
		TeProgress::instance()->setMessage(msg);
	}

	TeWaitCursor wait;
	TePointSet centroids;
    vector<string> keysVec;

	// calculate the centroids
	if(!layer->database()->centroid (layer->tableName (geomRep), geomRep, centroids, keysVec))
	{
		wait.resetWaitCursor();
		QString msg = tr("Fail generating the centroids!");
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}

	// save it to the new layer
	if (layerOut->addPoints(centroids))
	{
		if (!layerOut->database()->insertMetadata(layerOut->tableName(TePOINTS),layerOut->database()->getSpatialIdxColumn(TePOINTS), 0.005,0.005, layerOut->box()))
		{
			wait.resetWaitCursor();
			QString msg = tr((currentDatabase_->errorMessage()).c_str());
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}
		
		if (!layerOut->database()->createSpatialIndex (layerOut->tableName(TePOINTS), layerOut->database()->getSpatialIdxColumn(TePOINTS)))
		{
			wait.resetWaitCursor();
			QString msg = tr((currentDatabase_->errorMessage()).c_str());
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}
	}
	else
	{
		wait.resetWaitCursor();
		QString msg = tr("Error saving the centroids!");
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}

	// create an attribute table
	TeAttrTableVector tableVector;
	layer->getAttrTables(tableVector);
	TeAttrTableVector::iterator it = tableVector.begin();
	while(it != tableVector.end())
	{
		TeTable Tin = (*it);
		TeTable Tout = Tin;
		Tout.setId(-1);
		string tName = Tout.name() + "_Centr";
		j = 0;
		while (layer->database()->tableExist(tName))
		{
			++j;
			tName = Tout.name() + "_Centr" + Te2String(j);
		}
		Tout.name(tName);
		layerOut->createAttributeTable(Tout);
		
		TeAttributeList attList;
		layer->database()->getAttributeList(Tin.name(), attList);

		string ins = "INSERT INTO " + Tout.name() + " (";
		for(j=0; j<attList.size(); j++)
			ins += attList[j].rep_.name_ + ",";
		ins = ins.substr(0, ins.size()-1);

		ins += ") SELECT ";
		for(j=0; j<attList.size(); j++)
			ins += attList[j].rep_.name_ + ",";
		ins = ins.substr(0, ins.size()-1);
		ins += " FROM " + Tin.name();

		layer->database()->execute(ins);

		++it;
	}
	wait.resetWaitCursor();
	if (layer->database()->dbmsName() == "PostGIS" || layer->database()->dbmsName() == "OracleSpatial")
	{
		new TeQtLayerItem((TeQtDatabaseItem*)(layerItem->parent()),
		layerOut->name().c_str(), layerOut);

		QString msg = tr("The new layer was created successfully!");
		QMessageBox::information(this, tr("Information"), msg);
		return;
	}

	int response = QMessageBox::question(this, tr("Creating centroid infolayer"),
				tr("Do you wish to insert the centroids locations in an attribute table?"),
				tr("Yes"), tr("No"));

	if (response != 0)
	{
		new TeQtLayerItem((TeQtDatabaseItem*)(layerItem->parent()), layerOut->name().c_str(), layerOut);

		QString msg = tr("Centroid infolayer was created successfully!");
		QMessageBox::information(this, tr("Information"), msg);
		return;
	}

	bool doRefresh= false;
	if(this->getGrid()->getTheme() != NULL)
	{
		if (this->getGrid()->getTheme() && this->getGrid()->getTheme()->getTheme()->type() == TeTHEME)
		{		
			TeTheme* at = (TeTheme*) (this->getGrid()->getTheme()->getTheme());
			if (at->layer()->id() == layerOut->id())
			{
				this->getGrid()->clearPortal();
				doRefresh= true;
			}
		}
	}
	tableVector.clear();
	//if (!layer->getAttrTables(tableVector,TeAttrStatic))
	if (!layerOut->getAttrTables(tableVector,TeAttrStatic))
	{
		QString msg = tr("Layer doesn't have static attribute tables!");
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}
	string tablename;
	int ntab = 0;
	if (tableVector.size() == 1)
		tablename = tableVector[0].name();
	else
	{
		QStringList lst;
		unsigned int nn;
		for (nn=0; nn<tableVector.size(); ++nn)
			lst << tableVector[nn].name().c_str();
		QString tname;
		bool ok;
		QString text = QInputDialog::getItem(tr("Creating centroid infolayer"),  
			           tr("Choose a table to add the centroids location:"), lst, 0, false, &ok, this );
		if ( ok ) 
			tablename = text.latin1();
		else
			return;
		for (nn=0; nn<tableVector.size(); ++nn)
		{
			if (tablename == tableVector[nn].name())
			{
				ntab = nn;
				break;
			}
		}
	}
	string colname = "CentrX";
	unsigned int aux1 = tableVector[ntab].attributeList().size();
	unsigned int aux2 = 1;
	unsigned int aux3 = 0;
	while (aux3<aux1)
	{
		if (tableVector[ntab].attributeList()[aux3].rep_.name_ == colname)
		{
			colname = "CentrX_" + Te2String(aux2);
			aux3 = 0;
			++aux2;
		}
		else
			++aux3;
	}
	TeAttributeRep rep;
	rep.name_ = colname;
	rep.type_ = TeREAL;
	if (layerOut->projection()->units() == "Meters")
		rep.decimals_ = 3;
	else
		rep.decimals_ = 10;
	rep.isAutoNumber_ = false;
	rep.isPrimaryKey_ = false;
	if (!layerOut->database()->addColumn(tablename,rep))
	{
		QString msg = tr("Fail to add column to store centroids X location.");
		msg += layerOut->database()->errorMessage().c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}
	string sql = "UPDATE " + tablename;
	sql += " INNER JOIN ";
	sql += layerOut->tableName(TePOINTS);
	sql += " ON ";
	sql += tablename;
	sql += ".";
	sql += tableVector[ntab].linkName();
	sql += " = ";
	sql += layerOut->tableName(TePOINTS);
	sql += ".object_id SET " + colname + " = ";
	sql += layerOut->tableName(TePOINTS);
	sql += ".x";
	if (!layerOut->database()->execute(sql))
	{
		QString msg = tr("Fail to update X centroid location attribute!");
		msg += layerOut->database()->errorMessage().c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}
	colname = "CentrY";
	aux2 = 1;
	aux3 = 0;
	while (aux3<aux1)
	{
		if (tableVector[ntab].attributeList()[aux3].rep_.name_ == colname)
		{
			colname = "CentrY_" + Te2String(aux2);
			aux3 = 0;
			++aux2;
		}
		else
			++aux3;
	}
	rep.name_ = colname;
	if (!layerOut->database()->addColumn(tablename,rep))
	{
		QString msg = tr("Fail to add column to store centroids Y location.");
		msg += layerOut->database()->errorMessage().c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}
	sql = "UPDATE " + tablename;
	sql += " INNER JOIN ";
	sql += layerOut->tableName(TePOINTS);
	sql += " ON ";
	sql += tablename;
	sql += ".";
	sql += tableVector[ntab].linkName();
	sql += " = ";
	sql += layerOut->tableName(TePOINTS);
	sql += ".object_id SET " + colname + " = ";
	sql += layerOut->tableName(TePOINTS);
	sql += ".y";
	if (!layerOut->database()->execute(sql))
	{
		QString msg = tr("Fail to update Y centroid location attribute!");
		msg += layerOut->database()->errorMessage().c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}

	vector<TeQtViewItem*> viewItemVec = viewsListView_->getViewItemVec();
	for (unsigned int i = 0; i < viewItemVec.size(); ++i)
	{
		vector<TeQtThemeItem*> themeItemVec = viewItemVec[i]->getThemeItemVec();
		for (unsigned int j = 0; j < themeItemVec.size(); ++j)
		{
			TeAppTheme* appTheme = themeItemVec[j]->getAppTheme();
			if(appTheme->getTheme()->type() == TeTHEME)
			{
				TeTheme* theme = (TeTheme*)appTheme->getTheme();
				if (theme->isThemeTable(tableVector[ntab].name()))
				{
					theme->layer()->loadLayerTables();
					theme->loadThemeTables();
				}
			}
		}
	}
	if (doRefresh)
		grid_->init(currentTheme_);
	new TeQtLayerItem((TeQtDatabaseItem*)(layerItem->parent()),
		layerOut->name().c_str(), layerOut);

	QString msg = tr("The new layer was created successfully!");
	QMessageBox::information(this, tr("Information"), msg);
	return;

}

void TerraViewBase::popupCreateCountLayerDatabasesListViewItem()
{
	TeQtCheckListItem *checkItem = (TeQtCheckListItem*)databasesListView_->popupItem();
	TeLayer* layer = ((TeQtLayerItem*)(checkItem))->getLayer();
	TeDatabasePortal *portal = currentDatabase_->getPortal();

	string name;
	int	k = 1;
	while(name.empty())
	{
		string kname = layer->name() + "_Count_" + Te2String(k);
		string ss = "SELECT * FROM te_layer WHERE name = '" + kname + "'";
		portal->freeResult();
		if(portal->query(ss))
		{
			if(portal->fetchRow())
				k++;
			else
				name = kname;
		}
	}
	QString layerName = name.c_str();

	string vname;
	int	kk = k;
	while(vname.empty())
	{
		string kkname = layer->name() + "_Count_" + Te2String(kk);
		QString qname = tr("View_") + kkname.c_str();
		string name = qname.latin1();
		string ss = "SELECT * FROM te_view WHERE name = '" + name + "'";
		portal->freeResult();
		if(portal->query(ss))
		{
			if(portal->fetchRow())
				kk++;

			else
				vname = name;
		}
	}
	QString viewName = vname.c_str();

	string tname;
	kk = k;
	while(tname.empty())
	{
		string kkname = layer->name() + "_Count_" + Te2String(kk);
		QString qname = tr("Theme_") + kkname.c_str();
		string name = qname.latin1();
		string ss = "SELECT * FROM te_theme WHERE name = '" + name + "'";
		portal->freeResult();
		if(portal->query(ss))
		{
			if(portal->fetchRow())
				kk++;
			else
				tname = name;
		}
	}
	QString themeName = tname.c_str();

	QString m1 = tr("Creation of the new layer:") + " " + layerName + "\n";
	m1 += tr("New view:") + " " + viewName + "\n";
	m1 += tr("New theme:") + " " + themeName + "\n";
	m1 += tr("The theme is grouped by \"Unique Value\"") + " !\n";
	m1 += tr("Do you wish to continue?");
	int response = QMessageBox::question(this, tr("Question"), m1, tr("Yes"), tr("No"));

	if (response != 0)
	{
		delete portal;
		return;
	}

	TeWaitCursor wait;
	QString aguarde = tr("Processing. Please, wait!");
	statusBar()->message(aguarde);

	TeLayer *newLayer = new TeLayer(*layer);
	newLayer->name(name);
	newLayer->id(-1);
	if(currentDatabase_->insertLayer(newLayer) == false)
	{
		wait.resetWaitCursor();
		QString msg = tr("The layer could not be created!");
		QMessageBox::critical(this, tr("Error"), msg);
		statusBar()->message("");
		delete portal;
		return;
	}

	if(newLayer->addGeometry(TePOINTS) == false)
	{
		wait.resetWaitCursor();
		QString msg = tr("The Points representation could not be created!");
		QMessageBox::critical(this, tr("Error"), msg);
		statusBar()->message("");
		currentDatabase_->deleteLayer(newLayer->id());
		delete newLayer;
		delete portal;
		return;
	}

	TeAttributeList attList;
	TeAttribute at;
	at.rep_.type_ = TeSTRING;
	at.rep_.numChar_ = 16;
	at.rep_.name_ = "object_id";
	at.rep_.isPrimaryKey_ = true;
	attList.push_back(at);

	TeAttribute att;
	att.rep_.type_ = TeINT;
	att.rep_.name_ = "object_count";
	attList.push_back(att);

	TeTable attTable (newLayer->name(), attList, "object_id", "object_id", TeAttrStatic);

	// insert the table into the database
	if (!newLayer->createAttributeTable(attTable))
	{
		wait.resetWaitCursor();
		QString msg = tr("The attribute table could not be created!");
		QMessageBox::critical(this, tr("Error"), msg);
		statusBar()->message("");
		currentDatabase_->deleteLayer(newLayer->id());
		delete newLayer;
		delete portal;
		return;
	}

	int total = 0;
	string sloop = "SELECT count(*) FROM " + layer->tableName(TePOINTS) + " GROUP BY x, y";
	string ssloop = "SELECT count(*) FROM (" + sloop + ")";

	if((currentDatabase_->dbmsName() == "PostgreSQL") || (currentDatabase_->dbmsName() == "PostGIS"))
				ssloop += " AS r ";

	portal->freeResult();
	if(portal->query(ssloop))
	{
		if(portal->fetchRow())
			total = portal->getInt(0);
	}
	total *= 5;

	TeQtProgress progress(this, "ProgressDialog", true);
	progress.setCaption(tr("Creating Count Layer").latin1());
	progress.setLabelText("       Wait...");
	progress.setTotalSteps(total);

	int	id = 0;
	string sel = "SELECT count(*), x, y FROM " + layer->tableName(TePOINTS);
	sel += " GROUP BY x, y";
	portal->freeResult();
	if(portal->query(sel))
	{		
		double	x, y;
		TePointSet points;
		while(portal->fetchRow())
		{
			id++;
			TeTableRow row;
			row.push_back(Te2String(id));
			row.push_back(portal->getData(0));
			attTable.add(row);

			x = portal->getDouble(1);
			y = portal->getDouble(2);

			TePoint point;
			TeCoord2D xy (x, y);
			point.add ( xy );
			point.objectId (Te2String(id));
			points.add ( point );

			if((id % 200) == 0)
			{
				if (!layer->saveAttributeTable( attTable ))
				{
					wait.resetWaitCursor();
					QString msg = tr("The attribute table could not be saved!");
					QMessageBox::critical(this, tr("Error"), msg);
					delete portal;
					statusBar()->message("");
					currentDatabase_->deleteLayer(newLayer->id());
					delete newLayer;
					return;
				}
				attTable.clear();

				if(!newLayer->addPoints( points ))
				{
					wait.resetWaitCursor();
					QString msg = tr("The table of points could not be saved!");
					QMessageBox::critical(this, tr("Error"), msg);
					delete portal;
					statusBar()->message("");
					currentDatabase_->deleteLayer(newLayer->id());
					delete newLayer;
					return;
				}
				points.clear();
				progress.setProgress(id);
				if(progress.wasCancelled())
				{
					wait.resetWaitCursor();
					int response = QMessageBox::question(this, tr("Stop Layer Creation"),
					 tr("Do you really want to stop?"), tr("Yes"), tr("No"));

					if (response == 0)
					{
						delete portal;
						currentDatabase_->deleteLayer(newLayer->id());
						delete newLayer;
						statusBar()->message("");
						return;
					}
				}
			}
		}
		if(id % 200)
		{
			if (!layer->saveAttributeTable( attTable ))
			{
				wait.resetWaitCursor();
				QString msg = tr("The attribute table could not be saved!");
				QMessageBox::critical(this, tr("Error"), msg);
				statusBar()->message("");
				currentDatabase_->deleteLayer(newLayer->id());
				delete newLayer;
				delete portal;
				return;
			}
			attTable.clear();

			if(!newLayer->addPoints( points ))
			{
				wait.resetWaitCursor();
				QString msg = tr("The table of points could not be saved!");
				QMessageBox::critical(this, tr("Error"), msg);
				statusBar()->message("");
				currentDatabase_->deleteLayer(newLayer->id());
				delete newLayer;
				delete portal;
				return;
			}
			points.clear();
		}
		progress.setProgress(id);
	}
	delete portal;

	// create a projection to the view the parameters of the created layer
	TeProjection *viewProjection = TeProjectionFactory::make(newLayer->projection()->params());

	// create a view and a theme on the main window for data visualization
	// add the new view to the database and insert it on the map of Views
	TeView *view = new TeView (viewName.latin1(), currentDatabase_->user());
	view->projection(viewProjection);
	TeBox b;
	view->setCurrentBox(b);
	view->setCurrentTheme(-1);
	currentDatabase_->insertView(view);
	if(currentDatabase_->projectMap().empty() == false)
	{
		TeProjectMap& pm = currentDatabase_->projectMap();
		TeProject* project = pm.begin()->second;
		project->addView(view->id());
	}
	currentDatabase_->insertProjectViewRel(1, view->id());
	checkWidgetEnabling();
	
	// create the view item and set it as the current one
	TeQtViewItem* viewItem = new TeQtViewItem(viewsListView_, viewName, view);
	progress.setProgress(id*2);

	// create the theme that will be associated to this view
	new TeQtLayerItem(databasesListView_->currentDatabaseItem(),
		newLayer->name().c_str(), newLayer);
	progress.setProgress(id*3);
	
	TeAttrTableVector ATV;
	newLayer->getAttrTables(ATV, TeAttrStatic);
	if(createTheme(themeName.latin1(), view, newLayer, "", ATV) == false)
	{
		wait.resetWaitCursor();
		QString msg = tr("The theme") + " \"" + themeName + "\" " + tr("could not be inserted into the view") + " \"" + view->name().c_str() + "\" !";
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}

	vector<TeQtThemeItem*> themeItemVec = viewItem->getThemeItemVec();
	TeAppTheme* newTheme = themeItemVec[0]->getAppTheme();
	progress.setProgress(id*4);

	createCountLegends(newTheme, attTable.name(), "object_count");
	grid_->clear();
	QListViewItem* item = (QListViewItem*)viewsListView_->getThemeItem(newTheme);
	viewsListView_->setOpen(viewItem, true);
	viewsListView_->setOn((QCheckListItem*)item, true);
	viewsListView_->selectItem(item);

	drawAction_activated();
	progress.setProgress(id*5);
	progress.reset();

	statusBar()->message("");
	wait.resetWaitCursor();
	QString m2 = tr("Layer:") + " " + layerName + "\n";
	m2 += tr("View:") + " " + viewName + "\n";
	m2 += tr("Theme:") + " " + themeName + "\n";
	m2 += tr("were created successfully and the theme was grouped by \"Unique Value\" !") + "\n";
	QMessageBox::information(this, tr("Information"), m2);
}

void TerraViewBase::popupExportTableSlot()
{
	SaveAsTable* expTable = new SaveAsTable(this, "savaAsTable", true);
	expTable->exec();
	delete expTable;
}

void TerraViewBase::popupExportTableAsTxtSlot()
{
	SaveTableAsTxt* expTable = new SaveTableAsTxt(this, "savaTableAsTxt", true);
	expTable->exec();
	delete expTable;
}

void TerraViewBase::changeCurrentThemeSlot( TeAppTheme * curTheme )
{
     QListViewItem* item = (QListViewItem*)viewsListView_->getThemeItem(curTheme);
     viewsListView_->setOn((QCheckListItem*)item, true);
     viewsListView_->selectItem(item);
}

void TerraViewBase::mountLayerRepMap( TeLayer * layer )
{
	map<int, RepMap>::iterator it = layerId2RepMap_.find(layer->id());

	if(it != layerId2RepMap_.end())
		return;

	TeDatabasePortal* portal = currentDatabase_->getPortal();
	struct	RepMap rm;
	layerId2RepMap_[layer->id()] = rm;
	struct	RepMap& rmr = layerId2RepMap_[layer->id()];

	if(layer->hasGeometry(TePOLYGONS))
	{
		string query  = "SELECT * FROM " + layer->tableName(TePOLYGONS);

		if((currentDatabase_->dbmsName() != "OracleSpatial") && (currentDatabase_->dbmsName() != "PostGIS"))
			query += " ORDER BY object_id ASC, parent_id ASC, num_holes DESC";
		else
			query += " ORDER BY object_id ASC";

		//Plot the polygons whose number was placed in nSteps
		if (portal->query(query) == false)
		{
			delete portal;
			return;
		}

		if(portal->fetchRow())
		{
			bool flag = true;
			do
			{
				TePolygon poly;
				int geomId = portal->getInt(0);
				rmr.polygonMap_[geomId] = poly;
				flag = portal->fetchGeometry(rmr.polygonMap_[geomId]);
			}
			while (flag);
		}
	}
	if (layer->hasGeometry(TeLINES))
	{
		string query  = "SELECT * FROM " + layer->tableName(TeLINES);

		//Plot the polygons whose number was placed in nSteps
		if (portal->query(query) == false)
		{
			delete portal;
			return;
		}

		if(portal->fetchRow())
		{
			bool flag = true;
			do
			{
				TeLine2D line;
				int geomId = portal->getInt(0);
				rmr.lineMap_[geomId] = line;
				flag = portal->fetchGeometry(rmr.lineMap_[geomId]);
			}
			while (flag);
		}
	}
	delete portal;
}


void TerraViewBase::clearGridInformationSlot()
{
	statusBar()->message("");
}

void TerraViewBase::displayGridInformationSlot(QString info)
{
	statusBar()->message(info);
}



void TerraViewBase::popupCalculateAreaSlot()
{
	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	if(curTheme->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	TeWaitCursor wait;
	TeDatabase* db = curTheme->layer()->database();
	TeDatabasePortal* portal = db->getPortal();

	TeAttrTableVector attrTableVector; 
	curTheme->getAttTables(attrTableVector);
	TeTable& table = attrTableVector[0];
	string aTable = table.name();
	string glink = table.linkName();

	string gTable = curTheme->layer()->tableName(TePOLYGONS);

	string queryPolygons  = "SELECT " + gTable + ".*";
	queryPolygons += " FROM " + gTable + ", " + aTable;
	queryPolygons += " WHERE object_id = " + glink;

	if(db->dbmsName() != "OracleSpatial" && (currentDatabase_->dbmsName() != "PostGIS"))
		queryPolygons += " ORDER BY object_id ASC, parent_id ASC, num_holes DESC";
	else
		queryPolygons += " ORDER BY object_id ASC";

	//Get the number of polygons to be calculated
	string selectPolyCount; 
	if(db->dbmsName() != "OracleSpatial" && (currentDatabase_->dbmsName() != "PostGIS"))
		selectPolyCount = "SELECT DISTINCT parent_id FROM ";
	else
		selectPolyCount = "SELECT DISTINCT object_id FROM ";

	selectPolyCount += gTable + ", " + aTable + " WHERE object_id = " + glink;

	int nSteps;
	string polyCount;
	if (db->dbmsName() != "MySQL")
		polyCount = "SELECT COUNT(*) FROM (" + selectPolyCount + ") as r ";
	else
	{
		string createTempTable = "CREATE TEMPORARY TABLE count_table " + selectPolyCount;
		db->execute(createTempTable);
		polyCount = "SELECT COUNT(*) FROM count_table";
	}
	
	if (portal->query(polyCount))
	{
		if (portal->fetchRow())
			nSteps = portal->getInt(0);
	}
	else
	{
		delete portal;
		return;
	}

	if (db->dbmsName() == "MySQL")
		db->execute("DROP TABLE count_table");

	portal->freeResult();
	grid_->clear();

	int precision;	
	TeProjection* viewProjection = 0;
	bool doRemap = false;
	if (currentView_)
	{
		viewProjection = currentView_->projection();
		if (viewProjection && curTheme->layer()->projection() &&
			!(viewProjection == curTheme->layer()->projection()))
			doRemap = true;
	}
	if ((doRemap && viewProjection->units() == "DecimalDegrees") ||
		 (curTheme->layer()->projection()->units() == "DecimalDegrees"))
		precision = 6;
	else
	{
		TeBox box = curTheme->layer()->box();
		double diag = sqrt(box.width()*box.width() + box.height()*box.height());
		double logd = log10(diag);
		precision = int(logd);
		if(precision > 4)
			precision = 1;
		else if (precision >= 0)
			precision = 4 - precision;
		else
			precision = -precision + 4;
		precision /= 2;
		if(precision == 0)
			precision = 1;
	}

	TeAttributeRep atrep;
	atrep.type_ = TeREAL;
	atrep.name_ = "AREA";
	atrep.decimals_ = precision;
	if(db->addColumn(aTable, atrep) == false)
	{
		wait.resetWaitCursor();
		QString msg = tr("The \"AREA\" column could not be appended!") + "\n";
		msg += db->errorMessage().c_str();		
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}

	//Plot the polygons whose number was placed in nSteps
	if (portal->query(queryPolygons) == false)
	{
		delete portal;
		return;
	}

	int dt = CLOCKS_PER_SEC/4;
	int dt2 = CLOCKS_PER_SEC * 5;
	clock_t	t0, t1, t2;
	string caption = tr("Calculating the \"AREA\" values!").latin1();
	progress_->setCaption(caption.c_str());
	string text = tr("Please, wait!").latin1();
	progress_->setLabelText(text.c_str());
	progress_->setTotalSteps(nSteps);
	t2 = clock();
	t0 = t1 = t2;
	int	nLoops = 0;
	string objIdSt;
	string up;

	if(portal->fetchRow())
	{
		double area = 0;
		bool flag;
		flag = true;
		do
		{
			string objId = portal->getData(1);
			if(objIdSt.empty())
				objIdSt = objId;

			TePolygon poly;
			flag = portal->fetchGeometry(poly);
			t2 = clock();
			nLoops++;
			if (int(t2-t1) > dt)
			{
				t1 = t2;
				if(progress_->wasCancelled())
				{
					wait.resetWaitCursor();
					QString msg = tr("The values for the \"AREA\" column were partially calculated!") + "\n";
					QMessageBox::warning(this, tr("Warning"), msg);
					break;
				}
				if((int)(t2-t0) > dt2)
					progress_->setProgress(nLoops);
			}
			
			if(objIdSt == objId)
			{
				if (doRemap)
				{
					TePolygon pol2;
					TeVectorRemap(poly,curTheme->layer()->projection(),pol2,currentView_->projection());
					area += TeGeometryArea(pol2);
				}
				else
					area += TeGeometryArea(poly);
			}
			else
			{
				up = "UPDATE " + aTable + " SET AREA = " + Te2String(area, precision);
				up += " WHERE " + glink + " = '" + objIdSt + "'";
				db->execute(up);
				objIdSt = objId;
				if (doRemap)
				{
					TePolygon pol2;
					TeVectorRemap(poly,curTheme->layer()->projection(),pol2,currentView_->projection());
					area = TeGeometryArea(pol2);
				}
				else
					area = TeGeometryArea(poly);
			}
		}
		while (flag);

		if(objIdSt.empty() == false)
		{
			up = "UPDATE " + aTable + " SET AREA = " + Te2String(area, precision);
			up += " WHERE " + glink + " = '" + objIdSt + "'";
			db->execute(up);
		}
	}
	progress_->reset();
	delete portal;

	// Update all the themes that uses this table
	vector<TeQtViewItem*> viewItemVec = viewsListView_->getViewItemVec();
	for (unsigned int i = 0; i < viewItemVec.size(); ++i)
	{
		vector<TeQtThemeItem*> themeItemVec = viewItemVec[i]->getThemeItemVec();
		for (unsigned int j = 0; j < themeItemVec.size(); ++j)
		{
			TeAppTheme* appTheme = themeItemVec[j]->getAppTheme();
			TeTheme* theme = (TeTheme*)appTheme->getTheme();
			if(theme->type() == TeTHEME)
			{
				if (theme->isThemeTable(aTable) == true)
				{
					theme->layer()->loadLayerTables();
					theme->loadThemeTables();
				}
			}
		}
	}

	grid_->init(currentTheme_);

	wait.resetWaitCursor();
	QString msg = tr("The area of the objects was calculated in the column \"AREA\". \nThe values are in squared units of the view projection.") + "\n";
	QMessageBox::information(this, tr("Information"), msg);
}


void TerraViewBase::popupCalculatePerimeterSlot()
{
	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	if(curTheme->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	TeWaitCursor wait;

	TeDatabase* db = curTheme->layer()->database();
	TeDatabasePortal* portal = db->getPortal();

	TeAttrTableVector attrTableVector; 
	curTheme->getAttTables(attrTableVector);
	TeTable& table = attrTableVector[0];
	string aTable = table.name();
	string glink = table.linkName();

	int precision;	
	TeProjection* viewProjection = 0;
	bool doRemap = false;
	if (currentView_)
	{
		viewProjection = currentView_->projection();
		if (viewProjection && curTheme->layer()->projection() &&
			!(viewProjection == curTheme->layer()->projection()))
			doRemap = true;
	}
	if ((doRemap && viewProjection->units() == "DecimalDegrees") ||
		 (curTheme->layer()->projection()->units() == "DecimalDegrees"))
		precision = 6;
	else
	{
		TeBox box = curTheme->layer()->box();
		double diag = sqrt(box.width()*box.width() + box.height()*box.height());
		double logd = log10(diag);
		precision = int(logd);
		if(precision > 4)
			precision = 1;
		else if (precision >= 0)
			precision = 4 - precision;
		else
			precision = -precision + 4;
		precision /= 2;
		if(precision == 0)
			precision = 1;
	}

	if(curTheme->layer()->hasGeometry(TePOLYGONS))
	{
		string gTable = curTheme->layer()->tableName(TePOLYGONS);

		string queryPolygons  = "SELECT " + gTable + ".*";
		queryPolygons += " FROM " + gTable + ", " + aTable;
		queryPolygons += " WHERE object_id = " + glink;

		if(db->dbmsName() != "OracleSpatial" && (currentDatabase_->dbmsName() != "PostGIS"))
			queryPolygons += " ORDER BY object_id ASC, parent_id ASC, num_holes DESC";
		else
			queryPolygons += " ORDER BY object_id ASC";

		//Get the number of polygons to be calculated
		string selectPolyCount; 
		if(db->dbmsName() != "OracleSpatial" && (currentDatabase_->dbmsName() != "PostGIS"))
			selectPolyCount = "SELECT DISTINCT parent_id FROM ";
		else
			selectPolyCount = "SELECT DISTINCT object_id FROM ";

		selectPolyCount += gTable + ", " + aTable + " WHERE object_id = " + glink;

		int nSteps;
		string polyCount;
		if (db->dbmsName() != "MySQL")
			polyCount = "SELECT COUNT(*) FROM (" + selectPolyCount + ") AS r ";
		else
		{
			string createTempTable = "CREATE TEMPORARY TABLE count_table " + selectPolyCount;
			db->execute(createTempTable);
			polyCount = "SELECT COUNT(*) FROM count_table";
		}
		
		if (portal->query(polyCount))
		{
			if (portal->fetchRow())
				nSteps = portal->getInt(0);
		}
		else
		{
			delete portal;
			return;
		}

		if (db->dbmsName() == "MySQL")
			db->execute("DROP TABLE count_table");

		portal->freeResult();
		grid_->clear();

		TeAttributeRep atrep;
		atrep.type_ = TeREAL;
		atrep.name_ = "PERIMETRO";
		atrep.decimals_ = precision;
		
		if(db->addColumn(aTable, atrep) == false)
		{
			wait.resetWaitCursor();
			QString msg = tr("The \"PERIMETRO\" column could not be appended!") + "\n";
			msg += db->errorMessage().c_str();		
			QMessageBox::critical(this, tr("Error"), msg);
			return;
		}

		//Calculate the polygons PERIMETER whose number was placed in nSteps
		if (portal->query(queryPolygons) == false)
		{
			delete portal;
			return;
		}

		int dt = CLOCKS_PER_SEC/4;
		int dt2 = CLOCKS_PER_SEC * 5;
		clock_t	t0, t1, t2;
		QString caption = tr("Calculating Polygons Length!");
		progress_->setCaption(caption.latin1());
		QString text = tr("Please, wait!");
		progress_->setLabelText(text.latin1());
		progress_->setTotalSteps(nSteps);
		t2 = clock();
		t0 = t1 = t2;
		int	nLoops = 0;
		string objIdSt;
		string up;

		if(portal->fetchRow())
		{
			double perimetro = 0;
			bool flag;
			flag = true;
			do
			{
				string objId = portal->getData(1);
				if(objIdSt.empty())
					objIdSt = objId;

				TePolygon poly;
				flag = portal->fetchGeometry(poly);
				t2 = clock();
				nLoops++;
				if (int(t2-t1) > dt)
				{
					t1 = t2;
					if(progress_->wasCancelled())
					{
						wait.resetWaitCursor();
						QString msg = tr("The values for the \"PERIMETRO\" column were partially calculated!") + "\n";
						QMessageBox::warning(this, tr("Warning"), msg);
						break;
					}
					if((int)(t2-t0) > dt2)
						progress_->setProgress(nLoops);

				}
				
				if(objIdSt == objId)
				{
					TeLine2D line = poly[0];
					if (doRemap)
					{
						TeLine2D line2;
						TeVectorRemap(line,curTheme->layer()->projection(),line2,currentView_->projection());
						perimetro += TeLength(line2);
					}
					else
						perimetro += TeLength(line);
				}
				else
				{
					up = "UPDATE " + aTable + " SET PERIMETRO = " + Te2String(perimetro, precision);
					up += " WHERE " + glink + " = '" + objIdSt + "'";
					db->execute(up);
					objIdSt = objId;
					TeLine2D line = poly[0];
					if (doRemap)
					{
						TeLine2D line2;
						TeVectorRemap(line,curTheme->layer()->projection(),line2,currentView_->projection());
						perimetro = TeLength(line2);
					}
					else
						perimetro = TeLength(line);
				}
			}
			while (flag);

			if(objIdSt.empty() == false)
			{
				up = "UPDATE " + aTable + " SET PERIMETRO = " + Te2String(perimetro, precision);
				up += " WHERE " + glink + " = '" + objIdSt + "'";
				db->execute(up);
			}
		}
		progress_->reset();
	}

	if(curTheme->layer()->hasGeometry(TeLINES))
	{
		string gTable = curTheme->layer()->tableName(TeLINES);

		string queryLines  = "SELECT " + gTable + ".*";
		queryLines += " FROM " + gTable + ", " + aTable;
		queryLines += " WHERE object_id = " + glink;

		//Get the number of lines to be calculated
		string linesCount = "SELECT COUNT(*) FROM " + gTable + ", " + aTable;
		linesCount += " WHERE object_id = " + glink;

		int nSteps;
		portal->freeResult();
		if(grid_->getPortal())
		{
			grid_->clear();

			TeAttributeRep atrep;
			atrep.type_ = TeREAL;
			atrep.name_ = "PERIMETRO";
			if(db->addColumn(aTable, atrep) == false)
			{
				wait.resetWaitCursor();
				QString msg = tr("The \"PERIMETRO\" column could not be appended!") + "\n";
				msg += db->errorMessage().c_str();		
				QMessageBox::critical(this, tr("Error"), msg);
				return;
			}
		}

		//Plot the lines whose number was placed in nSteps
		portal->freeResult();
		if (portal->query(linesCount))
		{
			if (portal->fetchRow())
				nSteps = portal->getInt(0);
		}
		else
		{
			delete portal;
			return;
		}

		//Calculate the lines PERIMETER whose number was placed in nSteps
		portal->freeResult();
		if (portal->query(queryLines) == false)
		{
			delete portal;
			return;
		}

		int dt = CLOCKS_PER_SEC/4;
		int dt2 = CLOCKS_PER_SEC * 5;
		clock_t	t0, t1, t2;
		progress_->setCaption(tr("Calculating Lines Length!").latin1());
		progress_->setLabelText(tr("Please, wait!").latin1());
		progress_->setTotalSteps(nSteps);
		t2 = clock();
		t0 = t1 = t2;
		int	nLoops = 0;
		string objIdSt;
		string up;

		if(portal->fetchRow())
		{
			objIdSt.clear();
			double perimetro = 0;
			bool flag;
			flag = true;
			do
			{
				string objId = portal->getData(1);
				if(objIdSt.empty())
					objIdSt = objId;

				TeLine2D line;
				flag = portal->fetchGeometry(line);
				t2 = clock();
				nLoops++;
				if (int(t2-t1) > dt)
				{
					t1 = t2;
					if(progress_->wasCancelled())
					{
						wait.resetWaitCursor();
						QString msg = tr("The values for the \"PERIMETRO\" column were partially calculated!") + "\n";
						QMessageBox::warning(this, tr("Warning"), msg);
						break;
					}
					if((int)(t2-t0) > dt2)
						progress_->setProgress(nLoops);
				}
				
				if(objIdSt == objId)
				{
					if (doRemap)
					{
						TeLine2D line2;
						TeVectorRemap(line,curTheme->layer()->projection(),line2,currentView_->projection());
						perimetro += TeLength(line2);
					}
					else
						perimetro += TeLength(line);
				}
				else
				{
					up = "UPDATE " + aTable + " SET PERIMETRO = PERIMETRO + " + Te2String(perimetro, 3);
					up += " WHERE " + glink + " = '" + objIdSt + "' AND PERIMETRO is not null";
					db->execute(up);
					up = "UPDATE " + aTable + " SET PERIMETRO = " + Te2String(perimetro, precision);
					up += " WHERE " + glink + " = '" + objIdSt + "' AND PERIMETRO is null";
					db->execute(up);
					objIdSt = objId;
					if (doRemap)
					{
						TeLine2D line2;
						TeVectorRemap(line,curTheme->layer()->projection(),line2,currentView_->projection());
						perimetro = TeLength(line2);
					}
					else
						perimetro = TeLength(line);
				}
			}
			while (flag);

			if(objIdSt.empty() == false)
			{
				up = "UPDATE " + aTable + " SET PERIMETRO = PERIMETRO + " + Te2String(perimetro, 3);
				up += " WHERE " + glink + " = '" + objIdSt + "' AND PERIMETRO is not null";
				db->execute(up);
				up = "UPDATE " + aTable + " SET PERIMETRO = " + Te2String(perimetro, precision);
				up += " WHERE " + glink + " = '" + objIdSt + "' AND PERIMETRO is null";
				db->execute(up);
			}
		}
		progress_->reset();
	}
	delete portal;

	// Update all the themes that uses this table
	vector<TeQtViewItem*> viewItemVec = viewsListView_->getViewItemVec();
	for (unsigned int i = 0; i < viewItemVec.size(); ++i)
	{
		vector<TeQtThemeItem*> themeItemVec = viewItemVec[i]->getThemeItemVec();
		for (unsigned int j = 0; j < themeItemVec.size(); ++j)
		{
			TeAppTheme* appTheme = themeItemVec[j]->getAppTheme();
			TeTheme* theme = (TeTheme*)appTheme->getTheme();
			if(theme->type() == TeTHEME)
			{
				if (theme->isThemeTable(aTable) == true)
				{
					theme->layer()->loadLayerTables();
					theme->loadThemeTables();
				}
			}
		}
	}

	wait.resetWaitCursor();
	grid_->init(currentTheme_);
	QString msg = tr("The perimeter of the objects was calculated in the column \"PERIMETRO\". \nThe values are in units of the view projection.") + "\n";
	QMessageBox::information(this, tr("Information"), msg);
}


void TerraViewBase::popupCalculateCountSlot(int item)
{
	CountComputing *counting = new CountComputing(this, "CountComputing", true);
	counting->init(currentTheme_);
	counting->exec();
	delete counting;
	if(countingPrefix_.empty())
		return;

	TeWaitCursor wait;

	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	TeDatabase* db = curTheme->layer()->database();
	TeAttrTableVector attrTableVector; 
	curTheme->getAttTables(attrTableVector);
	TeTable& table = attrTableVector[0];
	string aTable = table.name();
	string linkName;
	string link = table.linkName();
//	if(link.find(".", string::npos) == string::npos)
	if(link.find(".") == string::npos)
		linkName = table.name() + "." + link;
	else
		linkName = link;

	string CT = curTheme->collectionTable();
	string CAT = curTheme->collectionAuxTable();
	string sql = curTheme->sqlGridFrom();
	string uniqueExt;

	unsigned int i;
	for(i=0; i<attrTableVector.size(); i++)
	{
		TeTable t = attrTableVector[i];
		if(t.tableType() == TeAttrExternal)
		{
			string lname = t.linkName();
//			if(lname.find(".", string::npos) == string::npos)
			if(lname.find(".") == string::npos)
				uniqueExt = t.name() + "." + lname;
			else
				uniqueExt = lname;
			break;
		}
	}

	grid_->clear();

	TeAttributeRep atrep;
	atrep.type_ = TeINT;
	atrep.name_ = countingPrefix_ + "_COUNT";

	if(db->addColumn(aTable, atrep) == false)
	{
		wait.resetWaitCursor();
		QString colName = atrep.name_.c_str();
		QString msg = tr("The column") + " \"" + tr(colName) + "\" " + tr("could not be appended!") + "\n";
		msg += db->errorMessage().c_str();		
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}

	// create temporary table
	string ttable;
	string user = db->user();
	if(user.empty())
		ttable = "Temp" + Te2String(curTheme->id());
	else
		ttable = db->user() + "_Temp";

	string drop = "DROP TABLE " + ttable;
	if(db->tableExist(ttable))
		db->execute(drop);

	TeAttributeList atl;
	TeAttribute		at;

	at.rep_.name_ = "id";
	at.rep_.type_ = TeSTRING;
	at.rep_.numChar_ = 50;
	at.rep_.isPrimaryKey_ = true;
	atl.push_back(at);

	at.rep_.isPrimaryKey_ = false;
	at.rep_.name_ = "conta";
	at.rep_.type_ = TeINT;
	atl.push_back(at);

	db->createTable(ttable, atl);

	string sel = "SELECT MIN(" + CT + ".c_object_id) AS cid, COUNT(*) AS contagem ";
	sel += sql + " WHERE " + uniqueExt + " is not null";
	if(item == 1) // Selected by Query
		sel += " AND (" + CAT + ".grid_status = 2 OR " + CAT + ".grid_status = 3)";
	else if(item == 1) // Selected by Pointing
		sel += " AND (" + CAT + ".grid_status = 1 OR " + CAT + ".grid_status = 3)";
	else if(item == 2) // Not Selected by Query
		sel += " AND (" + CAT + ".grid_status = 1 OR " + CAT + ".grid_status = 3)";
	else if(item == 3) // Not Selected by Query
		sel += " AND (" + CAT + ".grid_status < 2)";
	else if(item == 4) // Not Selected by Pointing
		sel += " AND (" + CAT + ".grid_status == 0 OR " + CAT + ".grid_status = 2)";
	else if(item == 5) // Selected by Query or Pointing
		sel += " AND (" + CAT + ".grid_status > 0)";
	else if(item == 6) // Selected by Query or Pointing
		sel += " AND (" + CAT + ".grid_status = 3)";
	sel += " GROUP BY " + CT + ".c_object_id";
	string ins = "INSERT INTO " + ttable + " (id, conta) " + sel;
	db->execute(ins);

	string up = "UPDATE (SELECT " + atrep.name_ + ", " + ttable + ".conta";
	up += " FROM " + aTable + ", " + ttable + " WHERE ";
	up += linkName + " = " + ttable + ".id) SET ";
	up += atrep.name_ + " = " + ttable + ".conta";
	if(db->execute(up) == false) // mysql case
	{
		string s = "SELECT " + linkName  + ", " + ttable + ".conta";
		s += " FROM " + aTable + ", " + ttable + " WHERE ";
		s += linkName + " = " + ttable + ".id";

		map<int, set<string> > countMap;
		TeDatabasePortal* portal = db->getPortal();

		if(portal->query(s))
		{
			while(portal->fetchRow())
			{
				string id = portal->getData(0);
				int c = portal->getInt(1);
				if(countMap.find(c) != countMap.end())
				{
					set<string>& idSet = countMap[c];
					idSet.insert(id);
				}
				else
				{
					set<string> idSet;
					idSet.insert(id);
					countMap[c] = idSet;
				}
			}
		}
		delete portal;

		map<int, set<string> > :: iterator it = countMap.begin();
		while(it != countMap.end())
		{
			int c = it->first;
			set<string>& idSet = it->second;

			set<string>::iterator itB = idSet.begin();
			set<string>::iterator itE = idSet.end();
			vector<string> svec = generateInClauses(itB,itE, db);

			int j = 0;
			vector<string>::iterator sit = svec.begin();
			while(sit != svec.end())
			{
				up = "UPDATE " + aTable + " SET " + atrep.name_ + " = " + Te2String(c);
				up += " WHERE " + linkName + " IN " + svec[j];
				db->execute(up);
				j++;
				sit++;
			}
			it++;
		}
	}

	up = "UPDATE " + aTable + " SET " + atrep.name_ + " = 0 ";
	up += "WHERE " + atrep.name_ + " is null";
	db->execute(up);

	db->execute(drop);

	// Update all the themes that uses this table
	set<TeLayer*> layerSet;
	vector<TeQtViewItem*> viewItemVec = viewsListView_->getViewItemVec();
	for (unsigned int i = 0; i < viewItemVec.size(); ++i)
	{
		vector<TeQtThemeItem*> themeItemVec = viewItemVec[i]->getThemeItemVec();
		for (unsigned int j = 0; j < themeItemVec.size(); ++j)
		{
			TeAppTheme* appTheme = themeItemVec[j]->getAppTheme();
			TeTheme* theme = (TeTheme*)appTheme->getTheme();
			if(theme->type() == TeTHEME)
			{
				if (theme->isThemeTable(aTable) == true)
				{
					theme->loadThemeTables();
					layerSet.insert(theme->layer());
				}
			}
		}
	}

	// Update the layer tables affected
	set<TeLayer*>::iterator setIt;
	for (setIt = layerSet.begin(); setIt != layerSet.end(); ++setIt)
		(*setIt)->loadLayerTables();

	grid_->init(currentTheme_);
	wait.resetWaitCursor();
}

void TerraViewBase::helpManualAction_activated()
{
//	QDir dir;
//	string dirname = dir.absPath().latin1();
//	string dirfile;
//
//	int f = dirname.find("\\");
//	if(f >= 0)
//		dirfile = dirname + "\\" + "manual\\manual.html";
//	else
//		dirfile = dirname + "/" + "manual/manual.html";
//
//	TeQtShowMedia manual;
//	manual.slotShowFile(dirfile);
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("terraViewBase.htm");
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

void TerraViewBase::faqAction_activated() 
{
	if(help_)
		delete help_;

	help_ = new Help(this, "FAQ", false);
	help_->setCaption(tr("FAQ"));
	help_->init("FAQ_indice.htm");	
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
	this->raise();
}

void TerraViewBase::popupCreateLayerFromTheme()
{
	if(currentTheme_ == NULL)
	{
		QMessageBox::information(this, tr("Information"), tr("There is not a selecetd theme. Select one first!"));
		return;
	}

	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	if(!(curTheme->type() == TeTHEME || curTheme->type() == TeEXTERNALTHEME))
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));
		return;
	}

	TeQtThemeItem *popupThemeItem = (TeQtThemeItem*)viewsListView_->popupItem();
	CreateLayerFromTheme *createLayer = new CreateLayerFromTheme(this, "createLayer", true);
	createLayer->setParams(currentDatabase_);
	createLayer->init(popupThemeItem);
	if (createLayer->exec() == QDialog::Accepted)
	{
		TeLayer* newLayer = createLayer->getLayer();
		TeQtDatabaseItem *databaseItem = getDatabasesListView()->currentDatabaseItem();
		TeQtLayerItem *layerItem = new TeQtLayerItem((QListViewItem *)databaseItem, newLayer->name().c_str(), newLayer);
		layerItem->setEnabled(true);
		if (getDatabasesListView()->isOpen(layerItem->parent()) == false)
			getDatabasesListView()->setOpen(layerItem->parent(),true);
			this->checkWidgetEnabling();
		QMessageBox::information(this, tr("Information"), tr("The layer was created successfully!"));
	}
	delete createLayer;
}


void TerraViewBase::popupImageContrast()
{
	TeQtThemeItem *popupThemeItem = (TeQtThemeItem*)viewsListView_->popupItem();
	TeAppTheme* appTheme = popupThemeItem->getAppTheme();
	TeTheme* theme = (TeTheme*)appTheme->getTheme();

	if (theme->layer()->raster()->params().photometric_[0] == TeRasterParams::TePallete)
	{
		QMessageBox::warning(this, tr("Warning"), 
			         tr("It is not possible to change contrast of a pallete image."));
		return;
	}

	if (theme->grouping().groupMode_ == TeRasterSlicing)
	{
		QMessageBox::warning(this, tr("Warning"),
			         tr("There is a legend defined for this theme. Remove legend if you want to change contrast."));
		return;
	}

	ContrastWindow *contrast = new ContrastWindow(this, "contrast", true);
	contrast->exec();
	delete contrast;
}

void TerraViewBase::cursorRestore()
{
//	cursorActionGroup_selected(pointerCursorAction);
	cursorActionGroup_selected(cursorSelected_);
}


void TerraViewBase::popupShowGridInformationSlot()
{
	bool b = grid_->showGridInformation();
	grid_->showGridInformation(!b);	
}


void TerraViewBase::importRasterSimpleAction_activated()
{
	ImportRasterSimple *importRasterS = new ImportRasterSimple(this, "ImportRasterSimple", true);
	if (importRasterS->exec() == QDialog::Accepted)
	{
		TeLayer *layer = importRasterS->getLayer();
		if (layer != 0)
		{
			QString msg = tr("The raster data was imported successfully!") + "\n";
			msg += tr("Do you want to display the data?");
			int response = QMessageBox::question(this, tr("Display Raster Data"), msg, tr("Yes"), tr("No"));

			if(response == 0)
			{
				// find a valid name for a view
				QString viewName = layer->name().c_str();
				bool flag = true;
				int count = 1;
				while (flag)
				{
					TeViewMap& viewMap = currentDatabase_->viewMap();
					TeViewMap::iterator it;
					for (it = viewMap.begin(); it != viewMap.end(); ++it)
					{
						if (it->second->name() == viewName.latin1())
							break;
					}
					if (it == viewMap.end())
						flag = false;
					else
					{
						char vnumber[5];
						sprintf(vnumber,"_%d",count);
						viewName += vnumber;
						count++;
					}
				}
				
				// create a projection to the view the parameters of the imported layer
				TeProjection *viewProjection = TeProjectionFactory::make(layer->projection()->params());

				// create a view and a theme on the main window for data visualization
				// add the new view to the database and insert it on the map of Views
				TeView *view = new TeView (viewName.latin1(), currentDatabase_->user());
				view->projection(viewProjection);
				TeBox b;
				view->setCurrentBox(b);
				view->setCurrentTheme(-1);
				currentDatabase_->insertView(view);
				if(currentDatabase_->projectMap().empty() == false)
				{
					TeProjectMap& pm = currentDatabase_->projectMap();
					TeProject* project = pm.begin()->second;
					project->addView(view->id());
				}
				currentDatabase_->insertProjectViewRel(1, view->id());
				checkWidgetEnabling();
				
				// create the view item and set it as the current one
				TeQtViewItem *viewItem = new TeQtViewItem(viewsListView_, viewName, view);

				// create the theme that will be associated to this view
				TeAttrTableVector ATV;
				layer->getAttrTables(ATV, TeAttrStatic);
				QString themeName = currentDatabase_->getNewThemeName(layer->name()).c_str();
				if(createTheme(themeName.latin1(), view, layer, "", ATV) == false)
				{
					QString msg = tr("The theme") + " \"" + themeName;
					msg += "\" " + tr("could not be inserted into the view") + " \"" + view->name().c_str() + "\" !";
					QMessageBox::critical(this, tr("Error"), msg);
				}

				TeAppTheme* newTheme = viewItem->getThemeItemVec()[0]->getAppTheme();

				grid_->clear();
				QListViewItem* item = (QListViewItem*)viewsListView_->getThemeItem(newTheme);
				viewsListView_->setOpen(viewItem, true);
				viewsListView_->setOn((QCheckListItem*)item, true);
				viewsListView_->selectItem(item);

				resetAction_activated();
			}
		}
	}
	delete importRasterS;
}

void TerraViewBase::popupLayerProjection()
{
	TeQtLayerItem *layerItem = (TeQtLayerItem*)databasesListView_->popupItem();
    layerProjection(layerItem);
}

void TerraViewBase::layerProjection(TeQtLayerItem* layerItem)
{
    TeLayer* layer = layerItem->getLayer();
	TeProjection* proj = layer->projection();

	ProjectionWindow *projectionWindow = new ProjectionWindow(this, "projection", true);
	if (proj)
		projectionWindow->loadProjection(proj);
	if (projectionWindow->exec() == QDialog::Accepted)
	{
		TeProjection* newproj = projectionWindow->getProjection();
		if(!(*newproj == *proj))
		{
			newproj->id(proj->id());
			layer->setProjection(newproj);
		}
	}
	delete projectionWindow;
}


void TerraViewBase::addressGeocodingAction_activated()
{
	SpatializationWindow* w = new SpatializationWindow(this, "w", true);
	w->exec();
	delete w;
}


void TerraViewBase::popupLocateAddress()
{
	if(currentTheme_ == NULL)
	{
		QMessageBox::information(this, tr("Information"), tr("There is not a selecetd theme. Select one first!"));
		return;
	}

	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	if(curTheme->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	if(addressLocatorWindow_ == 0)
	{
		addressLocatorWindow_ = new AddressLocatorWindow(this, "addressLocatorWindow_", false);
	}
	else
	{
		if(!addressLocatorWindow_->refreshForm())
		{
			return;
		}
	}
	if(!addressLocatorWindow_->_wasInited)
	{
		return;
	}

	addressLocatorWindow_->show();
}



void TerraViewBase::popupViewConnect()
{
	ConnectView* cv = new ConnectView(this, "connect View", true);
	cv->exec();
	delete cv;
}

AddressLocatorWindow* TerraViewBase::getAddressLocatorWindow()
{
    return addressLocatorWindow_;
}


void TerraViewBase::unselectObjectsAction_activated()
{
	if (currentTheme_ == 0)
		return;

	if (currentTheme_->getTheme()->type() >= 3)
		return;
		
	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	if (curTheme->visibility())
	{
		removePointingColor();
		removeQueryColor();
	}
}


void TerraViewBase::invertSelectionAction_activated()
{
	if (currentTheme_ == 0)
		return;

	int tipo = currentTheme_->getTheme()->type();
	if (!(tipo == TeTHEME || tipo == TeEXTERNALTHEME))
		return;
		
	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	if (curTheme->visibility())
		grid_->invertSelectionSlot();
}


void TerraViewBase::origDecimalSepIsPoint(bool flag)
{
	origDecimalSepIsPoint_ = flag;
}


void TerraViewBase::popupGeoprocessingSlot( int i)
{
	switch(i)
	{
		case 0:
		{
			GeoOpAggregation w(this, "Geoprocessing Aggregation", true);
			if(w.erro_ == false)
				w.exec();
		}
		break;
		case 1:
		{
			GeoOpAdd w(this, "Geoprocessing Add", true);
			if(w.erro_ == false)
				w.exec();
		}
		break;
		case 2:
		{
			GeoOpOverlayIntersection w(this, "Geoprocessing Overlay intersection", true);
			if(w.erro_ == false)
				w.exec();
		}
		break;
		case 3:
		{
			GeoOpOverlayDifference w(this, "Geoprocessing Overlay Difference", true);
			if(w.erro_ == false)
				w.exec();
		}
		break;
		case 4:
		{
//			GeoOpOverlayUnion w(this, "Geoprocessing Overlay Union", true);
//			if(w.erro_ == false)
//				w.exec();
		}
		break;
		case 5:
		{
			GeoOpAssignDataLocationDistribute w(this, "Geoprocessing Assign Data By Location (Distribute)", true);
			if(w.erro_ == false)
				w.exec();
		}
		break;
		case 6:
		{
			GeoOpAssignDataLocationCollect w(this, "Geoprocessing Assign Data By Location (Collect)", true);
			if(w.erro_ == false)
				w.exec();
		}
		break;
		case 7:
		{
			CreateBuffersWindow w(this, "Geoprocessing Buffer Creation", true);
			if(w.erro_ == false)
				w.exec();
		}
		default:
		break;
	}

}

void TerraViewBase::semivarAction_activated()
{
	if(semivar_)
		delete semivar_;
	semivar_ = new Semivar(this, "semivariogram", false);
	if(semivar_->erro_ == false)
		semivar_->show();
}


void TerraViewBase::scaleComboBox_activated( const QString &v )
{
	double scale = v.toDouble();
	canvas_->scaleApx(scale);
	displayWindow_->plotData();	
}



void TerraViewBase::skaterAction_activated()
{
	if (currentDatabase() && currentView() && currentLayer() && currentTheme())
	{
		bool hasPol = false;
		vector<TeViewNode *>& themeVec = currentView()->themes();
		for(unsigned int i=0; i<themeVec.size(); i++)
		{
			TeAbstractTheme* t = (TeAbstractTheme*)themeVec[i];
			if(t->type() < 2 && t->visibleGeoRep() & TePOLYGONS)
			{
				hasPol = true;
				break;
			}
		}
		if (!hasPol)
		{
		QMessageBox::warning(this, tr("Warning"), tr("Current view has no themes with POLYGON representation!"));
			return;
		}
		SkaterWindow skater(this,"Skater","");
		if (!skater.exec() == QDialog::Accepted)
		skater.close();
	}
	else
	{
		QString msg;
		if(currentDatabase() == 0)
			msg = tr("Select a database!");
		else if(currentView() == 0)
			msg = tr("Select a view!");
		else if(currentTheme()  == 0)
			msg = tr("Select a theme!");
		else if(currentLayer() == 0)
			msg = tr("Select a layer!");
		QMessageBox::warning(this, tr("Warning"), msg);
	}
}


void TerraViewBase::globalEmpiricalBayesAction_activated()
{
	if (currentDatabase() && currentView() && currentLayer() && currentTheme())
	{
 	  if(currentTheme()->getTheme()->type() != TeTHEME)
	  {
		  QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));
		  return;
	  }
		BayesWindow *bayes = new BayesWindow();
		bayes->exec();
		delete bayes;
		return;
	}
	else
	{
		QString msg;
		if(currentDatabase() == 0)
			msg = tr("Select a database!");
		else if(currentView() == 0)
			msg = tr("Select a view!");
		else if(currentTheme()  == 0)
			msg = tr("Select a theme!");
		else if(currentLayer() == 0)
			msg = tr("Select a layer!");
		QMessageBox::warning(this, tr("Warning"), msg);
	}
}



void TerraViewBase::showRasterDisplayProperties()
{
/* if (currentTheme_ == NULL)
	{
		QMessageBox::information(this, tr("Information"), tr("There is not a selecetd theme. Select one first!"));
		return;
	}

	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	if (curTheme == 0)
		return;
	TeRaster *raster = curTheme->layer()->raster();
	if (raster == 0)
		return;

	TeRasterParams& par = raster->params();
	if (par.nBands() == 1)
	{
		if (par.photometric_[0] == TeRasterParams::TePallete || curTheme->grouping().groupMode_ != TeNoGrouping)
		{
			LUTRadioButton->setEnabled(true);
			LUTRadioButton->setChecked(true);
			MChannelRadioButton->setEnabled(false);
			RChannelRadioButton->setEnabled(false);
			GChannelRadioButton->setEnabled(false);
			BChannelRadioButton->setEnabled(false);
			detailsRadioButton->setEnabled(false);
		}
		else
		{
			LUTRadioButton->setEnabled(false);
			MChannelRadioButton->setChecked(true);
			MChannelRadioButton->setEnabled(true);
			RChannelRadioButton->setEnabled(true);
			GChannelRadioButton->setEnabled(true);
			BChannelRadioButton->setEnabled(true);
			detailsRadioButton->setEnabled(false);
		}
	}
	else
	{
		LUTRadioButton->setEnabled(false);
		if (par.nBands() >= 3)
			detailsRadioButton->setChecked(true);
		else
			MChannelRadioButton->setChecked(true);
		MChannelRadioButton->setEnabled(true);
		RChannelRadioButton->setEnabled(true);
		GChannelRadioButton->setEnabled(true);
		BChannelRadioButton->setEnabled(true);
		detailsRadioButton->setEnabled(true);	
	}

	curTheme->rasterVisual();
	TeRasterTransform* vis = curTheme->rasterVisual();
	if (!vis)
		return;

	TeRasterTransform::TeRasterTransfFunctions tf = vis->getTransfFunction();
	if (tf == TeRasterTransform::TeMono2Three)
	{
		MChannelRadioButton->setChecked(true);
		return;
	}

	if (tf == TeRasterTransform::TePall2Three || tf == TeRasterTransform::TeLUT2Three)
	{
		LUTRadioButton->setChecked(true);
		return;
	}

	if (tf == TeRasterTransform::TeExtractBand)
	{
		if (vis->getDestBand() == 0)
			RChannelRadioButton->setChecked(true);
		else if (vis->getDestBand() == 1)
			GChannelRadioButton->setChecked(true);
		else if (vis->getDestBand() == 2)
			BChannelRadioButton->setChecked(true);
		return;
	}
	detailsRadioButton->setChecked(true);
	*/
}




void TerraViewBase::checkParamsPassing()
{
#ifdef WIN32
	int argc = qApp->argc();	
	char** argv = qApp->argv();

	if(argc == 3)
	{
		string argName = argv[1];

		if(argName != "-db")
			return;

		string dbName = TeGetBaseName(argv[2]);

		// Open database window in hide mode and fill the correspond fields
		DatabaseWindow *database = new DatabaseWindow(this, "database", false);
		database->databaseTypeComboBox_activated("Ado");
		database->databaseNameLineEdit->setText(dbName.c_str());
		database->dbComboBox->insertItem(argv[2]);
		database->dbComboBox->setCurrentItem(0);
		database->okPushButton_clicked();
		delete database;
		
		// Do interface config
		databasesTreeAction_toggled(true);
		viewsTreeAction_toggled(true);
		checkWidgetEnabling();	
		return;
	}
	else 
	{
		if (argc != 5)
			return;
	}

	string softwareName = argv[1];

	if(softwareName != "tabwin")
		return;



	string dbName = TeGetBaseName(argv[3]);

	// Open database window in hide mode and fill the correspond fields
	DatabaseWindow *database = new DatabaseWindow(this, "database", false);
	database->databaseTypeComboBox_activated("Ado");
	database->databaseNameLineEdit->setText(dbName.c_str());
	database->dbComboBox->insertItem(argv[3]);
	database->dbComboBox->setCurrentItem(0);
	database->okPushButton_clicked();
	delete database;
	
	// Do interface config
	databasesTreeAction_toggled(true);
	viewsTreeAction_toggled(true);
	checkWidgetEnabling();

	// Get database pointer
	TeQtDatabaseItem* dbItem = databasesListView_->currentDatabaseItem();

	if(!dbItem)
		return;

	TeDatabase* db = dbItem->getDatabase();

	// Try to find the correspond layer
	string layerName = argv[4];
	layerName = TeConvertToUpperCase(layerName);

	TeLayer* defaultLayer = 0;
	
	TeLayerMap& layerMap = db->layerMap();	

	TeLayerMap::iterator it = layerMap.begin();

	while(it != layerMap.end())
	{
		if(TeConvertToUpperCase(it->second->name()) == layerName)
		{
			defaultLayer = it->second;
			break;
		}

		++it;
	}

	if(!defaultLayer)
	{
		QString msg = tr("The layer: ") + " \"" + layerName.c_str() + "\" " + tr(" could not be found into the database!");
		QMessageBox::critical( this, tr("Error"), msg);
		showTabwinHelp();
		return;
	}


	// Try to import the "DBF" file
	string tabName = TeGetBaseName(argv[2]);
	string dbfFile = argv[2];

	int a = 0;

	string newTabName = tabName;

	while(db->tableExist(newTabName))
	{
		++a;
		newTabName = tabName + Te2String(a);
	}

	TeAttributeList attL;

	try
	{
		TeReadDBFAttributeList(dbfFile, attL);
	}
	catch(...)
	{
		QString msg = tr("The file:") + " \"" + dbfFile.c_str() + "\" " + tr(" could not be found!");
		QMessageBox::critical( this, tr("Error"), msg);
		showTabwinHelp();
		return;
	}

	bool foundCode = false;

	for(unsigned int i = 0; i < attL.size(); ++i)
	{
		if(TeConvertToUpperCase(attL[i].rep_.name_) == "CODIGO")
		{
			foundCode = true;
			break;
		}
	}

	if(!foundCode)
	{
		QString msg = tr("The file:") + " \"" + dbfFile.c_str() + "\" " + tr(" doesn't have a column called CODIGO!");
		QMessageBox::critical( this, tr("Error"), msg);
		showTabwinHelp();
		return;
	}

	TeTable tabWinTab(newTabName, attL, "CODIGO", "CODIGO", TeAttrStatic);

	if(!defaultLayer->createAttributeTable(tabWinTab))
	{
		QString msg = tr("Could't create table: ") + " \"" + newTabName.c_str() + "\" " + tr("!");
		QMessageBox::critical( this, tr("Error"), msg);
		return;
	}

	int in = 0;
	while(TeReadDBFAttributes(dbfFile, tabWinTab, 100, in))
	{
		defaultLayer->saveAttributeTable(tabWinTab);
		in += tabWinTab.size();
		tabWinTab.clear();
	}

	// Try to put the correspond view ("VIEW_"+dbName) as the active one
	TeViewMap& viewMap = db->viewMap();

	dbName = TeConvertToUpperCase(dbName);

	string viewName  = "VIEW_";
	       viewName += dbName;

	TeView* v = 0;

	TeViewMap::iterator itV = viewMap.begin();

	while(itV != viewMap.end())
	{
		if(TeConvertToUpperCase(itV->second->name()) == viewName)
		{
			v = itV->second;
			break;
		}

		++itV;
	}

	if(!v)
	{
		QString msg = tr("The view") + " \"" + viewName.c_str() + "\" " + tr("could not be found into the database!");
		QMessageBox::critical( this, tr("Error"), msg);
		showTabwinHelp();
		return;
	}

	viewsListView_->selectViewItem(v);

	// Try to put the active theme as default one
	TeQtViewItem* viewItem = viewsListView_->currentViewItem();
	TeQtThemeItem* themeItem = (TeQtThemeItem*)viewItem->firstChild();
	TeQtThemeItem* foundThemeItem = 0;

	//TeTheme* foundTheme = 0;
		
	while(themeItem)
	{
		TeAppTheme* appTheme = themeItem->getAppTheme();
		TeTheme* theme = (TeTheme*)appTheme->getTheme();
		if(TeConvertToUpperCase(theme->name()) == TeConvertToUpperCase(layerName))
		{
			themeItem->setState(QCheckListItem::On);
			theme->visibility(true);
			//foundTheme = themeItem->getTheme();
			foundThemeItem = themeItem;
		}
		else
		{
			themeItem->setState(QCheckListItem::Off);
			theme->visibility(false);
		}

		themeItem = (TeQtThemeItem*)themeItem->nextSibling();
	}

	// Try to put new table in theme and put it visible
	if(foundThemeItem)
	{
		viewsListView_->selectItem((TeQtViewItem*)foundThemeItem);

		TeAppTheme* appFoundTheme = foundThemeItem->getAppTheme();
		TeTheme* foundTheme = (TeTheme*)appFoundTheme->getTheme();
			
		foundTheme->addThemeTable(tabWinTab);

		db->updateThemeTable(foundTheme);

		getGrid()->clear();

		if(!foundTheme->createCollectionAuxTable() || !foundTheme->populateCollectionAux())
		{
			return;
		}

		getGrid()->init(appFoundTheme);
	}

	resetAction_activated();
	viewsTreeAction_toggled(true);
	checkWidgetEnabling();
#endif
}

void TerraViewBase::popupCreateThemeFromTheme()
{
	if(currentTheme_ == NULL)
	{
		QMessageBox::information(this, tr("Information"), tr("There is not a selecetd theme. Select one first!"));
		return;
	}

	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	if(curTheme->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	TeQtThemeItem *popupThemeItem = (TeQtThemeItem*)viewsListView_->popupItem();
	CreateThemeFromTheme *createTheme = new CreateThemeFromTheme(this, "createTheme", true);
	createTheme->setParams(currentDatabase_);
	createTheme->init(popupThemeItem);
	if (createTheme->exec() == QDialog::Accepted && createTheme->getNewTheme() != 0)
	{
			QMessageBox::information(this, tr("Information"), 
				tr("The theme was created successfully!"));
			
			TeAppTheme* newAppTheme = createTheme->getNewTheme();
			TeTheme* newTheme = (TeTheme*)newAppTheme->getTheme();
			// transform theme created by the function into a theme application
			// so TerraView understands it
			populateAppThemeTable(currentDatabase());
			popupThemeItem->setOn(false);
			TeQtThemeItem* themeItem = new TeQtThemeItem (viewsListView_->getViewItem(currentView_), newTheme->name().c_str(),newAppTheme);
			themeItem->setOn(true);
			grid_->clear();
			QListViewItem* item = (QListViewItem*)viewsListView_->getThemeItem(newAppTheme);
			viewsListView_->selectItem(item);
			resetAction_activated();
	}
	delete createTheme;
}

void TerraViewBase::localEmpiricalBayesAction_actived()
{
	if (currentDatabase() && currentView() && currentLayer() && currentTheme())
	{
    if(currentTheme()->getTheme()->type() != TeTHEME)
	  {
		  QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		  return;
	  }
		LocalBayesWindow *bayes = new LocalBayesWindow();
		bayes->exec();
		delete bayes;
		return;
	}
	else
	{
		QString msg;
		if(currentDatabase() == 0)
			msg = tr("Select a database!");
		else if(currentView() == 0)
			msg = tr("Select a view!");
		else if(currentTheme()  == 0)
			msg = tr("Select a theme!");
		else if(currentLayer() == 0)
			msg = tr("Select a layer!");
		
		QMessageBox::warning(this, tr("Warning"), msg);
	}
}


void TerraViewBase::kernelRatioAction_actived()
{

  kernelEnableControl(false); // to disable widgets


	bool res;
 	if (kernelRatio_ == NULL) {
   	 kernelRatio_ = new KernelRatioWindow(this, "kernelRatio", false);
   	 res = kernelRatio_->loadView(true);
	}
  else {
    		res = kernelRatio_->loadView(false);
	}
	if(res)
		kernelRatio_->show();
	else
		kernelEnableControl(true); // to enable widgets
}


void TerraViewBase::popupDatabaseRefreshSlot()
{
	TeWaitCursor wait;

	string userName = currentDatabase_->user();
	string hostName = currentDatabase_->host();
	string password = currentDatabase_->password();
//	int	portNumber = currentDatabase_->portNumber();
	string dbType;
	string dbmsName = currentDatabase_->dbmsName();
	if(dbmsName == "Ado")
		dbType = "Access";
	else if(dbmsName == "MySQL")
		dbType = "MySQL";
	else if(dbmsName == "OracleSpatial")
		dbType = "OracleSpatial";
	else if(dbmsName == "OracleOCI")
		dbType = "OracleOCI";
	else if(dbmsName == "OracleAdo")
		dbType = "OracleAdo";
	else if(dbmsName == "PostgreSQL")
		dbType = "PostgreSQL";
	else if(dbmsName == "PostGIS")
		dbType = "PostGIS";
	else if(dbmsName == "SqlServerAdo")
		dbType = "SQL Server";
	else if(dbmsName == "Firebird")
		dbType = "Firebird";

	string dirName = currentDatabase_->databaseName();
	string dbName = TeGetBaseName (dirName.c_str());

	grid_->clear();
	canvas_->clearAll();

	//Close the database
	currentDatabase_->close();
	delete currentDatabase_;

	currentDatabase_ = 0;
	currentLayer_ = 0;
	currentView_ = 0;
	currentTheme_ = 0;

	viewsListView_->clear();
	databasesListView_->removeItem((QListViewItem*)databasesListView_->currentDatabaseItem());
	checkWidgetEnabling();

	TeDatabase* tdb = 0;
	bool status = false;
	bool integrity = true;

	//verify what is the decimal separator
#ifdef WIN32
    if (dbType == "Access")
	{
		HKEY    hk;
		DWORD	DataSize = 2;
		DWORD   Type = REG_SZ;
		char    buf[2];

		string key = "Control Panel\\International";
		string sepDecimal = "sDecimal";	
		string sepDecimalResult = "";

		if (RegOpenKeyExA(HKEY_CURRENT_USER, key.c_str(), 0, KEY_READ, &hk) == ERROR_SUCCESS)
		{
			memset (buf, 0, 2);
			DataSize = 2;
			//decimal separator
			if (RegQueryValueExA(hk, sepDecimal.c_str(), NULL, &Type, (LPBYTE)buf, &DataSize) == ERROR_SUCCESS)
				sepDecimalResult = buf;
			
			RegCloseKey (hk);
		}

		if((!sepDecimalResult.empty()) && (sepDecimalResult==","))
		{
			origDecimalSepIsPoint(false);
			if (RegOpenKeyExA(HKEY_CURRENT_USER, key.c_str(), 0, KEY_SET_VALUE, &hk) == ERROR_SUCCESS)
			{				
				memset (buf, 0, 2);
				buf[0] = '.';
				DataSize = 2;
					
				RegSetValueExA(hk, sepDecimal.c_str(), NULL, Type, (LPBYTE)buf, DataSize);  				
				RegCloseKey (hk);
			}
		}
	}
#endif

	if (dbType == "Access")
	{

		TeDatabaseFactoryParams params;
		params.dbms_name_ = "Ado";
		tdb = TeDatabaseFactory::make(params);
		status = tdb->connect(dirName);
	}
	else if(dbType =="SQL Server")
	{
#ifdef WIN32
#ifndef __GNUC__
		TeDatabaseFactoryParams params;
		params.dbms_name_ = "SQL Server";
		tdb = TeDatabaseFactory::make(params);
		status = tdb->connect(hostName,userName,password,dbName, 0);
#endif
#endif
	}
	else if (dbType == "MySQL")
	{
		TeDatabaseFactoryParams params;
		params.dbms_name_ = "MySQL";
		tdb = TeDatabaseFactory::make(params);
		status = tdb->connect(hostName,userName,password,dbName, 0);
	}
	else if (dbType == "PostgreSQL")
	{
		TeDatabaseFactoryParams params;
		params.dbms_name_ = "PostgreSQL";
		tdb = TeDatabaseFactory::make(params);
		status = tdb->connect(hostName, userName, password, dbName, 0);
	}
	else if (dbType == "PostGIS")
	{
		TeDatabaseFactoryParams params;
		params.dbms_name_ = "PostGIS";
		tdb = TeDatabaseFactory::make(params);

		status = tdb->connect(hostName, userName, password, dbName, 0);
	}
	else if (dbType == "OracleSpatial")
	{		
		TeDatabaseFactoryParams params;
		params.dbms_name_ = "OracleSpatial";
		tdb = TeDatabaseFactory::make(params);
		dbName=hostName;
		status = tdb->connect(hostName,userName,password,dbName, 0);
		integrity = true;
		
	}
	else if (dbType == "OracleOCI")
	{		
		TeDatabaseFactoryParams params;
		params.dbms_name_ = "OracleOCI";
		tdb = TeDatabaseFactory::make(params);
		dbName=hostName;
		status = tdb->connect(hostName,userName,password,dbName, 0);
		integrity = true;
	}
	else if (dbType == "OracleAdo")
	{		
                #ifdef WIN32 // This driver only work in windows plataform
						TeDatabaseFactoryParams params;
						params.dbms_name_ = "OracleAdo";
						tdb = TeDatabaseFactory::make(params);
                        status = tdb->connect(hostName,userName,password,dbName, 0);
                        integrity = true;
                #endif
	}
	else if (dbType == "Firebird")
	{
		TeDatabaseFactoryParams params;
		params.dbms_name_ = "Firebird";
		tdb = TeDatabaseFactory::make(params);
		status = tdb->connect(hostName, userName, password, dirName, 0);
	}

	if (status == false)
	{
		wait.resetWaitCursor();
		QString errorMessage = tr("Error reconnecting");
		if(tdb != NULL)
		{
			errorMessage += "\n";
			errorMessage += tdb->errorMessage().c_str();
		}

		QMessageBox::critical(this, tr("Error"), errorMessage);

		if(tdb != NULL)
		{
			tdb->close();
			delete tdb;
		}
		return;
	}

	TeQtDatabaseItem *dbItem = new TeQtDatabaseItem(databasesListView_, dbName.c_str(), tdb);

	string DBver;
	if(needUpdateDB(tdb, DBver))
	{
		wait.resetWaitCursor();
		string dbVersion = TeDBVERSION;

		if(isLowerVersion(dbVersion, DBver))
		{
			tdb->close();
			delete tdb;

			QMessageBox::warning(this, tr("Warning"), tr("Cannot connect to database because the version of Terraview is lower than the version of the database!"));
			return;
		}

		QString msg = tr("The database must be converted to the model ") + dbVersion.c_str() + "! \n";

		if(tdb->dbmsName() == "PostgreSQL" && (DBver.empty() || DBver=="3.0.1" || DBver=="3.0" || DBver=="3.0.2" || DBver=="3.1.0"))			
		{
			msg += tr("This TerraLib and TerraView version (only for PostgreSQL) will update the geometry tables (vector and raster) to use R-Tree GiST. ") + "\n";
			msg += tr("This process may take a while. Be sure to make a backup before going on with this conversion.") + "\n";
		}
		else if(tdb->dbmsName() == "PostGIS" && (DBver.empty() || DBver=="3.0.1" || DBver=="3.0" || DBver=="3.0.2" || DBver=="3.1.0"))
		{
			msg += tr("This TerraLib and TerraView version (only for PostGIS) will update the geometry tables for raster type to use R-Tree GiST. ") + "\n";
			msg += tr("This process may take a while. Be sure to make a backup before going on with this conversion.") + "\n";
		}

		msg += tr("Do you want to convert it?");
		int response = QMessageBox::question(this, tr("Database Conversion"),
			msg, tr("Yes"), tr("No"));
		if(response == 0)
		{
			string error; 
			wait.setWaitCursor();
			if(!updateDBVersion(tdb, DBver, error))
			{
				wait.resetWaitCursor();
				QString msg = tr("Fail to convert the database to the new model!") + "\n";
				msg += error.c_str();
				QMessageBox::critical(this, tr("Error"), msg);
				return;		
			}
			else
			{
				wait.resetWaitCursor();
				QString msg = tr("Database was converted successfully!") + "\n";
				msg += error.c_str();
				QMessageBox::information(this, tr("Information"), msg);
			}
		}
		else
			return;
	}

	tdb->createConceptualModel(integrity, false);
	if (!tdb->tableExist("te_theme_application"))
		createAppThemeTable(tdb,integrity);
	if (!tdb->tableExist("te_chart_params"))
		createChartParamsTable(tdb,integrity);

	//populate the te_theme_application when the theme wasnt generated by TerraView
	if(!populateAppThemeTable(tdb))
		return;

	// Load connections from database
	//TeDBConnectionsPool::instance().clear();
    TeDBConnectionsPool::instance().loadExternalDBConnections(tdb);
	
	// Get layers from the database
	if (!tdb->loadLayerSet())
	{
		wait.resetWaitCursor();
		QString msg = tr("Fail to get the layers from the database!") + "\n";
		msg += tdb->errorMessage().c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;		
	}

	// Insert layers into the tree
	TeLayerMap& layerMap = tdb->layerMap();
	TeLayerMap::iterator itlay = layerMap.begin();
	TeQtLayerItem *layerItem;
	while ( itlay != layerMap.end() )
	{
		TeLayer *layer = (*itlay).second;
		string name = layer->name();
		layerItem = new TeQtLayerItem(dbItem, name.c_str(), layer);
		if (databasesListView_->isOpen(dbItem) == false)
			databasesListView_->setOpen(dbItem,true);
		itlay++;
	}

	if (!tdb->loadViewSet(userName))
	{
		wait.resetWaitCursor();
		QString msg = tr("Fail to get the views from the database!") + "\n";
		msg += tdb->errorMessage().c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}

	tdb->projectMap().clear(); // why I need ?
	if(!tdb->loadProjectSet())
	{
		QMessageBox::critical(this, tr("Error"), tr("Fail to load project!")); 
		return;
	}

// emit signal to plugins
	PluginsSignal sigrefresh(PluginsSignal::S_SET_DATABASE);
	sigrefresh.signalData_.store(PluginsSignal::D_DATABASE, dbItem->getDatabase());
	PluginsEmitter::instance().send(sigrefresh);

	getDatabasesListView()->selectItem(dbItem);
	databasesListView_->show();
	drawAction_activated();
	wait.resetWaitCursor();
}


void TerraViewBase::about()
{

}


void TerraViewBase::popupRasterHistogramSlot()
{
	if(viewsListView_->popupItem() == NULL)
	{
		return;
	}

  TeQtThemeItem *popupThemeItem = (TeQtThemeItem*)viewsListView_->popupItem();
  TeAppTheme* appTheme = popupThemeItem->getAppTheme();
  TeTheme* theme = (TeTheme*)appTheme->getTheme();
  TeRaster* input_raster = theme->layer()->raster();

  HistogramBuildForm* hbf = 
    new HistogramBuildForm (this, "histogram", true, 0);
  hbf->setRaster( input_raster );
  hbf->exec();
}



void TerraViewBase::popuRasterTransparencySlot()
{
	RasterTransparency* rasterT = new RasterTransparency(this, "transparency", true);
	rasterT->exec();
	delete rasterT;

}

void TerraViewBase::popupInsertFileDatabaseInfo()
{
	TeQtDatabaseItem *dbItem = (TeQtDatabaseItem*)databasesListView_->popupItem();
	TeDatabase *db = dbItem->getDatabase();

	QFileDialog filed(lastOpenDir_, QString::null, 0, 0, true);
	QString qfile = filed.getOpenFileName (QString::null, QString::null, 0, 0);

	if(qfile.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("No media was selected!"));
		return;
	}

	TeWaitCursor wait;

	string file = qfile.latin1();
	lastOpenDir_ = TeGetPath(file.c_str()).c_str();
	string type = TeGetExtension(file.c_str());

	string infoTable = "tv_generalinfo";

	if(db->tableExist(infoTable)) // delete old version
	{
		TeAttribute attr;
		if(db->columnExist(infoTable, "type_", attr) == false)
			db->deleteTable(infoTable);
	}

	if(db->tableExist(infoTable) == false)
	{
		TeAttribute at;
		TeAttributeList atL;

		at.rep_.type_ = TeINT;
		at.rep_.name_ = "layer_id";
		at.rep_.isPrimaryKey_ = true;
		atL.push_back(at);

		at.rep_.isPrimaryKey_ = false;
		at.rep_.type_ = TeSTRING;
		at.rep_.numChar_ = 255;
		at.rep_.name_ = "name_";
		atL.push_back(at);

		at.rep_.type_ = TeSTRING;
		at.rep_.name_ = "type_";
		at.rep_.numChar_ = 10;
		atL.push_back(at);

		at.rep_.type_ = TeBLOB;
		at.rep_.numChar_ = 0;
		at.rep_.name_ = "media_blob";
		atL.push_back(at);

		if(db->createTable(infoTable, atL) == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"),
				tr("Fail to create the general info table!"));
			return;
		}
		string fk = "fk_" + infoTable;
		if (db->createRelation(fk, infoTable, "layer_id", "te_layer", "layer_id", true) == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"),
				tr("Fail to create relation: tv_generalinfo and te_layer!"));
			return;
		}
	}

	// insert info
	if(insertInfo2Blob(db, -1, infoTable, file, type) == false)
	{
		wait.resetWaitCursor();
		db->execute("DELETE FROM tv_generalinfo WHERE layer_id = -1");
		QMessageBox::critical(this, tr("Error"),
			tr("Fail to insert information!"));
	}
}


void TerraViewBase::popupInsertURLDatabaseInfo()
{
	TeQtDatabaseItem *dbItem = (TeQtDatabaseItem*)databasesListView_->popupItem();
	TeDatabase *db = dbItem->getDatabase();

	URLWindow urlWindow(this, "urlInfo", true);
	if (urlWindow.exec() == QDialog::Rejected)
		return;

	QString qsite = urlWindow.URLLineEdit->text();
	if(qsite.isNull() || qsite.isEmpty())
		return;

	TeWaitCursor wait;
	string site = qsite.latin1();

	string infoTable = "tv_generalinfo";

	if(db->tableExist(infoTable)) // delete old version
	{
		TeAttribute attr;
		if(db->columnExist(infoTable, "type_", attr) == false)
			db->deleteTable(infoTable);
	}

	if(db->tableExist(infoTable) == false)
	{
		TeAttribute at;
		TeAttributeList atL;

		at.rep_.type_ = TeINT;
		at.rep_.name_ = "layer_id";
		at.rep_.isPrimaryKey_ = true;
		atL.push_back(at);

		at.rep_.isPrimaryKey_ = false;
		at.rep_.type_ = TeSTRING;
		at.rep_.numChar_ = 255;
		at.rep_.name_ = "name_";
		atL.push_back(at);

		at.rep_.type_ = TeSTRING;
		at.rep_.name_ = "type_";
		at.rep_.numChar_ = 10;
		atL.push_back(at);

		at.rep_.type_ = TeBLOB;
		at.rep_.numChar_ = 0;
		at.rep_.name_ = "media_blob";
		atL.push_back(at);

		if(db->createTable(infoTable, atL) == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"),
				tr("Fail to create the general info table!"));
			return;
		}
		string fk = "fk_" + infoTable;
		if (db->createRelation(fk, infoTable, "layer_id", "te_layer", "layer_id", true) == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"),
				tr("Fail to create relation: tv_generalinfo and te_layer!"));
			return;
		}
	}

	// insert info
	if(insertInfo2Blob(db, -1, infoTable, site, "URL") == false)
	{
		wait.resetWaitCursor();
		db->execute("DELETE FROM tv_generalinfo WHERE layer_id = -1");
		QMessageBox::critical(this, tr("Error"),
			tr("Fail to insert information!"));
	}
}

void TerraViewBase::popupRetrieveDatabaseInfo()
{
	TeQtDatabaseItem *dbItem = (TeQtDatabaseItem*)databasesListView_->popupItem();
	TeDatabase *db = dbItem->getDatabase();
	string type;

	string tempName = getInfoBlob2TemporaryFile(db, -1, "tv_generalinfo", type);
	if(type == "URL") // concatenar .URL
		tempName += ".URL";

	if(tempName.empty())
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Error to get blob information!"));
		return;
	}

	TeQtShowMedia smedia;
	smedia.slotShowFile(tempName);
}

void TerraViewBase::popupRemoveDatabaseInfo()
{
	TeQtDatabaseItem *dbItem = (TeQtDatabaseItem*)databasesListView_->popupItem();
	TeDatabase *db = dbItem->getDatabase();

	QString name = db->databaseName().c_str();
	int response = QMessageBox::question(this, tr("Remove general information"),
		tr("Do you really want to remove general information of the database:") + " " + name + "?",
			 tr("Yes"), tr("No"));

	if (response != 0)
		return;


	string del = "DELETE FROM tv_generalinfo WHERE layer_id = -1";
	if(db->execute(del))
		QMessageBox::information(this, tr("Information"),
			tr("Information removed with success!"));
}

void TerraViewBase::popupInsertFileLayerInfo()
{
	TeQtLayerItem *layerItem = (TeQtLayerItem*)databasesListView_->popupItem();
	TeLayer* layer = layerItem->getLayer();
	int id = layer->id();
	TeDatabase* db = layer->database();

	QFileDialog filed(lastOpenDir_, QString::null, 0, 0, true);
	QString qfile = filed.getOpenFileName (QString::null, QString::null, 0, 0);

	if(qfile.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("No media was selected!"));
		return;
	}

	TeWaitCursor wait;
	string file = qfile.latin1();
	lastOpenDir_ = TeGetPath(file.c_str()).c_str();
	string type = TeGetExtension(file.c_str());

	string infoTable = "tv_generalinfo";
	if(db->tableExist(infoTable) == false)
	{
		TeAttribute at;
		TeAttributeList atL;

		at.rep_.type_ = TeINT;
		at.rep_.name_ = "layer_id";
		at.rep_.isPrimaryKey_ = true;
		atL.push_back(at);

		at.rep_.isPrimaryKey_ = false;
		at.rep_.type_ = TeSTRING;
		at.rep_.numChar_ = 255;
		at.rep_.name_ = "name_";
		atL.push_back(at);

		at.rep_.type_ = TeSTRING;
		at.rep_.name_ = "type_";
		at.rep_.numChar_ = 10;
		atL.push_back(at);

		at.rep_.type_ = TeBLOB;
		at.rep_.numChar_ = 0;
		at.rep_.name_ = "media_blob";
		atL.push_back(at);

		if(db->createTable(infoTable, atL) == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"),
				tr("Fail to create the general info table!"));
			return;
		}
	}

	// insert info
	if(insertInfo2Blob(db, id, infoTable, file, type) == false)
	{
		wait.resetWaitCursor();
		string del = "DELETE FROM tv_generalinfo WHERE layer_id = " + Te2String(layer->id());
		db->execute(del);
		QMessageBox::critical(this, tr("Error"),
			tr("Fail to insert information!"));
	}
}

void TerraViewBase::popupInsertURLLayerInfo()
{
	TeQtLayerItem *layerItem = (TeQtLayerItem*)databasesListView_->popupItem();
	TeLayer* layer = layerItem->getLayer();
	int id = layer->id();
	TeDatabase* db = layer->database();

	URLWindow urlWindow(this, "urlInfo", true);
	if (urlWindow.exec() == QDialog::Rejected)
		return;

	QString qsite = urlWindow.URLLineEdit->text();
	if(qsite.isNull() || qsite.isEmpty())
		return;

	TeWaitCursor wait;

	string site = qsite.latin1();

	string infoTable = "tv_generalinfo";
	if(db->tableExist(infoTable) == false)
	{
		TeAttribute at;
		TeAttributeList atL;

		at.rep_.type_ = TeINT;
		at.rep_.name_ = "layer_id";
		at.rep_.isPrimaryKey_ = true;
		atL.push_back(at);

		at.rep_.isPrimaryKey_ = false;
		at.rep_.type_ = TeSTRING;
		at.rep_.numChar_ = 255;
		at.rep_.name_ = "name_";
		atL.push_back(at);

		at.rep_.type_ = TeSTRING;
		at.rep_.name_ = "type_";
		at.rep_.numChar_ = 10;
		atL.push_back(at);

		at.rep_.type_ = TeBLOB;
		at.rep_.numChar_ = 0;
		at.rep_.name_ = "media_blob";
		atL.push_back(at);

		if(db->createTable(infoTable, atL) == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"),
				tr("Fail to create the general info table!"));
			return;
		}
	}

	// insert info
	if(insertInfo2Blob(db, id, infoTable, site, "URL") == false)
	{
		wait.resetWaitCursor();
		string del = "DELETE FROM tv_generalinfo WHERE layer_id = " + Te2String(layer->id());
		db->execute(del);
		QMessageBox::critical(this, tr("Error"),
			tr("Fail to insert information!"));
	}
}

void TerraViewBase::popupRetrieveLayerInfo()
{
	TeQtLayerItem *layerItem = (TeQtLayerItem*)databasesListView_->popupItem();
	TeLayer* layer = layerItem->getLayer();
	int id = layer->id();
	TeDatabase* db = layer->database();
	string type;

	string tempName = getInfoBlob2TemporaryFile(db, id, "tv_generalinfo", type);

	if(tempName.empty())
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Error to get blob information!"));
		return;
	}

	if(type == "URL")
		tempName += ".URL";

	TeQtShowMedia smedia;
	smedia.slotShowFile(tempName);
}

void TerraViewBase::popupRemoveLayerInfo()
{
	TeQtLayerItem *layerItem = (TeQtLayerItem*)databasesListView_->popupItem();
	TeLayer* layer = layerItem->getLayer();
	int id = layer->id();
	TeDatabase* db = layer->database();

	QString name = layer->name().c_str();
	int response = QMessageBox::question(this, tr("Remove general information"),
		tr("Do you really want to remove general information of the layer:") + " " + name + "?",
			 tr("Yes"), tr("No"));

	if (response != 0)
		return;

	string del = "DELETE FROM tv_generalinfo WHERE layer_id = " + Te2String(id);
	if(db->execute(del))
		QMessageBox::information(this, tr("Information"),
			tr("Information removed with success!"));
}




void TerraViewBase::printFileResolutionFunc( const QString & filename)
{
	TeBox b = canvas_->getWorld();
	PrintFileResolution* pf = new PrintFileResolution(this, "printFileResolution");
	if(pf->exec() == QDialog::Rejected)
	{
		if(canvas_->numberOfPixmaps() < 3)
		{
			canvas_->numberOfPixmaps(3);
			canvas_->setWorld(b, cvContentsW_, cvContentsH_);
		}
		delete pf;
		return;
	}

	printFileResolution_ = pf->resolutionComboBox->currentText().toInt();
	if(pf->displayResolutionCheckBox->isChecked())
		printWithDisplayResolution_ = true;
	else
		printWithDisplayResolution_ = false;

	int width, height;
	if(printWithDisplayResolution_)
	{
		width = canvas_->width();
		height = canvas_->height();
	}
	else
	{
		QPaintDeviceMetrics pdm( canvas_->viewport() );
		int wmm = pdm.widthMM ();
		int hmm = pdm.heightMM ();

		width = (int)((double)wmm / 25.4 * (double)printFileResolution_ + .5);
		height = (int)((double)hmm / 25.4 * (double)printFileResolution_ + .5);
	}

	canvas_->widthRef2PrinterFactor(canvas_->getPixmap0()->width());
	canvas_->numberOfPixmaps(1);
	if(canvas_->setWorld(b, width, height) == true)
		displayWindow_->plotOnPrinter(filename);
	else
	{
		canvas_->numberOfPixmaps(3);
		QString erro = tr("It was not possible to allocate memory for these dimensions");
		QMessageBox::warning(this, tr("Warning"), erro);
		delete pf;
		canvas_->setWorld(b, cvContentsW_, cvContentsH_);
		printFileResolutionFunc(filename);
	}
}


void TerraViewBase::pluginsMenuSelected( int id )
{
  std::vector< PluginInfo > plist;
  plugins_manager_.getValidPluginsList( plist );
    
  /*TEAGN_TRUE_OR_THROW( ( ( id >= 0 ) && ( id < (int)plist.size() ) ),
    "Invalid plugin id" );*/

	TEAGN_TRUE_OR_THROW( ( ( id >= 0 ) ), "Invalid plugin id" );

  for(unsigned int i = 0; i < plist.size(); ++i)
  {
	  if(vecPluginFileNames[id] == plist[ i ].plugin_name_)
	  {
		  PluginParameters params = getPluginParameters();
		  params.currentPluginName_ = vecPluginNames[id];

		  plugins_manager_.setDefaultPluginsParameters( params );
		  plugins_manager_.runPlugin( plist[ i ] );		  
		  break;
	  }
  }
}


Graphic3DWindow* TerraViewBase::getGraphic3DWindow()
{
	return graphic3DWindow_;
}


void TerraViewBase::popupDispersion3DSlot()
{
 	if (graphic3DWindow_ == NULL)
   		graphic3DWindow_ = new Graphic3DWindow(this, "graphic 3D", false);
	else
		graphic3DWindow_->initData();
	graphic3DWindow_->show();
}


void TerraViewBase::setQApplicationPointer( QApplication* qapp_ptr )
{
  qapplication_ptr_ = qapp_ptr;
}


QApplication* TerraViewBase::getQApplicationPointer()
{
  return qapplication_ptr_;
}

void TerraViewBase::generateSamplePointsAction_activated()
{ 
	if (currentDatabase_->layerMap().empty())
	{
	    QMessageBox::critical(this, tr("Error"),tr("The database has no layers!"));
	    return;
	}

	if (!currentTheme_)
		return;

	TeTheme* curTheme = 0;
	if (currentTheme_->getTheme()->type() < 3)
		curTheme = (TeTheme*)(currentTheme_->getTheme());
	GenerateSamplePoints* samplepointsgenerator = new GenerateSamplePoints(this, "samplepoints", false);
	samplepointsgenerator->setParams(currentDatabase_, curTheme, this->currentLayer());
	if (samplepointsgenerator->exec()== QDialog::Accepted)
	{
		TeLayer *layer = samplepointsgenerator->getNewLayer();
		if (layer != 0)
		{
			TeQtDatabasesListView *databasesListView = getDatabasesListView();
			TeQtDatabaseItem *databaseItem = databasesListView->currentDatabaseItem();

			TeQtLayerItem *layerItem = new TeQtLayerItem((QListViewItem*)databaseItem, layer->name().c_str(), layer);
			layerItem->setEnabled(true);
			if (databasesListView->isOpen(layerItem->parent()) == false)
				databasesListView->setOpen(layerItem->parent(),true);
			checkWidgetEnabling();

			QString msg = tr("The layer of points was created! Do you want to diplay it?");
			int response = QMessageBox::question(this, tr("Display the Data"),
				msg, tr("Yes"), tr("No"));
			if(response == 0)
			{
				TeQtViewItem *viewItem = 0;
				TeView *view = 0;
				// if data didn't come from a Theme create a new view and a new Theme
				if (samplepointsgenerator->getViewId() == 0) 
				{
					// find a valid name for a view
					QString viewName = layer->name().c_str();
					bool flag = true;
					int count = 1;
					while (flag)
					{
						TeViewMap& viewMap = currentDatabase_->viewMap();
						TeViewMap::iterator it;
						for (it = viewMap.begin(); it != viewMap.end(); ++it)
						{
							if (it->second->name() == viewName.latin1())
								break;
						}
						if (it == viewMap.end())
							flag = false;

						else
						{
							char vnumber[5];
							sprintf(vnumber,"_%d",count);
							viewName += vnumber;
							count++;
						}
					}
					
					// create a projection to the view the parameters of the imported layer
					TeProjection *viewProjection = TeProjectionFactory::make(layer->projection()->params());

					// create a view and a theme on the main window for data visualization
					// add the new view to the database and insert it on the map of Views
					view = new TeView (viewName.latin1(), currentDatabase_->user());
					TeBox b;
					view->setCurrentBox(b);
					view->setCurrentTheme(-1);
					view->projection(viewProjection);
					if (!currentDatabase_->insertView(view))
					{
						QString msg = tr("The view") + " \"" + view->name().c_str() + "\" " + tr("could not be saved into the database!");
						QMessageBox::critical( this, tr("Error"), msg);
						delete samplepointsgenerator;
						return;
					}
					currentDatabase_->insertView(view);
					if(currentDatabase_->projectMap().empty() == false)
					{
						TeProjectMap& pm = currentDatabase_->projectMap();
						TeProject* project = pm.begin()->second;
						project->addView(view->id());
					}
					checkWidgetEnabling();

					// create the view item and set it as the current one
					viewItem = new TeQtViewItem(viewsListView_, viewName, view);
				}// if data came from a Theme use the Theme's view
				else
				{
					TeViewMap& viewMap = currentDatabase_->viewMap();
					TeViewMap::iterator it;
					for (it = viewMap.begin(); it != viewMap.end(); ++it)
					{
						if (it->second->id() == samplepointsgenerator->getViewId())
							break;
					}
					assert (it != viewMap.end());
					view = it->second;
					viewItem = viewsListView_->getViewItem(view);
				}

				// create the theme that will be associated to this view
				TeAttrTableVector ATV;
				layer->getAttrTables(ATV);
				QString themeName = currentDatabase_->getNewThemeName(layer->name()).c_str();
				if(createTheme(themeName.latin1(), view, layer, "", ATV) == false)
				{
					QString msg = tr("The theme") + " \"" + themeName + "\" ";
					msg += tr("could not be inserted into the view") + " \"" + view->name().c_str() + "\" !";
					QMessageBox::critical( this, tr("Error"), msg);
				}

				vector<TeQtThemeItem*> themeItemVec = viewItem->getThemeItemVec();
				unsigned int nt = themeItemVec.size(), aux=0;
				TeAppTheme* newTheme=0;
				for (aux=0;aux<nt;++aux)
				{
					newTheme = themeItemVec[aux]->getAppTheme();
					if(newTheme->getTheme()->name() == themeName.latin1())
						break;
				}
				if (newTheme)
				{
					grid_->clear();
					QListViewItem* item = (QListViewItem*)viewsListView_->getThemeItem(newTheme);
					viewsListView_->setOpen(viewItem, true);
					if(item)
					{
						viewsListView_->setOn((QCheckListItem*)item, true);
						viewsListView_->selectItem(item);
					}
					resetAction_activated();
				}
			}
		}
	}
	delete samplepointsgenerator;
}


void TerraViewBase::zoomAreaSlot()
{
	TeWaitCursor wait;
	TeBox box;

	if(currentView_ == 0)
		return;

	vector<TeViewNode*>& themeVector = currentView_->themes();
	if (themeVector.size() == 0)
		return;

	if (canvas_->getCursorMode() == TeQtCanvas::Area && zoomCursorAction->isOn())
	{
		box = canvas_->getCursorBox();
		canvas_->setWorld(box, cvContentsW_, cvContentsH_);

		displayWindow_->plotData();

		if(curBoxIndex_ >= 0)
		{
			TeBox bbox = boxesVector_[curBoxIndex_];
			if(bbox.x1_ == box.x1_ && bbox.y1_ == box.y1_ &&
				(bbox.width() == box.width() || bbox.height() == box.height()))
				return;
		}

		++curBoxIndex_;
		boxesVector_.push_back(box);
		checkWidgetEnabling();
	}
}


PluginParameters TerraViewBase::getPluginParameters()
{
	std::string userName = "";
	if(currentDatabase() != NULL)
	{
		userName = currentDatabase()->user();
	}

  PluginParameters plugin_params(
    this,
    TeProgress::instance(),
    getDatabasesListView(),
    getViewsListView(),
    getGrid(),
    getCanvas(),
    currentDatabase(),
    currentLayer(),
    currentView(),
    currentTheme(),
    &(PluginsEmitter::instance()),
	false,
	"",
	userName,
    PluginSupportFunctions::getCurrentDatabasePtr,
    PluginSupportFunctions::getCurrentLayerPtr,
    PluginSupportFunctions::getCurrentViewPtr,
    PluginSupportFunctions::getCurrentThemeAppPtr,
	PluginSupportFunctions::setCurrentThemeAppPtr,
    PluginSupportFunctions::updateTVInterface,
    PluginSupportFunctions::loadTranslationFile,
    PluginSupportFunctions::updatePluginParameters,
	PluginSupportFunctions::plotTVData,
	PluginSupportFunctions::setPluginMetadata,
	NULL
  );

  return plugin_params;
}


void TerraViewBase::loadPluginsSlot()
{
  plugins_manager_.setDefaultPluginsParameters( getPluginParameters() );
  
  if( plugins_manager_.loadPlugins( "./plugins/" ) > 0 ) {
  
    /* Updating the plugins menu with the compatible loaded plugins */
    
//    std::vector< PluginInfo > valid_plist;
//    plugins_manager_.getValidPluginsList( valid_plist );
//
//	int pluginCount = 0;
//    
//    if( valid_plist.size() > 0 ) {
//      std::map< int, QPopupMenu* > menuid2menupointer_map;
//      
//      for( unsigned int index = 0 ; index < valid_plist.size() ; ++index ) {
//        std::vector< std::string > plugin_menu_entries;
//        TeSplitString( valid_plist[ index ].plugin_type_, ".", 
//          plugin_menu_entries );
//        TEAGN_TRUE_OR_THROW( ( plugin_menu_entries.size() > 0 ),
//          "Invalid plugin meny entries number" );
//          
//        QPopupMenu* insertion_menu = pluginsMenu;
//        
//        unsigned int pme_index = 1;
//        
//        while( pme_index < plugin_menu_entries.size() ) {
//          QPopupMenu* target_sub_menu = 0;
//            
//          for( unsigned int insertion_menu_index = 0 ; 
//            insertion_menu_index < insertion_menu->count() ;
//            ++insertion_menu_index ) {
//            
//            int menuid = insertion_menu->idAt( insertion_menu_index );
//            
//            if( insertion_menu->text( menuid ) == 
//              plugin_menu_entries[ pme_index ].c_str() ) {
//              
//              target_sub_menu = menuid2menupointer_map[ menuid ];
//              TEAGN_TRUE_OR_THROW( ( target_sub_menu != 0 ),
//                "Popup menu map error" )
//              
//              break; 
//            }
//          }
//          
//          if( target_sub_menu == 0 ) {
//            int new_insertion_menu_index = insertion_menu->count();
//            
//            target_sub_menu = new QPopupMenu( insertion_menu,
//              plugin_menu_entries[ pme_index ].c_str() ); 
//           
//            insertion_menu->insertItem( plugin_menu_entries[ 
//              pme_index ].c_str(), target_sub_menu, -1, 
//                new_insertion_menu_index );
//                
//            int new_insertion_menu_id = insertion_menu->idAt( 
//              new_insertion_menu_index );
//              
//            menuid2menupointer_map[ new_insertion_menu_id ] = 
//              target_sub_menu;
//          }         
//
//          insertion_menu = target_sub_menu;
//          
//          ++pme_index;
//        };        
//
//		PluginsSignal sigGetNames(PluginsSignal::S_REQUEST_PLUGIN_METADATA);
//		sigGetNames.signalData_.store(PluginsSignal::D_STRING,  valid_plist[ index ].plugin_name_);
//		sigGetNames.signalData_.store(PluginsSignal::D_PLUGIN_PARAMS, getPluginParameters());
//		PluginsEmitter::instance().send(sigGetNames);
//
//		if(pluginMetadataSetted_ == true)
//		{
//			for(unsigned int i = 0; i < currentPluginMetadata_._vecPluginNames.size(); ++i)
//			{
//				QString plugin_name;
//				if(currentPluginMetadata_._vecTranslatedNames[i].empty())
//				{
//					plugin_name = valid_plist[ index ].plugin_name_.c_str();
//				}
//				else
//				{
//					plugin_name = currentPluginMetadata_._vecTranslatedNames[i].c_str();
//				}
//					
//        		insertion_menu->insertItem( plugin_name , this, SLOT( pluginsMenuSelected( int ) ), 0, pluginCount, -1 );
//
//				vecPluginFileNames.push_back(valid_plist[ index ].plugin_name_);
//				vecPluginNames.push_back(currentPluginMetadata_._vecPluginNames[i]);
//				vecTranslatedNames.push_back(currentPluginMetadata_._vecTranslatedNames[i]);
//				++pluginCount;
//			}
//		}
//		else
//		{
//			QString plugin_name( valid_plist[ index ].plugin_name_.c_str() );
//        	insertion_menu->insertItem( plugin_name , this, SLOT( pluginsMenuSelected( int ) ), 0, pluginCount, -1 );
//
//			vecPluginFileNames.push_back(valid_plist[ index ].plugin_name_);
//			vecPluginNames.push_back(valid_plist[ index ].plugin_name_);
//			vecTranslatedNames.push_back(valid_plist[ index ].plugin_name_);
//			++pluginCount;
//		}
//		pluginMetadataSetted_ = false;
//
//		
//        
////        connect( insertion_menu, SIGNAL( activated( int ) ), this, 
////         SLOT( pluginsMenuSelected( int ) ) );          
//      }    
//    }
//    
//    /* Warning the user about the incompatible plugins */
//    
//    std::vector< PluginInfo > invalid_plist;
//    plugins_manager_.getInvalidPluginsList( invalid_plist );
//    
//    if( invalid_plist.size() > 0 ) {
//      for( unsigned int index = 0 ; index < invalid_plist.size() ; ++index ) {
//        QString temp_string( 
//          tr( "Incompatible plugin found" )
//          + "\n\n" + tr( "Name" ) + QString( ": " ) + 
//            invalid_plist[ index ].plugin_name_.c_str()
//          + "\n" + tr( "Plugin interface version" ) + QString( ": " ) + 
//            invalid_plist[ index ].plugin_interface_version_.c_str()
//          + "\n" + tr( "Current TerraView interface version" ) + 
//            QString( ": " ) + QString( PLUGINPARAMETERS_VERSION )
//          + "\n" + tr( "Plugin version" ) + QString( ": " ) 
//          + QString( invalid_plist[ index ].plugin_build_number_.c_str() )
//          + "."
//          + QString( invalid_plist[ index ].plugin_major_version_.c_str() )
//          + "."
//          + QString( invalid_plist[ index ].plugin_minor_version_.c_str() )
//          + "\n" + tr( "Description" ) + QString( ": " ) + 
//            invalid_plist[ index ].plugin_description_.c_str()
//          + "\n" + tr( "Author" ) + QString( ": " ) + 
//            invalid_plist[ index ].plugin_vendor_.c_str()
//          + "\n" + tr( "Author e-mail" ) + QString( ": " ) + 
//            invalid_plist[ index ].plugin_vendor_email_.c_str()
//          + "\n" + tr( "Web page" ) + QString( ": " ) + 
//            invalid_plist[ index ].plugin_url_.c_str()
//        );
//
//        QMessageBox::warning( this, "TerraView", temp_string, 
//          QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton );
//      }
//    }
  }
}

void TerraViewBase::popupSaveThemeToFile()
{
	if(currentTheme_ == NULL)
	{
		QMessageBox::information(this, tr("Information"), tr("There is not a selecetd theme. Select one first!"));
		return;
	}

	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	if(curTheme->type() != TeTHEME)
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		return;
	}

	TeQtThemeItem *popupThemeItem = (TeQtThemeItem*)viewsListView_->popupItem();
	TeTheme* theme = (TeTheme*)(popupThemeItem->getAppTheme()->getTheme());
	if (theme->layer()->hasGeometry(TeRASTER) || theme->layer()->hasGeometry(TeRASTERFILE))
	{
		SaveRaster* saveRaster = new SaveRaster(this, "saveRaster", true);
		saveRaster->setParams(currentDatabase_,canvas_->getWorld(),canvas_->projection(), lastOpenDir_);
		saveRaster->init(popupThemeItem);
		if (saveRaster->exec() == QDialog::Accepted)
			lastOpenDir_ = saveRaster->getLastOpenDir();
		delete saveRaster;
	}
	else
	{
		TeAttrTableVector& attVector = theme->attrTables();
		TeAttrTableVector::iterator it = attVector.begin();
		while (it != attVector.end())
		{
			TeTable table = *it;
			if (table.tableType() == TeAttrExternal)
			{
				QMessageBox::information(this,tr("Information"),tr("Theme has an external table linked. It should be unlinked before saving it."));
				return;
			}
			else if (table.tableType() == TeFixedGeomDynAttr)
			{
				if (QMessageBox::question(this, tr("Question"),
					tr("Theme has a temporal table associated to it, that might not be exported properly to a data file.\nDo you wish to continue?"), tr("Yes"), tr("No")) == 1)
					return;

			}
			++it;
		}

		SaveThemeToFileWindow *saveThemeW = new SaveThemeToFileWindow(this, "SaveThemeToFile", true);
		saveThemeW->setParams(lastOpenDir_);
		saveThemeW->init(popupThemeItem);
		if (saveThemeW->exec()== QDialog::Accepted)
			lastOpenDir_ = saveThemeW->getLastOpenDir();
		delete saveThemeW;		
	}
}




void TerraViewBase::queryChanged()
{
	emit queryChangedSignal();
}


void TerraViewBase::groupChanged()
{
	emit groupChangedSignal();
}




void TerraViewBase::popupRasterDisplaySlot()
{
	viewsListView_->getPopupMenu()->hide();
	TeQtThemeItem *popupThemeItem = (TeQtThemeItem*)viewsListView_->popupItem();
	TeAppTheme* appTheme = popupThemeItem->getAppTheme();
	TeTheme* theme = (TeTheme*)appTheme->getTheme();
	if (!(theme->layer()->hasGeometry(TeRASTER) || theme->layer()->hasGeometry(TeRASTERFILE)))
	{
		QMessageBox::warning(this, tr("Warning"), tr("The theme has no raster geometry!"));
		return;
	}
	TeRaster* raster_ = theme->layer()->raster(); 
	if (raster_->params().photometric_[0] == TeRasterParams::TePallete)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Raster has a pallete associated. Can not change display properties."));
		return;
	}

	if (theme->visibleRep() & 0x40000000  &&				
		theme->grouping().groupMode_ == TeRasterSlicing)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Raster has a legend associated. Remove it before change display properties."));
		return;
	}

	RasterColorCompositionWindow* rcw = new RasterColorCompositionWindow(this, "rasterDisplay", true);
	rcw->init(popupThemeItem);
	TeTheme* curTheme = (TeTheme*)currentTheme_->getTheme();
	if (rcw->exec() == QDialog::Accepted)
	{
		currentDatabase_->insertRasterVisual(theme->id(), curTheme->rasterVisual());
		delete rcw;
		drawAction_activated();
		return;
	}
	delete rcw;
	return;
}


void TerraViewBase::infolayerMultiLayer_Vectorial_ExportAction_activated()
{

	if(!currentDatabase_)
	{
		return;
	}

	if (currentDatabase_->layerMap().empty())
	{
	    QMessageBox::critical(this, tr("Error"),tr("The database has no layers!"));
	    return;
	}

	ExportLayersThemes  clsELT(this, "Export Layers Themes", 0);

	clsELT.init(currentDatabase_, lastOpenDir_);

	clsELT.exec();

	lastOpenDir_ = clsELT.getLastOpenDir();
}


void TerraViewBase::infolayerUpdate_MetadataAction_activated()
{
	if(currentDatabase_ == NULL)
	{
		QMessageBox::warning(this, tr("TerraView error"), tr("There's no current database connected!"));

		return;
	}

	TelayerMetadata dlg(this, "TeLayer Metadata DLG", true);

	dlg.init(currentDatabase_);

	if(dlg.exec() == QDialog::Accepted)
		QMessageBox::information(this, tr("TerraView"), tr("Update metadata operation concluded!"));
}

void TerraViewBase::setPluginMetadata(const PluginMetadata& pluginMetadata)
{
	pluginMetadataSetted_ = true;
    currentPluginMetadata_ = pluginMetadata;
}


void TerraViewBase::showTabwinHelp()
{
	QMessageBox::warning(this, tr("TerraView"), tr("Please, see TerraView help to know how to call correctly TerraView from TabWin"));
	help_ = new Help(this, "help", false);
	help_->init("tabwin.htm");
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

void TerraViewBase::getLayerMenu(QPopupMenu* popupMenu)
{
    popupMenu->clear();

	popupMenu->insertItem(QString(tr("Rename")));
	popupMenu->insertItem(QString(tr("Remove")));
	popupMenu->insertSeparator();
	popupMenu->insertItem(QString(tr("Create Cells...")));
	popupMenu->insertSeparator();
	popupMenu->insertItem(tr("Create Layer Table..."));
	popupMenu->insertItem(QString(tr("Delete Layer Table...")));
	popupMenu->insertSeparator();
	popupMenu->insertItem(QString(tr("Add Centroid Representation")));
	popupMenu->insertItem(QString(tr("Create Layer of Centroids")));
	popupMenu->insertItem(QString(tr("Create Layer of Centroids Counting")));
	popupMenu->insertSeparator();
	popupMenu->insertItem(QString(tr("Load Memory")));
	popupMenu->insertItem(QString(tr("Clear Memory")));
	popupMenu->insertSeparator();
	popupMenu->insertItem(QString(tr("Projection...")));
	popupMenu->insertSeparator();

	QPopupMenu* popupLayerInfo = new QPopupMenu;
	popupMenu->insertItem(QString(tr("General Info")), popupLayerInfo);
	QPopupMenu* popupLayerInfoInsert = new QPopupMenu;
	popupLayerInfo->insertItem(QString(tr("Insert")), popupLayerInfoInsert);
	popupLayerInfoInsert->insertItem(QString(tr("File")));
	popupLayerInfoInsert->insertItem(QString(tr("URL")));
	popupLayerInfo->insertItem(QString(tr("Retrieve")));
	popupLayerInfo->insertItem(QString(tr("Remove")));
	popupMenu->insertSeparator();	
	popupMenu->insertItem(QString(tr("Properties...")));

    TeQtLayerItem* layerItem = getDatabasesListView()->currentLayerItem();
    if(layerItem == 0)
    {
        for(unsigned int i = 0; i < popupMenu->count(); ++i)
        {
            int id = popupMenu->idAt(i);
            popupMenu->setItemEnabled(id, false);
        }
        return;
    }

    TeLayer* layer = layerItem->getLayer();
    TeDatabase* db = layer->database();

	if(layer->geomRep() & TePOLYGONS || layer->geomRep() & TeLINES)
	{
		map<int, RepMap>::iterator it = layerId2RepMap_.find(layer->id());
		if(it == layerId2RepMap_.end())
		{
			popupMenu->setItemEnabled(popupMenu->idAt(12), true);	// load memory
			popupMenu->setItemEnabled(popupMenu->idAt(13), false);	// clear memory
		}
		else
		{
			popupMenu->setItemEnabled(popupMenu->idAt(12), false);	// load memory
			popupMenu->setItemEnabled(popupMenu->idAt(13), true);	// clear memory
		}
	}

	if(layer->geomRep() & TePOLYGONS || layer->geomRep() & TeLINES || layer->geomRep() & TeCELLS )
	{
		popupMenu->setItemEnabled(popupMenu->idAt(9), true);		// create layer of centroids
		if((layer->geomRep() & TePOINTS) == false)
			popupMenu->setItemEnabled(popupMenu->idAt(8), true);	// add centroid representation
		else
			popupMenu->setItemEnabled(popupMenu->idAt(8), false);	// add centroid representation
	}
	else
	{
		popupMenu->setItemEnabled(popupMenu->idAt(9), false);	// create layer of centroids
		popupMenu->setItemEnabled(popupMenu->idAt(8), false);	// add centroid representation
	}

	if(layer->geomRep() & TePOINTS)
	{
		string sel = "SELECT COUNT(*) FROM " + layer->tableName(TePOINTS);
		sel += " GROUP BY x, y ORDER BY COUNT(*) DESC";
		int max = 1;
		TeDatabasePortal *portal = db->getPortal();
		if(portal->query(sel))
		{
			if(portal->fetchRow())
				max = portal->getInt(0);
		}
		delete portal;
		if(max > 1)
			popupMenu->setItemEnabled(popupMenu->idAt(10), true);		// create layer of centroids counting
		else
			popupMenu->setItemEnabled(popupMenu->idAt(10), false);		// create layer of centroids counting
	}
	else
		popupMenu->setItemEnabled(popupMenu->idAt(10), false);			// create layer of centroids counting

	std::string file = "";

	if(db->tableExist("tv_generalinfo"))
	{
		TeDatabasePortal *portal = db->getPortal();
		string query = "SELECT name_ FROM tv_generalinfo WHERE layer_id = " + Te2String(layer->id());
		if(portal->query(query) && portal->fetchRow())
			file = portal->getData(0);
		delete portal;
	}

	if(file.empty())
	{
		popupLayerInfo->setItemEnabled(popupLayerInfo->idAt(0), true); // insert information 
		popupLayerInfo->setItemEnabled(popupLayerInfo->idAt(1), false); // retrieve information 
		popupLayerInfo->setItemEnabled(popupLayerInfo->idAt(2), false); // remove information 
	}
	else
	{
		popupLayerInfo->setItemEnabled(popupLayerInfo->idAt(0), false); // insert information 
		popupLayerInfo->setItemEnabled(popupLayerInfo->idAt(1), true); // retrieve information 
		popupLayerInfo->setItemEnabled(popupLayerInfo->idAt(2), true); // remove information 
	}
}

void TerraViewBase::getViewMenu(QPopupMenu* popupMenu)
{
    popupMenu->clear();

	popupMenu->insertItem(QString(tr("Rename")));
	popupMenu->insertItem(QString(tr("Remove")));
	popupMenu->insertItem(QString(tr("Export Themes")));
	popupMenu->insertSeparator();
	popupMenu->insertItem(QString(tr("Projection...")));
    popupMenu->insertSeparator();
    popupMenu->insertItem(QString(tr("Connect...")));
    popupMenu->insertSeparator();

    if(currentDatabase_ && currentDatabase_->viewMap().size() == 1)
        popupMenu->setItemEnabled(popupMenu->idAt(6), false);

	QPopupMenu *popupGeoprocessing = new QPopupMenu;
	popupMenu->insertItem(QString(tr("Geoprocessing")), popupGeoprocessing);
	popupGeoprocessing->insertItem(QString(tr("Aggregation...")));
	popupGeoprocessing->insertItem(QString(tr("Add...")));
	popupGeoprocessing->insertItem(QString(tr("Intersection...")));
	popupGeoprocessing->insertItem(QString(tr("Difference...")));
	QPopupMenu *popupGeoprocessingAssignData = new QPopupMenu;
	popupGeoprocessing->insertItem(QString(tr("Assign Data By Location")), popupGeoprocessingAssignData);
	popupGeoprocessingAssignData->insertItem(QString(tr("Distribute...")));
	popupGeoprocessingAssignData->insertItem(QString(tr("Collect...")));
	popupGeoprocessing->insertItem(QString(tr("Create Buffers...")));
	popupMenu->insertSeparator();
	popupMenu->insertItem(QString(tr("Properties...")));

    TeView* v = currentView();
    if(v == 0)
    {
        for(unsigned int i = 0; i < popupMenu->count(); ++i)
        {
            int id = popupMenu->idAt(i);
            popupMenu->setItemEnabled(id, false);
        }
    }
}

void TerraViewBase::getThemeMenu(QPopupMenu* popupMenu)
{
    popupMenu->clear();

	popupMenu->insertItem(tr("Rename"));
	popupMenu->insertItem(tr("Remove"));
    popupMenu->insertSeparator();

	QPopupMenu *popupThemeVisual = new QPopupMenu;
	popupMenu->insertItem(tr("Visual"), popupThemeVisual);
	popupThemeVisual->insertItem(tr("Default..."));
	popupThemeVisual->insertItem(tr("Pointing..."));
	popupThemeVisual->insertItem(tr("Query..."));
	popupThemeVisual->insertItem(tr("Pointing and Query..."));
	popupThemeVisual->insertSeparator();
	popupThemeVisual->insertItem(tr("Text..."));
	popupMenu->insertItem(tr("Visibility of the Representations..."));
	popupMenu->insertSeparator();
	popupMenu->insertItem(tr("Edit Legend..."));					
	popupMenu->insertItem(tr("Text Representation..."));
	popupMenu->insertSeparator();
	popupMenu->insertItem(tr("Create Layer from Theme..."));
	popupMenu->insertItem(tr("Create Theme from Theme..."));
	popupMenu->insertItem(tr("Save Theme to File..."));
	popupMenu->insertSeparator();
	popupMenu->insertItem(tr("Remove All Aliases"));
	popupMenu->insertSeparator();
	popupMenu->insertItem(tr("Properties..."));

    TeAppTheme* apptTheme = currentTheme_;
    if(apptTheme != 0)
    {
        TeTheme* t = dynamic_cast<TeTheme*>(apptTheme->getTheme());
        if(t != 0)
            return;
    }

    for(unsigned int i = 0; i < popupMenu->count(); ++i)
    {
        int id = popupMenu->idAt(i);
        popupMenu->setItemEnabled(id, false);
    }
}

void TerraViewBase::setAppCaption(const QString& caption)
{
	setCaption(caption);
	appCaption_ = caption;
}

