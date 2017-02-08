#ifndef EBEBINOMIAL_H
#define EBEBINOMIAL_H

//#undef min
//#undef max

#include <vector>

using namespace std;

void geodeticdistance(const std::vector< vector<double> > &LatLong, std::vector< std::vector <double> > &D, const double raio);

int EBEglobal(const std::vector<double> &pop,const std::vector<double> &eventos,
             std::vector<double> &b);

int EBElocal(const std::vector<double> &pop, const std::vector<double> &eventos, 
			 const std::vector< std::vector <double> > &D, std::vector<double> &blocal);


#endif
