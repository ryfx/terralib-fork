#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIParameters.hpp>
#include <TeAgnostic.h>
#include <TePDIUtils.hpp>
#include <TePDIPrincipalComponentsFusion.hpp>
#include <TePDIIHSFusion.hpp>
#include <TePDIVenturaFusion.hpp>
#include <TePDIGarguetFusion.hpp>
#include <TePDIGeneralizedIHSFusion.hpp>
#include <TePDIIHSWaveletFusion.hpp>
#include <TePDIPCAWaveletFusion.hpp>
#include <TePDIWaveletAtrousFusion.hpp>
#include <TePDIWiSpeRFusion.hpp>

#include <TeInitRasterDecoders.h>
#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TeRasterParams.h> 

#include <vector>

void GarguetFusion_test()
{
  TePDIParameters params1;

  /* Building rasters */

  TePDITypes::TePDIRasterPtrType reference_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( reference_raster->init(), 
    "Unable to init input_raster1" );
    
  TePDITypes::TePDIRasterPtrType lowres_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_contraste_halfsampled.tif" ), 
    'r' ) );
  TEAGN_TRUE_OR_THROW( lowres_raster->init(), 
    "Unable to init input_raster2" );    
    
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "output_raster Alloc error" );
    
  params1.SetParameter( "reference_raster" , reference_raster );
  params1.SetParameter( "lowres_raster" , lowres_raster );
  params1.SetParameter( "output_raster" , output_raster );
  params1.SetParameter( "reference_raster_band" , 0 );
  params1.SetParameter( "lowres_raster_band" , 0 );
  
  TePDIGarguetFusion fusion;
  
  TEAGN_TRUE_OR_THROW( fusion.Reset(params1), "Reset failed" );
  
  TEAGN_TRUE_OR_THROW( fusion.Apply(), "Apply error" );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster,
    TEPDIEXAMPLESBINPATH "Fusion_Garguet_test.tif" ), "GeoTIF generation error" );
}


void VenturaFusion_test()
{
  TePDIParameters params1;

  /* Building rasters */

  TePDITypes::TePDIRasterPtrType reference_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( reference_raster->init(), 
    "Unable to init input_raster1" );
    
  TePDITypes::TePDIRasterPtrType lowres_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_contraste_halfsampled.tif" ), 
    'r' ) );
  TEAGN_TRUE_OR_THROW( lowres_raster->init(), 
    "Unable to init input_raster2" );    
    
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "output_raster Alloc error" );
    
  params1.SetParameter( "reference_raster" , reference_raster );
  params1.SetParameter( "lowres_raster" , lowres_raster );
  params1.SetParameter( "output_raster" , output_raster );
  params1.SetParameter( "reference_raster_band" , 0 );
  params1.SetParameter( "lowres_raster_band" , 0 );
  
  TePDIVenturaFusion fusion;
  
  TEAGN_TRUE_OR_THROW( fusion.Reset(params1), "Reset failed" );
  
  TEAGN_TRUE_OR_THROW( fusion.Apply(), "Apply error" );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster,
    TEPDIEXAMPLESBINPATH "Fusion_Ventura_test.tif" ), "GeoTIF generation error" );
}


