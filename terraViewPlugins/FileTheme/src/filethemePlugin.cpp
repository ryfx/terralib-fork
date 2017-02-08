#include <filethemePlugin.h>

//TerraLib include files
#include <TeFileTheme.h>
#include <TeDriverSHPDBF.h>
#include <TeVectorRemap.h>

#include <FileTheme.h>

#include <TeQtThemeItem.h>
#include <TeQtCanvas.h>

#include <attrTable.h>
#include <PluginsSignal.h>
#include <TeAppTheme.h>
#include <TeQtViewsListView.h>

//Qt include files
#include <qaction.h>
#include <qmessagebox.h>
#include <qtable.h>

//XPM include files
#include <fileTheme.xpm>

FileThemePlugin::FileThemePlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params),
objPropsWindow_(0)
{
	loadTranslationFiles("fileTheme_");

	comm_.setHostObj( this, signalHandler );

}

FileThemePlugin::~FileThemePlugin()
{
	end();
}

void FileThemePlugin::init()
{
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(0, "ShowWinAction", false);
			showWinAction_->setMenuText(tr("Add File Theme"));
			showWinAction_->setText( tr("File Theme") );
			showWinAction_->setToolTip( tr("File Theme") );
			showWinAction_->setIconSet( QIconSet( QPixmap(fileTheme_xpm)));

			QPopupMenu* mnu = getPluginsMenu(tr("TerraViewPlugins.Themes").latin1());

			if(mnu != 0)
			{
				if(!showWinAction_->addTo(mnu))
					throw;
			}

			QPtrList< QToolBar > tv_tool_bars_list = tview->toolBars( Qt::DockTop );

			if( tv_tool_bars_list.count() > 0 )
				showWinAction_->addTo(tview->toolBars(Qt::DockTop).at(0));

			connect(showWinAction_, SIGNAL(activated()), this, SLOT(showWindow()));
		}
	}
	catch(...)
	{
		QMessageBox::critical(tview, tr("TerraView plug-in error"), tr("Can't create plug-in menu."));

		delete showWinAction_;
		showWinAction_ = 0;
	}
}

void FileThemePlugin::end()
{
	delete showWinAction_;
	delete objPropsWindow_;
}

void FileThemePlugin::signalHandler( const PluginsSignal& sig, void* receiver)
{
	((FileThemePlugin*)receiver)->signalHandler(sig);
}

void FileThemePlugin::showWindow()
{
	TerraViewBase* tview = getMainWindow();

	TeDatabase* db = params_->getCurrentDatabasePtr();

	if(db == 0)
	{
		QMessageBox::critical(tview, tr("Error"), tr("There is no database connected!"));
		return;
	}

	TeViewMap& viewMap = db->viewMap();

	if(viewMap.empty())
	{
		QMessageBox::critical(tview, tr("Error"), tr("You must create at least one view first."));
		return;
	}

	if (!TeFileTheme::createFileThemeTable(db))
	{
		QMessageBox::critical(tview, tr("TerraView plug-in error"), tr("Can not create metadata table to persist themes from files."));
		return;
	}


	FileTheme win(tview);

	try
	{
		win.updateForm(params_);

		if(win.exec() == QDialog::Accepted)
		{
			QMessageBox::information(tview, tr("Success"), tr("Theme created successfully."));
			TeAbstractTheme* theme = win.getCreatedTheme();

			if(theme == 0)
				throw TeException(UNKNOWN_ERROR_TYPE, tr("Fail to get created theme.").latin1());

			TeViewMap::iterator it = viewMap.find(theme->view());

			TeView* view = (it == viewMap.end()) ? 0 : it->second;

			if(view == 0)
				throw TeException(UNKNOWN_ERROR_TYPE, tr("Fail to get selected view.").latin1());

			TeAppTheme* fftheme = new TeAppTheme(theme);
			fftheme->getTheme()->view(view->id());

			QListViewItem* viewItem = (QListViewItem*)(params_->teqtviewslistview_ptr_->getViewItem(view));

			//Put the new theme on the interface and set it as the current theme
			TeQtThemeItem* themeItem = new TeQtThemeItem (viewItem, theme->name().c_str(),fftheme);
			themeItem->setOn(true);
		}
	}
	catch(TeException& e)
	{
		QMessageBox::critical(tview, tr("Error"), e.message().c_str());
	}
}

