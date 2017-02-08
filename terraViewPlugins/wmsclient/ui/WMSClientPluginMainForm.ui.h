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
/** \file WMSLientPluginMainForm.ui.h
 *  \brief This file contains the implementation of the theme creation window of WMS plugin.
 *  \author Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
 */

// WMS Plugin include files
#include "WMSClientPluginUtils.h"

// TerraOGC WMS include files
#include <wms/TeWMSGetCapabilitiesResponse.h>
#include <wms/TeWMSGetMapRequest.h>
#include <wms/TeWMSGetMapResponse.h>
#include <wms/TeWMSServiceMetadata.h>
#include <wms/TeWMSServiceSection.h>
#include <wms/TeWMSCapability.h>
#include <wms/TeWMSRequestSection.h>
#include <wms/TeWMSOperationType.h>
#include <wms/TeWMSBoundingBox.h>
#include <wms/TeWMSEX_GeographicBoundingBox.h>
#include <wms/TeWMSException.h>
#include <wms/TeWMSDCPType.h>
#include <wms/TeWMSHTTP.h>
#include <wms/TeWMSOnlineResource.h>
#include <wms/TeWMSStyle.h>

// TerraOGC WMS client
#include <wmsclient/TeWMSClient.h>

// TerraOGC SLD include files
#include <sld/TeSLDUserDefinedSymbolization.h>

// TerraOGC OWS include files
#include <ows/TeOWSMimeType.h>

// TerraOGC WMS Theme include files
#include <wmstheme/TeWMSTheme.h>

// TerraOGC OWS Theme include files
#include <owstheme/TeOWSThemeServiceInfoManager.h>
#include <owstheme/TeOWSThemeServiceOperation.h>
#include <owstheme/TeOWSThemeServiceInfo.h>

// TerraOGC Common include files
#include <common/TeOGCProjection.h>
#include <common/TeOGCUtils.h>

// TerraLib include files
#include <TeView.h>
#include <TeDatabase.h>
#include <TeVectorRemap.h>

// TerraView Support include files
#include <PluginsSignal.h>
#include <TeAppTheme.h>
#include <TeQtThemeItem.h>
#include <TeQtViewItem.h>
#include <TeWaitCursor.h>
#include <help.h>

// Qt include files
#include <qcolordialog.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qimage.h>

// XPM image include files
#include "wmsTheme.xpm"

#define MESSAGEBOXTITLE "WMS Client Plugin"

/* Inicio da implementacao do dialogo do cliente WMS*/
void WMSClientPluginMainForm::destroy()
{
	delete metadataRequest_;
	delete metadataResponse_;
	delete help_;
}

void WMSClientPluginMainForm::nextPageOnePushButton_clicked()
{
/* clear preview pixmaps */
	previewPixmapLabel->setBackgroundColor(Qt::white);

/* connect to server */
	if(servAddressLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr(MESSAGEBOXTITLE), tr("Please, you should select a server address before going on!"));
		return;
	}

	if(!connectToServer(servAddressLineEdit->text().latin1()))
		return;

	if(!saveServiceMetadata())
		return;

/* fill controls */
	TeOGC::TeWMSServiceMetadata* serviceMetadata = metadataResponse_->getServiceMetadata();

	FillLayerTree(serviceMetadata->getCapabilitiesInfo()->getLayerSection(), layerListView);

	titleTextLabel->setText(serviceMetadata->getServiceInfo()->getTitle().c_str());

	FillOperationTypeFormat(serviceMetadata->getCapabilitiesInfo()->getRequestSection()->getMap(), formatComboBox);
	FillOperationTypeFormat(serviceMetadata->getCapabilitiesInfo()->getRequestSection()->getFeatureInfo(), featureInfoComboBox);

/* open next page */
	wmsWidgetStack->raiseWidget(2);
}

