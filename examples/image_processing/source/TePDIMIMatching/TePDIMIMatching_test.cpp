#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIMIMatching.hpp>
#include <TePDIUtils.hpp>

#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TeAgnostic.h>

#include <math.h>


void raster2Tiff( 
  const TePDITypes::TePDIRasterPtrType& input_raster_ptr,
  unsigned int raster_channel,
  const std::string& out_file_name,
  TeSharedPtr< TeCoordPairVect > out_tie_points_ptr,
  unsigned int tie_points_space )
{   
  TEAGN_TRUE_OR_THROW( ( ! out_file_name.empty() ), 
    "Invalid file name" )
  TEAGN_TRUE_OR_THROW( ( input_raster_ptr->params().nlines_ > 0 ), 
    "Invalid matrix lines" )
  TEAGN_TRUE_OR_THROW( ( input_raster_ptr->params().ncols_ > 0 ), 
    "Invalid matrix cols" )
    
  TeRasterParams params;
  params.setNLinesNColumns( input_raster_ptr->params().nlines_,
    input_raster_ptr->params().ncols_ );
  params.nBands( 1 );
  params.setDataType( TeUNSIGNEDCHAR, -1 );
  params.nBands( 1 );
  params.decoderIdentifier_ = "TIF";
  params.mode_ = 'c';
  params.fileName_ = out_file_name;
  
  TeRaster out_raster( params );
  TEAGN_TRUE_OR_THROW( out_raster.init(), "Error init raster" );
  double value = 0;
  
  for( int line = 0 ; 
    line < input_raster_ptr->params().nlines_ ; ++line ) {
    for( int col = 0 ; 
      col < input_raster_ptr->params().ncols_ ; 
      ++col ) {
      
      input_raster_ptr->getElement( col, line, value, 
        raster_channel );
      

      TEAGN_TRUE_OR_THROW( out_raster.setElement( col, line, 
        value, 0 ),
        "Error writing raster" )
    }  
  }
  
  /* Draw tie-points */
  
  if( out_tie_points_ptr.isActive() ) {
    TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
    TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
    
    while( it != it_end ) {
      int x = 0;
      int y = 0;
      
      if( tie_points_space == 0 ) {
         x = TeRound( it->pt1.x() );
         y = TeRound( it->pt1.y() );
      } else {
         x = TeRound( it->pt2.x() );
         y = TeRound( it->pt2.y() );
      }
      
      TEAGN_TRUE_OR_THROW( ( x < input_raster_ptr->params().ncols_ ),
        "Invalid maxima column" )
      TEAGN_TRUE_OR_THROW( ( x >= 0 ),
        "Invalid maxima column" )      
      TEAGN_TRUE_OR_THROW( ( y < input_raster_ptr->params().nlines_ ),
        "Invalid maxima line" )
      TEAGN_TRUE_OR_THROW( ( y >= 0 ),
        "Invalid maxima line" )      
      
      TEAGN_TRUE_OR_THROW( out_raster.setElement( x, y, 
        255.0, 0 ),
        "Error writing raster" )    
    
      ++it;
    }
  
  }
}

