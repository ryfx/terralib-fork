#include <valarray>
#include <vector>
#include "Tango.h"
#include "dcdflib.h"
#include "newmatap.h"                // need matrix applications
#include <TeQtProgress.h>

//#define WANT_MATH                    // include.h will get math fns

#define PI 3.14159265358979323846
#define R 6371

using namespace std;

/////////////////////////////////////////////////////////////////////////////////

// Calcula matriz de distancias geodesicas (em km) a partir das coordenadas latitude e longitude
void geodeticdistance(const std::vector< vector<double> > &LatLong, Matrix &D)
 {
	int i, j, NSITES = LatLong.size();
	double temp;
	vector <double> Latrad(NSITES), Longrad(NSITES);

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

/////////////////////////////////////////////////////////////////////////////////

// Gera um vetor de números aleatorios da Uniforme[0,1]
vector<double> runif(const int num)
 {
  int i, x;
  std::vector<double> vec(num); 

  for (i=0; i<num; i++) { 
       x = rand(); 
       vec[i] = double (x) / RAND_MAX;
  } 
  return vec; 
 }

/////////////////////////////////////////////////////////////////////////////////

// Conta quantos elementos do vetor vec estao no intervalo [value1,value2]
int howMany (const std::vector<double> &vec, const double value1, const double value2) 
 { 
   int i, count = 0; 
   for (i=0; i<vec.size(); i++) { 
       if (vec[i] >= value1 && vec[i] < value2) 
           count++; 
   } 
   return count; 
 } 

/////////////////////////////////////////////////////////////////////////////////

// Conta quantos elementos de vec2 estao em cada subintervalo (vec1[i],vec1[i+1])
void countc(const std::vector<double> &vec1, const std::valarray<double> &vec2, 
                        ColumnVector &hist)
 {
  int i;
  hist.resize(vec2.size()); 

    for (i=0; i<vec2.size(); i++) { 
         if (i==0) 
             hist[i] = howMany (vec1, 0.0, vec2[i]); 
         else if (i>0 && i < vec2.size()-1) 
             hist[i] = howMany (vec1, vec2[i-1], vec2[i]); 
         else if (i = vec2.size()-1) 
             hist[i] = howMany (vec1, vec2[i], 1.0); 
    }
 } 

/////////////////////////////////////////////////////////////////////////////////

// gera uma sequencia de inteiros no intervalo [from,to]. by: incremento da sequencia
void seq(const int from, const int to, const int by, std::valarray<int> &seqq)
 {
   int i=0,  total = from;
   seqq.resize( (to - from + 1)/by ); 
   do{ 
      seqq[i] = total;
      total += by; 
      i++;
   } while(total < to);  
 }

/////////////////////////////////////////////////////////////////////////////////

// constroi uma matriz diagonal a partir dos elementos do vetor vec
void diagonal(const ColumnVector &vec, Matrix &diag_vec)
 {
    int i, dim = vec.Nrows();
    diag_vec = 0.0;
    
    for (i=0; i<dim; i++)
         diag_vec[i][i] = vec[i];
 }

/////////////////////////////////////////////////////////////////////////////////

// Ordena o vetor <vec2> em ordem descendente e guarda os indices da nova ordenacao no vetor <sortind>
/*void sort_ind( ColumnVector &vec2, const vector<string> &ind )
{
  int i, j, size = vec2.Nrows();
  double x;
  string t;

  vector<string> sortind(size);
  for (i=0; i<size; i++)
	        sortind[i] = ind[i];

  for (i=1; i<size; ++i)
      for (j=size-1; j>=i; --j){
          if (vec2[j-1] < vec2[j]){
             x = vec2[j-1];
             vec2[j-1] = vec2[j];
             vec2[j] = x;
             t = sortind[j-1];
             sortind[j-1] = sortind[j];
             sortind[j] = t;
          }
      }
}
*/
/////////////////////////////////////////////////////////////////////////////////

// Implementa o metodo MEET descrito em Tango(2000)
int Meet1( const int nloop, const std::valarray<int> &strata, 
          const std::valarray<int> &pop, const std::valarray<int> &cases, 
          const std::vector< vector<double> > &latlong , vector<string> &geocode, 
          std::valarray<double> &ZZ, double &pvalue, int &lambda_opt, double &Pmin, ColumnVector &ss, int &nk ) 
{
  int i, j, k, ijk, nrk, which = 1, status, rad; 
  double nj, sumpoo, soma, maxdc = 0.0, av, av2, av3, skew1, df1, eg1, vg1; 
  double gt1, stat1, aprox1, gt2, stat2, aprox2, pchisq, qchisq, bound;


  int nr = latlong.size();                          // number of regions 
  int nn = cases.sum();                             // total number of cases 
  nk = strata.max();                            // number of strata
//  std::valarray<int> regid; 
//  seq(1, nr, 1, regid);                             // sequence of IDs


  // Declaração das matrizes e vetores
  valarray<double> bbb(0.0,nr), pcc(0.0,nloop), cvv;  
  Matrix w(nr,nr);
  Matrix ac(nr,nr);
  Matrix hh(nr,nr);
  Matrix hh2(nr,nr);
  Matrix hhh(nr,nr);
  Matrix dc(nr,nr);
  Matrix frq(nloop,nr);
  Matrix qp;
  Matrix ggtt;
  Matrix ggtt2;
  ColumnVector p(nr);
  ColumnVector qdat(nr);
  //ColumnVector ss(nr);
  ColumnVector sortind(nr);
  ColumnVector temp1;
  ColumnVector p1;
  ColumnVector vv;
  ColumnVector r1;

  p = 0.0;
  dc = 0.0;  
  frq = 0.0;
  qdat= 0.0;
  
  // preenche a matriz de distancias
  geodeticdistance(latlong, dc);

  // encontra a maior distancia entre centroides de areas
  maxdc = dc.Maximum();

  // Gera a sequencia de valores de lambda
  double lambdamax = maxdc / 2.0;

  std::valarray<int> kvec; 
  seq(1, (int) ceil(lambdamax), 5, kvec);  // sequence of lambda values

  int kkk = kvec.size(); 
  // dimensao do vetor de lambdas
  if(TeProgress::instance())
  {
		string caption = "Tango";
		TeProgress::instance()->setCaption(caption.c_str());
		string msg = "Running Tango's Method. Please, wait!";
		TeProgress::instance()->setMessage(msg);
		TeProgress::instance()->setTotalSteps(kkk);
  }
  
  valarray<double> ptan(0.0,kkk);
  Matrix pval(nloop,kkk);
  pval = 0.0;
  Matrix pvalrow;
 
  
  // Calculation of (1) null distribution of test statistic Pmin //

  for(k=1; k<=nk; k++){   // abre loop for 1

      std::valarray <int> jdat = cases [strata == k]; // vetor de casos nas regioes do estrato k
      std::valarray <int> poo = pop [strata == k];    // vetor de populacao nas regioes do estrato k
      nj = jdat.sum();                                // soma dos casos nas regioes do estrato k
      sumpoo = poo.sum();                             // soma da populacao nas regioes do estrato k
      nrk = poo.size();

      p1.ReSize(nrk);
      vv.ReSize(nrk);
      cvv.resize(nrk);
      r1.ReSize(nrk); 

	  for (i=0; i<nrk; i++) 
          p1[i] = poo[i] / sumpoo;

      p += ( (nj/(double) nn) * p1 );                 // Eq. [2] de Tango(2000)

      for (i=0; i<nr; i++)
           qdat[i] += (double) jdat[i];

      // prenchendo a matriz w
      Matrix diag_p1(nrk,nrk); 
      diagonal(p1, diag_p1); // matriz diagonal(p1)
      
      w += (nj /(double) nn) * diag_p1 - p1*p1.t();

      vv = p1/p1.Sum();
      soma = 0.0; 
      for (i=0; i<nrk; i++) {
           soma += vv[i];
           cvv[i] = soma;           // soma acumulada do vetor vv
      }

      for (i=0; i<nloop; i++){
 	   	   countc( runif((int)nj), cvv, r1 );
 	   	   r1 /= (double) nn;  
           for (j=0; j<nr; j++){
                frq[i][j] += r1[j];   // frequencia relativa
           }
      }
  }          //  fecha loop for 1


  qdat /= (double) nn;
  
  for (ijk=0; ijk<kkk; ijk++){  // abre loop for 2
       rad = kvec[ijk];
	   if(TeProgress::instance())
	   {
           if (TeProgress::instance()->wasCancelled())
                 break;
           TeProgress::instance()->setProgress(ijk);
       }
       for (i=0; i<nr; i++){
           for (j=0; j<nr; j++){
                ac[i][j] = exp(-4.0 * pow((dc[i][j] / (double) rad),2.0));  // Eq. (6) de Tango(2000)
           }
       }

       // multiplicacao de matrizes
       hh = ac * w; 
       hh2 = hh * hh; 
       hhh = hh2 * hh; 

       av = 0.0, av2 = 0.0, av3 = 0.0;

       for (i=0; i<nr; i++){
            av += hh[i][i];                       // soma da diagonal da matriz hh
            av2 += hh2[i][i];                     // soma da diagonal da matriz hh2
            av3 += hhh[i][i];                     // soma da diagonal da matriz hhh
       }

       skew1= 2.0*sqrt(2.0)*av3 / pow(av2,1.5);       // Eq. [12] de Tango(2000)
       df1= 8.0 / skew1 / skew1;                  // Eq. [11] de Tango(2000)
       eg1= av;                                   // Eq. [9] de Tango(2000)
       vg1= 2.0 * av2;                            // Eq. [10] de Tango(2000)

       for (j=0; j<nloop; j++){
           qp = frq.Row(j+1);         // j-esima linha da matriz frq (A INDEXAÇAO COMECA EM 1 NESTA FUNCAO!!)
           temp1 = qp.AsColumn() - p;
           ggtt = ( (temp1.t() * ac) * temp1 ) * (double)nn;
           gt1 = ggtt.AsScalar();                 // considera matriz 1x1 como escalar
           stat1 = (gt1-eg1) / sqrt(vg1);
           aprox1 = df1 + sqrt(2.0*df1)*stat1;
           cdfchi ( &which, &pchisq, &qchisq, &aprox1, &df1, &status, &bound ); // cdf chi-quadrado
           pval[j][ijk]= qchisq;                  // Eq.(8) de Tango(2000)
       }

       temp1 = qdat-p;
       ggtt2 = ( ( temp1.t() * ac ) * temp1 ) * (double)nn;
       gt2 = ggtt2.AsScalar();                    // considera matriz 1x1 como escalar
       stat2 = (gt2-eg1) / sqrt(vg1);
       aprox2= df1 + sqrt(2.0*df1)*stat2;
       cdfchi ( &which, &pchisq, &qchisq, &aprox2, &df1, &status, &bound );  // cdf chi-quadrado
       ptan[ijk] = qchisq;                        // Eq.(8) de Tango(2000)

       // zera as matrizes hh, hh2 e hhh
       hh = 0.0; 
       hh2 = 0.0; 
       hhh = 0.0; 

  }             // fecha loop for 2
  if (TeProgress::instance())
       TeProgress::instance()->reset();


  // Calculation of (2) test statistic Pmin 
  Pmin = ptan.min();     // Pmin


  // Calcula o Pvalor
  for (i=0; i<nloop; i++){
       pvalrow = pval.Row(i+1); 
       pcc[i] = pvalrow.Minimum();
  }
  for (i=0; i<nloop; i++)
       ZZ[i] = pcc[i];
  ZZ[nloop] = Pmin;
  std::valarray <double> ZZp22 = ZZ [ZZ <= Pmin]; 
  pvalue = (double) ZZp22.size() / (double) (nloop+1);   // adjusted P-value


  // Identifica o lambda otimo
  std::valarray<int> jjj; 
  seq(0, kkk-1, 1, jjj);
  valarray <int> pind = jjj [ptan == Pmin]; 
  lambda_opt= kvec[pind[0]];                             // optimal lambda


  // Calcula a contribuicao de cada area
  for (i=0; i<nr; i++)
      for (j=0; j<nr; j++){
           ac[i][j] = exp(-4.0 * pow((dc[i][j] / (double) lambda_opt),2.0)); 
      }
  temp1 = qdat - p;
  ss = (ac * temp1) * nn;
  for (i=0; i<nr; i++)
       ss[i] = temp1[i]*ss[i];
  ss /= ss.Sum();              
  ss *= 100;                   // percent contribution to C  ( Eq. [14] de Tango(2000) )


  // Ordena o vetor ss segundo a contribuicao de cada area
//  sort_ind(ss, geocode);

  return (0);    
 }    

/////////////////////////////////////////////////////////////////////////////////
// Implementa o metodo MEET descrito em Tango(2000)
int Meet2( const int nloop, const std::valarray<int> &strata, 
          const std::valarray<int> &pop, const std::valarray<int> &cases, 
          const std::vector< vector<double> > &latlong , vector<string> &geocode, 
          std::valarray<double> &ZZ, double &pvalue, int &lambda_opt, double &Pmin, ColumnVector &ss, int &nk ) 
{
  int i, j, k, ijk, nrk, which = 1, status, rad; 
  double nj, sumpoo, soma, maxdc = 0.0, av, av2, av3, skew1, df1, eg1, vg1; 
  double gt1, stat1, aprox1, gt2, stat2, aprox2, pchisq, qchisq, bound;


  int nr = latlong.size();                          // number of regions 
  int nn = cases.sum();                             // total number of cases 
  nk = strata.max();                            // number of strata
//  std::valarray<int> regid; 
//  seq(1, nr, 1, regid);                             // sequence of IDs


  // Declaração das matrizes e vetores
  valarray<double> bbb(0.0,nr), pcc(0.0,nloop), cvv;  
  Matrix D(nr,nr);
  Matrix w(nr,nr);
  Matrix ac(nr,nr);
  Matrix hh(nr,nr);
  Matrix hh2(nr,nr);
  Matrix hhh(nr,nr);
  Matrix dc(nr,nr);
  Matrix frq(nloop,nr);
  Matrix qp;
  Matrix ggtt;
  Matrix ggtt2;
  ColumnVector p(nr);
  ColumnVector qdat(nr);
  //ColumnVector ss(nr);
  ColumnVector sortind(nr);
  ColumnVector temp1;
  ColumnVector p1;
  ColumnVector vv;
  ColumnVector r1;

  p = 0.0;
  dc = 0.0;  
  frq = 0.0;
  qdat= 0.0;
  
  // preenche a matriz de distancias
	for (i = 0; i < nr; i++) {
		for (j = 0; j < i; j++) {
             D[i][j] = sqrt( (latlong[i][0]-latlong[j][0])*(latlong[i][0]-latlong[j][0]) + 
				             (latlong[i][1]-latlong[j][1])*(latlong[i][1]-latlong[j][1]) );
             D[j][i] = D[i][j];
		}
	}


  // encontra a maior distancia entre centroides de areas
  maxdc = dc.Maximum();

  // Gera a sequencia de valores de lambda
  double lambdamax = maxdc / 2.0;

  std::valarray<int> kvec; 
  seq(1, (int) ceil(lambdamax), 5, kvec);  // sequence of lambda values

  int kkk = kvec.size();                   // dimensao do vetor de lambdas

  if(TeProgress::instance())
  {
		string caption = "Tango";
		TeProgress::instance()->setCaption(caption.c_str());
		string msg = "Running Tango's Method. Please, wait!";
		TeProgress::instance()->setMessage(msg);
		TeProgress::instance()->setTotalSteps(kkk);
  }

  valarray<double> ptan(0.0,kkk);
  Matrix pval(nloop,kkk);
  pval = 0.0;
  Matrix pvalrow;

  
  // Calculation of (1) null distribution of test statistic Pmin //

  for(k=1; k<=nk; k++){   // abre loop for 1

      std::valarray <int> jdat = cases [strata == k]; // vetor de casos nas regioes do estrato k
      std::valarray <int> poo = pop [strata == k];    // vetor de populacao nas regioes do estrato k
      nj = jdat.sum();                                // soma dos casos nas regioes do estrato k
      sumpoo = poo.sum();                             // soma da populacao nas regioes do estrato k
      nrk = poo.size();

      p1.ReSize(nrk);
      vv.ReSize(nrk);
      cvv.resize(nrk);
      r1.ReSize(nrk); 

      for (i=0; i<nrk; i++) 
          p1[i] = poo[i] / sumpoo;

      p += ( (nj/(double) nn) * p1 );                 // Eq. [2] de Tango(2000)

      for (i=0; i<nr; i++)
           qdat[i] += (double) jdat[i];

      // prenchendo a matriz w
      Matrix diag_p1(nrk,nrk); 
      diagonal(p1, diag_p1); // matriz diagonal(p1)
      
      w += (nj /(double) nn) * diag_p1 - p1*p1.t();

      vv = p1/p1.Sum();
      soma = 0.0; 
      for (i=0; i<nrk; i++) {
           soma += vv[i];
           cvv[i] = soma;           // soma acumulada do vetor vv
      }

      for (i=0; i<nloop; i++){
 	   	   countc( runif((int)nj), cvv, r1 );
 	   	   r1 /= (double) nn;  
           for (j=0; j<nr; j++){
                frq[i][j] += r1[j];   // frequencia relativa
           }
      }
  }          //  fecha loop for 1


  qdat /= (double) nn;
  
  for (ijk=0; ijk<kkk; ijk++){  // abre loop for 2
       rad = kvec[ijk];
    
	   if(TeProgress::instance())
	   {
           if (TeProgress::instance()->wasCancelled())
                 break;
           TeProgress::instance()->setProgress(ijk);
       }

       for (i=0; i<nr; i++){
           for (j=0; j<nr; j++){
                ac[i][j] = exp(-4.0 * pow((dc[i][j] / (double) rad),2.0));  // Eq. (6) de Tango(2000)
           }
       }

       // multiplicacao de matrizes
       hh = ac * w; 
       hh2 = hh * hh; 
       hhh = hh2 * hh; 

       av = 0.0, av2 = 0.0, av3 = 0.0;

       for (i=0; i<nr; i++){
            av += hh[i][i];                       // soma da diagonal da matriz hh
            av2 += hh2[i][i];                     // soma da diagonal da matriz hh2
            av3 += hhh[i][i];                     // soma da diagonal da matriz hhh
       }

       skew1= 2.0*sqrt(2.0)*av3 / pow(av2,1.5);       // Eq. [12] de Tango(2000)
       df1= 8.0 / skew1 / skew1;                  // Eq. [11] de Tango(2000)
       eg1= av;                                   // Eq. [9] de Tango(2000)
       vg1= 2.0 * av2;                            // Eq. [10] de Tango(2000)

       for (j=0; j<nloop; j++){
           qp = frq.Row(j+1);         // j-esima linha da matriz frq (A INDEXAÇAO COMECA EM 1 NESTA FUNCAO!!)
           temp1 = qp.AsColumn() - p;
           ggtt = ( (temp1.t() * ac) * temp1 ) * (double)nn;
           gt1 = ggtt.AsScalar();                 // considera matriz 1x1 como escalar
           stat1 = (gt1-eg1) / sqrt(vg1);
           aprox1 = df1 + sqrt(2.0*df1)*stat1;
           cdfchi ( &which, &pchisq, &qchisq, &aprox1, &df1, &status, &bound ); // cdf chi-quadrado
           pval[j][ijk]= qchisq;                  // Eq.(8) de Tango(2000)
       }

       temp1 = qdat-p;
       ggtt2 = ( ( temp1.t() * ac ) * temp1 ) * (double)nn;
       gt2 = ggtt2.AsScalar();                    // considera matriz 1x1 como escalar
       stat2 = (gt2-eg1) / sqrt(vg1);
       aprox2= df1 + sqrt(2.0*df1)*stat2;
       cdfchi ( &which, &pchisq, &qchisq, &aprox2, &df1, &status, &bound );  // cdf chi-quadrado
       ptan[ijk] = qchisq;                        // Eq.(8) de Tango(2000)

       // zera as matrizes hh, hh2 e hhh
       hh = 0.0; 
       hh2 = 0.0; 
       hhh = 0.0; 

  }             // fecha loop for 2
if (TeProgress::instance())
       TeProgress::instance()->reset();


  // Calculation of (2) test statistic Pmin 
  Pmin = ptan.min();     // Pmin


  // Calcula o Pvalor
  for (i=0; i<nloop; i++){
       pvalrow = pval.Row(i+1); 
       pcc[i] = pvalrow.Minimum();
  }
  for (i=0; i<nloop; i++)
       ZZ[i] = pcc[i];
  ZZ[nloop] = Pmin;
  std::valarray <double> ZZp22 = ZZ [ZZ <= Pmin]; 
  pvalue = (double) ZZp22.size() / (double) (nloop+1);   // adjusted P-value


  // Identifica o lambda otimo
  std::valarray<int> jjj; 
  seq(0, kkk-1, 1, jjj);
  valarray <int> pind = jjj [ptan == Pmin]; 
  lambda_opt= kvec[pind[0]];                             // optimal lambda


  // Calcula a contribuicao de cada area
  for (i=0; i<nr; i++)
      for (j=0; j<nr; j++){
           ac[i][j] = exp(-4.0 * pow((dc[i][j] / (double) lambda_opt),2.0)); 
      }
  temp1 = qdat - p;
  ss = (ac * temp1) * nn;
  for (i=0; i<nr; i++)
       ss[i] = temp1[i]*ss[i];
  ss /= ss.Sum();              
  ss *= 100;                   // percent contribution to C  ( Eq. [14] de Tango(2000) )


  // Ordena o vetor ss segundo a contribuicao de cada area
//  sort_ind(ss, geocode);

  return (0);    
 }    

/////////////////////////////////////////////////////////////////////////////////

// Implementa o metodo MEET descrito em Tango(2000)
int Meet3( const int nloop,  
          const std::valarray<int> &pop, const std::valarray<int> &cases, 
          const std::vector< vector<double> > &latlong, vector<string> &geocode, 
          std::valarray<double> &ZZ, double &pvalue, int &lambda_opt, double &Pmin, ColumnVector &ss ) 
{
  int i, j, ijk, nrk, which = 1, status, rad; 
  double nj, sumpoo, soma, maxdc = 0.0, av, av2, av3, skew1, df1, eg1, vg1; 
  double gt1, stat1, aprox1, gt2, stat2, aprox2, pchisq, qchisq, bound;

  int nr = latlong.size();                          // number of regions 
  int nn = cases.sum();                             // total number of cases 


  // Declaração das matrizes e vetores
  valarray<double> bbb(0.0,nr), pcc(0.0,nloop), cvv;  
  Matrix w(nr,nr);
  Matrix ac(nr,nr);
  Matrix hh(nr,nr);
  Matrix hh2(nr,nr);
  Matrix hhh(nr,nr);
  Matrix dc(nr,nr);
  Matrix frq(nloop,nr);
  Matrix qp;
  Matrix ggtt;
  Matrix ggtt2;
  ColumnVector p(nr);
  ColumnVector qdat(nr);
//  ColumnVector ss(nr);
  ColumnVector sortind(nr);
  ColumnVector temp1;
  ColumnVector p1;
  ColumnVector vv;
  ColumnVector r1;

  p = 0.0;
  dc = 0.0;  
  frq = 0.0;
  qdat= 0.0;
  
  // preenche a matriz de distancias
  geodeticdistance(latlong, dc);

  // encontra a maior distancia entre centroides de areas
  maxdc = dc.Maximum();

  // Gera a sequencia de valores de lambda
  double lambdamax = maxdc / 2.0;

  std::valarray<int> kvec; 
  seq(1, (int) ceil(lambdamax), 5, kvec);  // sequence of lambda values

  int kkk = kvec.size();                   // dimensao do vetor de lambdas

  if(TeProgress::instance())
  {
		string caption = "Tango";
		TeProgress::instance()->setCaption(caption.c_str());
		string msg = "Running Tango's Method. Please, wait!";
		TeProgress::instance()->setMessage(msg);
		TeProgress::instance()->setTotalSteps(kkk);
  }

  valarray<double> ptan(0.0,kkk);
  Matrix pval(nloop,kkk);
  pval = 0.0;
  Matrix pvalrow;

  
  // Calculation of (1) null distribution of test statistic Pmin //

      nj = cases.sum();                      // soma dos casos em todas regioes
      sumpoo = pop.sum();                    // soma da populacao em todas regioes 
      nrk = pop.size();

      p1.ReSize(nrk);
      vv.ReSize(nrk);
      cvv.resize(nrk);
      r1.ReSize(nrk); 

      for (i=0; i<nrk; i++) 
          p1[i] = pop[i] / sumpoo;

      p += ( (nj/(double) nn) * p1 );                 // Eq. [2] de Tango(2000)

      for (i=0; i<nr; i++)
           qdat[i] += (double) cases[i];

      // prenchendo a matriz w
      Matrix diag_p1(nrk,nrk); 
      diagonal(p1, diag_p1); // matriz diagonal(p1)
      
      w += (nj /(double) nn) * diag_p1 - p1*p1.t();

      vv = p1/p1.Sum();
      soma = 0.0; 
      for (i=0; i<nrk; i++) {
           soma += vv[i];
           cvv[i] = soma;           // soma acumulada do vetor vv
      }

      for (i=0; i<nloop; i++){
 	   	   countc( runif((int)nj), cvv, r1 );
 	   	   r1 /= (double) nn;  
           for (j=0; j<nr; j++){
                frq[i][j] += r1[j];   // frequencia relativa
           }
      }


  qdat /= (double) nn;
  
  for (ijk=0; ijk<kkk; ijk++){  // abre loop for 2
       rad = kvec[ijk];
    
	   if(TeProgress::instance())
	   {
           if (TeProgress::instance()->wasCancelled())
                 break;
           TeProgress::instance()->setProgress(ijk);
       }

       for (i=0; i<nr; i++){
           for (j=0; j<nr; j++){
                ac[i][j] = exp(-4.0 * pow((dc[i][j] / (double) rad),2.0));  // Eq. (6) de Tango(2000)
           }
       }

       // multiplicacao de matrizes
       hh = ac * w; 
       hh2 = hh * hh; 
       hhh = hh2 * hh; 

       av = 0.0, av2 = 0.0, av3 = 0.0;

       for (i=0; i<nr; i++){
            av += hh[i][i];                       // soma da diagonal da matriz hh
            av2 += hh2[i][i];                     // soma da diagonal da matriz hh2
            av3 += hhh[i][i];                     // soma da diagonal da matriz hhh
       }

       skew1= 2.0*sqrt(2.0)*av3 / pow(av2,1.5);       // Eq. [12] de Tango(2000)
       df1= 8.0 / skew1 / skew1;                  // Eq. [11] de Tango(2000)
       eg1= av;                                   // Eq. [9] de Tango(2000)
       vg1= 2.0 * av2;                            // Eq. [10] de Tango(2000)

       for (j=0; j<nloop; j++){
           qp = frq.Row(j+1);         // j-esima linha da matriz frq (A INDEXAÇAO COMECA EM 1 NESTA FUNCAO!!)
           temp1 = qp.AsColumn() - p;
           ggtt = ( (temp1.t() * ac) * temp1 ) * (double)nn;
           gt1 = ggtt.AsScalar();                 // considera matriz 1x1 como escalar
           stat1 = (gt1-eg1) / sqrt(vg1);
           aprox1 = df1 + sqrt(2.0*df1)*stat1;
           cdfchi ( &which, &pchisq, &qchisq, &aprox1, &df1, &status, &bound ); // cdf chi-quadrado
           pval[j][ijk]= qchisq;                  // Eq.(8) de Tango(2000)
       }

       temp1 = qdat-p;
       ggtt2 = ( ( temp1.t() * ac ) * temp1 ) * (double)nn;
       gt2 = ggtt2.AsScalar();                    // considera matriz 1x1 como escalar
       stat2 = (gt2-eg1) / sqrt(vg1);
       aprox2= df1 + sqrt(2.0*df1)*stat2;
       cdfchi ( &which, &pchisq, &qchisq, &aprox2, &df1, &status, &bound );  // cdf chi-quadrado
       ptan[ijk] = qchisq;                        // Eq.(8) de Tango(2000)

       // zera as matrizes hh, hh2 e hhh
       hh = 0.0; 
       hh2 = 0.0; 
       hhh = 0.0; 

  }             // fecha loop for 2
if (TeProgress::instance())
       TeProgress::instance()->reset();


  // Calculation of (2) test statistic Pmin 
  Pmin = ptan.min();     // Pmin


  // Calcula o Pvalor
  for (i=0; i<nloop; i++){
       pvalrow = pval.Row(i+1); 
       pcc[i] = pvalrow.Minimum();
  }
  for (i=0; i<nloop; i++)
       ZZ[i] = pcc[i];
  ZZ[nloop] = Pmin;
  std::valarray <double> ZZp22 = ZZ [ZZ <= Pmin]; 
  pvalue = (double) ZZp22.size() / (double) (nloop+1);   // adjusted P-value


  // Identifica o lambda otimo
  std::valarray<int> jjj; 
  seq(0, kkk-1, 1, jjj);
  valarray <int> pind = jjj [ptan == Pmin]; 
  lambda_opt= kvec[pind[0]];                             // optimal lambda


  // Calcula a contribuicao de cada area
  for (i=0; i<nr; i++)
      for (j=0; j<nr; j++){
           ac[i][j] = exp(-4.0 * pow((dc[i][j] / (double) lambda_opt),2.0)); 
      }
  temp1 = qdat - p;
  ss = (ac * temp1) * nn;
  for (i=0; i<nr; i++)
       ss[i] = temp1[i]*ss[i];
  ss /= ss.Sum();              
  ss *= 100;                   // percent contribution to C  ( Eq. [14] de Tango(2000) )


  // Ordena o vetor ss segundo a contribuicao de cada area
//  sort_ind(ss, geocode);

  return (0);    
 }    

/////////////////////////////////////////////////////////////////////////////////

// Implementa o metodo MEET descrito em Tango(2000)
int Meet4( const int nloop,  
          const std::valarray<int> &pop, const std::valarray<int> &cases, 
          const std::vector< vector<double> > &latlong, vector<string> &geocode, 
          std::valarray<double> &ZZ, double &pvalue, int &lambda_opt, double &Pmin, ColumnVector &ss ) 
{
  int i, j, ijk, nrk, which = 1, status, rad; 
  double nj, sumpoo, soma, maxdc = 0.0, av, av2, av3, skew1, df1, eg1, vg1; 
  double gt1, stat1, aprox1, gt2, stat2, aprox2, pchisq, qchisq, bound;

  int nr = latlong.size();                          // number of regions 
  int nn = cases.sum();                             // total number of cases 


  // Declaração das matrizes e vetores
  valarray<double> bbb(0.0,nr), pcc(0.0,nloop), cvv;  
  Matrix D(nr,nr);
  Matrix w(nr,nr);
  Matrix ac(nr,nr);
  Matrix hh(nr,nr);
  Matrix hh2(nr,nr);
  Matrix hhh(nr,nr);
  Matrix dc(nr,nr);
  Matrix frq(nloop,nr);
  Matrix qp;
  Matrix ggtt;
  Matrix ggtt2;
  ColumnVector p(nr);
  ColumnVector qdat(nr);
//  ColumnVector ss(nr);
  ColumnVector sortind(nr);
  ColumnVector temp1;
  ColumnVector p1;
  ColumnVector vv;
  ColumnVector r1;

  p = 0.0;
  dc = 0.0;  
  frq = 0.0;
  qdat= 0.0;
  
  // preenche a matriz de distancias
	for (i = 0; i < nr; i++) {
		for (j = 0; j < i; j++) {
             D[i][j] = sqrt( (latlong[i][0]-latlong[j][0])*(latlong[i][0]-latlong[j][0]) + 
				             (latlong[i][1]-latlong[j][1])*(latlong[i][1]-latlong[j][1]) );
             D[j][i] = D[i][j];
		}
	}


  // encontra a maior distancia entre centroides de areas
  maxdc = dc.Maximum();

  // Gera a sequencia de valores de lambda
  double lambdamax = maxdc / 2.0;

  std::valarray<int> kvec; 
  seq(1, (int) ceil(lambdamax), 5, kvec);  // sequence of lambda values

  int kkk = kvec.size();                   // dimensao do vetor de lambdas

  if(TeProgress::instance())
  {
		string caption = "Tango";
		TeProgress::instance()->setCaption(caption.c_str());
		string msg = "Running Tango's Method. Please, wait!";
		TeProgress::instance()->setMessage(msg);
		TeProgress::instance()->setTotalSteps(kkk);
  }

  valarray<double> ptan(0.0,kkk);
  Matrix pval(nloop,kkk);
  pval = 0.0;
  Matrix pvalrow;

  
  // Calculation of (1) null distribution of test statistic Pmin //

      nj = cases.sum();                      // soma dos casos em todas regioes
      sumpoo = pop.sum();                    // soma da populacao em todas regioes 
      nrk = pop.size();

      p1.ReSize(nrk);
      vv.ReSize(nrk);
      cvv.resize(nrk);
      r1.ReSize(nrk); 

      for (i=0; i<nrk; i++) 
          p1[i] = pop[i] / sumpoo;

      p += ( (nj/(double) nn) * p1 );                 // Eq. [2] de Tango(2000)

      for (i=0; i<nr; i++)
           qdat[i] += (double) cases[i];

      // prenchendo a matriz w
      Matrix diag_p1(nrk,nrk); 
      diagonal(p1, diag_p1); // matriz diagonal(p1)
      
      w += (nj /(double) nn) * diag_p1 - p1*p1.t();

      vv = p1/p1.Sum();
      soma = 0.0; 
      for (i=0; i<nrk; i++) {
           soma += vv[i];
           cvv[i] = soma;           // soma acumulada do vetor vv
      }

      for (i=0; i<nloop; i++){
 	   	   countc( runif((int)nj), cvv, r1 );
 	   	   r1 /= (double) nn;  
           for (j=0; j<nr; j++){
                frq[i][j] += r1[j];   // frequencia relativa
           }
      }


  qdat /= (double) nn;
  
  for (ijk=0; ijk<kkk; ijk++){  // abre loop for 2
       rad = kvec[ijk];
    
	   if(TeProgress::instance())
	   {
           if (TeProgress::instance()->wasCancelled())
                 break;
           TeProgress::instance()->setProgress(ijk);
       }

       for (i=0; i<nr; i++){
           for (j=0; j<nr; j++){
                ac[i][j] = exp(-4.0 * pow((dc[i][j] / (double) rad),2.0));  // Eq. (6) de Tango(2000)
           }
       }

       // multiplicacao de matrizes
       hh = ac * w; 
       hh2 = hh * hh; 
       hhh = hh2 * hh; 

       av = 0.0, av2 = 0.0, av3 = 0.0;

       for (i=0; i<nr; i++){
            av += hh[i][i];                       // soma da diagonal da matriz hh
            av2 += hh2[i][i];                     // soma da diagonal da matriz hh2
            av3 += hhh[i][i];                     // soma da diagonal da matriz hhh
       }

       skew1= 2.0*sqrt(2.0)*av3 / pow(av2,1.5);       // Eq. [12] de Tango(2000)
       df1= 8.0 / skew1 / skew1;                  // Eq. [11] de Tango(2000)
       eg1= av;                                   // Eq. [9] de Tango(2000)
       vg1= 2.0 * av2;                            // Eq. [10] de Tango(2000)

       for (j=0; j<nloop; j++){
           qp = frq.Row(j+1);         // j-esima linha da matriz frq (A INDEXAÇAO COMECA EM 1 NESTA FUNCAO!!)
           temp1 = qp.AsColumn() - p;
           ggtt = ( (temp1.t() * ac) * temp1 ) * (double)nn;
           gt1 = ggtt.AsScalar();                 // considera matriz 1x1 como escalar
           stat1 = (gt1-eg1) / sqrt(vg1);
           aprox1 = df1 + sqrt(2.0*df1)*stat1;
           cdfchi ( &which, &pchisq, &qchisq, &aprox1, &df1, &status, &bound ); // cdf chi-quadrado
           pval[j][ijk]= qchisq;                  // Eq.(8) de Tango(2000)
       }

       temp1 = qdat-p;
       ggtt2 = ( ( temp1.t() * ac ) * temp1 ) * (double)nn;
       gt2 = ggtt2.AsScalar();                    // considera matriz 1x1 como escalar
       stat2 = (gt2-eg1) / sqrt(vg1);
       aprox2= df1 + sqrt(2.0*df1)*stat2;
       cdfchi ( &which, &pchisq, &qchisq, &aprox2, &df1, &status, &bound );  // cdf chi-quadrado
       ptan[ijk] = qchisq;                        // Eq.(8) de Tango(2000)

       // zera as matrizes hh, hh2 e hhh
       hh = 0.0; 
       hh2 = 0.0; 
       hhh = 0.0; 

  }             // fecha loop for 2
if (TeProgress::instance())
       TeProgress::instance()->reset();



  // Calculation of (2) test statistic Pmin 
  Pmin = ptan.min();     // Pmin


  // Calcula o Pvalor
  for (i=0; i<nloop; i++){
       pvalrow = pval.Row(i+1); 
       pcc[i] = pvalrow.Minimum();
  }
  for (i=0; i<nloop; i++)
       ZZ[i] = pcc[i];
  ZZ[nloop] = Pmin;
  std::valarray <double> ZZp22 = ZZ [ZZ <= Pmin]; 
  pvalue = (double) ZZp22.size() / (double) (nloop+1);   // adjusted P-value


  // Identifica o lambda otimo
  std::valarray<int> jjj; 
  seq(0, kkk-1, 1, jjj);
  valarray <int> pind = jjj [ptan == Pmin]; 
  lambda_opt= kvec[pind[0]];                             // optimal lambda


  // Calcula a contribuicao de cada area
  for (i=0; i<nr; i++)
      for (j=0; j<nr; j++){
           ac[i][j] = exp(-4.0 * pow((dc[i][j] / (double) lambda_opt),2.0)); 
      }
  temp1 = qdat - p;
  ss = (ac * temp1) * nn;
  for (i=0; i<nr; i++)
       ss[i] = temp1[i]*ss[i];
  ss /= ss.Sum();              
  ss *= 100;                   // percent contribution to C  ( Eq. [14] de Tango(2000) )


  // Ordena o vetor ss segundo a contribuicao de cada area
//  sort_ind(ss, geocode);

  return (0);    
 }    

/////////////////////////////////////////////////////////////////////////////////
