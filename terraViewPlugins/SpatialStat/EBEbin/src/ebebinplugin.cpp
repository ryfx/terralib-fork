#include <ebebinplugin.h>

#include <EBEBinPluginWindow.h>

//Qt include files
#include <qmessagebox.h>

EBEbinPlugin::EBEbinPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("ebebin_");
	win_ = 0;
}

EBEbinPlugin::~EBEbinPlugin()
{
	end();
}

void EBEbinPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!

	TerraViewBase* tview = getMainWindow(); 

	try

	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "EBE_binomial");
			showWinAction_->setMenuText(tr("EBE_binomial"));
			showWinAction_->setText( tr("EBE_binomial") ); 

			QPopupMenu* mnu = getPluginsMenu(tr("TerraViewPlugin.Estimation of rates").latin1());

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

void EBEbinPlugin::end()
{
	delete win_;
}

void EBEbinPlugin::showWindow()
{
	if(win_ == 0)
		win_ = new EBEbinPluginWindow(getMainWindow());

	win_->updateForm(params_);
}
