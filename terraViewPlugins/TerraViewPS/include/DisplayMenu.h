#ifndef __TVPS_PLUGIN_DISPLAY_MENU_H_
#define __TVPS_PLUGIN_DISPLAY_MENU_H_

// tvps
#include <DisplayShortcut.h>

// Qt
#include <qobject.h>

// forward declarations
class TerraViewBase;
class QAction;
class QPopupMenu;

class DisplayMenu: public DisplayShortcut
{
    Q_OBJECT

public:

    DisplayMenu(TerraViewBase* tv, const QString& menuName, const int& index);

	virtual ~DisplayMenu();

public:

	void setEnabled(const bool& e = true);

private slots:

    void showMenu();
	
private:

    QPopupMenu*	_menu; //!< Qt menu.

    QAction* _promoteAction;
    QAction* _showGraphicalScaleAction;
    QAction* _drawOnCenter;
};

#endif // __TVPS_PLUGIN_DISPLAY_MENU_H_
