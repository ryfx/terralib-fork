/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#ifdef _WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

// TerraLib's includes
#include "TeView.h"
#include "TeFileTheme.h"
#include "TeDriverSHPDBF.h"
#include <TeDatabase.h>

// TerraView's includes
//#include "TeAppTheme.h"
//#include "PluginBase.h"
#include "PluginParameters.h"
#include <TeQtCanvas.h>
#include <TeQtThemeItem.h>

// Qt's includes
#include <qstring.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qmainwindow.h>

// interfaces include
#include <projection.h>
#include <attrTable.h> 
#include <help.h>

//XPM include files
#include <fileTheme.xpm>

// Constructor
void FileTheme::init()
{
	// initialize internal variables
    plugPars_ = 0;
    canvas_ = 0;
    viewId_ = -1;
    currentDatabase_ = 0;
//    proj_ = 0;
    fileTheme_ = 0;
//    objPropsWindow_ = 0;
//	shpDriver_ = 0;
//	help_ = 0;

	themePropTable->horizontalHeader()->setLabel(0, tr("Information"));
	themePropTable->horizontalHeader()->setLabel(1, tr("Value"));

	// connect itself to receive signals from TerraView
//     comm_.setHostObj( this, signalHandler );

	 std::vector<TeQtThemeItem*>::iterator it;
	 std::vector<TeAppTheme*>::iterator it2;
}

// Destructor
void FileTheme::destroy()
{
//	delete objPropsWindow_;
//	if (fileTheme_)
//	{
//		if (shpDriver_ && (fileTheme_->getDataDriver() != shpDriver_))
//			delete shpDriver_;
//		if (proj_ && (fileTheme_->getThemeProjection() != proj_))
//			delete proj_;
//		delete fileTheme_;
//	}
//	else
//	{
//		if (shpDriver_)
//			delete shpDriver_;
//		if (proj_)
//			delete proj_;
//	}
//	shpDriver_ = 0;
//	proj_ = 0;
//	fileTheme_ = 0;
//
//	delete showPluginWindow_;
//
//    std::vector<int>::iterator it;
//
//    QPopupMenu* themeMenu = findThemeMenu();
//
//    if(themeMenu == NULL)
//    {
//        return;
//    }
//
//    for(it = mnuIdxs_.begin(); it != mnuIdxs_.end(); ++it)
//    {
//        themeMenu->removeItem((*it));
//    }
}


//void FileTheme::initFromPluginParameters()
//{
//	objPropsWindow_ = new AttributesTable(this->plugPars_->qtmain_widget_ptr_);
//	objPropsWindow_->attributesTable->setCaption(tr("Object Information"));
//	objPropsWindow_->attributesTable->setNumCols(2);
//	objPropsWindow_->attributesTable->horizontalHeader()->setLabel(0, tr("Attribute"));
//	objPropsWindow_->attributesTable->horizontalHeader()->setLabel(1, tr("Value"));

//	/* Inserting the tool button into TerraView toolbar */
//
//	QPtrList< QToolBar > tv_tool_bars_list =
//		plugPars_->qtmain_widget_ptr_->toolBars( Qt::DockTop );
//
//	if( tv_tool_bars_list.count() > 0 )
//	{
//		showPluginWindow_ = new QAction(NULL, "showFTHPluginAction");
//	//    plugin_action_ptr_.reset( new QAction( tv_tool_bars_list.at( 0 ) ) );
//
//		showPluginWindow_->setText( tr("File Theme") );
//		showPluginWindow_->setMenuText( tr("File Theme") );
//		showPluginWindow_->setToolTip( tr("File Theme") );
//		showPluginWindow_->setIconSet( QIconSet( QPixmap(fileTheme_xpm)));
//
//		showPluginWindow_->addTo(plugPars_->qtmain_widget_ptr_->toolBars(Qt::DockTop).at(0));
//
//		connect( showPluginWindow_, SIGNAL( activated() ), this, SLOT( showWindow() ) );
//	}
//
//	QPopupMenu* themeMnu = findThemeMenu();
//
//    if(themeMnu == NULL)
//    {
//        return;
//    }
//
//    mnuIdxs_.push_back(themeMnu->insertSeparator());
//
//    mnuIdxs_.push_back(themeMnu->insertItem(tr("Add File Theme..."), this,
//        SLOT(showWindow())));
//}


