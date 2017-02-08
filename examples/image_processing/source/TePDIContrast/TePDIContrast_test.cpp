#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIContrast.hpp>
#include <TePDIParameters.hpp>
#include <TePDIUtils.hpp>
#include <TePDIRgbPaletteFunctions.hpp>

#include <TeRaster.h>
#include <TeDataTypes.h>
#include <TeInitRasterDecoders.h>
#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TeTime.h>

#include <TeAgnostic.h>

#include <string>

void TePDIContrastMinMax_autolevels_test()
{
  TePDIParameters params;

  TePDIContrast::TePDIContrastType contrast_type = 
    TePDIContrast::TePDIContrastMinMax;
  params.SetParameter( "contrast_type", contrast_type );

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  params.SetParameter( "input_band", (int)0 );
  params.SetParameter( "output_band", (int)0 );
  
  TePDITypes::TePDILutPtrType outlut( new TePDITypes::TePDILutType );
  params.SetParameter( "outlut", outlut );

  TePDIContrast contra;

  TEAGN_TRUE_OR_THROW( contra.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( contra.Apply(),
    "Apply error" );
    
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "TePDIContrastMinMax_autolevels_test.tif" ), "GeoTIF generation error" );
    
  /* Checking the generated lut */
  
  TEAGN_CHECK_EPS( outlut->size(), 256, 0, "Invalid lut size" );
}


void TePDIContrastMinMax_no_input_image_test()
{
  TePDIParameters params;

  TePDIContrast::TePDIContrastType contrast_type = 
    TePDIContrast::TePDIContrastMinMax;
  params.SetParameter( "contrast_type", contrast_type );

  TePDIHistogram::pointer input_histogram( new TePDIHistogram );
  {
    TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
      std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
    TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
    
    input_histogram->reset( inRaster, 0, 0, TeBoxPixelIn );
    params.SetParameter( "input_histogram", input_histogram );
  }
  
  params.SetParameter( "output_channel_min_level", (double)0 );
  params.SetParameter( "output_channel_max_level", (double)255 );
  
  TePDITypes::TePDILutPtrType outlut( new TePDITypes::TePDILutType );
  params.SetParameter( "outlut", outlut );

  TePDIContrast contra;
  TEAGN_TRUE_OR_THROW( contra.Reset( params ),
    "Invalid Parameters" );  
  TEAGN_TRUE_OR_THROW( contra.Apply(),
    "Apply error" );
    
  /* Checking the generated lut */

  TEAGN_CHECK_EPS( outlut->size(), 180, 0, "Invalid lut size" );
}


void TePDIContrastLinear_plus_histo_test()
{
  TePDIParameters params;

  TePDIContrast::TePDIContrastType contrast_type = 
    TePDIContrast::TePDIContrastLinear;
  params.SetParameter( "contrast_type", contrast_type );

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );
  
  /* Creating the histogram */
  
  TePDIHistogram::pointer input_histogram( new TePDIHistogram );
  TEAGN_TRUE_OR_THROW( input_histogram->reset( inRaster, 0, 256, 
    TeBoxPixelIn ),
    "Histogram building error" );
  params.SetParameter( "input_histogram", input_histogram );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  params.SetParameter( "input_band", (int)0 );
  params.SetParameter( "output_band", (int)0 );

  params.SetParameter( "min_level", (double)46 );
  params.SetParameter( "max_level", (double)102 );
  
  TePDITypes::TePDILutPtrType outlut( new TePDITypes::TePDILutType );
  params.SetParameter( "outlut", outlut );  

  TePDIContrast contra;

  TEAGN_TRUE_OR_THROW( contra.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( contra.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "TePDIContrastLinear_plus_histo_test.tif" ), "GeoTIF generation error" );
}

void TePDIContrastSquareRoot_test()
{
  TePDIParameters params;

  TePDIContrast::TePDIContrastType contrast_type = 
    TePDIContrast::TePDIContrastSquareRoot;
  params.SetParameter( "contrast_type", contrast_type );

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  params.SetParameter( "input_band", (int)0 );
  params.SetParameter( "output_band", (int)0 );

  params.SetParameter( "histo_levels", (int)256 );

  params.SetParameter( "min_level", (double)46 );
  params.SetParameter( "max_level", (double)102 );
  
  TePDITypes::TePDILutPtrType outlut( new TePDITypes::TePDILutType );
  params.SetParameter( "outlut", outlut );  

  TePDIContrast contra;

  TEAGN_TRUE_OR_THROW( contra.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( contra.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "TePDIContrastSquareRoot_test.tif" ), "GeoTIF generation error" );
    
  /* Checking the generated lut */
  
  TEAGN_TRUE_OR_THROW( ( outlut->size() == 256 ), "Invalid lut size" );
}

