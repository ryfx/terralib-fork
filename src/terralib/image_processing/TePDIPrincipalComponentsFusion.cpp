#include "TePDIPrincipalComponentsFusion.hpp"

#include "../kernel/TeAgnostic.h"
#include "TePDIUtils.hpp"
#include "TePDITypes.hpp"
#include "TePDIStatistic.hpp"
#include "TePDIPrincipalComponents.hpp"
#include "../kernel/TeRasterRemap.h"
#include "../kernel/TeMatrix.h"
#include "../kernel/TeUtils.h"
#include <math.h>

TePDIPrincipalComponentsFusion::TePDIPrincipalComponentsFusion()
{
}

TePDIPrincipalComponentsFusion::~TePDIPrincipalComponentsFusion()
{
}

void TePDIPrincipalComponentsFusion::ResetState(const TePDIParameters& /*params*/)
{
}

bool TePDIPrincipalComponentsFusion::CheckParameters(const TePDIParameters& parameters) const
{
/* Input rasters and bands checking */

	TePDITypes::TePDIRasterVectorType input_rasters;
	TEAGN_TRUE_OR_RETURN(parameters.GetParameter("input_rasters", input_rasters), "Missing parameter: input_rasters");
	TEAGN_TRUE_OR_RETURN(input_rasters.size() > 1, "Invalid input rasters number");
	
	std::vector<int> bands;
	TEAGN_TRUE_OR_RETURN(parameters.GetParameter("bands", bands), "Missing parameter: bands");
	TEAGN_TRUE_OR_RETURN(bands.size() == input_rasters.size(), "Invalid parameter: bands number");

	for( unsigned int input_rasters_index = 0 ; input_rasters_index < input_rasters.size(); ++input_rasters_index )
	{
		TEAGN_TRUE_OR_RETURN(input_rasters[input_rasters_index].isActive(), "Invalid parameter: input_raster " + Te2String(input_rasters_index) + " inactive");
		TEAGN_TRUE_OR_RETURN(input_rasters[input_rasters_index]->params().status_ != TeRasterParams::TeNotReady, "Invalid parameter: input_raster " + Te2String(input_rasters_index) + " not ready");
		TEAGN_TRUE_OR_RETURN(bands[input_rasters_index] < input_rasters[input_rasters_index]->nBands(), "Invalid parameter: bands[" + Te2String(input_rasters_index) + "]");
		TEAGN_TRUE_OR_RETURN(input_rasters[input_rasters_index]->params().nlines_ == input_rasters[0]->params().nlines_, "Invalid parameter: input_raster " + Te2String(input_rasters_index) + " with imcompatible number of lines");
		TEAGN_TRUE_OR_RETURN(input_rasters[input_rasters_index]->params().ncols_ == input_rasters[0]->params().ncols_, "Invalid parameter: input_raster " + Te2String( input_rasters_index ) + " with imcompatible number of columns" );
		TEAGN_TRUE_OR_RETURN(((input_rasters[input_rasters_index]->params().photometric_[bands[input_rasters_index]] == TeRasterParams::TeRGB) || (input_rasters[input_rasters_index]->params().photometric_[bands[input_rasters_index]] == TeRasterParams::TeMultiBand)), "Invalid parameter - input_rasters (invalid photometric interpretation)");
	}

/* Reference raster checking */

	int reference_raster_band;
	TEAGN_TRUE_OR_RETURN(parameters.GetParameter("reference_raster_band", reference_raster_band), "Missing parameter: reference_raster_band");

	TePDITypes::TePDIRasterPtrType reference_raster;
	TEAGN_TRUE_OR_RETURN(parameters.GetParameter("reference_raster", reference_raster), "Missing parameter: reference_raster");
	TEAGN_TRUE_OR_RETURN(((reference_raster->params().photometric_[reference_raster_band] == TeRasterParams::TeRGB ) || (reference_raster->params().photometric_[reference_raster_band] == TeRasterParams::TeMultiBand)), "Invalid parameter - reference_raster (invalid photometric interpretation)");

	TEAGN_TRUE_OR_RETURN(reference_raster_band < reference_raster->nBands(), "Invalid parameter - reference_raster_band");

/* Output rasters checking */

	TePDITypes::TePDIRasterVectorType output_rasters;
	TEAGN_TRUE_OR_RETURN(parameters.GetParameter("output_rasters", output_rasters), "Missing parameter: output_rasters");
	TEAGN_TRUE_OR_RETURN(output_rasters.size() > 1, "Invalid output rasters number");
	TEAGN_TRUE_OR_RETURN(output_rasters.size() == input_rasters.size(), "Invalid parameter: output rasters size");

	for(unsigned int input_rasters_index2 = 0; input_rasters_index2 < input_rasters.size(); ++input_rasters_index2)
	{
		TEAGN_TRUE_OR_RETURN(input_rasters[input_rasters_index2].isActive(), "Invalid parameter: input_raster " + Te2String(input_rasters_index2) + " inactive");
		TEAGN_TRUE_OR_RETURN(input_rasters[input_rasters_index2]->params().status_ != TeRasterParams::TeNotReady, "Invalid parameter: input_raster " + Te2String(input_rasters_index2) + " not ready");
	}

	TePDIInterpolator::InterpMethod resampling_type;
	TEAGN_TRUE_OR_RETURN(parameters.GetParameter("resampling_type", 
     resampling_type), "Missing parameter: resampling_type");

	bool fit_histogram;
	TEAGN_TRUE_OR_RETURN(parameters.GetParameter("fit_histogram", fit_histogram), "Missing parameter: fit_histogram");

	return true;
}

