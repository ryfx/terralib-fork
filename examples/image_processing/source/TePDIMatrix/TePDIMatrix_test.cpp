#define TEAGN_ENABLE_STDOUT_LOG

#include <TeAgnostic.h>

#include <TePDIMatrix.hpp>

#include <string>
#include <stdlib.h>

void generic_tests()
{
    /* Checking basic matrix creation and access */
  
    TePDIMatrix< double > m1;
    TEAGN_TRUE_OR_THROW( m1.Reset( 2, 2,
      TePDIMatrix< double >::RAMMemPol ), "Matrix reset error" );
    TEAGN_TRUE_OR_THROW( ( ! m1.IsEmpty() ), "Empty verification failed." );
    TEAGN_TRUE_OR_THROW( m1.getMemPolicy() == TePDIMatrix< double >::RAMMemPol, 
      "Invalid policy" );           
    
    m1( 0 , 0 ) = 0.0;
    m1( 0 , 1 ) = 1.0;
    m1( 1 , 0 ) = 2.0;
    m1( 1 , 1 ) = 3.0;
    
    TEAGN_TRUE_OR_THROW( m1.GetLines() == 2, "Invalid lines number" );
    TEAGN_TRUE_OR_THROW( m1.GetColumns() == 2, "Invalid columns number" );
    
    TEAGN_TRUE_OR_THROW( m1( 0 , 0 ) == 0.0, "Invalid value" );
    TEAGN_TRUE_OR_THROW( m1( 0 , 1 ) == 1.0, "Invalid value" );
    TEAGN_TRUE_OR_THROW( m1( 1 , 0 ) == 2.0, "Invalid value" );
    TEAGN_TRUE_OR_THROW( m1( 1 , 1 ) == 3.0, "Invalid value" );
    
    TEAGN_TRUE_OR_THROW( m1[ 0 ][ 0 ] == 0.0, "Invalid value" );
    TEAGN_TRUE_OR_THROW( m1[ 0 ][ 1 ] == 1.0, "Invalid value" );
    TEAGN_TRUE_OR_THROW( m1[ 1 ][ 0 ] == 2.0, "Invalid value" );
    TEAGN_TRUE_OR_THROW( m1[ 1 ][ 1 ] == 3.0, "Invalid value" );    
    
    /* Reset test */
    
    TEAGN_TRUE_OR_THROW( m1.Reset( 2, 2 ), "Reset error" );
    TEAGN_TRUE_OR_THROW( m1.getMemPolicy() == TePDIMatrix< double >::RAMMemPol, 
      "Invalid policy" );           
    
    
    m1( 0 , 0 ) = 0.0;
    m1( 0 , 1 ) = 1.0;
    m1( 1 , 0 ) = 2.0;
    m1( 1 , 1 ) = 3.0;
    
    TEAGN_TRUE_OR_THROW( m1.GetLines() == 2, "Invalid lines number" );
    TEAGN_TRUE_OR_THROW( m1.GetColumns() == 2, "Invalid columns number" );
    
    TEAGN_TRUE_OR_THROW( m1( 0 , 0 ) == 0.0, "Invalid value" );
    TEAGN_TRUE_OR_THROW( m1( 0 , 1 ) == 1.0, "Invalid value" );
    TEAGN_TRUE_OR_THROW( m1( 1 , 0 ) == 2.0, "Invalid value" );
    TEAGN_TRUE_OR_THROW( m1( 1 , 1 ) == 3.0, "Invalid value" );    
    
    /* Checking copy operator */
    
    TePDIMatrix< double > m2;
    TEAGN_TRUE_OR_THROW( m2.IsEmpty(), "Empty verification failed." );
    TEAGN_TRUE_OR_THROW( m2.getMemPolicy() == TePDIMatrix< double >::RAMMemPol, 
      "Invalid policy" );           
    
    
    TEAGN_TRUE_OR_THROW( m2.GetLines() == 0, "Invalid lines number" );
    TEAGN_TRUE_OR_THROW( m2.GetColumns() == 0, "Invalid columns number" );
    
    m2 = m1;
    
    TEAGN_TRUE_OR_THROW( m2.GetLines() == 2, "Invalid lines number" );
    TEAGN_TRUE_OR_THROW( m2.GetColumns() == 2, "Invalid columns number" );
    
    TEAGN_TRUE_OR_THROW( m2( 0 , 0 ) == 0.0, "Invalid value" );
    TEAGN_TRUE_OR_THROW( m2( 0 , 1 ) == 1.0, "Invalid value" );
    TEAGN_TRUE_OR_THROW( m2( 1 , 0 ) == 2.0, "Invalid value" );
    TEAGN_TRUE_OR_THROW( m2( 1 , 1 ) == 3.0, "Invalid value" );
    
    /* Checking copy constructor */
    
    TePDIMatrix< double > m3( m1 );
    TEAGN_TRUE_OR_THROW( m3.getMemPolicy() == TePDIMatrix< double >::RAMMemPol, 
      "Invalid policy" );           
    

    TEAGN_TRUE_OR_THROW( m3.GetLines() == 2, "Invalid lines number" );
    TEAGN_TRUE_OR_THROW( m3.GetColumns() == 2, "Invalid columns number" );
    
    TEAGN_TRUE_OR_THROW( m3( 0 , 0 ) == 0.0, "Invalid value" );
    TEAGN_TRUE_OR_THROW( m3( 0 , 1 ) == 1.0, "Invalid value" );
    TEAGN_TRUE_OR_THROW( m3( 1 , 0 ) == 2.0, "Invalid value" );
    TEAGN_TRUE_OR_THROW( m3( 1 , 1 ) == 3.0, "Invalid value" );    
}

