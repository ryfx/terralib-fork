#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIBDFilter.hpp>
#include <TePDIParameters.hpp>
#include <TeAgnostic.h>
#include <TePDIUtils.hpp>

#include <TeInitRasterDecoders.h>

#include <TeProgress.h>
#include <TeStdIOProgress.h>


void Sobel_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  channels.push_back( 1 );
  channels.push_back( 2 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_type", TePDIBDFilter::TePDISobel );

  params.SetParameter( "iterations", (int)2 );

  TePDIBDFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Sobel_test.tif" ), "GeoTIF generation error" );
}


void Roberts_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  channels.push_back( 1 );
  channels.push_back( 2 );  
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_type", TePDIBDFilter::TePDIRoberts );

  params.SetParameter( "iterations", (int)1 );

  TePDIBDFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Roberts_test.tif" ), "GeoTIF generation error" );
}


int main()
{
  TEAGN_LOGMSG( "Test started." );
  
  try{
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );
    
    TeInitRasterDecoders();

    Sobel_test();
    Roberts_test();
  }
  catch( const TeException& excpt ){
    TEAGN_LOGERR( excpt.message() )
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
