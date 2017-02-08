#include <KFunctionPluginWindow.h>
#include <k.h>
#include <math.h>
#include <vector>
#include <valarray>

//********************************************************************************
///This function actually computes the Kr estimates, after the vectors and
///variables a, b, dmax, h[], x[], y[] are initialized.
//********************************************************************************/

int KRipley(const TeBox &box, int n, int nh, double dmax,const std::vector<double> &x,const std::vector<double> &y, 
			const std::vector<double> &h, std::valarray<double> &khat)
{
	int i, j, k;
	double dist, wi, wj, area, distx, disty;

	area = box.height()*box.width();    /* Area of the Region assumed as  retangular   */
	dmax = dmax * dmax ;  	   /* Define maximum distance for estimation of K */

	for( i=0; i<nh+1; i++)
		khat[i] = 0.0;

	for(i=1; i < n; i++)
	{
		for( j=0; j < i; j++)
		{
			distx = (x[i]-x[j])*(x[i]-x[j]);
			disty = (y[i]-y[j])*(y[i]-y[j]);
 			dist =  distx + disty;
			/* Squared distance between 2 events */
			if( dist <= dmax )
			{
				dist = sqrt(dist);
									   /* Find the bin of dist in the h vector */
				k = find_place( dist, nh, h );
//				k++;
                                       			/* Edge correction for (xi,yi) */
				wi = edge_correction( x[i], y[i], dist, box);
                                                /* Edge correction for (xj,yj) */
				wj = edge_correction( x[j], y[j], dist, box);
                                               /* Put weights in the right bin */
				khat[k] += wi + wj;
        	}
        }
	}

//	double k1,k2;
	for( i=1; i < nh+1; i++){
		khat[i] += khat[i-1];		   /* Cumulative some over the khat vector */
//		k1 = khat[i];
	}

	for( i=0; i<nh+1; i++){          /* Correct khat by area and number of points */
		khat[i] = khat[i] * area / ( (double)n * (double)n);
//		k2 = khat[i];
	}

    return 0;
}
//********************************************************************************
///This function actually computes the Kr estimates, after the vectors and
///variables a, b, dmax, h[], x[], y[] are initialized.
//********************************************************************************/
int KStoyan(const TeBox &box, int n, int nh, double dmax,const std::vector<double> &x,const std::vector<double> &y, 
			const std::vector<double> &h, std::valarray<double> &khat)
{
	int i, j, k;
	double dist, w, distx, disty, c1, invlambda;

	invlambda =(box.height()*box.width()/(double) n );    /* Intensity estimation */
	c1 = invlambda * invlambda;  
	dmax = dmax * dmax ;  	   /* Define maximum distance for estimation of K */

	for( i=0; i<nh+1; i++)
		khat[i] = 0.0;

	for(i=1; i < n; i++)
	{
		for( j=0; j < i; j++)
		{
			distx = (x[i]-x[j])*(x[i]-x[j]);
			disty = (y[i]-y[j])*(y[i]-y[j]);
 			dist =  distx + disty;
			/* Squared distance between 2 events */
			if( dist <= dmax )
			{
				dist = sqrt(dist);
									   /* Find the bin of dist in the h vector */
				k = find_place( dist, nh, h );
//				k++;
                                       			/* Edge correction for (xi,yi) */
				w = (box.width() - fabs(x[i]-x[j])) * (box.height() - fabs(y[i]-y[j]));
                w = 1/w;
                                               /* Put weights in the right bin */
				khat[k] += (2*w);
        	}
        }
	}

//	double k1,k2;
	for( i=1; i < nh+1; i++){
		khat[i] += khat[i-1];		   /* Cumulative some over the khat vector */
//		k1 = khat[i];
	}
	
	for( i=0; i<nh+1; i++){          /* Correct khat by area and number of points */
		khat[i] *= c1;
//		k2 = khat[i];
	}

    return 0;
}
//********************************************************************************
///This function actually computes the Ks estimates, after the vectors and
///variables a, b, dmax, h[], x[], y[] are initialized.
//********************************************************************************/
int KStoyanM(const TeBox &box, int n, int nh, double dmax,const std::vector<double> &x, const std::vector<double> &y, 
			  const std::vector<double> &h, const std::vector<double> &m, std::valarray<double> &khat)
{
int i, j, k;
	double dist, w, m2, c1, invlambda, invavg;

    if (m.size() != x.size())
		return 1;

	invlambda = ( box.height()*box.width() / (double) n );    /* Intensity estimation   */
	invavg = 0;                   //MUDANCA (SEGUNDO CRESSIE A MEDIA DAS MARCAS DEVE ESTAR CONTIDA NO CALCULO)
	for( i=0; i<n; i++)
		invavg += m[i];
	invavg = n/invavg;
	c1 = invlambda * invlambda * invavg * invavg;     /* Constant factor        */
	dmax = dmax * dmax ;  	     /* Define maximum distance for estimation of K */

	for( i=0; i<nh+1; i++)
		khat[i] = 0.0;

	for(i=1; i < n; i++)
	{
		for( j=0; j < i; j++)
		{
 			dist = (double)( (x[i]-x[j])*(x[i]-x[j]) + (y[i]-y[j])*(y[i]-y[j]) );
									/* Squared distance between 2 events */
			if( dist <= dmax )
			{
				dist = sqrt(dist);
									   /* Find the bin of dist in the h vector */
				k = find_place( dist, nh, h );
//				k++;
				                                  /* Put weights in the right bin */
                w = box.width()*box.height() - box.height()*fabs(x[i]-x[j]) - box.width()*fabs(y[i]-y[j]) + fabs(x[i]-x[j])*fabs(y[i]-y[j]);
                w = 1/w;
												/* Product of the marks        */
				m2 = m[i] * m[j];
                                               /* Put weights in the right bin */
				khat[k] += (2*m2*w);
        	}
        }
	}
	for( i=1; i < nh+1; i++)
		khat[i] += khat[i-1];		   /* Cumulative some over the khat vector */

	for( i=0; i<nh+1; i++)          /* Correct khat by area and number of points */
		khat[i] = khat[i] * c1 ;

	return(0);
}

//********************************************************************************
///This function find the FIRST position in the h vector
///such as the distance between events is less or equal
///to the value of h[i]
//********************************************************************************/
int find_place( double d, int size, const std::vector<double> &vect)
{
    int i;

 	for(i=0; i< size; i++)
 	{
 		if( d < vect[i])
 			break;
  	}
	return(i);
}



//********************************************************************************
///This function computes the edge correction for Kr estimator of K-function
///for a given event at (x,y) and radius u,
///assuming that the study region is rectangular with sides a and b.
///See CRESSIE (1991) p.640 .
//********************************************************************************/
double edge_correction( float x, float y, double u,const TeBox &box)
{
	double w, d1, d2;

	if(u==0)
		return(1);

	d1 = ( double )MINK( x - box.x1(), (box.x2() - x) );
	d2 = ( double )MINK( y - box.y1(), (box.y2() - y) );

	if( u*u <= ( d1*d1 + d2*d2 ) )
		w = 1 - ( acos( MINK(d1,u)/u ) + acos( MINK(d2,u)/u ) ) /TePI;
	else
		w = 0.75 - ( ( acos(d1/u) + acos(d2/u) ) / (2*TePI) );

	w = 1/w;
	return( w );
}