void FileTheme::filePushButton_clicked()
{
	fileNameLineEdit->clear();
	QString qfileName = QFileDialog::getOpenFileName(0,
	"Shape Files (*.shp *.SHP)",  this, 0, tr("Open Shape File"));

	if (!qfileName.isEmpty())
		fileNameLineEdit->setText(qfileName);
	else
		return;

	/*int st = qfileName.find('\\');
	if (st == -1)
		st = qfileName.find('/');
	if (st == -1)
		st = 0;
	else
		++st;
	int ed = qfileName.find('.');
	shpThemeNameLineEdit->setText(qfileName.mid(st,ed-st));*/

    std:string themeName = TeGetBaseName(qfileName.latin1());
    shpThemeNameLineEdit->setText(themeName.c_str());

	TeShapefileDriver* shpDriver = new TeShapefileDriver(qfileName.latin1());

	if (!shpDriver->isDataAccessible())
	{
		delete shpDriver;
		QMessageBox::warning(this, tr("Warning"),tr("Can not access the data file!"));
		return;
	}

	fileTheme_ = new TeFileTheme(shpThemeNameLineEdit->text().latin1(), shpDriver, currentDatabase_, 0, viewId_);
	fileTheme_->setThemeProjection(shpDriver->getDataProjection());
	string mess = shpDriver->getDataProjection()->name() + "/" + shpDriver->getDataProjection()->datum().name();
	projLineEdit->setText(mess.c_str());
}


void FileTheme::projPushButton_clicked()
{
    if (fileNameLineEdit->text().isEmpty())
	{
        QMessageBox::warning(this, tr("Warning"),tr("Can not create projection: there's no file selected!"));
		return;
	}

    ProjectionWindow *projectionWindow = new ProjectionWindow(this, "projection", true);

    if (projectionWindow->exec() == QDialog::Accepted)
	{
		TeProjection* proj = projectionWindow->getProjection();
		string mess = proj->name() + "/" + proj->datum().name();
		projLineEdit->setText(mess.c_str());
		if (fileTheme_)
			fileTheme_->setThemeProjection(proj);
	}

	delete projectionWindow;
}


void FileTheme::previewPushButton_clicked()
{
	if (!fileTheme_)
	{
		QMessageBox::warning(this, tr("Warning"),tr("Select a shape file!"));
		return;
	}

	if (!fileTheme_->retrieveDataFromFile())
	{
		QMessageBox::critical(this, tr("Error"), tr("Can not read the data from file."));
		return ;
	}

	unsigned int i=0;
	themePropTable->setText(i, 0, tr("File name:"));
	themePropTable->setText(i, 1, fileNameLineEdit->text());

	themePropTable->setText(1, 0, tr("Number of objects:"));
	QString aux = QString("%1").arg(fileTheme_->getNumberOfObjects());
	themePropTable->setText(1, 1, aux.leftJustify(8));

	TeGeomRep rep = static_cast<TeGeomRep>(fileTheme_->visibleGeoRep());
	if(rep == TePOLYGONS)
	{
		themePropTable->setText(2, 0, tr("Geometry:"));
		themePropTable->setText(2, 1, tr("Polygons"));
	}
	else if(rep == TeLINES)
	{
		themePropTable->setText(2, 0, tr("Geometry:"));
		themePropTable->setText(2, 1, tr("Lines"));
	}
	else if(rep == TePOINTS)
	{
		themePropTable->setText(2, 0, tr("Geometry:"));
		themePropTable->setText(2, 1, tr("Points"));
	}

	string projdatum = fileTheme_->getThemeProjection()->name() + "/" + fileTheme_->getThemeProjection()->datum().name();
	themePropTable->setText(3, 0, tr("Projection"));
	themePropTable->setText(3, 1, projdatum.c_str());

	char fmt[100];
	if (fileTheme_->getThemeProjection()->name() == "NoProjection" ||
		fileTheme_->getThemeProjection()->name() == "LatLong")
		strcpy(fmt,"[%.10f, %.10f, %.10f, %.10f]");
	else
		strcpy(fmt,"[%.3f, %.3f, %.3f, %.3f]");

	QString myValue;
    unsigned int nObjs;
    TeBox bb; 

    fileTheme_->getDataDriver()->getDataInfo(nObjs, bb, rep);
	themePropTable->setText(4, 0,tr("Bounding box:"));
	myValue.sprintf(fmt,bb.x1_,bb.y1_,bb.x2_,bb.y2_);
	themePropTable->setText(4, 1,myValue);
	themePropTable->setNumRows(5);
	themePropTable->adjustColumn(0);
	themePropTable->adjustColumn(1);

	canvas_->plotOnPixmap0();
	canvas_->clearAll();
	canvas_->setPolygonColor(255, 0, 0);
	canvas_->setPolygonStyle(TePolyTypeFill);

	canvas_->setProjection(fileTheme_->getThemeProjection());
	canvas_->setDataProjection(fileTheme_->getThemeProjection());
	canvas_->setWorld(fileTheme_->getThemeBox());

	TeSTElementSet::iterator it = fileTheme_->getData()->begin();
	TeSTElementSet::iterator itend = fileTheme_->getData()->end();
	while (it != itend)
	{
		TeMultiGeometry multiGeo = it->getGeometries();
		if (multiGeo.hasPolygons())
		{
			TePolygonSet& polset = multiGeo.getPolygons();
			for (i=0; i<polset.size(); ++i)
				canvas_->plotPolygon(polset[i]);
		}
		if (multiGeo.hasLines())
		{
			TeLineSet& lineset = multiGeo.getLines();
			for (i=0; i<lineset.size(); ++i)
				canvas_->plotLine(lineset[i]);
		}
		if (multiGeo.hasPoints())
		{
			TePointSet& ptset = multiGeo.getPoints();
			for (i=0; i<ptset.size(); ++i)
				canvas_->plotPoint(ptset[i]);
		}
		++it;
	}
	canvas_->copyPixmapToWindow();
	shpWidgetStack->raiseWidget(1);
}


