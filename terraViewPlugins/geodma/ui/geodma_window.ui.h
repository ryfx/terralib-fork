/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the Epipform's constructor and
** destructor.
*****************************************************************************/

/**
 * @brief Main Window GeoDMA plugin code
 * @author Thales Sehn Korting <tkorting@dpi.inpe.br>
 * http://www.dpi.inpe.br/geodma/
 */

/* Defines */
#define VERSION "0.2.2"
// #undef _DEBUG
// #define _DEBUG

/* Debug functions */
#ifdef _DEBUG
#include <geodma_debug.h>
#endif

/* TerraLib Includes */
#include <TeAbstractTheme.h>
#include <TeAgnostic.h>
#include <TePDIAlgorithmFactory.hpp>
#include <TePDIBaatz.hpp>
#include <TeDatabase.h>
#include <TeGroupingAlgorithms.h>
#include <TeLayerFunctions.h>
#include <TePDIParallelSegmenter.hpp>
#include <TePDIPIManager.hpp>
#include <TePDIRaster2Vector.hpp>
#include <TePDIRegGrowSeg.hpp>
#include <TeQtCanvas.h>
#include <TeQtGrid.h>
#include <TeQtViewItem.h>
#include <TeQtViewsListView.h>
#include <TeSharedPtr.h>
#include <TeVisual.h>

/* Data Mining Includes */
#include <base_classifier.h>
#include <c45_classifier.h>
#include <som_classifier.h>
#include <nn_classifier.h>

/* QWT Includes */
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_symbol.h>

/* Internal Includes */
#include <curve_features.h>
#include <geodma_colors.h>
#include <geodma_distances.h>
#include <feature_names.h>
#include <TeLandscape.h>
#include <polygons_features.h>
#include <TextureProperties.h>

/* QT Includes */
#include <qbuttongroup.h>
#include <qfiledialog.h>
#include <qfont.h>
#include <qgrid.h>
#include <qmessagebox.h>
#include <qobjectlist.h>
#include <qstring.h>

/* Other Includes */
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <time.h>

using namespace std;

int get_tab_number(string tabname);

void geodma_window::init()
{
  #ifdef _DEBUG
  debug.start( "init()" );
  #endif

  initialize_frames();
  neet_to_update_tv = false;

  #ifdef _DEBUG
  debug.finish( "init()" );
  #endif
}

void geodma_window::destroy()
{
  #ifdef _DEBUG
  debug.start( "destroy()" );
  #endif

  q_frame->removeChild( myScatterplot );
  q_fr_timeseries_mt->removeChild( myTimeSeriesPlot );
  q_fr_polarvisualization_mt->removeChild( myPolarPlot );

  disconnect( plug_params_ptr_->teqtcanvas_ptr_, SIGNAL( mousePressed( TeCoord2D&, int, QPoint& ) ), this, SLOT( getMousePressed( TeCoord2D&, int, QPoint& ) ) );

  #ifdef _DEBUG
  debug.finish( "destroy()" );
  #endif
}

void geodma_window::closeEvent( QCloseEvent *e )
{
  #ifdef _DEBUG
  debug.start( "closeEvent()" );
  #endif

  disconnect( (QObject*) plug_params_ptr_->teqtdatabaseslistview_ptr_, 0, this, 0 );
  e->accept();

  #ifdef _DEBUG
  debug.finish( "closeEvent()" );
  #endif
}

/* ========== Functions to select/unselect items on listBoxes ========== */
void geodma_window::q_tb_select_attribute_norm_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_tb_select_attribute_norm_clicked()" );
  #endif

  vector<string> remain_items;
  for ( unsigned i = 0; i < q_lb_attributes->count(); i++ )
  {
    if ( q_lb_attributes->isSelected( i ) )
      q_lb_attributes_norm->insertItem( q_lb_attributes->text( i ) );
    else
      remain_items.push_back( q_lb_attributes->text( i ).ascii() );
  }

  q_lb_attributes->clear();
  for ( unsigned i = 0; i < remain_items.size(); i++ )
    q_lb_attributes->insertItem( QString( remain_items[ i ].c_str() ) );

  q_lb_attributes->sort();
  q_lb_attributes_norm->sort();

  #ifdef _DEBUG
  debug.finish( "q_tb_select_attribute_norm_clicked()" );
  #endif
}

void geodma_window::q_tb_select_attribute_normalize_all_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_tb_select_attribute_normalize_all_clicked()" );
  #endif
  
  q_lb_attributes->selectAll(true);
  q_tb_select_attribute_norm_clicked();
  
  #ifdef _DEBUG
  debug.finish( "q_tb_select_attribute_normalize_all_clicked()" );
  #endif
}

void geodma_window::q_tb_unselect_attribute_norm_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_tb_unselect_attribute_norm_clicked()" );
  #endif

  vector<string> remain_items;
  for ( unsigned i = 0; i < q_lb_attributes_norm->count(); i++ )
  {
    if ( q_lb_attributes_norm->isSelected( i ) )
      q_lb_attributes->insertItem( q_lb_attributes_norm->text( i ) );
    else
      remain_items.push_back( q_lb_attributes_norm->text( i ).ascii() );
  }

  q_lb_attributes_norm->clear();
  for ( unsigned i = 0; i < remain_items.size(); i++ )
    q_lb_attributes_norm->insertItem( QString( remain_items[ i ].c_str() ) );

  q_lb_attributes->sort();
  q_lb_attributes_norm->sort();

  #ifdef _DEBUG
  debug.finish( "q_tb_unselect_attribute_norm_clicked()" );
  #endif
}

void geodma_window::q_tb_select_attribute_extract_all_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_tb_select_attribute_extract_all_clicked()" );
  #endif
  
  q_lb_attributes_available->selectAll(true);
  q_tb_select_attribute_extract_clicked();
  
  #ifdef _DEBUG
  debug.finish( "q_tb_select_attribute_extract_all_clicked()" );
  #endif
}

void geodma_window::q_tb_select_attribute_extract_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_tb_select_attribute_extract_clicked()" );
  #endif

  vector<string> remain_items;
  for ( unsigned i = 0; i < q_lb_attributes_available->count(); i++ )
  {
    if ( q_lb_attributes_available->isSelected( i ) )
      q_lb_attributes_extract->insertItem( q_lb_attributes_available->text( i ) );
    else
      remain_items.push_back( q_lb_attributes_available->text( i ).ascii() );
  }

  q_lb_attributes_available->clear();
  for ( unsigned i = 0; i < remain_items.size(); i++ )
    q_lb_attributes_available->insertItem( QString( remain_items[ i ].c_str() ) );

  q_lb_attributes_available->sort();
  q_lb_attributes_extract->sort();

  #ifdef _DEBUG
  debug.finish( "q_tb_select_attribute_extract_clicked()" );
  #endif
}

void geodma_window::q_tb_unselect_attribute_extract_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_tb_unselect_attribute_extract_clicked()" );
  #endif

  vector<string> remain_items;
  for ( unsigned i = 0; i < q_lb_attributes_extract->count(); i++ )
  {
    if ( q_lb_attributes_extract->isSelected( i ) )
      q_lb_attributes_available->insertItem( q_lb_attributes_extract->text( i ) );
    else
      remain_items.push_back( q_lb_attributes_extract->text( i ).ascii() );
  }

  q_lb_attributes_extract->clear();
  for ( unsigned i = 0; i < remain_items.size(); i++ )
    q_lb_attributes_extract->insertItem( QString( remain_items[ i ].c_str() ) );

  q_lb_attributes_available->sort();
  q_lb_attributes_extract->sort();

  #ifdef _DEBUG
  debug.finish( "q_tb_unselect_attribute_extract_clicked()" );
  #endif
}

void geodma_window::q_tb_select_attribute_classify_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_tb_select_attribute_classify_clicked()" );
  #endif

  vector<string> remain_items;
  for ( unsigned i = 0; i < q_lb_attributes_candidate->count(); i++ )
  {
    if ( q_lb_attributes_candidate->isSelected( i ) )
      q_lb_attributes_classify->insertItem( q_lb_attributes_candidate->text( i ) );
    else
      remain_items.push_back( q_lb_attributes_candidate->text( i ).ascii() );
  }

  q_lb_attributes_candidate->clear();
  for ( unsigned i = 0; i < remain_items.size(); i++ )
    q_lb_attributes_candidate->insertItem( QString( remain_items[ i ].c_str() ) );

  q_lb_attributes_candidate->sort();
  q_lb_attributes_classify->sort();

  #ifdef _DEBUG
  debug.finish( "q_tb_select_attribute_classify_clicked()" );
  #endif
}

void geodma_window::q_tb_select_attribute_classify_all_released()
{
  #ifdef _DEBUG
  debug.start( "q_tb_select_attribute_classify_all_released()" );
  #endif
  
  q_lb_attributes_candidate->selectAll(true);
  q_tb_select_attribute_classify_clicked();
  
  #ifdef _DEBUG
  debug.finish( "q_tb_select_attribute_classify_all_released()" );
  #endif
}

void geodma_window::q_tb_unselect_attribute_classify_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_tb_unselect_attribute_classify_clicked()" );
  #endif

  vector<string> remain_items;
  for ( unsigned i = 0; i < q_lb_attributes_classify->count(); i++ )
  {
    if ( q_lb_attributes_classify->isSelected( i ) )
      q_lb_attributes_candidate->insertItem( q_lb_attributes_classify->text( i ) );
    else
      remain_items.push_back( q_lb_attributes_classify->text( i ).ascii() );
  }
  q_lb_attributes_classify->clear();
  for ( unsigned i = 0; i < remain_items.size(); i++ )
    q_lb_attributes_classify->insertItem( QString( remain_items[ i ].c_str() ) );

  q_lb_attributes_candidate->sort();
  q_lb_attributes_classify->sort();

  #ifdef _DEBUG
  debug.finish( "q_tb_unselect_attribute_classify_clicked()" );
  #endif
}

/* ========== Adjusting Inputs ========== */
void geodma_window::unlock_grid()
{
  #ifdef _DEBUG
  debug.start( "unlock_grid()" );
  #endif

  if ( plug_params_ptr_->teqtgrid_ptr_->getTheme() )
    if ( plug_params_ptr_->teqtgrid_ptr_->getTheme()->getTheme() )
      plug_params_ptr_->teqtgrid_ptr_->clear();

//  plug_params_ptr_->teqtgrid_ptr_->clear();
/*  
  if ( plug_params_ptr_->teqtgrid_ptr_->getTheme() ) 
    if ( plug_params_ptr_->teqtgrid_ptr_->getTheme()->getTheme() ) 
    {
      plug_params_ptr_->teqtgrid_ptr_->close();
//      plug_params_ptr_->teqtgrid_ptr_->clearPortal();
//      plug_params_ptr_->teqtgrid_ptr_->clear();
    }
*/

  #ifdef _DEBUG
  debug.finish( "unlock_grid()" );
  #endif
}

void geodma_window::q_tab_selected( const QString & )
{
  #ifdef _DEBUG
  debug.start( "q_tab_selected()" );
  #endif

  if ( !q_tab->currentPage()->isEnabled() )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Please, select the inputs, then click OK" ) );
    q_tab->setCurrentPage( get_tab_number( "Input" ) );
  }

  if ( q_tab->currentPageIndex() == get_tab_number( "Multitemporal (MT)" ) )
    connect( plug_params_ptr_->teqtcanvas_ptr_, SIGNAL( mousePressed( TeCoord2D&, int, QPoint& ) ), this, SLOT( getMousePressed( TeCoord2D&, int, QPoint& ) ) );
  else
  {
    if ( neet_to_update_tv )
      update_TV_interface();
    disconnect( plug_params_ptr_->teqtcanvas_ptr_, SIGNAL( mousePressed( TeCoord2D&, int, QPoint& ) ), this, SLOT( getMousePressed( TeCoord2D&, int, QPoint& ) ) );
  }

  #ifdef _DEBUG
  debug.finish( "q_tab_selected()" );
  #endif
}

void geodma_window::q_pb_reset_geodma_clicked()
{
  update_first_window();
}

void geodma_window::clear_inputs()
{
  #ifdef _DEBUG
  debug.start( "clear_inputs()" );
  #endif

  /* Cleaning all form data */

  /* Segmentation */
  q_cb_available_raster_layers_segmentation->clear();
  q_cb_segmentation_algorithm_highlighted( 0 );
  q_cb_segmentation_algorithm->setCurrentItem( 0 );

  /* Inputs */
  q_pb_reset_geodma->setEnabled( true );
  if ( !using_polygons && !using_cells && !using_points && !using_rasters )
  {
    q_cb_polygon_layers->clear();
    q_cb_polygon_tables->clear();

    q_cb_cell_layers->clear();
    q_cb_cell_tables->clear();

    q_cb_point_layers->clear();
    q_cb_point_tables->clear();

    q_cb_raster_layers->clear();
  }
  else
  {
    q_cb_polygon_layers->setEnabled( false );
    q_cb_polygon_tables->setEnabled( false );
    if ( using_polygons )
    {
      q_cb_polygon_layers->setCurrentText( get_polygons_layer()->name().c_str() );
      q_cb_polygon_tables->setCurrentText( polygons_attribute_table_name.c_str() );
    }

    q_cb_cell_layers->setEnabled( false );
    q_cb_cell_tables->setEnabled( false );
    if ( using_cells )
    {
      q_cb_cell_layers->setCurrentText( get_cells_layer()->name().c_str() );
      q_cb_cell_tables->setCurrentText( cells_attribute_table_name.c_str() );
    }

    q_cb_point_layers->setEnabled( false );
    q_cb_point_tables->setEnabled( false );
    q_gb_mt_parameters->setEnabled( false );
    if ( using_points )
    {
      q_cb_point_layers->setCurrentText( get_points_layer()->name().c_str() );
      q_cb_point_tables->setCurrentText( points_attribute_table_name.c_str() );
    }

    q_cb_raster_layers->setEnabled( false );
    if ( using_rasters )
    {
      q_cb_raster_layers->setCurrentText( get_rasters_layer()->name().c_str() );
    }
    q_pb_select_layers->setEnabled( false );
  }

  /* Extraction */
  q_lb_attributes_available->clear();
  q_lb_attributes_extract->clear();

  /* Normalization */
  q_lb_attributes->clear();
  q_lb_attributes_norm->clear();

  /* Training */
  q_le_new_class_name->clear();
  q_cb_new_class_color->clear();
  q_lb_training_classes->clear();

  /* Visualization */
  q_cb_yaxis->clear();
  q_cb_xaxis->clear();
  q_cb_labels->clear();
  clear_frames();

  /* Classification */
  q_lb_attributes_candidate->clear();
  q_lb_attributes_classify->clear();
  q_le_new_view->clear();
  q_cb_training_labels->clear();
  q_lb_ancillary_training_sets->clear();

  /* Validation */
  q_cb_reference_column->clear();
  q_cb_classification_column->clear();
  q_te_confusion_matrix->clear();
  q_cb_monte_carlo_labels->clear();
  q_te_monte_carlo_output->clear();
  q_lb_monte_carlo_attributes->clear();

  #ifdef _DEBUG
  debug.finish( "clear_inputs()" );
  #endif
}

QString geodma_window::function_name( QString name )
{
  #ifdef _DEBUG
  return name.section( "::", 1, 1);
  #endif

  return QString( "GeoDMA " ) + QString( VERSION );
}

void geodma_window::update_TV_interface()
{
  #ifdef _DEBUG
  debug.start( "update_TV_interface()" );
  #endif

  plug_params_ptr_->updateTVInterface();

  polygons_layers.clear();
  cells_layers.clear();
  points_layers.clear();
  rasters_layers.clear();

  load_input_data();

  neet_to_update_tv = false;

  #ifdef _DEBUG
  debug.finish( "update_TV_interface()" );
  #endif
}

