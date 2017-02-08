#include <stdio.h>
#include <stdlib.h>
#include <math.h>      ///para usar funções matematicas
#include <vector>      ///para usar vetores
#include <valarray>    ///para usar somatorio
using namespace std;

/// Calcula a matriz de derivadas 
void derivada1 (vector<double> d,vector<double> p,vector<double> &M1, int casos,int controles);
void derivada2 (vector<double> d,vector<double> p, vector < vector <double> > &M2, int casos,int controles);
void inverte(vector< vector<double> > &M2);
double quiquadrado1gl(double D);
double diggletest( double &alfa,double &beta, vector<double>d, int casos, int controles,double &L1,double &L0);