//#include "stdafx.h"
#include "ClusterAlg.h"
#include "float.h"
#include "util.h"
#include <TeQtProgress.h>


/// Estatistica de escore para identificar cluster espaco temporal -(N(ET) - (N(E) * N(T))/N(ET))/sqrt(N(E)*N(T)/N(ET))

double CSTInteractionScore::Likelihood(CSTZoneCount* z) {

	double razao = (double)z->SpaceEvents() * (double)z->TimeEvents() /
		(double)mTotalEvents;

	if (razao  <= 0.0) {
		//printf("%d %d %d\n", z->SpaceTimeEvents(), z->SpaceEvents(), z->TimeEvents());
		getchar();
	}

	//RASSERT((razao > 0.0),"Erro no likelihood");
		
	double like = ((double)z->SpaceTimeEvents() - razao)/sqrt(razao);
	return like;
}



///Avalia verossimilhanca dos dados para todas as zonas. O resultado desse algoritmo é o calculo dos valores em arquivo outAll se nao for nulo e os n mais significativos em output. Utiliza um vetor para manter as mais significativas.
void CIdSTClusterTest::DataLikelihood(int nZones){

	//Lista de mais significativos
	mNumSecondary = nZones;
	mNumNOver = 0;
	mMoreLike = new SLikeZone;
	mMoreLike->like = DBL_MAX;
	mMoreLike->next = NULL;
	mMoreLike->zone = NULL;

	mZoneGen->Initialize();

	CSTZone* nextZone;
	CSTZoneCount* z;
	double likeZ;
	while((nextZone = mZoneGen->GetNext()) != NULL) {

		likeZ = mTStat->ZoneLikelihood(nextZone);
		z = (CSTZoneCount *)nextZone;
		/*if (outAll != NULL) {
			fprintf(outAll,"%lf %d %d %d ", likeZ, z->SpaceTimeEvents(), z->SpaceEvents(), z->TimeEvents());
			z->Print(outAll);
			fprintf(outAll,"\n");
		}     */

		//Vai inserir somente se nenhum com likelihood maior que ele
		//nao estiver contido no mesmo
		bool contains = false;
		SLikeZone* aux = mMoreLike; //ponto de insercao
		while ((!contains) && (aux->next != NULL) && (aux->next->like > likeZ)) {
			contains = nextZone->Contain(aux->next->zone);
			aux = aux->next;
		}

		//Verifica se parou por intercessao -- nao vai inserir
		if (contains) {
			/*
			fprintf(output,"F %lf", likeZ);
			nextZone->Print(output);
			fprintf(output," contem %lf", aux->like);
			aux->zone->Print(output);
			fprintf(output,"\n");
			*/
			continue;
		}


		//Cria nova zona
		mNumNOver++;
		SLikeZone* newElem = new SLikeZone;
		newElem->like = likeZ;
		newElem->zone = nextZone->NewCopy();
		newElem->next = aux->next;
		aux->next = newElem;
		/*
		fprintf(output,"D %lf ", likeZ);
		nextZone->Print(output);
		fprintf(output,"\n");
		*/
		//Verifica se nos proximos menos verossimeis algum contem a zona
		//inserida, caso em que pode ser removido
		SLikeZone* delElem;
		aux = aux->next;
		while (aux->next != NULL) {
			if (aux->next->zone->Contain(nextZone)) {
				delElem = aux->next;
				/*
				fprintf(output,"E %lf ", delElem->like);
				delElem->zone->Print(output);
				fprintf(output," contem\n");
				*/
				aux->next = aux->next->next;
				mdelete((void **)&delElem);
				mNumNOver--;
			}
			else {
				aux = aux->next;
			}
		}
	}

	//Ao final escreve em output os nao sobrepostos
	//A lista ja esta ordenada, entao ao selecionar um, deve remover
	//todos os outros que se sobrepoem
	int i = 0;
	int max = min(mNumNOver, mNumSecondary);
	SLikeZone* most = mMoreLike;
	SLikeZone* overlap, *aux;
	mMaxLikeData = mMoreLike->like;

	while ((most->next != NULL) && (i < max)) {
		//remove sobrepostos da lista, desalocando
		aux = most->next;
		while(aux->next != NULL) {
			if (most->next->zone->Intercept(aux->next->zone)) {
				overlap = aux->next;
				aux->next = aux->next->next;
				/*
				fprintf(output,"O %lf ",overlap->like);
				overlap->zone->Print(output);
				fprintf(output,"\n");
				*/
				mdelete((void **)&overlap);
			}
			else {
				aux = aux->next;
			}
		}

	      /*	//Escreve na saida
		fprintf(output,"%d %lf ", i, most->next->like);
		most->next->zone->Print(output);
		fprintf(output,"\n");    */

		//Desaloca most
		overlap = most->next;
		most->next = most->next->next;
		mdelete((void **)&overlap);
		i++;
	}
}


