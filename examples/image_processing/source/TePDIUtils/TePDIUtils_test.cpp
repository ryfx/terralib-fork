#define TEAGN_ENABLE_STDOUT_LOG

#include <TePDIExamplesBase.hpp>

#include <TePDIUtils.hpp>

#include <TeAgnostic.h>

#include <string>

#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TePrecision.h>
#include <TeInitRasterDecoders.h>


void raster_compare_test()
{
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );

  TEAGN_TRUE_OR_THROW(
    TePDIUtils::rasterCompare( inRaster, inRaster ),
    "Raster compare error" );
}


void TeGetRasterMinMaxBounds_test()
{
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );

  double min = 0;
  double max = 0;

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeGetRasterMinMaxBounds( inRaster, 0, min, max ),
    "Unable to get bounds" );

  TEAGN_CHECK_EQUAL( min, 0, "Invalid min value" );
  TEAGN_CHECK_EQUAL( max, 255, "Invalid max value" );

}


void TeRaster2Geotiff_deprecated_test()
{
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  
  double pixel_value = 0;
  TEAGN_TRUE_OR_THROW( inRaster->getElement( 0, 0, pixel_value, 0 ),
    "Unable to get element" );
  TEAGN_CHECK_EPS( pixel_value, 88., 0.00001, 
    "Pixel value inversion detected" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( inRaster,
    TEPDIEXAMPLESBINPATH "TeRaster2Geotiff_test.tif" ), "GeoTIF generation error" );

  TePDITypes::TePDIRasterPtrType inRaster2( new TeRaster(
    std::string( TEPDIEXAMPLESBINPATH "TeRaster2Geotiff_test.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster2->init(), "Unable to init inRaster2" );
  
  TEAGN_TRUE_OR_THROW( inRaster2->getElement( 0, 0, pixel_value, 0 ),
    "Unable to get element" );
  TEAGN_CHECK_EPS( pixel_value, 88., 0.00001, 
    "Pixel value inversion detected" );

  TEAGN_TRUE_OR_THROW(
    TePDIUtils::rasterCompare( inRaster, inRaster2 ),
    "Raster compare error" );
}


void TeRaster2Geotiff_test()
{
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  
  double pixel_value = 0;
  TEAGN_TRUE_OR_THROW( inRaster->getElement( 0, 0, pixel_value, 0 ),
    "Unable to get element" );
  TEAGN_CHECK_EPS( pixel_value, 88., 0.00001, 
    "Pixel value inversion detected" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( inRaster,
    TEPDIEXAMPLESBINPATH "TeRaster2Geotiff_test.tif",
    true ), "GeoTIF generation error" );

  TePDITypes::TePDIRasterPtrType inRaster2( new TeRaster(
    std::string( TEPDIEXAMPLESBINPATH "TeRaster2Geotiff_test.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster2->init(), "Unable to init inRaster2" );
  
  TEAGN_TRUE_OR_THROW( inRaster2->getElement( 0, 0, pixel_value, 0 ),
    "Unable to get element" );
  TEAGN_CHECK_EPS( pixel_value, 88., 0.00001, 
    "Pixel value inversion detected" );

  TEAGN_TRUE_OR_THROW(
    TePDIUtils::rasterCompare( inRaster, inRaster2 ),
    "Raster compare error" );
}

void TeRaster2RGBGeotiff_test()
{
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_rgb342_crop1_chip0_0.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2RGBGeotiff( inRaster,
    TEPDIEXAMPLESBINPATH "TeRaster2RGBGeotiff_test.tif",
    true ), "GeoTIF generation error" );

  TePDITypes::TePDIRasterPtrType inRaster2( new TeRaster(
    std::string( TEPDIEXAMPLESBINPATH "TeRaster2RGBGeotiff_test.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster2->init(), "Unable to init inRaster2" );

  TEAGN_TRUE_OR_THROW(
    TePDIUtils::rasterCompare( inRaster, inRaster2 ),
    "Raster compare error" );
}

void TeRaster2Jpeg_test()
{
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  
  double pixel_value = 0;
  TEAGN_TRUE_OR_THROW( inRaster->getElement( 0, 0, pixel_value, 0 ),
    "Unable to get element" );
  TEAGN_CHECK_EPS( pixel_value, 88., 0.00001, 
    "Pixel value inversion detected" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Jpeg( inRaster,
    TEPDIEXAMPLESBINPATH "TeRaster2Jpeg_test.jpg",
    true, 100 ), "GeoTIF generation error" );

  TePDITypes::TePDIRasterPtrType inRaster2( new TeRaster(
    std::string( TEPDIEXAMPLESBINPATH "TeRaster2Jpeg_test.jpg" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster2->init(), "Unable to init inRaster2" );
  
  TEAGN_TRUE_OR_THROW( inRaster2->getElement( 0, 0, pixel_value, 0 ),
    "Unable to get element" );
  TEAGN_CHECK_EPS( pixel_value, 88., 0.00001, 
    "Pixel value inversion detected" );
}


void TeGetRasterReqMem_test()
{
  TePDITypes::TePDIRasterPtrType RAMRaster;

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( RAMRaster, 10,
    10, 10, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Allocation error" );
    
  TEAGN_TRUE_OR_THROW( 
    ( TePDIUtils::TeGetRasterReqMem( RAMRaster->params() ) == 1000 ),
    "Invalid TeGetRasterReqMem result" );
}


void TeAllocRAMRaster1_test()
{
  TePDITypes::TePDIRasterPtrType RAMRaster;

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( RAMRaster, 10,
    10, 10, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Allocation error" );

  TEAGN_TRUE_OR_THROW( 
    RAMRaster->params().status_ == TeRasterParams::TeReadyToWrite, 
    "Raster status error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->params().nlines_ == 10, "Lines error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->params().ncols_ == 10, "Columns error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->nBands() == 10, "Bands error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->params().dataType_[ 0 ] == TeUNSIGNEDCHAR,
    "data type error" );
}


void TeAllocRAMRaster_templateraster_test()
{
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );

  TePDITypes::TePDIRasterPtrType RAMRaster( new TeRaster );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( inRaster, RAMRaster, 0,
    true, false, TeUNSIGNEDCHAR ), "RAM Raster Allocation error" );

  TEAGN_TRUE_OR_THROW( RAMRaster->params().dataType_[ 0 ] == TeUNSIGNEDCHAR,
    "data type error" );
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->params().status_ == TeRasterParams::TeReadyToWrite, 
    "Raster status error" );
    
  TEAGN_TRUE_OR_THROW( RAMRaster->params().nlines_ == 
    inRaster->params().nlines_, "Checking error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->params().ncols_ == 
    inRaster->params().ncols_, "Checking error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->params().dataType_[ 0 ] == 
    inRaster->params().dataType_[ 0 ], "Checking error" );
  TEAGN_TRUE_OR_THROW( ( *(RAMRaster->params().projection()) ) == 
    ( *(inRaster->params().projection()) ), "Checking error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->params().box() == 
    inRaster->params().box(), "Checking error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->params().resx_ == 
    inRaster->params().resx_, "Checking error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->params().resy_ == 
    inRaster->params().resy_, "Checking error" );
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->params().status_ == TeRasterParams::TeReadyToWrite, 
    "Raster status error" );    
}


void TeAllocRAMRaster_usingparams_test()
{
  TePDITypes::TePDIRasterPtrType RAMRaster;
  
  TeRasterParams params;
  params.nBands( 10 );
  params.nlines_ = 10;
  params.ncols_ = 10;
  params.setDataType( TeUNSIGNEDCHAR, -1 );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( RAMRaster, params, 
    TePDIUtils::TePDIUtilsAutoMemPol ), "RAM Raster Allocation error" );

  TEAGN_TRUE_OR_THROW( 
    RAMRaster->params().status_ == TeRasterParams::TeReadyToWrite, 
    "Raster status error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->params().nlines_ == 10, "Lines error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->params().ncols_ == 10, "Columns error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->nBands() == 10, "Bands error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->params().dataType_[ 0 ] == TeUNSIGNEDCHAR,
    "data type error" );
}


void TeAllocMemoryMappedRaster_test()
{
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );

  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );

  TePDITypes::TePDIRasterPtrType RAMRaster;

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( inRaster, RAMRaster, 0,
    true, true, TeUNSIGNEDCHAR ), "RAM Raster Allocation error" );

  TEAGN_TRUE_OR_THROW( RAMRaster->params().dataType_[ 0 ] == TeUNSIGNEDCHAR,
    "data type error" );
  TEAGN_TRUE_OR_THROW( 
    RAMRaster->params().status_ == TeRasterParams::TeReadyToWrite, 
    "Raster status error" );    

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeCopyRasterBands( inRaster,
    RAMRaster, false, false ), "Raster band copy error" );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( RAMRaster,
    TEPDIEXAMPLESBINPATH "TeAllocMemoryMappedRaster_test.tif" ), "GeoTIF generation error" );
}


void IsFloatBand_test()
{

  TePDITypes::TePDIRasterPtrType RAMRaster_int;

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( RAMRaster_int, 10,
    10, 10, false, TeINTEGER, 0 ), "RAM Raster Allocation error" );
    
  TEAGN_TRUE_OR_THROW( ( ! TePDIUtils::IsFloatBand( RAMRaster_int, 0 ) ),
    "Invalid result" );
  
  TePDITypes::TePDIRasterPtrType RAMRaster_uchar;
    
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( RAMRaster_uchar, 10,
    10, 10, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Allocation error" );    
    
  TEAGN_TRUE_OR_THROW( ( ! TePDIUtils::IsFloatBand( RAMRaster_uchar, 0 ) ),
    "Invalid result" );
    
  TePDITypes::TePDIRasterPtrType RAMRaster_float;
    
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( RAMRaster_float, 10,
    10, 10, false, TeFLOAT, 0 ), "RAM Raster Allocation error" );    
    
  TEAGN_TRUE_OR_THROW( ( TePDIUtils::IsFloatBand( RAMRaster_float, 0 ) ),
    "Invalid result" );    
    
  TePDITypes::TePDIRasterPtrType RAMRaster_double;
    
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( RAMRaster_double, 10,
    10, 10, false, TeDOUBLE, 0 ), "RAM Raster Allocation error" );    
    
  TEAGN_TRUE_OR_THROW( ( TePDIUtils::IsFloatBand( RAMRaster_double, 0 ) ),
    "Invalid result" );     
};


