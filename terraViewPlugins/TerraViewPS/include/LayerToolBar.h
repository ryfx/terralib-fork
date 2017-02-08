#ifndef __TVPS_PLUGIN_LAYER_TOOL_BAR_H_
#define __TVPS_PLUGIN_LAYER_TOOL_BAR_H_

// tvps
#include <LayerShortcut.h>

// forward declarations
class TerraViewBase;
class QToolBar;

class LayerToolBar : public LayerShortcut
{
    Q_OBJECT

public:

	LayerToolBar(TerraViewBase* tv);
	
	virtual ~LayerToolBar();
	
public:

	void setEnabled(const bool& e = true);

    void setVisible(const bool& e = true);
	
private:

    QToolBar*	_toolBar; //!< Qt tool bar.
};

#endif // __TVPS_PLUGIN_LAYER_TOOL_BAR_H_
