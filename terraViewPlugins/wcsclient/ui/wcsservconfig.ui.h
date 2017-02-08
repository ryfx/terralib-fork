/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

//Qt3 include files
#include <qmessagebox.h>

void WCSServConfig::serv_tbl_clicked( int, int, int, const QPoint & )
{

}


void WCSServConfig::remove_serv_btn_clicked()
{

}


void WCSServConfig::cap_btn_clicked()
{
  if(serv_lne_->text().isEmpty())
  {
    QMessageBox::warning(this, tr("WCS client error"), tr("Enter a service address."));

    return;
  }

  emit showCapabilities ();
}

