#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIFilterMask.hpp>

#include <TeAgnostic.h>

//STL include files
#include <stdlib.h>


void printMask( TePDIFilterMask mask )
{
  std::cout << std::endl;

  for( unsigned int line = 0 ; line < mask.lines() ; ++line ) {
    std::cout << std::endl;

    for( unsigned int column = 0 ; column < mask.columns() ; ++column ) {
      std::cout << " " << mask.get( line, column );
    }
  }
}

int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    /* Checking mask creation */

    TePDIFilterMask mask1( 3, 0. );

    mask1.set( 0, 0, 1 );
    mask1.set( 0, 1, 1 );
    mask1.set( 0, 2, 1 );
    mask1.set( 1, 0, 1 );
    mask1.set( 1, 1, 1 );
    mask1.set( 1, 2, 1 );
    mask1.set( 2, 0, 1 );
    mask1.set( 2, 1, 1 );
    mask1.set( 2, 2, 1 );

    TEAGN_CHECK_EQUAL( mask1.get( 0, 0 ), 1., "Invalid value" );
    TEAGN_CHECK_EQUAL( mask1.get( 0, 1 ), 1., "Invalid value" );
    TEAGN_CHECK_EQUAL( mask1.get( 0, 2 ), 1., "Invalid value" );
    TEAGN_CHECK_EQUAL( mask1.get( 1, 0 ), 1., "Invalid value" );
    TEAGN_CHECK_EQUAL( mask1.get( 1, 1 ), 1., "Invalid value" );
    TEAGN_CHECK_EQUAL( mask1.get( 1, 2 ), 1., "Invalid value" );
    TEAGN_CHECK_EQUAL( mask1.get( 2, 0 ), 1., "Invalid value" );
    TEAGN_CHECK_EQUAL( mask1.get( 2, 1 ), 1., "Invalid value" );
    TEAGN_CHECK_EQUAL( mask1.get( 2, 2 ), 1., "Invalid value" );

    unsigned int cols = mask1.columns();
    TEAGN_CHECK_EQUAL( cols, 3, "Invalid columns number" );

    /* Checking weights matrix creation */

    double** wmatrix = mask1.getWeightsMatrix();

    TEAGN_CHECK_EQUAL( wmatrix[ 0 ][ 0 ], 1., "Invalid value" );
    TEAGN_CHECK_EQUAL( wmatrix[ 0 ][ 1 ], 1., "Invalid value" );
    TEAGN_CHECK_EQUAL( wmatrix[ 0 ][ 2 ], 1., "Invalid value" );
    TEAGN_CHECK_EQUAL( wmatrix[ 1 ][ 0 ], 1., "Invalid value" );
    TEAGN_CHECK_EQUAL( wmatrix[ 1 ][ 1 ], 1., "Invalid value" );
    TEAGN_CHECK_EQUAL( wmatrix[ 1 ][ 2 ], 1., "Invalid value" );
    TEAGN_CHECK_EQUAL( wmatrix[ 2 ][ 0 ], 1., "Invalid value" );
    TEAGN_CHECK_EQUAL( wmatrix[ 2 ][ 1 ], 1., "Invalid value" );
    TEAGN_CHECK_EQUAL( wmatrix[ 2 ][ 2 ], 1., "Invalid value" );

    TePDIFilterMask::deleteWeightsMatrix(
      wmatrix, mask1.lines() );


    /* Checking normalization procedure */

    mask1.normalize();

    TEAGN_CHECK_EPS(
      mask1.get( 0, 0 ), 0.111111, 0.000001, "Invalid value" );
    TEAGN_CHECK_EPS(
      mask1.get( 0, 1 ), 0.111111, 0.000001, "Invalid value" );
    TEAGN_CHECK_EPS(
      mask1.get( 0, 2 ), 0.111111, 0.000001, "Invalid value" );
    TEAGN_CHECK_EPS(
      mask1.get( 1, 0 ), 0.111111, 0.000001, "Invalid value" );
    TEAGN_CHECK_EPS(
      mask1.get( 1, 1 ), 0.111111, 0.000001, "Invalid value" );
    TEAGN_CHECK_EPS(
      mask1.get( 1, 2 ), 0.111111, 0.000001, "Invalid value" );
    TEAGN_CHECK_EPS(
      mask1.get( 2, 0 ), 0.111111, 0.000001, "Invalid value" );
    TEAGN_CHECK_EPS(
      mask1.get( 2, 1 ), 0.111111, 0.000001, "Invalid value" );
    TEAGN_CHECK_EPS(
      mask1.get( 2, 2 ), 0.111111, 0.000001, "Invalid value" );

    /* Checking pre-defined masks creation */

    TePDIFilterMask::pointer Dummy_mask = TePDIFilterMask::create_Dummy();
    //printMask( Dummy_mask );

    TePDIFilterMask::pointer Avg3x3_mask = TePDIFilterMask::create_Avg3x3();
    //printMask( Avg3x3_mask );

    TePDIFilterMask::pointer Avg5x5_mask = TePDIFilterMask::create_Avg5x5();
    //printMask( Avg5x5_mask );

    TePDIFilterMask::pointer Avg7x7_mask = TePDIFilterMask::create_Avg7x7();
    //printMask( Avg7x7_mask );

    TePDIFilterMask::pointer S_mask = TePDIFilterMask::create_S();
    //printMask( S_mask );

    TePDIFilterMask::pointer N_mask = TePDIFilterMask::create_N();
    //printMask( N_mask );

    TePDIFilterMask::pointer E_mask = TePDIFilterMask::create_E();
    //printMask( E_mask );

    TePDIFilterMask::pointer W_mask = TePDIFilterMask::create_W();
    //printMask( W_mask );

    TePDIFilterMask::pointer NW_mask = TePDIFilterMask::create_NW();
    //printMask( NW_mask );

    TePDIFilterMask::pointer SW_mask = TePDIFilterMask::create_SW();
    //printMask( SW_mask );

    TePDIFilterMask::pointer NE_mask = TePDIFilterMask::create_NE();
    //printMask( NE_mask );

    TePDIFilterMask::pointer SE_mask = TePDIFilterMask::create_SE();
    //printMask( SE_mask );

    TePDIFilterMask::pointer SE_NDLow = TePDIFilterMask::create_NDLow();
    //printMask( SE_NDLow );

    TePDIFilterMask::pointer SE_NDMed = TePDIFilterMask::create_NDMed();
    //printMask( SE_NDMed );

    TePDIFilterMask::pointer SE_NDHigh = TePDIFilterMask::create_NDHigh();
    //printMask( SE_NDHigh );

    TePDIFilterMask::pointer SE_TMEnh = TePDIFilterMask::create_TMEnh();
    //printMask( SE_TMEnh );

    /* Checking morfological masks creation */

    TePDIFilterMask::pointer MorfD = TePDIFilterMask::create_MorfD();
    TEAGN_TRUE_OR_THROW( MorfD->isMorfMask(), "Invalid mask" );
    TePDIFilterMask::pointer MorfE = TePDIFilterMask::create_MorfE();
    TEAGN_TRUE_OR_THROW( MorfE->isMorfMask(), "Invalid mask" );
    TePDIFilterMask::pointer MorfM = TePDIFilterMask::create_MorfM();
    TEAGN_TRUE_OR_THROW( MorfM->isMorfMask(), "Invalid mask" );
    TePDIFilterMask::pointer MorfM_ = TePDIFilterMask::create_MorfM_();
    TEAGN_TRUE_OR_THROW( MorfM_->isMorfMask(), "Invalid mask" );
    TePDIFilterMask::pointer MorfMPlus = TePDIFilterMask::create_MorfMPlus();
    TEAGN_TRUE_OR_THROW( MorfMPlus->isMorfMask(), "Invalid mask" );
    TePDIFilterMask::pointer MorfMtot = TePDIFilterMask::create_MorfMtot();
    TEAGN_TRUE_OR_THROW( MorfMtot->isMorfMask(), "Invalid mask" );
    TePDIFilterMask::pointer MorfMX = TePDIFilterMask::create_MorfMX();
    TEAGN_TRUE_OR_THROW( MorfMX->isMorfMask(), "Invalid mask" );


  }
  catch( const TeException& excpt ){
    TEAGN_LOGERR( excpt.message() )
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
