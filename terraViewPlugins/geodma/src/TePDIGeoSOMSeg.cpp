//   cd /home/tkorting/terralib_devel/terralibx/image_processing; make; cd /home/tkorting/terralib_devel/examples/image_processing/source/TePDISegmentation; qmake; make clean; make; cd /home/tkorting/terralib_devel/examples/image_processing/bin

#include <TePDIGeoSOMSeg.hpp>
#include <TePDIRaster2Vector.hpp>
#include <TePDIUtils.hpp>
#include <TePDIMorfFilter.hpp>
#include <TePDIFilterMask.hpp>
#include <TeBufferRegion.h>
#include <TePDIStatistic.hpp>

using namespace std;


/**********************************************************************************/
/* TerraLib main implementation for SOMSeg Segmentation                            */
/**********************************************************************************/

TePDIGeoSOMSeg::TePDIGeoSOMSeg()
{
}

bool TePDIGeoSOMSeg::CheckParameters( const TePDIParameters& parameters ) const
{
  // checking input_image
  TePDITypes::TePDIRasterPtrType input_image;
	TEAGN_TRUE_OR_RETURN( parameters.GetParameter( "input_image", input_image ),
		"Missing parameter: input_image" );
	TEAGN_TRUE_OR_RETURN( input_image.isActive(),
		"Invalid parameter: input_image inactive" );
	TEAGN_TRUE_OR_RETURN( input_image->params().status_ != TeRasterParams::TeNotReady,
		"Invalid parameter: input_image not ready" );
  vector<unsigned> input_bands;
  TEAGN_TRUE_OR_RETURN(parameters.GetParameter( "input_bands", input_bands ),
    "Missing parameter: input_bands");
  TEAGN_TRUE_OR_RETURN( input_bands.size() <= ( unsigned ) input_image->nBands(),
    "More input bands then bands in input image" );
  
  // checking output_image
  TePDITypes::TePDIRasterPtrType output_image;
  TEAGN_TRUE_OR_RETURN( parameters.GetParameter( "output_image", output_image ),
    "Missing parameter: output_image" );

  // checking filter_string
  string filter_string;
  TEAGN_TRUE_OR_RETURN( parameters.GetParameter( "filter_string", filter_string ),
    "Missing parameter: filter_string" );
/*
	// checking thresholds
  int neurons;
  TEAGN_TRUE_OR_RETURN( parameters.GetParameter( "neurons", neurons ),
    "Missing parameter: neurons" );
  TEAGN_TRUE_OR_RETURN( neurons > 0,
    "Parameter neurons is > 0" );
    */
  int min_area;
  TEAGN_TRUE_OR_RETURN( parameters.GetParameter( "min_area", min_area ),
    "Missing parameter: min_area" );
  TEAGN_TRUE_OR_RETURN( min_area >= 0,
    "Parameter min_area is >= 0" );
  int max_epochs;
  TEAGN_TRUE_OR_RETURN( parameters.GetParameter( "max_epochs", max_epochs ),
    "Missing parameter: max_epochs" );
  TEAGN_TRUE_OR_RETURN( max_epochs > 0,
    "Parameter max_epochs is > 0" );
  unsigned total_coordinates;
  TEAGN_TRUE_OR_RETURN( parameters.GetParameter( "total_coordinates", total_coordinates ),
    "Missing parameter: total_coordinates" );
  float radius;
  TEAGN_TRUE_OR_RETURN( parameters.GetParameter( "radius", radius ),
    "Missing parameter: radius" );
  float decreasing;
  TEAGN_TRUE_OR_RETURN( parameters.GetParameter( "decreasing", decreasing ),
    "Missing parameter: decreasing" );
  float alpha;
  TEAGN_TRUE_OR_RETURN( parameters.GetParameter( "alpha", alpha ),
    "Missing parameter: alpha" );
  float k;
  TEAGN_TRUE_OR_RETURN( parameters.GetParameter( "k", k ),
    "Missing parameter: k" );
  bool fixed_units;
  TEAGN_TRUE_OR_RETURN( parameters.GetParameter( "fixed_units", fixed_units ),
    "Missing parameter: fixed_units" );
    
  // checking output polygon sets parameter
  if( parameters.CheckParameter< TePDITypes::TePDIPolSetMapPtrType >( 
    "output_polsets" ) ) 
  {    

    TePDITypes::TePDIPolSetMapPtrType output_polsets;
    parameters.GetParameter( "output_polsets", output_polsets );
            
    TEAGN_TRUE_OR_RETURN( output_polsets.isActive(),
      "Invalid parameter output_polsets : Inactive polygon set pointer" );   
  }
	return true;
}

void TePDIGeoSOMSeg::ResetState( const TePDIParameters& )
{
}

vector<unsigned> shuffle_vector( unsigned N )
{
  vector<unsigned> r;
  
  // vector in correct order
  for ( unsigned i = 0; i < N; i++ )
    r.push_back( i );
  
  // shuffle vector
  for ( unsigned i = 0; i < ( N - 1 ); i++ )
  {
    unsigned new_pos = i + ( rand() % ( N - i ) );
    unsigned tmp = r[ i ];
    r[ i ] = r[ new_pos ];
    r[ new_pos ] = tmp;
  }
  
  return r;
}
     
