#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIBlender.hpp>
#include <TePDIUtils.hpp>

#include <TeAgnostic.h>
#include <TeProgress.h>
#include <TeStdIOProgress.h>

void noBlendMethodTest1()
{
  /* Initializing input rasters */

  TePDITypes::TePDIRasterPtrType raster1Ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( raster1Ptr->init(), 
    "Unable to init raster1Ptr" );
    
  TePDITypes::TePDIRasterPtrType raster2Ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop2.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( raster2Ptr->init(), 
    "Unable to init raster1Ptr" );
    
  // what bands we want to use from each raster ??
  
  std::vector< unsigned int > raster1ChannelsVec;
  raster1ChannelsVec.push_back( 0 );
  raster1ChannelsVec.push_back( 1 );
  raster1ChannelsVec.push_back( 2 );
  
  std::vector< unsigned int > raster2ChannelsVec;
  raster2ChannelsVec.push_back( 0 );
  raster2ChannelsVec.push_back( 1 );
  raster2ChannelsVec.push_back( 2 );
  
  // The geometric transformation parameters telling how the input rasters are 
  // overlapped
  
  TeGTParams transParams;
  
  TeCoordPair auxPair;
  
  auxPair.pt1.setXY( 38,25 ); // point over input image 1
  auxPair.pt2.setXY( 333,562 ); // the corresponding point over input image 2
  transParams.tiepoints_.push_back( auxPair );
  
  auxPair.pt1.setXY( 526,43 );
  auxPair.pt2.setXY( 820,580 );
  transParams.tiepoints_.push_back( auxPair );  
  
  auxPair.pt1.setXY( 276,395 );
  auxPair.pt2.setXY( 570,932 );
  transParams.tiepoints_.push_back( auxPair );  
    
  /* Initializing blender instance */
  
  std::vector< double > pixelScales;
  pixelScales.push_back( 1 );
  pixelScales.push_back( 1 );
  pixelScales.push_back( 1 );
  
  std::vector< double > pixelOffsets;
  pixelOffsets.push_back( 0 );
  pixelOffsets.push_back( 0 );
  pixelOffsets.push_back( 0 );
  
  TePDIBlender blender;
  TEAGN_TRUE_OR_THROW( blender.init( raster1Ptr, raster1ChannelsVec,
    raster2Ptr, raster2ChannelsVec, TePDIBlender::NoBlendMethod,
    TePDIInterpolator::NNMethod, transParams, 0, pixelOffsets, pixelScales, 
    pixelOffsets, pixelScales ),
    "Blender initiation error" );
    
  // Creating an output image
  
  TeRasterParams outputRasterParams = raster1Ptr->params();
  
  outputRasterParams.nBands( 3 );
  
  TeBox outputRasterBoundingBox = raster1Ptr->params().boundingBox();
  updateBox( outputRasterBoundingBox, raster2Ptr->params().boundingBox() );
  
  outputRasterParams.boundingBoxResolution( outputRasterBoundingBox.x1(),
    outputRasterBoundingBox.y1(), outputRasterBoundingBox.x2(),
    outputRasterBoundingBox.y2(), raster1Ptr->params().resx_,
    raster1Ptr->params().resy_ );
  
  TePDITypes::TePDIRasterPtrType outputRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outputRasterParams,
    outputRaster ), "outputRaster creation error" );    
  
  // Generate output raster blended values
  
  const int outNLines = outputRaster->params().nlines_;
  const int outNCols = outputRaster->params().ncols_;
  double value = 0;
  const double colOff = ( outputRaster->params().boundingBox().x1() -
    raster1Ptr->params().boundingBox().x1() ) /
    outputRaster->params().resx_;
  const double lineOff = -1.0 * ( outputRaster->params().boundingBox().y2() -
    raster1Ptr->params().boundingBox().y2() ) /
    outputRaster->params().resy_;    
  
  for( int band = 0 ; band < outputRaster->params().nBands() ; ++band )
  {
    for( int line = 0 ; line < outNLines ; ++line )  
    {
      for( int col = 0 ; col < outNCols ; ++col )
      {
        blender.blend( ((double)line) + lineOff, ((double)col) + colOff, band, 
          value );
        
        TEAGN_TRUE_OR_THROW( outputRaster->setElement( col, line, value, band ),
          "Error writing value" );
      }
    }
  }
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outputRaster,
    TEPDIEXAMPLESBINPATH "TePDIBlender_noBlendMethodTest1.tif" ), 
    "GeoTIF generation error" );
}

