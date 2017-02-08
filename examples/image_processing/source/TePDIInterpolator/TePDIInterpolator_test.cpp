#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIInterpolator.hpp>

#include <TeAgnostic.h>
#include <TePDITypes.hpp>
#include <TePDIUtils.hpp>

#include <TeInitRasterDecoders.h>

void NN_test()
{
  TePDITypes::TePDIRasterPtrType input_raster;
  
  TeRasterParams params;
  params.nBands( 1 );
  params.setNLinesNColumns( 2, 2 );
  params.setDataType( TeDOUBLE, -1 );
    
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( input_raster,
    params, false ), "Allocation error" );
    
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 0, 0, 1.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 1, 0, 2.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 0, 1, 3.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 1, 1, 4.0, 0 ), 
    "error defining raster element");
    
  TePDIInterpolator interp;
  
  TEAGN_TRUE_OR_THROW( interp.reset( input_raster, 
    TePDIInterpolator::NNMethod, 0 ), "Reset error" );
    
  double value = 0;
  
  // Outside image area
  
  interp.interpolate( -0.6, -0.6, 0, value );
  TEAGN_CHECK_EPS( 0.0, value , 0.000001, "");
  interp.interpolate( -0.6, 1.6, 0, value );
  TEAGN_CHECK_EPS( 0.0, value , 0.000001, "");      
  interp.interpolate( 1.6, -0.6, 0, value );
  TEAGN_CHECK_EPS( 0.0, value , 0.000001, "");
  interp.interpolate( 1.6, 1.6, 0, value );
  TEAGN_CHECK_EPS( 0.0, value , 0.000001, "");  
  
  // Inside image area but not over pixel centers
  
  interp.interpolate( -0.2, -0.2, 0, value );
  TEAGN_CHECK_EPS( 1.0, value , 0.000001, "");  
  interp.interpolate( 1.2, 1.2, 0, value );
  TEAGN_CHECK_EPS( 4.0, value , 0.000001, "");  
  interp.interpolate( 1.2, -0.2, 0, value );
  TEAGN_CHECK_EPS( 3.0, value , 0.000001, "");  
  interp.interpolate( -0.2, 1.2, 0, value );
  TEAGN_CHECK_EPS( 2.0, value , 0.000001, "");   
                
  // Center of pixels
    
  interp.interpolate( 0, 0, 0, value );
  TEAGN_CHECK_EPS( 1.0, value , 0.000001, "");
  interp.interpolate( 0, 1, 0, value );
  TEAGN_CHECK_EPS( 2.0, value , 0.000001, "");
  interp.interpolate( 1, 0, 0, value );
  TEAGN_CHECK_EPS( 3.0, value , 0.000001, "");
  interp.interpolate( 1, 1, 0, value );
  TEAGN_CHECK_EPS( 4.0, value , 0.000001, "");                
}


void bilinear_test()
{
  TePDITypes::TePDIRasterPtrType input_raster;
  
  TeRasterParams params;
  params.nBands( 1 );
  params.setNLinesNColumns( 2, 2 );
  params.setDataType( TeDOUBLE, -1 );
    
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( input_raster,
    params, false ), "Allocation error" );
    
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 0, 0, 1.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 1, 0, 2.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 0, 1, 3.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 1, 1, 4.0, 0 ), 
    "error defining raster element");
    
  TePDIInterpolator interp;
  
  TEAGN_TRUE_OR_THROW( interp.reset( input_raster, 
    TePDIInterpolator::BilinearMethod, 0 ), "Reset error" );
    
  double value = 0;
  
  // Outside image area
  
  interp.interpolate( -0.6, -0.6, 0, value );
  TEAGN_CHECK_EPS( 0.0, value , 0.000001, "");
  interp.interpolate( -0.6, 1.6, 0, value );
  TEAGN_CHECK_EPS( 0.0, value , 0.000001, "");      
  interp.interpolate( 1.6, -0.6, 0, value );
  TEAGN_CHECK_EPS( 0.0, value , 0.000001, "");
  interp.interpolate( 1.6, 1.6, 0, value );
  TEAGN_CHECK_EPS( 0.0, value , 0.000001, ""); 
    
  // Inside image area but not over pixel centers
  
  interp.interpolate( -0.2, -0.2, 0, value );
  TEAGN_CHECK_EPS( 1.0, value , 0.000001, "");  
  interp.interpolate( 1.2, 1.2, 0, value );
  TEAGN_CHECK_EPS( 4.0, value , 0.000001, "");  
  interp.interpolate( 1.2, -0.2, 0, value );
  TEAGN_CHECK_EPS( 3.0, value , 0.000001, "");  
  interp.interpolate( -0.2, 1.2, 0, value );
  TEAGN_CHECK_EPS( 2.0, value , 0.000001, "");     
  
  // Center of pixels
    
  interp.interpolate( 0, 0, 0, value );
  TEAGN_CHECK_EPS( 1.0, value , 0.000001, "");
  interp.interpolate( 0, 1, 0, value );
  TEAGN_CHECK_EPS( 2.0, value , 0.000001, "");
  interp.interpolate( 1, 0, 0, value );
  TEAGN_CHECK_EPS( 3.0, value , 0.000001, "");
  interp.interpolate( 1, 1, 0, value );
  TEAGN_CHECK_EPS( 4.0, value , 0.000001, "");
  
  // Image center 
  
  interp.interpolate( 0.5, 0.5, 0, value );
  TEAGN_CHECK_EPS( 2.5, value , 0.000001, "");  
}


