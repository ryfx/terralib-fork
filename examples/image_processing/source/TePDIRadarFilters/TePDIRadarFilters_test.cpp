#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIRadarLeeFilter.hpp>
#include <TePDIRadarKuanFilter.hpp>
#include <TePDIRadarFrostFilter.hpp>
#include <TePDIRadarGammaFilter.hpp>

#include <TePDIParameters.hpp>
#include <TeAgnostic.h>
#include <TePDIUtils.hpp>

#include <TeInitRasterDecoders.h>

#include <TeProgress.h>
#include <TeStdIOProgress.h>

void LeeFixed_test()
{
  TePDIParameters params;

  // Despite this filter is intended to be used with radar images a 
  // CBERS image will be used just for testing purposes
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
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

  params.SetParameter( "filter_type", TePDIRadarLeeFilter::TePDILeeFixedType );

  params.SetParameter( "det_type", TePDIRadarLeeFilter::TePDILeeDTLinear );

  params.SetParameter( "iterations", (int)3 );

  params.SetParameter( "mask_width", (int)3 );

  params.SetParameter( "look_number", (double)1. );

  params.SetParameter( "variance_threshold", (double)0.1 );

  TePDIRadarLeeFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "RadarLeeFixed_test.tif" ), "GeoTIF generation error" );
  
}


void LeeAdaptative_test()
{
  TePDIParameters params;

  // Despite this filter is intended to be used with radar images a 
  // CBERS image will be used just for testing purposes
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
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

  params.SetParameter( "filter_type", TePDIRadarLeeFilter::TePDILeeAdaptType );

  params.SetParameter( "det_type", TePDIRadarLeeFilter::TePDILeeDTLinear );

  params.SetParameter( "iterations", (int)1 );

  params.SetParameter( "mask_width", (int)3 );

  params.SetParameter( "look_number", (double)1. );

  params.SetParameter( "variance_threshold", (double)0.1 );

  TePDIRadarLeeFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "RadarLeeAdaptative_test.tif" ), "GeoTIF generation error" );

}

void KuanFixed_test()
{
  TePDIParameters params;

  // Despite this filter is intended to be used with radar images a 
  // CBERS image will be used just for testing purposes
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
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

  params.SetParameter( "filter_type", TePDIRadarKuanFilter::TePDIKuanFixedType );

  params.SetParameter( "det_type", TePDIRadarKuanFilter::TePDIKuanDTLinear );

  params.SetParameter( "iterations", (int)3 );

  params.SetParameter( "mask_width", (int)3 );

  params.SetParameter( "look_number", (double)1. );

  params.SetParameter( "variance_threshold", (double)0.1 );

  TePDIRadarKuanFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "RadarKuanFixed_test.tif" ), "GeoTIF generation error" );
    
}


void KuanAdaptative_test()
{
  TePDIParameters params;

  // Despite this filter is intended to be used with radar images a 
  // CBERS image will be used just for testing purposes
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
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

  params.SetParameter( "filter_type", TePDIRadarKuanFilter::TePDIKuanAdaptType );

  params.SetParameter( "det_type", TePDIRadarKuanFilter::TePDIKuanDTLinear );

  params.SetParameter( "iterations", (int)3 );

  params.SetParameter( "mask_width", (int)3 );

  params.SetParameter( "look_number", (double)1. );

  params.SetParameter( "variance_threshold", (double)0.1 );

  TePDIRadarKuanFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "RadarKuanAdaptative_test.tif" ), "GeoTIF generation error" );

}


void Frost_test()
{
  TePDIParameters params;

  // Despite this filter is intended to be used with radar images a 
  // CBERS image will be used just for testing purposes
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
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

  params.SetParameter( "det_type", TePDIRadarFrostFilter::TePDIFrostDTLinear );

  params.SetParameter( "iterations", (int)3 );

  params.SetParameter( "look_number", (double)1. );

  params.SetParameter( "variance_threshold", (double)0.1 );

  params.SetParameter( "corr_coef", (double)0.89 );

  TePDIRadarFrostFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "RadarFrost_test.tif" ), "GeoTIF generation error" );
 
}

void Gamma_test()
{
  TePDIParameters params;

  // Despite this filter is intended to be used with radar images a 
  // CBERS image will be used just for testing purposes
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
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

  params.SetParameter( "filter_type", TePDIRadarGammaFilter::TePDIGammaFixedType );

  params.SetParameter( "det_type", TePDIRadarGammaFilter::TePDIGammaDTQuadratic );

  params.SetParameter( "iterations", (int)1 );

  params.SetParameter( "mask_width", (int)3 );

  params.SetParameter( "look_number", (double)1. );

  TePDIRadarGammaFilter filter;

  TEAGN_TRUE_OR_THROW( filter.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( filter.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "RadarGammaFixed_test.tif" ), "GeoTIF generation error" );
    
}


int main()
{
  TEAGN_LOGMSG( "Test started." );

  TeStdIOProgress pi;
  TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );  

  TeInitRasterDecoders();

  LeeFixed_test();
  LeeAdaptative_test();

  KuanFixed_test();
  KuanAdaptative_test();

  Frost_test();

  Gamma_test();

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
