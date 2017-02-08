#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <vector>
#include "funcoes.h"
#include <TeQtProgress.h>


bool operator<(const Distancia& a, const Distancia& b){
   return a.dist < b.dist;}

bool operator<(const Tempo& a, const Tempo& b){
   return a.temp < b.temp;}


double min(std::vector<double> vetor)
  {
  int n = vetor.size();
  double minimo = vetor[0];
  for(int i = 1; i<n; i++)
    if(vetor[i]<minimo)
      minimo = vetor[i];

  return(minimo);
  }

//Cálculo da estatística de teste Jk e DJk
void statJk(const std::vector< std::vector<double> > &dists, 
			const std::vector< std::vector<double> > &distt, int k,
			int linha, std::vector< std::vector<int> > &Jk)
{
	int i, j, l,n, jtemp, nareas = dists.size();
	int troca;
	std::vector<int> temp(k,0);
	std::vector<Distancia> distancias(nareas);
	std::vector<Tempo> tempos(nareas);

	//Ordena de acordo com as distâncias (espaço e tempo)
    for(i=0;i<nareas;i++)
	{
		for (j=0;j<nareas;j++)
		{
			Distancia v;
			v.col = j;
			v.dist = dists[i][j];
			distancias[j]= v;
			Tempo z;
			z.col = j;
			z.temp = distt[i][j];
			tempos[j]=z;
		}
		std::sort(distancias.begin(),distancias.end());
		std::sort(tempos.begin(),tempos.end());
		srand(clock());
		//No caso de vizinhos com a mesma distância no espaço, estipula a ordem aleatoriamente 
		for (j=1;j<nareas;j++)
		{
			n=0;
			jtemp=j;
			while (dists[i][j]==dists[i][jtemp+1])
			{
				n++;
				jtemp++;
			}
			if (n!=0)
			{
                for (l=j;l<(j+n+1);l++)
                {
					troca = distancias[l].col;
					distancias[l].col = distancias[l+(rand()%(n+1+j-l))].col;
					distancias[l+(rand()%(n+1+j-l))].col=troca;
				}
				j+=(n-1);
			}
		}
		srand(clock());
		//No caso de vizinhos com a mesma distância no tempo, estipula a ordem aleatoriamente 
		for (j=1;j<nareas;j++)
		{
			n=0;
			jtemp=j;
			while (distt[i][j]==distt[i][jtemp+1])
			{
				n++;
				jtemp++;
			}
			if (n!=0)
			{
				for (l=j;l<(j+n+1);l++)
				{
					troca = tempos[l].col;
					tempos[l].col = tempos[l+(rand()%(n+1+j-l))].col;
					tempos[l+(rand()%(n+1+j-l))].col=troca;
				}
				j+=(n-1);
			}
		}
		
		//Contagem dos vizinhos mais próximos no tempo E no espaço 
		if (distancias[1].col==tempos[1].col)
		{
			(temp[0]++);
		}
		for(j=2;j<=k;j++)
		{
			for (l=1;l<j;l++)
			{
				if (distancias[j].col==tempos[l].col){
					(temp[j-1]++);
				}
				if (distancias[l].col==tempos[j].col){
					(temp[j-1]++);
				}
			}
			if (distancias[j].col==tempos[j].col){
				(temp[j-1]++);
			}
		}
	}
	//Cálculo da estatística Jk para cada k
	Jk[linha][0]=temp[0];
	if (k>1)
	{
        for (i=1;i<k;i++)
			Jk[linha][i]=temp[i]+Jk[linha][i-1];
	}
}


//Cálculo do p_valor
void calcula(const std::vector< std::vector<double> > &dists, const std::vector< std::vector<double> > &distt, int k,
			 int nsim, std::vector<int> &Jk0, std::vector<int> &DJk, std::vector<double> &pvalJk, double &pvalBonfJk, double &pvalSimesJk)


