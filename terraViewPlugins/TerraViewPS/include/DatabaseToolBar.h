#ifndef __TVPS_PLUGIN_DATABASE_TOOL_BAR_H_
#define __TVPS_PLUGIN_DATABASE_TOOL_BAR_H_

// tvps
#include <DatabaseShortcut.h>

// forward declarations
class TerraViewBase;
class QToolBar;

class DatabaseToolBar : public DatabaseShortcut
{
    Q_OBJECT

public:

	DatabaseToolBar(TerraViewBase* tv);
	
	virtual ~DatabaseToolBar();

public:

	void setEnabled(const bool& e = true);
    
    void setVisible(const bool& e = true);
	
private:

    QToolBar*	_toolBar; //!< Qt tool bar.
};

#endif // __TVPS_PLUGIN_DATABASE_TOOL_BAR_H_
