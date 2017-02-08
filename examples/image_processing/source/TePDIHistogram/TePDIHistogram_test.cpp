#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIHistogram.hpp>

#include <TePDIUtils.hpp>

#include <TeAgnostic.h>

#include <TeProgress.h>
#include <TeStdIOProgress.h>



#include <TeRaster.h>
#include <TeDataTypes.h>
#include <TeInitRasterDecoders.h>


void display_histogram( TePDIHistogram& hist )
{
  TePDIHistogram::iterator it;
  it = hist.begin();
  
  while( it != hist.end() ) {
    TEAGN_LOGMSG( it->first );
    TEAGN_LOGMSG( it->second );
    ++it;
  }
}

bool check_histogram_order( TePDIHistogram& hist )
{
  TePDIHistogram::iterator it1;
  TePDIHistogram::iterator it2;
  it1 = hist.begin();
  it2 = it1;
  ++it2;
  
  while( it2 != hist.end() ) {
    if( (*it1) > (*it2) ) {
      return false;
    }
    ++it1;
    ++it2;
  }
  
  return true;
}

void discrete_histogram_test_1()
{
    TePDITypes::TePDIRasterPtrType raster;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster, 1, 3, 1, false,
      TeUNSIGNEDCHAR, 0 ), "Unable to create raster" );

    raster->setElement( 0, 0, 1 );
    raster->setElement( 0, 1, 2 );
    raster->setElement( 0, 2, 3 );

    TePDIHistogram hist;

    TePDIHistogram::iterator it;

    /* Test 1 */

    TEAGN_TRUE_OR_THROW( hist.reset( raster, 0, 0, TeBoxPixelIn ),
      "Unable to create histogram" );
      
    TEAGN_TRUE_OR_THROW( check_histogram_order( hist ),
      "Histogram order error" );
      
    //display_histogram( hist );

    TEAGN_TRUE_OR_THROW( ( hist.size() == 3 ), "Invalid histogram size" );
    
    TEAGN_CHECK_EPS( hist.getTotalCount(), 3, 0, "Invalid total count" );
    
    TEAGN_TRUE_OR_THROW( hist.IsDiscrete(), "Discrete verification error" );
    TEAGN_TRUE_OR_THROW( hist.hasFixedStep(), "Step verification error" );

    it = hist.begin();

    TEAGN_CHECK_EQUAL( it->first, 1. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );

    ++it;
    
    TEAGN_CHECK_EQUAL( it->first, 2. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );
    
    ++it;    

    TEAGN_CHECK_EQUAL( it->first, 3. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );
    
    TEAGN_CHECK_EQUAL( hist.GetMinLevel(), 1., "" );
    TEAGN_CHECK_EQUAL( hist.GetMaxLevel(), 3., "" );

    TEAGN_CHECK_EQUAL( hist.GetMinCount(), 1, "" );
    TEAGN_CHECK_EQUAL( hist.GetMaxCount(), 1, "" );

    it = hist.begin();
    TePDIHistogram::iterator it_end = hist.end();
    unsigned int pixelsnmb = 0;
    while( it != it_end ) {
      pixelsnmb += it->second;
      ++it;
    }
    TEAGN_TRUE_OR_THROW( pixelsnmb == 3, "Invalid pixels number" );
}

void interpolated_histogram_test_1()
{
    TePDITypes::TePDIRasterPtrType raster;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster, 1, 5, 1, false,
      TeDOUBLE, 0 ), "Unable to create raster" );

    raster->setElement( 0, 0, 1 );
    raster->setElement( 0, 1, 1.9 );
    raster->setElement( 0, 2, 2 );
    raster->setElement( 0, 3, 2.1 );
    raster->setElement( 0, 4, 3 );

    TePDIHistogram hist;

    TePDIHistogram::iterator it;

    /* Test 1 */

    TEAGN_TRUE_OR_THROW( hist.reset( raster, 0, 0, TeBoxPixelIn ),
      "Unable to create histogram" );
      
    TEAGN_WATCH( hist.toString() );
      
    TEAGN_TRUE_OR_THROW( check_histogram_order( hist ),
      "Histogram order error" );
      
    //display_histogram( hist );

    TEAGN_TRUE_OR_THROW( ( hist.size() == 3 ), "Invalid histogram size" );

    TEAGN_CHECK_EPS( hist.getTotalCount(), 5, 0, "Invalid total count" );
            
    /* The histogram was generated from a float raster, but it
       must be a discrete histogram since the step between levels
       is an integer value ( 1.0 ) */
    TEAGN_TRUE_OR_THROW( hist.IsDiscrete(), 
      "Discrete verification error" );
      
    TEAGN_TRUE_OR_THROW( hist.hasFixedStep(), "Step verification error" );

    it = hist.begin();

    TEAGN_CHECK_EQUAL( it->first, 1. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );

    ++it;
    
    TEAGN_CHECK_EQUAL( it->first, 2. , "" );
    TEAGN_CHECK_EQUAL( it->second, 3 , "" );
    
    ++it;    

    TEAGN_CHECK_EQUAL( it->first, 3. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );
    
    TEAGN_CHECK_EQUAL( hist.GetMinLevel(), 1., "" );
    TEAGN_CHECK_EQUAL( hist.GetMaxLevel(), 3., "" );

    TEAGN_CHECK_EQUAL( hist.GetMinCount(), 1, "" );
    TEAGN_CHECK_EQUAL( hist.GetMaxCount(), 3, "" );

    it = hist.begin();
    TePDIHistogram::iterator it_end = hist.end();
    unsigned int pixelsnmb = 0;
    while( it != it_end ) {
      pixelsnmb += it->second;
      ++it;
    }
    TEAGN_TRUE_OR_THROW( pixelsnmb == 5, "Invalid pixels number" );
}


