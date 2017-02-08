#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>  
#include <vector>      ///para usar vetores
#include <valarray>    ///para usar somatorio
#include "normal01.h"
using namespace std;

double scorefunction(int tipo,vector<double> &casos,vector<double> &esperados,vector<double> &d,double pond );