bool geodma_window::load_input_data()
{
  #ifdef _DEBUG
  debug.start( "load_input_data()" );
  #endif

  TePDIPIManager getdata_status( tr( "Loading input data..." ).ascii(), 4, true );

  /* Getting PolygonSet */
  TeLayer* polygons_layer = get_polygons_layer();
  myPolygons = TePolygonSet();
  if ( using_polygons )
    if ( !polygons_layer->getPolygons( myPolygons ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to load Polygons" ) );
      return false;
    }

  getdata_status.Increment();

  /* Getting CellSet */
  TeLayer* cells_layer = get_cells_layer();
  myCells = TeCellSet();
  if ( using_cells )
    if ( !cells_layer->getCells( myCells ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to load Cells" ) );
      return false;
    }

  getdata_status.Increment();

  /* Getting PointSet */
  TeLayer* points_layer = get_points_layer();
  myPoints = TePointSet();
  if ( using_points )
    if ( !points_layer->getPoints( myPoints ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to load Points" ) );
      return false;
    }

  getdata_status.Increment();

  /* Getting Raster */
  TeLayer* rasters_layer = get_rasters_layer();
  myRaster = rasters_layer->raster();

  getdata_status.Toggle(false);

  #ifdef _DEBUG
  debug.finish( "load_input_data()" );
  #endif

  return true;
}

/* ========== Retrieving Layers ========== */
vector<string> geodma_window::get_all_polygons_layers()
{
  #ifdef _DEBUG
  debug.start( "get_all_polygons_layers()" );
  #endif

  vector<string> polygons_and_tables;
  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  vector<TeLayer*> all_polygons_layers;
  TeLayerMap::iterator lm_it = curr_database->layerMap().begin();
  while ( lm_it != curr_database->layerMap().end() )
  {
    if ( lm_it->second->hasGeometry( TePOLYGONS ) )
      all_polygons_layers.push_back( lm_it->second );
    ++lm_it;
  }

  q_lb_ancillary_training_sets->clear();
  for ( unsigned i = 0; i < all_polygons_layers.size(); i++ )
  {
    TeAttrTableVector att_tables;
    if ( !all_polygons_layers[ i ]->getAttrTables( att_tables, TeAttrStatic ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to get attribute tables from polygons layer" ) );
      return polygons_and_tables;
    }

    TeAttrTableVector::iterator att_tables_it = att_tables.begin();
    while ( att_tables_it != att_tables.end() )
    {
      if ( !( q_cb_polygon_layers->currentText().ascii() == all_polygons_layers[ i ]->name() &&
              q_cb_polygon_tables->currentText().ascii() == att_tables_it->name() ) )
        polygons_and_tables.push_back( all_polygons_layers[ i ]->name() + "->" + att_tables_it->name() );
      ++att_tables_it;
    }
  }

  #ifdef _DEBUG
  debug.finish( "get_all_polygons_layers()" );
  #endif

  return polygons_and_tables;
}

vector<string> geodma_window::get_all_cells_layers()
{
  #ifdef _DEBUG
  debug.start( "get_all_cells_layers()" );
  #endif

  vector<string> cells_and_tables;
  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  vector<TeLayer*> all_cells_layers;
  TeLayerMap::iterator lm_it = curr_database->layerMap().begin();
  while ( lm_it != curr_database->layerMap().end() )
  {
    if ( lm_it->second->hasGeometry( TeCELLS ) )
      all_cells_layers.push_back( lm_it->second );
    ++lm_it;
  }

  q_lb_ancillary_training_sets->clear();
  for ( unsigned i = 0; i < all_cells_layers.size(); i++ )
  {
    TeAttrTableVector att_tables;
    if ( !all_cells_layers[ i ]->getAttrTables( att_tables, TeAttrStatic ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to get attribute tables from cells layer" ) );
      return cells_and_tables;
    }

    TeAttrTableVector::iterator att_tables_it = att_tables.begin();
    while ( att_tables_it != att_tables.end() )
    {
      if ( !( q_cb_cell_layers->currentText().ascii() == all_cells_layers[ i ]->name() &&
              q_cb_cell_tables->currentText().ascii() == att_tables_it->name() ) )
        cells_and_tables.push_back( all_cells_layers[ i ]->name() + "->" + att_tables_it->name() );
      ++att_tables_it;
    }
  }

  #ifdef _DEBUG
  debug.finish( "get_all_cells_layers()" );
  #endif

  return cells_and_tables;
}

vector<string> geodma_window::get_all_points_layers()
{
  #ifdef _DEBUG
  debug.start( "get_all_points_layers()" );
  #endif

  vector<string> points_and_tables;
  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  vector<TeLayer*> all_points_layers;
  TeLayerMap::iterator lm_it = curr_database->layerMap().begin();
  while ( lm_it != curr_database->layerMap().end() ) 
  {
    if ( lm_it->second->hasGeometry( TePOINTS ) )
      all_points_layers.push_back( lm_it->second );
    ++lm_it;
  }

  q_lb_ancillary_training_sets->clear();
  for ( unsigned i = 0; i < all_points_layers.size(); i++ )
  {
    TeAttrTableVector att_tables;
    if ( !all_points_layers[ i ]->getAttrTables( att_tables, TeAttrStatic ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to get attribute tables from points layer" ) );
      return points_and_tables;
    }

    TeAttrTableVector::iterator att_tables_it = att_tables.begin();
    while ( att_tables_it != att_tables.end() ) 
    {
      if ( !( q_cb_point_layers->currentText().ascii() == all_points_layers[ i ]->name() &&
              q_cb_point_tables->currentText().ascii() == att_tables_it->name() ) )
        points_and_tables.push_back( all_points_layers[ i ]->name() + "->" + att_tables_it->name() );
      ++att_tables_it;
    }
  }

  #ifdef _DEBUG
  debug.finish( "get_all_points_layers()" );
  #endif

  return points_and_tables;
}

TeLayer* geodma_window::get_polygons_layer()
{
  #ifdef _DEBUG
  debug.start( "get_polygons_layer()" );
  #endif

  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();

  if ( polygons_layers.size() == 0 && curr_database != 0 )
  {
    TeLayerMap::iterator lm_it = curr_database->layerMap().begin();

    while ( lm_it != curr_database->layerMap().end() )
    {
      if ( lm_it->second->hasGeometry( TePOLYGONS ) )
        polygons_layers.push_back( lm_it->second );
      ++lm_it;
    }
  }

  TeLayer* rlayer;
  if ( !using_polygons || polygons_layers.size() == 0 || polygons_layer_index >= (int) polygons_layers.size() || polygons_layer_index == -1 )
    rlayer = new TeLayer();
  else
    rlayer = polygons_layers[ polygons_layer_index ];

  #ifdef _DEBUG
  debug.finish( "get_polygons_layer()" );
  #endif

  return rlayer;
}

TeLayer* geodma_window::get_cells_layer()
{
  #ifdef _DEBUG
  debug.start( "get_cells_layer()" );
  #endif

  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();

  if ( cells_layers.size() == 0 && curr_database != 0 )
  {
    TeLayerMap::iterator lm_it = curr_database->layerMap().begin();
    while ( lm_it != curr_database->layerMap().end() )
    {
      if ( lm_it->second->hasGeometry( TeCELLS ) )
        cells_layers.push_back( lm_it->second );
      ++lm_it;
    }
  }

  TeLayer* rlayer;
  if ( !using_cells || cells_layers.size() == 0 || cells_layer_index >= (int) cells_layers.size() || cells_layer_index == -1 )
    rlayer = new TeLayer();
  else
    rlayer = cells_layers[ cells_layer_index ];

  #ifdef _DEBUG
  debug.finish( "get_cells_layer()" );
  #endif

  return rlayer;
}

TeLayer* geodma_window::get_points_layer()
{
  #ifdef _DEBUG
  debug.start( "get_points_layer()" );
  #endif

  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();

  if ( points_layers.size() == 0 && curr_database != 0 )
  {
    TeLayerMap::iterator lm_it = curr_database->layerMap().begin();
    while ( lm_it != curr_database->layerMap().end() )
    {
      if ( lm_it->second->hasGeometry( TePOINTS ) )
        points_layers.push_back( lm_it->second );
      ++lm_it;
    }
  }

  TeLayer* rlayer;
  if ( !using_points || points_layers.size() == 0 || points_layer_index >= (int) points_layers.size() || points_layer_index == -1 )
    rlayer = new TeLayer();
  else
    rlayer = points_layers[ points_layer_index ];

  #ifdef _DEBUG
  debug.finish( "get_points_layer()" );
  #endif

  return rlayer;
}

TeLayer* geodma_window::get_rasters_layer()
{
  #ifdef _DEBUG
  debug.start( "get_rasters_layer()" );
  #endif

  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();

  /* Check if rasters_layers was already created */
  if ( rasters_layers.size() == 0 && curr_database != 0 )
  {
    TeLayerMap::iterator lm_it = curr_database->layerMap().begin();

    while ( lm_it != curr_database->layerMap().end() )
    {
      if ( lm_it->second->hasGeometry( TeRASTER ) || lm_it->second->hasGeometry( TeRASTERFILE ) )
        rasters_layers.push_back( lm_it->second );

      ++lm_it;
    }
  }

  TeLayer* rlayer;

  if ( !using_rasters || rasters_layers.size() == 0 || rasters_layer_index >= (int) rasters_layers.size() || rasters_layer_index == -1 )
    rlayer = new TeLayer();
  else
    rlayer = rasters_layers[ rasters_layer_index ];

  #ifdef _DEBUG
  debug.finish( "get_rasters_layer()" );
  #endif

  return rlayer;
}

TeLayer* geodma_window::get_layer( string layer_name )
{
  #ifdef _DEBUG
  debug.start( "get_layer( " + layer_name + " )" );
  #endif

  TeLayer *return_layer = new TeLayer();
  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  TeLayerMap::iterator lm_it = curr_database->layerMap().begin();
  while ( lm_it != curr_database->layerMap().end() )
  {
    if ( lm_it->second->name().compare( layer_name ) == 0 )
    {
      delete return_layer;
      return_layer = lm_it->second;
      break;
    }

    ++lm_it;
  }

  #ifdef _DEBUG
  debug.finish( "get_layer()" );
  #endif

  return return_layer;
}

void geodma_window::refresh_tables()
{
  #ifdef _DEBUG
  debug.start( "refresh_tables()" );
  #endif

  clear_inputs();

  q_tab->page( get_tab_number( "Segmentation" ) )->setEnabled( false );
  for ( int i = 1; i < q_tab->count(); i++ )
    q_tab->page( i )->setEnabled( true );
  q_cb_labels->insertItem( "No Labels" );
  q_cb_labels->setCurrentText( "No Labels" );
  q_cb_training_labels->insertItem( "No Labels" );
  q_cb_training_labels->setCurrentText( "No Labels" );

  /* Getting available name for the next classification view */
  TeQtViewsListView* views = plug_params_ptr_->teqtviewslistview_ptr_;
  vector<TeQtViewItem*> view_items = views->getViewItemVec();
  unsigned classification_views = 0;
  for ( unsigned i = 0; i < view_items.size(); i++ )
  {
    if ( view_items[ i ]->getView()->name().find( "Classification", 0 ) != string::npos )
      classification_views++;
  }
  string name = "Classification_" + Te2String( classification_views + 1 );
  q_le_new_view->insert( QString( name.c_str() ) );

  /* Getting available features to extract */
  vector<string> available_attributes;
  int nbands = 0;
  if ( using_rasters )
    nbands = myRaster->nBands();

  if ( using_polygons )
  {
    q_cb_visualizing_what_highlighted( 0 );
    q_cb_classifying_what_highlighted( 0 );
    q_cb_validating_what_highlighted( 0 );
    get_polygon_feature_names( available_attributes, nbands );
  }
  if ( using_cells )
  {
    if ( using_polygons )
      get_cell_feature_names( available_attributes, nbands );
    else
    {
      q_cb_visualizing_what_highlighted( 1 );
      q_cb_classifying_what_highlighted( 1 );
      q_cb_validating_what_highlighted( 1 );
    }
  }
  if ( using_points )
    get_point_feature_names( available_attributes );

  for ( unsigned i = 0; i < available_attributes.size(); i++ )
    q_lb_attributes_available->insertItem( QString( available_attributes[ i ].c_str() ) );

  /* Getting available features for normalization */
  string attribute_table_name;
  TeLayer* layer_choice;
  TeTable table_choice;
  TeAttributeList::iterator attr_list_it;
    
  /* Polygons Attributes */
  if ( using_polygons )
  {
    layer_choice = get_polygons_layer();
    attribute_table_name = polygons_attribute_table_name;

    if ( !layer_choice->getAttrTablesByName( attribute_table_name, table_choice, TeAllAttrTypes ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) );
      return;
    }
    TeAttributeList& attr_list = table_choice.attributeList();
    attr_list_it = attr_list.begin();
    while ( attr_list_it != attr_list.end() ) 
    {
      if ( attr_list_it->rep_.type_ == TeREAL && QString( attr_list_it->rep_.name_.c_str() ).right( 2 ).compare( "_n" ) != 0 )
        q_lb_attributes->insertItem( attr_list_it->rep_.name_.c_str() );

      ++attr_list_it;
    }
  }

  /* Cells Attributes */
  if ( using_cells )
  {
    layer_choice = get_cells_layer();
    attribute_table_name = cells_attribute_table_name;

    if ( !layer_choice->getAttrTablesByName( attribute_table_name, table_choice, TeAllAttrTypes ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) );
      return;
    }
    TeAttributeList& attr_list = table_choice.attributeList();
    attr_list_it = attr_list.begin();
    while ( attr_list_it != attr_list.end() ) 
    {
      if ( attr_list_it->rep_.type_ == TeREAL && QString( attr_list_it->rep_.name_.c_str() ).right( 2 ).compare( "_n" ) != 0 )
        q_lb_attributes->insertItem( attr_list_it->rep_.name_.c_str() );

      ++attr_list_it;
    }
  }

  q_lb_attributes_norm->sort();

  /* Training Tab */
  geodma_colors C;
  for ( unsigned i = 0; i < C.get_size(); i++ )
  {
    QPixmap item( 15, 15 );
    QColor color( C.get_R( i ), C.get_G( i ), C.get_B( i ) );
    item.fill( color );
    string name = "Color_" + Te2String( i );
    q_cb_new_class_color->insertItem( item, QString( name.c_str() ) );
  }

  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  TeTable training_table;
  if ( !curr_database->loadTable( "training_table", training_table ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not load the theme attribute table:\n" ) + curr_database->errorMessage().c_str() );
    return;
  }

  for ( unsigned i = 0; i < training_table.size(); i++ )
  {
    string class_name = training_table( i, 1 );
    int R = atoi( training_table( i, 2 ).c_str() ),
        G = atoi( training_table( i, 3 ).c_str() ),
        B = atoi( training_table( i, 4 ).c_str() );
    QPixmap item( 15, 15 );
    QColor color( R, G, B );
    item.fill( color );
    if ( training_table( i, 5 ) == "1" )
      q_lb_training_classes->insertItem( item, QString( class_name.c_str() ) );
  }

  /* Updating Classification Algorithms*/
  loaded_c45 = "";
  q_le_radius->setText( "+0.450" );
  q_le_decreasing->setText( "+0.750" );
  q_le_alpha->setText( "+0.650" );
  q_tb_classifiers->setCurrentIndex( 1 );
  q_le_learning_rate->setText( "+0.0750" );

  /* Ordering input fields */
  q_lb_attributes->sort();
  q_lb_attributes_norm->sort();
  q_lb_attributes_available->sort();
  q_lb_attributes_extract->sort();
  q_lb_attributes_candidate->sort();
  q_lb_attributes_classify->sort();
  q_lb_training_classes->sort();
  q_lb_monte_carlo_attributes->sort();

  #ifdef _DEBUG
  debug.finish( "refresh_tables()" );
  #endif
}

/* ========== Ancillary functions ========== */
bool geodma_window::execute_query( string query )
{
  #ifdef _DEBUG
  debug.start( "execute_query( " + query + " )" );
  #endif

  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  curr_database->beginTransaction();
  if ( !curr_database->execute( query ) )
  {
    QString message;
    message.sprintf( "Could not execute query\n %s\n", query.c_str() );
    QMessageBox::critical( this, function_name( __FUNCTION__ ), message + curr_database->errorMessage().c_str() ); 
    curr_database->rollbackTransaction();
    return false;
  }
  curr_database->commitTransaction();

  #ifdef _DEBUG
  debug.finish( "execute_query()" );
  #endif

  return true;
}

int geodma_window::find_index( string name, TeLayer *layer, string table_name )
{
  #ifdef _DEBUG
  debug.start( "find_index( " + name  + ", " + table_name + " )" );
  #endif

  TeTable objects_table;
  /* Find attribute index */
  if ( !layer->getAttrTablesByName( table_name, objects_table, TeAllAttrTypes ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) );
    return -1;
  }

  TeAttributeList& objects_attr_list = objects_table.attributeList();
  TeAttributeList::iterator objects_attr_list_it = objects_attr_list.begin();
  int curr_attr_index = 0,
      index_return = -1;

  while ( objects_attr_list_it != objects_attr_list.end() ) 
  {
    if ( objects_attr_list_it->rep_.name_.compare( name ) == 0 )
    {
      index_return = curr_attr_index;
      break;
    }

    ++curr_attr_index; 
    ++objects_attr_list_it; 
  }

  #ifdef _DEBUG
  debug.finish( "find_index( " + Te2String( index_return ) + " )" );
  #endif

  return index_return;
}

list<string> geodma_window::get_columns_names( TeLayer *layer, string attribute_table_name,
                                               TeTable table, TeAttrDataType restriction_type )
{
  #ifdef _DEBUG
  debug.start( "get_columns_names( " + attribute_table_name + " )" );
  #endif

  list<string> list_return;

  if ( !layer->getAttrTablesByName( attribute_table_name, table, TeAllAttrTypes ) ) 
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) );
    return list_return;
  }

  TeAttributeList& attr_list = table.attributeList();
  TeAttributeList::iterator attr_list_it = attr_list.begin();
  while ( attr_list_it != attr_list.end() )
  {
    if ( attr_list_it->rep_.type_ == restriction_type )
      list_return.push_back( attr_list_it->rep_.name_ );

    ++attr_list_it;
  }

  #ifdef _DEBUG
  debug.finish( "get_columns_names( " + Te2String( list_return.size() ) + " elements )" );
  #endif

  return list_return;
}

double geodma_window::get_kappa( TeMatrix confusion_matrix )
{
  #ifdef _DEBUG
  debug.start( "get_kappa()" );
  #endif

  TeMatrix sum_cols;
  TeMatrix sum_lines;
  sum_cols.Init( 1, confusion_matrix.Ncol(), 0.0 );
  sum_lines.Init( 1, confusion_matrix.Nrow(), 0.0 );

  int total = 0;
  double pra = 0.0;
  double pre = 0.0;
  for ( int i = 0; i < confusion_matrix.Nrow(); i++ )
  {
    pra += confusion_matrix( i, i );
    for ( int j = 0; j < confusion_matrix.Ncol(); j++ )
    {
      sum_cols( 0, j ) = sum_cols( 0, j ) + confusion_matrix( i, j );
      sum_lines( 0, i ) = sum_lines( 0, i ) + confusion_matrix( i, j );
      total += confusion_matrix( i, j );
    }
  }
  if ( total != 0 )
  {
    pra /= total;

    for ( int i = 0; i < confusion_matrix.Ncol(); i++ )
    {
      sum_cols( 0, i ) = sum_cols( 0, i ) / total;
      sum_lines( 0, i ) = sum_lines( 0, i ) / total;
      pre += sum_cols( 0, i ) * sum_lines( 0, i );
    }
  }
  else
  {
    pre = 0.0;
    pra = -1.0;
  }

  #ifdef _DEBUG
  debug.finish( "get_kappa()" );
  #endif

  return ( pre == 1.0 ) ? ( 1.0 ) : ( ( pra - pre ) / ( 1.0 - pre ) );
}

int get_tab_number(string tabname)
{
  if ( tabname.find( "Segmentation" ) != string::npos )
    return 0;

  if ( tabname.find( "Input" ) != string::npos )
    return 1;

  if ( tabname.find( "Features" ) != string::npos )
    return 2;

  if ( tabname.find( "Normalize" ) != string::npos )
    return 3;

  if ( tabname.find( "Train" ) != string::npos )
    return 4;

  if ( tabname.find( "Visualize" ) != string::npos )
    return 5;

  if ( tabname.find( "Classify" ) != string::npos )
    return 6;

  if ( tabname.find( "Validate" ) != string::npos )
    return 7;

  if ( tabname.find( "Multitemporal (MT)" ) != string::npos )
    return 8;

  if ( tabname.find( "About" ) != string::npos )
    return 9;

  return -1;
}

string create_update_query(string table_name, string unique_name, string object_id, vector<string> names_attributes, vector<double> values_attributes)
{
  string query = "UPDATE ";
  query += table_name;
  query += " SET ";

  query += names_attributes[ 0 ];
  query += " = '";
  query += Te2String( values_attributes[ 0 ] );
  query += "'";

  for (unsigned i = 1; i < names_attributes.size(); i++)
  {
    query += ", ";
    query += names_attributes[i];
    query += " = '";
    query += Te2String( values_attributes[ i ] );
    query += "'";
  }

  query += " WHERE ";
  query += unique_name;
  query += " = '";
  query += object_id;
  query += "'";

  return query;
}

/* ========== Starting REAL Implementation ========== */
void geodma_window::create_training_table()
{
  #ifdef _DEBUG
  debug.start( "create_training_table()" );
  #endif

  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  string training_table_name = "training_table";

  TeAttributeList training_attrs;
  TeAttribute class_id;
  class_id.rep_.name_ = "id";
  class_id.rep_.type_ = TeSTRING;
  class_id.rep_.numChar_ = 16;
  class_id.rep_.isPrimaryKey_ = true;
  training_attrs.push_back( class_id );

  if ( !curr_database->tableExist( training_table_name ) )
  {
    if ( !curr_database->createTable( training_table_name, training_attrs ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not create new table:\n" ) + curr_database->errorMessage().c_str() );
      return;
    }

    TeAttribute class_name;
    class_name.rep_.name_ = "training_class";
    class_name.rep_.type_ = TeSTRING;
    class_name.rep_.null_ = true;
    class_name.rep_.numChar_ = 50;
    training_attrs.push_back( class_name );
    if ( !curr_database->addColumn( training_table_name, class_name.rep_ ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could add column 'training_class' in training_table:\n" ) + curr_database->errorMessage().c_str() );
      return;
    }
    TeAttribute class_R;
    class_R.rep_.name_ = "class_r";
    class_R.rep_.type_ = TeINT;
    class_R.rep_.null_ = true;
    class_R.minValue_ = "0";
    class_R.maxValue_ = "255";
    training_attrs.push_back( class_R );
    if ( !curr_database->addColumn( training_table_name, class_R.rep_ ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could add column 'class_r' in training_table:\n" ) + curr_database->errorMessage().c_str() );
      return;
    }

    TeAttribute class_G;
    class_G.rep_.name_ = "class_g";
    class_G.rep_.type_ = TeINT;
    class_G.rep_.null_ = true;
    class_G.minValue_ = "0";
    class_G.maxValue_ = "255";
    training_attrs.push_back( class_G );
    if ( !curr_database->addColumn( training_table_name, class_G.rep_ ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could add column 'class_g' in training_table:\n" ) + curr_database->errorMessage().c_str() );
      return;
    }

    TeAttribute class_B;
    class_B.rep_.name_ = "class_b";
    class_B.rep_.type_ = TeINT;
    class_B.rep_.null_ = true;
    class_B.minValue_ = "0";
    class_B.maxValue_ = "255";
    training_attrs.push_back( class_B );
    if ( !curr_database->addColumn( training_table_name, class_B.rep_ ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could add column 'class_b' in training_table:\n" ) + curr_database->errorMessage().c_str() );
      return;
    }

    TeAttribute is_active;
    is_active.rep_.name_ = "is_active";
    is_active.rep_.type_ = TeSTRING;
    is_active.rep_.numChar_ = 2;
    training_attrs.push_back( is_active );
    if ( !curr_database->addColumn( training_table_name, is_active.rep_ ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could add column 'is_active' in training_table:\n" ) + curr_database->errorMessage().c_str() );
      return;
    }

    TeTable table( training_table_name, training_attrs, "" );
    TeTableRow row1;
    row1.push_back( "1" );
    row1.push_back( "class_name" );
    row1.push_back( "0" );
    row1.push_back( "0" );
    row1.push_back( "0" );
    row1.push_back( "0" );
    table.add( row1 );
    if ( !curr_database->insertTable( table ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could add first row in training_table:\n" ) + curr_database->errorMessage().c_str() );
      return;
    }
  }

  /* Inserting columns training_class/validation_class into polygons table */
  if ( using_polygons )
  {
    {
      TeAttribute class_name;
      class_name.rep_.name_ = "training_class";
      class_name.rep_.type_ = TeSTRING;
      class_name.rep_.null_ = true;
      class_name.rep_.numChar_ = 50;

      if ( !curr_database->columnExist( polygons_attribute_table_name, class_name.rep_.name_, class_name ) )
        if ( !curr_database->addColumn( polygons_attribute_table_name, class_name.rep_ ) )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not create polygons training_class column in polygons table:\n" ) + curr_database->errorMessage().c_str() );
          return;
        }
    }
    {
      TeAttribute class_name;
      class_name.rep_.name_ = "validation_class";
      class_name.rep_.type_ = TeSTRING;
      class_name.rep_.null_ = true;
      class_name.rep_.numChar_ = 50;

      if ( !curr_database->columnExist( polygons_attribute_table_name, class_name.rep_.name_, class_name ) )
        if ( !curr_database->addColumn( polygons_attribute_table_name, class_name.rep_ ) )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not create polygons validation_class column in polygons table:\n" ) + curr_database->errorMessage().c_str() );
          return;
        }
    }
  }

  /* Inserting new column training_class into cells table */
  if ( using_cells )
  {
    {
      TeAttribute class_name;
      class_name.rep_.name_ = "training_class";
      class_name.rep_.type_ = TeSTRING;
      class_name.rep_.null_ = true;
      class_name.rep_.numChar_ = 50;

      if ( !curr_database->columnExist( cells_attribute_table_name, class_name.rep_.name_, class_name ) )
        if ( !curr_database->addColumn( cells_attribute_table_name, class_name.rep_ ) )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not create cells training_class column in cells table:\n" ) + curr_database->errorMessage().c_str() );
          return;
        }
    }
    {
      TeAttribute class_name;
      class_name.rep_.name_ = "validation_class";
      class_name.rep_.type_ = TeSTRING;
      class_name.rep_.null_ = true;
      class_name.rep_.numChar_ = 50;

      if ( !curr_database->columnExist( cells_attribute_table_name, class_name.rep_.name_, class_name ) )
        if ( !curr_database->addColumn( cells_attribute_table_name, class_name.rep_ ) )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not create cells validation_class column in cells table:\n" ) + curr_database->errorMessage().c_str() );
          return;
        }
    }
  }

  /* Inserting new column training_class into points table */
  if ( using_points )
  {
    for ( unsigned t = mt_firstLabel; t < ( mt_firstLabel + ( myRaster->nBands() - mt_startingPoint ) / mt_pointsPerCurve ); t += mt_stepLabels )
    {
      TeAttribute tclass_name;
      tclass_name.rep_.name_ = "training_class_" + Te2String( t );
      tclass_name.rep_.type_ = TeSTRING;
      tclass_name.rep_.null_ = true;
      tclass_name.rep_.numChar_ = 50;

      if ( !curr_database->columnExist( points_attribute_table_name, tclass_name.rep_.name_, tclass_name ) )
        if ( !curr_database->addColumn( points_attribute_table_name, tclass_name.rep_ ) )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not create points training_class column in points table:\n" ) + curr_database->errorMessage().c_str() );
          return;
        }

      TeAttribute vclass_name;
      vclass_name.rep_.name_ = "validation_class_" + Te2String( t );
      vclass_name.rep_.type_ = TeSTRING;
      vclass_name.rep_.null_ = true;
      vclass_name.rep_.numChar_ = 50;

      if ( !curr_database->columnExist( points_attribute_table_name, vclass_name.rep_.name_, vclass_name ) )
        if ( !curr_database->addColumn( points_attribute_table_name, vclass_name.rep_ ) )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not create points validation_class column in points table:\n" ) + curr_database->errorMessage().c_str() );
          return;
        }
    }
  }

  #ifdef _DEBUG
  debug.finish( "create_training_table()" );
  #endif
}

void geodma_window::q_pb_select_layers_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_pb_select_layers_clicked()" );
  #endif

  /* Checking if 'No Polygons' option was selected */
  using_polygons = true;
  if ( q_cb_polygon_layers->currentText() == "No Polygons" )
    using_polygons = false;

  /* Checking if 'No Cells' option was selected */
  using_cells = true;
  if ( q_cb_cell_layers->currentText() == "No Cells" )
    using_cells = false;

  /* Checking if 'No Points' option was selected */
  using_points = true;
  if ( q_cb_point_layers->currentText() == "No Points" )
    using_points = false;

  /* Checking if 'No Rasters' option was selected */
  using_rasters = true;
  if ( q_cb_raster_layers->currentText() == "No Rasters" )
    using_rasters = false;

  if ( !using_polygons && !using_cells && !using_points )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "You cannot select 'No Polygons', 'No Cells' and 'No Points'" ) );
    return;
  }

  if ( !load_input_data() )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to load input data" ) );
    return;
  }

  if ( using_rasters )
  {
    if ( !myRaster )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Raster is not active" ) );
      return;
    }

    if ( using_polygons )
    {
      TeBox rasterbbox = myRaster->params().boundingBox();
      TePolygon raster_bpol = polygonFromBox( rasterbbox );
      TeBox polygonsbbox = myPolygons.box();
      TePolygon polygons_bpol = polygonFromBox( polygonsbbox );

      if ( TeRelation( raster_bpol, polygons_bpol ) == TeDISJOINT )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Polygons and Raster doesn't fit" ) );
        return;
      }

      /* Reprojecting geometries, if necessary */
      if ( get_polygons_layer()->projection()->name() != get_rasters_layer()->projection()->name() )
      {
        TePolygonSet reprojected_polygons;
        TeVectorRemap( myPolygons, get_polygons_layer()->projection(), reprojected_polygons, get_rasters_layer()->projection() );
        myPolygons = reprojected_polygons;
      }
    }

    if ( using_cells )
    {
      TeBox rasterbbox = myRaster->params().boundingBox();
      TePolygon raster_bpol = polygonFromBox( rasterbbox );
      TeBox cellsbbox = myPolygons.box();
      TePolygon cells_bpol = polygonFromBox( cellsbbox );

      if ( TeRelation( raster_bpol, cells_bpol ) == TeDISJOINT )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Cells and Raster doesn't fit" ) );
        return;
      }

      /* Reprojecting geometries, if necessary */
      if ( get_cells_layer()->projection()->name() != get_rasters_layer()->projection()->name() )
      {
        TeCellSet reprojected_cells;
        TeVectorRemap( myCells, get_cells_layer()->projection(), reprojected_cells, get_rasters_layer()->projection() );
        myCells = reprojected_cells;
      }
    }
  }

  /* Verifying and adjusting projections */
  if ( using_polygons && using_cells )
  {
    TeBox polygonsbbox = myPolygons.box();
    TePolygon polygons_bpol = polygonFromBox( polygonsbbox );
    TeBox cellsbbox = myCells.box();
    TePolygon cells_bpol = polygonFromBox( cellsbbox );

    if ( TeRelation( polygons_bpol, cells_bpol ) == TeDISJOINT )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Polygons and Cells doesn't fit" ) );
      return;
    }
  }

  /* Getting polygons attribute table name */
  polygons_attribute_table_name = "";
  if ( using_polygons )
  {
    if ( !q_cb_polygon_tables->count() )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "First select the Layer with Polygons and its associated Table" ) );
      return;
    }
    polygons_attribute_table_name = q_cb_polygon_tables->currentText().ascii();
  }

  /* Getting cells attribute table name */
  cells_attribute_table_name = "";
  if ( using_cells )
  {
    if ( !q_cb_cell_tables->count() )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "First select the Layer with Cells and its associated Table" ) );
      return;
    }
    cells_attribute_table_name = q_cb_cell_tables->currentText().ascii();
  }

  /* Getting points attribute table name */
  points_attribute_table_name = "";
  if ( using_points )
  {
    if ( !q_cb_point_tables->count() )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "First select the Layer with Points and its associated Table" ) );
      return;
    }
    points_attribute_table_name = q_cb_point_tables->currentText().ascii();

    mt_startingPoint = (unsigned) q_sb_mt_startingpoint->text().toInt();
    mt_pointsPerCurve = (unsigned) q_sb_mt_pointspercurve->text().toInt();
    mt_firstLabel = (unsigned) q_sb_mt_firstlabel->text().toInt();
    mt_stepLabels = (unsigned) q_sb_mt_steplabel->text().toInt();

    q_sl_interval_mt->setMinValue( mt_firstLabel );
    q_sl_interval_mt->setMaxValue( mt_firstLabel + ( myRaster->nBands() - mt_startingPoint ) / mt_pointsPerCurve - mt_stepLabels );
    q_sl_interval_mt->setLineStep( mt_stepLabels );
    q_sl_interval_mt->setPageStep( mt_stepLabels );

    TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
    if ( !curr_database )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "First open database" ) ); 
      return;
    }

    /* Inserting mt_info into database table */
    TeTable mt_info_table;
    if ( !curr_database->loadTable( "mt_info_table", mt_info_table ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get mt_info_table" ) );
      return;
    }

    int valid_id = 0;
    for ( unsigned i = 0; i < mt_info_table.size(); i++ )
    {
      int id = atoi( mt_info_table( i, 0 ).c_str() );
      valid_id = ( id > valid_id ? id : valid_id );
    }
    ++valid_id;

    /* Inserting new class into database */
    string query = "INSERT INTO mt_info_table ( id, raster_layer_name, points_layer_name, starting_point, first_label, points_per_curve, step_labels ) VALUES ( ";
    query += Te2String( valid_id );
    query += ", '" + get_rasters_layer()->name() + "'";
    query += ", '" + get_points_layer()->name() + "'";
    query += ", " + Te2String( mt_startingPoint );
    query += ", " + Te2String( mt_firstLabel );
    query += ", " + Te2String( mt_pointsPerCurve );
    query += ", " + Te2String( mt_stepLabels );
    query += " );";

    if ( !execute_query( query ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not insert the mt_info into database:\n" ) + curr_database->errorMessage().c_str() );
      return;
    }
  }

  unlock_grid();

  /* Creating Training Table */
  create_training_table();

  /* Refresh all form tables */
  refresh_tables();
  q_tab->setCurrentPage( get_tab_number( "Features" ) );

  plug_params_ptr_->teqtgrid_ptr_->showGrid(); // ???

  #ifdef _DEBUG
  debug.finish( "q_pb_select_layers_clicked()" );
  #endif
}

