#ifndef NORMAL01_H_
#define NORMAL01_H_
#include<iostream>
#include<math.h>
#include <vector>

#define PI 3.141592653

using namespace std;

double Z(double x)
{
	double retorno;

	retorno = 1/sqrt(2*PI)*exp(-x*x/2);

	return(retorno);
}


double AcNormal(double x)
{

	vector<double> b;
	double t, retorno, p, fator;
	bool xneg = false;

	b.push_back(0.31938);
	b.push_back(-0.35656);
	b.push_back(1.78147);
	b.push_back(-1.82125);
	b.push_back(1.33027);

	p = 0.23164;

	if(x<0)
	{
		x = x*(-1);
		xneg = true;
	}

	t = 1/(1+p*x);


	fator =0;

	for(unsigned int i=0; i<b.size(); i++)
		fator = fator + b[i]*pow(t,(int)i+1);

	retorno = 1- Z(x)*(fator);

	if(xneg==true)
		retorno = 1 - retorno;


	return(retorno);

}

#endif