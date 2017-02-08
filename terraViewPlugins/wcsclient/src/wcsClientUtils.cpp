#include <wcsClientUtils.h>
#include <wcslayersinfo.h>
#include <wcsservinfo.h>

//TerraOGC include files
#include <common/TeOGCException.h>
#include <common/TeOGCProjection.h>

#include <ows/TeOWSWGS84BoundingBox.h>
#include <ows/TeOWSServiceIdentification.h>
#include <ows/TeOWSKeywords.h>
#include <ows/TeOWSServiceProvider.h>
#include <ows/TeOWSOnlineResource.h>
#include <ows/TeOWSResponsiblePart.h>
#include <ows/TeOWSContact.h>
#include <ows/TeOWSTelephone.h>
#include <ows/TeOWSAddress.h>
#include <ows/TeOWSOperationsMetadata.h>
#include <ows/TeOWSOperation.h>
#include <ows/TeOWSDCP.h>
#include <ows/TeOWSHTTP.h>
#include <ows/TeOWSRequestMethod.h>
#include <ows/TeOWSMimeType.h>
//#include <ows/TeOWSMetadata.h>

#include <owstheme/TeOWSThemeServiceInfo.h>
#include <owstheme/TeOWSThemeServiceInfoManager.h>

#include <wcs/TeWCSCapabilities.h>
#include <wcs/TeWCSGetCapabilities.h>
#include <wcs/TeWCSContents.h>
#include <wcs/TeWCSCoverageSummary.h>
#include <wcs/TeWCSCoverageDescription.h>
#include <wcs/TeWCSCoverageDescriptions.h>
#include <wcs/TeWCSDescribeCoverage.h>
#include <wcs/TeWCSRange.h>
#include <wcs/TeWCSField.h>
#include <wcs/TeWCSAxis.h>
#include <wcs/TeWCSAvailableKeys.h>
#include <wcs/TeWCSSpatialDomain.h>
#include <wcs/TeWCSCoverageDomain.h>
#include <wcs/TeWCSGetCoverage.h>
//#include <wcs/TeWCSOutput.h>
//#include <wcs/TeWCSGridCRS.h>
#include <wcs/TeWCSCoverages.h>

#include <wcsclient/TeWCSClient.h>

//TerraLib include files
#include <TeDatabase.h>
#include <TeVectorRemap.h>

//Qt3 include files
#include <qlistview.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qfiledialog.h>
#include <qcursor.h>

#include <iostream>

#define MESSAGEBOXTITLE "WCS Client Plugin"

TeOGC::TeWCSCoverageDescriptions* describeCoverages ( const std::string& serv_url, const std::string& serv_version, const std::vector<std::string>& cover_ids )
{
  TeOGC::TeWCSDescribeCoverage desc(cover_ids);
  TeOGC::TeWCSClient client(serv_url);
  desc.setVersion(serv_version);

  TeOGC::TeWCSCoverageDescriptions* descs = client.describeCoverage(desc);

  return descs;
}

