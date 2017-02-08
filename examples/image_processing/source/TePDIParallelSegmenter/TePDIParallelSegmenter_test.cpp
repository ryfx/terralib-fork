#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TeAgnostic.h>
#include <TePDIUtils.hpp>
#include <TePDIParallelSegmenter.hpp>
#include <TePDIBaatz.hpp>
#include <TeProgress.h>
#include <TeStdIOProgress.h>

void RegionGrowing_Strategy_pattern1_test()
{
  // Input data
  
  TePDITypes::TePDIRasterPtrType input_image_ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "pattern1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_image_ptr->init(), "Unable to init inRaster" );
  
  std::vector< unsigned int > input_channels;
  input_channels.push_back( 0 );
  
  TeRasterParams outputImageParams;
  outputImageParams.nBands( 1 );
  outputImageParams.setNLinesNColumns( 1, 1 );
  outputImageParams.setDataType( TeUNSIGNEDLONG, -1 );
  
  TePDITypes::TePDIRasterPtrType output_image_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outputImageParams,
    output_image_ptr ), "RAM Raster Alloc error" );    
    
  // Creating segmenter startegy specific parameters 
  // TePDIParaSegRegGrowStrategy
  
  TePDIParameters strategy_params;
  strategy_params.SetParameter( "euc_treshold", (double)1 );
  strategy_params.SetParameter( "area_min", (int)5 );
  
  // Creating segmenter parameters
  
  TePDIParameters segmenterParams;
  segmenterParams.SetParameter( "input_image_ptr", input_image_ptr );
  segmenterParams.SetParameter( "input_channels", input_channels );
  segmenterParams.SetParameter( "strategy_name", 
    std::string( "RegionGrowing") );
  segmenterParams.SetParameter( "output_image_ptr", output_image_ptr );
  segmenterParams.SetParameter( "strategy_params", strategy_params );
  segmenterParams.SetParameter( "max_seg_threads", (unsigned int)0 );
  segmenterParams.SetParameter( "max_block_size", (unsigned int)128 );
  //segmenterParams.SetParameter( "merge_adjacent_blocks", (bool)false );
  
  // Running segmenter
  
  TePDIParallelSegmenter segmenter;
  
  TEAGN_TRUE_OR_THROW( segmenter.Apply( segmenterParams ), "Apply error" );
    
  // Saving result to disk
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_image_ptr,
    TEPDIEXAMPLESBINPATH "TePDIParallelSegmenter_RG_pattern1_test.tif" ), 
    "GeoTIF generation error" );   
}


void RegionGrowing_Strategy_test()
{
  // Input data
  
  TePDITypes::TePDIRasterPtrType input_image_ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_image_ptr->init(), "Unable to init inRaster" );
  
  std::vector< unsigned int > input_channels;
  input_channels.push_back( 0 );
  input_channels.push_back( 1 );
  input_channels.push_back( 2 );
  
  TeRasterParams outputImageParams;
  outputImageParams.nBands( 1 );
  outputImageParams.setNLinesNColumns( 1, 1 );
  outputImageParams.setDataType( TeUNSIGNEDLONG, -1 );
  
  TePDITypes::TePDIRasterPtrType output_image_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outputImageParams,
    output_image_ptr ), "RAM Raster Alloc error" );    
    
  // Creating segmenter startegy specific parameters 
  // TePDIParaSegRegGrowStrategy
  
  TePDIParameters strategy_params;
  strategy_params.SetParameter( "euc_treshold", (double)20 );
  strategy_params.SetParameter( "area_min", (int)5 );
  
  // Creating segmenter parameters
  
  TePDIParameters segmenterParams;
  segmenterParams.SetParameter( "input_image_ptr", input_image_ptr );
  segmenterParams.SetParameter( "input_channels", input_channels );
  segmenterParams.SetParameter( "strategy_name", 
    std::string( "RegionGrowing") );
  segmenterParams.SetParameter( "output_image_ptr", output_image_ptr );
  segmenterParams.SetParameter( "strategy_params", strategy_params );
  //segmenterParams.SetParameter( "max_seg_threads", (unsigned int)1 );
  //segmenterParams.SetParameter( "max_block_size", (unsigned int)100 );
  //segmenterParams.SetParameter( "merge_adjacent_blocks", (bool)false );
  
  // Running segmenter
  
  TePDIParallelSegmenter segmenter;
  
  TEAGN_TRUE_OR_THROW( segmenter.Apply( segmenterParams ), "Apply error" );
    
  // Saving result to disk
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_image_ptr,
    TEPDIEXAMPLESBINPATH "TePDIParallelSegmenter_RG_test.tif" ), 
    "GeoTIF generation error" );   
}

void Baatz_Strategy_test()
{
  // Input data
  
  TePDITypes::TePDIRasterPtrType input_image_ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_image_ptr->init(), "Unable to init inRaster" );
  
  std::vector< unsigned int > input_channels;
  input_channels.push_back( 0 );
  input_channels.push_back( 1 );
  input_channels.push_back( 2 );
  
  TeRasterParams outputImageParams;
  outputImageParams.nBands( 1 );
  outputImageParams.setNLinesNColumns( 1, 1 );
  outputImageParams.setDataType( TeUNSIGNEDLONG, -1 );
  
  TePDITypes::TePDIRasterPtrType output_image_ptr;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outputImageParams,
    output_image_ptr ), "RAM Raster Alloc error" );    
    
  // Creating segmenter startegy specific parameters 
  // TePDIParaSegBaatzStrategy
  
  TePDIParameters strategy_params;
  strategy_params.SetParameter( "scale", (float)20 );
  strategy_params.SetParameter( "compactness", (float)0.5 );
  strategy_params.SetParameter( "color", (float)0.5 );
  strategy_params.SetParameter( "euc_treshold", (double)20 );
  
  std::vector<float> input_weights;
  input_weights.push_back( (float) 0.33 );
  input_weights.push_back( (float) 0.33 );
  input_weights.push_back( (float) 0.33 );
  strategy_params.SetParameter( "input_weights", input_weights );  
  
  // Creating segmenter parameters
  
  TePDIParameters segmenterParams;
  segmenterParams.SetParameter( "input_image_ptr", input_image_ptr );
  segmenterParams.SetParameter( "input_channels", input_channels );
  segmenterParams.SetParameter( "strategy_name", 
    std::string( "Baatz") );
  segmenterParams.SetParameter( "output_image_ptr", output_image_ptr );
  segmenterParams.SetParameter( "strategy_params", strategy_params );
  //segmenterParams.SetParameter( "max_seg_threads", (unsigned int)0 );
  //segmenterParams.SetParameter( "max_block_size", (unsigned int)100 );
  //segmenterParams.SetParameter( "merge_adjacent_blocks", (bool)false );
  
  // Running segmenter
  
  TePDIParallelSegmenter segmenter;
  
  TEAGN_TRUE_OR_THROW( segmenter.Apply( segmenterParams ), "Apply error" );
    
  // Saving result to disk
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_image_ptr,
    TEPDIEXAMPLESBINPATH "TePDIParallelSegmenter_Baatz_test.tif" ), 
    "GeoTIF generation error" );   
}


int main()
{
  TEAGN_LOGMSG( "Test started." );

  TeStdIOProgress pi;
  TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );     
  
  RegionGrowing_Strategy_pattern1_test();
  Baatz_Strategy_test();
  RegionGrowing_Strategy_test();

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}

