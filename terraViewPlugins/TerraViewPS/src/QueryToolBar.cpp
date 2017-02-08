// tvps
#include <QueryToolBar.h>

// TerraLib
#include <TerraViewBase.h>

// Qt
#include <qtoolbar.h>

QueryToolBar::QueryToolBar(TerraViewBase* tv)
: QueryShortcut(tv),
_toolBar(new QToolBar(tv))
{
	createAction("spatialQuery.png", "", tr("Perform a spatial query"), SLOT(spatialQuery()), _toolBar);
	createAction("attrQuery.png", "", tr("Perform an attribute query"), SLOT(attributeQuery()),_toolBar);
}

QueryToolBar::~QueryToolBar()
{
	delete _toolBar;
}

void QueryToolBar::setEnabled(const bool& e)
{
    _toolBar->setEnabled(e);
}

void QueryToolBar::setVisible(const bool& e)
{
    if(!e)
        _toolBar->hide();
    else
        _toolBar->show();
}