void IHSFusion_test()
{
  /* Initializing the reference raster */

  TePDITypes::TePDIRasterPtrType reference_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_hrc_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( reference_raster->init(), 
    "Unable to init input_raster1" );
    
  /* Initializing the rgb raster */
  
  TePDITypes::TePDIRasterPtrType rgb_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( rgb_raster->init(), 
    "Unable to init input_raster1" );

  TePDITypes::TePDIRasterVectorType lowres_rasters;
  lowres_rasters.push_back( rgb_raster );
  lowres_rasters.push_back( rgb_raster );
  lowres_rasters.push_back( rgb_raster );

  std::vector< int > lowres_channels;
  lowres_channels.push_back( 0 );
  lowres_channels.push_back( 1 );
  lowres_channels.push_back( 2 );
  
  /* Building output raster */
    
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster,
    1, 1, 1, false, rgb_raster->params().dataType_[ 0 ], 0 ), "output_raster Alloc error" );
    
  /* Building algorithm parameters */
    
  TePDIParameters params1;
    
  params1.SetParameter( "reference_raster" , reference_raster );
  params1.SetParameter( "lowres_rasters" , lowres_rasters );
  params1.SetParameter( "lowres_channels" , lowres_channels );
  params1.SetParameter( "output_raster" , output_raster );
  params1.SetParameter( "reference_raster_band" , 0 );
  params1.SetParameter( "interpol_method" , TePDIInterpolator::NNMethod );
  
  TePDIIHSFusion fusion;
  
  TEAGN_TRUE_OR_THROW( fusion.Reset(params1), "Reset failed" );
  
  TEAGN_TRUE_OR_THROW( fusion.Apply(), "Apply error" );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster,
    TEPDIEXAMPLESBINPATH "Fusion_IHS_test.tif" ), "GeoTIF generation error" );
}


void TePDIPrincipalComponentsFusion_test()
{
	TePDIParameters params;

	TePDITypes::TePDIRasterVectorType input_rasters;
	TePDITypes::TePDIRasterVectorType output_rasters;
	std::vector<int> bands_direct;

	for (unsigned int b = 0; b < 3; b++)
	{
		TePDITypes::TePDIRasterPtrType inRaster(new TeRaster(
		  TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif", 'r'));
		TEAGN_TRUE_OR_THROW(inRaster->init(), "Unable to init inRaster " + 
		  Te2String(b));
		input_rasters.push_back(inRaster);

		TePDITypes::TePDIRasterPtrType outRaster;
		TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster(outRaster, 1, 1, 1, 
      false, TeUNSIGNEDCHAR, 0), "RAM Raster " + Te2String(b+1) + " Alloc error");
		output_rasters.push_back(outRaster);
		
		bands_direct.push_back( (int)b );
	}

	TePDITypes::TePDIRasterPtrType reference_raster(new TeRaster(
	  std::string( TEPDIEXAMPLESRESPATH "cbers2b_hrc_crop.tif" ), 'r'));
	TEAGN_TRUE_OR_THROW(reference_raster->init(), "Unable to init reference_raster");

	params.SetParameter("input_rasters", input_rasters);
	params.SetParameter("bands", bands_direct);
	params.SetParameter("output_rasters", output_rasters);
	params.SetParameter("reference_raster", reference_raster);
	params.SetParameter("reference_raster_band", 0);
	params.SetParameter("resampling_type", 
	  TePDIInterpolator::NNMethod);
	params.SetParameter("fit_histogram", false);
	
	TePDIPrincipalComponentsFusion pcf;
	TEAGN_TRUE_OR_THROW(pcf.Reset(params), "Invalid Parameters");
	TEAGN_TRUE_OR_THROW(pcf.Apply(), "Apply error");
	
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_rasters[ 0 ],
    TEPDIEXAMPLESBINPATH "Fusion_PC_test_output_b0.tif" ), 
    "GeoTIF generation error" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_rasters[ 1 ],
    TEPDIEXAMPLESBINPATH "Fusion_PC_test_output_b1.tif" ), 
    "GeoTIF generation error" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_rasters[ 2 ],
    TEPDIEXAMPLESBINPATH "Fusion_PC_test_output_b2.tif" ), 
    "GeoTIF generation error" );        
}

