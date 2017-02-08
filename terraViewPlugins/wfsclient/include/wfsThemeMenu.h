#ifndef __TERRAVIEWWFSTHEMEMENU_INTERNAL_WFSTHEMEMENU_H
#define __TERRAVIEWWFSTHEMEMENU_INTERNAL_WFSTHEMEMENU_H

#include <qpopupmenu.h>

//Forward declarations
class QListViewItem;
class TerraViewBase;

class WFSPopupMenu : public QPopupMenu
{
	Q_OBJECT
	
public:

	WFSPopupMenu(TerraViewBase* tview, char* name=0);
	
	~WFSPopupMenu();

	void setListViewItem(QListViewItem* item);

protected slots:
	
	void renameTheme();

	void importData();

protected:

	void createWFSMenu();

	QListViewItem* item_;
	TerraViewBase* tview_;
};

#endif //__TERRAVIEWWFSTHEMEMENU_INTERNAL_WFSTHEMEMENU_H
