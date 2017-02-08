#include "nn_classifier.h"
#include "base_classifier.h"

nn_classifier_params& nn_classifier_params::operator=(const nn_classifier_params& p)
{
  if ( this != &p )
  {
    max_epochs_ = p.max_epochs_;
    eta_ = p.eta_;
    hidden_neurons_ = p.hidden_neurons_;
    type_ = p.type_;
  }
  return *this;
}

nn_classifier_params::~nn_classifier_params()
{
  #ifdef _DEBUG
  cout << "~nn_classifier_params()" << endl;
  #endif
}

nn_classifier_params::nn_classifier_params( const unsigned& me, 
                                            const double& e, 
                                            const unsigned& hn, 
                                            const string& t,
                                            const unsigned& nb ) : max_epochs_( me ), 
                                                                eta_( e ), 
                                                                hidden_neurons_( hn ), 
                                                                type_( t ),
                                                                nbits_( nb )
{
  #ifdef _DEBUG
  cout << "begin nn_classifier_params()" << endl;
  #endif
  
  TEAGN_TRUE_OR_THROW( max_epochs_ > 0 , "Maximum of epochs must be > 0" );
  TEAGN_TRUE_OR_THROW( eta_ >= 0 && eta_ <= 1, "Eta parameter should be [0, 1]" );
  TEAGN_TRUE_OR_THROW( hidden_neurons_ > 0 , "Number of hidden neurons must be > 0" );
  // TEAGN_TRUE_OR_THROW( type_ == "quadratic" || type_ == "logistic" || type_ == "linear" , "Activation function must be 'quadratic', 'logistic' or 'linear'" );
  TEAGN_TRUE_OR_THROW( nbits_ > 2 , "Number of bits must be > 2" );

  #ifdef _DEBUG
  cout << "end nn_classifier_params()" << endl;
  #endif
}

double nn_classifier::f( double v )
{
  if ( params_.type_ == "quadratic" )
  {
    return v * v;
  }
  else if ( params_.type_ == "logistic" )
  {
    return ( 1 / ( 1 + exp ( -v ) ) );
  }
  else if ( params_.type_ == "linear" )
  {
    return v >= 0;
  }
  
  return 0;
}

double nn_classifier::df( double y )
{
  if ( params_.type_ == "quadratic" )
  {
    return 2 * y;
  }
  else if ( params_.type_ == "logistic" )
  {
    return ( 1 - y ) * y;
  }
  else if ( params_.type_ == "linear" )
  {
    return y >= 0;
  }
  
  return 0;
}

long mypow( long n, long p )
{
  long r = 1;

  for (int i = 0; i < p; i++)
    r *= n;

  return n;
}

void dec2bin( long decimal, char *binary )
{
  int k = 0, n = 0;
  int neg_flag = 0;
  int remain;
  int old_decimal; // for test
  char temp[80];
  
  // take care of negative input
  if (decimal < 0)
  {
    decimal = -decimal;
    neg_flag = 1;
  }
  do
  {
    old_decimal = decimal; // for test
    remain = decimal % 2;
    // whittle down the decimal number
    decimal = decimal / 2;
    // this is a test to show the action
//     printf("%d/2 = %d remainder = %d\n", old_decimal, decimal, remain);
    // converts digit 0 or 1 to character '0' or '1'
    temp[k++] = remain + '0';
  } while (decimal > 0);
/*  
  if (neg_flag)
    temp[k++] = '-'; // add - sign
  else
    temp[k++] = ' '; // space
*/
  // reverse the spelling
  while (k >= 0)
    binary[n++] = temp[--k];
  
  binary[n-1] = 0; // end with NULL
}

int bin2dec( char *bin )   
{
  int  b, k, m, n;
  int  len, sum = 0;
 
  len = strlen(bin) - 1;
  for(k = 0; k <= len; k++) 
  {
    n = (bin[k] - '0'); // char to numeric value
    if ((n > 1) || (n < 0)) 
    {
      puts("\n\n ERROR! BINARY has only 1 and 0!\n");
      return (0);
    }
    for(b = 1, m = len; m > k; m--) 
    {
      // 1 2 4 8 16 32 64 ... place-values, reversed here
      b *= 2;
    }
    // sum it up
    sum = sum + n * b;
    //printf("%d*%d + ",n,b);  // uncomment to show the way this works
  }
  return( sum );
}

