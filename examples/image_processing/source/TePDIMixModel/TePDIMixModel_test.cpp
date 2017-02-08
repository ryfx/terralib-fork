#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <vector>

#include <TeInitRasterDecoders.h>
#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TeRasterParams.h>
#include <TePDITypes.hpp>
#include <TePDIParameters.hpp>
#include <TeAgnostic.h>
#include <TePDIUtils.hpp>
#include <TeRasterParams.h>
#include <TePDIMixModel.hpp>
#include <TePDIMixModelComponentList.hpp>
#include <TePDIMixModelSpectralBandList.hpp>

void PrincoMixModel_test()
{
  #define componentsNumber 3
  #define spectralBandsNumber 3

  TePDIParameters params;

// ----- Setting spectralbands e components -----
  TePDIMixModelComponentList componentList;
  componentList.insertComponent(0, "nuvem");
  componentList.insertPixel(0, 0, 1.000);
  componentList.insertPixel(0, 1, 0.725);
  componentList.insertPixel(0, 2, 1.000);
  
  componentList.insertComponent(1, "vegetacao");
  componentList.insertPixel(1, 0, 0.392);
  componentList.insertPixel(1, 1, 0.251);
  componentList.insertPixel(1, 2, 0.604);
  
  componentList.insertComponent(2, "sombra");
  componentList.insertPixel(2, 0, 0.165);
  componentList.insertPixel(2, 1, 0.098);
  componentList.insertPixel(2, 2, 0.196);
  
  TePDIMixModelSpectralBandList spectralBandList;
  spectralBandList.insertSpectralBand(TePDIMixModelSpectralBand(250, 2, 0.450000, 0.520000, "CBERS2_CCD_BLUE"));
  spectralBandList.insertSpectralBand(TePDIMixModelSpectralBand(250, 3, 0.520000, 0.590000, "CBERS2_CCD_GREEN"));
  spectralBandList.insertSpectralBand(TePDIMixModelSpectralBand(250, 4, 0.630000, 0.690000, "CBERS2_CCD_RED"));

// Setting input rasters parameter
  TePDITypes::TePDIRasterVectorType input_rasters;
  
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string(TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r'));
  TEAGN_TRUE_OR_THROW(inRaster->init(), "Unable to init input_raster" );
  
  for(unsigned int i = 0; i < spectralBandsNumber; i++)
  {
    input_rasters.push_back(inRaster);
  }
  
// Setting number of bands in each input raster
  std::vector<int> bands;
  bands.push_back(0);
  bands.push_back(1);
  bands.push_back(2);

// Setting output rasters parameter
  TePDITypes::TePDIRasterVectorType output_rasters;
  for(unsigned int i = 0; i < componentsNumber; i++)
  {
    TePDITypes::TePDIRasterPtrType outRaster;
    TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster(outRaster, 1, 1, 1, false, TeDOUBLE, 0), "RAM raster " + Te2String(i+1) + " alloc error" );
    output_rasters.push_back(outRaster);
  }

  int  computeErrorRasters = 1,
    computeAverageError = 1,
    normalize = 0;

// Setting the parameters created before
  params.SetParameter("mixmodel_type", string("princo"));
  params.SetParameter("component_list", componentList);
  params.SetParameter("spectral_band_list", spectralBandList);
  params.SetParameter("input_rasters", input_rasters);
  params.SetParameter("bands", bands);
  params.SetParameter("output_rasters", output_rasters);
  params.SetParameter("compute_error_rasters", computeErrorRasters);
  params.SetParameter("compute_average_error", computeAverageError);
  params.SetParameter("normalize", normalize);

// Checking if it's necessary to create error rasters
  if (computeErrorRasters)
  {
    TePDITypes::TePDIRasterVectorType output_error_rasters;
    for(unsigned int i = 0; i < componentsNumber; i++)
    {
      TePDITypes::TePDIRasterPtrType outRaster;
      TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster(outRaster, 1, 1, 1, false, TeDOUBLE, 0), "RAM error raster " + Te2String(i+1) + " alloc error" );
      output_error_rasters.push_back(outRaster);
    }
    params.SetParameter("output_error_rasters", output_error_rasters);
  }

