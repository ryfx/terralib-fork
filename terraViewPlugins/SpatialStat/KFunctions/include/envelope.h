#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <valarray>
#include <vector>
#include <k.h>
#include <TeBox.h>
#include <KFunctionPluginWindow.h>


                                    /* Define Macro functions */
#define I( v1, v2 )  ( (v1) < (v2) ? 1 : 0 )

int envelope(const TeBox &box,int nsim, int n, int nh, double dmax, int control, const std::vector<double> &h, std::valarray<double> &upper,
			std::valarray<double> &lower);
int EnvKSM(const TeBox &box,int nsim, int n, int nh, double dmax,const std::vector<double> &x, const std::vector<double> &y, 
			  const std::vector<double> &h, std::vector<double> m, std::valarray<double> &upper, std::valarray<double> &lower);
int envelope12(const TeBox &box,int nsim, int n1, int n2, int nh, double dmax, const std::vector<double> &x1,const std::vector<double> &y1, 
			std::vector<double> x2, std::vector<double> y2, const std::vector<double> &h, const int &control, std::valarray<double> &upper, std::valarray<double> &lower);
int EnvKSM12(const TeBox &box, int nsim, int n1, int n2, int nh, double dmax, const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2,const std::vector<double> &h, std::vector<double> m1,
			std::vector<double> m2, std::valarray<double> &upper,std::valarray<double> &lower);
int EnvLabel_Stoyan(const TeBox &box, int nsim, int n1, int n2, int nh, double dmax,const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h,  
			std::valarray<double> &upper, std::valarray<double> &lower);
int EnvLabel_Ripley(const TeBox &box, int nsim, int n1, int n2, int nh, double dmax,const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h,  
			std::valarray<double> &upper, std::valarray<double> &lower);
int EnvLabelM(const TeBox &box, int nsim, int n1, int n2, int nh, double dmax,const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h, const std::vector<double> &m1,  
			const std::vector<double> &m2, int funcId, bool L, std::valarray<double> &upper, 
			std::valarray<double> &lower);
void rand_marks(int n1, int n2, std::vector<double> &m1, std::vector<double> &m2);
void torus(const TeBox &box, int n, std::vector<double> &x, std::vector<double> &y);
void rand_mark(std::vector<double> &A, int n);
void ppoisson( int n,const TeBox &box, std::vector<double> &x, std::vector<double> &y );
void maxmin(int nh,const std::valarray<double> &khat, std::valarray<double> &upper, std::valarray<double> &lower);
void rand_label(int n1, int n2, std::vector<double> &x1, std::vector<double> &y1, 
				 std::vector<double> &x2, std::vector<double> &y2);
void rand_labelM(int n1, int n2, std::vector<double> &x1, std::vector<double> &y1, 
				 std::vector<double> &x2, std::vector<double> &y2, std::vector<double> m1,
				 std::vector<double> m2);
#endif