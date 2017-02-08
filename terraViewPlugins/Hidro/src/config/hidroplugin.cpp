#include <hidroplugin.h>

#include <HidroPluginStarter.h>

HidroPlugin::HidroPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	loadTranslationFiles("hidro_");
}

HidroPlugin::~HidroPlugin()
{
	end();
}

void HidroPlugin::init()
{
	//Here you must put the enterings for the plug-in. Use qt signal/slots to do it!
  
  /* Creating the plugin window instance */
	HidroPluginStarter* pluginStarter = new HidroPluginStarter(params_);
  pluginStarter->start();
}

void HidroPlugin::end()
{
}