void RAMMemPol_test()
{
    const unsigned int max_ram  = (unsigned int)
      ( 0.20 * (double)MIN( ( TeGetTotalVirtualMemory() / 2.0 ) - 
      TeGetUsedVirtualMemory(), TeGetTotalPhysicalMemory() ) );    

    unsigned long int lines = (unsigned long int)(
      ( (double)( max_ram ) ) /
      ( ( (double)sizeof( double ) ) * 1000.00 ) );
    unsigned long int cols = 1000;

    TePDIMatrix< double > m1;
    TEAGN_TRUE_OR_THROW( m1.Reset( lines, cols, 
      TePDIMatrix< double >::RAMMemPol ), "Matrix reset error" );
      
    TEAGN_TRUE_OR_THROW( m1.getMemPolicy() == TePDIMatrix< double >::RAMMemPol, 
      "Invalid policy" );      
      
    double counter = 0.0;
  
    unsigned int line = 0;
    unsigned int col = 0;
      
    for( line = 0 ; line < lines ; ++line ) {
      for( col = 0 ; col < cols ; ++col ) {
        m1( line , col ) = counter;
        
        ++counter;
      }
    }
    
    counter = 0.0;
    
    for( line = 0 ; line < lines ; ++line ) {
      for( col = 0 ; col < cols ; ++col ) {
        TEAGN_TRUE_OR_THROW( ( m1( line , col ) == counter ), 
          "Element value check failed" );
          
        ++counter;          
      }
    }
    
    return;
}

void AutoMemPol_test()
{
    const unsigned int max_ram  = (unsigned int)
      ( 0.50 * (double)MIN( ( TeGetTotalVirtualMemory() / 2.0 ) - 
      TeGetUsedVirtualMemory(), TeGetTotalPhysicalMemory() ) );    

    unsigned long int lines = (unsigned long int)(
      ( (double)( max_ram + ( 256 * 1024 * 1024 ) ) ) /
      ( ( (double)sizeof( double ) ) * 1000.00 ) );
    unsigned long int cols = 1000;

    TePDIMatrix< double > m1;
    TEAGN_TRUE_OR_THROW( m1.Reset( lines, cols, 
      TePDIMatrix< double >::AutoMemPol ), "Matrix reset error" );
      
    TEAGN_TRUE_OR_THROW( m1.getMemPolicy() == TePDIMatrix< double >::AutoMemPol, 
      "Invalid policy" );        
      
    double counter = 0.0;
  
    unsigned int line = 0;
    unsigned int col = 0;
      
    for( line = 0 ; line < lines ; ++line ) {
      for( col = 0 ; col < cols ; ++col ) {
        m1( line , col ) = counter;
        
        ++counter;
      }
    }
    
    counter = 0.0;
    
    for( line = 0 ; line < lines ; ++line ) {
      for( col = 0 ; col < cols ; ++col ) {
        TEAGN_TRUE_OR_THROW( ( m1( line , col ) == counter ), 
          "Element value check failed" );
          
        ++counter;          
      }
    }
    
    return;
}

void DiskMemPol_test()
{
    unsigned long int lines = 3000;
    unsigned long int cols = 4000;

    TePDIMatrix< double > m1;
    TEAGN_TRUE_OR_THROW( m1.Reset( lines, cols, 
      TePDIMatrix< double >::DiskMemPol ), "Matrix reset error" );      
      
    TEAGN_TRUE_OR_THROW( m1.getMemPolicy() == TePDIMatrix< double >::DiskMemPol, 
      "Invalid policy" );          
    TEAGN_TRUE_OR_THROW( m1.GetLines() == lines, "Invalid lines number" );
    TEAGN_TRUE_OR_THROW( m1.GetColumns() == cols, "Invalid columns number" );      
    
    unsigned int line = 0;
    unsigned int col = 0;
    double counter = 0.0;
      
    for( line = 0 ; line < lines ; ++line ) {
      for( col = 0 ; col < cols ; ++col ) {
        m1( line , col ) = counter;
        ++counter;
      }
    }
    
    counter = 0.0;
    
    for( line = 0 ; line < lines ; ++line ) {
      for( col = 0 ; col < cols ; ++col ) {
        TEAGN_TRUE_OR_THROW( ( m1( line , col ) == counter ), 
          "Element value check failed" );
        ++counter;
      }
    }
}


int main()
{
  TEAGN_LOGMSG( "Test started." );

  generic_tests();
  RAMMemPol_test();
  DiskMemPol_test();
  AutoMemPol_test();
  
  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