// Testing no-overlaped images (simulated)
void noBlendMethodNoOverlapTest()
{
  /* Initializing input rasters */

  // This image has 875 x 1009 pixels
  TePDITypes::TePDIRasterPtrType raster1Ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( raster1Ptr->init(), 
    "Unable to init raster1Ptr" );
    
  // This image has 838 x 1009 pixels
  TePDITypes::TePDIRasterPtrType raster2Ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop2.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( raster2Ptr->init(), 
    "Unable to init raster1Ptr" );
    
  // what bands we want to use from each raster ??
  
  std::vector< unsigned int > raster1ChannelsVec;
  raster1ChannelsVec.push_back( 0 );
  raster1ChannelsVec.push_back( 1 );
  raster1ChannelsVec.push_back( 2 );
  
  std::vector< unsigned int > raster2ChannelsVec;
  raster2ChannelsVec.push_back( 0 );
  raster2ChannelsVec.push_back( 1 );
  raster2ChannelsVec.push_back( 2 );
  
  // The geometric transformation parameters telling how the input rasters are 
  // overlapped
  
  TeGTParams transParams;
  
  TeCoordPair auxPair;
  
  auxPair.pt1.setXY( 885,0 ); // point over input image 1
  auxPair.pt2.setXY( 0,0 ); // TopLeft - image 2
  transParams.tiepoints_.push_back( auxPair );
  
  auxPair.pt1.setXY( 885 + 838,0 );
  auxPair.pt2.setXY( 837,0 ); // TopRight - image 2
  transParams.tiepoints_.push_back( auxPair );  
  
  auxPair.pt1.setXY( 885,1009 );
  auxPair.pt2.setXY( 0,1008 ); // LowerLeft - image 2
  transParams.tiepoints_.push_back( auxPair );  
  
  auxPair.pt1.setXY( 885 + 838,1009 );
  auxPair.pt2.setXY( 837,1008 ); // LowerRight - image 2
  transParams.tiepoints_.push_back( auxPair );    
    
  /* Initializing blender instance */
  
  std::vector< double > pixelScales;
  pixelScales.push_back( 1 );
  pixelScales.push_back( 1 );
  pixelScales.push_back( 1 );
  
  std::vector< double > pixelOffsets;
  pixelOffsets.push_back( 0 );
  pixelOffsets.push_back( 0 );
  pixelOffsets.push_back( 0 );
  
  TePDIBlender blender;
  TEAGN_TRUE_OR_THROW( blender.init( raster1Ptr, raster1ChannelsVec,
    raster2Ptr, raster2ChannelsVec, TePDIBlender::NoBlendMethod,
    TePDIInterpolator::NNMethod, transParams, 0, pixelOffsets, pixelScales, 
    pixelOffsets, pixelScales ),
    "Blender initiation error" );
    
  // Creating an output image
  
  TeRasterParams outputRasterParams;
  outputRasterParams.setDataType( TeUNSIGNEDCHAR, 1 );
  outputRasterParams.setNLinesNColumns( raster1Ptr->params().nlines_, 
    10 + raster1Ptr->params().ncols_ + raster2Ptr->params().ncols_ );
  outputRasterParams.nBands( 3 );
  outputRasterParams.setPhotometric( TeRasterParams::TeRGB, -1 );
  
  TePDITypes::TePDIRasterPtrType outputRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outputRasterParams,
    outputRaster ), "outputRaster creation error" );    
  
  // Generate output raster blended values
  
  const int outNLines = outputRaster->params().nlines_;
  const int outNCols = outputRaster->params().ncols_;
  const int outNBands = outputRaster->params().nBands();
  double value = 0;
  
  for( int band = 0 ; band < outNBands ; ++band )
  {
    for( int line = 0 ; line < outNLines ; ++line )  
    {
      for( int col = 0 ; col < outNCols ; ++col )
      {
        blender.blend( line, col, band, value );
        
        TEAGN_TRUE_OR_THROW( outputRaster->setElement( col, line, value, band ),
          "Error writing value" );
      }
    }
  }

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outputRaster,
    TEPDIEXAMPLESBINPATH "TePDIBlender_noBlendMethodNoOverlapTest.tif" ), 
    "GeoTIF generation error" );
}

