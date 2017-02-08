// TerraView WMS Client Plugin include files
#include "WMSClientTerraViewUtils.h"
#include "WMSClientPluginUtils.h"
#include "WMSFeatureInfo.h"
#include "WMSPopupWidget.h"

// TerraOGC WMS include files
#include <wms/TeWMSGetMapResponse.h>
#include <wms/TeWMSGetFeatureInfoResponse.h>
#include <wms/TeWMSGetCapabilitiesResponse.h>
#include <wms/TeWMSServiceSection.h>
#include <wms/TeWMSLayerSection.h>
#include <wms/TeWMSOnlineResource.h>
#include <wms/TeWMSKeywordList.h>
#include <wms/TeWMSKeyword.h>
#include <wms/TeWMSBoundingBox.h>
#include <wms/TeWMSException.h>
#include <wms/TeWMSOperationType.h>
#include <wms/TeWMSEX_GeographicBoundingBox.h>
#include <wms/TeWMSStyle.h>

// TerraOGC OWS Theme include files
#include <owstheme/TeOWSThemeServiceInfoManager.h>
#include <owstheme/TeOWSThemeServiceInfo.h>
#include <owstheme/TeOWSThemeServiceType.h>
#include <owstheme/TeOWSThemeServiceOperation.h>

// TerraOGC WMS Theme include files
#include <wmstheme/TeWMSTheme.h>

// TerraOGC OWS Theme include files
#include <ows/TeOWSMimeType.h>

// TerraOGC Common Theme include files
#include <common/TeOGCProjection.h>

// TerraLib include files
#include <TeUtils.h>
#include <TeAbstractTheme.h>
#include <TeView.h>
#include <TeVectorRemap.h>

// TerraView Support include files
#include <PluginBase.h>
#include <PluginsSignal.h>
#include <TeQtCanvas.h>
#include <TeQtThemeItem.h>
#include <TeWaitCursor.h>
#include <TeAppTheme.h>

// Qt include files
#include <qlistview.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qtable.h>
#include <qpoint.h>
#include <qimage.h>
#include <qcombobox.h>

// XPM image include files
#include "wmsTheme.xpm"

#define MESSAGEBOXTITLE "WMS Client Plugin"

/* This pointer will keep track of TerraView environment - POG */
static PluginParameters* plug_pars_ptr = 0;

static std::vector<int> mnuIdxs_;

void WMSSetPluginParameters(PluginParameters* p)
{
	plug_pars_ptr = p;
}

PluginParameters* WMSGetPluginParameters()
{
	return plug_pars_ptr;
}


void WMSClientPluginSignalHandler(const PluginsSignal& x, void* objptr)
{
	if(x.signalType_ == PluginsSignal::S_DRAW_THEME)
	{
		WMSClientPluginDrawTheme(x);
	}
	else if(x.signalType_ == PluginsSignal::S_SET_QTTHEMEICON)
	{
		WMSClientPluginSetThemeIcon(x);
	}
	else if(x.signalType_ == PluginsSignal::S_SET_DATABASE)
	{
		WMSClientPluginSetDatabase(x);
	}
	else if(x.signalType_ == PluginsSignal::S_SHOW_THEME_POPUP)
	{
		WMSClientPluginShowThemePopup(x);
	}
	else if(x.signalType_ == PluginsSignal::S_POINT_QUERY)
	{
		WMSClientPluginPointQuery(x);
	}
}

void WMSClientPluginDrawTheme(const PluginsSignal& x)
{
/* retrieve theme */
	TeAbstractTheme* theme = 0;

	if(!x.signalData_.retrive(PluginsSignal::D_ABSTRACT_THEME, theme))
	{
		QMessageBox::critical(0, MESSAGEBOXTITLE, "The signal received is not about a WMS theme!");
		return;
	}

	if(!theme)
		QMessageBox::critical(0, MESSAGEBOXTITLE, "Didn't receive a valid theme!");

	if(theme->type() != TeWMSTHEME)
		return;

	TeOGC::TeWMSTheme* wmstheme = dynamic_cast<TeOGC::TeWMSTheme*>(theme);

	if(!wmstheme)
	{
		QMessageBox::critical(0, MESSAGEBOXTITLE, "Could not convert abstract theme to wms theme!");
		return;
	}

/* retrieve canvas */
	TeQtCanvas* canvas = 0;

	if(!x.signalData_.retrive(PluginsSignal::D_QT_CANVAS, canvas))
	{
		QMessageBox::critical(0, MESSAGEBOXTITLE, "The signal received is not about a Qt canvas!");
		return;
	}

	TeOGC::TeWMSGetMapResponse* map = 0;

	try
	{
		TeDatabase* db = wmstheme->getLocalDatabase();

		if(db == NULL)
		{
			throw;
		}

		TeBox canvasBox = canvas->getDataWorld();

		TeProjection* fromProj = canvas->projection();
		TeProjection* toProj = wmstheme->getThemeProjection();

		if (fromProj == NULL ||  toProj == NULL)
		{
			return;
		}

		if (!(*fromProj == *toProj))
		{
			canvasBox = TeRemapBox(canvasBox, fromProj, toProj);
		}

		std::string crs = "EPSG:" + Te2String(TeOGC::TeGetEPSGFromTeProjection(toProj));
		map = wmstheme->getMap(crs, canvasBox, canvas->width(), canvas->height());

		QImage ima;
		ima.fill(QColor(255, 255, 255).pixel());
		std::string imgtype = TeConvertToUpperCase(map->getMimeType()->getSubType());
		ima.loadFromData((const unsigned char*)(map->getData()), map->getDataSize(), imgtype.c_str());

		canvas->plotOnPixmap0();
		QPainter* painter = canvas->getPainter();
		painter->drawImage(0, 0, ima);
		canvas->copyPixmap0ToWindow();
	}
	catch(TeOGC::TeOGCException& e)
	{
		delete map;
		QMessageBox::critical(0, MESSAGEBOXTITLE, e.getErrorMessage().c_str());
		return;
	}
	catch(...)
	{
		delete map;
		QMessageBox::critical(0, MESSAGEBOXTITLE, "Unexpected error during GetMap request.");
		return;
	}
}

