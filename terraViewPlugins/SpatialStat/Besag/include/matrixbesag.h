// Library for MatrixBesag operations
// by Marcelo Azevedo Costa
// April 14th, 2003
#ifndef MATRIXBESAG_H
#define MATRIXBESAG_H

 #include <iostream>
 #include <stdlib.h>
 #include <stdio.h>
 #include <time.h>
 #include <string.h>
 #include <vector>

using namespace std;

 class MatrixBesag{
  private:
     int row, col;
  public:
	 vector< vector<double> > data;						// public pointer to data (MatrixBesag)
     //static MatrixBesag *answ;				// pointer to MatrixBesag answer (global)
	 //MatrixBesag *answ;				// pointer to MatrixBesag answer (global)
     MatrixBesag();                		    // Constructors
     MatrixBesag(int,int,int);
     MatrixBesag(int,int);
     MatrixBesag(int,int,char*);
	 MatrixBesag(int,int,vector<double>);
	 MatrixBesag(int,int,vector< vector <double> >);
     // MatrixBesag(char*);					// Load a MatrixBesag and assigns the
     ~MatrixBesag();							// number of rows and columns

     MatrixBesag& operator=(const MatrixBesag &m);
     MatrixBesag& operator+(const MatrixBesag &a);
     MatrixBesag& operator+(double number);
     MatrixBesag& operator-(const MatrixBesag &a);
     MatrixBesag& operator-(double number);
     MatrixBesag& operator*(const MatrixBesag &m);
     MatrixBesag& operator*(double number);

//   Functions
     int rows();				// function to return the number of rows
     int cols();				// function to return the number of columns
     void show();
     MatrixBesag& prod(const MatrixBesag &m);			// MatrixBesag elements product
     MatrixBesag& trans();							// Transpose MatrixBesag
     //MatrixBesag& inv();	  							// MatrixBesag inversion
     void clear();				 				// Clear the MatrixBesag (memory)
     void init(int r, int c);					// Allocates a MatrixBesag
     double sum();								// Sum the elements of the MatrixBesag
     // double norm(const MatrixBesag &m);

     void tofile(char*);	  					// Save MatrixBesag to file (float)
     void tofileint(char*);	  					// Save MatrixBesag to file (int)
 };

 MatrixBesag::MatrixBesag()
  { cout << "\n Non defined MatrixBesag \n "; exit(0); }

 MatrixBesag::MatrixBesag(int r, int c, int s)
  {
   int i,j;
   double aux;

   row = r;
   col = c;
   data.resize(r);		// alocates the number of rows
   //if(data.empty()) {cout<< "\n out of memory \n"; exit(0);}
   //else
   //{
     for(i=0;i<r;i++)
     {
      data[i].resize(c);
      //if(data[i].){ cout << "\n out of memory \n"; exit(0); }
     }
   //}

   if(s==1) aux=1;
   else aux=0;

   for(i=0;i<r;i++){
      for(j=0;j<c;j++){
        if(i==j) { data[i][j]=aux; }
        else { data[i][j]=0; }
      }
   }
  }

// Generate a random MatrixBesag - it needs improvements
 MatrixBesag::MatrixBesag(int r, int c)
  {
   int i,j;
   time_t t;

   srand((unsigned) time(&t));

   row = r;
   col = c;
   data.resize(r);		// alocates the number of rows
   //if(!data) {cout<< "\n out of memory \n"; exit(0);}
   //else
   //{
     for(i=0;i<r;i++)
     {
      data[i].resize(c);
     // if(!data[i]){ cout << "\n out of memory \n"; exit(0); }
     }
   //}

   for(i=0;i<r;i++){
    for(j=0;j<c;j++){
      data[i][j]=(double)rand()/RAND_MAX-0.5;		// random number [-0.5,0.5]
    }
   }
  }

