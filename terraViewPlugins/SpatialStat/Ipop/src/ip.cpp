#include <stdio.h>
#include <math.h>      //para usar funções matematicas
#include <vector>      //para usar vetores
#include <valarray>
#include <time.h>
#include <IpopPluginWindow.h>
#include "ip.h"
#include <TeProgress.h>

using namespace std;

#define ITMAX 100
#define EPS 3.0e-7
#define FPMIN 1.0e-30


double gammln(double xx)
//returns the value ln(t(xx)) for xx>0
{
double x, y, tmp, ser;
static double cof[6]={76.18009172947146,-86.50532032941677 , 
        24.01409824083091, -1.231739572450155, 
        0.1208650973866179e-2, -0.5395239384953e-5};
int j;

y=x=xx;
tmp=x+5.5;
tmp -=(x+0.5)*log(tmp);
ser=1.000000000190015;
for (j=0;j<=5;j++) ser +=cof[j]/++y;
return -tmp+log(2.5066282746310005*ser/x);
}


void gser(double *gamser, double a, double x, double *gln)
/* Returns the incomplete gamma function P(a,x) evaluated by series representation as gamser.Also returns ln T(a) as gln. */
{
double gammln(double xx);
int n;
double sum,del,ap;

*gln=gammln(a);
if ( x <= 0.0) {
    if (x<0.0) printf("x e menor que 0 na rotina gser");  //x less than 0 in routines gser
    *gamser=0.0;
     return;
     } 
     else
      {
       ap=a;
       del=sum=1.0/a;
       for (n=1;n<=ITMAX;n++)  {
         ++ap;
         del *=x/ap;
         sum += del;
         if  (fabs(del)  < fabs(sum)*EPS) { 
             *gamser= sum*exp(-x+a*log(x)-(*gln));
             return;
         }
     }
     printf("a too large, ITMAX too small in routine gser");
     return;
  }
}


void gcf(double *gammcf, double a, double x, double *gln)
/* Returns the incomplete gamma function Q(a,x) evaluated by its continued fraction representation as gammcf.
Also returns ln t(a) as gln. */
{
double gammln(double xx);
int i;
double an,b,c,d,del,h;

*gln=gammln(a);
b=x+1.0-a;
c=1.0/FPMIN;
d=1.0/b;
h=d;
for (i=1;i<=ITMAX;i++)
   {
     an = -i*(i-a);
     b+=2.0;
     d=an*d+b;
     if (fabs(d) < FPMIN) d=FPMIN;
     c=b+an/c;
     if (fabs(c) < FPMIN) c=FPMIN;
     d=1.0/d;
     del=d*c;
     h*=del;
     if (fabs(del-1.0) < EPS) break;
   }
if (i> ITMAX) 
   {
     printf("Itmax muito grande ou muito pequeno em gcf"); // a too large Itmax too small in gcf
     return;}
     *gammcf= exp(-x+a*log(x)-(*gln))*h;   //Put factors in front.
    }
double gammp(double a, double x)
 {
   void gcf(double *gammcf, double a, double x, double *gln);
   void gser(double *gamser, double a, double x, double *gln);
   double gamser,gammcf,gln;

    if  (x < 0.0 || a <= 0.0){
	  // printf("Rotina inválida nos argumentos de gammp"); //Invalid arguments in routs gammp
	   return(0);
	}
    if  (x < (a+1.0))     {
         gser(&gamser,a,x,&gln);
         return gamser;
    }
	else {
         gcf(&gammcf,a,x,&gln);
         return 1.0-gammcf;
         }
}

double gammq(double a, double x)
{
   void gcf(double *gammcf, double a, double x, double *gln);
   void gser(double *gamser, double a, double x, double *gln);
   double gamser,gammcf,gln;

if (x < 0.0 || a <= 0.0)
   {
    // printf (“Invalid arguments in routs gammq”);
	 return (0);
   }
if (x < (a+1.0)) {
    gser(&gamser,a,x,&gln);
    return 1.0-gamser;
} else {
    gcf(&gammcf,a,x,&gln);
    return gammcf;
}
}
  
double erfc(double x)  
{
 double gammp(double a, double x);
 double gammq(double a, double x);
 return x < 0.0 ? 1.0+gammp(0.5,x*x) : gammq(0.5, x*x);
}


/*Devolve um vetor de tamanho "areas" permutado.
Para a chamada da função foi necessario fornecer o tamanho do vetor*/
void Permut(valarray<double> &n, int m)
{
 int i,j,resultado;
 double b;
 srand(time(NULL));
 for(i =(m-1); i>0; i --)
 {
   resultado=rand()%(m-1);
   j = resultado;
   b = n[i];
   n[i] = n[j];
   n[j] = b;
 }
}

