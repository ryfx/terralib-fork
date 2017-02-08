#include "TeLandscape.h"

#include <TePDIPIManager.hpp>
#include <TePDIUtils.hpp>
#include <TePDIPIManager.hpp>

#include <set>

TeLandscape::TeLandscape( const TeCellSet &cellSet, const TePolygonSet &polygonSet )
: cellSet_ ( cellSet ), polygonSet_ ( polygonSet )
{
  for ( unsigned i = 0; i < cellSet.size(); i++ )
  {
    polygonSetIntersects_.push_back( TePolygonSet() );
    polygonSetIntersectsRaster_.push_back( TePolygonSet() );
  }

  created_raster_ = false;
}

TeLandscape::~TeLandscape()
{
}

TePolygonSet TeLandscape::getPolygonSetInsideCell( unsigned cellIndex )
{
  if ( polygonSetIntersects_[ cellIndex ].size() == 0 )
  {
    TePolygonSet polygonSet( polygonSet_ );
    TePolygonSet polygonSetCell;
    TePolygonSet polygonSetIntersect_partial;
    TePolygonSet polygonSetIntersect;

    polygonSetCell.add( TeMakePolygon( cellSet_[ cellIndex ].box() ) );
    for ( unsigned i = 0; i < polygonSet.size(); i++ )
    {
      TePolygonSet tmp_polygonSet;
      tmp_polygonSet.add( polygonSet[ i ] );
      TeOVERLAY::TeIntersection( polygonSetCell, tmp_polygonSet, polygonSetIntersect_partial );
      for ( unsigned j = 0; j < polygonSetIntersect_partial.size(); j++ )
        polygonSetIntersect.add( polygonSetIntersect_partial[ j ] );
    }

    polygonSetIntersects_[ cellIndex ].clear();
    for ( unsigned i = 0; i < polygonSetIntersect.size(); i++)
      polygonSetIntersects_[ cellIndex ].add( polygonSetIntersect[ i ] );
  }

  return polygonSetIntersects_[ cellIndex ];
}

TePolygonSet TeLandscape::getPolygonSetInsideCellRaster( unsigned cellIndex )
{
  unsigned curr_col;
  unsigned curr_line;
  TeRaster::iteratorPoly it;
  TePolygonSet internal_full_polygons;

  // discover cellSet size
  unsigned last_line = 0;
  unsigned last_col = 0;
  for ( unsigned i = 0; i < cellSet_.size(); i++ )
  {
    if ( cellSet_[ i ].line() > (int) last_line )
      last_line = cellSet_[ i ].line();
    if ( cellSet_[ i ].column() > (int) last_col )
      last_col = cellSet_[ i ].column();
  }

  if ( polygonSetIntersectsRaster_[ cellIndex ].size() == 0 )
  {
    // create raster, or use already created
    if ( !created_raster_ )
    {
      double width = last_col * 50;
      double height = last_line * 50;

      TeRasterParams r_params;
      r_params.nBands( 1 );
      r_params.setDataType( TeDOUBLE );
      r_params.boundingBoxLinesColumns( cellSet_.box().x1(), cellSet_.box().y1(),
                                        cellSet_.box().x2(), cellSet_.box().y2(), (int) height, (int) width );
      TePDIUtils::TeAllocRAMRaster( raster_polygonset_, r_params, TePDIUtils::TePDIUtilsRamMemPol );

      // initialize raster
      for ( curr_col = 0; curr_col < (unsigned) raster_polygonset_->params().ncols_; curr_col++ )
        for ( curr_line = 0; curr_line < (unsigned) raster_polygonset_->params().nlines_; curr_line++ )
          raster_polygonset_->setElement( curr_col, curr_line, 0.0 );

      // populate raster (0 - no value, > 0 - polygon id's)
      for ( double i = 0; i < polygonSet_.size(); i++ )
      {
        it = raster_polygonset_->begin( polygonSet_[ i ], TeBoxPixelIn, 0 );
        while( it != raster_polygonset_->end( polygonSet_[ i ], TeBoxPixelIn, 0 ) )
        {
          curr_col = it.currentColumn();
          curr_line = it.currentLine();

          raster_polygonset_->setElement( curr_col, curr_line, i + 1 );
          ++it;
        }
      }
      created_raster_ = true;
    }

    // create polygon from cell
    TePolygon polygon_from_cell = TeMakePolygon( cellSet_[ cellIndex ].box() );

    // iterate over raster inside cell
    double polygon_id;
    set< double > polygon_ids;
    set< double >::iterator polygon_ids_it;
    it = raster_polygonset_->begin( polygon_from_cell, TeBoxPixelIn, 0 );

    while( it != raster_polygonset_->end( polygon_from_cell, TeBoxPixelIn, 0 ) )
    {
      curr_col = it.currentColumn();
      curr_line = it.currentLine();
      raster_polygonset_->getElement( curr_col, curr_line, polygon_id );
      ++it; 
      // id == 0, is not a polygon
      if ( polygon_id == 0.0 )
        continue;
     
      polygon_ids.insert( polygon_id );
    }

    // find full polygons inside cell
    for ( polygon_ids_it = polygon_ids.begin(); polygon_ids_it != polygon_ids.end(); ++polygon_ids_it )
      internal_full_polygons.add( polygonSet_[ (unsigned) *polygon_ids_it - 1 ] );

    // make intersection between polygons and cell
    polygonSetIntersectsRaster_[ cellIndex ].clear();
    TePolygonSet intersection;
    TePolygonSet polygonset_from_cell;
    polygonset_from_cell.add( polygon_from_cell );
    TeOVERLAY::TeIntersection( polygonset_from_cell, internal_full_polygons, intersection );
    for ( unsigned i = 0; i < intersection.size(); i++ )
      polygonSetIntersectsRaster_[ cellIndex ].add( intersection[ i ] );
  }

//  return internal_full_polygons;
  return polygonSetIntersectsRaster_[ cellIndex ];
}

