#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIRegister.hpp>

#include <TePDIParameters.hpp>
#include <TeAgnostic.h>
#include <TePDIUtils.hpp>

#include <TeInitRasterDecoders.h>
#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TePrecision.h>


void Register_test1()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType reference_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_A.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( reference_raster->init(), 
    "Unable to init reference_raster" );
    
  TePDITypes::TePDIRasterPtrType adjust_disk_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_B_contraste.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( adjust_disk_raster->init(), 
    "Unable to init adjust_disk_raster" );     
    
  TePDITypes::TePDIRasterPtrType adjust_raster( new TeRaster() );
  {
    TeRasterParams adjust_raster_params;
    
    adjust_raster_params.setDataType( TeUNSIGNEDCHAR, -1 );
    adjust_raster_params.nBands( 1 );
    adjust_raster_params.setNLinesNColumns( 
      adjust_disk_raster->params().nlines_,
      adjust_disk_raster->params().ncols_ );
    
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( adjust_raster,
      adjust_raster_params, false ), "adjust_raster Alloc error" );   
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeCopyRasterBands( adjust_disk_raster,
      adjust_raster, false, false ), "Pixel copy error" );
  }
    
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "output_raster Alloc error" );   
    
  TePDITypes::TeProjParamsPtrType proj_params( new TeProjectionParams );
  *proj_params = reference_raster->projection()->params();
  params.SetParameter( "proj_params" , proj_params );  

  params.SetParameter( "res_x" , reference_raster->params().resx_ );
  params.SetParameter( "res_y" , reference_raster->params().resy_ );
      
  params.SetParameter( "adjust_raster" , adjust_raster );
  params.SetParameter( "output_raster" , output_raster );
  params.SetParameter( "gt_name" , std::string( "affine" ) );
  params.SetParameter( "dummy_value" , (double)0.0 );
  params.SetParameter( "interpolator" , TePDIInterpolator::NNMethod );
  
  std::vector< int > adjust_channels;
  adjust_channels.push_back( 0 );
  params.SetParameter( "adjust_channels" , adjust_channels );
  
  TePDITypes::TeCoordPairVectPtrType tie_points( new TeCoordPairVect );
  
  tie_points->push_back( 
    TeCoordPair( TeCoord2D( 278, 269 ),
    reference_raster->index2Coord( TeCoord2D( 492, 502 ) ) ) );
  tie_points->push_back( 
    TeCoordPair( TeCoord2D( 282, 88 ),
    reference_raster->index2Coord( TeCoord2D( 495, 320 ) ) ) );  
  tie_points->push_back( 
    TeCoordPair( TeCoord2D( 66, 12 ),
    reference_raster->index2Coord( TeCoord2D( 279, 245 ) ) ) );  
    
    
  params.SetParameter( "tie_points" , tie_points );
  
  TePDIRegister algo; 
  TEAGN_TRUE_OR_THROW( algo.Reset(params), "Apply error" );
  TEAGN_TRUE_OR_THROW( algo.Apply(), "Apply error" );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster,
    TEPDIEXAMPLESBINPATH "Register_test1.tif" ), "GeoTIF generation error" );  
  
  TEAGN_CHECK_EPS( output_raster->params().nlines_, 
    adjust_disk_raster->params().nlines_, 7.0, "Invalid lines number" );
  TEAGN_CHECK_EPS( output_raster->params().ncols_, 
    adjust_disk_raster->params().ncols_, 2.0, "Invalid cols number" );
  TEAGN_CHECK_EPS( output_raster->params().nBands(), 
    adjust_disk_raster->params().nBands(), 0.0001, "Invalid bands number" );    
  TEAGN_CHECK_EPS( output_raster->params().resx_, 
    adjust_disk_raster->params().resx_, 0.0001, "Invalid x res" );
  TEAGN_CHECK_EPS( output_raster->params().resy_, 
    adjust_disk_raster->params().resy_, 0.0001, "Invalid y res" );
  TEAGN_CHECK_EPS( output_raster->params().box().x1(), 
    adjust_disk_raster->params().box().x1(), 29.0, "Invalid box x1" );    
  TEAGN_CHECK_EPS( output_raster->params().box().x2(), 
    adjust_disk_raster->params().box().x2(), 22.0, "Invalid box x2" );    
  TEAGN_CHECK_EPS( output_raster->params().box().y1(), 
    adjust_disk_raster->params().box().y1(), 100.0, "Invalid box y1" );    
  TEAGN_CHECK_EPS( output_raster->params().box().y2(), 
    adjust_disk_raster->params().box().y2(), 51.0, "Invalid box y2" );    

  reference_raster.reset();
  adjust_disk_raster.reset();
  adjust_raster.reset();
  output_raster.reset();
  tie_points.reset();
}

int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TeInitRasterDecoders();
    
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );     
    
    TePrecision::instance().setPrecision( 0.000000000001 );

    Register_test1();
  }
  catch( const TeException& e ){
    TEAGN_LOGERR( "Test Failed - " + e.message() );
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}

