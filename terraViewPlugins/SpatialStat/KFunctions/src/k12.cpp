#include <k.h>


//********************************************************************************
///This function actually computes the Ks12 estimates, after the vectors and
///variables a, b, dmax, h[], x[], y[] are initialized.
//********************************************************************************/

#include <k12.h>
#include <k.h>

int KStoyan12(const TeBox &box, int n1, int n2, int nh, double dmax, const std::vector<double> &x1,const std::vector<double> &y1, 
			  const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h, std::valarray<double> &khat)
{
	int i, j, k;
	double dist, w, c1;

	c1 = ((box.height() * box.width()) * (box.height() * box.width()) ) / ( (double) n1 * (double) n2 );
																/* Constant factor  */
	dmax = dmax * dmax ;  	   /* Define maximum distance for estimation of K */

	for( i=0; i<nh+1; i++)
		khat[i] = 0.0;

    for(i=0; i < n1; i++)
	{
		for( j=0; j < n2; j++)
		{
 			dist = (double)( (x1[i]-x2[j])*(x1[i]-x2[j]) + (y1[i]-y2[j])*(y1[i]-y2[j]) );
									/* Squared distance between 2 events */
			if( dist <= dmax )
			{
				dist = sqrt(dist);
									   /* Find the bin of dist in the h vector */
				k = find_place( dist, nh, h );
//				k++;
				w = box.width()*box.height() - box.height()*fabs(x1[i]-x2[j]) - box.width()*fabs(y1[i]-y2[j]) + fabs(x1[i]-x2[j])*fabs(y1[i]-y2[j]);
                w = 1/w;
				khat[k] +=  w;
        	}
        }
	}

	for( i=1; i < nh+1; i++)
		khat[i] += khat[i-1];

	for( i=0; i<nh+1; i++)
		khat[i] = khat[i] * c1 ;

	return(0);
}

//********************************************************************************
///This function actually computes the Ks12 estimates, after the vectors and
///variables a, b, dmax, h[], x[], y[] are initialized.
//*******************************************************************************/
int KRipley12(const TeBox &box, int n1, int n2, int nh, double dmax, const std::vector<double> &x1,const std::vector<double> &y1, 
			  const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h, std::valarray<double> &khat)
{
	int i, j, k;
	double dist, w, w1, w2;
	dmax = dmax * dmax ;  	   /* Define maximum distance for estimation of K */

	for( i=0; i<nh+1; i++)
		khat[i] = 0.0;

	for(i=0; i < n1; i++)
	{
		for( j=0; j < n2; j++)
		{
 			dist = (x1[i]-x2[j])*(x1[i]-x2[j]) + (y1[i]-y2[j])*(y1[i]-y2[j]);
									/* Squared distance between 2 events */
			if( dist <= dmax )
			{
				dist = sqrt(dist);
									   /* Find the bin of dist in the h vector */
				k = find_place( dist, nh, h );
//				k++;
				w1 = edge_correction( x1[i], y1[i], dist, box);
				w2 = edge_correction( x2[j], y2[j], dist, box);
				w = (n1*w1 + n2*w2)/(n1+n2);
				khat[k] +=  w * (box.width()*box.height()/(double)(n1*n2));
        	}
        }
	}

	for( i=1; i < nh+1; i++)
		khat[i] += khat[i-1];

	return(0);
}
//********************************************************************************
///This function actually computes the Ks12 estimates, after the vectors and
///variables a, b, dmax, h[], x[], y[] are initialized.
//********************************************************************************/
int KStoyan12M(const TeBox &box, int n1, int n2, int nh, double dmax, const std::vector<double> &x1,const std::vector<double> &y1, 
			   const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h, const std::vector<double> &m1,
			   const std::vector<double> &m2, std::valarray<double> &khat)
{
	int i, j, k;
	double dist, w, c1, invlambda, invavg1, invavg2, mp;

	invlambda = ((box.height() * box.width()) * (box.height() * box.width()) ) / ( (double) n1 * (double) n2 );    /* Intensity estimation   */
	invavg1 = 0;                   //MUDANCA (SEGUNDO CRESSIE A MEDIA DAS MARCAS DEVE ESTAR CONTIDA NO CALCULO)
	for( i=0; i<n1; i++)
		invavg1 += m1[i];
	invavg1 = n1/invavg1;
	invavg2 = 0;                   //MUDANCA (SEGUNDO CRESSIE A MEDIA DAS MARCAS DEVE ESTAR CONTIDA NO CALCULO)
	for( i=0; i<n2; i++)
		invavg2 += m2[i];
	invavg2 = n2/invavg2;
	c1 = invlambda * invavg1 * invavg2;             /* Constant factor        */
	dmax = dmax * dmax ;  	   /* Define maximum distance for estimation of K */

	for( i=0; i<nh+1; i++)
		khat[i] = 0.0;

 	for(i=0; i < n1; i++)
	{
		for( j=0; j < n2; j++)
		{
 			dist=(double)((x1[i]-x2[j])*(x1[i]-x2[j]) +(y1[i]-y2[j])*(y1[i]-y2[j]));
									/* Squared distance between 2 events */
			if( dist <= dmax )
			{
				dist = sqrt(dist);
									   /* Find the bin of dist in the h vector */
				k = find_place( dist, nh, h );
	//			k++;
				w = box.width()*box.height() - box.height()*fabs(x1[i]-x2[j]) - box.width()*fabs(y1[i]-y2[j]) + fabs(x1[i]-x2[j])*fabs(y1[i]-y2[j]);
                w = 1/w;
												/* Product of the marks        */
				mp = m1[i] * m2[j];
                                              /* Put weights in the right bin */
				khat[k] +=  (mp * w);
        	}
        }
	}

	for( i=1; i < nh+1; i++)
		khat[i] += khat[i-1];

	for( i=0; i<nh+1; i++)
		khat[i] = khat[i] * c1 ;

	return(0);
}


