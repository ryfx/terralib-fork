#include <TVPLGplugin.h>

#include <plggenerator.h>
#include <TVPLGCodeGenerator.h>

//Qt include files
#include <qaction.h>
#include <qpopupmenu.h>
#include <qmessagebox.h>
#include <qlineedit.h>

TVPLGplugin::TVPLGplugin(PluginParameters* params) :
QObject(),
TViewAbstractPlugin(params),
showWinAction_(0)
{
	loadTranslationFiles("pluginGenerator_");
}

TVPLGplugin::~TVPLGplugin()
{
	end();
}

void TVPLGplugin::init()
{
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(0, "ShowWinAction", false);
			showWinAction_->setMenuText(tr("Create terraView plug-in"));

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

void TVPLGplugin::end()
{
	delete showWinAction_;
}

void TVPLGplugin::showWindow()
{
	QMainWindow* mainWin = getMainWindow();

	plgGenerator plgWin(mainWin, 0, true);

	if(plgWin.exec() == QDialog::Accepted)
	{
		QString name = plgWin.plgName_LNE_->text(),
			description = plgWin.desc_LNE_->text(),
			location = plgWin.location_LNE_->text(),
			vendor = plgWin.plgVendor_LNE_->text(),
			vendorMail = plgWin.mail_LNE_->text(),
			vendorHP = plgWin.homePage_LNE_->text(),
			className = plgWin.className_LNE_->text();

		TVPLGCodeGenerator gen;

		try
		{
			gen.generatePlgCode(name.latin1(), className.latin1(), description.latin1(), location.latin1(), vendor.latin1(), 
				vendorMail.latin1(), vendorHP.latin1());

			QMessageBox::information(mainWin, tr("Success"), tr("The new plug-in project was created successfully."));
		}
		catch(TeException& e)
		{
			QMessageBox::critical(&plgWin, tr("Project creation failed"), tr("Error creating project: ") + e.message().c_str());
		}
	}
}
