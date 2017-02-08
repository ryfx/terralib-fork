#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIBlender.hpp>
#include <TePDIParameters.hpp>
#include <TeAgnostic.h>
#include <TePDIUtils.hpp>
#include <TePDIGeoMosaic.hpp>
#include <TePDIBatchGeoMosaic.hpp>
#include <TePDITPMosaic.hpp>

#include <TeInitRasterDecoders.h>
#include <TeProgress.h>
#include <TeStdIOProgress.h>

#include <vector> 

void TPMosaic_test1()
{
  // Initiating images
  
  TePDITypes::TePDIRasterPtrType input_raster1( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_C.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster1->init(), 
    "Unable to init input_raster1" );
    
  TePDITypes::TePDIRasterPtrType input_raster2( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_B_contraste.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster2->init(), 
    "Unable to init input_raster2" );    
    
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "output_raster Alloc error" );
    
  // The geometric transformation parameters telling how the input rasters are 
  // overlapped
  
  TeGTParams trans_params;
  trans_params.transformation_name_ = "affine";
  trans_params.out_rem_strat_ = TeGTParams::NoOutRemotion;  
  
  TeCoordPair auxPair;
  
  auxPair.pt1.setXY( 36,234 ); // point over input image 1
  auxPair.pt2.setXY( 245,21 ); // the corresponding point over input image 2
  trans_params.tiepoints_.push_back( auxPair );
  
  auxPair.pt1.setXY( 319,252 );
  auxPair.pt2.setXY( 528,39 );
  trans_params.tiepoints_.push_back( auxPair );  
  
  auxPair.pt1.setXY( 136,416 );
  auxPair.pt2.setXY( 345,203 );
  trans_params.tiepoints_.push_back( auxPair );
      
  // Other parameters
  
  std::vector< unsigned int > channels1;
  channels1.push_back( 0 );  
  
  std::vector< unsigned int > channels2;
  channels2.push_back( 0 );  
    
  // Creating algorithm parameters
    
  TePDIParameters params;
  params.SetParameter( "input_raster1" , input_raster1 );
  params.SetParameter( "input_raster2" , input_raster2 );
  params.SetParameter( "channels1" , channels1 );
  params.SetParameter( "channels2" , channels2 );
  params.SetParameter( "output_raster" , output_raster );
  params.SetParameter( "trans_params" , trans_params );
  params.SetParameter( "blend_method", TePDIBlender::EuclideanBlendMethod );
  params.SetParameter( "interp_method", TePDIInterpolator::NNMethod );
  params.SetParameter( "dummy_value" , (double)0.0 );
  params.SetParameter( "auto_equalize" , (int)1 );
  
  TePDITPMosaic mos;
  TEAGN_TRUE_OR_THROW( mos.Apply( params ), "Apply error" );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster,
    TEPDIEXAMPLESBINPATH "TePDITPMosaic_test1.tif" ), "GeoTIF generation error" );
}

