#ifndef POISSON_H_
#define POISSON_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * The author of this software is Catherine Loader, c@herine.net
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose without fee is hereby granted, with the exceptions noted below,
 * and provided that this entire notice is included in all copies of any
 * software which is or includes a copy or modification of this software
 * and in all copies of the supporting documentation for such software.
 * THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY.  IN PARTICULAR, NEITHER THE AUTHOR NOR LUCENT TECHNOLOGIES
 * MAKE ANY REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE
 * MERCHANTABILITY OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
 */


/* NTYPE is the type used for the n and x arguments.
For 32-bit integers, the maximum n is 2^31-1=2147483647.
If larger n is required, NTYPE must be double.
*/

typedef int NTYPE;
#define PI2 6.283185307179586476925286
#define S0 0.083333333333333333333 /* 1/12 */
#define S1 0.00277777777777777777778 /* 1/360 */
#define S2 0.00079365079365079365079365 /* 1/1260 */
#define S3 0.000595238095238095238095238 /* 1/1680 */
#define S4 0.0008417508417508417508417508 /* 1/1188 */

static double sfe[16] = {
0, 0.081061466795327258219670264,
0.041340695955409294093822081, 0.0276779256849983391487892927,
0.020790672103765093111522771, 0.0166446911898211921631948653,
0.013876128823070747998745727, 0.0118967099458917700950557241,
0.010411265261972096497478567, 0.0092554621827127329177286366,
0.008330563433362871256469318, 0.0075736754879518407949720242,
0.006942840107209529865664152, 0.0064089941880042070684396310,
0.005951370112758847735624416, 0.0055547335519628013710386899
};

/* stirlerr(n) = log(n!) - log( sqrt(2*pi*n)*(n/e)^n ) */
double stirlerr(NTYPE n)
 {
 double nn;
 if (n<16) return(sfe[(int)n]);
 nn = (double)n;
 nn = nn*nn;
 if (n>500) return((S0-S1/nn)/n);
 if (n>80) return((S0-(S1-S2/nn)/nn)/n);
 if (n>35) return((S0-(S1-(S2-S3/nn)/nn)/nn)/n);
 return((S0-(S1-(S2-(S3-S4/nn)/nn)/nn)/nn)/n);
 }

/* Evaluate the deviance term
bd0(x,np) = x log(x/np) + np - x
*/

double bd0(NTYPE x, double np)
 {
 double ej, s, s1, v;
 int j;
 if (fabs(x-np)<0.1*(x+np))
  {
  s = (x-np)*(x-np)/(x+np);
  v = (x-np)/(x+np);
  ej = 2*x*v;

  for (j=1; ;j++)
   {
   ej *= v*v;
   s1 = s+ej/(2*j+1);
   if (s1==s) return(s1);
   s = s1;
   }
  }
 return(x*log(x/np)+np-x);
 }


double dpois(NTYPE x, double lb)
 {
 if (lb==0) return( (x==0) ? 1.0 : 0.0);
 if (x==0) return(exp(-lb));
 return(exp(-stirlerr(x)-bd0(x,lb))/sqrt(PI2*x));
 }


double AcPoisson(double x, double lb)
 {
  int xnew = (int)x;
  double prob = 0.0;

  for(int i=0; i<xnew; i++)
    prob += dpois(i,lb);

  return(prob);
 }


#endif