void WMSClientPluginMainForm::viewCapDocPushButton_clicked()
{
	if(servAddressLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr(MESSAGEBOXTITLE), tr("Please, you should select a server address before going on!"));
		return;
	}

	if(!metadataResponse_ ||
	   (metadataResponse_->getServiceMetadata()->getCapabilitiesInfo()->getRequestSection()->getCapabilities()->getDCPList()[0]->getHTTP()->getRequestGet()->getLink() != servAddressLineEdit->text().latin1()))
	{
/* connect to server */
		if(!connectToServer(servAddressLineEdit->text().latin1()))
			return;

		if(!saveServiceMetadata())
			return;

/* fill controls */
		TeOGC::TeWMSServiceMetadata* serviceMetadata = metadataResponse_->getServiceMetadata();

		FillLayerTree(serviceMetadata->getCapabilitiesInfo()->getLayerSection(), layerListView);

		titleTextLabel->setText(serviceMetadata->getServiceInfo()->getTitle().c_str());

		FillOperationTypeFormat(serviceMetadata->getCapabilitiesInfo()->getRequestSection()->getMap(), formatComboBox);
		FillOperationTypeFormat(serviceMetadata->getCapabilitiesInfo()->getRequestSection()->getFeatureInfo(), featureInfoComboBox);
	}

	FillServiceInfo(metadataResponse_->getServiceMetadata()->getServiceInfo(), serviceInfoListView);

/* fill user defined symbolization support */
	TeOGC::TeWMSCapability* capa = metadataResponse_->getServiceMetadata()->getCapabilitiesInfo();
	FillUserDefSymbolization(GetUserDefinedSymbolization(capa->getExtendedCapabilitites()), serviceInfoListView);

	wmsWidgetStack->raiseWidget(1);
}

void WMSClientPluginMainForm::nextPageTwoPushButton_clicked()
{
	wmsWidgetStack->raiseWidget(2);
}

void WMSClientPluginMainForm::previousPageThreePushButton_clicked()
{
	wmsWidgetStack->raiseWidget(0);
}

void WMSClientPluginMainForm::colorPushButton_clicked()
{
	QColor	qc = QColorDialog::getColor(bgFrame->backgroundColor(), this);
	bgFrame->setBackgroundColor(qc);
}

void WMSClientPluginMainForm::layerListView_clicked( QListViewItem * item )
{
	if(!item)
		return;

/* fill layer info control */
	std::string layerName = item->text(1);

	TeOGC::TeWMSLayerSection* layerClicked = metadataResponse_->getServiceMetadata()->getCapabilitiesInfo()->getLayerSection()->getLayer(layerName);

	FillLayerInfo(layerClicked, layerAttrListView);

/* makes layer preview */
	if(((QCheckListItem*)item)->isOn())
	{
		PreviewMap(layerClicked);
/* disable sub-layer (child layers) */
		DisableSubItens(dynamic_cast<QCheckListItem*>(item));
	}
	else
	{
/* enable sub-layer (child layers) */
		QPixmap* preview = new QPixmap();
		preview->fill(QColor(255, 255, 255));
		previewPixmapLabel->setPixmap(*preview);
		delete preview;
		EnableSubItens(dynamic_cast<QCheckListItem*>(item));
	}
}

bool WMSClientPluginMainForm::connectToServer( std::string url )
{
	TeWaitCursor waitcursor;
/* clear old request and responses */
	delete metadataResponse_;
	metadataResponse_ = 0;

	delete metadataRequest_;
	metadataRequest_ = new TeOGC::TeWMSGetCapabilitiesRequest;

/* query server */
	try
	{
		TeOGC::TeWMSClient cli;
		cli.setServiceURL(url);
		metadataResponse_ = cli.getCapabilities(*metadataRequest_);

		if(!metadataResponse_)
		{
			waitcursor.resetWaitCursor();
			QMessageBox::warning(this, tr(MESSAGEBOXTITLE), tr("The server response was not understood."));
			return false;
		}
	}
	catch(const TeOGC::TeOGCException& e)
	{
		waitcursor.resetWaitCursor();
		QMessageBox::warning(this, tr(MESSAGEBOXTITLE), e.getErrorMessage().c_str());
		return false;
	}
	catch(...)
	{
		waitcursor.resetWaitCursor();
		QMessageBox::warning(this, tr(MESSAGEBOXTITLE), "Could not connect to server!");
		return false;
	}

	return true;
}

void WMSClientPluginMainForm::addressTable_clicked( int row, int, int, const QPoint & )
{
	if(row == -1)
		return;

	if(addressTable->text(row, 1).isEmpty())
	{
		servAddressLineEdit->setText("");
		return;
	}

	std::string url = addressTable->text(row,1);

	servAddressLineEdit->setText(url.c_str());
}

