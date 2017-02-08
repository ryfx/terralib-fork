#include <postgisadapter.h>

#include <PostGISAdapterWindow.h>

//Qt include files
#include <qaction.h>
#include <qmessagebox.h>

PostGISAdapter::PostGISAdapter(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("postgisadapter_");
}

PostGISAdapter::~PostGISAdapter()
{
	end();
}

void PostGISAdapter::init()
{
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(0, "ShowWinAction", false);
			showWinAction_->setMenuText(tr("PostGIS Adapter"));
			showWinAction_->setToolTip( tr("PostGIS Adapter") );

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

void PostGISAdapter::end()
{
	delete showWinAction_;
}

void PostGISAdapter::showWindow()
{
	TerraViewBase* tview = getMainWindow();

	PostGISAdapterWindow win(tview);
	win.updateForm(params_);
}
