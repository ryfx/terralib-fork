#include <map>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <list>
//#include <RogersonPluginWindow.h>

#ifndef algoritmos_H
#define algoritmos_H

struct SVEvent {
 double x, y, t;
 friend bool operator<(const SVEvent &a, const SVEvent &b) {
  return (a.t < b.t);
 }
};

typedef std::list<SVEvent> SVEventLst;

typedef struct sv_ent
{
   double x;
   double y;
   int t;

friend bool operator<(sv_ent &a, sv_ent &b){
       return(a.t<b.t);
    }    
}SistVigEnt;


double NRaphson(double arl);
int CalculaNSt (std::vector< std::vector<  short> > MSpaceTime, int ele);
int CalculaNS (std::vector< std::vector< short > > MSpace, int ele);
int CalculaNT (std::vector< std::vector< short > > MTime, int ele);
double CalculaVar(std::vector< int >NS,int ele, double Ni, double Soma, int nEvts);
void ConverteData(int unidade, std::vector<SistVigEnt>& Dados, std::vector<SistVigEnt>& DadosLimiar, int EvLimiar, int nEvts, char *inp);
std::vector<double> SistemadeVigilancia(SVEventLst &ev, std::vector<SistVigEnt> Dados, int nEvts, double k,double TempoC, long double RaioC, bool & wasCancelled);

#endif