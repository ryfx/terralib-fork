#ifndef __BASE_CLASSIFIER_H
#define __BASE_CLASSIFIER_H

#include <vector>
#include <string>
#include <math.h>
#include <list>
#include <map>

#include <TeUtils.h>
#include <TeMatrix.h>
#include <TeThread.h>

using namespace std; 

/* one sample */
typedef vector<double> TeClassSample;

/* a sample set, several samples per label (string) */
typedef map<string, vector<TeClassSample> >  TeClassSampleSet;

#endif

