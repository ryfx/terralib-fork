#include<stdio.h>
#include<stdlib.h>
#include <vector>
#include <algorithm>


#ifndef funcoes_h
#define funcoes_h

struct Distancia{
       int col;
       double dist;
};

struct Tempo{
       int col;
       double temp;
};

void statJk(const std::vector< std::vector<double> > &dists, 
			const std::vector< std::vector<double> > &distt, int k,
			int linha, std::vector<int> &Jk);

void calcula(const std::vector< std::vector<double> > &dists, const std::vector< std::vector<double> > &distt,
         int k, int nsim, std::vector<int> &Jk0, std::vector<int> &DJk, std::vector<double> &pvalJk, double &pvalBonfJk, double &pvalSimesJk);
void jacqueztest(const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &tempo,
         int k, int nsim, std::vector<int> &Jk0, std::vector<int> &DJk, std::vector<double> &pvalJk, double &pvalBonfJk, double &pvalSimesJk);
#endif