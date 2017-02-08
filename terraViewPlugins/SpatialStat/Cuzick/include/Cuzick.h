#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>  
#include <vector>      ///para usar vetores
#include <valarray>    ///para usar somatorio
#include <algorithm>
#include <iterator>
#include <functional>
using namespace std;

void Mdist (vector<double> coordx, vector<double> coordy, vector< vector<double> > &dist, int casos, int controles);
void Vizinhok (vector< vector<double> > dist , vector<double> &distk, int k, int casos, int controles);
int Cuzick ( int k, vector<double> distk,vector< vector<double> > dist , int casos, int controles);
void Permut(vector<double> &coordx,vector<double> &coordy);
double pvalor(vector<double> coordx,vector<double> coordy,int casos, int controles, double Tk,int k, vector<double> distk, vector< vector<double> > dist, int Npermut );