void WMSClientPluginMainForm::horizontal_header_clicked( int section )
{
	if(section == -1)
		return;

	servAddressLineEdit->setText(addressTable->text(section, 1));
}

void WMSClientPluginMainForm::removePushButton_clicked()
{
    if(addressTable->currentRow() == -1)
	{
		QMessageBox::warning(this, tr(MESSAGEBOXTITLE), tr("Service Entry not selected!"));
		return;
	}

	if(addressTable->text(addressTable->currentRow(), 1).isEmpty())
	{
		QMessageBox::warning(this, tr(MESSAGEBOXTITLE), tr("The selected row is empty and so it cannot be removed!"));
		return;
	}

	std::string getCapabilitiesURL = addressTable->text(addressTable->currentRow(), 1).latin1();

	TeOGC::TeOWSThemeServiceInfo* sInfo = TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceInfoByOperationURL(getCapabilitiesURL);

/* check if another wms theme is using this service info */
	int wmsServiceId = sInfo->getId();

	bool status = false;

	if(!getServiceStatus(wmsServiceId, status))
	{
		QMessageBox::warning(this, tr(MESSAGEBOXTITLE), tr("Error getting service status!"));
		return;
	}

	if(status)
	{
		QMessageBox::warning(this, tr(MESSAGEBOXTITLE), tr("Service Entry is in use by another wms theme!"));
		return;
	}

/* remove service info */
	TeOGC::TeOWSThemeServiceInfo::remove(localDB_, sInfo);
	TeOGC::TeOWSThemeServiceType* serviceType = TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceType("WMS");
	FillServiceAddreeTable(*(serviceType), addressTable);
}

bool WMSClientPluginMainForm::getServiceStatus( int serviceId, bool & status )
{
	std::string sql = "SELECT * FROM te_wms_theme WHERE service_id = " + Te2String(serviceId);

	TeDatabasePortal* dbPortal = localDB_->getPortal();

	if(!dbPortal)
		return false;

	if(!dbPortal->query(sql) )
	{
		dbPortal->freeResult();
		delete dbPortal;
		return false;
	}

	if(dbPortal->fetchRow())
	{
		status = true;
	}

	dbPortal->freeResult();
	delete dbPortal;

    return true;
}

void WMSClientPluginMainForm::init( TeDatabase * localDB, TeView * currentView, TeCommunicator<PluginsSignal> * signalEmitter, TeQtViewItem * currentQtView )
{
/* clear all properties */
	metadataRequest_ = 0;
	metadataResponse_ = 0;
	localDB_ = localDB;
	currentView_ = currentView;
	currentQtView_ = currentQtView;
	help_ = 0;
	previewPixmapLabel->setBackgroundColor(Qt::white);



/* connect slots */
	QHeader* tHeader = addressTable->verticalHeader();
	connect(tHeader, SIGNAL(clicked(int)), this, SLOT(horizontal_header_clicked(int)));

/* fill server address table */
	TeOGC::TeOWSThemeServiceType* serviceType = TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceType("WMS");
	FillServiceAddreeTable(*(serviceType), addressTable);
}


