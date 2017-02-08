#ifndef FATORIAL_H_
#define FATORIAL_H_
#include<iostream>
#include<math.h>


int fac(int a, int k)
  {
  int retorno=a;

  for(int i=1; i< k; i++)
   {
   retorno*=(a-1);
   a--;
   }

  return(retorno);
  }


#endif
 