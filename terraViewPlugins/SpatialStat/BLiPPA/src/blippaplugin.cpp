#include <blippaplugin.h>

#include <BLiPPAPluginWindow.h>

//Qt include files
#include <qmessagebox.h>

BLiPPAPlugin::BLiPPAPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("blippa_");
	win_ = 0;
}

BLiPPAPlugin::~BLiPPAPlugin()
{
	end();
}

void BLiPPAPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "BLiPPA");
			showWinAction_->setMenuText(tr("BLiPPA"));
			showWinAction_->setText( tr("BLiPPA") );

			QPopupMenu* mnu = getPluginsMenu(tr("TerraViewPlugin.Analysis of point patterns").latin1());

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

void BLiPPAPlugin::end()
{
	delete win_;
}

void BLiPPAPlugin::showWindow()
{
	if(win_ == 0)
		win_ = new BLiPPAPluginWindow(getMainWindow());

	win_->updateForm(params_);

	win_->exec();
}
