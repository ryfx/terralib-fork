/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <wcsClientUtils.h>

//Qt3 include files
#include <qlineedit.h>
//#include <qlistview.h>

void WCSServInfo::serviceChanged( const QString& serv_url )
{

}

void WCSServInfo::init()
{
  serv_info_lst_->setColumnWidthMode(0, QListView::Manual);
}
