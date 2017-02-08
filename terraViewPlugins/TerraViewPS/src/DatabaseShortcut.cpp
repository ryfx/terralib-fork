// tvps
#include <DatabaseShortcut.h>

// TerraLib
#include <TeQtDatabasesListView.h>
#include <TerraViewBase.h>

DatabaseShortcut::DatabaseShortcut(TerraViewBase* tv)
: Shortcut(tv)
{}

DatabaseShortcut::~DatabaseShortcut()
{}

void DatabaseShortcut::databaseRefresh()
{
    if(_tv->currentDatabase())
        _tv->popupDatabaseRefreshSlot();
}

void DatabaseShortcut::databaseInfo()
{
    TeQtDatabaseItem* item = _tv->getDatabasesListView()->currentDatabaseItem();
    if(item == 0)
        return;
    _tv->databaseProperties(item);
}

void DatabaseShortcut::removeExternalTable()
{
    TeQtDatabaseItem* item = _tv->getDatabasesListView()->currentDatabaseItem();
    if(item == 0)
        return;
    _tv->removeExternalTable(item);
}