void TePDIContrastSquare_test()
{
  TePDIParameters params;

  TePDIContrast::TePDIContrastType contrast_type = 
    TePDIContrast::TePDIContrastSquare;
  params.SetParameter( "contrast_type", contrast_type );

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  params.SetParameter( "input_band", (int)0 );
  params.SetParameter( "output_band", (int)0 );

  params.SetParameter( "histo_levels", (int)256 );

  params.SetParameter( "min_level", (double)46 );
  params.SetParameter( "max_level", (double)102 );
  
  TePDITypes::TePDILutPtrType outlut( new TePDITypes::TePDILutType );
  params.SetParameter( "outlut", outlut );  

  TePDIContrast contra;

  TEAGN_TRUE_OR_THROW( contra.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( contra.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "TePDIContrastSquare_test.tif" ), "GeoTIF generation error" );
    
  /* Checking the generated lut */
  
  TEAGN_TRUE_OR_THROW( ( outlut->size() == 256 ), "Invalid lut size" );
}

void TePDIContrastLog_test()
{
  TePDIParameters params;

  TePDIContrast::TePDIContrastType contrast_type = 
    TePDIContrast::TePDIContrastLog;
  params.SetParameter( "contrast_type", contrast_type );

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  params.SetParameter( "input_band", (int)0 );
  params.SetParameter( "output_band", (int)0 );

  params.SetParameter( "histo_levels", (int)256 );

  params.SetParameter( "min_level", (double)46 );
  params.SetParameter( "max_level", (double)102 );
  
  TePDITypes::TePDILutPtrType outlut( new TePDITypes::TePDILutType );
  params.SetParameter( "outlut", outlut );  

  TePDIContrast contra;

  TEAGN_TRUE_OR_THROW( contra.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( contra.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "TePDIContrastLog_test.tif" ), "GeoTIF generation error" );
    
  /* Checking the generated lut */
  
  TEAGN_TRUE_OR_THROW( ( outlut->size() == 256 ), "Invalid lut size" );
}


void TePDIContrastLog_with_use_dummy_test()
{
  TePDIParameters params;

  TePDIContrast::TePDIContrastType contrast_type = 
    TePDIContrast::TePDIContrastLog;
  params.SetParameter( "contrast_type", contrast_type );

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  params.SetParameter( "input_band", (int)0 );
  params.SetParameter( "output_band", (int)0 );

  params.SetParameter( "histo_levels", (int)256 );

  params.SetParameter( "min_level", (double)46 );
  params.SetParameter( "max_level", (double)102 );
  
  TePDITypes::TePDILutPtrType outlut( new TePDITypes::TePDILutType );
  params.SetParameter( "outlut", outlut );  
  
  params.SetParameter( "dummy_value", (double)0 );  

  TePDIContrast contra;

  TEAGN_TRUE_OR_THROW( contra.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( contra.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "TePDIContrastLog_with_use_dummy_test.tif" ), "GeoTIF generation error" );
    
  /* Checking the generated lut */
  
  TEAGN_TRUE_OR_THROW( ( outlut->size() == 256 ), "Invalid lut size" );
}


void TePDIContrastNegative_test()
{
  TePDIParameters params;

  TePDIContrast::TePDIContrastType contrast_type = 
    TePDIContrast::TePDIContrastNegative;
  params.SetParameter( "contrast_type", contrast_type );

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  params.SetParameter( "input_band", (int)0 );
  params.SetParameter( "output_band", (int)0 );

  params.SetParameter( "histo_levels", (int)256 );

  params.SetParameter( "min_level", (double)46 );
  params.SetParameter( "max_level", (double)102 );
  
  TePDITypes::TePDILutPtrType outlut( new TePDITypes::TePDILutType );
  params.SetParameter( "outlut", outlut );  

  TePDIContrast contra;

  TEAGN_TRUE_OR_THROW( contra.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( contra.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "TePDIContrastNegative_test.tif" ), "GeoTIF generation error" );
    
  /* Checking the generated lut */
  
  TEAGN_TRUE_OR_THROW( ( outlut->size() == 256 ), "Invalid lut size" );
}