bool TePDIGeoSOMSeg::RunImplementation()
{
  TePDITypes::TePDIRasterPtrType input_image;
	params_.GetParameter( "input_image", input_image );

  vector<unsigned> input_bands;
  params_.GetParameter( "input_bands", input_bands );
  
  unsigned total_coordinates;
  params_.GetParameter( "total_coordinates", total_coordinates );
  
  /* retrieve input_image parameters */
	unsigned H = input_image->params().nlines_,
           W = input_image->params().ncols_,
           B = input_bands.size();
  
  /* normalizing attributes */
  vector<TeMatrix> normal_raster_matrix;
  for ( unsigned b = 0; b < B; b++ )
  {
    TeMatrix tmp_matrix;
    tmp_matrix.Init( H, W, 0.0 );
    normal_raster_matrix.push_back( tmp_matrix );
  }
  TePDIParameters statistic_params;
  TePDITypes::TePDIRasterVectorType rasters;
  vector< int > bands;
  for ( unsigned b = 0; b < B; b++ )
  {
    rasters.push_back( input_image );
    bands.push_back( input_bands[ b ] );
  }
  statistic_params.SetParameter( "rasters", rasters );
  statistic_params.SetParameter( "bands", bands );
  TeBox box = input_image->params().boundingBox();
  TePolygon pol = polygonFromBox( box );
  TePDITypes::TePDIPolygonSetPtrType polset( new TePolygonSet );
  polset->add( pol );
  statistic_params.SetParameter( "polygonset", polset );
  TePDIStatistic statistic;
  TEAGN_TRUE_OR_THROW( statistic.Reset( statistic_params ), "Statistic Reset error" );
  vector<double> mean, stddev;
  for ( unsigned b = 0; b < B; b++ )
  {
    mean.push_back( statistic.getMean( input_bands[ b ] ) );
    stddev.push_back( statistic.getStdDev( input_bands[ b ] ) );
  }
  for ( unsigned lin = 0; lin < H; lin++ )
    for ( unsigned col = 0; col < W; col++ )
    {
      for ( unsigned b = 0; b < B; b++ )
      {
        double pixel;
        input_image->getElement( col, lin, pixel, input_bands[ b ] );
        normal_raster_matrix[ input_bands[ b ] ]( lin, col ) = ( pixel - mean[ b ] ) / stddev[ b ];
      }
    }

  srand((unsigned) time(0));
  vector<unsigned> shuffled_lines = shuffle_vector( H );
  vector<unsigned> shuffled_columns = shuffle_vector( W );
  /* Getting Training Data, in random order. And Input Data, in correct order. */
  TeClassSampleSet training_samples;
  vector<TeClassSample> input_attributes;
  unsigned training_step = 0;
  for ( unsigned l = 0; l < H; l++ )
    for ( unsigned c = 0; c < W; c++ )
    {
      int lin = (int) shuffled_lines[ l ];
      int col = (int) shuffled_columns[ c ];
      TeClassSample training_sample;
      TeClassSample input_sample;
      for ( unsigned b = 0; b < B; b++ )
      {
        double pixel;
        double pixel_right,
               pixel_left,
               pixel_top,
               pixel_bottom;
        double pixel_rt,
               pixel_rb,
               pixel_lt,
               pixel_lb;
        double mean_cross = 0.0;
        double mean_x = 0.0;
        
        double input_pixel;
        double input_pixel_right,
               input_pixel_left,
               input_pixel_top,
               input_pixel_bottom;
        double input_mean_cross = 0.0;
        
        // get center pixel, training
        pixel = normal_raster_matrix[ input_bands[ b ] ]( lin, col );
        training_sample.push_back( pixel );
        // get center pixel, input
        input_pixel = normal_raster_matrix[ input_bands[ b ] ]( l, c );
        input_sample.push_back( input_pixel );
        
        unsigned col_p1 = col + 1,
                 lin_p1 = lin + 1;
        int col_m1 = col - 1,
            lin_m1 = lin - 1;
        if ( col_p1 >= W )
          col_p1 = W - 1;
        if ( col_m1 < 0 )
          col_m1 = 0;
        if ( lin_p1 >= H )
          lin_p1 = H - 1;
        if ( lin_m1 < 0 )
          lin_m1 = 0;
        
        unsigned input_c_p1 = c + 1,
                 input_l_p1 = l + 1;
        int input_c_m1 = c - 1,
            input_l_m1 = l - 1;
        if ( input_c_p1 >= W )
          input_c_p1 = W - 1;
        if ( input_c_m1 < 0 )
          input_c_m1 = 0;
        if ( input_l_p1 >= H )
          input_l_p1 = H - 1;
        if ( input_l_m1 < 0 )
          input_l_m1 = 0;
        
        // get neighbors in cross
        //input_image->getElement( col + 1, lin, pixel_right, input_bands[ b ] );
        //input_image->getElement( col - 1, lin, pixel_left, input_bands[ b ] );
        //input_image->getElement( col, lin - 1, pixel_top, input_bands[ b ] );
        //input_image->getElement( col, lin + 1, pixel_bottom, input_bands[ b ] );
        pixel_right = normal_raster_matrix[ input_bands[ b ] ]( lin, col_p1 );
        pixel_left = normal_raster_matrix[ input_bands[ b ] ]( lin, col_m1 );
        pixel_top = normal_raster_matrix[ input_bands[ b ] ]( lin_m1, col );
        pixel_bottom = normal_raster_matrix[ input_bands[ b ] ]( lin_p1, col );

        mean_cross = ( pixel_right + pixel_left + pixel_top + pixel_bottom ) / 4;
//         training_sample.push_back( mean_cross );
        
        input_pixel_right = normal_raster_matrix[ input_bands[ b ] ]( l, input_c_p1 );
        input_pixel_left = normal_raster_matrix[ input_bands[ b ] ]( l, input_c_m1 );
        input_pixel_top = normal_raster_matrix[ input_bands[ b ] ]( input_l_m1, c );
        input_pixel_bottom = normal_raster_matrix[ input_bands[ b ] ]( input_l_p1, c );

        input_mean_cross = ( input_pixel_right + input_pixel_left + input_pixel_top + input_pixel_bottom ) / 4;
//         input_sample.push_back( input_mean_cross );

        // get neighbors in x
        //input_image->getElement( col - 1, lin - 1, pixel_lt, input_bands[ b ] );
        //input_image->getElement( col + 1, lin - 1, pixel_rt, input_bands[ b ] );
        //input_image->getElement( col - 1, lin + 1, pixel_lb, input_bands[ b ] );
        //input_image->getElement( col + 1, lin + 1, pixel_rb, input_bands[ b ] );
        pixel_lt = normal_raster_matrix[ input_bands[ b ] ]( lin_m1, col_m1 );
        pixel_rt = normal_raster_matrix[ input_bands[ b ] ]( lin_m1, col_p1 );
        pixel_lb = normal_raster_matrix[ input_bands[ b ] ]( lin_p1, col_m1 );
        pixel_rb = normal_raster_matrix[ input_bands[ b ] ]( lin_p1, col_p1 );

        mean_x = ( pixel_lt + pixel_rt + pixel_lb + pixel_rb ) / 4;
//         training_sample.push_back( mean_x );
        
        // textures
//         training_sample.push_back( pixel_right - pixel );
//         training_sample.push_back( pixel_rb - pixel );
//         training_sample.push_back( pixel_bottom - pixel );
//         training_sample.push_back( pixel_lb - pixel );
//         training_sample.push_back( pixel_left - pixel );
//         training_sample.push_back( pixel_lt - pixel );
//         training_sample.push_back( pixel_top - pixel );
        
      }
      
      // insert coordinates as attributes, normalized in the interval [0, 1]
      if ( total_coordinates > 0 )
      {
        training_sample.push_back( (double) ( col + 0.5 ) / W );
        training_sample.push_back( (double) ( lin + 0.5 ) / H );
        input_sample.push_back( (double) ( c + 0.5 ) / W );
        input_sample.push_back( (double) ( l + 0.5 ) / H );
      }
      if ( training_step++ < ( W * H / 100 ) )
        training_samples[ "pixels" ].push_back( training_sample );
      input_attributes.push_back( input_sample );
    }

  float radius;
  params_.GetParameter( "radius", radius );
  float decreasing;
  params_.GetParameter( "decreasing", decreasing );
  float alpha;
  params_.GetParameter( "alpha", alpha );
  float k;
  params_.GetParameter( "k", k );
  bool fixed_units;
  params_.GetParameter( "fixed_units", fixed_units );
  /* SOM parameters */
  vector<int> clas_dim;
  for ( unsigned b = 0; b < training_samples[ "pixels" ][ 0 ].size(); b++ )
    clas_dim.push_back( b );
  int max_epochs;
  params_.GetParameter( "max_epochs", max_epochs );
//   float radius = 0.3;
//   float decreasing = 0.8;
//   float alpha = 0.45;
//   double k = 0.8;
//   bool fixed_units = false;
  /* retrieve algorithm parameters */
  unsigned map_width;
  params_.GetParameter( "neurons_w", map_width );
  unsigned map_height;
  params_.GetParameter( "neurons_h", map_height );
  string filter_string;
  params_.GetParameter( "filter_string", filter_string );
  int min_area;
  params_.GetParameter( "min_area", min_area );

  cout << "neurons_h: " << map_height << endl << "neurons_w: " << map_width << endl << "max_epochs: " << max_epochs << endl << "filter_string: " << filter_string << endl << "min_area: " << min_area << endl << "radius: " << radius << endl << "decreasing: " << decreasing << endl << "alpha: " << alpha << endl << "k: " << k << endl << "fixed_units: " << fixed_units << endl;

  /* SOM classifier, training and classification */ 
  list<int> pol_classes;
  TeDMSOMClassifier som_classify( TeDMSOMClassifierParams( clas_dim.size(), map_width, 
                                                           map_height, alpha, 
                                                           radius, decreasing, 
                                                           max_epochs, total_coordinates,
                                                           k, fixed_units) );
  som_classify.train( training_samples.begin(), training_samples.end(), clas_dim );
  som_classify.classify( input_attributes.begin(), input_attributes.end(), clas_dim, pol_classes );

	/* write results */
	TePDITypes::TePDIRasterPtrType raster_to_filter;
  TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( raster_to_filter,
    1, 1, 1, false, TeUNSIGNEDLONG, 0 ), "RAM Raster Alloc error" );
	TeRasterParams raster_to_filter_params = raster_to_filter->params();
	raster_to_filter_params.nBands( 1 );
	raster_to_filter_params.setDataType( TeUNSIGNEDLONG );
  raster_to_filter_params.boundingBoxLinesColumns( 
    input_image->params().boundingBox().x1(),
    input_image->params().boundingBox().y1(),
    input_image->params().boundingBox().x2(),
    input_image->params().boundingBox().y2(),
    input_image->params().nlines_ + 4,
    input_image->params().ncols_ + 4 );
  raster_to_filter_params.projection( input_image->params().projection() );
	TEAGN_TRUE_OR_RETURN( raster_to_filter->init( raster_to_filter_params ),
    "Error initiating output classification image" );
    
  list<int>::iterator list_it = pol_classes.begin();
  unsigned col = 0;
  unsigned lin = 0;
  while ( list_it != pol_classes.end() )
  {
    raster_to_filter->setElement( col + 2, lin + 2, *list_it );
    col++;
    if ( col == W )
    {
      col = 0;
      lin++;
    }
    ++list_it;
  }

  /* Filtering output, to remove spurious pixels */
  vector<int> channels;
  channels.push_back( 0 );

  for ( unsigned i = 0; i < filter_string.length(); i++ )
  {
    TePDIParameters filter_params;
    filter_params.SetParameter( "input_image", raster_to_filter );
    TePDITypes::TePDIRasterPtrType output_filtered_image;
    TEAGN_TRUE_OR_THROW( TePDIUtils::TeAllocRAMRaster( output_filtered_image,
      1, 1, 1, false, TeUNSIGNEDLONG, 0 ), "RAM Raster Alloc error" );
    filter_params.SetParameter( "output_image", output_filtered_image );
    filter_params.SetParameter( "channels", channels );
    filter_params.SetParameter( "iterations", (int) 1 );
    filter_params.SetParameter( "filter_mask", TePDIFilterMask::create_MorfMtot() );
    char filter_char = filter_string[ i ];
    if ( filter_char == 'M' )
      filter_params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMMedianType );
    else if ( filter_char == 'E' )
      filter_params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMErosionType );
    else if ( filter_char == 'D' )
      filter_params.SetParameter( "filter_type", TePDIMorfFilter::TePDIMDilationType );
    else
      continue;
    
    TePDIMorfFilter filter;
    TEAGN_TRUE_OR_THROW( filter.Reset( filter_params ), "Invalid Parameters" );
    TEAGN_TRUE_OR_THROW( filter.Apply(), "Apply error" );
    
    for ( unsigned lin = 0; lin < (unsigned) raster_to_filter->params().nlines_; lin++ )
      for ( unsigned col = 0; col < (unsigned) raster_to_filter->params().ncols_; col++ )
      {
        double pixel;
        output_filtered_image->getElement( col, lin, pixel );
        raster_to_filter->setElement( col, lin, pixel );
      }
  }
    
  /* saving resultant output image */
  TePDITypes::TePDIRasterPtrType output_image;
  params_.GetParameter( "output_image", output_image );
  TeRasterParams output_image_params = output_image->params();
  output_image_params.nBands( 1 );
  output_image_params.setDataType( TeUNSIGNEDLONG );
  output_image_params.boundingBoxLinesColumns( 
    input_image->params().boundingBox().x1(),
    input_image->params().boundingBox().y1(),
    input_image->params().boundingBox().x2(),
    input_image->params().boundingBox().y2(),
    input_image->params().nlines_,
    input_image->params().ncols_);
  output_image_params.projection( input_image->params().projection() );
  TEAGN_TRUE_OR_RETURN( output_image->init( output_image_params ),
    "Error initiating output classification image" );

  for ( unsigned lin = 0; lin < H; lin++ )
    for ( unsigned col = 0; col < W; col++ )
    {
      double pixel;
      raster_to_filter->getElement( col + 2, lin + 2, pixel );
      output_image->setElement( col, lin, pixel );
    }

  /* generate output polygonset, merge polygons above min_area */
  unsigned limit_merges = 20;
  unsigned total_merges = 0;
  unsigned previous_merges = 0;
  for ( unsigned i = 0; i < limit_merges; i++ )
  {
    cout << "iteration " << i << endl;
    TePDITypes::TePDIPolSetMapPtrType output_polsets;
    if( params_.CheckParameter< TePDITypes::TePDIPolSetMapPtrType >( 
        "output_polsets" ) ) 
      params_.GetParameter( "output_polsets", output_polsets );  
    else
      output_polsets.reset( new TePDITypes::TePDIPolSetMapType );

    TePDIParameters params_output;
    params_output.SetParameter( "rotulated_image", output_image );
    params_output.SetParameter( "channel", (unsigned) 0 );
    params_output.SetParameter( "output_polsets", output_polsets );

    TePDIRaster2Vector raster2Vector;  
    TEAGN_TRUE_OR_THROW( raster2Vector.Reset( params_output ),
      "Invalid Parameters for raster2Vector" );
    TEAGN_TRUE_OR_THROW( raster2Vector.Apply(), 
      "Apply error" );
      
    TePDITypes::TePDIPolSetMapType::iterator it = output_polsets->begin();
    TePDITypes::TePDIPolSetMapType::iterator it_end = output_polsets->end();  
    TePolygonSet pols;
    while( it != it_end ) 
    {
      pols.copyElements( it->second );
      ++it;
    }
    double small_pixel;
    double big_pixel;
    double pol_area;
    total_merges = 0;
    for ( unsigned p = 0; p < pols.size() - 1; p++ )
    {
      pol_area = TeGeometryArea( pols[ p ] );
      if ( pol_area < min_area )
      {
        total_merges++;
//         cout << p << "-" ;
        TeRaster::iteratorPoly it_pol = output_image->begin( pols[ p ], TeBoxPixelIn );
        unsigned c = it_pol.currentColumn();
        unsigned l = it_pol.currentLine();
        output_image->getElement( c, l, small_pixel );
//         cout << " from " << small_pixel;
        output_image->getElement( c + it_pol.nColsInPoly(), l + it_pol.nLinesInPoly(), big_pixel );
        if ( big_pixel != small_pixel )
          output_image->getElement( c, l + it_pol.nLinesInPoly(), big_pixel );
        if ( big_pixel != small_pixel )
          output_image->getElement( c + it_pol.nColsInPoly(), l, big_pixel );
        if ( big_pixel != small_pixel )
          output_image->getElement( c - 1, l, big_pixel );
        if ( big_pixel != small_pixel )
          output_image->getElement( c, l - 1, big_pixel );
//         cout << " to " << big_pixel << ",";
        while( it_pol != output_image->end( pols[ p ], TeBoxPixelIn ) )
        {
          output_image->setElement( it_pol.currentColumn(), it_pol.currentLine(), big_pixel );
          ++it_pol;
        }
        /*
        output_image->getElement( c, l, pixel );
        cout << "inside pixel: " << pixel << endl;
        output_image->getElement( c + it_pol.nColsInPoly(), l + it_pol.nLinesInPoly(), pixel );
        cout << "outside pixel 1: " << pixel << endl;
        output_image->getElement( c, l + it_pol.nLinesInPoly(), pixel );
        cout << "outside pixel 2: " << pixel << endl;
        output_image->getElement( c + it_pol.nColsInPoly(), l, pixel );
        cout << "outside pixel 3: " << pixel << endl;
        while( it_pol != output_image->end( pols[ p ], TeBoxPixelIn ) )
        {
          output_image->getElement( it_pol.currentColumn(), it_pol.currentLine(), pixel );
          cout << pixel << " ";
          ++it_pol;
        }
        cout << endl;
        
        it_pol = output_image->begin( pols[ p + 1 ], TeBoxPixelIn );
        while( it_pol != output_image->end( pols[ p + 1 ], TeBoxPixelIn ) )
        {
          output_image->getElement( it_pol.currentColumn(), it_pol.currentLine(), pixel );
          cout << pixel << " ";
          ++it_pol;
        }
        cout << endl;*/
      }
    }
    if ( total_merges == previous_merges )
      i = limit_merges;
    cout << "total_merges: " << total_merges << " previous_merges: " << previous_merges << endl;
    previous_merges = total_merges;
  }
  return true;
}

