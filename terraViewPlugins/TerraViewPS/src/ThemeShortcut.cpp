// tvps
#include <ThemeShortcut.h>

// TerraLib
#include <TeAppTheme.h>
#include <TeQtThemeItem.h>
#include <TeQtViewsListView.h>
#include <TerraViewBase.h>
#include <pieBarChart.h>

ThemeShortcut::ThemeShortcut(TerraViewBase* tv)
: Shortcut(tv)
{}

ThemeShortcut::~ThemeShortcut()
{}

TeQtThemeItem* ThemeShortcut::getCurrentThemeItem()
{
    TeAppTheme* t = _tv->currentTheme();
    return _tv->getViewsListView()->getThemeItem(t);
}

void ThemeShortcut::removeTheme()
{
    TeQtThemeItem* item = getCurrentThemeItem();
    if(item == 0)
        return;
    _tv->removeItem(item);
}

void ThemeShortcut::renameTheme()
{
    TeQtThemeItem* item = getCurrentThemeItem();
    if(item == 0)
        return;
    item->startRename(0);
}

void ThemeShortcut::themeInfo()
{
    TeQtThemeItem* item = getCurrentThemeItem();
    if(item == 0)
        return;
    _tv->themeProperties(item);
}

void ThemeShortcut::themeVisual()
{
    TeQtThemeItem* item = getCurrentThemeItem();
    if(item == 0)
        return;
    _tv->setDefaultVisual(item);
}

void ThemeShortcut::themeVisualPointing()
{
    TeQtThemeItem* item = getCurrentThemeItem();
    if(item == 0)
        return;
    _tv->setPointingVisual(item);
}

void ThemeShortcut::themeVisualQuery()
{
    TeQtThemeItem* item = getCurrentThemeItem();
    if(item == 0)
        return;
    _tv->setQueryVisual(item);
}

void ThemeShortcut::themeVisualPointingQuery()
{
    TeQtThemeItem* item = getCurrentThemeItem();
    if(item == 0)
        return;
    _tv->setPointingQueryVisual(item);
}

void ThemeShortcut::visRepresentations()
{
    _tv->setCurrentThemeVisibility();
}

void ThemeShortcut::editLegend()
{
    _tv->editLegend();
}

void ThemeShortcut::textRepresentation()
{
    _tv->textRepresentation();
}

void ThemeShortcut::createLayerFromTheme()
{
    TeQtThemeItem* item = getCurrentThemeItem();
    if(item == 0)
        return;
    _tv->getViewsListView()->setPopupItem(item);
    _tv->popupCreateLayerFromTheme();
}

void ThemeShortcut::createThemeFromTheme()
{
    TeQtThemeItem* item = getCurrentThemeItem();
    if(item == 0)
        return;
    _tv->getViewsListView()->setPopupItem(item);
    _tv->popupCreateThemeFromTheme();
}

void ThemeShortcut::saveThemeToFile()
{
    TeQtThemeItem* item = getCurrentThemeItem();
    if(item == 0)
        return;
    _tv->getViewsListView()->setPopupItem(item);
    _tv->popupSaveThemeToFile();
}

void ThemeShortcut::removeAliases()
{
    TeQtThemeItem* item = getCurrentThemeItem();
    if(item == 0)
        return;
    _tv->getViewsListView()->setPopupItem(item);
    _tv->popupThemeRemoveAllAlias();
}

void ThemeShortcut::addTheme()
{
    _tv->addThemeAction_activated();
}

void ThemeShortcut::editPieBarChart()
{
    TeQtThemeItem* item = getCurrentThemeItem();
    if(item == 0)
        return;

    TeTheme* t = dynamic_cast<TeTheme*>(item->getAppTheme()->getTheme());
    if(t == 0)
        return;

    PieBarChart w(_tv, "PieBarChart", false);
	w.exec();
}

void ThemeShortcut::graphicParameters()
{
    TeQtThemeItem* item = getCurrentThemeItem();
    if(item == 0)
        return;

    TeTheme* t = dynamic_cast<TeTheme*>(item->getAppTheme()->getTheme());
    if(t == 0)
        return;

    _tv->setGraphicParams();
}