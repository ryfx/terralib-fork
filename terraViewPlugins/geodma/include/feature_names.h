#ifndef FEATURE_NAMES_H
  #define FEATURE_NAMES_H

#include <vector>
#include <string>
#include <TeUtils.h>
#include "curve_features.h"

using namespace std;

void get_cell_feature_names(vector<string>& names, int nbands)
{
  names.push_back( "c_ca" );
  names.push_back( "c_percentland" );
  names.push_back( "c_pd" );
  names.push_back( "c_mps" );
  names.push_back( "c_lsi" );
  names.push_back( "c_msi" );
  names.push_back( "c_awmsi" );
  names.push_back( "c_mpfd" );
  names.push_back( "c_awmpfd" );
  names.push_back( "c_ed" );
  names.push_back( "c_mpar" );
  names.push_back( "c_pssd" );
  names.push_back( "c_pscov" );
  names.push_back( "c_np" );
  names.push_back( "c_te" );
  names.push_back( "c_iji" );

  for ( int b = 0; b < nbands; b++ )
  {
    names.push_back( "rc_amplitude_" + Te2String( b ) );
    names.push_back( "rc_entropy_" + Te2String( b ) );
    names.push_back( "rc_mean_" + Te2String( b ) );
    names.push_back( "rc_mode_" + Te2String( b ) );
    names.push_back( "rc_stddev_" + Te2String( b ) );
    names.push_back( "rc_sum_" + Te2String( b ) );

    names.push_back( "rc_dissimilarity_" + Te2String( b ) );
    names.push_back( "rc_entropy2_" + Te2String( b ) );
    names.push_back( "rc_homogeneity_" + Te2String( b ) );
  }
};

void get_polygon_feature_names(vector<string>& names, int nbands)
{
  names.push_back( "p_angle" );
  names.push_back( "p_area" );
  names.push_back( "p_box_area" );
  names.push_back( "p_circle" );
  names.push_back( "p_compacity" );
  names.push_back( "p_contiguity" );
  names.push_back( "p_density" );
  names.push_back( "p_elliptic_fit" );
  names.push_back( "p_fractal_dimension" );
  names.push_back( "p_gyration_radius" );
  names.push_back( "p_length" );
  names.push_back( "p_perimeter" );
  names.push_back( "p_perimeter_area_ratio" );
  names.push_back( "p_rectangular_fit" );
  names.push_back( "p_shape_index" );
  names.push_back( "p_width" );

  for ( int b = 0; b < nbands; b++ )
  {
    names.push_back( "rp_amplitude_" + Te2String( b ) );
    names.push_back( "rp_entropy_" + Te2String( b ) );
    names.push_back( "rp_mean_" + Te2String( b ) );
    names.push_back( "rp_mode_" + Te2String( b ) );
    names.push_back( "rp_stddev_" + Te2String( b ) );
    names.push_back( "rp_sum_" + Te2String( b ) );

    names.push_back( "rp_dissimilarity_" + Te2String( b ) );
    names.push_back( "rp_entropy2_" + Te2String( b ) );
    names.push_back( "rp_homogeneity_" + Te2String( b ) );
  }
};

void get_point_feature_names(vector<string>& names)
{
  vector<string> tmp = get_mt_features_names();

  for ( unsigned i = 0; i < tmp.size(); i++)
    names.push_back( tmp[ i ] );
};

#endif // FEATURE_NAMES_H