double calculaI_pop(const valarray<double> &x, const valarray<double> &n, const vector< vector <double> > &M, double &S0,const vector<double> &d, double &X, double &b, double &A )
{
 unsigned int i,j;
 double N=0.0,H=0.0,G=0.0,B=0.0,I_pop;
 vector<double> e(x.size(),0.0),z(x.size(),0.0);
 N=n.sum(); 
 X=x.sum(); 
 for (i=0;i<x.size();i++) 
	  e[i]=n[i]/N;
 b=N/X;
 for (i=0;i<x.size();i++){
	  G+=M[i][i]*e[i]; 
	  B+=M[i][i]*d[i];
	  for (j=0;j<x.size();j++){
	  H+=M[i][j]*(e[i]-d[i])*(e[j]-d[j]);
	  A+=M[i][j]*d[i]*d[j];
	  }
 }

 S0=(pow(X,2)*A-X*B);
 I_pop=((pow(N,2)*H)-(N*(1-2*b)*G)-(N*b*B))/(S0*b*(1-b));
 
 return (I_pop);}

double calcula_p_valor(double S0, double X,const vector<double> &d, double &b,const vector< vector<double> > &M,const double &A, Saida_IPOP &saida)
{
double F=0.0,E=0.0,D=0.0,C=0.0, S2, S1, b2, p_valor=0.0, eaux, faux;
long double E_Ipop2=0.0;
unsigned int i,j=0;

for (i=0;i<d.size();i++){
	D+=d[i]*pow(M[i][i],2.0);
	eaux = 0.0;
	faux = 0.0;
	for (j=0;j<d.size();j++){
	     C+=d[i]*d[j]*pow((M[i][j]+M[j][i]),2.0);
		 eaux += pow(d[j]*(M[i][j]+M[j][i]),2.0);
		 faux +=d[j]*(M[i][j]+M[j][i]);
	}
    E += (eaux*d[i]);
	F += (d[i]*M[i][i]*faux);
}  

S1=pow(X,2)*(C/2)-2*X*D;
S2=pow(X,3)*E-4*pow(X,2)*F+4*X*D;
b2=(1/(b*(1-b)))-3;

saida.esp=-1/(X-1);

//Calcula a E(X^2)
E_Ipop2 = X*(pow(X,2)-3*X+3)*S1;
E_Ipop2 = E_Ipop2 - (X*X*S2);
E_Ipop2 = E_Ipop2 + (X*3*pow(S0,2));
E_Ipop2 = E_Ipop2 - (b2*X*(X-1)*S1);
E_Ipop2 = E_Ipop2 + (b2*2*X*S2);
E_Ipop2 = E_Ipop2 - (b2*6*pow(S0,2));
E_Ipop2 = E_Ipop2 / ((X-1)*(X-2)*(X-3)*pow(S0,2));

saida.var = E_Ipop2 - pow(saida.esp,2);
saida.Z= (saida.Ipop - saida.esp) / sqrt(saida.var);
saida.varA = (2*A*A + C/2.0 - E)/(A*A*X*X);
p_valor=0.5*erfc(saida.Z/sqrt(2.0));
return(p_valor);
}

int iFunction(valarray<double> &n,const valarray<double> &x,const vector< vector <double> > &M,const vector<double> &d, const int &m,const int &NPermut, Saida_IPOP &saida)
{
	double S0=0.0, b=0.0, X=0.0,A=0.0,aux;
	int Nge=1, i;
	   
	saida.Ipop=calculaI_pop(x,n,M,S0,d,X,b,A);
	saida.Ipopl = saida.Ipop/b;
	saida.pvalor=calcula_p_valor(S0,X,d,b,M,A,saida);
	
	TeProgress::instance()->reset();
	TeProgress::instance()->setTotalSteps(NPermut);
	TeProgress::instance()->setCaption("Ipop");
	TeProgress::instance()->setMessage("Executing Ipop Function...");

	for (i=0;i<NPermut;i++)
	{
		Permut(n,m);
		aux=calculaI_pop(x,n,M,S0,d,X,b,A);
		if (aux>saida.Ipop)
			Nge++;

		TeProgress::instance()->setProgress(i);

		if(TeProgress::instance()->wasCancelled())
			return 0;
	}
	saida.pMC = (double)Nge/(double)(NPermut+1);

	TeProgress::instance()->reset();

	return(0);
}
