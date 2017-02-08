#include <wmsclientPlugin.h>

#include <WMSConfigurer.h>
#include <WMSClientTerraViewUtils.h>
#include <owstheme/TeOWSThemeServiceInfoManager.h>

//TerraOGC include files
#include <wmstheme/TeWMSTheme.h>
#include <wmstheme/TeWMSThemeFactory.h>

//Qt include files
#include <qaction.h>
#include <qmessagebox.h>
#include <qimage.h>

//XPM include files
#include <images/wmsTheme.xpm>


static TeOGC::TeWMSThemeFactory wmsthemefactory;

WMSClientPlugin::WMSClientPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params),
wmsConfigurer_(0)
{
	loadTranslationFiles("wmsclient_");

	initialize();
}

WMSClientPlugin::~WMSClientPlugin()
{
	end();
}

void WMSClientPlugin::init()
{
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(0, "", false);
			showWinAction_->setMenuText(tr("Add WMS Theme"));
			showWinAction_->setText( tr("WMS Theme") );
			showWinAction_->setToolTip( tr("WMS Theme") );
			showWinAction_->setIconSet( QIconSet( QPixmap(wmsTheme_xpm)));

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

		initialize();
	}
	catch(...)
	{
		QMessageBox::critical(tview, tr("TerraView plug-in error"), tr("Can't create plug-in menu."));

		delete showWinAction_;
		showWinAction_ = 0;
	}
}

void WMSClientPlugin::end()
{
	delete showWinAction_;
	delete wmsConfigurer_;

	WMSSetPluginParameters(params_);
	TerraViewBase* tview = getMainWindow();

	/* Checking the TerraView plugin interface compatibility */
	if(WMSGetPluginParameters()->getVersion() == PLUGINPARAMETERS_VERSION)
	{
/* Clear cache */
		TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().finalize();
/* Clear Curl */
		TeOGC::TeWMSTheme::finalizePlatform();
	}
	else
	{
		QString warningMessage(QObject::tr("WMS Plugin's parameter version is different from TerraView's!"));
		QMessageBox::warning(tview, "TerraView WMS Client Plugin", warningMessage, 0, 0, 0);
	}
}

void WMSClientPlugin::showWindow()
{
    WMSSetPluginParameters(params_);

	/* Checking the TerraView plugin interface compatibility */
    if(WMSGetPluginParameters()->getVersion() == PLUGINPARAMETERS_VERSION)
	{
		wmsConfigurer_->showPluginWindow();
	}
	else
	{
		QString warningMessage(QObject::tr("WMS Plugin's parameter version is different from TerraView's!"));
		QMessageBox::warning(WMSGetPluginParameters()->qtmain_widget_ptr_, "TerraView WMS Client Plugin", warningMessage, 0, 0, 0);
	}
}

void WMSClientPlugin::initialize()
{
    WMSSetPluginParameters(params_);

/* Checking the TerraView plugin interface compatibility */
    if(WMSGetPluginParameters()->getVersion() == PLUGINPARAMETERS_VERSION)
	{
/* Initializes WMS theme support */
		TeOGC::TeWMSTheme::initializePlatform();

/* connects to TerraView signal emitter instance */
		wmsPluginSignalReceiver_.setHostObj(0, WMSClientPluginSignalHandler);
		wmsPluginSignalReceiver_.connect(*(WMSGetPluginParameters()->signal_emitter_));

		wmsConfigurer_ = new TeWMSConfigurer(WMSGetPluginParameters());
	}
	else
	{
		QString warningMessage(QObject::tr("WMS Plugin's parameter version is different from TerraView's!"));
		QMessageBox::warning(WMSGetPluginParameters()->qtmain_widget_ptr_, "TerraView WMS Client Plugin", warningMessage, 0, 0, 0);
	}
}
