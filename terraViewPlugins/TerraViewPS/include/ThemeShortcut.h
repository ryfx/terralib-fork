#ifndef __TVPS_PLUGIN_THEME_SHORTCUT_H_
#define __TVPS_PLUGIN_THEME_SHORTCUT_H_

// tvps
#include <Shortcut.h>

// Qt
#include <qobject.h>

// forward declarations
class TerraViewBase;
class TeQtThemeItem;

class ThemeShortcut : public Shortcut
{
    Q_OBJECT

public:

    ThemeShortcut(TerraViewBase* tv);

	virtual ~ThemeShortcut();

protected:
	
    TeQtThemeItem* getCurrentThemeItem();

public slots:
	
	virtual void removeTheme();
    virtual void renameTheme();
    virtual void themeInfo();
    virtual void themeVisual();
    virtual void themeVisualPointing();
    virtual void themeVisualQuery();
    virtual void themeVisualPointingQuery();
    virtual void visRepresentations();
    virtual void editLegend();
    virtual void textRepresentation();
    virtual void createLayerFromTheme();
    virtual void createThemeFromTheme();
    virtual void saveThemeToFile();
    virtual void removeAliases();
    virtual void addTheme();
    virtual void editPieBarChart();
    virtual void graphicParameters();
};

#endif // __TVPS_PLUGIN_THEME_SHORTCUT_H_
