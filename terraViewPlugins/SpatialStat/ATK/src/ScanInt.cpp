//#include "stdafx.h"
#include "ScanInt.h"



////////////////////////////////////////////////////////////////////////
/// Classe CSTZGenerator

int DistanceCompare(const void* arg1, const void* arg2) {

	SDistDesc a1 = *(SDistDesc *)arg1;
	SDistDesc a2 = *(SDistDesc *)arg2;

	if (a1.evDist < a2.evDist) 
		return -1;
	if (a1.evDist == a2.evDist)
		return 0;
	return 1;

}

double* gVectTime;

int TimeCompare(const void* arg1, const void* arg2) {

	double a1 = gVectTime[*(unsigned int *)arg1];
	double a2 = gVectTime[*(unsigned int *)arg2];
	if (a1 < a2)
		return -1;
	if (a1 == a2)
		return 0;
	return 1;

}

CSTZGenerator::CSTZGenerator(CScanGrid* grid, CEventSet* evt, double thSpace,
							 double thTime, long minPop, CSTZone* zObj):mPerm(evt->Size()) {

	mGrid = grid;
	mEvts = evt;
	mSpaceTh = thSpace;
	mTimeTh = thTime;
	mMinPopulation = minPop;
	mZoneObj = zObj;
	//Distancias espaciais
	mZoneSpatial = new SGridDesc[mGrid->Total()];
	unsigned int i;
	for(i = 0; i < mGrid->Total(); i++) {
		mZoneSpatial[i].evVec = NULL;
	}
	mZoneWork.evVec = new SDistDesc[mEvts->Size()];
	mSpatialNeighbor = new bool[mGrid->Total() * mEvts->Size()];

	//Itera em cada um dos pontos do grid jah deixando calculadas zonas espaciais
	CTPoint gridPoint, evPoint;
	unsigned int p,e;

	double dist;

	for(p = 0; p < mGrid->Total(); p++) {
		gridPoint = mGrid->Get(p);
		
		//Calcula distancias, verifica se eh vizinho e inicializa vetor 
		mZoneWork.evTot = 0;
		for(e = 0; e < mEvts->Size(); e++) {
			evPoint = mEvts->Get(e)->Location();
			dist = gridPoint.Distance(evPoint);
			if (dist <= mSpaceTh) {
				mZoneWork.evVec[mZoneWork.evTot].evDist = dist;
				mZoneWork.evVec[mZoneWork.evTot].evNum = e;
				mZoneWork.evVec[mZoneWork.evTot].evAvailable = true;
				mZoneWork.evTot++;
				mSpatialNeighbor[p * mEvts->Size() + e] = true;
			}
			else {
				mSpatialNeighbor[p * mEvts->Size() + e] = false;
			}
		}

		//Inicializa zonas espaciais do ponto do grid e ordena
		mZoneSpatial[p].evTot = mZoneWork.evTot;
		mZoneSpatial[p].evVec = new SDistDesc[mZoneWork.evTot];
#ifdef DEBUG
		//printf("%d %d\n", p+1, mZoneWork.evTot);
#endif
		for(e = 0; e < mZoneWork.evTot; e++) {
			mZoneSpatial[p].evVec[e] = mZoneWork.evVec[e];

		}
		
		qsort(mZoneSpatial[p].evVec, mZoneSpatial[p].evTot,
			sizeof(SDistDesc), DistanceCompare);

#ifdef DEBUG
		/*for(e = 0;  e < mZoneWork.evTot; e++) {
			printf("%d %lf\n", mZoneSpatial[p].evVec[e].evNum+1, mZoneSpatial[p].evVec[e].evDist);
		}*/
#endif
	}

	//Zonas Temporais
	mIndTime = new unsigned int[mEvts->Size()];
	mOrdTime = new unsigned int[mEvts->Size()];
	mEvTime = new double[mEvts->Size()];
	gVectTime = mEvTime;

}


CSTZGenerator::~CSTZGenerator() {
	unsigned int i;
	for(i = 0; i < mGrid->Total(); i++) {
		mvdelete((void **)&(mZoneSpatial[i].evVec));
	}
	mvdelete((void **)&mZoneSpatial);
	mvdelete((void **)&(mZoneWork.evVec));
	mvdelete((void **)&(mSpatialNeighbor));

	mvdelete((void **)&mIndTime);
	mvdelete((void **)&mOrdTime);
	mvdelete((void **)&mEvTime);
}


