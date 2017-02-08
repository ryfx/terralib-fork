#include <bithellplugin.h>

#include <BithellPluginWindow.h>

//Qt include files
#include <qmessagebox.h>

BithellPlugin::BithellPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("bithell_");
	win_ = 0;
}

BithellPlugin::~BithellPlugin()
{
	end();
}

void BithellPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!

	TerraViewBase* tview = getMainWindow(); 

	try

	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "Bithell");
			showWinAction_->setMenuText(tr("Bithell"));
			showWinAction_->setText( tr("Bithell") ); 

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

void BithellPlugin::end()
{
	delete win_;
}

void BithellPlugin::showWindow()
{
	if(win_ == 0)
		win_ = new BithellPluginWindow(getMainWindow());

	win_->updateForm(params_);
}
