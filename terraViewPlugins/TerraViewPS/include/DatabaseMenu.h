#ifndef __TVPS_PLUGIN_DATABASE_MENU_H_
#define __TVPS_PLUGIN_DATABASE_MENU_H_

// tvps
#include <DatabaseShortcut.h>

// Qt
#include <qobject.h>

// forward declarations
class TerraViewBase;
class QPopupMenu;

class DatabaseMenu: public DatabaseShortcut
{
    Q_OBJECT

public:

    DatabaseMenu(TerraViewBase* tv, const QString& menuName, const int& index);

	virtual ~DatabaseMenu();

public:

	void setEnabled(const bool& e = true);

private slots:

    void showMenu();
	
private:

    QPopupMenu*	_menu; //!< Qt menu.
};

#endif // __TVPS_PLUGIN_DATABASE_MENU_H_
