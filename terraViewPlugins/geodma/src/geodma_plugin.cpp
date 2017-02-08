#include <geodma_plugin.h>
#include <geodma_window.h>

// QT includes
#include <qmessagebox.h>

static geodma_window* plugin_window = 0;

geodma_plugin::geodma_plugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
  loadTranslationFiles("geodma_");
}

geodma_plugin::~geodma_plugin()
{
  end();
}

void geodma_plugin::init()
{
  TerraViewBase* terraview = getMainWindow();
  
  if ( terraview )
  {
    QPopupMenu *popup_menu = getPluginsMenu();
    
    if ( popup_menu  != 0 )
    {
      popup_menu->insertItem( tr( "GeoDMA" ), this, SLOT( execute_plugin() ) );
    }
  }
}

void geodma_plugin::end()
{
  delete plugin_window;
  plugin_window = 0;
}

void geodma_plugin::execute_plugin()
{
  TerraViewBase* terraview = getMainWindow();
  
  if ( terraview )
  {
    // start geodma here
    plugin_window = new geodma_window( terraview );
    plugin_window->started = false;
    plugin_window->plug_params_ptr_ = params_;
    plugin_window->showEvent( new QShowEvent() );
  }
}
