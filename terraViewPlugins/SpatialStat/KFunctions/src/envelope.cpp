#include <envelope.h>
//#include <valarray>

//********************************************************************************
///This function actually computes the K estimates, after the vectors and
///variables a, b, dmax, h[], x[], y[] and m[] are initialized.
//*******************************************************************************/

int envelope(const TeBox &box, int nsim, int n, int nh, double dmax, int control, const std::vector<double> &h, std::valarray<double> &upper,
			std::valarray<double> &lower)
{
	int i,j;
	std::vector<double> x(n,0.0);
	std::vector<double> y(n,0.0);
    TeBox boxenv;
	std::valarray<double> khat;

	khat.resize(nh+1,0.0);

	boxenv = makeBox(0.0,0.0,box.width(),box.height());
    ppoisson(n,boxenv,x,y);
//	for(j=0;j<nh+1;++j)
//			khat[j] = 0.0;
	switch(control)
	{
		case 0:
			{
				KStoyan(boxenv,n,nh,dmax,x,y,h,khat);
				break;
			}
		case 1:
			{
				KRipley(boxenv,n,nh,dmax,x,y,h,khat);
				break;
			}
	}
	

	for( i=0 ; i<nh+1 ; i++)
	{
		upper[i] = khat[i];
		lower[i] = khat[i];
	}

	for( i=1 ; i<=nsim ; i++ )
	{
		ppoisson(n,boxenv,x,y);
		for(j=0;j<nh+1;j++)
			khat[j] = 0.0;
		switch(control)
		{
			case 0:
				{
					KStoyan(boxenv,n,nh,dmax,x,y,h,khat);
					break;
				}
			case 1:
				{
					KRipley(boxenv,n,nh,dmax,x,y,h,khat);
					break;
				}
		}
	
		maxmin(nh+1,khat,upper,lower);
	}
	return (0);
}
//********************************************************************************
///This function actually computes the Kmrk estimates, after the vectors and
///variables a, b, dmax, h[], x[], y[] and m[] are initialized.
//********************************************************************************/
int EnvKSM(const TeBox &box,int nsim, int n, int nh, double dmax,const std::vector<double> &x, const std::vector<double> &y, 
			  const std::vector<double> &h, std::vector<double> m, std::valarray<double> &upper, std::valarray<double> &lower)
{
	int i,j;
	std::valarray<double> khat;

	khat.resize(nh+1,0.0);
    
    srand( (unsigned)time( NULL ) );
    
    rand_mark(m, n);
//	for(j=0;j<nh+1;++j)
//			khat[j] = 0.0;
	KStoyanM(box,n,nh,dmax,x,y,h,m,khat);

	for( i=0 ; i<nh+1 ; i++)
	{
		upper[i] = khat[i];
		lower[i] = khat[i];
	}

	for( i=1 ; i<=nsim ; i++ )
	{
		rand_mark(m, n);
		for(j=0; j<nh+1; j++)
			khat[j] = 0.0;
		KStoyanM(box,n,nh,dmax,x,y,h,m,khat);
		maxmin(nh+1,khat,upper,lower);
	}

   
	return 0;
}
//////////////////////////////////////////////////////////////////////////////
///This function actually computes the K12 estimates, after the vectors and
///variables a, b, dmax, h[], x[], y[] and m[] are initialized.
int envelope12(const TeBox &box,int nsim, int n1, int n2, int nh, double dmax, const std::vector<double> &x1,const std::vector<double> &y1, 
			std::vector<double> x2, std::vector<double> y2, const std::vector<double> &h, const int &control, std::valarray<double> &upper, std::valarray<double> &lower)
{
	int i,j;
	std::valarray<double> khat;

	khat.resize(nh+1,0.0);

    torus(box,n2,x2,y2);
//	for(j=0;j<nh+1;++j)
//			khat[j] = 0.0;
	switch(control)
	{
		case 0:
			{
				KStoyan12(box,n1,n2,nh,dmax,x1,y1,x2,y2,h,khat);
				break;
			}
		case 1:
			{
				KRipley12(box,n1,n2,nh,dmax,x1,y1,x2,y2,h,khat);
				break;
			}
	}

	for( i=0 ; i<nh+1 ; i++)
	{
		upper[i] = khat[i];
		lower[i] = khat[i];
	}
    for( i=1 ; i<=nsim ; i++ )
	{
    	torus(box,n2,x2,y2);
		for(j=0; j<nh+1; j++)
			khat[j] = 0.0;
		switch(control)
		{
			case 0:
				{
					KStoyan12(box,n1,n2,nh,dmax,x1,y1,x2,y2,h,khat);
					break;
				}
			case 1:
				{
					KRipley12(box,n1,n2,nh,dmax,x1,y1,x2,y2,h,khat);
					break;
				}
		}
		maxmin(nh+1,khat,upper,lower);
	}

   	return 0;

}
//****************************************************************************
///	Computes the ENVELOPE for estimates of Ripley's K-function
///    			for 2 types of MARKED POINT PROCESSES
///                using the estimator proposed by Stoyan (1984).
///                See CRESSIE (1991) p.641 .
//****************************************************************************/
int EnvKSM12(const TeBox &box, int nsim, int n1, int n2, int nh, double dmax, const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2,const std::vector<double> &h, std::vector<double> m1,
			std::vector<double> m2, std::valarray<double> &upper,std::valarray<double> &lower)
{
	int i,j;
	std::valarray<double> khat;

	khat.resize(nh+1,0.0);

    rand_marks(n1,n2,m1,m2);
//	for(j=0;j<nh+1;++j)
//			khat[j] = 0.0;
	KStoyan12M(box,n1,n2,nh,dmax,x1,y1,x2,y2,h,m1,m2,khat);

	for(i=0; i<nh+1; i++)
	{
		upper[i] = khat[i];
		lower[i] = khat[i];
	}

	for(i=0; i<nsim; i++)
	{
		rand_marks(n1,n2,m1,m2);
		for(j=0; j<nh+1; j++)
			khat[j] = 0.0;
		KStoyan12M(box,n1,n2,nh,dmax,x1,y1,x2,y2,h,m1,m2,khat);
		maxmin(nh+1,khat,upper,lower);
	}
	return 0;
}
/****************************************************************************
	Computes the ENVELOPE for estimates of Ripley's K-function
    			for 2 types of MARKED POINT PROCESSES
                using the estimator proposed by Stoyan (1984).
                See CRESSIE (1991) p.641 .
****************************************************************************/
int EnvLabel_Stoyan(const TeBox &box, int nsim, int n1, int n2, int nh, double dmax,const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h,  
			std::valarray<double> &upper, std::valarray<double> &lower)
{
	int i;
	std::valarray<double> khat;
	std::vector<double> xaux1, xaux2, yaux1, yaux2;

	khat.resize(nh+1,0.0);

	for(i = 0; i < n1; i++)
	{
		xaux1.push_back(x1[i]);
		yaux1.push_back(y1[i]);
	}

	for(i = 0; i < n2; i++)
	{
		xaux2.push_back(x2[i]);
		yaux2.push_back(y2[i]);
	}

    rand_label(n1, n2, xaux1, yaux1, xaux2, yaux2);
	Klabel_Stoyan(box, n1, n2, nh, dmax, xaux1, yaux1, xaux2, yaux2, h, khat);

	for(i=0; i<nh+1; i++)
	{
		upper[i] = khat[i];
		lower[i] = khat[i];
	}

	for(i=0; i<nsim; i++)
	{
		rand_label(n1, n2, xaux1, yaux1, xaux2, yaux2);
		Klabel_Stoyan(box, n1, n2, nh, dmax, xaux1, yaux1, xaux2, yaux2, h, khat);
		maxmin(nh+1,khat,upper,lower);
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
int EnvLabel_Ripley(const TeBox &box, int nsim, int n1, int n2, int nh, double dmax,const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h,  
			std::valarray<double> &upper, std::valarray<double> &lower)
{
	int i;
	std::valarray<double> khat;
	std::vector<double> xaux1, xaux2, yaux1, yaux2;

	khat.resize(nh+1,0.0);

	for(i = 0; i < n1; i++)
	{
		xaux1.push_back(x1[i]);
		yaux1.push_back(y1[i]);
	}

	for(i = 0; i < n2; i++)
	{
		xaux2.push_back(x2[i]);
		yaux2.push_back(y2[i]);
	}

    rand_label(n1, n2, xaux1, yaux1, xaux2, yaux2);
	Klabel_Ripley(box, n1, n2, nh, dmax, xaux1, yaux1, xaux2, yaux2, h, khat);

	for(i=0; i<nh+1; i++)
	{
		upper[i] = khat[i];
		lower[i] = khat[i];
	}

	for(i=0; i<nsim; i++)
	{
		rand_label(n1, n2, xaux1, yaux1, xaux2, yaux2);
		Klabel_Ripley(box, n1, n2, nh, dmax, xaux1, yaux1, xaux2, yaux2, h, khat);
		maxmin(nh+1,khat,upper,lower);
	}
	return 0;
}

/****************************************************************************
	Computes the ENVELOPE for estimates of Ripley's K-function
    			for 2 types of MARKED POINT PROCESSES
                using the estimator proposed by Stoyan (1984).
                See CRESSIE (1991) p.641 .
****************************************************************************/
int EnvLabelM(const TeBox &box, int nsim, int n1, int n2, int nh, double dmax,const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h, const std::vector<double> &m1,  
			const std::vector<double> &m2, int funcId, bool L, std::valarray<double> &upper, 
			std::valarray<double> &lower)
{
	int i;
	std::valarray<double> khat;
	std::vector<double> xaux1, xaux2, yaux1, yaux2, maux1, maux2;

	khat.resize(nh+1,0.0);

	for(i = 0; i < n1; i++)
	{
		xaux1.push_back(x1[i]);
		yaux1.push_back(y1[i]);
		maux1.push_back(m1[i]);
	}

	for(i = 0; i < n2; i++)
	{
		xaux2.push_back(x2[i]);
		yaux2.push_back(y2[i]);
		maux2.push_back(m2[i]);
	}

    rand_labelM(n1, n2, xaux1, yaux1, xaux2, yaux2, maux1, maux2);
	KlabelM(box, n1, n2, nh, dmax, xaux1, yaux1, xaux2, yaux2, h, maux1, maux2, khat, funcId, L);

	for(i=0; i<nh+1; i++)
	{
		upper[i] = khat[i];
		lower[i] = khat[i];
	}

	for(i=0; i<nsim; i++)
	{
		rand_labelM(n1, n2, xaux1, yaux1, xaux2, yaux2, maux1, maux2);
		KlabelM(box, n1, n2, nh, dmax, xaux1, yaux1, xaux2, yaux2, h, maux1, maux2, khat, funcId, L);
		maxmin(nh+1,khat,upper,lower);
	}
	return 0;
}
//****************************************************************************
///	This function randomly assign type (1 or 2) to each point of
///		the groupe (x,y) of events.
///		It re-builds the vectors (x1[], y1[]) and (x2[], y2[]).
//****************************************************************************/
void rand_marks(int n1, int n2, std::vector<double> &m1, std::vector<double> &m2)
{
	int i, randnum;
	double tmp;

	srand( (unsigned)time( NULL ) );		/* generate seed for random numbers */

    for( i=0 ; i < n1 ; i++)
    {
        randnum = 1 + (int) (((double) n1 * (double)rand()) / ((double)RAND_MAX +1.0));
        tmp = m1[ i ];
        m1[ i ] = m1[ randnum ];
        m1[ randnum ] = tmp;
    }

    for( i=0 ; i < n2 ; i++)
    {
        randnum = 1 + (int) (((double) n2 * (double)rand()) / ((double)RAND_MAX +1.0));
        tmp = m2[ i ];
        m2[ i ] = m2[ randnum ];
        m2[ randnum ] = tmp;
    }
}
///This function randomly assign the distribution for the K12 function
void torus(const TeBox &box, int n, std::vector<double> &x, std::vector<double> &y)
{
	int i, Ix, Iy, Ixk, Iyk;
	double xc, yc, xk, yk, xkr, ykr;

  	srand( (unsigned)time( NULL ) );

	xc = (double) box.width()*rand()/ (RAND_MAX);
	yc = (double) box.height()*rand()/ (RAND_MAX);

	Ix = I( xc, box.width()/2 );
	Iy = I( yc, box.height()/2 );

	xk = xc - box.width()/2 + box.width() * Ix;
	yk = yc - box.height()/2 + box.height() * Iy;

	xkr = xk + box.x1();
	ykr = yk + box.y1();

    for (i=0; i< n; i++) 
    {
		Ixk = I( x[i], xkr );
		Iyk = I( y[i], ykr );

		x[i] += box.width() * Ixk - xk;
		y[i] += box.height() * Iyk - yk;
	}
}

///	This function randomly permute a vector with size n
void rand_mark(std::vector<double> &A, int n)               //MUDANCA (SIMPLIFICACAO DA PERMUTACAO DO VETOR DE MARCAS) 
{
 int i,j;
 double b;
 for(i = n-1; i>0; i --)
 {
   j = (i+1)* (int) rand()/ (RAND_MAX+1); ;
   b = A[i];
   A[i] = A[j];
   A[j] = b;
 }
}

//*******************************************************************************
///This function generates a Point Poisson Process of with n events in a
///rectangular area of sides: a and b.
//********************************************************************************/
void ppoisson( int n,const TeBox &box, std::vector<double> &x, std::vector<double> &y )
{
	int i;
	double w,h;

	w=box.width();
	h= box.height();

 	srand( (unsigned)time( NULL ) );

    for (i=0; i< n; i++)                       //MUDANCA(DIMINUICAO DO NUMERO DE FOR)
    {
		x[i] = (double) box.width()*rand()/ (RAND_MAX);
		y[i] = (double) box.height()*rand()/ (RAND_MAX);
    }

}
//********************************************************************************
///This function selecte the Maximum and Minimum values for khat
//********************************************************************************/
void maxmin(int nh,const std::valarray<double> &khat, std::valarray<double> &upper, std::valarray<double> &lower)
{
	int i;

	for( i=0 ; i<nh ; i++)
	{
		upper[i] = MAXK(upper[i], khat[i]);
		lower[i] = MINK(lower[i], khat[i]);
	}

}
/****************************************************************************
	This function randomly assign type (1 or 2) to each point of
		the groupe (x,y) of events.
		It re-builds the vectors (x1[], y1[]) and (x2[], y2[]).
****************************************************************************/
void rand_label(int n1, int n2, std::vector<double> &x1, std::vector<double> &y1, 
				 std::vector<double> &x2, std::vector<double> &y2)
{
	std::vector<double> aux, tmpx, tmpy;
	unsigned int i;

	for(i=0;i<n1;++i)
	{
		aux.push_back(i);
		tmpx.push_back(x1[i]);
		tmpy.push_back(y1[i]);
	}

	for(i=0;i<n2;++i)
	{
		aux.push_back(n1+i);
		tmpx.push_back(x2[i]);
		tmpy.push_back(y2[i]);
	}

    rand_mark(aux, n1+n2);

	for(i=0;i<n1;++i)
	{
		x1[i] = tmpx[(int)aux[i]];
		y1[i] = tmpy[(int)aux[i]];
	}

	for(i=0;i<n2;++i)
	{
		x2[i] = tmpx[(int)aux[n1+i]];
		y2[i] = tmpy[(int)aux[n1+i]];
	}
}
/****************************************************************************
	This function randomly assign type (1 or 2) to each point of
		the groupe (x,y) of events.
		It re-builds the vectors (x1[], y1[]) and (x2[], y2[]).
****************************************************************************/
void rand_labelM(int n1, int n2, std::vector<double> &x1, std::vector<double> &y1, 
				 std::vector<double> &x2, std::vector<double> &y2, std::vector<double> m1,
				 std::vector<double> m2)
{
	std::vector<double> aux, tmpx, tmpy, tmpm;
	unsigned int i;

	for(i=0;i<n1;++i)
	{
		aux.push_back(i);
		tmpx.push_back(x1[i]);
		tmpy.push_back(y1[i]);
		tmpm.push_back(m1[i]);
	}

	for(i=0;i<n2;++i)
	{
		aux.push_back(n1+i);
		tmpx.push_back(x2[i]);
		tmpm.push_back(m2[i]);
	}

    rand_mark(aux, n1+n2);

	for(i=0;i<n1;++i)
	{
		x1[i] = tmpx[(int)aux[i]];
		y1[i] = tmpy[(int)aux[i]];
		m1[i] = tmpy[(int)aux[i]];
	}

	for(i=0;i<n2;++i)
	{
		x2[i] = tmpx[(int)aux[n1+i]];
		y2[i] = tmpy[(int)aux[n1+i]];
		m2[i] = tmpy[(int)aux[n1+i]];
	}
}