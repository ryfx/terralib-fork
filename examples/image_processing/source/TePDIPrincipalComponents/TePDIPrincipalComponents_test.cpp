#include <TePDIPrincipalComponents.hpp>
#include <TePDIParameters.hpp>
#include <TeAgnostic.h>
#include <TePDIUtils.hpp>

#include <TeRaster.h>

#include <TeProgress.h>
#include <TeStdIOProgress.h>

#include <TePDIExamplesBase.hpp>

void pca_test()
{
/********* Direct analysis *********/

	TePDIPrincipalComponents::TePDIPCAType analysis_type = 
	  TePDIPrincipalComponents::TePDIPCADirect;

	TePDIParameters params_direct;

	TePDITypes::TePDIRasterPtrType inRaster1( new TeRaster(
	  std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 
	  'r') );
	TEAGN_TRUE_OR_THROW(inRaster1->init(), 
	  "Unable to init inRaster1");
	  
	TePDITypes::TePDIRasterPtrType inRaster2( new TeRaster(
	  std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ),
	  'r') );
	TEAGN_TRUE_OR_THROW(inRaster2->init(), 
	  "Unable to init inRaster2");
	
	TePDITypes::TePDIRasterPtrType inRaster3(new TeRaster(
	  std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ),
	  'r') );
	TEAGN_TRUE_OR_THROW(inRaster3->init(), "Unable to init inRaster3");
	
	TePDITypes::TePDIRasterVectorType input_rasters;
	input_rasters.push_back(inRaster1);
	input_rasters.push_back(inRaster2);
	input_rasters.push_back(inRaster3);

	std::vector<int> bands_direct;
	bands_direct.push_back(0);
	bands_direct.push_back(1);
	bands_direct.push_back(2);
	
	TePDITypes::TePDIRasterPtrType outRaster1_direct;
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster(
	  outRaster1_direct, 1, 1, 1, false, TeDOUBLE, 0), 
	  "RAM Raster 1 Alloc error");
	  
	TePDITypes::TePDIRasterPtrType outRaster2_direct;
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster(
	  outRaster2_direct, 1, 1, 1, false, TeDOUBLE, 0), 
	  "RAM Raster 2 Alloc error");
	  
	TePDITypes::TePDIRasterPtrType outRaster3_direct;
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster(
	  outRaster3_direct, 1, 1, 1, false, TeDOUBLE, 0 ), 
	  "RAM Raster 3 Alloc error");
	
	TePDITypes::TePDIRasterVectorType output_rasters_direct;
	output_rasters_direct.push_back(outRaster1_direct);
	output_rasters_direct.push_back(outRaster2_direct);
	output_rasters_direct.push_back(outRaster3_direct);
	
	TeSharedPtr<TeMatrix> covariance_matrix(new TeMatrix);
	
	params_direct.SetParameter("analysis_type", analysis_type);
	params_direct.SetParameter("input_rasters", input_rasters);
	params_direct.SetParameter("bands", bands_direct);
	params_direct.SetParameter("output_rasters", output_rasters_direct);
	params_direct.SetParameter("covariance_matrix", covariance_matrix);
	
	TePDIPrincipalComponents pc_direct;
	TEAGN_TRUE_OR_THROW(pc_direct.Reset(params_direct), "Invalid Parameters");
	TEAGN_TRUE_OR_THROW(pc_direct.Apply(), "Apply error");
	
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeRaster2Geotiff(
	  output_rasters_direct[0], 
	  TEPDIEXAMPLESBINPATH "TePDIPrincipalComponents_pca0.tif" ), 
	  "GeoTIFF0 generation error");
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeRaster2Geotiff(
	  output_rasters_direct[1], 
	  TEPDIEXAMPLESBINPATH "TePDIPrincipalComponents_pca1.tif" ), 
	  "GeoTIFF1 generation error");
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeRaster2Geotiff(
	  output_rasters_direct[2], 
	  TEPDIEXAMPLESBINPATH "TePDIPrincipalComponents_pca2.tif" ), 
	  "GeoTIFF2 generation error");

/********* Inverse analysis *********/
	
	analysis_type = TePDIPrincipalComponents::TePDIPCAInverse;
	
	TePDIParameters params_inverse;

	std::vector< int > bands_inverse;
	bands_inverse.push_back(0);
	bands_inverse.push_back(0);
	bands_inverse.push_back(0);

	TePDITypes::TePDIRasterPtrType outRaster1_inverse;
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster(
	  outRaster1_inverse, 1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), 
	  "RAM Raster 1 Alloc error");
	  
	TePDITypes::TePDIRasterPtrType outRaster2_inverse;
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster(
	  outRaster2_inverse, 1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), 
	  "RAM Raster 2 Alloc error");
	  
	TePDITypes::TePDIRasterPtrType outRaster3_inverse;
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster(
	  outRaster3_inverse, 1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), 
	  "RAM Raster 3 Alloc error");
	
	TePDITypes::TePDIRasterVectorType output_rasters_inverse;
	output_rasters_inverse.push_back(outRaster1_inverse);
	output_rasters_inverse.push_back(outRaster2_inverse);
	output_rasters_inverse.push_back(outRaster3_inverse);
	
	params_inverse.SetParameter("analysis_type", analysis_type);
	params_inverse.SetParameter("input_rasters", 
	  output_rasters_direct);
	params_inverse.SetParameter("bands", bands_inverse);
	params_inverse.SetParameter("output_rasters", 
	  output_rasters_inverse);
	params_inverse.SetParameter("covariance_matrix", 
	  covariance_matrix);
	
	TePDIPrincipalComponents pc_inverse;
	TEAGN_TRUE_OR_THROW(pc_inverse.Reset(params_inverse), 
	  "Invalid Parameters");
	TEAGN_TRUE_OR_THROW(pc_inverse.Apply(), "Apply error");
	
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeRaster2Geotiff(
	  output_rasters_inverse[0], 
	  TEPDIEXAMPLESBINPATH "TePDIPrincipalComponents_band0.tif"), 
	  "GeoTIFF0 generation error");
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeRaster2Geotiff(
	  output_rasters_inverse[1], 
	  TEPDIEXAMPLESBINPATH "TePDIPrincipalComponents_band1.tif"), 
	  "GeoTIFF1 generation error");
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeRaster2Geotiff(
	  output_rasters_inverse[2], 
	  TEPDIEXAMPLESBINPATH "TePDIPrincipalComponents_band2.tif"), 
	  "GeoTIFF2 generation error");
}

int main()
{
  TEAGN_LOGMSG( "Test started." );

  TEAGN_DEBUG_MODE_CHECK;

  try{
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );

    pca_test();
  }
  catch( const TeException& e ){
    TEAGN_LOGERR( "Test Failed - " + e.message() );
    return EXIT_FAILURE;
  }  

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
