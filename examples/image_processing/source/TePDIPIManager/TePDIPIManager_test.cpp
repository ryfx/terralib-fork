#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIPIManager.hpp>


#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TeAgnostic.h>

#include <stdlib.h>

int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );  
    
    /* Test 0 */
    {
      TePDIPIManager manager1( "Manager1", 100, true );
      manager1.Update( 50 );
      manager1.Update( 100 );
    }    
  
    /* Test 1 */
    {
      TePDIPIManager manager1( "Manager1", 100, true );
      manager1.Update( 25 );
      
      TePDIPIManager manager2( "Manager2", 100, true );
      manager2.Update( 50 );
      
      TePDIPIManager manager3( "Manager3", 100, true );
      manager3.Update( 75 );   
      
      manager1.Update( 100 );  
      manager2.Update( 100 ); 
      manager3.Update( 100 );  
    }
    
    /* Test 2 */
    {
      TePDIPIManager manager1( "Manager1", 100, true );
      TePDIPIManager manager2( "Manager2", 100, true );
      TePDIPIManager manager3( "Manager3", 100, true );
      
      manager1.Update( 50 );
      manager2.Update( 50 );
      manager3.Update( 50 ); 
      
      manager1.Update( 100 );
      manager2.Update( 100 );
      manager3.Update( 100 );       
    }    
  }
  catch( const TeException& e ){
    TEAGN_LOGERR( "Test Failed - " + e.message() );
    return EXIT_FAILURE;
  }  

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
