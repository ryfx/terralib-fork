/*

2011abr07

Testing the Nelder-Mead simplex method applied to Rosenbrock's function.

Initial Values
-1.200000 1.000000 24.200000
-0.234074 1.258819 146.491360
-0.941181 1.965926 120.430700
Iteration 1
-1.200000 1.000000 24.200000
-0.652332 1.370891 92.099529
-0.941181 1.965926 120.430700
...
Iteration 53
0.998553 0.998282 0.000140
0.995260 0.990385 0.000025
1.006654 1.013696 0.000056
The minimum was found at
9.952601e-01
9.903851e-01
105 Function Evaluations
53 Iterations through program
0.995260
0.990385

*/

#define TEAGN_ENABLE_STDOUT_LOG

#include "TePDISimplex.hpp"
/*
#include <TePDIExamplesBase.hpp>
#include <TePDIUtils.hpp>
#include <TeProgress.h>
#include <TeStdIOProgress.h>
#include <TeAgnostic.h>
#include <string>
*/

double rosen(double *x)
{
	return (100*(x[1]-x[0]*x[0])*(x[1]-x[0]*x[0])+(1.0-x[0])*(1.0-x[0]));
}

int main()
{
    char const *msg = "Teste";
    TePDIParameters params;
    params.SetParameter( "msg" , msg );  

    int n = 2;
    double *x = new double[n];
    /* starting points */
    x[0] = -1.2;
    x[1] = 1.0;
    int max_it = 300;
    double epsilon = 1.0e-4;
    params.SetParameter( "objective_function" , &rosen );
    params.SetParameter( "num_variables" , n );
    params.SetParameter( "x_array" , x );
    params.SetParameter( "max_iterations" , max_it );
    params.SetParameter( "epsilon" , epsilon );

    TePDISimplex algo;
    
    TEAGN_TRUE_OR_THROW( algo.Reset(params), "Reset failed" );
    TEAGN_TRUE_OR_THROW( algo.Apply(), "Apply error" );
    
    /*
    int i;
    printf("Solution: \n");
    for (i = 0; i < n; i++)
        printf("%f  ",x[i]);
    printf("\n");
*/

    getchar();
    delete [] x;

    return EXIT_SUCCESS;
}