void sameImageTest()
{
  /* Creating parameters */

  TePDIParameters params;
  
  TePDITypes::TePDIRasterPtrType input_image1_ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_image1_ptr->init(), 
    "Unable to init input_image1_ptr" );    
  params.SetParameter( "input_image1_ptr" , input_image1_ptr );
  
  params.SetParameter( "input_channel1" , (unsigned int)0 );
    
  TePDITypes::TePDIRasterPtrType input_image2_ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_image2_ptr->init(), 
    "Unable to init input_image2_ptr" ); 
  params.SetParameter( "input_image2_ptr" , input_image2_ptr ); 
  
  params.SetParameter( "input_channel2" , (unsigned int)0 );       
    
  TeSharedPtr< TeCoordPairVect > out_tie_points_ptr( new TeCoordPairVect );
  params.SetParameter( "out_tie_points_ptr" , out_tie_points_ptr );        
  
  TeBox input_box1( TeCoord2D( 142, 826 ) , 
    TeCoord2D( 542, 426 ) );
  params.SetParameter( "input_box1" , input_box1 );
  
  TeBox input_box2( TeCoord2D( 238, 689 ) , 
    TeCoord2D( 391, 473 ) );
  params.SetParameter( "input_box2" , input_box2 );  
  
  params.SetParameter( "enable_multi_thread" , (int)1 ); 
  
  params.SetParameter( "pixel_x_relation" , (double)1 ); 
  params.SetParameter( "pixel_y_relation" , (double)1 ); 
  
  TePDIMIMatching match_instance; 
  match_instance.ToggleProgInt( true );
  TEAGN_TRUE_OR_THROW( match_instance.Reset( params ),
    "Algorithm reset error" )
    
  TEAGN_LOGMSG( "Algorithm started" )
  
  time_t init_time = clock() / CLOCKS_PER_SEC;
    
  TEAGN_TRUE_OR_THROW( match_instance.Apply(),
    "Algorithm apply error" )      
    
  time_t end_time = clock() / CLOCKS_PER_SEC;
  
  TEAGN_LOGMSG( "Time elapsed (sameImageDifBoxesTest): " + 
    TeAgnostic::to_string( (long int)( end_time - init_time ) ) +
    " seconds" );
    
  /* Displaying tie-points */
  {
    TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
    TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
    
    while( it != it_end ) {
      std::cout << "[" + Te2String( it->pt1.x(),1 ) + " , " +
        Te2String( it->pt1.y(),1 ) + "] -> [" +
        Te2String( it->pt2.x(),1 ) + " , " + 
        Te2String( it->pt2.y(),1 ) + "]" << std::endl;
      
      ++it;
    }
  }
  
  /* Tie-points draw */
  
  raster2Tiff( input_image1_ptr, 0, TEPDIEXAMPLESBINPATH
    "TeMIMatching_test_sameImageTest_input_image1.tif",
    out_tie_points_ptr, 0 );
  raster2Tiff( input_image2_ptr, 0, TEPDIEXAMPLESBINPATH
    "TeMIMatching_test_sameImageTest_input_image2.tif",
    out_tie_points_ptr, 1 );    
    
  TEAGN_WATCH( (unsigned int)out_tie_points_ptr->size() );
  TEAGN_CHECK_EPS( 4, out_tie_points_ptr->size(),
    0, "Invalid tie-points number" )    
}

/* inverted boxes test */
void sameImageTest2()
{
  /* Creating parameters */

  TePDIParameters params;
  
  TePDITypes::TePDIRasterPtrType input_image1_ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_image1_ptr->init(), 
    "Unable to init input_image1_ptr" );    
  params.SetParameter( "input_image1_ptr" , input_image1_ptr );
  
  params.SetParameter( "input_channel1" , (unsigned int)0 );
    
  TePDITypes::TePDIRasterPtrType input_image2_ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_image2_ptr->init(), 
    "Unable to init input_image2_ptr" ); 
  params.SetParameter( "input_image2_ptr" , input_image2_ptr ); 
  
  params.SetParameter( "input_channel2" , (unsigned int)0 );       
    
  TeSharedPtr< TeCoordPairVect > out_tie_points_ptr( new TeCoordPairVect );
  params.SetParameter( "out_tie_points_ptr" , out_tie_points_ptr );        
  
  TeBox input_box2( TeCoord2D( 142, 826 ) , 
    TeCoord2D( 542, 426 ) );
  params.SetParameter( "input_box2" , input_box2 );
  
  TeBox input_box1( TeCoord2D( 238, 689 ) , 
    TeCoord2D( 391, 473 ) );
  params.SetParameter( "input_box1" , input_box1 );  
  
  params.SetParameter( "enable_multi_thread" , (int)1 ); 
  
  params.SetParameter( "pixel_x_relation" , (double)1 ); 
  params.SetParameter( "pixel_y_relation" , (double)1 ); 
  
  TePDIMIMatching match_instance; 
  match_instance.ToggleProgInt( true );
  TEAGN_TRUE_OR_THROW( match_instance.Reset( params ),
    "Algorithm reset error" )
    
  TEAGN_LOGMSG( "Algorithm started" )
  
  time_t init_time = clock() / CLOCKS_PER_SEC;
    
  TEAGN_TRUE_OR_THROW( match_instance.Apply(),
    "Algorithm apply error" )      
    
  time_t end_time = clock() / CLOCKS_PER_SEC;
  
  TEAGN_LOGMSG( "Time elapsed (sameImageDifBoxesTest): " + 
    TeAgnostic::to_string( (long int)( end_time - init_time ) ) +
    " seconds" );
    
  /* Displaying tie-points */
  {
    TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
    TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
    
    while( it != it_end ) {
      std::cout << "[" + Te2String( it->pt1.x(),1 ) + " , " +
        Te2String( it->pt1.y(),1 ) + "] -> [" +
        Te2String( it->pt2.x(),1 ) + " , " + 
        Te2String( it->pt2.y(),1 ) + "]" << std::endl;
      
      ++it;
    }
  }
  
  /* Tie-points draw */
  
  raster2Tiff( input_image1_ptr, 0, TEPDIEXAMPLESBINPATH
    "TeMIMatching_test_sameImageTest2_input_image1.tif",
    out_tie_points_ptr, 0 );
  raster2Tiff( input_image2_ptr, 0, TEPDIEXAMPLESBINPATH
    "TeMIMatching_test_sameImageTest2_input_image2.tif",
    out_tie_points_ptr, 1 );    
    
  TEAGN_WATCH( (unsigned int)out_tie_points_ptr->size() );
  TEAGN_CHECK_EPS( 4, out_tie_points_ptr->size(),
    0, "Invalid tie-points number" )    
}


