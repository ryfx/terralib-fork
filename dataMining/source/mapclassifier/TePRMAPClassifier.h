
/*! \file TePRMAPClassifier.h
    \brief This file contains structures and functions of a MAP 
	(Maximum A Posteriori Probability) classifier for geographical data 
	(vector or raster).  
	\author Karine Reis Ferreira <karine@dpi.inpe.br>
*/
#ifndef  __PR_INTERNAL_MAPCLASSIFIER_H
#define  __PR_INTERNAL_MAPCLASSIFIER_H


#include <vector>
#include <string>

#include "TeMatrix.h"
#include "TePRAbstractClassifier.h"
#include "TePRUtils.h"

using namespace std; 


/*! \class TeMAPClassifierParams
    \brief This class contains parameters which are required by MAP Classifier.
*/
class PR_DLL TePRMAPClassifierParams
{

public:

	//! Constructor used when the priori probabilities are known
	TePRMAPClassifierParams(const int& numClasses, const std::vector<double>& thresholds, const std::vector<double>& prioriProbs) : 
			numClasses_(numClasses), thresholds_(thresholds), prioriProbs_(prioriProbs), calcPrioriProbs_(false)
	{ }

	//! Constructor used when the priori probabilities must be calculated
	TePRMAPClassifierParams(const int& numClasses, const std::vector<double>& thresholds, const int& numIterations, 
		const double& acceptDiff, const double& percentOfClassifiedPatterns ) : 
			numClasses_(numClasses), thresholds_(thresholds), numIterations_(numIterations), 
				acceptableDiff_(acceptDiff), percentOfClassifiedPatterns_(percentOfClassifiedPatterns), calcPrioriProbs_(true)
	{ }

public:

	int								numClasses_;    //!< Number of classes
	std::vector< double >			thresholds_;	//!< A threshould value for each class. Values from 0 to 100. Possible values: 75., 90., 95., 99., 99.9
	std::vector< double >			prioriProbs_;	//!< Priori probabilities, one for each class. Values from 0 to 1  
	std::vector< double >			logPrioriProbs_;//!< Logarithm of priori probabilities. They are calculated by the classifier.
	
	int								numIterations_;  //!< Maximum number of iterations used to calculate the priori prababilities 
	double							acceptableDiff_; //!< Acceptable difference between two iterations when it is calculating the priori probabilities. Possible values: 0 a 1
	double							percentOfClassifiedPatterns_; //!< Percent of patterns that must be classified during the priori probability computation. Possible values: 12.5, 25., 50.0, 100. 
	
	bool							calcPrioriProbs_;
	std::vector< double >			thresholdsChi_;
	std::vector< vector<double> >	meanVectors_;
	std::vector< TeMatrix >			covarMatrixes_;
	std::vector< double >			logDetCovMatrix_;
	std::vector< TeMatrix >			inverseCovarMatrixes_;
};


class PR_DLL TePRMAPClassifier : public TePRAbstractClassifier
{

protected:

	TePRMAPClassifierParams		params_;  //!< MAP classifier parameters

public:

	//! Construct
	TePRMAPClassifier(const TePRMAPClassifierParams& p) : params_(p) 
	{ }

	//! Sets MAP classifier parameters
	void setMapClassifierParams(const TePRMAPClassifierParams& p)
	{	params_ = p; }


	//! Gets MAP classifier parameters
	TePRMAPClassifierParams& getMapClassifierParams()
	{	return params_; }
	

	//! Trains the classifier based on training pattern set
	/*!
      \param itBegin		iterator that points to the first training pattern
	  \param itEnd		iterator that points to the last training pattern
	  \param dimentions	dimentions of the training patterns which must be considered in the training 
	*/
	template<typename trainingPatternSetIterator> 
		bool train(	trainingPatternSetIterator& itBegin, trainingPatternSetIterator& itEnd, 
					const std::vector<int>& dimentions);

	//! Classifies the a pattern set
	/*!
      \param itBegin			iterator that points to the first pattern that will be classified
	  \param itEnd			iterator that points to the last pattern that will be classified
	  \param dimentions		dimentions of the patterns which must be considered in the classification 
	  \param classifyResult	the result class associated to each pattern
	*/
	template<typename patternSetIterator>
		bool classify(	patternSetIterator& itBegin, patternSetIterator& itEnd, 
						const std::vector<int>& dimentions, list<int>& classifyResult);

	//! Calculate priori probabilities based on pattern set 
	template<typename patternSetIterator> 
		bool calculatePrioriProbabilities(	patternSetIterator& itBegin, patternSetIterator& itEnd, 
											const std::vector<int>& dimentions);
};