void buildDetailedBox_test()
{
  TeNoProjection proj;

  TeRasterParams params;
  params.nBands( 1 );
  params.nlines_ = 3;
  params.ncols_ = 3;
  params.projection( &proj );
  
  TePDITypes::TePDIRasterPtrType ram_raster;
  TEAGN_TRUE_OR_THROW( 
    TePDIUtils::TeAllocRAMRaster( ram_raster, params, false ), 
    "RAM Raster Allocation error" );
    
  TePolygon box_polygon;
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::buildDetailedBox( ram_raster,
    box_polygon), "detailed box building error" );
    
  TEAGN_TRUE_OR_THROW( ( box_polygon.size() == 1 ), 
    "Invalid linear rings number" );
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ].size() == 9 ), 
    "Invalid points number" );
    
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 0 ] == 
    ram_raster->index2Coord( TeCoord2D( 0, 0 ) ) ), 
    "Invalid point" );
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 1 ] == 
    ram_raster->index2Coord( TeCoord2D( 1, 0 ) ) ), 
    "Invalid point" );
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 2 ] == 
    ram_raster->index2Coord( TeCoord2D( 2, 0 ) ) ), 
    "Invalid point" );
  
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 3 ] == 
    ram_raster->index2Coord( TeCoord2D( 2, 1 ) ) ), 
    "Invalid point" );
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 4 ] == 
    ram_raster->index2Coord( TeCoord2D( 2, 2 ) ) ), 
    "Invalid point" );
    
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 5 ] == 
    ram_raster->index2Coord( TeCoord2D( 1, 2 ) ) ), 
    "Invalid point" );
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 6 ] == 
    ram_raster->index2Coord( TeCoord2D( 0, 2 ) ) ), 
    "Invalid point" );
    
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 7 ] == 
    ram_raster->index2Coord( TeCoord2D( 0, 1 ) ) ), 
    "Invalid point" );
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 8 ] == 
    ram_raster->index2Coord( TeCoord2D( 0, 0 ) ) ), 
    "Invalid point" );
            
}