/* Funcao de call-back para sinalizacao de alteracao no banco de dados: forma de receber sinais do TerraView. */
void WMSClientPluginSetDatabase(const PluginsSignal& x)
{
/* retrieve database */
	TeDatabase* db = 0;

	if(!x.signalData_.retrive(PluginsSignal::D_DATABASE, db))
	{
		QMessageBox::critical(0, MESSAGEBOXTITLE, "The received signal is not about a database connection!");
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
	//TeOGC::TeOWSThemeServiceType::createServiceTypeTable(db);
	//TeOGC::TeOWSThemeServiceOperation::createServiceOperationTable(db);
	//TeOGC::TeOWSThemeServiceInfo::createServiceInfoTable(db);
	//TeOGC::TeOWSThemeServiceInfoOperation::createServiceInfoOperationTable(db);

/* try to create WMS client specific tables */
	TeOGC::TeWMSTheme::createWMSThemeTable(db);
	TeOGC::TeWMSTheme::insertWMSOperations(db);
	TeOGC::TeWMSTheme::insertWMSServiceType(db);

/* try to load OWS client specific metadata */
	TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().initialize(db);
}

void WMSClientPluginPointQuery(const PluginsSignal& x)
{
	TeWaitCursor waitcursor;

/* retrieve theme */
	TeAbstractTheme* theme = 0;

	if(!x.signalData_.retrive(PluginsSignal::D_ABSTRACT_THEME, theme))
	{
		waitcursor.resetWaitCursor();
		QMessageBox::critical(0, MESSAGEBOXTITLE, "The signal received is not about a WMS theme!");
		return;
	}

	if(!theme)
	{
		waitcursor.resetWaitCursor();
		QMessageBox::critical(0, MESSAGEBOXTITLE, "Didn't receive a valid theme!");
		return;
	}

	if(theme->type() != TeWMSTHEME)
		return;

	TeOGC::TeWMSTheme* wmstheme = dynamic_cast<TeOGC::TeWMSTheme*>(theme);

	if(!wmstheme)
	{
		waitcursor.resetWaitCursor();
		QMessageBox::critical(0, MESSAGEBOXTITLE, "Could not convert abstract theme to wms theme!");
		return;
	}

/* check if server support get feature info */
	if(wmstheme->getFeatureInfoFormat().empty())
	{
		waitcursor.resetWaitCursor();
		QMessageBox::warning(0, MESSAGEBOXTITLE, "The server of this theme doesn't support Get Feature Information.");
		return;
	}


/* retrieve canvas */
	TeQtCanvas* canvas = 0;

	if(!x.signalData_.retrive(PluginsSignal::D_QT_CANVAS, canvas))
	{
		waitcursor.resetWaitCursor();
		QMessageBox::critical(0, MESSAGEBOXTITLE, "The signal received is not about a Qt canvas!");
		return;
	}

/* retrieve location */
	TeCoord2D location;

	if(!x.signalData_.retrive(PluginsSignal::D_COORD2D, location))
	{
		waitcursor.resetWaitCursor();
		QMessageBox::critical(0, MESSAGEBOXTITLE, "The signal received is not about a location!");
		return;
	}

	QPoint pt = canvas->mapDWtoV(location);

/* get information */
	TeBox canvasBox = canvas->getDataWorld();

	TeProjection* fromProj = canvas->projection();
	TeProjection* toProj = wmstheme->getThemeProjection();

	if (fromProj == NULL ||  toProj == NULL)
	{
		return;
	}

	if (!(*fromProj == *toProj))
	{
		canvasBox = TeRemapBox(canvasBox, fromProj, toProj);
	}

	std::string crs = "EPSG:" + Te2String(TeOGC::TeGetEPSGFromTeProjection(toProj));

	TeOGC::TeWMSGetFeatureInfoResponse* fInfo = 0;
	WMSFeatureInfo* fInfoWindow = 0;
	try
	{
		fInfo = wmstheme->getFeatureInfo(crs, canvas->getWorld(), canvas->width(), canvas->height(), pt.y(), pt.x());

		fInfoWindow = new WMSFeatureInfo(canvas, MESSAGEBOXTITLE, true);
		fInfoWindow->setLayerName(wmstheme->name());
		fInfoWindow->showFeatureInformation(fInfo->getFeatureInformation());
		waitcursor.resetWaitCursor();
		fInfoWindow->exec();
		delete fInfoWindow;
	}
	catch(TeOGC::TeOGCException& e)
	{
		waitcursor.resetWaitCursor();
		delete fInfo;
		delete fInfoWindow;
		QMessageBox::critical(0, MESSAGEBOXTITLE, e.getErrorMessage().c_str());
		return;
	}
	catch(...)
	{
		waitcursor.resetWaitCursor();
		delete fInfo;
		delete fInfoWindow;
		QMessageBox::critical(0, MESSAGEBOXTITLE, "Unexpected error during GetFeatureInfo request.");
		return;
	}
}

void WMSClientPluginShowThemePopup(const PluginsSignal& x)
{

	int themeType = 0;
	
	if(!x.signalData_.retrive(PluginsSignal::D_THEMETYPE, themeType))
	{		
		return;
	}

	if(themeType != TeWMSTHEME)
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

/* mount menu */
	WMSPopupWidget* popupmenu = new WMSPopupWidget(0, MESSAGEBOXTITLE, true);
	popupmenu->init(qtThemeItem, static_cast<int>(location.x()), static_cast<int>(location.y()));
	delete popupmenu;
}

void WMSClientPluginSetThemeIcon(const PluginsSignal& x)
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

/* set theme icon if theme is a WMS */
	if(qtThemeItem->getAppTheme()->getTheme()->type() == TeWMSTHEME)
        qtThemeItem->setPixmap(0, QPixmap(wmsTheme_xpm));
}

