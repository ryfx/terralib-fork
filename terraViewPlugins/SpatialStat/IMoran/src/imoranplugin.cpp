#include <imoranplugin.h>

#include <IMoranPluginWindow.h>

//Qt include files
#include <qmessagebox.h>

IMoranPlugin::IMoranPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("imoran_");
	win_ = 0;
}

IMoranPlugin::~IMoranPlugin()
{
	end();
}

void IMoranPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "EBI Moran");
			showWinAction_->setMenuText(tr("EBI Moran"));
			showWinAction_->setText( tr("EBI Moran") );

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

void IMoranPlugin::end()
{
	delete win_;
}

void IMoranPlugin::showWindow()
{
	if(win_ == 0)
		win_ = new IMoranPluginWindow(getMainWindow());

	win_->updateForm(params_);
}