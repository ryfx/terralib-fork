/************************************************************************************
TerraView WMS Client Plugin.
Copyright  2001-2007 INPE.
This file is part of TerraView WMS Client Plugin.
TerraView WMS Client Plugin is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

You should have received a copy of the GNU General Public License
along with TerraView WMS Client Plugin.
The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The software provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall the author be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use of
this program and its documentation.
*************************************************************************************/
/** \file WMSClientPluginUtils.cpp
 *  \brief This file contains the implementation of utilities functions that can be used in WMS Plugin.
 *  \author Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
 */

// WMS Plugin include files
#include "WMSClientPluginUtils.h"
#include "WMSFeatureInfo.h"
#include "WMSPopupWidget.h"

// TerraOGC WMS include files
#include <wms/TeWMSGetMapResponse.h>
#include <wms/TeWMSGetFeatureInfoResponse.h>
#include <wms/TeWMSGetCapabilitiesResponse.h>
#include <wms/TeWMSServiceSection.h>
#include <wms/TeWMSLayerSection.h>
#include <wms/TeWMSOnlineResource.h>
#include <wms/TeWMSKeywordList.h>
#include <wms/TeWMSKeyword.h>
#include <wms/TeWMSBoundingBox.h>
#include <wms/TeWMSException.h>
#include <wms/TeWMSOperationType.h>
#include <wms/TeWMSEX_GeographicBoundingBox.h>
#include <wms/TeWMSStyle.h>
#include <wms/TeWMSContactInformation.h>
#include <wms/TeWMSContactPersonPrimary.h>
#include <wms/TeWMSContactAddress.h>

// TerraOGC WMS Theme include files
#include <wmstheme/TeWMSTheme.h>

// TerraOGC SLD Theme include files
#include <sld/TeSLDUserDefinedSymbolization.h>

// TerraOGC OWS Theme include files
#include <owstheme/TeOWSThemeServiceInfoManager.h>
#include <owstheme/TeOWSThemeServiceInfo.h>
#include <owstheme/TeOWSThemeServiceType.h>
#include <owstheme/TeOWSThemeServiceOperation.h>

// TerraOGC OWS Theme include files
#include <ows/TeOWSMimeType.h>

// TerraOGC Common Theme include files
#include <common/TeOGCException.h>
#include <common/TeOGCProjection.h>

// TerraOGC XML include files
#include <xml/TeXMLDecoder.h>
#include <xml/TeXMLFunctions.h>
#include <xml/TeXMLParserParams.h>
#include <xml/TeXMLDecoderFactory.h>
#include <xml/TeXMLException.h>

// TerraLib include files
#include <TeUtils.h>
#include <TeAbstractTheme.h>
#include <TeView.h>

// Qt include files
#include <qlistview.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qtable.h>
#include <qpoint.h>
#include <qimage.h>
#include <qcombobox.h>

// XPM image include files
#include "wmsTheme.xpm"

#define MESSAGEBOXTITLE "WMS Client Plugin"