void TePDIContrastHistEqualizer_test()
{
  TePDIParameters params;

  TePDIContrast::TePDIContrastType contrast_type = 
    TePDIContrast::TePDIContrastHistEqualizer;
  params.SetParameter( "contrast_type", contrast_type );

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  params.SetParameter( "input_band", (int)0 );
  params.SetParameter( "output_band", (int)0 );

  params.SetParameter( "histo_levels", (int)256 );

  params.SetParameter( "min_level", (double)46 );
  params.SetParameter( "max_level", (double)102 );
  
  TePDITypes::TePDILutPtrType outlut( new TePDITypes::TePDILutType );
  params.SetParameter( "outlut", outlut );  

  TePDIContrast contra;

  TEAGN_TRUE_OR_THROW( contra.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( contra.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "TePDIContrastHistEqualizer_test.tif" ), "GeoTIF generation error" );
    
  /* Checking the generated lut */
  
  TEAGN_CHECK_EPS( outlut->size(), 180, 0, "Invalid lut size" );
}

void TePDIContrastSimpleSlicer_test()
{
  TePDIParameters params;

  TePDIContrast::TePDIContrastType contrast_type = 
    TePDIContrast::TePDIContrastSimpleSlicer;
  params.SetParameter( "contrast_type", contrast_type );

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  params.SetParameter( "input_band", (int)0 );
  params.SetParameter( "output_band", (int)0 );

  params.SetParameter( "histo_levels", (int)( 102 - 47 + 1 ) );

  params.SetParameter( "min_level", (double)47 );
  params.SetParameter( "max_level", (double)102 );
  
  TePDITypes::TePDILutPtrType outlut( new TePDITypes::TePDILutType );
  params.SetParameter( "outlut", outlut );  

  TePDIRgbPalette::pointer pal =
    TePDIRgbPaletteFunctions::createLSBPalette( 20 );
  params.SetParameter( "rgb_palette", pal );

  TePDIContrast contra;

  TEAGN_TRUE_OR_THROW( contra.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( contra.Apply(),
    "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "TePDIContrastSimpleSlicer_test.tif" ), "GeoTIF generation error" );
    
  /* Checking the generated lut */
  
  TEAGN_TRUE_OR_THROW( ( outlut->size() == 56 ), "Invalid lut size" );
}


void TePDIContrastStat_test()
{
  /* from "../resources/cbers_b2_crop_contraste.tif" */   
  double target_mean = 121.809123;
  double target_variance = 1561.214764;
  
  /* Building contrast parameters */

  TePDIParameters params;

  TePDIContrast::TePDIContrastType contrast_type = 
    TePDIContrast::TePDIContrastStat;
  params.SetParameter( "contrast_type", contrast_type );

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  params.SetParameter( "input_image", inRaster );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );
  params.SetParameter( "output_image", outRaster );

  params.SetParameter( "input_band", (int)0 );
  params.SetParameter( "output_band", (int)0 );
  params.SetParameter( "target_mean", target_mean );
  params.SetParameter( "target_variance", target_variance );

  TePDIContrast contra;

  TEAGN_TRUE_OR_THROW( contra.Reset( params ),
    "Invalid Parameters" );

  TEAGN_TRUE_OR_THROW( contra.Apply(),
    "Apply error" );
    
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "TePDIContrastStat_test.tif" ), "GeoTIF generation error" );
}


int main()
{
  TEAGN_LOGMSG( "Test started." );
    
  TeStdIOProgress pi;
  TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );    

  TePDIContrastLinear_plus_histo_test();
  TePDIContrastStat_test();
  TePDIContrastMinMax_autolevels_test();
  TePDIContrastMinMax_no_input_image_test();
  TePDIContrastSquareRoot_test();
  TePDIContrastSquare_test();
  TePDIContrastLog_test();
  TePDIContrastLog_with_use_dummy_test();
  TePDIContrastNegative_test();
  TePDIContrastHistEqualizer_test();
  TePDIContrastSimpleSlicer_test();

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
