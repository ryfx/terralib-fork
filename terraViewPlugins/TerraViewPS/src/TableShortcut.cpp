// tvps
#include <TableShortcut.h>

// TerraLib
#include <TeAppTheme.h>
#include <TerraViewBase.h>

TableShortcut::TableShortcut(TerraViewBase* tv)
: Shortcut(tv)
{}

TableShortcut::~TableShortcut()
{}

void TableShortcut::addColumn()
{
    TeAppTheme* appTheme = _tv->currentTheme();
    if(appTheme == 0)
        return;
    TeTheme* t = dynamic_cast<TeTheme*>(appTheme->getTheme());
    if(t == 0)
        return;
    _tv->popupAddColumnSlot();
}

void TableShortcut::allColumnsStatistict()
{ /* TODO */ }
