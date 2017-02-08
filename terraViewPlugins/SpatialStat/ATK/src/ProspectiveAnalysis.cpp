// ProspectiveAnalysis.cpp: implementation of the CProspectiveAnalysis class.
//
//////////////////////////////////////////////////////////////////////


#include "Clustering.h"
#include "ProspectiveAnalysis.h"
#include "Geometry.h"
#include "util.h"
#include "Event.h"
#include <vector>


using namespace std;

  void mdelete2(void** p) {
	delete *p;
	*p = NULL;
}

void mvdelete2(void** p) {
	delete [] *p;
	*p = NULL;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEventSet* gEvtSet;

CProspectiveAnalysis::CProspectiveAnalysis(CSortedAEventSet* evSet, double sRadius, double tRadius)
{

	//Atribui parametros
	mEvtSet = evSet;
	gEvtSet = mEvtSet;
	mSpatialRadius = sRadius;
	mTemporalRadius = tRadius;
	mIdxAlarm = 0;

	//Avalia distancias espaciais guardando em vetor se sao vizinhos ou nao
	mSpatialNeigh = new bool[mEvtSet->Size() * mEvtSet->Size()];
	CTPoint p;
	bool neigh;
	for(int i = 0; i < mEvtSet->Size(); i++) {
		mSpatialNeigh[i * mEvtSet->Size() + i] = false;
		p = mEvtSet->Get(i)->Location();
		for(int j = i+1; j < mEvtSet->Size(); j++) {
			neigh = (p.Distance(mEvtSet->Get(j)->Location()) <= mSpatialRadius);
			mSpatialNeigh[j * mEvtSet->Size() + i] =
			   mSpatialNeigh[i * mEvtSet->Size() + j] = neigh;

		}
	}

	this->mNumTempNeigh = new int[mEvtSet->Size()];
	this->mCSum = new double[mEvtSet->Size()];
}

///Avalia limiar de alarme para que haja probabilidade de alpha falso positivos durante nEvts. tIni, nIter e prec sao parametros para o Newton Raphson.
double CProspectiveAnalysis::AlarmThreshold(double alpha, long nEvts, double tIni, long nIter, double prec) {

	//calucla average run length
	mARL = -1*nEvts/log(1-alpha);

        // calcula h crítico usando método numérico de Newton Raphson e os
        // parâmetros: h inicial, max iteração, convergência, ARL
	double fh, dfh, dif, tAnt;
	int i;

	i = 0;
	fh  = 2*(exp(tIni+1.166)-tIni-2.166)-mARL;
	dfh = 2*(exp(tIni+1.166)-1);
	tAnt = tIni;
	do
	{
		i++;
		mThreshold = tAnt - fh/dfh;
		dif = fabs(mThreshold-tAnt);

		fh  = 2*(exp(mThreshold+1.166)-mThreshold-2.166)-mARL;
		dfh = 2*(exp(mThreshold+1.166)-1);
		tAnt = mThreshold;
	} while(i < nIter && dif > prec);


        return(mThreshold);
}

double max(double X, double Y)
  {
  double maxi;
  if(X>=Y)
    maxi=X;
  else
    maxi=Y;

  return(maxi);
  }



///Calcula valor de interacao espaco-temporal, somente habilitando alarme para os ultimos nLastEvents (soh neles o valor de zi sera atualizado
void  CProspectiveAnalysis::SearchAlarm( vector<int>  &eventos,  vector<double> &espX,
       vector<double> &varX,  vector<double> &padZi,  vector<double> &somi) {

		   
        eventos.resize(mEvtSet->Size());
        espX.resize(mEvtSet->Size());
        varX.resize(mEvtSet->Size());
        padZi.resize(mEvtSet->Size());
        somi.resize(mEvtSet->Size());


      //	fprintf(output,"T %lf\n", mThreshold);

 	CTPoint p;
	double nNT = 0, nNST = 0, nNSi = 0, nNSTi = 0, nNTi = 0;
	double nNTI2 = 0;

	double EX, VX, Zi, Si;

	//Itera nos ultimos eventos para avaliar se houve alarme
	for(int evt = 0; evt < mEvtSet->Size(); evt++)	{
		

		p = mEvtSet->GetOrdered(evt)->Location();
		mNumTempNeigh[evt] = 0;
		//Atualiza vizinhos temporais e
		//E espacial e espaco-temporal do evento
		//Atualiza soma geral ao quadrado e soma de vizinhos no tempo
		nNTi = nNSi = nNSTi = 0;
		for(int i = 0; i < evt; i++) {
			if (p.TemporalDistance(mEvtSet->GetOrdered(i)->Location()) <= mTemporalRadius) {
				nNTi++;
				nNTI2 += 2 * mNumTempNeigh[i] + 1;
				mNumTempNeigh[i]++;
				if (mSpatialNeigh[i*mEvtSet->Size()+evt]) {
					nNSTi++;
					nNSi++;
				}
			}
			else if (mSpatialNeigh[i*mEvtSet->Size()+evt])
				nNSi++;
		}

		mNumTempNeigh[evt] = nNTi;
		nNTI2 += (nNTi * nNTi);

		//Atualiza valores globais
        nNT += nNTi;
		nNST += nNSTi;

		if (evt <= 1) {
			Si = 0;
			Zi = 0;
			((CSTPCEvent *)mEvtSet->Get(evt))->SetCovariable(Zi);
			continue;
		}

		//Calcula esperanca
		EX = 2.0 * (double)nNT * (double)nNSi / ((double)evt * ((double)evt+1.0));

		//Calcula variancia
		VX = ((double)nNSi*((double)nNSi-1.0)*((double)nNTI2-2.0*(double)nNT));
		VX /= (((double)evt + 1.0) * (double)evt * ((double)evt - 1.0));
		VX += (EX - (EX * EX));
		if (VX < 0) {
			printf("OOPSS");
		}
		RASSERT(VX >= 0.0,"Erro no calculo da variancia");
		if (VX == 0.0) {
			VX = 0.000001;
		}
		//Calcula Zi (padronizacao de nSTi) e Si (soma acumulada)

		Zi = ((double)nNSTi - EX - 0.5) / sqrt(VX);
		((CSTPCEvent *)mEvtSet->Get(evt))->SetCovariable(Zi);
		Si = max(0, Si + Zi - 0.5);
		mCSum[evt] = Si;

                eventos[evt]= evt;
                espX[evt]=EX;
                varX[evt]=VX;
                padZi[evt]=Zi;
                somi[evt]=Si;


	
	}
}


CProspectiveAnalysis::~CProspectiveAnalysis()
{

	mvdelete2((void **)&mSpatialNeigh);
	mvdelete2((void **)&mNumTempNeigh);
	mvdelete2((void **)&mCSum);
}