// Load a MatrixBesag from a file
 MatrixBesag::MatrixBesag(int r, int c, char *file_name)
  {
   int i,j;
   FILE *fp;
   float aux;

   row = r;
   col = c;
   data.resize(r);		// alocates the number of rows
   //if(!data) {cout<< "\n out of memory \n"; exit(0);}
   //else
   //{
     for(i=0;i<r;i++)
     {
      data[i].resize(c);
      //if(!data[i]){ cout << "\n out of memory \n"; exit(0); }
     }
   //}
   // Try to load the data from file
   if((fp=fopen(file_name,"r"))==NULL){
     cout<< "\n File: "<< file_name << "could not be oppened";
     exit(1);
   }
   for(i=0;i<r;i++){
    for(j=0;j<c;j++){
      fscanf(fp,"%f",&aux);
      data[i][j] = (double) aux;
      if(feof(fp)){cout<<"\n problem in file reading"; exit(1);}
    }
   }
   fclose(fp);
  }

 MatrixBesag::MatrixBesag(int r,int c ,vector<double> d)
 {
	 int i,j;
	 row = r;
	 col = c;
	 data.resize(r);
	 for(i=0;i<r;i++)
		 data[i].resize(c);
		
	 for(i=0;i<r;i++)
		 for(j=0;j<c;j++)
			 data[i][j] = d[i];
 }

 MatrixBesag::MatrixBesag(int r,int c,vector< vector <double> > d)
 {
	 row = r;
	 col = c;
	 data = d;
 }
 MatrixBesag::~MatrixBesag()					// The destructor
  {
   for(int i=0;i<row;i++) data[i].clear();
   data.clear();
  }

 MatrixBesag& MatrixBesag::operator=(const MatrixBesag& m)
  {
  int i,j;

    if((row!=m.row)&(col!=m.col))
      {cout << "\n Non compatible matrizes (=)"; exit(1);}
    row = m.row;
    col = m.col;
    for(i=0;i<m.row;i++)
      for(j=0;j<m.col;j++)
        data[i][j]=m.data[i][j];
    return *this;
  }

 MatrixBesag& MatrixBesag::operator+(const MatrixBesag &a)
  {
   int i,j;

   if((a.row!=row)|(a.col!=col))
     {cout << "\n Non compatible matrizes (+)"; exit(1);}

   for(i=0;i<row;i++)
     for(j=0;j<col;j++)
        data[i][j] = data[i][j]+a.data[i][j];

   return *this;
  }

 MatrixBesag& MatrixBesag::operator+(double number)
  {
   int i,j;

   for(i=0;i<row;i++)
     for(j=0;j<col;j++)
        data[i][j] = data[i][j]+number;
   
   return *this;
  }

 MatrixBesag& MatrixBesag::operator-(const MatrixBesag &a)
  {
   int i,j;

   if((a.row!=row)|(a.col!=col))
     {cout << "\n Non compatible matrizes (-)"; exit(1);}

   for(i=0;i<row;i++)
     for(j=0;j<col;j++)
        data[i][j] = data[i][j]-a.data[i][j];
   
   return *this;
  }

 MatrixBesag& MatrixBesag::operator-(double number)
  {
   int i,j;

   for(i=0;i<row;i++)
     for(j=0;j<col;j++)
        data[i][j] = data[i][j]-number;
   
   return *this;
  }

 MatrixBesag& MatrixBesag::operator*(const MatrixBesag &a)
  {
   int i,j,k;
   double s;   				// the "sum" variable

   if(col!=a.row)
     {cout << "\n Non compatible matrizes (*)"; exit(1);}

   
   MatrixBesag aux(row,a.col,0);
   for(i=0;i<aux.row;i++)
     for(j=0;j<aux.col;j++){
       s=0;
       for(k=0;k<col;k++)
         s = s+(data[i][k]*a.data[k][j]);
       aux.data[i][j]=s;
     }
   for(i=0;i<row;i++)
       data[i].clear();
	 
   data.resize(aux.row);
   for(i=0;i<row;i++)
	   data[i].resize(aux.col);
   for(i=0;i<row;i++)
	   for(j=0;j<row;j++)
		   data[i][j] = aux.data[i][j];
   
   return *this;
  }

 MatrixBesag& MatrixBesag::operator*(double number)
  {
   int i,j;

   MatrixBesag aux(row,col,0);					// Creates an auxiliary class

   for(i=0;i<row;i++)
     for(j=0;j<col;j++)
        data[i][j] = data[i][j]*number;
   
   return *this;
  }

 int MatrixBesag::rows()
  {
   return row;
  }

 int MatrixBesag::cols()
  {
   return col;
  }

 void MatrixBesag::show()
 {
  for(int r=0;r<row;r++){
     for(int c=0;c<col;c++) { cout <<" "<< data[r][c]; }
       cout << "\n";
  }
 }

 MatrixBesag& MatrixBesag::prod(const MatrixBesag &a)
  {
   int i,j;

   if((a.row!=row)|(a.col!=col))
     {cout << "\n Non compatible matrizes (*=)"; exit(1);}

    for(i=0;i<row;i++)
     for(j=0;j<col;j++)
        data[i][j] = data[i][j]*a.data[i][j];
   
   return *this;
  }

// double MatrixBesag::norm(const MatrixBesag &m)

 void MatrixBesag::tofile(char* file_name)					// Save MatrixBesag to file
  {
   int i,j;
   FILE *fp;

   // Try to open/create file
   if((fp=fopen(file_name,"w"))==NULL){
     cout<< "\n File: "<< file_name << "could not be creatted/oppened";
     exit(1);
   }
   for(i=0;i<row;i++){
    for(j=0;j<col;j++){
      fprintf(fp,"%12.6f ",(float) data[i][j]);	// writting resolution
    }
    fprintf(fp,"\n");
   }
   fclose(fp);
  }

 void MatrixBesag::tofileint(char* file_name)					// Save MatrixBesag to file
  {
   int i,j;
   FILE *fp;

   // Try to open/create file
   if((fp=fopen(file_name,"w"))==NULL){
     cout<< "\n File: "<< file_name << "could not be creatted/oppened";
     exit(1);
   }
   for(i=0;i<row;i++){
    for(j=0;j<col;j++){
      fprintf(fp,"%d ",(int) data[i][j]);	// writting resolution
    }
    fprintf(fp,"\n");
   }
   fclose(fp);
  }



 MatrixBesag& MatrixBesag::trans()		   // Transpose MatrixBesag
  {
   int i,j;

   MatrixBesag aux(col,row,0);					// Creates an auxiliary class

   for(i=0;i<row;i++)
     for(j=0;j<col;j++)
        aux.data[j][i] = data[i][j];
   for(i=0;i<row;i++)
     for(j=0;j<col;j++)
		 data[i][j] = aux.data[i][j];
   
   return *this;
  }

  // Calculates the inverse of a squared MatrixBesag according to the Jordan method
