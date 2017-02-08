#include "cluster.h"
#include "math.h" 
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

/** Funcao que calcula a verossimilhanca de um cluster - Modelo de Bernoulli
 ** Parametros
      clu: apontador para o cluster. O subgrafo induzido pelas areas eh necessariamente conexo.
      vareas: vetor de descritores das areas.
 ** Retorno: verossimilhanca
 ** 
 ** A ser implementada pelo Marcelo - Implementado em 27/01/2005
 **/
 extern double VerossimilhancaCluster(TCluster* clu, TArea* vAreas, double TotalPop, double TotalCasos)
 {
	 double ClusterPop = 0;			// Popula��o total do cluster
	 double ClusterCasos = 0;		// Casos total do cluster
	 double log_veross;				// Verossimilhanca do cluster
	 int    cont;

	 for(cont=0;cont<clu->tamanho;cont++){	// Contagem da Pop e Casos do Cluster
		 ClusterPop += vAreas[clu->indices[cont]].pop;
		 ClusterCasos += vAreas[clu->indices[cont]].casos;
	 }

	 double p = ClusterCasos/ClusterPop;
	 double r = (TotalCasos-ClusterCasos)/(TotalPop-ClusterPop);

	 if (p > r){
		 log_veross =						// Log Verossimilhanca - Modelo: Binomial
			  ClusterCasos*log(p) + (ClusterPop-ClusterCasos)*log(1-p)
			  + (TotalCasos-ClusterCasos)*log(r) 
			  + (TotalPop-ClusterPop-TotalCasos+ClusterCasos)*log(1-r)
			  - ( TotalCasos*log((double) TotalCasos) 
				  + (TotalPop-TotalCasos)*log((double) TotalPop-TotalCasos) 
				  - TotalPop*log((double) TotalPop) );
	     return(exp(log_veross));
	 }
	 else return(1);
 }

/** A fazer: Funcoes para calculo da verossimilhanca para modelo Binomial e de Poisson **/
// extern double BinomialVerossimilhancaCluster(TCluster* clu, TArea* vAreas);
// extern double PoissonVerossimilhancaCluster(TCluster* clu, TArea* vAreas);

/** Funcao que calcula quais e quantos vizinhos o cluster realmente possui
 ** O objetivo da fun��o e determinar quantas e quais as areas que sao vizinhas
 ** ao cluster
 ** Parametros
      vizin: cluster de vizinhanca que sera preenchido
      clu: apontador para o cluster. O subgrafo induzido pelas areas eh necessariamente conexo.
      vAresta: vetor de descritores das arestas
 ** Retorno: no de vizinhos
 **/
int VizinhosCluster(TCluster* vizin, TCluster* clu, TAresta* vAresta, int NumAreas, int NumArestas)
  {
	  int contArst;				// Contador para aresta
	  int contArea;				// Contador para area
	  int contVizin = 0;		// Contador para numero de areas vizinhas
	  bool BoolAreas[MAX_AREAS];// Vetor de Flag para areas a partir da contagem
								// sobre as arestas

	  // inicializacao do vetor de Bool
	  for(contArea=0;contArea<NumAreas;contArea++){
		  BoolAreas[contArea] = false;
	  }

	  // Identifica as areas que estao conectadas aas areas do cluster
	  for(contArst=0;contArst<NumArestas;contArst++){
		  for(contArea=0;contArea<clu->tamanho;contArea++){
			  if (vAresta[contArst].area1 == clu->indices[contArea]){
				  BoolAreas[vAresta[contArst].area2] = true;
			  }
		  }
	  }

	  // Retira os elementos que ja fazem parte do cluster
	  for(contArea=0;contArea<clu->tamanho;contArea++){
		  BoolAreas[clu->indices[contArea]] = false;
	  }

	  // Conta o numero de elementos do cluster
	  for(contArea=0;contArea<NumAreas;contArea++){
		  if(BoolAreas[contArea] == true) {
			  contVizin++;
		  }
	  }
	   
      vizin->tamanho = contVizin;

	  // Preenche o vetor de indices com as areas vizinhas do cluster
	  contArst = 0;
	  for(contArea=0;((contArea<NumAreas)&&(contArst<contVizin));contArea++){
		  if(BoolAreas[contArea] == true){
			  vizin->indices[contArst] = contArea;
			  contArst++;
		  }
	  }
      
	  return(contVizin);
  }

/** Funcao que calcula quais e quantos vizinhos DUPLOS o cluster realmente possui
 ** O objetivo da fun��o e determinar quantas e quais as areas que sao vizinhas
 ** ao cluster e que estao conectadas a pelo menos duas areas do cluster
 ** Parametros
      vizin: cluster de vizinhanca que sera preenchido
      clu: apontador para o cluster. O subgrafo induzido pelas areas eh necessariamente conexo.
      vAresta: vetor de descritores das arestas
 ** Retorno: no de vizinhos encontrados para o cluster
 **/
