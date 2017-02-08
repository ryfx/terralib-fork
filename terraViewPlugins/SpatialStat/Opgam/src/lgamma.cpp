#include <math.h>

double lgamma(int x) {
	if (x<=2) 
		return(0);
	else {
	  double res=0;
      for (int i=2; i<x; i++)
         res += log((double)i);
      return(res);
	}
}