// with full pivot (?)  (com pivotacao total)
/*
 MatrixBesag& MatrixBesag::inv()			// MatrixBesag inversion
 {
  int j,k,m,n,p,q;
  double temp, mul;
  MatrixBesag aux(row,col,0);		// Generates an auxiliary MatrixBesag

  aux = *this;						// Probably there will be problems here

  // Verify if the MatrixBesag is squared 

  if (row != col)
  {
    cout << "\n Non squared MatrixBesag (cannot inverte the MatrixBesag)\n";
    exit(0);
  }

    n = row;            // order of the identity MatrixBesag 
    MatrixBesag I(n,n,1);		// Creates an identity MatrixBesag

    n--;
    for (m=0;m<=n;m++) {         // chooses the pivot A[m,m] 
       if(data[m][m] == 0.0)  {
         data[m][m]=0;
         for (k=m;data[k][m]==0;k++) {
            if (k>=n) {
            cout << "\n Matriz Singular MatrixBesag: Determinant = zero \n";
            exit(0);
            }
         }

         for (j=0;j<=n;j++) {
            temp = data[m][j];        // Change lines m and k 
            data[m][j] = data[k][j];
            data[k][j] = temp;
            temp = I.data[m][j];        // Switch lines m and k 
            I.data[m][j] = I.data[k][j];
            I.data[k][j] = temp;
         }
       }

       for (p=(m+1);p<=n;p++) {   // p is the line below the pivot's line 
          mul = -data[p][m]/data[m][m];  // whose column m must be zeros  

          for (q=m+1;q<=n;q++) {
             if (data[p][q] == data[p][m]) data[p][q] = 0.0;
             else data[p][q] = mul*data[m][q]+data[p][q];
          }
          data[p][m] = 0.0;

          for (q=0;q<=n;q++) {
             I.data[p][q] = mul*I.data[m][q]+I.data[p][q];
          }
       }

       for (p=(m-1);p>=0;p--) {   // p is the line above the pivot's line 
          mul = -data[p][m]/data[m][m];  // whose column must be zeros    

          for (q=m+1;q<=n;q++) {
             if (data[p][q] == data[p][m]) data[p][q] = 0.0;
             else data[p][q] = mul*data[m][q]+data[p][q];
          }
          data[p][m] = 0.0;

          for (q=0;q<=n;q++) {
             I.data[p][q] = mul*I.data[m][q]+I.data[p][q];
          }
       }
       temp = data[m][m];
       for (q=0;q<=n;q++) {
          data[m][q] = data[m][q]/temp;
          I.data[m][q] = I.data[m][q]/temp;
       }
    }

	
	
	answ->clear();
    answ->init(row,col);
    *answ=I;
    *this=aux;
    return *answ;
 }
*/
//
//
//

 void MatrixBesag::clear()			// Clear the MatrixBesag (memory)
  {
   for(int i=0;i<row;i++) data[i].clear();
   data.clear();
   row=0;
   col=0;
   //data=NULL;
  }

 void MatrixBesag::init(int r, int c)		// Allocates a MatrixBesag
  {
   int i,j;

   if(!data.empty()) clear();		// Verify if the MatrixBesag already exists
   row = r;
   col = c;
   data.resize(r);		// alocates the number of rows
   //if(!data) {cout<< "\n out of memory (init)\n"; exit(0);}
   //else
   //{
     for(i=0;i<r;i++)
     {
      data[i].resize(c);
      //if(!data[i]){ cout << "\n out of memory (init)\n"; exit(0); }
     }
   //}

   for(i=0;i<r;i++)
      for(j=0;j<c;j++)
        data[i][j]=0;
  }

 double MatrixBesag::sum()		// Allocates a MatrixBesag
  {
   int i,j;
   double result=0;

   for(i=0;i<row;i++)
      for(j=0;j<col;j++)
        result = result + data[i][j];

   return result;
  }

 // Desired operations
 // sum, incialization, inversion, deduction

 // Create the global variable "answer" and sets the static
 // pointer to its position

  //MatrixBesag answer(1,1,0);					// MatrixBesag "answer"
  //MatrixBesag *MatrixBesag::answ=&answer;


#endif


