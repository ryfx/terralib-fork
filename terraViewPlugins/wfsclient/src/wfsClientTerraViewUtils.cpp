#include <wfsClientTerraViewUtils.h>
#include <PluginsSignal.h>
#include <PluginParameters.h>

#include <wfsThemeMenu.h>
#include <wfsattr.h>

//TerraOGC include files
#include <common/TeOGCException.h>

#include <owstheme/TeOWSThemeServiceInfoManager.h>
#include <owstheme/TeOWSThemeServiceInfo.h>
#include <owstheme/TeOWSThemeServiceType.h>
#include <owstheme/TeOWSThemeServiceOperation.h>

#include <wfs/TeWFSCapabilities.h>
#include <wfs/TeWFSService.h>

#include <wfstheme/TeWFSTheme.h>

//TerraView include files
#include <TeQtThemeItem.h>
#include <TeAppTheme.h>
#include <TeQtCanvas.h>
#include <terraViewBase.h>

//TerraLib include files
#include <TeDatabase.h>
#include <TeWaitCursor.h>
#include <TeVectorRemap.h>
#include <TeSTElementSet.h>

//Qt3 include files
#include <qmessagebox.h>
#include <qwidget.h>
#include <qimage.h>

//Images
#include <images/wmsTheme.xpm>

#define MESSAGEBOXTITLE "WFS Client Plugin"

static PluginParameters* plug_pars_ptr = 0;
static WFSAttr* att_dlg = 0;

void createOWSTables(TeDatabase* db)
{
	TeOGC::TeOWSThemeServiceOperation::createServiceOperationTable(db);
	TeOGC::TeOWSThemeServiceInfo::createServiceInfoTable(db);
	TeOGC::TeOWSThemeServiceInfoOperation::createServiceInfoOperationTable(db);
	TeOGC::TeOWSThemeServiceType::createServiceTypeTable(db);
}

void drawObjects(TeSTElementSet* elemSet, TeQtCanvas* canvas, TeLegendEntry leg, TeProjection* thProj)
{
	if(elemSet != 0)
	{
		TeSTElementSet::iterator it = elemSet->begin();
		TeSTElementSet::iterator itend = elemSet->end();

		while (it != itend)
		{
			TeMultiGeometry multiGeo = it->getGeometries();

			if (multiGeo.hasPolygons())
			{
				canvas->setPolygonColor(leg.visual(TePOLYGONS)->color().red_,
										leg.visual(TePOLYGONS)->color().green_,
										leg.visual(TePOLYGONS)->color().blue_);
				canvas->setPolygonStyle((int)leg.visual(TePOLYGONS)->style(),
										leg.visual(TePOLYGONS)->transparency());
				canvas->setPolygonLineColor (leg.visual(TePOLYGONS)->contourColor().red_,
											leg.visual(TePOLYGONS)->contourColor().green_,
											leg.visual(TePOLYGONS)->contourColor().blue_);
				canvas->setPolygonLineStyle((int)leg.visual(TePOLYGONS)->contourStyle(),
											leg.visual(TePOLYGONS)->contourWidth());

				TePolygonSet polset;

				TePolygonSet::iterator pIt;

                                if(thProj != 0)	TeVectorRemap<TePolygonSet>(multiGeo.getPolygons(), thProj, polset, canvas->projection());
                                else            polset = multiGeo.getPolygons();


				for(pIt = polset.begin(); pIt != polset.end(); ++pIt)
					canvas->plotPolygon(*pIt);
			}
			if (multiGeo.hasLines())
			{
				canvas->setLineColor(leg.visual(TeLINES)->color().red_,
										leg.visual(TeLINES)->color().green_,
										leg.visual(TeLINES)->color().blue_);
				canvas->setLineStyle((int)leg.visual(TeLINES)->style(),
										leg.visual(TeLINES)->width());

				TeLineSet lset;

				TeLineSet::iterator lIt;

                                if (thProj != 0)    TeVectorRemap<TeLineSet>(multiGeo.getLines(), thProj, lset, canvas->projection());
                                else                lset = multiGeo.getLines();

				for(lIt = lset.begin(); lIt != lset.end(); ++lIt)
					canvas->plotLine(*lIt);
			}
			if (multiGeo.hasPoints())
			{
				canvas->setPointColor(leg.visual(TePOINTS)->color().red_,
										leg.visual(TePOINTS)->color().green_,
										leg.visual(TePOINTS)->color().blue_);
				canvas->setPointStyle((int)leg.visual(TePOINTS)->style(),
										leg.visual(TePOINTS)->size());

				TePointSet pset;

				TePointSet::iterator pIt;

                                if (thProj != 0)    TeVectorRemap<TePointSet>(multiGeo.getPoints(), thProj, pset, canvas->projection());
                                else                pset = multiGeo.getPoints();

				for(pIt = pset.begin(); pIt != pset.end(); ++pIt)
					canvas->plotPoint(*pIt);
			}
			++it;
		}

		canvas->copyPixmapToWindow();
	}
}

void WFSSetPluginParameters(PluginParameters* p)
{
	plug_pars_ptr = p;
}