void buildDetailedBBox_test()
{
  TeNoProjection proj;

  TeRasterParams params;
  params.nBands( 1 );
  params.nlines_ = 3;
  params.ncols_ = 3;
  params.projection( &proj );
  
  TePDITypes::TePDIRasterPtrType ram_raster;
  TEAGN_TRUE_OR_THROW( 
    TePDIUtils::TeAllocRAMRaster( ram_raster, params, false ), 
    "RAM Raster Allocation error" );
    
  TePolygon box_polygon;
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::buildDetailedBBox( ram_raster,
    box_polygon), "detailed box building error" );
    
  TEAGN_TRUE_OR_THROW( ( box_polygon.size() == 1 ), 
    "Invalid linear rings number" );
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ].size() == 13 ), 
    "Invalid points number" );
    
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 0 ] == 
    ram_raster->index2Coord( TeCoord2D( -0.5, -0.5 ) ) ), 
    "Invalid point" );
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 1 ] == 
    ram_raster->index2Coord( TeCoord2D( 0.5, -0.5 ) ) ), 
    "Invalid point" );
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 2 ] == 
    ram_raster->index2Coord( TeCoord2D( 1.5, -0.5 ) ) ), 
    "Invalid point" );
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 3 ] == 
    ram_raster->index2Coord( TeCoord2D( 2.5, -0.5 ) ) ), 
    "Invalid point" );    
  
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 4 ] == 
    ram_raster->index2Coord( TeCoord2D( 2.5, 0.5 ) ) ), 
    "Invalid point" );
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 5 ] == 
    ram_raster->index2Coord( TeCoord2D( 2.5, 1.5 ) ) ), 
    "Invalid point" );
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 6 ] == 
    ram_raster->index2Coord( TeCoord2D( 2.5, 2.5 ) ) ), 
    "Invalid point" );    
    
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 7 ] == 
    ram_raster->index2Coord( TeCoord2D( 1.5, 2.5 ) ) ), 
    "Invalid point" );
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 8 ] == 
    ram_raster->index2Coord( TeCoord2D( 0.5, 2.5 ) ) ), 
    "Invalid point" );
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 9 ] == 
    ram_raster->index2Coord( TeCoord2D( -0.5, 2.5 ) ) ), 
    "Invalid point" );    
    
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 10 ] == 
    ram_raster->index2Coord( TeCoord2D( -0.5, 1.5 ) ) ), 
    "Invalid point" );
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 11 ] == 
    ram_raster->index2Coord( TeCoord2D( -0.5, 0.5 ) ) ), 
    "Invalid point" );    
  TEAGN_TRUE_OR_THROW( ( box_polygon[ 0 ][ 12 ] == 
    ram_raster->index2Coord( TeCoord2D( -0.5, -0.5 ) ) ), 
    "Invalid point" );    
}

