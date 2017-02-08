// tvps
#include <DisplayShortcut.h>

// TerraLib
#include <TeAppTheme.h>
#include <TerraViewBase.h>
#include <display.h>

DisplayShortcut::DisplayShortcut(TerraViewBase* tv)
: Shortcut(tv),
_display(tv->getDisplayWindow())
{}

DisplayShortcut::~DisplayShortcut()
{}

void DisplayShortcut::autoPromote()
{
    _tv->autoPromoteSlot();
}

void DisplayShortcut::graphicScaleVisual()
{
    _display->popupGraphicScaleVisualSlot();
}

void DisplayShortcut::showGraphicScale()
{
    _display->showGraphicScaleSlot();
}

void DisplayShortcut::geographicalGridVisual()
{
    _display->popupGeographicalCoordinatesVisualSlot();
}

void DisplayShortcut::showGeographicalGrid()
{
    _display->showGeographicalCoordinatesSlot();
}

void DisplayShortcut::insertSymbol()
{
    _display->popupInsertSymbolSlot();
}

void DisplayShortcut::insertText()
{
    _display->insertTextSlot();
}

void DisplayShortcut::deletePointedObjects()
{
    TeAppTheme* appTheme = _tv->currentTheme();
    if(appTheme == 0)
        return;
    TeTheme* t = dynamic_cast<TeTheme*>(appTheme->getTheme());
    if(t == 0)
        return;
    _display->popupDeletePointedObjectsSlot();
}

void DisplayShortcut::drawSelectedOnCenter()
{
    _display->popupDrawSelectedOnCenterSlot();
}

void DisplayShortcut::animation()
{
    _display->popupAnimationSlot();
}

void DisplayShortcut::displaySize()
{
    _display->displaySizeSlot();
}
