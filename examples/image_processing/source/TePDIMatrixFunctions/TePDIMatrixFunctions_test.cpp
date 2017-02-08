#define TEAGN_ENABLE_STDOUT_LOG

#include <TeAgnostic.h>

#include <TePDIMatrixFunctions.hpp>

#include <string>

void GetCorrelation_test()
{
    TePDIMatrix< double > m1;
    TEAGN_TRUE_OR_THROW( m1.Reset( 2, 2 ), "Reset error" );
    
    TePDIMatrix< double > m2;
    TEAGN_TRUE_OR_THROW( m2.Reset( 2, 2 ), "Reset error" );
    
    m1( 0 , 0 ) = 0;
    m1( 0 , 1 ) = 1;
    m1( 1 , 0 ) = 2;
    m1( 1 , 1 ) = 3;
    
    m2( 0 , 0 ) = 0;
    m2( 0 , 1 ) = 1;
    m2( 1 , 0 ) = 2;
    m2( 1 , 1 ) = 3;

    TEAGN_CHECK_EPS( TePDIMatrixFunctions::GetCorrelation( m1, m2 ),
      1.0, 0.000001 , "Invalid GetCorrelation return" );
}


void Abs_test()
{
    TePDIMatrix< double > m1;
    TEAGN_TRUE_OR_THROW( m1.Reset( 2, 2 ), "Reset error" );
    
    TePDIMatrix< double > m2;
    
    m1( 0 , 0 ) = 0;
    m1( 0 , 1 ) = -1;
    m1( 1 , 0 ) = 2;
    m1( 1 , 1 ) = -3;
    
    TEAGN_TRUE_OR_THROW( TePDIMatrixFunctions::Abs( m1, m2 ),
      "Abs calcule failed" );
      
    TEAGN_CHECK_EPS( m2( 0, 0 ), 0.0, 0.000001, "Invalid value" );
    TEAGN_CHECK_EPS( m2( 0, 1 ), 1.0, 0.000001, "Invalid value" );
    TEAGN_CHECK_EPS( m2( 1, 0 ), 2.0, 0.000001, "Invalid value" );
    TEAGN_CHECK_EPS( m2( 1, 1 ), 3.0, 0.000001, "Invalid value" );
}


void WhereIsGreater_test()
{
    TePDIMatrix< double > m1;
    TEAGN_TRUE_OR_THROW( m1.Reset( 2, 2 ), "Reset error" );
    
    TePDIMatrix< double > m2;
    TEAGN_TRUE_OR_THROW( m2.Reset( 2, 2 ), "Reset error" );
    
    TePDIMatrix< double > gt;
    
    m1( 0 , 0 ) = 0;
    m1( 0 , 1 ) = 1;
    m1( 1 , 0 ) = 2;
    m1( 1 , 1 ) = 3;
    
    m2( 0 , 0 ) = 0;
    m2( 0 , 1 ) = 2;
    m2( 1 , 0 ) = 2;
    m2( 1 , 1 ) = 1;
    
    TEAGN_TRUE_OR_THROW( TePDIMatrixFunctions::WhereIsGreater( m1, m2, gt ),
      "WhereIsGreater calcule failed" );    

    TEAGN_CHECK_EPS( gt( 0, 0 ), 0.0, 0.000001 , 
      "Invalid WhereIsGreater result" );
    TEAGN_CHECK_EPS( gt( 0, 1 ), 0.0, 0.000001 , 
      "Invalid WhereIsGreater result" );
    TEAGN_CHECK_EPS( gt( 1, 0 ), 0.0, 0.000001 , 
      "Invalid WhereIsGreater result" );
    TEAGN_CHECK_EPS( gt( 1, 1 ), 1.0, 0.000001 , 
      "Invalid WhereIsGreater result" );            
}


void Negate_test()
{
    TePDIMatrix< double > m1;
    TEAGN_TRUE_OR_THROW( m1.Reset( 2, 2 ), "Reset error" );
    
    TePDIMatrix< double > neg;
    TEAGN_TRUE_OR_THROW( neg.Reset( 2, 2 ), "Reset error" );
    
    m1( 0 , 0 ) = 0;
    m1( 0 , 1 ) = -1;
    m1( 1 , 0 ) = 2;
    m1( 1 , 1 ) = -3;
    
    TEAGN_TRUE_OR_THROW( TePDIMatrixFunctions::Negate( m1, neg ),
      "Abs calcule failed" );
      
    TEAGN_CHECK_EPS( neg( 0, 0 ), 1.0, 0.000001, "Invalid value" );
    TEAGN_CHECK_EPS( neg( 0, 1 ), 0.0, 0.000001, "Invalid value" );
    TEAGN_CHECK_EPS( neg( 1, 0 ), 0.0, 0.000001, "Invalid value" );
    TEAGN_CHECK_EPS( neg( 1, 1 ), 0.0, 0.000001, "Invalid value" );
}


int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    GetCorrelation_test();
    Abs_test();
    WhereIsGreater_test();
    Negate_test();
  }
  catch( const TeException& e ){
    TEAGN_LOGERR( "Test Failed - " + e.message() );
    return EXIT_FAILURE;
  } 

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
