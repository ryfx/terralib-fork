/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <wcslayersinfo.h>
#include <wcsservconfig.h>
#include <wcsservinfo.h>

#include <wcsClientUtils.h>

//Qt3 include files
#include <qwidgetstack.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qcombobox.h>
#include <qcursor.h>
#include <qtable.h>
#include <qmessagebox.h>

enum WCSWINS
{
  WCS_SERV_CONFIG = 1,
  WCS_LAYERS_INFO = 2,
  WCS_SERV_INFO = 3
};

void WCSClientMainWin::help_btn_clicked()
{

}


void WCSClientMainWin::prev_btn_clicked()
{
  checkWidget(WCS_SERV_CONFIG);
}


void WCSClientMainWin::next_btn_clicked()
{
  checkWidget(WCS_LAYERS_INFO);
}


void WCSClientMainWin::init()
{
  WCSServConfig* dlg1 = new WCSServConfig(wid_stck_, "WCSServiceConfiguration");
  WCSLayersInfo* dlg2 = new WCSLayersInfo(wid_stck_, "WCSLayersInformation");
  WCSServInfo* dlg3 = new WCSServInfo(wid_stck_, "WCSServiceInformation");

  wid_stck_->addWidget(dlg1, WCS_SERV_CONFIG);
  wid_stck_->addWidget(dlg2, WCS_LAYERS_INFO);
  wid_stck_->addWidget(dlg3, WCS_SERV_INFO);

  currentDlg_ = 0;
  checkWidget(WCS_SERV_CONFIG);

  resize( QSize(500, 500).expandedTo(minimumSizeHint()) );
  clearWState( WState_Polished );

  connect (dlg1, SIGNAL(showCapabilities()), this, SLOT(showServInfo()));
  connect( dlg2->layers_lst_, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(selectedLayerChanged(QListViewItem*)));

  dlg1->serv_tbl_->setNumRows(0);
}


void WCSClientMainWin::destroy()
{

}


void WCSClientMainWin::checkWidget(const int& newWidId)
{
try
{
  WCSServConfig* dlg1 = ((WCSServConfig*)wid_stck_->widget(WCS_SERV_CONFIG));
  WCSLayersInfo* dlg2 = ((WCSLayersInfo*)wid_stck_->widget(WCS_LAYERS_INFO));
  WCSServInfo* dlg3 = ((WCSServInfo*)wid_stck_->widget(WCS_SERV_INFO));

  QString url = dlg1->serv_lne_->text();

  

  bool refreshServer = (!(currentDlg_ == WCS_SERV_INFO)) && (newWidId != WCS_SERV_CONFIG);

  if(refreshServer)
  {
    std::string version;
    std::vector<std::string> oids = fillServiceInformation ( url, version, dlg3, dlg2 );
    serv_version_ = version.c_str();

    dlg2->setServerURL(url);

    if(!oids.empty())
    {
      dlg2->setLayerOids(oids);
      dlg2->connectToServer(serv_version_);
    }
  }

  switch(newWidId)
  {
    case WCS_SERV_CONFIG:
      prev_btn_->setEnabled(false);
      next_btn_->setEnabled(true);
      save_btn_->setEnabled(false);
    break;

    case WCS_LAYERS_INFO:
      prev_btn_->setEnabled(true);
      next_btn_->setEnabled(false);
      save_btn_->setEnabled(false);
    break;

    case WCS_SERV_INFO:
      prev_btn_->setEnabled(true);
      next_btn_->setEnabled(true);
      save_btn_->setEnabled(false);
    break;
  }
  currentDlg_ = newWidId;
  wid_stck_->raiseWidget(currentDlg_);
}catch(char *err)
{
	std::string strErr=std::string(err);
	QMessageBox::warning(this, tr("WCS Error"), strErr.c_str());	
}
}

WCSCoverageParams getParams(WCSLayersInfo* info)
{
  WCSCoverageParams par;
  par.img_crs_ = info->img_crs_cbb_->currentText().latin1();
  par.img_format_ = info->img_frm_cbb_->currentText().latin1();
  par.img_id_ = info->layers_lst_->currentItem()->text(0).latin1();

  bool ok;
  double x1, y1, x2, y2;

  x1 = info->bx1_lne_->text().toDouble(&ok);
  y1 = info->by1_lne_->text().toDouble(&ok);
  x2 = info->bx2_lne_->text().toDouble(&ok);
  y2 = info->by2_lne_->text().toDouble(&ok);

  if(!ok)
    throw;

  par.img_box_ = TeBox(x1, y1, x2, y2);

  return par;
}

void WCSClientMainWin::save_btn_clicked()
{
  QCursor cur(Qt::WaitCursor);

  WCSServConfig* dlg1 = ((WCSServConfig*)wid_stck_->widget(WCS_SERV_CONFIG));

  QString url = dlg1->serv_lne_->text();

  WCSCoverageParams par = getParams((WCSLayersInfo*)wid_stck_->widget(WCS_LAYERS_INFO));
  
  par.serv_version_ = serv_version_.latin1();

  getImage(url.latin1(), par, this);

  cur.setShape(Qt::ArrowCursor);
}

void WCSClientMainWin::showServInfo()
{
  checkWidget(WCS_SERV_INFO);
}


void WCSClientMainWin::selectedLayerChanged( QListViewItem * item)
{
  save_btn_->setEnabled ((item != 0) ? true : false);
}