int DoubleVizinhosCluster(TCluster* vizin, TCluster* clu, TAresta* vAresta, int NumAreas, int NumArestas)
  {
	  int contArst;				// Contador para aresta
	  int contArea;				// Contador para area
	  int contVizin = 0;		// Contador para numero de areas vizinhas
	  int vetAreas[MAX_AREAS];  // Vetor de Flag para areas a partir da contagem
								// sobre as arestas

	  // inicializacao do vetor de Bool
	  for(contArea=0;contArea<NumAreas;contArea++){
		  vetAreas[contArea] = 0;
	  }

	  // Identifica as areas que estao conectadas aas areas do cluster
	  for(contArst=0;contArst<NumArestas;contArst++){
		  for(contArea=0;contArea<clu->tamanho;contArea++){
			  if (vAresta[contArst].area1 == clu->indices[contArea]){
				  vetAreas[vAresta[contArst].area2]++;
			  }
		  }
	  }

	  // Retira os elementos que ja fazem parte do cluster
	  for(contArea=0;contArea<clu->tamanho;contArea++){
		  vetAreas[clu->indices[contArea]] = 0;
	  }

	  // Conta o numero de elementos do cluster
	  for(contArea=0;contArea<NumAreas;contArea++){
		  if(vetAreas[contArea] != 0) {
			  contVizin++;
		  }
	  }
      vizin->tamanho = contVizin;

	  // Verifica se existe somente uma area no cluster
	  if(clu->tamanho == 1){
		      // Preenche o vetor de indices com as areas vizinhas do cluster
		  	  contArst = 0;
			  for(contArea=0;((contArea<NumAreas)&&(contArst<contVizin));contArea++){
				  if(vetAreas[contArea] != 0){
					  vizin->indices[contArst] = contArea;
					  contArst++;
				  }
			  }
			  return(contVizin);
	  }

	  // Identifica os vizinhos que possuem DoubleConnection com o Cluster Principal
	  else{
		  // Conta o numero de elementos do cluster Doubly Connected
		  contVizin = 0;
		  for(contArea=0;contArea<NumAreas;contArea++){
			  if(vetAreas[contArea] > 1) { contVizin++; }
		  }
		  vizin->tamanho = contVizin;
		  // Preenche o vetor de indices com as areas vizinhas do cluster
		  contArst = 0;
		  for(contArea=0;((contArea<NumAreas)&&(contArst<contVizin));contArea++){
			  if(vetAreas[contArea] > 1){
				  vizin->indices[contArst] = contArea;
				  contArst++;
			  }
		  }
		  return(contVizin);
	  }
}
  
/** Funcao que calcula a verossimilhanca do cluster juntamente com as possiveis
 ** areas a serem agregadas. Ao final agrega a area que apresenta a maior 
 ** verossimilhanca entre as areas vizinhas.
 ** Parametros
      clu: apontador para o cluster. O subgrafo induzido pelas areas eh necessariamente conexo.
      viz: areas vizinhas ao cluster - uma delas sera agregada
 ** Retorno: Verossimilhanca do cluster
 **/
double AgregaAreaCluster(TCluster* clu, TCluster* viz, TArea* VetAreas, double TotalPop, double TotalCasos)
{
	int     contArea;
	int     bestVizin  =  0;
	double  bestVeross = -1;
	double  veross;

	// Atualiza o tamanho do cluster
	clu->tamanho++;

	// Varredura sobre a vizinhanca do cluster
	for(contArea=0;contArea<viz->tamanho;contArea++){
		clu->indices[clu->tamanho - 1] = viz->indices[contArea];
		veross = VerossimilhancaCluster(clu,VetAreas,TotalPop, TotalCasos);
		if(veross>bestVeross){
			bestVeross = veross;
			bestVizin  = viz->indices[contArea];
		}
	}

	// Define o novo cluster - agregado `a area de maior verossimilhanca
	clu->indices[clu->tamanho - 1] = bestVizin;
    clu->veross = bestVeross;

	return(bestVeross);	// Retorna a verossimilhanca do novo cluster
}

/** Funcao que calcula a verossimilihanca do cluster juntamente com as possiveis
 ** areas a serem agregadas. Ao final agrega a area que apresenta a menor 
 ** verossimilhanca entre os vizinhos que apresentam verossimilhanca maior que
 ** o cluster original. A fun��o retorna a verossimilhan�a encontrada ou zero
 ** caso n�o haja nenhuma �rea vizinha com veross maior que o cluster original
 ** Parametros
      clu: apontador para o cluster. O subgrafo induzido pelas areas eh necessariamente conexo.
      viz: areas vizinhas ao cluster - uma delas sera agregada
 ** Retorno: Verossimilhanca do cluster
 **/
double AgregaAreaCluster2(TCluster* clu, TCluster* viz, TArea* VetAreas, double TotalPop, double TotalCasos)
{
	int     contArea;
	int     bestVizin  =   0;
	double  bestVeross =  -1;
	double  MaiorVeross = -1;
	double  veross;
	double  VerossAtual;

	// Calcula a verossimilhanca do cluster atual
    VerossAtual = VerossimilhancaCluster(clu,VetAreas,TotalPop, TotalCasos);

	// Atualiza o tamanho do cluster
	clu->tamanho++;

	// Varredura sobre a vizinhanca do cluster
	for(contArea=0;contArea<viz->tamanho;contArea++){
		clu->indices[clu->tamanho - 1] = viz->indices[contArea];
		veross = VerossimilhancaCluster(clu,VetAreas,TotalPop, TotalCasos);
		if(veross>VerossAtual){										//bestVeross

			// Atribui a maior verossimilhanca caso ainda n�o exista
			if(MaiorVeross == -1)
			{
				MaiorVeross = veross;
				bestVeross  = veross;
				bestVizin   = viz->indices[contArea];
			}

			// Tenta Encontrar a menor verossimilhan�a dentre aquelas maiores
			// que o do cluster original
			else if(veross<MaiorVeross)
				{
					bestVeross = veross;
					bestVizin  = viz->indices[contArea];
					MaiorVeross = veross;
				}
		}
	}

	// Define o novo cluster incluindo a area que aumenta a verossimilhanca
	if(bestVeross != -1)
	{
		clu->indices[clu->tamanho - 1] = bestVizin;
		clu->veross = bestVeross;
		return(bestVeross);	// Retorna a verossimilhanca do novo cluster
	}
	else 
	{
		clu->tamanho--;
		return(-1);
	}
}

