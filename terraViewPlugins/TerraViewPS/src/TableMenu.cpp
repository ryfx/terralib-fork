// tvps
#include <TableMenu.h>

// TerraLib
#include <TeAppTheme.h>
#include <TeQtThemeItem.h>
#include <TerraViewBase.h>

// Qt
#include <qmenubar.h>
#include <qpopupmenu.h>

TableMenu::TableMenu(TerraViewBase* tv, const QString& menuName, const int& index)
: TableShortcut(tv),
_menu(new QPopupMenu(0))
{   
	QMenuBar* mb = _tv->menuBar();
    mb->insertItem(menuName, _menu, -1, index);
	
	createAction("", tr("Add Column"), "", SLOT(addColumn()), _menu);

    connect(_menu, SIGNAL(aboutToShow()), this, SLOT(showMenu()));
}

TableMenu::~TableMenu()
{
	delete _menu;
}

void TableMenu::showMenu()
{
    bool on = true;
    TeAppTheme* appTheme = _tv->currentTheme();
    if(appTheme == 0)
        on = false;
    else
    {
        TeTheme* t = dynamic_cast<TeTheme*>(appTheme->getTheme());
        if(t == 0)
            on = false;
    }

    setEnabled(on);
}

void TableMenu::setEnabled(const bool& e)
{
    for(unsigned int i = 0; i < _menu->count(); ++i)
    {
        int id = _menu->idAt(i);
        _menu->setItemEnabled(id, e);
    }
}
