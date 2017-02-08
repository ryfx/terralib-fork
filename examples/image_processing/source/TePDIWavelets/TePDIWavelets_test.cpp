#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIMallatWavelets.hpp>
#include <TePDIWaveletAtrous.hpp>
#include <TeAgnostic.h>
#include <TePDIUtils.hpp>

#include <TeInitRasterDecoders.h>
#include <TeProgress.h>
#include <TeStdIOProgress.h>


void Mallat_low_low_swap_test()
{
  /* Building rasters */

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster, 1, 1, 1, false,
    TeDOUBLE, 0 ), "RAM Raster Alloc error" );    
    
  TePDITypes::TePDIRasterPtrType lowlow;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( lowlow, 1, 1, 1, false,
    TeDOUBLE, 0 ), "Unable to create temporary low-low raster" );
    
 /* Building filters  - Reference coef 07 */
  
  std::vector< double > a_filter_l;
  a_filter_l.push_back( -0.05 );
  a_filter_l.push_back( 0.25 );
  a_filter_l.push_back( 0.6 );
  a_filter_l.push_back( 0.25 );
  a_filter_l.push_back( -0.05 );
  
  std::vector< double > a_filter_h;
  a_filter_h.push_back( 0.0107143 );
  a_filter_h.push_back( -0.0535714 );
  a_filter_h.push_back( -0.2607143 );
  a_filter_h.push_back( 0.6071429 );
  a_filter_h.push_back( -0.2607143 );
  a_filter_h.push_back( -0.0535714 );
  a_filter_h.push_back( 0.0107143 );
  
  std::vector< double > s_filter_l;
  s_filter_l.push_back( -0.0107143 );
  s_filter_l.push_back( -0.0535714 );
  s_filter_l.push_back( 0.2607143 );
  s_filter_l.push_back( 0.6071429 );
  s_filter_l.push_back( 0.2607143 );
  s_filter_l.push_back( -0.0535714 );
  s_filter_l.push_back( -0.0107143 );
  
  std::vector< double > s_filter_h;
  s_filter_h.push_back( -0.05 );
  s_filter_h.push_back( -0.25 );
  s_filter_h.push_back( 0.6 );
  s_filter_h.push_back( -0.25 );
  s_filter_h.push_back( -0.05 );  
  
  /* Building parameters for the two filters */  
  
  TePDIParameters params1;
  params1.SetParameter( "wavelets_type", std::string( "mallat" ) );
  params1.SetParameter( "filter_task", std::string( "SBExtract" ) );
  params1.SetParameter( "input_image", inRaster );
  params1.SetParameter( "band", (int)0 );
  params1.SetParameter( "a_filter_l", a_filter_l );
  params1.SetParameter( "a_filter_h", a_filter_h );
  params1.SetParameter( "s_filter_l", s_filter_l );
  params1.SetParameter( "s_filter_h", s_filter_h );
  params1.SetParameter( "filters_scale", (double)0.71 );         
  params1.SetParameter( "levels", (int)3 );  
  params1.SetParameter( "sub_band", lowlow );  
  params1.SetParameter( "sub_band_index", (int)0 );  
  
  TePDIParameters params2;
  params2.SetParameter( "wavelets_type", std::string( "mallat" ) );
  params2.SetParameter( "filter_task", std::string( "SBSwap" ) );
  params2.SetParameter( "input_image", inRaster );
  params2.SetParameter( "band", (int)0 );
  params2.SetParameter( "output_image", outRaster );
  params2.SetParameter( "a_filter_l", a_filter_l );
  params2.SetParameter( "a_filter_h", a_filter_h );
  params2.SetParameter( "s_filter_l", s_filter_l );
  params2.SetParameter( "s_filter_h", s_filter_h );    
  params2.SetParameter( "filters_scale", (double)0.71 );         
  params2.SetParameter( "levels", (int)3 );  
  params2.SetParameter( "sub_band", lowlow ); 
  params2.SetParameter( "sub_band_index", (int)0 );   
  
  /* starting filters */
  
  TePDIMallatWavelets filter1;  
  TEAGN_TRUE_OR_THROW( filter1.Reset( params1 ),
    "Invalid Parameters1" );
  TEAGN_TRUE_OR_THROW( filter1.Apply(), "Filter1 apply error" );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( lowlow,
    TEPDIEXAMPLESBINPATH "Wavelets_Mallat_llswap_test_lowlow.tif" ), "Low-low GeoTIF generation error" );  
    
  TePDIMallatWavelets filter2;  
  TEAGN_TRUE_OR_THROW( filter2.Reset( params2 ),
    "Invalid Parameters2" );  
  TEAGN_TRUE_OR_THROW( filter2.Apply(), "Filter2 apply error" );
  
  TEAGN_TRUE_OR_THROW( ( ( outRaster->params().nlines_ ==
    inRaster->params().nlines_ ) && ( outRaster->params().ncols_ ==
    inRaster->params().ncols_ ) ), "Invalid output raster dimensions" );  
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Wavelets_Mallat_llswap_test_recomposed.tif" ), 
    "Recomposition GeoTIF generation error" );   
    
}