// non-overlapped images test (simulated)
void TPMosaic_test2()
{
  // Initiating images
  
  TePDITypes::TePDIRasterPtrType input_raster1( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster1->init(), 
    "Unable to init input_raster1" );
    
  TePDITypes::TePDIRasterPtrType input_raster2( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop2.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster2->init(), 
    "Unable to init input_raster2" );    
    
  // pre-initializing the output raster
  
  TeRasterParams outputRasterParams;
  outputRasterParams.setDataType( TeUNSIGNEDCHAR, 1 );
  outputRasterParams.setNLinesNColumns( 1, 1 );
  outputRasterParams.nBands( 3 );
  outputRasterParams.setPhotometric( TeRasterParams::TeRGB, -1 );  
    
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outputRasterParams,
    output_raster ), "outputRaster creation error" );
    
  // The geometric transformation parameters telling how the input rasters are 
  // overlapped
  
  TeGTParams trans_params;
  trans_params.transformation_name_ = "affine";
  trans_params.out_rem_strat_ = TeGTParams::NoOutRemotion;  
  
  TeCoordPair auxPair;
  
  auxPair.pt1.setXY( 885,0 ); // point over input image 1
  auxPair.pt2.setXY( 0,0 ); // TopLeft - image 2
  trans_params.tiepoints_.push_back( auxPair );
  
  auxPair.pt1.setXY( 885 + 838,0 );
  auxPair.pt2.setXY( 837,0 ); // TopRight - image 2
  trans_params.tiepoints_.push_back( auxPair );  
  
  auxPair.pt1.setXY( 885,1009 );
  auxPair.pt2.setXY( 0,1008 ); // LowerLeft - image 2
  trans_params.tiepoints_.push_back( auxPair );  
  
  auxPair.pt1.setXY( 885 + 838,1009 );
  auxPair.pt2.setXY( 837,1008 ); // LowerRight - image 2
  trans_params.tiepoints_.push_back( auxPair ); 
      
  // Other parameters
  
  std::vector< unsigned int > channels1;
  channels1.push_back( 0 );
  channels1.push_back( 1 );  
  channels1.push_back( 2 );    
  
  std::vector< unsigned int > channels2;
  channels2.push_back( 0 );
  channels2.push_back( 1 );  
  channels2.push_back( 2 );    
    
  // Creating algorithm parameters
    
  TePDIParameters params;
  params.SetParameter( "input_raster1" , input_raster1 );
  params.SetParameter( "input_raster2" , input_raster2 );
  params.SetParameter( "channels1" , channels1 );
  params.SetParameter( "channels2" , channels2 );
  params.SetParameter( "output_raster" , output_raster );
  params.SetParameter( "trans_params" , trans_params );
  params.SetParameter( "blend_method", TePDIBlender::EuclideanBlendMethod );
  params.SetParameter( "interp_method", TePDIInterpolator::NNMethod );
  params.SetParameter( "dummy_value" , (double)0.0 );
  params.SetParameter( "auto_equalize" , (int)1 );
  
  TePDITPMosaic mos;
  TEAGN_TRUE_OR_THROW( mos.Apply( params ), "Apply error" );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster,
    TEPDIEXAMPLESBINPATH "TePDITPMosaic_test2.tif" ), "GeoTIF generation error" );
}

void GeoMosaic_test1()
{
  // Initiating images
  
  TePDITypes::TePDIRasterPtrType input_raster1( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_A.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster1->init(), 
    "Unable to init input_raster1" );
    
  TePDITypes::TePDIRasterPtrType input_raster2( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_B_contraste.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster2->init(), 
    "Unable to init input_raster2" );    
    
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "output_raster Alloc error" );
    
  // Creating algorithm parameters
    
  TePDIParameters params;
    
  params.SetParameter( "blend_method", TePDIBlender::EuclideanBlendMethod );
  params.SetParameter( "input_raster1" , input_raster1 );
  params.SetParameter( "input_raster2" , input_raster2 );
  params.SetParameter( "output_raster" , output_raster );
  
  std::vector< unsigned int > channels1;
  channels1.push_back( 0 );
  params.SetParameter( "channels1" , channels1 );
  
  std::vector< unsigned int > channels2;
  channels2.push_back( 0 );
  params.SetParameter( "channels2" , channels2 );
  
  params.SetParameter( "interp_method", TePDIInterpolator::BilinearMethod );
  params.SetParameter( "dummy_value" , (double)0. );
  params.SetParameter( "auto_equalize" , (int)1 );
  
  TePDIGeoMosaic mos;
  TEAGN_TRUE_OR_THROW( mos.Apply( params ), "Apply error" );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster,
    TEPDIEXAMPLESBINPATH "TePDIGeoMosaic_test1.tif" ), "GeoTIF generation error" );
}

