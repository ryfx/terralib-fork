#include <externalthemePlugin.h>

#include <externalDBThemeWindow.h>

//Qt include files
#include <qaction.h>
#include <qmessagebox.h>
#include <qimage.h>
//#include <qiconset.h>

//XPM include files
#include <externalTheme.xpm>

ExtThemePlugin::ExtThemePlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("externalTheme_");
}

ExtThemePlugin::~ExtThemePlugin()
{
	end();
}

void ExtThemePlugin::init()
{
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(0, "External theme action");
			showWinAction_->setMenuText(tr("Add External Theme"));
			showWinAction_->setText( tr("External Theme") );
			showWinAction_->setToolTip( tr("External Theme") );
			showWinAction_->setIconSet( QIconSet( QPixmap(externalTheme_xpm)));

			QPopupMenu* mnu = getPluginsMenu(tr("TerraViewPlugins.Themes").latin1());

			if(mnu != 0)
			{
				if(!showWinAction_->addTo(mnu))
					throw;
			}

			QPtrList< QToolBar > tv_tool_bars_list = tview->toolBars( Qt::DockTop );

			if( tv_tool_bars_list.count() > 0 )
				showWinAction_->addTo(tview->toolBars(Qt::DockTop).at(0));

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

void ExtThemePlugin::end()
{
	delete showWinAction_;
}

void ExtThemePlugin::showWindow()
{
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(params_->getCurrentDatabasePtr() == 0)
		{
			QMessageBox::critical(tview, tr("Error"), tr("There is no database connected!"));
			return;
		}

		if(params_->getCurrentViewPtr() == 0)
		{
			QMessageBox::warning(tview, tr("TerraView plug-in error"), tr("You need to select a view."));
			return;
		}


		externalDBThemeWindow win(tview);
		win.updateForm(params_);

		if(win.exec() == QDialog::Accepted)
			QMessageBox::information(tview, tr("TerraView"), tr("External theme created successfully."));
	}
	catch(TeException& e)
	{
		QMessageBox::critical(tview, tr("TerraView plug-in error"), tr("error creating external theme."));
	}
}
