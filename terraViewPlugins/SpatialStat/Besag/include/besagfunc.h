#ifndef BESAGFUNC_H
#define BESAGFUNC_H


#undef min
#undef max

#include <vector>
#include <valarray>

using namespace std;

// Declaração do escopo das funções
 bool BesagNewell(const int &, const int &,const vector< vector <double> > &,const valarray<double>  &,const valarray<double> &, double &, vector< vector <double> > &); 
 void ordenacao(vector< vector <double> > &dist, vector< vector <double> > &index);	// Ordena por linha matriz de dados e de indices
 void ordena(vector< vector <double> > &dist);							// Ordena somente vetor de dados
 double pvalor(double m, double C, double M, int kcasos);  // Calcula o p_valor

#endif
