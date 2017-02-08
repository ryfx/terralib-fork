#ifndef __TVPS_PLUGIN_LAYER_SHORTCUT_H_
#define __TVPS_PLUGIN_LAYER_SHORTCUT_H_

// tvps
#include <Shortcut.h>

// Qt
#include <qobject.h>

// forward declarations
class TerraViewBase;
class TeQtLayerItem;

class LayerShortcut : public Shortcut
{
    Q_OBJECT

public:

    LayerShortcut(TerraViewBase* tv);

	virtual ~LayerShortcut();

protected:
	
    TeQtLayerItem* getCurrentLayerItem();

protected slots:

	virtual void removeLayer();
    virtual void renameLayer();
    virtual void layerInfo();
    virtual void layerProjection();
    virtual void createCells();
    virtual void createLayerTable();
    virtual void deleteLayerTable();
    virtual void addCentroidRepresentation();
    virtual void createCentroidLayer();
    virtual void createCentroidCountLayer();
    virtual void loadMemory();
    virtual void clearMemory();
    virtual void insertFileInfo();
    virtual void insertURLInfo();
    virtual void retrieveInfo();
    virtual void removeInfo();
    virtual void updateMetadata();
};

#endif // __TVPS_PLUGIN_LAYER_SHORTCUT_H_