void discrete_histogram_test_2()
{
    TePDITypes::TePDIRasterPtrType raster;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster, 1, 3, 1, false,
      TeUNSIGNEDCHAR, 0 ), "Unable to create raster" );

    raster->setElement( 0, 0, 1 );
    raster->setElement( 0, 1, 2 );
    raster->setElement( 0, 2, 3 );

    TePDIHistogram hist;

    TePDIHistogram::iterator it;

    /* Test 1 */

    TEAGN_TRUE_OR_THROW( hist.reset( raster, 0, 0, TeBoxPixelIn ),
      "Unable to create histogram" );
      
    TEAGN_WATCH( hist.toString() );
      
    TEAGN_TRUE_OR_THROW( check_histogram_order( hist ),
      "Histogram order error" );

    TEAGN_TRUE_OR_THROW( ( hist.size() == 3 ), "Invalid histogram size" );
    
    TEAGN_CHECK_EPS( hist.getTotalCount(), 3, 0, "Invalid total count" );    
    
    TEAGN_TRUE_OR_THROW( hist.IsDiscrete(), "Discrete verification error" );
    TEAGN_TRUE_OR_THROW( hist.hasFixedStep(), "Step verification error" );

    it = hist.begin();

    TEAGN_CHECK_EQUAL( it->first, 1. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );

    ++it;
    
    TEAGN_CHECK_EQUAL( it->first, 2. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );
    
    ++it;    

    TEAGN_CHECK_EQUAL( it->first, 3. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );
    
    TEAGN_CHECK_EQUAL( hist.GetMinLevel(), 1., "" );
    TEAGN_CHECK_EQUAL( hist.GetMaxLevel(), 3., "" );

    TEAGN_CHECK_EQUAL( hist.GetMinCount(), 1, "" );
    TEAGN_CHECK_EQUAL( hist.GetMaxCount(), 1, "" );
    
    it = hist.begin();
    TePDIHistogram::iterator it_end = hist.end();
    unsigned int pixelsnmb = 0;
    while( it != it_end ) {
      pixelsnmb += it->second;
      ++it;
    }
    TEAGN_TRUE_OR_THROW( pixelsnmb == 3, "Invalid pixels number" );
}


