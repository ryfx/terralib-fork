#include <KFunctionPluginWindow.h>

#include <kfuncplugin.h>

//Qt include files
#include <qmessagebox.h>

KFuncPlugin::KFuncPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("kfunctions_");
	win_ = 0;
}

KFuncPlugin::~KFuncPlugin()
{
	end();
}

void KFuncPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!

	TerraViewBase* tview = getMainWindow(); 

	try

	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "K Functions");
			showWinAction_->setMenuText(tr("K Functions"));
			showWinAction_->setText( tr("K Functions") ); 

			QPopupMenu* mnu = getPluginsMenu(tr("TerraViewPlugin.Analysis of point patterns").latin1());

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

void KFuncPlugin::end()
{
	delete win_;
}

void KFuncPlugin::showWindow()
{
	if(win_ == 0)
		win_ = new KFunctionPluginWindow(getMainWindow());

	win_->updateForm(params_);
}