void Mallat_GetPyramid_and_RecomposePyramid_test()
{
  /* Building rasters */

  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );

  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outRaster, 1, 1, 1, false,
    TeDOUBLE, 0 ), "RAM Raster Alloc error" );    
    
 /* Building filters  - Reference coef 07 */
  
  std::vector< double > a_filter_l;
  a_filter_l.push_back( -0.05 );
  a_filter_l.push_back( 0.25 );
  a_filter_l.push_back( 0.6 );
  a_filter_l.push_back( 0.25 );
  a_filter_l.push_back( -0.05 );
  
  std::vector< double > a_filter_h;
  a_filter_h.push_back( 0.0107143 );
  a_filter_h.push_back( -0.0535714 );
  a_filter_h.push_back( -0.2607143 );
  a_filter_h.push_back( 0.6071429 );
  a_filter_h.push_back( -0.2607143 );
  a_filter_h.push_back( -0.0535714 );
  a_filter_h.push_back( 0.0107143 );
  
  std::vector< double > s_filter_l;
  s_filter_l.push_back( -0.0107143 );
  s_filter_l.push_back( -0.0535714 );
  s_filter_l.push_back( 0.2607143 );
  s_filter_l.push_back( 0.6071429 );
  s_filter_l.push_back( 0.2607143 );
  s_filter_l.push_back( -0.0535714 );
  s_filter_l.push_back( -0.0107143 );
  
  std::vector< double > s_filter_h;
  s_filter_h.push_back( -0.05 );
  s_filter_h.push_back( -0.25 );
  s_filter_h.push_back( 0.6 );
  s_filter_h.push_back( -0.25 );
  s_filter_h.push_back( -0.05 );  
  
  /* Applying filter 1 - Pyramid Generation */
  
  TePDITypes::TePDIRasterVectorPtrType pyramid( 
    new TePDITypes::TePDIRasterVectorType );
  
  TePDIParameters params1;
  params1.SetParameter( "wavelets_type", std::string( "mallat" ) );
  params1.SetParameter( "filter_task", std::string( "GetPyramid" ) );
  params1.SetParameter( "input_image", inRaster );
  params1.SetParameter( "band", (int)0 );
  params1.SetParameter( "a_filter_l", a_filter_l );
  params1.SetParameter( "a_filter_h", a_filter_h );
  params1.SetParameter( "s_filter_l", s_filter_l );
  params1.SetParameter( "s_filter_h", s_filter_h );
  params1.SetParameter( "filters_scale", (double)0.71 );         
  params1.SetParameter( "levels", (int)3 );  
  params1.SetParameter( "pyramid", pyramid );  
  
  TePDIMallatWavelets filter1;  
  
  TEAGN_TRUE_OR_THROW( filter1.Reset( params1 ),
    "Invalid Parameters1" );   
  TEAGN_TRUE_OR_THROW( filter1.Apply(), "Filter1 apply error" );
  
  /* Checking the generated pyramid */
  
  TEAGN_TRUE_OR_THROW( pyramid.isActive(), "Inactive generated pyramid" );
  TEAGN_TRUE_OR_THROW( ( pyramid->size() == 12 ), "Invalid pyramid size" );
  
  for( unsigned int py_index = 0 ; py_index < pyramid->size() ; ++py_index ) {
    TEAGN_TRUE_OR_THROW( (*pyramid)[ py_index ].isActive(),
      "Inactive pyramid raster found at " + Te2String( py_index ) );
  
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( 
      (*pyramid)[ py_index ],
      TEPDIEXAMPLESBINPATH "Wavelets_Mallat_OutPyramid_index" + Te2String( py_index ) + ".tif" ), 
      "GeoTIF generation error" );      
  }
  
  /* Applying the second filter - Pyramid Recomposition */
  
  TePDIParameters params2;
  params2.SetParameter( "wavelets_type", std::string( "mallat" ) );
  params2.SetParameter( "filter_task", std::string( "RecomposePyramid" ) );
  params2.SetParameter( "input_image", inRaster );
  params2.SetParameter( "band", (int)0 );
  params2.SetParameter( "output_image", outRaster );
  params2.SetParameter( "a_filter_l", a_filter_l );
  params2.SetParameter( "a_filter_h", a_filter_h );
  params2.SetParameter( "s_filter_l", s_filter_l );
  params2.SetParameter( "s_filter_h", s_filter_h );    
  params2.SetParameter( "filters_scale", (double)0.71 );
  params2.SetParameter( "pyramid", pyramid );
  
  TePDIMallatWavelets filter2;  
  TEAGN_TRUE_OR_THROW( filter2.Reset( params2 ),
    "Invalid Parameters2" );  
  TEAGN_TRUE_OR_THROW( filter2.Apply(), "Filter2 apply error" );
  
  TEAGN_TRUE_OR_THROW( ( ( outRaster->params().nlines_ ==
    inRaster->params().nlines_ ) && ( outRaster->params().ncols_ ==
    inRaster->params().ncols_ ) ), "Invalid output raster dimensions" );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outRaster,
    TEPDIEXAMPLESBINPATH "Wavelets_Mallat_GetPyramid_and_RecomposePyramid_test.tif" ), 
    "GeoTIF generation error" );     
}