void GeoMosaic_test2()
{
  // Initiating images
  
  TePDITypes::TePDIRasterPtrType input_raster1( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_C.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster1->init(), 
    "Unable to init input_raster1" );
    
  TePDITypes::TePDIRasterPtrType input_raster2( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_B_contraste.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster2->init(), 
    "Unable to init input_raster2" );    
    
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "output_raster Alloc error" );
    
  // Creating algorithm parameters
    
  TePDIParameters params;
    
  params.SetParameter( "blend_method", TePDIBlender::EuclideanBlendMethod );
  params.SetParameter( "input_raster1" , input_raster1 );
  params.SetParameter( "input_raster2" , input_raster2 );
  params.SetParameter( "output_raster" , output_raster );
  
  std::vector< unsigned int > channels1;
  channels1.push_back( 0 );
  params.SetParameter( "channels1" , channels1 );
  
  std::vector< unsigned int > channels2;
  channels2.push_back( 0 );
  params.SetParameter( "channels2" , channels2 );
  
  params.SetParameter( "interp_method", TePDIInterpolator::BilinearMethod );
  params.SetParameter( "dummy_value" , (double)0. );
  params.SetParameter( "auto_equalize" , (int)1 );
  
  TePDIGeoMosaic mos;
  TEAGN_TRUE_OR_THROW( mos.Apply( params ), "Apply error" );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster,
    TEPDIEXAMPLESBINPATH "TePDIGeoMosaic_test2.tif" ), "GeoTIF generation error" );
}

// Non-overlapped images test
void GeoMosaic_test3()
{
  // Initiating images
  
  TePDITypes::TePDIRasterPtrType input_raster1( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_chip0_0.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster1->init(), 
    "Unable to init input_raster1" );
    
  TePDITypes::TePDIRasterPtrType input_raster2( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_chip500_500.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster2->init(), 
    "Unable to init input_raster2" );  
      
  // pre-initializing the output raster
  
  TeRasterParams outputRasterParams;
  outputRasterParams.setDataType( TeUNSIGNEDCHAR, 1 );
  outputRasterParams.setNLinesNColumns( 1, 1 );
  outputRasterParams.nBands( 3 );
  outputRasterParams.setPhotometric( TeRasterParams::TeRGB, -1 );  
    
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outputRasterParams,
    output_raster ), "outputRaster creation error" );      
    
  // Creating algorithm parameters
    
  TePDIParameters params;
    
  params.SetParameter( "blend_method", TePDIBlender::EuclideanBlendMethod );
  params.SetParameter( "input_raster1" , input_raster1 );
  params.SetParameter( "input_raster2" , input_raster2 );
  params.SetParameter( "output_raster" , output_raster );
  
  std::vector< unsigned int > channels1;
  channels1.push_back( 0 );
  channels1.push_back( 1 );
  channels1.push_back( 2 );
  params.SetParameter( "channels1" , channels1 );
  
  std::vector< unsigned int > channels2;
  channels2.push_back( 0 );
  channels2.push_back( 1 );
  channels2.push_back( 2 );
  params.SetParameter( "channels2" , channels2 );
  
  params.SetParameter( "interp_method", TePDIInterpolator::BilinearMethod );
  params.SetParameter( "dummy_value" , (double)0. );
  //params.SetParameter( "auto_equalize" , (int)1 );
  
  TePDIGeoMosaic mos;
  TEAGN_TRUE_OR_THROW( mos.Apply( params ), "Apply error" );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster,
    TEPDIEXAMPLESBINPATH "TePDIGeoMosaic_test3.tif" ), "GeoTIF generation error" );
}

// Testing images with different resolutions
void GeoMosaic_test4()
{
  // Initiating images
  
  TePDITypes::TePDIRasterPtrType input_raster1( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_halfsampled_cropB.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster1->init(), 
    "Unable to init input_raster1" );
    
  TePDITypes::TePDIRasterPtrType input_raster2( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_cropA.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster2->init(), 
    "Unable to init input_raster2" );    
    
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "output_raster Alloc error" );
    
  // Creating algorithm parameters
    
  TePDIParameters params;
    
  params.SetParameter( "blend_method", TePDIBlender::EuclideanBlendMethod );
  params.SetParameter( "input_raster1" , input_raster1 );
  params.SetParameter( "input_raster2" , input_raster2 );
  params.SetParameter( "output_raster" , output_raster );
  
  std::vector< unsigned int > channels1;
  channels1.push_back( 2 );
  params.SetParameter( "channels1" , channels1 );
  
  std::vector< unsigned int > channels2;
  channels2.push_back( 0 );
  params.SetParameter( "channels2" , channels2 );
  
  params.SetParameter( "interp_method", TePDIInterpolator::NNMethod );
  params.SetParameter( "dummy_value" , (double)0. );
  //params.SetParameter( "auto_equalize" , (int)1 );
  
  TePDIGeoMosaic mos;
  TEAGN_TRUE_OR_THROW( mos.Apply( params ), "Apply error" );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster,
    TEPDIEXAMPLESBINPATH "TePDIGeoMosaic_test4.tif" ), "GeoTIF generation error" );
}