/**
 * Prepara geracao de zonas temporais.
 * Para cada ponto do grid vai guardar os indices de inicio e termino de zonas
 * temporais
 **/
void CSTZGenerator::Initialize() {

	unsigned int e;
	for(e = 0; e < mEvts->Size(); e++) {
		//Alteracao para pegar o tempo corretamente
		mEvTime[e] = mEvts->Get(e)->Location().mT;
		mIndTime[e] = e;
	}

	//Ordena eventos quanto ao tempo
	qsort(mIndTime, mEvts->Size(), sizeof(unsigned int), TimeCompare);	

	//Define posicao de eventos no tempo
	for(e = 0; e < mEvts->Size(); e++) {
		mOrdTime[mIndTime[e]] = e;
	}

	//Inicializa iteradores
	mGridInd = 0;
	NewGridPoint();
}

/**
 * Para um novo ponto do grid, calcula limites de vizinhos temporais
 **/
void CSTZGenerator::NewGridPoint() {


	mGridPoint = mGrid->Get(mGridInd);
	mZoneDesc.mBase.NewCenter(mGridPoint);

	//Verifica se ponto do grid pode ser utilizado como centro de cluster
	if (this->ValidGridPoint(mGridPoint)) {
		mIndZoneST = 0;
	}
	else {
		mIndZoneST = mEvts->Size();
	}

	for(;
		(mIndZoneST < mEvts->Size()) &&
	    ((mGridPoint.mT - mEvTime[mIndTime[mIndZoneST]]) > mTimeTh);
		mIndZoneST++);
	
//Numero de vizinhos temporais e dado por mIndZoneET - mIndZoneST
	//Impossivel gerar zona pois nao esta inscrito temporalmente
	if (mIndZoneST == mEvts->Size()) {
		this->mSInd = mEvts->Size();
		this->mETInd = mIndZoneET = mEvts->Size();
		this->mSTInd = mIndZoneGGT = mEvts->Size();
		mNoZone = true;
		return;
	}
	//Vai definir limite para iniciar o final dos cilindros: termina
	for(mIndZoneLGT = mIndZoneST; 
	   (mIndZoneLGT < mEvts->Size()) && 
	   (mEvTime[mIndTime[mIndZoneLGT]] < mGridPoint.mT); 
	   mIndZoneLGT++);

	if (mEvTime[mIndTime[mIndZoneLGT]] > mGridPoint.mT) {
		mIndZoneGGT = mIndZoneLGT;
		mIndZoneLGT--;
	}
	else {
		//mIndZoneGGT soh sera igual se for o ultimo ponto do vetor
		for(mIndZoneGGT = mIndZoneLGT; 
			(mIndZoneGGT < mEvts->Size()) && 
			(mEvTime[mIndTime[mIndZoneGGT]] == mGridPoint.mT); 
			mIndZoneGGT++);
	}

	for(mIndZoneET = mIndZoneGGT;
	    (mIndZoneET < mEvts->Size()) &&
	    (fabs(mEvTime[mIndTime[mIndZoneET]] - mGridPoint.mT) <= mTimeTh); 
		mIndZoneET++);

	

	//Verifica primeiro ponto que eh vizinho espacial
	for(mSTInd = mIndZoneST;
		(mSTInd < mIndZoneGGT) &&
		(!mSpatialNeighbor[mGridInd * mGrid->Total() + mIndTime[mSTInd]]);
		mSTInd++);
	
#ifdef DEBUG
	/*printf("\n%d %d %d %d %d\n", mGridInd, mIndZoneST, mIndZoneLGT, mIndZoneGGT, mIndZoneET);*/
#endif

	NewStartPoint();
}


/**
 * Um novo ponto inicial vizinho no espaco foi gerado.
 * Define primeiro ponto final vizinho no espaco que pode ser um evento ou o 
 * proprio tempo do grid
 **/
