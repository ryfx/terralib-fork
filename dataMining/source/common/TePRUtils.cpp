
#include "TePRUtils.h"
#include "TePRChiTable.h"
#include <iostream>

bool
TePRGetThreshold(std::vector<double>& inThresholds, const int& numOfDimentions, std::vector<double>& outThresholds)
{
	outThresholds.clear();
	std::vector<double>::iterator it = inThresholds.begin();
	while(it != inThresholds.end())
	{
		int	index, igl;

		if( (*it) < 90. ) index = 0;		// 75%
		else if( (*it) < 95.  ) index = 1;	// 90%
		else if( (*it) < 99.  ) index = 2;	// 95%
		else if( (*it) < 99.9 ) index = 3;	// 99%
		else if( (*it) < 100. ) index = 4;	// 99.9%
		else (*it) = 5;			// 100%

		if( numOfDimentions > TeChiTabMaxDegree )
			igl = TeChiTabMaxDegree - 1;
		else
			igl = numOfDimentions - 1;
		
		outThresholds.push_back(TeChiTab[igl][index]);
		++it;
	}

	return true;
}


bool
TePRGetInverseCovMatrix(TeMatrix& covMatrix, TeMatrix& inverseCovMatrix)
{
	//verify if it is possible get the inverse matrix with cholesk decomp 
	TeMatrix aux;

	//verify if covariance matrix is versible
	if(covMatrix.Inverse(aux))
	{
		inverseCovMatrix = aux;
		return true;
	}
		
	//if covariance matrix is inversible, update eigen values. Verify if there are
	//values equal to zero in the main diagonal of eigen values matrix.
	//If there are, replace them to the minimal values of the main diagonal.
	// CovMatr = eigenVectors * eigenValues * eigenVectors T  
	TeMatrix eigenValues;
	covMatrix.EigenValues(eigenValues);

	//get the minimal value of the main diagonal 
	double minValue = eigenValues(0,0);
	for(int i=1; i<eigenValues.Nrow(); ++i)
	{
		if(minValue==0. || (eigenValues(i, i)< minValue && eigenValues(i, i) != 0.))
			minValue = eigenValues(i, i);
	}

	//replace zero values to the minimal values
	for(int i=0; i<eigenValues.Nrow(); ++i)
	{
		if(eigenValues(i,i)==0.)
			eigenValues(i,i) = minValue;
	}


	//inverse matrix
	TeMatrix eigenVectors;
	covMatrix.EigenVectors(eigenVectors);

	TeMatrix inverseEigenVectors, inverseEigenValues, aux2;
	eigenVectors.Inverse(inverseEigenVectors);
	eigenValues.Inverse(inverseEigenValues);
	
	aux2 = eigenVectors * inverseEigenValues; 
	inverseCovMatrix = aux2 * inverseEigenVectors;  

	return true;
}







