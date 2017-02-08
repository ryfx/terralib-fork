#ifndef FUNCAUX_H_
#define FUNCAUX_H_

#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
using namespace std;



double Dist(const double& X0,const double& Y0,const double& XF, const double& YF)
// Calcula distância do ponto i ao j
{
   return sqrt(pow((XF-X0),2) + pow((YF-Y0),2));
}


long DefineNumeroVizinhos(const long& n_eventos, const vector<long>& Perm,
                         const vector<bool>& VizEspaco, const vector<bool>& VizTempo)
// Define o número de vizinhos
{
 long num, i ,j;

	num = 0;

	for(i=0; i<n_eventos; ++i)
  {
   for(j=i+1; j<n_eventos; ++j)
    {
 	  if ((VizEspaco[i * n_eventos + j]) && (VizTempo[(Perm[i] * n_eventos) + Perm[j]]))
				num++;
    }
 }

  return num;
}


void DefinePermutacaoAleatoria(const long& n_eventos, vector<long>& Perm)
// Define uma permutacao aleatoria
{
 srand ( time(NULL) );

   long i, pos;

	for(i=0; i<n_eventos; ++i)
		Perm[i] = -1;

	for(i=0; i<n_eventos; ++i)
   {
      pos = rand() % n_eventos;
      while(Perm[pos] != -1)
			  pos = rand() % n_eventos;
		Perm[pos] = i;
   }
}

long ContViz(const vector<bool>& Vet,const long& numEvt)
// Conta o numero de vizinhos
{
   long Cont=0;
   long i,j;
   
   for(i=0; i<numEvt; ++i)
   {
	  //for(j=0; j<i; ++j)
      for(j=i+1; j<numEvt; ++j)
      {
         if(Vet[i*numEvt+j])
            ++Cont;
      }
   }

   return Cont;
}


#endif