void halfsampledImageTest()
{
  /* Creating parameters */

  TePDIParameters params;
  
  TePDITypes::TePDIRasterPtrType input_image1_ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_image1_ptr->init(), 
    "Unable to init input_image1_ptr" );    
  params.SetParameter( "input_image1_ptr" , input_image1_ptr );
  
  params.SetParameter( "input_channel1" , (unsigned int)0 );
    
  TePDITypes::TePDIRasterPtrType input_image2_ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_contraste_halfsampled.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_image2_ptr->init(), 
    "Unable to init input_image2_ptr" ); 
  params.SetParameter( "input_image2_ptr" , input_image2_ptr ); 
  
  params.SetParameter( "input_channel2" , (unsigned int)0 );       
    
  TeSharedPtr< TeCoordPairVect > out_tie_points_ptr( new TeCoordPairVect );
  params.SetParameter( "out_tie_points_ptr" , out_tie_points_ptr );        
  
  TeBox input_box1( TeCoord2D( 198, 486 ) , 
    TeCoord2D( 577, 186 ) );
  params.SetParameter( "input_box1" , input_box1 );
  
  TeBox input_box2( TeCoord2D( 143, 180 ) , 
    TeCoord2D( 227, 113 ) );
  params.SetParameter( "input_box2" , input_box2 );  
  
  params.SetParameter( "enable_multi_thread" , (int)1 ); 
  
  params.SetParameter( "pixel_x_relation" , (double)0.5 ); 
  params.SetParameter( "pixel_y_relation" , (double)0.5 ); 
  
  TePDIMIMatching match_instance; 
  match_instance.ToggleProgInt( true );
  TEAGN_TRUE_OR_THROW( match_instance.Reset( params ),
    "Algorithm reset error" )
    
  TEAGN_LOGMSG( "Algorithm started" )
  
  time_t init_time = clock() / CLOCKS_PER_SEC;
    
  TEAGN_TRUE_OR_THROW( match_instance.Apply(),
    "Algorithm apply error" )      
    
  time_t end_time = clock() / CLOCKS_PER_SEC;
  
  TEAGN_LOGMSG( "Time elapsed (sameImageDifBoxesTest): " + 
    TeAgnostic::to_string( (long int)( end_time - init_time ) ) +
    " seconds" );
    
  /* Displaying tie-points */
  {
    TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
    TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
    
    while( it != it_end ) {
      std::cout << "[" + Te2String( it->pt1.x(),1 ) + " , " +
        Te2String( it->pt1.y(),1 ) + "] -> [" +
        Te2String( it->pt2.x(),1 ) + " , " + 
        Te2String( it->pt2.y(),1 ) + "]" << std::endl;
      
      ++it;
    }
  }
  
  /* Tie-points draw */
  
  raster2Tiff( input_image1_ptr, 0, TEPDIEXAMPLESBINPATH
    "TeMIMatching_test_halfsampledImageTest_input_image1.tif",
    out_tie_points_ptr, 0 );
  raster2Tiff( input_image2_ptr, 0, TEPDIEXAMPLESBINPATH
    "TeMIMatching_test_halfsampledImageTest_input_image2.tif",
    out_tie_points_ptr, 1 );    
    
  TEAGN_WATCH( (unsigned int)out_tie_points_ptr->size() );
  TEAGN_CHECK_EPS( 4, out_tie_points_ptr->size(),
    0, "Invalid tie-points number" )    
}


