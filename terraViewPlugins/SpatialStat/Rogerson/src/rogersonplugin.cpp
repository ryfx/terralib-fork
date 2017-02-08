#include <rogersonplugin.h>

#include <RogersonPluginWindow.h>

//Qt include files
#include <qmessagebox.h>

RogersonPlugin::RogersonPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("rogerson_");
	win_ = 0;
}

RogersonPlugin::~RogersonPlugin()
{
	end();
}

void RogersonPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!

	TerraViewBase* tview = getMainWindow(); 

	try

	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "Rogerson");
			showWinAction_->setMenuText(tr("Rogerson"));
			showWinAction_->setText( tr("Rogerson") ); 

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

void RogersonPlugin::end()
{
	delete win_;
}

void RogersonPlugin::showWindow()
{
	if(win_ == 0)
		win_ = new RogersonPluginWindow(getMainWindow());

	win_->updateForm(params_);
}
