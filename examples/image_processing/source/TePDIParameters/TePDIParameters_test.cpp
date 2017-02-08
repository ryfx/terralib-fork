#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIParameters.hpp>
#include <TeAgnostic.h>

int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TePDIParameters params_copy;
    
    const int x1_int = 1;
    const double x1_double = 1;
    
    {
      TePDIParameters params;
      
      params.SetParameter( "int", x1_int );
      params.SetParameter( "double", x1_double );    
      
      TEAGN_TRUE_OR_THROW( params.CheckParameter< int >( "int" ), "" )
      TEAGN_TRUE_OR_THROW( params.CheckParameter< double >( "double" ), "" )
      
      int x2_int = 0;
      TEAGN_TRUE_OR_THROW( params.GetParameter( "int", x2_int ),
        "Missing parameter" );    
      TEAGN_CHECK_EPS( x2_int, x1_int, 0, "" )
      
      double x2_double = 0;
      TEAGN_TRUE_OR_THROW( params.GetParameter( "double", 
        x2_double ), "Missing parameter" );
      TEAGN_CHECK_EPS( x2_double, x1_double, 0, "" )      
      
      params_copy = params;
      
      TEAGN_TRUE_OR_THROW( ( params_copy == params ),
        "Invalid ==operator result" )
    }
    
    TEAGN_TRUE_OR_THROW( params_copy.CheckParameter< int >( "int" ), "" )
    TEAGN_TRUE_OR_THROW( params_copy.CheckParameter< double >( "double" ), "" )
    
    int x2_int = 0;
    TEAGN_TRUE_OR_THROW( params_copy.GetParameter( "int", x2_int ),
      "Missing paramter" );    
    TEAGN_CHECK_EPS( x2_int, x1_int, 0, "" )
    
    double x2_double = 0;
    TEAGN_TRUE_OR_THROW( params_copy.GetParameter( "double", 
      x2_double ), "Missing parameter" );
    TEAGN_CHECK_EPS( x2_double, x1_double, 0, "" )
    
    /* Checking parameter remotion */
    
    TEAGN_TRUE_OR_THROW( params_copy.CheckParameter< double >( 
      "double" ), "" )
    params_copy.RemoveParameter( "double" );
    TEAGN_TRUE_OR_THROW( ! params_copy.CheckParameter< double >( 
      "double" ), "" )    
  }
  catch( const TeException& e ){
    TEAGN_LOGERR( "Test Failed - " + e.message() );
    return EXIT_FAILURE;
  }  

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
