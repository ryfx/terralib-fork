// tvps
#include <ThemeToolBar.h>

// TerraLib
#include <TerraViewBase.h>

// Qt
#include <qtoolbar.h>

ThemeToolBar::ThemeToolBar(TerraViewBase* tv)
: ThemeShortcut(tv),
_toolBar(new QToolBar(tv))
{
	createAction("themeRename.png", "", tr("Rename the current theme"), SLOT(renameTheme()), _toolBar);
    createAction("themeRemove.png", "", tr("Remove the current theme"), SLOT(removeTheme()), _toolBar);
    createAction("themeInfo.png", "", tr("Show the current theme information"), SLOT(themeInfo()), _toolBar);
    createAction("themeVisual.png", "", tr("Define the current theme visual"), SLOT(themeVisual()), _toolBar);
}

ThemeToolBar::~ThemeToolBar()
{
	delete _toolBar;
}

void ThemeToolBar::setEnabled(const bool& e)
{
    _toolBar->setEnabled(e);
}

void ThemeToolBar::setVisible(const bool& e)
{
    if(!e)
        _toolBar->hide();
    else
        _toolBar->show();
}