void discrete_histogram_test_16bits()
{
    TePDITypes::TePDIRasterPtrType raster;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster, 1, 3, 1, false,
      TeUNSIGNEDSHORT, 0 ), "Unable to create raster" );

    raster->setElement( 0, 0, 1 );
    raster->setElement( 0, 1, 2 );
    raster->setElement( 0, 2, 3 );

    TePDIHistogram hist;

    TePDIHistogram::iterator it;

    /* Test 1 */

    TEAGN_TRUE_OR_THROW( hist.reset( raster, 0, 0, TeBoxPixelIn ),
      "Unable to create histogram" );
      
    TEAGN_TRUE_OR_THROW( check_histogram_order( hist ),
      "Histogram order error" );
      
    //display_histogram( hist );

    TEAGN_TRUE_OR_THROW( ( hist.size() == 3 ), "Invalid histogram size" );
    
    TEAGN_CHECK_EPS( hist.getTotalCount(), 3, 0, "Invalid total count" );    
    
    TEAGN_TRUE_OR_THROW( hist.IsDiscrete(), "Discrete verification error" );
    TEAGN_TRUE_OR_THROW( hist.hasFixedStep(), "Step verification error" );

    it = hist.begin();

    TEAGN_CHECK_EQUAL( it->first, 1. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );

    ++it;
    
    TEAGN_CHECK_EQUAL( it->first, 2. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );
    
    ++it;    

    TEAGN_CHECK_EQUAL( it->first, 3. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );
    
    TEAGN_CHECK_EQUAL( hist.GetMinLevel(), 1., "" );
    TEAGN_CHECK_EQUAL( hist.GetMaxLevel(), 3., "" );

    TEAGN_CHECK_EQUAL( hist.GetMinCount(), 1, "" );
    TEAGN_CHECK_EQUAL( hist.GetMaxCount(), 1, "" );

    it = hist.begin();
    TePDIHistogram::iterator it_end = hist.end();
    unsigned int pixelsnmb = 0;
    while( it != it_end ) {
      pixelsnmb += it->second;
      ++it;
    }
    TEAGN_TRUE_OR_THROW( pixelsnmb == 3, "Invalid pixels number" );
}


void normal_levels_test2()
{
    TePDITypes::TePDIRasterPtrType raster;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster, 1, 1, 2, false,
      TeUNSIGNEDCHAR, 0 ), "Unable to create raster" );

    raster->setElement( 0, 0, 1 );
    raster->setElement( 1, 0, 2 );

    TePDIHistogram hist;

    TePDIHistogram::iterator it;

    /* Test 1 */

    TEAGN_TRUE_OR_THROW( hist.reset( raster, 0, 2, TeBoxPixelIn ),
      "Unable to create histogram" );
      
    TEAGN_TRUE_OR_THROW( check_histogram_order( hist ),
      "Histogram order error" );
                
    //display_histogram( hist );

    TEAGN_TRUE_OR_THROW( ( hist.size() == 2 ), "Invalid histogram size" );
    
    TEAGN_CHECK_EPS( hist.getTotalCount(), 2, 0, "Invalid total count" );
        
    TEAGN_TRUE_OR_THROW( hist.hasFixedStep(), "Step verification error" );

    it = hist.begin();

    TEAGN_CHECK_EPS( it->first, 1., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 2., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    TEAGN_CHECK_EPS( hist.GetMinLevel(), 1., 0, "" );
    TEAGN_CHECK_EPS( hist.GetMaxLevel(), 2., 0, "" );

    TEAGN_CHECK_EPS( hist.GetMinCount(), 1., 0, "" );
    TEAGN_CHECK_EPS( hist.GetMaxCount(), 1., 0, "" );

    it = hist.begin();
    TePDIHistogram::iterator it_end = hist.end();
    unsigned int pixelsnmb = 0;
    while( it != it_end ) {
      pixelsnmb += it->second;
      ++it;
    }
    TEAGN_TRUE_OR_THROW( pixelsnmb == 2, "Invalid pixels number" );
}


void normal_levels_test3()
{
    TePDITypes::TePDIRasterPtrType raster;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster, 1, 1, 3, false,
      TeUNSIGNEDCHAR, 0 ), "Unable to create raster" );

    raster->setElement( 0, 0, 1 );
    raster->setElement( 1, 0, 2 );
    raster->setElement( 2, 0, 3 );

    TePDIHistogram hist;

    TePDIHistogram::iterator it;

    /* Test 1 */

    TEAGN_TRUE_OR_THROW( hist.reset( raster, 0, 3, TeBoxPixelIn ),
      "Unable to create histogram" );
      
    TEAGN_TRUE_OR_THROW( check_histogram_order( hist ),
      "Histogram order error" );      
      
    //display_histogram( hist );

    TEAGN_TRUE_OR_THROW( ( hist.size() == 3 ), "Invalid histogram size" );
    
    TEAGN_CHECK_EPS( hist.getTotalCount(), 3, 0, "Invalid total count" );
        
    TEAGN_TRUE_OR_THROW( hist.hasFixedStep(), "Step verification error" );

    it = hist.begin();

    TEAGN_CHECK_EPS( it->first, 1., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 2., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );
    
    ++it;

    TEAGN_CHECK_EPS( it->first, 3., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );
    

    TEAGN_CHECK_EPS( hist.GetMinLevel(), 1., 0, "" );
    TEAGN_CHECK_EPS( hist.GetMaxLevel(), 3., 0, "" );

    TEAGN_CHECK_EPS( hist.GetMinCount(), 1., 0, "" );
    TEAGN_CHECK_EPS( hist.GetMaxCount(), 1., 0, "" );

    it = hist.begin();
    TePDIHistogram::iterator it_end = hist.end();
    unsigned int pixelsnmb = 0;
    while( it != it_end ) {
      pixelsnmb += it->second;
      ++it;
    }
    TEAGN_TRUE_OR_THROW( pixelsnmb == 3, "Invalid pixels number" );
}


