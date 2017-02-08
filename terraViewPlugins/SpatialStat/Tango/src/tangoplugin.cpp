#include <tangoplugin.h>

#include <TangoPluginWindow.h>

//Qt include files
#include <qmessagebox.h>

TangoPlugin::TangoPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("tango_");
	win_ = 0;
}

TangoPlugin::~TangoPlugin()
{
	end();
}

void TangoPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!

	TerraViewBase* tview = getMainWindow(); 

	try

	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "Tango");
			showWinAction_->setMenuText(tr("Tango"));
			showWinAction_->setText( tr("Tango") ); 

			QPopupMenu* mnu = getPluginsMenu(tr("TerraViewPlugin.Cluster Detection.Espacials.Global Tests").latin1());

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

void TangoPlugin::end()
{
	delete win_;
}

void TangoPlugin::showWindow()
{
	if(win_ == 0)
		win_ = new TangoPluginWindow(getMainWindow());

	win_->updateForm(params_);
}
