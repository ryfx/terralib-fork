#include <cuzickplugin.h>

#include <CuzickPluginWindow.h>

//Qt cinclude files
#include <qmessagebox.h>

CuzickPlugin::CuzickPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("cuzick_");
	win_ = 0;
}

CuzickPlugin::~CuzickPlugin()
{
	end();
}

void CuzickPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "Cuzick");
			showWinAction_->setMenuText(tr("Cuzick"));
			showWinAction_->setText( tr("Cuzick") );

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
		QMessageBox::critical(tview, tr("Cuzick TerraView plug-in error"), tr("Can't create plug-in menu."));

		delete showWinAction_;
		showWinAction_ = 0;
	}
}

void CuzickPlugin::end()
{
	delete win_;
}

void CuzickPlugin::showWindow()
{
	if(win_ == 0)
		win_ = new CuzickPluginWindow(getMainWindow());

	win_->updateForm(params_);
}
