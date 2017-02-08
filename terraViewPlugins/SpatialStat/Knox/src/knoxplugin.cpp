#include <knoxplugin.h>

#include <KnoxPluginWindow.h>

//Qt include files 
#include <qmessagebox.h>

KnoxPlugin::KnoxPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("knox_");
	win_ = 0;
}

KnoxPlugin::~KnoxPlugin()
{
	end();
}

void KnoxPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "Knox");
			showWinAction_->setMenuText(tr("Knox"));
			showWinAction_->setText( tr("Knox") );

			QPopupMenu* mnu = getPluginsMenu(tr("TerraViewPlugin.Cluster Detection.Temporal-Space").latin1());

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

void KnoxPlugin::end()
{
	delete win_;
}

void KnoxPlugin::showWindow()
{
	if (win_ == 0)
		win_ = new KnoxPluginWindow(getMainWindow());

	win_->updateForm(params_);
}