void FileTheme::okPushButton_clicked()
{
	if (!fileTheme_)
	{
		QMessageBox::warning(this, tr("Warning"),tr("Select a shape file!"));
		return;
	}

	TeViewMap& viewMap = currentDatabase_->viewMap();
	TeViewMap::iterator viewIt;
	TeView* curView = 0;
	for (viewIt = viewMap.begin(); viewIt != viewMap.end(); ++viewIt)
	{
		if (viewIt->second->name() == viewComboBox->currentText().latin1())
		{
			viewId_ = viewIt->second->id();
			curView = viewIt->second;
			break;
		}
	}
	if (!curView)
	{
		QMessageBox::warning(this, tr("Warning"),tr("Select a view!"));
        shpWidgetStack->raiseWidget(0);
        viewComboBox->setFocus();
		return;
	}

    std::string themeName = shpThemeNameLineEdit->text().latin1();
    if(themeName.empty())
    {
        QMessageBox::warning(this, tr("Warning"),tr("Inform a name for the new theme!"));
        shpWidgetStack->raiseWidget(0);
        shpThemeNameLineEdit->setFocus();
		return;
    }

	bool found = false;
	std::vector<TeViewNode*> vecNodes = curView->themes();
	for(unsigned int i = 0; i < vecNodes.size(); ++i)
	{
		TeViewNode* node = vecNodes[i];
		if(TeConvertToLowerCase(node->name()) == TeConvertToLowerCase(themeName))
		{
			found = true;
			break;
		}
	}

	if(found == true)
	{
		QMessageBox::warning(this, tr("Warning"),tr("There is already a theme with the same name in the given View. Choose another name!"));
        shpWidgetStack->raiseWidget(0);
        shpThemeNameLineEdit->setFocus();
		return;
	}


	fileTheme_->name(themeName);
    fileTheme_->view(curView->id());
	fileTheme_->setLocalDatabase(currentDatabase_);
    
	curView->add(fileTheme_);

	if (!fileTheme_->save())
	{
		QString msg = tr("Error inserting theme!") + "\n";
		QMessageBox::critical(this, tr("Error"), msg);
		delete fileTheme_;
		return;
	}

	fileTheme_->parentId(fileTheme_->id());

	currentDatabase_->updateTheme(fileTheme_);

	fileNameLineEdit->clear();
	projLineEdit->clear();
	shpThemeNameLineEdit->clear();
	shpWidgetStack->raiseWidget(0);

	// we don't delete this internal pointers... they should be deleted when the abstract theme is deleted
//	proj_ = 0;
//	fileTheme_ = 0;
//	shpDriver_ = 0;
	accept();
}


void FileTheme::okPrevPushButton_clicked()
{
    okPushButton_clicked();
}


void FileTheme::previousPushButton_clicked()
{
	shpWidgetStack->raiseWidget(0);
}


