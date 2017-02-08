#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include <iostream>
#include "CImg.h"
#include <math.h>
#include <vector>
//#include<fstream>

#ifndef algoritmos_H
#define algoritmos_H

#ifndef MAX
#define MAX(a,b) ( (a>b) ? a : b )	//!< Macro that returns max between two values
#endif

#ifndef MIN
#define MIN(a,b) ( (a<b) ? a : b )	//!< Macro that returns min between two values
#endif


int whichquad(double ptx, double pty, double origx, double origy);
bool within(double x, double y, const std::vector<double> &xp, const std::vector<double> &yp);
double distance(double x1, double y1, double x2, double y2);
double arzz(double d, double theta);
double ssarea( double d1t, double d2t, double h);
int kernelink(unsigned int nx,unsigned  int ny, double h1, double h2, double x,
             double y, const std::vector<double> &xp, double xmin, double xmax,
             const std::vector<double> &yp, double ymin, double ymax,              
             const std::vector<double> &x1, const std::vector<double> &y1, 
             const std::vector<double> &x2, const std::vector<double> &y2, 
             std::vector<double> &xgrid, std::vector<double> &ygrid, 
             std::vector< std::vector<double> > &zgrid, double &MaxZ);
int limited(int n);
double rand0a1();
void Permut(std::vector<int> &A);
double Z(double x);
double AcNormal(double x);
void ScoreStat(const std::vector<double> &x1, const std::vector<double> &y1, 
               const std::vector<double> &x2, const std::vector<double> &y2,
               double h1, double h2, double &Obs, double &Exp, double &Var,
               std::vector<int> &i1, std::vector<int> &i2, bool & wasCancelled);
void ScoreTest(std::vector<int> i1, std::vector<int> i2, int n, int nsim,
               const double Obs, double &pvalue, bool & wasCancelled);
int ConditionalKernel(const unsigned int nx, const unsigned  int ny, 
					  const double h1, const double h2,
					  const std::vector<double> &xp, double xmin, double xmax,
					  const std::vector<double> &yp, double ymin, double ymax,              
					  const std::vector<double> &x1, const std::vector<double> &y1, 
					  const std::vector<double> &x2, const std::vector<double> &y2, 
					  const double rate, const bool MaxRegion);


#endif