TeMatrix mean( TeMatrix input_matrix )
{
  TeMatrix r;
  if ( input_matrix.Nrow() == 1 )
  {
    TeMatrix tmp;
    input_matrix.Transpose( tmp );
    input_matrix = tmp;
  }
  
  r.Init ( 1, input_matrix.Ncol(), 0.0 );
  
  for ( unsigned c = 0; c < (unsigned) input_matrix.Ncol(); c++ )
  {
    double mean = 0.0;
    for ( unsigned l = 0; l < (unsigned) input_matrix.Nrow(); l++ )
      mean += input_matrix ( l, c );
    r( 0, c ) = mean / input_matrix.Nrow();
  }
  return r;
}

bool nn_classifier::train( TeClassSampleSet::iterator itBegin, 
                           TeClassSampleSet::iterator itEnd, 
                           const vector<int> dimensions )
{
  #ifdef _DEBUG
  cout << "begin train()" << endl;
  #endif
  
  /* Initializing neurons */
  
  // every input value are resampled to the interval [0, 2^nbits_]
  // which corresponds to nbits_ bits, plus one bit, the bias
  unsigned input_bits = dimensions.size() * params_.nbits_ + 1;
  TeMatrix input_values;
  input_values.Init( input_bits, 1, 0.0 );
  hidden_weights.Init( params_.hidden_neurons_, input_bits, 0.0 );
  hidden_v.Init( params_.hidden_neurons_, 1, 0.0 );
  hidden_v_bias.Init( params_.hidden_neurons_ + 1, 1, 0.0 );
  
  /* initializing hidden neurons */
  unsigned seed = (unsigned) time( 0 );
  srand(seed);
  for ( unsigned l = 0; l < (unsigned) hidden_weights.Nrow(); l++ )
    for ( unsigned c = 0; c < (unsigned) hidden_weights.Ncol(); c++ )
      hidden_weights( l, c ) = ( rand() / (double) RAND_MAX );
    
  TeClassSampleSet::iterator attr_it = itBegin;
  map<string, string> classes_bit;
  map<string, unsigned> classes_int;
  unsigned class_number = 0;
  while (attr_it != itEnd)
  {
    char bits[ 20 ];
    dec2bin( class_number, bits );
    classes_bit[ (*attr_it).first ] = bits;
    classes_int[ (*attr_it).first ] = class_number++;
    ++attr_it;
  }
/*  
  // a single bit is activated per class
  unsigned output_bits = classes_bit.size();
*/
  // more than one bit activated per class
  unsigned output_bits = (unsigned) ceil( log( (double) classes_bit.size() ) / log( 2.0 ) );
  output_weights.Init( output_bits, params_.hidden_neurons_ + 1, 0.0 );
  output_v.Init( output_bits, 1, 0.0 );
  TeMatrix output_train;
  output_train.Init( output_bits, 1, 0.0 );
  
  /* initializing output neurons */
  for ( unsigned l = 0; l < (unsigned) output_weights.Nrow(); l++ )
    for ( unsigned c = 0; c < (unsigned) output_weights.Ncol(); c++ )
      output_weights( l, c ) = ( rand() / (double) RAND_MAX );
    
/*cout << "initial hidden_heigts: " << endl;
hidden_weights.Print();
cout << "initial output_weights: " << endl;
output_weights.Print();*/
const unsigned same_class_step = 1;

unsigned errors_counter = 0;
TeMatrix error_window;
error_window.Init( 5 * class_number * same_class_step, 1, 1.0 );

// cout << "clear all;" << endl;
// cout << "e = [ ";
// cerr << "eta = [ ";

  const double increasing_eta = ( 1.0 - params_.eta_ ) / (double) params_.max_epochs_;
  TePDIPIManager status("Training NN...", params_.max_epochs_, true);
  for( unsigned epoch = 0; epoch < params_.max_epochs_; epoch++ )
  {
    // one iteration per small set of patterns
    bool there_are_elements = true;
    unsigned actual_element = 0;
    while( there_are_elements )
    {
      there_are_elements = false;
      attr_it = itBegin;
      while( attr_it != itEnd )
      {
        string current_class = (*attr_it).first;
        vector<TeClassSample> samples = (*attr_it).second;
/*cout << "current_class: " << current_class << endl;
cout << "samples.size(): " << samples.size() << endl;
cout << "actual_element: " << actual_element << endl;*/
        if ( actual_element < samples.size() )
        {
          there_are_elements = true;
          for ( unsigned e = actual_element; e < ( actual_element + same_class_step ); e++ )
          {
            if ( e > samples.size() )
              break;
            TeClassSample sample = samples[ e ];
/*
        // one iteration per set of patterns    
        attr_it = itBegin;
        while( attr_it != itEnd )
        {
          string current_class = (*attr_it).first;
          vector<TeClassSample> samples = (*attr_it).second;
          for ( unsigned i = 0; i < samples.size(); i++ )
          {
            TeClassSample sample = samples[ i ];
*/
            
            // setting remaining values
            unsigned bit = 0;
            for ( unsigned d = 0; d < dimensions.size(); d++ )
            {
              char bits[ 10 ];
              dec2bin( (mypow( 2, params_.nbits_ ) - 1) * (long) sample[ dimensions[ d ] ], bits );
              for ( unsigned b = 0; b < ( params_.nbits_ - strlen( bits ) ); b++ )
                input_values( bit++, 0 ) = 0;
              for ( unsigned b = 0; b < strlen( bits ); b++ )
                input_values( bit++, 0 ) = (int) ( bits[ b ] == '1');
            }
            // setting bias
            input_values ( bit, 0 ) = -1;
/*
            // a single bit is activated per class
            for( unsigned b = 0; b < output_bits; b++ )
              output_train( b, 0 ) = 0;
            output_train( classes_int[ current_class ], 0 ) = 1;
*/
            // more than one bit activated per class
            bit = 0;
            for( unsigned b = 0; b < ( output_bits - strlen( classes_bit[ current_class ].c_str() ) ); b++ )
              output_train( bit++, 0 ) = 0;
            for( unsigned b = 0; b < strlen( classes_bit[ current_class ].c_str() ); b++ )
              output_train( bit++, 0 ) = (int) ( classes_bit[ current_class ][ b ] == '1'); 

/*cout << "input_values:" << endl;  
TeMatrix input_values_t;
input_values.Transpose( input_values_t );
input_values_t.Print();*/
          /* propagation */
            hidden_v = hidden_weights * input_values;
    /*cout << "v:" << endl;
    hidden_v.Print();*/
            for ( unsigned l = 0; l < (unsigned) hidden_v.Nrow(); l++ )
              hidden_v_bias( l, 0 ) = f( hidden_v( l, 0 ) );
            hidden_v_bias( hidden_v_bias.Nrow() - 1, 0 ) = -1;
    /*cout << "y:" << endl;
    hidden_v_bias.Print();*/
            output_v = output_weights * hidden_v_bias;

    /*cout << "v_output:" << endl;
    output_v.Print();*/
            for ( unsigned l = 0; l < (unsigned) output_v.Nrow(); l++ )
              output_v( l, 0 ) = f( output_v( l, 0 ) );
// cout << "y_output:" << endl;
// output_v.Print();
// cout << "output_train:" << endl;
// output_train.Print();

            /* updating weights */
            TeMatrix output_errors = output_train - output_v;
/*  cout << "output_errors:" << endl;
  output_errors.Print();*/
            TeMatrix mean_errors( output_errors );
            for ( unsigned l = 0; l < (unsigned) output_v.Nrow(); l++ )
              mean_errors( l, 0 ) = fabs( output_errors( l, 0 ) );
            TeMatrix mean_error = mean( mean_errors );
            
            for ( unsigned l = 0; l < (unsigned) error_window.Nrow() - 1; l++ )
              error_window( l, 0 ) = error_window( l + 1, 0 );
            error_window( error_window.Nrow() - 1, 0 ) = mean_error( 0, 0 );
            TeMatrix mean_error_window = mean( error_window );
            if ( mean_error_window( 0, 0 ) < 0.001 )
              epoch = params_.max_epochs_;
if ( errors_counter++ > 500 )            
{
  errors_counter = 0;
//   cout << mean_error( 0, 0 ) << " ";
//   cout << mean_error_window( 0, 0 ) << " ";
//   cerr << params_.eta_ << " ";
}
            TeMatrix d_output( output_errors );
            for ( unsigned l = 0; l < (unsigned) output_v.Nrow(); l++ )
              d_output( l, 0 ) = output_errors( l, 0 ) * df( output_v( l, 0 ) );
            TeMatrix hidden_v_bias_transposed;
            hidden_v_bias.Transpose( hidden_v_bias_transposed );
            output_weights = output_weights + params_.eta_ * d_output * hidden_v_bias_transposed;

    /*cout << "w_output: " << endl;
    output_weights.Print();*/
            TeMatrix mean_d_output ( mean( d_output ) );
            TeMatrix output_weights_transposed;
            output_weights.Transpose( output_weights_transposed );
            TeMatrix mean_mean_output_weights_transposed ( mean ( mean( output_weights_transposed ) ) );
            TeMatrix d_hidden;
            d_hidden.Init( hidden_v.Nrow(), 1, 0.0 );
    
    /*cout << "mean:" << mean_d_output( 0, 0 ) << endl;
    cout << "mean_mean_ " << mean_mean_output_weights_transposed( 0, 0 ) << endl;*/
            for ( unsigned l = 0; l < (unsigned) d_hidden.Nrow(); l++ )
            {
              d_hidden( l, 0 ) = mean_d_output( 0, 0 ) * mean_mean_output_weights_transposed( 0, 0 ) * df( hidden_v_bias( l, 0 ) );
    //         cout << df( hidden_v_bias( l, 0 ) ) << " " ;
            }
    /*cout << endl;      
    cout << "d: " << endl;
    d_hidden.Print();*/
    
            TeMatrix input_values_transposed;
            input_values.Transpose( input_values_transposed );
            for ( unsigned l = 0; l < (unsigned) hidden_weights.Nrow(); l++ )
              for( unsigned c = 0; c < (unsigned) hidden_weights.Ncol(); c++ )
                hidden_weights( l, c ) = hidden_weights( l, c ) + params_.eta_ * d_hidden( l, 0 ) * input_values_transposed( 0, c );
    // cout << "w_hidden: " << endl;
    // hidden_weights.Print();
//           }
//             cout << endl;
          }
        }
        ++attr_it;
      }
      // one iteration per small set of patterns
      actual_element += same_class_step;
//       cout << endl;
    }
//     status.Increment();
    // increasing eta (experimental)
//     params_.eta_ *= 1.008;
//     if ( params_.eta_ > 1 )
//       params_.eta_ = 1;
    params_.eta_ += increasing_eta;
  }
/*  cout << "];" << endl;
  cerr << "];" << endl;*/
  status.Toggle( false );
  
  #ifdef _DEBUG
  cout << "end train()" << endl;
  #endif

  return true;
}