void CSTZGenerator::NewStartPoint() {

	mSTGrid = false;
	mETGrid = false;

	//Nao possui vizinhos espaco-temporais com tempo menor, entao tempo inicial
	//eh o do grid, se nao for igual ao de evento -- nesse caso nao existe zona
	if (mSTInd == mIndZoneGGT) {
		mSTGrid = true;
		//Nao existe zona espaco-temporal jah que nenhum eh vizinho no espaco
		if (mEvTime[mIndTime[mIndZoneLGT]] == mGridPoint.mT) {
			mETInd = mIndZoneET;
			mSInd = mZoneSpatial[mGridInd].evTot;
			mNoZone = true;
			return;
		}
		else {
			mZoneDesc.mTInitial = mGridPoint.mT;
			mETInd = mIndZoneGGT;
		}
	}
	else {
		mSTGrid = false;
		mZoneDesc.mTInitial = mEvTime[mIndTime[mSTInd]];
		//Inicio do fim
		if (mEvTime[mIndTime[mIndZoneLGT]] == mGridPoint.mT) {
			mETInd = mIndZoneLGT;
		}
		else {
			mETInd = mIndZoneGGT;
		}
	}

	//Procura primeiro final com vizinho espacial
	for(;
		(mETInd < mIndZoneET) &&
		(!mSpatialNeighbor[mGridInd * mGrid->Total() + mIndTime[mETInd]]);
		mETInd++);

		if (mSTGrid) {
			//printf(" F %d\n", mETInd);
		}
	NewEndPoint();
	
}

/**
 * Um novo ponto final foi gerado.
 * Define a menor zona espacial que contem ambos os limites 
 **/
void CSTZGenerator::NewEndPoint() {

	bool foundS;
	bool foundE;

	//Verifica se conseguiu algum que eh vizinho espacial
	if (mETInd == mIndZoneET) {
		mETGrid = true;
		//Se ponto inicial jah era grid
		if (mSTGrid) {
			mSInd = mZoneSpatial[mGridInd].evTot;
			mNoZone = true;
			return;
		}
		//Final eh o grid
		mZoneDesc.mTFinal = mGridPoint.mT;
	}
	else {
		mETGrid = false;
		mZoneDesc.mTFinal = mEvTime[mIndTime[mETInd]];
	}

	//Verifica para qual tem que achar raio
	foundS = mSTGrid;
	foundE = mETGrid;

	//Menor zona espacial deve conter eventos que estao nos limites temporais
	mSInd = 0;
	unsigned int evNum;
	double evTime;
	double radius;
	mZoneObj->Initialize();
#ifdef DEBUG
	//printf("%d %d %d: ", mGridInd+1, mIndTime[mSTInd]+1, mIndTime[mETInd]+1);
#endif
	while((mZoneObj->IsValid()) && (mSInd < mZoneSpatial[mGridInd].evTot) 
		&& ((!foundS) || (!foundE))) {
	
		evNum = mZoneSpatial[mGridInd].evVec[mSInd].evNum;
		evTime = mEvTime[evNum];
		if (!foundS)
			foundS = (evTime == mZoneDesc.mTInitial);
		if (!foundE)
			foundE = (evTime == mZoneDesc.mTFinal);
	
		//Verifica se esta inserido no espaco-tempo
		if (mZoneDesc.InsideTime(evTime)) {
			//Vai inserir evento
			mZoneObj->InsertEvent(mEvts->Get(evNum));
			radius = mZoneSpatial[mGridInd].evVec[mSInd].evDist;
		}
		mSInd++;
	}

	//Verifica se existe zona valida com aquele tempo final
	//Se nao existe anda com os contadores
	if (!mZoneObj->IsValid()) {
		mNoZone = true;
		mSInd = mZoneSpatial[mGridInd].evTot;
		mETInd = mIndZoneET;
#ifdef DEBUG
		printf("Invalidado \n");
#endif
		return;
	}

	//O raio da zona foi definido, tem que inserir os outros que estao a mesma distancia
	mZoneDesc.NewRadius(radius);
	mZoneObj->SetSTDescription(mZoneDesc);
	while ((mZoneObj->IsValid()) && (mSInd < mZoneSpatial[mGridInd].evTot) &&
		(mZoneSpatial[mGridInd].evVec[mSInd].evDist == mZoneDesc.Radius())) {
		evNum =  mZoneSpatial[mGridInd].evVec[mSInd].evNum;
		if (mZoneDesc.InsideTime(mEvTime[evNum])) {
			mZoneObj->InsertEvent(mEvts->Get(evNum));
		}
		mSInd++;
	}

	//Verifica se existe zona valida com aquele tempo final
	//Se nao existe anda com os contadores
	if (!mZoneObj->IsValid()) {
		mNoZone = true;
		mSInd = mZoneSpatial[mGridInd].evTot;
		mETInd = mIndZoneET;
#ifdef DEBUG
		printf("Invalidado \n");
#endif
		return;
	}

#ifdef DEBUG
	printf("\n");
#endif

	if (mZoneObj->Population() <= 1) {
		NewSpatialPoint();
	}
	else {
		mNoZone = false;
	}
}

