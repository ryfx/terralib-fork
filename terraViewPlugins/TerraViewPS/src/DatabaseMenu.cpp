// tvps
#include <DatabaseMenu.h>

// TerraLib
#include <TerraViewBase.h>

// Qt
#include <qmenubar.h>
#include <qpopupmenu.h>

DatabaseMenu::DatabaseMenu(TerraViewBase* tv, const QString& menuName, const int& index)
: DatabaseShortcut(tv),
_menu(new QPopupMenu(0))
{   
	QMenuBar* mb = _tv->menuBar();
    mb->insertItem(menuName, _menu, -1, index);
	
	createAction("databaseInfo.png", tr("Properties"), "", SLOT(databaseInfo()), _menu);
	createAction("databaseRefresh.png", tr("Reconnect database"), "", SLOT(databaseRefresh()), _menu);
	createAction("", tr("Remove External Table..."), "", SLOT(removeExternalTable()), _menu);

    connect(_menu, SIGNAL(aboutToShow()), this, SLOT(showMenu()));
}

DatabaseMenu::~DatabaseMenu()
{
	delete _menu;
}

void DatabaseMenu::showMenu()
{
    bool on = _tv->currentDatabase() != 0;
    setEnabled(on);
}

void DatabaseMenu::setEnabled(const bool& e)
{
    for(unsigned int i = 0; i < _menu->count(); ++i)
    {
        int id = _menu->idAt(i);
        _menu->setItemEnabled(id, e);
    }
}
