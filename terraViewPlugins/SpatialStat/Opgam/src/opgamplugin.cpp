#include <opgamplugin.h>

#include <OpgamPluginWindow.h>

//Qt include files
#include <qmessagebox.h>

OpgamPlugin::OpgamPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("opgam_");
	win_ = 0;
}

OpgamPlugin::~OpgamPlugin()
{
	end();
}

void OpgamPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!

	TerraViewBase* tview = getMainWindow(); 

	try

	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "Opgam");
			showWinAction_->setMenuText(tr("Opgam"));
			showWinAction_->setText( tr("Opgam") ); 

			QPopupMenu* mnu = getPluginsMenu(tr("TerraViewPlugin.Cluster Detection.Espacials.Focused Testing").latin1());

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

void OpgamPlugin::end()
{
	delete win_;
}

void OpgamPlugin::showWindow()
{
	if(win_ == 0)
		win_ = new OpgamPluginWindow(getMainWindow());

	win_->updateForm(params_);
}