/**********************************************************************************/
/* TerraLib main implementation for SOM parameters                                */
/**********************************************************************************/

TeDMSOMClassifierParams& TeDMSOMClassifierParams::operator=(const TeDMSOMClassifierParams& p)
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
    total_coordinates_ = p.total_coordinates_;
    k_ = p.k_;
    fixed_units_ = p.fixed_units_;
  }
  return *this;
}

TeDMSOMClassifierParams::~TeDMSOMClassifierParams()
{
  #ifdef _DEBUG
  cout << "~TeDMSOMClassifierParams()" << endl;
  #endif
}

TeDMSOMClassifierParams::TeDMSOMClassifierParams( const unsigned& ta, const unsigned& ww, const unsigned& wh, const double& al, 
                                                  const double& ra, const double& dr, const unsigned& me, const unsigned& tc,
                                                  const double& k, const bool& fu ) : 
                                                  total_attributes_( ta ), w_width_( ww ), w_height_( wh ), alpha_( al ), 
                                                  radius_( ra ), decreasing_rate_( dr ), max_epochs_( me ), total_coordinates_( tc ),
                                                  k_( k ), fixed_units_( fu )
{
  #ifdef _DEBUG
  cout << "begin TeDMSOMClassifierParams()" << endl;
  #endif

  TEAGN_TRUE_OR_THROW( alpha_ >= 0 && alpha_ <= 1, "Alpha parameter should be [0, 1]" );
  TEAGN_TRUE_OR_THROW( radius_ > 0 , "Radius of influence must be > 0" );
  TEAGN_TRUE_OR_THROW( total_attributes_ > 0 , "Number of attributes must be > 0" );
  TEAGN_TRUE_OR_THROW( max_epochs_ > 0 , "Maximum of epochs must be > 0" );

  srand((unsigned) time(0));
  for ( unsigned i = 0; i < total_attributes_ - total_coordinates_; i++ )
  {
    TeMatrix tmp_matrix;
    tmp_matrix.Init( w_width_, w_height_, 0.0 );
    for ( unsigned w = 0; w < w_width_; w++ )
      for ( unsigned h = 0; h < w_height_; h++ )
        tmp_matrix( w, h ) = ( rand() / (double) RAND_MAX );
    weights_.push_back( tmp_matrix );
  }
  TeMatrix horizontal;
  horizontal.Init( w_width_, w_height_, 0.0 );
  TeMatrix vertical;
  vertical.Init( w_width_, w_height_, 0.0 );
  for ( unsigned w = 0; w < w_width_; w++ )
    for ( unsigned h = 0; h < w_height_; h++ )
    {
      horizontal( w, h ) = (double) ( w + 0.5 ) / w_width_;
      vertical( w, h ) = (double) ( h + 0.5 ) / w_height_;
    }
  weights_.push_back( horizontal );
  weights_.push_back( vertical );

  #ifdef _DEBUG
  cout << "end TeDMSOMClassifierParams()" << endl;
  #endif
}

