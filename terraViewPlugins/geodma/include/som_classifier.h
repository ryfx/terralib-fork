/**
  \file som_classifier.h
  \brief This file contains structures and functions for Self Organizing Maps - SOM.
  \author Thales Sehn Korting <tkorting@dpi.inpe.br>
*/

#ifndef __SOM_CLASSIFIER_H
#define __SOM_CLASSIFIER_H

#define INF 99999999

#include <base_classifier.h>

#include <TeMatrix.h>
#include <TeAgnostic.h>
#include <TePDIPIManager.hpp>

#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;

/**
  \class som_classifier_params
  \brief This class contains parameters which are required by SOM Classifier.
  \author Thales Sehn Korting <tkorting@dpi.inpe.br>
*/

class som_classifier_params
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

    //! neighborhood function
    double h( vector<unsigned> w1, vector<unsigned> w2 );

    //! returns euclidean distance between two vectors
    double get_distance( vector<double> a, vector<double> b );

  public:
    //! Constructor without parameters
    som_classifier_params() {};

    //! Constructor with parameters
    som_classifier_params( const unsigned& ta, const unsigned& ww, const unsigned& wh, const double& al, const double& ra, const double& dr, const unsigned& me );

    //! Destructor
    ~som_classifier_params();

    //! Operator =
    som_classifier_params& operator=(const som_classifier_params& p);

};

/*
Let 
  X be the set of n training patterns x1, x2,..xn 
  W be a p x q grid of units wij where i and j are 
    their coordinates on that grid 
  a  be the learning rate, assuming values in 
    ]0,1[, initialized to a given initial  
    learning rate 
  r be the radius of the neighborhood function 
    h(wij,wmn,r),initialized to a given initial 
    radius 
 
1 Repeat 
2   For k=1 to n 
3     For all wij E W, calculate dij = || xk - wij || 
4       Select the unit that minimizes dij as the 
        winner wwinner 
5       Update each unit wij E W: wij = wij +  
        alpha h(wwinner,wij,r) ||xk�wij || 
6   Decrease the value of a and r 
7 Until a reaches 0
*/

/**
  \class som_classifier
  \brief This class contains parameters which are required by SOM Classifier.
  \author Thales Sehn Korting <tkorting@dpi.inpe.br>
*/

class som_classifier
{
  protected:
    som_classifier_params params_;

  public:
    //! Constructor
    som_classifier(const som_classifier_params& p): params_(p) {};

    //! Sets SOM classifier parameters
    void setSOMClassifierParams( const som_classifier_params& p )
    { params_ = p; }

    //! Gets SOM classifier parameters
    som_classifier_params& getSOMClassifierParams()
    { return params_; }

    //! Trains the classifier based on training pattern set
    /*!
      \param itBegin: iterator that points to the first training pattern
      \param itEnd: iterator that points to the last training pattern
      \param dimensions: dimensions of the training patterns which must be considered in the training 
    */
    bool train( TeClassSampleSet::iterator itBegin, TeClassSampleSet::iterator itEnd, const vector<int> dimensions );

    //! Classifies the a pattern set
    /*!
      \param itBegin: iterator that points to the first pattern that will be classified
      \param itEnd: iterator that points to the last pattern that will be classified
      \param dimensions: dimensions of the patterns which must be considered in the classification 
      \param classifyResult: the result class associated to each pattern
    */
    bool classify( vector<TeClassSample>::iterator itBegin, vector<TeClassSample>::iterator itEnd, const vector<int> dimensions, list<int>& classifyResult );
};

#endif
