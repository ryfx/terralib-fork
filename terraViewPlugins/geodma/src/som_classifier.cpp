#include "som_classifier.h"
#include "base_classifier.h"

som_classifier_params& som_classifier_params::operator=(const som_classifier_params& p)
{
  if ( this != &p )
  {
    weights_ = p.weights_;
    w_height_ = p.w_height_;
    w_width_ = p.w_width_;
    alpha_ = p.alpha_;
    radius_ = p.radius_;
    decreasing_rate_ = p.decreasing_rate_;
    total_attributes_ = p.total_attributes_;
    max_epochs_ = p.max_epochs_;
  }
  return *this;
}

som_classifier_params::~som_classifier_params()
{
  #ifdef _DEBUG
  cout << "~som_classifier_params()" << endl;
  #endif
}

som_classifier_params::som_classifier_params( const unsigned& ta, const unsigned& ww, const unsigned& wh, 
                                                  const double& al, const double& ra, const double& dr, const unsigned& me ) : 
                                                  total_attributes_( ta ), w_width_( ww ), w_height_( wh ), 
                                                  alpha_( al ), radius_( ra ), decreasing_rate_( dr ), max_epochs_( me )
{
  TEAGN_TRUE_OR_THROW( total_attributes_ > 0 , "Number of attributes must be > 0" );
  TEAGN_TRUE_OR_THROW( alpha_ >= 0 && alpha_ <= 1, "Alpha parameter should be [0, 1]" );
  TEAGN_TRUE_OR_THROW( radius_ >= 0 , "Radius of influence must be >= 0" );
  TEAGN_TRUE_OR_THROW( max_epochs_ > 0 , "Maximum of epochs must be > 0" );

  unsigned seed = (unsigned) time(0);
  srand(seed);
  for ( unsigned i = 0; i < total_attributes_; i++ )
  {
    TeMatrix tmp_matrix;
    tmp_matrix.Init( w_width_, w_height_, 0.0 );
    for ( unsigned w = 0; w < w_width_; w++ )
      for ( unsigned h = 0; h < w_height_; h++ )
        tmp_matrix(w, h) = ( rand() / (double) RAND_MAX );
    weights_.push_back( tmp_matrix );
  }

  #ifdef _DEBUG
  cout << "end som_classifier_params()" << endl;
  #endif
}

double som_classifier_params::h( vector<unsigned> w1, vector<unsigned> w2 )
{
  double r = 0.0;
  double tmp;

  if ( w1.size() == 0 || w1.size() != w2.size() )
    return 0.0;

  for ( unsigned i = 0; i < w1.size(); i ++ )
  {
    tmp = w1[ i ] - w2[ i ];
    r = r + tmp * tmp;
  }

  return exp( -0.5 * sqrt( r / radius_ ) );
}

double som_classifier_params::get_distance( vector<double> a, vector<double> b )
{
  double r = 0.0;
  double tmp;

  if ( a.size() == 0 || a.size() != b.size() )
    return 0.0;

  for ( unsigned i = 0; i < a.size(); i ++ )
  {
    tmp = ( a[ i ] - b[ i ] );
    r = r + (tmp * tmp);
  }

  return sqrt(r);
}

