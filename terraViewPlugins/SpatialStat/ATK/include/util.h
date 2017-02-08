#ifndef __UTILH
#define __UTILH

#include <stdio.h>

//Maximo de caracteres
#define MAXLINE 300

/**
 *Funcao para delete remove area apontada por p
 *e coloca apontador NULL
 **/
void mdelete(void **p);

void mvdelete(void **p);

#define RASSERT(x,m) if (!(x)) {fprintf(stderr,"%s\n", m); exit(0);}

#endif

