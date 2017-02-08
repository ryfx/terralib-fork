#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIArithmetic.hpp>
#include <TePDIUtils.hpp>

#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TeAgnostic.h>

#include <string>

void TePDIArithmetic_operator_test1( const std::string& operatorToken,
  const std::string& tifFileNameToken )
{
  TePDITypes::TePDIRasterPtrType inputRasterPointer( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inputRasterPointer->init(), "Unable to init inRaster1" );
  
  TePDITypes::TePDIRasterVectorType input_rasters;
  input_rasters.push_back( inputRasterPointer );
  
  std::string arithmetic_string( 
    "( R0:0 " + operatorToken + " R0:1 ) " + operatorToken + " R0:2" );
  TEAGN_WATCH( arithmetic_string );
  
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );  
   
  // Creating algorithm parameters
    
  TePDIParameters params;  
  params.SetParameter( "input_rasters", input_rasters );
  params.SetParameter( "arithmetic_string", arithmetic_string );
  params.SetParameter( "output_raster", output_raster );
  params.SetParameter( "normalize_output", (int)1 );

  // Running the algorithm

  TePDIArithmetic algoInstance;
  TEAGN_TRUE_OR_THROW( algoInstance.Apply( params ), "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster,
    TEPDIEXAMPLESBINPATH "TePDIArithmetic_operator_test1_" + tifFileNameToken +
    "_.tif" ), "GeoTIF generation error" );
}

void TePDIArithmetic_precedence_test()
{
  TePDITypes::TePDIRasterPtrType inputRasterPointer( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inputRasterPointer->init(), "Unable to init inRaster1" );
  
  TePDITypes::TePDIRasterVectorType input_rasters;
  input_rasters.push_back( inputRasterPointer );
  input_rasters.push_back( inputRasterPointer );
  input_rasters.push_back( inputRasterPointer );
  
  std::string arithmetic_string( "R0:0 + R0:1 * R0:2" );
  TEAGN_WATCH( arithmetic_string );
  
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );  
   
  // Creating algorithm parameters
    
  TePDIParameters params;  
  params.SetParameter( "input_rasters", input_rasters );
  params.SetParameter( "arithmetic_string", arithmetic_string );
  params.SetParameter( "output_raster", output_raster );
  params.SetParameter( "normalize_output", (int)1 );

  // Running the algorithm

  TePDIArithmetic algoInstance;
  TEAGN_TRUE_OR_THROW( algoInstance.Apply( params ), "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster,
    TEPDIEXAMPLESBINPATH "TePDIArithmetic_precedence_test.tif" ), 
    "GeoTIF generation error" );
}

void TePDIArithmetic_real_number_test( const std::string& operatorToken,
  const std::string& tifFileNameToken )
{
  TePDITypes::TePDIRasterPtrType inputRasterPointer( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inputRasterPointer->init(), "Unable to init inRaster1" );
  
  TePDITypes::TePDIRasterVectorType input_rasters;
  input_rasters.push_back( inputRasterPointer );
  input_rasters.push_back( inputRasterPointer );
  input_rasters.push_back( inputRasterPointer );
  
  std::string arithmetic_string( "( R0:0 " + operatorToken + " 1.0 ) " + 
    operatorToken + " ( 1.0 " + operatorToken + " R0:1 )" );
  TEAGN_WATCH( arithmetic_string );
  
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Alloc error" );  
   
  // Creating algorithm parameters
    
  TePDIParameters params;  
  params.SetParameter( "input_rasters", input_rasters );
  params.SetParameter( "arithmetic_string", arithmetic_string );
  params.SetParameter( "output_raster", output_raster );
  params.SetParameter( "normalize_output", (int)1 );

  // Running the algorithm

  TePDIArithmetic algoInstance;
  TEAGN_TRUE_OR_THROW( algoInstance.Apply( params ), "Apply error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster,
    TEPDIEXAMPLESBINPATH "TePDIArithmetic_real_number_test_" + tifFileNameToken +
    "_.tif" ), "GeoTIF generation error" );
}

int main()
{
  TEAGN_LOGMSG( "Test started." );

  TeStdIOProgress pi;
  TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );
  
  TePDIArithmetic_operator_test1( "+", "plus" );
  TePDIArithmetic_operator_test1( "-", "minus" );
  TePDIArithmetic_operator_test1( "*", "multi" );
  TePDIArithmetic_operator_test1( "/", "division" );
  
  TePDIArithmetic_precedence_test();
  
  TePDIArithmetic_real_number_test( "+", "plus" );
  TePDIArithmetic_real_number_test( "-", "minus" );
  TePDIArithmetic_real_number_test( "*", "multi" );
  TePDIArithmetic_real_number_test( "/", "division" );  

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
