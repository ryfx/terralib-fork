#include <spl.h>
#if SPL_PLATFORM == SPL_PLATFORM_WIN32
  // Include windows.h - Needed for Windows projects.
  #include <windows.h>
#endif
#include <PluginParameters.h>

SPL_DEFINE_PLUGIN_EXPORTS();

SPL_DEFINE_PLUGIN_INFO( 
  1, // Build number (xx.1.0)..
  0, // Major version (1.xx.0).	
  1, // Minor version (0.1.xx).
  true,// Always "true".
  "Mercado",// The plugin's name.
  "INPE-DPI",// The plugin's author.
  "A Mercado Plugin",// The plugin's general description.
  PLUGINPARAMETERS_VERSION,// Always "PLUGINPARAMETERS_VERSION".
  "http://www.dpi.inpe.br",// The plugin's homepage.
  "eric@dpi.inpe.br",// The plugin author's email.
  "TerraViewPlugin.Generic Plugins" );//Plugins menu disposition
  
SPL_DEFINE_PLUGIN_DLLMAIN();
SPL_IMPLEMENT_PLUGIN_GETINFO();