void normal_levels_test4()
{
    TePDITypes::TePDIRasterPtrType raster;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster, 1, 2, 2, false,
      TeUNSIGNEDCHAR, 0 ), "Unable to create raster" );

    raster->setElement( 0, 0, 1 );
    raster->setElement( 1, 0, 2 );
    raster->setElement( 0, 1, 3 );
    raster->setElement( 1, 1, 4 );

    TePDIHistogram hist;

    TePDIHistogram::iterator it;

    /* Test 1 */

    TEAGN_TRUE_OR_THROW( hist.reset( raster, 0, 4, TeBoxPixelIn ),
      "Unable to create histogram" );
      
    TEAGN_TRUE_OR_THROW( check_histogram_order( hist ),
      "Histogram order error" );      
      
    //display_histogram( hist );

    TEAGN_TRUE_OR_THROW( ( hist.size() == 4 ), "Invalid histogram size" );
    
    TEAGN_CHECK_EPS( hist.getTotalCount(), 4, 0, "Invalid total count" );
        
    TEAGN_TRUE_OR_THROW( hist.hasFixedStep(), "Step verification error" );

    it = hist.begin();

    TEAGN_CHECK_EPS( it->first, 1., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 2., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 3., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 4., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    TEAGN_CHECK_EPS( hist.GetMinLevel(), 1., 0, "" );
    TEAGN_CHECK_EPS( hist.GetMaxLevel(), 4., 0, "" );

    TEAGN_CHECK_EPS( hist.GetMinCount(), 1., 0, "" );
    TEAGN_CHECK_EPS( hist.GetMaxCount(), 1., 0, "" );

    it = hist.begin();
    TePDIHistogram::iterator it_end = hist.end();
    unsigned int pixelsnmb = 0;
    while( it != it_end ) {
      pixelsnmb += it->second;
      ++it;
    }
    TEAGN_TRUE_OR_THROW( pixelsnmb == 4, "Invalid pixels number" );
}


void auto_levels_test()
{
    TePDITypes::TePDIRasterPtrType raster;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster, 1, 2, 2, false,
      TeUNSIGNEDCHAR, 0 ), "Unable to create raster" );

    raster->setElement( 0, 0, 1 );
    raster->setElement( 1, 0, 2 );
    raster->setElement( 0, 1, 3 );
    raster->setElement( 1, 1, 4 );

    TePDIHistogram hist;

    TePDIHistogram::iterator it;

    TEAGN_TRUE_OR_THROW( hist.reset( raster, 0, 0, TeBoxPixelIn ),
      "Unable to create histogram" );
      
    TEAGN_TRUE_OR_THROW( check_histogram_order( hist ),
      "Histogram order error" );      

    TEAGN_TRUE_OR_THROW( ( hist.size() == 4 ), "Invalid histogram size" );
    
    TEAGN_CHECK_EPS( hist.getTotalCount(), 4, 0, "Invalid total count" );
        
    TEAGN_TRUE_OR_THROW( hist.hasFixedStep(), "Step verification error" );

    it = hist.begin();

    TEAGN_CHECK_EPS( it->first, 1., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 2., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 3., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );
    
    ++it;

    TEAGN_CHECK_EPS( it->first, 4., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    TEAGN_CHECK_EPS( hist.GetMinLevel(), 1, 0, "" );
    TEAGN_CHECK_EPS( hist.GetMaxLevel(), 4., 0, "" );

    TEAGN_CHECK_EPS( hist.GetMinCount(), 1., 0, "" );
    TEAGN_CHECK_EPS( hist.GetMaxCount(), 1., 0, "" );    

    it = hist.begin();
    TePDIHistogram::iterator it_end = hist.end();
    unsigned int pixelsnmb = 0;
    while( it != it_end ) {
      pixelsnmb += it->second;
      ++it;
    }
    TEAGN_TRUE_OR_THROW( pixelsnmb == 4, "Invalid pixels number" );
}


