/**
 * @file sv.cpp
 * @author Marcos Olveira Prates.
 * @date April 06, 2006
 */

//includes
#include "algoritmos.h"
#include "sv.h"
#include <cmath>

int SistemadeVigilancia(SVEventLst &ev, const double RaioC, const double epslon,
                        std::valarray<double> &R)
{
 size_t i, j, NCj, NumTotEvt, NumEvtCil;
 short **MSpace;
 double pontox, pontoy, DistEucl, Soma, UCj, fator;
 
 //order the list of files
 ev.sort();
   
 SVEventLst::size_type n_event = ev.size();

 //create the spatio matrix
 MSpace = new short* [n_event];
 if( MSpace == NULL )
  return 1;
 for( i = 0; i < n_event; i++ ) {
  MSpace[i] = new short[n_event];
  if( MSpace[i] == NULL ) {
   delete []MSpace;
   return 1;
  }
 }

 //create the output vector
 R.resize(n_event);
 if( R.size() != n_event ) {
  for( i = 0; i < n_event; i++ ) {
   delete []MSpace[i];
  }
  delete []MSpace;
  return 1;
 }

 //populate the spatio matrix with 1 if is close in spatio and 0 if not  
 i = 0;
 for( SVEventLst::iterator it = ev.begin(); it != ev.end(); ++it, i++ ) {
  j = 0;
  for( SVEventLst::iterator jt = ev.begin(); jt != ev.end(); ++jt, j++ ) {
   pontox = (*it).x-(*jt).x;
   pontoy = (*it).y-(*jt).y;
   DistEucl = sqrt((pontox*pontox)+(pontoy*pontoy));
   if((DistEucl < RaioC))
    MSpace[i][j]=1;
   else
    MSpace[i][j]=0;
  }
 }

 //do the calculs to find the output value of each event
 for( i = 0; i < n_event; i++ ) {
  Soma = 0.0;
  for( j = 0; j <= i; j++ ) {
   NCj = CalculaNCj(MSpace,i,j);
   NumTotEvt = ContaEvt(MSpace,i,j);
   NumEvtCil = i-j+1;
   UCj = ((double)NumEvtCil*(double)NumTotEvt)/(double)(i+1);
   fator = 1.0+epslon;
   Soma += (pow(fator,(double)NCj) * exp((-epslon)*UCj));
  }
  R[i] = Soma;
 }

 //clean memory
 for( i = 0; i < n_event; i++ ) {
  delete [] MSpace[i];
 }
 delete [] MSpace;
 return 0;
}

int CalculaLambda(SVEventLst &ev, const double RaioC, const double epslon, std::valarray<double> &R, unsigned int &numObs)
{
 size_t i, j, NCj, NumTotEvt, NumEvtCil;
 short **MSpace;
 double pontox, pontoy, DistEucl, Soma, UCj, fator, lambda, lambdaMax;

 ev.sort();
   
 SVEventLst::size_type n_event = ev.size();

 //create the spatio matrix
 MSpace = new short* [n_event];
 if( MSpace == NULL )
  return 1;
 for( i = 0; i < n_event; i++ ) {
  MSpace[i] = new short[n_event];
  if( MSpace[i] == NULL ) {
   delete []MSpace;
   return 1;
  }
 }

 //create the output vector
 R.resize(n_event);
 if( R.size() != n_event ) {
  for( i = 0; i < n_event; i++ ) {
   delete []MSpace[i];
  }
  delete []MSpace;
  return 1;
 }

 //populate the spatio matrix with 1 if is close in spatio and 0 if not  
 i = 0;
 for( SVEventLst::iterator it = ev.begin(); it != ev.end(); ++it, i++ ) {
  j = 0;
  for( SVEventLst::iterator jt = ev.begin(); jt != ev.end(); ++jt, j++ ) {
   pontox = (*it).x-(*jt).x;
   pontoy = (*it).y-(*jt).y;
   DistEucl = sqrt((pontox*pontox)+(pontoy*pontoy));
   if((DistEucl < RaioC))
    MSpace[i][j]=1;
   else
    MSpace[i][j]=0;
  }
 }

 //do the calculs to find the output value of each event
 
   i = numObs;
   lambdaMax = 0;
   for( j = 0; j <= i; j++ ) {
   NCj = CalculaNCj(MSpace,i,j);
   NumTotEvt = ContaEvt(MSpace,i,j);
   NumEvtCil = i-j+1;
   UCj = ((double)NumEvtCil*(double)NumTotEvt)/(double)(i+1);
   fator = 1.0+epslon;
   lambda = (pow(fator,(double)NCj) * exp((-epslon)*UCj));
   if (lambda > lambdaMax){
	   lambdaMax = lambda;
	   numObs = j;
   }
   }
  
 

 //clean memory
 for( i = 0; i < n_event; i++ ) {
  delete [] MSpace[i];
 }
 delete [] MSpace;
 return 0;
}