#ifndef __TVPS_PLUGIN_DATABASE_SHORTCUT_H_
#define __TVPS_PLUGIN_DATABASE_SHORTCUT_H_

// tvps
#include <Shortcut.h>

// Qt
#include <qobject.h>

// forward declarations
class TerraViewBase;

class DatabaseShortcut : public Shortcut
{
    Q_OBJECT

public:

    DatabaseShortcut(TerraViewBase* tv);

	virtual ~DatabaseShortcut();

protected slots:

    virtual void databaseRefresh();
    virtual void databaseInfo();
    virtual void removeExternalTable();
};

#endif // __TVPS_PLUGIN_DATABASE_SHORTCUT_H_
