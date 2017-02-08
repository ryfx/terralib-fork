#ifndef __TVPS_PLUGIN_THEME_MENU_H_
#define __TVPS_PLUGIN_THEME_MENU_H_

// tvps
#include <ThemeShortcut.h>

// Qt
#include <qobject.h>

// forward declarations
class TerraViewBase;
class QPopupMenu;

class ThemeMenu : public ThemeShortcut
{
    Q_OBJECT

public:

    ThemeMenu(TerraViewBase* tv, QPopupMenu* menu);

	virtual ~ThemeMenu();

public:

	void setEnabled(const bool& e = true);

private slots:

    virtual void buildMenu();
	
private:

    QPopupMenu*	_menu; //!< Qt menu.
};

#endif // __TVPS_PLUGIN_THEME_MENU_H_
