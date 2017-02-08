#ifndef __TVPS_PLUGIN_LAYER_MENU_H_
#define __TVPS_PLUGIN_LAYER_MENU_H_

// tvps
#include <LayerShortcut.h>

// Qt
#include <qobject.h>

// forward declarations
class TerraViewBase;
class QPopupMenu;

class LayerMenu : public LayerShortcut
{
    Q_OBJECT

public:

    LayerMenu(TerraViewBase* tv, QPopupMenu* menu);

	virtual ~LayerMenu();

public:

	void setEnabled(const bool& e = true);

private slots:

    virtual void buildMenu();
	
private:

    QPopupMenu*	_menu; //!< Qt menu.
};

#endif // __TVPS_PLUGIN_LAYER_MENU_H_