void FileTheme::updateForm(PluginParameters * pparams)
{
	plugPars_ = pparams;

	currentDatabase_ = plugPars_->getCurrentDatabasePtr();

	// Updating the main window with the new parameters
	if (currentDatabase_ == NULL)
	{
		QMessageBox::critical(plugPars_->qtmain_widget_ptr_, tr("Error"), tr("There is no database connected!"));
		return;
	}
	if (!TeFileTheme::createFileThemeTable(currentDatabase_))
	{
		QMessageBox::critical(plugPars_->qtmain_widget_ptr_, tr("Error"), tr("Can not create metadata table to persist themes from files."));
		return;
	}

	TeViewMap& viewMap = currentDatabase_->viewMap();
	if(viewMap.empty())
	{
		QMessageBox::critical(plugPars_->qtmain_widget_ptr_, tr("Error"), tr("You must create at least one view first."));
		return;
	}

	viewId_ = -1;
	if (plugPars_->getCurrentViewPtr())
		viewId_ = plugPars_->getCurrentViewPtr()->id();
//	proj_ = 0;
	fileTheme_ = 0;
//	shpDriver_ = 0;
	viewComboBox->clear();
	if (currentDatabase_)
	{
		unsigned int i = 0;
		unsigned int curViewItem = 0;
		TeViewMap& viewMap = currentDatabase_->viewMap();
		TeViewMap::iterator viewIt;
		for (viewIt = viewMap.begin(); viewIt != viewMap.end(); ++viewIt)
		{
			viewComboBox->insertItem(viewIt->second->name().c_str());
			if (viewIt->second->id() == viewId_)
				curViewItem = i;
			++i;
		}
		viewComboBox->setCurrentItem(curViewItem);
	}
	canvas_ = new TeQtCanvas(themeFrame);
	canvas_->resize(themeFrame->width(), themeFrame->height());

//	objPropsWindow_ = new AttributesTable(this->plugPars_->qtmain_widget_ptr_);
//	objPropsWindow_->attributesTable->setCaption(tr("Object Information"));
//	objPropsWindow_->attributesTable->setNumCols(2);
//	objPropsWindow_->attributesTable->horizontalHeader()->setLabel(0, tr("Attribute"));
//	objPropsWindow_->attributesTable->horizontalHeader()->setLabel(1, tr("Value"));

//	this->show();
}

