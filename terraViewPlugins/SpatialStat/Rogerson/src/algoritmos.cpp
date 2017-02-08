#include "algoritmos.h"
#include <TeProgress.h>
#include <TeUtils.h>


std::vector<SistVigEnt> PermTempo(std::vector<SistVigEnt> Dados, int nEvts)
{
  int numSort,i;
  double t;
  std::vector<SistVigEnt> DadoTemp, DadoNovo;

  DadoTemp.resize(nEvts);
  DadoNovo.resize(nEvts);
  
  for(i=0;i<nEvts;i++)
  {
    DadoTemp[i].x = Dados[i].x;
    DadoTemp[i].y = Dados[i].y;
    DadoTemp[i].t = Dados[i].t;
  }
  for(i=0;i<nEvts;i++)
  {
    DadoNovo[i].x = DadoTemp[i].x;
    DadoNovo[i].y = DadoTemp[i].y;
    DadoNovo[i].t = DadoTemp[i].t;
  }

  srand(clock());

  for(i=0;i<nEvts;i++)
  {
    numSort = rand()%(nEvts-i);
    numSort += i;

    t=DadoNovo[i].t;

    DadoNovo[i].t = DadoTemp[numSort].t;

    DadoTemp[numSort].t = t;
  }

  return(DadoNovo);
}

//---------------------------------------------------------------------------
/* Caucula o Mi Espaco-Temporal do i-esimo elemento */
int CalculaNSt (std::vector< std::vector< short > >MSpaceTime, int ele)
{
 int j;
 int NSt=0;

 for (j=0;j<=ele;j++)
  NSt += MSpaceTime[ele][j];
  //NSt -= 1;
 return(NSt);
}
//---------------------------------------------------------------------------
/* Caucula o Mi Espacial do i-esimo elemento */
int CalculaNS (std::vector< std::vector< short > >MSpace, int ele)
{
 int j;
 int NS=0;

 for (j=0;j<=ele;j++)
  NS += MSpace[ele][j];
  NS -= 1;
 return(NS);
}
//---------------------------------------------------------------------------
/* Caucula o Mi Temporal do i-esimo elemento */
int CalculaNT (std::vector< std::vector< short > > MTime, int ele)
{
 int j;
 int NT = 0;
 
 //for (i=0;i<=nEvts;i++)
 //{
 for (j=0;j<=ele;j++)
	  NT += MTime[ele][j];
	//  NT -= 1;
 return(NT);
}
//---------------------------------------------------------------------------
/* Calcula a Variancia */
double CalculaVar(std::vector< int >NS, int aux, double Ni,  double Soma, int nEvts)
{
  double Vx, a, b, c, d;
  double  fator;
  a = NS[aux-3];
/*  
  fator = (double)(aux - a)/(double)(aux-1);
  b = (double)((nEvts) * ((nEvts-1) * (nEvts-2)));
  c = (a * (a - 1) / b);
  d = ((2 * Ni * a)/(nEvts * (nEvts-1)));
  Vx = ((Soma - 2 * Ni) * c);
  Vx += (d - (d * d));
*/
  d = (double)aux;
  b = (Soma*(a/(d*((d-1)*(d-1))))*(((d-1-a)/(d-2))+a));
  c = (((2*Ni*a)/(d*(d-1)))*((d-1-a)/(d-2))) - (((2*Ni*a)/(d*(d-1)))*((2*Ni*a)/(d*(d-1))));
  Vx = (b + c);                 
           

return(Vx);
}

//---------------------------------------------------------------------------
/* Método de Newton Raphson */
double NRaphson(double arl)
{
  double h, aux, fdf, dif = 1.0;
  int cont = 0;

  aux = 3.0;
  while( ((dif > 0.01) || (dif < -0.01)) && cont < 1000 )
  {
      h = aux;
	  fdf = ((2*(exp(h + 1.166) - h - 2.166) - arl)/(2 * exp(h + 1.166) - 2));
	  aux = h - fdf;
	  dif = aux - h;
	  cont += 1;
  }
  return (aux);
}

