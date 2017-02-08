#include "MosaicPluginMainWindow.h"

#include <TePDIUtils.hpp>
#include <TePDIAlgorithmFactory.hpp>
#include <TePDIBlendStratFactory.hpp>

#include <TeQtViewsListView.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <TeImportRaster.h>
#include <TeQtDatabaseItem.h>

#include <TeDatabaseFactory.h>

#include <qlineedit.h>
#include <qstring.h>
#include <qlistbox.h>
#include <qcombobox.h>
#include <qmessagebox.h>
#include <qcheckbox.h>

#include <float.h>

MosaicPluginMainWindow::MosaicPluginMainWindow( PluginParameters* pp )
: MosaicPluginMainForm( pp->qtmain_widget_ptr_, "MosaicPluginMainWindow", 
  Qt::WType_TopLevel | Qt::WShowModal )
{
  plugin_params_ = pp;
  last_selected_raster_index_ = 0;
    
  /* Updating the available blending types */
  
  TePDIBlendStratFactory::TeFactoryMap::iterator blend_t_it = 
    TePDIBlendStratFactory::instance().begin();
  TePDIBlendStratFactory::TeFactoryMap::iterator blend_t_it_end = 
    TePDIBlendStratFactory::instance().end();
    
  while( blend_t_it != blend_t_it_end ) {
    qblending_type_->insertItem( blend_t_it->first.c_str(), -1 );
  
    ++blend_t_it;
  }
  
  /* Inserting the tool button into TerraView toolbar */
  
  QPtrList< QToolBar > tv_tool_bars_list = 
    pp->qtmain_widget_ptr_->toolBars( Qt::DockTop );
    
  if( tv_tool_bars_list.count() > 0 ) {
    
    plugin_action_ptr_.reset( new QAction( tv_tool_bars_list.at( 0 ) ) );
    
    plugin_action_ptr_->setText( "Mosaic Plugin" );
    plugin_action_ptr_->setMenuText( "Mosaic Plugin" );
    plugin_action_ptr_->setToolTip( "Mosaic Plugin" );
    plugin_action_ptr_->setIconSet( QIconSet( QPixmap::fromMimeSource( 
      "MosaicPluginIcon.bmp" ) ) );
    
    plugin_action_ptr_->addTo( tv_tool_bars_list.at( 0 ) );
    
    connect( plugin_action_ptr_.nakedPointer(), SIGNAL( activated() ), 
      this, SLOT( showPluginWindow() ) );
  }
  
  /* Connection widgets */
  
  connect( qinput_rasters_, SIGNAL( highlighted( int ) ), this,
    SLOT( RasterSelected( int ) ) );
  connect( qinput_bands_, SIGNAL( selectionChanged() ), this,
    SLOT( BandSelectionChanged() ) );     
}


MosaicPluginMainWindow::~MosaicPluginMainWindow()
{
}


void MosaicPluginMainWindow::showPluginWindow()
{
  /* Updating the global plugin parameters to
     the current TerraView state */

  plugin_params_->updatePluginParameters( *plugin_params_ );
  
  /* Cleanning dialog boxes */

  last_selected_raster_index_ = 0;
  qinput_rasters_->clear();
  qinput_bands_->clear();
  selected_rasters_.clear();
  selected_bands_.clear();
  
  /* Updating dialog boxes */

  if( plugin_params_->teqtviewslistview_ptr_ != 0 ) {
    TeQtViewItem* current_view_item = 
      plugin_params_->teqtviewslistview_ptr_->currentViewItem();
      
    if( current_view_item != 0 ) {
      std::vector< QListViewItem* > view_items =
        current_view_item->getChildren();
      
      std::vector< unsigned int > dummy_int_vec;
      dummy_int_vec.push_back( 0 );    
      
      std::vector< QListViewItem* >::iterator it = view_items.begin();
      std::vector< QListViewItem* >::iterator it_end = view_items.end();
    
      while( it != it_end ) {
        std::string theme_name( (*it)->text( 0 ).ascii() );
        TeTheme* selected_theme = 
          plugin_params_->current_view_ptr_->get( theme_name );
          
          
        if( selected_theme != 0 ) {
          TeQtThemeItem* selected_theme_item = 
            plugin_params_->teqtviewslistview_ptr_->currentThemeItem();
            
          if( selected_theme_item != 0 ) {
            if( selected_theme_item->isOn() ) {
              TeLayer* selected_layer = selected_theme->layer();
              
              if( selected_layer->hasGeometry( TeRASTER ) ||
                selected_layer->hasGeometry( TeRASTERFILE ) ) {
                
                SelectedRastersNodeT temp_node;
                temp_node.first = theme_name;
                temp_node.second = TePDITypes::TePDIRasterPtrType( 
                  selected_layer->raster(), true );
                  
                selected_rasters_.push_back( temp_node );
                qinput_rasters_->insertItem( QString( theme_name.c_str() ) );
                selected_bands_.push_back( dummy_int_vec );
              }
            }
          }
        }
      
        ++it;
      }
    }
    
    qoutput_raster_name_->setText( "MosaicRaster" );
     
    if( qinput_rasters_->count() > 0 ) {
      qinput_rasters_->setSelected( 0, true );
    } 
  }
  
  show();
}


