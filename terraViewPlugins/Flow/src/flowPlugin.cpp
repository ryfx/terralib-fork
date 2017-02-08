#include <flowPlugin.h>

#include <TeFlowWindow.h>

//Qt include files
#include <qaction.h>
#include <qmessagebox.h>

FlowPlugin::FlowPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("flow_");
}

FlowPlugin::~FlowPlugin()
{
	end();
}

void FlowPlugin::init()
{
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(0, "", false);
			showWinAction_->setMenuText(tr("Flow"));
			showWinAction_->setText( tr("Flow") );
			showWinAction_->setToolTip( tr("Flow plug-in") );

			QPopupMenu* mnu = getPluginsMenu();

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

void FlowPlugin::end()
{
	delete showWinAction_;
}

void FlowPlugin::showWindow()
{
	TerraViewBase* tview = getMainWindow();

	TeFlowWindow win(tview);

	if(!params_->getCurrentDatabasePtr())
	{
		QMessageBox::critical(tview, tr("Error"), tr("There is no database connected!"));
		return;
	}

	if(!params_->getCurrentLayerPtr())
	{
		QMessageBox::warning(tview, tr("Warning"),tr("Error, layer not found!"));
		return;
	}

	if(!params_->getCurrentThemeAppPtr())
	{
		QMessageBox::warning(tview, tr("Warning"), tr("Error, theme not found!"));
		return;
	}

	win.init(params_);

	win.exec();
}
