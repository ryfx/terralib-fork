#include <wfsThemeMenu.h>
#include <wfsimport.h>

#include <wfsPluginUtils.h>

#include <wfstheme/TeWFSTheme.h>

#include <terraViewBase.h>
#include <TeQtThemeItem.h>
#include <TeQtLayerItem.h>
#include <TeQtDatabasesListView.h>

//Qt include files
#include <qlistview.h>
#include <qcursor.h>
#include <qmessagebox.h>

WFSPopupMenu::WFSPopupMenu(TerraViewBase* tview, char* name) :
QPopupMenu(tview, name)
{
	item_ = 0;
	tview_ = tview;

	createWFSMenu();
}

WFSPopupMenu::~WFSPopupMenu()
{
}

void WFSPopupMenu::setListViewItem(QListViewItem* item)
{
	item_ = item;
}

void WFSPopupMenu::renameTheme()
{
	if(item_ != 0)
		item_->startRename(0);
}

void WFSPopupMenu::importData()
{
	TeOGC::TeWFSTheme* theme = dynamic_cast<TeOGC::TeWFSTheme*>(((TeQtThemeItem*)item_)->getAppTheme()->getTheme());

	if(theme == 0)
		return;

	WFSImport imp(tview_);
	imp.setTheme(theme);

	//cur.setShape(Qt::ArrowCursor);
	//tview_->setCursor(cur);

	TeDatabase* db = tview_->currentDatabase();

	imp.setDatabase(db);

	if(imp.exec() == QDialog::Accepted)
	{
		/*tview_->*/setCursor(QCursor(Qt::WaitCursor));

		std::vector<std::string> atts = imp.getAttributes();

		TeViewMap vMap = db->viewMap();
		TeViewMap::iterator vIt = vMap.find(theme->view());
		TeView* view = (vIt == vMap.end()) ? 0 : vIt->second;

		TeBox bx;

		TeLayer* layer = 0;
		
		try
		{
			if(view == 0)
				throw TeException(UNKNOWN_ERROR_TYPE, tr("Fail to get theme view!").latin1());

			bx = getWorldInThemeProjection(view, theme);
			
			layer = importWFS(theme, tview_->currentDatabase(), bx, atts, imp.getThemeName().latin1());

			if (layer != 0)
			{
				TeQtDatabasesListView *databasesListView = tview_->getDatabasesListView();
				TeQtDatabaseItem *databaseItem = databasesListView->currentDatabaseItem();

				TeQtLayerItem *layerItem = new TeQtLayerItem((QListViewItem*)databaseItem, layer->name().c_str(), layer);
				layerItem->setEnabled(true);
				if (databasesListView->isOpen(layerItem->parent()) == false)
					databasesListView->setOpen(layerItem->parent(),true);
				tview_->checkWidgetEnabling();
			}

			/*tview_->*/setCursor(QCursor(Qt::ArrowCursor));

			QMessageBox::information(tview_, tr("Import success"), tr("Layer imported successfully!"));
		}
		catch(TeException& e)
		{
			/*tview_->*/setCursor(QCursor(Qt::ArrowCursor));
			QMessageBox::critical(tview_, tr("Import failure"), tr("Fail importing WFS data: ") + e.message().c_str());
		}
	}
}

void WFSPopupMenu::createWFSMenu()
{
	QPopupMenu::insertItem(tr("Rename..."), this, SLOT(renameTheme()));
	QPopupMenu::insertItem(tr("Remove..."), tview_, SLOT(popupRemoveViewsListViewItem()));
	QPopupMenu::insertSeparator();

	QPopupMenu* popupVisual = new QPopupMenu(this);
	insertItem(tr("Change visual"), popupVisual);
	popupVisual->insertItem(tr("Default..."), tview_, SLOT(popupDefaultVisual()));
	popupVisual->insertItem(tr("Pointing..."), tview_, SLOT(popupPointingVisual()));

	QPopupMenu::insertSeparator();
	QPopupMenu::insertItem(tr("Properties..."), tview_, SLOT(themeProperties()));

	QPopupMenu::insertSeparator();
	QPopupMenu::insertItem(tr("Import data..."), this, SLOT(importData()));
}