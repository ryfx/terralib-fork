// Geometry.h: interface for the CCircle class.
//
//////////////////////////////////////////////////////////////////////

/*#if !defined(AFX_GEOMETRY_H__5690A4F7_32BF_4DE0_82A1_004F8B7A3DA7__INCLUDED_)
#define AFX_GEOMETRY_H__5690A4F7_32BF_4DE0_82A1_004F8B7A3DA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000*/

#ifndef __GEOMETRYH__
#define __GEOMETRYH__

#include <math.h>
#include<stdio.h>
#include<vector>


using namespace std;
///Classe para ponto espacial
class CMPoint  
{
public:

	//Coordenadas
	double mY;
	double mX;

	CMPoint() {
		mY = mX = 0;
	}

	CMPoint(double x, double y) {
		mX = x;
		mY = y;
	}

	~CMPoint() {}

	/// Distancia euclidiana entre dois pontos
	double Distance(const CMPoint& p) {
		return Distance(mX, mY, p.mX, p.mY);
	}

	///Distancia euclidiana para duas coordenadas
	 
	static double Distance(double x1, double y1, double x2, double y2) {

		double mx = x1 - x2;
		double my = y1 - y2;

		return sqrt((mx * mx) + (my * my));
	}

};

///Ponto espaco-temporal
 class CTPoint : public CMPoint {

public:

	//Tempo
	double mT;

	CTPoint() {
		mT = 0;
	}

	CTPoint(double x, double y, double t):CMPoint(x,y) {
		mT = t;
	}

	double TemporalDistance(CTPoint p) {
		return fabs(this->mT - p.mT);
	}

};


///Circulo
 class CCircle 
{
protected:
	double mRaio;
	CMPoint mCentro;

public:
	
	CCircle(CMPoint centro, double raio) {
		mRaio = raio;
		mCentro = centro;
	}

	CCircle() {
		mRaio = 0.0;
	}

	virtual ~CCircle(){}
	
	void NewRadius(double r) {
		mRaio = r;
	}

	void NewCenter(CMPoint p) {
		mCentro = p;
	}


	double Radius() {
		return mRaio;
	}

	bool Intercept(const CCircle& c) {
		double dist = mCentro.Distance(c.mCentro);
		return (dist <= (mRaio + c.mRaio));
	}


	bool Contain(const CCircle& c) {
		if (this->mRaio < c.mRaio)
			return false;
		double dist = mCentro.Distance(c.mCentro);
		return (this->mRaio >= dist + c.mRaio);
	}


	void Print( vector<double> &centroX,  vector<double> &centroY,  vector<double> &raio) {
		centroX.push_back(mCentro.mX);
		centroY.push_back(mCentro.mY);
		raio.push_back(mRaio);
		//fprintf(output," %lf %lf %lf ", mCentro.mX, mCentro.mY, mRaio);
	}

};

///Cilindro espaco-temporal. A base e um circulo e define tempo inicial e final.
 
class CCylinder 
{
protected:


public:
	CCircle mBase;
	//Tempo inicial e final
	double mTInitial;
	double mTFinal;

	CCylinder() {
		mTInitial = mTFinal = 0.0;
	}

	CCylinder(CCircle base, double ini, double fin) {
		NewValues(base, ini, fin);
	}
	
	void NewValues(CCircle base, double ini, double fin) {
		mBase = base;
		mTInitial = ini;
		mTFinal = fin;
	}
	
	void NewRadius(double radius) {
		mBase.NewRadius(radius);
	}

	bool InsideTime(double t) {
		return ((t <= mTFinal) && (t >= mTInitial));
	}

	double Radius() {
		return mBase.Radius();
	}

	bool Intercept(const CCylinder& c) {

		//Verifica intersecao no tempo 
		if (((c.mTInitial <= this->mTInitial) && (c.mTFinal >= this->mTInitial)) ||
			((this->mTInitial <= c.mTInitial) && (this->mTFinal >= c.mTInitial))) {
			return mBase.Intercept(c.mBase);
		}

		return false;
	}

	bool Contain(const CCylinder& c) {

		//Verifica contem no tempo
		if ((this->mTInitial <= c.mTInitial) && (this->mTFinal >= c.mTFinal)) {
			return mBase.Contain(c.mBase);
		}
		return false;
	}

	void Print( vector<double> &centroX,  vector<double> &centroY,  vector<double> &raio,  vector<double> &Tinicial,  vector<double> &Tfinal) {
		mBase.Print(centroX, centroY, raio);
		Tinicial.push_back(mTInitial);
		Tfinal.push_back(mTFinal);
		//fprintf(output," %lf %lf ", mTInitial, mTFinal);
	}
};



#endif // !defined(AFX_GEOMETRY_H__5690A4F7_32BF_4DE0_82A1_004F8B7A3DA7__INCLUDED_)
