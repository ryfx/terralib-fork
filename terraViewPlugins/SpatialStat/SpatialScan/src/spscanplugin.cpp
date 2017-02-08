#include <spscanplugin.h>

#include <SpatialScanPluginWindow.h>

//Qt include files
#include <qmessagebox.h>

SPSCanPlugin::SPSCanPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("spatialscan_");
	win_ = 0;
}

SPSCanPlugin::~SPSCanPlugin()
{
	end();
}

void SPSCanPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "Spatial Scan");
			showWinAction_->setMenuText(tr("Spatial Scan"));
			showWinAction_->setText( tr("Spatial Scan") );

			QPopupMenu* mnu = getPluginsMenu(tr("TerraViewPlugin.Cluster Detection.Espacials.Global Tests").latin1());

			if(mnu != 0)
			{
				if(!showWinAction_->addTo(mnu))
					throw;
			}

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

void SPSCanPlugin::end()
{
	delete win_;
}

void SPSCanPlugin::showWindow()
{
	if(win_ == 0)
		win_ = new SpatialScanPluginWindow(getMainWindow());

	win_->updateForm(params_);
}