/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <qpopupmenu.h>
#include <qmessagebox.h>
#include <TeKmlTheme.h>
#include <TeQtThemeItem.h>
#include <TeQtViewItem.h>
#include <TeQtViewsListView.h>
#include <TeAppTheme.h>

#include <TeApplicationUtils.h>

#include <TeView.h>

#include <kmlplugin.h>

void KmlPopupWidget::init(TerraViewBase* tViewBase, TeQtThemeItem * themeItem, const int & x, const int & y )
{
    _themeItem = themeItem;
    _tViewBase = tViewBase;

	TeKmlTheme* theme = dynamic_cast<TeKmlTheme*>(_themeItem->getAppTheme()->getTheme());

	if(!theme)
		return;


	QPopupMenu* popupMenu = new QPopupMenu(themeItem->listView());
	popupMenu->move(x, y);
	
	int menuIdx = 0;

	menuIdx = popupMenu->insertItem(tr("Remove"), this, SLOT(remove()));

	//menuIdx = popupMenu->insertItem(tr("Rename"), this, SLOT(rename()));

	popupMenu->exec();
	
    delete popupMenu;
}


void KmlPopupWidget::remove()
{
	
	QString question = tr("Do you really want to remove theme \"") + _themeItem->text() + "\" ?";

	int response = QMessageBox::question(this, tr("Remove Item"), question, tr("Yes"), tr("No"));

	if(response != 0)
		return;

	TeAppTheme* appTheme = _themeItem->getAppTheme();
	TeKmlTheme* baseTheme = dynamic_cast<TeKmlTheme*>(appTheme->getTheme());
	
	if(!baseTheme)
		return;

	if(_tViewBase->currentTheme() == appTheme)
	{
		_tViewBase->setCurrentAppTheme(0);

		if(_tViewBase->currentView()->getCurrentTheme() == baseTheme->id())
		{
			_tViewBase->currentView()->setCurrentTheme(-1);
			_tViewBase->currentDatabase()->updateView(_tViewBase->currentView());
		}
	}

	_tViewBase->getViewsListView()->removeThemeItem(_themeItem->getAppTheme());
	deleteAppTheme(_tViewBase->currentDatabase(), baseTheme);

	/*if(!_tViewBase->currentDatabase()->deleteTheme(baseTheme->id()))
		return;	*/
	
	
}


void KmlPopupWidget::rename()
{
	
}


