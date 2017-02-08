#include "DisolvePluginCode.h"
#include "DisolvePluginMainWindow.h"

#include <PluginBase.h>

#include <TeSharedPtr.h>

#include <qmessagebox.h>
#include <qtextedit.h>
#include <qwidget.h>
#include <qtextcodec.h>
#include <qaction.h>

#include <string>

/* A pointer to the plugin main window instance */
TeSharedPtr< DisolvePluginMainWindow > main_window_instance_ptr;

/* A pointer to the plugin tool tip action */

TeSharedPtr< QAction > plugin_action_ptr;

//
// This function will be executed at plugin's loading time.
//
SPL_PLUGIN_API bool SPL_INIT_NAME_CODE( slcPluginArgs* a_pPluginArgs )
{
  /* Extracting plugin parameters */
  
  void* arg_ptrs[ 1 ];
  
  if( a_pPluginArgs->GetArg( 0, arg_ptrs ) == 0 ) {
    PluginParameters* plug_pars_ptr = ( PluginParameters* ) arg_ptrs[ 0 ];
    
    if( plug_pars_ptr->getVersion() == PLUGINPARAMETERS_VERSION ) {
      
      /* This is needed for windows - TeSingletons doesn't work with DLLs */
      
      TeProgress::setProgressInterf( plug_pars_ptr->teprogressbase_ptr_ );    
      
      /* Creating the plugin main window */
      
      main_window_instance_ptr.reset( new DisolvePluginMainWindow(plug_pars_ptr) );    
        
    }
  }
  
  return true;
}

//
// This function will be executed at plugin's unloading time.
//
SPL_PLUGIN_API bool SPL_SHUTDOWN_NAME_CODE( slcPluginArgs* a_pPluginArgs )
{
  /* Extracting plugin parameters */
  
  void* arg_ptrs[ 1 ];
  
  if( a_pPluginArgs->GetArg( 0, arg_ptrs ) == 0 ) {
    PluginParameters* plug_pars_ptr = 0;
    plug_pars_ptr = ( PluginParameters* ) arg_ptrs[ 0 ];
    
    /* Destroying the tool button */
    
    plugin_action_ptr.reset( 0 );
    
    /* Destroying the plugin main window */
    
    main_window_instance_ptr.reset( 0 );
  }
  
  return true;
}


//
// This function will be executed when the user activates the plugin
// menu by clicking on it.
//
SPL_PLUGIN_API bool SPL_RUN_NAME_CODE( slcPluginArgs* a_pPluginArgs )
{
  /* Extracting plugin parameters */
  
  void* arg_ptrs[ 1 ];
  
  if( a_pPluginArgs->GetArg( 0, arg_ptrs ) == 0 ) {
    PluginParameters* plug_pars_ptr = 0;
    plug_pars_ptr = ( PluginParameters* ) arg_ptrs[ 0 ];
    
    /* Updating the main window with the new parameters */
  
    main_window_instance_ptr->showPluginWindow();
    
    return true;
  } else {
    return false;
  }
}

