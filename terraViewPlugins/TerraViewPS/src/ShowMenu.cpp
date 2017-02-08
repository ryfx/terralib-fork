// tvps
#include <tvpsPlugin.h>
#include <ShowMenu.h>

// TerraLib
#include <TerraViewBase.h>

// Qt
#include <qaction.h>
#include <qpopupmenu.h>

ShowMenu::ShowMenu(TerraViewBase* tv, tvpsPlugin* tvps, QPopupMenu* menu)
: Shortcut(tv),
_menu(menu)
{
    _menu->insertSeparator();
    createAction("check.png", tr("Database Toolbar"), "", SLOT(showDatabaseToolbar()), _menu, true, tvps);
    createAction("check.png", tr("Layer Toolbar"), "", SLOT(showLayerToolbar()), _menu, true, tvps);
    createAction("check.png", tr("View Toolbar"), "", SLOT(showViewToolbar()), _menu, true, tvps);
    createAction("check.png", tr("Theme Toolbar"), "", SLOT(showThemeToolbar()), _menu, true, tvps);
    createAction("check.png", tr("Query Toolbar"), "", SLOT(showQueryToolbar()), _menu, true, tvps);
}

ShowMenu::~ShowMenu()
{}

void ShowMenu::setEnabled(const bool& e)
{
    _menu->setEnabled(e);
}