void halfsampledImageTest2()
{
  /* Creating parameters */

  TePDIParameters params;
  
  TePDITypes::TePDIRasterPtrType input_image1_ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_contraste_halfsampled.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_image1_ptr->init(), 
    "Unable to init input_image1_ptr" );    
  params.SetParameter( "input_image1_ptr" , input_image1_ptr );
  
  params.SetParameter( "input_channel1" , (unsigned int)0 );
    
  TePDITypes::TePDIRasterPtrType input_image2_ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_image2_ptr->init(), 
    "Unable to init input_image2_ptr" ); 
  params.SetParameter( "input_image2_ptr" , input_image2_ptr ); 
  
  params.SetParameter( "input_channel2" , (unsigned int)0 );       
    
  TeSharedPtr< TeCoordPairVect > out_tie_points_ptr( new TeCoordPairVect );
  params.SetParameter( "out_tie_points_ptr" , out_tie_points_ptr );        
  
  TeBox input_box2( TeCoord2D( 198, 486 ) , 
    TeCoord2D( 577, 186 ) );
  params.SetParameter( "input_box2" , input_box2 );
  
  TeBox input_box1( TeCoord2D( 143, 180 ) , 
    TeCoord2D( 227, 113 ) );
  params.SetParameter( "input_box1" , input_box1 );  
  
  params.SetParameter( "enable_multi_thread" , (int)1 ); 
  
  params.SetParameter( "pixel_x_relation" , (double)2 ); 
  params.SetParameter( "pixel_y_relation" , (double)2 ); 
  
  TePDIMIMatching match_instance; 
  match_instance.ToggleProgInt( true );
  TEAGN_TRUE_OR_THROW( match_instance.Reset( params ),
    "Algorithm reset error" )
    
  TEAGN_LOGMSG( "Algorithm started" )
  
  time_t init_time = clock() / CLOCKS_PER_SEC;
    
  TEAGN_TRUE_OR_THROW( match_instance.Apply(),
    "Algorithm apply error" )      
    
  time_t end_time = clock() / CLOCKS_PER_SEC;
  
  TEAGN_LOGMSG( "Time elapsed (sameImageDifBoxesTest): " + 
    TeAgnostic::to_string( (long int)( end_time - init_time ) ) +
    " seconds" );
    
  /* Displaying tie-points */
  {
    TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
    TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
    
    while( it != it_end ) {
      std::cout << "[" + Te2String( it->pt1.x(),1 ) + " , " +
        Te2String( it->pt1.y(),1 ) + "] -> [" +
        Te2String( it->pt2.x(),1 ) + " , " + 
        Te2String( it->pt2.y(),1 ) + "]" << std::endl;
      
      ++it;
    }
  }
  
  /* Tie-points draw */
  
  raster2Tiff( input_image1_ptr, 0, TEPDIEXAMPLESBINPATH
    "TeMIMatching_test_halfsampledImageTest2_input_image1.tif",
    out_tie_points_ptr, 0 );
  raster2Tiff( input_image2_ptr, 0, TEPDIEXAMPLESBINPATH
    "TeMIMatching_test_halfsampledImageTest2_input_image2.tif",
    out_tie_points_ptr, 1 );    
    
  TEAGN_WATCH( (unsigned int)out_tie_points_ptr->size() );
  TEAGN_CHECK_EPS( 4, out_tie_points_ptr->size(),
    0, "Invalid tie-points number" )    
}