double TeLandscape::getPerimeter( TePolygon p )
{
  double perimeter = 0.0;

  for ( unsigned l = 0; l < p.size(); l++)
    perimeter += TeLength ( p[ l ] );

  return perimeter;
}

unsigned TeLandscape::getTotalCellFrags( unsigned cellIndex )
{
  TePolygonSet intersect = getPolygonSetInsideCellRaster( cellIndex );

  return intersect.size();
}

double TeLandscape::getTotalFragsArea( unsigned cellIndex )
{
  TePolygonSet intersect = getPolygonSetInsideCellRaster( cellIndex );
  double total = 0.0;

  if ( !intersect.empty() )
    for ( unsigned i = 0; i < intersect.size(); i++ )
      total += TeGeometryArea( intersect[i] );

  return total;
}

double TeLandscape::getTotalFragsPerimeter( unsigned cellIndex )
{
  TePolygonSet intersect = getPolygonSetInsideCellRaster( cellIndex );
  double total = 0.0;

  if ( !intersect.empty() )
    for ( unsigned i = 0; i < intersect.size(); i++ )
      total += getPerimeter( intersect[i] );

  return total;
}

double TeLandscape::getLandscapePerimeter( unsigned cellIndex )
{
  return getPerimeter( TeMakePolygon( cellSet_[ cellIndex ].box() ) );
}

double TeLandscape::getLandscapeArea( unsigned cellIndex )
{
  return TeGeometryArea( cellSet_[ cellIndex ].box() );
}

double TeLandscape::getCA( unsigned cellIndex )
{
  TePolygonSet intersect = getPolygonSetInsideCellRaster( cellIndex );

  double total = 0.0;

  if ( !intersect.empty() )
    for ( unsigned i = 0; i < intersect.size(); i++ )
      total += TeGeometryArea( intersect[ i ] );

  return ( total / 10000.0 ); // 1ha = 10000m2
}

double TeLandscape::getPercentLand( unsigned cellIndex )
{
  double area = getLandscapeArea( cellIndex );
  if ( area == 0.0 )
    area = 9999999999.9;

  return ( getCA( cellIndex ) * 10000 * 100 / area );
}

double TeLandscape::getPD( unsigned cellIndex )
{
  double area = getLandscapeArea( cellIndex );
  if ( area == 0.0 )
    area = 9999999999.9;

  return ( getTotalCellFrags( cellIndex ) / area ) * 10000 * 100;
}

double TeLandscape::getTE( unsigned cellIndex )
{
  double polygonsPerimeter = getTotalFragsPerimeter( cellIndex );

  return ( polygonsPerimeter );
}

double TeLandscape::getMPS( unsigned cellIndex )
{
  unsigned frags = getTotalCellFrags( cellIndex );

  if ( frags == 0 )
    return 0.00000001;

  return ( getCA( cellIndex ) / frags );
}

double TeLandscape::getPSSD( unsigned cellIndex )
{
  double polygonArea;
  double sum = 0.0;

  TePolygonSet intersect = getPolygonSetInsideCellRaster( cellIndex );
  unsigned frags = getTotalCellFrags( cellIndex );
  double meanArea = getMPS( cellIndex ) * 10000;

  for ( unsigned i = 0; i < intersect.size(); i++ )
  {
    polygonArea = TeGeometryArea( intersect[ i ] );
    sum += ( polygonArea - meanArea ) *
           ( polygonArea - meanArea );
  }

  if ( frags == 0 )
    return 0;

  return sqrt( sum / frags ) / 10000.0;
}

