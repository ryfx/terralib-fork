#include <mercadoPlugin.h>

#include <mercadoWindow.h>

//Qt include files
#include <qaction.h>
#include <qmessagebox.h>

mercadoPlugin::mercadoPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("mercado_");
}

mercadoPlugin::~mercadoPlugin()
{
	end();
}

void mercadoPlugin::init()
{
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(0, "", false);
			showWinAction_->setMenuText(tr("Mercado"));
			showWinAction_->setText( tr("Mercado") );
			showWinAction_->setToolTip( tr("Mercado Plug-in") );

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

void mercadoPlugin::end()
{
	delete showWinAction_;
}

void mercadoPlugin::showWindow()
{
	TerraViewBase* tview = getMainWindow();

	mercadoWindow win(tview);

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