void sameImageGreenXRedTest()
{
  /* Creating parameters */

  TePDIParameters params;
  
  TePDITypes::TePDIRasterPtrType input_image1_ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_image1_ptr->init(), 
    "Unable to init input_image1_ptr" );    
  params.SetParameter( "input_image1_ptr" , input_image1_ptr );
  
  params.SetParameter( "input_channel1" , (unsigned int)0 );
    
  TePDITypes::TePDIRasterPtrType input_image2_ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_image2_ptr->init(), 
    "Unable to init input_image2_ptr" ); 
  params.SetParameter( "input_image2_ptr" , input_image2_ptr ); 
  
  params.SetParameter( "input_channel2" , (unsigned int)1 );       
    
  TeSharedPtr< TeCoordPairVect > out_tie_points_ptr( new TeCoordPairVect );
  params.SetParameter( "out_tie_points_ptr" , out_tie_points_ptr );        
  
  TeBox input_box1( TeCoord2D( 142, 826 ) , 
    TeCoord2D( 542, 426 ) );
  params.SetParameter( "input_box1" , input_box1 );
  
  TeBox input_box2( TeCoord2D( 238, 689 ) , 
    TeCoord2D( 391, 473 ) );
  params.SetParameter( "input_box2" , input_box2 );  
  
  params.SetParameter( "enable_multi_thread" , (int)1 ); 
  
  params.SetParameter( "pixel_x_relation" , (double)1 ); 
  params.SetParameter( "pixel_y_relation" , (double)1 ); 
  
  TePDIMIMatching match_instance; 
  match_instance.ToggleProgInt( true );
  TEAGN_TRUE_OR_THROW( match_instance.Reset( params ),
    "Algorithm reset error" )
    
  TEAGN_LOGMSG( "Algorithm started" )
  
  time_t init_time = clock() / CLOCKS_PER_SEC;
    
  TEAGN_TRUE_OR_THROW( match_instance.Apply(),
    "Algorithm apply error" )      
    
  time_t end_time = clock() / CLOCKS_PER_SEC;
  
  TEAGN_LOGMSG( "Time elapsed (sameImageDifBoxesTest): " + 
    TeAgnostic::to_string( (long int)( end_time - init_time ) ) +
    " seconds" );
    
  /* Displaying tie-points */
  {
    TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
    TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
    
    while( it != it_end ) {
      std::cout << "[" + Te2String( it->pt1.x(),1 ) + " , " +
        Te2String( it->pt1.y(),1 ) + "] -> [" +
        Te2String( it->pt2.x(),1 ) + " , " + 
        Te2String( it->pt2.y(),1 ) + "]" << std::endl;
      
      ++it;
    }
  }
  
  /* Tie-points draw */
  
  raster2Tiff( input_image1_ptr, 0, TEPDIEXAMPLESBINPATH
    "TeMIMatching_test_sameImageGreenXRedTest_input_image1.tif",
    out_tie_points_ptr, 0 );
  raster2Tiff( input_image2_ptr, 1, TEPDIEXAMPLESBINPATH
    "TeMIMatching_test_sameImageGreenXRedTest_input_image2.tif",
    out_tie_points_ptr, 1 );    
    
  TEAGN_WATCH( (unsigned int)out_tie_points_ptr->size() );
  TEAGN_CHECK_EPS( 4, out_tie_points_ptr->size(),
    0, "Invalid tie-points number" )    
}

