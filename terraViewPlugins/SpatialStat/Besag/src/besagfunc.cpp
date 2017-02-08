// Programa para simulação do teste de varredura de kulldorff

//#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <time.h>                      // define time()
#include <math.h>
#include "besagfunc.h"

using namespace std;

// Função principal
 bool BesagNewell(const int &size, const int &k_casos,const vector< vector <double> > &geo,const valarray<double> &casos,const valarray<double> &pop, double &p_valor, vector< vector <double> > &raios) 
 {

   // Declaração dos contadores e constantes
  int i,j,k,cont;
  int centroides;

  // Declaração das matrizes
  vector< vector<double> > m_dist(size), m_index(size);
  
  for(i=0;i<size;i++){
	  m_dist[i].resize(size,0.0);
	  m_index[i].resize(size,0.0);
  }

  // Variáveis auxiliares
  double aux;

  centroides = geo.size();

  // Preenchimento das matrizes m_dist e m_index
  for(i=0;i<(int)m_dist.size();i++)
    for(j=0;j<(int)m_dist[0].size();j++)
      m_index[i][j] = (double) j;

  for(i=1;i<(int)m_dist.size();i++)
    for(j=0;j<i;j++)
    {
     aux = (geo[i][0]-geo[j][0])*(geo[i][0]-geo[j][0]);
     aux = aux + (geo[i][1]-geo[j][1])*(geo[i][1]-geo[j][1]);
     aux = sqrt(aux);
     m_dist[i][j] = aux;
     m_dist[j][i] = aux;
    }

  // Ordenação das matrizes m_dist e m_index
  //cout << "\n > Ordenacao das matrizes: m_dist e m_index ";
  ordenacao(m_dist,m_index);

  // Teste gráfico de Besag e Newel para a detecção de Clusters
  //

  // Variáveis para armazenamento de casos e indivíduos em risco
  double C, N, cz, nz;
  double p;						// Probabilidades
  double Max_pop;				// Máxima contagem da população

  C = casos.sum();
  N = pop.sum();
  Max_pop = 0.2*N;			// 20% da população

  for(k=0;k<centroides;k++)
  {
    cz = 0;
    nz = 0;
    if(casos[k]>0)							// Verifica se existe pelo menos
     {                                          // um caso no centróide
      for(i=0;i<(centroides-1);i++)
      {
       cont = (int) m_index[k][i];
       cz = cz + casos[cont];				// Contador para no. de casos
       nz = nz + pop[cont];					// Contador para populacao

       if(cz >= k_casos)						// Testa a estatística para o conglomerado
       {
        p = pvalor(nz, C, N, k_casos);
        if(p <=p_valor)
        {
          //cout << "\n [k , p_valor] : " << k << " , " << p;
          raios[k][2] = p;
          raios[k][1] = m_dist[k][i];
          raios[k][0] = 1;
        }
       }

      }
     }
  }
 // raios.tofile("besag.txt");
 return(true);
 }

/* - - - - - - - - - - - - - - Functions - - - - - - - - - - - */
 void ordenacao(vector< vector <double> > &dist, vector< vector <double> > &index)
 {
  int x,y,k,N;
  int min_indice, max_indice;
  double minimo, maximo, aux;

  N = dist[0].size();

  for(k=0;k<N;k++)			// Para cada linha da matriz
  {

    for(x=0;x<N/2;x++)
    {
      minimo = dist[k][x];
      maximo = dist[k][N-x-1];
      min_indice = x;
      max_indice = N-x-1;
      for(y=x;y<N-x;y++)
      {
        if(dist[k][y]<minimo)
          {
           minimo = dist[k][y];
           min_indice = y;
          }
        if(dist[k][y]>maximo)
          {
           maximo = dist[k][y];
           max_indice = y;
          }
      }
      aux = dist[k][min_indice];
      dist[k][min_indice] = dist[k][x];
      dist[k][x] = aux;
      aux = index[k][min_indice];
      index[k][min_indice] = index[k][x];
      index[k][x] = aux;
      if(max_indice==x)
        max_indice = min_indice;

      aux = dist[k][max_indice];
      dist[k][max_indice] = dist[k][N-x-1];
      dist[k][N-x-1] = aux;
      aux = index[k][max_indice];
      index[k][max_indice] = index[k][N-x-1];
      index[k][N-x-1] = aux;
    }
  }
 }

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

 void ordena(vector< vector <double> > &dist)
 {
  int x,y,N;
  int min_indice, max_indice;
  double minimo, maximo, aux;

  N = dist[0].size();

    for(x=0;x<N/2;x++)
    {
      minimo = dist[x][0];
      maximo = dist[N-x-1][0];
      min_indice = x;
      max_indice = N-x-1;
      for(y=x;y<N-x;y++)
      {
        if(dist[y][0]<minimo)
          {
           minimo = dist[y][0];
           min_indice = y;
          }
        if(dist[y][0]>maximo)
          {
           maximo = dist[y][0];
           max_indice = y;
          }
      }
      aux = dist[min_indice][0];
      dist[min_indice][0] = dist[x][0];
      dist[x][0] = aux;
      if(max_indice==x)
        max_indice = min_indice;

      aux = dist[max_indice][0];
      dist[max_indice][0] = dist[N-x-1][0];
      dist[N-x-1][0] = aux;
    }
 }


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

double pvalor(double m, double C, double M, int kcasos)
 {
  double aux,aux2,fat;
  int j;

  fat = 1;
  aux = 0;
  aux2 = m*C/M;
  for(j=1;j<=(kcasos-1);j++)
   {
    fat = fat*(double)j;
    aux = aux + (exp(j*log(aux2))*exp(-aux2)/fat);
   }
  return 1-aux;
 }
