#include <copydbplugin.h>

#include <CopyDatabaseWindow.h>

//Qt include file
#include <qaction.h>
#include <qmessagebox.h>

CopyDBPlugin::CopyDBPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("copyDatabase_");
}

CopyDBPlugin::~CopyDBPlugin()
{
	end();
}

void CopyDBPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(0, "Copy DB Action", false);
			showWinAction_->setMenuText(tr("Copy database"));
			showWinAction_->setText( tr("Copy database") );
			showWinAction_->setToolTip( tr("Copy database") );

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

void CopyDBPlugin::end()
{
	delete showWinAction_;
}

void CopyDBPlugin::showWindow()
{
	if(params_->getCurrentDatabasePtr() == 0)
	{
		QMessageBox::critical(params_->qtmain_widget_ptr_, tr("Error"), tr("There is no database connected!"));
		return;
	}

	CopyDatabaseWindow cpWin(params_->qtmain_widget_ptr_, "Copy database window");
	cpWin.updateForm(params_);

	cpWin.exec();
}
