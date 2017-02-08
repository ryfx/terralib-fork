#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>
#include <TePDIUtils.hpp>
#include <TePDISensorSimulator.hpp>
#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TeAgnostic.h>

void TePDISensorSimulator_test()
{
  TePDIParameters params;

  TePDITypes::TePDIRasterPtrType input_raster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_raster->init(), 
    "Unable to init input_raster" );
  params.SetParameter( "input_raster" , input_raster );  
    
  std::vector<int> channels;
  channels.push_back( 0 );
  channels.push_back( 1 );
  channels.push_back( 2 );
  params.SetParameter( "channels" , channels );
  
  TePDITypes::TePDIRasterPtrType output_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster,
    1, 1, 1, false, TeUNSIGNEDCHAR, 0 ), "output_raster Alloc error" );    
  params.SetParameter( "output_raster" , output_raster ); 
  
  params.SetParameter( "ifov_in" , (double)20.0 );
  params.SetParameter( "eifov_in_across" , (double)35.0 );
  params.SetParameter( "eifov_in_long" , (double)64.0 );

  params.SetParameter( "ifov_out" , (double)73.0 );
  params.SetParameter( "eifov_out_across" , (double)108.0 );
  params.SetParameter( "eifov_out_long" , (double)111.0 );
    

  TePDISensorSimulator algo;
  
  TEAGN_TRUE_OR_THROW( algo.Reset(params), "Reset failed" );
  TEAGN_TRUE_OR_THROW( algo.Apply(), "Apply error" );
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( output_raster,
    TEPDIEXAMPLESBINPATH "TePDISensorSimulator_test.tif" ), "GeoTIF generation error" );  
}

int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );     
    
    TePDISensorSimulator_test();
  }
  catch( const TeException& e ){
    TEAGN_LOGERR( "Test Failed - " + e.message() );
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}

