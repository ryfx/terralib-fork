#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include "normal01.h";
#include "poisson.h";
#include"fatorial.h";


using namespace std;


struct estrutura
 {
 int regiao;
 double variavel;
 vector<int> vizinhos;
 bool highrisk;
 };


int main()
  {
  FILE *filevalor;
  FILE *filevizinho;

  char strval[50], strviz[50];
  int ind, nobj=0, estA = 0, viz = 0, n=0;
  int regiaotrans, viztrans, A;
  float dummy;
  double pcorte, valortrans, y, Vary = 0, EA;
  double CR, CV, VarA, par, pvalorP, pvalorN;


  cout<<"Digite o nome do arquivo com os valores de cada area:";
  gets(strval);

  cout<< endl;

  filevalor = fopen(strval,"r");


  while (fscanf(filevalor,"%lf\n", &dummy) == 1)
     nobj++;

  nobj/=2;

  fclose(filevalor);

  vector<estrutura> Dados(nobj);

  cout<<"Digite o nome do arquivo da vizinhanca:";
  gets(strviz);

  cout << endl;

  cout <<"Digite o ponto de corte:";
  cin >> pcorte;

 filevalor = fopen(strval,"r");

  while(!feof(filevalor))                  // Enquanto não for final do arquivo faça
    {
    fscanf(filevalor,"%d",&regiaotrans);
    Dados[regiaotrans].regiao = regiaotrans;

    fscanf(filevalor,"%lf",&valortrans);
    Dados[regiaotrans].variavel = valortrans;

    if(valortrans >= pcorte)
      {
      Dados[regiaotrans].highrisk = true;
      n++;
      }
    else
      {Dados[regiaotrans].highrisk = false;}

    }

  fclose(filevalor);

  cout << "n: " << n << endl;
  
  filevizinho = fopen(strviz,"r");

  while(!feof(filevizinho))                  // Enquanto não for final do arquivo faça
    {
    fscanf(filevizinho,"%d",&regiaotrans);   // Coloca as posições na variavel de transição região_trans

    fscanf(filevizinho,"%d",&viztrans);
    Dados[regiaotrans].vizinhos.push_back(viztrans);   // Coloca a variável de transição no vetor regiões

    if(Dados[regiaotrans].highrisk == true && Dados[viztrans].highrisk == true)
      estA++;

    viz++;
    }

  fclose(filevizinho);

  
  estA /= 2.0;

  cout<< "Estatistica A: " << estA << endl;

  y = viz/(double)nobj; //número médio de vizinhos por objeto

  cout << "y: " << y << endl;

  for(unsigned int i=0; i<Dados.size(); i++)
    Vary += (Dados[i].vizinhos.size()- y)*(Dados[i].vizinhos.size() - y);

  Vary /=(double)(nobj-1);

  cout<< "Var(Y): " << Vary << endl;


  /*nobj =100;
  n = 14;
  y=4.66;
  Vary = 2.347;
  estA = 12; */

//valor esperado de A

  EA =y*n*(n-1)/(double)(2*(nobj-1));

  cout<<"EA:"<<EA<<endl;
//componente de regularidade
  CR = 1 + 2*(y-1)*(n-2)/(double)(nobj-2) + (nobj*y - 4*y + 2)*(n-2)*(n-3)/(double)(2*(nobj-2)*(nobj-3)) - EA;

  CR = CR*EA;

  cout<<"CR:"<<CR<<endl;
//componente de variabilidade
  CV = fac(n,3)/(double)fac(nobj-1,2) - fac(n,4)/(double)fac(nobj-1,3);
  CV = CV*Vary;

    cout<<"CV:"<<CV<<endl;

  VarA = CR + CV;

  cout<<"VarA:"<<VarA<<endl;

  pvalorP = 1 - AcPoisson(estA,EA);

  par = (estA-EA)/sqrt(VarA);

  cout<<"z-score:"<< par<<endl;
  pvalorN = 1-AcNormal(par);

  cout << "P-valor (Poisson):" << pvalorP<< endl;
  cout << "P-valor (Normal):" << pvalorN<< endl;


  system("pause");

  return 0;

  }





    

