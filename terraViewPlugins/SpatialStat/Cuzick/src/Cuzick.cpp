#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>  
#include <vector>      ///para usar vetores
#include <valarray>    ///para usar somatorio
#include <algorithm>
#include <iterator>
#include <functional>
using namespace std;

void Mdist (vector<double> coordx, vector<double> coordy, vector< vector<double> > &dist, int casos, int controles)
{
	int i,j;


	for ( i=0;i<casos+controles; i++)
		{
			
			for ( j=0;j<casos+controles; j++)
			{
				
				dist[i][j]= sqrt( pow( (coordx[i]-coordx[j]), 2) + pow( (coordy[i]-coordy[j]), 2) );
				
				
			}
		}
	

}


void Vizinhok (vector< vector<double> > dist , vector<double> &distk, int k, int casos, int controles)
{
	int i,j,p;
	vector<double> aux(casos+controles);


	for (i=0;i<casos+controles;i++)
	{

		for(j=0;j<casos+controles;j++)
		{
			aux[j]=dist[i][j];
			
		}
		sort(aux.begin(),aux.end());
		if(i<casos)
			distk[i]=aux[k+1];
		
		
	}
}

	
	// Função para calcular a estatistica de teste de Cuzizk-Edwars
int Cuzick ( int k, vector<double> distk,vector< vector<double> > dist , int casos, int controles)
{
        int i, j, Tk=0 ;
       

	    for ( i=0;i<casos; i++)
		{
			for ( j=0;j<casos; j++)
			{
				// printf("%lf \n %lf",dist[i][j], distk[i]);
				if ( dist[i][j]< distk[i])
					Tk++ ;
			}
		}

	return(Tk-casos);

}

	
	

///Função para calculo do p-valor

void Permut(vector<double> &coordx,vector<double> &coordy)
{
   int i,j,resultado;
   double b,c;
   srand(time(NULL));
   for(i =(coordx.size()-1); i>0; i --)
   {
     resultado=rand()%(coordx.size()-1);
      j = resultado;
      b = coordx[i];
	  c = coordy[i];
      coordx[i] = coordx[j];
	  coordy[i] = coordy[j];
     coordx[j] = b;
	 coordy[j] = c;
    }
}


double pvalor(vector<double> coordx,vector<double> coordy,int casos, int controles, double Tk,int k, vector<double> distk, vector< vector<double> > dist, int Npermut )
{
     int i;
	 double aux=0.0,Tnovo, pvalor;


		
     for (i=0;i<Npermut;i++)
     {
         Permut(coordx,coordy);
		 Mdist (coordx, coordy,dist,casos,controles);
		 Vizinhok (dist , distk, k,casos, controles);
         Tnovo=Cuzick(k, distk,dist ,  casos,  controles);
		 //printf("%lf",Tnovo);
         if (Tnovo>=Tk) aux++;
   }

pvalor=(aux/Npermut);
return(pvalor);
}