void FillServiceInfo(TeOGC::TeWMSServiceSection* service, QListView* serviceInfoListView)
{
	serviceInfoListView->clear();

	if(!service)
	{
		QMessageBox::warning(serviceInfoListView, QObject::tr(MESSAGEBOXTITLE), QObject::tr("Invalid service information."));
		return;
	}

	serviceInfoListView->setRootIsDecorated(true);
	serviceInfoListView->setSorting(-1);

	QListViewItem* currentItem = 0;

	std::string title = "Title: ";
	title += service->getTitle();
	QListViewItem* titleItem = new QListViewItem(serviceInfoListView);	
	titleItem->setText(0, title.c_str());
	currentItem = titleItem;

	std::string abstract = service->getAbstract();
	TeTrim(abstract);
	std::string abs = "Abstract: " + abstract;
	QListViewItem* abstractItem = new QListViewItem(serviceInfoListView, currentItem);
	abstractItem->setText(0, abs.c_str());
	currentItem = abstractItem;

	if(service->getKeywordList())
	{
		QListViewItem* keywordsItem = new QListViewItem(serviceInfoListView, currentItem);
		keywordsItem->setText(0, "Keyword");
		const std::vector<TeOGC::TeWMSKeyword*>& keywordVec = service->getKeywordList()->getKeywordList();

		for(unsigned int i = 0; i < keywordVec.size(); i++)
		{
			QListViewItem* keywordSubItem = new QListViewItem(keywordsItem);
			keywordSubItem->setText(0, keywordVec[i]->getValue().c_str());
		}

		currentItem = keywordsItem;
	}

	if(service->getOnlineResource())
	{
        std::string onLine = "OnLine Resource: ";
        onLine += service->getOnlineResource()->getLink();
        QListViewItem* onLineItem = new QListViewItem(serviceInfoListView, currentItem);
        onLineItem->setText(0, onLine.c_str());
		currentItem = onLineItem;
	}

	TeOGC::TeWMSContactInformation* contactInfo = service->getContactInformation();

	if(contactInfo)
	{
		QListViewItem* contactItem = new QListViewItem(serviceInfoListView, currentItem);
		currentItem = 0;
		contactItem->setText(0, "Contact Information");

		if(contactInfo->getPersonPrimary())
		{
			if(!contactInfo->getPersonPrimary()->getPerson().empty())
			{
				std::string contPers = "Contact Person: "; 
				contPers += contactInfo->getPersonPrimary()->getPerson();
				currentItem = new QListViewItem(contactItem);
				currentItem->setText(0, contPers.c_str());
			}

			if(!contactInfo->getPersonPrimary()->getOrganization().empty())
			{
				std::string org = "Organization: ";
				org += contactInfo->getPersonPrimary()->getOrganization();
				currentItem = new QListViewItem(contactItem, currentItem);
				currentItem->setText(0, org.c_str());
			}
		}

		if(!contactInfo->getPosition().empty())
		{
			std::string pos = "Position: ";
			pos += contactInfo->getPosition();
			currentItem = new QListViewItem(contactItem, currentItem);
			currentItem->setText(0, pos.c_str());
		}

		if(contactInfo->getAddress())
		{
			QListViewItem* addressItem = new QListViewItem(contactItem, currentItem);
			currentItem = 0;
			addressItem->setText(0, "Address");

			if(!contactInfo->getAddress()->getAddressType().empty())
			{
				std::string address  = "Address Type: ";
				            address += contactInfo->getAddress()->getAddressType();
				currentItem = new QListViewItem(addressItem, currentItem);
				currentItem->setText(0, address.c_str());
			}

			if(!contactInfo->getAddress()->getAddress().empty())
			{
				std::string address  = "Address: ";
				            address += contactInfo->getAddress()->getAddress();
				currentItem = new QListViewItem(addressItem, currentItem);
				currentItem->setText(0, address.c_str());
			}

			if(!contactInfo->getAddress()->getCity().empty())
			{
				std::string city  = "City: ";
				            city += contactInfo->getAddress()->getCity();
				currentItem = new QListViewItem(addressItem, currentItem);
				currentItem->setText(0, city.c_str());
			}

			if(!contactInfo->getAddress()->getPostCode().empty())
			{
				std::string postal  = "Postal Code: ";
							postal += contactInfo->getAddress()->getPostCode();
				currentItem = new QListViewItem(addressItem, currentItem);
				currentItem->setText(0, postal.c_str());
			}

			if(!contactInfo->getAddress()->getStateOrProvince().empty())
			{
				std::string state  = "State or Province: ";
				            state += contactInfo->getAddress()->getStateOrProvince();
				currentItem = new QListViewItem(addressItem, currentItem);
				currentItem->setText(0, state.c_str());
			}

			if(!contactInfo->getAddress()->getCountry().empty())
			{
				std::string country  = "Country: ";
							country += contactInfo->getAddress()->getCountry();
				currentItem = new QListViewItem(addressItem, currentItem);
				currentItem->setText(0, country.c_str());
			}

			currentItem = addressItem;
		}

		if(!contactInfo->getVoiceTelephone().empty())
		{
			std::string vPhone = "Voice Phone: ";
			            vPhone += contactInfo->getVoiceTelephone();
			currentItem = new QListViewItem(contactItem, currentItem);
			currentItem->setText(0, vPhone.c_str());
		}

		if(!contactInfo->getFacsimileTelephone().empty())
		{
			std::string fPhone  = "Facsimile Phone: ";
					    fPhone += contactInfo->getFacsimileTelephone();
			currentItem = new QListViewItem(contactItem, currentItem);
			currentItem->setText(0, fPhone.c_str());
		}

		if(!contactInfo->getElectronicMailAddress().empty())
		{
            std::string mail  = "E-mail: ";
						mail += contactInfo->getElectronicMailAddress();
			currentItem = new QListViewItem(contactItem, currentItem);
			currentItem->setText(0, mail.c_str());
		}
	}
}

