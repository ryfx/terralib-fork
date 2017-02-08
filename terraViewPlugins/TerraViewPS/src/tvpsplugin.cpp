// tvps
#include <DatabaseMenu.h>
#include <DatabaseToolBar.h>
#include <LayerMenu.h>
#include <LayerToolBar.h>
#include <ViewMenu.h>
#include <ViewToolBar.h>
#include <ThemeMenu.h>
#include <ThemeToolBar.h>
#include <QueryToolBar.h>
#include <TableMenu.h>
#include <ThematicMappingMenu.h>
#include <DisplayMenu.h>
#include <ShowMenu.h>

#include <tvpsplugin.h>

// Qt
#include <qaction.h>
#include <qmenubar.h>

tvpsPlugin::tvpsPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params),
_dbMenu(0),
_dbToolBar(0),
_layerMenu(0),
_layerToolBar(0),
_viewMenu(0),
_viewToolBar(0),
_themeMenu(0),
_themeToolBar(0),
_queryToolBar(0),
_tableMenu(0),
_tmMenu(0),
_displayMenu(0),
_showMenu(0)
{
	loadTranslationFiles("tvps_");
}

tvpsPlugin::~tvpsPlugin()
{
	end();
}

void tvpsPlugin::init()
{  
	// Gets TerraView Main Window...
    TerraViewBase* tv = getMainWindow();
	tv->setAppCaption(tr("TerraView - Social Policy"));

    // Database
    _dbMenu = new DatabaseMenu(tv, tr("&Database"), 2);
    _dbToolBar = new DatabaseToolBar(tv);

    // Layer
    QPopupMenu* layerMenu = getMenu(tr("&Infolayer").latin1());
    _layerMenu = new LayerMenu(tv, layerMenu);
    _layerToolBar = new LayerToolBar(tv);

    // View
    QPopupMenu* viewMenu = getMenu(tr("&View").latin1());
    _viewMenu = new ViewMenu(tv, viewMenu);
    _viewToolBar = new ViewToolBar(tv);

    // Theme
    QPopupMenu* themeMenu = getMenu(tr("&Theme").latin1());
    _themeMenu = new ThemeMenu(tv, themeMenu);
    _themeToolBar = new ThemeToolBar(tv);

    // Query
    _queryToolBar = new QueryToolBar(tv);

    // Thematic Mapping
    _tmMenu = new ThematicMappingMenu(tv, tr("T&hematic Mapping"), 8);

    // Table
    _tableMenu = new TableMenu(tv, tr("&Table"), 9);

    // Display
    _displayMenu = new DisplayMenu(tv, tr("&Display Options"), 2);

    // Show
    QPopupMenu* showMenu = getMenu(tr("&Show").latin1());
    _showMenu = new ShowMenu(tv, this, showMenu);
}

void tvpsPlugin::end()
{
    delete _dbMenu;
    delete _dbToolBar;

    delete _layerMenu;
    delete _layerToolBar;

    delete _viewMenu;
    delete _viewToolBar;

    delete _themeMenu;
    delete _themeToolBar;

    delete _queryToolBar;
    
    delete _tableMenu;

    delete _tmMenu;

    delete _displayMenu;
}

void tvpsPlugin::showDatabaseToolbar()
{
    QAction* a = (QAction*)sender();
    _dbToolBar->setVisible(a->isOn());
    updateActionIcon(a);
}

void tvpsPlugin::showLayerToolbar()
{
    QAction* a = (QAction*)sender();
    _layerToolBar->setVisible(a->isOn());
    updateActionIcon(a);
}

void tvpsPlugin::showViewToolbar()
{
    QAction* a = (QAction*)sender();
    _viewToolBar->setEnabled(a->isOn());
    updateActionIcon(a);
}

void tvpsPlugin::showThemeToolbar()
{
    QAction* a = (QAction*)sender();
    _themeToolBar->setVisible(a->isOn());
    updateActionIcon(a);
}

void tvpsPlugin::showQueryToolbar()
{
    QAction* a = (QAction*)sender();
    _queryToolBar->setVisible(a->isOn());
    updateActionIcon(a);
}

void tvpsPlugin::updateActionIcon(QAction* a)
{
    if(a->isOn())
        a->setIconSet(QIconSet(QPixmap::fromMimeSource("check.png")));
    else
        a->setIconSet(QIconSet(QPixmap::fromMimeSource("uncheck.png")));
}