void fillServiceInfo ( TeOGC::TeWCSCapabilities* cap, QListView& infoList )
{
  infoList.clear();

  //Server information
  QListViewItem* servTitleItem = new QListViewItem(&infoList, QObject::tr("Title"));
  QListViewItem* absItem = new QListViewItem(&infoList, QObject::tr("Abstract"));
  QListViewItem* servVersionItem = new QListViewItem(&infoList, QObject::tr("Server version"));
  QListViewItem* capVersionItem = new QListViewItem(&infoList, QObject::tr("Capabilities version"));
  QListViewItem* keywordsItem = new QListViewItem(&infoList, QObject::tr("Keywords"));
  QListViewItem* providerItem = new QListViewItem(&infoList, QObject::tr("Provider"));

  TeOGC::TeOWSServiceIdentification* servIdentify = cap->getServiceIdentification();

  new QListViewItem(servTitleItem, servIdentify->getTitle().c_str());
  new QListViewItem(absItem, servIdentify->getAbstract().c_str());
  new QListViewItem(capVersionItem, cap->getVersion().c_str());

  //Keywords
  std::vector<TeOGC::TeOWSKeywords*> keys = servIdentify->getKeywordsList();

  for (unsigned int i=0; i<keys.size(); i++)
  {
    std::vector<std::string> keyws = keys[i]->getKeywords();
      
    for (unsigned int j=0; j<keyws.size(); j++)
      new QListViewItem(keywordsItem, keyws[j].c_str());
  }

  //Server versions
  std::vector<std::string> versions = servIdentify->getServiceTypeVersionList();

  for(unsigned int i=0; i<versions.size(); i++)
    new QListViewItem(servVersionItem, versions[i].c_str());

  //Provider informations
  TeOGC::TeOWSServiceProvider* prov = cap->getServiceProvider();
  QListViewItem* provName = new QListViewItem(providerItem, QObject::tr("Name"));
  QListViewItem* provSite = new QListViewItem(providerItem, QObject::tr("Site"));
  QListViewItem* contactSite = new QListViewItem(providerItem, QObject::tr("Contact"));

  new QListViewItem(provName, prov->getProviderName().c_str());
  new QListViewItem(provSite, prov->getProviderSite()->getLink().c_str());

  TeOGC::TeOWSResponsiblePart* rpart = prov->getServiceContact();
  TeOGC::TeOWSContact* contInfo = rpart->getContactInfo();

  QListViewItem* indName = new QListViewItem(contactSite, QObject::tr("Individual name"));
  QListViewItem* posName = new QListViewItem(contactSite, QObject::tr("Position name"));
  QListViewItem* contactInfo = new QListViewItem(contactSite, QObject::tr("Contact info"));
  QListViewItem* foneInfo = new QListViewItem(contactInfo, QObject::tr("Phone"));
  QListViewItem* voicefoneInfo = new QListViewItem(foneInfo, QObject::tr("Voice"));
  QListViewItem* faxfoneInfo = new QListViewItem(foneInfo, QObject::tr("Facsimile"));

  new QListViewItem(indName, rpart->getIndividualName().c_str());
  new QListViewItem(posName, rpart->getPositionName().c_str());

  //Phones list
  std::vector<std::string> voices = contInfo->getPhone()->getVoicePhoneList();
  std::vector<std::string> fax = contInfo->getPhone()->getFacsimileList();

  for(unsigned int i=0; i<voices.size(); i++)
    new QListViewItem(voicefoneInfo, voices[i].c_str());

  for(unsigned int i=0; i<fax.size(); i++)
    new QListViewItem(faxfoneInfo, fax[i].c_str());

  //Address
  QListViewItem* addressInfo = new QListViewItem(contactInfo, QObject::tr("Address"));
  TeOGC::TeOWSAddress* address = contInfo->getAddress();

  QListViewItem* addressItem = new QListViewItem(addressInfo, QObject::tr("Delivery point"));
  std::vector<std::string> ads = address->getDeliveryPointList();

  for(unsigned int i=0; i<ads.size(); i++)
    new QListViewItem(addressItem, ads[i].c_str());

  addressItem = new QListViewItem(addressInfo, QObject::tr("City"));
  new QListViewItem (addressItem, address->getCity().c_str());

  addressItem = new QListViewItem(addressInfo, QObject::tr("Administrative area"));
  new QListViewItem (addressItem, address->getAdministrativeArea().c_str());

  addressItem = new QListViewItem(addressInfo, QObject::tr("Postal code"));
  new QListViewItem (addressItem, address->getPostalCode().c_str());

  addressItem = new QListViewItem(addressInfo, QObject::tr("Country"));
  new QListViewItem (addressItem, address->getCountry().c_str());

  addressItem = new QListViewItem(addressInfo, QObject::tr("Eletronic mail address"));

  ads = address->getEletronicMailAddressList();

  for(unsigned int i=0; i<ads.size(); i++)
    new QListViewItem(addressItem, ads[i].c_str());

  QListViewItem* ops = new QListViewItem(&infoList, QObject::tr("Operations"));

  TeOGC::TeOWSOperationsMetadata* meta_data = cap->getOperationsMetadata();

  std::vector<TeOGC::TeOWSOperation*> operations = meta_data->getOperationList();

  for ( unsigned int i=0; i<operations.size(); i++ )
  {
    TeOGC::TeOWSOperation* op = operations[i];
    QListViewItem *opItem = new QListViewItem(ops, op->getName().c_str());

    std::vector<TeOGC::TeOWSDCP*> dcp_list = op->getDCPList();

    for( unsigned int j=0; j<dcp_list.size(); j++ )
    {
      TeOGC::TeOWSDCP* dcp = dcp_list[j];
      std::vector<TeOGC::TeOWSRequestMethod*> methods = dcp->getHTTP()->getRequestGetList();

      for(unsigned int z=0; z<methods.size(); z++)
        new QListViewItem(opItem, QString("GET: ") + methods[z]->getLink().c_str());

      methods = dcp->getHTTP()->getRequestPostList();

      for(unsigned int z=0; z<methods.size(); z++)
        new QListViewItem(opItem, QString("POST: ") + methods[z]->getLink().c_str());
    }
  }
}
void fillLayerInfo ( TeOGC::TeWCSCapabilities* cap, QListView& infoList, std::vector<std::string>& ids)
{
  infoList.clear();

  std::vector<TeOGC::TeWCSCoverageSummary*> summary = cap->getContents()->getCoverageSummary();

  cap->getContents()->getCoveragesIdentifiers(ids);

  for (unsigned int i=0; i<summary.size(); i++)
  {
    QListViewItem* lay = new QListViewItem(&infoList, ids[i].c_str());

    std::string abs = summary[i]->getAbstract();
    
    if(!abs.empty())
    {
      new QListViewItem(lay, abs.c_str());
      lay->setOpen(true);
    }
  }
}

