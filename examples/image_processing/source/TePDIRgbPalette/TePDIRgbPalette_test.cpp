#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIUtils.hpp>

#include <TeInitRasterDecoders.h>

#include <TeAgnostic.h>
#include <TePDIRgbPalette.hpp>

int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TePDIRgbPalette pal;

    pal[ 1.0 ] = TePDIRgbPaletteNode( 1, 1, 1 );

    TePDIRgbPalette::iterator it = pal.begin();

    TEAGN_TRUE_OR_THROW( ( it->first == 1. ), "Invalid value" );
    TEAGN_TRUE_OR_THROW( ( it->second.red_ == 1 ), "Invalid value" );
    TEAGN_TRUE_OR_THROW( ( it->second.green_ == 1 ), "Invalid value" );
    TEAGN_TRUE_OR_THROW( ( it->second.blue_ == 1 ), "Invalid value" );

  }
  catch( const TeException& e ){
    TEAGN_LOGERR( "Test Failed - " + e.message() );
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
