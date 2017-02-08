#include <oraclespatialadapterPlugin.h>

#include <OracleSpatialAdapterWindow.h>

//Qt include files
#include <qaction.h>
#include <qmessagebox.h>

oraSPAdaptPlugin::oraSPAdaptPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params),
showWinAction_(0)
{
	loadTranslationFiles("oracleSpatialAdapter_");
}

oraSPAdaptPlugin::~oraSPAdaptPlugin()
{
	end();
}

void oraSPAdaptPlugin::init()
{
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(0, "ShowWinAction", false);
			showWinAction_->setMenuText(tr("Oracle Spatial Adapter"));
			showWinAction_->setToolTip( tr("Oracle Spatial Adapter") );

			QPopupMenu* mnu = getPluginsMenu();

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

void oraSPAdaptPlugin::end()
{
	delete showWinAction_;
}

void oraSPAdaptPlugin::showWindow()
{
	TerraViewBase* tview = getMainWindow();

	OracleSpatialAdapterWindow win(tview);
	win.updateForm(params_);
}
