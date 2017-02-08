#ifndef KLABEL_H
#define KLABEL_H

#include <vector>
#include <valarray>
#include <KFunctionPluginWindow.h>
#include <TeBox.h>
#include <k.h>

int Klabel_Stoyan(const TeBox &box, int n1, int n2, int nh, double dmax,const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h, std::valarray<double> &khat);

int Klabel_Ripley(const TeBox &box, int n1, int n2, int nh, double dmax,const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h, std::valarray<double> &khat);

int KlabelM(const TeBox &box, int n1, int n2, int nh, double dmax,const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h,  
			const std::vector<double> &m1,  const std::vector<double> &m2, std::valarray<double> &khat, int funcId, bool L);

#endif