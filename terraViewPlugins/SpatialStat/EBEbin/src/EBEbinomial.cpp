/* Estimador Bayesiano empírico para dados binomiais */
/* baseado no artigo de Assunção et al. (1999)       */

//#include <EBEbinPluginWindow.h>
#include "EBEbinomial.h"
#include <math.h>
#include <vector>

#define PI 3.14159265358979323846
#define R 6371

using namespace std;

//////////////////////////////////////////////////////////////////

// Calcula matriz de vizinhanca < D > baseada nas distancias geodesicas (em km) calculadas 
// a partir das coordenadas latitude e longitude
void geodeticdistance(const std::vector< vector<double> > &LatLong, std::vector< std::vector <double> > &D, const double raio)
 {
	int i, j, NSITES = LatLong.size();
	double Dij, temp;
	vector <double> Latrad(NSITES), Longrad(NSITES);

    // declaracao da matriz D
    for(i=0; i<NSITES; i++)
	    D[i].resize(NSITES,0.0);

	for (i = 0; i < NSITES; i++) {
		 Latrad[i] = LatLong[i][1] * PI / 180;
		 Longrad[i] = LatLong[i][0] * PI / 180;
	}

	for (i=0; i<NSITES; i++) {
		 for (j=0; j<i; j++) {
              temp = sin(Latrad[i])*sin(Latrad[j])
                     +  cos(Latrad[i])*cos(Latrad[j])*cos(fabs(Longrad[i]-Longrad[j]));
              Dij = R*acos(temp);  // distancia em km entre as areas i e j

              // preenche a matriz de vizinhanca
              if(Dij < raio)
                 D[i][j] = 1.0;
              else  
                 D[i][j] = 0.0;

              D[j][i] = D[i][j];
		 }
         D[i][i] = 1.0;
	}
 }

//////////////////////////////////////////////////////////////////

// Função que calcula o EBE global //
int EBEglobal(const std::vector<double> &pop,const std::vector<double> &eventos,
             std::vector<double> &b)
    {        
     int i, size = pop.size();
     std::vector<double> x(size), c(size);
     double a, den, m, expostos=0.0, s2=0.0, sumevent=0.0;
  
     // Estimando M //
     for(i=0; i<size; i++){
         if(eventos[i] > pop[i])
            return(1);
         x[i] = eventos[i]/pop[i]; 
         expostos += pop[i];
         sumevent += eventos[i];
     }
     if(expostos == 0)
        return(1);
     m =( sumevent/expostos );
  
     // Estimando a variância S^2 //
     for(i=0; i<size; i++)
         s2 += pop[i]*pow((x[i]-m),2.0);    
     s2 /= expostos; 
  
     // Estimando A //
     den=(1-(size/expostos));
     if(den == 0)
       return(1);      
     else
       a = s2-((m-(m*m))*size/expostos);
       a /= den;
    
     // Estimando ci //
     for(i=0; i<size; i++){          
         den = a + ((m-a-(m*m))/pop[i]);
         if (den != 0){                    // Condicionando o denominador diferente de 0 //
             c[i] = a/den;
             b[i] = m+(c[i]*(x[i]-m));       // Estimador Bayesiano empírico (global) para o risco Pi //        
             if (!(b[i]>=0 && b[i]<=1))
                 b[i] = m;
         }
         else
             b[i] = m;  
     }
     return(0);
   }

//////////////////////////////////////////////////////////////////

// Função que calcula o EBE local //
int EBElocal(const std::vector<double> &pop, const std::vector<double> &eventos, 
			 const std::vector< std::vector <double> > &D, std::vector<double> &blocal)
   { 
     int size = pop.size();
     std::vector<double> N, Y;
     std::vector<int> indices;
     int i, j, k, auxiliar, ind, indsize;

     // identificando população (N) e casos (Y) nas áreas vizinhas de cada área i  //
     for(i=0; i<size; i++) {
         for(j=0; j<size; j++) {
             if(D[i][j] == 1.0) {
                Y.push_back(eventos[j]);
                N.push_back(pop[j]);
                indices.push_back(j);
             }
         }   

         // identificando o índice da área  i no mapa local  //
         indsize = indices.size();
		 for(k=0; k<indsize; k++){
             if(indices[k] == i) {
                ind = k;
                break;
             }   
         }

         // calculando o Estimador Bayesiano Empírico local para a área i  //
         std::vector<double> estimativa(indices.size());
         auxiliar = EBEglobal(N, Y, estimativa);
         blocal[i] = estimativa[ind];
      
         estimativa.assign(size, 0);
         Y.clear();
         N.clear();
         indices.clear();
     }
     return(0);
   }

//////////////////////////////////////////////////////////////////

