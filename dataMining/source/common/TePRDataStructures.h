
#ifndef  __PR_INTERNAL_STRUCTURERS_H
#define  __PR_INTERNAL_STRUCTURERS_H

#include <vector>
#include <map>
#include <string>
#include <math.h>

#include "TeUtils.h"
#include "TeMatrix.h"
#include "TeThread.h"

using namespace std; 

//uma amostra
typedef std::vector<double>  TeClassSample;

//mapeamento de cada lugar geométrico (object_id) para os valores das amostras desse lugar
typedef std::map<std::string, std::vector<TeClassSample> >  TePlaceToSamplesMap;

//mapeamento de cada classe e suas amostras (para os classificadores)
typedef std::map<std::string, std::vector<TeClassSample> >  TeClassSampleSet;

//mapeamento de cada classe para os lugares e suas amostras
typedef std::map<std::string,  TePlaceToSamplesMap > TeClassToSamplesMap;


#endif



