/*

2011jul20

Harmonic Analysis of NDVI Time-Series (HANTS).

*/

#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>
#include <TePDIUtils.hpp>
#include "TePDIHants.hpp"
#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TeAgnostic.h>

#include <TeInitRasterDecoders.h>

//#include <time>

namespace PDI 
{
	double rint(double x) {
		return floor(x + 0.5);
	}
}

void TePDIHants_test()
{
    // input files: data (original NDVI values), smoothed NDVI values (by IDL), mask ('mascara terra-agua')
    char const * const fndata = TEPDIEXAMPLESRESPATH "/data.tif";
    char const * const fnfilt = TEPDIEXAMPLESRESPATH "/groundTruth.tif";
    char const * const fnmask = TEPDIEXAMPLESRESPATH "/mask.tif";
    // input data (NDVI series)
    TePDITypes::TePDIRasterPtrType dataRaster( new TeRaster(std::string( fndata ), 'r' ) );
    TEAGN_TRUE_OR_THROW( dataRaster->init(), "Unable to init dataRaster" );
    int nl = dataRaster->params().nlines_;
    int nc = dataRaster->params().ncols_;
    int nb = dataRaster->params().nBands();
    printf("nl=%d  nc=%d  nb=%d\n",nl, nc, nb);
    // filtered/smoothed data used as ground-truth
    TePDITypes::TePDIRasterPtrType filtRaster( new TeRaster(std::string( fnfilt ), 'r' ) );
    TEAGN_TRUE_OR_THROW( filtRaster->init(), "Unable to init dataRaster" );
    // mask: 0 = do not perform; 1 = perform algorithm
    TePDITypes::TePDIRasterPtrType maskRaster( new TeRaster(std::string( fnmask ), 'r' ) );
    TEAGN_TRUE_OR_THROW( maskRaster->init(), "Unable to init dataRaster" );

    // output raster for smoothed NDVI values
    TeRasterParams r_params;
    r_params.nBands( nb );
    r_params.setDataType( TeSHORT );
    r_params.boundingBoxLinesColumns( 0, 0, nl, nc, nl, nc ); 
    TePDITypes::TePDIRasterPtrType outRaster;
    TePDIUtils::TeAllocRAMRaster( outRaster, r_params, TePDIUtils::TePDIUtilsRamMemPol );

    // Hants parameters:
    
    // 1D array with frequences that should be selected from the fourier spectrum. i.e. freqs=[0,1,2,3] to use the fourier compoments 
    // 0 (mean), 1 (1 sine wave), 2 (2 sine waves) and 3
	int tmpvi[] = {0,1,2,3};
    int *freqs = tmpvi;
    // number of selected freqs
    int nfreqs = 4;
    
    // Array of size 2 to specify the minimum and maximum valid data values, i.e. range=[1,254]
    double tmpvd[] = { 1000.0,10000.0 };
    double *range = tmpvd;
    
    // Fit Error Tolerance, is maximum tolerable downward deviation between fourier fit and NDVI data values (in DN values)
    double FET = 1500;
    
    // Throw-Away-Threshold, maximum number of points that may be thrown away by the FFT fitting
    int TAT=1;

    // maximum number of iterations to be performed
    int iMAX=15;

    int t1 = clock();	
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TePDIParameters params;
    params.SetParameter( "input_raster" , dataRaster );  
    params.SetParameter( "output_raster" , outRaster );  
    params.SetParameter( "mask_raster" , maskRaster );  
    params.SetParameter( "freqs" , freqs );  
    params.SetParameter( "nfreqs" , nfreqs );  
    params.SetParameter( "range" , range );  
    params.SetParameter( "FET" , FET );  
    params.SetParameter( "TAT" , TAT );  
    params.SetParameter( "iMAX" , iMAX );  
    TePDIHants algo;
    TEAGN_TRUE_OR_THROW( algo.Reset(params), "Reset failed" );
    TEAGN_TRUE_OR_THROW( algo.Apply(), "Apply error" );

    printf("Checking with ground-truth (NDVI values between 0 and 10000):\n");
	int n = nb;
    double *tmpvdouble = new double[n];
    double *tmpvdouble2 = new double[n];
    double *gt = tmpvdouble;
    double *smNDVI = tmpvdouble2;
    int toterro = 0;
    int maxdiff = 0;
    double current_level;
    for (int i=0; i < nl; i++)
        for (int j=0; j < nc; j++) {
            maskRaster->getElement( j, i, current_level, 0 );
            int perform = (int)current_level;
            if (perform) {
                for (int k=0; k < nb; k++) {
                    outRaster->getElement( j, i, current_level, k );
                    smNDVI[k] = current_level;
                    filtRaster->getElement( j, i, current_level, k );
                    gt[k] = current_level;
                }
                for (int k=0; k < nb; k++) {
                    int calc = (int)(PDI::rint(smNDVI[k]));
                    int gab = (int)(PDI::rint(gt[k]));
                    int diff = (int)fabs((float)(calc-gab));
                    if (diff > maxdiff)
                        maxdiff = diff;
                    // prints any difference above 1 unit (assuming that NDVI values between 0 and 10000)
                    if (diff > 1) {
                        toterro++;
                        printf("Erro: smNDVI[%d][%d][%d] = %d != %d = vfilt[][][]  (diff=%d)\n",i,j,k,calc,gab,diff);
                    }
                }
            }
            if ((i*nc+j) % (int)((nl*nc)/10) == 0)
                printf("%d%%\n",(((i*nc+j)/((int)((nl*nc)/10)))+1)*10);
        }
    printf("Total ( diff > 1 ): %d\n", toterro);
    printf("maxDiff = %d\n", maxdiff);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int t2 = clock();
    double tt = ((double)t2-t1);
    printf("%.6lf seconds elapsed\n", tt/CLOCKS_PER_SEC);
/*
    // exception on malloc ???
    char *fout = TEPDIEXAMPLESBINPATH "/output-smNDVI.tif";
    TePDIUtils::TeRaster2Geotiff( outRaster, fout );
    printf("output: %s\n", fout);
    */

    delete [] tmpvdouble;
    delete [] tmpvdouble2;

}

int main()
{
    TEAGN_LOGMSG( "Test started." );

    try{
      TeStdIOProgress pi;
      TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );     
      
      TePDIHants_test();
    }
    catch( const TeException& e ){
      TEAGN_LOGERR( "Test Failed - " + e.message() );
      return EXIT_FAILURE;
    }

    TEAGN_LOGMSG( "Test OK." );
    
//    getchar();
    
    return EXIT_SUCCESS;
}

