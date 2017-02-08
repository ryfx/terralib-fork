/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright © 2001-2007 INPE and Tecgraf/PUC-Rio.
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

#ifndef PLUGINSUPPORTFUNCTIONS_H
  #define PLUGINSUPPORTFUNCTIONS_H
  
  #include <PluginParameters.h>

  #include <TViewDefines.h>
  
  /**
   * @brief A namespace for plugin support functions.
   * @author Emiliano F. Castejon <castejon@dpi.inpe.br>
   */  
  namespace PluginSupportFunctions 
  {
    /**
     * Updates the terraview interface ( database reload ) using
     * the qtparent_widget_ptr_ inside the plugin parameters 
     *
     * @param pp The plugin parameters.  
     */
    TVIEW_DLL void updateTVInterface( const PluginParameters& pp );
    
    /**
     * Load the translation file dictionary into TerraView translation
     * tables.
     *
     * @note The translation file must be created with QTLinguist.
     *
     * @param pp The plugin parameters.  
     * @param filename The translation file name.
     * @return true if OK, otherwise false.
     */
    bool loadTranslationFile( const PluginParameters& pp,
      const std::string& filename );    

    /**
     * Update the supplied plugin parameters using data from 
     * the terraview interface.
     *
     * @param pp The plugin parameters.  
     */
    TVIEW_DLL void updatePluginParameters( PluginParameters& pp );
    
    /**
     * @brief Returns the current database pointer.
     * @param pp The plugin parameters.      
     * @return The current database pointer or zero if 
     * there are no current database.
     */
    TVIEW_DLL TeDatabase* getCurrentDatabasePtr( const PluginParameters& pp );    
    
    /**
     * @brief Returns the current layer pointer.
     * @param pp The plugin parameters.      
     * @return The current layer pointer or zero if 
     * there are no current layer.
     */
    TVIEW_DLL TeLayer* getCurrentLayerPtr( const PluginParameters& pp );      
    
    /**
     * @brief Returns the current view pointer.
     * @param pp The plugin parameters.      
     * @return The current view pointer or zero if 
     * there are no current view.
     */
    TVIEW_DLL TeView* getCurrentViewPtr( const PluginParameters& pp );
    
    /**
     * @brief Returns the current theme application pointer.
     * @param pp The plugin parameters.      
     * @return The current theme application pointer or zero if 
     * there are no current theme application.
     */
    TVIEW_DLL TeAppTheme* getCurrentThemeAppPtr( const PluginParameters& pp );       

	/**
     * @brief Returns the current theme application pointer.
     * @param pp The plugin parameters.      
     * @return The current theme application pointer or zero if 
     * there are no current theme application.
     */
    TVIEW_DLL void setCurrentThemeAppPtr(TeAppTheme*, const PluginParameters& pp );

	/**
     * @brief Function used to plot data using display window pointer.
     * @param pp The plugin parameters.
     */
	TVIEW_DLL void plotTVData(const PluginParameters& pp );

	TVIEW_DLL void setPluginMetadata(const PluginParameters& pp, const PluginMetadata& pluginMetadata );
  };

#endif
