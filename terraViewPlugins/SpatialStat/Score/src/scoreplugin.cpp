#include <scoreplugin.h>

#include <ScorePluginWindow.h>

//Qt include files
#include <qmessagebox.h>

ScorePlugin::ScorePlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("score_");
	win_ = 0;
}

ScorePlugin::~ScorePlugin()
{
	end();
}

void ScorePlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!

	TerraViewBase* tview = getMainWindow(); 

	try

	{
		if(tview)
		{
			showWinAction_ = new QAction(this, "Score");
			showWinAction_->setMenuText(tr("Score"));
			showWinAction_->setText( tr("Score") ); 

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

void ScorePlugin::end()
{
	delete win_;
}

void ScorePlugin::showWindow()
{
	if(win_ == 0)
		win_ = new ScorePluginWindow(getMainWindow());

	win_->updateForm(params_);
}