//void
//FileTheme::signalHandler(const PluginsSignal& x)
//{
//	if (x.signalType_ == PluginsSignal::S_DRAW_THEME)
//		drawEventHandler(x);
//	else if (x.signalType_ == PluginsSignal::S_POINT_QUERY)
//		pointingEventHandler(x);
//}
//
//void FileTheme::drawEventHandler( const PluginsSignal& x)
//{
//	/* retrieve theme */
//	TeAbstractTheme* theme = 0;
//	if(!x.signalData_.retrive(PluginsSignal::D_ABSTRACT_THEME, theme))
//		return;
//
//	TeFileTheme* fileTheme = dynamic_cast<TeFileTheme*>(theme);
//	if(!fileTheme)
//		return;
//
//	/* retrieve canvas */
//	TeQtCanvas* canvas = 0;
//	if(!x.signalData_.retrive(PluginsSignal::D_QT_CANVAS, canvas))
//		return;
//
//	if(!fileTheme->getData())
//	{
//		if (!fileTheme->getDataDriver())
//		{
//			if (!fileTheme->getFileName().empty())
//			{
//				TeGeoDataDriver* shpDriver = new TeShapefileDriver(fileTheme->getFileName());
//				if (!shpDriver->isDataAccessible())
//				{
//					QMessageBox::warning(0, "Warning","Can not access the data file!");
//					return;
//				}
//				else
//					fileTheme->setDataDriver(shpDriver);
//			}
//			else
//				return;
//		}
//		fileTheme->retrieveDataFromFile();
//		if (!fileTheme->getData())
//		{
//			QMessageBox::warning(0, "Warning","Can not access the data file!");
//			return;
//		}
//	}
//
//	if(fileTheme->minScale() > fileTheme->maxScale())
//	{
//		if(fileTheme->minScale() < canvas->scaleApx() || fileTheme->maxScale() > canvas->scaleApx())
//			return;
//	}
//
//	unsigned int nSteps = fileTheme->getNumberOfObjects();
//	if (nSteps <= 0)
//		return;
//
//	// TODO: how to use own visual...
//	if(fileTheme->visibleRep() & TePOLYGONS)
//	{
//		canvas->setPolygonColor(fileTheme->defaultLegend().visual(TePOLYGONS)->color().red_,
//								fileTheme->defaultLegend().visual(TePOLYGONS)->color().green_,
//								fileTheme->defaultLegend().visual(TePOLYGONS)->color().blue_);
//		canvas->setPolygonStyle((int)fileTheme->defaultLegend().visual(TePOLYGONS)->style(),
//								fileTheme->defaultLegend().visual(TePOLYGONS)->transparency());
//		canvas->setPolygonLineColor (fileTheme->defaultLegend().visual(TePOLYGONS)->contourColor().red_,
//									fileTheme->defaultLegend().visual(TePOLYGONS)->contourColor().green_,
//									fileTheme->defaultLegend().visual(TePOLYGONS)->contourColor().blue_);
//		canvas->setPolygonLineStyle((int)fileTheme->defaultLegend().visual(TePOLYGONS)->contourStyle(),
//									fileTheme->defaultLegend().visual(TePOLYGONS)->contourWidth());
//	}
//
//	if(fileTheme->visibleRep() & TeLINES)
//	{
//		canvas->setLineColor(fileTheme->defaultLegend().visual(TeLINES)->color().red_,
//				                fileTheme->defaultLegend().visual(TeLINES)->color().green_,
//				                fileTheme->defaultLegend().visual(TeLINES)->color().blue_);
//		canvas->setLineStyle((int)fileTheme->defaultLegend().visual(TeLINES)->style(),
//				                fileTheme->defaultLegend().visual(TeLINES)->width());
//	}
//
//	if (fileTheme->visibleRep() & TePOINTS)
//	{
//		canvas->setPointColor(fileTheme->defaultLegend().visual(TePOINTS)->color().red_,
//				                fileTheme->defaultLegend().visual(TePOINTS)->color().green_,
//								fileTheme->defaultLegend().visual(TePOINTS)->color().blue_);
//		canvas->setPointStyle((int)fileTheme->defaultLegend().visual(TePOINTS)->style(),
//				                fileTheme->defaultLegend().visual(TePOINTS)->size());
//	}
//
//	unsigned int i;
//	TeSTElementSet::iterator it = fileTheme->getData()->begin();
//	TeSTElementSet::iterator itSetEnd = fileTheme->getData()->end();
//	while (it != itSetEnd)
//	{
//		TeMultiGeometry multiGeo = it->getGeometries();
//		if (multiGeo.hasPolygons())
//		{
//			TePolygonSet& polset = multiGeo.getPolygons();
//			for (i=0; i<polset.size(); ++i)
//				canvas->plotPolygon(polset[i]);
//		}
//		if (multiGeo.hasLines())
//		{
//			TeLineSet& lineset = multiGeo.getLines();
//			for (i=0; i<lineset.size(); ++i)
//				canvas->plotLine(lineset[i]);
//		}
//		if (multiGeo.hasPoints())
//		{
//			TePointSet& ptset = multiGeo.getPoints();
//			for (i=0; i<ptset.size(); ++i)
//				canvas->plotPoint(ptset[i]);
//		}
//		++it;
//	}
//	canvas->copyPixmapToWindow();
//}
//
//
//void FileTheme::pointingEventHandler( const PluginsSignal& x )
//{
//	/* retrieve theme */
//	TeAbstractTheme* theme = 0;
//	if(!x.signalData_.retrive(PluginsSignal::D_ABSTRACT_THEME, theme))
//		return;
//
//	TeFileTheme* fileTheme = dynamic_cast<TeFileTheme*>(theme);
//	if(!fileTheme)
//		return;
//
//	/* retrieve canvas */
//	TeQtCanvas* canvas = 0;
//	if(!x.signalData_.retrive(PluginsSignal::D_QT_CANVAS, canvas))
//		return;
//
//	/* retrieve the coordinate that was clicked */
//	TeCoord2D pt;
//	if(!x.signalData_.retrive(PluginsSignal::D_COORD2D, pt))
//		return;
//
//	float delta = 0.0;
//	TeCoord2D pdw;
//	if(canvas->projection())
//	{
//		TeProjection* themeProjection = fileTheme->getThemeProjection();
//		if(themeProjection && !(*themeProjection == *(canvas->projection())))
//		{
//			double resolution = canvas->pixelSize();
//			TeCoord2D pl(pt.x()-resolution/2., pt.y()-resolution/2.);
//			TeCoord2D pu(pt.x()+resolution/2., pt.y()+resolution/2.);
//			TeBox	box(pl, pu);
//			box = TeRemapBox(box, canvas->projection(), themeProjection);
//			resolution = box.x2_ - box.x1_;
//			canvas->projection()->setDestinationProjection(themeProjection);
//			pdw = canvas->projection()->PC2LL(pt);
//			pdw = themeProjection->LL2PC(pdw);
//			delta = 3.*resolution;
//		}
//		else
//		{
//			delta = 3.*canvas->pixelSize();
//			pdw = pt;
//		}
//	}
//	else
//	{
//		delta = 3.*canvas->pixelSize();
//		pdw = pt;
//	}
//
//	if (!TeWithin(pdw, fileTheme->getThemeBox()))
//		return;
//
//	if(!fileTheme->getData())
//	{
//		if (!fileTheme->getDataDriver())
//		{
//			if (!fileTheme->getFileName().empty())
//			{
//				TeGeoDataDriver* shpDriver = new TeShapefileDriver(fileTheme->getFileName());
//				if (!shpDriver->isDataAccessible())
//				{
//					QMessageBox::warning(0, "Warning","Can not access the data file!");
//					return;
//				}
//				else
//					fileTheme->setDataDriver(shpDriver);
//			}
//			else
//				return;
//		}
//		fileTheme->retrieveDataFromFile();
//		if (!fileTheme->getData())
//			return;
//	}
//
//	TeBox boxS(pdw.x()-delta, pdw.y()-delta, pdw.x()+delta, pdw.y()+delta);
//	TeSTElementSet* data = fileTheme->getData();
//	TeAttributeList& dAtt = data->getAttributeList();
//	std::vector<TeSTInstance*> result;
//	data->search(boxS,result);
//	if (result.empty())
//		return;
//
//	unsigned int nob = 0;
//	if (result.size() > 1)
//	{
//		unsigned int aux, aux1;
//		for (aux =0; aux<result.size(); ++aux)
//		{
//			TeMultiGeometry mGeom;
//			result[aux]->getGeometry(mGeom);
//
//			vector<TeGeometry*> geoms;
//			mGeom.getGeometry(geoms);
//			bool found=false;
//			for (aux1=0; aux1<geoms.size(); ++aux1)
//			{
//				if (geoms[aux1]->elemType()==TePOLYGONS)
//					found = TeWithin(pdw,*((TePolygon*)geoms[aux1]));
//				else if (geoms[aux1]->elemType()==TeLINES)
//					found = TeTouches(pdw,*((TeLine2D*)geoms[aux1]));
//				else
//					found = true;
//				if (found)
//					break;
//			}
//			if (found)
//			{
//				nob=aux;
//				break;
//			}
//		}
//	}
//
//	std::vector<std::string> values;
//	data->getProperties(result[nob]->objectId(), values);
//
//	objPropsWindow_->attributesTable->setNumRows(dAtt.size());
//	unsigned int i;
//	for(i=0; i<dAtt.size(); ++i)
//	{
//		objPropsWindow_->attributesTable->setText(i, 0, dAtt[i].rep_.name_.c_str());
//		objPropsWindow_->attributesTable->setText(i, 1, values[i+1].c_str());
//	}
//	objPropsWindow_->attributesTable->adjustColumn(0);
//	objPropsWindow_->attributesTable->adjustColumn(1);
//	objPropsWindow_->show();
//	objPropsWindow_->raise();
//
//	// to blink the object...
//	TePolygonSet& polset = result[nob]->getPolygons();
//	if (!polset.empty())
//	{
//		canvas->setPolygonColor(fileTheme->pointingLegend().visual(TePOLYGONS)->color().red_,
//								fileTheme->pointingLegend().visual(TePOLYGONS)->color().green_,
//								fileTheme->pointingLegend().visual(TePOLYGONS)->color().blue_);
//
//		for (i=0; i<polset.size(); ++i)
//			canvas->plotPolygon(polset[i]);
//		canvas->copyPixmapToWindow();
//#ifdef _WINDOWS
//		Sleep(200);
//#endif
//		canvas->setPolygonColor(fileTheme->defaultLegend().visual(TePOLYGONS)->color().red_,
//								fileTheme->defaultLegend().visual(TePOLYGONS)->color().green_,
//								fileTheme->defaultLegend().visual(TePOLYGONS)->color().blue_);
//
//		for (i=0; i<polset.size(); ++i)
//			canvas->plotPolygon(polset[i]);
//		canvas->copyPixmapToWindow();
//
//	}
//	TeLineSet& linset = result[nob]->getLines();
//	if (!linset.empty())
//	{
//		canvas->setLineColor(fileTheme->pointingLegend().visual(TeLINES)->color().red_,
//				                fileTheme->pointingLegend().visual(TeLINES)->color().green_,
//				                fileTheme->pointingLegend().visual(TeLINES)->color().blue_);
//
//		for (i=0; i<linset.size(); ++i)
//			canvas->plotLine(linset[i]);
//		canvas->copyPixmapToWindow();
//#ifdef _WINDOWS
//		Sleep(200);
//#endif
//		canvas->setLineColor(fileTheme->defaultLegend().visual(TeLINES)->color().red_,
//				                fileTheme->defaultLegend().visual(TeLINES)->color().green_,
//				                fileTheme->defaultLegend().visual(TeLINES)->color().blue_);
//		for (i=0; i<linset.size(); ++i)
//			canvas->plotLine(linset[i]);
//		canvas->copyPixmapToWindow();
//
//	}
//	TePointSet& ptset = result[nob]->getPoints();
//	if (!ptset.empty())
//	{
//		canvas->setPointColor(fileTheme->pointingLegend().visual(TePOINTS)->color().red_,
//				                fileTheme->pointingLegend().visual(TePOINTS)->color().green_,
//								fileTheme->pointingLegend().visual(TePOINTS)->color().blue_);
//
//		for (i=0; i<ptset.size(); ++i)
//			canvas->plotPoint(ptset[i]);
//		canvas->copyPixmapToWindow();
//#ifdef _WINDOWS
//		Sleep(200);
//#endif
//		canvas->setPointColor(fileTheme->defaultLegend().visual(TePOINTS)->color().red_,
//				                fileTheme->defaultLegend().visual(TePOINTS)->color().green_,
//								fileTheme->defaultLegend().visual(TePOINTS)->color().blue_);
//		for (i=0; i<ptset.size(); ++i)
//			canvas->plotPoint(ptset[i]);
//		canvas->copyPixmapToWindow();
//
//	}
//	return;
//}
//
//void FileTheme::close()
//{
//	if (fileTheme_)
//	{
//		if (shpDriver_ && (fileTheme_->getDataDriver() != shpDriver_))
//			delete shpDriver_;
//		if (proj_ && (fileTheme_->getThemeProjection() != proj_))
//			delete proj_;
//		delete fileTheme_;
//	}
//	else
//	{
//		if (shpDriver_)
//			delete shpDriver_;
//		if (proj_)
//			delete proj_;
//	}
//	shpDriver_ = 0;
//	proj_ = 0;
//	fileTheme_ = 0;
//
//	fileNameLineEdit->clear();
//	projLineEdit->clear();
//	shpThemeNameLineEdit->clear();
//	shpWidgetStack->raiseWidget(0);
//	if (help_)
//	{
//		delete help_;
//		help_ = 0;
//	}
//	hide();
//}
//
//
//void FileTheme::signalHandler(const PluginsSignal& sig, void* receiver)
//{
//	((FileTheme*)receiver)->signalHandler(sig);
//}

void FileTheme::helpPushButton_clicked()
{
	Help help(this, "help", false);
	help.init("fileTheme.htm");

	if(!help.erro_)
		help.exec();
}

TeAbstractTheme* FileTheme::getCreatedTheme() const
{
	return fileTheme_;
}
//
//QPopupMenu* FileTheme::findThemeMenu()
//{
//	QMenuBar* mnu = plugPars_->qtmain_widget_ptr_->menuBar();
//
//    int idx = -1;
//
//    for(unsigned int i = 0; i < mnu->count() -1; i++)
//    {
//        idx = mnu->idAt(i);
//		if(mnu->text(idx).isEmpty())
//		{
//			continue;
//		}
//
//		if(mnu->text(idx) == tr("&Theme"))
//        {
//            break;
//        }
//    }
//
//    if(idx == -1)
//    {
//        return NULL;
//    }
//
//    return mnu->findItem(idx)->popup();
//}
//
//
//
//
//void FileTheme::showWindow()
//{
//	updateForm(plugPars_);
//}
