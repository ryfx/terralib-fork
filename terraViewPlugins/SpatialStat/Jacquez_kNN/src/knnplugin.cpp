#include <knnplugin.h>

#include <kNNPluginWindow.h>

//Qt include files
#include <qmessagebox.h>

KNNPlugin::KNNPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("jacquez_knn_");
	win_ = 0;
}

KNNPlugin::~KNNPlugin()
{
	end();
}

void KNNPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!

	TerraViewBase* tview = getMainWindow(); 

	try

	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "Jacquez's kNN Method");
			showWinAction_->setMenuText(tr("Jacquez's kNN Method"));
			showWinAction_->setText( tr("Jacquez's kNN Method") ); 

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

void KNNPlugin::end()
{
	delete win_;
}

void KNNPlugin::showWindow()
{
	if(win_ == 0)
		win_ = new kNNPluginWindow(getMainWindow());

	win_->updateForm(params_);
}