void getAttributeDialog(QWidget* w)
{
	if(att_dlg != 0)
		delete att_dlg;

	att_dlg = new WFSAttr(w);
}

void cleanAttrDialogPtr()
{
	delete att_dlg;
	att_dlg = 0;
}

void WFSClientPluginSignalHandler(const PluginsSignal& x, void* objptr)
{
	if(x.signalType_ == PluginsSignal::S_DRAW_THEME)
	{
		WFSClientPluginDrawTheme(x);
	}
	else if(x.signalType_ == PluginsSignal::S_SET_QTTHEMEICON)
	{
		WFSClientPluginSetThemeIcon(x);
	}
	else if(x.signalType_ == PluginsSignal::S_SET_DATABASE)
	{
		WFSClientPluginSetDatabase(x);
	}
	else if(x.signalType_ == PluginsSignal::S_SHOW_THEME_POPUP)
	{
		WFSClientPluginShowThemePopup(x);
	}
	else if(x.signalType_ == PluginsSignal::S_POINT_QUERY)
	{
		WFSClientPluginPointQuery(x);
	}
}

void WFSClientPluginDrawTheme(const PluginsSignal& x)
{
	TeWaitCursor waitcursor;

/* retrieve theme */
	TeAbstractTheme* theme = 0;

		/* retrieve canvas */
	TeQtCanvas* canvas = 0;
	if(!x.signalData_.retrive(PluginsSignal::D_QT_CANVAS, canvas))
		return;

	if(!x.signalData_.retrive(PluginsSignal::D_ABSTRACT_THEME, theme))
	{
		waitcursor.resetWaitCursor();
		QMessageBox::critical(0, MESSAGEBOXTITLE, QObject::tr("The signal received is not about a WMS theme!"));
		return;
	}

	if(!theme)
	{
		waitcursor.resetWaitCursor();
		QMessageBox::critical(0, MESSAGEBOXTITLE, QObject::tr("Didn't receive a valid theme!"));
		return;
	}

	if(theme->type() != TeWFSTHEME)
		return;

	if(theme->minScale() > theme->maxScale())
		if(theme->minScale() < canvas->scaleApx() || theme->maxScale() > canvas->scaleApx())
			return;

	//Change from canvas projetion
	TeProjection* cvProj = canvas->projection();
	TeProjection* thProj = theme->getThemeProjection();

	if(thProj == 0)
	{
		thProj = new TeNoProjection();
		((TeOGC::TeWFSTheme*)theme)->setThemeProjection(thProj);
	}

	bool hasToChange = !((*cvProj) == (*thProj));

	TeBox bx = canvas->getWorld();

	bx = TeRemapBox(bx, cvProj, thProj);

	if(!TeIntersects(bx, theme->getThemeBox()))
		return;

	TeSTElementSet* elemSet = 0;

	try
	{
		elemSet = ((TeOGC::TeWFSTheme*)theme)->getDataSet(bx, 1000, false);

		if(elemSet == 0 || elemSet->getSTInstances().size() <= 0)
		{
			delete elemSet;
			return;
		}

		TeLegendEntry leg = theme->defaultLegend();

		drawObjects(elemSet, canvas, leg, (hasToChange) ? cvProj : 0);

		canvas->copyPixmap0ToPixmap1();

		delete elemSet;
	}
	catch(TeException& e)
	{
		waitcursor.resetWaitCursor();
		QMessageBox::critical(0, MESSAGEBOXTITLE, e.message().c_str());
		return;
	}

	waitcursor.resetWaitCursor();
}

/* Funcao de call-back para sinalizacao de alteracao no banco de dados: forma de receber sinais do TerraView. */
void WFSClientPluginSetDatabase(const PluginsSignal& x)
{
/* retrieve database */
	TeDatabase* db = 0;

	if(!x.signalData_.retrive(PluginsSignal::D_DATABASE, db))
	{
		QMessageBox::critical(0, MESSAGEBOXTITLE, QObject::tr("The received signal is not about a database connection!"));
		return;
	}

	if(!db)
		return;

/* Clear cache */
	TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().finalize();

/* try to create OWS client specific tables */
	try
	{
		createOWSTables(db);
	}
	catch(TeException& e)
	{
		QMessageBox::critical(0, MESSAGEBOXTITLE, e.message().c_str());
		return;
	}

/* try to create WMS client specific tables */
	TeOGC::TeWFSTheme::createThemeTable(db);
	TeOGC::TeWFSTheme::insertWFSOperations(db);
	TeOGC::TeWFSTheme::insertWFSServiceType(db);

/* try to load OWS client specific metadata */
	TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().initialize(db);
}

