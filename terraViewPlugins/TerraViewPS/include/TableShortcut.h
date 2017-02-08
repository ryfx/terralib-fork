#ifndef __TVPS_PLUGIN_TABLE_SHORTCUT_H_
#define __TVPS_PLUGIN_TABLE_SHORTCUT_H_

// tvps
#include <Shortcut.h>

// Qt
#include <qobject.h>

// forward declarations
class TerraViewBase;

class TableShortcut : public Shortcut
{
    Q_OBJECT

public:

    TableShortcut(TerraViewBase* tv);

	virtual ~TableShortcut();

protected slots:

	virtual void addColumn();
    virtual void allColumnsStatistict();
};

#endif // __TVPS_PLUGIN_TABLE_SHORTCUT_H_