bool nn_classifier::classify( vector<TeClassSample>::iterator itBegin, 
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
  
  /* Initializing neurons */
  
  // every input value are now resampled to the interval [0, 2^nbits_]
  // which corresponds to nbits_ bits, plus one bit, the bias
  unsigned input_bits = dimensions.size() * params_.nbits_ + 1;
  TeMatrix input_values;
  input_values.Init( input_bits, 1, 0.0 );
  TePDIPIManager status("Classifying data using NN...", total_elements , true);
  attr_it = itBegin;
  while ( attr_it != itEnd )
  {
    TeClassSample data = ( *attr_it );
    // setting remaining values
    unsigned bit = 0;
    for ( unsigned d = 0; d < dimensions.size(); d++ )
    {
      char bits[ 10 ];
      dec2bin( ( mypow( 2, params_.nbits_ ) - 1 ) * (long) data[ dimensions[ d ] ], bits );
      for ( unsigned b = 0; b < ( params_.nbits_ - strlen( bits ) ); b++ )
        input_values( bit++, 0 ) = 0;
      for ( unsigned b = 0; b < strlen( bits ); b++ )
        input_values( bit++, 0 ) = (int) ( bits[ b ] == '1');
    }
    input_values ( bit, 0 ) = -1;

/*cout << "input_values: " << endl;
TeMatrix input_values_t;
input_values.Transpose( input_values_t );
input_values_t.Print();*/
    /* propagation */
    hidden_v = hidden_weights * input_values;
// cout << "hidden_v:" << endl;
// hidden_v.Print();

    for ( unsigned l = 0; l < (unsigned) hidden_v.Nrow(); l++ )
      hidden_v_bias( l, 0 ) = f( hidden_v( l, 0 ) );
    hidden_v_bias( hidden_v_bias.Nrow() - 1, 0 ) = -1;
/*cout << "hidden_v_bias:" << endl;
hidden_v_bias.Print();*/
    output_v = output_weights * hidden_v_bias;
/*
    // a single bit is activated per class
    unsigned activated_class = 0;
    for ( unsigned l = 0; l < (unsigned) output_v.Nrow(); l++ )
    {
      output_v( l, 0 ) = f( output_v( l, 0 ) );
      if ( output_v( l, 0 ) > 0.5 )
        activated_class = l;
    }
    classifyResult.push_back( activated_class );
*/

    // more than one bit activated per class
    char bin_output[ 20 ];
    for ( unsigned l = 0; l < (unsigned) output_v.Nrow(); l++ )
    {
      output_v( l, 0 ) = f( output_v( l, 0 ) );
      bin_output[ l ] = ( output_v( l, 0 ) > 0.5 ? '1': '0' );
    }
    bin_output[ output_v.Nrow() ] = 0;
    classifyResult.push_back( bin2dec( bin_output ) );

/*cout << "output_v: " << endl;
output_v.Print();*/
// cout << "bin_output: " << bin_output << " and converted to int: " << bin2dec( bin_output ) << endl;

    status.Increment();
    
    ++attr_it;
  }
  status.Toggle( false );

  #ifdef _DEBUG
  cout << "end classify()" << endl;
  #endif
  
  return true;
}