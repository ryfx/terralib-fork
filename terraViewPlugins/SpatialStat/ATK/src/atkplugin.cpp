#include <atkplugin.h>

#include <ATKPluginWindow.h>

//Qt include files
#include <qmessagebox.h>

ATKPlugin::ATKPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("atk_");
	win_ = 0;
}

ATKPlugin::~ATKPlugin()
{
	end();
}

void ATKPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!

	TerraViewBase* tview = getMainWindow(); 

	try

	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "ATK");
			showWinAction_->setMenuText(tr("ATK"));
			showWinAction_->setText( tr("ATK") ); 

			QPopupMenu* mnu = getPluginsMenu(tr("TerraViewPlugin.Cluster Detection.Surveillance Methods").latin1());

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

void ATKPlugin::end()
{
	delete win_;
}

void ATKPlugin::showWindow()
{
	if(win_ == 0)
		win_ = new ATKPluginWindow(getMainWindow());

	win_->updateForm(params_);
}
