#include <wcsclientPlugin.h>

#include <wcsclientmainwin.h>

//TerraOGC include files
#include <wcsclient/TeWCSClient.h>

// #include <WMSConfigurer.h>
// #include <WMSClientTerraViewUtils.h>
// #include <owstheme/TeOWSThemeServiceInfoManager.h>

// //TerraOGC include files
// #include <wmstheme/TeWMSTheme.h>
// #include <wmstheme/TeWMSThemeFactory.h>

//Qt include files
#include <qaction.h>
#include <qmessagebox.h>
#include <qimage.h>
#include "icon.xpm"

//XPM include files
// #include <images/wmsTheme.xpm>


//static TeOGC::TeWMSThemeFactory wmsthemefactory;

WCSClientPlugin::WCSClientPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
// wmsConfigurer_(0)
{
	loadTranslationFiles("wcsclient_");

	initialize();
}

WCSClientPlugin::~WCSClientPlugin()
{
	end();
}

void WCSClientPlugin::init()
{
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(0, "", false);
			showWinAction_->setMenuText(tr("Open WCS"));
			showWinAction_->setText( tr("Open WCS") );
			showWinAction_->setToolTip( tr("Open WCS") );
			showWinAction_->setIconSet( QIconSet( QPixmap(icon_xpm)));

			QPopupMenu* mnu = getPluginsMenu(tr("TerraViewPlugins").latin1());

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

		initialize();
	}
	catch(...)
	{
		QMessageBox::critical(tview, tr("TerraView plug-in error"), tr("Can't create plug-in menu."));

		delete showWinAction_;
		showWinAction_ = 0;
	}
}

void WCSClientPlugin::end()
{
	delete showWinAction_;

  TeOGC::TeWCSClient::finalizePlatform();

	// delete wmsConfigurer_;

	// WMSSetPluginParameters(params_);
	// TerraViewBase* tview = getMainWindow();

	// /* Checking the TerraView plugin interface compatibility */
	// if(WMSGetPluginParameters()->getVersion() == PLUGINPARAMETERS_VERSION)
	// {
// /* Clear cache */
		// TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().finalize();
// /* Clear Curl */
		// TeOGC::TeWMSTheme::finalizePlatform();
	// }
	// else
	// {
		// QString warningMessage(QObject::tr("WMS Plugin's parameter version is different from TerraView's!"));
		// QMessageBox::warning(tview, "TerraView WMS Client Plugin", warningMessage, 0, 0, 0);
	// }
}

void WCSClientPlugin::showWindow()
{
  WCSClientMainWin win;
  win.exec();

    // WMSSetPluginParameters(params_);

	// /* Checking the TerraView plugin interface compatibility */
    // if(WMSGetPluginParameters()->getVersion() == PLUGINPARAMETERS_VERSION)
	// {
		// wmsConfigurer_->showPluginWindow();
	// }
	// else
	// {
		// QString warningMessage(QObject::tr("WMS Plugin's parameter version is different from TerraView's!"));
		// QMessageBox::warning(WMSGetPluginParameters()->qtmain_widget_ptr_, "TerraView WMS Client Plugin", warningMessage, 0, 0, 0);
	// }
}

void WCSClientPlugin::initialize()
{
    if(params_->getVersion() == PLUGINPARAMETERS_VERSION)
	{
/* Initializes WFS theme support */
		TeOGC::TeWCSClient::initializePlatform();

	}
	else
	{
		QString warningMessage(tr("WCS Plugin's parameter version is different from TerraView!"));
		QMessageBox::warning(params_->qtmain_widget_ptr_, "TerraView WCS Client Plugin", warningMessage, 0, 0, 0);
	}
}
