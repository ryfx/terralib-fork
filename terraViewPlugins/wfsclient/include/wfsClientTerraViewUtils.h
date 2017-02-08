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
/** \file WFSClientTerraViewUtils.h
 *  \brief This file contains utilities functions that can be used in WFS Plugin.
 *  \author Frederico Augusto Bede <frederico.bede@funcate.org.br>
 */

#ifndef __TERRAVIEWWFSCLIENTPLUGIN_INTERNAL_WFSCLIENTTERRAVIEWUTILS_H
#define __TERRAVIEWWFSCLIENTPLUGIN_INTERNAL_WFSCLIENTTERRAVIEWUTILS_H

#include <TeLegendEntry.h>

//STL include files
#include <string>

//Forward declarations
namespace TeOGC
{
	class TeWFSCapabilities;
}

class PluginsSignal;
class PluginParameters;
class TeDatabase;
class TeSTElementSet;
class TeQtCanvas;
class TeProjection;
class TeLegendEntry;

class QWidget;

void drawObjects(TeSTElementSet* elemSet, TeQtCanvas* canvas, TeLegendEntry leg, TeProjection* thProj);

void WFSSetPluginParameters(PluginParameters* p);

void cleanAttrDialogPtr();

/* Funcao de call-back para sinalizacao de desenho, consulta de apontamento, troca do banco local. */
void WFSClientPluginSignalHandler(const PluginsSignal& x, void* objptr);

/* Funcao de tratamento de um sinal de desenho de tema WMS */
void WFSClientPluginDrawTheme(const PluginsSignal& x);

/* Funcao de tratamento de ajuste do banco local usado pelo plugin WMS */
void WFSClientPluginSetDatabase(const PluginsSignal& x);

/* Funcao de tratamento de consulta de apontamento sobre o canvas */
void WFSClientPluginPointQuery(const PluginsSignal& x);

/* Funcao de tratamento de abertura de popup sobre tema WMS */
void WFSClientPluginShowThemePopup(const PluginsSignal& x);

/* Funcao que ajusta o icone de um tema WMS */
void WFSClientPluginSetThemeIcon(const PluginsSignal& x);

#endif //__TERRAVIEWWFSCLIENTPLUGIN_INTERNAL_WFSCLIENTTERRAVIEWUTILS_H