std::vector<std::string> fillServiceInformation ( const QString& serv_url, std::string& serv_version, WCSServInfo* serverInfo, WCSLayersInfo* layersInfo )
{
  std::string url = serv_url.latin1();
  std::vector<std::string> cov_ids;

  TeOGC::TeWCSCapabilities* cap = 0;

  try
  {
    TeOGC::TeWCSClient client(url);
    TeOGC::TeWCSGetCapabilities request;

    cap = client.getCapabilities(request);

    serv_version = cap->getVersion();

    if( layersInfo != 0)
    {
      layersInfo->serv_url_lbl_->setText(cap->getServiceIdentification()->getAbstract().c_str());
      fillLayerInfo(cap, *layersInfo->layers_lst_, cov_ids);
    }

    if( serverInfo !=0 )
      fillServiceInfo (cap, *serverInfo->serv_info_lst_);

    delete cap;
  }
  catch ( ... )
  {
    delete cap;
  }

  return cov_ids;
}

TeOGC::TeWCSCoverageDescription* findDescription(const std::string& oid, const std::vector<TeOGC::TeWCSCoverageDescription*>& descs )
{
  std::vector<TeOGC::TeWCSCoverageDescription*>::const_iterator it;

  for(it=descs.begin(); it!=descs.end(); ++it)
    if((*it)->getIdentifier().compare(oid) == 0)
      return *it;

  return 0;
}

