/************************************************************************************
TerraView WFS Client Plugin.
Copyright  2001-2007 INPE.
This file is part of TerraView WFS Client Plugin.
TerraView WFS Client Plugin is free software; you can redistribute it 
and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

You should have received a copy of the GNU General Public License
along with TerraView WFS Client Plugin.
The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The software provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall the author be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use of
this program and its documentation.
*************************************************************************************/
/** \file WFSPluginUtils.h
 *  \brief This file contains utilities functions that can be used in WFS Plugin.
 *  \author Frederico Augusto Bede <frederico.bede@funcate.org.br>
 */

#ifndef __TERRAVIEWWFSPLUGIN_INTERNAL_WFSUTILS_H
#define __TERRAVIEWWFSPLUGIN_INTERNAL_WFSUTILS_H

#include <TeBox.h>

#include <string>

//Forward declarations
namespace TeOGC
{
//	class TeWFSCapabilities;
	class TeWFSTheme;
}

class TeDatabase;
class TeView;
class TeAbstractTheme;
class TeLayer;

class QWidget;
class QTable;

struct WFSParams
{
public:
	WFSParams(const std::string& themeName, const std::string& wfsLayerName, const std::string& uri, const int& serviceId,
		const std::string& projection, const TeBox& bx, const int& viewId=-1) :
	themeName_(themeName),
	wfsLayerName_(wfsLayerName),
	uri_(uri),
	serviceId_(serviceId),
	bx_(bx),
	projection_(projection),
	viewId_(viewId)
	{
	}

	WFSParams(const WFSParams& other) :
	themeName_(other.themeName_),
	wfsLayerName_(other.wfsLayerName_),
	uri_(other.uri_),
	serviceId_(other.serviceId_),
	bx_(other.bx_),
	projection_(other.projection_),
	viewId_(other.viewId_)
	{
	}

	WFSParams& operator=(const WFSParams& other)
	{
		themeName_ = other.themeName_;
		wfsLayerName_ = other.wfsLayerName_;
		uri_ = other.uri_;
		serviceId_ = other.serviceId_;
		bx_ = other.bx_;
		projection_ = other.projection_;
		viewId_ = other.viewId_;

		return *this;
	}

	std::string themeName_,
		wfsLayerName_,
		uri_,
		projection_;
	int	viewId_,
		serviceId_;

	TeBox bx_;
};

//returns the service identifier, or -1 if the operation fails
int saveService(TeDatabase* db, const std::string& url, QWidget* parent=0);

void updateServicesTable(QTable* table);

//Returns the world presented in canvas in theme coordinate system.
TeBox getWorldInThemeProjection(TeView* view, TeAbstractTheme* theme);

TeLayer* importWFS(TeOGC::TeWFSTheme* theme, TeDatabase* db, const TeBox& box, const std::vector<std::string>& attrs, const std::string& layerName);

bool layerExists(const std::string& layerName, TeDatabase* db);

#endif //__TERRAVIEWWFSPLUGIN_INTERNAL_WFSUTILS_H