void discretize_test()
{
    TePDITypes::TePDIRasterPtrType raster;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster, 1, 2, 2, false,
      TeUNSIGNEDCHAR, 0 ), "Unable to create raster" );

    raster->setElement( 0, 0, 1 );
    raster->setElement( 1, 0, 2 );
    raster->setElement( 0, 1, 3 );
    raster->setElement( 1, 1, 4 );

    TePDIHistogram hist;

    TePDIHistogram::iterator it;

    TEAGN_TRUE_OR_THROW( hist.reset( raster, 0, 6, TeBoxPixelIn ),
      "Unable to create histogram" );
      
    TEAGN_WATCH( hist.toString() )
      
    TEAGN_TRUE_OR_THROW( check_histogram_order( hist ),
      "Histogram order error" );            

    TEAGN_TRUE_OR_THROW( ( hist.size() == 4 ), "Invalid histogram size" );
    
    TEAGN_CHECK_EPS( hist.getTotalCount(), 4, 0, "Invalid total count" );
        
    TEAGN_TRUE_OR_THROW( ( ! hist.hasFixedStep() ), 
      "Step verification error" );
    
    //display_histogram( hist );

    it = hist.begin();

    TEAGN_CHECK_EPS( it->first, 1., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 2.20, 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 2.8, 0.0000001, "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 4, 0.0000001, "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );


    TEAGN_CHECK_EPS( hist.GetMinLevel(), 1., 0, "" );
    TEAGN_CHECK_EPS( hist.GetMaxLevel(), 4., 0, "" );
    
    TEAGN_CHECK_EPS( hist.GetMinCount(), 1., 0, "" );
    TEAGN_CHECK_EPS( hist.GetMaxCount(), 1., 0, "" );    

    it = hist.begin();
    TePDIHistogram::iterator it_end = hist.end();
    unsigned int pixelsnmb = 0;
    while( it != it_end ) {
      pixelsnmb += it->second;
      ++it;
    }
    TEAGN_TRUE_OR_THROW( pixelsnmb == 4, "Invalid pixels number" );


    TEAGN_TRUE_OR_THROW( ! hist.IsDiscrete(),
      "Discretize verification error" );
    TEAGN_TRUE_OR_THROW( hist.Discretize(), "Unable to discretize" );
    TEAGN_TRUE_OR_THROW( hist.IsDiscrete(), "Discretize verification error" );
    TEAGN_WATCH( hist.toString() )
    
    TEAGN_TRUE_OR_THROW( check_histogram_order( hist ),
      "Histogram order error" );  
    TEAGN_TRUE_OR_THROW( hist.hasFixedStep(), "Step verification error" );  
    
    //display_histogram( hist );

    TEAGN_TRUE_OR_THROW( ( hist.size() == 4 ), "Invalid histogram size" );
    
    TEAGN_CHECK_EPS( hist.getTotalCount(), 4, 0, "Invalid total count" );

    it = hist.begin();

    TEAGN_CHECK_EPS( it->first, 1., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 2., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 3., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 4., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    TEAGN_CHECK_EPS( hist.GetMinLevel(), 1., 0, "" );
    TEAGN_CHECK_EPS( hist.GetMaxLevel(), 4., 0, "" );

    TEAGN_CHECK_EPS( hist.GetMinCount(), 1., 0, "" );
    TEAGN_CHECK_EPS( hist.GetMaxCount(), 1., 0, "" );

    it = hist.begin();
    it_end = hist.end();
    pixelsnmb = 0;
    while( it != it_end ) {
      pixelsnmb += it->second;
      ++it;
    }
    TEAGN_TRUE_OR_THROW( pixelsnmb == 4, "Invalid pixels number" );
}


void performance_test()
{
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 
    'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  
  TePDIHistogram hist;

  TEAGN_TRUE_OR_THROW( hist.reset( inRaster, 0, 0, TeBoxPixelIn ),
    "Unable to create histogram" );
    
  TEAGN_WATCH( hist.toString() )
    
  TEAGN_TRUE_OR_THROW( check_histogram_order( hist ),
    "Histogram order error" );    
    
  TEAGN_CHECK_EPS( hist.size(), 180, 0, "Invalid histogram size" );
  
  TEAGN_CHECK_EPS( hist.getTotalCount(), 720657, 0, "Invalid total count" );
}


