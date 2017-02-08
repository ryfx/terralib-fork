#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>
#include <TePDIMorfFilter.hpp>
#include <TePDIFilterMask.hpp>
#include <TePDIParameters.hpp>
#include <TeAgnostic.h>
#include <TePDIUtils.hpp>
#include <TeProgress.h>
#include <TeStdIOProgress.h>

void Dilation_1_iteration_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMDilationType );

  params.SetParameter( "iterations", (int)1 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );
    
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Dilation_test_1_iteration.tif" ), "GeoTIF generation error" );
}


void Dilation_2_iterations_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMDilationType );

  params.SetParameter( "iterations", (int)2 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );
    
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Dilation_test_2_iterations.tif" ), "GeoTIF generation error" );
}


void Dilation_3_iterations_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMDilationType );

  params.SetParameter( "iterations", (int)3 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );
    
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Dilation_test_3_iterations.tif" ), "GeoTIF generation error" );
}


void Dilation_4_iterations_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMDilationType );

  params.SetParameter( "iterations", (int)4 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );
    
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Dilation_test_4_iterations.tif" ), "GeoTIF generation error" );
}


void Dilation_with_dummy_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType diskRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( diskRaster->init(), "Unable to init diskRaster" );
  
  TePDITypes::TePDIRasterPtrType inRaster;
  TeRasterParams inRaster_params = diskRaster->params();
  inRaster_params.nBands( 3 );
  inRaster_params.setDummy( 0, -1 );
  inRaster_params.setPhotometric( TeRasterParams::TeMultiBand, -1 );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( inRaster,
    inRaster_params, TePDIUtils::TePDIUtilsAutoMemPol ), 
    "Unable to alloc RAM Raster" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeCopyRasterPixels( diskRaster, 0,
    inRaster, 0, false, false ), "Unable to copy raster pixels" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeCopyRasterPixels( diskRaster, 0,
    inRaster, 1, false, false ), "Unable to copy raster pixels" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeCopyRasterPixels( diskRaster, 0,
    inRaster, 2, false, false ), "Unable to copy raster pixels" );
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

  params.SetParameter( "filter_mask", TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMDilationType );

  params.SetParameter( "iterations", (int)3 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Dilation_with_dummy_test.tif" ), "GeoTIF generation error" );
}


void Erosion_1_iteration_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMErosionType );

  params.SetParameter( "iterations", (int)1 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Erosion_1_iteration_test.tif" ), "GeoTIF generation error" );
}


void Erosion_2_iterations_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMErosionType );

  params.SetParameter( "iterations", (int)2 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Erosion_2_iterations_test.tif" ), "GeoTIF generation error" );
}


void Erosion_3_iterations_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMErosionType );

  params.SetParameter( "iterations", (int)3 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Erosion_3_iteration_test.tif" ), "GeoTIF generation error" );
}


void Erosion_4_iterations_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMErosionType );

  params.SetParameter( "iterations", (int)4 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Erosion_4_iteration_test.tif" ), "GeoTIF generation error" );
}


void Erosion_with_dummy_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType diskRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( diskRaster->init(), "Unable to init diskRaster" );
  
  TePDITypes::TePDIRasterPtrType inRaster;
  TeRasterParams inRaster_params = diskRaster->params();
  inRaster_params.nBands( 3 );
  inRaster_params.setDummy( 0, -1 );
  inRaster_params.setPhotometric( TeRasterParams::TeMultiBand, -1 );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( inRaster,
    inRaster_params, TePDIUtils::TePDIUtilsAutoMemPol ), 
    "Unable to alloc RAM Raster" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeCopyRasterPixels( diskRaster, 0,
    inRaster, 0, false, false ), "Unable to copy raster pixels" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeCopyRasterPixels( diskRaster, 0,
    inRaster, 1, false, false ), "Unable to copy raster pixels" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeCopyRasterPixels( diskRaster, 0,
    inRaster, 2, false, false ), "Unable to copy raster pixels" );
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

  params.SetParameter( "filter_mask", TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMErosionType );

  params.SetParameter( "iterations", (int)3 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Erosion_with_dummy_test.tif" ), "GeoTIF generation error" );
}