void reprojectRaster_test()
{
  TePDITypes::TePDIRasterPtrType inRaster( new TeRaster(
    std::string( TEPDIEXAMPLESRESPATH "cbers_b2_crop.tif" ), 'r' ) );
  TEAGN_TRUE_OR_THROW( inRaster->init(), "Unable to init inRaster" );
  
  TePDITypes::TePDIRasterPtrType ram_raster;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( ram_raster, 10,
    10, 10, false, TeUNSIGNEDCHAR, 0 ), "RAM Raster Allocation error" ); 
    
  //EPSG:4618-LatLong/SAD69
  TeSharedPtr< TeProjection > projPtr( TeProjectionFactory::make( 4618 ) );
  TEAGN_TRUE_OR_THROW( projPtr.isActive(), "Unable to build a projection" );    
    
  TEAGN_TRUE_OR_THROW( TePDIUtils::reprojectRaster( *inRaster,
     inRaster->params().nlines_, inRaster->params().ncols_,
     *projPtr, true, true, *ram_raster ),
    "Raster reprojection error" );
    
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeRaster2Geotiff( ram_raster,
    TEPDIEXAMPLESBINPATH "TePDIUtils_reprojectRaster_test.tif" ), "GeoTIF generation error" );        
    
  TEAGN_TRUE_OR_THROW( ram_raster->projection()->name() == "LatLong",
    "Invalid projection name" );
  TEAGN_CHECK_EPS( -50.616788,
    ram_raster->params().box().x1(), 0.000001, "Invalid box" );
  TEAGN_CHECK_EPS( -50.452947,
    ram_raster->params().box().x2(), 0.000001, "Invalid box" );    
  TEAGN_CHECK_EPS( -19.082162,
    ram_raster->params().box().y1(), 0.0001, "Invalid box" );
  TEAGN_CHECK_EPS( -18.930647,
    ram_raster->params().box().y2(), 0.0001, "Invalid box" );    
  TEAGN_CHECK_EPS( 0.000191,
    ram_raster->params().resx_, 0.0001, "Invalid box" );    
  TEAGN_CHECK_EPS( 0.000191,
    ram_raster->params().resy_, 0.0001, "Invalid box" );    
}


void TeRelation_test()
{
  TeLinearRing r1;
  r1.add( TeCoord2D( 0, 0 ) );
  r1.add( TeCoord2D( 3, 0 ) );
  r1.add( TeCoord2D( 3, 3 ) );
  r1.add( TeCoord2D( 0, 3.3 ) );
  r1.add( TeCoord2D( 0, 0 ) );
  
  TeLinearRing r2;
  r2.add( TeCoord2D( 1, 1 ) );
  r2.add( TeCoord2D( 2, 1 ) );
  r2.add( TeCoord2D( 2, 2 ) );
  r2.add( TeCoord2D( 1, 2 ) );
  r2.add( TeCoord2D( 1, 1 ) );
  
  TePolygon pol1;
  pol1.add( r1 );
  pol1.add( r2 );
  
  TePDITileIndexer ti( pol1, 0.3 ); 
  
  TEAGN_TRUE_OR_THROW( 
    ( TePDIUtils::TeRelation( TeCoord2D( 1.5, 0.5 ), ti ) == TeINSIDE ),
    "Invalid relation" );   
  TEAGN_TRUE_OR_THROW( 
    ( TePDIUtils::TeRelation( TeCoord2D( 2.5, 1.5 ), ti ) == TeINSIDE ),
    "Invalid relation" );   
  TEAGN_TRUE_OR_THROW( 
    ( TePDIUtils::TeRelation( TeCoord2D( 1.5, 2.5 ), ti ) == TeINSIDE ),
    "Invalid relation" );   
  TEAGN_TRUE_OR_THROW( 
    ( TePDIUtils::TeRelation( TeCoord2D( 0.5, 1.5 ), ti ) == TeINSIDE ),
    "Invalid relation" );   
  TEAGN_TRUE_OR_THROW( 
    ( TePDIUtils::TeRelation( TeCoord2D( 0.5, 1.5 ), ti ) == TeINSIDE ),
    "Invalid relation" );   
  TEAGN_TRUE_OR_THROW( 
    ( TePDIUtils::TeRelation( TeCoord2D( 0.1, 3.1 ), ti ) == TeINSIDE ),
    "Invalid relation" );     
      
  TEAGN_TRUE_OR_THROW( 
    ( TePDIUtils::TeRelation( TeCoord2D( 1.5, 1.5 ), ti ) == TeOUTSIDE ),
    "Invalid relation" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIUtils::TeRelation( TeCoord2D( 2.5, 3.5 ), ti ) == TeOUTSIDE ),
    "Invalid relation" );
  TEAGN_TRUE_OR_THROW( 
    ( TePDIUtils::TeRelation( TeCoord2D( -1.0, 1.0 ), ti ) == TeOUTSIDE ),
    "Invalid relation" );    
  TEAGN_TRUE_OR_THROW( 
    ( TePDIUtils::TeRelation( TeCoord2D( -1.0, -1.0 ), ti ) == TeOUTSIDE ),
    "Invalid relation" );    
  TEAGN_TRUE_OR_THROW( 
    ( TePDIUtils::TeRelation( TeCoord2D( 1.0, -1.0 ), ti ) == TeOUTSIDE ),
    "Invalid relation" );  
  TEAGN_TRUE_OR_THROW( 
    ( TePDIUtils::TeRelation( TeCoord2D( 4.0, 4.0 ), ti ) == TeOUTSIDE ),
    "Invalid relation" );        
}