std::string fillCoverageInfo( const std::vector<TeOGC::TeWCSCoverageDescription*>& descs, const QString& cov_id, WCSLayersInfo* layersInfo )
{
  layersInfo->layer_info_lst_->clear();

  TeOGC::TeWCSCoverageDescription* desc = findDescription(cov_id.latin1(), descs);
  
  QListViewItem* item=0;

  std::string crs;

  if(desc != 0)
  {
    TeBox auxBx;

    int precision = getBoundingRect(*desc, crs, auxBx);

    layersInfo->bx1_lne_->setText(Te2String(auxBx.x1(), precision).c_str());
    layersInfo->by1_lne_->setText(Te2String(auxBx.y1(), precision).c_str());
    layersInfo->bx2_lne_->setText(Te2String(auxBx.x2(), precision).c_str());
    layersInfo->by2_lne_->setText(Te2String(auxBx.y2(), precision).c_str());

    std::vector<TeOGC::TeOWSMimeType*> imgTypes = desc->getSupportedFormat();
    std::vector<TeOGC::TeOWSMimeType*>::iterator tpIt;

    layersInfo->img_frm_cbb_->clear();

    for(tpIt=imgTypes.begin(); tpIt!=imgTypes.end(); ++tpIt)
      layersInfo->img_frm_cbb_->insertItem((*tpIt)->getMimeString().c_str());

    if(!desc->getTitle().empty())
    {
      item = new QListViewItem(layersInfo->layer_info_lst_, QObject::tr("Title"));
      new QListViewItem(item, desc->getTitle().c_str());
    }

    if(!desc->getIdentifier().empty())
    {
      item = new QListViewItem(layersInfo->layer_info_lst_, QObject::tr("Identifier"));
      new QListViewItem(item, desc->getIdentifier().c_str());
    }

    if(!desc->getAbstract().empty())
    {
      item = new QListViewItem(layersInfo->layer_info_lst_, QObject::tr("Abstract"));
      new QListViewItem(item, desc->getAbstract().c_str());
    }

    std::vector<std::string> crss = desc->getSupportedCRS();
    if(!crss.empty())
    {
      std::vector<std::string>::iterator it;

      item = new QListViewItem(layersInfo->layer_info_lst_, QObject::tr("Supported CRS"));

      layersInfo->img_crs_cbb_->clear();

      for (it = crss.begin(); it != crss.end(); ++it)
      {
        new QListViewItem(item, (*it).c_str());
        layersInfo->img_crs_cbb_->insertItem((*it).c_str());
      }
    }

    TeOGC::TeWCSRange* range = desc->getRange();

    if(range != 0)
    {
      item = new QListViewItem(layersInfo->layer_info_lst_, QObject::tr("Range"));
      QListViewItem* item2 = new QListViewItem(item, QObject::tr("Fields"));
      
      std::vector<TeOGC::TeWCSField*> field = range->getField();
      std::vector<TeOGC::TeWCSField*>::iterator fIt;

      for(fIt=field.begin(); fIt!=field.end(); ++fIt)
      {
        TeOGC::TeWCSField* f = *fIt;
        
        QListViewItem* item3 = new QListViewItem(item2, f->getTitle().c_str());
        QListViewItem* item4 = 0;
        
        if(!f->getAbstract().empty())
        {
          QListViewItem* item4 = new QListViewItem(item3, QObject::tr("Abstract"));
          new QListViewItem(item4, f->getAbstract().c_str());
        }

        if(!f->getAxis().empty())
        {
          item4 = new QListViewItem(item3, QObject::tr("Axis"));
          std::vector<TeOGC::TeWCSAxis*> axis = f->getAxis();
          std::vector<TeOGC::TeWCSAxis*>::iterator aIt;

          for(aIt=axis.begin(); aIt!=axis.end(); ++aIt)
          {
            TeOGC::TeWCSAxis* ax = *aIt;

            QListViewItem* item5 = new QListViewItem(item4, ax->getTitle().c_str());
            QListViewItem* item6 = 0;

            if(!ax->getAbstract().empty())
            {
              QListViewItem* item6 = new QListViewItem(item5, QObject::tr("Abstract"));
              new QListViewItem(item6, ax->getAbstract().c_str());
            }

            if(!ax->getIdentifier().empty())
            {
              item6 = new QListViewItem(item5, QObject::tr("Identifier"));
              new QListViewItem(item6, ax->getIdentifier().c_str());
            }

            std::vector<std::string> keys = ax->getAvailableKeys()->getKey();

            if(!keys.empty())
            {
              std::vector<std::string>::iterator it;
              item6 = new QListViewItem(item5, QObject::tr("Available keys"));

              for(it=keys.begin(); it!=keys.end(); ++it)
                new QListViewItem(item6, (*it).c_str());
            }
          }
        }
      }
    }
  }

  return crs;
}
void getImage(const std::string& serv_url, const WCSCoverageParams& par, QWidget* wid)
{
  TeBox rect = par.img_box_;
  std::map<std::string, std::string> kvp;
  int precision = adjustBox(rect);
  std::string bbox = Te2String(rect.x1(), precision)+","+Te2String(rect.y1(), precision)+","+Te2String(rect.x2(), precision)+","+Te2String(rect.y2(), precision)+","+par.img_crs_;

  kvp["SERVICE"] = "wcs";
  kvp["REQUEST"] = "getCoverage";
  kvp["IDENTIFIER"] = par.img_id_;
  kvp["FORMAT"] = par.img_format_;
  kvp["BOUNDINGBOX"] = bbox;
  kvp["VERSION"] = par.serv_version_;

  TeOGC::TeWCSClient client(serv_url);
  TeOGC::TeWCSGetCoverage request;
  request.readFromKVP(kvp);

  TeOGC::TeWCSCoverages* covs = client.getCoverage(request); 

  int size=0;
  char* img = covs->getResponseData(size);

  QCursor cur(Qt::ArrowCursor);

  QString fileName = QFileDialog::getSaveFileName(QString::null, QObject::tr("Tiff images (*.tif *.TIF)"), wid);

  if(!fileName.isEmpty())
  {
    int pos = fileName.find(".tif");

    if(pos == -1)
      pos = fileName.find(".TIF");

    if(pos == -1)
      fileName += ".tif";

    FILE *out; 
    out = fopen(fileName.latin1(), "wb");
    size_t bytesWritten = fwrite(img, sizeof(char), size, out);
    fclose(out);

    QMessageBox::information(wid, QObject::tr("Save image success"), QObject::tr("The image was saved successfully in ")+fileName + ".");
  }

  delete covs;

  cur.setShape(Qt::WaitCursor);
}

