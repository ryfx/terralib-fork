
/*#if !defined(AFX_EVENTO_H__C1CF063A_022F_4038_8C17_D704ACB96292__INCLUDED_)
#define AFX_EVENTO_H__C1CF063A_022F_4038_8C17_D704ACB96292__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000*/

#ifndef __EVENTH__
#define __EVENTH__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

#include "Geometry.h"

using namespace std;

//Tipos de arquivos de entrada
#define EV_LIXYT 0
#define EV_DXYT  1

//Tipo de evento (numero de covariaveis)
#define EV_STP   0
#define EV_STPC  1


/// Classe para evento espaco-temporal pontual
 
class CSTPEvent {

	long   mId;
	CTPoint mP;

protected:

	
	void SetTime(double t) {
		mP.mT = t;
	}

public:
	
	///Le evento de arquivo transformando-o em um ponto
	 CSTPEvent(double x, double y, double t);


	long Identifier() {
		return mId;
	}

	CTPoint Location() {
		return mP;
	}

	///Distancia espacial entre os eventos
	double SpatialDistance(const CSTPEvent& evt) {
		return mP.Distance(evt.mP);
	}

	double TemporalDistance(const CSTPEvent& evt) {
		return mP.TemporalDistance(evt.mP);
	}

	friend class CEventSet;
};


/// Classe para evento com uma covariavel
 
class CSTPCEvent : public CSTPEvent {

protected:

	double mCovariable;

public:

	CSTPCEvent(double x, double y, double t) : CSTPEvent(x, y, t) {
	}

	double Covariable() {
		return mCovariable;
	}

	void SetCovariable(double c) {
		mCovariable = c;
	}

};

///Classe para permutacao de um conjunto de 0 a mN-1. Gera permutacoes aleatorias independentes
 class CSequencePermutation {
	int *mV;
	int  mN;

public:

	CSequencePermutation(int n) {
		mN = n;
		mV = new int[mN];
		Initialize();
	}

	~CSequencePermutation() {
		delete [] mV;
	}

	/// Nova sequencia. Calcula fazendo novo valor aleatorio e verificando se nao existe. 
	 void NewSequence() {

		int e,f;
		for(e = 0; e < mN; ) {
			int valor = rand()%mN;
			for(f = 0; (f < e) && (mV[f] != valor); f++); 
			if (f == e) {
				mV[e] = valor;
				e++;
			}
		}
		
	}

	void Initialize() {
		for(int e = 0; e < mN; e++) {
			mV[e] = e;
		}
	}

	//Valor da permutacao no ponto
	int Value(int idx) { return mV[idx];}
};




///Classe para guardar um conjunto de eventos
 class CEventSet {

protected:

	void SetNewTime(unsigned int idx, double t) {
		CSTPEvent* evt = Get(idx);
		if (evt)
			evt->SetTime(t);
	}
	
public:

	//Pega evento pela posicao
	virtual CSTPEvent* Get(unsigned int idx) = 0;
	//Tamanho do conjunto
	virtual unsigned int Size() = 0;
	//Tamanho do intervalo de tempo do conjunto
	virtual double TimeInterval() = 0;
	//Maior tempo entre todos os eventos
	virtual double MaximumTime() = 0;
	//Maior dimensao (x ou y) linear do conjunto
	virtual double LinearSize() = 0;
	//Permutacao de tempo nos eventos -- para facilitar na hora de usar algoritmos
	virtual void NewTimePermutation(CSequencePermutation* perm) = 0;
};


///Classe que implementa conjunto de eventos como vetor simples
class CArrayEventSet : public CEventSet {

protected:

	CSTPEvent** mEvts;
	unsigned int mTotal;
	double       mTimeInterval;
	double       mMaxTime;
	double       mLinearSize;
	CSequencePermutation* mPerm;

	//Vetor para guardar tempos originais
	double*      mEvTimes;

public:

	CArrayEventSet( vector<double> coordx,  vector<double> coordy,  vector<double> coordt, short type_event);
	~CArrayEventSet();

	//Métodos Virtuais
	virtual CSTPEvent* Get(unsigned int idx) {

		if (idx < mTotal) {
			return mEvts[idx];
		}
		return NULL;
	}

	virtual unsigned int Size() {
		return mTotal;
	}

	virtual double MaximumTime() { return mMaxTime;}
	virtual double TimeInterval() { return mTimeInterval; }
	virtual double LinearSize() {return mLinearSize; }

	///Altera eventos originais atribuindo nova permutacao
	 virtual void NewTimePermutation(CSequencePermutation *perm) {

		int e;
		for(e = 0; e < mTotal; e++) {
			SetNewTime(e, mEvTimes[perm->Value(e)]);
		}
		mPerm = perm;
	}
};

///Conjunto que mantem os eventos ordenados pelo tempo
 
class CSortedAEventSet : public CArrayEventSet {

protected:

	int* mIndEvt;
	void SortEvents();
	int mSize;

public:
	CSortedAEventSet( vector<double> coordx,  vector<double> coordy,  vector<double> coordt, short type_evt);
	~CSortedAEventSet();

	
    //Metodos Virtuais

	virtual unsigned int Size() {
		if (mSize < 0)
			return mTotal;
		return mSize;
	}

	virtual void NewTimePermutation(CSequencePermutation *perm) {
		CArrayEventSet::NewTimePermutation(perm);
		SortEvents();
	}

	
	CSTPEvent* GetOrdered(unsigned int idx) {

		if (idx < mTotal) {
			return mEvts[mIndEvt[idx]];
		}
		return NULL;
	}

	void SetNewSize(int s) {
		mSize = s;
	}
};


#endif // !defined(AFX_EVENTO_H__C1CF063A_022F_4038_8C17_D704ACB96292__INCLUDED_)