void makeSegmentSet_test()
{
  TeLine2D l1;
  l1.add( TeCoord2D( 0,0 ) );
  l1.add( TeCoord2D( 1,0 ) );
  l1.add( TeCoord2D( 0,1 ) );
  l1.add( TeCoord2D( 0,0 ) );
  
  TeLinearRing lr1( l1 );
  
  TePolygon p1;
  p1.add( lr1 );
  
  TeLineSet ls1;
  TePDIUtils::makeSegmentSet( p1, ls1 );
  
  TEAGN_TRUE_OR_THROW( ( ls1.size() == 3 ), "Invalid line set size" );
  
  TEAGN_TRUE_OR_THROW( ( ls1[ 0 ].size() == 2 ), "Invalid segment size" );
  TEAGN_TRUE_OR_THROW( ( ls1[ 1 ].size() == 2 ), "Invalid segment size" );
  TEAGN_TRUE_OR_THROW( ( ls1[ 2 ].size() == 2 ), "Invalid segment size" );
  
  TEAGN_TRUE_OR_THROW( ( ls1[ 0 ][ 0 ] == TeCoord2D( 0, 0 ) ), 
    "Invalid point" );
  TEAGN_TRUE_OR_THROW( ( ls1[ 0 ][ 1 ] == TeCoord2D( 1, 0 ) ), 
    "Invalid point" );  
    
  TEAGN_TRUE_OR_THROW( ( ls1[ 1 ][ 0 ] == TeCoord2D( 1, 0 ) ), 
    "Invalid point" );
  TEAGN_TRUE_OR_THROW( ( ls1[ 1 ][ 1 ] == TeCoord2D( 0, 1 ) ), 
    "Invalid point" );      

  TEAGN_TRUE_OR_THROW( ( ls1[ 2 ][ 0 ] == TeCoord2D( 0, 1 ) ), 
    "Invalid point" );
  TEAGN_TRUE_OR_THROW( ( ls1[ 2 ][ 1 ] == TeCoord2D( 0, 0 ) ), 
    "Invalid point" );      
}