/** Funcao que calcula a verossimilhanca do cluster juntamente com as possiveis
 ** areas a serem agregadas. Ao final agrega a area que apresenta verossimilhanca maior que
 ** o cluster original. A fun��o retorna a varia��o da log verossimilhan�a ou 0
 ** caso n�o haja nenhuma �rea vizinha com veross maior que o cluster original
 ** Parametros
      clu: apontador para o cluster. O subgrafo induzido pelas areas eh necessariamente conexo.
      viz: areas vizinhas ao cluster - uma delas sera agregada
 ** Retorno: Verossimilhanca do cluster
 **/
double AgregaAreaCluster3(TCluster* clu, TCluster* viz, TArea* VetAreas, double TotalPop, double TotalCasos)
{
	int     contArea;
	int     bestVizin  =  -1;
	double  actualVeross = VerossimilhancaCluster(clu,VetAreas,TotalPop, TotalCasos); 
	double  bestVeross   = actualVeross;  
	double  veross;

	// Atualiza o tamanho do cluster
	clu->tamanho++;

	// Varredura sobre a vizinhanca do cluster
	for(contArea=0;contArea<viz->tamanho;contArea++){
		clu->indices[clu->tamanho - 1] = viz->indices[contArea];
		veross = VerossimilhancaCluster(clu,VetAreas,TotalPop, TotalCasos);
		if(veross>bestVeross){
			bestVeross = veross;
			bestVizin  = viz->indices[contArea];
		}
	}

	if(bestVizin != -1)
	{
		// Define o novo cluster - agregado `a area de maior verossimilhanca
	    clu->indices[clu->tamanho - 1] = bestVizin;
		clu->veross = bestVeross;
	    return(log(bestVeross)-log(actualVeross));	// Retorna a verossimilhanca do novo cluster
	}
	else{
		clu->tamanho--;
		return(0);
	}
}

/** Funcao que calcula a verossimilhanca do cluster juntamente com as possiveis
 ** areas a serem agregadas. Ao final agrega a area capaz de promover um aumento
 ** de pelo menos "Chi" (variavel definido pelo usu�rio)
 ** A fun��o retorna a varia��o da log verossimilhan�a ou 0
 ** caso n�o haja nenhuma �rea vizinha seja capaz de atender aos requisitos
 ** Parametros
      clu: apontador para o cluster. O subgrafo induzido pelas areas eh necessariamente conexo.
      viz: areas vizinhas ao cluster - uma delas sera agregada
	  chi: float - parametro que define o criterio de variacao na verossimilhanca
 ** Retorno: Verossimilhanca do cluster
 **/
double AgregaAreaClusterChi(TCluster* clu, TCluster* viz, float chi, TArea* VetAreas, double TotalPop, double TotalCasos)
{
	int     contArea;
	int     bestVizin  =  -1;
	double  actualVeross = VerossimilhancaCluster(clu,VetAreas,TotalPop, TotalCasos); 
	double  bestVeross   = actualVeross;  
	double  veross;

	// Atualiza o tamanho do cluster
	clu->tamanho++;

	// Varredura sobre a vizinhanca do cluster
	for(contArea=0;contArea<viz->tamanho;contArea++){
		clu->indices[clu->tamanho - 1] = viz->indices[contArea];
		veross = VerossimilhancaCluster(clu,VetAreas,TotalPop, TotalCasos);
		if(veross>bestVeross){
			bestVeross = veross;
			bestVizin  = viz->indices[contArea];
		}
	}

	// Verifica tambem a variacao na verossimilhanca
	if( (bestVizin != -1)&&((log(bestVeross)-log(actualVeross))>=chi) )
	{
		// Define o novo cluster - agregado `a area de maior verossimilhanca
	    clu->indices[clu->tamanho - 1] = bestVizin;
		clu->veross = bestVeross;
	    return(bestVeross);	// Retorna a verossimilhanca do novo cluster
	}
	else{
		clu->tamanho--;
		return(0);
	}
}

/** Funcao para copia completamente a estrutura de um cluster para outro 
 ** E' assumido que ambos os clusters ja tenham sido previamente 
 ** criados.
 ** Parametros
      orig: cluster de origem, que sera copiado (origem)
      dest: cluster que recebera a copia (destino)
 ** Retorno: Tamanho do cluster copiado
 **/
int CopiaCluster(TCluster* orig, TCluster* dest)
{

	dest->tamanho = orig->tamanho;
	dest->veross  = orig->veross;

	// Faz a copia dos elementos do cluster de origem no cluster destino
	for(int cont=0;cont<dest->tamanho;cont++){
		dest->indices[cont] = orig->indices[cont];
	}

	return(dest->tamanho);
}

/** Funcao para a geracao de um vetor de casos sob Ho
 ** Parametros
 **     vtcasos : Vetor da contagem de casos
 **		prob  : vetor de probabilidades cuja soma dos elementos e' 1;
 **		tam   : tamanho do vetor de probabilidades
 **     casos : numero de casos a serem distribuidos 
 ** Retorno: apontador para o vetor de casos (inteiros)
 **/