void FileThemePlugin::signalHandler( const PluginsSignal& x)
{
	if (x.signalType_ == PluginsSignal::S_DRAW_THEME)
		drawEventHandler(x);
	else if (x.signalType_ == PluginsSignal::S_POINT_QUERY)
		pointingEventHandler(x);
}

void FileThemePlugin::drawEventHandler( const PluginsSignal& x)
{
	/* retrieve theme */
	TeAbstractTheme* theme = 0;
	if(!x.signalData_.retrive(PluginsSignal::D_ABSTRACT_THEME, theme))
		return;

	TeFileTheme* fileTheme = dynamic_cast<TeFileTheme*>(theme);
	if(!fileTheme)
		return;

	/* retrieve canvas */
	TeQtCanvas* canvas = 0;
	if(!x.signalData_.retrive(PluginsSignal::D_QT_CANVAS, canvas))
		return;

	if(!fileTheme->getData())
	{
		if (fileTheme->getDataDriver() == false)
		{
			QMessageBox::warning(0, "Warning","Invalid data driver!");
			return;
		}
		if (fileTheme->getDataDriver()->isDataAccessible() == false)
		{
			QMessageBox::warning(0, "Warning","Can not access the data file!");
			return;
		}

		fileTheme->retrieveDataFromFile();

		if (!fileTheme->getData())
		{
			QMessageBox::warning(0, "Warning","Can not access the data file!");
			return;
		}
	}

	if(fileTheme->minScale() > fileTheme->maxScale())
	{
		if(fileTheme->minScale() < canvas->scaleApx() || fileTheme->maxScale() > canvas->scaleApx())
			return;
	}

	unsigned int nSteps = fileTheme->getNumberOfObjects();
	if (nSteps <= 0)
		return;

	// TODO: how to use own visual...
	if(fileTheme->visibleRep() & TePOLYGONS)
	{
		canvas->setPolygonColor(fileTheme->defaultLegend().visual(TePOLYGONS)->color().red_,
								fileTheme->defaultLegend().visual(TePOLYGONS)->color().green_,
								fileTheme->defaultLegend().visual(TePOLYGONS)->color().blue_);
		canvas->setPolygonStyle((int)fileTheme->defaultLegend().visual(TePOLYGONS)->style(),
								fileTheme->defaultLegend().visual(TePOLYGONS)->transparency());
		canvas->setPolygonLineColor (fileTheme->defaultLegend().visual(TePOLYGONS)->contourColor().red_,
									fileTheme->defaultLegend().visual(TePOLYGONS)->contourColor().green_,
									fileTheme->defaultLegend().visual(TePOLYGONS)->contourColor().blue_);
		canvas->setPolygonLineStyle((int)fileTheme->defaultLegend().visual(TePOLYGONS)->contourStyle(),
									fileTheme->defaultLegend().visual(TePOLYGONS)->contourWidth());
	}

	if(fileTheme->visibleRep() & TeLINES)
	{
		canvas->setLineColor(fileTheme->defaultLegend().visual(TeLINES)->color().red_,
				                fileTheme->defaultLegend().visual(TeLINES)->color().green_,
				                fileTheme->defaultLegend().visual(TeLINES)->color().blue_);
		canvas->setLineStyle((int)fileTheme->defaultLegend().visual(TeLINES)->style(),
				                fileTheme->defaultLegend().visual(TeLINES)->width());
	}

	if (fileTheme->visibleRep() & TePOINTS)
	{
		canvas->setPointColor(fileTheme->defaultLegend().visual(TePOINTS)->color().red_,
				                fileTheme->defaultLegend().visual(TePOINTS)->color().green_,
								fileTheme->defaultLegend().visual(TePOINTS)->color().blue_);
		canvas->setPointStyle((int)fileTheme->defaultLegend().visual(TePOINTS)->style(),
				                fileTheme->defaultLegend().visual(TePOINTS)->size());
	}

	unsigned int i;
	TeSTElementSet::iterator it = fileTheme->getData()->begin();
	TeSTElementSet::iterator itSetEnd = fileTheme->getData()->end();
	while (it != itSetEnd)
	{
		TeMultiGeometry multiGeo = it->getGeometries();
		if (multiGeo.hasPolygons())
		{
			TePolygonSet& polset = multiGeo.getPolygons();
			for (i=0; i<polset.size(); ++i)
				canvas->plotPolygon(polset[i]);
		}
		if (multiGeo.hasLines())
		{
			TeLineSet& lineset = multiGeo.getLines();
			for (i=0; i<lineset.size(); ++i)
				canvas->plotLine(lineset[i]);
		}
		if (multiGeo.hasPoints())
		{
			TePointSet& ptset = multiGeo.getPoints();
			for (i=0; i<ptset.size(); ++i)
				canvas->plotPoint(ptset[i]);
		}
		++it;
	}
	canvas->copyPixmapToWindow();
}


