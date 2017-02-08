#include <srplugin.h>

#include <SRPluginWindow.h>

//Qt include files
#include <qmessagebox.h>

SRPlugin::SRPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("sr_");
	win_ = 0;
}

SRPlugin::~SRPlugin()
{
	end();
}

void SRPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "Shirayev-Roberts");
			showWinAction_->setMenuText(tr("Shirayev-Roberts"));
			showWinAction_->setText( tr("Shirayev-Roberts") );

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

void SRPlugin::end()
{
	delete win_;
}

void SRPlugin::showWindow() 
{
	if(win_ == 0)
		win_ = new SRPluginWindow(getMainWindow());

	win_->updateForm(params_);
}