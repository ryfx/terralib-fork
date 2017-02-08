
/*! \file TePRUtils.h
	\brief This file contains useful functions for classification 
	\author Karine Reis Ferreira <karine@dpi.inpe.br>
*/

#ifndef  __PR_INTERNAL_PRUTILS_H
#define  __PR_INTERNAL_PRUTILS_H

#include<vector>
#include<string>

#include "TeMatrix.h"
#include "TePRDefines.h"

using namespace std; 

//! Calculates threshold values based on chi table
PR_DLL bool TePRGetThreshold(std::vector<double>& inThresholds, const int& numOfDimentions, std::vector<double>& outThresholds);

//! Invert a covarinace matrix. This function treats inversible matrixes.  
PR_DLL bool TePRGetInverseCovMatrix(TeMatrix& covMatrix, TeMatrix& inverseCovMatrix);

//! Calculates a mean vector of a class from its trainning pattern set 
template<typename trainingPatternSetIterator> bool
TePRCalculateMeanVector(const trainingPatternSetIterator& itBegin, const trainingPatternSetIterator& itEnd, 
					  const std::vector<int>& dimentions, std::vector<double>& means);

//! Calculates a covariance matrix of a class from its trainning pattern set and mean vector 
template<typename trainingPatternSetIterator> bool
TePRCalculateCovMatrix(const trainingPatternSetIterator& itBegin, const trainingPatternSetIterator& itEnd, 
					 const std::vector<int>& dimentions, const std::vector<double>& means, TeMatrix& covarMatrix);


//----------------------- implementation

template<typename trainingPatternSetIterator>  bool
TePRCalculateMeanVector(const trainingPatternSetIterator& itBegin, const trainingPatternSetIterator& itEnd, 
					  const std::vector<int>& dimentions, std::vector<double>& means)
{
	means.clear();
	unsigned int i=0;

	trainingPatternSetIterator itS = itBegin;
			
	//initialize the means with zero value
	for(i=0; i<dimentions.size(); ++i)
		means.push_back(0.);
		
	//for each sample 
	int numSamples = 0;
	while(itS != itEnd)
	{
		//for each value of each sample
		for(i=0; i<itS->size(); ++i)
			means[i] += (*itS)[i]; //sum the values
		++itS;
		++numSamples;
	}
	
	//calculate the mean divided the sum by number of samples
	for(i=0; i<means.size(); ++i)
		means[i] /= numSamples;

	return true;
}

template<typename trainingPatternSetIterator> bool
TePRCalculateCovMatrix(const trainingPatternSetIterator& itBegin, const trainingPatternSetIterator& itEnd, 
					 const std::vector<int>& dimentions, const std::vector<double>& means, TeMatrix& covarMatrix)
{
	unsigned int i, dim; 
	i = dim =0;
	
	//matrix dimentions
	dim = dimentions.size();
		
	//Initialize the matrix with zero values
	covarMatrix.Init(dim, dim, 0.);

	//Get sample set 
	int numSamples = 0;
	for(unsigned int lin=0; lin<dim; ++lin)
	{
		for(unsigned int col=0; col<dim; ++col)
		{
			trainingPatternSetIterator it = itBegin;
			numSamples = 0;
			while(it != itEnd)
			{
				double val = (*it)[lin]- means[lin];
				val *= (*it)[col]- means[col]; 
				covarMatrix(lin, col) += val;  
				++numSamples;
				++it;
			}
		}
	}

	//traverse all matrix and multiply each element by 1/numSamples
	for(i=0; i<dim; ++i)
		for(unsigned j=0; j<dim; ++j)
			covarMatrix(i, j) /= numSamples;
		
	return true;
}


#endif


