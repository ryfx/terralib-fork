#ifndef K_H
#define K_H

#include <vector>
#include <valarray>
#include <KFunctionPluginWindow.h>
#include <TeBox.h>
#include <k12.h>


/* Define Macro functions */
#define MINK( v1, v2 )  ( (v1) < (v2) ? (v1) : (v2) )
#define MAXK( v1, v2 )  ( (v1) > (v2) ? (v1) : (v2) )

/*  Function definitions to K-Ripley   */
int KRipley(const TeBox &box, int n, int nh, double dmax,const std::vector<double> &x, const std::vector<double> &y, 
			const std::vector<double> &h, std::valarray<double> &khat);
int KStoyan(const TeBox &box, int n, int nh, double dmax,const std::vector<double> &x, const std::vector<double> &y, 
			const std::vector<double> &h, std::valarray<double> &khat);
int KStoyanM(const TeBox &box, int n, int nh, double dmax,const std::vector<double> &x, const std::vector<double> &y, 
			const std::vector<double> &h, const std::vector<double> &m, std::valarray<double> &khat);
int find_place( double d, int size, const std::vector<double> &vect);
double edge_correction( float x, float y, double u,const TeBox &box);

#endif