double TeDMSOMClassifierParams::h( vector<unsigned> w1, vector<unsigned> w2 )
{
  double r = 0.0;
  double tmp;

  if ( w1.size() == 0 || w1.size() != w2.size() )
    return 0.0;

  for ( unsigned i = 0; i < w1.size(); i++ )
  {
    tmp = (double) w1[ i ] - (double) w2[ i ];
    r += tmp * tmp;
  }

  return exp( -0.5 * sqrt( r / radius_ ) );
}

double TeDMSOMClassifierParams::get_distance( vector<double> a, vector<double> b )
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

string to_string( unsigned a, unsigned b )
{
  return string( Te2String(a) + Te2String(b) );
}

bool present( string q, vector<string> string_set )
{
  for ( unsigned i = 0; i < string_set.size(); i++ )
    if ( string_set[ i ] == q )
      return true;
  return false;
}

/**********************************************************************************/
/* TerraLib main implementation for SOM classifier                                */
/**********************************************************************************/

template<typename trainingPatternSetIterator> bool TeDMSOMClassifier::train( trainingPatternSetIterator itBegin, 
                                                                             trainingPatternSetIterator itEnd, 
                                                                             const vector<int> dimensions )
{
  #ifdef _DEBUG
  cout << "begin train()" << endl;
  #endif

  unsigned current_iteration = 0;
  unsigned winner_i = 0;
  unsigned winner_j = 0;
  double winner_d = INF;

// unsigned print_flag = 0;
unsigned time = 1900;
unsigned neuron_id = 0;
cout << "ID, TIME, x, y," << endl;
for ( unsigned i = 0; i < params_.w_width_; i++ )
{
  for ( unsigned j = 0; j < params_.w_height_; j++ )
  {
    cout << neuron_id++ << ", " << time << ", ";
    for ( unsigned a = params_.total_attributes_ - params_.total_coordinates_; a < params_.total_attributes_; a++ )
      cout << params_.weights_[ a ]( i, j ) << ", ";
    cout << endl;
  }
}
time++;

  TePDIPIManager status( "Training SOM...", params_.max_epochs_, true );
  while ( ( params_.alpha_ >= 0.0001 || params_.radius_ >= 0.0001 ) && current_iteration < params_.max_epochs_ )
  {
//     cout << endl << "=================== " << current_iteration << " ===================" << endl << endl;
    status.Increment();
    current_iteration++;
    trainingPatternSetIterator attr_it = itBegin;

    while ( attr_it != itEnd )
    {
      vector<TeClassSample> vector_sample = (*attr_it).second;
      vector<TeClassSample>::iterator data_it = vector_sample.begin();
      while ( data_it != vector_sample.end() )
      {
        TeClassSample xk_geo;
        for ( unsigned i = dimensions.size() - params_.total_coordinates_; i < dimensions.size(); i++ )
          xk_geo.push_back( (*data_it)[ dimensions[ i ] ] );
        double winnergeo_d = INF;
        unsigned winnergeo_i = 0;
        unsigned winnergeo_j = 0;
        
// cout << endl << "X : " << xk_geo[0] << ", " << xk_geo[1] << endl;
    
        // find w_winnergeo
        vector<double> w_winnergeo;
        for ( unsigned i = 0; i < params_.w_width_; i++ )
          for ( unsigned j = 0; j < params_.w_height_; j++ )
          {
            vector<double> wij_geo;
            double d = 0.0;
            for ( unsigned a = params_.total_attributes_ - params_.total_coordinates_; a < params_.total_attributes_; a++ )
              wij_geo.push_back( params_.weights_[ a ]( i, j ) );
            d = params_.get_distance( xk_geo, wij_geo );

            if ( d < winnergeo_d )
            {
              winnergeo_i = i;
              winnergeo_j = j;
              winnergeo_d = d;
              w_winnergeo = wij_geo;
            }
          }
// cout << "w_winnergeo: " << w_winnergeo[0] << ", " << w_winnergeo[1] << endl;

        vector<string> W_winners;
        for ( unsigned i = 0; i < params_.w_width_; i++ )
          for ( unsigned j = 0; j < params_.w_height_; j++ )
          {
            vector<double> wij_geo;
            double d = 0.0;
            for ( unsigned a = params_.total_attributes_ - params_.total_coordinates_; a < params_.total_attributes_; a++ )
              wij_geo.push_back( params_.weights_[ a ]( i, j ) );
            d = params_.get_distance( w_winnergeo, wij_geo );
// cout << "  wij_geo: " << wij_geo[0] << ", " << wij_geo[1] << ": " << d << endl;

            if ( d <= params_.k_ )
              W_winners.push_back( to_string( i, j ) );
          }        

// cout << "total of W_winners: " << W_winners.size() << endl;
// for (unsigned i = 0; i < W_winners.size(); i++)
//   cout << "   " << W_winners[i] << endl;

        TeClassSample xk;
        winner_i = 0;
        winner_j = 0;
        winner_d = INF;
        for (unsigned i = 0; i < dimensions.size() - params_.total_coordinates_; i++)
          xk.push_back( (*data_it)[ dimensions[ i ] ] );
        
        // find w_winner
        vector<double> w_winner;
        for ( unsigned i = 0; i < params_.w_width_; i++ )
          for ( unsigned j = 0; j < params_.w_height_; j++ )
          {
            if ( !present( to_string( i, j ), W_winners ) )
              continue;

            vector<double> wij;
            double d = 0.0;
            for ( unsigned a = 0; a < params_.total_attributes_ - params_.total_coordinates_; a++ )
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
/*cout << "xk" << endl;          
for ( unsigned i = 0; i < xk.size(); i++)
  cout << xk[i] << " ";
cout << endl;
cout << "w_winner  " << winner_i << ", " << winner_j << endl;          
for ( unsigned i = 0; i < w_winner.size(); i++)
  cout << w_winner[i] << " ";
cout << endl;*/
          
        xk.clear();
        for (unsigned i = 0; i < dimensions.size(); i++)
          xk.push_back( (*data_it)[ dimensions[ i ] ] );
        
        vector<unsigned> winner_ij;
        winner_ij.push_back( winner_i );
        winner_ij.push_back( winner_j );
        // update weights based on w_winner
        for ( unsigned i = 0; i < params_.w_width_; i++ )
          for ( unsigned j = 0; j < params_.w_height_; j++ )
          {
/*            cout << "   updating neuron " << i << ", " << j << endl;
            cout << "      updating neuron " << params_.weights_[ params_.total_attributes_ - 2 ]( i, j ) << ", ";
            cout << params_.weights_[ params_.total_attributes_ - 1 ]( i, j ) << endl;*/
            vector<unsigned> ij;
            ij.push_back( i );
            ij.push_back( j );

            double H = params_.h( ij, winner_ij );
            unsigned weights_to_atualize = params_.total_attributes_;
/*            cout << "      H: " << H << endl;
            cout << "      weights_to_atualize: " << weights_to_atualize << endl;*/
            if ( params_.fixed_units_ )
              weights_to_atualize -= params_.total_coordinates_;
            for ( unsigned a = 0; a < weights_to_atualize; a++ )
              params_.weights_[ a ]( i, j ) = params_.weights_[ a ]( i, j ) + 
                                              params_.alpha_ * H * ( xk[ a ] - params_.weights_[ a ]( i, j ) );
          }
/*
cout << "==== W ====" << endl;
for ( unsigned i = 0; i < params_.w_width_; i++ )
{
  for ( unsigned j = 0; j < params_.w_height_; j++ )
  {
    cout << params_.weights_[ params_.total_attributes_ - 2 ]( i, j ) << ", ";
    cout << params_.weights_[ params_.total_attributes_ - 1 ]( i, j ) << "  ";
  }
  cout << endl;
}*/

// {
//   neuron_id = 0;
//   for ( unsigned i = 0; i < params_.w_width_; i++ )
//   {
//     for ( unsigned j = 0; j < params_.w_height_; j++ )
//     {
//       cout << neuron_id++ << ", " << time << ", ";
//       cout << params_.weights_[ params_.total_attributes_ - 2 ]( i, j ) << ", ";
//       cout << params_.weights_[ params_.total_attributes_ - 1 ]( i, j ) << endl;
//     }
//   }
//   time++;
// }

        ++data_it;
      }

  neuron_id = 0;
for ( unsigned i = 0; i < params_.w_width_; i++ )
{
  for ( unsigned j = 0; j < params_.w_height_; j++ )
  {
    cout << neuron_id++ << ", " << time << ", ";
    for ( unsigned a = params_.total_attributes_ - params_.total_coordinates_; a < params_.total_attributes_; a++ )
      cout << params_.weights_[ a ]( i, j ) << ", ";
    cout << endl;
  }
}
time++;

      params_.alpha_ = params_.alpha_ * params_.decreasing_rate_;
      params_.radius_ = params_.radius_ * params_.decreasing_rate_;
      ++attr_it;
    }
  }
  status.Toggle( false );

 /* for ( unsigned i = 0; i < params_.w_width_; i++ )
  {
    for ( unsigned j = 0; j < params_.w_height_; j++ )
    {
      cout << "neuron " << i << ", " << j << ": ";
      for (unsigned k = 0; k < params_.total_attributes_; k++ )
        cout << params_.weights_[k](i,j) << " ";
      cout << endl;
    }
  }*/
  
  #ifdef _DEBUG
  cout << "end train()" << endl;
  #endif

  return true;
}

template<typename patternSetIterator> bool TeDMSOMClassifier::classify( patternSetIterator itBegin, 
                                                                        patternSetIterator itEnd, 
                                                                        const vector<int> dimensions,
                                                                        list<int>& classifyResult )
{
  #ifdef _DEBUG
  cout << "begin classify()" << endl;
  #endif

  patternSetIterator attr_it = itBegin;
  unsigned total_elements = 0;
  while ( attr_it != itEnd )
  {
    total_elements++;
    ++attr_it;
  }

  unsigned for_begin = 0;
  unsigned for_end = params_.total_attributes_;
  
//   if ( params_.fixed_units_ )
//     for_end -= params_.total_coordinates_;
//   else
//     for_begin = params_.total_attributes_ - params_.total_coordinates_;

  unsigned position = 0;
  map<string, vector<double> > map_weights;
  map<unsigned, string> map_positions;
  for ( unsigned i = 0; i < params_.w_width_; i++ )
    for ( unsigned j = 0; j < params_.w_height_; j++ )
    {
      vector<double> weight_vector;
      for ( unsigned a = params_.total_attributes_ - params_.total_coordinates_; a < params_.total_attributes_; a++ )
        weight_vector.push_back( params_.weights_[ a ]( i, j ) );
      map_weights[ to_string( i, j ) ] = weight_vector;
      map_positions[ position++ ] = to_string( i, j );
    }
  TeMatrix distances;
  distances.Init( position, position, 0.0 );
  for ( unsigned i = 0; i < position; i++ )
    for ( unsigned j = 0; j < position; j++ )
    {
      vector<double> w1, w2;
      w1 = map_weights[ map_positions[ i ] ];
      w2 = map_weights[ map_positions[ j ] ];
      distances( i, j ) = params_.get_distance( w1, w2 );
    }
  distances.Print();
  
  map<string, int> class_map;
  int class_index = 0;
  for ( unsigned i = 0; i < params_.w_width_; i++ )
    for ( unsigned j = 0; j < params_.w_height_; j++ )
      class_map[ to_string( i, j ) ] = class_index++;

  cout << "before merging classes" <<  endl;
  for ( unsigned i = 0; i < map_positions.size(); i++ )
    cout << "  " << class_map[ map_positions[ i ] ] << endl;
  /*
  for ( unsigned i = 0; i < position; i++ )
    for ( unsigned j = 0; j < position; j++ )
    {
      if ( distances( class_map[ map_positions[ i ] ], class_map[ map_positions[ j ] ] ) < 0.08 )
        class_map[ map_positions[ j ] ] = class_map[ map_positions[ i ] ];
    }
  */
  cout << "after merging classes" <<  endl;
  for ( unsigned i = 0; i < map_positions.size(); i++ )
    cout << "  " << class_map[ map_positions[ i ] ] << endl;
    
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

    for ( unsigned i = for_begin; i < for_end; i++ )
      xk.push_back( data[ dimensions[ i ] ] );
    
    // find w_winner
    for ( unsigned i = 0; i < params_.w_width_; i++ )
      for ( unsigned j = 0; j < params_.w_height_; j++ )
      {
        vector<double> wij;
        double d = 0.0;

        for ( unsigned a = for_begin; a < for_end; a++ )
          wij.push_back( params_.weights_[ a ]( i, j ) );
/*cout << "comparing xk:" << endl;
for (unsigned k = 0; k < xk.size(); k++)
  cout << "  " << xk[k];
cout << endl;
cout << "with neuron: " << i << ", " << j << endl;
for (unsigned k = 0; k < wij.size(); k++)
  cout << "  " << wij[k];
cout << endl;*/
        d = params_.get_distance( xk, wij );
// cout << "d: " << d << " and winner_d: " << winner_d << endl;        
        if ( d < winner_d )
        {
          winner_i = i;
          winner_j = j;
          winner_d = d;
        }
      }
//     cout << "and the winner is: " << to_string( winner_i, winner_j ) << endl;
    classifyResult.push_back( class_map[ to_string( winner_i, winner_j ) ] );

    ++attr_it;
  }
  status.Toggle( false );


  #ifdef _DEBUG
  cout << "end classify()" << endl;
  #endif
  
  return true;
}