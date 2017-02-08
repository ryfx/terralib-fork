#ifndef EBI_H
#define EBI_H

#include <cstdio>
#include <vector>    //para usar vetores
#include <valarray>
#include <cstdlib>  //para nº aleatório
#include <time.h>    //para usar o relógio (semente)

class Saida_EBI
{
 public:
	double EBI, esperanca, varN, ZN, varR, ZR, pvalor;

	Saida_EBI(){EBI = esperanca = varN = ZN = varR = ZR = pvalor = 0.0;}
};

using namespace std;

void calculaZi(const valarray<double> &, const valarray<double> &, vector<double> &);
double calculaEBI(vector<double> &,const vector< vector <double> > &);
void Permut(vector<double> &, int);
double calculaPvalor (vector<double> &, const vector< vector <double> > &, const int &, const double &);
double varN(const double &, const double &,const double &,const double &, const int &);
double varR(const double &, const double &,const double &,const double &,const vector<double> &);
double S0(const vector< vector <double> > &);
double S1(const vector< vector <double> > &);
double S2(const vector< vector <double> > &);
int EBI(const vector< vector <double> > &,const valarray<double>&, const valarray<double>&, Saida_EBI&,const int &);
#endif