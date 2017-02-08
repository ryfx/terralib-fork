#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>  
#include <vector>      
#include <valarray>    
using namespace std;


/// Função para calcular a estatistica de teste de Bithel
double Bithell ( int tipo, double &betha,double &phi,vector<double> &casos,vector<double> &d )
{
       unsigned int i, k;
       double T=0.0;

	   k=casos.size();

   

       if (tipo==1)
        {
                  for(i=0;i<k;i++)
                    {
                       T+= (double)casos[i]*log(1+betha*exp(-d[i]/phi));
                       }
       }
         if (tipo==2)
        {
                  for(i=0;i<k;i++)
                    {
                       T+= (double)casos[i]*log(1+betha*exp(pow(-d[i]/phi,2)));
                       }
       }
        if (tipo==3)
        {
                  for(i=0;i<k;i++)
                    {
                       T+= (double)casos[i]*log(1+betha/(1+d[i]/phi));
                       }

        }
        return(T);
        }

///Função para calculo do p-valor
void Permut(vector<double> &casos)
{
   int i,j,resultado;
   double b;
   srand(time(NULL));
   for(i =(casos.size()-1); i>0; i --)
   {
     resultado=rand()%(casos.size()-1);
      j = resultado;
      b = casos[i];
      casos[i] = casos[j];
     casos[j] = b;
    }
}


double pvalor(int tipo, double betha,double phi,vector<double> casos,vector<double> &d, double T,int Npermut)
{
     int i;
	 double aux=0.0,Tnovo, pvalor;
   
     for (i=0;i<Npermut;i++)
     {
         Permut(casos);
         Tnovo=Bithell(tipo, betha,phi,casos,  d);
         if (Tnovo>T) aux++;
   }

pvalor=(aux/Npermut);
return(pvalor);
}