// Checking if it's necessary to create average error double
  double averageError = 0.0;
  if (computeAverageError)
    params.SetParameter("average_error", averageError);

// Making the algorithm and calling its execution
  TePDIMixModel mixmodel;
  TEAGN_TRUE_OR_THROW(mixmodel.Reset(params), "Reset failed");
  TEAGN_TRUE_OR_THROW(mixmodel.Apply(), "Apply error");

// Write output rasters in disk
  TEAGN_TRUE_OR_THROW(params.GetParameter("output_rasters", output_rasters), "Unable to retrive generated images");
  TEAGN_TRUE_OR_THROW(output_rasters.size() == componentsNumber, "Invalid output rasters number");
  for(unsigned int i = 0; i < componentsNumber; i++)
    TEAGN_TRUE_OR_THROW(TePDIUtils::TeRaster2Geotiff(output_rasters[i], TEPDIEXAMPLESBINPATH "TePDIPrincoMixModel" + Te2String(i+1) + ".tif"), "GeoTIFF " + Te2String(i+1) + " generation error");

// Write a color composite raster in disk  
  TeRasterParams base_raster_params = input_rasters[0]->params();
  base_raster_params.nBands( componentsNumber );
  base_raster_params.setDataType( TeDOUBLE, -1 );
  TePDITypes::TePDIRasterPtrType outRaster;
  TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster(outRaster, 
    base_raster_params, false ),
    "RAM Color Raster Alloc error" );
  for(int k = 0; k < componentsNumber; k++)
    for (int i = 0; i < base_raster_params.nlines_; i++)
      for (int j = 0; j < base_raster_params.ncols_; j++)
      {
        double p;
        output_rasters[k]->getElement(j, i, p);
        outRaster->setElement(j, i, p, k);
      }
  TEAGN_TRUE_OR_THROW(TePDIUtils::TeRaster2Geotiff(outRaster, TEPDIEXAMPLESBINPATH "TePDIPrincoMixModelCOLOR.tif"), "GeoTIFF Color generation error");

// Write output error rasters in disk
  params.GetParameter("compute_error_rasters", computeErrorRasters);
  if (computeErrorRasters)
  {
    TePDITypes::TePDIRasterVectorType output_error_rasters;
    TEAGN_TRUE_OR_THROW(params.GetParameter("output_error_rasters", output_error_rasters), "Unable to retrive generated error rasters");
    TEAGN_TRUE_OR_THROW(output_error_rasters.size() == componentsNumber, "Invalid output error rasters number");
    for(unsigned int i = 0; i < componentsNumber; i++)
      TEAGN_TRUE_OR_THROW(TePDIUtils::TeRaster2Geotiff(output_error_rasters[i], TEPDIEXAMPLESBINPATH "TePDIPrincoMixModelError" + Te2String(i+1) + ".tif"), "GeoTIFF error " + Te2String(i+1) + " generation error");
  }

// Parameter don't come back after the algorithm execution
  params.GetParameter("compute_average_error", computeAverageError);
  if (computeAverageError)
  {
    TEAGN_TRUE_OR_THROW(params.GetParameter("average_error", averageError), "Unable to retrive average error");
    //cout << "Average Erro = " << averageError << endl;
  }

}

int main()
{
  TEAGN_LOGMSG("Test started.");
  
  try
  {
    TeInitRasterDecoders();
    
    TeStdIOProgress pi;
    TeProgress::setProgressInterf(dynamic_cast< TeProgressBase* >(&pi));
    
    PrincoMixModel_test();
  }
  catch( const TeException& e ){
    TEAGN_LOGERR( "Test Failed - " + e.message() );
    return EXIT_FAILURE;
  } 
  
  TEAGN_LOGMSG("Test OK.");
  return EXIT_SUCCESS;
}