/**
 * Verifica para aqueles limites de tempo as sucessivas zonas espaciais possiveis
 * O valor de mSInd aponta para o proximo a ser inserido
 * Se for = mZoneSpatial[mGridInd].evTot acabou de processar
 **/
void CSTZGenerator::NewSpatialPoint() {

	bool found = false;
	double radius;
	unsigned int evNum;

	while((mZoneObj->IsValid()) && (mSInd < mZoneSpatial[mGridInd].evTot) && (!found)) {
		evNum = mZoneSpatial[mGridInd].evVec[mSInd].evNum;
		//Verifica se esta inserido no espaco-tempo
		if (mZoneDesc.InsideTime(mEvTime[evNum])) {
			//Vai inserir evento
			mZoneObj->InsertEvent(mEvts->Get(evNum));
			radius = mZoneSpatial[mGridInd].evVec[mSInd].evDist;
			found = true;
		}
		mSInd++;
	}

	//Nao possui mais zonas espaciais
	if (!found) {
		mSInd = mZoneSpatial[mGridInd].evTot;
		mNoZone = true;
		return;
	}

	//O raio da zona foi definido, tem que inserir os outros que estao a mesma distancia
	mZoneDesc.NewRadius(radius);
	mZoneObj->SetSTDescription(mZoneDesc);
	while ((mZoneObj->IsValid()) && (mSInd < mZoneSpatial[mGridInd].evTot) &&
		(mZoneSpatial[mGridInd].evVec[mSInd].evDist == mZoneDesc.Radius())) {
		evNum =  mZoneSpatial[mGridInd].evVec[mSInd].evNum;
		if (mZoneDesc.InsideTime(mEvTime[evNum])) {
			mZoneObj->InsertEvent(mEvts->Get(evNum));
		}
		mSInd++;
	}

    //Verifica se existe zona valida com aquele limite espacial
	//Se nao existe anda com os contadores
	if (!mZoneObj->IsValid()) {
		mNoZone = true;
		mSInd = mZoneSpatial[mGridInd].evTot;
#ifdef DEBUG
		printf("Invalidado \n");
#endif
		return;
	}

#ifdef DEBUG
	printf("\n");
#endif
	mNoZone = false;
}

/**
 * Gera proxima zona atraves de iteracoes
 **/

CSTZone * CSTZGenerator::GetNext() {

	double prevTime;

	//Vai iterar enquanto nao tem zona
	while (mNoZone) {
		//Ja processou todas as espaciais?
		if (mSInd >= mZoneSpatial[mGridInd].evTot) {

			if (mETInd < mIndZoneET) {
			//Anda com indices de tempo igual ao atual
			prevTime = mEvTime[mIndTime[mETInd]];
			for(mETInd++;
			    (mETInd < mIndZoneET) &&
				(mEvTime[mIndTime[mETInd]] == prevTime); mETInd++);

			//Procura proximo final com vizinho espacial andando com mETInd
			for(;
				(mETInd < mIndZoneET) &&
				(!mSpatialNeighbor[mGridInd * mGrid->Total() + mIndTime[mETInd]]);
				mETInd++);
			}
			//Ja processou todos os tempos finais?
			if (mETInd >= mIndZoneET) {

				if (mSTInd < mIndZoneGGT) {
				//Anda com indices de tempo igual ao atual
				prevTime = mEvTime[mIndTime[mSTInd]];
				for(mSTInd++;
					(mSTInd < mIndZoneGGT) &&
					(mEvTime[mIndTime[mSTInd]] == prevTime); mSTInd++);
				//Procura proximo final vizinho espacial andando em mSTInd
				//Verifica primeiro ponto que eh vizinho espacial
				for(;
					(mSTInd < mIndZoneGGT) &&
					(!mSpatialNeighbor[mGridInd * mGrid->Total() + mIndTime[mSTInd]]);
					mSTInd++);	
				}
				//Ja processou todos os tempos iniciais?
				if (mSTInd >= mIndZoneGGT) {
					mGridInd++;
					//printf("%d \n", mGridInd);
					//Ja processou todos os pontos do grid?
					if (mGridInd >= mGrid->Total()) {
						return NULL;
					}
					else {
						NewGridPoint();
					}
				}
				//Define proximo final andando com mSTInd ate ter vizinho espacial
				else {
					//printf("S");
					NewStartPoint();		
				}
			}
			else {
				NewEndPoint();
			}
		}
		//Gera nova zona espacial se houver
		else {
			NewSpatialPoint();
		}
		
		//Verifica limite inferior de eventos
		//09/01/03
		if ((!mNoZone) && (mZoneObj->Population() < mMinPopulation)) {
			mNoZone = true;
		}
	}

	UpdateZoneCovariables(mZoneObj);

	mNoZone = true;
	return mZoneObj;
}