void remapBox(const std::string& crsFrom, const std::string& crsTo, TeBox& box)
{
  TeProjection* fromProj = TeOGC::TeGetTeProjectionFromEPSG(crsFrom);
  TeProjection* toProj = TeOGC::TeGetTeProjectionFromEPSG(crsTo);

  if(fromProj==0 || toProj==0)
    return;

  box = TeRemapBox(box, fromProj, toProj);
}

int getBoundingRect (const TeOGC::TeWCSCoverageDescription& desc, std::string& crs, TeBox& box)
{
    std::vector<double> ll = desc.getDomain()->getSpatialDomain()->getBoundingBox()->getLowerCorner();
    std::vector<double> ur = desc.getDomain()->getSpatialDomain()->getBoundingBox()->getUpperCorner();

    box = TeBox (ll[0], ll[1], ur[0], ur[1]);
    int precision = adjustBox(box);

    crs = desc.getDomain()->getSpatialDomain()->getBoundingBox()->getCRS();

    return precision;
}

//returns the service identifier, or -1 if the operation fails
int saveService(TeDatabase* db, const std::string& url, QWidget* parent)
{
  if ( db==0 )
    return -1;

	TeOGC::TeWCSClient client;
	client.setServiceURL(url);

	TeOGC::TeWCSCapabilities* cap = client.getCapabilities(TeOGC::TeWCSGetCapabilities());

  std::string title = cap->getServiceIdentification()->getTitle(),
		version = cap->getVersion();

	delete cap;

	TeOGC::TeOWSThemeServiceInfo* serviceInfo = TeOGC::TeOWSThemeServiceInfoManager::instance().findServiceInfoByOperationURL(url);

	if(serviceInfo)
		return serviceInfo->getId();

/* otherwise we save its information */
	serviceInfo = new TeOGC::TeOWSThemeServiceInfo;
	serviceInfo->setTitle(title);
	serviceInfo->setType(TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceType("WCS"));

	TeOGC::TeOWSThemeServiceInfoOperation op;
	op.setServiceOperation(TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceOperation("GetCapabilities"));
	op.setServiceInfo(serviceInfo);
	op.setOperationURL(url);
	serviceInfo->insert(op);

	TeOGC::TeOWSThemeServiceInfoOperation op1;
	op1.setServiceOperation(TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceOperation("GetCoverage"));
	op1.setServiceInfo(serviceInfo);
	op1.setOperationURL(url);
	serviceInfo->insert(op1);

	TeOGC::TeOWSThemeServiceInfoOperation op2;
	op2.setServiceOperation(TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceOperation("DescribeCoverage"));
	op2.setServiceInfo(serviceInfo);
	op2.setOperationURL(url);
	serviceInfo->insert(op2);

	serviceInfo->setVersion(version);

	try
	{
		TeOGC::TeOWSThemeServiceInfo::save(db, serviceInfo);
	}
	catch(const TeOGC::TeOGCException& e)
	{
		QMessageBox::critical(parent, MESSAGEBOXTITLE, e.getErrorMessage().c_str());
		delete serviceInfo;

		return -1;
	}
	catch(...)
	{
		QMessageBox::critical(parent, MESSAGEBOXTITLE, QObject::tr("Unexpected error..."));
		delete serviceInfo;

		return -1;
	}

	return serviceInfo->getId();
}