void geodma_window::q_cb_polygon_layers_highlighted( int item )
{
  #ifdef _DEBUG
  debug.start( "q_cb_polygon_layers_highlighted( " + Te2String( item ) + " )" );
  #endif

  polygons_layer_index = item;
  TeLayer* polygons_layer = get_polygons_layer();
  q_cb_polygon_tables->clear();

  if ( q_cb_polygon_layers->text( item ) == "No Polygons" || polygons_layer->id() == -1 )
    q_cb_polygon_tables->setEnabled( false );
  else
  {
    q_cb_polygon_tables->setEnabled( true );

    TeAttrTableVector att_tables;
    if ( !polygons_layer->getAttrTables( att_tables, TeAttrStatic ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to get attribute tables from polygons layer" ) );
      return;
    }

    TeAttrTableVector::iterator att_tables_it = att_tables.begin();
    while ( att_tables_it != att_tables.end() )
    {
      q_cb_polygon_tables->insertItem( QString( att_tables_it->name().c_str() ) );
      ++att_tables_it;
    }
  }

  #ifdef _DEBUG
  debug.finish( "q_cb_polygon_layers_highlighted( " + Te2String( item ) + " )" );
  #endif
}

void geodma_window::q_cb_cell_layers_highlighted( int item )
{
  #ifdef _DEBUG
  debug.start( "q_cb_cell_layers_highlighted( " + Te2String( item ) + " )" );
  #endif

  cells_layer_index = item;
  TeLayer* cells_layer = get_cells_layer();

  q_cb_cell_tables->clear();
  if ( q_cb_cell_layers->text( item ) == "No Cells" || cells_layer->id() == -1 )
    q_cb_cell_tables->setEnabled( false );
  else
  {
    q_cb_cell_tables->setEnabled( true );

    TeAttrTableVector att_tables;
    if ( !cells_layer->getAttrTables( att_tables, TeAttrStatic ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to get attribute tables from cells layer" ) );
      return;
    }

    TeAttrTableVector::iterator att_tables_it = att_tables.begin();
    while ( att_tables_it != att_tables.end() ) 
    {
      q_cb_cell_tables->insertItem( QString( att_tables_it->name().c_str() ) );
      ++att_tables_it;
    }
  }

  #ifdef _DEBUG
  debug.finish( "q_cb_cell_layers_highlighted( " + Te2String( item ) + " )" );
  #endif
}

void geodma_window::q_cb_point_layers_highlighted( int item )
{
  #ifdef _DEBUG
  debug.start( "q_cb_point_layers_highlighted( " + Te2String( item ) + " )" );
  #endif

  points_layer_index = item;
  TeLayer* points_layer = get_points_layer();

  q_cb_point_tables->clear();
  if ( q_cb_point_layers->text( item ) == "No Points" || points_layer->id() == -1 )
    q_cb_point_tables->setEnabled( false );
  else
  {
    q_cb_point_tables->setEnabled( true );

    TeAttrTableVector att_tables;
    if ( !points_layer->getAttrTables( att_tables, TeAttrStatic ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to get attribute tables from points layer" ) );
      return;
    }

    TeAttrTableVector::iterator att_tables_it = att_tables.begin();
    while ( att_tables_it != att_tables.end() ) 
    {
      q_cb_point_tables->insertItem( QString( att_tables_it->name().c_str() ) );
      ++att_tables_it;
    }
  }

  #ifdef _DEBUG
  debug.finish( "q_cb_point_layers_highlighted( " + Te2String( item ) + " )" );
  #endif
}

void geodma_window::q_cb_raster_layers_highlighted( int item )
{
  #ifdef _DEBUG
  debug.start( "q_cb_raster_layers_highlighted( " + Te2String( item ) + " )" );
  #endif

  rasters_layer_index = item;

  #ifdef _DEBUG
  debug.finish( "q_cb_raster_layers_highlighted( " + Te2String( item ) + " )" );
  #endif
}

void geodma_window::q_cb_visualizing_what_highlighted( int item )
{
  #ifdef _DEBUG
  debug.start( "q_cb_visualizing_what_highlighted( " + Te2String( item ) + " )" );
  #endif

  if ( q_tab->page( get_tab_number( "Input" ) )->isEnabled() )
  {
    string attribute_table_name;
    TeLayer* layer_choice;
    TeTable table_choice;

    if ( q_cb_visualizing_what->text( item ) == "Polygons" )
    {
      if ( !using_polygons )
        return;
      layer_choice = get_polygons_layer();
      attribute_table_name = polygons_attribute_table_name;
    }
    else if ( q_cb_visualizing_what->text( item ) == "Cells" )
    {
      if ( !using_cells )
        return;
      layer_choice = get_cells_layer();
      attribute_table_name = cells_attribute_table_name;
    }
    else // Points
    {
      if ( !using_points )
        return;
      layer_choice = get_points_layer();
      attribute_table_name = points_attribute_table_name;
    }

    list<string> tereal_names = get_columns_names( layer_choice, attribute_table_name, table_choice, TeREAL );

    list<string> int_names = get_columns_names( layer_choice, attribute_table_name, table_choice, TeINT );
    list<string>::iterator names_it = int_names.begin();
    while ( names_it != int_names.end() )
    {
      tereal_names.push_back( *names_it );
      ++names_it;
    }

    if ( tereal_names.size() == 0 )
      return;
    tereal_names.sort();
    names_it = tereal_names.begin();
    q_cb_visualizing_what->setCurrentItem( item );
    q_cb_xaxis->clear();
    q_cb_yaxis->clear();
    while ( names_it != tereal_names.end() )
    {
      string name = *names_it;
      q_cb_xaxis->insertItem( QString( name.c_str() ) );
      q_cb_yaxis->insertItem( QString( name.c_str() ) );
      ++names_it;
    }

    /* Inserting unique label names */
    list<string> label_names = get_columns_names( layer_choice, attribute_table_name, table_choice, TeSTRING );
    if ( label_names.size() == 0 )
      return;

    label_names.sort();
    set<string> unique_label_names;
    names_it = label_names.begin();
    while ( names_it != label_names.end() )
    {
      unique_label_names.insert( *names_it );
      ++names_it;
    }

    set<string>::iterator label_it;
    q_cb_labels->clear();
    for ( label_it = unique_label_names.begin(); label_it != unique_label_names.end(); ++label_it )
      q_cb_labels->insertItem( QString( ( *label_it ).c_str() ) );
  }

  #ifdef _DEBUG
  debug.finish( "q_cb_visualizing_what_highlighted( " + Te2String( item ) + " )" );
  #endif
}

void geodma_window::q_cb_classifying_what_highlighted( int item )
{
  #ifdef _DEBUG
  debug.start( "q_cb_classifying_what_highlighted( " + Te2String( item ) + " )" );
  #endif

  if ( q_tab->page( get_tab_number( "Input" ) )->isEnabled() )
  {
    string attribute_table_name;
    TeLayer* layer_choice;
    TeTable table_choice;
    vector<string> objects_and_tables;

    if ( q_cb_classifying_what->text( item ) == "Polygons" )
    {
      if ( !using_polygons )
        return;
      layer_choice = get_polygons_layer();
      attribute_table_name = polygons_attribute_table_name;
      objects_and_tables = get_all_polygons_layers();
    }
    else if ( q_cb_classifying_what->text( item ) == "Cells" )
    {
      if ( !using_cells )
        return;
      layer_choice = get_cells_layer();
      attribute_table_name = cells_attribute_table_name;
      objects_and_tables = get_all_cells_layers();
    }
    else // Points
    {
      if ( !using_points )
        return;
      layer_choice = get_points_layer();
      attribute_table_name = points_attribute_table_name;
      objects_and_tables = get_all_points_layers();
    }

    /* Insert ancillary tables */
    q_lb_ancillary_training_sets->clear();
    for ( unsigned i = 0; i < objects_and_tables.size(); i++ )
      q_lb_ancillary_training_sets->insertItem( QString( objects_and_tables[ i ].c_str() ) );

    list<string> tereal_names = get_columns_names( layer_choice, attribute_table_name, table_choice, TeREAL );

    list<string> int_names = get_columns_names( layer_choice, attribute_table_name, table_choice, TeINT );
    list<string>::iterator names_it = int_names.begin();
    while ( names_it != int_names.end() )
    {
      tereal_names.push_back( *names_it );
      ++names_it;
    }

    if ( tereal_names.size() == 0 )
      return;
    tereal_names.sort();
    names_it = tereal_names.begin();
    q_cb_classifying_what->setCurrentItem( item );
    q_lb_attributes_candidate->clear();
    q_lb_attributes_classify->clear();
    while ( names_it != tereal_names.end() )
    {
      string name = *names_it;
      q_lb_attributes_candidate->insertItem( QString( name.c_str() ) );

      ++names_it;
    }

    /* Inserting unique label names */
    list<string> label_names = get_columns_names( layer_choice, attribute_table_name, table_choice, TeSTRING );
    if ( label_names.size() == 0 )
      return;

    label_names.sort();
    set<string> unique_label_names;
    names_it = label_names.begin();
    while ( names_it != label_names.end() )
    {
      unique_label_names.insert( *names_it );
      ++names_it;
    }

    set<string>::iterator label_it;
    q_cb_training_labels->clear();
    for ( label_it = unique_label_names.begin(); label_it != unique_label_names.end(); ++label_it )
      q_cb_training_labels->insertItem( QString( ( *label_it ).c_str() ) );
  }

  #ifdef _DEBUG
  debug.finish( "q_cb_classifying_what_highlighted( " + Te2String( item ) + " )" );
  #endif
}

void geodma_window::q_cb_validating_what_highlighted( int item )
{
  #ifdef _DEBUG
  debug.start( "q_cb_validating_what_highlighted( " + Te2String( item ) + " )" );
  #endif

  q_cb_validating_what->setCurrentItem( item );

  if ( q_tab->page( get_tab_number( "Input" ) )->isEnabled() )
  {
    string attribute_table_name;
    TeLayer* layer_choice;
    TeTable table_choice;

    if ( q_cb_validating_what->text( item ) == "Polygons" )
    {
      if ( !using_polygons )
        return;
      layer_choice = get_polygons_layer();
      attribute_table_name = polygons_attribute_table_name;
    }
    else if ( q_cb_validating_what->text( item ) == "Cells" )
    {
      if ( !using_cells )
        return;
      layer_choice = get_cells_layer();
      attribute_table_name = cells_attribute_table_name;
    }
    else // Points
    {
      if ( !using_points )
        return;
      layer_choice = get_points_layer();
      attribute_table_name = points_attribute_table_name;
    }

    list<string> tereal_names = get_columns_names( layer_choice, attribute_table_name, table_choice, TeREAL );

    list<string> int_names = get_columns_names( layer_choice, attribute_table_name, table_choice, TeINT );
    list<string>::iterator names_it = int_names.begin();
    while ( names_it != int_names.end() )
    {
      tereal_names.push_back( *names_it );
      ++names_it;
    }

    if ( tereal_names.size() == 0 )
      return;
    tereal_names.sort();
    names_it = tereal_names.begin();
    q_lb_monte_carlo_attributes->clear();
    while ( names_it != tereal_names.end() )
    {
      string name = *names_it;
      q_lb_monte_carlo_attributes->insertItem( QString( name.c_str() ) );

      ++names_it;
    }

    q_cb_reference_column->clear();
    q_cb_classification_column->clear();
    q_cb_monte_carlo_labels->clear();
    
    list<string> label_names = get_columns_names( layer_choice, attribute_table_name, table_choice, TeSTRING );
    if ( label_names.size() == 0 )
      return;
    label_names.sort();
    names_it = label_names.begin();
    while ( names_it != label_names.end() )
    {
      string name = *names_it;
      q_cb_reference_column->insertItem( QString( name.c_str() ) );
      q_cb_classification_column->insertItem( QString( name.c_str() ) );
      q_cb_monte_carlo_labels->insertItem( QString( name.c_str() ) );

      ++names_it;
    }
  }

  #ifdef _DEBUG
  debug.finish( "q_cb_validating_what_highlighted( " + Te2String( item ) + " )" );
  #endif
}

void geodma_window::q_cb_segmentation_algorithm_highlighted( int item )
{
  #ifdef _DEBUG
  debug.start( "q_cb_segmentation_algorithm_highlighted( " + Te2String( item ) + " )" );
  #endif

  q_fr_regiongrowing->setEnabled( q_cb_segmentation_algorithm->text( item ) == "Region Growing" );
  q_fr_baatz->setEnabled( q_cb_segmentation_algorithm->text( item ) == "Baatz & Shape" );
  q_fr_chessboard->setEnabled( q_cb_segmentation_algorithm->text( item ) == "Chessboard" );
  q_fr_geosomseg->setEnabled( q_cb_segmentation_algorithm->text( item ) == "GeoSOM Segmentation" );

  #ifdef _DEBUG
  debug.finish( "q_cb_segmentation_algorithm_highlighted( " + Te2String( item ) + " )" );
  #endif
}

/* =========== INITIALIZATION =========== */
void geodma_window::showEvent( QShowEvent * )
{
  #ifdef _DEBUG
  debug.start( "showEvent()" );
  #endif

  this->setMinimumSize( QSize( 730, 450 ) );
  this->setMaximumSize( QSize( 730, 450 ) );
  q_tab->setMinimumSize( QSize( 700, 400 ) );
  q_tab->setMaximumSize( QSize( 700, 400 ) );

  if ( !started )
  {
    started = true;
    fill_about();
    if ( update_first_window() )
      show();
  }

  #ifdef _DEBUG
  debug.finish( "showEvent()" );
  #endif
}

bool geodma_window::update_first_window()
{
  #ifdef _DEBUG
  debug.start( "update_first_window()" );
  #endif

  /* Get the current database pointer */
  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  if ( !curr_database )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "First open database" ) ); 
    this->close();
    return false;
  }

  polygons_layer_index = 0;
  cells_layer_index = 0;
  points_layer_index = 0;
  rasters_layer_index = 0;

  clear_inputs();
  /* Initializing main input */
  q_cb_polygon_layers->clear();
  q_cb_polygon_layers->setEnabled( true );
  q_cb_polygon_tables->clear();
  q_cb_polygon_tables->setEnabled( true );

  q_cb_cell_layers->clear();
  q_cb_cell_layers->setEnabled( true );
  q_cb_cell_tables->clear();
  q_cb_cell_tables->setEnabled( true );

  q_cb_point_layers->clear();
  q_cb_point_layers->setEnabled( true );
  q_cb_point_tables->clear();
  q_cb_point_tables->setEnabled( true );
  q_gb_mt_parameters->setEnabled( true );

  q_cb_raster_layers->clear();
  q_cb_raster_layers->setEnabled( true );

  q_pb_select_layers->setEnabled( true );
  q_pb_reset_geodma->setEnabled( false );

  /* Leave Segmentation and About tabs enabled */
  for ( int i = 1; i < q_tab->count() - 1; i++ )
    q_tab->page( i )->setEnabled( false );
  q_tab->page( get_tab_number( "Segmentation" ) )->setEnabled( true );
  q_tab->page( get_tab_number( "Input" ) )->setEnabled( true );
  q_tab->setCurrentPage( get_tab_number( "Input" ) );

  /* Updating input segments layer box */
  TeLayerMap::iterator lm_it = curr_database->layerMap().begin();
  polygons_layers.clear();

  while ( lm_it != curr_database->layerMap().end() )
  {
    if ( lm_it->second->hasGeometry( TePOLYGONS ) )
    {
      q_cb_polygon_layers->insertItem( lm_it->second->name().c_str() );
      polygons_layers.push_back( lm_it->second );
    }
    ++lm_it;
  }

  q_cb_polygon_layers->insertItem( "No Polygons" );
  q_cb_polygon_layers_highlighted( 0 );

  /* Updating input cells layer box */
  lm_it = curr_database->layerMap().begin();
  cells_layers.clear();

  while ( lm_it != curr_database->layerMap().end() )
  {
    if ( lm_it->second->hasGeometry( TeCELLS ) )
    {
      q_cb_cell_layers->insertItem( lm_it->second->name().c_str() );
      cells_layers.push_back( lm_it->second );
    }
    ++lm_it;
  }

  q_cb_cell_layers->insertItem( "No Cells" );
  q_cb_cell_layers_highlighted( 0 );

  /* Updating input points layer box */
  lm_it = curr_database->layerMap().begin();
  points_layers.clear();

  while ( lm_it != curr_database->layerMap().end() )
  {
    if ( lm_it->second->hasGeometry( TePOINTS ) )
    {
      q_cb_point_layers->insertItem( lm_it->second->name().c_str() );
      points_layers.push_back( lm_it->second );
    }
    ++lm_it;
  }

  q_cb_point_layers->insertItem( "No Points" );
  q_cb_point_layers_highlighted( 0 );

  /* Updating input raster layer box */
  lm_it = curr_database->layerMap().begin();
  rasters_layers.clear();

  while ( lm_it != curr_database->layerMap().end() )
  {
    if ( lm_it->second->hasGeometry( TeRASTER ) || lm_it->second->hasGeometry( TeRASTERFILE ) )
    {
      q_cb_raster_layers->insertItem( lm_it->second->name().c_str() );
      q_cb_available_raster_layers_segmentation->insertItem( lm_it->second->name().c_str() );
      rasters_layers.push_back( lm_it->second );
    }
    ++lm_it;
  }

  q_cb_raster_layers->insertItem( "No Rasters" );

  #ifdef _DEBUG
  debug.finish( "update_first_window()" );
  #endif

  return true;
}

/* ========== SEGMENTATION ========== */
string geodma_window::get_valid_layer_name( string needle )
{
  #ifdef _DEBUG
  debug.start( "get_valid_layer_name( " + needle + " )" );
  #endif

  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  TeLayerMap::iterator lm_it = curr_database->layerMap().begin();
  unsigned total_needle = 0;
  while ( lm_it != curr_database->layerMap().end() ) 
  {
    if ( ( lm_it->second->hasGeometry( TePOLYGONS ) /*|| lm_it->second->hasGeometry( TeCELLS ) */ ) && lm_it->second->name().find( needle ) != string::npos )
      total_needle++;
    ++lm_it;
  }
  string result = needle + "_" + Te2String( total_needle + 1 );

  #ifdef _DEBUG
  debug.finish( "get_valid_layer_name( " + result + " )" );
  #endif

  return result;
}

bool geodma_window::create_polygons_theme( TeLayer *polygons_layer, string theme_name, TeLayer *raster_layer )
{
  #ifdef _DEBUG
  debug.start( "create_polygons_theme( " + theme_name + " )" );
  #endif

  /* Creating new View */
  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  TeProjection* new_view_proj = TeProjectionFactory::make( polygons_layer->projection()->params() );
  TeView* new_view = new TeView ( polygons_layer->name(), curr_database->user() );
  new_view->projection( new_view_proj );
  new_view->setCurrentBox( polygons_layer->box() );
  new_view->setCurrentTheme( -1 );
  if ( !curr_database->insertView( new_view ) )
  {
    delete new_view;
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to insert the new view into database" ) );
    return false;
  }

  /* Creating new Theme */
  TeTheme* new_theme = new TeTheme( polygons_layer->name(), polygons_layer );
  if ( !new_theme )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Invalid Theme pointer" ) );
    return false;
  }

  new_view->add( new_theme );

  /* Adjusting theme visual */
  TeVisual* new_visual = new TeVisual( TePOLYGONS );
  TeColor new_color;
  TeColor contour_color;
  new_color.init( 255, 255, 255 );
  contour_color.init( 0, 0, 0 );
  new_visual->color( new_color ); 
  new_visual->contourColor( contour_color );
  new_visual->transparency( 75 );
  new_theme->setVisualDefault( new_visual, TePOLYGONS );

  TeVisual* point_visual = new TeVisual( TePOINTS );
  new_color.init( 0, 0, 0 );
  point_visual->color( new_color );
  point_visual->style( TePtTypeX );
  point_visual->contourColor( contour_color );
  new_theme->setVisualDefault( point_visual, TePOINTS );

  int allRep = polygons_layer->geomRep();
  new_theme->visibleRep( allRep );
  new_theme->setAttTables( polygons_layer->attrTables() );

  if ( !new_theme->save() )
  {
    delete new_theme;
    curr_database->deleteView( new_view->id() );
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to create the new theme" ) );
    return false;
  }
  if ( !new_theme->buildCollection() )
  {
    delete new_theme;
    curr_database->deleteView( new_view->id() );
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to build the new theme collection" ) );
    return false;
  }

  /* Creating Theme for Raster*/
  TeTheme* raster_theme = new TeTheme( "input_raster", raster_layer );
  if ( !raster_theme )
  {
    delete raster_theme;
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Invalid Theme pointer" ) );
    return false;
  }
  new_view->add( raster_theme );

  int raster_allRep = raster_layer->geomRep();
  raster_theme->visibleRep( raster_allRep );
  raster_theme->setAttTables( raster_layer->attrTables() );

  if ( !raster_theme->save() )
  {
    delete raster_theme;
    curr_database->deleteView( new_view->id() );
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to create the new theme" ) );
    return false;
  }

  if ( !raster_theme->buildCollection() )
  {
    delete raster_theme;
    curr_database->deleteView( new_view->id() );
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to build the new theme collection" ) );
    return false;
  }

  #ifdef _DEBUG
  debug.finish( "create_polygons_theme( " + theme_name + " )" );
  #endif

  return true;
}

