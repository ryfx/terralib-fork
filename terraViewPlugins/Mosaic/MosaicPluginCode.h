/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright  2001-2004 INPE and Tecgraf/PUC-Rio.
This file is part of TerraView. TerraView is free software; you can redistribute it 
and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

You should have received a copy of the GNU General Public License
along with TerraView.
The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The software provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use of
this program and its documentation.
*************************************************************************************/

#include <spl.h>

#if SPL_PLATFORM == SPL_PLATFORM_WIN32
  // Include windows.h - Needed for Windows projects.
  #include <windows.h>
#endif

#include <PluginParameters.h>

//
// Define standard export symbols for Win32.
// Note: You don't have to remove this macro for other platforms.
//
SPL_DEFINE_PLUGIN_EXPORTS();

//
// Define all plugin info (this info is used by TerraView).
//
SPL_DEFINE_PLUGIN_INFO( 
  1,		// The build number (e.g. xx.1.0)..
  2,		// The major version (e.g. 1.xx.0).	
  0,		// The minor version (e.g. 0.1.xx).
  true,		// Always "true".
  "Mosaic",	// The plugin's name.
  "INPE-DPI",	///< The plugin's vendor.
  "A TerraView plugin to mosaic geo-referenced rasters",	///< The plugin's general description.
  PLUGINPARAMETERS_VERSION,	///< Always "PLUGINPARAMETERS_VERSION".
  "http://www.dpi.inpe.br/~castejon/software/tvplugins/mosaic/",	///< The plugin's homepage.
  "castejon@dpi.inpe.br",	///< The plugin author's email address.
  "TerraViewPlugin.Image Processing" );	// The plugin's UUID (The first string must
                                        // always be "TerraViewPlugin". The next
                                        // strings will define the plugin
                                        // sub-menus. All strings must be separated
                                        // by the a "." (dot). 
  
//
// Define a standard DllMain() function for Win32 plugin DLLs.
// Note: You haven't to delete or comment out this function for other platforms.
// SPL automatically detects the current platform and removes unused/unwanted code
// behind the macros.
//
SPL_DEFINE_PLUGIN_DLLMAIN();

//
// We don't want to change the default get information method, so simply
// use the default implementation.
//
SPL_IMPLEMENT_PLUGIN_GETINFO();
