#ifndef SV_H
#define SV_H

#include <vector>

int OpenData(const char *ArqO, const char *ArqD, const char *ArqP,
			 std::vector<double> &xp, double &xmin, double &xmax,
             std::vector<double> &yp, double &ymin, double &ymax,              
             std::vector<double> &x1, std::vector<double> &y1, 
             std::vector<double> &x2, std::vector<double> &y2);
#endif