void operator_equal_from_map_test()
{
  std::map< double, unsigned int > simple_map1;
  simple_map1[ 0.0 ] = 1;
  simple_map1[ 1.0 ] = 2;
  simple_map1[ 2.0 ] = 3;
  simple_map1[ 3.0 ] = 4;
  
  TePDIHistogram hist1;
  hist1 = simple_map1;
  
  TEAGN_TRUE_OR_THROW( ( hist1.size() == 4 ), "Invalid histogram size" );
  
  TEAGN_CHECK_EPS( hist1.getTotalCount(), 10, 0, "Invalid total count" );
  
  TEAGN_TRUE_OR_THROW( check_histogram_order( hist1 ),
    "Histogram order error" );
  TEAGN_CHECK_EPS( hist1.GetMinLevel(), 0, 0, "" );
  TEAGN_CHECK_EPS( hist1.GetMaxLevel(), 3, 0, "" );
  TEAGN_CHECK_EPS( hist1.GetMinCount(), 1, 0, "" );
  TEAGN_CHECK_EPS( hist1.GetMaxCount(), 4, 0, "" );  
  TEAGN_TRUE_OR_THROW( hist1.hasFixedStep(), "Step verification error" );
  
  {
    TePDIHistogram::iterator it = hist1.begin();

    TEAGN_CHECK_EPS( it->first, 0., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 1., 0., "" );
    TEAGN_CHECK_EPS( it->second, 2., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 2., 0., "" );
    TEAGN_CHECK_EPS( it->second, 3., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 3., 0., "" );
    TEAGN_CHECK_EPS( it->second, 4., 0., "" );  
  }
  
  /* Testing with external floating point levels map */
  
  std::map< double, unsigned int > simple_map2;
  simple_map2[ 0.0 ] = 1;
  simple_map2[ 1.1 ] = 2;
  simple_map2[ 2.0 ] = 3;
  simple_map2[ 3.0 ] = 4;
  
  TePDIHistogram hist2;
  hist2 = simple_map2;
  
  //display_histogram( hist2 );
  
  TEAGN_TRUE_OR_THROW( ( hist2.size() == 4 ), "Invalid histogram size" );
  
  TEAGN_CHECK_EPS( hist2.getTotalCount(), 10, 0, "Invalid total count" );
  
  TEAGN_TRUE_OR_THROW( check_histogram_order( hist2 ),
    "Histogram order error" );
  TEAGN_CHECK_EPS( hist2.GetMinLevel(), 0, 0, "" );
  TEAGN_CHECK_EPS( hist2.GetMaxLevel(), 3, 0, "" );
  TEAGN_CHECK_EPS( hist2.GetMinCount(), 1, 0, "" );
  TEAGN_CHECK_EPS( hist2.GetMaxCount(), 4, 0, "" );  
  TEAGN_TRUE_OR_THROW( ( hist2.hasFixedStep() ), "Step verification error" );
  
  {
    TePDIHistogram::iterator it = hist2.begin();

    TEAGN_CHECK_EPS( it->first, 0., 0., "" );
    TEAGN_CHECK_EPS( it->second, 1., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 1., 0., "" );
    TEAGN_CHECK_EPS( it->second, 2., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 2., 0., "" );
    TEAGN_CHECK_EPS( it->second, 3., 0., "" );

    ++it;

    TEAGN_CHECK_EPS( it->first, 3., 0., "" );
    TEAGN_CHECK_EPS( it->second, 4., 0., "" );  
  }  
}


void histogram_from_TeCHAR_test()
{
    TePDITypes::TePDIRasterPtrType raster;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster, 1, 3, 1, false,
      TeCHAR, 0 ), "Unable to create raster" );

    raster->setElement( 0, 0, -1 );
    raster->setElement( 0, 1, 0 );
    raster->setElement( 0, 2, 1 );

    TePDIHistogram hist;

    TePDIHistogram::iterator it;

    /* Test 1 */

    TEAGN_TRUE_OR_THROW( hist.reset( raster, 0, 0, TeBoxPixelIn ),
      "Unable to create histogram" );
      
    TEAGN_TRUE_OR_THROW( check_histogram_order( hist ),
      "Histogram order error" );
      
    //display_histogram( hist );
      
    TEAGN_TRUE_OR_THROW( ( hist.size() == 3 ), "Invalid histogram size" );
    
    TEAGN_CHECK_EPS( hist.getTotalCount(), 3, 0, "Invalid total count" );    
    
    it = hist.begin();

    TEAGN_CHECK_EQUAL( it->first, -1. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );

    ++it;
    
    TEAGN_CHECK_EQUAL( it->first, 0. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );
    
    ++it;    

    TEAGN_CHECK_EQUAL( it->first, 1. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );
    
    TEAGN_TRUE_OR_THROW( hist.IsDiscrete(), "Discrete verification error" );
    TEAGN_TRUE_OR_THROW( hist.hasFixedStep(), "Step verification error" );    
    
    TEAGN_CHECK_EQUAL( hist.GetMinLevel(), -1., "" );
    TEAGN_CHECK_EQUAL( hist.GetMaxLevel(), 1., "" );

    TEAGN_CHECK_EQUAL( hist.GetMinCount(), 1, "" );
    TEAGN_CHECK_EQUAL( hist.GetMaxCount(), 1, "" );

    it = hist.begin();
    TePDIHistogram::iterator it_end = hist.end();
    unsigned int pixelsnmb = 0;
    while( it != it_end ) {
      pixelsnmb += it->second;
      ++it;
    }
    TEAGN_TRUE_OR_THROW( pixelsnmb == 3, "Invalid pixels number" );
}