void BatchGeoMosaic_test1()
{
  TePDIParameters params1;

  /* Building rasters */

  TePDITypes::TePDIRasterPtrType input_raster1( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_A.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster1->init(), 
    "Unable to init input_raster1" );
    
  TePDITypes::TePDIRasterPtrType input_raster2( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_B_contraste.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster2->init(), 
    "Unable to init input_raster2" ); 
    
  TePDITypes::TePDIRasterPtrType input_raster3( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_C.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster3->init(), 
    "Unable to init input_raster2" );      
    
  std::vector< unsigned int > bands;
  bands.push_back( 0 );  // from raster 1
  bands.push_back( 0 );  // from raster 2 
  bands.push_back( 0 );  // from raster 3 
    
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster,
    1, 1, 1, false, TeDOUBLE, 0 ), "output_raster Alloc error" );
    
  TePDITypes::TePDIRasterVectorType input_rasters;
  input_rasters.push_back( input_raster1 );
  input_rasters.push_back( input_raster2 );
  input_rasters.push_back( input_raster3 );
  
  // Building algorithm parameters
    
  TePDIParameters params;
  params.SetParameter( "input_rasters" , input_rasters );
  params.SetParameter( "bands" , bands );
  params.SetParameter( "output_raster" , output_raster );
  params.SetParameter( "blend_method", TePDIBlender::EuclideanBlendMethod );
  params.SetParameter( "interp_method", TePDIInterpolator::BilinearMethod );
  params.SetParameter( "dummy_value" , (double)0. );
  params.SetParameter( "auto_equalize" , (int)1 );
  
  TePDIBatchGeoMosaic mos;
  TEAGN_TRUE_OR_THROW( mos.Apply( params ), "Apply error" );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster,
    TEPDIEXAMPLESBINPATH "TePDIBatchGeoMosaic_test1.tif" ), 
    "GeoTIF generation error" );
}

