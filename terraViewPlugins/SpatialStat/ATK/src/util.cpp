#include "util.h"
//#include "stdafx.h"

void mdelete(void** p) {
	delete *p;
	*p = NULL;
}

void mvdelete(void** p) {
	delete [] *p;
	*p = NULL;
}