void Median_1_iteration_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMMedianType );

  params.SetParameter( "iterations", (int)1 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Median_1_iteration_test.tif" ), "GeoTIF generation error" );
}


void Median_2_iterations_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMMedianType );

  params.SetParameter( "iterations", (int)2 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Median_2_iterations_test.tif" ), "GeoTIF generation error" );
}


void Median_3_iterations_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMMedianType );

  params.SetParameter( "iterations", (int)3 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Median_3_iterations_test.tif" ), "GeoTIF generation error" );
}


void Median_4_iterations_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMMedianType );

  params.SetParameter( "iterations", (int)4 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Median_4_iterations_test.tif" ), "GeoTIF generation error" );
}


void Median_with_dummy_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType diskRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( diskRaster->init(), "Unable to init diskRaster" );
  
  TePDITypes::TePDIRasterPtrType inRaster;
  TeRasterParams inRaster_params = diskRaster->params();
  inRaster_params.nBands( 3 );
  inRaster_params.setDummy( 0, -1 );
  inRaster_params.setPhotometric( TeRasterParams::TeMultiBand, -1 );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( inRaster,
    inRaster_params, TePDIUtils::TePDIUtilsAutoMemPol ), 
    "Unable to alloc RAM Raster" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeCopyRasterPixels( diskRaster, 0,
    inRaster, 0, false, false ), "Unable to copy raster pixels" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeCopyRasterPixels( diskRaster, 0,
    inRaster, 1, false, false ), "Unable to copy raster pixels" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeCopyRasterPixels( diskRaster, 0,
    inRaster, 2, false, false ), "Unable to copy raster pixels" );
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

  params.SetParameter( "filter_mask", TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMMedianType );

  params.SetParameter( "iterations", (int)3 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Median_with_dummy_test.tif" ), "GeoTIF generation error" );
}


void Mode_1_iteration_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", 
    TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", 
    TePDIMorfFilter::TePDIMModeType );

  params.SetParameter( "iterations", (int)1 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Mode_1_iteration_test.tif" ), 
    "GeoTIF generation error" );
}

void Mode_2_iteration_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", 
    TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", 
    TePDIMorfFilter::TePDIMModeType );

  params.SetParameter( "iterations", (int)2 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Mode_2_iteration_test.tif" ), 
    "GeoTIF generation error" );
}

void Mode_3_iteration_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeDOUBLE, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  std::vector<int> channels;
  channels.push_back( 0 );
  params.SetParameter( "channels", channels );

  params.SetParameter( "filter_mask", 
    TePDIFilterMask::create_MorfMtot() );

  params.SetParameter( "filter_type", 
    TePDIMorfFilter::TePDIMModeType );

  params.SetParameter( "iterations", (int)3 );

  TePDIMorfFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Mode_3_iteration_test.tif" ), 
    "GeoTIF generation error" );
}

int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );

    Mode_1_iteration_test();
    Mode_2_iteration_test();
    Mode_3_iteration_test();
    Dilation_1_iteration_test();
    Dilation_2_iterations_test();
    Dilation_3_iterations_test();
    Dilation_4_iterations_test();
    Dilation_with_dummy_test();
    Erosion_1_iteration_test();
    Erosion_2_iterations_test();
    Erosion_3_iterations_test();
    Erosion_4_iterations_test();
    Erosion_with_dummy_test();
    Median_1_iteration_test();
    Median_2_iterations_test();
    Median_3_iterations_test();
    Median_4_iterations_test();
    Median_with_dummy_test();
  }
  catch( const TeException& e ){
    TEAGN_LOGERR( "Test Failed - " + e.message() );
    return EXIT_FAILURE;
  } 

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
