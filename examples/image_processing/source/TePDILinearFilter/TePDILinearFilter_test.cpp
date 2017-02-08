#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDILinearFilter.hpp>
#include <TePDIFilterMask.hpp>
#include <TePDIParameters.hpp>
#include <TeAgnostic.h>
#include <TePDIUtils.hpp>

#include <TeInitRasterDecoders.h>
#include <TeProgress.h>
#include <TeStdIOProgress.h>

void DummyMask_RGB_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );
  
  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );  
  
  std::vector<int> channels;
  channels.push_back( 0 );
  channels.push_back( 1 );
  channels.push_back( 2 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_Dummy() );

  params.SetParameter( "iterations", (int)1 );

  TePDILinearFilter filter;  

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "TePDILinearFilter_DummyMask_RGB.tif" ), 
    "GeoTIF generation error" );  
    
}


void Avg3x3_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_Avg3x3() );

  params.SetParameter( "iterations", (int)3 );

  TePDILinearFilter filter;  

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "TePDILinearFilter_Avg3x3_test.tif" ), "GeoTIF generation error" );
}


void Avg7x7_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_Avg7x7() );

  params.SetParameter( "iterations", (int)2 );

  TePDILinearFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "TePDILinearFilter_Avg7x7_test.tif" ), "GeoTIF generation error" );
}


void NDHigh_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_NDHigh() );

  params.SetParameter( "iterations", (int)1 );

  params.SetParameter( "level_offset", (double)20 );

  TePDILinearFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "TePDILinearFilter_NDHigh_test.tif" ), "GeoTIF generation error" );
}


void S_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_S() );

  params.SetParameter( "iterations", (int)1 );

  TePDILinearFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "TePDILinearFilter_S_test.tif" ), "GeoTIF generation error" );
}



int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TeInitRasterDecoders();
    
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );     

    DummyMask_RGB_test();
    Avg3x3_test();
    Avg7x7_test();
    NDHigh_test();
    S_test();
  }
  catch( const TeException& excpt ){
    TEAGN_LOGERR( excpt.message() )
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