bool geodma_window::create_polygons_layer( TePolygonSet pols, string polygons_layer_name, TeProjection *layer_projection )
{
  #ifdef _DEBUG
  debug.start( "create_polygons_layer( " + polygons_layer_name + " )" );
  #endif
 
  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  
  /* Creating layer */
  if ( curr_database->layerExist( polygons_layer_name ) )
  {
    QString message;
    message.sprintf( "Layer %s already exists", polygons_layer_name.c_str() );
    QMessageBox::critical( this, function_name( __FUNCTION__ ), message );
    return false;
  }

  TeLayer *new_layer = new TeLayer( polygons_layer_name, curr_database, layer_projection );
  for ( unsigned i = 0; i < pols.size(); i++ )
    pols[ i ].objectId( Te2String( i ) );

  /* Adding polygons geometry */
  if ( !new_layer->addPolygons( pols ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not add polygons into layer" ) );
    delete new_layer;
    return false;
  }

  /* Creating new attribute */
  TeAttributeList attList;
  TeAttribute at;
  at.rep_.name_ = "id";
  at.rep_.type_ = TeSTRING;
  at.rep_.numChar_ = 16;
  at.rep_.isPrimaryKey_ = true;
  attList.push_back( at );

  string polygons_attribute_table_name = polygons_layer_name;
  TeTable polygons_table( polygons_attribute_table_name, attList, "id", "id" );
  if ( !new_layer->createAttributeTable( polygons_table ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not create the attribute table" ) );
    delete new_layer;
    return false;
  }

  TeTableRow attr_row;
  for ( unsigned i = 0; i < pols.size(); i++ )
  {
    attr_row.push_back( Te2String( i ) ); 
    polygons_table.add( attr_row );
    attr_row.clear();
  }
  if ( !new_layer->saveAttributeTable( polygons_table ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not save the attribute table" ) );
    delete new_layer;
    return false;
  }

  #ifdef _DEBUG
  debug.finish( "create_polygons_layer()" );
  #endif
 
  return true;
}

void geodma_window::q_pb_run_segmentation_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_pb_run_segmentation_clicked()" );
  #endif

  map<string, string> algorithms_suffixes;
  algorithms_suffixes[ "Region Growing" ] = "_regiongrowing";
  algorithms_suffixes[ "Baatz & Shape" ] = "_baatzshape";
  algorithms_suffixes[ "Chessboard" ] = "_chessboard";
  algorithms_suffixes[ "GeoSOM Segmentation" ] = "_geosom";

  string algorithm_suffix = algorithms_suffixes[ q_cb_segmentation_algorithm->currentText().ascii() ];

  /* Attributes validation */
  if ( !q_cb_available_raster_layers_segmentation->count() )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "No Layer to select" ) );
    return;
  }

  /* Retrieving segmentation parameters */
  TePDITypes::TePDIRasterPtrType ptr_raster;
  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  TeLayer *raster_layer = new TeLayer();
  TeLayerMap::iterator lm_it = curr_database->layerMap().begin();
  while ( lm_it != curr_database->layerMap().end() ) 
  {
    if ( lm_it->second->hasGeometry( TeRASTER ) || lm_it->second->hasGeometry( TeRASTERFILE ) )
      if ( lm_it->second->name().c_str() == q_cb_available_raster_layers_segmentation->currentText() )
      {
        ptr_raster.reset( lm_it->second->raster(), true );
        raster_layer = lm_it->second;
      }
    ++lm_it;
  }

  if ( !ptr_raster.isActive() )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem opening raster" ) );
    return;
  }

  TePDITypes::TePDIRasterPtrType segmented_raster;
  TeRasterParams segmented_raster_params;
  segmented_raster_params.setDataType( TeUNSIGNEDLONG );
  segmented_raster_params.setNLinesNColumns( 1, 1 );
  segmented_raster_params.nBands( 1 );
  segmented_raster_params.mode_ = 'c';
  segmented_raster_params.decoderIdentifier_ = "SMARTMEM";
  segmented_raster.reset( new TeRaster( segmented_raster_params ) );
  segmented_raster->init();

  /* Choose selected algorithm and parameters */
  TePDITypes::TePDIPolSetMapPtrType output_polsets( new TePDITypes::TePDIPolSetMapType );
  ptr_raster->params().useDummy_ = false;
  {
    bool big_image = false;
    if ( ptr_raster->params().ncols_ * ptr_raster->params().nlines_ > 1000000 )
      big_image = true;

    TePDIParameters segmentation_params;
    if ( algorithm_suffix == "_regiongrowing" )
    {
      if ( big_image )
      {
        /* Region Growing with parallel implementation */
        /* Defining output image */
        TeRasterParams out_params;
        out_params.nBands( 1 );
        out_params.setNLinesNColumns( 1, 1 );
        out_params.setDataType( TeUNSIGNEDLONG );

        TePDITypes::TePDIRasterPtrType bins_raster;
        TePDIUtils::TeAllocRAMRaster( out_params, bins_raster );

        /* Defining segmenter parameters */
        TePDIParameters strategy_params;
        strategy_params.SetParameter( "euc_treshold", ( double ) q_sb_regiongrowing_euclideandistance->text().toDouble() );
        strategy_params.SetParameter( "area_min", ( int ) q_sb_regiongrowing_minimumarea->text().toInt() );

        vector<unsigned> input_bands;
        for ( int i = 0; i < ptr_raster->params().nBands(); i++ )
          input_bands.push_back( i );

        /* Defining parallelism parameters */
        TePDIParameters segmenterParams;
        segmenterParams.SetParameter( "input_image_ptr", ptr_raster );
        segmenterParams.SetParameter( "input_channels", input_bands );
        segmenterParams.SetParameter( "strategy_name", std::string( "RegionGrowing") );
        segmenterParams.SetParameter( "output_image_ptr", bins_raster );
        segmenterParams.SetParameter( "strategy_params", strategy_params );
        segmenterParams.SetParameter( "max_seg_threads", (unsigned int) 0 );
        segmenterParams.SetParameter( "max_block_size", (unsigned int) 512 );
        //segmenterParams.SetParameter( "merge_adjacent_blocks", (bool)false );

        /* Applying segmentation */
        TePDIParallelSegmenter segmenter;
        if ( !segmenter.Apply( segmenterParams ) )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem in Region Growing Segmentation" ) );
          return;
        }

        /* Creating output polset */
        segmentation_params.SetParameter( "rotulated_image", bins_raster );
        segmentation_params.SetParameter( "channel", (unsigned int) 0 );
        segmentation_params.SetParameter( "output_polsets", output_polsets );
        // segmentation_params.SetParameter( "max_pols", polygon_label );

        TePDIRaster2Vector segmentation_instance;
        if ( !segmentation_instance.Reset( segmentation_params ) )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem resetting Region Growing Vectorization Algorithm" ) );
          return;
        }

        /* Applying segmentation */
        if ( !segmentation_instance.Apply() )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem in Region Growing Vectorization" ) );
          return;
        }
      }
      else
      {
        /* Region Growing without parallel implementation */
        segmentation_params.SetParameter( "input_image", ptr_raster );
        segmentation_params.SetParameter( "output_polsets", output_polsets );
        segmentation_params.SetParameter( "euc_treshold", ( double ) q_sb_regiongrowing_euclideandistance->text().toDouble() );
        segmentation_params.SetParameter( "area_min", ( int ) q_sb_regiongrowing_minimumarea->text().toInt() );

        TePDIRegGrowSeg segmentation_instance;
        if ( !segmentation_instance.Reset( segmentation_params ) )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem resetting Region Growing Algorithm" ) );
          return;
        }

        /* Applying segmentation */
        if ( !segmentation_instance.Apply() )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem in Region Growing Segmentation" ) );
          return;
        }
      }
    }
    else if ( algorithm_suffix == "_baatzshape" )
    {
      if ( big_image )
      {
        /* Baatz with parallel implementation */
        /* Defining output image */
        TeRasterParams out_params;
        out_params.nBands( 1 );
        out_params.setNLinesNColumns( 1, 1 );
        out_params.setDataType( TeUNSIGNEDLONG );

        TePDITypes::TePDIRasterPtrType baatz_raster;
        TePDIUtils::TeAllocRAMRaster( out_params, baatz_raster );

        /* Defining segmenter parameters */
        TePDIParameters strategy_params;
        strategy_params.SetParameter( "scale", (float) q_sb_baatz_scale->text().toFloat() );
        strategy_params.SetParameter( "compactness", (float) ( q_sb_baatz_compactness->text().toDouble() / 100 ) );
        strategy_params.SetParameter( "color", (float) ( q_sb_baatz_color->text().toFloat() / 100 ));
        strategy_params.SetParameter( "euc_treshold", (double) 20 );

        vector<unsigned> input_bands;
        vector<float> input_weights;
        for ( int i = 0; i < ptr_raster->params().nBands(); i++ )
        {
          input_bands.push_back( i );
          input_weights.push_back( ( float ) 1 / ptr_raster->params().nBands() );
        }
        strategy_params.SetParameter( "input_weights", input_weights );

        /* Defining parallelism parameters */
        TePDIParameters segmenterParams;
        segmenterParams.SetParameter( "input_image_ptr", ptr_raster );
        segmenterParams.SetParameter( "input_channels", input_bands );
        segmenterParams.SetParameter( "strategy_name", std::string( "Baatz") );
        segmenterParams.SetParameter( "output_image_ptr", baatz_raster );
        segmenterParams.SetParameter( "strategy_params", strategy_params );
        segmenterParams.SetParameter( "max_seg_threads", (unsigned int) 0 );
        segmenterParams.SetParameter( "max_block_size", (unsigned int) 512 );
        //segmenterParams.SetParameter( "merge_adjacent_blocks", (bool)false );

        /* Applying segmentation */
        TePDIParallelSegmenter segmenter;
        if ( !segmenter.Apply( segmenterParams ) )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem in Baatz Segmentation" ) );
          return;
        }

        /* Creating output polset */
        segmentation_params.SetParameter( "rotulated_image", baatz_raster );
        segmentation_params.SetParameter( "channel", (unsigned int) 0 );
        segmentation_params.SetParameter( "output_polsets", output_polsets );
        // segmentation_params.SetParameter( "max_pols", polygon_label );

        TePDIRaster2Vector segmentation_instance;
        if ( !segmentation_instance.Reset( segmentation_params ) )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem resetting Baatz Vectorization Algorithm" ) );
          return;
        }

        /* Applying segmentation */
        if ( !segmentation_instance.Apply() )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem in Baatz Vectorization" ) );
          return;
        }
      }
      else
      {
        /* Baatz without parallel implementation */
        segmentation_params.SetParameter( "input_image", ptr_raster );
        segmentation_params.SetParameter( "output_image", segmented_raster );
        segmentation_params.SetParameter( "output_polsets", output_polsets );
        segmentation_params.SetParameter( "scale", ( float ) q_sb_baatz_scale->text().toFloat() );
        segmentation_params.SetParameter( "color", ( float ) ( q_sb_baatz_color->text().toFloat() / 100 ) );
        segmentation_params.SetParameter( "compactness", ( float ) ( q_sb_baatz_compactness->text().toDouble() / 100 ) );
        vector<unsigned> input_bands;
        vector<float> input_weights;

        for ( int i = 0; i < ptr_raster->params().nBands(); i++ )
        {
          input_bands.push_back( i );
          input_weights.push_back( ( float ) 1 / ptr_raster->params().nBands() );
        }
        segmentation_params.SetParameter( "input_weights", input_weights );
        segmentation_params.SetParameter( "input_channels", input_bands );

        TePDIBaatz segmentation_instance;
        if ( !segmentation_instance.Reset( segmentation_params ) )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem resetting Baatz Algorithm" ) );
          return;
        }

        /* Applying segmentation */
        if ( !segmentation_instance.Apply() )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem in Baatz Segmentation" ) );
          return;
        }
      }
    }
    else if ( algorithm_suffix == "_chessboard" )
    {
      TePDITypes::TePDIRasterPtrType chessboard_raster;
      TeRasterParams r_params( ptr_raster->params() );
      r_params.nBands( 1 );
      r_params.setDataType( TeUNSIGNEDCHAR );
      r_params.boundingBoxLinesColumns( ptr_raster->params().box().x1(), ptr_raster->params().box().y1(), 
                                        ptr_raster->params().box().x2(), ptr_raster->params().box().y2(),
                                        q_sb_chessboard_lines->text().toInt(), 
                                        q_sb_chessboard_columns->text().toInt() );

      TePDIUtils::TeAllocRAMRaster( chessboard_raster, r_params, TePDIUtils::TePDIUtilsRamMemPol );

      /* Initialize Raster */
      unsigned long polygon_label = 1;
      for ( unsigned curr_col = 0; curr_col < (unsigned) chessboard_raster->params().ncols_; curr_col++ )
        for ( unsigned curr_line = 0; curr_line < (unsigned) chessboard_raster->params().nlines_; curr_line++ )
          chessboard_raster->setElement( curr_col, curr_line, polygon_label++ );

      segmentation_params.SetParameter( "rotulated_image", chessboard_raster );
      segmentation_params.SetParameter( "channel", (unsigned int) 0 );
      segmentation_params.SetParameter( "output_polsets", output_polsets );
      segmentation_params.SetParameter( "max_pols", polygon_label );

      TePDIRaster2Vector segmentation_instance;
      if ( !segmentation_instance.Reset( segmentation_params ) )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem resetting Chessboard Algorithm" ) );
        return;
      }

      /* Applying segmentation */
      if ( !segmentation_instance.Apply() )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem in Chessboard Segmentation" ) );
        return;
      }
    }
    else if ( algorithm_suffix == "_geosom" )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "GeoSOM Segmentation not implemented yet" ));
      return;
    }
  }

  TePDITypes::TePDIPolSetMapType::iterator psmap_it = output_polsets->begin();
  TePolygonSet pols;
  while ( psmap_it != output_polsets->end() )
  {
    TePolygonSet::iterator ps_it = psmap_it->second.begin();
    while ( ps_it != psmap_it->second.end() )
    {
      pols.add( *ps_it );
      ++ps_it;
    }
    ++psmap_it;
  }

  /* Creating layer with polygons */
  string polygons_layer_name = get_valid_layer_name( string( q_cb_available_raster_layers_segmentation->currentText().ascii() ) + algorithm_suffix );
  
  if ( !create_polygons_layer( pols, polygons_layer_name, ptr_raster->projection() ) )
    return;

  /* Showing, or not, the polygons */
  QString message;
  message.sprintf( "Segmentation done!\n %d polygon(s) created. Show the polygons?", pols.size() );
  if ( QMessageBox::question( this, function_name( __FUNCTION__ ), message, "Yes", "No" ) == 0 )
    create_polygons_theme( get_layer( polygons_layer_name ), polygons_layer_name, raster_layer );

  update_first_window();
  update_TV_interface();

  #ifdef _DEBUG
  debug.finish( "q_pb_run_segmentation_clicked()" );
  #endif
}

