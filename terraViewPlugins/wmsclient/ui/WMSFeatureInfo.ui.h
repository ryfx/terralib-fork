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
/** \file WMSFeatureInfo.ui.h
 *  \brief This file contains the implementation of a feature info display window for WMS plugin.
 *  \author Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
 */

// TerraLib include files
#include <TeUtils.h>

// TerraView include files
#include <help.h>

#define MESSAGEBOXTITLE "WMS Client Plugin"

void WMSFeatureInfo::showFeatureInformation( const string& info )
{
	std::vector<std::string> results;
	TeSplitString(info, "\n", results);
	std::string auxtexto;
	for(unsigned int i = 0; i < results.size(); ++i)
		auxtexto += results[i] + "<br>";

	std::string texto = "<html><body>";
	texto += "<table border='0' bordercolor='#0000FF' cellspacing='0' cellpadding='0'>";
	texto += "<tr>";
	texto += "<td>";
	//texto += info.getFeatureInformation();
	texto += auxtexto;
	texto += "</td>";
	texto += "</tr>";
	texto += "</table></body></html>";
	txtFeatureInformation->setText(texto.c_str());
}


void WMSFeatureInfo::setLayerName( const std::string & name )
{
    lblLayerName->setText(name.c_str());
}


void WMSFeatureInfo::helpPushButtonClicked()
{
	delete help_;
	help_ = new Help(this, tr(MESSAGEBOXTITLE));
	QString helpFile = "wmsclient_arquivos/wmspointquery.htm";
	help_->init(helpFile);
	help_->show();
}


void WMSFeatureInfo::init()
{
    help_ = 0;
}


void WMSFeatureInfo::destroy()
{
    delete help_;
}