void euclideanBlendMethodTest1()
{
  /* Initializing input rasters */

  TePDITypes::TePDIRasterPtrType raster1Ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_A.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( raster1Ptr->init(), 
    "Unable to init raster1Ptr" );
    
  TePDITypes::TePDIRasterPtrType raster2Ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_B_contraste.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( raster2Ptr->init(), 
    "Unable to init raster1Ptr" );
    
  // what bands we want to use from each raster ??
  
  std::vector< unsigned int > raster1ChannelsVec;
  raster1ChannelsVec.push_back( 0 );
  
  std::vector< unsigned int > raster2ChannelsVec;
  raster2ChannelsVec.push_back( 0 );
  
  // The geometric transformation parameters telling how the input rasters are 
  // overlapped
  
  TeGTParams transParams;
  
  TeCoordPair auxPair;
  
  auxPair.pt2.setXY( 18,48 ); // point over input image 2
  auxPair.pt1.setXY( 232,280 ); // the corresponding point over input image 1
  transParams.tiepoints_.push_back( auxPair );
  
  auxPair.pt2.setXY( 148,345 );
  auxPair.pt1.setXY( 362,577 );
  transParams.tiepoints_.push_back( auxPair );  
  
  auxPair.pt2.setXY( 327,17 );
  auxPair.pt1.setXY( 541,249 );
  transParams.tiepoints_.push_back( auxPair );  
    
  /* Initializing blender instance */
  
  std::vector< double > pixelScales;
  pixelScales.push_back( 1 );

  std::vector< double > pixelOffsets;
  pixelOffsets.push_back( 0 );
  
  TePDIBlender blender;
  TEAGN_TRUE_OR_THROW( blender.init( raster1Ptr, raster1ChannelsVec,
    raster2Ptr, raster2ChannelsVec, TePDIBlender::EuclideanBlendMethod,
    TePDIInterpolator::NNMethod, transParams, 0, pixelOffsets, pixelScales, 
    pixelOffsets, pixelScales ),
    "Blender initiation error" );
    
  // Creating an output image
  
  TeRasterParams outputRasterParams = raster1Ptr->params();
  
  outputRasterParams.nBands( 1 );
  
  TeBox outputRasterBoundingBox = raster1Ptr->params().boundingBox();
  updateBox( outputRasterBoundingBox, raster2Ptr->params().boundingBox() );
  
  outputRasterParams.boundingBoxResolution( outputRasterBoundingBox.x1(),
    outputRasterBoundingBox.y1(), outputRasterBoundingBox.x2(),
    outputRasterBoundingBox.y2(), raster1Ptr->params().resx_,
    raster1Ptr->params().resy_ );
  
  TePDITypes::TePDIRasterPtrType outputRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outputRasterParams,
    outputRaster ), "outputRaster creation error" );    
  
  // Generate output raster blended values
  
  const int outNLines = outputRaster->params().nlines_;
  const int outNCols = outputRaster->params().ncols_;
  double value = 0;
  const double colOff = ( outputRaster->params().boundingBox().x1() -
    raster1Ptr->params().boundingBox().x1() ) /
    outputRaster->params().resx_;
  const double lineOff = -1.0 * ( outputRaster->params().boundingBox().y2() -
    raster1Ptr->params().boundingBox().y2() ) /
    outputRaster->params().resy_;    
  
  for( int band = 0 ; band < outputRaster->params().nBands() ; ++band )
  {
    for( int line = 0 ; line < outNLines ; ++line )  
    {
      for( int col = 0 ; col < outNCols ; ++col )
      {
        blender.blend( ((double)line) + lineOff, ((double)col) + colOff, band, 
          value );
        
        TEAGN_TRUE_OR_THROW( outputRaster->setElement( col, line, value, band ),
          "Error writing value" );
      }
    }
  }
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outputRaster,
    TEPDIEXAMPLESBINPATH "TePDIBlender_euclideanBlendMethodTest1.tif" ), 
    "GeoTIF generation error" );
}

