#include "curve_features.h"

double get_max(vector<double> /*ts*/)
{
  return 0.0;
}

double get_mode(vector<double> /*ts*/)
{
  return 0.0;
}

double get_sum(vector<double> /*ts*/)
{
  return 0.0;
}

double get_mean(vector<double> /*ts*/)
{
  return 0.0;
}

double get_std(vector<double> /*ts*/)
{
  return 0.0;
}

double get_min(vector<double> /*ts*/)
{
  return 0.0;
}

double get_median(vector<double> /*ts*/)
{
  return 0.0;
}

int get_zero_cross(vector<double> /*ts*/, char /*mode*/, double /*threshold*/)
{
  return 0;
}

int get_peacks(vector<double> /*ts*/)
{
  return 0;
}

int get_valleys(vector<double> /*ts*/)
{
  return 0;
}

double get_central_moment(vector<double> /*x*/, int /*p*/, int /*q*/, vector<double> /*y*/)
{
  return 0.0;
}

double get_eccentricity(vector<double> /*x*/, vector<double> /*y*/)
{
  return 0.0;
}

double get_perimeter(vector<double> /*x*/, vector<double> /*y*/)
{
  return 0.0;
}

double get_ellipse_ratio(vector<double> /*x*/, vector<double> /*y*/)
{
  return 0.0;
}

vector<double> get_slope(vector<double> /*ts*/)
{
  return vector<double> ();
}

vector<double> get_x(vector<pair<double, double> > /*xy*/)
{
  return vector<double> ();
}

vector<double> get_y(vector<pair<double, double> > /*xy*/)
{
  return vector<double> ();
}

vector<double> get_series(int /*id*/, int /*year*/, string /*rfilename*/)
{
  return vector<double> ();
}

vector<pair<double, double> > get_xy_from_polar(vector<double> /*ts*/)
{
  return vector<pair<double, double> > ();
}

void explode_string(string /*str*/, string /*separator*/, vector<string>* /*results*/)
{
}

vector<string> get_mt_features_names()
{
  return vector<string> ();
}

double get_feature(string /*name*/, vector<double> /*ts*/)
{
  return 0.0;
}

TePolygon get_polygon(vector<pair<double, double> > /*xy*/)
{
  return TePolygon();
}


