#ifndef MANTEL_H_
#define MANTEL_H_

#include <vector>      
#include <valarray>
#include <time.h>


#define PI 3.14159265358979323846
#define R 6371

using namespace std;

//////////////////////////////////////////////////////////////////

// Calcula matriz de distancias geodesicas (em km) a partir das coordenadas latitude e longitude
void geodeticdistance(const vector< vector<double> > &LatLong, vector< vector<double> > &D)
 {
	int i, j, NSITES = LatLong.size();
	double temp;
	vector <double> Latrad(NSITES), Longrad(NSITES);

    // declaracao da matriz D
    for(i=0; i<NSITES; i++)
	    D[i].resize(NSITES,0.0);

	for (i = 0; i < NSITES; i++) {
		 Latrad[i] = LatLong[i][1] * PI / 180;
		 Longrad[i] = LatLong[i][0] * PI / 180;
	}

	for (i = 0; i < NSITES; i++) {
		 for (j = 0; j < i; j++) {
              temp = sin(Latrad[i])*sin(Latrad[j])
                     + cos(Latrad[i])*cos(Latrad[j])*cos(fabs(Longrad[i]-Longrad[j]));
              D[i][j] = R*acos(temp);
              if (D[i][j] < 0.0000001) D[i][j] = 0.0;
              else D[i][j] = D[i][j];
              D[j][i] = D[i][j];
		 }
	}
 }

//////////////////////////////////////////////////////////////////

// Calcula matriz de distancias geodesicas (em km) para dados transformados, 
// a partir das coordenadas latitude e longitude
void geodetic_dist_transf(const vector< vector<double> > &LatLong, const double &shift_S, 
                          const double &power_S, vector< vector<double> > &D)
 {
	int i, j, NSITES = LatLong.size();
	double temp, Dij;
	vector <double> Latrad(NSITES), Longrad(NSITES);

    // declaracao da matriz D
    for(i=0; i<NSITES; i++)
	    D[i].resize(NSITES,0.0);

	for (i = 0; i < NSITES; i++) {
		 Latrad[i] = LatLong[i][1] * PI / 180;
		 Longrad[i] = LatLong[i][0] * PI / 180;
	}

	for (i = 0; i < NSITES; i++) {
		 for (j = 0; j < i; j++) {
              temp = sin(Latrad[i])*sin(Latrad[j])
                     + cos(Latrad[i])*cos(Latrad[j])*cos(fabs(Longrad[i]-Longrad[j]));
              Dij = R*acos(temp);
              if (Dij < 0.0000001) D[i][j] = pow( (0.0 + shift_S), power_S );
              else D[i][j] = pow( (Dij + shift_S), power_S );
              D[j][i] = D[i][j];
		 }
	}
 }

//////////////////////////////////////////////////////////////////

/*Devolve um vetor de tamanho "m" permutado.
Para a chamada da função foi necessario fornecer o tamanho do vetor*/
void Permut(vector<double> &n, int m)
 {
  int i,j,resultado;
  double b;
 // srand(time(NULL));
  for(i =(m-1); i>0; i --) {
      resultado=rand()%(m-1);
      j = resultado;
      b = n[i];
      n[i] = n[j];
      n[j] = b;
  }
 }

//////////////////////////////////////////////////////////////////

// Calcula a estatística de Mantel padronizada Z a partir das matrizes 
// de distancia espacial (S) e temporal (T)
double Z_statistic(const vector< vector <double> > &S, const vector< vector <double> > &T)
  {
   int i, j;
   double Z=0, sum_T=0.0, sum_S=0.0, desv_T=0.0, desv_S=0.0, std_S, std_T, med_S, med_T;
   int N = S[0].size();
   int N2 = N*N;

   // médias de S e T
   for (i=0; i<N; i++){
       for (j=0; j<N; j++){
		   sum_S += S[i][j]; 
		   sum_T += T[i][j]; 
       }
   }
   med_S = sum_S / N2;
   med_T = sum_T / N2;

   // desvios padroes de S e T
   for (i=0; i<N; i++){
       for (j=0; j<N; j++){
		   desv_T += pow((T[i][j]-med_T),2.0); 
		   desv_S += pow((S[i][j]-med_S),2.0); 
	   }
   }
   std_T = sqrt(desv_T / (N2-1));
   std_S = sqrt(desv_S / (N2-1));


   // Estatística Z de Mantel
   for (i=0; i<N; i++){
       for (j=0; j<N; j++){
           Z += ((S[i][j]-med_S)/std_S) * ((T[i][j]-med_T)/std_T);
       }
   }
   Z *=  1 / (double)(N2-N-1); 

   return Z;
  }

