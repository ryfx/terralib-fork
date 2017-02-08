#include <diggleplugin.h>

#include <DigglePluginWindow.h>

//Qt include files
#include <qmessagebox.h>

DigglePlugin::DigglePlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("diggle_");
	win_ = 0;
}

DigglePlugin::~DigglePlugin()
{
	end();
}

void DigglePlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!

	TerraViewBase* tview = getMainWindow(); 

	try

	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "Diggle");
			showWinAction_->setMenuText(tr("Diggle"));
			showWinAction_->setText( tr("Diggle") ); 

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

void DigglePlugin::end()
{
	delete win_;
}

void DigglePlugin::showWindow()
{
	if(win_ == 0)
		win_ = new DigglePluginWindow(getMainWindow());

	win_->updateForm(params_);
}
