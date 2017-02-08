#ifndef __SCANINT__
#define __SCANINT__

#include "Scan.h"
#include "util.h"
#include "ProspectiveAnalysis.h"


///Grid definido por eventos. Os pontos centrais de zonas são exatamente os eventos definidos em um conjunto qualquer.
class CGridEvent : public CScanGrid {

	CEventSet* mEvSet;

public:

	CGridEvent(CEventSet* eSet) {
		mEvSet = eSet;
	}

	~CGridEvent() {
	}

	
	//Numero total de pontos do grid
	virtual unsigned int Total() {
		return mEvSet->Size();
	}

	//Idx-esimo ponto do grid
	virtual CTPoint Get(unsigned int idx) {
		RASSERT((idx < mEvSet->Size()),"CGridEvent.Get");
		RASSERT((idx >= 0),"CGridEvent.Get");
		return mEvSet->Get(idx)->Location();
	}



};


///Zona espaco-temporal de contagem de eventos: interacao espaco-temporal.
 
class CSTZoneCount : public CSTZone {

protected:

	unsigned int mPop;
	unsigned int mMaxPop;
	unsigned int mMinPop;
	unsigned int mSZone;
	unsigned int mTZone;
	CCylinder    mSTDesc;
public:

	CSTZoneCount(unsigned int maxPop, unsigned int minPop) {
		mMaxPop = maxPop;
		mMinPop = minPop;
		mPop = 0;
	}

	//Atualiza valores internos da zona ao inserir novo evento
	virtual void InsertEvent(CSTPEvent* evt) {
#ifdef DEBUG
		//printf("%d ", evt->Identifier());
#endif
		mPop++;
	}

	//Intersecao vazia ou nao
	virtual bool Intercept(CSTZone* z) {
		CSTZoneCount* ztc = (CSTZoneCount *)z;
		return mSTDesc.Intercept(ztc->mSTDesc);
	}

	//Contem ou nao outra zona
	virtual bool Contain(CSTZone *z) {
		CSTZoneCount* ztc = (CSTZoneCount *)z;
		return mSTDesc.Contain(ztc->mSTDesc);
	}

	//Verifica se a zona eh valida de acordo com algum criterio interno colocar restricao de menor valor
	virtual bool IsValid() {
		//return ((mPop <= mMaxPop) && (mPop >= mMinPop));
		return mPop <= mMaxPop;
	}

	//Inicializa zona
	virtual void Initialize() {
		mPop = 0;
#ifdef DEBUG
		//printf("\n");
#endif
	}

	//Descricao espaco-temporal
	virtual void SetSTDescription(const CCylinder& c) {
		mSTDesc = c;
	}

	virtual void SetSAndTZones(unsigned int sZone, unsigned int tZone) {
		mSZone = sZone;
		mTZone = tZone;
	}

	virtual void Print( vector<int> &n_eventos,  vector<int>  &n_eventos_espaco,  vector<int> &n_eventos_tempo, vector<double> &centroX,  vector<double> &centroY,  vector<double> &raio, vector<double> &Tinicial, vector<double> &Tfinal) {
		n_eventos.push_back(mPop);
		n_eventos_espaco.push_back(mSZone);
		n_eventos_tempo.push_back(mTZone);
		mSTDesc.Print(centroX, centroY, raio,Tinicial,Tfinal);
	}

	

	virtual CSTZone* NewCopy() {
		CSTZoneCount* nZ = new CSTZoneCount(mMaxPop, mMinPop);
		(*nZ) = (*this);
		return nZ;
	}


	virtual void Copy(CSTZone *dest) {
		CSTZoneCount* nZ = (CSTZoneCount *)dest;
		(*this) = (*nZ);
	}

	int SpaceTimeEvents() {
		return mPop;
	}

	int SpaceEvents() {
		return mSZone;
	}

	int TimeEvents() {
		return mTZone;
	}

	unsigned int Population() {
		return (unsigned int) mPop;
	}

};

///Classe para zona onde mantem contador de covariavel
 class CSTZoneCov : public CSTZoneCount {

	 double mCovSum;
 public:

	 CSTZoneCov(int max, int min):CSTZoneCount(max,min) {
		 mCovSum = 0.0;
	 }

	//Metodos virtuais
	//Atualiza valores internos da zona ao inserir novo evento
	virtual void InsertEvent(CSTPEvent* evt) {
		CSTZoneCount::InsertEvent(evt);
		CSTPCEvent* evCov = (CSTPCEvent *)evt;
		mCovSum += evCov->Covariable();
	}

	virtual CSTZone* NewCopy() {
		CSTZoneCov* nZ = new CSTZoneCov(mMaxPop, mMinPop);
		(*nZ) = (*this);
		return nZ;
	}

	virtual void Copy(CSTZone *dest) {
		CSTZoneCov* nZ = (CSTZoneCov *)dest;
		(*this) = (*nZ);
	}

	//Inicializa zona
	virtual void Initialize() {
		mCovSum = 0;
		CSTZoneCount::Initialize();
	}

	double CovariableSum() {
		return mCovSum;
	}
 };

