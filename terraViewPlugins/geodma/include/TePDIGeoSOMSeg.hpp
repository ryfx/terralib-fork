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

#ifndef TEPDIGEOSOMSEG_HPP
  #define TEPDIGEOSOMSEG_HPP

  #define INF 99999999

  #include <vector>
  #include <map>
  #include <string>
  #include <math.h>
  
  // TerraLib includes
  #include <TePDIAlgorithm.hpp>
  #include <TePDITypes.hpp>
  #include <TePDIPIManager.hpp>
  #include <TePDIUtils.hpp>
  #include <TeSharedPtr.h>
  #include <TeUtils.h>
  #include <TeMatrix.h>

  /**
    * @brief This is the class models an image segmentation process. 
    * @author Thales Sehn Korting <tkorting@dpi.inpe.br>
    * @ingroup TePDISegmentationGroup
    *
    * Segmentation using Self-Organizing Maps
    *
    * @note The general required parameters :
    *
    * @param input_image (TePDITypes::TePDIRasterPtrType)
    * @param input_bands (vector<unsigned>) used bands for input image
    * @param neurons (int) Number of neurons > 1
    * @param output_image (TePDITypes::TePDIRasterPtrType)
    * @param output_polsets (TePDITypes::TePDIPolSetMapPtrType) - 
    * A pointer to the output polygon sets map.
    *
    */

/**********************************************************************************/
/* TerraLib main class for SOMSeg Segmentation                                     */
/**********************************************************************************/

  class TePDIGeoSOMSeg : public TePDIAlgorithm {
    public :
      
      /**
       * @brief Default Constructor.
       *
       */
      TePDIGeoSOMSeg();

      /**
       * @brief Default Destructor
       */
	  ~TePDIGeoSOMSeg() {};
      
      /**
       * @brief Checks if the supplied parameters fits the requirements of 
       * each PDI algorithm implementation.
       *
       * @note Error log messages must be generated. No exceptions generated.
       *
       * @param parameters The parameters to be checked.
       * @return true if the parameters are OK. false if not.
       */
      bool CheckParameters( const TePDIParameters& parameters ) const;      

  protected:    
      /**
       * @brief Reset the internal state to the initial state.
       *
       * @param params The new parameters referente at initial state.
       */
      void ResetState( const TePDIParameters& );    
     
      /**
       * @brief Runs the current algorithm implementation.
       *
       * @return true if OK. false on error.
       */
      bool RunImplementation();
      
  };
  
/** @example TePDISegmentation_test.cpp
 *    Shows how to use this class.
 */    

using namespace std; 

// one sample
typedef vector<double> TeClassSample;

// map of every class to its samples
typedef map<string, vector<TeClassSample> >  TeClassSampleSet;


// class of parameters
/**
  \class TeSOMClassifierParams
  \brief This class contains parameters which are required by SOM Classifier.
  \author Thales Sehn Korting <tkorting@dpi.inpe.br>
*/
class TeDMSOMClassifierParams
{
  public:
    //! grid of weights
    vector<TeMatrix> weights_;

    //! number of attributes
    unsigned total_attributes_;

    //! width of the weights grid
    unsigned w_width_;

    //! height of the weights grid
    unsigned w_height_;

    //! learning rate
    double alpha_;

    //! radius of the neighborhood function
    double radius_;

    //! decreasing rate, for alpha_ and radius_
    double decreasing_rate_;

    //! maximum of epochs
    unsigned max_epochs_;

    //! number of coordinates to be used in the GeoSOM approach - always the last weights
    unsigned total_coordinates_;

    //! radius of geographical BMU
    double k_;
    
    //! if the neurons have fixed position
    bool fixed_units_;

    //! neighborhood function
    double h( vector<unsigned> w1, vector<unsigned> w2 );

    //! returns euclidean distance between two vectors
    double get_distance( vector<double> a, vector<double> b );

  public:
    //! Constructor without parameters
    TeDMSOMClassifierParams() {};

    //! Constructor with parameters
    TeDMSOMClassifierParams( const unsigned& ta, const unsigned& ww, const unsigned& wh, const double& al, 
                             const double& ra, const double& dr, const unsigned& me, const unsigned& tc,
                             const double& k, const bool& fu );

    //! Destructor
    ~TeDMSOMClassifierParams();

    //! Operator =
    TeDMSOMClassifierParams& operator=(const TeDMSOMClassifierParams& p);

};

// class of som classifier
/**
  \class TeSOMClassifier
  \brief This class contains parameters which are required by SOM Classifier.
  \author Thales Sehn Korting <tkorting@dpi.inpe.br>
*/
class TeDMSOMClassifier
{
  protected:
    TeDMSOMClassifierParams params_;

  public:
    //! Constructor
    TeDMSOMClassifier(const TeDMSOMClassifierParams& p): params_(p) {};

    //! Sets SOM classifier parameters
    void setSOMClassifierParams( const TeDMSOMClassifierParams& p )
    { params_ = p; }

    //! Gets SOM classifier parameters
    TeDMSOMClassifierParams& getSOMClassifierParams()
    { return params_; }

    //! Trains the classifier based on training pattern set
    /*!
      \param itBegin: iterator that points to the first training pattern
      \param itEnd: iterator that points to the last training pattern
      \param dimensions: dimensions of the training patterns which must be considered in the training 
    */
    template<typename trainingPatternSetIterator> bool train( trainingPatternSetIterator itBegin, trainingPatternSetIterator itEnd, const vector<int> dimensions );

    //! Classifies the a pattern set
    /*!
      \param itBegin: iterator that points to the first pattern that will be classified
      \param itEnd: iterator that points to the last pattern that will be classified
      \param dimensions: dimensions of the patterns which must be considered in the classification 
      \param classifyResult: the result class associated to each pattern
    */
    template<typename patternSetIterator> bool classify( patternSetIterator itBegin, patternSetIterator itEnd, const vector<int> dimensions, list<int>& classifyResult );
};

#endif
