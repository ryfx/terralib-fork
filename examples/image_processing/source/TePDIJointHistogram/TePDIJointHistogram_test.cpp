#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIJointHistogram.hpp>
#include <TePDIHistogram.hpp>

#include <TePDIUtils.hpp>

#include <TeAgnostic.h>

#include <TeProgress.h>
#include <TeStdIOProgress.h>

void jointHistogram_8bits_test()
{
    // Generating test rasters

    TePDITypes::TePDIRasterPtrType raster1;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster1, 1, 2, 2, 
      false, TeUNSIGNEDCHAR, 0 ), "Unable to create raster" );
    TEAGN_TRUE_OR_THROW( raster1->setElement( 0, 0, 1, 0 ), "error" )
    TEAGN_TRUE_OR_THROW( raster1->setElement( 0, 1, 2, 0 ), "error" )
    TEAGN_TRUE_OR_THROW( raster1->setElement( 1, 0, 1, 0 ), "error" );
    TEAGN_TRUE_OR_THROW( raster1->setElement( 1, 1, 2, 0 ), "error" );
    
    TePDITypes::TePDIRasterPtrType raster2;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster2, 1, 2, 2, 
      false, TeUNSIGNEDCHAR, 0 ), "Unable to create raster" );
    TEAGN_TRUE_OR_THROW( raster2->setElement( 0, 0, 1, 0 ), "error" );
    TEAGN_TRUE_OR_THROW( raster2->setElement( 0, 1, 3, 0 ), "error" );
    TEAGN_TRUE_OR_THROW( raster2->setElement( 1, 0, 1, 0 ), "error" );
    TEAGN_TRUE_OR_THROW( raster2->setElement( 1, 1, 3, 0 ), "error" );

    // Generating joint histogram

    TePDIJointHistogram hist;

    TEAGN_TRUE_OR_THROW( hist.update( *raster1, 0, *raster2, 0,
      TeBoxPixelIn, 0 ), "Unable to create histogram" );
      
    TEAGN_WATCH( hist.toString() );
      
    // Checking histogram
    {
      TEAGN_TRUE_OR_THROW( ( hist.size() == 2 ), "Invalid histogram size" );
      
      TEAGN_TRUE_OR_THROW( ( hist.getFreqSum() == 4 ), 
        "Invalid histogram" );
  
      TePDIJointHistogram::const_iterator it = hist.begin();
  
      TEAGN_CHECK_EQUAL( it->first.first, 1 , "" );
      TEAGN_CHECK_EQUAL( it->first.second, 1 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;
  
      TEAGN_CHECK_EQUAL( it->first.first, 2 , "" );
      TEAGN_CHECK_EQUAL( it->first.second, 3 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;
    }
    // Checking raster 1 histogram
    
    {
      const TePDIHistogram& r1hist = hist.getRaster1Hist();
      TEAGN_TRUE_OR_THROW( ( r1hist.size() == 2 ), "Invalid histogram size" );
  
      TePDIHistogram::const_iterator it = r1hist.begin();
  
      TEAGN_CHECK_EQUAL( it->first, 1 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;
  
      TEAGN_CHECK_EQUAL( it->first, 2 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;      
    }    
    
    // Checking raster 2 histogram
 
    {
      const TePDIHistogram& r2hist = hist.getRaster2Hist();
      TEAGN_TRUE_OR_THROW( ( r2hist.size() == 2 ), "Invalid histogram size" );
  
      TePDIHistogram::const_iterator it = r2hist.begin();
  
      TEAGN_CHECK_EQUAL( it->first, 1 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;
            
      TEAGN_CHECK_EQUAL( it->first, 3 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;      
    }     
}

void jointHistogram_8bits_images_test()
{
  // Initializing raster instances
  
  TePDITypes::TePDIRasterPtrType inRaster1( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 
    'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster1->init(), "Unable to init inRaster" );

  TePDITypes::TePDIRasterPtrType inRaster2( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 
    'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster2->init(), "Unable to init inRaster" );

  // Generating joint histogram

  TePDIJointHistogram jhist;

  TEAGN_TRUE_OR_THROW( jhist.update( *inRaster1, 0, *inRaster2, 1,
    TeBoxPixelIn, 0 ), "Unable to create histogram" );  
    
  TEAGN_CHECK_EPS( jhist.getFreqSum(), ( inRaster1->params().nlines_ *
    inRaster1->params().ncols_ ), 0, "Invalid histogram" );    
  
  // Generating single histograms (just for comparison)
  
  TePDIHistogram hist1;
  TEAGN_TRUE_OR_THROW( hist1.reset( inRaster1, 0, 0, TeBoxPixelIn ),
    "Unable to create histogram" );

  TePDIHistogram hist2;
  TEAGN_TRUE_OR_THROW( hist2.reset( inRaster2, 1, 0, TeBoxPixelIn ),
    "Unable to create histogram" );
    
  // Comparing single histograms with that ones created by the joint histogram.
  
  const TePDIHistogram& hist1b = jhist.getRaster1Hist();
  const TePDIHistogram& hist2b = jhist.getRaster2Hist();
  
  TEAGN_TRUE_OR_THROW( hist1 == hist1b, "Histogram mismatch" )
  TEAGN_TRUE_OR_THROW( hist2 == hist2b, "Histogram mismatch" )
}

void jointHistogram_float_test()
{
    // Generating test rasters

    TePDITypes::TePDIRasterPtrType raster1;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster1, 1, 2, 2, 
      false, TeDOUBLE, 0 ), "Unable to create raster" );
    TEAGN_TRUE_OR_THROW( raster1->setElement( 0, 0, 1, 0 ), "error" )
    TEAGN_TRUE_OR_THROW( raster1->setElement( 0, 1, 2, 0 ), "error" )
    TEAGN_TRUE_OR_THROW( raster1->setElement( 1, 0, 1, 0 ), "error" );
    TEAGN_TRUE_OR_THROW( raster1->setElement( 1, 1, 2, 0 ), "error" );
    
    TePDITypes::TePDIRasterPtrType raster2;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster2, 1, 2, 2, 
      false, TeDOUBLE, 0 ), "Unable to create raster" );
    TEAGN_TRUE_OR_THROW( raster2->setElement( 0, 0, 1, 0 ), "error" );
    TEAGN_TRUE_OR_THROW( raster2->setElement( 0, 1, 3, 0 ), "error" );
    TEAGN_TRUE_OR_THROW( raster2->setElement( 1, 0, 1, 0 ), "error" );
    TEAGN_TRUE_OR_THROW( raster2->setElement( 1, 1, 3, 0 ), "error" );

    // Generating joint histogram

    TePDIJointHistogram hist;

    TEAGN_TRUE_OR_THROW( hist.update( *raster1, 0, *raster2, 0,
      TeBoxPixelIn, 3 ), "Unable to create histogram" );
      
    TEAGN_WATCH( hist.toString() );
      
    // Checking histogram
    {
      TEAGN_TRUE_OR_THROW( ( hist.size() == 2 ), "Invalid histogram size" );
    
      TEAGN_TRUE_OR_THROW( ( hist.getFreqSum() == 4 ), "Invalid histogram" );          
  
      TePDIJointHistogram::const_iterator it = hist.begin();
  
      TEAGN_CHECK_EQUAL( it->first.first, 1 , "" );
      TEAGN_CHECK_EQUAL( it->first.second, 1 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;
  
      TEAGN_CHECK_EQUAL( it->first.first, 2 , "" );
      TEAGN_CHECK_EQUAL( it->first.second, 3 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;
    }
    // Checking raster 1 histogram
    
    {
      const TePDIHistogram& r1hist = hist.getRaster1Hist();
      TEAGN_TRUE_OR_THROW( ( r1hist.size() == 2 ), "Invalid histogram size" );
  
      TePDIHistogram::const_iterator it = r1hist.begin();
  
      TEAGN_CHECK_EQUAL( it->first, 1 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;
  
      TEAGN_CHECK_EQUAL( it->first, 2 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;      
    }    
    
    // Checking raster 2 histogram
 
    {
      const TePDIHistogram& r2hist = hist.getRaster2Hist();
      TEAGN_TRUE_OR_THROW( ( r2hist.size() == 2 ), "Invalid histogram size" );
  
      TePDIHistogram::const_iterator it = r2hist.begin();
  
      TEAGN_CHECK_EQUAL( it->first, 1 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;
            
      TEAGN_CHECK_EQUAL( it->first, 3 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;      
    }     
}

void jointHistogram_integer_test()
{
    // Generating test rasters

    TePDITypes::TePDIRasterPtrType raster1;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster1, 1, 2, 2, 
      false, TeINTEGER, 0 ), "Unable to create raster" );
    TEAGN_TRUE_OR_THROW( raster1->setElement( 0, 0, 1, 0 ), "error" )
    TEAGN_TRUE_OR_THROW( raster1->setElement( 0, 1, 2, 0 ), "error" )
    TEAGN_TRUE_OR_THROW( raster1->setElement( 1, 0, 1, 0 ), "error" );
    TEAGN_TRUE_OR_THROW( raster1->setElement( 1, 1, 2, 0 ), "error" );
    
    TePDITypes::TePDIRasterPtrType raster2;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster2, 1, 2, 2, 
      false, TeINTEGER, 0 ), "Unable to create raster" );
    TEAGN_TRUE_OR_THROW( raster2->setElement( 0, 0, 1, 0 ), "error" );
    TEAGN_TRUE_OR_THROW( raster2->setElement( 0, 1, 3, 0 ), "error" );
    TEAGN_TRUE_OR_THROW( raster2->setElement( 1, 0, 1, 0 ), "error" );
    TEAGN_TRUE_OR_THROW( raster2->setElement( 1, 1, 3, 0 ), "error" );

    // Generating joint histogram

    TePDIJointHistogram hist;

    TEAGN_TRUE_OR_THROW( hist.update( *raster1, 0, *raster2, 0,
      TeBoxPixelIn, 0 ), "Unable to create histogram" );
      
    // Checking histogram
    {
      TEAGN_TRUE_OR_THROW( ( hist.size() == 2 ), "Invalid histogram size" );
    
      TEAGN_TRUE_OR_THROW( ( hist.getFreqSum() == 4 ), "Invalid histogram" );          
  
      TePDIJointHistogram::const_iterator it = hist.begin();
  
      TEAGN_CHECK_EQUAL( it->first.first, 1 , "" );
      TEAGN_CHECK_EQUAL( it->first.second, 1 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;
  
      TEAGN_CHECK_EQUAL( it->first.first, 2 , "" );
      TEAGN_CHECK_EQUAL( it->first.second, 3 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;
    }
    // Checking raster 1 histogram
    
    {
      const TePDIHistogram& r1hist = hist.getRaster1Hist();
      TEAGN_TRUE_OR_THROW( ( r1hist.size() == 2 ), "Invalid histogram size" );
  
      TePDIHistogram::const_iterator it = r1hist.begin();
  
      TEAGN_CHECK_EQUAL( it->first, 1 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;
  
      TEAGN_CHECK_EQUAL( it->first, 2 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;      
    }    
    
    // Checking raster 2 histogram
 
    {
      const TePDIHistogram& r2hist = hist.getRaster2Hist();
      TEAGN_TRUE_OR_THROW( ( r2hist.size() == 2 ), "Invalid histogram size" );
  
      TePDIHistogram::const_iterator it = r2hist.begin();
  
      TEAGN_CHECK_EQUAL( it->first, 1 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;
            
      TEAGN_CHECK_EQUAL( it->first, 3 , "" );
      TEAGN_CHECK_EQUAL( it->second, 2 , "" );
      ++it;      
    }     
}

int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{ 
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) ); 
    
    jointHistogram_8bits_images_test();
    jointHistogram_8bits_test();
    jointHistogram_float_test();
    jointHistogram_integer_test();
  }
  catch( const TeException& excpt ){
    TEAGN_LOGERR( excpt.message() )
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}