//---------------------------------------------------------------------------
//Converte a data de entrada para o formato do programa
/*void ConverteData(int unidade, std::vector<SistVigEnt>& Dados, std::vector<SistVigEnt>& DadosLimiar, int EvLimiar, int nEvts, char *inp)
{
  FILE *fp, *out;
  int i;

  AbrirArquivo(&fp,inp,LEITURA);
  if (unidade == 0)
  {
    for (i=0; i<EvLimiar; i++)
      fscanf(fp,"%lf %lf %lf",&DadosLimiar[i].x,&DadosLimiar[i].y,&DadosLimiar[i].t);
    for (i=EvLimiar; i<nEvts; i++)
      fscanf(fp,"%lf %lf %lf",&Dados[i-EvLimiar].x,&Dados[i-EvLimiar].y,&Dados[i-EvLimiar].t);
  }
  else
  {
    int diab, dia, mesb, mes, anob, ano, tempo;

    fscanf(fp,"%lf %lf %d %d %d",&DadosLimiar[0].x,&DadosLimiar[0].y,&diab,&mesb,&anob);
    DadosLimiar[0].t = 0.0;
    for (i=1; i<EvLimiar; i++)
    {
      tempo = 0;
      fscanf(fp,"%lf %lf %d %d %d",&DadosLimiar[i].x,&DadosLimiar[i].y,&dia,&mes,&ano);
      tempo += (ano-anob)*365;
      tempo += (mes-mesb)*30;
      tempo += (dia-diab);
      DadosLimiar[i].t = (double)tempo;
    }
    for (i=0; i<nEvts-EvLimiar; i++)
    {
      tempo = 0;
      fscanf(fp,"%lf %lf %d %d %d",&Dados[i].x,&Dados[i].y,&dia,&mes,&ano);
      tempo += (ano-anob)*365;
      tempo += (mes-mesb)*30;
      tempo += (dia-diab);
      Dados[i].t = (double)tempo;
    }
    switch(unidade)
    {
       case 1:
          break;
       case 2:
       {
         for (i=0; i<EvLimiar; i++)
           DadosLimiar[i].t /= 7.0;
         for (i=0; i<nEvts-EvLimiar; i++)
           Dados[i].t /= 7.0;
         break;
       }
       case 3:
       {
         for (i=0; i<EvLimiar; i++)
           DadosLimiar[i].t /= 30.0;
         for (i=0; i<nEvts-EvLimiar; i++)
           Dados[i].t /= 30.0;
         break;
       }
       case 4:
       {
         for (i=0; i<EvLimiar; i++)
           DadosLimiar[i].t /= 365.0;
         for (i=0; i<nEvts-EvLimiar; i++)
           Dados[i].t /= 365.0;
         break;
       }
    }
  }
  FecharArquivo(&fp);
}*/
//-----------------------------------------------------------------------------
std::vector<double> SistemadeVigilancia(SVEventLst &ev, std::vector<SistVigEnt> Dados, int nEvts, double k, double TempoC, long double RaioC, bool & wasCancelled)
{
  
  std::vector< std::vector< short > > MSpace, MTime, MSpTi;
  std::vector< double > Zi;
  double Soma, Ni, Var;
  std::vector<double> Si;
  long double pontox, pontoy, DistEucl;
  std::vector< int > NT, NSt, NS;
  int i, j;


//  std::sort(Dados.begin(), Dados.end());
  SVEventLst::size_type n_event = ev.size(); 

  //create the spatio matrix
  MSpace.resize(n_event);
  for(int i = 0; i < n_event; i++)
	MSpace[i].resize(n_event);

 //create the spatio matrix
 MTime.resize(n_event);
 for(int i = 0; i < n_event; i++)
	MTime[i].resize(n_event);
  
 //create the spatio matrix
 MSpTi.resize(n_event);
 for(i = 0; i < n_event; i++)
	MSpTi[i].resize(n_event);
  

 Si.resize(nEvts,-1.0);


 /* Aloca o vetor NSt */
 NSt.resize(nEvts);
   
 NT.resize(nEvts);
   
 NS.resize(nEvts);
  
 Zi.resize(nEvts);
   
 /* Cria as Matrizes: espacial, temporal e a espacial-temporal */
 /* Matriz Espacial */
 double xi, xj, yi, yj;
 /*for (int i=0;i<nEvts;i++)
 {
    for (int j=0;j<nEvts;j++)
    {
		xi= Dados[i].x;
		xj = Dados[j].x;
		yi = Dados[i].y;
		yj = Dados[j].y;
		pontox = (Dados[i].x-Dados[j].x);
		pontoy = (Dados[i].y-Dados[j].y);
		DistEucl = sqrtl((pontox*pontox)+(pontoy*pontoy));
		if((DistEucl < RaioC) && (i != j))
			MSpace[i][j]=1;
		else
			MSpace[i][j]=0;
    }
 }*/

i = 0;
 for( SVEventLst::iterator it = ev.begin(); it != ev.end(); ++it, i++ ) {
  j = 0;
  for( SVEventLst::iterator jt = ev.begin(); jt != ev.end(); ++jt, j++ ) {
	  xi = (*it).x;
	  xj = (*jt).x;
	  yi = (*it).y;
	  yi = (*jt).y;
	  pontox = (*it).x-(*jt).x;
   pontoy = (*it).y-(*jt).y;
   DistEucl = sqrt((pontox*pontox)+(pontoy*pontoy));
   if((DistEucl < RaioC))
    MSpace[i][j]=1;
   else
    MSpace[i][j]=0;
  }
 }

 /* Matriz Temporal */
 i = 0;
 for( SVEventLst::iterator it = ev.begin(); it != ev.end(); ++it, i++ ) {
  j = 0;
  for( SVEventLst::iterator jt = ev.begin(); jt != ev.end(); ++jt, j++ ) {
	  if((abs((*it).t-(*jt).t) < TempoC) && (i != j))
	     MTime[i][j] = 1;
     else
		 MTime[i][j] = 0;
    }
 }
 /*for (int i=0;i<nEvts;i++)
 {
    for (int j=0;j<nEvts;j++)
    {
     if((abs(Dados[i].t - Dados[j].t) < TempoC) && (i != j))
	     MTime[i][j] = 1;
     else
		 MTime[i][j] = 0;
    }
 }
*/
 /* Matriz Espacial-Temporal */
 for (int i=0;i<nEvts;i++)
 {
     for (int j=0;j<nEvts;j++)
     {
	     if((MSpace[i][j] == 1) && (MTime[i][j] == 1))
			MSpTi[i][j] = 1;
		 else
			MSpTi[i][j] = 0;
     }
 }

 
 /*Calcula o N-Tempo, Espaco e Espaco-Tempo para todos os eventos, sendo
   que o tempo de i é dados por a soma até i-1 */

	int progressAux = 0;
	TeProgress::instance()->reset();
	TeProgress::instance()->setCaption("Rogerson");
	TeProgress::instance()->setMessage("Executing...");
	TeProgress::instance()->setTotalSteps(3*nEvts);


 /*Calcula os valores do vetor NT */
 for(int i=0;i<nEvts;i++)
 {
	
    NT[i]= CalculaNT(MTime, i);
	TeProgress::instance()->setProgress(progressAux);
	progressAux++;
	
	if(TeProgress::instance()->wasCancelled())
	{
		wasCancelled = true;
		break;
	}
 } 

 /*Calcula os valores do vetor NSt */
 if(!wasCancelled)
 for(int i=0;i<nEvts;i++)
 {
    NSt[i]= CalculaNSt(MSpTi, i);
	TeProgress::instance()->setProgress(progressAux);
	progressAux++;

	if(TeProgress::instance()->wasCancelled())
	{
		wasCancelled = true;
		break;
	}
	
 } 

 /*Calcula os valores do vetor NS */
 if(!wasCancelled)
 for(int i=0;i<nEvts;i++)
 {
    NS[i]= CalculaNS(MSpace, i);
	TeProgress::instance()->setProgress(progressAux);
	progressAux++;

	if(TeProgress::instance()->wasCancelled())
	{
		wasCancelled = true;
		break;
	}
 }

 TeProgress::instance()->reset();

 if(wasCancelled == true)
 {
	std::vector<double> can;
	return can;
 }
 

   /* Calcula os Zi´s do arquivo de entrada e o grava no vetor */

   Zi[0]=0;
   Zi[1]=0;

   int nTl, l, nTG;
   for(int i=0;i<nEvts;i++)
   {
    /* Calcula o somatório utilizado na Variancia */
    Soma = 0.0;
    nTG = 0;
    for (int j=0;j<=i;j++)
    {
     nTl = 0;
	 int f = NT[j];
     nTG += NT[j];
     for (l=0;l<=j;l++)
      nTl += MTime[j][l];
     Soma += double(nTl * nTl);
    }

    /* Calcula o Z[i] */
    int c = NS[i];
    Ni=(2.0 * nTG/*NT[i]*/ * (double)NS[i])/((double)(i+1) * (double)(i+2));
    Var = CalculaVar(NS,(i+3),nTG,Soma, nEvts);
    if (Var <= 0.0)
     Zi[i] = 0.0;
    else
     Zi[i]= ((double)NSt[i] - (double)Ni - 0.5) / sqrt(Var);


   }
   /* Calcula o Si´s */
   Si[0] = 0;
   for (int i=1;i<nEvts;i++)
   {
    double a, b;
	a = Si[i-1];
	b = Zi[i];
    if((Si[i-1]+Zi[i]-k) <= 0.0)
     Si[i] = 0.0;
    else
     Si[i] = Si[i-1]+Zi[i]-k;
   }

 
   return(Si);

}