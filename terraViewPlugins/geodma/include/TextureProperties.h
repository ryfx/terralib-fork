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

#ifndef TEXTUREPROPERTIES_HPP
  #define TEXTUREPROPERTIES_HPP

#include <TePDIUtils.hpp>
#include <TePDIStatistic.hpp>
#include <TeOverlay.h>
#include <TeDefines.h>

#include <map>
#include <vector>
#include <set>
using namespace std;

/**
* @class TextureProperties Class for texture metrics extraction
* @author Thales Sehn Korting <tkorting@dpi.inpe.br>
*/

class TextureProperties
{
  private:
    /**
      * Internal coocurrence matrix.
      */     
    vector<map<string, unsigned> > cooccurrence_;
    /**
      * Internal raster reference.
      */     
    TePDITypes::TePDIRasterPtrType raster_;
    /**
      * Internal polygon set reference.
      */     
    const TePolygonSet& polSet_;
    /**
      * A flag if the cooccurrence matrix is already calculated or not
      */     
    vector<bool> exist_matrix_;

    /**
      * Stores pixel combinations to reduce processing time
      */
    map<unsigned, set<pair<double, double> > > pijs_;

    /**
      * Stores minimuns and maximuns pixel values
      */
    vector<float> min_pixel_;
    vector<float> max_pixel_;

  public:
    /**
      * @brief Default Constructor.
      * @param polSet Input polygon set from where the properties 
      * will be generated.
      */
    TextureProperties( const TePolygonSet& polSet, const TePDITypes::TePDIRasterPtrType raster );

    /**
      * @brief Default Destructor
      */
    ~TextureProperties();

    /**
      * @brief Returns the dissimilarity of pixels inside the given polygon.
      * @param band The raster band to calculate the matrix
      * @param polIndex The polygon index inside the input polygon set 
      * vector.
      */
    double getDissimilarity( int band, int polIndex );

    /**
      * @brief Returns the entropy of pixels inside the given polygon.
      * @param polIndex The polygon index inside the input polygon set 
      * vector.
      */
    double getEntropy( int band, int polIndex );

    /**
      * @brief Returns the homogeneity of pixels inside the given polygon.
      * @param polIndex The polygon index inside the input polygon set 
      * vector.
      */
    double getHomogeneity( int band, int polIndex );

    /**
      * @brief Calculates the co-occurrence matrix inside the given polygon.
      * @param polIndex The polygon index inside the input polygon set 
      * vector.
      */
    void calcCooccurrenceMatrix( int polIndex );
};

#endif