/// Realiza permutacoes e guarda valor do maximo
 double CIdSTClusterTest::PValue(int nPerm) {

	 int n;
	 double* maxLike = new double[nPerm];
	 CSTZone* z;
	 CSTZone* zone = NULL;

	 double max;
	 double zLike;


	 if(TeProgress::instance())
		{
			string caption = "ATK";
			TeProgress::instance()->setCaption(caption.c_str());
			string msg = "Running ATK's Method. Please, wait!";
			TeProgress::instance()->setMessage(msg);
			TeProgress::instance()->setTotalSteps(nPerm);

		}

		for(n = 0; n < nPerm; n++) {

		max = -DBL_MAX;
		mdelete((void **)&zone);
		mZoneGen->NewPermutation();

		if(TeProgress::instance())
		   {
           if (TeProgress::instance()->wasCancelled())
                   break;
           TeProgress::instance()->setProgress(n);
		   }


		while((z = mZoneGen->GetNextPerm()) != NULL) {
			zLike = mTStat->ZoneLikelihood(z);
			if (zLike > max) {
				max = zLike;
				if (zone) {
					zone->Copy(z);
				}
				else {
					zone = z->NewCopy();
				}
			}
		}
		maxLike[n] = max;
	 }
    if (TeProgress::instance())
       TeProgress::instance()->reset();

	 int nGreater = 0;
	 for(n = 0; n < nPerm; n++) {
		 if (maxLike[n] > mMaxLikeData) {
			 nGreater++;
		 }
	 }

	 mdelete((void **)&maxLike);

	return (double)nGreater/(double)nPerm;


 }


///Avalia verossimilhanca dos dados para todas as zonas.O resultado desse algoritmo eh o calculo dos valores em arquivo outAll se nao for nulo e os n mais significativos em output. Utiliza um vetor para manter as mais significativas
void CIdUniqueTest::DataLikelihood(){

	mMaxLikeData = -DBL_MAX;
	CSTZoneCount* maxZone = NULL;

	mZoneGen->Initialize();

	CSTZone* nextZone;
	CSTZoneCount* z;
	double likeZ;
	while((nextZone = mZoneGen->GetNext()) != NULL) {

		likeZ = mTStat->ZoneLikelihood(nextZone);
		z = (CSTZoneCount *)nextZone;
	       /*	if (outAll != NULL) {
			fprintf(outAll,"%lf %d %d %d ", likeZ, z->SpaceTimeEvents(), z->SpaceEvents(), z->TimeEvents());
			z->Print(outAll);
			fprintf(outAll,"\n");
		} */

		if (likeZ > mMaxLikeData) {
			if (maxZone == NULL)
				maxZone = (CSTZoneCount *)z->NewCopy();
			else
				maxZone->Copy(z);
			mMaxLikeData = likeZ;
		}
	}

     /*	//Escreve na saida
	fprintf(output,"%lf ", mMaxLikeData);
	maxZone->Print(output);
	fprintf(output,"\n");     */
}


///Verifica se z nao intercepta as nZ primeiras zonas de mMoreLikely
bool CIdSTClusterRecalcTest::NotIntercept(int nZ, CSTZone* z) {

	bool nIntercept = true;
	int n = 0;
	while ((nIntercept) && (n < nZ)) {
		nIntercept = !z->Intercept(mMoreLikely[n].zone);
		n++;
	}
	return nIntercept;
}



/// Gera as zonas mais significativas recalculando sempre. Guarda lista das mais significativas e testa inclusao antes de avaliar verossimilhanca.
void CIdSTClusterRecalcTest::DataLikelihood(int nZones,  vector<int> &n_eventos,  vector<int> &n_eventos_espaco,  vector<int> &n_eventos_tempo,  vector<double> &veros,  vector<double> &centroX,  vector<double> &centroY,  vector<double> &raio,
            vector<double> &Tinicial,  vector<double> &Tfinal){

					
	//Aloca espaco para guardar as zonas
	this->mMoreLikely = new sDZone[nZones];
	int nGen;

	//Gera nZones zonas mais significativas nao sobrepostas ou ate nao existir mais
	//nao sobrepostas
	double        maxLike; //maior likelihood atual
	CSTZoneCount* maxZone = NULL; //zona de maior likelihood


	for(nGen = 0; nGen < nZones; nGen++) {

		maxLike = -DBL_MAX;
		mZoneGen->Initialize();
		CSTZone* nextZone;
		CSTZoneCount* z;
		double likeZ;
		while((nextZone = mZoneGen->GetNext()) != NULL) {


			//Verifica sobreposicao a zonas da lista
			if (NotIntercept(nGen, nextZone)) {
				likeZ = mTStat->ZoneLikelihood(nextZone);

				z = (CSTZoneCount *)nextZone;

				//Tenta manter clusters sempre maiores se for de verossimilhanca
				//igual
				if (likeZ >= maxLike) {
					if (maxZone == NULL)
						maxZone = (CSTZoneCount *)z->NewCopy();
					else {
						if (likeZ == maxLike) {
							if (z->Population() > maxZone->Population())
								maxZone->Copy(z);
						}
						else
							maxZone->Copy(z);
					}
					maxLike = likeZ;
				}
			}
		}

		if (maxLike == -DBL_MAX)
			break;
		mMoreLikely[nGen].like = maxLike;
		mMoreLikely[nGen].zone = (CSTZoneCount *)maxZone->NewCopy();

		if (nGen == 0)
			this->mMaxLikeData = maxLike;

	}

 
	for (int n = 0; n < nGen; n++) {
               
        veros.push_back(this->mMoreLikely[n].like);
		this->mMoreLikely[n].zone->Print(n_eventos, n_eventos_espaco, n_eventos_tempo, centroX, centroY, raio, Tinicial, Tfinal);
	}



	mdelete((void **)&maxZone);
	mvdelete((void **)&(this->mMoreLikely));

}