int multinomial(long int* vtcasos, double* prob, int tam, int casos)
{
	double  acumul;					// armazena a soma acumulada das probabilidades
	double  aleat;					// Elemento aleatorio
	int     cont;

	// Verifica se a soma acumulada � unit�ria
	acumul = prob[0];
	for(cont=1;cont<tam;cont++){
		acumul+=prob[cont];
	}

	if(fabs(acumul - 1) < 1e-12){ // Verifica se a soma e' proxima de "1"
		// Inicializa o vetor de casos com zeros
		for(cont=0;cont<tam;cont++){
			vtcasos[cont] = 0;
		}

		// Preenche o vetor de casos "aleatoriamente"
		for(int contcasos=0;contcasos<casos;contcasos++){
			aleat = (double) rand()/RAND_MAX; // Uniforme[0,1]
			cont  = 0;
			acumul = prob[0];
			while((acumul<aleat)&&(cont<(tam-1))){	// ESTOU AQUI
				acumul+=prob[cont+1];
				cont++;
			}
			vtcasos[cont]++;
		}
		return(1);
	}
	else return(0);
}

/** Funcao para a geracao de um vetor de casos sob Ho
 ** Parametros
 **     vtcasos : Vetor da contagem de casos (double)
 **		prob  : vetor de probabilidades cuja soma dos elementos e' 1;
 **		tam   : tamanho do vetor de probabilidades
 **     casos : numero de casos a serem distribuidos 
 ** Retorno: apontador para o vetor de casos (inteiros)
 **/
int multinomiald(double* vtcasos, double* prob, int tam, int casos)
{
	double  acumul;					// armazena a soma acumulada das probabilidades
	double  aleat;					// Elemento aleatorio
	int     cont;

	// Verifica se a soma acumulada � unit�ria
	acumul = prob[0];
	for(cont=1;cont<tam;cont++){
		acumul+=prob[cont];
	}

	if(fabs(acumul - 1) < 1e-12){ // Verifica se a soma e' proxima de "1"
		// Inicializa o vetor de casos com zeros
		for(cont=0;cont<tam;cont++){
			vtcasos[cont] = 0;
		}

		// Preenche o vetor de casos "aleatoriamente"
		for(int contcasos=0;contcasos<casos;contcasos++){
			aleat = (double) rand()/RAND_MAX; // Uniforme[0,1]
			cont  = 0;
			acumul = prob[0];
			while((acumul<aleat)&&(cont<(tam-1))){	// ESTOU AQUI
				acumul+=prob[cont+1];
				cont++;
			}
			vtcasos[cont]++;
		}
		return(1);
	}
	else return(0);
}

/** Funcao para verificar se os clusters possuem interse�ao 
      first: primeiro cluster
      sec: segundo cluster
 ** Retorno: Tamanho da interse��o
 **/
int Intersection(TCluster* first, TCluster* sec)
{

	int  i=0;				// Intersection size
	char vt[MAX_AREAS];		// Auxiliary vector

	// Initialize the auxiliary vector
	for(int cont=0;cont<MAX_AREAS;cont++){ vt[cont] = 0; }


	// Faz a copia dos elementos do cluster de origem no cluster destino
	for(int cont=0;cont<first->tamanho;cont++){
		vt[first->indices[cont]] = 1;
	}

	// Realiza a contagem dos elementos na interse��o
	for(int cont=0;cont<sec->tamanho;cont++){
		if(vt[sec->indices[cont]] == 1){
			i++;
		}
	}

	return(i);
}

 /** Funcao para alocacao de memoria para vetor **/
 double* criavetor(int tam)
 {
	 double *vt;

	 vt = new double[tam];
	 if(!vt)
		 return(NULL);
	 else
		 return(vt);
 }

 /** Funcao para alocacao de memoria para matriz **/
 double** criamatriz(int rows, int cols)
 {
	 double **mt;
	 int i;

	 mt = new double*[rows];
	 if(!mt)
		 return(NULL);
	 else{
		 for(i=0;i<rows;i++){
			 mt[i] = new double[cols];
			 if(!mt[i]){
				 return(NULL);
				 break;
			 }
		 }
		 return(mt);
	 }
 }

 /** Funcao para liberacao de area alocada para matriz **/
 void liberamatriz(double** mt, int rows)
 {
	 int i;
	 for(i=0;i<rows;i++)
		 delete [] mt[i];
	 delete [] mt;
 }

 /** Ordena��o da matriz				**/
 void ordenacao(double** dist, double** index, int tam)
 {
  int x,y,k;
  int min_indice, max_indice;
  double minimo, maximo, aux;

  for(k=0;k<tam;k++)			// Para cada linha da matriz
  {

    for(x=0;x<tam/2;x++)
    {
      minimo = dist[k][x];
      maximo = dist[k][tam-x-1];
      min_indice = x;
      max_indice = tam-x-1;
      for(y=x;y<tam-x;y++)
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
      dist[k][max_indice] = dist[k][tam-x-1];
      dist[k][tam-x-1] = aux;
      aux = index[k][max_indice];
      index[k][max_indice] = index[k][tam-x-1];
      index[k][tam-x-1] = aux;
    }
  }
 }