void WFSClientPluginPointQuery(const PluginsSignal& x)
{
	TeWaitCursor waitcursor;

/* retrieve theme */
	TeAbstractTheme* theme = 0;

		/* retrieve canvas */
	TeQtCanvas* canvas = 0;
	if(!x.signalData_.retrive(PluginsSignal::D_QT_CANVAS, canvas))
		return;

	if(!x.signalData_.retrive(PluginsSignal::D_ABSTRACT_THEME, theme))
	{
		waitcursor.resetWaitCursor();
		QMessageBox::critical(0, MESSAGEBOXTITLE, QObject::tr("The signal received is not about a WMS theme!"));
		return;
	}

	if(!theme)
	{
		waitcursor.resetWaitCursor();
		QMessageBox::critical(0, MESSAGEBOXTITLE, QObject::tr("Didn't receive a valid theme!"));
		return;
	}

	if(theme->type() != TeWFSTHEME)
		return;

	if(theme->minScale() > theme->maxScale())
		if(theme->minScale() < canvas->scaleApx() || theme->maxScale() > canvas->scaleApx())
			return;

	//Change from canvas projetion
	TeProjection* cvProj = canvas->projection();
	TeProjection* thProj = theme->getThemeProjection();

	if(thProj == 0)
	{
		thProj = new TeNoProjection();
		((TeOGC::TeWFSTheme*)theme)->setThemeProjection(thProj);
	}

	TePrecision::instance().setPrecision(TeGetPrecision(thProj));

/* retrieve location */
	TeCoord2D location;

	if(!x.signalData_.retrive(PluginsSignal::D_COORD2D, location))
	{
		waitcursor.resetWaitCursor();
		QMessageBox::critical(0, MESSAGEBOXTITLE, "The signal received is not about a location!");
		return;
	}

	bool hasToChange = !((*cvProj) == (*thProj));

	TeBox bx(location.x(), location.y(), location.x(), location.y());

	bx = TeRemapBox(bx, cvProj, thProj);

	double offset = canvas->mapVtoDW(3);
//	canvas->mapDWtoV(bx);

	bx.x1_ -= offset;
	bx.y1_ -= offset;
	bx.x2_ += offset;
	bx.y2_ += offset;

//	canvas->mapVtoCW(bx);

	if(!TeIntersects(bx, theme->getThemeBox()))
		return;

	TeSTElementSet* elemSet = 0;

	try
	{
		TeOGC::TeWFSTheme* wTheme = (TeOGC::TeWFSTheme*)theme;

		elemSet = wTheme->getDataSet(bx);

		if(elemSet == 0 || elemSet->getSTInstances().size() <= 0)
			throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("No object selected from server.").latin1());

		TeLegendEntry leg = theme->pointingLegend();

		canvas->copyPixmap1ToPixmap0();

		drawObjects(elemSet, canvas, leg, (hasToChange) ? cvProj : 0);

		TerraViewBase* tview = dynamic_cast<TerraViewBase*>(plug_pars_ptr->qtmain_widget_ptr_);

		getAttributeDialog(tview);

		att_dlg->setElementSet(elemSet);

		att_dlg->show();

		delete elemSet;

		waitcursor.resetWaitCursor();
	}
	catch(TeException& e)
	{
		cleanAttrDialogPtr();
		delete elemSet;

		waitcursor.resetWaitCursor();
		QMessageBox::critical(0, MESSAGEBOXTITLE, e.message().c_str());

		return;
	}
}

void WFSClientPluginShowThemePopup(const PluginsSignal& x)
{
	int themeType = 0;

	if(!x.signalData_.retrive(PluginsSignal::D_THEMETYPE, themeType))
	{
		return;
	}

	if(themeType != TeWFSTHEME)
		return;

	/* retrieve Qt theme */
	TeQtThemeItem* qtThemeItem = 0;

	if(!x.signalData_.retrive(PluginsSignal::D_QT_THEMEITEM, qtThemeItem))
	{
		QMessageBox::critical(0, MESSAGEBOXTITLE, "The signal received is not about a Qt Theme Item!");
		return;
	}

	if(!qtThemeItem)
	{
		QMessageBox::critical(0, MESSAGEBOXTITLE, "Didn't receive a valid Qt Theme Item!");
		return;
	}

/* retrieve location */
	TeCoord2D location;

	if(!x.signalData_.retrive(PluginsSignal::D_COORD2D, location))
	{
		QMessageBox::critical(0, MESSAGEBOXTITLE, "The signal received is not about a location!");
		return;
	}

	TerraViewBase* tview = dynamic_cast<TerraViewBase*>(plug_pars_ptr->qtmain_widget_ptr_);

	WFSPopupMenu mnu(tview);

	mnu.move(location.x(), location.y());

	mnu.setListViewItem(qtThemeItem);

	if(tview == 0)
		return;

	mnu.exec();
}

void WFSClientPluginSetThemeIcon(const PluginsSignal& x)
{
/* retrieve Qt theme */
	TeQtThemeItem* qtThemeItem = 0;

	if(!x.signalData_.retrive(PluginsSignal::D_QT_THEMEITEM, qtThemeItem))
	{
		QMessageBox::critical(0, MESSAGEBOXTITLE, "The signal received is not about a Qt Theme Item!");
		return;
	}

	if(!qtThemeItem)
	{
		QMessageBox::critical(0, MESSAGEBOXTITLE, "Didn't receive a valid Qt Theme Item!");
		return;
	}

/* set theme icon if theme is a WFS */
	if(qtThemeItem->getAppTheme()->getTheme()->type() == TeWFSTHEME)
        qtThemeItem->setPixmap(0, QPixmap(wmsTheme_xpm));
}
