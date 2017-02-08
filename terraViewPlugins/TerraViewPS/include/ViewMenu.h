#ifndef __TVPS_PLUGIN_VIEW_MENU_H_
#define __TVPS_PLUGIN_VIEW_MENU_H_

// tvps
#include <ViewShortcut.h>

// Qt
#include <qobject.h>

// forward declarations
class TerraViewBase;
class QPopupMenu;

class ViewMenu : public ViewShortcut
{
    Q_OBJECT

public:

    ViewMenu(TerraViewBase* tv, QPopupMenu* menu);

	virtual ~ViewMenu();

public:

	void setEnabled(const bool& e = true);

private slots:

    virtual void buildMenu();
	
private:

    QPopupMenu*	_menu; //!< Qt menu.
};

#endif // __TVPS_PLUGIN_VIEW_MENU_H_
