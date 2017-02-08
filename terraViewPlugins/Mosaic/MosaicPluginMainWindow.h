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

#ifndef PLUGINMAINWINDOW_HPP
  #define PLUGINMAINWINDOW_HPP
  
  #include <MosaicPluginMainForm.h>
  
  #include <PluginParameters.h>
  
  #include <TePDITypes.hpp>
  
  #include <qaction.h>
  
  #include <string>
  #include <vector>

  /**
   * @brief A class for visual mosaic plugin form.
   * @author Emiliano F. Castejon <castejon@dpi.inpe.br>
   */   
  class MosaicPluginMainWindow : public MosaicPluginMainForm
  {
    Q_OBJECT
    
    public :
    
      /**
       * Default constructor.
       */      
      MosaicPluginMainWindow( PluginParameters* pp );
        
      /**
       * Default destructor.
       */       
      ~MosaicPluginMainWindow();
      
    public slots :
      
      /**
       * Update the form input fields with new plugin parameters and show it.
       */       
      void showPluginWindow();
      
    protected :

      /**
       * Type definition for selected rasters node.
       *
       * @param std::string Theme name.
       * @param TePDITypes::TePDIRasterPtrType Raster pointer.
       */     
      typedef std::pair< std::string, TePDITypes::TePDIRasterPtrType >
        SelectedRastersNodeT;
        
      /**
       * Type definition for selected rasters vector.
       */        
      typedef std::vector< SelectedRastersNodeT > SelectedRastersT;

      /**
       * The index ( visual list class ) of the last selected raster.
       */        
      int last_selected_raster_index_;

      /**
       * The selected rasters vector.
       */        
      SelectedRastersT selected_rasters_;

      /**
       * The selected bands vector ( for each selected raster ).
       */        
      std::vector< std::vector< unsigned int > > selected_bands_;
      
      /**
       * A pointer to the plugin tool tip action 
       */
      TeSharedPtr< QAction > plugin_action_ptr_;      

      /**
       * The current plugin parameters.
       */        
      PluginParameters* plugin_params_;

      /**
       * Close event slot implementation.
       */        
      void closeEvent( QCloseEvent* e );
      
    protected slots :
    
      /**
       * Raster selection event slot implementation.
       */     
      void RasterSelected( int index );
      
      /**
       * Band selection event slot implementation.
       */       
      void BandSelectionChanged();
      
      /**
       * OK buttom click event slot implementation.
       */       
      void OkButtonPressed();
      
  };

#endif
