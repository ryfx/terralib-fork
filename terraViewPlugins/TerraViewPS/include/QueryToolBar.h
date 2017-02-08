#ifndef __TVPS_PLUGIN_QUERY_TOOL_BAR_H_
#define __TVPS_PLUGIN_QUERY_TOOL_BAR_H_

// tvps
#include <QueryShortcut.h>

// forward declarations
class TerraViewBase;
class QToolBar;

class QueryToolBar : public QueryShortcut
{
    Q_OBJECT

public:

	QueryToolBar(TerraViewBase* tv);
	
	virtual ~QueryToolBar();
	
public:

	void setEnabled(const bool& e = true);

    void setVisible(const bool& e = true);
	
private:

    QToolBar*	_toolBar; //!< Qt tool bar.
};

#endif // __TVPS_PLUGIN_QUERY_TOOL_BAR_H_
