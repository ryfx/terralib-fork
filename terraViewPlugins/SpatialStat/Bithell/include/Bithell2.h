#ifndef BITHELL2_H
#define BITHELL2_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>  
#include <vector>      
#include <valarray> 

using namespace std;

double Bithell ( int tipo, double &betha,double &phi,vector<double> &casos,vector<double> &d );
void Permut(vector<double> &casos);
double pvalor(int tipo, double betha,double phi,vector<double> casos,vector<double> &d, double T,int Npermut);

#endif