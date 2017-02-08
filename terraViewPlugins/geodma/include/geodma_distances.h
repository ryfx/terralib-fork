#ifndef __GEODMA_DISTANCES_H_
#define __GEODMA_DISTANCES_H_

#include <TeMatrix.h>
#define ln(x) log(x)/log( exp(1.0) )

class geodma_distances
{
	private:

	public:

    /* Constructor and destructor */
		geodma_distances();
    ~geodma_distances();
    
		double get_Mahalanobis(TeMatrix means_i, TeMatrix covariance_i, TeMatrix means_j, TeMatrix covariance_j);
		double get_Battacharyya(TeMatrix means_i, TeMatrix covariance_i, TeMatrix means_j, TeMatrix covariance_j);
		double get_Matusita(double b);
};

#endif //__GEODMA_DISTANCES_H_