#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIStatistic.hpp>
#include <TePDIParameters.hpp>
#include <TeAgnostic.h>

#include <TeInitRasterDecoders.h>
#include <TeRaster.h>
#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TeGeometry.h>
#include <TeBox.h>

void numeric_test_with_polygonset()
{
  TePDITypes::TePDIRasterPtrType inRaster1( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster1->init(), "Unable to init inRaster1" );

  TePDITypes::TePDIRasterPtrType inRaster2( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster2->init(), "Unable to init inRaster2" );
  
  TePDITypes::TePDIRasterPtrType inRaster3( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers2b_rgb342_crop.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster3->init(), "Unable to init inRaster2" );  

  TePDIParameters pars;
  
  TePDITypes::TePDIRasterVectorType rasters;
  rasters.push_back( inRaster1 );
  rasters.push_back( inRaster2 );
  rasters.push_back( inRaster3 );
  
  pars.SetParameter( "rasters", rasters );
  
  std::vector< int > bands;
  bands.push_back( 0 );
  bands.push_back( 1 );
  bands.push_back( 2 );
  
  pars.SetParameter( "bands", bands );
  
  TeBox box = inRaster1->params().boundingBox();
  TePolygon pol = polygonFromBox( box );
  TePDITypes::TePDIPolygonSetPtrType polset( new TePolygonSet );
  polset->add( pol );
  pars.SetParameter( "polygonset", polset );

  TePDIStatistic stat;

  TEAGN_TRUE_OR_THROW( stat.Reset( pars ), "Reset error" );
  
  TEAGN_CHECK_EPS( stat.getMin( 0 ), 15, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getMin( 1 ), 61, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getMin( 2 ), 10, 0.0001, "" );

  TEAGN_CHECK_EPS( stat.getMax( 0 ), 255, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getMax( 1 ), 255, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getMax( 2 ), 255, 0.0001, "" );
  
  TEAGN_CHECK_EPS( stat.getMean( 0 ), 134.405948, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getMean( 1 ), 209.620141, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getMean( 2 ), 148.682201, 0.0001, "" );
  
  TEAGN_CHECK_EPS( stat.getStdDev( 0 ), 46.058313, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getStdDev( 1 ), 29.600182, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getStdDev( 2 ), 36.111183, 0.0001, "" );    

  TEAGN_CHECK_EPS( stat.getVariance( 0 ), 2121.368232, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getVariance( 1 ), 876.170773, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getVariance( 2 ), 1304.017573, 0.0001, "" );
  
  TEAGN_CHECK_EPS( stat.getCovariance( 0, 0, 0 ), 2121.368232, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getCovariance( 0, 1, 0 ), -135.178860, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getCovariance( 0, 2, 0 ), 1307.875240, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getCovariance( 1, 0, 0 ), -135.178860, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getCovariance( 1, 1, 0 ), 876.170773, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getCovariance( 1, 2, 0 ), 482.378791, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getCovariance( 2, 0, 0 ), 1307.875240, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getCovariance( 2, 1, 0 ), 482.378791, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getCovariance( 2, 2, 0 ), 1304.017573, 0.0001, "" );
      
  TEAGN_CHECK_EPS( stat.getCorrelation( 0, 0, 0 ), 1, 0.000001, "" );
  TEAGN_CHECK_EPS( stat.getCorrelation( 0, 1, 0 ), -0.099153, 0.000001, "" );
  TEAGN_CHECK_EPS( stat.getCorrelation( 0, 2, 0 ), 0.786351, 0.000001, "" );
  TEAGN_CHECK_EPS( stat.getCorrelation( 1, 0, 0 ), -0.099153, 0.000001, "" );
  TEAGN_CHECK_EPS( stat.getCorrelation( 1, 1, 0 ), 1, 0.000001, "" );
  TEAGN_CHECK_EPS( stat.getCorrelation( 1, 2, 0 ), 0.451286, 0.000001, "" );
  TEAGN_CHECK_EPS( stat.getCorrelation( 2, 0, 0 ), 0.786351, 0.000001, "" );
  TEAGN_CHECK_EPS( stat.getCorrelation( 2, 1, 0 ), 0.451286, 0.000001, "" );      
  TEAGN_CHECK_EPS( stat.getCorrelation( 2, 2, 0 ), 1, 0.000001, "" );            
  
  TEAGN_CHECK_EPS( stat.getSum( 0 ), 17408124.000000, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getSum( 1 ), 27149791.000000, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getSum( 2 ), 19257170.000000, 0.0001, "" );
  
  TEAGN_CHECK_EPS( stat.getSum3( 0 ), 428939435700.000000, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getSum3( 1 ), 1262662900207.000000, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getSum3( 2 ), 499433085194.000000, 0.0001, "" );

  TEAGN_CHECK_EPS( stat.getSum4( 0 ), 75997706109496.000000, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getSum4( 1 ), 278887661881795.000000, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getSum4( 2 ), 85455958573178.000000, 0.0001, "" );    
  
  TEAGN_CHECK_EPS( stat.getEntropy( 0 ), 5.110813, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getEntropy( 1 ), 5.304196, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getEntropy( 2 ), 3.823902, 0.0001, "" );    
  
  TEAGN_CHECK_EPS( stat.getMode( 0 ), 133.000000, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getMode( 1 ), 255.000000, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getMode( 2 ), 163.000000, 0.0001, "" );
  
  TEAGN_CHECK_EPS( stat.getPercentile( 0., 0, 0 ), 15.000000, 0, 
    "Invalid Percentile" );
  TEAGN_CHECK_EPS( stat.getPercentile( 50., 0, 0 ), 133.000000, 0, 
    "Invalid Percentile" );
  TEAGN_CHECK_EPS( stat.getPercentile( 75., 0, 0 ), 158.000000, 0, 
    "Invalid Percentile" );
  TEAGN_CHECK_EPS( stat.getPercentile( 100., 0, 0 ), 255, 0, 
    "Invalid Percentile" );
     
  TeMatrix VarMatrix = stat.getVarMatrix();

  TeMatrix StdDevMatrix = stat.getStdDevMatrix();
  
  TeMatrix CMMatrix = stat.getCMMatrix();
  TEAGN_CHECK_EPS( CMMatrix( 0, 0 ), 2121.368232, 0.0001, "" );
  TEAGN_CHECK_EPS( CMMatrix( 0, 1 ), 876.170773, 0.000001, "" );
  TEAGN_CHECK_EPS( CMMatrix( 1, 0 ), 28376.279826, 0.00001, "" );
  TEAGN_CHECK_EPS( CMMatrix( 1, 1 ), -12962.252229, 0.000001, "" );
  TEAGN_CHECK_EPS( CMMatrix( 2, 0 ), 15235700.947282, 0.00001, "" );
  TEAGN_CHECK_EPS( CMMatrix( 2, 1 ), 2351893.787613, 0.00001, "" );

  TeMatrix CovMatrix = stat.getCovMatrix();

  TeMatrix CorMatrix = stat.getCorMatrix();
    
  TeMatrix AssMatrix = stat.getAssimetryMatrix();
  TEAGN_CHECK_EPS( AssMatrix( 0, 0 ), 0.290423, 0.000001, "" );
  TEAGN_CHECK_EPS( AssMatrix( 0, 1 ), -0.499801, 0.000001, "" );

  TeMatrix KurMatrix = stat.getKurtosisMatrix();
  TEAGN_CHECK_EPS( KurMatrix( 0, 0 ), 0.385558, 0.000001, "" );
  TEAGN_CHECK_EPS( KurMatrix( 0, 1 ), 0.063657, 0.000001, "" );

  TeMatrix VarCoefMatrix = stat.getVarCoefMatrix();
  TEAGN_CHECK_EPS( VarCoefMatrix( 0, 0 ), 0.342681, 0.000001, "" );
  TEAGN_CHECK_EPS( VarCoefMatrix( 0, 1 ), 0.141209, 0.000001, "" );
}


