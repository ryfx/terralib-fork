#include <besageplugin.h>

#include <BesagPluginWindow.h>

//QT cinclude files
#include <qaction.h>
#include <qmessagebox.h>
#include <qmenubar.h>

QPopupMenu* getMenu(QPopupMenu* tviewMenu, QMenuBar* mBar, const std::string& popupName)
{
	QPopupMenu* res = tviewMenu;

	std::vector<std::string> results;
	std::vector<std::string>::iterator it;
	int idx = -1;

	TeSplitString(popupName, ".", results);

	for(it = results.begin(); it != results.end(); ++it)
	{
		QPopupMenu* aux = 0;
		int ct = res->count() -1;

		for(int i = 0; i < ct; i++)
		{
			idx = res->idAt(i);
			if(res->text(idx).isEmpty())
				continue;

			if(res->text(idx) == QString((*it).c_str()))
				aux = res->findItem(idx)->popup();
		}

		if(aux != 0)
			res = aux;
		else
		{
			idx = res->insertItem((*it).c_str());
			res = mBar->findItem(idx)->popup();
		}
	}
	
	return res;
}

BesagePlugin::BesagePlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("besag_");
	win_ = 0;
}

BesagePlugin::~BesagePlugin()
{
	end();
}

void BesagePlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!
	TerraViewBase* tview = getMainWindow();

	try
	{
		if(tview)
		{
			showWinAction_ = new QAction(0, "Besag Newell");
			showWinAction_->setMenuText(tr("Besag Newell"));
			showWinAction_->setText( tr("Besag Newell") );

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

void BesagePlugin::end()
{
	delete showWinAction_;
	delete win_;
}

void BesagePlugin::showWindow()
{
	if(win_ == 0)
		win_ = new BesagPluginWindow(getMainWindow());

	win_->updateForm(params_);
}
