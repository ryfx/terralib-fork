/*
TerraLib - a library for developing GIS applications.
Copyright  2001, 2002, 2003 INPE and Tecgraf/PUC-Rio.

This code is part of the TerraLib library.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

You should have received a copy of the GNU Lesser General Public
License along with this library.

The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular
purpose. The library provided hereunder is on an "as is" basis, and the
authors have no obligation to provide maintenance, support, updates,
enhancements, or modifications.
In no event shall INPE be held liable to any party
for direct, indirect, special, incidental, or consequential damages arising
out of the use of this library and its documentation.
*/

#ifndef TELANDSCAPE_HPP
  #define TELANDSCAPE_HPP

#include <TePDIUtils.hpp>
#include <TePDIStatistic.hpp>
#include <TeOverlay.h>
#include <TeDefines.h>

/**
* @class TeLandscape Class for landscape metrics extraction
* @author Marcio Azeredo <azeredo@dpi.inpe.br>
* @author Thales Sehn Korting <tkorting@dpi.inpe.br>
* @author Ewerton Ribeiro <ewertonce@gmail.com>
*/

class TeLandscape
{

  public:
    /**
    * @brief Default Constructor.
    * @param cellSet, Input cellular set from where the properties will be generated.
    * @param polygonSet, Input polygons used by the cells.
    */
    TeLandscape( const TeCellSet &cellSet, const TePolygonSet &polygonSet );

    /**
    * @brief Default Destructor.
    */
    ~TeLandscape();

    /**
    * @brief Returns the Metric CA, sum of areas (in hectares) of a cell.
    * @param cellIndex, the cell index.
    */
    double getCA( unsigned cellIndex );

    /**
    * @brief Returns the percentual area occupied by polygons inside a cell.
    * @param cellIndex, the cell index.
    */
    double getPercentLand( unsigned cellIndex );

    /**
    * @brief Returns number of patches inside a cell. (This metric is called NP)
    * @param cellIndex, the cell index.
    */
    unsigned getTotalCellFrags( unsigned cellIndex );

    /**
    * @brief Returns the percentual...
    * @param cellIndex, the cell index.
    */
    double getIJI( unsigned cellIndex );

    /**
    * @brief Returns the number of polygons per square kilometer (km2) inside a cell.
    * @param cellIndex, the cell index.
    */
    double getPD( unsigned cellIndex );

    /**
    * @brief Returnn the Total Edge (TE), the sum of the lengths of all edge
    * segments involving the corresponding patch type.
    * @param cellIndex, the cell index.
    */
    double getTE( unsigned cellIndex );

    /**
    * @brief Returns the average patch size, in hectares, inside a cell.
    * @param cellIndex, the cell index.
    */
    double getMPS( unsigned cellIndex );

    /**
    * @brief Returns the shape compexity of all patches inside a cell.
    * @param cellIndex, the cell index.
    */
    double getLSI( unsigned cellIndex );

    /**
    * @brief Returns the average perimeter to area ratio inside a cell.
    * @param cellIndex, the cell index.
    */
    double getMSI( unsigned cellIndex );

    /**
    * @brief Returns the average perimeter to area ratio, weighted by patch area inside a cell.
    * @param cellIndex, the cell index.
    */
    double getAWMSI( unsigned cellIndex );

    /**
    * @brief Returns the average fractal dimension of patches inside a cell.
    * @param cellIndex, the cell index.
    */
    double getMPFD( unsigned cellIndex );

    /**
    * @brief Returns the average fractal dimension wighted by patch area inside a cell.
    * @param cellIndex, the cell index.
    */
    double getAWMPFD( unsigned cellIndex );

    /**
    * @brief Prints all attributes for a given cell, or all cellSet.
    * @param cellIndex, the cell index.
    */
    void Print( int cellIndex = -1 );

    /**
    * @brief Returns the Edge Density (ED), sum of the perimeters inside
    * the cell, divided by the cell area.
    * @param cellIndex, the cell index.
    */
    double getED( unsigned cellIndex );

    /**
    * @brief Returns the Mean Perimeter Area Ratio (MPAR).
    * @param cellIndex, the cell index.
    */
    double getMPAR( unsigned cellIndex );

    /**
    * @brief Returns the Patch Size Standard Deviation (PSSD).
    * @param cellIndex, the cell index.
    */
    double getPSSD( unsigned cellIndex );

    /**
    * @brief Returns the Patch Size Coefficient of Variation (PSCOV).
    * @param cellIndex, the cell index.
    */
    double getPSCOV( unsigned cellIndex );

    /**
    * @brief Returns the feature value according a specific feature name.
    * @param name The feature name.
    * @param cellIndex, the cell index.
    * @param value A reference to the value.
    * @return True in case feature is found, and False otherwise.
    */
    bool getFeature( string name, unsigned cellIndex, double& value );

  private:
    /**
    * Internal polygon set reference.
    */
    const TePolygonSet& polygonSet_;

    /**
    * Internal cell set reference.
    */
    const TeCellSet& cellSet_;

    /**
    * Stores a polygonset for every cell.
    */
    vector<TePolygonSet> polygonSetIntersects_;
    vector<TePolygonSet> polygonSetIntersectsRaster_;

    /**
    * Rasterized polygonset
    */
    TePDITypes::TePDIRasterPtrType raster_polygonset_;

    /**
    * Flag to avoid multiple raster creations
    */
    bool created_raster_;

    /**
    * @brief Get all the polygons inside one single cell.
    * @param cellIndex, the cell index.
    */
    TePolygonSet getPolygonSetInsideCell( unsigned cellIndex );

    /**
    * @brief Get all the polygons inside one single cell, using rasterized polygonset.
    * @param cellIndex, the cell index.
    */
    TePolygonSet getPolygonSetInsideCellRaster( unsigned cellIndex );

    /**
    * @brief Returns the perimeter from a polygon.
    * @param p, the polygon.
    */
    double getPerimeter( TePolygon p );

    /**
    * @brief Returns the area of a cell.
    * @param cellIndex, the cell index.
    */
    double getLandscapeArea( unsigned cellIndex );

    /**
    * @brief Returns the perimeter of the cell.
    * @param cellIndex, the cell index.
    */
    double getLandscapePerimeter( unsigned cellIndex );

    /**
    * @brief Returns the sum of areas from patches inside a cell.
    * @param cellIndex, the cell index.
    */
    double getTotalFragsArea( unsigned cellIndex );

    /**
    * @brief Returns the sum of perimeters from patches inside a cell.
    * @param cellIndex, the cell index.
    */
    double getTotalFragsPerimeter( unsigned cellIndex );
};

#endif