void TeSegSetIntersection_test()
{
  {
    /* X test */
  
    TeLine2D line1;
    line1.add( TeCoord2D( -1, -1 ) );
    line1.add( TeCoord2D( 1, 1 ) );
    
    TeLineSet ls1;
    ls1.add( line1 );
    
    TeLine2D line2;
    line2.add( TeCoord2D( -1, 1 ) );
    line2.add( TeCoord2D( 1, -1 ) );
    
    TeLineSet ls2;
    ls2.add( line2 );    
    
    TePointSet ps1;
    TePDIUtils::TeSegSetIntersection( ls1, ls2, ps1 );
    
    TEAGN_TRUE_OR_THROW( ( ps1.size() == 1 ), "Invalid point set size" )
    TEAGN_TRUE_OR_THROW( ( ps1[ 0 ].elem() == TeCoord2D( 0, 0 ) ), 
      "Invalid intersection point" )
  }
  
  {
    /* + Test */
  
    TeLine2D line1;
    line1.add( TeCoord2D( -1, 0 ) );
    line1.add( TeCoord2D( 1, 0 ) );
    
    TeLineSet ls1;
    ls1.add( line1 );
    
    TeLine2D line2;
    line2.add( TeCoord2D( 0, 1 ) );
    line2.add( TeCoord2D( 0, -1 ) );
    
    TeLineSet ls2;
    ls2.add( line2 );    
    
    TePointSet ps1;
    TePDIUtils::TeSegSetIntersection( ls1, ls2, ps1 );
    
    TEAGN_TRUE_OR_THROW( ( ps1.size() == 1 ), "Invalid point set size" )
    TEAGN_TRUE_OR_THROW( ( ps1[ 0 ].elem() == TeCoord2D( 0, 0 ) ), 
      "Invalid intersection point" )
  }  
  
  {
    /* Y Axis Parallel line 1 Test */
  
    TeLine2D line1;
    line1.add( TeCoord2D( 0, 1 ) );
    line1.add( TeCoord2D( 0, -1 ) );
    
    TeLineSet ls1;
    ls1.add( line1 );
    
    TeLine2D line2;
    line2.add( TeCoord2D( -1, -1 ) );
    line2.add( TeCoord2D( 1, 1 ) );
    
    TeLineSet ls2;
    ls2.add( line2 );    
    
    TePointSet ps1;
    TePDIUtils::TeSegSetIntersection( ls1, ls2, ps1 );
    
    TEAGN_TRUE_OR_THROW( ( ps1.size() == 1 ), "Invalid point set size" )
    TEAGN_TRUE_OR_THROW( ( ps1[ 0 ].elem() == TeCoord2D( 0, 0 ) ), 
      "Invalid intersection point" )
  }  
  
  {
    /* Y Axis Parallel line 2 Test */
  
    TeLine2D line1;
    line1.add( TeCoord2D( -1, -1 ) );
    line1.add( TeCoord2D( 1, 1 ) );
    
    TeLineSet ls1;
    ls1.add( line1 );
    
    TeLine2D line2;
    line2.add( TeCoord2D( 0, 1 ) );
    line2.add( TeCoord2D( 0, -1 ) );
    
    TeLineSet ls2;
    ls2.add( line2 );    
    
    TePointSet ps1;
    TePDIUtils::TeSegSetIntersection( ls1, ls2, ps1 );
    
    TEAGN_TRUE_OR_THROW( ( ps1.size() == 1 ), "Invalid point set size" )
    TEAGN_TRUE_OR_THROW( ( ps1[ 0 ].elem() == TeCoord2D( 0, 0 ) ), 
      "Invalid intersection point" )
  }    
  
  {
    /* Y Axis Parallel line 2 and line 2 Test */
  
    TeLine2D line1;
    line1.add( TeCoord2D( 0, 1 ) );
    line1.add( TeCoord2D( 0, 1 ) );
    
    TeLineSet ls1;
    ls1.add( line1 );
    
    TeLine2D line2;
    line2.add( TeCoord2D( 0, 1 ) );
    line2.add( TeCoord2D( 0, 1 ) );
    
    TeLineSet ls2;
    ls2.add( line2 );    
    
    TePointSet ps1;
    TePDIUtils::TeSegSetIntersection( ls1, ls2, ps1 );
    
    TEAGN_TRUE_OR_THROW( ( ps1.size() == 0 ), "Invalid point set size" )
  }
  
  {
    /* Parallel lines Test */
  
    TeLine2D line1;
    line1.add( TeCoord2D( -1, 1 ) );
    line1.add( TeCoord2D( 1, 1 ) );
    
    TeLineSet ls1;
    ls1.add( line1 );
    
    TeLine2D line2;
    line2.add( TeCoord2D( -1, -1 ) );
    line2.add( TeCoord2D( 1, -1 ) );
    
    TeLineSet ls2;
    ls2.add( line2 );    
    
    TePointSet ps1;
    TePDIUtils::TeSegSetIntersection( ls1, ls2, ps1 );
    
    TEAGN_TRUE_OR_THROW( ( ps1.size() == 0 ), "Invalid point set size" )
  }  
  
  {
    /* Non-intercepted segments Test */
  
    TeLine2D line1;
    line1.add( TeCoord2D( 0, 0 ) );
    line1.add( TeCoord2D( 2, 2 ) );
    
    TeLineSet ls1;
    ls1.add( line1 );
    
    TeLine2D line2;
    line2.add( TeCoord2D( 1, 0 ) );
    line2.add( TeCoord2D( 2, 0 ) );
    
    TeLineSet ls2;
    ls2.add( line2 );    
    
    TePointSet ps1;
    TePDIUtils::TeSegSetIntersection( ls1, ls2, ps1 );
    
    TEAGN_TRUE_OR_THROW( ( ps1.size() == 0 ), "Invalid point set size" )
  }  
  
}


void getBandMinMaxValues_test()
{
  TePDITypes::TePDIRasterPtrType RAMRaster;
  
  TeRasterParams params;
  params.nBands( 1 );
  params.nlines_ = 1;
  params.ncols_ = 3;
  params.setDataType( TeUNSIGNEDCHAR, -1 );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( RAMRaster, params, 
    TePDIUtils::TePDIUtilsAutoMemPol ), "RAM Raster Allocation error" );
    
  TEAGN_TRUE_OR_THROW( RAMRaster->setElement( 0, 0, 0.0, 0 ),
    "setElement error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->setElement( 1, 0, 1.0, 0 ),
    "setElement error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->setElement( 2, 0, 2.0, 0 ),
    "setElement error" );
   
  double min = -1.0;
  double max = 10.0;
  TePDIUtils::getBandMinMaxValues( RAMRaster, 0, false, 0, min, max );
  
  TEAGN_CHECK_EPS( min, 0.0, 0.0, "invalid min value" )
  TEAGN_CHECK_EPS( max, 2.0, 0.0, "invalid max value" )
}


