/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <wcsClientUtils.h>

#include <wcs/TeWCSCoverageDescription.h>
#include <wcs/TeWCSCoverageDescriptions.h>
#include <wcs/TeWCSException.h>

#include <TeUtils.h>

//Qt3 include files
#include <qmessagebox.h>
#include <qlistview.h>


//Functions
TeBox getBox (WCSLayersInfo* info)
{
  bool ok;
  double x1, y1, x2, y2;

  x1 = info->bx1_lne_->text().toDouble(&ok);
  y1 = info->by1_lne_->text().toDouble(&ok);
  x2 = info->bx2_lne_->text().toDouble(&ok);
  y2 = info->by2_lne_->text().toDouble(&ok);

  if(!ok)
    throw;

  return TeBox(x1, y1, x2, y2);
}

void updateInfoBox(const TeBox& box, WCSLayersInfo* info)
{
  TeBox aux = box;
  int prc = adjustBox(aux);

  info->bx1_lne_->setText(Te2String(aux.x1(), prc).c_str());
  info->by1_lne_->setText(Te2String(aux.y1(), prc).c_str());
  info->bx2_lne_->setText(Te2String(aux.x2(), prc).c_str());
  info->by2_lne_->setText(Te2String(aux.y2(), prc).c_str());
}

void WCSLayersInfo::layerChanged( QListViewItem * item)
{
 
	if(coverDescs_ == 0) 
	{
	//	QMessageBox::warning(this, tr("WCS Error"), tr("Invalid coverage description!"));	
	//	return;
		throw "Invalid coverage description";
	}

	std::vector<TeOGC::TeWCSCoverageDescription*> descs = coverDescs_->getCoverageDescription();

	curLayerId_ = item->text(0);

	curCrs_ = fillCoverageInfo(descs, curLayerId_, this ).c_str();
}


void WCSLayersInfo::init()
{
  coverDescs_= 0;
}


void WCSLayersInfo::destroy()
{
  delete coverDescs_;
}


void WCSLayersInfo::setServerURL( const QString & url)
{
  serv_url_ = url;
}


void WCSLayersInfo::setLayerOids( const std::vector<std::string> & ids)
{
  oids_ = ids;
}


void WCSLayersInfo::connectToServer(const QString& servVersion)
{
  delete coverDescs_;
  coverDescs_=0;

  if(serv_url_.isEmpty() || oids_.empty())
  {
    QMessageBox::warning(this, tr("WCS Error"), tr("Empty server url or layers ids set. Try to set it before use."));
    return;
  }

  try
  {
    coverDescs_ = describeCoverages ( serv_url_.latin1(), servVersion.latin1(), oids_ );
  }
  catch( TeOGC::TeWCSException& e )
  {
   // QMessageBox::warning(this, tr("WCS Error"), tr("Fail to describe coverages."));
	  throw "Fail to describe coverages.";
  }

  img_crs_cbb_->clear();
  img_frm_cbb_->clear();
  bx1_lne_->clear();
  by1_lne_->clear();
  bx2_lne_->clear();
  by2_lne_->clear();
  layer_info_lst_->clear();
}


void WCSLayersInfo::projectiontChanged( const QString & newCrs)
{
    TeOGC::TeWCSCoverageDescription* desc = findDescription( curLayerId_.latin1(), coverDescs_->getCoverageDescription() );
//    TeBox bx;

    //std::string crs;
    //int prc = getBoundingRect (*desc, crs, bx);

    TeBox bx = getBox(this);

    remapBox(curCrs_.latin1(), newCrs.latin1(), bx);
    curCrs_ = newCrs;

    updateInfoBox(bx, this);
}
