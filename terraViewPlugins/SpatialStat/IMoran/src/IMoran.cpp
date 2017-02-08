#include <stdio.h>
#include <math.h>    //para usar fun��es matematicas
#include <vector>    //para usar vetores
#include <valarray>
#include <stdlib.h>  //para n� aleat�rio
#include <time.h>    //para usar o rel�gio (semente)

using namespace std;

int LeTamArquivo(char *arq) // Le o tamanho do arquivo
{
    FILE *fp;
    int cont = 0;
    float bla;
    fp = fopen(arq,"r");
    
    if (!fp)
     printf ("Erro na abertura do arquivo.");
     
     while(!feof(fp))
    {
       fscanf(fp,"%f",&bla);
       cont++;
    }
    fclose(fp);
    return(cont/3);
}
int LeTamArqEnt(char *arq) // Le o tamanho do arquivo
{
    FILE *fp;
    int cont = 0;
    float bla;
    fp = fopen(arq,"r");
    
    if (!fp)
     printf ("Erro na abertura do arquivo.");
     
     while(!feof(fp))
    {
       fscanf(fp,"%f",&bla);
       cont++;
    }
    fclose(fp);
    return(cont/2);
}   
void LeMatriz(char *arq,int cont,vector< vector<double> > &vizin)  // Le a matriz vizinhan�a
{
 FILE *fp;
 int i ,j;
 double aux;
 fp = fopen(arq,"r");
 if (fp == NULL)
   return;
 for(i=0;i<cont;i++)
  for(j=0;j<3;j++){           
   fscanf(fp,"%lf",&aux);
   vizin[i].push_back(aux);
 }  
 fclose(fp);
}
void LeEntrada(char *arq,int cont,valarray<double> &y,valarray<double> &n)  // Le dados de entrada
{
 FILE *fp;
 int i ,j;
 double aux;
 fp = fopen(arq,"r");
 for(i=0;i<cont;i++){
   fscanf(fp,"%lf",&n[i]);
   fscanf(fp,"%lf",&y[i]);
 }  
 fclose(fp);
}    



int main()
{
// Abrir o arquivo de vetores: o n� de areas i, yi(n�de casos ocorridos em i) e ni(n� de pessoas em risco).
// Abrir o arquivo matriz vizinhan�a (cont�m nas duas primeiras colunas pares de �reas vizinhas 
// e na terceira coluna encontra-se o peso referente a proximidade entre elas.
// Recebe o valor de NPermut (n� de permuta��es).

int i, areas, cont;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
double EBI, m=0.0, pvalor=0.0, NPermut=999.0;
char *arq , *ent;

//Arquivos de entrada
arq = "viz.txt";
ent = "casos.txt";
//Matriz de vizinhanca
cont=LeTamArquivo(arq);
vector< vector <double> > vizin(cont);
LeMatriz(arq,cont,vizin);
//Dados de entrada
areas = LeTamArqEnt(ent);
valarray<double> ni(areas), yi(areas);   
vector<double> z(areas);
LeEntrada(ent,areas,yi,ni);


calculaZi(yi,ni,z,m);

EBI=calculaEBI(z,vizin,m);

pvalor=calculaPvalor(z,vizin,NPermut,m,areas,EBI);

printf("\n O EBIMoran e: %lf",EBI);
printf("\n O P-valor e: %lf",pvalor);

int bla=0; scanf("%lf", &bla);
return(0);
}
