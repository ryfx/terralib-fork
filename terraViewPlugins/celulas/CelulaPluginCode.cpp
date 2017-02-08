#include "CelulaPluginCode.h"
#include "CelulasWindow.h"

#include <PluginBase.h>

#include <TeSharedPtr.h>
#include <qmessagebox.h>
#include <qtextedit.h>
#include <qwidget.h>
#include <qtextcodec.h>

#include <string>


TeSharedPtr< CelulasWindow > createMainWindow( 
  const PluginParameters& plug_pars )
{
  std::string locale_str( QTextCodec::locale() );
  std::string trans_file_name = std::string( "Celulas_" ) + locale_str;
  
  plug_pars.loadTranslationFile( plug_pars, trans_file_name );
  
  TeSharedPtr< CelulasWindow > return_ptr( new CelulasWindow( 
    plug_pars.qtmain_widget_ptr_ ) );
  
  return return_ptr;
}

//
// Implement the plugin run method. 
//
SPL_PLUGIN_API bool SPL_RUN_NAME_CODE( slcPluginArgs* a_pPluginArgs )
{
	/* Extracting plugin parameters */
	void* arg_ptrs[ 1 ];

	if( a_pPluginArgs->GetArg( 0, arg_ptrs ) == 0 )
	{
		static PluginParameters local_plug_pars;
		local_plug_pars = *( ( PluginParameters* ) arg_ptrs[ 0 ] );

		/* This is needed for windows - TeSingletons doesn't work with DLLs */
		TeProgress::setProgressInterf( local_plug_pars.teprogressbase_ptr_ );

		/* Creating the plugin form instance ( this is done statically to 
		create just one instance ) */

		static TeSharedPtr< CelulasWindow > CelulasWindow_instance = createMainWindow( local_plug_pars );

		CelulasWindow_instance->UpdateForm( local_plug_pars );
		//CelulasWindow_instance->show();

		return true;
	}
	else
	{
		return false;
	}
}