bool som_classifier::train( TeClassSampleSet::iterator itBegin, 
                            TeClassSampleSet::iterator itEnd, 
                            const vector<int> dimensions )                                                                             
{
  #ifdef _DEBUG
  cout << "begin train()" << endl;
  #endif

  unsigned current_iteration = 0;
  unsigned winner_i = 0;
  unsigned winner_j = 0;
  double winner_d = INF;

  TePDIPIManager status("Training SOM...", params_.max_epochs_, true);

  while ( params_.alpha_ >= 0.0001 && params_.radius_ >= 0.0001 && current_iteration < params_.max_epochs_ )
  {
    status.Increment();
    current_iteration++;
    TeClassSampleSet::iterator attr_it = itBegin;

    while (attr_it != itEnd)
    {
      vector<TeClassSample> vector_sample = (*attr_it).second;
      vector<TeClassSample>::iterator data_it = vector_sample.begin();
      while ( data_it != vector_sample.end() )
      {
        TeClassSample xk;
        winner_i = 0;
        winner_j = 0;
        winner_d = INF;
        for (unsigned i = 0; i < dimensions.size(); i++)
          xk.push_back( (*data_it)[ dimensions[ i ] ] );
        
        // find w_winner
        vector<double> w_winner;
        for ( unsigned i = 0; i < params_.w_width_; i++ )
          for ( unsigned j = 0; j < params_.w_height_; j++ )
          {
            vector<double> wij;
            double d = 0.0;
            for ( unsigned a = 0; a < params_.total_attributes_; a++ )
              wij.push_back( params_.weights_[ a ]( i, j ) );

            d = params_.get_distance( xk, wij );

            if ( d < winner_d )
            {
              winner_i = i;
              winner_j = j;
              winner_d = d;
              w_winner = wij;
            }
          }
          
        vector<unsigned> winner_ij;
        winner_ij.push_back( winner_i );
        winner_ij.push_back( winner_j );
        // update weights based on w_winner
        for ( unsigned i = 0; i < params_.w_width_; i++ )
          for ( unsigned j = 0; j < params_.w_height_; j++ )
          {
            vector<unsigned> ij;
            ij.push_back( i );
            ij.push_back( j );
            double H = params_.h( ij, winner_ij );
            for ( unsigned a = 0; a < params_.total_attributes_; a++ )
              params_.weights_[ a ]( i, j ) = params_.weights_[ a ]( i, j ) + params_.alpha_ * H * ( xk[ a ] - params_.weights_[ a ]( i, j ) );
          }

        ++data_it;
      }

      params_.alpha_ = params_.alpha_ * params_.decreasing_rate_;
      params_.radius_ = params_.radius_ * params_.decreasing_rate_;
      ++attr_it;
    }
  }
  status.Toggle( false );
  
   #ifdef _DEBUG
   cout << "end train()" << endl;
   #endif

  return true;
}

 /*
1 Repeat 
2   For k=1 to n 
3     For all wij E W, calculate dij = || xk - wij || 
4       Select the unit that minimizes dij as the winner wwinner 
5       Update each unit wij E W: wij = wij + alpha * h(wwinner,wij,r) * ||xk_wij || 
6   Decrease the value of a and r 
7 Until a reaches 0
*/

bool som_classifier::classify( vector<TeClassSample>::iterator itBegin, 
                               vector<TeClassSample>::iterator itEnd, 
                               const vector<int> dimensions, 
                               list<int>& classifyResult )
{
  #ifdef _DEBUG
  cout << "begin classify()" << endl;
  #endif

  vector<TeClassSample>::iterator attr_it = itBegin;
  unsigned total_elements = 0;

  while ( attr_it != itEnd )
  {
    total_elements++;
    ++attr_it;
  }

  TePDIPIManager status("Classifying regions...", total_elements , true);
  attr_it = itBegin;
  while ( attr_it != itEnd )
  {
    status.Increment();
    TeClassSample data = (*attr_it);
    TeClassSample xk;
    unsigned winner_i = 0;
    unsigned winner_j = 0;
    double winner_d = INF;

    for ( unsigned i = 0; i < dimensions.size(); i++ )
      xk.push_back( data[ dimensions[ i ] ] );
    
    // find w_winner
    for ( unsigned i = 0; i < params_.w_width_; i++ )
      for ( unsigned j = 0; j < params_.w_height_; j++ )
      {
        vector<double> wij;
        double d = 0.0;
        for ( unsigned a = 0; a < params_.total_attributes_; a++ )
          wij.push_back( params_.weights_[ a ]( i, j ) );

        d = params_.get_distance( xk, wij );
        if ( d < winner_d )
        {
          winner_i = i;
          winner_j = j;
          winner_d = d;
        }
      }
    classifyResult.push_back( winner_i + params_.w_width_ * winner_j );

    ++attr_it;
  }
  status.Toggle( false );

  #ifdef _DEBUG
  cout << "end classify()" << endl;
  #endif
  
  return true;
}