//////////////////////////////////////////////////////////////////
  
// Implementa o teste de Mantel //
int Mantel(const vector< vector<double> > &latlong, const vector<double> &t, 
              const int &nloop, valarray<double> &ZZ, double &pvalue, double &Zobs)
  {
   int i, j, k;
   double Tij, aux;
   int N = t.size();

  // Declaração das matrizes
  vector< vector<double> > T(N), T_repl(N);
  for(i=0;i<N;i++){
	  T[i].resize(N,0.0);
	  T_repl[i].resize(N,0.0);
  }

   vector<double> t_repl(N);
   t_repl = t;

   // preenchendo a matriz de distancias espacial
   vector< vector<double> > S(N);
   geodeticdistance(latlong, S);

   // preenchendo a matriz de distancias temporal
   for (i=1; i<N; i++){
      for (j=0; j<i; j++){
          Tij = fabs(t[i]-t[j]);
          T[i][j] = Tij;
          T[j][i] = Tij;
      }
   }

   Zobs = Z_statistic(S,T);  // estatistica de Mantel para os dados observados

   // Calculo da distribuicao nula da estatistica de teste
   for (k=0; k<nloop; k++){

		Permut(t_repl,N);  // permuta o vetor de tempos

        for (i=0; i<N; i++){
            for (j=0; j<N; j++){
                 T_repl[i][j] = fabs(t_repl[i] - t_repl[j]);  // replica a matriz T
            }
        }
		aux = Z_statistic(S, T_repl);  // estatistica de Mantel para os dados permutados
		ZZ[k] = aux;
   }
   ZZ[nloop] = Zobs;

   // Calculo do P_valor
   valarray <double> ZZGNE = ZZ [abs(ZZ) >= Zobs]; 
   pvalue = (double) (ZZGNE.size()) / (double) (ZZ.size());    // P_valor ajustado

   return (0);
  }

//////////////////////////////////////////////////////////////////

// Implementa o teste de Mantel con dados transformados //
int Mantel_transform(const vector< vector<double> > &latlong,  
              const vector<double> &t, const int &nloop, const double &shift_S, 
              const double &power_S, const double &shift_T, const double &power_T, 
              valarray<double> &ZZ, double &pvalue, double &Zobs)
  {
   int i, j, k;
   double Tij, aux;
   int N = t.size();

  // Declaração das matrizes
  vector< vector<double> > T(N), T_repl(N);
  for(i=0;i<N;i++){
	  T[i].resize(N,0.0);
	  T_repl[i].resize(N,0.0);
  }

   vector<double> t_repl(N);
   t_repl = t;

   // preenchendo a matriz de distancias espacial
   vector< vector<double> > S(N);
   geodetic_dist_transf(latlong, shift_S, power_S, S);

   // preenchendo a matriz de distancias temporal
   for (i=1; i<N; i++){
      for (j=0; j<i; j++){
          Tij = fabs(t[i]-t[j]);
          T[i][j] = pow( (Tij + shift_T), power_T );
          T[j][i] = pow( (Tij + shift_T), power_T );
      }
   }

   Zobs = Z_statistic(S,T);  // estatistica de Mantel para os dados observados

   // Calculo da distribuicao nula da estatistica de teste
   for (k=0; k<nloop; k++){

		Permut(t_repl,N);  // permuta o vetor de tempos

        for (i=0; i<N; i++){
            for (j=0; j<N; j++){
                 T_repl[i][j] = fabs(t_repl[i] - t_repl[j]);  // replica a matriz T
            }
        }
		aux = Z_statistic(S, T_repl);  // estatistica de Mantel para os dados permutados
		ZZ[k] = aux;
   }
   ZZ[nloop] = Zobs;

   // Calculo do P_valor
   valarray <double> ZZGNE = ZZ [abs(ZZ) >= Zobs]; 
   pvalue = (double) (ZZGNE.size()) / (double) (ZZ.size());    // P_valor ajustado

   return (0);
  }

////////////////////////////////////////////////////////////////////  
#endif
