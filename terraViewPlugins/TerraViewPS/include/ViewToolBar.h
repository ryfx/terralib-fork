#ifndef __TVPS_PLUGIN_VIEW_TOOL_BAR_H_
#define __TVPS_PLUGIN_VIEW_TOOL_BAR_H_

// tvps
#include <ViewShortcut.h>

// forward declarations
class TerraViewBase;
class QToolBar;

class ViewToolBar : public ViewShortcut
{
    Q_OBJECT

public:

	ViewToolBar(TerraViewBase* tv);
	
	virtual ~ViewToolBar();
	
public:

	void setEnabled(const bool& e = true);

    void setVisible(const bool& e = true);
	
private:

    QToolBar*	_toolBar; //!< Qt tool bar.
};

#endif // __TVPS_PLUGIN_VIEW_TOOL_BAR_H_
