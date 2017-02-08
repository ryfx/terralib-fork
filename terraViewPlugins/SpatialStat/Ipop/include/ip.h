#ifndef IP_H
#define IP_H

#include <vector>
#include <valarray>
#include <IpopPluginWindow.h>

class Saida_IPOP
{
 public:
	double Ipop, Ipopl, esp, var, Z, pvalor, varA, pMC;

	Saida_IPOP(){Ipop = Ipopl = esp = var = varA = Z = pvalor = 0.0;}
};

using namespace std;

double gammln(double);
void gser(double *,double,double,double *);
void gcf(double *,double,double,double *);
double gammp(double,double);
double gammq(double,double);
double erfc(double);
void Permut(valarray<double> &, int);
double calculaI_pop(const valarray<double> &,const valarray<double> &,const vector< vector <double> > &,double &,const vector<double> &,double &,double &,double &);
double calcula_p_valor(double,double,const vector<double> &,double &,const vector< vector<double> > &,const double &, Saida_IPOP &);
int iFunction(valarray<double> &,const valarray<double> &,const vector< vector <double> > &,const vector<double> &,const int &,const int &,Saida_IPOP &);

#endif