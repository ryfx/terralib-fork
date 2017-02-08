#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>  
#include <vector>      ///para usar vetores
#include <valarray>    ///para usar somatorio
using namespace std;


/// Função para calcular a estatistica de teste Score
double scorefunction (int tipo,vector<double> &casos,vector<double> &esperados,vector<double> &d,double pond )
{
       unsigned int i, k;
       double U=0.0,VarU=0.0,Ust;

	   k=casos.size();
             
	   if (tipo==0)
	   {	   
			for(i=0;i<k;i++)
			{

				U += (casos[i]-esperados[i])*exp(-d[i]/pond);
				VarU += exp(-d[i]/pond)*exp(-d[i]/pond)*esperados[i];
			}
	   }

	  if (tipo==1)
	  {
		  for(i=0;i<k;i++)
		  {
			  double aux = casos[i];
			  double aux2 = esperados[i];
              U += (casos[i]-esperados[i])*(1/d[i]);
			  VarU += pow(d[i],-2)*esperados[i];
		  }
	  }
      Ust= U/sqrt(VarU);	
      
	  return(Ust);
}
