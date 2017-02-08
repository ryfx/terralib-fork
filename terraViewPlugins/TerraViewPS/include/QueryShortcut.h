#ifndef __TVPS_PLUGIN_QUERY_SHORTCUT_H_
#define __TVPS_PLUGIN_QUERY_SHORTCUT_H_

// tvps
#include <Shortcut.h>

// Qt
#include <qobject.h>

// forward declarations
class TerraViewBase;

class QueryShortcut : public Shortcut
{
    Q_OBJECT

public:

    QueryShortcut(TerraViewBase* tv);

	virtual ~QueryShortcut();

protected:

    bool isQueriedTheme();

protected slots:

    virtual void spatialQuery();
    virtual void attributeQuery();
};

#endif // __TVPS_PLUGIN_QUERY_SHORTCUT_H_