void TePDIGeneralizedIHSFusion_test()
{
  /* Initializing the reference raster */

  TePDITypes::TePDIRasterPtrType reference_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_hrc_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( reference_raster->init(), 
    "Unable to init input_raster1" );
    
  /* Initializing the input rasters */
  
  TePDITypes::TePDIRasterPtrType input_raster1( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_red_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster1->init(), 
    "Unable to init input_raster1" );
    
  TePDITypes::TePDIRasterPtrType input_raster2( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_green_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster2->init(), 
    "Unable to init input_raster1" );   
     
  TePDITypes::TePDIRasterPtrType input_raster3( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_blue_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster3->init(), 
    "Unable to init input_raster1" );       
  
  /* Building output rasters */
    
  TePDITypes::TePDIRasterPtrType ihs_output_raster1;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( ihs_output_raster1,
    1, 1, 1, false, TeFLOAT, 0 ), 
    "output_raster Alloc error" );
    
  TePDITypes::TePDIRasterPtrType ihs_output_raster2;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( ihs_output_raster2,
    1, 1, 1, false, TeFLOAT, 0 ), 
    "output_raster Alloc error" );
        
  TePDITypes::TePDIRasterPtrType ihs_output_raster3;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( ihs_output_raster3,
    1, 1, 1, false, TeFLOAT, 0 ), 
    "output_raster Alloc error" );     
       
  TePDITypes::TePDIRasterPtrType rgb_output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( rgb_output_raster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), 
    "output_raster Alloc error" );           
    
  /* Building algorithm parameters */
  
  TePDIParameters params;
  
  params.SetParameter("pp_type", 
    TePDIGeneralizedIHSFusion::MeanPreProcessing);
    
  params.SetParameter("mean", (double)128 );
      
  params.SetParameter("variance", (double)128 );
      
  vector<double> bands_center;
  bands_center.push_back( 0 );
  bands_center.push_back( 0 );
  bands_center.push_back( 0 );
  params.SetParameter("bands_center", bands_center );
  
  TePDITypes::TePDIRasterVectorType input_rasters;
  input_rasters.push_back( input_raster1 );
  input_rasters.push_back( input_raster2 );
  input_rasters.push_back( input_raster3 );
  params.SetParameter("input_rasters", input_rasters);
  
  params.SetParameter("pansharpening", (bool)false);
    
  params.SetParameter("reference_raster", reference_raster);
  
  params.SetParameter("reference_raster_band", (int)0 );
  
  TePDITypes::TePDIRasterVectorType ihs_output_rasters;
  ihs_output_rasters.push_back( ihs_output_raster1 );
  ihs_output_rasters.push_back( ihs_output_raster2 );
  ihs_output_rasters.push_back( ihs_output_raster3 );
  params.SetParameter("ihs_output_rasters", ihs_output_rasters);
  
  params.SetParameter("rgb_output_raster", rgb_output_raster);

  // Running algorithm
  
  TePDIGeneralizedIHSFusion fusion;
  
  TEAGN_TRUE_OR_THROW( fusion.Apply( params ), "Apply error" );
  
  // Saving data to output files
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( rgb_output_raster,
    TEPDIEXAMPLESBINPATH "TePDIGeneralizedIHSFusion_test_rgb_output_raster.tif" ), 
    "GeoTIF generation error" );
}

