//#include "stdafx.h"
#include<vector>
#include <float.h>
#include "Event.h"
#include "util.h"

using namespace std;



//////////////////////////////////////////////////////////////////////
// CSTPEvent
//////////////////////////////////////////////////////////////////////

         
CSTPEvent::CSTPEvent(double x, double y, double t) {

         mP.mX=x;
         mP.mY=y;
         mP.mT=t;

}

/*double __min(double X, double Y)
  {
  double min;
  if(X<=Y)
    min=X;
  else
    min =Y;
  return (min);
  }

 double __max(double X, double Y)
  {
  double max;
  if(X>=Y)
    max=X;
  else
    max =Y;
  return (max);
  }*/



//////////////////////////////////////////////////////////////////////
// CArrayEventSet
//////////////////////////////////////////////////////////////////////

///Le arquivo com eventos e cria vetor com os mesmos -- evento com covariaveis
CArrayEventSet::CArrayEventSet( vector<double> coordx,  vector<double> coordy,  vector<double> coordt, short type_event) {

//Conta eventos

        mTotal=coordx.size();

	//Le eventos e define limiares espaco tempo
	double minX, maxX, minY, maxY, minT, maxT;

	minX = minY = minT = DBL_MAX;
	maxX = maxY = maxT = -DBL_MAX;

	unsigned int e = 0;
	mEvts = new CSTPEvent *[mTotal];
	this->mEvTimes = new double[mTotal];


        for(unsigned int e=0;e<mTotal;e++) {
            switch(type_event) {
		case EV_STP:
			mEvts[e]= new CSTPEvent(coordx[e], coordy[e], coordt[e]);
			
			break;
		case EV_STPC:
			mEvts[e] = new CSTPCEvent(coordx[e], coordy[e], coordt[e]);
			
			break;

			
		}
		mEvTimes[e] = mEvts[e]->Location().mT;
		minX = min(minX, mEvts[e]->Location().mX);
		maxX = max(maxX, mEvts[e]->Location().mX);
		minY = min(minY, mEvts[e]->Location().mY);
		maxY = max(maxY, mEvts[e]->Location().mY);
		minT = min(minT, mEvts[e]->Location().mT);
		maxT = max(maxT, mEvts[e]->Location().mT);

		
	}



	double height = maxY - minY;
	double width = maxX - minX;
	this->mLinearSize = (height > width) ? height : width;
	this->mTimeInterval = maxT - minT;
	this->mMaxTime = maxT;
}

CArrayEventSet::~CArrayEventSet() {

	unsigned int e;
	for(e = 0; e < mTotal; e++)
		mdelete((void **)mEvts+e);

	mvdelete((void **)&mEvts);
	mvdelete((void **)&mEvTimes);
}


double* gTimes;

int TimeOrdering(const void *a, const void* b) {

	if (gTimes[(*((int *)a))]>
		gTimes[(*((int *)b))])
		return 1;
	return -1;
}

void CSortedAEventSet::SortEvents() {
	for(unsigned int e = 0; e < mTotal; e++) {
		mIndEvt[e] = e;
	}
	qsort(mIndEvt, mTotal, sizeof(int), TimeOrdering);
}


CSortedAEventSet::CSortedAEventSet( vector<double> coordx,  vector<double> coordy,  vector<double> coordt, short type_evt) :
	  CArrayEventSet(coordx, coordy, coordt, type_evt) {
	gTimes = mEvTimes;
	mSize = -1;

	mIndEvt = new int[mTotal];
	SortEvents();
}

CSortedAEventSet::~CSortedAEventSet() {
	mvdelete((void **)mIndEvt);
}