void FileThemePlugin::pointingEventHandler( const PluginsSignal& x )
{
	/* retrieve theme */
	TeAbstractTheme* theme = 0;
	if(!x.signalData_.retrive(PluginsSignal::D_ABSTRACT_THEME, theme))
		return; 

	TeFileTheme* fileTheme = dynamic_cast<TeFileTheme*>(theme);
	if(!fileTheme)
		return;

	/* retrieve canvas */
	TeQtCanvas* canvas = 0;
	if(!x.signalData_.retrive(PluginsSignal::D_QT_CANVAS, canvas))
		return;

	/* retrieve the coordinate that was clicked */
	TeCoord2D pt;
	if(!x.signalData_.retrive(PluginsSignal::D_COORD2D, pt))
		return;

	float delta = 0.0;
	TeCoord2D pdw;
	if(canvas->projection())
	{
		TeProjection* themeProjection = fileTheme->getThemeProjection();
		if(themeProjection && !(*themeProjection == *(canvas->projection())))
		{
			double resolution = canvas->pixelSize();
			TeCoord2D pl(pt.x()-resolution/2., pt.y()-resolution/2.);
			TeCoord2D pu(pt.x()+resolution/2., pt.y()+resolution/2.);
			TeBox	box(pl, pu);
			box = TeRemapBox(box, canvas->projection(), themeProjection);
			resolution = box.x2_ - box.x1_;
			canvas->projection()->setDestinationProjection(themeProjection);
			pdw = canvas->projection()->PC2LL(pt);
			pdw = themeProjection->LL2PC(pdw);
			delta = 3.*resolution;
		}
		else
		{
			delta = 3.*canvas->pixelSize();
			pdw = pt;
		}
	}
	else
	{
		delta = 3.*canvas->pixelSize();
		pdw = pt;
	}

	if (!TeWithin(pdw, fileTheme->getThemeBox()))
		return;

	if(!fileTheme->getData())
	{
		if (fileTheme->getDataDriver() == false)
		{
			QMessageBox::warning(0, "Warning","Invalid data driver!");
			return;
		}
		if (fileTheme->getDataDriver()->isDataAccessible() == false)
		{
			QMessageBox::warning(0, "Warning","Can not access the data file!");
			return;
		}

		fileTheme->retrieveDataFromFile();

		if (!fileTheme->getData())
		{
			QMessageBox::warning(0, "Warning","Can not access the data file!");
			return;
		}
	}

	TeBox boxS(pdw.x()-delta, pdw.y()-delta, pdw.x()+delta, pdw.y()+delta);
	TeSTElementSet* data = fileTheme->getData();
	TeAttributeList& dAtt = data->getAttributeList();
	std::vector<TeSTInstance*> result;
	data->search(boxS,result);
	if (result.empty())
		return;

	unsigned int nob = 0;
	if (result.size() > 1)
	{
		unsigned int aux, aux1;
		for (aux =0; aux<result.size(); ++aux)
		{
			TeMultiGeometry mGeom;
			result[aux]->getGeometry(mGeom);

			vector<TeGeometry*> geoms;
			mGeom.getGeometry(geoms);
			bool found=false;
			for (aux1=0; aux1<geoms.size(); ++aux1)
			{
				if (geoms[aux1]->elemType()==TePOLYGONS)
					found = TeWithin(pdw,*((TePolygon*)geoms[aux1]));
				else if (geoms[aux1]->elemType()==TeLINES)
					found = TeTouches(pdw,*((TeLine2D*)geoms[aux1]));
				else
					found = true;
				if (found)
					break;
			}
			if (found)
			{
				nob=aux;
				break;
			}
		}
	}

	std::vector<std::string> values;
	data->getProperties(result[nob]->objectId(), values);

	if(objPropsWindow_ != 0)
		delete objPropsWindow_;

	objPropsWindow_ = new AttributesTable(params_->qtmain_widget_ptr_);
	objPropsWindow_->attributesTable->setCaption(tr("Object Information"));
	objPropsWindow_->attributesTable->setNumCols(2);
	objPropsWindow_->attributesTable->horizontalHeader()->setLabel(0, tr("Attribute"));
	objPropsWindow_->attributesTable->horizontalHeader()->setLabel(1, tr("Value"));

	objPropsWindow_->attributesTable->setNumRows(dAtt.size());
	unsigned int i;

	for(i=0; i<dAtt.size(); ++i)
	{
		objPropsWindow_->attributesTable->setText(i, 0, dAtt[i].rep_.name_.c_str());
		objPropsWindow_->attributesTable->setText(i, 1, values[i+1].c_str());
	}

	objPropsWindow_->attributesTable->adjustColumn(0);
	objPropsWindow_->attributesTable->adjustColumn(1);
	objPropsWindow_->show();
	objPropsWindow_->raise();


	// to blink the object...
	TePolygonSet& polset = result[nob]->getPolygons();
	if (!polset.empty())
	{
		canvas->setPolygonColor(fileTheme->pointingLegend().visual(TePOLYGONS)->color().red_,
								fileTheme->pointingLegend().visual(TePOLYGONS)->color().green_,
								fileTheme->pointingLegend().visual(TePOLYGONS)->color().blue_);

		for (i=0; i<polset.size(); ++i)
			canvas->plotPolygon(polset[i]);
		canvas->copyPixmapToWindow();
#ifdef _WINDOWS
		Sleep(200);
#endif
		canvas->setPolygonColor(fileTheme->defaultLegend().visual(TePOLYGONS)->color().red_,
								fileTheme->defaultLegend().visual(TePOLYGONS)->color().green_,
								fileTheme->defaultLegend().visual(TePOLYGONS)->color().blue_);

		for (i=0; i<polset.size(); ++i)
			canvas->plotPolygon(polset[i]);
		canvas->copyPixmapToWindow();

	}
	TeLineSet& linset = result[nob]->getLines();
	if (!linset.empty())
	{
		canvas->setLineColor(fileTheme->pointingLegend().visual(TeLINES)->color().red_,
				                fileTheme->pointingLegend().visual(TeLINES)->color().green_,
				                fileTheme->pointingLegend().visual(TeLINES)->color().blue_);

		for (i=0; i<linset.size(); ++i)
			canvas->plotLine(linset[i]);
		canvas->copyPixmapToWindow();
#ifdef _WINDOWS
		Sleep(200);
#endif
		canvas->setLineColor(fileTheme->defaultLegend().visual(TeLINES)->color().red_,
				                fileTheme->defaultLegend().visual(TeLINES)->color().green_,
				                fileTheme->defaultLegend().visual(TeLINES)->color().blue_);
		for (i=0; i<linset.size(); ++i)
			canvas->plotLine(linset[i]);
		canvas->copyPixmapToWindow();

	}
	TePointSet& ptset = result[nob]->getPoints();
	if (!ptset.empty())
	{
		canvas->setPointColor(fileTheme->pointingLegend().visual(TePOINTS)->color().red_,
				                fileTheme->pointingLegend().visual(TePOINTS)->color().green_,
								fileTheme->pointingLegend().visual(TePOINTS)->color().blue_);

		for (i=0; i<ptset.size(); ++i)
			canvas->plotPoint(ptset[i]);
		canvas->copyPixmapToWindow();
#ifdef _WINDOWS
		Sleep(200);
#endif
		canvas->setPointColor(fileTheme->defaultLegend().visual(TePOINTS)->color().red_,
				                fileTheme->defaultLegend().visual(TePOINTS)->color().green_,
								fileTheme->defaultLegend().visual(TePOINTS)->color().blue_);
		for (i=0; i<ptset.size(); ++i)
			canvas->plotPoint(ptset[i]);
		canvas->copyPixmapToWindow();

	}
	return;
}