template<typename trainingPatternSetIterator> bool 
TePRMAPClassifier::train(trainingPatternSetIterator& itBegin, trainingPatternSetIterator& itEnd, 
					const std::vector<int>& dimentions)
{
	unsigned int dim = dimentions.size();
	unsigned int i = 0;
	trainingPatternSetIterator it = itBegin;
	
	//------ step 0: Get threshold values 

	//Each class must have a associated treshould 
	if(params_.thresholds_.size() != params_.numClasses_)
	{
		setErrorMessage("Each class must have a associated treshould!");
		return false;
	}

	TePRGetThreshold(params_.thresholds_, dim, params_.thresholdsChi_);
	if(!params_.calcPrioriProbs_)
	{
		for(unsigned int i=0; i<params_.prioriProbs_.size(); ++i)
			params_.logPrioriProbs_.push_back(log(params_.prioriProbs_[i]));  
	}

	//------ step 1: calculate the mean vector and covariance matrix for each class
	while(it!=itEnd)
	{
		//mean vector
		std::vector<double> vecTemp;
		if(!TePRCalculateMeanVector(it->second.begin(), it->second.end(), dimentions, vecTemp))
		{
			setErrorMessage("Error calculating mean vector!");
			return false;
		}

		params_.meanVectors_.push_back(vecTemp);

		//covariance matrix
		TeMatrix covMatrix;
		if(!TePRCalculateCovMatrix(it->second.begin(), it->second.end(), dimentions, vecTemp, covMatrix))
		{
			setErrorMessage("Error calculating covariance matrix!");
			return false;
		}
		params_.covarMatrixes_.push_back(covMatrix);
		++it;
	}

	//Discriminant function is: 
	// gi(x) = log p(wi) - ( 0.5 * log Det(CovMati) ) - (0.5 (x-Meani)T * InvCovMati * (x-Meani))
	//
	// logPrioriProbs is a vector of: log p(wi)	
	// logDetCovMatrix is a vector of: -( 0.5 * log Det(CovMati) )	
	// mahaDistance is a vector of: -( 0.5 * (x-Meani)T * InvCovMati * (x-Meani))
	//
	// gi(x) = logPrioriProbs[i] + logDetCovMatrix[i] + mahaDistance[i]
		
	//------ step 2: calculate the constant factor to each class and inverse matrix
	//the logDetCovMatrix is = -(1/2) ln (Determ CovMatrix)
	bool useDiagonal = false;
	for(i=0; i< params_.covarMatrixes_.size(); ++i)
	{
		//Get the determinant value
		double det = params_.covarMatrixes_[i].Determinant();
		if(det<=0.0)
			params_.logDetCovMatrix_.push_back(0.);
		else
			params_.logDetCovMatrix_.push_back((-0.5)*log(det));

		//Get the inverse matrix
		TeMatrix aux1;
		if(!TePRGetInverseCovMatrix(params_.covarMatrixes_[i], aux1))
		{
			setErrorMessage("Error calculating inverse matrix!");
			return false;
		}
		params_.inverseCovarMatrixes_.push_back(aux1);
	}

	setStatus(TePRTrainedClassifier);
	return true;
}

template<typename patternSetIterator> bool 
TePRMAPClassifier::classify(patternSetIterator& itBegin, patternSetIterator& itEnd, 
						const std::vector<int>& dimentions, list<int>& classifyResult)
{
	unsigned int dim = dimentions.size();
	unsigned int numClasses = params_.numClasses_;
	unsigned int i, j, z; 
	i=j=z=0;
	
	//------ step 1: calculate the priori probabilities for each class
	if(params_.calcPrioriProbs_)
		calculatePrioriProbabilities(itBegin, itEnd, dimentions);
	else if(params_.prioriProbs_.empty())
	{
		double p = 1.0/numClasses;
		for(i=0; i<numClasses; ++i)
			params_.logPrioriProbs_.push_back(log(p));
	}

	//------ step 2: classification
	unsigned int nCol, nLin;
	patternSetIterator it = itBegin; 
	classifyResult.clear();
	while(it!=itEnd)
	{
		//Get the attribute values of each dimention 
		vector<double> x;
		for(j=0; j<dim; ++j)
			x.push_back((*it)[dimentions[j]]); 
			
		//verify what class is nearest
		double resMahalDist = 0.;
		double resDiscFunction = 0.;
		int resClass = 0;
		for(i=0; i<numClasses; ++i)
		{
            TeMatrix xMinusMean;
			xMinusMean.Init(1, (int)x.size(), (double)0.);
			for(j=0; j < x.size(); ++j)
				xMinusMean(0, j) = x[j]-params_.meanVectors_[i][j];

			//calculate the mahalanobis distance: (x-mean)T * CovMatrizInvert * (x-mean)
			double auxDiscFunction = 0.;
			double auxMahalDist = 0.;
			TeMatrix aux;
			aux = xMinusMean * params_.inverseCovarMatrixes_[i];
			for(nCol=0; nCol<dim; ++nCol)
				auxMahalDist += aux(0, nCol) * xMinusMean(0, nCol); 
			auxDiscFunction = params_.logPrioriProbs_[i] + params_.logDetCovMatrix_[i] - 0.5 * auxMahalDist;
									
			// Discriminant function: g1(x) > g2(x)  classify x in class 1
			if(i==0 || auxDiscFunction>resDiscFunction)
			{
				resClass = i;
				resDiscFunction = auxDiscFunction;
				resMahalDist = auxMahalDist;
			}
		}

		//verify if it is smaller than the threshold
		double t = params_.thresholdsChi_[resClass];
		if( resMahalDist > t )
			classifyResult.push_back(-1); //indefined class
		else
			classifyResult.push_back(resClass); //indefined class
		++it;
	}

	return true;
}


