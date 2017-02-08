#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>
#include <TePDISAMClassifier.hpp>
#include <TePDIIsosegClas.hpp>
#include <TePDIKMeansClas.hpp>
#include <TePDIEMClas.hpp>
#include <TePDIRegGrowSeg.hpp>
#include <TePDIUtils.hpp>

#include <TeInitRasterDecoders.h>
#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TeAgnostic.h>

#include <vector>


void buildData( TePolygonSet& polset, TePDITypes::TePDIRasterPtrType& raster )
{
  TePDIParameters params;

  params.SetParameter( "input_image", raster );

  TePDITypes::TePDIPolSetMapPtrType output_polsets( 
    new TePDITypes::TePDIPolSetMapType );
  params.SetParameter( "output_polsets", output_polsets );
  
  params.SetParameter( "euc_treshold", (double)20 );
  params.SetParameter( "area_min", (int)15 );  
  
  TePDIRegGrowSeg segmenter;
  
  TEAGN_TRUE_OR_THROW( segmenter.Reset(params), "Reset failed" );
  
  TEAGN_TRUE_OR_THROW( segmenter.Apply(), "Apply error" );    
  
  TePDITypes::TePDIPolSetMapType::iterator psmap_it = 
    output_polsets->begin();
  TePDITypes::TePDIPolSetMapType::iterator psmap_it_end = 
    output_polsets->end();
    
  polset.clear();
    
  while( psmap_it != psmap_it_end ) {
    TePolygonSet::iterator ps_it = psmap_it->second.begin();
    TePolygonSet::iterator ps_it_end = psmap_it->second.end();
    
    while( ps_it != ps_it_end ) {
      polset.add( *ps_it );
      ++ps_it;  
    }
    
    ++psmap_it;
  }
}

void TePDISAMClassifier_test( TePDITypes::TePDIRasterPtrType& raster)
{ 
  // Initiating the output RAM raster
  
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster( output_raster, 1, 1, 1, 
    false, TeUNSIGNEDCHAR, 0), "output_raster Alloc error");
    
  // Creating the vector of input raster used bands
  
  std::vector< unsigned int > bands;
  bands.push_back(0);
  bands.push_back(1);
  bands.push_back(2);
    
  // Creating the spectral samples vector
  
  TePDISAMClassifier::ClassSpectralSamples class1Samples;
  class1Samples.classId_ = 80;
  class1Samples.maxAngularDist_ = 0.2;
  class1Samples.samplesMatrix_.Reset( 1, 3 ); // one sample, 3 bands
  class1Samples.samplesMatrix_( 0, 0) = 166;
  class1Samples.samplesMatrix_( 0, 1) = 255;
  class1Samples.samplesMatrix_( 0, 2) = 255;
  
  TePDISAMClassifier::ClassSpectralSamples class2Samples;
  class2Samples.classId_ = 155;
  class2Samples.maxAngularDist_ = 0.2;
  class2Samples.samplesMatrix_.Reset( 1, 3 ); // one sample, 3 bands
  class2Samples.samplesMatrix_( 0, 0) = 36;
  class2Samples.samplesMatrix_( 0, 1) = 255;
  class2Samples.samplesMatrix_( 0, 2) = 76;  
  
  TePDISAMClassifier::ClassSpectralSamples class3Samples;
  class3Samples.classId_ = 255;
  class2Samples.maxAngularDist_ = 0.2;
  class3Samples.samplesMatrix_.Reset( 1, 3 ); // one sample, 3 bands
  class3Samples.samplesMatrix_( 0, 0) = 36;
  class3Samples.samplesMatrix_( 0, 1) = 36;
  class3Samples.samplesMatrix_( 0, 2) = 76;    
  
  TePDISAMClassifier::SpectralSamplesVecPtrT spectral_samples( new
    TePDISAMClassifier::SpectralSamplesVectorT );
  spectral_samples->push_back( class1Samples );
  spectral_samples->push_back( class2Samples );
  spectral_samples->push_back( class3Samples );
  
  TePDIParameters params;
  params.SetParameter("input_raster", raster);
  params.SetParameter("bands", bands);
  params.SetParameter("output_raster", output_raster);
  params.SetParameter("spectral_samples", spectral_samples);
  params.SetParameter("output_raster", output_raster);
  params.SetParameter("enable_multi_thread", (int)1);

  TePDISAMClassifier classification;
  
  TEAGN_TRUE_OR_THROW(classification.Reset(params), "Reset failed");
  
  TEAGN_TRUE_OR_THROW(classification.Apply(), "Apply error");
  
  TEAGN_TRUE_OR_THROW(TePDIUtils::TeRaster2Geotiff(output_raster, 
    TEPDIEXAMPLESBINPATH "TePDISAMClassifier_test.tif"), 
    "GeoTIF generation error");
}

