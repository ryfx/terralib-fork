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
  
  #include <UnionPluginMainForm.h>
  #include<TeSharedPtr.h>
  
  #include <PluginParameters.h>
  
  #include <qaction.h>
  
  #include <string>
  #include <vector>

  class UnionPluginMainWindow : public UnionPluginMainForm
  {
    Q_OBJECT
    
    public :
    
      /**
       * Default constructor.
       */      
      UnionPluginMainWindow( PluginParameters* pp );
        
      /**
       * Default destructor.
       */       
      ~UnionPluginMainWindow();
      
    public slots :
      /**
       * Update the form input fields with new plugin parameters and show it.
       */       
      void showPluginWindow();
      
    protected :

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
		void execute();
		void cmbTemaA_activated();
  };

#endif
