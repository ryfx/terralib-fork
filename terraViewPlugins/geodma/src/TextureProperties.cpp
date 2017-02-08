#include "TextureProperties.h"
#include <limits.h>
#include <TeRaster.h>
//#include <TePDIPIManager.h>

TextureProperties::TextureProperties( const TePolygonSet& polSet, 
                                      const TePDITypes::TePDIRasterPtrType raster ): polSet_( polSet ), raster_( raster )
{
  map<string, unsigned> empty_matrix;
  for ( unsigned i = 0; i < polSet_.size(); i++ )
  {
    cooccurrence_.push_back( empty_matrix );
    exist_matrix_.push_back( false );
  }
}

TextureProperties::~TextureProperties()
{
  cooccurrence_.clear();
  exist_matrix_.clear();
  pijs_.clear();
}

double TextureProperties::getDissimilarity( int band, int polIndex )
{
  TEAGN_DEBUG_CONDITION( band < raster_->params().nBands(), "Invalid band" )
  TEAGN_DEBUG_CONDITION( polIndex < (int) polSet_.size(), "Invalid index" )

  //TePDIPIManager status( "Extracting Dissimilarity...", max_pixel[ polIndex ] - min_pixel[ polIndex ], true ) ;

  calcCooccurrenceMatrix( polIndex );

  double dissimilarity = 0.0;
  pair<double, double> pixels;
  double pixel_i, pixel_j, pij;
  string pos;  

  set<pair<double, double> >::iterator it_pij = pijs_[ polIndex ].begin();
  while( it_pij != pijs_[ polIndex ].end() )
  {
    pixels = *it_pij;
    pixel_i = pixels.first;
    pixel_j = pixels.second;

    pos = Te2String( pixel_i ) + "." + Te2String( pixel_j ) + "." + Te2String( band );
    pij = cooccurrence_[ polIndex ][ pos ];
    if ( pij > 0 )
      dissimilarity += pij * fabs( pixel_i - pixel_j );

    //status.Increment();
    ++it_pij;
  }
  //status.Toggle( false );

  return dissimilarity;
}

double TextureProperties::getEntropy( int band, int polIndex )
{
  TEAGN_DEBUG_CONDITION( band < raster_->params().nBands(), "Invalid band" )
  TEAGN_DEBUG_CONDITION( polIndex < (int) polSet_.size(), "Invalid index" )

  //TePDIPIManager status( "Extracting Entropy...", max_pixel[ polIndex ] - min_pixel[ polIndex ], true ) ;

  calcCooccurrenceMatrix( polIndex );

  double entropy = 0.0;
  pair<double, double> pixels;
  double pixel_i, pixel_j, pij;
  string pos;  

  set<pair<double, double> >::iterator it_pij = pijs_[ polIndex ].begin();
  while( it_pij != pijs_[ polIndex ].end() )
  {
    pixels = *it_pij;
    pixel_i = pixels.first;
    pixel_j = pixels.second;

    pos = Te2String( pixel_i ) + "." + Te2String( pixel_j ) + "." + Te2String( band );
    pij = cooccurrence_[ polIndex ][ pos ];
    if ( pij > 0 )
      entropy += pij * log( pij );
    //status.Increment();
    ++it_pij;
  }
  //status.Toggle( false );

  return entropy;
}

double TextureProperties::getHomogeneity( int band, int polIndex )
{
  TEAGN_DEBUG_CONDITION( band < raster_->params().nBands(), "Invalid band" )
  TEAGN_DEBUG_CONDITION( polIndex < (int) polSet_.size(), "Invalid index" )

  //TePDIPIManager status( "Extracting Homogeneity...", max_pixel[ polIndex ] - min_pixel[ polIndex ], true ) ;

  calcCooccurrenceMatrix( polIndex );

  double homogeneity = 0.0;
  pair<double, double> pixels;
  double pixel_i, pixel_j, pij;
  string pos;  

  set<pair<double, double> >::iterator it_pij = pijs_[ polIndex ].begin();
  while( it_pij != pijs_[ polIndex ].end() )
  {
    pixels = *it_pij;
    pixel_i = pixels.first;
    pixel_j = pixels.second;

    pos = Te2String( pixel_i ) + "." + Te2String( pixel_j ) + "." + Te2String( band );
    pij = cooccurrence_[ polIndex ][ pos ];
    if ( pij > 0 )
      homogeneity += pij / ( 1 + ( pixel_i - pixel_j ) * ( pixel_i - pixel_j ) );
    //status.Increment();
    ++it_pij;
  }
  //status.Toggle( false );

  return homogeneity;
}

void TextureProperties::calcCooccurrenceMatrix( int polIndex )
{
  TEAGN_DEBUG_CONDITION( polIndex < (int) polSet_.size(), "Invalid index" )

  if ( exist_matrix_[ polIndex ] )
    return;

  TeRaster::iteratorPoly itPol = raster_->begin( polSet_[ polIndex ], TeBoxPixelIn );
  double pixel_i, pixel_j;
  pair<double, double> pixels;

  while( itPol != raster_->end( polSet_[ polIndex ], TeBoxPixelIn ) )
  {
    int x = itPol.currentColumn(),
        y = itPol.currentLine();

    for ( unsigned band = 0; band < (unsigned) raster_->params().nBands(); band++ )
    {
      if ( raster_->getElement( x, y, pixel_i, band ) && 
           raster_->getElement( x + 1, y, pixel_j, band ) )
      {
        string pos = Te2String( pixel_i ) + "." + Te2String( pixel_j ) + "." + Te2String( band );
        cooccurrence_[ polIndex ][ pos ]++;
        pixels.first = pixel_i;
        pixels.second = pixel_j;

        pijs_[ polIndex ].insert( pixels );
      }
    }

    ++itPol;
  }
  exist_matrix_[ polIndex ] = true;
}