double TeLandscape::getED( unsigned cellIndex )
{
  double landscapeArea = getLandscapeArea( cellIndex );
  if ( landscapeArea <= 0 )
    landscapeArea = 0.00000001;
  double polygonsPerimeter = getTotalFragsPerimeter( cellIndex );

  return ( 10000.0 * polygonsPerimeter / landscapeArea );
}

double TeLandscape::getLSI( unsigned cellIndex )
{
  double perimeter = getTotalFragsPerimeter( cellIndex );
  double area = getLandscapeArea( cellIndex );
  if ( area == 0.0 )
    area = 9999999999.9;

  return perimeter / ( 2 * sqrt( TePI * area ) );
}

double TeLandscape::getMSI( unsigned cellIndex )
{
  double perimeter;
  double area;
  double frags = getTotalCellFrags( cellIndex );

  TePolygonSet intersect = getPolygonSetInsideCellRaster( cellIndex );
  double total = 0.0;

  if ( !intersect.empty() )
  {
    for ( unsigned i = 0; i < intersect.size(); i++ )
    {
      perimeter = getPerimeter( intersect[ i ] );
      area = TeGeometryArea( intersect[ i ] );
      if ( area == 0 )
        area = 0.00000001;
      total += perimeter / ( 2 * sqrt ( TePI * area ) );
    }

    if ( frags == 0 )
      frags = 0.00000001;

    return ( total / frags );
  }

  return 0;
}

double TeLandscape::getAWMSI( unsigned cellIndex )
{
  double polygonArea;
  double polygonPerimeter;
  double fragsArea = getTotalFragsArea( cellIndex );
  double total = 0.0;
  TePolygonSet intersect = getPolygonSetInsideCellRaster( cellIndex );

  if ( !intersect.empty() )
  {
    for ( unsigned i = 0; i < intersect.size(); i++ )
    {
      polygonPerimeter = getPerimeter( intersect[ i ] );
      polygonArea = TeGeometryArea( intersect[ i ] );

      if ( polygonArea == 0.0 )
        polygonArea = 0.00000001;
      if ( fragsArea == 0.0 )
        fragsArea = 0.00000001;
      total += ( polygonPerimeter / ( 2 * sqrt( TePI * polygonArea ) ) *
               ( polygonArea / fragsArea ) );
    }
    return total;
  }

  return 0;
}

double TeLandscape::getMPFD( unsigned cellIndex )
{
  double polygonArea;
  double polygonPerimeter;
  double frags = getTotalCellFrags( cellIndex );
  double total = 0.0;
  TePolygonSet intersect = getPolygonSetInsideCellRaster( cellIndex );

  if ( !intersect.empty() )
  {
    for ( unsigned i = 0; i < intersect.size(); i++ )
    {
      polygonPerimeter = getPerimeter( intersect[ i ] );
      if ( polygonPerimeter == 0.0 )
        polygonPerimeter = 0.00000001;

      polygonArea = TeGeometryArea( intersect[ i ] );
      if ( polygonArea == 0.0 )
        polygonArea = 0.00000001;
      if ( polygonArea == 1.0 )
        polygonArea = 1.00000001;

      total += ( 2 * log ( polygonPerimeter ) ) / log ( polygonArea );
    }
    if ( frags == 0 )
      frags = 0.00000001;

    return total / frags;
  }

  return 0.0;
}

double TeLandscape::getAWMPFD( unsigned cellIndex )
{
  double polygonArea;
  double polygonPerimeter;
  double fragsArea = getTotalFragsArea( cellIndex );
  double total = 0.0;
  TePolygonSet intersect = getPolygonSetInsideCellRaster( cellIndex );

  if ( !intersect.empty() )
  {
    for ( unsigned i = 0; i < intersect.size(); i++ )
    {
      polygonPerimeter = getPerimeter( intersect[ i ] );
      if ( polygonPerimeter == 0.0 )
        polygonPerimeter = 0.00000001;
      polygonArea = TeGeometryArea( intersect[ i ] );
      if ( polygonArea == 0.0 )
        polygonArea = 0.00000001;
      if ( polygonArea == 1.0 )
        polygonArea = 1.00000001;

      if ( fragsArea == 0.0 )
        fragsArea = 0.00000001;

      total += ( 2 * log ( polygonPerimeter ) / log ( polygonArea ) ) *
               ( polygonArea / fragsArea );

    }
    return total;
  }

  return 0.0;
}