void euclideanBlendMethodTest2()
{
  /* Initializing input rasters */

  TePDITypes::TePDIRasterPtrType raster1Ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_C.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( raster1Ptr->init(), 
    "Unable to init raster1Ptr" );
    
  TePDITypes::TePDIRasterPtrType raster2Ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop_B_contraste.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( raster2Ptr->init(), 
    "Unable to init raster1Ptr" );
    
  // what bands we want to use from each raster ??
  
  std::vector< unsigned int > raster1ChannelsVec;
  raster1ChannelsVec.push_back( 0 );
  
  std::vector< unsigned int > raster2ChannelsVec;
  raster2ChannelsVec.push_back( 0 );
  
  // The geometric transformation parameters telling how the input rasters are 
  // overlapped
  
  TeGTParams transParams;
  
  TeCoordPair auxPair;
  
  auxPair.pt1.setXY( 36,234 ); // point over input image 1
  auxPair.pt2.setXY( 245,21 ); // the corresponding point over input image 2
  transParams.tiepoints_.push_back( auxPair );
  
  auxPair.pt1.setXY( 319,252 );
  auxPair.pt2.setXY( 528,39 );
  transParams.tiepoints_.push_back( auxPair );  
  
  auxPair.pt1.setXY( 136,416 );
  auxPair.pt2.setXY( 345,203 );
  transParams.tiepoints_.push_back( auxPair );  
    
  /* Initializing blender instance */
  
  std::vector< double > pixelScales;
  pixelScales.push_back( 1 );

  std::vector< double > pixelOffsets;
  pixelOffsets.push_back( 0 );
  
  TePDIBlender blender;
  TEAGN_TRUE_OR_THROW( blender.init( raster1Ptr, raster1ChannelsVec,
    raster2Ptr, raster2ChannelsVec, TePDIBlender::EuclideanBlendMethod,
    TePDIInterpolator::NNMethod, transParams, 0, pixelOffsets, pixelScales, 
    pixelOffsets, pixelScales ),
    "Blender initiation error" );
    
  // Creating an output image
  
  TeRasterParams outputRasterParams = raster1Ptr->params();
  
  outputRasterParams.nBands( 1 );
  
  TeBox outputRasterBoundingBox = raster1Ptr->params().boundingBox();
  updateBox( outputRasterBoundingBox, raster2Ptr->params().boundingBox() );
  
  outputRasterParams.boundingBoxResolution( outputRasterBoundingBox.x1(),
    outputRasterBoundingBox.y1(), outputRasterBoundingBox.x2(),
    outputRasterBoundingBox.y2(), raster1Ptr->params().resx_,
    raster1Ptr->params().resy_ );
  
  TePDITypes::TePDIRasterPtrType outputRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outputRasterParams,
    outputRaster ), "outputRaster creation error" );    
  
  // Generate output raster blended values
  
  const int outNLines = outputRaster->params().nlines_;
  const int outNCols = outputRaster->params().ncols_;
  double value = 0;
  const double colOff = ( outputRaster->params().boundingBox().x1() -
    raster1Ptr->params().boundingBox().x1() ) /
    outputRaster->params().resx_;
  const double lineOff = -1.0 * ( outputRaster->params().boundingBox().y2() -
    raster1Ptr->params().boundingBox().y2() ) /
    outputRaster->params().resy_;    
  
  for( int band = 0 ; band < outputRaster->params().nBands() ; ++band )
  {
    for( int line = 0 ; line < outNLines ; ++line )  
    {
      for( int col = 0 ; col < outNCols ; ++col )
      {
        blender.blend( ((double)line) + lineOff, ((double)col) + colOff, band, 
          value );
        
        TEAGN_TRUE_OR_THROW( outputRaster->setElement( col, line, value, band ),
          "Error writing value" );
      }
    }
  }
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outputRaster,
    TEPDIEXAMPLESBINPATH "TePDIBlender_euclideanBlendMethodTest2.tif" ), 
    "GeoTIF generation error" );
}

