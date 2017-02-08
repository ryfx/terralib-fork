#ifndef __CLUSTERALGH__
#define __CLUSTERALGH__

#include "Scan.h"
#include "ScanInt.h"

///Classe para calcular a estatistica de teste de cluster para uma zona espaco-temporal
 
class CTestStatistics {

public:
	//Verossimilhanca de uma zona espaco-temporal
	virtual double ZoneLikelihood(const CSTZone* z) = 0;
};


/// Classe para calcular a estatistica de teste baseada em score para determinar se ha cluster no espaco-tempo
class CSTInteractionScore : public CTestStatistics {
protected:

	unsigned int mTotalEvents;

	double Likelihood(CSTZoneCount* z);

public:

	CSTInteractionScore(unsigned int te) {
		mTotalEvents = te;
	}

	virtual double ZoneLikelihood(const CSTZone * z) {
		return Likelihood((CSTZoneCount *)z);
	}
};


///Classe para verossimilhanca de cluster prospectivo

class CSTCovSum : public CTestStatistics {

public:

	virtual double ZoneLikelihood(const CSTZone* z) {

		CSTZoneCov* zone = (CSTZoneCov *)z;

		return zone->CovariableSum();
	}
};

///Classe para testar a existencia de cluster no espaco tempo
 
class CIdSTClusterTest  
{

protected:

	CScanZoneGenerator*   mZoneGen;
	CTestStatistics*      mTStat;

	//Manter mais significativas
	typedef struct lZone {
		double    like;
		CSTZone*  zone;
		struct lZone* next;
	} SLikeZone;

	SLikeZone*            mMoreLike;
	
	int                   mNumSecondary;
	int                   mNumNOver;
	double                mMaxLikeData;

public:

	CIdSTClusterTest(CScanZoneGenerator* gen, CTestStatistics* stat) {
		mZoneGen = gen;
		mTStat = stat;

	}

	//Avalia likelihood dos dados
	//Escreve todos na saida All se nao for nulo
	//Escreve os nZones mais significativos na saida output
	virtual void DataLikelihood(int nZones);


	//Avalia significancia dos resultados de dados por permutacoes
	//aleatorias. Realiza nPerm permutacoes aleatorias
	virtual double PValue(int nPerm);

	virtual ~CIdSTClusterTest() {
	}

};

///Gera um unico cluster de maior verossimilhanca

class CIdUniqueTest : public CIdSTClusterTest {

public:

	CIdUniqueTest(CScanZoneGenerator* gen, CTestStatistics* stat) :
	  CIdSTClusterTest(gen,stat) {
	}

	virtual void DataLikelihood();

};

///Gera os n clusters mais significativos mas recalculando os valores a cada passo
 
class CIdSTClusterRecalcTest : public CIdSTClusterTest {

protected:

	struct sDZone {
		double       like;
		CSTZoneCount* zone;
	};

	struct sDZone* mMoreLikely;

	//Verifica se zone nao intercepta as nZones primeiras zonas de mMoreLikely
	bool NotIntercept(int nZones, CSTZone* zone);

public:

	CIdSTClusterRecalcTest(CScanZoneGenerator* gen, CTestStatistics* stat) :
	  CIdSTClusterTest(gen,stat) {
	}


	//Avalia likelihood dos dados
	//Escreve todos na saida All se nao for nulo
	//Escreve os nZones mais significativos na saida output
	virtual void DataLikelihood(int nZones,  vector<int> &n_eventos,  vector<int> &n_eventos_espaco,  vector<int> &n_eventos_tempo,
		 vector<double> &veros,  vector<double> &centroX,  vector<double> &centroY,  vector<double> &raio,
            vector<double> &Tinicial,  vector<double> &Tfinal);

};
#endif