void MosaicPluginMainWindow::closeEvent( QCloseEvent* e )
{ 
  e->accept();
}


void MosaicPluginMainWindow::RasterSelected( int index )
{
  if( index < 0 ) return;
  
  qinput_bands_->clear();
  last_selected_raster_index_ = index;
  TePDITypes::TePDIRasterPtrType raster = selected_rasters_[ index ].second;
  
  for( int band = 0 ; band < raster->nBands() ; ++band ) {
    qinput_bands_->insertItem( QString::number( band ) );
  }    
    
  if( selected_bands_[ index ].size() != 0 ) {
    std::vector< unsigned int > old_band_selection = 
      selected_bands_[ index ];
        
    for( unsigned int sbindex = 0 ; 
         sbindex < old_band_selection.size() ; 
         ++sbindex ) {
           
      qinput_bands_->setSelected( old_band_selection[ sbindex ], 
        true );
    }
  }
}


void MosaicPluginMainWindow::BandSelectionChanged()
{
  if( last_selected_raster_index_ < ((int)selected_bands_.size()) ) {
    selected_bands_[ last_selected_raster_index_ ].clear();
  
    for( unsigned int index = 0 ; index < qinput_bands_->count(); 
        ++index ) {
       
      if( qinput_bands_->isSelected( index ) ) {
        selected_bands_[ last_selected_raster_index_ ].push_back( index );
      }
    }
  }
}