void FillLayerInfo(TeOGC::TeWMSLayerSection* layer, QListView* layerAttrListView)
{
	layerAttrListView->clear();

	if(!layer)
	{
		QMessageBox::warning(layerAttrListView, QObject::tr(MESSAGEBOXTITLE), QObject::tr("Invalid layer information."));
		return;
	}    

	layerAttrListView->setRootIsDecorated(true);
	layerAttrListView->setSorting(-1);
	std::string title = "Title: ";
	title += layer->getTitle();
	QListViewItem* titleItem = new QListViewItem(layerAttrListView);
	titleItem->setText(0, title.c_str());

	std::vector<std::string> crsVec = layer->getCRSList();
	if(crsVec.size() > 0)
	{
		QListViewItem* crsItem = new QListViewItem(layerAttrListView, titleItem);
		crsItem->setText(0, "CRS");

		for(unsigned int i = 0; i < crsVec.size(); i++)
		{
			QListViewItem* crsSubItem = new QListViewItem(crsItem);
			crsSubItem->setText(0, crsVec[i].c_str());
		}
	}
	
	const std::vector<TeOGC::TeWMSBoundingBox*>& mbrsVec = layer->getMBRList();
	if(mbrsVec.size() > 0)
	{
		QListViewItem* mbrsItem = new QListViewItem(layerAttrListView, titleItem);
		mbrsItem->setText(0, "MBRS");

		for(unsigned int i = 0; i < mbrsVec.size(); i++)
		{
			//crs
			std::string crsMBRS = "CRS: " + mbrsVec[i]->getCRS();
			QListViewItem* crsMBRSItem = new QListViewItem(mbrsItem);
			crsMBRSItem->setText(0, crsMBRS.c_str());
			//upper right X
			std::string upperX = "Upper Right X: " + Te2String(mbrsVec[i]->getMaxX(), 9);
			QListViewItem* upperXItem = new QListViewItem(mbrsItem, crsMBRSItem);
			upperXItem->setText(0, upperX.c_str());
			// lower left X
			std::string lowerX = "Lower Left X: " + Te2String(mbrsVec[i]->getMinX(), 9);
			QListViewItem* lowerXItem = new QListViewItem(mbrsItem, upperXItem);
			lowerXItem->setText(0, lowerX.c_str());
			//upper right Y
			std::string upperY = "Upper Right Y: " + Te2String(mbrsVec[i]->getMaxY(), 9);
			QListViewItem* upperYItem = new QListViewItem(mbrsItem, lowerXItem);
			upperYItem->setText(0, upperY.c_str());
			//lower left Y
			std::string lowerY = "Lower Left Y: " + Te2String(mbrsVec[i]->getMinY(), 9);
			QListViewItem* lowerYItem = new QListViewItem(mbrsItem, upperYItem);
			lowerYItem->setText(0, lowerY.c_str());
		}
	}

	//query
	bool isQueryable = layer->getQueryable();
	std::string queryFlag;
	if(isQueryable)
		queryFlag = "Query: Is Queryable";
	else
		queryFlag = "Query: Is Not Queryable";

	QListViewItem* queryItem = new QListViewItem(layerAttrListView, titleItem);
	queryItem->setText(0, queryFlag.c_str());
	
	//fixed Width
	std::string width = "Fixed Width: " + Te2String(layer->getFixedWidth(), 2);
	QListViewItem* widthItem = new QListViewItem(layerAttrListView, queryItem);
	widthItem->setText(0, width.c_str());
	//fixed Height
	std::string height = "Fixed Height: " + Te2String(layer->getFixedHeight(), 2);
	QListViewItem* heightItem = new QListViewItem(layerAttrListView, widthItem);
	heightItem->setText(0, height.c_str());
}

