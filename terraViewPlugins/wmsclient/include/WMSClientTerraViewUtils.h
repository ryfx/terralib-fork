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
/** \file WMSClientTerraViewUtils.h
 *  \brief This file contains utilities functions that can be used in WMS Plugin.
 *  \author Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
 */

#ifndef __TERRAVIEWWMSCLIENTPLUGIN_INTERNAL_WMSCLIENTTERRAVIEWUTILS_H
#define __TERRAVIEWWMSCLIENTPLUGIN_INTERNAL_WMSCLIENTTERRAVIEWUTILS_H

#include <TeException.h>

// TerraView Forward declarations
class PluginsSignal;
class PluginParameters;
class TeDatabase;

void WMSSetPluginParameters(PluginParameters* p);

PluginParameters* WMSGetPluginParameters();

/* Funcao de call-back para sinalizacao de desenho, consulta de apontamento, troca do banco local. */
void WMSClientPluginSignalHandler(const PluginsSignal& x, void* objptr);

/* Funcao de tratamento de um sinal de desenho de tema WMS */
void WMSClientPluginDrawTheme(const PluginsSignal& x);

/* Funcao de tratamento de ajuste do banco local usado pelo plugin WMS */
void WMSClientPluginSetDatabase(const PluginsSignal& x);

/* Funcao de tratamento de consulta de apontamento sobre o canvas */
void WMSClientPluginPointQuery(const PluginsSignal& x);

/* Funcao de tratamento de abertura de popup sobre tema WMS */
void WMSClientPluginShowThemePopup(const PluginsSignal& x);

/* Funcao que ajusta o icone de um tema WMS */
void WMSClientPluginSetThemeIcon(const PluginsSignal& x);

void createOWSTables(TeDatabase* db) throw (TeException);

#endif //__TERRAVIEWWMSCLIENTPLUGIN_INTERNAL_WMSCLIENTTERRAVIEWUTILS_H
