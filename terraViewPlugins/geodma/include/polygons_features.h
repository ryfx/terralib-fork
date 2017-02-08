#ifndef POLYGONS_FEATURES_H
  #define POLYGONS_FEATURES_H

  #include "TeDefines.h"
  #include "TeDataTypes.h"

  class TePolygonSet;
  class TePolygon;
  class TeMatrix;
  class TeCoord2D;
  class TeRaster;

  /**
   * @class polygons_features Polygon set properties generation.
   * @author Thales Sehn Korting <tkorting@dpi.inpe.br>
   */
  class polygons_features
  {

    public :

      /**
       * @brief Default Constructor.
       * @param polSet Input polygon set from where the properties 
       * will be generated.
       */
      polygons_features( const TePolygonSet& polSet );

      /**
       * @brief Default Destructor
       */
      ~polygons_features();

      /**
       * @brief Returns the area of the given polygon.
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonArea( unsigned int polIndex );

      /**
       * @brief Returns the bounding box area of the given polygon.
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonBoxArea( unsigned int polIndex );

      /**
       * @brief Returns the perimeter of the given polygon.
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonPerimeter( unsigned int polIndex );

      /**
       * @brief Returns the fractal dimension of the given polygon.
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonFractalDimension( unsigned int polIndex );

      /**
       * @brief Returns the perimeter/area ratio of the given polygon.
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonPerimeterAreaRatio( unsigned int polIndex );

      /**
       * @brief Returns the compacity of the given polygon.
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonCompacity( unsigned int polIndex );

      /**
       * @brief Returns the shape index of the given polygon.
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonShapeIndex( unsigned int polIndex );
      
      /**
       * @brief Returns the density of the given polygon.
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonDensity( unsigned int polIndex );

      /**
       * @brief Returns the bounding box length from the given polygon.
       * @brief [0, INF)
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonLength( unsigned int polIndex );

      /**
       * @brief Returns the bounding box width from the given polygon.
       * @brief [0, INF)
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonWidth( unsigned int polIndex );

      /**
       * @brief Returns the related circunscribing circle from the given polygon.
       * @brief [0, 1]
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonCircle( unsigned int polIndex );

      /**
       * @brief Returns the contiguity index from the given polygon.
       * @brief [0, 1]
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonContiguity( unsigned int polIndex );

      /**
       * @brief Returns the radius of gyration from the given polygon.
       * @brief [0, INF)
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonGyrationRadius( unsigned int polIndex );

      /**
       * @brief Returns the elliptic fit of the given polygon.
       * @brief [0, 1]
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonEllipticFit( unsigned int polIndex );

      /**
       * @brief Returns the main angle of the given polygon.
       * @brief [0, TePI]
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonAngle( unsigned int polIndex );

      /**
       * @brief Returns the rectangular fit of the given polygon.
       * @brief [0, TePI]
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonRectangularFit( unsigned int polIndex );

      /**
       * @brief Print all attributes for a specific (or every) polygon.
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      void printProperties( int polIndex = -1 );

    protected:

      /**
       * Internal polygon set reference.
       */
      const TePolygonSet& polSet_;

    private:

      // Copy not allowed
      const polygons_features& operator=( const polygons_features& ) { return *this; };

      /**
       * @brief Returns the highest radius inside the given polygon.
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      double getPolygonRadius( unsigned int polIndex );

      /**
       * @brief Returns a vector of coordinates from the polygon border.
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      vector<TeCoord2D> getBorderCoordinates( unsigned int polIndex );

      /**
       * @brief Returns the six(6) terms for a ellipse wich best fits the polygon.
       * @param xyPositions The vector of coordinates from the polygon edges.
       */
      vector<double> fitEllipse( vector<TeCoord2D> xyPositions );

      /**
       * @brief Returns an elliptic polygon.
       * @param A The six(6) ellipse parameters.
       */
      TePolygon createEllipse( vector<double> A );

      /**
       * @brief Returns a rotated polygon.
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       * @param angle The rotation angle.
       */
      TePolygon rotatePolygon( unsigned int polIndex, double angle );
      
      /**
       * @brief Returns a raster with the polygon's bounding box.
       * @param polIndex The polygon index inside the input polygon set 
       * vector.
       */
      TeRaster* getRasterInPolygon( unsigned int polIndex );

      /**
       * @brief Find the eigenvectors of a matrix.
       * @param input_matrix The input matrix.
       * @param e_vec The double vector where the eigenvectors will be.
       */
      bool findEigenVectors( TeMatrix input_matrix, double e_vec[] );
  };

#endif // POLYGONS_FEATURES_H