// Testing no-overlaped images (simulated)
void euclideanBlendMethodNoOverlapTest()
{
  /* Initializing input rasters */

  // This image has 875 x 1009 pixels
  TePDITypes::TePDIRasterPtrType raster1Ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( raster1Ptr->init(), 
    "Unable to init raster1Ptr" );
    
  // This image has 838 x 1009 pixels
  TePDITypes::TePDIRasterPtrType raster2Ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop2.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( raster2Ptr->init(), 
    "Unable to init raster1Ptr" );
    
  // what bands we want to use from each raster ??
  
  std::vector< unsigned int > raster1ChannelsVec;
  raster1ChannelsVec.push_back( 0 );
  raster1ChannelsVec.push_back( 1 );
  raster1ChannelsVec.push_back( 2 );
  
  std::vector< unsigned int > raster2ChannelsVec;
  raster2ChannelsVec.push_back( 0 );
  raster2ChannelsVec.push_back( 1 );
  raster2ChannelsVec.push_back( 2 );
  
  // The geometric transformation parameters telling how the input rasters are 
  // overlapped
  
  TeGTParams transParams;
  
  TeCoordPair auxPair;
  
  auxPair.pt1.setXY( 885,0 ); // point over input image 1
  auxPair.pt2.setXY( 0,0 ); // TopLeft - image 2
  transParams.tiepoints_.push_back( auxPair );
  
  auxPair.pt1.setXY( 885 + 838,0 );
  auxPair.pt2.setXY( 837,0 ); // TopRight - image 2
  transParams.tiepoints_.push_back( auxPair );  
  
  auxPair.pt1.setXY( 885,1009 );
  auxPair.pt2.setXY( 0,1008 ); // LowerLeft - image 2
  transParams.tiepoints_.push_back( auxPair );  
  
  auxPair.pt1.setXY( 885 + 838,1009 );
  auxPair.pt2.setXY( 837,1008 ); // LowerRight - image 2
  transParams.tiepoints_.push_back( auxPair );    
    
  /* Initializing blender instance */
  
  std::vector< double > pixelScales;
  pixelScales.push_back( 1 );
  pixelScales.push_back( 1 );
  pixelScales.push_back( 1 );
  
  std::vector< double > pixelOffsets;
  pixelOffsets.push_back( 0 );
  pixelOffsets.push_back( 0 );
  pixelOffsets.push_back( 0 );
  
  TePDIBlender blender;
  TEAGN_TRUE_OR_THROW( blender.init( raster1Ptr, raster1ChannelsVec,
    raster2Ptr, raster2ChannelsVec, TePDIBlender::EuclideanBlendMethod,
    TePDIInterpolator::NNMethod, transParams, 0, pixelOffsets, pixelScales, 
    pixelOffsets, pixelScales ),
    "Blender initiation error" );
    
  // Creating an output image
  
  TeRasterParams outputRasterParams;
  outputRasterParams.setDataType( TeUNSIGNEDCHAR, 1 );
  outputRasterParams.setNLinesNColumns( raster1Ptr->params().nlines_, 
    10 + raster1Ptr->params().ncols_ + raster2Ptr->params().ncols_ );
  outputRasterParams.nBands( 3 );
  outputRasterParams.setPhotometric( TeRasterParams::TeRGB, -1 );
  
  TePDITypes::TePDIRasterPtrType outputRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outputRasterParams,
    outputRaster ), "outputRaster creation error" );    
  
  // Generate output raster blended values
  
  const int outNLines = outputRaster->params().nlines_;
  const int outNCols = outputRaster->params().ncols_;
  const int outNBands = outputRaster->params().nBands();
  double value = 0;
  
  for( int band = 0 ; band < outNBands ; ++band )
  {
    for( int line = 0 ; line < outNLines ; ++line )  
    {
      for( int col = 0 ; col < outNCols ; ++col )
      {
        blender.blend( line, col, band, value );
        
        TEAGN_TRUE_OR_THROW( outputRaster->setElement( col, line, value, band ),
          "Error writing value" );
      }
    }
  }

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outputRaster,
    TEPDIEXAMPLESBINPATH "TePDIBlender_euclideanBlendMethodNoOverlapTest.tif" ), 
    "GeoTIF generation error" );
}

