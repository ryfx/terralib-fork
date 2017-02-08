/**
  \file nn_classifier.h
  \brief This file contains structures and functions for Neural Networks - NN.
  \author Thales Sehn Korting <tkorting@dpi.inpe.br>
*/

#ifndef __NN_CLASSIFIER_H
#define __NN_CLASSIFIER_H

#define INF 99999999
// #define _DEBUG

#include <base_classifier.h>

#include <TeMatrix.h>
#include <TeAgnostic.h>
#include <TePDIPIManager.hpp>

#include <vector>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

/**
  \class nn_classifier_params
  \brief This class contains parameters which are required by NN Classifier.
  \author Thales Sehn Korting <tkorting@dpi.inpe.br>
*/

class nn_classifier_params
{
  public:
    //! maximum of epochs
    unsigned max_epochs_;
    
    //! learning rate
    double eta_;
    
    //! number of neurons in the hidden layer
    unsigned hidden_neurons_;
    
    //! activation function type (quadratic, logistic, linear)
    string type_; 
    
    //! number of bits to represent each input value [0, 2^nbits_]
    unsigned nbits_;

  public:
    //! Constructor without parameters
    nn_classifier_params() {};

    //! Constructor with parameters
    nn_classifier_params( const unsigned& me, const double& e, const unsigned& hn, const string& t, const unsigned& nb );

    //! Destructor
    ~nn_classifier_params();

    //! Operator =
    nn_classifier_params& operator=(const nn_classifier_params& p);

};

/**
  \class nn_classifier
  \brief This class contains parameters which are required by NN Classifier.
  \author Thales Sehn Korting <tkorting@dpi.inpe.br>
*/

class nn_classifier
{
  protected:
    nn_classifier_params params_;
    
    //! Activation function
    /*!
      \param v: input value
      \param type: type of activation function, which can be "quadratic", "logistic" or "linear"
    */
    double f ( double v );
    
    //! Slope of activation function
    /*!
      \param y: input value
      \param type: type of activation function, which can be "quadratic", "logistic" or "linear"
    */
    double df ( double y );
    
    //! Matrix of weights from the hidden layer
    TeMatrix hidden_weights;

    //! Matrix of resultant values from activation function inside hidden layer
    TeMatrix hidden_v;

    //! Matrix of resultant values from activation function inside hidden layer plus bias value
    TeMatrix hidden_v_bias;

    //! Matrix of weights from the output layer
    TeMatrix output_weights;

    //! Matrix of resultant values from activation function inside output layer
    TeMatrix output_v;
    
  public:
    //! Constructor
    nn_classifier(const nn_classifier_params& p): params_(p) {};

    //! Sets NN classifier parameters
    void setNNClassifierParams( const nn_classifier_params& p )
    { params_ = p; }

    //! Gets NN classifier parameters
    nn_classifier_params& getNNClassifierParams()
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
