// tvps
#include <ViewToolBar.h>

// TerraLib
#include <TerraViewBase.h>

// Qt
#include <qtoolbar.h>

ViewToolBar::ViewToolBar(TerraViewBase* tv)
: ViewShortcut(tv),
_toolBar(new QToolBar(tv))
{
	createAction("viewRename.png", "", tr("Rename the current view"), SLOT(renameView()), _toolBar);
    createAction("viewRemove.png", "", tr("Remove the current view"), SLOT(removeView()), _toolBar);
    createAction("viewInfo.png", "", tr("Show the current view the information"), SLOT(viewInfo()), _toolBar);
    createAction("viewProjection.png", "", tr("Define the current view projection"), SLOT(viewProjection()), _toolBar);
}

ViewToolBar::~ViewToolBar()
{
	delete _toolBar;
}

void ViewToolBar::setEnabled(const bool& e)
{
    _toolBar->setEnabled(e);
}

void ViewToolBar::setVisible(const bool& e)
{
    if(!e)
        _toolBar->hide();
    else
        _toolBar->show();
}
