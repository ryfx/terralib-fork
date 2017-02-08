/*

2011jun27

Given an input image,
(1) firstly this demo computes its Fast Fourier Transform,
(2) then its Inverse FFT,
(3) and finally the absolute difference between (1) and (2).

*/

#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>
#include <TePDIUtils.hpp>
#include <TePDIFFT.hpp>
#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TeAgnostic.h>

void TePDIFFT_test()
{
    /* reads input image (as in TePDIContrast_test.cpp) */
    TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
      std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
    TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );

    /* allocates rasters to represent mxn complex matrices for the FFT */
    int m = inRaster->params().nlines_;
    int n = inRaster->params().ncols_;
    TeRasterParams r_params;
    r_params.nBands( 2 ); // two channels to store real and imaginary parts of a complex number
    r_params.setDataType( TeDOUBLE );
    r_params.boundingBoxLinesColumns( 0, 0, m, n, m, n ); 
    TePDITypes::TePDIRasterPtrType inpraster, outraster;
    TePDIUtils::TeAllocRAMRaster( inpraster, r_params, TePDIUtils::TePDIUtilsRamMemPol );
    TePDIUtils::TeAllocRAMRaster( outraster, r_params, TePDIUtils::TePDIUtilsRamMemPol );

    /* input mxn complex matrix, channel 0: real, channel 1: imaginary */
    int i, j;
    for (i = 0; i < m; i++)
      for (j = 0; j < n; j++) {
          double level;
          inRaster->getElement( j, i, level, 0 );
          inpraster->setElement( j, i, level, 0 ); // real
          inpraster->setElement( j, i, 0, 1 );     // imaginary
      }

    /* (1) computes FFT on the input */
    TePDIParameters params;
    params.SetParameter( "input_raster" , inpraster );  
    params.SetParameter( "output_raster" , outraster );  
    TePDIFFT algo;
    TEAGN_TRUE_OR_THROW( algo.Reset(params), "Reset failed" );
    TEAGN_TRUE_OR_THROW( algo.Apply(), "Apply error" );

    /* writes the image representing the real part of the FFT of the input matrix */
    TePDITypes::TePDIRasterPtrType outRaster;
    TePDIUtils::TeAllocRAMRaster( outRaster, 1, 1, 1, false, TeUNSIGNEDCHAR, 0 );
    TeRasterParams new_outRaster_params = outRaster->params();
    new_outRaster_params.nBands( 1 );
    TeSharedPtr< TeProjection > proj( TeProjectionFactory::make( inRaster->projection()->params() ) );          
    new_outRaster_params.projection( proj.nakedPointer() );
    new_outRaster_params.boxResolution( inRaster->params().box().x1(), inRaster->params().box().y1(), inRaster->params().box().x2(), 
                                        inRaster->params().box().y2(), inRaster->params().resx_, inRaster->params().resy_ );            
    new_outRaster_params.useDummy_ = false;
    new_outRaster_params.setPhotometric( TeRasterParams::TeMultiBand );
    outRaster->init( new_outRaster_params );
    double min=9999.9f, max=-1;
    for (i = 0; i < m; i++)
      for (j = 0; j < n; j++) {
          double level;
          outraster->getElement( j, i, level, 0 );
          double leveli;
          outraster->getElement( j, i, leveli, 1 );
          double magnitude = sqrt(level*level + leveli*leveli);
          if (magnitude < min)
              min = magnitude;
          if (magnitude > max)
              max = magnitude;
      }
    //double min2=9999.9f, max2=-1;
    for (i = 0; i < m; i++)
      for (j = 0; j < n; j++) {
          double level;
          outraster->getElement( j, i, level, 0 );
          double leveli;
          outraster->getElement( j, i, leveli, 1 );
          double magnitude = sqrt(level*level + leveli*leveli);
          double newlevel = ((magnitude-min) / (max-min)) * 1000000.0; /* for visualization purposes */
          outRaster->setElement( j, i, (unsigned char)(newlevel), 0 );
      }
    TePDIUtils::TeRaster2Geotiff( outRaster, TEPDIEXAMPLESBINPATH "output1-fft.tif" );

    /* (2) tests the Inverse FFT */
    TePDITypes::TePDIRasterPtrType outraster2;
    TePDIUtils::TeAllocRAMRaster( outraster2, r_params, TePDIUtils::TePDIUtilsRamMemPol );
    params.SetParameter( "input_raster" , outraster );  
    params.SetParameter( "output_raster" , outraster2 );  
    params.SetParameter( "fft_ifft_flag" , -1 );  // flags: +1 FFT, -1 IFFT
    TEAGN_TRUE_OR_THROW( algo.Reset(params), "Reset failed" );
    TEAGN_TRUE_OR_THROW( algo.Apply(), "Apply error" );
    
    for (i = 0; i < m; i++)
      for (j = 0; j < n; j++) {
          double level;
          outraster2->getElement( j, i, level, 0 );
          outRaster->setElement( j, i, (unsigned char)(level), 0 );
      }
    TePDIUtils::TeRaster2Geotiff( outRaster, TEPDIEXAMPLESBINPATH "output2-ifft.tif" );

    /* (3) difference between original input image and the ifft result */
    for (i = 0; i < m; i++)
      for (j = 0; j < n; j++) {
          double level;
          outraster2->getElement( j, i, level, 0 );
          double level2;
          inRaster->getElement( j, i, level2, 0 );
          outRaster->setElement( j, i, (unsigned char)(fabs(level2-level)), 0 );
      }
    TePDIUtils::TeRaster2Geotiff( outRaster, TEPDIEXAMPLESBINPATH "output3-diff.tif" );
}

int main()
{
    TEAGN_LOGMSG( "Test started." );

    try{
      TeStdIOProgress pi;
      TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );     
      
      TePDIFFT_test();
    }
    catch( const TeException& e ){
      TEAGN_LOGERR( "Test Failed - " + e.message() );
      return EXIT_FAILURE;
    }

    TEAGN_LOGMSG( "Test OK." );
    
    return EXIT_SUCCESS;
}

