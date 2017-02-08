// tvps
#include <DisplayMenu.h>

// TerraLib
#include <display.h>
#include <TerraViewBase.h>
#include <TeDatabase.h>
#include <TeQtGrid.h>

// Qt
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>

DisplayMenu::DisplayMenu(TerraViewBase* tv, const QString& menuName, const int& index)
: DisplayShortcut(tv),
_menu(new QPopupMenu(0))
{   
	QMenuBar* mb = _tv->menuBar();
    mb->insertItem(menuName, _menu, -1, index);

    _promoteAction = createAction("", tr("Automatic Promotion on the Grid"), "", SLOT(autoPromote()), _menu, true);
    _menu->insertSeparator();

    createAction("", tr("Graphic Scale Visual..."), "", SLOT(graphicScaleVisual()), _menu);
    _showGraphicalScaleAction = createAction("", tr("Show Graphic Scale"), "", SLOT(showGraphicScale()), _menu, true);
    _menu->insertSeparator();

    createAction("", tr("Geographical Grid Visual..."), "", SLOT(geographicalGridVisual()), _menu);
    createAction("", tr("Show Gegraphical Grid"), "", SLOT(showGeographicalGrid()), _menu, true);
    _menu->insertSeparator();

    createAction("", tr("Insert Symbol..."), "", SLOT(insertSymbol()), _menu);
    createAction("", tr("Insert Text..."), "", SLOT(insertText()), _menu);
    _menu->insertSeparator();

    createAction("", tr("Delete Pointed Objects..."), "", SLOT(deletePointedObjects()), _menu);
    _menu->insertSeparator();

    _drawOnCenter = createAction("", tr("Centralize the Drawing of the Selected Objects"), "", SLOT(drawSelectedOnCenter()), _menu, true);
    _menu->insertSeparator();

    createAction("", tr("Animation..."), "", SLOT(animation()), _menu);
    _menu->insertSeparator();

    createAction("", tr("Display Size..."), "", SLOT(displaySize()), _menu);

    connect(_menu, SIGNAL(aboutToShow()), this, SLOT(showMenu()));
}

DisplayMenu::~DisplayMenu()
{
	delete _menu;
}

void DisplayMenu::showMenu()
{
    TeDatabase* db = _tv->currentDatabase();
    if(db == 0)
    {
        setEnabled(false);
        return;
    }

    TeView* v = _tv->currentView();
    if(v == 0)
    {
        setEnabled(false);
        return;
    }

    _promoteAction->setOn(_tv->getGrid()->autoPromote_);
    _drawOnCenter->setOn(_display->drawSetOnCenter());

    bool scaleVisible = false;
	TeDatabasePortal* portal  = db->getPortal();
    std::string sql = "SELECT visible FROM graphic_scale WHERE view_id = " + Te2String(v->id());
	if(portal->query(sql) && portal->fetchRow())
		scaleVisible = portal->getInt(0);
    portal->freeResult();
    _showGraphicalScaleAction->setOn(scaleVisible);

    setEnabled(true);
}

void DisplayMenu::setEnabled(const bool& e)
{
    for(unsigned int i = 0; i < _menu->count(); ++i)
    {
        int id = _menu->idAt(i);
        _menu->setItemEnabled(id, e);
    }
}