void convert2MultiBand_test()
{
  TePDITypes::TePDIRasterPtrType RAMRaster;
  
  TeRasterParams params;
  params.nBands( 1 );
  params.setDataType( TeUNSIGNEDCHAR, -1 );
  
  TeProjectionParams projparams;
  projparams.name = "NoProjection";
  TeSharedPtr< TeProjection > proj( TeProjectionFactory::make( 
    projparams ) );          
  params.projection( proj.nakedPointer() );   
  
  params.setNLinesNColumns( 1, 3 );
  
  params.setPhotometric( TeRasterParams::TePallete );
  
  params.lutr_.push_back( 255 );
  params.lutg_.push_back( 0 );
  params.lutb_.push_back( 0 );

  params.lutr_.push_back( 0 );
  params.lutg_.push_back( 255 );
  params.lutb_.push_back( 0 );
  
  params.lutr_.push_back( 0 );
  params.lutg_.push_back( 0 );
  params.lutb_.push_back( 255 );  
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( RAMRaster, params, 
    TePDIUtils::TePDIUtilsAutoMemPol ), "RAM Raster Allocation error" );
    
  TEAGN_TRUE_OR_THROW( RAMRaster->setElement( 0, 0, 0, 0), 
    "setElement error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->setElement( 1, 0, 1, 0),
    "setElement error" );
  TEAGN_TRUE_OR_THROW( RAMRaster->setElement( 2, 0, 2, 0),
    "setElement error" );
    
  TePDITypes::TePDIRasterPtrType RAMRaster2;
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::convert2MultiBand( RAMRaster, true, 
    RAMRaster2 ), "MultiBand conversion error" );
    
  TEAGN_TRUE_OR_THROW( RAMRaster2.isActive(), "Inactive pointer" )
  TEAGN_TRUE_OR_THROW( ( RAMRaster2->params().nlines_ == 1 ), 
    "Invalid lines" )
  TEAGN_TRUE_OR_THROW( ( RAMRaster2->params().ncols_ == 3 ), 
    "Invalid columns" )
  TEAGN_TRUE_OR_THROW( ( RAMRaster2->params().nBands() == 3 ), 
    "Invalid lines" )
    
  TEAGN_TRUE_OR_THROW( ( RAMRaster2->params().photometric_[ 0 ] == 
    TeRasterParams::TeMultiBand ), "Invalid photometric interpretation" )
  TEAGN_TRUE_OR_THROW( ( RAMRaster2->params().photometric_[ 1 ] == 
    TeRasterParams::TeMultiBand ), "Invalid photometric interpretation" )
  TEAGN_TRUE_OR_THROW( ( RAMRaster2->params().photometric_[ 2 ] == 
    TeRasterParams::TeMultiBand ), "Invalid photometric interpretation" )
    
  double value = -1.0;
  
  TEAGN_TRUE_OR_THROW( RAMRaster2->getElement( 0,0, value, 0),
    "getElement error" )
  TEAGN_CHECK_EPS( value, 255, 0, "wrong value" );
  
  TEAGN_TRUE_OR_THROW( RAMRaster2->getElement( 0,0, value, 1),
    "getElement error" )
  TEAGN_CHECK_EPS( value, 0, 0, "wrong value" );
  
  TEAGN_TRUE_OR_THROW( RAMRaster2->getElement( 0,0, value, 2),
    "getElement error" )
  TEAGN_CHECK_EPS( value, 0, 0, "wrong value" );    
  
  
  TEAGN_TRUE_OR_THROW( RAMRaster2->getElement( 1,0, value, 0),
    "getElement error" )
  TEAGN_CHECK_EPS( value, 0, 0, "wrong value" );
  
  TEAGN_TRUE_OR_THROW( RAMRaster2->getElement( 1,0, value, 1),
    "getElement error" )
  TEAGN_CHECK_EPS( value, 255, 0, "wrong value" );

  TEAGN_TRUE_OR_THROW( RAMRaster2->getElement( 1,0, value, 2),
    "getElement error" )
  TEAGN_CHECK_EPS( value, 0, 0, "wrong value" );
  
  
  TEAGN_TRUE_OR_THROW( RAMRaster2->getElement( 2,0, value, 0),
    "getElement error" )
  TEAGN_CHECK_EPS( value, 0, 0, "wrong value" );
  
  TEAGN_TRUE_OR_THROW( RAMRaster2->getElement( 2,0, value, 1),
    "getElement error" )
  TEAGN_CHECK_EPS( value, 0, 0, "wrong value" );

  TEAGN_TRUE_OR_THROW( RAMRaster2->getElement( 2,0, value, 2),
    "getElement error" )
  TEAGN_CHECK_EPS( value, 255, 0, "wrong value" );
}


void compareGeometryPoints_tests()
{
  TePolygonSet ps1;
  {
    TeLinearRing lr;
    lr.add( TeCoord2D( 0, 0 ) );
    lr.add( TeCoord2D( 1, 0 ) );
    lr.add( TeCoord2D( 1, 1 ) );
    lr.add( TeCoord2D( 0, 1 ) );
    lr.add( TeCoord2D( 0, 0 ) );
    
    TePolygon pol;
    pol.add( lr );
    
    ps1.add( pol );
  }
  
  TePolygonSet ps2;
  {
    TeLinearRing lr;
    lr.add( TeCoord2D( 0, 0 ) );
    lr.add( TeCoord2D( 1, 0 ) );
    lr.add( TeCoord2D( 1, 1 ) );
    lr.add( TeCoord2D( 0, 1 ) );
    lr.add( TeCoord2D( 0, 0 ) );
    
    TePolygon pol;
    pol.add( lr );
    
    ps2.add( pol );
  }  
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::compareGeometryPoints( ps1, ps2 ),
    "Invalid compareGeometryPoints result" )
}


