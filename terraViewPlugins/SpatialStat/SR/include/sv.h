
#ifndef SV_H
#define SV_H

#include <list>
#include <valarray>

struct SVEvent {
 double x, y, t;
 friend bool operator<(const SVEvent &a, const SVEvent &b) {
  return (a.t < b.t);
 }
};

typedef std::list<SVEvent> SVEventLst;

int SistemadeVigilancia(SVEventLst &, const double, const double, std::valarray<double> &);
int CalculaLambda(SVEventLst &ev, const double RaioC, const double epslon, std::valarray<double> &R, unsigned int &numObs);

#endif
