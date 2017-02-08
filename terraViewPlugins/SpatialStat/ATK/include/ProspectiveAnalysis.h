// ProspectiveAnalysis.h: interface for the CProspectiveAnalysis class.
//
//////////////////////////////////////////////////////////////////////

/*#if !defined(AFX_PROSPECTIVEANALYSIS_H__6195B53F_8FB4_42B4_AD8B_0FE41CA1AF87__INCLUDED_)
#define AFX_PROSPECTIVEANALYSIS_H__6195B53F_8FB4_42B4_AD8B_0FE41CA1AF87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000*/

#ifndef __PROSPECTIVEANALYSISH__
#define __PROSPECTIVEANALYSISH__


#include "Event.h"

/// Classe para metodo de analise prospectiva de cluster espaco-temporal baseado no metodo desenvolvido para o SIDS
 
class CProspectiveAnalysis  
{

protected:

	//Conjunto de eventos
	CSortedAEventSet* mEvtSet;
	
	//Parametros
	double     mSpatialRadius;
	double     mTemporalRadius;

	double     mARL;         //Average Run Length 
	double     mThreshold;   //Limiar para soar alarme a partir da soma acumulada

	//Vetores auxiliares
	bool*      mSpatialNeigh; //Guarda vizinhanca espacial entre i e j
	int*       mNumTempNeigh;
	double*    mCSum;
	int        mIdxAlarm;

public:

	CProspectiveAnalysis(CSortedAEventSet* evSet, double sRadius, double tRadius);

	double AlarmThreshold(double alpha, long nEvents, double tIni, long nIter, double prec);
	
	///Avalia zi e soma acumulada para os ultimos nLastEvents alarmes
	void SearchAlarm( vector<int>  &eventos,  vector<double> &espX,
       vector<double> &varX,  vector<double> &padZi,  vector<double> &somi);

	///Define pontos onde o alarme soou. Indice do evento
	 
	int InitialAlarmIndex() {
		mIdxAlarm = 0;
		return NextAlarmIndex();
	}
	
	int NextAlarmIndex() {
		for(; mIdxAlarm < mEvtSet->Size(); mIdxAlarm++) {
			if (mCSum[mIdxAlarm] >= mThreshold) {
				mIdxAlarm++;
				return mIdxAlarm-1;
			}
		}
		return mIdxAlarm;
	}

	virtual ~CProspectiveAnalysis();

};

#endif // !defined(AFX_PROSPECTIVEANALYSIS_H__6195B53F_8FB4_42B4_AD8B_0FE41CA1AF87__INCLUDED_)