///Estruturas auxiliares
typedef struct dDesc {
	unsigned int evNum;
    double       evDist;
	bool         evAvailable; 
} SDistDesc;

typedef struct gDesc {
	unsigned int evTot;
    SDistDesc*   evVec;
} SGridDesc;
    

///Classe para geracao interativa de zonas espaco-temporais.Recebe limiares para espaco e tempo. Gera as zonas definindo primeiramente distancias espaciais.
class CSTZGenerator : public CScanZoneGenerator {

protected:


	CScanGrid          * mGrid;
	CEventSet          * mEvts;
	CSequencePermutation mPerm;

	//Limiares para cortes
	double     mSpaceTh;
	double     mTimeTh;
	long       mMinPopulation;

	//Vetores de armazenamento de zonas espaciais
	SGridDesc* mZoneSpatial;
	bool*      mSpatialNeighbor;
	SGridDesc  mZoneWork;

    //ordenacao no tempo
	unsigned int* mIndTime; 
	//posicao do i-simo no tempo
	unsigned int* mOrdTime; 
	//tempo do evento
	double*       mEvTime;  
	 //Indice do que é o primeiro vizinho no tempo
	unsigned int mIndZoneST; 
	//Indice do que é o ultimo vizinho no tempo
	unsigned int mIndZoneET;  
	//Indice do primeiro que é igual ou ultimo que eh menor no tempo
	unsigned int mIndZoneLGT;  
	//Indice do primeiro que é maior no tempo
	unsigned int mIndZoneGGT; 

	//Iteradores
	//Indice nos pontos do grid
	unsigned int mGridInd; 
	//Indice do tempo inicial
	unsigned int mSTInd;   
	//Se inicial é o grid
	bool         mSTGrid;  
	 //Indice do tempo final
	unsigned int mETInd;  
	//Se final é o grid
	bool         mETGrid;  
	//Indice no espaco
	unsigned int mSInd;    

	CTPoint      mGridPoint;
	CCylinder    mZoneDesc;
	CSTZone*     mZoneObj;

	bool         mNoZone;

	void NewGridPoint();
	void NewStartPoint();
	void NewEndPoint();
	void NewSpatialPoint();



public:

	///Construtor de geracao de zonas. inclusao para prospectivo
	 CSTZGenerator(CScanGrid* grid, CEventSet* evt,
		double percSpace,
		double percTime, long minPop, CSTZone* zObj);

	~CSTZGenerator();

	////////////////////////////////////////////////////////////

	//Inicializa geracao
	virtual void     Initialize();

	//Proxima zona
	virtual CSTZone* GetNext(); 


	//Proxima zona permutada
	virtual CSTZone* GetNextPerm() {
		return GetNext();
	}
};

class CScanSTIGenerator : public CSTZGenerator {

protected:
	void UpdateZoneCovariables(CSTZone* zone);

	virtual bool ValidGridPoint(CTPoint p) {
		return true;
	}

public:

	CScanSTIGenerator(CScanGrid* grid, CEventSet* evt,
		double percSpace,
		double percTime, long minPop, CSTZone* zObj):
	CSTZGenerator(grid, evt, percSpace, percTime, minPop, zObj) {
	}

	//Nova permutacao
	virtual void     NewPermutation();



};


///Classe para geracao de zonas do problema prospectivo
 class CScanAlarmGenerator : public CScanSTIGenerator {


protected:

	CProspectiveAnalysis* mRogerson;

	virtual bool ValidGridPoint(CTPoint p);

public:

	CScanAlarmGenerator(CScanGrid* grid, CEventSet* evt,
						CProspectiveAnalysis* cSum,
		double percSpace,
		double percTime, long minPop, CSTZone* zObj,  vector<int> &eventos, vector<double> &espX, 
		 vector<double> &varX,  vector<double> &padZi,  vector<double> &somi);

	//Nova permutacao
	virtual void     NewPermutation( vector<int>  &eventos,  vector<double> &espX,
       vector<double> &varX,  vector<double> &padZi,  vector<double> &somi);
	
};

#endif