/* ========== EXTRACTION ========== */
void geodma_window::q_pb_extract_attributes_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_pb_extract_attributes_clicked()" );
  #endif

  unlock_grid();

  if ( !q_lb_attributes_extract->count() )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Select at least one attribute" ) );
    return;
  }

  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  bool updated_cells_table = false;
  bool updated_polygons_table = false;
  bool updated_points_table = false;

  /* Importing Rasters */
  // TeLayer* rasters_layer = get_rasters_layer();
  // TeRaster* raster = rasters_layer->raster();
  vector<int> bands;
  TePDITypes::TePDIRasterVectorType rasters;
  TePDITypes::TePDIRasterPtrType tmp_raster;
  if ( using_rasters )
    for ( int b = 0; b < myRaster->params().nBands(); b++ )
    {
      bands.push_back( b );
      tmp_raster.reset( myRaster, true );
      rasters.push_back( tmp_raster );
    }

  bool need_polygons_features = false;
  bool need_cells_features = false;
  bool need_rasters_polygons_features = false;
  bool need_rasters_cells_features = false;
  bool need_mt_features = false;

  for ( unsigned j = 0; j < q_lb_attributes_extract->count(); j++ )
  {
    if ( q_lb_attributes_extract->text( j ).left( 2 ).compare( QString( "p_" ) ) == 0 )
      need_polygons_features = true;
    if ( q_lb_attributes_extract->text( j ).left( 2 ).compare( QString( "c_" ) ) == 0 )
      need_cells_features = true;
    if ( q_lb_attributes_extract->text( j ).left( 3 ).compare( QString( "rp_" ) ) == 0 )
      need_rasters_polygons_features = true;
    if ( q_lb_attributes_extract->text( j ).left( 3 ).compare( QString( "rc_" ) ) == 0 )
      need_rasters_cells_features = true;
    if ( q_lb_attributes_extract->text( j ).left( 3 ).compare( QString( "mt_" ) ) == 0 )
      need_mt_features = true;
  }

  /* cell_set extraction */
  if ( using_cells && ( need_cells_features || need_rasters_cells_features ) )
  {
    TeLayer* cells_layer = get_cells_layer();
    /* Creating attributes columns in cells table */
    vector<string> cells_attributes_names;
    vector<unsigned> cells_attributes_indexes;
    TeTable cells_table;
    for ( unsigned i = 0; i < q_lb_attributes_extract->count(); i++ )
    {
      if ( ( q_lb_attributes_extract->text( i ).left( 2 ).compare( QString( "c_" ) ) == 0 ) ||
         ( q_lb_attributes_extract->text( i ).left( 3 ).compare( QString( "rc_" ) ) == 0 ) )
        cells_attributes_names.push_back( q_lb_attributes_extract->text( i ).ascii() );
      else
        continue;
      updated_cells_table = true;

      if ( !cells_layer->getAttrTablesByName( cells_attribute_table_name, cells_table, TeAllAttrTypes ) )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the cells theme attribute table" ) );
        return;
      }

      /* Creating new attribute */
      TeAttribute new_attr;
      QString norm_name = q_lb_attributes_extract->text( i );
      new_attr.rep_.name_ = norm_name.ascii();
      new_attr.rep_.type_ = TeREAL;
      new_attr.rep_.null_ = true;
      new_attr.rep_.numChar_ = 15;
      new_attr.rep_.decimals_ = 6;

      if ( !curr_database->columnExist( cells_attribute_table_name, new_attr.rep_.name_, new_attr ) )
        if ( !curr_database->addColumn( cells_attribute_table_name, new_attr.rep_ ) )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not create the new column.\n" ) + curr_database->errorMessage().c_str() );
          return;
        }
    }

    /* Finding attributes indexes in cells table */
    for ( unsigned i = 0; i < cells_attributes_names.size(); i++ )
    {
      int attr_index = find_index( cells_attributes_names[ i ], cells_layer, cells_attribute_table_name );
      if ( attr_index < 0 )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find cell Attribute Index" ) );
        return;
      }
      cells_attributes_indexes.push_back( attr_index );
    }
    cells_table.clear();
    if ( !curr_database->loadTable( cells_attribute_table_name, cells_table ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the cell theme attribute table" ) );
      return;
    }

    /* Getting primary key index in cells table */
    int cells_real_index = -1;
    string cells_primary_key;
    TeAttributeList& cells_attr_list = cells_table.attributeList();
    TeAttributeList::iterator cells_attr_list_it = cells_attr_list.begin();

    while ( cells_attr_list_it != cells_attr_list.end() )
    {
      if ( cells_attr_list_it->rep_.isPrimaryKey_ )
      {
        cells_primary_key = cells_attr_list_it->rep_.name_;
        break;
      }
      ++cells_attr_list_it;
    }
    int cells_primary_index = find_index( cells_primary_key, cells_layer, cells_attribute_table_name );
    if ( cells_primary_index < 0 )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find cells primary key index" ) );
      return;
    }

    /* Defining Cellular Attributes */
    //TeCellSet cell_set;
    //get_cells_layer()->getCells( cell_set );
    //TePolygonSet polygons;
    //get_polygons_layer()->getPolygons( polygons );
    TeLandscape landscape_attributes( myCells, myPolygons );

    /* Creating Polygons from Cells */
    TePolygonSet cells_polygons;
    for ( unsigned c = 0; c < myCells.size(); c++ )
    {
      TePolygon cell_polygon = polygonFromBox( myCells[ c ].box() );
      cells_polygons.add( cell_polygon );
    }

    /* Defining Spectral Attributes */
    TePDITypes::TePDIPolygonSetPtrType polygon_set_vector( &cells_polygons, true );
    TePDIParameters statistic_params;
    statistic_params.SetParameter( "rasters", rasters );
    statistic_params.SetParameter( "bands", bands );
    statistic_params.SetParameter( "polygonset", polygon_set_vector );
    TePDIStatistic rasters_attributes;
    rasters_attributes.ToggleProgInt(false);

    TePDITypes::TePDIRasterPtrType input_raster;
    input_raster.reset( myRaster, true );
    TextureProperties texture_properties( cells_polygons, input_raster );

    if ( using_rasters )
    {
      if ( !rasters_attributes.Reset( statistic_params ) )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to create statistics with Raster" ) );
        return;
      }
    }

    /* Extracting Attributes from cells */
    TePDIPIManager cell_status( tr( "Extracting cellular features..." ).ascii(), myCells.size(), true ) ;
    for ( unsigned i = 0; i < myCells.size(); i++ )
    {
      cell_status.Increment();
      vector<double> extracted_attributes;
      vector<string> names_attributes;

      /* Extracting attributes from each cell */
      for ( unsigned j = 0; j < q_lb_attributes_extract->count(); j++ )
      {
        /* Spatial Attributes */
        if ( need_cells_features )
        {
          double value;
          if ( landscape_attributes.getFeature( string( q_lb_attributes_extract->text( j ).ascii() ), i, value ) )
          {
            extracted_attributes.push_back( value );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
        }

        /* Spectral Attributes */
        if ( need_rasters_cells_features )
        {
          for ( unsigned b = 0; b < bands.size(); b++ )
          {
            string amplitude_name = "rc_amplitude_" + Te2String( b );
            string entropy_name = "rc_entropy_" + Te2String( b );
            string mean_name = "rc_mean_" + Te2String( b );
            string mode_name = "rc_mode_" + Te2String( b );
            string stddev_name = "rc_stddev_" + Te2String( b );
            string sum_name = "rc_sum_" + Te2String( b );

            string dissimilarity_name = "rc_dissimilarity_" + Te2String( b );
            string entropy2_name = "rc_entropy2_" + Te2String( b );
            string homogeneity_name = "rc_homogeneity_" + Te2String( b );

            if ( q_lb_attributes_extract->text( j ).compare( QString( amplitude_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( rasters_attributes.getMax( b, i ) - rasters_attributes.getMin( b, i ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
            else if ( q_lb_attributes_extract->text( j ).compare( QString( entropy_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( rasters_attributes.getEntropy( b, i ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
            else if ( q_lb_attributes_extract->text( j ).compare( QString( mean_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( rasters_attributes.getMean( b, i ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
            else if ( q_lb_attributes_extract->text( j ).compare( QString( mode_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( rasters_attributes.getMode( b, i ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
            else if ( q_lb_attributes_extract->text( j ).compare( QString( stddev_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( rasters_attributes.getStdDev( b, i ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
            else if ( q_lb_attributes_extract->text( j ).compare( QString( sum_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( rasters_attributes.getSum( b, i ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
            else if ( q_lb_attributes_extract->text( j ).compare( QString( dissimilarity_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( texture_properties.getDissimilarity( b, i ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
            else if ( q_lb_attributes_extract->text( j ).compare( QString( entropy2_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( texture_properties.getEntropy( b, i ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
            else if ( q_lb_attributes_extract->text( j ).compare( QString( homogeneity_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( texture_properties.getHomogeneity( b, i ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
          }
        }
      }

      /* Storing attributes in table */
      cells_real_index = -1;
      for ( unsigned k = 0; k < cells_table.size(); k++ )
        if ( cells_table( k, cells_primary_index ).c_str() == myCells[ i ].objectId() )
        {
          cells_real_index = k;
          break;
        }

      if ( cells_real_index < 0 )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find cells attribute index" ) );
        return;
      }

      if ( !execute_query( create_update_query( cells_attribute_table_name, cells_primary_key, Te2String( cells_real_index ), names_attributes, extracted_attributes) ) )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not update table with polygon information:\n" ) + curr_database->errorMessage().c_str() );
        return;
      }
    }

    cell_status.Toggle( false );
  }

  /* polygon_set extraction */
  if ( using_polygons && ( need_polygons_features || need_rasters_polygons_features ) )
  {
    /* Importing polygons from current theme */
    //TePolygonSet polygon_set;
    //TeLayer* polygons_layer = get_polygons_layer();
    //TeBox b( -TeMAXFLOAT, -TeMAXFLOAT, TeMAXFLOAT, TeMAXFLOAT );
    //if ( !curr_database->loadPolygonSet( polygons_layer->tableName( TePOLYGONS ), b, polygon_set ) )
    //{
    //  QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to load polygons" ) );
    //  return;
    //}

    /* Creating attributes columns in polygons table */
    vector<string> polygons_attributes_names;
    vector<unsigned> polygons_attributes_indexes;
    TeTable polygons_table;
    for ( unsigned i = 0; i < q_lb_attributes_extract->count(); i++ )
    {
      if ( ( q_lb_attributes_extract->text( i ).left( 2 ).compare( QString( "p_" ) ) == 0 ) ||
         ( q_lb_attributes_extract->text( i ).left( 3 ).compare( QString( "rp_" ) ) == 0 ) )
        polygons_attributes_names.push_back( q_lb_attributes_extract->text( i ).ascii() );
      else
        continue;
      updated_polygons_table = true;
      if ( !get_polygons_layer()->getAttrTablesByName( polygons_attribute_table_name, polygons_table, TeAllAttrTypes ) )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the polygons theme attribute table" ) );
        return;
      }

      /* Creating new attribute */
      TeAttribute new_attr;
      QString norm_name = q_lb_attributes_extract->text( i );
      new_attr.rep_.name_ = norm_name.ascii();
      new_attr.rep_.type_ = TeREAL;
      new_attr.rep_.null_ = true;
      new_attr.rep_.numChar_ = 15;
      new_attr.rep_.decimals_ = 6;

      if ( !curr_database->columnExist( polygons_attribute_table_name, new_attr.rep_.name_, new_attr ) )
        if ( !curr_database->addColumn( polygons_attribute_table_name, new_attr.rep_ ) )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not create the new column.\n" ) + curr_database->errorMessage().c_str() );
          return;
        }
    }

    /* Finding attributes indexes in polygons table */
    for ( unsigned i = 0; i < polygons_attributes_names.size(); i++ )
    {
      int attr_index = find_index( polygons_attributes_names[ i ], get_polygons_layer(), polygons_attribute_table_name );
      if ( attr_index < 0 )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find polygons Attribute Index" ) );
        return;
      }
      polygons_attributes_indexes.push_back( attr_index );
    }

    polygons_table.clear();
    if ( !curr_database->loadTable( polygons_attribute_table_name, polygons_table ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the polygons theme attribute table" ) );
      return;
    }

    /* Getting primary key index in polygons table */
    int polygons_real_index = -1;
    string polygons_primary_key;
    TeAttributeList& polygons_attr_list = polygons_table.attributeList();
    TeAttributeList::iterator polygons_attr_list_it = polygons_attr_list.begin();

    while ( polygons_attr_list_it != polygons_attr_list.end() )
    {
      if ( polygons_attr_list_it->rep_.isPrimaryKey_ )
      {
        polygons_primary_key = polygons_attr_list_it->rep_.name_;
        break;
      }
      ++polygons_attr_list_it;
    }

    int polygons_primary_index = find_index( polygons_primary_key, get_polygons_layer(), polygons_attribute_table_name );
    if ( polygons_primary_index < 0 )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find polygons primary key index" ) );
      return;
    }
    /* Defining Spatial Attributes */
    polygons_features polygons_attributes( myPolygons );

    /* Defining Spectral Attributes */
    TePDITypes::TePDIPolygonSetPtrType polygon_set_vector( &myPolygons, true );
    TePDIParameters statistic_params;
    statistic_params.SetParameter( "rasters", rasters );
    statistic_params.SetParameter( "bands", bands );
    statistic_params.SetParameter( "polygonset", polygon_set_vector );
    TePDIStatistic rasters_attributes;
    rasters_attributes.ToggleProgInt(false);
    if ( using_rasters )
      rasters_attributes.Reset( statistic_params );

    TePDITypes::TePDIRasterPtrType input_raster;
    input_raster.reset( myRaster, true );
    TextureProperties texture_properties( myPolygons, input_raster );

    /* Extracting Attributes from polygon_set */
    TePDIPIManager polygons_status( tr( "Extracting polygons features..." ).ascii(), myPolygons.size(), true );
    for ( unsigned p = 0; p < myPolygons.size(); p++ )
    {
      polygons_status.Increment();
      vector<double> extracted_attributes;
      vector<string> names_attributes;

      /* Extracting attributes from each polygon */
      for ( unsigned j = 0; j < q_lb_attributes_extract->count(); j++ )
      {
        /* Spatial Attributes */
        if ( need_polygons_features )
        {
          if ( q_lb_attributes_extract->text( j ).compare( QString( "p_angle" ) ) == 0 )
          {
            extracted_attributes.push_back( polygons_attributes.getPolygonAngle( p ) );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
          if ( q_lb_attributes_extract->text( j ).compare( QString( "p_area" ) ) == 0 )
          {
            extracted_attributes.push_back( polygons_attributes.getPolygonArea( p ) );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
          if ( q_lb_attributes_extract->text( j ).compare( QString( "p_box_area" ) ) == 0 )
          {
            extracted_attributes.push_back( polygons_attributes.getPolygonBoxArea( p ) );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
          if ( q_lb_attributes_extract->text( j ).compare( QString( "p_circle" ) ) == 0 )
          {
            extracted_attributes.push_back( polygons_attributes.getPolygonCircle( p ) );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
          if ( q_lb_attributes_extract->text( j ).compare( QString( "p_compacity" ) ) == 0 )
          {
            extracted_attributes.push_back( polygons_attributes.getPolygonCompacity( p ) );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
          if ( q_lb_attributes_extract->text( j ).compare( QString( "p_contiguity" ) ) == 0 )
          {
            extracted_attributes.push_back( polygons_attributes.getPolygonContiguity( p ) );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
          if ( q_lb_attributes_extract->text( j ).compare( QString( "p_density" ) ) == 0 )
          {
            extracted_attributes.push_back( polygons_attributes.getPolygonDensity( p ) );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
          if ( q_lb_attributes_extract->text( j ).compare( QString( "p_elliptic_fit" ) ) == 0 )
          {
            extracted_attributes.push_back( polygons_attributes.getPolygonEllipticFit( p ) );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
          if ( q_lb_attributes_extract->text( j ).compare( QString( "p_fractal_dimension" ) ) == 0 )
          {
            extracted_attributes.push_back( polygons_attributes.getPolygonFractalDimension( p ) );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
          if ( q_lb_attributes_extract->text( j ).compare( QString( "p_gyration_radius" ) ) == 0 )
          {
            extracted_attributes.push_back( polygons_attributes.getPolygonGyrationRadius( p ) );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
          if ( q_lb_attributes_extract->text( j ).compare( QString( "p_length" ) ) == 0 )
          {
            extracted_attributes.push_back( polygons_attributes.getPolygonLength( p ) );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
          if ( q_lb_attributes_extract->text( j ).compare( QString( "p_perimeter" ) ) == 0 )
          {
            extracted_attributes.push_back( polygons_attributes.getPolygonPerimeter( p ) );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
          if ( q_lb_attributes_extract->text( j ).compare( QString( "p_perimeter_area_ratio" ) ) == 0 )
          {
            extracted_attributes.push_back( polygons_attributes.getPolygonPerimeterAreaRatio( p ) );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
          if ( q_lb_attributes_extract->text( j ).compare( QString( "p_rectangular_fit" ) ) == 0 )
          {
            extracted_attributes.push_back( polygons_attributes.getPolygonRectangularFit( p ) );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
          if ( q_lb_attributes_extract->text( j ).compare( QString( "p_shape_index" ) ) == 0 )
          {
            extracted_attributes.push_back( polygons_attributes.getPolygonShapeIndex( p ) );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
          if ( q_lb_attributes_extract->text( j ).compare( QString( "p_width" ) ) == 0 )
          {
            extracted_attributes.push_back( polygons_attributes.getPolygonWidth( p ) );
            names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
          }
        }

        /* Spectral Attributes */
        if ( need_rasters_polygons_features )
        {
          for ( unsigned b = 0; b < bands.size(); b++ )
          {
            string amplitude_name = "rp_amplitude_" + Te2String( b );
            string entropy_name = "rp_entropy_" + Te2String( b );
            string mean_name = "rp_mean_" + Te2String( b );
            string mode_name = "rp_mode_" + Te2String( b );
            string stddev_name = "rp_stddev_" + Te2String( b );
            string sum_name = "rp_sum_" + Te2String( b );

            string dissimilarity_name = "rp_dissimilarity_" + Te2String( b );
            string entropy2_name = "rp_entropy2_" + Te2String( b );
            string homogeneity_name = "rp_homogeneity_" + Te2String( b );

            if ( q_lb_attributes_extract->text( j ).compare( QString( amplitude_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( rasters_attributes.getMax( b, p ) - rasters_attributes.getMin( b, p ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
            else if ( q_lb_attributes_extract->text( j ).compare( QString( entropy_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( rasters_attributes.getEntropy( b, p ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
            else if ( q_lb_attributes_extract->text( j ).compare( QString( mean_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( rasters_attributes.getMean( b, p ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
            else if ( q_lb_attributes_extract->text( j ).compare( QString( mode_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( rasters_attributes.getMode( b, p ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
            else if ( q_lb_attributes_extract->text( j ).compare( QString( stddev_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( rasters_attributes.getStdDev( b, p ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
            else if ( q_lb_attributes_extract->text( j ).compare( QString( sum_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( rasters_attributes.getSum( b, p ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }

            else if ( q_lb_attributes_extract->text( j ).compare( QString( dissimilarity_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( texture_properties.getDissimilarity( b, p ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
            else if ( q_lb_attributes_extract->text( j ).compare( QString( entropy2_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( texture_properties.getEntropy( b, p ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
            else if ( q_lb_attributes_extract->text( j ).compare( QString( homogeneity_name.c_str() ) ) == 0 )
            {
              extracted_attributes.push_back( texture_properties.getHomogeneity( b, p ) );
              names_attributes.push_back( q_lb_attributes_extract->text( j ).ascii() );
            }
          }
        }
      }

      /* Storing attributes in table */
      polygons_real_index = -1;
      for ( unsigned i = 0; i < polygons_table.size(); i++ )
        if ( polygons_table( i, polygons_primary_index ).c_str() == myPolygons[ p ].objectId() )
        {
          polygons_real_index = i;
          break;
        }

      if ( polygons_real_index < 0 )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find attribute index" ) );
        return;
      }

      if ( !execute_query( create_update_query( polygons_attribute_table_name, polygons_primary_key, Te2String( polygons_real_index ), names_attributes, extracted_attributes) ) )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not update table with polygon information:\n" ) + curr_database->errorMessage().c_str() );
        return;
      }
    }

    polygons_status.Toggle( false );
  }

  /* point_set extraction */
  if ( using_points && need_mt_features )
  {
    TeLayer* points_layer = get_points_layer();
    /* Creating attributes columns in points table */
    vector<string> points_features_names;
    vector<unsigned> points_features_indexes;
    TeTable points_table;
    for ( unsigned i = 0; i < q_lb_attributes_extract->count(); i++ )
    {
      if ( q_lb_attributes_extract->text( i ).left( 3 ).compare( QString( "mt_" ) ) == 0 )
        points_features_names.push_back( q_lb_attributes_extract->text( i ).section( "_", 1 ).ascii() );
      else
        continue;
      updated_points_table = true;
    }

    if ( !points_layer->getAttrTablesByName( points_attribute_table_name, points_table, TeAllAttrTypes ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the points theme attribute table" ) );
      return;
    }

    /* Creating attributes columns in points table */
    TePDIPIManager table_status( tr( "Creating attributes in table..." ).ascii(), points_features_names.size(), true );
    for ( unsigned i = 0; i < points_features_names.size(); i++ )
    {
      table_status.Increment();
      if ( !points_layer->getAttrTablesByName( points_attribute_table_name, points_table, TeAllAttrTypes ) )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the points theme attribute table" ) );
        return;
      }

      for ( unsigned t = mt_firstLabel; t < ( mt_firstLabel + ( myRaster->nBands() - mt_startingPoint ) / mt_pointsPerCurve ); t += mt_stepLabels )
      {
        /* Creating new attribute */
        TeAttribute new_attr;
        new_attr.rep_.name_ = "mt_" + Te2String( t ) + "_" + points_features_names[ i ];
        new_attr.rep_.type_ = TeREAL;
        new_attr.rep_.null_ = true;
        new_attr.rep_.numChar_ = 15;
        new_attr.rep_.decimals_ = 6;

        if ( !curr_database->columnExist( points_attribute_table_name, new_attr.rep_.name_, new_attr ) )
          if ( !curr_database->addColumn( points_attribute_table_name, new_attr.rep_ ) )
          {
            QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not create the new column.\n" ) + curr_database->errorMessage().c_str() ); 
            curr_database->rollbackTransaction();
            return;
          }
      }
    }
    table_status.Toggle( false );

    /* Finding attributes indexes in points table */
    for ( unsigned i = 0; i < points_features_names.size(); i++ )
    {
      for ( unsigned t = mt_firstLabel; t < ( mt_firstLabel + ( myRaster->nBands() - mt_startingPoint ) / mt_pointsPerCurve ); t += mt_stepLabels )
      {
        int attr_index = find_index( "mt_" + Te2String( t ) + "_" + points_features_names[ i ], points_layer, points_attribute_table_name );
        if ( attr_index < 0 )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find point Attribute Index" ) );
          return;
        }
        points_features_indexes.push_back( attr_index );
      }
    }
    points_table.clear();
    if ( !curr_database->loadTable( points_attribute_table_name, points_table ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the point theme attribute table" ) );
      return;
    }

    /* Getting primary key index in points table */
    int points_real_index = -1;
    string points_primary_key;
    TeAttributeList& points_attr_list = points_table.attributeList();
    TeAttributeList::iterator points_attr_list_it = points_attr_list.begin();

    while ( points_attr_list_it != points_attr_list.end() )
    {
      if ( points_attr_list_it->rep_.isPrimaryKey_ )
      {
        points_primary_key = points_attr_list_it->rep_.name_;
        break;
      }
      ++points_attr_list_it;
    }

    int points_primary_index = find_index( points_primary_key, points_layer, points_attribute_table_name );
    if ( points_primary_index < 0 )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find points primary key index" ) );
      return;
    }

    /* Defining Points Attributes */
    //TePointSet point_set;
    //get_points_layer()->getPoints( point_set );

    /* Extracting features from points */
    TePDIPIManager point_status( tr( "Extracting points features..." ).ascii(), myPoints.size(), true );
    for ( unsigned i = 0; i < myPoints.size(); i++ )
    {
      point_status.Increment();

      /* Get spectral time series i-th from point, and extract attributes */
      TeCoord2D cl = myRaster->coord2Index( myPoints[ i ].location() );
      double pixel;

      for ( unsigned t = mt_firstLabel; t < ( mt_firstLabel+ ( myRaster->nBands() - mt_startingPoint ) / mt_pointsPerCurve ); t += mt_stepLabels )
      {
        vector<double> extracted_features;
        vector<string> names_features;
        vector<double> ts;

        unsigned band = mt_startingPoint + ( t - mt_firstLabel ) * mt_pointsPerCurve;
        for ( unsigned b = 0; b < mt_pointsPerCurve; b++)
        {
          myRaster->getElement( cl.x(), cl.y(), pixel, band++ );
          ts.push_back( pixel );
        }
        for ( unsigned a = 0; a < points_features_names.size(); a++)
        {
          extracted_features.push_back( get_feature( "mt_" + points_features_names[ a ], ts ) );
          names_features.push_back( "mt_" + Te2String(t) + "_" + points_features_names[ a ] );
        }

        /* Storing attributes in table */
        points_real_index = -1;
        for ( unsigned k = 0; k < points_table.size(); k++ )
          if ( points_table( k, points_primary_index ).c_str() == myPoints[ i ].objectId() )
          {
            points_real_index = k;
            break;
          }
        if ( points_real_index < 0 )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find points attribute index" ) );
          curr_database->rollbackTransaction();
          return;
        }

        if ( !execute_query( create_update_query( points_attribute_table_name, points_primary_key, Te2String( points_real_index ), names_features, extracted_features ) ) )
        {
          curr_database->rollbackTransaction();
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not update table with polygon information:\n" ) + curr_database->errorMessage().c_str() );
          return;
        }
      }
    }

    point_status.Toggle( false );
  }

  /* All done */
  update_TV_interface();

  refresh_tables();
  q_tab->setCurrentPage( get_tab_number( "Features" ) );
  QMessageBox::information( this, function_name( __FUNCTION__ ), tr( "Feature extraction done!" ) );

  #ifdef _DEBUG
  debug.finish( "q_pb_extract_attributes_clicked()" );
  #endif
}

/* ========== NORMALIZATION ========== */
void geodma_window::q_pb_normalize_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_pb_normalize_clicked()" );
  #endif

  if ( !q_lb_attributes_norm->count() )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Select at least one item to normalize!" ) );
    return;
  }

  /* i == 0 for Polygons and i == 1 for Cells */
  vector<string> object_types;
  object_types.push_back( "Polygons" );
  object_types.push_back( "Cells" );
  map<string, vector<string> > object_suffixes;
  object_suffixes[ "Polygons" ].push_back( "p_" );
  object_suffixes[ "Polygons" ].push_back( "rp_" );
  object_suffixes[ "Cells" ].push_back( "c_" );
  object_suffixes[ "Cells" ].push_back( "rc_" );

  for ( unsigned o = 0; o < 2; o++ )
  {
    unlock_grid();
    TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
    curr_database->beginTransaction();
    /* Creating the normalized columns with normalized data */
    TeLayer* layer_choice;
    TeTable table_choice;
    string attribute_table_name;

    if ( object_types[ o ] == "Polygons" )
    {
      layer_choice = get_polygons_layer();
      attribute_table_name = polygons_attribute_table_name;
    }
    else
    {
      layer_choice = get_cells_layer();
      attribute_table_name = cells_attribute_table_name;
    }

    TePDIPIManager status( tr( "Normalizing " ).ascii() + object_types[ o ] + tr( " data..." ).ascii(), q_lb_attributes_norm->count(), true );
    for ( unsigned i = 0; i < q_lb_attributes_norm->count(); i++ )
    {
      status.Increment();
      if ( ( q_lb_attributes_norm->text( i ).left( 2 ).compare( QString( object_suffixes[ object_types[ o ] ][ 0 ].c_str() ) ) != 0 ) &&
           ( q_lb_attributes_norm->text( i ).left( 3 ).compare( QString( object_suffixes[ object_types[ o ] ][ 1 ].c_str() ) ) != 0 ) )
        continue;

      if ( !layer_choice->getAttrTablesByName( attribute_table_name, table_choice, TeAllAttrTypes ) ) 
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) ); 
        return;
      }

      /* Adding Columns */
      TeAttribute new_attr;
      QString norm_name = q_lb_attributes_norm->item( i )->text() + "_n";
      new_attr.rep_.name_ = norm_name.ascii();
      new_attr.rep_.type_ = TeREAL;
      new_attr.rep_.decimals_ = 6;
      new_attr.rep_.numChar_ = 15;
      new_attr.rep_.null_ = true;

      if ( !curr_database->columnExist( attribute_table_name, new_attr.rep_.name_, new_attr ) ) 
        if ( !curr_database->addColumn( attribute_table_name, new_attr.rep_ ) ) 
        {
          curr_database->rollbackTransaction();
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not create the new column:\n" ) + curr_database->errorMessage().c_str() );
          return;
        }

      /* Finding parameters for Normalization - MAX, MIN and MEAN for each column */
      float MAX = 0.0, MIN = 0.0, MEAN = 0.0;

      TeGroupingAttr norm_attributes;
      pair<TeAttributeRep, TeStatisticType> norm_max( TeAttributeRep( q_lb_attributes_norm->item( i )->text().ascii() ), TeMAXVALUE );
      norm_attributes.push_back( norm_max );
      pair<TeAttributeRep, TeStatisticType> norm_min( TeAttributeRep( q_lb_attributes_norm->item( i )->text().ascii() ), TeMINVALUE );
      norm_attributes.push_back( norm_min );
      pair<TeAttributeRep, TeStatisticType> norm_mean( TeAttributeRep( q_lb_attributes_norm->item( i )->text().ascii() ), TeMEAN );
      norm_attributes.push_back( norm_mean );
      TeDatabasePortal* portal = curr_database->getPortal();

      QString sql = "SELECT ";
      sql.append( curr_database->getSQLStatistics( norm_attributes ).c_str() );
      sql += " FROM ";
      sql += attribute_table_name.c_str();

      if ( !portal )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not open portal" ) ); 
        return;
      }
      if ( !portal->query( sql.ascii() ) )
      {
        delete portal;
        curr_database->rollbackTransaction();
        QString message;
        message.sprintf( "Could not execute query\n %s", sql.latin1() );
        QMessageBox::critical( this, function_name( __FUNCTION__ ), message ); 
        return;
      }
      TeAttributeList list = portal->getAttributeList();
      if ( portal->fetchRow() )
      {
        MAX = atof( portal->getData( 0 ) );
        MIN = atof( portal->getData( 1 ) );
        MEAN = atof( portal->getData( 2 ) );
      }
      portal->freeResult();

      /* Finding attributes indexes */
      int attr_index = find_index ( q_lb_attributes_norm->item( i )->text().ascii(), layer_choice, attribute_table_name ),
          attr_norm_index = find_index( norm_name.ascii(), layer_choice, attribute_table_name ); 
      if ( attr_index < 0 || attr_norm_index < 0 )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find Attribute Index!" ) );
        return;
      }

      /* Calculating Numerator and Denominator for normalization */
      float NUM = 0.0,
      DEN = 0.0;
      if ( q_rb_normal->isChecked() )
      {
        NUM = MIN;
        DEN = MAX - MIN;
      }
      else if ( q_rb_mean->isChecked() )
      {
        NUM = MEAN;
        DEN = MEAN - MIN;
      }
      if ( DEN == 0.0 )
        DEN = ( float ) 0.0000000001;

      /* Performing Normalization */
      table_choice.clear();
      if ( !curr_database->loadTable( attribute_table_name, table_choice ) ) 
      {
        curr_database->rollbackTransaction();
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) );
        return;
      }

      for ( unsigned j = 0; j < table_choice.size(); j++ )
      {
        string norm_value = table_choice( j, attr_index );
        float f_norm_value = ( atof( norm_value.c_str() ) - NUM ) / DEN;
        string s_norm_value = Te2String( f_norm_value );
        table_choice.setValue( j, attr_norm_index, s_norm_value );
      }

      if ( !curr_database->updateTable( table_choice ) ) 
      {
        curr_database->rollbackTransaction();
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to update attribute table" ) );
        return;
      }
    }
    status.Toggle( false );
    curr_database->commitTransaction();
  }

  /* All done */
  update_TV_interface();
  refresh_tables();
  q_tab->setCurrentPage( get_tab_number( "Normalize" ) );
  QMessageBox::information( this, function_name( __FUNCTION__ ), tr( "Normalization done!" ) );

  #ifdef _DEBUG
  debug.finish( "q_pb_normalize_clicked()" );
  #endif
}

/* ========== TRAINING ========== */
void geodma_window::q_pb_create_class_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_pb_create_class_clicked( " + string( q_le_new_class_name->text().ascii() )  + " )" );
  #endif

  // unlock_grid();

  if ( q_le_new_class_name->text().isEmpty() )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "First set the New Class name" ) );
    return;
  }
  if ( q_lb_training_classes->findItem( q_le_new_class_name->text(), Qt::ExactMatch ) != 0 )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Class Name already exists" ) );
    return;
  }

  /* Inserting class into database table */
  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  TeTable training_table;
  if ( !curr_database->loadTable( "training_table", training_table ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) );
    return;
  }

  geodma_colors c;
  int color_item = q_cb_new_class_color->currentItem();
  QColor color ( c.get_R( color_item ), c.get_G( color_item ), c.get_B( color_item ) );
  int valid_id = 0;
  for ( unsigned i = 0; i < training_table.size(); i++ )
  {
    int id = atoi( training_table( i, 0 ).c_str() );
    valid_id = ( id > valid_id ? id : valid_id );
  }
  ++valid_id;

  /* Inserting new class into database */
  string query = "INSERT INTO training_table ( id, training_class, class_r, class_g, class_b, is_active ) VALUES ( ";
  query += Te2String( valid_id ) + ", '";
  query += q_le_new_class_name->text().ascii();
  query += "', ";
  query += Te2String( c.get_R( color_item ) ) + ", ";
  query += Te2String( c.get_G( color_item ) ) + ", ";
  query += Te2String( c.get_B( color_item ) ) + ", 1 );";

  if ( !execute_query( query ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not insert the new class into database:\n" ) + curr_database->errorMessage().c_str() );
    return;
  }

  /* Inserting class name into list box */
  QPixmap item( 15, 15 );
  item.fill( color );
  q_lb_training_classes->insertItem( item, q_le_new_class_name->text() );
  q_lb_training_classes->sort();
  q_le_new_class_name->clear();

  #ifdef _DEBUG
  debug.finish( "q_pb_create_class_clicked()" );
  #endif
}

void geodma_window::q_pb_remove_class_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_pb_remove_class_clicked()" );
  #endif

  if ( q_lb_training_classes->currentText().isEmpty() )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "First select one Class to remove" ) );
    return;
  }

  string class_name = q_lb_training_classes->currentText();

  /* Load database and table information */
  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  curr_database->beginTransaction();
  TeTable training_table;
  if ( !curr_database->loadTable( "training_table", training_table ) )
  {
    curr_database->rollbackTransaction();
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) );
    return;
  }

  for ( unsigned i = 0; i < training_table.size(); i++ )
    if ( training_table( i, 1 ) == class_name && training_table( i, 5 ) == "1" )
    {
      string v;
      v = "0";
      training_table.setValue( i, 5, v );
      break;
    }

  training_table.setUniqueName( "id" );
  if ( !curr_database->updateTable( training_table ) )
  {
    curr_database->rollbackTransaction();
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not remove training_class table:\n" ) + curr_database->errorMessage().c_str() );
    return;
  }
  q_lb_training_classes->removeItem( q_lb_training_classes->currentItem() );
  curr_database->commitTransaction();

  #ifdef _DEBUG
  debug.finish( "q_pb_remove_class_clicked()" );
  #endif
}


void geodma_window::q_pb_edit_class_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_pb_edit_class_clicked()" );
  #endif

  /* Get selected class name */
  q_le_new_class_name->clear();
  q_le_new_class_name->setText( q_lb_training_classes->currentText() );

  /* Get selected class color */
  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  TeTable training_table;
  if ( !curr_database->loadTable( "training_table", training_table ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not load the theme attribute table:\n" ) + curr_database->errorMessage().c_str() );
    return;
  }

  for ( unsigned i = 0; i < training_table.size(); i++ )
  {
    string selected_class = q_lb_training_classes->currentText().ascii();
    string current_class = training_table( i, 1 );

    if ( current_class == selected_class && training_table( i, 5 ) == "1" )
    {
      int R = atoi( training_table( i, 2 ).c_str() ),
          G = atoi( training_table( i, 3 ).c_str() ),
          B = atoi( training_table( i, 4 ).c_str() );
      geodma_colors C;
      for ( unsigned j = 0; j < C.get_size(); j++ )
      {
        if ( R == C.get_R( j ) && G == C.get_G( j ) && B == C.get_B( j ) )
        {
          string name = "Color_" + Te2String( j );
          q_cb_new_class_color->setCurrentText( QString( name.c_str() ) );
          j = C.get_size();
        }
      }

      break;
    }
  }

  q_pb_remove_class_clicked();

  #ifdef _DEBUG
  debug.finish( "q_pb_edit_class_clicked()" );
  #endif
}

void geodma_window::q_pb_associate_class_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_pb_associate_class_clicked()" );
  #endif

  if ( q_lb_training_classes->currentText().isEmpty() )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "First select one class" ) );
    return;
  }

  /* Load database, layer and theme information */
  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  curr_database->beginTransaction();

  TeLayer* layer_choice;
  string attribute_table_name;
  TeAppTheme* app_theme = plug_params_ptr_->getCurrentThemeAppPtr();
  TeTheme* theme_choice = ( TeTheme* ) app_theme->getTheme();
  if ( theme_choice->layer()->hasGeometry( TePOLYGONS ) )
  {
    attribute_table_name = polygons_attribute_table_name;
    layer_choice = get_polygons_layer();
  }
  else if ( theme_choice->layer()->hasGeometry( TeCELLS ) )
  {
    attribute_table_name = cells_attribute_table_name;
    layer_choice = get_cells_layer();
  }
  else if ( theme_choice->layer()->hasGeometry( TePOINTS ) )
  {
    attribute_table_name = points_attribute_table_name;
    layer_choice = get_points_layer();
  }
  else
  {
    curr_database->rollbackTransaction();
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Current Theme doesn't have points, cells nor polygons" ) );
    return;
  }

  if ( !theme_choice )
  {
    curr_database->rollbackTransaction();
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Invalid or Unselected Theme" ) );
    return;
  }

  if ( theme_choice->layer() != layer_choice )
  {
    curr_database->rollbackTransaction();
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Choose one Theme from the selected Layer" ) );
    return;
  }

  /* Get the selected objects */
  set<string> selected_objs_ids = theme_choice->getObjects( TeSelectedByPointing );
  if ( selected_objs_ids.size() < 1 )
  {
    curr_database->rollbackTransaction();
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Select at least one object" ) );
    return;
  }

  /* Finding attribute index */
  vector<int> attr_index;
  attr_index.push_back( find_index ( "training_class", layer_choice, attribute_table_name ) );
  attr_index.push_back( find_index ( "validation_class", layer_choice, attribute_table_name ) );
  if ( attr_index[0] < 0 || attr_index[1] < 0 )
  {
    curr_database->rollbackTransaction();
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find Attribute Index" ) );
    return;
  }

  TePDIPIManager associate_status( tr( "Associating objects with training/validation classes..." ).ascii(), selected_objs_ids.size(), true ) ;

  /* Set the selected class to the objects */
  TeTable attribute_table;
  if ( !curr_database->loadTable( attribute_table_name, attribute_table ) )
  {
    curr_database->rollbackTransaction();
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) );
    return;
  }

  set<string>::iterator selected_objs_ids_it = selected_objs_ids.begin();
  unsigned objs_counter = 0;
  while ( selected_objs_ids_it != selected_objs_ids.end() )
  {
    int object_id = -1;
    associate_status.Increment();
    for ( unsigned i = 0; i < attribute_table.size(); i++ )
    {
      TeTableRow row = attribute_table[ i ];
      if ( row[ attribute_table.attrUniquePosition() ] == ( *selected_objs_ids_it ) )
      {
        object_id = i;
        continue;
      }
    }

    if ( object_id == -1 )
    {
      curr_database->rollbackTransaction();
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not find selected object in database" ) );
      return;
    }

    unsigned col_i = 0;
    string col_name = "training_class";
    if ( objs_counter++ > ( 0.6 * (selected_objs_ids.size() - 1) ) )
    {
      col_i = 1;
      col_name = "validation_class";
    }

    string class_name = q_lb_training_classes->currentText().ascii();
    attribute_table.setValue( object_id, attr_index[col_i], class_name );

    /* Executing query to update line */
    string query = "UPDATE ";
    query += attribute_table_name;
    query += " SET ";
    query += col_name;
    query += " = '";
    query += class_name;
    query += "' WHERE ";
    query += attribute_table.uniqueName();
    query += " = '";
    query += attribute_table( object_id, attribute_table.attrUniquePosition() );
    query += "'";

    if ( !execute_query( query ) )
    {
      curr_database->rollbackTransaction();
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not associate class:\n" ) + curr_database->errorMessage().c_str() );
      return;
    }

    ++selected_objs_ids_it;
  }

  associate_status.Toggle( false );
  refresh_tables();
  q_tab->setCurrentPage( get_tab_number( "Train" ) );
  curr_database->commitTransaction();
  QMessageBox::information( this, function_name( __FUNCTION__ ), tr( "Class associated!" ) );
 
  plug_params_ptr_->teqtgrid_ptr_->removePointingColorSlot();
  plug_params_ptr_->teqtgrid_ptr_->removeQueryColorSlot();

  #ifdef _DEBUG
  debug.finish( "q_pb_associate_class_clicked()" );
  #endif
}

/* ========== VISUALIZATION ========== */
void geodma_window::initialize_frames()
{
  #ifdef _DEBUG
  debug.start( "initialize_frames()" );
  #endif

  /* Scatterplot */
  myScatterplot = new QwtPlot( QwtText( "" ), q_frame );
  myScatterplot->setAxisFont( QwtPlot::xBottom, QFont( "helvetica", 7 ) );
  myScatterplot->setAxisFont( QwtPlot::yLeft, QFont( "helvetica", 7 ) );
  myScatterplot->setGeometry( 5, 5, q_frame->frameGeometry().width() - 10, q_frame->frameGeometry().height() - 10 );

  myScatterPoints = new QwtPlotCurve( QwtText( "" ) );
  myScatterPoints->attach( myScatterplot );

  myScatterplot->show();

  /* Time Series Plot */
  myTimeSeriesPlot = new QwtPlot( QwtText( "" ), q_fr_timeseries_mt );
  myTimeSeriesPlot->setAxisFont( QwtPlot::xBottom, QFont( "helvetica", 7 ) );
  myTimeSeriesPlot->setAxisFont( QwtPlot::yLeft, QFont( "helvetica", 7 ) );
  myTimeSeriesPlot->setAxisScale( QwtPlot::yLeft, 0, 1 );
  myTimeSeriesPlot->setAxisScale( QwtPlot::xBottom, 0, 1 );
  myTimeSeriesPlot->setGeometry( 0, 0, q_fr_timeseries_mt->frameGeometry().width(), q_fr_timeseries_mt->frameGeometry().height() );

  QwtSymbol symbol_ts;
  symbol_ts.setStyle( QwtSymbol::NoSymbol );
  symbol_ts.setPen( QPen( QColor( 0, 0, 0 ) ) );
  symbol_ts.setSize( 4 );
  symbol_ts.setBrush( QBrush( QColor( 0, 0, 0 ) ) );

  myTimeSeriesPoints = new QwtPlotCurve( QwtText( "" ) );
  myTimeSeriesPoints->setSymbol( symbol_ts );
  myTimeSeriesPoints->setStyle( QwtPlotCurve::Lines );
  myTimeSeriesPoints->attach( myTimeSeriesPlot );

  myTimeSeriesPlot->show();

  /* Polar Plot */
  myPolarPlot = new QwtPlot( QwtText( "" ), q_fr_polarvisualization_mt );
  myPolarPlot->setAxisFont( QwtPlot::xBottom, QFont( "helvetica", 7 ) );
  myPolarPlot->setAxisFont( QwtPlot::yLeft, QFont( "helvetica", 7 ) );
  myPolarPlot->setAxisScale( QwtPlot::yLeft, -1, 1 );
  myPolarPlot->setAxisScale( QwtPlot::xBottom, -1, 1 );
  myPolarPlot->setGeometry( 0, 0, q_fr_polarvisualization_mt->frameGeometry().width(), q_fr_polarvisualization_mt->frameGeometry().height() );

  QwtSymbol symbol_polar;
  symbol_polar.setStyle( QwtSymbol::NoSymbol );
  symbol_polar.setPen( QPen( QColor( 0, 0, 0 ) ) );
  symbol_polar.setSize( 4 );
  symbol_polar.setBrush( QBrush( QColor( 0, 0, 0 ) ) );

  myPolarPoints = new QwtPlotCurve( QwtText( "" ) );
  myPolarPoints->setSymbol( symbol_polar );
  myPolarPoints->setStyle( QwtPlotCurve::Lines );
  myPolarPoints->attach( myPolarPlot );

  myPolarPlot->show();

  #ifdef _DEBUG
  debug.start( "initialize_frames()" );
  #endif
}

void geodma_window::clear_frames()
{
  #ifdef _DEBUG
  debug.start( "clear_frames()" );
  #endif

  myScatterplot->erase();
  myTimeSeriesPlot->erase();
  myPolarPlot->erase();

  #ifdef _DEBUG
  debug.finish( "clear_frames()" );
  #endif
}

void geodma_window::q_pb_plot_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_pb_plot_clicked()" );
  #endif
  // unlock_grid();

  string attribute_table_name;
  TeTable table_choice;
  TeLayer *layer_choice;

  if ( q_cb_visualizing_what->currentText() == "Polygons" )
  {
    attribute_table_name = polygons_attribute_table_name;
    layer_choice = get_polygons_layer();
  }
  else if ( q_cb_visualizing_what->currentText() == "Cells" )
  {
    attribute_table_name = cells_attribute_table_name;
    layer_choice = get_cells_layer();
  }
  else // Points
  {
    attribute_table_name = points_attribute_table_name;
    layer_choice = get_points_layer();
  }

  if ( q_cb_xaxis->currentText().length() == 0 || q_cb_yaxis->currentText().length() == 0 )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Couldn't find axis" ) );
    return;
  }

  /* Finding attributes indexes */
  int attr_xindex = find_index( q_cb_xaxis->currentText().ascii(), layer_choice, attribute_table_name ),
      attr_yindex = find_index( q_cb_yaxis->currentText().ascii(), layer_choice, attribute_table_name );

  if ( attr_xindex < 0 || attr_yindex < 0 )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find Attribute Index" ) );
    return;
  }

  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  if ( !curr_database->loadTable( attribute_table_name, table_choice ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table:\n" ) + curr_database->errorMessage().c_str() );
    return;
  }
  TeTable training_table;
  if ( !curr_database->loadTable( "training_table", training_table ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table:\n" ) + curr_database->errorMessage().c_str() );
    return;
  }

  vector<double> vx, vy;
  vector<string> labels;
  set<string> labels_names;
  unsigned srate = 1,
           total_values = 0;
  if ( table_choice.size() > ( unsigned ) q_sb_srate->value() )
    srate = q_sb_srate->value();
  for ( unsigned j = 0; j < table_choice.size(); j += srate )
  {
    string xvalue = table_choice( j, attr_xindex ),
           yvalue = table_choice( j, attr_yindex );
    vx.push_back( atof( xvalue.c_str() ) );
    vy.push_back( atof( yvalue.c_str() ) );
    ++total_values;
  }

  if ( q_cb_labels->currentText() != "No Labels" )
  {
    int label_index = find_index( q_cb_labels->currentText().ascii(), layer_choice, attribute_table_name  );
    if ( label_index < 0 )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find Label Index" ) );
      return;
    }

    for ( unsigned j = 0; j < table_choice.size(); j += srate )
    {
      string label = table_choice( j, label_index );
      if ( label == "" )
        label = "No Label";
      labels.push_back( label );
      labels_names.insert( label );
    }
  }
  else
  {
    labels_names.insert( "No Label" );
    for ( unsigned j = 0; j < table_choice.size(); j += srate )
      labels.push_back( "No Label" );
  }

  set<string>::iterator labels_it = labels_names.begin();
  vector<string> labels_unique;
  while ( labels_it != labels_names.end() )
  {
    labels_unique.push_back( *labels_it );
    ++labels_it;
  }

  geodma_colors C;
  double *x = new double[ total_values ];
  double *y = new double[ total_values ];
  for ( unsigned i = 0; i < labels_unique.size(); i++ )
  {
    int total_values_label = 0;

//    if ( myScatterPoints )
//      delete myScatterPoints;
//    myScatterPoints = new QwtPlotCurve( QwtText( labels_unique[ i ].c_str() ) );
    myScatterPoints->setTitle( labels_unique[ i ].c_str() );

    /* Get data to plot curves */
    for ( unsigned j = 0, k = 0; j < total_values; j++ )
      if ( labels[ j ] == labels_unique[ i ] )
      {
        x[k] = vx[ j ];
        y[k] = vy[ j ];
        ++k;
        ++total_values_label;
      }

    int R = C.get_R( i ),
        G = C.get_G( i ),
        B = C.get_B( i );
    for ( unsigned j = 0; j < training_table.size(); j++ )
    {
      string class_name = training_table( j, 1 );
      if ( class_name == labels_unique[ i ] && training_table( j, 5 ) == "1" )
      {
        R = atoi( training_table( j, 2 ).c_str() ),
        G = atoi( training_table( j, 3 ).c_str() ),
        B = atoi( training_table( j, 4 ).c_str() );
      }
    }

    QColor color( R, G, B );
    QwtSymbol symbol;
    symbol.setStyle( QwtSymbol::Ellipse );
    symbol.setPen( QPen( color ) );
    symbol.setSize( 4 );
    symbol.setBrush( QBrush( color ) );
    myScatterPoints->setSymbol( symbol );
    myScatterPoints->setStyle( QwtPlotCurve::Dots );
    myScatterPoints->setData( x, y, total_values_label );
    myScatterPoints->attach( myScatterplot );
  }
  delete [] x;
  delete [] y;

  /* Draw the scatterplot */
  QwtLegend *legends = new QwtLegend();
  legends->setDisplayPolicy( QwtLegend::FixedIdentifier, QwtLegendItem::ShowSymbol | QwtLegendItem::ShowText );
  myScatterplot->setAxisFont( QwtPlot::xBottom, QFont( "helvetica", 7 ) );
  myScatterplot->setAxisFont( QwtPlot::yLeft, QFont( "helvetica", 7 ) );
  myScatterplot->insertLegend( legends, QwtPlot::BottomLegend );
  myScatterplot->setGeometry( 5, 5, q_frame->frameGeometry().width() - 10, q_frame->frameGeometry().height() - 10 );
  myScatterplot->show();
  myScatterplot->replot();

  #ifdef _DEBUG
  debug.finish( "q_pb_plot_clicked()" );
  #endif
}

void geodma_window::q_pb_spatialize_attribute_y_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_pb_spatialize_attribute_y_clicked()" );
  #endif
  // unlock_grid();

  string attribute_table_name;
  TeTable table_choice;
  TeLayer *layer_choice;

  if ( q_cb_visualizing_what->currentText() == "Polygons" )
  {
    attribute_table_name = polygons_attribute_table_name;
    layer_choice = get_polygons_layer();
  }
  else if ( q_cb_visualizing_what->currentText() == "Cells" )
  {
    attribute_table_name = cells_attribute_table_name;
    layer_choice = get_cells_layer();
  }
  else // Points
  {
    attribute_table_name = points_attribute_table_name;
    layer_choice = get_points_layer();
  }

  if ( q_cb_yaxis->currentText().length() == 0 )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Couldn't find axis" ) );
    return;
  }

  /* Finding attributes indexes */
  int attr_yindex = find_index( q_cb_yaxis->currentText().ascii(), layer_choice, attribute_table_name  );

  if ( attr_yindex < 0 )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find Attribute Index" ) );
    return;
  }

  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  if ( !curr_database->loadTable( attribute_table_name, table_choice ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table:\n" ) + curr_database->errorMessage().c_str() );
    return;
  }

  /* Creating a new View */
  TeProjection* new_view_proj = TeProjectionFactory::make( layer_choice->projection()->params() );
  TeView* new_view = new TeView ( layer_choice->name(), curr_database->user() );
  new_view->name( "Spatialization" );
  new_view->setCurrentBox( layer_choice->box() );
  new_view->setCurrentTheme( -1 );
  new_view->projection( new_view_proj );
  if ( !curr_database->insertView( new_view ) )
  {
    delete new_view;
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to insert the new view into database" ) );
    return;
  }

  /* Creating a new Theme */
  TeTheme* new_theme = new TeTheme( string( q_cb_yaxis->currentText().ascii() )/*layer_choice->name()*/, layer_choice );
  if ( !new_theme )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Invalid Theme pointer" ) );
    return;
  }

  new_theme->name( string( q_cb_yaxis->currentText().ascii() ) );
  new_view->add( new_theme );

  TeVisual* new_visual = new TeVisual( TePOLYGONS );
  TeColor new_color,
  contour_color;
  new_color.init( 255, 255, 255 );
  contour_color.init( 0, 0, 0 );
  new_visual->color( new_color ); 
  new_visual->contourColor( contour_color );
  new_visual->contourStyle( TeLnTypeNone );
  new_visual->transparency( 0 );
  new_theme->setVisualDefault( new_visual, TePOLYGONS );

  TeVisual* point_visual = new TeVisual( TePOINTS );
  new_color.init( 255, 255, 255 );
  point_visual->color( new_color );
  point_visual->style( TePtTypeX );
  point_visual->contourColor( contour_color );
  new_theme->setVisualDefault( point_visual, TePOINTS );

  int allRep = layer_choice->geomRep();
  new_theme->visibleRep( allRep );
  new_theme->setAttTables( layer_choice->attrTables() );

  if ( !new_theme->save() )
  {
    delete new_theme;
    curr_database->deleteView( new_view->id() );
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to create the new theme" ) );
    return;
  }

  if ( !new_theme->buildCollection() )
  {
    delete new_theme;
    curr_database->deleteView( new_view->id() );
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to build the new theme collection" ) );
    return;
  }

  /* Setting attribute to spatialize */
  TeAttributeRep attribute_rep;
  attribute_rep.name_ = string( q_cb_yaxis->currentText().ascii() );
  attribute_rep.type_ = TeREAL;

  /* Creating Grouping */
  TeGrouping spatialization;
  spatialization.groupMode_ = TeEqualSteps;
  spatialization.groupNumSlices_ = 100;
  spatialization.groupAttribute_ = attribute_rep;

  TePDIPIManager status( tr( "Spatializing feature..." ).ascii(), spatialization.groupNumSlices_ + 1, true );
  status.Increment();

  if ( !new_theme->buildGrouping( spatialization ) )
  {
    delete new_theme;
    curr_database->deleteView( new_view->id() );
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to build Legend Grouping" ) );
    return;
  }

  /* Assigning to each slice one color */
  for ( unsigned slice_color = 0; slice_color < (unsigned) new_theme->grouping().groupNumSlices_; slice_color++ )
  {
    status.Increment();
    int new_color = ( slice_color / (double) spatialization.groupNumSlices_ ) * 255;
    TeColor color( new_color, new_color, new_color );
    TeVisual* visual = TeVisualFactory::make( "tevisual" );
    visual->color( color );
    visual->transparency( new_theme->defaultLegend().visual( TePOLYGONS )->transparency() );
    visual->contourStyle( new_theme->defaultLegend().visual( TePOLYGONS )->contourStyle() );
    visual->contourWidth( new_theme->defaultLegend().visual( TePOLYGONS )->contourWidth() );
    visual->contourColor( new_theme->defaultLegend().visual( TePOLYGONS )->contourColor() );
    new_theme->setGroupingVisual( slice_color + 1, visual, TePOLYGONS );
  }
  status.Toggle( false );


  /* Saving the theme legend to the database */
  if ( !new_theme->saveGrouping() )
  {
    delete new_theme;
    curr_database->deleteView( new_view->id() );
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to save Legend Grouping" ) );
    return;
  }

  if ( !new_theme->saveLegendInCollection() )
  {
    delete new_theme;
    curr_database->deleteView( new_view->id() );
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to save Legend in Collection" ) );
    return;
  }

  /* All done */
  update_TV_interface();
  QMessageBox::information( this, function_name( __FUNCTION__ ), tr( "Spatialization done!" ) );

  delete new_view;
  delete new_theme;
  delete new_visual;
  delete point_visual;

  #ifdef _DEBUG
  debug.finish( "q_pb_spatialize_attribute_y_clicked()" );
  #endif
}

/* ========== CLASSIFICATION ========== */
void geodma_window::q_pb_load_c45_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_pb_load_c45_clicked()" );
  #endif

  QFileDialog *q_fd_load_c45 = new QFileDialog( ".", "Decision Tree ( *.c45 )", this, "Load Decision Tree", TRUE );
  q_fd_load_c45->setMode( QFileDialog::ExistingFile );
  if ( q_fd_load_c45->exec() == QDialog::Accepted )
  {
    /* Open file to load attributes list */
    loaded_c45 = q_fd_load_c45->selectedFile().latin1();
    ifstream c45_file( loaded_c45.c_str() );
    string tmp;
    do
    {
      c45_file >> tmp;
    } while ( tmp != "###" );
    /* Unselect all attributes */
    q_lb_attributes_classify->selectAll( true );
    q_tb_unselect_attribute_classify_clicked();
    /* Clear all selected attributes */
    q_lb_attributes_classify->clearSelection();
    /* Read class names */
    unsigned total_classes;
    string class_name;
    c45_file >> total_classes;
    for ( unsigned i = 0; i < total_classes; i++ )
      c45_file >> class_name;
    /* Select specific attributes */
    unsigned total_attributes;
    string attribute_name;
    c45_file >> total_attributes;
    for ( unsigned i = 0; i < total_attributes; i++ )
    {
      c45_file >> attribute_name;
      QListBoxItem *item = q_lb_attributes_candidate->findItem( QString( attribute_name.c_str() ) );
      if ( item == 0 )
      {
        QString message;
        message.sprintf( "Missing Attribute %s", attribute_name.c_str() );
        QMessageBox::critical( this, function_name( __FUNCTION__ ), message ); 
        return;
      }
      q_lb_attributes_candidate->setSelected( item, TRUE );
    }
    q_tb_select_attribute_classify_clicked();
    c45_file.close();
    q_cb_training_labels->setCurrentText( "training_class" );

    QMessageBox::information( this, function_name( __FUNCTION__ ), tr( "Decision Tree Opened!\nPress Classify to start classification." ) );
  }

  delete q_fd_load_c45;

  #ifdef _DEBUG
  debug.finish( "q_pb_load_c45_clicked()" );
  #endif
}

void geodma_window::q_pb_classify_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_pb_classify_clicked()" );
  #endif
  
  unlock_grid();

  string algorithm = q_tb_classifiers->itemLabel( q_tb_classifiers->currentIndex() ).latin1();
  list<int> classes_choice;

  if ( q_lb_attributes_classify->count() < 1 )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Choose at least one attribute" ) );
    return;
  }
  /*
  if ( q_cb_create_themes->isChecked() && q_le_new_view->text().length() == 0 )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Set the View name" ) );
    return;
  }
  */

  /* Check for Training Labels */
  bool is_supervised = false;
  if ( algorithm == "DECISION_TREE" || algorithm == "NEURAL_NETWORKS" )
    is_supervised = true;
  else if ( algorithm == "SOM" )
    is_supervised = false;
  if ( q_cb_training_labels->currentText() == "No Labels" && is_supervised )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Please select one training label" ) );
    return;
  }
  if ( is_supervised == false && q_cb_training_labels->currentText() != "No Labels" )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "You chose an unsupervised algorithm, please select 'No Labels'" ) );
    return;
  }

  TeLayer* layer_choice;
  string attribute_table_name;
  TeTable table_choice;

  if ( q_cb_classifying_what->currentText() == "Polygons" )
  {
    layer_choice = get_polygons_layer();
    attribute_table_name = polygons_attribute_table_name;
  }
  else if ( q_cb_classifying_what->currentText() == "Cells" )
  {
    layer_choice = get_cells_layer();
    attribute_table_name = cells_attribute_table_name;
  }
  else // Points
  {
    layer_choice = get_points_layer();
    attribute_table_name = points_attribute_table_name;
  }

  /* Finding attributes indexes */
  vector<int> attributes_indexes;
  for ( unsigned i = 0; i < q_lb_attributes_classify->count(); i++ )
  {
    int attr_index = find_index( q_lb_attributes_classify->item( i )->text().ascii(), layer_choice, attribute_table_name ); 
    if ( attr_index < 0 )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find Attribute Index" ) );
      return;
    }
    attributes_indexes.push_back( attr_index );
  }

  /* Loading Table */
  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  if ( !curr_database->loadTable( attribute_table_name, table_choice ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) );
    return;
  }

  if ( table_choice.size() == 0 )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Empty attribute table" ) );
    return;
  }

  /* Getting Input Data */
  TePDIPIManager status( tr( "Selecting input data..." ).ascii(), table_choice.size(), true );
  vector<TeClassSample> input_attributes;
  string attribute_value;
  bool have_empty_values;
  for ( unsigned i = 0; i < table_choice.size(); i++ )
  {
    status.Increment();
    TeClassSample tmp_attributes;
    have_empty_values = false;
    for ( unsigned j = 0; j < attributes_indexes.size(); j++ )
    {
      attribute_value = table_choice( i, attributes_indexes[ j ] );
      if ( attribute_value.size() > 0 )
        tmp_attributes.push_back( (double) atof ( attribute_value.c_str() ) );
      else
        have_empty_values = true;
    }
    if ( !have_empty_values )
      input_attributes.push_back( tmp_attributes );
  }
  status.Toggle( false );

  /* Getting Training Data */
  TeClassSampleSet training_samples;

  /* Getting Ancillary Data */
  status.Reset( "Selecting ancillary data...", q_lb_ancillary_training_sets->count() );
  status.Toggle( true );
  TeClassSampleSet ancillary_training_samples;
  for ( unsigned i = 0; i < q_lb_ancillary_training_sets->count(); i++ )
  {
    if ( !q_lb_ancillary_training_sets->isSelected( i ) )
      continue;

    string layer_name = q_lb_ancillary_training_sets->item( i )->text().ascii();
    layer_name.erase( layer_name.find( "->" ) );
    TeLayer* ancillary_layer = get_layer( layer_name );
    string ancillary_table_name = q_lb_ancillary_training_sets->item( i )->text().ascii();
    ancillary_table_name.erase( 0, ancillary_table_name.find( "->" ) + 2 );
    TeTable ancillary_table;

    /* Finding Ancillary Attributes Indexes */
    vector<int> ancillary_attributes_indexes;
    for ( unsigned j = 0; j < q_lb_attributes_classify->count(); j++ )
    {
      int attr_index = find_index( q_lb_attributes_classify->item( j )->text().ascii(), ancillary_layer, ancillary_table_name ); 
      if ( attr_index < 0 )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find ancillary attribute " ) +
                                                                    q_lb_attributes_classify->item( j )->text().ascii() +
                                                                    tr( " in table " ) +
                                                                    q_lb_ancillary_training_sets->item( i )->text().ascii() );
        return;
      }
      ancillary_attributes_indexes.push_back( attr_index );
    }

    /* Loading Ancillary Table */
    curr_database = plug_params_ptr_->getCurrentDatabasePtr();
    if ( !curr_database->loadTable( ancillary_table_name, ancillary_table ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the ancillary theme attribute table" ) );
      return;
    }
    if ( ancillary_table.size() == 0 )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Empty ancillary attribute table" ) );
      return;
    }

    /* Ancillary Data */
    int ancillary_label_index = find_index( q_cb_training_labels->currentText().ascii(), ancillary_layer, ancillary_table_name ); 
    if ( ancillary_label_index == -1 ) 
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "The column '" ) +
                                                                  q_cb_training_labels->currentText().ascii() +
                                                                  tr( "' was not found on ancillary table '" ) +
                                                                  ancillary_table_name.c_str() );
      return;
    }

    for ( unsigned j = 0; j < ancillary_table.size(); j++ )
    {
      TeClassSample tmp_attributes;
      string ancillary_training_class_name;
      if ( ancillary_label_index >= 0 )
        ancillary_training_class_name = ancillary_table( j, ancillary_label_index ).c_str();
      if ( ancillary_training_class_name.empty() )
        ancillary_training_class_name = "No Labels";
      if ( is_supervised && ancillary_training_class_name == "No Labels" )
        continue;
      TeClassSample training_sample;
      for ( unsigned k = 0; k < ancillary_attributes_indexes.size(); k++ )
        training_sample.push_back( ( double ) atof ( ancillary_table( j, ancillary_attributes_indexes[ k ] ).c_str() ) );
      ancillary_training_samples[ ancillary_training_class_name ].push_back( training_sample );
    }
    status.Increment();
  }
  status.Toggle( false );

  /* Get attributes names */
  vector<string> attributes_names;
  for ( unsigned i = 0; i < q_lb_attributes_classify->count(); i++ )
    attributes_names.push_back( q_lb_attributes_classify->item( i )->text().ascii() );
  /* Find training label index */
  int label_index = find_index( q_cb_training_labels->currentText().ascii(), layer_choice, attribute_table_name ); 
  /* Supervised Classification */
  if ( q_cb_training_labels->currentText().compare( QString( "No Labels" ) ) )
  {
    if ( label_index < 0 )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find Label Attribute Index" ) );
      return;
    }
    /*  No samples needed */
    if ( loaded_c45.length() > 0 )
    {
      ifstream class_names( loaded_c45.c_str() );
      string tmp;
      do
      {
        class_names >> tmp;
      } while ( tmp != "###" );
      unsigned total_classes;
      string class_name;
      class_names >> total_classes;
      for ( unsigned i = 0; i < total_classes; i++ )
      {
        class_names >> class_name;
        TeClassSample training_sample;
        training_sample.push_back( ( double ) 0.0 );
        training_samples[ class_name ].push_back( training_sample );
      }
      class_names.close();
    }
    else
    {
      for ( unsigned i = 0; i < table_choice.size(); i++ )
      {
        string training_class_name = table_choice( i, label_index ).c_str();
        if ( !training_class_name.empty() )
        {
          TeClassSample training_sample;
          for ( unsigned j = 0; j < attributes_indexes.size(); j++ )
          {
            training_sample.push_back( ( double ) atof ( table_choice( i, attributes_indexes[ j ] ).c_str() ) );
          }
          training_samples[ training_class_name ].push_back( training_sample );
        }
      }
    }
  }
  /* Unsupervised Classification */
  else
  {
    for ( unsigned i = 0; i < table_choice.size(); i++ )
    {
      TeClassSample training_sample;
      for ( unsigned j = 0; j < attributes_indexes.size(); j++ )
        training_sample.push_back( ( double ) atof ( table_choice( i, attributes_indexes[ j ] ).c_str() ) );
      training_samples[ "No Labels" ].push_back( training_sample );
    }
  }

  /* Copy Ancillary Data */
  string ancillary_class;
  vector<TeClassSample> ancillary_samples;
  TeClassSampleSet::iterator ancillary_it;
  for ( ancillary_it = ancillary_training_samples.begin(); ancillary_it != ancillary_training_samples.end(); ++ancillary_it )
  {
    ancillary_class = ancillary_it->first;
    ancillary_samples = ancillary_it->second;
    for ( unsigned i = 0; i < ancillary_samples.size(); i++ )
      training_samples[ ancillary_class ].push_back( ancillary_samples[ i ] );
  }

  /* Avoid empty training sets*/
  if ( training_samples.size() < 1 )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "You must set training samples!" ) );
    return;
  }
  if ( is_supervised && training_samples.size() < 2 )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "You must define at least two training classes!" ) );
    return;
  }

  /* Used attributes in training/classification */
  vector<int> clas_dim;
  for ( unsigned i = 0; i < q_lb_attributes_classify->count(); i++ )
    clas_dim.push_back( i );

  /* Loading selected classifier */
  if ( algorithm == "SOM" )
  {
    /* SOM parameters */
    unsigned max_epochs = q_sb_max_epochs->value();
    float radius = atof( q_le_radius->text().ascii() );
    float decreasing = atof( q_le_decreasing->text() );
    float alpha = atof( q_le_alpha->text() );
    unsigned map_width = q_sb_map_width->value();
    unsigned map_height = q_sb_map_height->value();

    /* SOM classifier, training and classification */ 
    debug.print("som params");
    
    som_classifier_params som_params( clas_dim.size(), map_width, map_height, alpha, radius, decreasing, max_epochs );
    som_classifier som_classify( som_params );
    som_classify.train( training_samples.begin(), training_samples.end(), clas_dim );
    som_classify.classify( input_attributes.begin(), input_attributes.end(), clas_dim, classes_choice );
  }
  else if ( algorithm == "DECISION_TREE" )
  {
    /* C45 classifier, training and classification */
    c45_classifier c45_classify( attributes_names );
    
    if ( loaded_c45.length() > 0 )
      c45_classify.classify_from_file( input_attributes.begin(), input_attributes.end(), clas_dim, classes_choice, loaded_c45 );
    else
    {
      c45_classify.train( training_samples.begin(), training_samples.end(), clas_dim );
      c45_classify.classify( input_attributes.begin(), input_attributes.end(), clas_dim, classes_choice );
      if ( q_cb_save_c45->isChecked() )
      {
        q_cb_save_c45->setChecked( false );
        QFileDialog *q_fd_save_c45 = new QFileDialog( ".", "Decision Tree ( *.c45 )", this, "Save Decision Tree", TRUE );
        q_fd_save_c45->setMode( QFileDialog::AnyFile );
        if ( q_fd_save_c45->exec() == QDialog::Accepted )
        {
          int total_samples = 0;

          /* Open file to save attributes list */
          string c45_filename = q_fd_save_c45->selectedFile().latin1();
          if ( c45_filename.find( ".c45" ) == string::npos )
            c45_filename += ".c45";
          ofstream c45_file( c45_filename.c_str() );

          /* Save decision tree */
          c45_file << c45_classify.get_tree() << endl << "###" << endl;

          /* Save class names */
          c45_file << training_samples.size() << endl;
          TeClassSampleSet::iterator samples_it = training_samples.begin();
          while ( samples_it != training_samples.end() )
          {
            total_samples += samples_it->second.size();
            c45_file << samples_it->first << endl;
            samples_it++;
          }

          /* Save attributes names */
          c45_file << q_lb_attributes_classify->count() << endl;
          for ( unsigned i = 0; i < q_lb_attributes_classify->count(); i++ )
            c45_file << q_lb_attributes_classify->text( i ) << endl;
          c45_file << "###" << endl;
          c45_file.close();
        }

        delete q_fd_save_c45;
      }
      QMessageBox::information( this, "Generated Decision Tree", c45_classify.get_tree().c_str() );
    }
  }
  else if ( algorithm == "NEURAL_NETWORKS" )
  {
    /* NEURAL_NETWORKS classification */
    unsigned max_epochs = q_sb_max_epochs_ann->value();
    float learning_rate = atof( q_le_learning_rate->text().ascii() );
    unsigned hidden_neurons = q_sb_hidden_neurons->value();
    string activation_function = q_cb_activation_function->currentText();

    /* NEURAL_NETWORKS classifier, training and classification */ 
    nn_classifier neural_networks_classify( nn_classifier_params( max_epochs, learning_rate, hidden_neurons, activation_function, 4 ) );
    neural_networks_classify.train( training_samples.begin(), training_samples.end(), clas_dim );
    neural_networks_classify.classify( input_attributes.begin(), input_attributes.end(), clas_dim, classes_choice );
  }

  /* Create classification results column, with the same name of the view */
  TeAttribute new_attr;
  string new_attr_name = q_le_new_view->text().ascii();
  new_attr.rep_.name_ = new_attr_name;
  new_attr.rep_.type_ = TeSTRING;
  new_attr.rep_.numChar_ = 50;
  new_attr.rep_.null_ = true;

  if ( !curr_database->columnExist( attribute_table_name, new_attr.rep_.name_, new_attr ) )
    if ( !curr_database->addColumn( attribute_table_name, new_attr.rep_ ) )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not create the new column:\n" ) +
                                                                  curr_database->errorMessage().c_str() );
      return;
    }

  int attr_index = find_index( new_attr_name, layer_choice, attribute_table_name );
  if ( attr_index < 0 )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find Attribute Index" ) );
    return;
  }
  table_choice.clear();
  if ( !curr_database->loadTable( attribute_table_name, table_choice ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) );
    return;
  }

  /* Create labels for classes, will be numbers (unsupervised) or names (supervised). */
  TeClassSampleSet::iterator labels_it = training_samples.begin();
  vector<string> labels_unique;
  if ( training_samples.size() > 1 )
  {
    /* Supervised algorithms */
    while ( labels_it != training_samples.end() )
    {
      labels_unique.push_back( labels_it->first );
      ++labels_it;
    }
  }
  else
  {
    /* Unsupervised algorithms, retrieve the maximum number of classes */
    unsigned max_classes = 0;
    if ( algorithm == "SOM" )
    {
      unsigned map_width = q_sb_map_width->value();
      unsigned map_height = q_sb_map_height->value();
      max_classes = map_width * map_height;
    }

    for ( unsigned c = 0; c < max_classes; c++ )
      labels_unique.push_back( Te2String( c ) );
  }

  unsigned k = 0;
  list<int>::iterator list_it = classes_choice.begin();
  while ( list_it != classes_choice.end() )
  {
    string alg_class = labels_unique[ *list_it ];
    table_choice.setValue( k, attr_index, alg_class );
    ++k;
    ++list_it;
  }

  /* Save results */
  if ( !curr_database->updateTable( table_choice ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to update attribute table:\n" ) + curr_database->errorMessage().c_str() );
    return;
  }

  /* Create Themes with classes */
  if ( q_le_new_view->text().length() > 0 )
  {
    TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
    TeLayer* layer_choice;
    
    if ( q_cb_classifying_what->currentText() == "Polygons" )
      layer_choice = get_polygons_layer();
    else if ( q_cb_classifying_what->currentText() == "Cells" )
      layer_choice = get_cells_layer();
    else
      layer_choice = get_points_layer();

    /* Creating a new View */
    TeProjection* new_view_proj = TeProjectionFactory::make( layer_choice->projection()->params() );
    TeView* new_view = new TeView ( layer_choice->name(), curr_database->user() );
    new_view->name( q_le_new_view->text().ascii() );
    new_view->setCurrentBox( layer_choice->box() );
    new_view->setCurrentTheme( -1 );
    new_view->projection( new_view_proj );
    if ( !curr_database->insertView( new_view ) )
    {
      delete new_view;
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to insert the new view into database" ) );
      return;
    }

    geodma_colors C;
    if ( q_rb_create_themes->isChecked() )
    {
      /* Creating a Theme per Class */
      for ( unsigned i = 0; i < labels_unique.size(); i++ )
      {
        TeTheme* new_theme = new TeTheme( layer_choice->name(), layer_choice );
        if ( !new_theme )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Invalid Theme pointer" ) );
          return;
        }

        TeTable training_table;
        if ( !curr_database->loadTable( "training_table", training_table ) )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) );
          return;
        }
        int R = C.get_R( i ),
            G = C.get_G( i ),
            B = C.get_B( i );
        if ( q_cb_training_labels->currentText() != "No Labels" )
        {
          for ( unsigned j = 0; j < training_table.size(); j++ )
          {
            string class_name = training_table( j, 1 );
            if ( class_name == labels_unique[ i ] && training_table( j, 5 ) == "1" )
            {
              R = atoi( training_table( j, 2 ).c_str() ),
              G = atoi( training_table( j, 3 ).c_str() ),
              B = atoi( training_table( j, 4 ).c_str() );
            }
          }
        }

        new_theme->name( labels_unique[ i ] );
        QString name;
        if ( is_supervised )
          name.sprintf( "%s = '%s'", new_attr_name.c_str(), labels_unique[ i ].c_str() );
        else
          name.sprintf( "%s = '%d'", new_attr_name.c_str(), i );
        new_theme->attributeRest( name.ascii() );
        new_view->add( new_theme );

        TeVisual* new_visual = new TeVisual( TePOLYGONS );
        TeColor new_color,
        contour_color;
        new_color.init( R, G, B );
        contour_color.init( 0, 0, 0 );
        new_visual->color( new_color ); 
        new_visual->contourColor( contour_color );
        new_visual->contourStyle( TeLnTypeNone );
        if ( using_rasters )
          new_visual->transparency( 50 );
        else
          new_visual->transparency( 0 );
        new_theme->setVisualDefault( new_visual, TePOLYGONS );

        TeVisual* point_visual = new TeVisual( TePOINTS );
        new_color.init( R, G, B );
        point_visual->color( new_color );
        point_visual->style( TePtTypeX );
        point_visual->contourColor( contour_color );
        new_theme->setVisualDefault( point_visual, TePOINTS );

        int allRep = layer_choice->geomRep();
        new_theme->visibleRep( allRep );
        new_theme->setAttTables( layer_choice->attrTables() );

        if ( !new_theme->save() )
        {
          delete new_theme;
          curr_database->deleteView( new_view->id() );
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to create the new theme" ) );
          return;
        }

        if ( !new_theme->buildCollection() )
        {
          delete new_theme;
          curr_database->deleteView( new_view->id() );
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to build the new theme collection" ) );
          return;
        }

        delete new_theme;
        delete new_visual;
        delete point_visual;
      }
    }
    else if ( q_rb_create_legends->isChecked() )
    {
      /* Creating one Theme and one Legend per Class */
      TeTheme* new_theme = new TeTheme( "geometries", layer_choice );
      if ( !new_theme )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Invalid Theme pointer" ) );
        return;
      }
      new_theme->name( "geometries" );
      new_view->add( new_theme );

      int allRep = layer_choice->geomRep();
      new_theme->visibleRep( allRep );
      new_theme->setAttTables( layer_choice->attrTables() );

      if ( !new_theme->save() ) 
      {
        delete new_theme;
        curr_database->deleteView( new_view->id() );
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to create the new theme" ) );
        return;
      }

      if ( !new_theme->buildCollection() ) 
      {
        delete new_theme;
        curr_database->deleteView( new_view->id() );
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to build the new theme collection" ) );
        return;
      }

      new_theme->legend().clear();

      /* Setting attribute to create Legend */
      TeAttributeRep attribute_rep;
      attribute_rep.name_ = new_attr_name;
      attribute_rep.type_ = TeSTRING;

      /* Creating Grouping */
      TeGrouping classification_groups;
      classification_groups.groupMode_ = TeUniqueValue;
      classification_groups.groupAttribute_ = attribute_rep;

      if ( !new_theme->buildGrouping( classification_groups ) )
      {
        delete new_theme;
        curr_database->deleteView( new_view->id() );
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to build Legend Grouping" ) );
        return;
      }

      TeVisual visual( TePOLYGONS );
      TeColor color;
      for( unsigned i = 0; i < labels_unique.size(); i++ )
      {
        TeTable training_table;
        if ( !curr_database->loadTable( "training_table", training_table ) )
        {
          QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) );
          return;
        }
        int R = C.get_R( i ),
            G = C.get_G( i ),
            B = C.get_B( i );
        if ( q_cb_training_labels->currentText() != "No Labels" )
        {
          for ( unsigned j = 0; j < training_table.size(); j++ )
          {
            string class_name = training_table( j, 1 );
            if ( class_name == labels_unique[ i ] && training_table( j, 5 ) == "1" )
            {
              R = atoi( training_table( j, 2 ).c_str() ),
              G = atoi( training_table( j, 3 ).c_str() ),
              B = atoi( training_table( j, 4 ).c_str() );
            }
          }
        }
        color.init( R, G, B );
        visual.color( color );
        if ( using_rasters )
          visual.transparency( 50 );
        else
          visual.transparency( 0 );
        visual.contourStyle( TeLnTypeNone );
        visual.contourWidth( new_theme->defaultLegend().visual( TePOLYGONS )->contourWidth() );
        visual.contourColor( new_theme->defaultLegend().visual( TePOLYGONS )->contourColor() );
        new_theme->setGroupingVisual( i + 1, &visual, TePOLYGONS );
      }

      /* Saving the theme legend to the database */
      if ( !new_theme->saveGrouping() )
      {
        delete new_theme;
        curr_database->deleteView( new_view->id() );
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to save Legend Grouping" ) );
        return;
      }

      if ( !new_theme->saveLegendInCollection() )
      {
        delete new_theme;
        curr_database->deleteView( new_view->id() );
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to save Legend in Collection" ) );
        return;
      }
    }
    /* Creating Theme for Raster */
    if ( using_rasters )
    {
      TeTheme* raster_theme = new TeTheme( "input_raster", get_rasters_layer() );
      if ( !raster_theme )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Invalid Theme pointer" ) );
        return;
      }
      new_view->add( raster_theme );

      int raster_allRep = get_rasters_layer()->geomRep();
      raster_theme->visibleRep( raster_allRep );
      raster_theme->setAttTables( get_rasters_layer()->attrTables() );

      if ( !raster_theme->save() )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to create the new theme" ) );
        curr_database->deleteView( new_view->id() );
        return;
      }
      if ( !raster_theme->buildCollection() )
      {
        QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Unable to build the new theme collection" ) );
        curr_database->deleteView( new_view->id() );
        return;
      }
    }
  }

  /* All done */
  update_TV_interface();
  refresh_tables();
  q_tab->setCurrentPage( get_tab_number( "Classify" ) );
  QMessageBox::information( this, function_name( __FUNCTION__ ), tr( "Classification done!" ) );

  #ifdef _DEBUG
  debug.finish( "q_pb_classify_clicked()" );
  #endif
}

/* ========== VALIDATION ========== */
void geodma_window::q_pb_create_confusion_matrix_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_pb_create_confusion_matrix_clicked()" );
  #endif

  string attribute_table_name;
  TeLayer* layer_choice;
  TeTable table_choice;
  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  
  if ( q_cb_validating_what->currentText() == "Polygons" )
  {
    layer_choice = get_polygons_layer();
    attribute_table_name = polygons_attribute_table_name;
  }
  else if ( q_cb_validating_what->currentText() == "Cells" )
  {
    layer_choice = get_cells_layer();
    attribute_table_name = cells_attribute_table_name;
  }
  else // Points
  {
    layer_choice = get_points_layer();
    attribute_table_name = points_attribute_table_name;
  }

  table_choice.clear();
  if ( !curr_database->loadTable( attribute_table_name, table_choice ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) );
    return;
  }

  if ( !layer_choice->getAttrTablesByName( attribute_table_name, table_choice, TeAllAttrTypes ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) );
    return;
  }

  table_choice.clear();
  if ( !curr_database->loadTable( attribute_table_name, table_choice ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) );
    return;
  }

  /* Find training label index */
  int reference_label_index = find_index( q_cb_reference_column->currentText().ascii(), layer_choice, attribute_table_name );
  int classification_label_index = find_index( q_cb_classification_column->currentText().ascii(), layer_choice, attribute_table_name );

  if ( reference_label_index < 0 || classification_label_index < 0 )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find Labels' Attribute Index" ) );
    return;
  }

  vector<string> references;
  vector<string> classifications;
  set<string> classes_unique;
  for ( unsigned i = 0; i < table_choice.size(); i++ )
  {
    string reference_name = table_choice( i, reference_label_index ).c_str();
    string classification_name = table_choice( i, classification_label_index ).c_str();
    if ( !reference_name.empty() && !classification_name.empty() )
    {
      references.push_back( reference_name );
      classifications.push_back( classification_name );
      classes_unique.insert( reference_name );
      classes_unique.insert( classification_name );
    }
  }
  /* Avoid empty columns */
  if ( classes_unique.size() < 1 )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "No classification data found at reference, or test column." ) );
    return;
  }

  map<string, int> classes_indices;
  map<int, string> indices_classes;
  int index = 0;
  set<string>::iterator classes_it = classes_unique.begin();
  while ( classes_it != classes_unique.end() )
  {
    indices_classes[ index ] = *classes_it;
    classes_indices[ *classes_it ] = index++;
    ++classes_it;
  }

  TeMatrix confusion_matrix;
  confusion_matrix.Init( classes_unique.size(), classes_unique.size(), 0.0 );
  for ( unsigned i = 0; i < references.size(); i++ )
  {
    int reference_index = classes_indices[ references[ i ] ];
    int classification_index = classes_indices[ classifications[ i ] ];
    confusion_matrix( reference_index, classification_index ) = confusion_matrix( reference_index, classification_index ) + 1;
  }

  int max_confusion = 0;
  for ( int i = 0; i < confusion_matrix.Nrow(); i++ )
    for ( int j = 0; j < confusion_matrix.Ncol(); j++ )
    {
      if ( max_confusion < confusion_matrix( i, j ) )
        max_confusion = confusion_matrix( i, j );
    }

  /* Printing confusion matrix */
  int decimal = log10( (double) max_confusion );
  stringstream confusion_string;
  confusion_string << setw( decimal + 1 ) << " " << "   ";
  for ( unsigned i = 0; i < indices_classes.size(); i++ )
    confusion_string << setw( decimal + 1 ) << i << " ";
  confusion_string << endl;
  unsigned line_size = confusion_string.str().size();
  for ( unsigned i = 0; i < line_size + 1; i++ )
    confusion_string << "-";
  confusion_string << endl;

  for ( int i = 0; i < confusion_matrix.Nrow(); i++ )
  {
    confusion_string << setw( decimal + 1 ) << i << " | ";
    for ( int j = 0; j < confusion_matrix.Ncol(); j++ )
      confusion_string << setw( decimal + 1 ) << confusion_matrix( i, j ) << " ";
    confusion_string << endl;
  }

  for ( unsigned i = 0; i < indices_classes.size(); i++ )
    confusion_string << i << " -> " << indices_classes[ i ] << endl;

  double kappa = get_kappa( confusion_matrix );
  confusion_string << endl << "kappa: " << kappa << endl;
  q_te_confusion_matrix->setText( confusion_string.str().c_str() );

  #ifdef _DEBUG
  debug.finish( "q_pb_create_confusion_matrix_clicked()" );
  #endif
}

void geodma_window::q_pb_start_monte_carlo_clicked()
{
  #ifdef _DEBUG
  debug.start( "q_pb_start_monte_carlo_clicked()" );
  #endif

  TeLayer* layer_choice;
  string attribute_table_name;
  TeTable table_choice;

  if ( q_cb_validating_what->currentText() == "Polygons" )
  {
    layer_choice = get_polygons_layer();
    attribute_table_name = polygons_attribute_table_name;
  }
  else if ( q_cb_validating_what->currentText() == "Cells" )
  {
    layer_choice = get_cells_layer();
    attribute_table_name = cells_attribute_table_name;
  }
  else // Points
  {
    layer_choice = get_points_layer();
    attribute_table_name = points_attribute_table_name;
  }

  /* Retrieving attributes indexes and names */
  vector<int> attributes_indexes;
  vector<string> attributes_names;
  for ( unsigned i = 0; i < q_lb_monte_carlo_attributes->count(); i++ )
  {
    if ( !q_lb_monte_carlo_attributes->isSelected( i ) )
      continue;

    int attr_index = find_index( q_lb_monte_carlo_attributes->item( i )->text().ascii(), layer_choice, attribute_table_name );
    if ( attr_index < 0 )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find Attribute Index" ) );
      return;
    }
    attributes_indexes.push_back( attr_index );
    attributes_names.push_back( q_lb_monte_carlo_attributes->item( i )->text().ascii() );
  }

  /* Loading Table */
  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  if ( !curr_database->loadTable( attribute_table_name, table_choice ) ) 
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get the theme attribute table" ) );
    return;
  }
  if ( table_choice.size() == 0 )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Empty attribute table" ) );
    return;
  }

  TePDIPIManager status( tr( "Performing Monte Carlo simulation..." ).ascii(), q_sb_monte_carlo_iterations->value(), true );
  srand( (unsigned) time( 0 ) );
  q_te_monte_carlo_output->clear();

  if ( q_cb_show_only_kappas->isChecked() )
    q_te_monte_carlo_output->append( "List of kappa values: ");

  double min_kappa = 1.0;
  double max_kappa = -1.0;
  double avg_kappa = 0.0;

  for ( int m = 0; m < q_sb_monte_carlo_iterations->value(); m++ )
  {
    status.Increment();
    stringstream output_montecarlo;

    /* Getting Training Data */
    TeClassSampleSet training_samples;
    TeClassSampleSet partial_training_samples;

    /* Find training label index */
    int label_index = find_index( q_cb_monte_carlo_labels->currentText().ascii(), layer_choice, attribute_table_name );
    if ( label_index < 0 )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Problem to find Training Label Index" ) );
      return;
    }

    /* Loading training data */
    for ( unsigned i = 0; i < table_choice.size(); i++ )
    {
      string training_class_name = table_choice( i, label_index ).c_str();
      if ( !training_class_name.empty() )
      {
        TeClassSample training_sample;
        for ( unsigned j = 0; j < attributes_indexes.size(); j++ )
          training_sample.push_back( ( double ) atof ( table_choice( i, attributes_indexes[ j ] ).c_str() ) );

        training_samples[ training_class_name ].push_back( training_sample );
      }
    }

    map<string, unsigned> classes_indices;
    unsigned c = 0;
    TeClassSampleSet::iterator training_it = training_samples.begin();
    while ( training_it != training_samples.end() )
    {
      classes_indices[ training_it->first ] = c++;
      ++training_it;
    }

    /* Splitting data in Training and Test */
    vector<TeClassSample> input_attributes;
    list<int> test_classes;
    training_it = training_samples.begin();
    while ( training_it != training_samples.end() )
    {
      for ( unsigned i = 0; i < training_samples[ training_it->first ].size(); i++ )
      {
        /* 70% for training, 30% for testing */
        unsigned random_number = ( rand() % 10 );
        if ( random_number < 7 ) 
          partial_training_samples[ training_it->first ].push_back( training_samples[ training_it->first ][i] );
        else
        {
          input_attributes.push_back( training_samples[ training_it->first ][i] );
          test_classes.push_back( classes_indices[ training_it->first ] );
        }
      }
      ++training_it;
    }

    /* Avoid empty training sets*/
    if ( partial_training_samples.size() < 1 )
    {
      QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "You must set training samples!" ) );
      return;
    }
    /* Used attributes in training/classification */
    vector<int> clas_dim;
    for ( unsigned i = 0; i < attributes_names.size(); i++ )
      clas_dim.push_back( i );

    /* C45 classifier, training and classification */
    list<int> classes_choice;
    c45_classifier c45_classify( attributes_names );
    c45_classify.train( partial_training_samples.begin(), partial_training_samples.end(), clas_dim );
    c45_classify.classify( input_attributes.begin(), input_attributes.end(), clas_dim, classes_choice );

    if ( !q_cb_show_only_kappas->isChecked() )
    {
      output_montecarlo << endl << "iteration " << m;
      output_montecarlo << c45_classify.get_tree() << endl;
      output_montecarlo << "* kappa: ";
    }

    /* Showing outputs */
    list<int>::iterator list_it = test_classes.begin();
    list_it = classes_choice.begin();

    TeMatrix confusion_matrix;
    confusion_matrix.Init( classes_indices.size(), classes_indices.size(), 0.0 );
    list<int>::iterator reference_it = test_classes.begin();
    list<int>::iterator classification_it = classes_choice.begin();
    while ( reference_it != test_classes.end() )
    {
      confusion_matrix( *reference_it, *classification_it ) = confusion_matrix( *reference_it, *classification_it ) + 1;
      ++reference_it;
      ++classification_it;
    }
    double kappa = get_kappa( confusion_matrix );
    output_montecarlo << kappa << " ";
    q_te_monte_carlo_output->append( output_montecarlo.str().c_str() );

    if ( kappa < min_kappa )
      min_kappa = kappa;
    if ( kappa > max_kappa )
      max_kappa = kappa;
    avg_kappa += kappa;
  }

  avg_kappa /= q_sb_monte_carlo_iterations->value();
  stringstream overall_montecarlo;
  overall_montecarlo << endl << "-------------------" << endl;
  overall_montecarlo << "Overall statistics:" << endl;
  overall_montecarlo << "Min: " << setw( 4 ) << min_kappa << endl;
  overall_montecarlo << "Max: " << setw( 4 ) << max_kappa << endl;
  overall_montecarlo << "Average: " << setw( 4 ) << avg_kappa << endl;
  q_te_monte_carlo_output->append( overall_montecarlo.str().c_str() );

  status.Toggle( false );
  QMessageBox::information( this, function_name( __FUNCTION__ ), tr( "Monte Carlo simulation done!" ) );

  #ifdef _DEBUG
  debug.finish( "q_pb_start_monte_carlo_clicked()" );
  #endif
}

/* ========== ABOUT ========== */
void geodma_window::fill_about()
{
  #ifdef _DEBUG
  debug.start( "fill_about()" );
  #endif

  QString about_text;
  about_text.sprintf( "<p>Version: %s</p>", VERSION );
  q_te_version->setText( about_text );
  
  if ( QDate::currentDate().month() != 12 )
    q_pm_xmas->hide();
  if ( QDate::currentDate().month() != 9 )
    q_pm_flag->hide();

  #ifdef _DEBUG
  debug.finish( "fill_about()" );
  #endif
}

/* ========== MULTITEMPORAL ========== */
void geodma_window::getMousePressed( TeCoord2D & coord, int state, QPoint &point )
{
  #ifdef _DEBUG
  debug.start( "getMousePressed( " + Te2String( coord.x() ) + ", " + Te2String( coord.y() ) + ", " + Te2String( state ) + ", " + Te2String( point.x() ) + ", " + Te2String( point.y() ) + ")" );
  #endif

  TeCoord2D rep_coord;
  TeVectorRemap( coord, plug_params_ptr_->teqtcanvas_ptr_->projection(), rep_coord, myRaster->projection() );

  if ( !TeWithin( rep_coord, myRaster->box() ) )
    return;

  plug_params_ptr_->teqtcanvas_ptr_->plotPoint( coord );
  plug_params_ptr_->teqtcanvas_ptr_->plotOnWindow();

  /* Get multitemporal information from table */
  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();
  TeTable mt_info_table;
  if ( !curr_database->loadTable( "mt_info_table", mt_info_table ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not get mt_info_table:\n" ) + curr_database->errorMessage().c_str() );
    return;
  }

  unsigned t0 = q_sl_interval_mt->value();

  /* Get spectral time series from pressend point, and plot curve */
  myLLCoord = coord;
  myRowCol = myRaster->coord2Index( myLLCoord );

  plot_ts( mt_startingPoint + (t0 - mt_firstLabel) * mt_pointsPerCurve, mt_pointsPerCurve );

  string text = "Point at:\n";
  text += Te2String( myLLCoord.x(), 4 ) + ", ";
  text += Te2String( myLLCoord.y(), 4 );

  q_te_mt_status->setText( text.c_str() );

  #ifdef _DEBUG
  debug.finish( "getMousePressed( " + Te2String( coord.x() ) + ", " + Te2String( coord.y() ) + ", " + Te2String( state ) + ", " + Te2String( point.x() ) + ", " + Te2String( point.y() ) + ")" );
  #endif
}

void geodma_window::plot_ts( unsigned t0, unsigned steps )
{
  #ifdef _DEBUG
  debug.start( "plot_ts( " + Te2String( t0 ) + ", " + Te2String( steps ) + ")" );
  #endif

  if ( q_tab->currentPageIndex() != get_tab_number( "Multitemporal (MT)" ) )
    return;

  unsigned row = myRowCol.y();
  unsigned col = myRowCol.x();

  /* Data to plot curves */
  double *x = new double[ steps + 1 ],
         *y = new double[ steps + 1 ];

  double pixel;
  vector<double> ts;

  for ( unsigned t = t0, b = 0; b < steps; t++, b++ )
  {
    myRaster->getElement( col, row, pixel, t );
    ts.push_back( pixel );

    x[ b ] = b;
    y[ b ] = pixel;
  }

  /* Updating time series plot */
  myTimeSeriesPoints->setData( x, y, steps );
  myTimeSeriesPlot->setAxisScale( QwtPlot::xBottom, 0, steps );
  myTimeSeriesPlot->setAxisScale( QwtPlot::yLeft, 0, 1 );
  myTimeSeriesPlot->erase();
  myTimeSeriesPlot->replot();

  /* Updating polar visualization */
  vector<pair<double, double> > xy = get_xy_from_polar(ts);
  vector<double> px = get_x(xy);
  vector<double> py = get_y(xy);

  for (unsigned i = 0; i < px.size(); i++)
  {
    x[ i ] = px[ i ];
    y[ i ] = py[ i ];
  }

  myPolarPoints->setData( x, y, px.size() );
  myPolarPlot->erase();
  myPolarPlot->replot();

  delete [] x;
  delete [] y;

  #ifdef _DEBUG
  debug.finish( "plot_ts( " + Te2String( t0 ) + ", " + Te2String( steps ) + ")" );
  #endif
}

void geodma_window::q_sl_interval_mt_valueChanged( int )
{
  plot_ts( mt_startingPoint + ( q_sl_interval_mt->value() - mt_firstLabel ) * mt_pointsPerCurve, mt_pointsPerCurve );

  string text = "t = " + Te2String( q_sl_interval_mt->value() );

  q_tl_current_time->setText( text.c_str() );
}

void geodma_window::q_pb_others_pressed()
{
  neet_to_update_tv = true;

  TeTable points_table;

  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();

  if ( !curr_database->loadTable( points_attribute_table_name, points_table ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not load the theme attribute table:\n" ) + curr_database->errorMessage().c_str() );
    return;
  }

  TeLayer* points_layer = get_points_layer();

  string new_point_id = Te2String( points_table.size() );

  TePoint new_point( myLLCoord.x(), myLLCoord.y() );

  new_point.objectId( new_point_id );

  TePointSet ps;

  ps.add( new_point );

  if ( !points_layer->addPoints( ps ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not add point into layer" ) );
    return;
  }

  vector<string> attr_names;
  points_table.attributeNames( attr_names );

  string query = "INSERT INTO ";
  query += points_attribute_table_name + " ( ";
  query += attr_names[ points_table.attrUniquePosition() ];
  query += ", training_class_" + Te2String( q_sl_interval_mt->value() );
  query += " ) VALUES ( " + new_point_id + ", 'others' ); ";

  if ( !execute_query( query ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not insert the new class into database:\n" ) + curr_database->errorMessage().c_str() );
    return;
  }
}

void geodma_window::q_pb_agriculture_1_pressed()
{
  neet_to_update_tv = true;

  TeTable points_table;

  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();

  if ( !curr_database->loadTable( points_attribute_table_name, points_table ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not load the theme attribute table:\n" ) + curr_database->errorMessage().c_str() );
    return;
  }

  TeLayer* points_layer = get_points_layer();

  string new_point_id = Te2String( points_table.size() );

  TePoint new_point( myLLCoord.x(), myLLCoord.y() );

  new_point.objectId( new_point_id );

  TePointSet ps;

  ps.add( new_point );

  if ( !points_layer->addPoints( ps ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not add point into layer" ) );
    return;
  }

  vector<string> attr_names;
  points_table.attributeNames( attr_names );

  string query = "INSERT INTO ";
  query += points_attribute_table_name + " ( ";
  query += attr_names[ points_table.attrUniquePosition() ];
  query += ", training_class_" + Te2String( q_sl_interval_mt->value() );
  query += " ) VALUES ( " + new_point_id + ", 'one_cycle' ); ";

  if ( !execute_query( query ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not insert the new class into database:\n" ) + curr_database->errorMessage().c_str() );
    return;
  }
}

void geodma_window::q_pb_agriculture_2_pressed()
{
  neet_to_update_tv = true;

  TeTable points_table;

  TeDatabase* curr_database = plug_params_ptr_->getCurrentDatabasePtr();

  if ( !curr_database->loadTable( points_attribute_table_name, points_table ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not load the theme attribute table:\n" ) + curr_database->errorMessage().c_str() );
    return;
  }

  TeLayer* points_layer = get_points_layer();

  string new_point_id = Te2String( points_table.size() );

  TePoint new_point( myLLCoord.x(), myLLCoord.y() );

  new_point.objectId( new_point_id );

  TePointSet ps;

  ps.add( new_point );

  if ( !points_layer->addPoints( ps ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not add point into layer" ) );
    return;
  }

  vector<string> attr_names;
  points_table.attributeNames( attr_names );

  string query = "INSERT INTO ";
  query += points_attribute_table_name + " ( ";
  query += attr_names[ points_table.attrUniquePosition() ];
  query += ", training_class_" + Te2String( q_sl_interval_mt->value() );
  query += " ) VALUES ( " + new_point_id + ", 'two_cycles' ); ";

  if ( !execute_query( query ) )
  {
    QMessageBox::critical( this, function_name( __FUNCTION__ ), tr( "Could not insert the new class into database:\n" ) + curr_database->errorMessage().c_str() );
    return;
  }
}

void geodma_window::q_pb_classify_mt_data_pressed()
{
  QFileDialog *q_fd_load_c45 = new QFileDialog( ".", "Decision Tree ( *.c45 )", this, "Load Decision Tree", TRUE );
  q_fd_load_c45->setMode( QFileDialog::ExistingFile );
  vector<string> points_features_names;
  if ( q_fd_load_c45->exec() == QDialog::Accepted )
  {
    /* Open file to load attributes list */
    loaded_c45 = q_fd_load_c45->selectedFile().latin1();
    ifstream c45_file( loaded_c45.c_str() );
    string tmp;
    do
    {
      c45_file >> tmp;
    } while ( tmp != "###" );
    /* Read class names */
    unsigned total_classes;
    string class_name;
    c45_file >> total_classes;
    for ( unsigned i = 0; i < total_classes; i++ )
      c45_file >> class_name;
    /* Select specific attributes */
    unsigned total_attributes;
    string attribute_name;
    c45_file >> total_attributes;
    for ( unsigned i = 0; i < total_attributes; i++ )
    {
      c45_file >> attribute_name;
      points_features_names.push_back( attribute_name );
    }
    c45_file.close();

    // QMessageBox::information( this, function_name( __FUNCTION__ ), tr( "Decision Tree Opened!" ) );
  }

  delete q_fd_load_c45;

  /* Used attributes in training/classification */
  vector<int> clas_dim;
  for ( unsigned i = 0; i < points_features_names.size(); i++ )
    clas_dim.push_back( i );
  c45_classifier c45_classify( points_features_names );

  for ( unsigned t = mt_firstLabel; t < ( mt_firstLabel+ ( myRaster->nBands() - mt_startingPoint ) / mt_pointsPerCurve ); t += mt_stepLabels )
  {
    /* Creating output raster */
    TePDITypes::TePDIRasterPtrType mt_classified_raster;
    TeRasterParams mt_classified_raster_params;
    mt_classified_raster_params.setDataType( TeUNSIGNEDLONG );
    mt_classified_raster_params.setNLinesNColumns( myRaster->params().nlines_, myRaster->params().ncols_ );
    mt_classified_raster_params.nBands( 1 );
    mt_classified_raster_params.mode_ = 'c';
    mt_classified_raster_params.decoderIdentifier_ = "SMARTMEM";
    mt_classified_raster.reset( new TeRaster( mt_classified_raster_params ) );
    mt_classified_raster->init();

    /* Extracting features and classifying pixels */
    double pixel;
    string attribute_value;
    TePDIPIManager point_status( tr( "Extracting pixels features for time..." ).ascii() + Te2String( t ), myRaster->params().ncols_ * myRaster->params().nlines_, true );
    for ( unsigned r = 0; r < myRaster->params().nlines_; r++ )
      for ( unsigned c = 0; c < myRaster->params().ncols_; c++ )
      {
        point_status.Increment();

        vector<TeClassSample> input_features;
        TeClassSample tmp_features;
        list<int> classes_choice;

        vector<double> extracted_features;
        vector<double> ts;

        unsigned band = mt_startingPoint + ( t - mt_firstLabel ) * mt_pointsPerCurve;
        for ( unsigned b = 0; b < mt_pointsPerCurve; b++)
        {
          myRaster->getElement( c, r, pixel, band++ );
          ts.push_back( pixel );
        }

        for ( unsigned a = 0; a < points_features_names.size(); a++)
        {
          QString feature_name( points_features_names[ a ].c_str() );

          tmp_features.push_back( get_feature( "mt_" + string( feature_name.section( "_", 2 ).ascii() ), ts ) );
        }

        input_features.push_back( tmp_features );

        c45_classify.classify_from_file( input_features.begin(), input_features.end(), clas_dim, classes_choice, loaded_c45 );

        mt_classified_raster->setElement( c, r, *( classes_choice.begin() ) );
      }

    point_status.Toggle( false );

    TePDIUtils::TeRaster2Geotiff( mt_classified_raster, "c:/raster_" + Te2String( t ) + ".tif", true );
    /* Saving classified raster in disk (or new layer) */
    // ...
  }
}
