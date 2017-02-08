#include <openModellerTVPluginCode.h>
#include <PluginParameters.h>

#include <openModellerPluginWindow.h>

#include <qcombobox.h>

openModellerPluginWindow* window_ptr;

SPL_PLUGIN_API bool SPL_INIT_NAME_CODE( slcPluginArgs* a_pPluginArgs )
{
	/* Retriving parameters */
  void* arg_ptrs[ 1 ];
  a_pPluginArgs->GetArg( 0, arg_ptrs );
  PluginParameters* plug_pars_ptr = 
    ( PluginParameters* ) arg_ptrs[ 0 ];

  /* Creating the plugin window instance */
  window_ptr = new openModellerPluginWindow( plug_pars_ptr->qtmain_widget_ptr_ );

	window_ptr->plugin_params_ = plug_pars_ptr;

	return true;
}

SPL_PLUGIN_API bool SPL_RUN_NAME_CODE( slcPluginArgs* a_pPluginArgs )
{
	if( !window_ptr->loadData() )
  {//*
	  QMessageBox::information ( window_ptr->plugin_params_->qtmain_widget_ptr_,
		  "Open Modeller Plugin", "Terraview is not connected to a database." );
      return false;//*/
  }

	window_ptr->show();
    
  return true;
}

SPL_PLUGIN_API bool SPL_SHUTDOWN_NAME_CODE( slcPluginArgs* a_pPluginArgs )
{
  delete window_ptr;
  
  return true;
}