template<typename patternSetIterator> bool
TePRMAPClassifier::calculatePrioriProbabilities(patternSetIterator& itBegin, patternSetIterator& itEnd, const std::vector<int>& dimentions)
{
	bool flag = true;
	unsigned int dim = dimentions.size();
	unsigned int numClasses = params_.numClasses_;
	unsigned int i, j, z, iter; 
	i=j=z=iter=0;
	
	vector<int> numElementsByClass; 
	int			numClassifiedElements = 0;

	//initialize the priori probs
	params_.logPrioriProbs_.clear();
	params_.prioriProbs_.clear();
	double p = 1.0/numClasses;
	for(i=0; i<numClasses; ++i)
	{
		params_.prioriProbs_.push_back(p);
		params_.logPrioriProbs_.push_back(log(p));
		numElementsByClass.push_back(0);
	}

	while((int)iter<params_.numIterations_ && flag)
	{
		unsigned int nCol, nLin;
		patternSetIterator it = itBegin; 
		while(it!=itEnd)
		{
			//Get the attribute values of each dimention 
			vector<double> x;
			for(j=0; j<dim; ++j)
				x.push_back((*it)[dimentions[j]]); 
				
			//verify what class is nearest
			double resMahalDist = 0.;
			double resDiscFunction = 0.;
			int resClass = 0;
			for(i=0; i<numClasses; ++i)
			{
				TeMatrix xMinusMean;
				xMinusMean.Init(1, (int)x.size(), (double)0.);
				for(j=0; j < x.size(); ++j)
					xMinusMean(0, j) = x[j]-params_.meanVectors_[i][j];

				//calculate the mahalanobis distance: (x-mean)T * CovMatrizInvert * (x-mean)
				double auxDiscFunction = 0.;
				double auxMahalDist = 0.;
				TeMatrix aux;
				aux = xMinusMean * params_.inverseCovarMatrixes_[i];
				for(nCol=0; nCol<dim; ++nCol)
					auxMahalDist += aux(0, nCol) * xMinusMean(0, nCol); 
				auxDiscFunction = params_.logPrioriProbs_[i] + params_.logDetCovMatrix_[i] - 0.5 * auxMahalDist;
										
				// Discriminant function: g1(x) > g2(x)  classify x in class 1
				if(i==0 || auxDiscFunction>resDiscFunction)
				{
					resClass = i;
					resDiscFunction = auxDiscFunction;
					resMahalDist = auxMahalDist;
				}
			}
			
			//verify if is smaller than the threshold
			double t = params_.thresholdsChi_[resClass];
			if( resMahalDist <= t )
			{
				numElementsByClass[resClass] += 1;
				++numClassifiedElements;
			}
				
			//12.5, 25., 50.0, 100. 
			if(params_.percentOfClassifiedPatterns_==100.)
				++it;
			else if (params_.percentOfClassifiedPatterns_==50.)
			{
				++it; 
				if(it==itEnd)
					break;
				++it;
			}
			else if (params_.percentOfClassifiedPatterns_==25.)
			{
				++it; 
				if(it==itEnd)
					break;
				++it;
				if(it==itEnd)
					break;
				++it;
			}
			else  //12.5 
			{
				++it; 
				if(it==itEnd)
					break;
				++it;
				if(it==itEnd)
					break;
				++it;
				if(it==itEnd)
					break;
				++it;
			}
		}

		++iter;

		//verify if the difference among the probabilities is acceptable
		vector<double> probs;
		flag = false;
		params_.logPrioriProbs_.clear();
		
		for(i=0; i<numClasses; ++i)
		{
			double pr = double (numElementsByClass[i]) / double (numClassifiedElements);
			probs.push_back(pr);
			params_.logPrioriProbs_.push_back(log(pr));
			if(abs(pr-params_.prioriProbs_[i])>=params_.acceptableDiff_)
				flag = true;
		}

		params_.prioriProbs_.clear();
		params_.prioriProbs_=probs; 
	}//for each iteratios

	return true;
}

#endif




