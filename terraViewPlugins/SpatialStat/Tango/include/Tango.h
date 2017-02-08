#ifndef __TANGOH__
#define __TANGOH__

#include <valarray>
#include <vector>
#include <string>

#include "dcdflib.h"
#include "newmatap.h"                

#define PI 3.14159265358979323846
#define R 6371

using namespace std;

/////////////////////////////////////////////////////////////////////////////////

// Calcula matriz de distancias geodesicas (em km) a partir das coordenadas latitude e longitude
void geodeticdistance(const std::vector< vector<double> > &LatLong, Matrix &D);
 
vector<double> runif(const int num);
 
int howMany (const std::vector<double> &vec, const double value1, const double value2);
 

void countc(const std::vector<double> &vec1, const std::valarray<double> &vec2, 
                        ColumnVector &hist);
 
void seq(const int from, const int to, const int by, std::valarray<int> &seqq);
 
void diagonal(const ColumnVector &vec, Matrix &diag_vec);
 
//void sort_ind( ColumnVector &vec2, const vector<string> &sortind );

int Meet1( const int nloop, const std::valarray<int> &strata, 
          const std::valarray<int> &pop, const std::valarray<int> &cases, 
          const std::vector< vector<double> > &latlong, vector<std::string> &geocode, 
          std::valarray<double> &ZZ, double &pvalue, int &lambda_opt, double &Pmin, ColumnVector &ss, int &nk ); 

int Meet2( const int nloop, const std::valarray<int> &strata, 
          const std::valarray<int> &pop, const std::valarray<int> &cases, 
          const std::vector< vector<double> > &latlong, vector<std::string> &geocode, 
          std::valarray<double> &ZZ, double &pvalue, int &lambda_opt, double &Pmin, ColumnVector &ss, int &nk ); 

int Meet3( const int nloop,  
          const std::valarray<int> &pop, const std::valarray<int> &cases, 
          const std::vector< vector<double> > &latlong, vector<std::string> &geocode, 
          std::valarray<double> &ZZ, double &pvalue, int &lambda_opt, double &Pmin, ColumnVector &ss ); 

int Meet4( const int nloop,  
          const std::valarray<int> &pop, const std::valarray<int> &cases, 
          const std::vector< vector<double> > &latlong, vector<std::string> &geocode, 
          std::valarray<double> &ZZ, double &pvalue, int &lambda_opt, double &Pmin, ColumnVector &ss ); 

#endif
