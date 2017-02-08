// tvps
#include <LayerShortcut.h>

// TerraLib
#include <TeQtDatabasesListView.h>
#include <TeQtLayerItem.h>
#include <TerraViewBase.h>

LayerShortcut::LayerShortcut(TerraViewBase* tv)
: Shortcut(tv)
{}

LayerShortcut::~LayerShortcut()
{}

TeQtLayerItem* LayerShortcut::getCurrentLayerItem()
{
    return _tv->getDatabasesListView()->currentLayerItem();
}

void LayerShortcut::removeLayer()
{
    TeQtLayerItem* item = getCurrentLayerItem();
    if(item == 0)
        return;
    _tv->removeItem(item);
}

void LayerShortcut::renameLayer()
{
    TeQtLayerItem* item = getCurrentLayerItem();
    if(item == 0)
        return;
    item->startRename(0);
}

void LayerShortcut::layerInfo()
{
    TeQtLayerItem* item = getCurrentLayerItem();
    if(item == 0)
        return;
    _tv->layerProperties(item);
}

void LayerShortcut::layerProjection()
{
    TeQtLayerItem* item = getCurrentLayerItem();
    if(item == 0)
        return;
    _tv->layerProjection(item);
}

void LayerShortcut::createCells()
{
    TeQtLayerItem* item = getCurrentLayerItem();
    if(item == 0)
        return;
    _tv->createCellsAction_activated();
}

void LayerShortcut::createLayerTable()
{
    TeQtLayerItem* item = getCurrentLayerItem();
    if(item == 0)
        return;
    _tv->getDatabasesListView()->setPopupItem(item);
    _tv->createLayerTable();
}

void LayerShortcut::deleteLayerTable()
{
    TeQtLayerItem* item = getCurrentLayerItem();
    if(item == 0)
        return;
    _tv->getDatabasesListView()->setPopupItem(item);
    _tv->deleteLayerTable();
}

void LayerShortcut::addCentroidRepresentation()
{
    TeQtLayerItem* item = getCurrentLayerItem();
    if(item == 0)
        return;
    _tv->getDatabasesListView()->setPopupItem(item);
    _tv->popupAddCentroidRepDatabasesListViewItem();
}

void LayerShortcut::createCentroidLayer()
{
    TeQtLayerItem* item = getCurrentLayerItem();
    if(item == 0)
        return;
    _tv->getDatabasesListView()->setPopupItem(item);
    _tv->popupCreateCentroidLayerDatabasesListViewItem();
}

void LayerShortcut::createCentroidCountLayer()
{
    TeQtLayerItem* item = getCurrentLayerItem();
    if(item == 0)
        return;
    _tv->getDatabasesListView()->setPopupItem(item);
    _tv->popupCreateCountLayerDatabasesListViewItem();
}

void LayerShortcut::loadMemory()
{
    TeQtLayerItem* item = getCurrentLayerItem();
    if(item == 0)
        return;
    _tv->getDatabasesListView()->setPopupItem(item);
    _tv->popupLayerLoadMemory();
}

void LayerShortcut::clearMemory()
{
    TeQtLayerItem* item = getCurrentLayerItem();
    if(item == 0)
        return;
    _tv->getDatabasesListView()->setPopupItem(item);
    _tv->popupLayerClearMemory();
}

void LayerShortcut::insertFileInfo()
{
    TeQtLayerItem* item = getCurrentLayerItem();
    if(item == 0)
        return;
    _tv->getDatabasesListView()->setPopupItem(item);
    _tv->popupInsertFileLayerInfo();
}

void LayerShortcut::insertURLInfo()
{
    TeQtLayerItem* item = getCurrentLayerItem();
    if(item == 0)
        return;
    _tv->getDatabasesListView()->setPopupItem(item);
    _tv->popupInsertURLLayerInfo();
}

void LayerShortcut::retrieveInfo()
{
    TeQtLayerItem* item = getCurrentLayerItem();
    if(item == 0)
        return;
    _tv->getDatabasesListView()->setPopupItem(item);
    _tv->popupRetrieveLayerInfo();
}

void LayerShortcut::removeInfo()
{
    TeQtLayerItem* item = getCurrentLayerItem();
    if(item == 0)
        return;
    _tv->getDatabasesListView()->setPopupItem(item);
    _tv->popupRemoveLayerInfo();
}

void LayerShortcut::updateMetadata()
{
    _tv->infolayerUpdate_MetadataAction_activated();
}
