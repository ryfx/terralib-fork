#ifndef __TVPS_PLUGIN_TABLE_MENU_H_
#define __TVPS_PLUGIN_TABLE_MENU_H_

// tvps
#include <TableShortcut.h>

// Qt
#include <qobject.h>

// forward declarations
class TerraViewBase;
class QPopupMenu;

class TableMenu: public TableShortcut
{
    Q_OBJECT

public:

    TableMenu(TerraViewBase* tv, const QString& menuName, const int& index);

	virtual ~TableMenu();

public:

	void setEnabled(const bool& e = true);

private slots:

    void showMenu();
	
private:

    QPopupMenu*	_menu; //!< Qt menu.
};

#endif // __TVPS_PLUGIN_TABLE_MENU_H_