// Non-overlapped images test
void BatchGeoMosaic_test2()
{
  TePDIParameters params1;

  /* Initializing input rasters */

  TePDITypes::TePDIRasterPtrType input_raster1( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_chip0_0.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster1->init(), 
    "Unable to init input raster" );
    
  TePDITypes::TePDIRasterPtrType input_raster2( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_chip0_250.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster2->init(), 
    "Unable to init input raster" ); 
    
  TePDITypes::TePDIRasterPtrType input_raster3( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_chip0_500.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster3->init(), 
    "Unable to init input raster" );      
    
  TePDITypes::TePDIRasterPtrType input_raster4( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_chip0_750.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster4->init(), 
    "Unable to init input raster" );      
    
  TePDITypes::TePDIRasterPtrType input_raster5( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_chip250_0.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster5->init(), 
    "Unable to init input raster" );      
    
  TePDITypes::TePDIRasterPtrType input_raster6( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_chip250_250.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster6->init(), 
    "Unable to init input raster" );      
    
  TePDITypes::TePDIRasterPtrType input_raster7( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_chip250_500.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster7->init(), 
    "Unable to init input raster" );      
    
  TePDITypes::TePDIRasterPtrType input_raster8( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_chip250_750.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster8->init(), 
    "Unable to init input raster" );      
    
  TePDITypes::TePDIRasterPtrType input_raster9( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_chip500_0.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster9->init(), 
    "Unable to init input raster" );      
    
  TePDITypes::TePDIRasterPtrType input_raster10( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_chip500_250.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster10->init(), 
    "Unable to init input raster" );      
    
  TePDITypes::TePDIRasterPtrType input_raster11( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_chip500_500.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster11->init(), 
    "Unable to init input raster" );      
    
  TePDITypes::TePDIRasterPtrType input_raster12( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_chip500_750.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster12->init(), 
    "Unable to init input raster" );      
    
  // Input bands from each raster  
    
  std::vector< unsigned int > bands;
  
  bands.push_back( 0 );  // from raster 0 
  bands.push_back( 1 );  // from raster 0 
  bands.push_back( 2 );  // from raster 0
  
  bands.push_back( 0 );  // from raster 1 
  bands.push_back( 1 );  // from raster 1 
  bands.push_back( 2 );  // from raster 1 
  
  bands.push_back( 0 );  // from raster 2 
  bands.push_back( 1 );  // from raster 2 
  bands.push_back( 2 );  // from raster 2 
  
  bands.push_back( 0 );   
  bands.push_back( 1 );   
  bands.push_back( 2 );   
  
  bands.push_back( 0 );   
  bands.push_back( 1 );   
  bands.push_back( 2 );  
   
  bands.push_back( 0 );   
  bands.push_back( 1 );   
  bands.push_back( 2 );  
   
  bands.push_back( 0 );   
  bands.push_back( 1 );   
  bands.push_back( 2 );  
   
  bands.push_back( 0 );   
  bands.push_back( 1 );   
  bands.push_back( 2 );  
   
  bands.push_back( 0 );   
  bands.push_back( 1 );   
  bands.push_back( 2 );  
   
  bands.push_back( 0 );   
  bands.push_back( 1 );   
  bands.push_back( 2 );   
  
  bands.push_back( 0 );   
  bands.push_back( 1 );   
  bands.push_back( 2 ); 
    
  bands.push_back( 0 );   
  bands.push_back( 1 );   
  bands.push_back( 2 );   
  
  // pre-initializing the output raster
  
  TeRasterParams outputRasterParams;
  outputRasterParams.setDataType( TeUNSIGNEDCHAR, 1 );
  outputRasterParams.setNLinesNColumns( 1, 1 );
  outputRasterParams.nBands( 3 );
  outputRasterParams.setPhotometric( TeRasterParams::TeRGB, -1 );  
    
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outputRasterParams,
    output_raster ), "outputRaster creation error" );     
    
  // Creating the input rasters pointers vector
    
  TePDITypes::TePDIRasterVectorType input_rasters;
  input_rasters.push_back( input_raster1 );
  input_rasters.push_back( input_raster2 );
  input_rasters.push_back( input_raster3 );
  input_rasters.push_back( input_raster4 );
  input_rasters.push_back( input_raster5 );
  input_rasters.push_back( input_raster6 );
  input_rasters.push_back( input_raster7 );
  input_rasters.push_back( input_raster8 );
  input_rasters.push_back( input_raster9 );
  input_rasters.push_back( input_raster10 );
  input_rasters.push_back( input_raster11 );
  input_rasters.push_back( input_raster12 );
  
  // Building algorithm parameters
    
  TePDIParameters params;
  params.SetParameter( "input_rasters" , input_rasters );
  params.SetParameter( "bands" , bands );
  params.SetParameter( "output_raster" , output_raster );
  params.SetParameter( "blend_method", TePDIBlender::NoBlendMethod );
  //params.SetParameter( "blend_method", TePDIBlender::MeanBlendMethod );
  //params.SetParameter( "blend_method", TePDIBlender::EuclideanBlendMethod );
  //params.SetParameter( "interp_method", TePDIInterpolator::NNMethod );
  //params.SetParameter( "interp_method", TePDIInterpolator::BilinearMethod );
  params.SetParameter( "interp_method", TePDIInterpolator::BicubicMethod );
  params.SetParameter( "dummy_value" , (double)0. );
  params.SetParameter( "auto_equalize" , (int)1 );
  
  TePDIBatchGeoMosaic mos;
  TEAGN_TRUE_OR_THROW( mos.Apply( params ), "Apply error" );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster,
    TEPDIEXAMPLESBINPATH "TePDIBatchGeoMosaic_test2.tif" ), 
    "GeoTIF generation error" );
}

int main()
{
  TEAGN_LOGMSG( "Test started." );

  TeStdIOProgress pi;
  TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );     

  GeoMosaic_test4();
  BatchGeoMosaic_test2();
  GeoMosaic_test3();
  TPMosaic_test2();
  TPMosaic_test1();
  GeoMosaic_test1();
  GeoMosaic_test2();
  BatchGeoMosaic_test1();

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}

