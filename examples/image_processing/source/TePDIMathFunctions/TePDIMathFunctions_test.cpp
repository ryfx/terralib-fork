#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIMathFunctions.hpp>
#include <TeAgnostic.h>


void DecimLevels_test()
{
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevels( 0 ) == 0 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevels( 1 ) == 0 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevels( 2 ) == 1 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevels( 3 ) == 2 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevels( 4 ) == 2 ),
    "Invalid result" );
}


void DecimLevelSize_test()
{
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 1, 2 ) == 1 ),
    "Invalid result" );
    
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 1, 3 ) == 2 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 2, 3 ) == 1 ),
    "Invalid result" );
    
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 1, 4 ) == 2 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 2, 4 ) == 1 ),
    "Invalid result" );

  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 1, 5 ) == 3 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 2, 5 ) == 2 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 3, 5 ) == 1 ),
    "Invalid result" );
    
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 1, 6 ) == 3 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 2, 6 ) == 2 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 3, 6 ) == 1 ),
    "Invalid result" );
    
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 1, 7 ) == 4 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 2, 7 ) == 2 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 3, 7 ) == 1 ),
    "Invalid result" );
    
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 1, 8 ) == 4 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 2, 8 ) == 2 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 3, 8 ) == 1 ),
    "Invalid result" );
    
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 1, 9 ) == 5 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 2, 9 ) == 3 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 3, 9 ) == 2 ),
    "Invalid result" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIMathFunctions::DecimLevelSize( 4, 9 ) == 1 ),
    "Invalid result" );
}


int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    DecimLevels_test();
    DecimLevelSize_test();
  }
  catch( const TeException& excpt ){
    TEAGN_LOGERR( excpt.message() )
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