bool TePDIPrincipalComponentsFusion::RunImplementation()
{
/* Getting parameters */

	TePDITypes::TePDIRasterVectorType input_rasters;
	TEAGN_TRUE_OR_RETURN(params_.GetParameter("input_rasters", input_rasters), "Missing parameter: input_rasters");

	std::vector<int> bands;
	TEAGN_TRUE_OR_RETURN(params_.GetParameter("bands", bands), 
    "Missing parameter: bands");

	TePDITypes::TePDIRasterVectorType output_rasters;
	TEAGN_TRUE_OR_RETURN(params_.GetParameter("output_rasters", output_rasters), "Missing parameter: output_rasters");

	TePDITypes::TePDIRasterPtrType reference_raster;
	TEAGN_TRUE_OR_RETURN(params_.GetParameter("reference_raster", reference_raster), "Missing parameter: reference_raster");

	int reference_raster_band;
	TEAGN_TRUE_OR_RETURN(params_.GetParameter("reference_raster_band", reference_raster_band), "Missing parameter: reference_raster_band");

	TePDIInterpolator::InterpMethod resampling_type;
	TEAGN_TRUE_OR_RETURN(params_.GetParameter("resampling_type", resampling_type), "Missing parameter: resampling_type");

	bool fit_histogram;
	TEAGN_TRUE_OR_RETURN(params_.GetParameter("fit_histogram", fit_histogram), "Missing parameter: fit_histogram");

  /* Resampling the input rasters */
  
  TePDITypes::TePDIRasterVectorType resampled_input_rasters;
  std::vector< int > resampled_input_rasters_bands;
  
  {
    for(unsigned int b = 0; b < input_rasters.size(); b++)
    {
      TeRasterParams resampled_input_rasters_params = 
        input_rasters[b]->params();
      resampled_input_rasters_params.setNLinesNColumns( 1, 1 );
      resampled_input_rasters_params.nBands( 1 );
        
      TePDITypes::TePDIRasterPtrType resampled_input_rasters_temp;
      TEAGN_TRUE_OR_RETURN(TePDIUtils::TeAllocRAMRaster(
        resampled_input_rasters_temp,  
        resampled_input_rasters_params, 
        TePDIUtils::TePDIUtilsAutoMemPol), 
        "Unable create the resampled input rasters");
        
      std::vector< unsigned int > inputResampBands;
      inputResampBands.push_back( (unsigned int)bands[ b ] );
      
      TEAGN_TRUE_OR_RETURN( TePDIUtils::resampleRasterByLinsCols(
        input_rasters[b], 
        inputResampBands,
        (unsigned int)reference_raster->params().nlines_, 
        (unsigned int)reference_raster->params().ncols_, 
        progress_enabled_,
        resampling_type,
        resampled_input_rasters_temp ), "Raster resample error" );
        
      resampled_input_rasters.push_back(resampled_input_rasters_temp);
      resampled_input_rasters_bands.push_back( 0 );
/*      
TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff(
  resampled_input_rasters[ b ],
  "TePDIPrincipalComponentsFusion_resamp_" + Te2String( b ) + ".tif" ),
  "Error generating geotiff" );
*/  
    }
  }

/* PCA Direct analysis */

	TePDITypes::TePDIRasterVectorType output_rasters_direct;
  TeSharedPtr<TeMatrix> covariance_matrix(new TeMatrix);
   
  {
    for (unsigned b = 0; b < output_rasters.size(); b++)
    {
      TePDITypes::TePDIRasterPtrType outRaster_direct;
      TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster(
        outRaster_direct, 1, 1, 1, false, TeDOUBLE, 0), 
        "RAM Raster " + Te2String(b) + " Alloc error");
      output_rasters_direct.push_back(outRaster_direct);
    }
    
    TePDIPrincipalComponents::TePDIPCAType analysis_type = 
      TePDIPrincipalComponents::TePDIPCADirect;
  
    TePDIParameters params_direct;
  
    params_direct.SetParameter("analysis_type", analysis_type);
    params_direct.SetParameter("input_rasters", resampled_input_rasters);
    params_direct.SetParameter("bands", resampled_input_rasters_bands);
    params_direct.SetParameter("output_rasters", output_rasters_direct);
    params_direct.SetParameter("covariance_matrix", covariance_matrix);
    
    TePDIPrincipalComponents pc_direct;
    TEAGN_TRUE_OR_THROW(pc_direct.Reset(params_direct), "Invalid Parameters");
    pc_direct.ToggleProgInt(progress_enabled_);
    TEAGN_TRUE_OR_THROW(pc_direct.Apply(), "Apply error");
  } 

  /* Computing statistics to fit the histograms */
	
	double	pixel = 0.0,
		gain = 1.0,
		offset = 0.0;

	if (fit_histogram)
	{
		TePDIStatistic stat;
		
		TePDIParameters stat_pars;
    
		TePDITypes::TePDIRasterVectorType stat_rasters;
		stat_rasters.push_back(output_rasters_direct[0]);
		stat_rasters.push_back(reference_raster);
		stat_pars.SetParameter("rasters", stat_rasters);
    
		std::vector<int> stat_bands;
		stat_bands.push_back(0);
		stat_bands.push_back(reference_raster_band);
		stat_pars.SetParameter("bands", stat_bands);
		
		TEAGN_TRUE_OR_RETURN(stat.Reset(stat_pars), "Unable to inialize the statistc module");
		stat.ToggleProgInt(progress_enabled_);
	
    double current_variance = stat.getVariance( 1 );
    double target_variance = stat.getVariance( 0 );
    
    double current_mean = stat.getMean( 1 );
    double target_mean = stat.getMean( 0 );
    
    gain = ( ( current_variance == 0.0 ) ? 1.0 :
      sqrt( target_variance / current_variance ) );
    offset = target_mean - ( gain * current_mean ); 
	}
  
  /* Swapping reference raster by first principal component */

  {
	  TePDIPIManager progress("Swapping PCA 1 by reference raster", 
     reference_raster->params().nlines_, progress_enabled_);
     
    TeRaster& inRasterRef = (*reference_raster);
    TeRaster& outRasterRef = (*output_rasters_direct[0]);
    
    for (int j = 0; j < reference_raster->params().nlines_; j++)
    {
      for (int i = 0; i < reference_raster->params().ncols_; i++)
      {
        TEAGN_TRUE_OR_RETURN( inRasterRef.getElement(i, j, pixel, 
          reference_raster_band), "Error reading data" )
          
        TEAGN_TRUE_OR_RETURN( outRasterRef.setElement(i, j, 
          gain*pixel+offset, 0), "Error writing data" );
      }
      
      TEAGN_FALSE_OR_RETURN( progress.Increment(),
        "Canceled by the user" );
    }
  } 

/* Inverse analysis */

	TePDIPrincipalComponents::TePDIPCAType analysis_type = 
    TePDIPrincipalComponents::TePDIPCAInverse;
	
	TePDIParameters params_inverse;

	std::vector< int > bands_inverse(output_rasters.size(), 0);

	params_inverse.SetParameter("analysis_type", analysis_type);
	params_inverse.SetParameter("input_rasters", output_rasters_direct);
	params_inverse.SetParameter("bands", bands_inverse);
	params_inverse.SetParameter("output_rasters", output_rasters);
	params_inverse.SetParameter("covariance_matrix", covariance_matrix);
	
	TePDIPrincipalComponents pc_inverse;
	TEAGN_TRUE_OR_THROW(pc_inverse.Reset(params_inverse), "Invalid Parameters");
	pc_inverse.ToggleProgInt(progress_enabled_);
	TEAGN_TRUE_OR_THROW(pc_inverse.Apply(), "Apply error");

	return true;
}