void TePDIIHSWaveletFusion_test()
{
  /* Initializing the reference raster */

  TePDITypes::TePDIRasterPtrType reference_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_hrc_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( reference_raster->init(), 
    "Unable to init input_raster1" );
    
  /* Initializing the rgb raster */
  
  TePDITypes::TePDIRasterPtrType rgb_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( rgb_raster->init(), 
    "Unable to init input_raster1" );
  
  /* Building output rasters */
    
  TePDITypes::TePDIRasterPtrType output_raster1;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster1,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), 
    "output_raster Alloc error" );
    
  TePDITypes::TePDIRasterPtrType output_raster2;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster2,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), 
    "output_raster Alloc error" );
        
  TePDITypes::TePDIRasterPtrType output_raster3;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster3,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), 
    "output_raster Alloc error" );        
    
  /* Building algorithm parameters */
  
  TePDIParameters params;
  
  TePDITypes::TePDIRasterVectorType input_rasters;
  input_rasters.push_back( rgb_raster );
  input_rasters.push_back( rgb_raster );
  input_rasters.push_back( rgb_raster );
  params.SetParameter("input_rasters", input_rasters);
  
  std::vector< int > bands;
  bands.push_back( 0 );
  bands.push_back( 1 );
  bands.push_back( 2 );
  params.SetParameter("bands", bands);
  
  TePDITypes::TePDIRasterVectorType output_rasters;
  output_rasters.push_back( output_raster1 );
  output_rasters.push_back( output_raster2 );
  output_rasters.push_back( output_raster3 );
  params.SetParameter("output_rasters", output_rasters);
  
  params.SetParameter("reference_raster", reference_raster);
  
  params.SetParameter("reference_raster_band", (int)0 );
  
  params.SetParameter("i_levels", (int)3 );
  
  params.SetParameter("filter_file", 
    std::string( TEPDIEXAMPLESRESPATH "b3spline.flr" ) );
    
  params.SetParameter("reference_levels", (int)3 );
  
  params.SetParameter("resampling_type", TePDIIHSWaveletFusion::NNMethod );
  
  params.SetParameter("fit_histogram", (bool)false );
  
  // Running algorithm
  
  TePDIIHSWaveletFusion fusion;
  
  TEAGN_TRUE_OR_THROW( fusion.Apply( params ), "Apply error" );
  
  // Saving data to output files
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster1,
    TEPDIEXAMPLESBINPATH "TePDIIHSWaveletFusion_test_output_raster1.tif" ), 
    "GeoTIF generation error" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster2,
    TEPDIEXAMPLESBINPATH "TePDIIHSWaveletFusion_test_output_raster2.tif" ), 
    "GeoTIF generation error" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster3,
    TEPDIEXAMPLESBINPATH "TePDIIHSWaveletFusion_test_output_raster3.tif" ), 
    "GeoTIF generation error" ); 
}

void TePDIPCAWaveletFusion_test()
{
  /* Initializing the reference raster */

  TePDITypes::TePDIRasterPtrType reference_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_hrc_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( reference_raster->init(), 
    "Unable to init input_raster1" );
    
  /* Initializing the rgb raster */
  
  TePDITypes::TePDIRasterPtrType rgb_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( rgb_raster->init(), 
    "Unable to init input_raster1" );
  
  /* Building output rasters */
    
  TePDITypes::TePDIRasterPtrType output_raster1;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster1,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), 
    "output_raster Alloc error" );
    
  TePDITypes::TePDIRasterPtrType output_raster2;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster2,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), 
    "output_raster Alloc error" );
        
  TePDITypes::TePDIRasterPtrType output_raster3;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster3,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), 
    "output_raster Alloc error" );        
    
  /* Building algorithm parameters */
  
  TePDIParameters params;
  
  TePDITypes::TePDIRasterVectorType input_rasters;
  input_rasters.push_back( rgb_raster );
  input_rasters.push_back( rgb_raster );
  input_rasters.push_back( rgb_raster );
  params.SetParameter("input_rasters", input_rasters);
  
  std::vector< int > bands;
  bands.push_back( 0 );
  bands.push_back( 1 );
  bands.push_back( 2 );
  params.SetParameter("bands", bands);
  
  TePDITypes::TePDIRasterVectorType output_rasters;
  output_rasters.push_back( output_raster1 );
  output_rasters.push_back( output_raster2 );
  output_rasters.push_back( output_raster3 );
  params.SetParameter("output_rasters", output_rasters);
  
  params.SetParameter("reference_raster", reference_raster);
  
  params.SetParameter("reference_raster_band", (int)0 );
  
  params.SetParameter("pca1_levels", (int)3 );
  
  params.SetParameter("filter_file", 
    std::string( TEPDIEXAMPLESRESPATH "b3spline.flr" ) );
    
  params.SetParameter("reference_levels", (int)3 );
  
  params.SetParameter("resampling_type", TePDIPCAWaveletFusion::NNMethod );
  
  params.SetParameter("fit_histogram", (bool)false );
  
  // Running algorithm
  
  TePDIPCAWaveletFusion fusion;
  
  TEAGN_TRUE_OR_THROW( fusion.Apply( params ), "Apply error" );
  
  // Saving data to output files
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster1,
    TEPDIEXAMPLESBINPATH "TePDIPCAWaveletFusion_test_output_raster1.tif" ), 
    "GeoTIF generation error" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster2,
    TEPDIEXAMPLESBINPATH "TePDIPCAWaveletFusion_test_output_raster2.tif" ), 
    "GeoTIF generation error" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster3,
    TEPDIEXAMPLESBINPATH "TePDIPCAWaveletFusion_test_output_raster3.tif" ), 
    "GeoTIF generation error" ); 
}