void FillLayerTree(TeOGC::TeWMSLayerSection* layerSection, QListView* layerListView)
{
	if(!layerSection)
	{
		QMessageBox::warning(layerListView, QObject::tr(MESSAGEBOXTITLE), QObject::tr("No layers supplied."));
		return;
	}

	layerListView->clear();
	layerListView->setRootIsDecorated(true);
	layerListView->addColumn("Layer Name", QListView::Manual);
	layerListView->hideColumn(1);

// layer principal topo da arvore
	QListViewItem* firstLayer = new QCheckListItem(layerListView, layerSection->getTitle().c_str(), QCheckListItem::CheckBox);
	firstLayer->setText(1, layerSection->getName().c_str());
	layerListView->insertItem(firstLayer);

// obtem layers secundarios
	const std::vector<TeOGC::TeWMSLayerSection*>& layerVec = layerSection->getLayerList();

	//layerListView->setOpen(firstLayer, true);
	firstLayer->setPixmap(0, QPixmap::fromMimeSource("infolayerWMS.bmp"));
	std::vector<TeOGC::TeWMSLayerSection*>::const_iterator itLayer = layerVec.begin();
	while (itLayer != layerVec.end())
	{
		TeOGC::TeWMSLayerSection* layer = *itLayer;
		//layers secundarios folhas da arvore
		QListViewItem* itemChild = new QCheckListItem(firstLayer, layer->getTitle().c_str(), QCheckListItem::CheckBox);
		itemChild->setText(1, layer->getName().c_str());
		firstLayer->insertItem(itemChild);
		itemChild->setPixmap(0, QPixmap::fromMimeSource("infolayerWMS.bmp"));
		if(!layer->getLayerList().empty())
		{
			//layerAttrListView->setOpen(itemChild, true);
			FillSubLayerTree(layer, itemChild);
		}
		++itLayer;		
	}
}

void FillStyleTree(const std::vector<TeOGC::TeWMSStyle*>& styles, QListView* styleListView, const std::string& deafultStyleName)
{
	if(styles.empty())
		return;

	styleListView->clear();
	styleListView->setRootIsDecorated(true);
	styleListView->addColumn("Style Name", QListView::Manual);
	styleListView->hideColumn(1);
	

// controller principal topo da arvore
	QCheckListItem* firstStyle = new QCheckListItem(styleListView, QObject::tr("Avaible Styles"), QCheckListItem::RadioButtonController);
	firstStyle->setText(1, "Avaible Styles");
	firstStyle->setOpen(true);
	styleListView->insertItem(firstStyle);	

// show styles
	for(unsigned int i = 0; i < styles.size(); ++i)
	{
		QCheckListItem* itemChild = new QCheckListItem(firstStyle, styles[i]->getTitle().c_str(), QCheckListItem::RadioButton);
		itemChild->setText(1, styles[i]->getName().c_str());
		firstStyle->insertItem(itemChild);

		if(deafultStyleName == styles[i]->getName())
			itemChild->setState(QCheckListItem::On);
	}
}

void FillSubLayerTree(TeOGC::TeWMSLayerSection* layerSection, QListViewItem* item)
{
	const std::vector<TeOGC::TeWMSLayerSection*>& layerVec = layerSection->getLayerList();

	std::vector<TeOGC::TeWMSLayerSection*>::const_iterator itLayer = layerVec.begin();

	while(itLayer != layerVec.end())
	{
		TeOGC::TeWMSLayerSection* layer = *itLayer;

		QListViewItem* itemChild = new QCheckListItem(item, layer->getTitle().c_str(), QCheckListItem::CheckBox);
		itemChild->setText(1, layer->getName().c_str());
		item->insertItem(itemChild);
		itemChild->setPixmap(0, QPixmap::fromMimeSource("infolayerWMS.bmp"));

		if(layer->getLayerList().size() > 0)
			FillSubLayerTree(layer, itemChild);

		++itLayer;
	}
}

void FillServiceAddreeTable(const TeOGC::TeOWSThemeServiceType& serviceType, QTable* addressTable)
{

	try
	{
		std::vector<TeOGC::TeOWSThemeServiceInfo*> services;

		TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().getServicesInfo(services);
		addressTable->setNumRows(0);

        for(unsigned int i = 0; i < services.size(); ++i)
        {

        	TeOGC::TeOWSThemeServiceInfo* t = services[i];
			if(services[i]->getType()->getId() != serviceType.getId())
				continue;

			addressTable->setNumRows(addressTable->numRows()+1);

            std::string title = services[i]->getTitle();
			std::string url("");

			for(unsigned int j = 0; j < services[i]->getOperations().size(); ++j)
				if(TeConvertToUpperCase(services[i]->getOperations()[j].getServiceOperation()->getName()) == std::string("GETCAPABILITIES"))
				{
					url = services[i]->getOperations()[j].getOperationURL();
					break;
				}

			addressTable->setText(addressTable->numRows()-1, 0, title.c_str());
			addressTable->setText(addressTable->numRows()-1, 1, url.c_str());
        }

        addressTable->adjustColumn(0);
        addressTable->adjustColumn(1);
	}
	catch(const TeOGC::TeWMSException& e)
	{
		QMessageBox::critical(addressTable, QObject::tr(MESSAGEBOXTITLE), e.getErrorMessage().c_str());
	}
	catch(...)
	{
		QMessageBox::critical(addressTable, QObject::tr(MESSAGEBOXTITLE), QObject::tr("Unexpected error while filling address server table."));
	}
}