{
	int i, j,cont,nge, nareas = dists.size();
	std::vector< std::vector<int> > Jk(nsim+1);
	std::vector<double> aleat(nareas);
	std::vector< std::vector<double> > disttemp1(nareas),disttemp2(nareas);
	std::vector<Distancia> distancias;
	std::vector<Tempo> tempos;
	std::vector<double>pvalJkSimes(k);

	DJk.resize(k);
	pvalJk.resize(k);


	if(TeProgress::instance())
		{
			string caption = "Jacquez";
			TeProgress::instance()->setCaption(caption.c_str());
			string msg = "Running Jacquez's KNN Method. Please, wait!";
			TeProgress::instance()->setMessage(msg);
			TeProgress::instance()->setTotalSteps(nsim);

		}
//Permutação para teste de pvalor

        for(j=0;j<nsim+1;j++)
			Jk[j].resize(k);
		srand(clock());
        for(j=0;j<nareas;j++)
		{
			disttemp1[j].resize(nareas);
			disttemp2[j].resize(nareas);
		}


	statJk(dists,distt,k,0,Jk);

	//Cálculo da estatística DJk (diferença entre os Jk's)
	if (k>1)
	{
        for (i=1;i<k;i++)
			DJk[i]=Jk[0][i]-Jk[0][i-1];
	}

	srand(clock());

	//Permutação dos pontos seguindo uma uniforme (0,1) para teste de monte carlo

	for(i=1;i<=nsim;i++)
	{
		std::vector<Distancia> distanciasMC;
        
		if(TeProgress::instance())
		{
               if (TeProgress::instance()->wasCancelled())
                       break;
               TeProgress::instance()->setProgress(i);
        }

		for(j=0;j<nareas;j++)
            aleat[j] = (float)rand()/(float)RAND_MAX;
		for(j=0;j<nareas;j++)
		{
			Distancia v;
			v.col = j;
			v.dist = aleat[j];
			distanciasMC.push_back(v);
		}
		std::sort(distanciasMC.begin(),distanciasMC.end());
		for (j=0;j<nareas;j++)
		{
			// Ordenar matriz pela linha/coluna
            for (cont=0;cont<nareas;cont++)
                disttemp1[cont][j]=distt[cont][distanciasMC[j].col];
		}
		for (j=0;j<nareas;j++)
		{
			// Ordenar matriz pela linha/coluna
            for (cont=0;cont<nareas;cont++)
                disttemp2[j][cont]=disttemp1[distanciasMC[j].col][cont];
		}
		statJk(dists,disttemp2,k,i,Jk);
	}
	if (TeProgress::instance())
       TeProgress::instance()->reset();

	//Cálculo do p_valor
	for (i=0;i<k;i++)
	{
		nge=1;
		for (j=1;j<=nsim;j++)
		{
			if (Jk[j][i]>Jk[0][i]) (nge++);
		}
		pvalJk[i]=(float)(nge)/(float)(nsim+1);
                Jk0.push_back(Jk[0][i]);
		pvalJkSimes[i]=(k+1-(i+1))*pvalJk[i];
	}

    pvalBonfJk = k*min(pvalJk);
	pvalSimesJk = min(pvalJkSimes);

    }


//Cálculo das estatísticas de teste (Jk e DJk) e do p_valor
void jacqueztest(const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &tempo,
         int k, int nsim, std::vector<int> &Jk0, std::vector<int> &DJk, std::vector<double> &pvalJk, double &pvalBonfJk, double &pvalSimesJk)
{

	int i,j, nareas = x.size();
	std::vector<double> pval(k);
	std::vector< std::vector<double> > dists(nareas),distt(nareas);
 
	
//	Calcula distância entre os pontos (espaço e tempo) - dists e distt
    for (i=0;i<nareas;i++)
	{
		dists[i].resize(nareas,0.0);
		distt[i].resize(nareas,0.0);
	}
    for (i=0;i<nareas;i++)
		for (j=i+1;j<nareas;j++)
		{
				dists[i][j]=sqrt(((x[j]-x[i])*(x[j]-x[i]))+((y[j]-y[i])*(y[j]-y[i])));
                distt[i][j]=(fabs(tempo[j]-tempo[i]));
				dists[j][i]=dists[i][j];
				distt[j][i]=distt[i][j];
		}
	
	calcula(dists,distt,k,nsim,Jk0,DJk,pvalJk,pvalBonfJk, pvalSimesJk);
}