// Testing overlaped images (one image is inside another)
void euclideanBlendMethodContainsTest()
{
  /* Initializing input rasters */

  TePDITypes::TePDIRasterPtrType raster1Ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( raster1Ptr->init(), 
    "Unable to init raster1Ptr" );
    
  // This image has 250 x 250 pixels
  TePDITypes::TePDIRasterPtrType raster2Ptr( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_chip500_500.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( raster2Ptr->init(), 
    "Unable to init raster1Ptr" );
    
  // what bands we want to use from each raster ??
  
  std::vector< unsigned int > raster1ChannelsVec;
  raster1ChannelsVec.push_back( 2 );
  
  std::vector< unsigned int > raster2ChannelsVec;
  raster2ChannelsVec.push_back( 1 );
  
  // The geometric transformation parameters telling how the input rasters are 
  // overlapped
  
  TeCoord2D topLeftCoord = raster2Ptr->params().index2Coord(
    TeCoord2D( 0, 0 ) );
  TeCoord2D topLeftIdxOverR1 = raster1Ptr->params().coord2Index(
    topLeftCoord );       
  
  TeGTParams transParams;
  TEAGN_TRUE_OR_THROW( transParams.direct_parameters_.Init( 3, 3, 0.0 ),
    "Matrix init error" );
  TEAGN_TRUE_OR_THROW( transParams.inverse_parameters_.Init( 3, 3, 0.0 ),
    "Matrix init error" );
  transParams.direct_parameters_( 0, 0 ) = 1.0;
  transParams.direct_parameters_( 0, 2 ) = -500.0;
  transParams.direct_parameters_( 1, 1 ) = 1.0;
  transParams.direct_parameters_( 1, 2 ) = -500.0;
  transParams.inverse_parameters_( 0, 0 ) = 1.0;
  transParams.inverse_parameters_( 0, 2 ) = 500.0;
  transParams.inverse_parameters_( 1, 1 ) = 1.0;
  transParams.inverse_parameters_( 1, 2 ) = 500.0;  
    
  /* Initializing blender instance */
  
  std::vector< double > pixelScales;
  pixelScales.push_back( 1 );
  
  std::vector< double > pixelOffsets;
  pixelOffsets.push_back( 0 );
  
  TePDIBlender blender;
  TEAGN_TRUE_OR_THROW( blender.init( raster1Ptr, raster1ChannelsVec,
    raster2Ptr, raster2ChannelsVec, TePDIBlender::EuclideanBlendMethod,
    TePDIInterpolator::NNMethod, transParams, 0, pixelOffsets, pixelScales, 
    pixelOffsets, pixelScales ),
    "Blender initiation error" );
    
  // Creating an output image
  
  TeRasterParams outputRasterParams;
  outputRasterParams.setDataType( TeUNSIGNEDCHAR, 1 );
  outputRasterParams.setNLinesNColumns( raster1Ptr->params().nlines_, 
    raster1Ptr->params().ncols_ );
  outputRasterParams.nBands( 1 );
  outputRasterParams.setPhotometric( TeRasterParams::TeRGB, -1 );
  
  TePDITypes::TePDIRasterPtrType outputRaster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( outputRasterParams,
    outputRaster ), "outputRaster creation error" );    
  
  // Generate output raster blended values
  
  const int outNLines = outputRaster->params().nlines_;
  const int outNCols = outputRaster->params().ncols_;
  double value = 0;
  
  for( int line = 0 ; line < outNLines ; ++line )  
  {
    for( int col = 0 ; col < outNCols ; ++col )
    {
      blender.blend( line, col, 0, value );
      
      TEAGN_TRUE_OR_THROW( outputRaster->setElement( col, line, value, 0 ),
        "Error writing value" );
    }
  }

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( outputRaster,
    TEPDIEXAMPLESBINPATH "TePDIBlender_euclideanBlendMethodContainsTest.tif" ), 
    "GeoTIF generation error" );
}

int main()
{
  TEAGN_LOGMSG( "Test started." );

  TeStdIOProgress pi;
  TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );     

  euclideanBlendMethodContainsTest();
  euclideanBlendMethodNoOverlapTest();
  noBlendMethodNoOverlapTest();
  euclideanBlendMethodTest2();
  euclideanBlendMethodTest1();
  noBlendMethodTest1();

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}