/** Funcao que retorna o cluster encontrado segundo o metodo scan utilizando a verossimilhan�a de Poisson
 ** Parametros	
 **		cluster		: cluster encontrado
 **		centroides	: numero de centroides na regiao de estudo
 **		pop			: vetor de popula��o
 **		casos		: vetor de casos
 **		geo			: matrix de coordenadas dos centroides
 **		interacoes	: numero de iteracoes para simula��o Monte Carlo
 **		tx			: proporcao maxima para varredura da populacao (default 20%) - Tamanho do Cluster
 ** Retorno: inteiro que define se houve (1) ou nao erro (0) - Pode ser expandido para discriminar erros
 **/
int ScanClusterBernoulli(TCluster &clu, long int centroides, double* pop, double* casos, double** geo, 
						 long int iteracoes, double tx )
{

	/** Parametros obrigatorios para a gera��o da distribui��o Multinomial **/
	/** para a simula��o de Monte Carlo									**/
	time_t   t_multinom;	// Variavel de tempo - para geracao de numeros aleatorios

	// multinomial - Inicializa o gerador aleatorio &t_multinom
	srand((unsigned) time(&t_multinom));
	/** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -**/


	// Declara��o dos contadores
	long int i,j,k,cont,cont2;

	// Declara��o das matrizes e vetores

	double *v_lambda	= criavetor(iteracoes);					// Vetor de lambdas
	double **m_dist		= criamatriz(centroides,centroides);	// Matriz de dist�ncia
	double **m_index	= criamatriz(centroides,centroides);	// Matriz de indices
	double *probi		= new double[centroides];				// Vetor de probabilidades
	long int *dest		= new long int[centroides];				// Vetor destino de contagem

	// Fazer tratamento de alocacao - Se houver problema a funcao retorna 1
	if(!v_lambda && !m_dist && !probi && !dest) return(1);


	// Vari�veis para armazenamento de casos e indiv�duos em risco
	double		C, N, cz, nz;		// Casos Total, Populacao Total, Casos cluster, Populacao Cluster
	double		p,r;				// Probabilidades dentro e fora do cluster
	double		Max_pop;			// M�xima contagem da popula��o (Limite de busca)
	double		L, Lo, lambda;		// Lambdas - estat�sticas de teste
	long int	lambda_x, lambda_y;	// Indice da linha e coluna do cluster mais significativo
	double		lambda_p, lambda_r;	// Valores de probabilidade dos centr. mais signific.
	double		lambda_aux;			// Variavel auxiliar para lambda
	double		aux_L = 0;			// Variavel auxiliar tambem para lambda

	// Vari�veis auxiliares
	double aux;

	// Preenchimento das matrizes m_dist e m_index
	for(i=0;i<centroides;i++){
		for(j=0;j<centroides;j++){
		m_index[i][j] = (double) j;
		m_dist[i][j] = 0;
		}
	}

	for(i=1;i<centroides;i++){
		for(j=0;j<i;j++){		
		aux = (geo[0][i]-geo[0][j])*(geo[0][i]-geo[0][j]);				// aux = (geo[i][0]-geo[j][0])*(geo[i][0]-geo[j][0]);
		aux = aux + (geo[1][i]-geo[1][j])*(geo[1][i]-geo[1][j]);		// aux = aux + (geo[i][1]-geo[j][1])*(geo[i][1]-geo[j][1]);
		aux = sqrt(aux);
		m_dist[i][j] = aux;
		m_dist[j][i] = aux;
		}
	}

	// Ordena��o das matrizes m_dist e m_index para varredura das areas
	ordenacao(m_dist,m_index,centroides);

	C = 0;									// Inicializa��o da variavel Populacao Total
	for(j=0;j<centroides;j++) C+=casos[j];	// Calcula o n�mero total de casos
	N = 0;									// Inicializa��o da variavel Casos Total
	for(j=0;j<centroides;j++) N+=pop[j];	// Calcula a populacao total
	Max_pop = tx*N;							// Varredura m�xima de 20% da popula��o
	
  
	// Calculo do Lo - Bernoulli
	Lo =  C*log((double) C) + (N-C)*log((double) N-C) - N*log((double) N);

	// Teste de scan sob a hip�tese nula para obter, utilizando um
	// procedimento de simula��o Monte Carlo, a distribui��o exata
	// de lambda condicionada ao n�mero total de casos observado C

	for(cont=0;cont<centroides;cont++){ probi[cont] = pop[cont]/N; }

	// Loop de repeti��o : intera��es de Monte Carlo
	for(j=0;j<iteracoes;j++){
		lambda_aux = -1;

   		// Gera a distribui��o multinomial
		multinomial(dest,probi,centroides,(int) C);

		for(k=0;k<centroides;k++){
			cz = 0;
			nz = 0;
			for(i=0;i<(centroides-1);i++){
				cont = (int) m_index[k][i];
				cz = cz + (double) dest[cont];
				nz = nz + pop[cont];

				p = cz/nz;
				r = (C-cz)/(N-nz);			// Cuidado : Divis�o por zero - N�o Tratado - Improvavel de ocorrer

				if(p<=r){
					L = Lo;
					aux = 0;
				}
				else	// Calculo de L - Bernoulli ou Poisson
				{
					// Calculo do log da razao de verossimilhanca com o modelo de Bernoulli
					L = cz*log(p)+(nz-cz)*log(1-p)+(C-cz)*log(r)+(N-nz-C+cz)*log(1-r);
					aux = (L-Lo);		// log-lambda
				}

				if(aux > lambda_aux)
					lambda_aux = aux;
				if(nz>Max_pop)		// Quebra o Loop se a popula��o interna � maior que 20% da total
					break;
			}
		}

		// Armazenar uma matriz com os valores de lambda (log)
		v_lambda[j] = lambda_aux;

	}
  
	// Teste de scan para a determina��o do valor (lambda) para o
	// conglomerado mais veross�mil, que corresponde ao maior valor
	// de lambda inserido no conjunto de dados

	lambda = -1;					// Valor inicial - OBS: lambda � sempre positivo
	for(k=0;k<centroides;k++){
		cz = 0;
		nz = 0;
		for(i=0;i<(centroides-1);i++){
			cont = (int) m_index[k][i];
			cz += casos[cont];
			nz += pop[cont];

			p = cz/nz;
			r = (C-cz)/(N-nz);			// Cuidado : Divis�o por zero - N�o Tratado - Improvavel de ocorrer

			if(p<=r){
				L = Lo;
				aux = 0;
			}
			else{
				// Calculo da razao de verossimilhanca com o modelo de Bernoulli
				L = cz*log(p)+(nz-cz)*log(1-p)+(C-cz)*log(r)+(N-nz-C+cz)*log(1-r);
	   			aux = (L-Lo);		// lambda
			}


			if(aux > lambda){		// Armazena as informacoes do cluster de maior veross. (log)
				lambda = aux;
				lambda_x = k;
				lambda_y = i;
				lambda_p = p;
				lambda_r = r;
			}

			if(nz>Max_pop)			// Quebra o Loop se a popula��o interna � maior que 20% da total
				break;
		}
	}


    cont2 = 0;
    for(j=0;j<iteracoes;j++){		// Loop para contagem sob a simula��o de Monte Carlo
      if(v_lambda[j] >= lambda) 	// C�lculo do p_valor
        cont2 += 1;
	}

	// Armazena as principais informa��es em um vetor
    clu.pvalue  = (float)(1 + cont2)/(iteracoes + 1);		// p-valor Monte Carlo
	clu.tamanho = lambda_y + 1;								// Tamanho do cluster em centroides
	clu.veross  = lambda;									// log-verossimilhanca do cluster
    //geo[lambda_x][0];										// Coordenada x do centro do cluster
    //geo[lambda_x][1];										// Coordenada y do centro do cluster
    //m_dist[lambda_x][lambda_y];							// Raio do Cluster
    //lambda_p;												// Probabilidade dentro do cluster
    //lambda_r;												// Probabilidade fora do cluster
    //lambda;												// Estat�stica de teste - Logaritmo de lambda

	// Armazena os indices do cluster encontrado
    for(k=0;k<=lambda_y;k++)	clu.indices[k] = (int)m_index[lambda_x][k];

   // Libera a mem�ria dos vetores
	delete [] probi;
	delete [] dest;
	delete [] v_lambda;
	liberamatriz(m_dist,centroides);
	liberamatriz(m_index,centroides);

	// Retorna zero se a execu��o foi realizada com sucesso
	return(0);
 }

 /** Funcao que retorna o cluster encontrado segundo o metodo scan utilizando a verossimilhan�a de Poisson
 ** Parametros	
 **		cluster		: cluster encontrado (Passado por refer�ncia)
 **		NumAreas	: numero de centroides na regiao de estudo (Areas)
 **		MonteCarlo	: numero de iteracoes de Monte Carlo
 **		VetAreas	: Vetor com as informa��es das �reas: pop, casos, indice, coord x e y (centroide) e casos
 **		NumArestas	: numero de arestas
 **     VetArestas  : Vetor com informa��o de vizinhanca - Arestas
 **		MaxAreas	: Tamanho M�ximo do cluster
 **     TamMin      : Tamanho M�nimo do cluster
 ** Retorno: inteiro que define se houve (1) ou nao erro (0) - Pode ser expandido para discriminar erros
 **/
