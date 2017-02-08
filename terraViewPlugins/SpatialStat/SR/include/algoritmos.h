#ifndef algoritmos_H
#define algoritmos_H

typedef struct sv_ent{
 double x;
 double y;
 double t;
}SistVigEnt;

int CalculaNCj(short **MSpace, const int EvtN, const int EvtJ);
int ContaEvt(short **MSpace, const int EvtN, const int EvtJ);

#endif
