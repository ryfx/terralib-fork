#include <geodma_distances.h>
#include <math.h>
#include <TeMatrix.h>
#define ln(x) log(x)/log( exp(1.0) )

geodma_distances::geodma_distances()
{
}

geodma_distances::~geodma_distances()
{
}

double geodma_distances::get_Mahalanobis(TeMatrix means_i, TeMatrix covariance_i, TeMatrix means_j, TeMatrix covariance_j)
{
  TeMatrix a,
            b = 0.5 * (covariance_i + covariance_j),
            c = means_i - means_j;
  a.Transpose(c);

  TeMatrix r = a * c;

  return r(0, 0);
};

double geodma_distances::get_Battacharyya(TeMatrix means_i, TeMatrix covariance_i, TeMatrix means_j, TeMatrix covariance_j)
{
  TeMatrix a,
            b = covariance_i + covariance_j,
            c = means_i - means_j;
  double d = b.Determinant(),
          e = sqrt( covariance_i.Determinant() * covariance_j.Determinant() );
    
  a.Transpose(c);	
  b = 0.5 * b;
  b.Inverse(b);

  TeMatrix r = 1/8 * a * b * c;

  return r(0, 0) + 1/2 * ln(d / e);
};

double geodma_distances::get_Matusita(double b)
{
  return sqrt(2 * ( 1 - exp(-b) ) );
};
