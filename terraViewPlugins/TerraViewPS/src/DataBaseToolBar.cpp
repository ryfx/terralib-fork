// tvps
#include <DatabaseToolBar.h>

// TerraLib
#include <TerraViewBase.h>

// Qt
#include <qtoolbar.h>

DatabaseToolBar::DatabaseToolBar(TerraViewBase* tv)
: DatabaseShortcut(tv),
_toolBar(new QToolBar(tv))
{
	createAction("databaseInfo.png", "", tr("Show the database info"), SLOT(databaseInfo()), _toolBar);
    createAction("databaseRefresh.png", "", tr("Reconnect to the database"), SLOT(databaseRefresh()), _toolBar);
}

DatabaseToolBar::~DatabaseToolBar()
{
	delete _toolBar;
}

void DatabaseToolBar::setEnabled(const bool& e)
{
    _toolBar->setEnabled(e);
}

void DatabaseToolBar::setVisible(const bool& e)
{
    if(!e)
        _toolBar->hide();
    else
        _toolBar->show();
}
