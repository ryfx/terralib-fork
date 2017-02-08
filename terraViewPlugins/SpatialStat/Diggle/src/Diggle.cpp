#include <stdio.h>
#include <stdlib.h>
#include <math.h>      ///para usar funções matematicas
#include <vector>      ///para usar vetores
#include <valarray>    ///para usar somatorio
using namespace std;

/// Calcula a matriz de derivadas 
void derivada1 (vector<double> d,vector<double> p,vector<double> &M1, int casos,int controles) //calcula a matriz de derivadas primeiras
{
  int i;

  for(i=0; i<2; i++)
	 M1[i] = 0.0 ;
  	
  M1[0]= casos;
  for (i=0;i<casos;i++)
  {
	M1[1] += d[i];
  }

  for (i=0;i<casos+controles;i++)
  {
	M1[0] -= p[i];
	M1[1] -= p[i]*d[i];
  }
}

/// Calcula a matriz de derivadas segundas
void derivada2 (vector<double> d,vector<double> p, vector < vector <double> > &M2, int casos,int controles) //calcula a matriz de derivadas segundas
{
	int i;
	M2[0].resize(2,0);
	M2[1].resize(2,0);


	for (i=0;i<casos+controles;i++)
	{
		M2[0][0]-= p[i]*(1-p[i]);
		M2[0][1]-= p[i]*(1-p[i])*d[i];
		M2[1][1]-= p[i]*(1-p[i])*d[i]*d[i];
	}

	M2[1][0]= M2[0][1];
}
 
 /// Inverte a matriz de derivadas segundas
void inverte(vector< vector<double> > &M2)
{
	 double det;
     vector< vector< double > > aux(2);
	 aux[0].resize(2);
	 aux[1].resize(2);
	      
	 det=M2[0][0]*M2[1][1];
	 det-= M2[1][0]*M2[0][1];  
 
	 aux[0][0]= M2[1][1]/det;
	 aux[1][1]= M2[0][0]/det;
	 M2[1][0]= -M2[1][0]/det;
	 M2[0][1]=M2[1][0];
	 M2[0][0]=aux[0][0];
	 M2[1][1]=aux[1][1];
}
     
/////Função para calculo do p-valor de uma Qui-quadrado com gl=1
double quiquadrado1gl(double D)
{

	int i;
	vector<double> P(11), X(11);

	X[0]=0; X[1]=0.0002; X[2]=0.004; X[3]=0.02; X[4]=0.45; X[5]=2.71; 
	X[6]=3.84;  X[7]=5.02; X[8]=6.63; X[9]=7.88; X[10]=1000000000000.;
	P[0]=0.99; P[1]=0.99; P[2]=0.95; P[3]=0.90; P[4]=0.50; P[5]=0.10;
	P[6]=0.05; P[7]=0.025; P[8]=0.01; P[9]=0.005; P[10]=0.000;
	double pvalor;

	for( i=1 ; i < 11 ; i++)
		if(X[i-1]<=D && D < X[i]) 
		{
			pvalor=P[i-1];
			break;
		}
	return(pvalor);
}

/// Função para o calcula da estatistica de Teste
double diggletest( double &alfa,double &beta, vector<double>d, int casos, int controles,double &L1,double &L0)
{
	int i=0,j=0;
	double  alfai, betai,D=0.0;
	//  double L0=0.0.L1=0.0;
	vector< double > M1(2);
	vector< double > p(casos+controles);
	for(i=0; i<2; i++)
		M1[i] = 0.0	 ;
  
	vector< vector<double> > M2(2);
 
	M2[0].resize(2,0);
	M2[1].resize(2,0);

	for (i=0;i<1000;i++)
	{ 
		for ( j=0;j<casos+controles;j++)
		{
	 		p[j]= exp(alfa+beta*d[j])/(1+exp(alfa+beta*d[j]));
  
		}
		derivada1(d, p, M1, casos, controles);
		derivada2(d, p, M2, casos, controles);
		inverte(M2);
		///inicializa os parametros de interesse para o teste de hipotese

		alfai= alfa;
		betai = beta;
		/// Processo iterativo para encontrar os parametros
		alfa = alfa - M2[0][0]*M1[0]- M2[1][0]*M1[1];
		beta = beta - M2[0][1]*M1[0]- M2[1][1]*M1[1];


	    if (abs(alfa-alfai/alfa)<=0.001)    // 0.001 é o erro
		{		
			if (abs(beta-betai/beta)<=0.001)break;
		}
	
	}

 
	L1= alfa*casos;
	for (i=0;i<casos;i++)
    {
		L1 +=  beta*d[i];
    }
    for (i=0;i<casos+controles;i++)
	{
		L1 -= log( 1 + exp(alfa+beta*d[i]));
	}
  
	L0 = (double)casos*log((double)casos/(double)(controles+casos)) + (double)controles*log((double)(controles)/(double)(casos+controles));

	D= 2*(L1-L0);

	return(D);
}