void createOWSTables(TeDatabase* db) throw (TeException)
{
	bool hasTochangeSvcInfo = true;

	if(db->tableExist("te_ows_theme_service_operation"))
	{
		db->alterTable("te_ows_theme_service_operation", "te_ows_srv_op");

		//if(!db->alterTable("te_ows_theme_service_operation", "te_ows_srv_op"))
		//	throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to rename table: ").latin1() + string("te_ows_theme_service_operation"));
	}
	else
		TeOGC::TeOWSThemeServiceOperation::createServiceOperationTable(db);

	if(db->tableExist("te_ows_theme_service_info"))
	{
		db->alterTable("te_ows_theme_service_info", "te_ows_srv_info");

		//if(!db->alterTable("te_ows_theme_service_info", "te_ows_srv_info"))
		//	throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to rename table: ").latin1() + string("te_ows_theme_service_info"));
	}
	else
		TeOGC::TeOWSThemeServiceInfo::createServiceInfoTable(db);

	if(db->tableExist("te_ows_theme_service_info_op"))
	{
		hasTochangeSvcInfo = false;

		db->alterTable("te_ows_theme_service_info_op", "te_ows_srv_info_op");

		//if(!db->alterTable("te_ows_theme_service_info_op", "te_ows_srv_info_op"))
		//	throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to rename table: ").latin1() + string("te_ows_theme_service_info_op"));
	}

	if(db->tableExist("te_ows_th_service_inf_op"))
	{
		if(hasTochangeSvcInfo)
		{
			db->alterTable("te_ows_th_service_inf_op", "te_ows_srv_info_op");

			//if(!db->alterTable("te_ows_th_service_inf_op", "te_ows_srv_info_op"))
			//	throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to rename table: ").latin1() + string("te_ows_th_service_inf_op"));

			hasTochangeSvcInfo = false;
		}
		else
			db->deleteTable("te_ows_th_service_inf_op");

			//if(!db->deleteTable("te_ows_th_service_inf_op"))
			//	throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to drop table: ").latin1() + string("te_ows_th_service_inf_op"));
	}

	if(db->tableExist("te_ows_theme_service_info_operation"))
	{
		if(hasTochangeSvcInfo)
		{
			db->alterTable("te_ows_theme_service_info_operation", "te_ows_srv_info_op");

			//if(!db->alterTable("te_ows_theme_service_info_operation", "te_ows_srv_info_op"))
			//	throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to rename table: ").latin1() + string("te_ows_theme_service_info_operation"));
		}
		else
			db->deleteTable("te_ows_theme_service_info_operation");

			//if(!db->deleteTable("te_ows_theme_service_info_operation"))
			//	throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to drop table: ").latin1() + string("te_ows_theme_service_info_operation"));
	}
	else
		TeOGC::TeOWSThemeServiceInfoOperation::createServiceInfoOperationTable(db);

	if(db->tableExist("te_ows_theme_service_type"))
	{
		db->alterTable("te_ows_theme_service_type", "te_ows_srv_type");

		//if(!db->alterTable("te_ows_theme_service_type", "te_ows_srv_type"))
		//	throw TeException(UNKNOWN_ERROR_TYPE, QObject::tr("Fail to rename table: ").latin1() + string("te_ows_theme_service_type"));
	}
	else
		TeOGC::TeOWSThemeServiceType::createServiceTypeTable(db);
}
