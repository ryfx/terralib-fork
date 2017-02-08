// tvps
#include <ViewShortcut.h>

// TerraLib
#include <TeQtViewItem.h>
#include <TeQtViewsListView.h>
#include <TerraViewBase.h>

ViewShortcut::ViewShortcut(TerraViewBase* tv)
: Shortcut(tv)
{}

ViewShortcut::~ViewShortcut()
{}

TeQtViewItem* ViewShortcut::getCurrentViewItem()
{
    TeView* v = _tv->currentView();
    return _tv->getViewsListView()->getViewItem(v);
}

void ViewShortcut::removeView()
{
    TeQtViewItem* item = getCurrentViewItem();
    if(item == 0)
        return;
    _tv->removeItem(item);
}

void ViewShortcut::renameView()
{
    TeQtViewItem* item = getCurrentViewItem();
    if(item == 0)
        return;
    item->startRename(0);
}

void ViewShortcut::viewInfo()
{
    TeQtViewItem* item = getCurrentViewItem();
    if(item == 0)
        return;
    _tv->viewProperties(item);
}

void ViewShortcut::viewProjection()
{
    TeQtViewItem* item = getCurrentViewItem();
    if(item == 0)
        return;
    _tv->viewProjection(item);
}

void ViewShortcut::exportThemes()
{
    TeQtViewItem* item = getCurrentViewItem();
    if(item == 0)
        return;
    _tv->getViewsListView()->setPopupItem(item);
    _tv->popupExportThemesViewListViewItem();
}

void ViewShortcut::connectView()
{
    _tv->popupViewConnect();
}

void ViewShortcut::aggreagtion()
{
    _tv->popupGeoprocessingSlot(0);
}

void ViewShortcut::add()
{
    _tv->popupGeoprocessingSlot(1);
}

void ViewShortcut::intersection()
{
    _tv->popupGeoprocessingSlot(2);
}

void ViewShortcut::difference()
{
    _tv->popupGeoprocessingSlot(3);
}

void ViewShortcut::buffer()
{
    _tv->popupGeoprocessingSlot(7);
}

void ViewShortcut::assignDistribute()
{
    _tv->popupGeoprocessingSlot(5);
}

void ViewShortcut::assignColect()
{
    _tv->popupGeoprocessingSlot(6);
}

void ViewShortcut::addView()
{
    _tv->addViewAction_activated();
}