void FillOperationTypeFormat(TeOGC::TeWMSOperationType* operation, QComboBox* formatComboBox)
{
	formatComboBox->clear();

	if(!operation)
		return;

	const std::vector<std::string>& formats = operation->getFormatList();

	for(unsigned int i = 0; i < formats.size(); ++i)
		formatComboBox->insertItem(formats[i].c_str());

	if(formatComboBox->count() > 0)
        formatComboBox->setCurrentItem(0);
}

void FillProjection(TeOGC::TeWMSLayerSection* layerSection, QComboBox* projComboBox, TeView* view)
{
	projComboBox->clear();

	if(!layerSection)
		return;

	const std::vector<std::string>& projVec = layerSection->getCRSList();

	int iEPSGCode = TeOGC::TeGetEPSGFromTeProjection(view->projection());
	std::string sEPSGCode = "EPSG:" + Te2String(iEPSGCode);

	int currentItem = 0;

	for(unsigned int i = 0; i < projVec.size(); ++i)
	{
		projComboBox->insertItem(projVec[i].c_str());

		if(projVec[i] == sEPSGCode)
			currentItem = i;
	}

	projComboBox->setCurrentItem(currentItem);
}

void GetCheckedLayers(std::vector<TeOGC::TeWMSLayerSection*>& layerVec, QListView* layerListView, TeOGC::TeWMSLayerSection* layerSection)
{
	layerVec.clear();

	if(!layerListView)
		return;

	QListViewItemIterator it(layerListView);

	while(it.current())
	{
		if(((QCheckListItem*)it.current())->isOn())
		{
			string layerName = it.current()->text(1);

			TeOGC::TeWMSLayerSection* layer = layerSection->getLayer(layerName);

			if(!layer)
			{
				QMessageBox::warning(0, MESSAGEBOXTITLE, "Could not find a given layer.");
				return;
			}

			layerVec.push_back(layer);
		}

		++it;
	}
}

void DisableSubItens(QCheckListItem* item)
{
	if(!item)
		return;

	QCheckListItem* childItem = dynamic_cast<QCheckListItem*>(item->firstChild());

	while(childItem)
	{
		childItem->setEnabled(false);
		childItem->setOn(false);
		childItem = dynamic_cast<QCheckListItem*>(childItem->nextSibling());
	}

	return;
}

void EnableSubItens(QCheckListItem* item)
{
	if(!item)
		return;

	QCheckListItem* childItem = dynamic_cast<QCheckListItem*>(item->firstChild());

	while(childItem)
	{
		childItem->setEnabled(true);
		childItem->setOn(false);
		childItem = dynamic_cast<QCheckListItem*>(childItem->nextSibling());
	}

	return;
}

bool GetBoundingBox(TeOGC::TeWMSLayerSection* layerSection, const std::string& crs, TeBox& b)
{
	if(!layerSection)
		return false;

	if(layerSection->getGeoMBR())
	{
		TeOGC::TeWMSEX_GeographicBoundingBox* bbox = layerSection->getGeoMBR();
		if(bbox->getCRS() == crs)
		{
			b = TeBox(bbox->getWestBoundLongitude(), bbox->getSouthBoundLatitude(), bbox->getEastBoundLongitude(), bbox->getNorthBoundLatitude());
			return true;
		}
	}

	const std::vector<TeOGC::TeWMSBoundingBox*>& mbrList = layerSection->getMBRList();

	for(unsigned int i = 0; i < mbrList.size(); ++i)
	{
		TeOGC::TeWMSBoundingBox* bbox = mbrList[i];

		if(bbox->getCRS() == crs)
		{
			b = TeBox(bbox->getMinX(), bbox->getMinY(), bbox->getMaxX(), bbox->getMaxY());
			return true;
		}
	}

	return GetBoundingBox(layerSection->getParentLayer(), crs, b);
}

