#ifndef __TVPS_PLUGIN_THEME_TOOL_BAR_H_
#define __TVPS_PLUGIN_THEME_TOOL_BAR_H_

// tvps
#include <ThemeShortcut.h>

// forward declarations
class TerraViewBase;
class QToolBar;

class ThemeToolBar : public ThemeShortcut
{
    Q_OBJECT

public:

	ThemeToolBar(TerraViewBase* tv);
	
	virtual ~ThemeToolBar();
	
public:

	void setEnabled(const bool& e = true);

    void setVisible(const bool& e = true);
	
private:

    QToolBar*	_toolBar; //!< Qt tool bar.
};

#endif // __TVPS_PLUGIN_THEME_TOOL_BAR_H_
