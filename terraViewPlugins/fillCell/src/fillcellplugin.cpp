#include <fillcellplugin.h>
#include <FillCellDlg.h>

//Qt include files
#include <qaction.h>
#include <qmessagebox.h>

FillCellPlugin::FillCellPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("fillCell_");
}

FillCellPlugin::~FillCellPlugin()
{
	end();
}

void FillCellPlugin::init()
{
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(0, "Show Fill Cell Win", false);
			showWinAction_->setMenuText(tr("Fill cell"));
			showWinAction_->setText( tr("Fill cell") );
			showWinAction_->setToolTip( tr("Fill cell") );

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

void FillCellPlugin::end()
{
	delete showWinAction_;
}

void FillCellPlugin::showWindow()
{
	TerraViewBase* tview = getMainWindow();

	if(tview == 0)
		return;

	FillCellDlg dlg(tview);

	dlg.UpdateForm(*params_);
}