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
/** \file WMSPropertiesWindow.ui.h
 *  \brief This file contains the implementation of a properties window for WMS plugin.
 *  \author Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
 */
 
// TerraView Plugin include files
#include "WMSClientPluginUtils.h"

// TerraOGC WMS include files
#include <wms/TeWMSGetCapabilitiesResponse.h>
#include <wms/TeWMSServiceMetadata.h>
#include <wms/TeWMSOperationType.h>
#include <wms/TeWMSLayerSection.h>
#include <wms/TeWMSCapability.h>
#include <wms/TeWMSRequestSection.h>

// TerraOGC WMS Theme include files
#include <wmstheme/TeWMSTheme.h>

// TerraOGC Common include files
#include <common/TeOGCException.h>
#include <common/TeOGCUtils.h>

// TerraView Support include files
#include <TeQtThemeItem.h>
#include <TeAppTheme.h>
#include <TeWaitCursor.h>
#include <help.h>

// Qt include files
#include <qmessagebox.h>
#include <qcolordialog.h>

#define MESSAGEBOXTITLE "WMS Client Plugin"

void WMSPropertiesWindow::init( TeQtThemeItem * themeItem )
{
	help_ = 0;
/* initialize properties */
	TeWaitCursor waitcursor;

    qtThemeItem_ = themeItem;

	if(!qtThemeItem_)
		return;

/* get WMS theme pointer */
	TeOGC::TeWMSTheme* theme = dynamic_cast<TeOGC::TeWMSTheme*>(qtThemeItem_->getAppTheme()->getTheme());

	if(!theme)
		return;

/* call GetCapabilities */
	TeOGC::TeWMSGetCapabilitiesResponse* capabilities = 0;
	try
	{
		capabilities = theme->getCapabilities();
	}
	catch(const TeOGC::TeOGCException& e)
	{
		waitcursor.resetWaitCursor();
		QMessageBox::warning(this, tr(MESSAGEBOXTITLE), e.getErrorMessage().c_str());
		return;
	}

	if(!capabilities)
	{
		waitcursor.resetWaitCursor();
		QMessageBox::warning(this, tr(MESSAGEBOXTITLE), tr("Could not get Capabilities document."));
		return;
	}

/* fill controls */
	layerNameLabel->setText(theme->getLayerName().c_str());

	FillServiceInfo(capabilities->getServiceMetadata()->getServiceInfo(), serviceInfoListView);

/* fill user defined symbolization support */ 
	TeOGC::TeWMSCapability* capa = capabilities->getServiceMetadata()->getCapabilitiesInfo();
	FillUserDefSymbolization(GetUserDefinedSymbolization(capa->getExtendedCapabilitites()), serviceInfoListView);

	TeOGC::TeWMSLayerSection* layer = capabilities->getServiceMetadata()->getCapabilitiesInfo()->getLayerSection()->getLayer(theme->getLayerName());
	FillLayerInfo(layer, layerInfoListView);

	FillStyleTree(layer->getStyleList(), styleListView, theme->getLayerStyleName());

	FillOperationTypeFormat(capabilities->getServiceMetadata()->getCapabilitiesInfo()->getRequestSection()->getMap(), imageFormatComboBox);
	imageFormatComboBox->setCurrentText(theme->getImageFormat().c_str());

	FillOperationTypeFormat(capabilities->getServiceMetadata()->getCapabilitiesInfo()->getRequestSection()->getFeatureInfo(), featureInfoComboBox);
	featureInfoComboBox->setCurrentText(theme->getFeatureInfoFormat().c_str());

	int r = 255, g = 255, b = 255;
	TeOGC::TeGetColorFromHexa(theme->getImageBackgroundColor(), r, g, b);
	bgFrame->setBackgroundColor(QColor(r, g, b));

	transparencyCheckBox->setChecked(theme->getImageTransparency());
		
	delete capabilities;
}

void WMSPropertiesWindow::applyPushButtonClicked()
{
	TeWaitCursor waitcursor;

/* get WMS theme pointer */
	TeOGC::TeWMSTheme* theme = dynamic_cast<TeOGC::TeWMSTheme*>(qtThemeItem_->getAppTheme()->getTheme());

	if(!theme)
	{
		applyPushButton->setEnabled(false);
		return;
	}

	std::string styleName;
	GetStyleName(styleListView, styleName);

/* set new properties for WMS theme */
	std::string hcolor;
	TeOGC::TeGetHexaColor(bgFrame->backgroundColor().red(), bgFrame->backgroundColor().green(), bgFrame->backgroundColor().blue(), hcolor);

	theme->setImageBackgroundColor(hcolor);
	theme->setFeatureInfoFormat(featureInfoComboBox->currentText().latin1());
	theme->setImageFormat(imageFormatComboBox->currentText().latin1());
	theme->setImageTransparency(transparencyCheckBox->isChecked());
	theme->setLayerStyleName(styleName);

/* save changes */

	try
	{
		if(!theme->save())
		{
			waitcursor.resetWaitCursor();
			QMessageBox::warning(this, tr(MESSAGEBOXTITLE), tr("Could not get Capabilities document."));
			return;
		}
	}
	catch(const TeOGC::TeOGCException& e)
	{
		waitcursor.resetWaitCursor();
		QMessageBox::warning(this, tr(MESSAGEBOXTITLE), e.getErrorMessage().c_str());
		return;
	}
	
	applyPushButton->setEnabled(false);
}



void WMSPropertiesWindow::enableApplyPushButton()
{
	applyPushButton->setEnabled(true);
}


void WMSPropertiesWindow::colorPushButtonClicked()
{
	QColor	qc = QColorDialog::getColor(bgFrame->backgroundColor(), this);
	bgFrame->setBackgroundColor(qc);
	applyPushButton->setEnabled(true);
}


void WMSPropertiesWindow::helpPushButtonClicked()
{
	delete help_;
	help_ = new Help(this, tr(MESSAGEBOXTITLE));
	QString helpFile = "wmsclient_arquivos/wmsthemeinfo.htm";
	help_->init(helpFile);
	help_->show();
}


void WMSPropertiesWindow::destroy()
{
delete help_;
help_ = 0;
}