double TeLandscape::getMPAR( unsigned cellIndex )
{
  double polygonArea;
  double polygonPerimeter;
  double total = 0.0;
  double frags = getTotalCellFrags( cellIndex );

  TePolygonSet intersect = getPolygonSetInsideCellRaster( cellIndex );

  for ( unsigned i = 0; i < intersect.size(); i++ )
  {
    polygonPerimeter = getPerimeter( intersect[ i ] );
    polygonArea = TeGeometryArea( intersect[ i ] );
    if ( polygonArea == 0.0 )
      polygonArea = 0.00000001;

    total += ( polygonPerimeter / polygonArea );
  }

  if ( frags == 0.0 )
    return 0;

  return total / frags;
}

double TeLandscape::getPSCOV( unsigned cellIndex )
{
  return ( 100.0 * getPSSD( cellIndex ) /
                   getMPS( cellIndex ) );
}

double TeLandscape::getIJI( unsigned cellIndex )
{
  double polygonPerimeter = 0.0;
  double cellPerimeter = 0.0;
  double varE = 0.0;
  double result = 0.0;

  TePolygon polygon_from_cell = TeMakePolygon( cellSet_[ cellIndex ].box() );
  cellPerimeter = getPerimeter( polygon_from_cell );
  varE = getTE( cellIndex ) + cellPerimeter;

  TePolygonSet intersect = getPolygonSetInsideCellRaster( cellIndex );

  for ( unsigned i = 0; i < intersect.size(); i++)
  {
    polygonPerimeter = getPerimeter ( intersect[ i ] );

    result += ( polygonPerimeter / varE ) * log( polygonPerimeter / varE );
  }

  return (-100) * result;
}

void TeLandscape::Print( int cellIndex )
{
  unsigned start = 0;
  unsigned finish = cellSet_.size();
  if ( cellIndex != -1 )
  {
    start = cellIndex;
    finish = cellIndex + 1;
  }
  for ( unsigned i = start; i < finish; i++ )
  {
    cout << "Cell " << i << endl;
    cout << "  getCA: " << getCA( i ) << endl;
    cout << "  getPercentLand: " << getPercentLand( i ) << endl;
    cout << "  getNP: " << getTotalCellFrags( i ) << endl;
    cout << "  getPD: " << getPD( i ) << endl;
    cout << "  getTE: " << getTE( i ) << endl;
    cout << "  getMPS: " << getMPS( i ) << endl;
    cout << "  getLSI: " << getLSI( i ) << endl;
    cout << "  getMSI: " << getMSI( i ) << endl;
    cout << "  getAWMSI: " << getAWMSI( i ) << endl;
    cout << "  getMPFD: " << getMPFD( i ) << endl;
    cout << "  getAWMPFD: " << getAWMPFD( i ) << endl;
    cout << "  getED: " << getED( i ) << endl;
    cout << "  getMPAR: " << getMPAR( i ) << endl;
    cout << "  getPSSD: " << getPSSD( i ) << endl;
    cout << "  getPSCOV: " << getPSCOV( i ) << endl;
    cout << "  getIJI: " << getIJI( i ) << endl;
  }
}

bool TeLandscape::getFeature( string name, unsigned cellIndex, double& value )
{
  value = 0;

  if ( name == "c_ca" )
  {
    value = getCA( cellIndex );
    return true;
  }
  if ( name == "c_percentland" )
  {
    value = getPercentLand( cellIndex );
    return true;
  }
  if ( name == "c_pd" )
  {
    value = getPD( cellIndex );
    return true;
  }
  if ( name == "c_mps" )
  {
    value = getMPS( cellIndex );
    return true;
  }
  if ( name == "c_lsi" )
  {
    value = getLSI( cellIndex );
    return true;
  }
  if ( name == "c_msi" )
  {
    value = getMSI( cellIndex );
    return true;
  }
  if ( name == "c_awmsi" )
  {
    value = getAWMSI( cellIndex );
    return true;
  }
  if ( name == "c_mpfd" )
  {
    value = getMPFD( cellIndex );
    return true;
  }
  if ( name == "c_awmpfd" )
  {
    value = getAWMPFD( cellIndex );
    return true;
  }
  if ( name == "c_ed" )
  {
    value = getED( cellIndex );
    return true;
  }
  if ( name == "c_mpar" )
  {
    value = getMPAR( cellIndex );
    return true;
  }
  if ( name == "c_pssd" )
  {
    value = getPSSD( cellIndex );
    return true;
  }
  if ( name == "c_pscov" )
  {
    value = getPSCOV( cellIndex );
    return true;
  }
  if ( name == "c_np" )
  {
    value = getTotalCellFrags( cellIndex );
    return true;
  }
  if ( name == "c_te" )
  {
    value = getTE( cellIndex );
    return true;
  }
  if ( name == "c_iji" )
  {
    value = getIJI( cellIndex );
    return true;
  }

  return false;
}
