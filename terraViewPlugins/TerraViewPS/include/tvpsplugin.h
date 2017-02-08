#ifndef __TVPS_PLUGIN_H_
#define __TVPS_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>

// forward declarations
class DatabaseMenu;
class DatabaseToolBar;
class LayerMenu;
class LayerToolBar;
class ViewMenu;
class ViewToolBar;
class ThemeMenu;
class ThemeToolBar;
class QueryToolBar;
class TableMenu;
class ThematicMappingMenu;
class DisplayMenu;
class ShowMenu;

class tvpsPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	tvpsPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~tvpsPlugin();

	/**	\brief Initializer method.
	 */
	void init();

	/**	\brief Finalizer method.
	 */
	void end();

protected slots:

    virtual void showDatabaseToolbar();
    virtual void showLayerToolbar();
    virtual void showViewToolbar();
	virtual void showThemeToolbar();
    virtual void showQueryToolbar();

private:
    
    virtual void updateActionIcon(QAction* a);

private:

    DatabaseMenu*    _dbMenu;
    DatabaseToolBar* _dbToolBar;

    LayerMenu*    _layerMenu;
    LayerToolBar* _layerToolBar;

    ViewMenu*    _viewMenu;
    ViewToolBar* _viewToolBar;

    ThemeMenu*    _themeMenu;
    ThemeToolBar* _themeToolBar;

    QueryToolBar* _queryToolBar;

    TableMenu* _tableMenu;

    ThematicMappingMenu* _tmMenu;

    DisplayMenu* _displayMenu;

    ShowMenu* _showMenu;
};

#endif //__TVPS_PLUGIN_H_
