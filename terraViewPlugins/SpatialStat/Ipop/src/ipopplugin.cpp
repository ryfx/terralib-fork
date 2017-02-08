#include <ipopplugin.h>

#include <IpopPluginWindow.h>

//Qt include files
#include <qmessagebox.h>

IpopPlugin::IpopPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("ipop_");
	win_ = 0;
}

IpopPlugin::~IpopPlugin()
{
	end();
}

void IpopPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "Ipop");
			showWinAction_->setMenuText(tr("Ipop"));
			showWinAction_->setText( tr("Ipop") );

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

void IpopPlugin::end()
{
	delete win_;
}

void IpopPlugin::showWindow()
{
	if(win_ == 0)
		win_ = new IpopPluginWindow(getMainWindow());

	win_->updateForm(params_);
}
