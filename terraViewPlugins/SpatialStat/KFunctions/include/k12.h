#ifndef K12_H
#define K12_H

#include <vector>
#include <valarray>
#include <KFunctionPluginWindow.h>
#include <TeBox.h>
#include <k.h>


int KStoyan12(const TeBox &box, int n1, int n2, int nh, double dmax, const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h, std::valarray<double> &khat);
int KRipley12(const TeBox &box, int n1, int n2, int nh, double dmax, const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h, std::valarray<double> &khat);
int KStoyan12M(const TeBox &box, int n1, int n2, int nh, double dmax, const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h, const std::vector<double> &m1,
			const std::vector<double> &m2, std::valarray<double> &khat);

#endif