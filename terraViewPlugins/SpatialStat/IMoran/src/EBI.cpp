#include "EBI.h"
#include <math.h>    //para usar funções matematicas
using namespace std;

/* Calculo de Z. Parametros de entrada o vetor de valores observados (yi), 
o nº de casos (ni).*/
void calculaZi(const valarray<double> &yi,const valarray<double> &ni, vector<double> &z)
{
 double b, aux=0.0, s2, a, vi,temp,m=0.0;
 unsigned int i;
 vector<double> ri(yi.size(),0.0);

 b = yi.sum()/ni.sum();         //calcula a variancia pelo estimador b^
 temp = ni.sum();
 m = temp/ni.size();        //calcula m 


 for (i=0;i<yi.size();i++) {
   ri[i]=yi[i]/ni[i];          // calcula ri e armazena em um vetor ri[i]
   aux += ni[i]*(pow((ri[i]-b),2));
 }
 s2= aux/ni.sum(); 
 a= s2 - b/m;				   //calcula a média pelo estimador â
 for (i=0;i<yi.size();i++)      //calcula vi[i]
 {
	vi = (b/(ni[i]+a));
    if (vi<=0) 
      vi=b/ni[i];
    z[i] = (ri[i]-b)/sqrt(vi);
 }
}

double S0(const vector< vector <double> > &vizin)
{
	int i;
	double s0=0.0;
	for (i=0;i < (int)vizin.size();i++)
		s0+= vizin[i][2];
	return(s0);
}
double S1(const vector< vector <double> > &pesos)
{
	int i,j;
	double s1=0.0;
	for (i=0;i < (int)pesos.size();i++)
		for(j=i+1;j < (int)pesos.size();j++)
			s1 += (pow(pesos[i][j] + pesos[j][i],2.0));
	return(s1);
}
double S2(const vector< vector <double> > &pesos)
{
	int i,j,temp;
	double s2=0.0,pesol,pesocol;
	temp = pesos.size();
	for (i=0;i< (int)pesos.size();i++){
		pesol = 0.0;
		pesocol = 0.0;
		for(j=0;j< (int)pesos.size();j++){
			pesol += pesos[i][j];
		    pesocol += pesos[j][i];
		}
		s2 += pow((pesol+pesocol),2.0);
		
	}
	return(s2);
}

double varN(const double &esp, const double &s0,const double &s1,const double &s2, const int &areas)
{
	double var;

	var = 1.0/((areas-1) * (areas+1) * s0 * s0);
	var *= ((areas*areas*s1) - (areas*s2) + (3*s0*s0));
	var -= (esp*esp);

	return(var);
}

double varR(const double &esp, const double &s0,const double &s1,const double &s2,const vector<double> &z)
{
	int i,N;
	double var,b2,m4=0.0,m2=0.0;

    N = z.size();

	for(i=0;i<N;i++){
		m4 += pow(z[i],4.0);
		m2 += pow(z[i],2.0);
	}
	m4 /= (double)N;
	m2 /= (double)N;
    b2 = m4/(m2*m2);

	var = 1.0/((N-1) * (N-2) * (N-3) * s0 * s0);
	var *= ((N*(N*N - 3*N + 3)*s1 - N*s2 + 3*s0*s0) - b2*((N*N-N)*s1 - 2*N*s2 + 6*s0*s0));
	var -= (esp*esp);

	return(var);
}

// Calculo de EBIMoran. Parametros de entrada são o vetor de z,  a matriz de vizinhança (vizin) 
//  e o valor de m(população média por área).
double calculaEBI(vector<double> &z,const vector< vector <double> > &vizin)
{
unsigned int i;
double EBI=0.0,s0=0.0,Zmedio=0.0, soma=0.0;

 for (i=0;i<vizin.size();i++)
     EBI += vizin[i][2]*(z[(int)vizin[i][1]])*(z[(int)vizin[i][0]]); 
 EBI *= z.size();
 
 s0 = S0(vizin);
 
 EBI /= s0;
 
 for (i= 0;i<z.size();i++)
    Zmedio += z[i];
 Zmedio /= z.size();
  
 for (i= 0;i<z.size();i++) 
    soma+=pow((z[i]-Zmedio),2);
 
 EBI /= soma;

 
 return(EBI);
}

/*Devolve um vetor de tamanho "areas" permutado.
Para a chamada da função foi necessario fornecer o tamanho do vetor*/
void Permut(vector<double> &z, int areas)
{
 int i,j,resultado;
 double b;
 srand(time(NULL));
 for(i =(areas-1); i>0; i--)
 {
   resultado = rand()/RAND_MAX;
   resultado *= (areas-1);
   j = resultado;
   b = z[i];
   z[i] = z[j];
   z[j] = b;
 }
}
double calculaPvalor (vector<double> &z, const vector< vector <double> > &vizin, double NPermut,const int &areas, const double &EBI)
{
int i, aux=0;
double pvalor, EBInovo=0.0;

for (i=0;i<NPermut;i++)
   {
    Permut(z,areas);
    EBInovo = calculaEBI(z,vizin); 
    if (EBInovo>=EBI) aux++;
   }
   pvalor=((aux+1)/(double)NPermut);
   return(pvalor);
}

int EBI(const vector< vector <double> > &vizin,const valarray<double> &ni, const valarray<double> &yi, Saida_EBI &saida,const int &NPermut)
{
	int i, pos1,pos2;
	double s0,s1,s2;
	vector<double> z(ni.size());
    vector< vector <double> > pesos(ni.size());
	
	for(i=0;i< (int)ni.size();i++)
		pesos[i].resize(ni.size(),0.0);

	for(i=0;i< (int)vizin.size();i++){
		pos1 = (int)vizin[i][0];
		pos2 = (int)vizin[i][1];
		pesos[pos1][pos2] = vizin[i][2];
	}

	s0 = S0(vizin);
	s1 = S1(pesos);
	s2 = S2(pesos);

	calculaZi(yi,ni,z);
    
	saida.EBI = calculaEBI(z,vizin);
	saida.esperanca = -1.0/(double)(ni.size()-1);
	saida.varN = varN(saida.esperanca,s0,s1,s2,z.size());
	saida.varR = varR(saida.esperanca,s0,s1,s2,z);
	saida.ZN = ((saida.EBI - saida.esperanca)/pow(saida.varN,0.5));
	saida.ZR = ((saida.EBI - saida.esperanca)/pow(saida.varR,0.5));
    saida.pvalor = calculaPvalor(z,vizin,NPermut,ni.size(),saida.EBI);
	return(0);
}