int dMSTClusterBernoulli(TCluster &clu, long int NumAreas, long int MonteCarlo, TArea*   VetAreas,
						   long int NumArestas, TAresta* VetArestas, long int LimiteAreas, int TamMin, bool doubly)
{

	/* - - - Variaveis - - - */
	TCluster  clustScan;		// Cluster de varredura sobre todas as areas conexas
	TCluster  vizinClust;		// Vizinhos do cluster
	TCluster  bestClust;		// Cluster mais verossimil
	TCluster  SecClust;			// Cluster secund�rio
	TCluster  ThirdClust;		// Cluster Terceario
	TCluster  bestClustMC;		// Cluster mais verossimil via Monte Carlo
	double*   vtprob;			// Vetor de probabilidades de cada area sob Ho
	double*	  vtcasos;			// Vetor de casos em cada area sob Ho
	double    TotalPop   = 0;	// Total de Populacao na Regiao de estudo	
	double    TotalCasos = 0;	// Total de Casos na Regiao de estudo		

	int	cont, cnt, cntarea;		// Contadores
	time_t   t_multinom;		// Variavel de tempo - para geracao de numeros aleatorios
	// multinomial - Inicializa o gerador aleatorio &t_multinom
	srand((unsigned) time(&t_multinom));

	// Selecao da Opcao Doubly - Metodo DoubleVizinhosCluster ou VizinhosCluster
	int (*VizClu)(TCluster*,TCluster*,TAresta*,int,int)	= VizinhosCluster;	// Declaracao do ponteiro para funcao
	if(doubly)	VizClu = DoubleVizinhosCluster;							// Altera o ponteiro caso "doubly"

	// Alocacao de vetores
	vtcasos = new double[NumAreas];		// Vetor de casos MC
	vtprob  = new double[NumAreas];		// Vetor de probabilidades (proporcional a pop.)

	// Calculo da Populacao Total e Total de Casos na Regiao
	for(cont=0;cont<NumAreas;cont++){
		TotalPop   += VetAreas[cont].pop;
		TotalCasos += VetAreas[cont].casos;
	}

	// Define a opcao doubly (ponteiro para funcao)



	// Inicializa os apontadores
	bestClust.indices  = new int[LimiteAreas];	// Cria o vetor de indices...
	SecClust.indices   = new int[LimiteAreas];  // Inicializa o secundario
	ThirdClust.indices = new int[LimiteAreas];  // Inicializa o terceiro


	// Inicializa o vizinClust - estrutura que armazena os vizinhos do cluster
	vizinClust.indices = new int[NumAreas];	// Cria o vetor de indices...
	// Inicializa o vetor de indices do clustScan
	clustScan.indices = new int[LimiteAreas];	// Cria o vetor de indices...

	// Teste de consistencia
	if(!vtcasos && !vtprob && !bestClust.indices && !SecClust.indices &&
		!ThirdClust.indices && !vizinClust.indices && !clustScan.indices) 
		return(1);

	// Inicializa o bestCluster - estrutura que ira armazenar o cluster mais
	// verossimil - inicializado com os dados da 1a area
	bestClust.tamanho     = 1;					// Cluster com uma area
	SecClust.tamanho      = 1;
	ThirdClust.tamanho    = 1;
	bestClust.indices[0]  = 0;					// Apontando para a 1a area
	SecClust.indices[0]   = 0;					// Apontando para a 1a area
	ThirdClust.indices[0] = 0;					// Apontando para a 1a area
		

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -	
	for(cont=1;cont<TamMin;cont++){
		// Calcula os vizinhos do cluster de varredura
		VizinhosCluster(&vizinClust,&bestClust,VetArestas,NumAreas,NumArestas);
		// Agrega Vizinho
		AgregaAreaCluster(&bestClust,&vizinClust,VetAreas,TotalPop,TotalCasos);
	}
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	// Calcula a verossimilhanca do bestCluster e dos clusters secundarios
	bestClust.veross  = VerossimilhancaCluster(&bestClust,VetAreas,TotalPop,TotalCasos);
	SecClust.veross   = VerossimilhancaCluster(&SecClust,VetAreas,TotalPop,TotalCasos);
	ThirdClust.veross = VerossimilhancaCluster(&ThirdClust,VetAreas,TotalPop,TotalCasos);

	// Inicia a Varredura iniciando sobre todas as areas da regiao
	// e crescendo ate' que o limite de popula��o ou no. de areas do
	// cluster de varredura seja atingido
	for(cntarea=0;cntarea<NumAreas;cntarea++){

		// Inicializa o cluster de varredura
		clustScan.tamanho = 1;						// Cluster com uma area
		clustScan.indices[0] = cntarea;				// Apontando para a area cntarea

		// Calcula a verossimilhanca do cluster de varredura
		clustScan.veross = VerossimilhancaCluster(&clustScan,VetAreas,TotalPop,TotalCasos);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -	
		for(cont=1;cont<TamMin;cont++){
			// Calcula os vizinhos do cluster de varredura
			VizinhosCluster(&vizinClust,&clustScan,VetArestas,NumAreas,NumArestas);
			// Agrega Vizinho
			AgregaAreaCluster(&clustScan,&vizinClust,VetAreas,TotalPop,TotalCasos);
		}
		// Verifica se o cluster � melhor que besClust, caso afirmativo define
		// o novo bestCluster
		if(clustScan.veross > bestClust.veross)	CopiaCluster(&clustScan,&bestClust);

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		while( (clustScan.tamanho<LimiteAreas) &&
			(*VizClu)(&vizinClust,&clustScan,VetArestas,NumAreas,NumArestas) ){
			//VizinhosCluster(&vizinClust,&clustScan,VetArestas,NumAreas,NumArestas) ){
			//DoubleVizinhosCluster(&vizinClust,&clustScan,VetArestas) ){
			if(!AgregaAreaCluster3(&clustScan,&vizinClust,VetAreas,TotalPop,TotalCasos)){
				if(clustScan.veross>bestClust.veross){
					CopiaCluster(&clustScan,&bestClust);
				}
				// Tratamento dos clusters secundarios
				else{
					if((clustScan.veross>SecClust.veross)&&
						(!Intersection(&clustScan,&bestClust))){
						CopiaCluster(&clustScan,&SecClust);
					}
					else{
						if((clustScan.veross>ThirdClust.veross)&&
							(!Intersection(&clustScan,&SecClust))&&
							(!Intersection(&clustScan,&bestClust))){
							CopiaCluster(&clustScan,&ThirdClust);
						}
					}
				}
				break;
			}
		}
	}
	// Armazena o cluster, a verossimilhanca e zero o p_valor (MC)
	clu.tamanho = bestClust.tamanho;
	clu.veross  = log(bestClust.veross);
	clu.pvalue  = 0;
	for(cntarea=0;cntarea<bestClust.tamanho;cntarea++)
		clu.indices[cntarea] = bestClust.indices[cntarea];
	
	/**	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
	 **	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
						S I M U L A C A O   M O N T E   C A R L O
	 **	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
	 **	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **/
	bestClust.pvalue  = 0;
	SecClust.pvalue   = 0;
	ThirdClust.pvalue = 0;
	
	bestClustMC.indices = new int[LimiteAreas];	// Cria o vetor de indices...	

	//Inicializa o vetor de probabilidades com a proporcao da populacao em cada area
	for(cont=0;cont<NumAreas;cont++){
		vtprob[cont] = (double) VetAreas[cont].pop/TotalPop;
	}

	// Funcao para Gera��o  do p_valor via monte carlo
	// Casos sao gerados aleatoriamento segundo a proporcao de populacao em cada
	// area de estudo
	// Armazenamento em arquivo do bestCluster
	for(cont=0;cont<MonteCarlo;cont++){			// Varredura sobre os cen�rios Monte Carlo

		multinomiald(vtcasos, vtprob, NumAreas,(int) TotalCasos); // TotalCasos e' double

		// Atribui os casos aas areas
		for(cntarea=0;cntarea<NumAreas;cntarea++){
			VetAreas[cntarea].casos = vtcasos[cntarea];
		}

		// Inicializa o bestClustMC - estrutura que ira armazenar o cluster mais
		// verossimil - inicializado com os dados da 1a area
		bestClustMC.tamanho = 1;					// Cluster com uma area
		bestClustMC.indices[0] = 0;					// Apontando para a 1a area

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -	
		for(cnt=1;cnt<TamMin;cnt++){
			// Calcula os vizinhos do cluster de varredura
			VizinhosCluster(&vizinClust,&bestClustMC,VetArestas,NumAreas,NumArestas);
			// Agrega Vizinho
			AgregaAreaCluster(&bestClustMC,&vizinClust,VetAreas,TotalPop,TotalCasos);
		}
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

		// Calcula a verossimilhanca do bestClustMC
		bestClustMC.veross = VerossimilhancaCluster(&bestClustMC,VetAreas,TotalPop,TotalCasos);

		// Inicia a Varredura iniciando sobre todas as areas da regiao
		// e crescendo ate' que o limite de popula��o ou no. de areas do
		// cluster de varredura seja atingido
		for(cntarea=0;cntarea<NumAreas;cntarea++){

			// Inicializa o cluster de varredura
			clustScan.tamanho    = 1;			// Cluster com uma area
			clustScan.indices[0] = cntarea;		// Apontando para a area cntarea

			// Calcula a verossimilhanca do cluster de varredura
			clustScan.veross = VerossimilhancaCluster(&clustScan,VetAreas,TotalPop,TotalCasos);

			// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -	
			for(cnt=1;cnt<TamMin;cnt++){
				// Calcula os vizinhos do cluster de varredura
				VizinhosCluster(&vizinClust,&clustScan,VetArestas,NumAreas,NumArestas);
				// Agrega Vizinho
				AgregaAreaCluster(&clustScan,&vizinClust,VetAreas,TotalPop,TotalCasos);
			// Verifica se o cluster � melhor que besClust, caso afirmativo define
			// o novo bestCluster
			}
			if(clustScan.veross > bestClustMC.veross){
				CopiaCluster(&clustScan,&bestClustMC);
			}
			// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
			while( (clustScan.tamanho<LimiteAreas) &&
				(*VizClu)(&vizinClust,&clustScan,VetArestas,NumAreas,NumArestas) ){
				//VizinhosCluster(&vizinClust,&clustScan,VetArestas,NumAreas,NumArestas) ){
				//DoubleVizinhosCluster(&vizinClust,&clustScan,VetArestas) ){
				if(!AgregaAreaCluster3(&clustScan,&vizinClust,VetAreas,TotalPop,TotalCasos)){
					if(clustScan.veross>bestClustMC.veross){
						CopiaCluster(&clustScan,&bestClustMC);
					}
					else{
					break;
					}
				}
			}
		}

		// Verifica a Veross Simulada com a real e atualiza o pvalor
		if(log(bestClustMC.veross)>log(bestClust.veross)) { bestClust.pvalue++; }
		if(log(bestClustMC.veross)>log(SecClust.veross))  { SecClust.pvalue++; }
		if(log(bestClustMC.veross)>log(ThirdClust.veross)){ ThirdClust.pvalue++; }
		
		// Verifica a Veross Simulada com a real e atualiza o pvalor
		if(log(bestClustMC.veross)>clu.veross)	clu.pvalue++;
	
		// Imprime os resultados da simula��o Monte Carlo
		printf("\n MC %d %d %f ",cont,bestClustMC.tamanho,log(bestClustMC.veross));			

	}
	bestClust.pvalue  = (bestClust.pvalue + 1)/(MonteCarlo + 1);
	SecClust.pvalue   = (SecClust.pvalue + 1)/(MonteCarlo + 1);
	ThirdClust.pvalue = (ThirdClust.pvalue + 1)/(MonteCarlo + 1);
    
	clu.pvalue = (clu.pvalue + 1)/(MonteCarlo + 1);

	delete [] clustScan.indices;
	delete [] bestClust.indices;
	delete [] bestClustMC.indices;
	delete [] vizinClust.indices;
	delete [] vtprob;
	delete [] vtcasos;

	return(0);					// Retorna zero caso a simula��o tenha sido completada sem erro
}