/**
 * Atualiza valores de vizinhos no espaco e no tempo
 * 15/01/03
 **/
void CScanSTIGenerator::UpdateZoneCovariables(CSTZone* zone) {

	CSTZoneCount* z = (CSTZoneCount *)zone;

	//Tem uma zona -- vai retorna-la
	unsigned int spatialNeighbors = mSInd;	
	
	//Vizinhos temporais -- tem que olhar os indices para determinar numero
	//de vizinhos -- tomar cuidado com os limites
	int iST;
	//Inicial eh sem evento
	if (mSTGrid) {
		iST = mIndZoneGGT;
		//printf("Mudei pois eh inicial\n");
	}
	else {
		if (mSTInd >= mEvts->Size())
			printf("Erro na definicao de inicio\n");
		iST = mSTInd;
	    for(; (iST > 0) && 
			  (mEvTime[mIndTime[iST-1]] == mEvTime[mIndTime[mSTInd]]); 
			iST--);
	}

	int iET;

	if (mETGrid) {
		iET = mIndZoneLGT; //primeiro menor
		//printf("Mudei pois eh final\n");
	}
	else {
		iET = mETInd;
		if (mETInd >= mEvts->Size())
			printf("Erro na definicao de fim\n");
		for(; (iET < mEvts->Size()-1) && (mEvTime[mIndTime[iET+1]] == mEvTime[mIndTime[mETInd]]); iET++);
	}

	//if ((mETGrid) || (mSTGrid))
	  //printf("%d %d %d %d %d %d %d %d\n", mZoneObj->Population(), mSTInd, mETInd, iST, iET, iET-iST+1,mIndZoneGGT,mIndZoneET);


	unsigned int temporalNeighbors = iET - iST + 1;

	z->SetSAndTZones(spatialNeighbors, temporalNeighbors);
}

/**
 * Inicializa permutacao de tempo
 * 25/11/02
 * 09/01/03 -- alteracao para passar permutacao para grid (estava errando na hora de pegar
 * os pontos)
 **/
void CScanSTIGenerator::NewPermutation() {

	mPerm.NewSequence();
	mEvts->NewTimePermutation(&mPerm);
	Initialize();
}


/**
 * Construtor, deve chamar alarme para gerar covariaveis
  **/
CScanAlarmGenerator::CScanAlarmGenerator(CScanGrid* grid, CEventSet* evt, CProspectiveAnalysis* proc,
   double percSpace, double percTime, long minPop, CSTZone* zObj,  vector<int> &eventos,  vector<double> &espX,
    vector<double> &varX,  vector<double> &padZi,    vector<double> &somi) :
   
	CScanSTIGenerator(grid, evt, percSpace, percTime, minPop, zObj) {

	mRogerson = proc;
	mRogerson->SearchAlarm(eventos, espX, varX, padZi, somi);
}



/**
 * Gera nova permutacao e depois novos valores zi para eventos
 * 15/01/03
 **/
void CScanAlarmGenerator::NewPermutation( vector<int>  &eventos,  vector<double> &espX,
       vector<double> &varX,  vector<double> &padZi,  vector<double> &somi) {
	mPerm.NewSequence();
	mEvts->NewTimePermutation(&mPerm);
	mRogerson->SearchAlarm(eventos, espX, varX, padZi, somi);
	Initialize();
}


/**
 * Verifica se um ponto do grid pode ser utilizado para definir alarme
 * Para isso verifica se ele eh um ponto dentro de soar alarme
 * 16/01/03
 **/
bool CScanAlarmGenerator::ValidGridPoint(CTPoint p) {

	return true;

	int idx;
	idx = mRogerson->InitialAlarmIndex();
	while (idx < mEvts->Size()) {
		if ( (mEvts->Get(idx)->Location().mT - p.mT) > this->mTimeTh )
			return true;
		idx = mRogerson->NextAlarmIndex();
	}
	return false;
}