void TePDIWaveletAtrousFusion_test()
{
  /* Initializing the reference raster */

  TePDITypes::TePDIRasterPtrType reference_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_hrc_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( reference_raster->init(), 
    "Unable to init input_raster1" );
    
  /* Initializing the rgb raster */
  
  TePDITypes::TePDIRasterPtrType rgb_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( rgb_raster->init(), 
    "Unable to init input_raster1" );
  
  /* Building output rasters */
    
  TePDITypes::TePDIRasterPtrType output_raster1;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster1,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), 
    "output_raster Alloc error" );
    
  TePDITypes::TePDIRasterPtrType output_raster2;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster2,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), 
    "output_raster Alloc error" );
        
  TePDITypes::TePDIRasterPtrType output_raster3;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster3,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), 
    "output_raster Alloc error" );        
    
  /* Building algorithm parameters */
  
  TePDIParameters params;
  
  TePDITypes::TePDIRasterVectorType input_rasters;
  input_rasters.push_back( rgb_raster );
  input_rasters.push_back( rgb_raster );
  input_rasters.push_back( rgb_raster );
  params.SetParameter("input_rasters", input_rasters);
  
  std::vector< int > bands;
  bands.push_back( 0 );
  bands.push_back( 1 );
  bands.push_back( 2 );
  params.SetParameter("bands", bands);
  
  TePDITypes::TePDIRasterVectorType output_rasters;
  output_rasters.push_back( output_raster1 );
  output_rasters.push_back( output_raster2 );
  output_rasters.push_back( output_raster3 );
  params.SetParameter("output_rasters", output_rasters);
  
  params.SetParameter("reference_raster", reference_raster);
  
  params.SetParameter("reference_raster_band", (int)0 );
  
  params.SetParameter("rasters_levels", (int)3 );
  
  params.SetParameter("filter_type", TePDIWaveletAtrous::B3SplineFilter );    
    
  params.SetParameter("reference_levels", (int)3 );
  
  params.SetParameter("resampling_type", TePDIWaveletAtrousFusion::NNMethod );
  
  params.SetParameter("fit_histogram", (bool)false );
  
  // Running algorithm
  
  TePDIWaveletAtrousFusion fusion;
  
  TEAGN_TRUE_OR_THROW( fusion.Apply( params ), "Apply error" );
  
  // Saving data to output files
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster1,
    TEPDIEXAMPLESBINPATH "TePDIWaveletAtrousFusion_test_output_raster1.tif" ), 
    "GeoTIF generation error" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster2,
    TEPDIEXAMPLESBINPATH "TePDIWaveletAtrousFusion_test_output_raster2.tif" ), 
    "GeoTIF generation error" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster3,
    TEPDIEXAMPLESBINPATH "TePDIWaveletAtrousFusion_test_output_raster3.tif" ), 
    "GeoTIF generation error" ); 
}

