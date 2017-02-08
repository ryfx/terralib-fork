#ifndef CURVE_FEATURES_H
  #define CURVE_FEATURES_H

  #include <float.h>
  #include <fstream>
  #include <iostream>
  #include <list>
  #include <map>
  #include <math.h>
  #include <stdlib.h>
  #include <vector>

  // TerraLib
  #include <TeGeometryAlgorithms.h>
  #include <TeGeometry.h>

  #define PI 3.1416
  #define abs(X) (X < 0? -X: X)

  using namespace std;

  // features from time series
  double get_max(vector<double> ts);
  double get_mode(vector<double> ts);
  double get_sum(vector<double> ts);
  double get_mean(vector<double> ts);
  double get_std(vector<double> ts);
  double get_min(vector<double> ts);
  double get_median(vector<double> ts);
  int get_zero_cross(vector<double> ts, char mode, double threshold);
  int get_peacks(vector<double> ts);
  int get_valleys(vector<double> ts);

  // features from polar visualization
  double get_central_moment(vector<double>x, int p, int q, vector<double> y);
  double get_eccentricity(vector<double> x, vector<double> y);
  double get_perimeter(vector<double> x, vector<double> y);
  double get_ellipse_ratio(vector<double> x, vector<double> y);

  // ancillary functions
  vector<double> get_slope(vector<double> ts);
  vector<double> get_x(vector<pair<double, double> > xy);
  vector<double> get_y(vector<pair<double, double> > xy);
  vector<double> get_series(int id, int year, string rfilename = "curves.csv");
  vector<pair<double, double> > get_xy_from_polar(vector<double> ts);
  void explode_string(string str, string separator, vector<string>* results);
  vector<string> get_mt_features_names();
  double get_feature(string name, vector<double> ts);
  TePolygon get_polygon(vector<pair<double, double> > xy);

#endif