void histogram_from_TeSHORT_test()
{
    TePDITypes::TePDIRasterPtrType raster;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster, 1, 3, 1, false,
      TeSHORT, 0 ), "Unable to create raster" );

    raster->setElement( 0, 0, -1 );
    raster->setElement( 0, 1, 0 );
    raster->setElement( 0, 2, 1 );

    TePDIHistogram hist;

    TePDIHistogram::iterator it;

    /* Test 1 */

    TEAGN_TRUE_OR_THROW( hist.reset( raster, 0, 0, TeBoxPixelIn ),
      "Unable to create histogram" );
      
    TEAGN_TRUE_OR_THROW( check_histogram_order( hist ),
      "Histogram order error" );
      
    //display_histogram( hist );

    TEAGN_TRUE_OR_THROW( ( hist.size() == 3 ), "Invalid histogram size" );
    
    TEAGN_CHECK_EPS( hist.getTotalCount(), 3, 0, "Invalid total count" );    
    
    it = hist.begin();

    TEAGN_CHECK_EQUAL( it->first, -1. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );

    ++it;
    
    TEAGN_CHECK_EQUAL( it->first, 0. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );
    
    ++it;    

    TEAGN_CHECK_EQUAL( it->first, 1. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );
    
    TEAGN_TRUE_OR_THROW( hist.IsDiscrete(), "Discrete verification error" );
    TEAGN_TRUE_OR_THROW( hist.hasFixedStep(), "Step verification error" );    
    
    TEAGN_CHECK_EQUAL( hist.GetMinLevel(), -1., "" );
    TEAGN_CHECK_EQUAL( hist.GetMaxLevel(), 1., "" );

    TEAGN_CHECK_EQUAL( hist.GetMinCount(), 1, "" );
    TEAGN_CHECK_EQUAL( hist.GetMaxCount(), 1, "" );

    it = hist.begin();
    TePDIHistogram::iterator it_end = hist.end();
    unsigned int pixelsnmb = 0;
    while( it != it_end ) {
      pixelsnmb += it->second;
      ++it;
    }
    TEAGN_TRUE_OR_THROW( pixelsnmb == 3, "Invalid pixels number" );
}


void discrete_histogram_with_external_polygonset_test()
{
    TePDITypes::TePDIRasterPtrType raster;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster, 1, 3, 1, false,
      TeUNSIGNEDCHAR, 0 ), "Unable to create raster" );

    raster->setElement( 0, 0, 1 );
    raster->setElement( 0, 1, 2 );
    raster->setElement( 0, 2, 3 );

    TePDIHistogram hist;

    TePDIHistogram::iterator it;
    
    /* Creating polygon set from raster box */
    
    TeSharedPtr< TePolygonSet > polsetptr( new TePolygonSet );
    TeBox rasterbox( raster->params().boundingBox() );
    polsetptr->add( polygonFromBox( rasterbox ) );    

    /* Test 1 */

    TEAGN_TRUE_OR_THROW( hist.reset( raster, 0, 0,  
      TeBoxPixelIn, polsetptr ),
      "Unable to create histogram" );
      
    TEAGN_TRUE_OR_THROW( check_histogram_order( hist ),
      "Histogram order error" );
      
    //display_histogram( hist );

    TEAGN_TRUE_OR_THROW( ( hist.size() == 3 ), "Invalid histogram size" );
    
    TEAGN_CHECK_EPS( hist.getTotalCount(), 3, 0, "Invalid total count" );    
    
    TEAGN_TRUE_OR_THROW( hist.IsDiscrete(), "Discrete verification error" );
    TEAGN_TRUE_OR_THROW( hist.hasFixedStep(), "Step verification error" );

    it = hist.begin();

    TEAGN_CHECK_EQUAL( it->first, 1. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );

    ++it;
    
    TEAGN_CHECK_EQUAL( it->first, 2. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );
    
    ++it;    

    TEAGN_CHECK_EQUAL( it->first, 3. , "" );
    TEAGN_CHECK_EQUAL( it->second, 1 , "" );
    
    TEAGN_CHECK_EQUAL( hist.GetMinLevel(), 1., "" );
    TEAGN_CHECK_EQUAL( hist.GetMaxLevel(), 3., "" );

    TEAGN_CHECK_EQUAL( hist.GetMinCount(), 1, "" );
    TEAGN_CHECK_EQUAL( hist.GetMaxCount(), 1, "" );

    it = hist.begin();
    TePDIHistogram::iterator it_end = hist.end();
    unsigned int pixelsnmb = 0;
    while( it != it_end ) {
      pixelsnmb += it->second;
      ++it;
    }
    TEAGN_TRUE_OR_THROW( pixelsnmb == 3, "Invalid pixels number" );
}

