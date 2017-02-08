#ifndef __TVPS_PLUGIN_SHOW_MENU_H_
#define __TVPS_PLUGIN_SHOW_MENU_H_

// tvps
#include <Shortcut.h>

// Qt
#include <qobject.h>

// forward declarations
class TerraViewBase;
class tvpsPlugin;
class QPopupMenu;

class ShowMenu : public Shortcut
{
    Q_OBJECT

public:

    ShowMenu(TerraViewBase* tv, tvpsPlugin* tvps, QPopupMenu* menu);

	virtual ~ShowMenu();

public:

	void setEnabled(const bool& e = true);
	
private:

    QPopupMenu*	_menu; //!< Qt menu.
};

#endif // __TVPS_PLUGIN_SHOW_MENU_H_
