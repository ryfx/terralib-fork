#ifndef __TVPS_PLUGIN_VIEW_SHORTCUT_H_
#define __TVPS_PLUGIN_VIEW_SHORTCUT_H_

// tvps
#include <Shortcut.h>

// Qt
#include <qobject.h>

// forward declarations
class TerraViewBase;
class TeQtViewItem;

class ViewShortcut : public Shortcut
{
    Q_OBJECT

public:

    ViewShortcut(TerraViewBase* tv);

	virtual ~ViewShortcut();

protected:
	
    TeQtViewItem* getCurrentViewItem();

public slots:

	virtual void removeView();
    virtual void renameView();
    virtual void viewInfo();
    virtual void viewProjection();
    virtual void exportThemes();
    virtual void connectView();
    virtual void aggreagtion();
    virtual void add();
    virtual void intersection();
    virtual void difference();
    virtual void buffer();
    virtual void assignDistribute();
    virtual void assignColect();
    virtual void addView();
};

#endif // __TVPS_PLUGIN_VIEW_SHORTCUT_H_
