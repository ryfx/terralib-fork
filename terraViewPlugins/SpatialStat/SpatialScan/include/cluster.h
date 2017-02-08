/**************************************************************************************
 ****
 ****  Arquivo com definicoes globais de tipos de dados e funcoes auxiliares
 ****  utilizados para geracao de clusters de formato arbitrario a partir de estruturas
 ****  de vizinhanca espacial
 ****
 ****  Autor: Andrea Tavares
 ****  Data:  25/01/05
 ***************************************************************************************/
#ifndef __SCANCLUSTERH
#define __SCANCLUSTERH


/////////////////////////////////////////////////////////////////////////////////////////
//  TIPOS DE DADOS
/////////////////////////////////////////////////////////////////////////////////////////

/** Tipo para areas. Contem coordenada do centroide, populacao e numero de casos **/
typedef struct ar {
  int     indice; // Identificador unico. Varia de 0 ate NumAreas - 1
  double  pop;    // Populacao da area
  double  casos;  // Numero de casos na area
  double  x, y;   // Coordenadas x e y da area
} TArea;


/** Tipo para arestas entre areas, definindo o grafo de vizinhanca. Sao arestas nao
 ** direcionadas, que alem do indice de origem e destino, contem ainda um peso   **/
typedef struct link {
  int     area1, area2; //Identificador unico das areas que estao contectadas
  double  peso;         //Peso da aresta, varia de significado de acordo com o algoritmo
} TAresta;


/** Tipo para um cluster. Contem o numero de areas e o identificador das areas no cluster.
 ** Possivelmente podera virar uma classe **/
typedef struct cluster {
  int    tamanho;		// Numero de areas no cluster, eh o tamanho do vetor indices
  int*   indices;		// Indices das areas que estao no cluster
  double veross;		// Verossimilhanca do cluster ***Marcelo (acrescentado em 02/02/2005) 
  float  pvalue;		// pvalor ***Marcelo (acrescentado em 13/02/2006) 
} TCluster;


/////////////////////////////////////////////////////////////////////////////////////////
//  CONSTANTES E VARIAVES GLOBAIS
/////////////////////////////////////////////////////////////////////////////////////////

#define  MAX_AREAS     5000
#define  MAX_ARESTAS  50000

/////////////////////////////////////////////////////////////////////////////////////////
//  PROTOTIPOS DE FUNCOES
/////////////////////////////////////////////////////////////////////////////////////////

/** Funcao que calcula a distancia K-L entre as areas de indices id1 e id2.
 ** Parametros
      id1, id2: identificadores das duas areas
      vareas: vetor de descritores das areas
 ** Retorno: distancia K-L
 ** 
 ** A ser implementada pelo Sabino
 **/
extern double DistanciaKL(int id1, int id2, TArea* vAreas);

/** Funcao que calcula a verossimilhanca de um cluster.
 ** Parametros
      clu: apontador para o cluster. O subgrafo induzido pelas areas eh necessariamente conexo.
      vAreas: vetor de descritores das areas.
 ** Retorno: verossimilhanca
 ** 
 ** A ser implementada pelo Marcelo
 **/
extern double VerossimilhancaCluster(TCluster* clu, TArea* vAreas, double TotalPop, double TotalCasos);

/** A fazer: Funcoes para calculo da verossimilhanca para modelo Binomial e de Poisson **/
// extern double BinomialVerossimilhancaCluster(TCluster* clu, TArea* vAreas);
// extern double PoissonVerossimilhancaCluster(TCluster* clu, TArea* vAreas);

/** Funcao para a Leitura das arestas a partir do arquivo **/
extern int VizinhosCluster(TCluster* vizin, TCluster* clu, TAresta* vAresta, int NumAreas, int NumArestas);

/** Funcao para calculo dos vizinhos de um cluster **/
extern int DoubleVizinhosCluster(TCluster* vizin, TCluster* clu, TAresta* vAresta, int NumAreas, int NumArestas);

/** Funcao para Calculo de Verossimilhanca e agregacao do cluster **/
extern double AgregaAreaCluster(TCluster* clu, TCluster* viz, TArea* VetAreas); // Retorna Verossimilhanca

/** Funcao para Calculo de Verossimilhanca e agregacao do cluster caso tenha aumento
    da verossimilhanca em relação à original quando acrescido um dos vizinhos **/
extern double AgregaAreaCluster2(TCluster* clu, TCluster* viz, TArea* VetAreas); // Retorna Verossimilhanca ou -1

/** Funcao para Calculo de Verossimilhanca e agregacao do cluster caso tenha aumento
    da verossimilhanca em relação à original quando acrescido um dos vizinhos **/
extern double AgregaAreaCluster3(TCluster* clu, TCluster* viz, TArea* VetAreas); // Retorna Verossimilhanca ou 0

/** Funcao para Calculo de Verossimilhanca e agregacao do cluster caso tenha um aumento
    da verossimilhanca de pelo menos o parametro "chi" em relação à original quando 
    acrescido um dos vizinhos **/
extern double AgregaAreaClusterChi(TCluster* clu, TCluster* viz, float chi, TArea* VetAreas);

/** Funcao para reproducao de um cluster **/
extern int CopiaCluster(TCluster* orig, TCluster* dest);

/** Funcao para a geracao de uma distribuicao Multinomial de casos **/
extern int multinomial(long int* vtcasos, double* prob, int tam, int casos);

/** Funcao para a geracao de uma distribuicao Multinomial de casos **/
extern int multinomiald(double* vtcasos, double* prob, int tam, int casos);

/** Funcao para indentificacao do numero de elementos na intersecao **/
extern int Intersection(TCluster* first, TCluster* sec);

/** Funcao para alocacao de memoria para vetor **/
extern double* criavetor(int tam);

/** Funcao para alocacao de memoria para matriz **/
extern double** criamatriz(int rows, int cols);

/** Funcao para liberacao de area alocada para matriz **/
extern void liberamatriz(double** mt, int rows);

/** Funcao para determinacao do cluster candidato e seu p-valor **/
extern int ScanClusterBernoulli(TCluster &clu, long int centroides, double* pop, double* casos, double** geo, long int interacoes, double tx );

/** Funcao para detecção de cluster segundo metodo dMST **/
extern int dMSTClusterBernoulli(TCluster &clu, long int NumAreas, long int MonteCarlo, TArea*   VetAreas,
						   long int NumArestas, TAresta* VetArestas, long int LimiteAreas, int TamMin, bool doubly);

/** Funcao para detecção de cluster segundo metodo PatilsDoublyBayesianScan **/


#endif
