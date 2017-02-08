#ifndef __TVPS_PLUGIN_THEMATIC_MAPPING_MENU_H_
#define __TVPS_PLUGIN_THEMATIC_MAPPING_MENU_H_

// tvps
#include <ThemeShortcut.h>

// Qt
#include <qobject.h>

// forward declarations
class TerraViewBase;
class QPopupMenu;

class ThematicMappingMenu: public ThemeShortcut
{
    Q_OBJECT

public:

    ThematicMappingMenu(TerraViewBase* tv, const QString& menuName, const int& index);

	virtual ~ThematicMappingMenu();

public:

	void setEnabled(const bool& e = true);

private slots:

    void showMenu();
	
private:

    QPopupMenu*	_menu; //!< Qt menu.
};

#endif // __TVPS_PLUGIN_THEMATIC_MAPPING_MENU_H_