void TePDIWiSpeRFusion_test()
{
  /* Initializing the reference raster */

  TePDITypes::TePDIRasterPtrType reference_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_hrc_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( reference_raster->init(), 
    "Unable to init input_raster1" );
    
  /* Initializing the rgb raster */
  
  TePDITypes::TePDIRasterPtrType rgb_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( rgb_raster->init(), 
    "Unable to init input_raster1" );
  
  /* Building output rasters */
    
  TePDITypes::TePDIRasterPtrType output_raster1;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster1,
    1, 1, 1, false, rgb_raster->params().dataType_[ 0 ], 0 ), 
    "output_raster Alloc error" );
    
  TePDITypes::TePDIRasterPtrType output_raster2;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster2,
    1, 1, 1, false, rgb_raster->params().dataType_[ 0 ], 0 ), 
    "output_raster Alloc error" );
        
  TePDITypes::TePDIRasterPtrType output_raster3;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster3,
    1, 1, 1, false, rgb_raster->params().dataType_[ 0 ], 0 ), 
    "output_raster Alloc error" );        
    
  /* Building algorithm parameters */
  
  TePDIParameters params;
  
  TePDITypes::TePDIRasterVectorType input_rasters;
  input_rasters.push_back( rgb_raster );
  input_rasters.push_back( rgb_raster );
  input_rasters.push_back( rgb_raster );
  params.SetParameter("input_rasters", input_rasters);
  
  std::vector< int > bands;
  bands.push_back( 0 );
  bands.push_back( 1 );
  bands.push_back( 2 );
  params.SetParameter("bands", bands);
  
  TePDITypes::TePDIRasterVectorType output_rasters;
  output_rasters.push_back( output_raster1 );
  output_rasters.push_back( output_raster2 );
  output_rasters.push_back( output_raster3 );
  params.SetParameter("output_rasters", output_rasters);
  
  params.SetParameter("reference_raster", reference_raster);
  
  params.SetParameter("reference_raster_band", (int)0 );
  
  params.SetParameter("rasters_levels", (int)3 );
  
  params.SetParameter("filter_file", 
    std::string( TEPDIEXAMPLESRESPATH "b3spline.flr" ) );
    
  params.SetParameter("reference_levels", (int)3 );
  
  params.SetParameter("resampling_type", TePDIWiSpeRFusion::NNMethod );
  
  params.SetParameter("fit_histogram", (bool)false );
  
  params.SetParameter("satelliteName", std::string( 
    TEPDIEXAMPLESRESPATH "spot5.srf" ) );
  
  std::vector<int> bands_order;
  bands_order.push_back( 0 );
  bands_order.push_back( 1 );
  bands_order.push_back( 2 );
  params.SetParameter("bands_order", bands_order );
  
  // Running algorithm
  
  TePDIWiSpeRFusion fusion;
  
  TEAGN_TRUE_OR_THROW( fusion.Apply( params ), "Apply error" );
  
  // Saving data to output files
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster1,
    TEPDIEXAMPLESBINPATH "TePDIWiSpeRFusion_test_output_raster1.tif" ), 
    "GeoTIF generation error" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster2,
    TEPDIEXAMPLESBINPATH "TePDIWiSpeRFusion_test_output_raster2.tif" ), 
    "GeoTIF generation error" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster3,
    TEPDIEXAMPLESBINPATH "TePDIWiSpeRFusion_test_output_raster3.tif" ), 
    "GeoTIF generation error" );        
}

int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TeInitRasterDecoders();
    
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );     

//    TePDIGeneralizedIHSFusion_test(); ALGORITHM NOT WORKING !!
    TePDIIHSWaveletFusion_test();
    TePDIPCAWaveletFusion_test();
    TePDIWaveletAtrousFusion_test();
//    TePDIWiSpeRFusion_test(); ALGORITHM NOT WORKING !!
    IHSFusion_test();
    GarguetFusion_test();
    VenturaFusion_test();
    TePDIPrincipalComponentsFusion_test();
  }
  catch( const TeException& excpt ){
    TEAGN_LOGERR( excpt.message() )
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}

