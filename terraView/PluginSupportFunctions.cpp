#include <PluginSupportFunctions.h>

#include <terraViewBase.h>
#include <display.h>

#include <TeAgnostic.h>

#include <qtranslator.h>


namespace PluginSupportFunctions
{
  void updateTVInterface( const PluginParameters& pp )
  {
    TEAGN_TRUE_OR_THROW( ( pp.qtmain_widget_ptr_ != 0 ),
      "Invalid parent widget pointer");
  
    ((TerraViewBase*)pp.qtmain_widget_ptr_)->popupDatabaseRefreshSlot();
  }
  
  
  bool loadTranslationFile( const PluginParameters& pp,
    const std::string& filename )
  {
    TEAGN_TRUE_OR_THROW( ( pp.qtmain_widget_ptr_ != 0 ),
      "Invalid parent widget pointer");
    
    QApplication* qapp_ptr = 
      ((TerraViewBase*)pp.qtmain_widget_ptr_)->getQApplicationPointer();
      
    TEAGN_TRUE_OR_THROW( ( qapp_ptr != 0 ),
      "Invalid QApplication pointer");
      
    QTranslator* trans_ptr = new QTranslator( pp.qtmain_widget_ptr_,
      "translator" );
        
    if( trans_ptr->load( filename.c_str(), "." ) ) {
      qapp_ptr->installTranslator( trans_ptr );
        
      return true;
    } else {
      delete trans_ptr;
        
      return false;      
    }
  }


  void updatePluginParameters( PluginParameters& pp )
  {
    TEAGN_TRUE_OR_THROW( ( pp.qtmain_widget_ptr_ != 0 ),
      "Invalid parent widget pointer");
  
    pp = ((TerraViewBase*)pp.qtmain_widget_ptr_)->getPluginParameters();
  }
  
  
  TeDatabase* getCurrentDatabasePtr( const PluginParameters& pp )
  {
    TEAGN_TRUE_OR_THROW( ( pp.qtmain_widget_ptr_ != 0 ),
      "Invalid parent widget pointer");
  
    return ((TerraViewBase*)pp.qtmain_widget_ptr_)->currentDatabase();
  }
  
  
  TeLayer* getCurrentLayerPtr( const PluginParameters& pp )
  {
    TEAGN_TRUE_OR_THROW( ( pp.qtmain_widget_ptr_ != 0 ),
      "Invalid parent widget pointer");
  
    return ((TerraViewBase*)pp.qtmain_widget_ptr_)->currentLayer();
  }
  
  
  TeView* getCurrentViewPtr( const PluginParameters& pp )
  {
    TEAGN_TRUE_OR_THROW( ( pp.qtmain_widget_ptr_ != 0 ),
      "Invalid parent widget pointer");
  
    return ((TerraViewBase*)pp.qtmain_widget_ptr_)->currentView();
  }
  
  
  TeAppTheme* getCurrentThemeAppPtr( const PluginParameters& pp )
  {
    TEAGN_TRUE_OR_THROW( ( pp.qtmain_widget_ptr_ != 0 ),
      "Invalid parent widget pointer");
  
    return ((TerraViewBase*)pp.qtmain_widget_ptr_)->currentTheme();
  }      

  void setCurrentThemeAppPtr( TeAppTheme* appTheme, const PluginParameters& pp )
  {
    TEAGN_TRUE_OR_THROW( ( pp.qtmain_widget_ptr_ != 0 ),
      "Invalid parent widget pointer");
  
	((TerraViewBase*)pp.qtmain_widget_ptr_)->setCurrentAppTheme(appTheme);
  }      

  void plotTVData(const PluginParameters& pp )
  {
	 TEAGN_TRUE_OR_THROW( ( pp.qtmain_widget_ptr_ != 0 ),
      "Invalid parent widget pointer");

	((TerraViewBase*)pp.qtmain_widget_ptr_)->getDisplayWindow()->plotData();
  }

  void setPluginMetadata(const PluginParameters& pp, const PluginMetadata& pluginMetadata )
  {
	TEAGN_TRUE_OR_THROW( ( pp.qtmain_widget_ptr_ != 0 ),
      "Invalid parent widget pointer");

	((TerraViewBase*)pp.qtmain_widget_ptr_)->setPluginMetadata(pluginMetadata);
  }
};