void resampleRaster_test()
{
  TePDITypes::TePDIRasterPtrType input_raster;
  TePDITypes::TePDIRasterPtrType output_raster;

  TeRasterParams input_raster_params;
  input_raster_params.nBands( 1 );
  input_raster_params.setDataType( TeDOUBLE, -1 );
  input_raster_params.setNLinesNColumns( 4, 4 );
    
  TeRasterParams output_raster_params;
  output_raster_params.nBands( 1 );
  output_raster_params.setDataType( TeDOUBLE, -1 );
  output_raster_params.setNLinesNColumns( 2, 2 );

  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( input_raster, 
    input_raster_params, TePDIUtils::TePDIUtilsAutoMemPol ), 
    "RAM Raster Allocation error" );
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_raster, 
    output_raster_params, TePDIUtils::TePDIUtilsAutoMemPol ), 
    "RAM Raster Allocation error" );
    
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 0, 0, 1.0, 0 ),
    "setElement error" );
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 1, 0, 1.0, 0 ),
    "setElement error" );
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 2, 0, 1.0, 0 ),
    "setElement error" );
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 3, 0, 1.0, 0 ),
    "setElement error" );
        
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 0, 1, 1.0, 0 ),
    "setElement error" );
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 1, 1, 1.0, 0 ),
    "setElement error" );    
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 2, 1, 1.0, 0 ),
    "setElement error" );  
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 3, 1, 1.0, 0 ),
    "setElement error" );       

  TEAGN_TRUE_OR_THROW( input_raster->setElement( 0, 2, 1.0, 0 ),
    "setElement error" );
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 1, 2, 1.0, 0 ),
    "setElement error" );    
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 2, 2, 1.0, 0 ),
    "setElement error" );    
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 3, 2, 1.0, 0 ),
    "setElement error" ); 
    
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 0, 3, 1.0, 0 ),
    "setElement error" );
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 1, 3, 1.0, 0 ),
    "setElement error" );
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 2, 3, 1.0, 0 ),
    "setElement error" );
  TEAGN_TRUE_OR_THROW( input_raster->setElement( 3, 3, 1.0, 0 ),
    "setElement error" );
    
  double resolution_ratio = 2.0;
  
  TEAGN_TRUE_OR_THROW( TePDIUtils::resampleRasterByRes( input_raster, 
    output_raster,
    resolution_ratio , resolution_ratio, 
    TePDIInterpolator::NNMethod,
    true ), 
    "Raster resample error" )
  
  
  TEAGN_CHECK_EPS( output_raster->params().nlines_, 
    ( input_raster->params().nlines_ / resolution_ratio ), 0.0, 
    "invalid number of lines" )
  TEAGN_CHECK_EPS( output_raster->params().ncols_, 
    ( input_raster->params().ncols_ / resolution_ratio ), 0.0, 
    "invalid number of columns" )
  TEAGN_CHECK_EPS( output_raster->params().resx_, 3, 0.0, 
    "invalid X resolution" )
  TEAGN_CHECK_EPS( output_raster->params().resy_, 3, 0.0, 
    "invalid Y() resolution" )
  TEAGN_CHECK_EPS( output_raster->params().nBands(), 
    input_raster->params().nBands(), 0.0, 
    "invalid bands number" )
    
  double value = 0;
    
  TEAGN_TRUE_OR_THROW( output_raster->getElement( 0, 0, value, 0 ),
    "getElement error" )
  TEAGN_CHECK_EPS( value, 1.0, 0.0, "Incorrect value" )
  
  TEAGN_TRUE_OR_THROW( output_raster->getElement( 0, 1, value, 0 ),
    "getElement error" )
  TEAGN_CHECK_EPS( value, 1.0, 0.0, "Incorrect value" )
  
  TEAGN_TRUE_OR_THROW( output_raster->getElement( 1, 0, value, 0 ),
    "getElement error" )
  TEAGN_CHECK_EPS( value, 1.0, 0.0, "Incorrect value" )  
  
  TEAGN_TRUE_OR_THROW( output_raster->getElement( 1, 1, value, 0 ),
    "getElement error" )
  TEAGN_CHECK_EPS( value, 1.0, 0.0, "Incorrect value" )  
}


int main()
{
  TEAGN_LOGMSG( "Test started." );

  TeStdIOProgress pi;
  TeProgress::setProgressInterf( dynamic_cast< TeProgressBase* >( &pi ) );  
  
  TeInitRasterDecoders();
  
  buildDetailedBox_test();
  buildDetailedBBox_test();
  resampleRaster_test();
  getBandMinMaxValues_test();
  makeSegmentSet_test();
  TeSegSetIntersection_test();
  TeRelation_test();
  TeAllocRAMRaster_usingparams_test();
  TeAllocRAMRaster1_test();
  TeAllocRAMRaster_templateraster_test();
  raster_compare_test();
  TeGetRasterMinMaxBounds_test();
  TeGetRasterReqMem_test();    
  IsFloatBand_test();
  TeRaster2Geotiff_test();
  TeRaster2RGBGeotiff_test();
  TeRaster2Geotiff_deprecated_test();
  TeRaster2Jpeg_test();
  TeAllocMemoryMappedRaster_test();
  convert2MultiBand_test();
  compareGeometryPoints_tests();
  reprojectRaster_test();

  TEAGN_LOGMSG( "Test OK." );
  return EXIT_SUCCESS;
}
