
#define TEAGN_ENABLE_STDOUT_LOG
#include <TePDIExamplesBase.hpp>

#include <TePDIDecorrelationEnhancement.hpp>
#include <TePDIParameters.hpp>
#include <TeAgnostic.h>
#include <TePDIUtils.hpp>

#include <TeRaster.h>
#include <TeInitRasterDecoders.h>

#include <TeProgress.h>
#include <TeStdIOProgress.h>

void TePDIDecorrelationEnhancement_test()
{
	TePDIParameters params;

	TePDITypes::TePDIRasterVectorType input_rasters;
	TePDITypes::TePDIRasterPtrType inRaster(new TeRaster(
	  TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif", 'r'));
	TEAGN_TRUE_OR_THROW(inRaster->init(), "Unable to init inRaster");
	input_rasters.push_back(inRaster);
	input_rasters.push_back(inRaster);
	input_rasters.push_back(inRaster);

	std::vector<int> bands_direct;
	bands_direct.push_back(0);
	bands_direct.push_back(1);
	bands_direct.push_back(2);
	
	TePDITypes::TePDIRasterVectorType output_rasters;
	TePDITypes::TePDIRasterPtrType outRaster1;
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster(outRaster1, 1, 1, 1, 
	  false, TeUNSIGNEDCHAR, 0), "RAM Raster 1 Alloc error");
	TePDITypes::TePDIRasterPtrType outRaster2;
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster(outRaster2, 1, 1, 1, 
	  false, TeUNSIGNEDCHAR, 0), "RAM Raster 2 Alloc error");
	TePDITypes::TePDIRasterPtrType outRaster3;
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster(outRaster3, 1, 1, 1, 
	  false, TeUNSIGNEDCHAR, 0 ), "RAM Raster 3 Alloc error");
	output_rasters.push_back(outRaster1);
	output_rasters.push_back(outRaster2);
	output_rasters.push_back(outRaster3);
	
	params.SetParameter("input_rasters", input_rasters);
	params.SetParameter("bands", bands_direct);
	params.SetParameter("output_rasters", output_rasters);
	
	TePDIDecorrelationEnhancement de;
	TEAGN_TRUE_OR_THROW(de.Reset(params), "Invalid Parameters");
	TEAGN_TRUE_OR_THROW(de.Apply(), "Apply error");
	
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeRaster2Geotiff(output_rasters[0], 
	  TEPDIEXAMPLESBINPATH "TePDIDecorrelationEnhancement0.tif" ), 
	  "GeoTIFF0 generation error");
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeRaster2Geotiff(output_rasters[1], 
	  TEPDIEXAMPLESBINPATH "TePDIDecorrelationEnhancement1.tif" ), 
	  "GeoTIFF1 generation error");
	TEAGN_TRUE_OR_THROW(TePDIUtils::TeRaster2Geotiff(output_rasters[2], 
	  TEPDIEXAMPLESBINPATH "TePDIDecorrelationEnhancement2.tif" ), 
	  "GeoTIFF2 generation error");
}

int main()
{
	TEAGN_LOGMSG("Test started.");
	
	TEAGN_DEBUG_MODE_CHECK;
	
	try
	{
		TeStdIOProgress pi;
		TeProgress::setProgressInterf(dynamic_cast<TeProgressBase*>(&pi));
		TeInitRasterDecoders();
		TePDIDecorrelationEnhancement_test();
	}
  catch( const TeException& excpt ){
    TEAGN_LOGERR( excpt.message() )
    return EXIT_FAILURE;
  }
	
	TEAGN_LOGMSG("Test OK.");
	return EXIT_SUCCESS;
}