void IsosegClassification_test( TePolygonSet& polset, 
  TePDITypes::TePDIRasterPtrType& raster)
{
  TePDIParameters params;

  vector<int> bands;
  bands.push_back(0);
  bands.push_back(1);
  bands.push_back(2);
  params.SetParameter("bands", bands);

  TePDITypes::TePDIRasterVectorType input_rasters;
  input_rasters.push_back( raster );
  input_rasters.push_back( raster );
  input_rasters.push_back( raster );
  params.SetParameter("input_rasters", input_rasters);

  TePDITypes::TePDIPolygonSetPtrType input_polygonset(  &polset, true );
  params.SetParameter("input_polygonset", input_polygonset);

  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster( output_raster, 1, 1, 1, 
    false, TeDOUBLE, 0), "output_raster Alloc error");
  params.SetParameter("output_raster", output_raster);
  
  params.SetParameter("acceptance_limiar", (double)90.0);

  TePDIIsosegClas classification;
  
  TEAGN_TRUE_OR_THROW(classification.Reset(params), "Reset failed");
  
  TEAGN_TRUE_OR_THROW(classification.Apply(), "Apply error");
  
  TEAGN_TRUE_OR_THROW(TePDIUtils::TeRaster2Geotiff(output_raster, 
    TEPDIEXAMPLESBINPATH "Classification_Isoseg_test.tif"), "GeoTIF generation error");
}


void KMeansClassification_test( TePDITypes::TePDIRasterPtrType& raster)
{ 
  TePDIParameters params;

  params.SetParameter("input_raster", raster);

  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster( output_raster, 1, 1, 1, 
    false, TeDOUBLE, 0), "output_raster Alloc error");
  params.SetParameter("output_raster", output_raster);
  
  vector<int> bands;
  bands.push_back(0);
  bands.push_back(1);
  bands.push_back(2);
  params.SetParameter("bands", bands);  
  
  params.SetParameter("classes_number", (int)10);
  params.SetParameter("sample", (int)1);
  params.SetParameter("iterations_number", (int)10);
  params.SetParameter("line_begin", (int)0);
  params.SetParameter("line_end", (int)( raster->params().nlines_ - 1 ) );
  params.SetParameter("column_begin", (int)0);
  params.SetParameter("column_end", (int)( raster->params().ncols_ - 1 ) );

  TePDIKMeansClas classification;
  
  TEAGN_TRUE_OR_THROW(classification.Reset(params), "Reset failed");
  
  TEAGN_TRUE_OR_THROW(classification.Apply(), "Apply error");
  
  TEAGN_TRUE_OR_THROW(TePDIUtils::TeRaster2Geotiff(output_raster, 
    TEPDIEXAMPLESBINPATH "KMeansClassification_test.tif"), "GeoTIF generation error");
}


void EMClassification_test( TePDITypes::TePDIRasterPtrType& raster)
{
  TePDIParameters params;

  TePDITypes::TePDIRasterVectorType input_rasters;
  input_rasters.push_back( raster );
  input_rasters.push_back( raster );
  input_rasters.push_back( raster );
  params.SetParameter("input_rasters", input_rasters);
  
  vector<int> bands;
  bands.push_back(0);
  bands.push_back(1);
  bands.push_back(2);
  params.SetParameter("bands", bands);
  
  params.SetParameter("classes_to_find", (int)5);

  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW(TePDIUtils::TeAllocRAMRaster( output_raster, 1, 1, 1, 
    false, TeDOUBLE, 0), "output_raster Alloc error");
  params.SetParameter("output_raster", output_raster);
  
  params.SetParameter("sx", (int)5);
  params.SetParameter("sy", (int)5);
  params.SetParameter("max_iterations", (int)3);
  params.SetParameter("epsilon", (double)3.5);
  params.SetParameter("shift_threshold", (double)35);

  TePDIEMClas classification;
  
  TEAGN_TRUE_OR_THROW(classification.Reset(params), "Factory make failed");
  
  TEAGN_TRUE_OR_THROW(classification.Apply(), "Apply error");
  
  TEAGN_TRUE_OR_THROW(TePDIUtils::TeRaster2Geotiff(output_raster, 
    TEPDIEXAMPLESBINPATH "EMClassification_test.tif"), "GeoTIF generation error");
}

int main()
{
  TEAGN_LOGMSG("Test started.");
  
  try
  {
    TeInitRasterDecoders();
    
    TeStdIOProgress pi;
    TeProgress::setProgressInterf(dynamic_cast< TeProgressBase* >(&pi));
    
    /* Opening image */
    
    TePDITypes::TePDIRasterPtrType raster;
    TEAGN_TRUE_OR_THROW( TePDIUtils::loadRaster( 
      std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 
      raster, true ), "Error loading raster" );    
    
    /* Polygons independent algorithms */
    
    TePDISAMClassifier_test( raster );
    EMClassification_test( raster );
    KMeansClassification_test( raster );
    
    /* Generating polygons */
    
    TePolygonSet polset;
    buildData( polset, raster );
    
    /* Polygons dependent algorithms */
    
    IsosegClassification_test( polset, raster );
  }
  catch( const TeException& excpt ){
    TEAGN_LOGERR( excpt.message() )
    return EXIT_FAILURE;
  }
  
  TEAGN_LOGMSG("Test OK.");
  return EXIT_SUCCESS;
}