void palette_based_histogram_test()
{
  /* Building palette based raster */
  
  TeRasterParams params;
  params.setNLinesNColumns( 3, 1 );
  params.nBands( 1 );
  params.setDataType( TeUNSIGNEDCHAR );
  params.setPhotometric( TeRasterParams::TePallete, -1 );
  params.lutr_.push_back( 1 );
  params.lutr_.push_back( 2 );
  params.lutr_.push_back( 4 );

  TePDITypes::TePDIRasterPtrType raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster, params,
    TePDIUtils::TePDIUtilsAutoMemPol ), "Unable to create raster" );

  TEAGN_TRUE_OR_THROW( raster->setElement( 0, 0, 0 ), "setElement error" );
  TEAGN_TRUE_OR_THROW( raster->setElement( 0, 1, 1 ), "setElement error" );
  TEAGN_TRUE_OR_THROW( raster->setElement( 0, 2, 2 ), "setElement error" );

  TePDIHistogram hist;

  TePDIHistogram::iterator it;

  /* Test 1 */

  TEAGN_TRUE_OR_THROW( hist.reset( raster, 0, 0,  
    TeBoxPixelIn ),
    "Unable to create histogram" );
    
  TEAGN_WATCH( hist.toString() )
    
  TEAGN_TRUE_OR_THROW( check_histogram_order( hist ),
    "Histogram order error" );
    
  //display_histogram( hist );

  TEAGN_TRUE_OR_THROW( ( hist.size() == 3 ), "Invalid histogram size" );
  
  TEAGN_CHECK_EPS( hist.getTotalCount(), 3, 0, "Invalid total count" );
  
  TEAGN_TRUE_OR_THROW( hist.IsDiscrete(), "Discrete verification error" );

  it = hist.begin();
  
  TEAGN_CHECK_EQUAL( it->first, 1. , "" );
  TEAGN_CHECK_EQUAL( it->second, 1 , "" );
  
  ++it;    

  TEAGN_CHECK_EQUAL( it->first, 2. , "" );
  TEAGN_CHECK_EQUAL( it->second, 1 , "" );

  ++it;    

  TEAGN_CHECK_EQUAL( it->first, 4. , "" );
  TEAGN_CHECK_EQUAL( it->second, 1 , "" );
    
  
  TEAGN_CHECK_EQUAL( hist.GetMinLevel(), 1., "" );
  TEAGN_CHECK_EQUAL( hist.GetMaxLevel(), 4., "" );

  TEAGN_CHECK_EQUAL( hist.GetMinCount(), 1, "" );
  TEAGN_CHECK_EQUAL( hist.GetMaxCount(), 1, "" );

  it = hist.begin();
  TePDIHistogram::iterator it_end = hist.end();
  unsigned int pixelsnmb = 0;
  while( it != it_end ) {
    pixelsnmb += it->second;
    ++it;
  }
  TEAGN_TRUE_OR_THROW( pixelsnmb == 3, "Invalid pixels number" );
}


int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TeInitRasterDecoders();
  
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) ); 
    
    discrete_histogram_test_1();
    discrete_histogram_test_2();
    discrete_histogram_test_16bits();
    interpolated_histogram_test_1();
    normal_levels_test2();
    normal_levels_test3();
    normal_levels_test4();
    auto_levels_test();
    discretize_test();
    performance_test();
    operator_equal_from_map_test();
    histogram_from_TeCHAR_test();
    histogram_from_TeSHORT_test();
    discrete_histogram_with_external_polygonset_test();
    palette_based_histogram_test();
  }
  catch( const TeException& excpt ){
    TEAGN_LOGERR( excpt.message() )
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}