bool GetLatLongBoundingBox(TeOGC::TeWMSLayerSection* layerSection, std::string& crs, TeBox& b)
{
	if(!layerSection)
		return false;

	if(layerSection->getGeoMBR())
	{
		TeOGC::TeWMSEX_GeographicBoundingBox* bbox = layerSection->getGeoMBR();
		crs = bbox->getCRS();
		b = TeBox(bbox->getWestBoundLongitude(), bbox->getSouthBoundLatitude(), bbox->getEastBoundLongitude(), bbox->getNorthBoundLatitude());
		return true;
	}

	return GetLatLongBoundingBox(layerSection->getParentLayer(), crs, b);
}

bool GetStyleName(QListView* styleListView, std::string& styleName)
{
	QListViewItem* root = styleListView->firstChild();

	if(root)
	{
        QListViewItem* item = root->firstChild();

		while(item)
		{
			QCheckListItem* checkItem = dynamic_cast<QCheckListItem*>(item);
			if(checkItem && checkItem->isOn())
			{
				styleName = checkItem->text(1).latin1();
				return true;
			}

			item = root->nextSibling();
		}
	}

	return false;
}

TeOGC::TeXMLDecoder* OpenXML(const std::string& path)
{
	TeOGC::TeXMLParserParams par;
	TeOGC::TeXMLDecoder* xmlDecoder = TeOGC::TeXMLDecoderFactory::make(par);
	try
    {
		xmlDecoder->setDocumentFromFile(path);
    }
	catch(const TeOGC::TeXMLException& e)
	{
		delete xmlDecoder;
		throw e;
	}

	return xmlDecoder;
}

TeOGC::TeSLDUserDefinedSymbolization*
GetUserDefinedSymbolization(const std::vector<TeOGC::TeWMSExtendedCapability*>& extCapa)
{
	for(unsigned int i = 0; i < extCapa.size(); ++i)
	{
		TeOGC::TeSLDUserDefinedSymbolization* uds = dynamic_cast<TeOGC::TeSLDUserDefinedSymbolization*>(extCapa[i]);
		if(uds != 0)
			return uds;
	}
	return 0;
}

void FillUserDefSymbolization(TeOGC::TeSLDUserDefinedSymbolization* userDefSymb, QListView* serviceInfoListView)
{
	if(userDefSymb == 0)
		return;

	// Root element - User Defined Symbolization
	QListViewItem* userDefSymbItem = new QListViewItem(serviceInfoListView, serviceInfoListView->lastItem());
	userDefSymbItem->setText(0, "User Defined Symbolization");

	// Remote WCS
	std::string txt = "Remote WCS: ";
	userDefSymb->supportRemoteWCS() ? txt += "Yes" : txt += "No";

	QListViewItem* wcsItem = new QListViewItem(userDefSymbItem);
	wcsItem->setText(0, txt.c_str());

	// Remote WFS
	txt = "Remote WFS: ";
	userDefSymb->supportRemoteWFS() ? txt += "Yes" : txt += "No";

	QListViewItem* wfsItem = new QListViewItem(userDefSymbItem);
	wfsItem->setText(0, txt.c_str());

	// Inline Feature Data
	txt = "Inline Feature Data: ";
	userDefSymb->supportInlineFeature() ? txt += "Yes" : txt += "No";

	QListViewItem* ftDataItem = new QListViewItem(userDefSymbItem);
	ftDataItem->setText(0, txt.c_str());

	// User Style
	txt = "User Style: ";
	userDefSymb->supportUserStyle() ? txt += "Yes" : txt += "No";

	QListViewItem* userStyleItem = new QListViewItem(userDefSymbItem);
	userStyleItem->setText(0, txt.c_str());

	// User Layer
	txt = "User Layer: ";
	userDefSymb->supportUserLayer() ? txt += "Yes" : txt += "No";

	QListViewItem* userLayerItem = new QListViewItem(userDefSymbItem);
	userLayerItem->setText(0, txt.c_str());

	// SLD
	txt = "SLD: ";
	userDefSymb->supportSLD() ? txt += "Yes" : txt += "No";

	QListViewItem* sldItem = new QListViewItem(userDefSymbItem);
	sldItem->setText(0, txt.c_str());
}
