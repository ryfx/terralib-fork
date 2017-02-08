/**
 * @file algrotimos.cpp
 * @author Marcos Oliveira Prates.
 * @date April 06, 2006
 */

#include "algoritmos.h"

// Calculate the number of events centered in xi,yi trough the critic radius given, from 
// the time tj until ti
int CalculaNCj(short **MSpace, const int EvtN, const int EvtJ)
{
 int i;
 int Soma=0;
 for (i=EvtJ;i<=EvtN;i++)
 	 Soma += MSpace[EvtJ][i];
 return(Soma);
}

// Calculate the number of events centered in xi,yi trough the critic radius given
int ContaEvt(short **MSpace, const int EvtN, const int EvtJ)
{
 int i;
 int Soma=0;
 for (i=0;i<=EvtN;i++)
 	 Soma += MSpace[EvtJ][i];
 return(Soma);
}

