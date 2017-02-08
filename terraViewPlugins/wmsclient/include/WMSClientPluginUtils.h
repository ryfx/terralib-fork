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
/** \file WMSClientPluginUtils.h
 *  \brief This file contains utilities functions that can be used in WMS Plugin.
 *  \author Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
 */

#ifndef  __TERRAVIEWWMSCLIENTPLUGIN_INTERNAL_WMSCLIENTPLUGINUTILS_H
#define  __TERRAVIEWWMSCLIENTPLUGIN_INTERNAL_WMSCLIENTPLUGINUTILS_H

// STL include files
#include <iostream>
#include <vector>

// TerraLib Forward declarations
class TeView;
struct TeBox;

// Qt Forward declarations
class QListView;
class QListViewItem;
class QTable;
class QComboBox;
class QCheckListItem;

// TerraOGC forward declarations
namespace TeOGC
{
	class TeWMSServiceSection;
	class TeWMSLayerSection;
	class TeOWSThemeServiceType;
	class TeWMSOperationType;
	class TeWMSStyle;
	class TeWMSExtendedCapability;
	class TeSLDUserDefinedSymbolization;
	class TeXMLDecoder;
};

void FillServiceInfo(TeOGC::TeWMSServiceSection* service, QListView* serviceInfoListView);

void FillLayerInfo(TeOGC::TeWMSLayerSection* layer, QListView* layerAttrListView);

/** \fn void FillLayerTree(TeOGC::TeWMSLayerSection* layerSection, QListView* layerListView)
 *  \brief Create a root entry in this list view and then fill it with all the sub-layers in layerSection.
 *  \note All sub-layers will be put as checkable itens.
 */
void FillLayerTree(TeOGC::TeWMSLayerSection* layerSection, QListView* layerListView);

/** \fn void FillStyleTree(const std::vector<TeOGC::TeWMSStyle*>& styles, QListView* styleListView)
 *  \brief Create a root entry in this list view and then fill it with all styles in the vector.
 *  \note All styles will be put as option button itens.
 */
void FillStyleTree(const std::vector<TeOGC::TeWMSStyle*>& styles, QListView* styleListView, const std::string& deafultStyleName);

void FillSubLayerTree(TeOGC::TeWMSLayerSection* layerSection, QListViewItem* item);

void FillServiceAddreeTable(const TeOGC::TeOWSThemeServiceType& serviceType, QTable* addressTable);

void FillOperationTypeFormat(TeOGC::TeWMSOperationType* operation, QComboBox* formatComboBox);

void FillProjection(TeOGC::TeWMSLayerSection* layerSection, QComboBox* projComboBox, TeView* view);

void GetCheckedLayers(std::vector<TeOGC::TeWMSLayerSection*>& layerVec, QListView* layerListView, TeOGC::TeWMSLayerSection* layerSection);

void DisableSubItens(QCheckListItem* item);

void EnableSubItens(QCheckListItem* item);

bool GetBoundingBox(TeOGC::TeWMSLayerSection* layerSection, const std::string& crs, TeBox& b);

bool GetLatLongBoundingBox(TeOGC::TeWMSLayerSection* layerSection, std::string& crs, TeBox& b);

bool GetStyleName(QListView* styleListView, std::string& styleName);

/* \brief  Open a XML document given a path.
 * \param  path The path of XML document. (Ex.: C:\Documents\MyDocument.xml or http://mysite/MyDocument.xml). (Input)
 * \return a XML Decoder associated with the XML documet.
 * \note   The caller of this method will take the ownership of the returned decoder.
 * \exception Exception It throws an exception if something goes wrong during the document access / parsing.
 */
TeOGC::TeXMLDecoder* OpenXML(const std::string& path);

/* \brief  Return the first User Defined Symbolization element given a list of abstracts WMS Extended Capabilities.
 * \param  extCapa The vector of abstracts WMS Extended Capabilities.
 * \return The first User Defined Symbolization found.
 * \note   The caller of this method will NOT take the ownership of the returned element.
 * \note   return 0 case any User Defined Symbolization element is found.
 */
TeOGC::TeSLDUserDefinedSymbolization* GetUserDefinedSymbolization(const std::vector<TeOGC::TeWMSExtendedCapability*>& extCapa);

void FillUserDefSymbolization(TeOGC::TeSLDUserDefinedSymbolization* userDefSymb, QListView* serviceInfoListView);

#endif	// __TERRAVIEWWMSCLIENTPLUGIN_INTERNAL_WMSCLIENTPLUGINUTILS_H
