#include <grimsonplugin.h>

#include <GrimsonPluginWindow.h>

//Qt include files
#include <qmessagebox.h>

GrimsonPlugin::GrimsonPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("grimson_");
	win_ = 0;
}

GrimsonPlugin::~GrimsonPlugin()
{
	end();
}

void GrimsonPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "Grimson");
			showWinAction_->setMenuText(tr("Grimson"));
			showWinAction_->setText( tr("Grimson") );

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

void GrimsonPlugin::end()
{
	delete win_;
}

void GrimsonPlugin::showWindow()
{
	if(win_ == 0)
		win_ = new GrimsonPluginWindow(getMainWindow());

	win_->updateForm(params_);
}