void sameImageGreenXBlueTest()
{
  /* Creating parameters */

  TePDIParameters params;
  
  TePDITypes::TePDIRasterPtrType input_image1_ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_image1_ptr->init(), 
    "Unable to init input_image1_ptr" );    
  params.SetParameter( "input_image1_ptr" , input_image1_ptr );
  
  params.SetParameter( "input_channel1" , (unsigned int)1 );
    
  TePDITypes::TePDIRasterPtrType input_image2_ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_image2_ptr->init(), 
    "Unable to init input_image2_ptr" ); 
  params.SetParameter( "input_image2_ptr" , input_image2_ptr ); 
  
  params.SetParameter( "input_channel2" , (unsigned int)2 );       
    
  TeSharedPtr< TeCoordPairVect > out_tie_points_ptr( new TeCoordPairVect );
  params.SetParameter( "out_tie_points_ptr" , out_tie_points_ptr );        
  
  TeBox input_box1( TeCoord2D( 142, 826 ) , 
    TeCoord2D( 542, 426 ) );
  params.SetParameter( "input_box1" , input_box1 );
  
  TeBox input_box2( TeCoord2D( 238, 689 ) , 
    TeCoord2D( 391, 473 ) );
  params.SetParameter( "input_box2" , input_box2 );  
  
  params.SetParameter( "enable_multi_thread" , (int)1 ); 
  
  params.SetParameter( "pixel_x_relation" , (double)1 ); 
  params.SetParameter( "pixel_y_relation" , (double)1 ); 
  
  TePDIMIMatching match_instance; 
  match_instance.ToggleProgInt( true );
  TEAGN_TRUE_OR_THROW( match_instance.Reset( params ),
    "Algorithm reset error" )
    
  TEAGN_LOGMSG( "Algorithm started" )
  
  time_t init_time = clock() / CLOCKS_PER_SEC;
    
  TEAGN_TRUE_OR_THROW( match_instance.Apply(),
    "Algorithm apply error" )      
    
  time_t end_time = clock() / CLOCKS_PER_SEC;
  
  TEAGN_LOGMSG( "Time elapsed (sameImageDifBoxesTest): " + 
    TeAgnostic::to_string( (long int)( end_time - init_time ) ) +
    " seconds" );
    
  /* Displaying tie-points */
  {
    TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
    TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
    
    while( it != it_end ) {
      std::cout << "[" + Te2String( it->pt1.x(),1 ) + " , " +
        Te2String( it->pt1.y(),1 ) + "] -> [" +
        Te2String( it->pt2.x(),1 ) + " , " + 
        Te2String( it->pt2.y(),1 ) + "]" << std::endl;
      
      ++it;
    }
  }
  
  /* Tie-points draw */
  
  raster2Tiff( input_image1_ptr, 1, TEPDIEXAMPLESBINPATH
    "TeMIMatching_test_sameImageGreenXBlueTest_input_image1.tif",
    out_tie_points_ptr, 0 );
  raster2Tiff( input_image2_ptr, 2, TEPDIEXAMPLESBINPATH
    "TeMIMatching_test_sameImageGreenXBlueTest_input_image2.tif",
    out_tie_points_ptr, 1 );    
    
  TEAGN_WATCH( (unsigned int)out_tie_points_ptr->size() );
  TEAGN_CHECK_EPS( 4, out_tie_points_ptr->size(),
    0, "Invalid tie-points number" )    
}