void MosaicPluginMainWindow::OkButtonPressed()
{
  /* Verifying bands selection */

  if( selected_bands_.size() < 2 ) {
    QMessageBox::critical( this, "TerraView",
      tr( "At least two rasters are needed" ) );
    return;
  }
  
  for( unsigned int selected_bands_index = 1 ; 
       selected_bands_index < selected_bands_.size() ; 
       ++selected_bands_index ) {
    if( selected_bands_[ selected_bands_index].size() !=
        selected_bands_[ 0 ].size() ) {
        
      QMessageBox::critical( this, "TerraView",
        tr( "The number of selected bands must be the same for all rasters" ) 
        );
      return;
    }
  }
  
  if( selected_bands_[ 0 ].size() < 1 ) {
    QMessageBox::critical( this, "TerraPixel",
      tr( "At least one band must be selected for each raster" ) );
    return;
  }   
  
  /* Specific mosaic parameters checking */
  
  TePDIParameters mosaic_params;
  TeDataType output_pixel_type = TeUNSIGNEDCHAR;
  
  if( qmosaic_type_->currentText() == "GeoMosaic" ) {
  
    mosaic_params.SetParameter( "mosaic_type" , 
      std::string( "batch_geo_mosaic" ) );
    
    /* Inserting input_rasters and bands parameters */
    
    TePDITypes::TePDIRasterVectorType input_rasters;  
    std::vector< int > bands;   
  
    for( unsigned int rasters_index = 0 ; 
         rasters_index < selected_rasters_.size() ; ++rasters_index ) {
    
      TePDITypes::TePDIRasterPtrType raster_ref = 
        selected_rasters_[ rasters_index ].second;
        
      if( raster_ref->params().projection() == 0 ) {
            
        QMessageBox::critical( this, "TerraView",
          tr( "Invalid raster projection - " ) + " " + qinput_rasters_->text( 
          rasters_index ) );
              
        return;            
      }
        
      if( raster_ref->params().projection()->name() == "NoProjection" ) {
            
        QMessageBox::critical( this, "TerraView",
          tr( "Invalid raster projection - " ) + " " + qinput_rasters_->text( 
          rasters_index ) );
              
        return;            
      }        
      
      for( unsigned int raster_bands_index = 0 ; 
        raster_bands_index < selected_bands_[ rasters_index ].size() ;
        ++raster_bands_index ) {
        
        bands.push_back( 
          selected_bands_[ rasters_index ][ raster_bands_index ] );
      }       
        
      input_rasters.push_back( raster_ref );
    }
    
    mosaic_params.SetParameter( "bands" , bands );
    mosaic_params.SetParameter( "input_rasters" , input_rasters );
    
    /* Extra parameters */
    
    mosaic_params.SetParameter( "keep_best_res" , (int)1 );    
    
    output_pixel_type = TePDIUtils::chooseBestPixelType( input_rasters );
  }
  
  /* Blending type switch */
  
  mosaic_params.SetParameter( "blending_type" , 
    std::string( qblending_type_->currentText().ascii() ) );
  
  /* Forced Dummy value use */
  
  if( q_force_dummy_->isChecked() ) {
    bool conversion_ok = false;
    
    double dummy_value = q_dummy_->text().toDouble( &conversion_ok );
    
    if( conversion_ok ) {
      mosaic_params.SetParameter( "dummy_value" , dummy_value );
    } else {
      QMessageBox::critical( this, "TerraView", 
        tr( "Invalid dummy value" ) );    
            
      return;
    }
  }
  
  /* Auto equilize checking */
  
  if( q_auto_equalize_->isChecked() ) {
    mosaic_params.SetParameter( "auto_equalize" , (int)1 );
  }   
 
  /* Verifying layer name */
  
  std::string layer_name = 
    qoutput_raster_name_->text().ascii();
  
  if( plugin_params_->current_database_ptr_->layerExist( layer_name ) ) {
    
     QMessageBox::critical( this, "TerraView", 
        tr( "Invalid output layer name" ) );    
            
     return;    
  }
  
  /* Allocating output raster */
  
  TePDITypes::TePDIRasterPtrType output_raster;
  
  TeRasterParams output_raster_params;
  output_raster_params.nBands( 1 );
  output_raster_params.nlines_ = 10;
  output_raster_params.ncols_ = 10;
  output_raster_params.setDataType( output_pixel_type, -1 );
  
  if( TePDIUtils::TeAllocRAMRaster( output_raster, output_raster_params, 
    TePDIUtils::TePDIUtilsAutoMemPol ) ) {
    
    mosaic_params.SetParameter( "output_raster" , output_raster );
  } else {
    QMessageBox::information( this, "TerraPixel",
      tr( "Output raster creation error" ) );
      
    return;
  }  
  
  /* Mosaic process */
  
  TePDIAlgorithm::pointer mosaic_instance( 
    TePDIAlgorithmFactory::make( "TePDIBatchGeoMosaic", mosaic_params) );
    
  if( mosaic_instance->Apply() ) {
    /* Reseting progres interface instance */
    
    TeProgress::instance()->reset();    
  
    /* Creating the new layer */
    
    TeLayer* newlayer = new TeLayer( layer_name, 
      plugin_params_->current_database_ptr_, output_raster->projection() );
      
    if( newlayer->id() <= 0 ) {
      QMessageBox::information( this, "TerraView",
        tr( "Unable to create the new layer" ) );
       
      return;
    }
  
    if( TeImportRaster( newlayer, output_raster.nakedPointer(),
      256, 256, TeRasterParams::TeZLib, "",
      ( output_raster->params().useDummy_ ? 
      output_raster->params().dummy_[ 0 ] : 0 ), 
      output_raster->params().useDummy_, TeRasterParams::TeNoExpansible ) != 
      0 ) {
      
      plugin_params_->updateTVInterface( *plugin_params_ );
      
      QMessageBox::information( this, "TerraView",
        tr( "Mosaicking done" ) );
        
      close();
    } else {
      QMessageBox::critical( this, "TerraView", 
        tr( "Fail to import raster into database" ) );     
        
      plugin_params_->current_database_ptr_->deleteLayer( newlayer->id() );
      
      return;  
    }
  } else {
     QMessageBox::critical( this, "TerraView",
       tr( "Mosaicking error" ) );
  }
  
  return;
}