void WMSClientPluginMainForm::okPageThreePushButton_clicked()
{
/* get selected layers */
	std::vector<TeOGC::TeWMSLayerSection*> layerVecSelected;
	GetCheckedLayers(layerVecSelected, layerListView, metadataResponse_->getServiceMetadata()->getCapabilitiesInfo()->getLayerSection());

	if(layerVecSelected.empty())
	{
		QMessageBox::warning(this, tr(MESSAGEBOXTITLE), tr("Please, you should select at least one layer first."));
		return;
	}

/* ask for confirmation in order to create themes */
	QString question = tr("Do you really want to create the following themes:");

	for(unsigned int i = 0; i < layerVecSelected.size(); ++i)
	{
		question += "\n- ";
        question += layerVecSelected[i]->getTitle().c_str();
	}

	int response = QMessageBox::question(this, tr(MESSAGEBOXTITLE), question, tr("Yes"), tr("No"));

	if(response != 0)
		return;

	TeWaitCursor waitcursor;

/* get service info */
	//std::string getCapabilitiesURL = servAddressLineEdit->text().latin1();
	std::string getCapabilitiesURL = metadataResponse_->getServiceMetadata()->getCapabilitiesInfo()->getRequestSection()->getCapabilities()->getDCPList()[0]->getHTTP()->getRequestGet()->getLink();
	TeOGC::TeOWSThemeServiceInfo* sInfo = TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceInfoByOperationURL(getCapabilitiesURL);

/* create a WMS theme for each layer */
	for(unsigned int i = 0; i < layerVecSelected.size(); ++i)
	{
		TeBox bbox;
		std::string crs;

		if(!GetLatLongBoundingBox(layerVecSelected[i], crs, bbox))
		{
			waitcursor.resetWaitCursor();
			QMessageBox::warning(this, tr(MESSAGEBOXTITLE), tr("Invalid layer bounding box: it must have a lat/long bounding box. See Capabilties document for details."));
			return;
		}

		std::string styleName;

		if(!layerVecSelected[i]->getStyleList().empty())
			styleName = layerVecSelected[i]->getStyleList()[0]->getName();

		std::string featureInfoFormat;

		if(featureInfoComboBox->count() > 0)
			featureInfoFormat = featureInfoComboBox->currentText().latin1();

		std::string themeName = layerVecSelected[i]->getTitle();
		std::string baseThemeName = themeName;
		bool nameExists = false;
		unsigned int nameCount = 0;
		do
		{
			nameExists = false;
			std::vector<TeViewNode*> vecNodes = currentView_->themes();
			for(unsigned int i = 0; i < vecNodes.size(); ++i)
			{
				TeViewNode* node = vecNodes[i];
				if(TeConvertToLowerCase(node->name()) == TeConvertToLowerCase(themeName))
				{
					nameExists = true;
					themeName = baseThemeName + "_" + Te2String(nameCount);
					++nameCount;
					break;
				}
			}
		}
		while(nameExists == true);


		std::string bgColor;
		TeOGC::TeGetHexaColor(bgFrame->backgroundColor().red(), bgFrame->backgroundColor().green(), bgFrame->backgroundColor().blue(), bgColor);

		TeOGC::TeWMSTheme* wmsTheme = new TeOGC::TeWMSTheme(themeName,
															layerVecSelected[i]->getName(),
														    layerVecSelected[i]->getTitle(),
															styleName,
														    formatComboBox->currentText().latin1(),
															bgColor,
															transparencyCheckBox->isOn(),
															crs,
															bbox,
															featureInfoFormat,
															sInfo,
															localDB_);

		if(!wmsTheme)
		{
			waitcursor.resetWaitCursor();
			QMessageBox::warning(this, tr(MESSAGEBOXTITLE), tr("Could not create WMS Theme."));
			return;
		}

		wmsTheme->view(currentView_->id());

		try
		{
			if(!wmsTheme->save())
			{
				waitcursor.resetWaitCursor();
				delete wmsTheme;
				QMessageBox::warning(this, tr(MESSAGEBOXTITLE), tr("Could not create WMS Theme."));
				return;
			}

			currentView_->add(wmsTheme);

			if(currentView_->size() == 1)
            {
				currentView_->setCurrentBox(wmsTheme->getThemeBox());

				TeProjection* thProj = wmsTheme->getThemeProjection();

				if(thProj == 0)
					return;

				TeProjection* oldProj = currentView_->projection();
				TeProjection* newProj = NULL;

				newProj = TeProjectionFactory::make(thProj->params());

				TeBox viewBox = currentView_->getCurrentBox();

				viewBox = TeRemapBox(viewBox, oldProj, newProj);

				if((newProj != NULL) && (!(*oldProj == *newProj)))
				{
					int id = oldProj->id();
					delete oldProj;

					newProj->id(id);

					currentView_->setCurrentBox(viewBox);
					currentView_->projection(newProj);

        			if(!localDB_->updateProjection(newProj))
						throw TeException(UNKNOWN_ERROR_TYPE, tr("Error trying to update view projection.").latin1());

        			string sql = "UPDATE te_view SET lower_x=" + Te2String(viewBox.x1())  + ",lower_y=" + Te2String(viewBox.y1()) +
        					",upper_x=" + Te2String(viewBox.x2()) + ",upper_y=" + Te2String(viewBox.y2()) + " WHERE view_id=" + Te2String(currentView_->id());

        			if(!localDB_->execute(sql))
						throw TeException(UNKNOWN_ERROR_TYPE, tr("Error trying to update view box.").latin1());
				}
            }
		}
		catch(const TeOGC::TeOGCException& e)
		{
			waitcursor.resetWaitCursor();
			delete wmsTheme;
			QMessageBox::warning(this, tr(MESSAGEBOXTITLE), e.getErrorMessage().c_str());
			return;
		}

		TeAppTheme* themeExternal = new TeAppTheme(wmsTheme);
		std::string themename = themeExternal->getTheme()->name();
		QString qthemename(themename.c_str());

/* put the new theme on the interface and set it as the current theme */
		TeQtThemeItem* themeItem = new TeQtThemeItem(currentQtView_,
													 qthemename,
													 themeExternal);



		themeItem->setPixmap(0, QPixmap(wmsTheme_xpm));
		themeItem->setOn(true);
	}

	accept();
}

