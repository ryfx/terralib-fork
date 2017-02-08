// tvps
#include <ThematicMappingMenu.h>

// TerraLib
#include <TerraViewBase.h>
#include <TeQtThemeItem.h>

// Qt
#include <qmenubar.h>
#include <qpopupmenu.h>

ThematicMappingMenu::ThematicMappingMenu(TerraViewBase* tv, const QString& menuName, const int& index)
: ThemeShortcut(tv),
_menu(new QPopupMenu(0))
{   
	QMenuBar* mb = _tv->menuBar();
    mb->insertItem(menuName, _menu, -1, index);
	
    createAction("", tr("Edit Legend"), "", SLOT(editLegend()), _menu);
	createAction("", tr("Edit Bar or Pie Chart"), "", SLOT(editPieBarChart()), _menu);
    createAction("", tr("Text Representation"), "", SLOT(textRepresentation()), _menu);
	createAction("", tr("Graphic Parameters"), "", SLOT(graphicParameters()), _menu);

    connect(_menu, SIGNAL(aboutToShow()), this, SLOT(showMenu()));
}

ThematicMappingMenu::~ThematicMappingMenu()
{
	delete _menu;
}

void ThematicMappingMenu::showMenu()
{
    TeQtThemeItem* item = getCurrentThemeItem();
    bool on = true;
    if(item == 0)
        on = false;
    else
    {
        TeTheme* t = dynamic_cast<TeTheme*>(item->getAppTheme()->getTheme());
        if(t == 0)
            on = false;
    }

    setEnabled(on);
}

void ThematicMappingMenu::setEnabled(const bool& e)
{
    for(unsigned int i = 0; i < _menu->count(); ++i)
    {
        int id = _menu->idAt(i);
        _menu->setItemEnabled(id, e);
    }
}
