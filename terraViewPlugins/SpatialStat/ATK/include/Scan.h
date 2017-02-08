#ifndef __SCANH__
#define __SCANH__

#include "Geometry.h"
#include "Event.h"

///Classe para gerar centros dos pontos onde comecarao as zonas
 
class CScanGrid 
{
public:

	//Numero total de pontos do grid
	virtual unsigned int Total() = 0;
	//Idx-esimo ponto do grid
	virtual CTPoint Get(unsigned int idx) = 0;
};

///Classe para zonas espaco-temporais. Deve no minimo definir intersecao entre zonas.
class CSTZone {
public:

	//Atualiza valores internos da zona ao inserir novo evento
	virtual void InsertEvent(CSTPEvent* evt) = 0;
	//Intersecao vazia ou nao
	virtual bool Intercept(CSTZone* z) = 0;
	//Esta ou nao contido
	virtual bool Contain(CSTZone* z) = 0;
	//Verifica se a zona eh valida de acordo com algum criterio interno
	virtual bool IsValid() = 0;
	//Inicializa zona
	virtual void Initialize() = 0;
	//Descricao espaco-temporal
	virtual void SetSTDescription(const CCylinder& c) = 0;
	//Impressao para arquivo da descricao da zona
	virtual void Print( vector<int> &n_eventos,  vector<int>  &n_eventos_espaco,  vector<int> &n_eventos_tempo, vector<double> &centroX,  vector<double> &centroY,  vector<double> &raio, vector<double> &Tinicial, vector<double> &Tfinal) = 0;
	//Retorna populacao da zona
	virtual unsigned int Population() = 0;
	//Retorna copia alocando espaco
	virtual CSTZone* NewCopy() = 0;
	virtual void Copy(CSTZone* dest) = 0;
};

///Classe para gerar as zonas espaco-temporais. Recebe como parametros o grid e o conjunto de eventos e gera atraves de iterador as zonas espaco-temporais correspondentes definas pelo cilindro.

class CScanZoneGenerator
{
	
protected:

	//Metodo para atualizar as possiveis covariaveis de uma zona depois que ela é gerada
	virtual void UpdateZoneCovariables(CSTZone* zone) = 0;

	//Metodo para definir se um ponto do grid pode ser utilizado. Deve usar critérios como prospectivo, pontos onde alarme soou, etc.
	virtual bool ValidGridPoint(CTPoint p) = 0;

public:

	//Inicializa geracao
	virtual void     Initialize() = 0;

	//Proxima zona
	virtual CSTZone* GetNext() = 0;

	//Nova permutacao para teste de Monte Carlo
	virtual void     NewPermutation() = 0;

	//Proxima zona da permutacao
	virtual CSTZone* GetNextPerm() = 0;
};

#endif