void WMSClientPluginMainForm::previousPageTwoPushButton_clicked()
{
	serviceInfoListView->clear();
	wmsWidgetStack->raiseWidget(0);
}

bool WMSClientPluginMainForm::saveServiceMetadata()
{
/* see if service info is already in cache */
	TeOGC::TeOWSThemeServiceInfo* serviceInfo = TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceInfoByOperationURL(metadataResponse_->getServiceMetadata()->getCapabilitiesInfo()->getRequestSection()->getCapabilities()->getDCPList()[0]->getHTTP()->getRequestGet()->getLink());

	if(serviceInfo)
		return true;

/* otherwise we save its information */
	serviceInfo = new TeOGC::TeOWSThemeServiceInfo;
	serviceInfo->setTitle(metadataResponse_->getServiceMetadata()->getServiceInfo()->getTitle());
	serviceInfo->setType(TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceType("WMS"));

	TeOGC::TeWMSRequestSection* requestSection = metadataResponse_->getServiceMetadata()->getCapabilitiesInfo()->getRequestSection();

	if(requestSection->getCapabilities())
	{
		TeOGC::TeOWSThemeServiceInfoOperation op;
		op.setOperationURL(requestSection->getCapabilities()->getDCPList()[0]->getHTTP()->getRequestGet()->getLink());
		op.setServiceOperation(TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceOperation("GetCapabilities"));
		op.setServiceInfo(serviceInfo);
		serviceInfo->insert(op);
	}

	if(requestSection->getMap())
	{
		TeOGC::TeOWSThemeServiceInfoOperation op;
		op.setOperationURL(requestSection->getMap()->getDCPList()[0]->getHTTP()->getRequestGet()->getLink());
		op.setServiceOperation(TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceOperation("GetMap"));
		op.setServiceInfo(serviceInfo);
		serviceInfo->insert(op);
	}

	if(requestSection->getFeatureInfo())
	{
		TeOGC::TeOWSThemeServiceInfoOperation op;
		op.setOperationURL(requestSection->getFeatureInfo()->getDCPList()[0]->getHTTP()->getRequestGet()->getLink());
		op.setServiceOperation(TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceOperation("GetFeatureInfo"));
		op.setServiceInfo(serviceInfo);
		serviceInfo->insert(op);
	}

	serviceInfo->setVersion(metadataResponse_->getVersion());

	try
	{
		TeOGC::TeOWSThemeServiceInfo::save(localDB_, serviceInfo);
	}
	catch(const TeOGC::TeOGCException& e)
	{
		QMessageBox::critical(this, tr(MESSAGEBOXTITLE), e.getErrorMessage().c_str());
		delete serviceInfo;
		serviceInfo = 0;
		return false;
	}
	catch(...)
	{
		QMessageBox::critical(this, tr(MESSAGEBOXTITLE), "Unexpected error...");
	}
	TeOGC::TeOWSThemeServiceType* serviceType = TeOGC::TeOWSThemeServiceInfoManager::instance().getInstanceForPlugin().findServiceType("WMS");
	FillServiceAddreeTable(*(serviceType), addressTable);

	return true;
}