void numeric_with_histograms_test()
{
  TePDITypes::TePDIRasterPtrType inRaster1( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster1->init(), "Unable to init inRaster1" );

  TePDITypes::TePDIRasterPtrType inRaster2( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_contraste.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster2->init(), "Unable to init inRaster2" );
  
  /* Building histograms */
  
  std::vector< TePDIHistogram::pointer > histograms;
  
  TePDIHistogram::pointer user_hist1_ptr( new TePDIHistogram );
  TEAGN_TRUE_OR_THROW( user_hist1_ptr->reset( inRaster1, 0, 0, 
    TeBoxPixelIn ),
    "Histogram generation error" );
  histograms.push_back( user_hist1_ptr );
    
  TePDIHistogram::pointer user_hist2_ptr( new TePDIHistogram );
  TEAGN_TRUE_OR_THROW( user_hist2_ptr->reset( inRaster2, 0, 0, 
    TeBoxPixelIn ),
    "Histogram generation error" );
  histograms.push_back( user_hist2_ptr );
  
  /* Building statistic parameters */

  TePDIParameters pars;
  
  pars.SetParameter( "histograms", histograms );
  
  TePDITypes::TePDIRasterVectorType rasters;
  rasters.push_back( inRaster1 );
  rasters.push_back( inRaster2 );
  
  pars.SetParameter( "rasters", rasters );
  
  std::vector< int > bands;
  bands.push_back( 0 );
  bands.push_back( 0 );
  
  pars.SetParameter( "bands", bands );
  
  TePDIStatistic stat;

  TEAGN_TRUE_OR_THROW( stat.Reset( pars ), "Reset error" );
  
  TePDIHistogram hist1 = stat.getHistogram( 0 );
  TEAGN_TRUE_OR_THROW( ( hist1.size() == user_hist1_ptr->size() ), 
    "Invalid histogram1 size" );

  TePDIHistogram hist2 = stat.getHistogram( 1 );
  TEAGN_TRUE_OR_THROW( ( hist2.size() == user_hist2_ptr->size() ), 
    "Invalid histogram2 size" );
      
  TEAGN_CHECK_EPS( stat.getSum( 0 ), 55438975.000000, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getSum( 1 ), 87782597.000000, 0.0001, "" );

  TEAGN_CHECK_EPS( stat.getSum3( 0 ), 340982998465.000000, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getSum3( 1 ), 1689469326995.000000, 0.0001, "" );

  TEAGN_CHECK_EPS( stat.getSum4( 0 ), 27381030612693.000000, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getSum4( 1 ), 252981440505241.000000, 0.0001, "" );  
    
  TEAGN_CHECK_EPS( stat.getMean( 0 ), 76.928379, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getMean( 1 ), 121.809123, 0.0001, "" );

  TEAGN_CHECK_EPS( stat.getVariance( 0 ), 75.432528, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getVariance( 1 ), 1561.214764, 0.0001, "" );
  
  TEAGN_CHECK_EPS( stat.getStdDev( 0 ), 8.685190, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getStdDev( 1 ), 39.512210, 0.0001, "" );
  
  TEAGN_CHECK_EPS( stat.getEntropy( 0 ), 5.004244, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getEntropy( 1 ), 4.998273, 0.0001, "" );  
  
  TEAGN_CHECK_EPS( stat.getMin( 0 ), 47, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getMin( 1 ), 0, 0.0001, "" );

  TEAGN_CHECK_EPS( stat.getMax( 0 ), 255, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getMax( 1 ), 255, 0.0001, "" );

  TEAGN_CHECK_EPS( stat.getMode( 0 ), 81, 0.0001, "" );
  TEAGN_CHECK_EPS( stat.getMode( 1 ), 141, 0.0001, "" );
  
  TEAGN_CHECK_EPS( stat.getPercentile( 0., 0, 0 ), 47, 0, 
    "Invalid Percentile" );
  TEAGN_CHECK_EPS( stat.getPercentile( 50., 0, 0 ), 78, 0, 
    "Invalid Percentile" );
  TEAGN_CHECK_EPS( stat.getPercentile( 51., 0, 0 ), 78, 0, 
    "Invalid Percentile" );
  TEAGN_CHECK_EPS( stat.getPercentile( 100., 0, 0 ), 255, 0, 
    "Invalid Percentile" );  
          
  TEAGN_CHECK_EPS( stat.getCovariance( 0, 0 ),
    75.432528, 0.001, "" );
    
  TEAGN_CHECK_EPS( stat.getCovariance( 0, 1 ),
    335.166, 0.001, "" );

  TEAGN_CHECK_EPS( stat.getCovariance( 1, 0 ),
    335.166, 0.001, "" );
        
  TEAGN_CHECK_EPS( stat.getCovariance( 1, 1 ),
    1561.214764, 0.001, "" );
    
  TEAGN_CHECK_EPS( stat.getCorrelation( 0, 0 ),
     1.0, 0.000001, "" );

  TEAGN_CHECK_EPS( stat.getCorrelation( 0, 1 ),
     0.976674, 0.000001, "" );

  TEAGN_CHECK_EPS( stat.getCorrelation( 1, 0 ),
     0.976674, 0.000001, "" );
     
  TeMatrix VarMatrix = stat.getVarMatrix();
  TEAGN_CHECK_EPS( VarMatrix( 0, 0 ), 75.432528, 0.000001, "" );
  TEAGN_CHECK_EPS( VarMatrix( 1, 0 ), 1561.214764, 0.000001, "" );  

  TeMatrix StdDevMatrix = stat.getStdDevMatrix();
  TEAGN_CHECK_EPS( StdDevMatrix( 0, 0 ), 8.685190, 0.000001, "" );
  TEAGN_CHECK_EPS( StdDevMatrix( 1, 0 ), 39.512210, 0.000001, "" );  
  
  TeMatrix CMMatrix = stat.getCMMatrix();
  TEAGN_CHECK_EPS( CMMatrix( 0, 0 ), 75.432528, 0.0001, "" );
  TEAGN_CHECK_EPS( CMMatrix( 0, 1 ), 1561.214764, 0.000001, "" );
  TEAGN_CHECK_EPS( CMMatrix( 1, 0 ), 486.770730, 0.00001, "" );
  TEAGN_CHECK_EPS( CMMatrix( 1, 1 ), -33502.920720, 0.000001, "" );
  TEAGN_CHECK_EPS( CMMatrix( 2, 0 ), 143871.693264, 0.00001, "" );
  TEAGN_CHECK_EPS( CMMatrix( 2, 1 ), 8229548.372509, 0.000001, "" );

  TeMatrix CovMatrix = stat.getCovMatrix();
  TEAGN_CHECK_EPS( CovMatrix( 0, 0 ), 75.432528, 0.000001, "" );  
  TEAGN_CHECK_EPS( CovMatrix( 0, 1 ), 335.166257, 0.000001, "" );  
  TEAGN_CHECK_EPS( CovMatrix( 1, 0 ), 335.166257, 0.000001, "" );  
  TEAGN_CHECK_EPS( CovMatrix( 1, 1 ), 1561.214764, 0.000001, "" );  

  TeMatrix CorMatrix = stat.getCorMatrix();
  TEAGN_CHECK_EPS( CorMatrix( 0, 0 ), 1.0, 0.000001, "" );  
  TEAGN_CHECK_EPS( CorMatrix( 0, 1 ), 0.976674, 0.000001, "" );  
  TEAGN_CHECK_EPS( CorMatrix( 1, 0 ), 0.976674, 0.000001, "" );  
  TEAGN_CHECK_EPS( CorMatrix( 1, 1 ), 1.0, 0.000001, "" );  
    
  TeMatrix AssMatrix = stat.getAssimetryMatrix();
  TEAGN_CHECK_EPS( AssMatrix( 0, 0 ), 0.742996, 0.000001, "" );
  TEAGN_CHECK_EPS( AssMatrix( 0, 1 ), -0.543111, 0.000001, "" );

  TeMatrix KurMatrix = stat.getKurtosisMatrix();
  TEAGN_CHECK_EPS( KurMatrix( 0, 0 ), 22.284713, 0.000001, "" );
  TEAGN_CHECK_EPS( KurMatrix( 0, 1 ), 0.376375, 0.000001, "" );

  TeMatrix VarCoefMatrix = stat.getVarCoefMatrix();
  TEAGN_CHECK_EPS( VarCoefMatrix( 0, 0 ), 0.112900, 0.000001, "" );
  TEAGN_CHECK_EPS( VarCoefMatrix( 0, 1 ), 0.324378, 0.000001, "" );
}


int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );
      
    TeInitRasterDecoders();

    numeric_test_with_polygonset();
    numeric_with_histograms_test();
  }
  catch( const TeException& e ){
    TEAGN_LOGERR( "Test Failed - " + e.message() );
    return EXIT_FAILURE;
  }

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