void TePDIWaveletAtrous_test()
{
  TePDITypes::TePDIRasterVectorType output_wavelets;
  
  // Testing decomposition
  
  {
    TePDIParameters waveletAtrousParams;
    
    waveletAtrousParams.SetParameter("direction", 
      (int)TePDIWaveletAtrous::DECOMPOSE);
      
    TePDITypes::TePDIRasterPtrType input_raster( new TeRaster(
      std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
    TEAGN_TRUE_OR_THROW( input_raster->init(), "Unable to init inRaster" );    
    waveletAtrousParams.SetParameter("input_raster", input_raster);
    
    waveletAtrousParams.SetParameter("band", (int)0);
    
    waveletAtrousParams.SetParameter("levels", (int)2);
    
    waveletAtrousParams.SetParameter("filter_type", 
      TePDIWaveletAtrous::TriangleFilter );
      
    TeRasterParams outputWaveletParams = input_raster->params();
    outputWaveletParams.nBands( 2 ); // Desired Wavelet planes number
    outputWaveletParams.setDataType( TeFLOAT, -1 );
    TePDITypes::TePDIRasterPtrType output_wavelet0;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outputWaveletParams,
      output_wavelet0 ), "RAM Raster Alloc error" );
    TePDITypes::TePDIRasterPtrType output_wavelet1;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outputWaveletParams,
      output_wavelet1 ), "RAM Raster Alloc error" );
    TePDITypes::TePDIRasterPtrType output_wavelet2;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outputWaveletParams,
      output_wavelet2 ), "RAM Raster Alloc error" );          
    output_wavelets.push_back( output_wavelet0 );
    output_wavelets.push_back( output_wavelet1 );  
    output_wavelets.push_back( output_wavelet2 );
    waveletAtrousParams.SetParameter("output_wavelets", 
      output_wavelets);
    
    TePDIWaveletAtrous wa;
    TEAGN_TRUE_OR_THROW(wa.Apply( waveletAtrousParams ), "Apply error");
    
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( 
      output_wavelet0,
      TEPDIEXAMPLESBINPATH "TePDIWaveletAtrous_test_output_wavelet0.tif" ), 
      "GeoTIF generation error" );     
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( 
      output_wavelet1,
      TEPDIEXAMPLESBINPATH "TePDIWaveletAtrous_test_output_wavelet1.tif" ), 
      "GeoTIF generation error" );     
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( 
      output_wavelet2,
      TEPDIEXAMPLESBINPATH "TePDIWaveletAtrous_test_output_wavelet2.tif" ), 
      "GeoTIF generation error" );                 
  }
  
  // Testing recomposition
  
  {
    TePDIParameters waveletAtrousParams;
    
    waveletAtrousParams.SetParameter("direction", 
      (int)TePDIWaveletAtrous::RECOMPOSE);
      
    std::vector<TePDITypes::TePDIRasterVectorType> input_rasters_wavelets;
    input_rasters_wavelets.push_back( output_wavelets );
    waveletAtrousParams.SetParameter("input_rasters_wavelets", 
      input_rasters_wavelets );
      
    waveletAtrousParams.SetParameter("rasters_levels", (int)2);  
      
    // We are using output_wavelets as reference_raster_wavelets
    // just for test.
    waveletAtrousParams.SetParameter("reference_raster_wavelets", 
      output_wavelets );
      
    waveletAtrousParams.SetParameter("reference_levels", (int)2);  
      
    TeRasterParams output_raster_params = output_wavelets[ 0 ]->params();
    output_raster_params.nBands( 1 );
    output_raster_params.setDataType( TeUNSIGNEDCHAR, -1 );
    TePDITypes::TePDIRasterPtrType output_raster0;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster_params,
      output_raster0 ), "RAM Raster Alloc error" );
    TePDITypes::TePDIRasterPtrType output_raster1;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster_params,
      output_raster1 ), "RAM Raster Alloc error" );   
    TePDITypes::TePDIRasterPtrType output_raster2;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster_params,
      output_raster2 ), "RAM Raster Alloc error" );        
    TePDITypes::TePDIRasterVectorType output_rasters;
    output_rasters.push_back( output_raster0 );
    output_rasters.push_back( output_raster1 );  
    output_rasters.push_back( output_raster2 );
    waveletAtrousParams.SetParameter( "output_rasters", 
      output_rasters );
    
    TePDIWaveletAtrous wa;
    TEAGN_TRUE_OR_THROW(wa.Apply( waveletAtrousParams ), "Apply error");
    
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( 
      output_raster0,
      TEPDIEXAMPLESBINPATH "TePDIWaveletAtrous_test_output_raster0.tif" ), 
      "GeoTIF generation error" );   
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( 
      output_raster1,
      TEPDIEXAMPLESBINPATH "TePDIWaveletAtrous_test_output_raster1.tif" ), 
      "GeoTIF generation error" );    
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( 
      output_raster2,
      TEPDIEXAMPLESBINPATH "TePDIWaveletAtrous_test_output_raster2.tif" ), 
      "GeoTIF generation error" );                  
  }  
};


int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TeInitRasterDecoders();
    
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );     

    TePDIWaveletAtrous_test();
    Mallat_low_low_swap_test();
    Mallat_GetPyramid_and_RecomposePyramid_test();
  }
  catch( const TeException& e ){
    TEAGN_LOGERR( "Test Failed - " + e.message() );
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}

