#include <mantelplugin.h>

#include <MantelPluginWindow.h>

//Qt include files
#include <qmessagebox.h>

MantelPlugin::MantelPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("mantel_");
	win_ = 0;
}

MantelPlugin::~MantelPlugin()
{
	end();
}

void MantelPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "Mantel");
			showWinAction_->setMenuText(tr("Mantel"));
			showWinAction_->setText( tr("Mantel") );

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

void MantelPlugin::end()
{
	delete win_;
}

void MantelPlugin::showWindow()
{
	if(win_ == 0)
		win_ = new MantelPluginWindow(getMainWindow());

	win_->updateForm(params_);
	win_->show();
}