void bicubic_test()
{
  TePDITypes::TePDIRasterPtrType input_raster;
  
  TeRasterParams params;
  params.nBands( 1 );
  params.setNLinesNColumns( 4, 4 );
  params.setDataType( TeDOUBLE, -1 );
    
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( input_raster,
    params, false ), "Allocation error" );
    
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 0, 0, 1.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 1, 0, 2.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 2, 0, 3.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 3, 0, 4.0, 0 ), 
    "error defining raster element");
    
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 0, 1, 5.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 1, 1, 6.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 2, 1, 7.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 3, 1, 8.0, 0 ), 
    "error defining raster element");
    
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 0, 2, 9.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 1, 2, 10.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 2, 2, 11.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 3, 2, 12.0, 0 ), 
    "error defining raster element");
    
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 0, 3, 13.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 1, 3, 14.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 2, 3, 15.0, 0 ), 
    "error defining raster element");
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 3, 3, 16.0, 0 ), 
    "error defining raster element");
    
  TePDIInterpolator interp;
  
  TEAGN_TRUE_OR_THROW( interp.reset( input_raster, 
    TePDIInterpolator::BicubicMethod, 0 ), "Reset error" );
    
  double value = 0;
  
  // Outside image area
  
  interp.interpolate( -0.6, -0.6, 0, value );
  TEAGN_CHECK_EPS( 0.0, value , 0.000001, "");
  interp.interpolate( -0.6, 3.6, 0, value );
  TEAGN_CHECK_EPS( 0.0, value , 0.000001, "");      
  interp.interpolate( 3.6, -0.6, 0, value );
  TEAGN_CHECK_EPS( 0.0, value , 0.000001, "");
  interp.interpolate( 3.6, 3.6, 0, value );
  TEAGN_CHECK_EPS( 0.0, value , 0.000001, "");  
  
  // Inside image area but not over pixel centers
  
  interp.interpolate( -0.2, -0.2, 0, value );
  TEAGN_CHECK_EPS( 1.0, value , 0.000001, "");  
  interp.interpolate( 3.2, 3.2, 0, value );
  TEAGN_CHECK_EPS( 16.0, value , 0.000001, "");  
  interp.interpolate( 3.2, -0.2, 0, value );
  TEAGN_CHECK_EPS( 13.0, value , 0.000001, "");  
  interp.interpolate( -0.2, 3.2, 0, value );
  TEAGN_CHECK_EPS( 4.0, value , 0.000001, "");    
  
  // Center of pixels
    
  interp.interpolate( 0, 0, 0, value );
  TEAGN_CHECK_EPS( 1.0, value , 0.000001, "");
  interp.interpolate( 0, 1, 0, value );
  TEAGN_CHECK_EPS( 2.0, value , 0.000001, "");
  interp.interpolate( 0, 2, 0, value );
  TEAGN_CHECK_EPS( 3.0, value , 0.000001, "");
  interp.interpolate( 0, 3, 0, value );
  TEAGN_CHECK_EPS( 4.0, value , 0.000001, "");
  
  interp.interpolate( 1, 0, 0, value );
  TEAGN_CHECK_EPS( 5.0, value , 0.000001, "");
  interp.interpolate( 1, 1, 0, value );
  TEAGN_CHECK_EPS( 6.0, value , 0.000001, "");
  interp.interpolate( 1, 2, 0, value );
  TEAGN_CHECK_EPS( 7.0, value , 0.000001, "");
  interp.interpolate( 1, 3, 0, value );
  TEAGN_CHECK_EPS( 8.0, value , 0.000001, "");
  
  interp.interpolate( 2, 0, 0, value );
  TEAGN_CHECK_EPS( 9.0, value , 0.000001, "");
  interp.interpolate( 2, 1, 0, value );
  TEAGN_CHECK_EPS( 10.0, value , 0.000001, "");
  interp.interpolate( 2, 2, 0, value );
  TEAGN_CHECK_EPS( 11.0, value , 0.000001, "");
  interp.interpolate( 2, 3, 0, value );
  TEAGN_CHECK_EPS( 12.0, value , 0.000001, "");
  
  interp.interpolate( 3, 0, 0, value );
  TEAGN_CHECK_EPS( 13.0, value , 0.000001, "");
  interp.interpolate( 3, 1, 0, value );
  TEAGN_CHECK_EPS( 14.0, value , 0.000001, "");
  interp.interpolate( 3, 2, 0, value );
  TEAGN_CHECK_EPS( 15.0, value , 0.000001, "");
  interp.interpolate( 3, 3, 0, value );
  TEAGN_CHECK_EPS( 16.0, value , 0.000001, "");
    
  // Image center
  
  interp.interpolate( 1.5, 1.5, 0, value );
  TEAGN_CHECK_EPS( 8.5, value , 0.000001, "");  
  
  // Inside image area but not over pixel centers
}

int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TeInitRasterDecoders();

    NN_test();
    bilinear_test();
    bicubic_test();
  }
  catch( const TeException& excpt ){
    TEAGN_LOGERR( excpt.message() )
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}

