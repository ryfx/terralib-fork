#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIHaralick.hpp>
#include <TePDIParameters.hpp>
#include <TeAgnostic.h>

#include <TeInitRasterDecoders.h>
#include <TeRaster.h>
#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TeBox.h>

void numeric_test_with_polygonset()
{
  /* Building synthetic raster */
  
  TePDITypes::TePDIRasterPtrType input_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster->init(), "Unable to init inRaster" );
  
  TePDIParameters pars;
  
  pars.SetParameter( "input_raster", input_raster );
  
  /* Building a polygon set based on the raster bounding box */
  
  TeBox box = input_raster->params().boundingBox();
  TePolygon pol = polygonFromBox( box );
  TePDITypes::TePDIPolygonSetPtrType polset( new TePolygonSet );
  polset->add( pol );
  pars.SetParameter( "polygonset", polset );
  
  pars.SetParameter( "direction", TePDIHaralick::East );
  
  /* Starting algorithm */
  
  TePDIHaralick algoinstance;    
  double value = 0;

  TEAGN_TRUE_OR_THROW( algoinstance.Reset( pars ), "Reset error" );
  
  TEAGN_TRUE_OR_THROW( algoinstance.getGLCMEntropy( 0, 0, value ), 
    "Error generation value" )
  TEAGN_CHECK_EPS( value, 5.314640, 0.00001, "Invalid value" );


  TEAGN_TRUE_OR_THROW( algoinstance.getGLCMEnergy( 0, 0, value ), 
    "Error generation value" )
  TEAGN_CHECK_EPS( value, 0.007787, 0.00001, "Invalid value" );


  TEAGN_TRUE_OR_THROW( algoinstance.getGLCMContrast( 0, 0, value ), 
    "Error generation value" )
  TEAGN_CHECK_EPS( value, 3.765557, 0.00001, "Invalid value" );


  TEAGN_TRUE_OR_THROW( algoinstance.getGLCMHomogeneity( 0, 0, value ), 
    "Error generation value" )
  TEAGN_CHECK_EPS( value, 0.572850, 0.00001, "Invalid value" );


  TEAGN_TRUE_OR_THROW( algoinstance.getGLCMQuiSquare( 0, 0, value ), 
    "Error generation value" )
  TEAGN_CHECK_EPS( value, 30.240689, 0.00001, "Invalid value" );
  
  TEAGN_TRUE_OR_THROW( algoinstance.getGLCMMean( 0, 0, value ), 
    "Error generation value" )
  TEAGN_CHECK_EPS( value, 76.925340, 0.00001, "Invalid value" );  
  
  TEAGN_TRUE_OR_THROW( algoinstance.getGLCMDissimilarity( 0, 0, value ), 
    "Error generation value" )
  TEAGN_CHECK_EPS( value, 1.163392, 0.00001, "Invalid value" );   
   
  TEAGN_TRUE_OR_THROW( algoinstance.getGLCMStdDev( 0, 0, value ), 
    "Error generation value" )
  TEAGN_CHECK_EPS( value, 8.770714, 0.00001, "Invalid value" );   
  
  TEAGN_TRUE_OR_THROW( algoinstance.getGLCMCorrelation( 0, 0, value ), 
    "Error generation value" )
  TEAGN_CHECK_EPS( value, 0.955870, 0.00001, "Invalid value" );   
  
}




int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );
      
    TeInitRasterDecoders();

    numeric_test_with_polygonset();
  }
  catch( const TeException& excpt ){
    TEAGN_LOGERR( excpt.message() )
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