void WMSClientPluginMainForm::PreviewMap( TeOGC::TeWMSLayerSection * layer )
{
	TeWaitCursor waitcursor;

	if(!layer || (formatComboBox->count() == 0))
		return;

/* clear preview */
	int w = previewPixmapLabel->frameRect().width();
	int h = previewPixmapLabel->frameRect().height();

	try
	{
/* try to find a box to query wms server */
		TeBox bbox;
		std::string crs;

		if(!GetLatLongBoundingBox(layer, crs, bbox))
		{
			waitcursor.resetWaitCursor();
			QMessageBox::warning(this, tr(MESSAGEBOXTITLE), tr("Invalid layer bounding box: it must have a lat/long bounding box. See Capabilties document for details."));
			return;
		}

/* adjust box to query data with aspect ratio */
		TeOGC::TeWMSClient::adjustWindowBox(bbox.x1_, bbox.y1_, bbox.x2_, bbox.y2_, w, h);

/* find style for layer*/
		std::string styleName;

		if(!layer->getStyleList().empty())
			styleName = layer->getStyleList()[0]->getName();

		std::vector<std::string> styleList;
		styleList.push_back(styleName);

/* mount GetMap request */
		TeOGC::TeWMSGetMapRequest mapRequest;
		mapRequest.setVersion(metadataResponse_->getVersion());
		mapRequest.setBBox(bbox.x1_, bbox.y1_, bbox.x2_, bbox.y2_);
		mapRequest.setCRS(crs);
		mapRequest.setBGColor(bgFrame->backgroundColor().red(), bgFrame->backgroundColor().green(), bgFrame->backgroundColor().blue());
		mapRequest.setFormat(formatComboBox->currentText().latin1());
		mapRequest.setTransparent(transparencyCheckBox->isOn());
		mapRequest.setWidth(w);
		mapRequest.setHeight(h);
		std::vector<std::string> layerList;
		layerList.push_back(layer->getName());
		mapRequest.setLayers(layerList);
		mapRequest.setStyles(styleList);

/* send request */
		const std::string& getMapURL = metadataResponse_->getServiceMetadata()->getCapabilitiesInfo()->getRequestSection()->getMap()->getDCPList()[0]->getHTTP()->getRequestGet()->getLink();
		TeOGC::TeWMSClient cli;
		cli.setServiceURL(getMapURL);
		TeOGC::TeWMSGetMapResponse* mapResponse = cli.getMap(mapRequest);

/* draw result */
		std::string imgtype = TeConvertToUpperCase(mapResponse->getMimeType()->getSubType());
		QPixmap* preview = new QPixmap();
		preview->fill(QColor(bgFrame->backgroundColor().red(),
			                 bgFrame->backgroundColor().green(),
							 bgFrame->backgroundColor().blue()));
		bool result = preview->loadFromData((const unsigned char*)(mapResponse->getData()), mapResponse->getDataSize(), imgtype.c_str());

		if(result)
			previewPixmapLabel->setPixmap(*preview);
		else
		{
			waitcursor.resetWaitCursor();
            std::string imgNotSupported = "The image type format is not supported: " +  mapRequest.getFormat() + ". Please, see More Configurations for additional image format.";
			QMessageBox::warning(this, tr(MESSAGEBOXTITLE), imgNotSupported .c_str());
		}

		delete mapResponse;
		delete preview;
	}
	catch(const TeOGC::TeOGCException& e)
	{
		waitcursor.resetWaitCursor();
		QMessageBox::critical(this, tr(MESSAGEBOXTITLE), e.getErrorMessage().c_str());
	}
	catch(...)
	{
		waitcursor.resetWaitCursor();
		QMessageBox::critical(this, tr(MESSAGEBOXTITLE), tr("Could not load preview for this layer!"));
	}

}



void WMSClientPluginMainForm::helpPushButtonClicked()
{
	delete help_;
	help_ = new Help(this, tr(MESSAGEBOXTITLE));
	QString helpFile = "wmsclient_arquivos/wmscreatetheme.htm";
	help_->init(helpFile);
	help_->show();
}