void sameInvertedImageTest()
{
  /* Creating parameters */

  TePDIParameters params;
  
  TePDITypes::TePDIRasterPtrType input_image1_ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( input_image1_ptr->init(), 
    "Unable to init input_image1_ptr" );    
  params.SetParameter( "input_image1_ptr" , input_image1_ptr );
  
  params.SetParameter( "input_channel1" , (unsigned int)0 );
    
  TePDITypes::TePDIRasterPtrType input_image2_ptr;
  {
    TePDITypes::TePDIRasterPtrType di2_ptr( new TeRaster(
      std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
    TEAGN_TRUE_OR_THROW( di2_ptr->init(), 
      "Unable to init input_image2_ptr" ); 
      
    TeRasterParams input_image2_pars = di2_ptr->params();
    input_image2_pars.nBands( 1 );
    
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( input_image2_ptr,
      input_image2_pars, false ), "Raster alloc error" )
      
    double value = 0;
    for( int lin = 0 ; lin < input_image2_pars.nlines_ ; ++lin )
      for( int col = 0 ; col < input_image2_pars.ncols_ ; ++col )
      {
        di2_ptr->getElement( col, lin, value, 0 );
        input_image2_ptr->setElement( col, lin, 255.0 - value, 0 );
      }
  }
  params.SetParameter( "input_image2_ptr" , input_image2_ptr ); 
  
  params.SetParameter( "input_channel2" , (unsigned int)0 );       
    
  TeSharedPtr< TeCoordPairVect > out_tie_points_ptr( new TeCoordPairVect );
  params.SetParameter( "out_tie_points_ptr" , out_tie_points_ptr );        
  
  TeBox input_box1( TeCoord2D( 142, 826 ) , 
    TeCoord2D( 542, 426 ) );
  params.SetParameter( "input_box1" , input_box1 );
  
  TeBox input_box2( TeCoord2D( 238, 689 ) , 
    TeCoord2D( 391, 473 ) );
  params.SetParameter( "input_box2" , input_box2 );  
  
  params.SetParameter( "enable_multi_thread" , (int)1 ); 
  
  params.SetParameter( "pixel_x_relation" , (double)1 ); 
  params.SetParameter( "pixel_y_relation" , (double)1 ); 
  
  TePDIMIMatching match_instance; 
  match_instance.ToggleProgInt( true );
  TEAGN_TRUE_OR_THROW( match_instance.Reset( params ),
    "Algorithm reset error" )
    
  TEAGN_LOGMSG( "Algorithm started" )
  
  time_t init_time = clock() / CLOCKS_PER_SEC;
    
  TEAGN_TRUE_OR_THROW( match_instance.Apply(),
    "Algorithm apply error" )      
    
  time_t end_time = clock() / CLOCKS_PER_SEC;
  
  TEAGN_LOGMSG( "Time elapsed (sameImageDifBoxesTest): " + 
    TeAgnostic::to_string( (long int)( end_time - init_time ) ) +
    " seconds" );
    
  /* Displaying tie-points */
  {
    TeCoordPairVect::iterator it = out_tie_points_ptr->begin();
    TeCoordPairVect::iterator it_end = out_tie_points_ptr->end();
    
    while( it != it_end ) {
      std::cout << "[" + Te2String( it->pt1.x(),1 ) + " , " +
        Te2String( it->pt1.y(),1 ) + "] -> [" +
        Te2String( it->pt2.x(),1 ) + " , " + 
        Te2String( it->pt2.y(),1 ) + "]" << std::endl;
      
      ++it;
    }
  }
  
  /* Tie-points draw */
  
  raster2Tiff( input_image1_ptr, 0, TEPDIEXAMPLESBINPATH
    "TeMIMatching_test_sameInvertedImageTest_input_image1.tif",
    out_tie_points_ptr, 0 );
  raster2Tiff( input_image2_ptr, 0, TEPDIEXAMPLESBINPATH
    "TeMIMatching_test_sameInvertedImageTest_input_image2.tif",
    out_tie_points_ptr, 0 );    
    
  TEAGN_WATCH( (unsigned int)out_tie_points_ptr->size() );
  TEAGN_CHECK_EPS( 4, out_tie_points_ptr->size(),
    0, "Invalid tie-points number" )    
}

int main()
{
  TEAGN_LOGMSG( "Test started." );

  try{
    TeStdIOProgress pi;
    TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );     
    
    time_t init_time = clock() / CLOCKS_PER_SEC;
    
    sameImageTest();
    sameImageTest2();
    halfsampledImageTest();
    halfsampledImageTest2();
    sameImageGreenXRedTest();
    sameImageGreenXBlueTest();
    sameInvertedImageTest();
    
    time_t end_time = clock() / CLOCKS_PER_SEC;
    
    TEAGN_LOGMSG( "Total elapsed time: " + 
      TeAgnostic::to_string( (long int)( end_time - init_time ) ) +
      " seconds" );    
  }
  catch( const TeException& e ){
    TEAGN_LOGERR( "Test Failed - " + e.message() );
    return EXIT_FAILURE;
  }  

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}

