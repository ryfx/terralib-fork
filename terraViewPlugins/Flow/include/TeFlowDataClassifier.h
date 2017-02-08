/************************************************************************************
TerraLib - a library for developing GIS applications.
Copyright © 2001-2004 INPE and Tecgraf/PUC-Rio.

This code is part of the TerraLib library.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

You should have received a copy of the GNU Lesser General Public
License along with this library.

The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The library provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use
of this library and its documentation.
*************************************************************************************/

/*! \file TeFlowDataClassifier.h
    This file contains algorithms for flow data classifier.
	\author Eric Silva Abreu and Gilberto Ribeiro de Queiroz
*/

#ifndef  __TERRALIB_INTERNAL_FLOWDATACLASSIFIER_H
#define  __TERRALIB_INTERNAL_FLOWDATACLASSIFIER_H

#include <string>
#include <map>
#include <vector>
#include <set>

class TeFlowMetadata;
class TeTheme;

using namespace std;

class TeDatabasePortal;


//! Classe de suporte a classificacao dos fluxos
/*!
	Definição da classe TeFlowDataClassifier.
*/
/*!
	Definicao da estrutura TeFlowNode.
		Esta estrutura fornece suporte necessario para a identificacao
		e caracterizacao de cada no que a rede ira possuir. Essas
		informacoes serao de grande importancia para que se possa
		percorrer a rede e para que se possa realizar a classificacao
		de cada no pertencente a esta rede bem como a aresta.
		Como resultado sera criado e adicionado ao novo tema( criado
		pelo TeFlowDataGenerator) duas colunas nas quais terao as
		classificacoes de cada fluxo, em uma sera mostrado o codigo
		do tipo de fluxo, na outra sera mostrado o nome do tipo de fluxo.
  \sa
	TeFlowMetadata, TeFlowDataGenerator
*/
class TeDominantFlowNetwork
{
	protected:

		//! struct TeFlowNode
		/*!
			Definicao de uma estrutura interna chamada TeFlowNode.
				Esta estrutura fornece suporte necessario para a identificacao
				e caracterizacao de cada no que a rede de fluxos domionates ira possuir.
				Essas informacoes serao de grande importancia para que se possa
				percorrer a rede e para que se possa realizar a classificacao
				de cada no pertencente a esta rede bem como a aresta.		
		\sa
			TeFlowMetadata, TeFlowDataGenerator
		*/
		struct TeFlowNode
		{
			string originId_;				//!< Contem o identificador do objeto que eh considerado a origem de todos os fluxos que partem desse noh
			double dominanceValue_;			//!< Valor de dominancia associado a este noh
			string flowId_;					//!< Id do fluxo dominante que parte desse no
			string destinyId_;				//!< Id do objeto de destino do fluxo dominante deste no
			double weightValue_;			//!< Armazena o peso do fluxo dominante (originId_ para destinyId_)
			vector<string> dominatedNodes_;	//!< Guarda um link, atraves de um lista com todos os nos que estao pendurados neste no atraves de seus fluxos dominantes
			vector<string> flows_;			//!< Lista com todos os Ids de fluxos que partem desse noh
			vector<string> destinyNodes_;	//!< Lista de todos os nohs de destino de fluxos que partem deste noh, ou seja, lista do destinos dos fluxos secundarios MAIS o fluxo dominante
			vector<double> weightValues_;	//!< Valores de peso associado a cada fluxo em destinyNodes_
			int flowInTot_;					//!< Contador que indica quantos outros nohs possuem fluxo para este noh
			double flowWeightSumIn_;		//!< Somatorio dos pesos dos fluxos de entrada

			int level_;			//!< Flag utilizado para determinar o nivel do no na arvore da rede
			int branchId_;		//!< Flag que indica qual o ramo de origem do no na arvore da rede => Id do root que chega neste no

			//! Default constructor
			TeFlowNode()
				: dominanceValue_(0.0), weightValue_(0.0), flowInTot_(0), flowWeightSumIn_(0.0), level_(-1), branchId_(-1)
			{
			}
		};

		//! Define o tipo da estrutura de dados da arvore que ira representar a rede hierarquica de fluxos dominates
		typedef map<string, TeFlowNode> TeDominantFlowTree;	

		//! Define o tipo da raiz da estrutura de dados da arvore que ira representar a rede hierarquica de fluxos dominates
		typedef set<string> TeDominantFlowRoots;

		//! Define o tipo da estrutura de dados da arvore que ira representar os nohs isolados (tem apenas o fluxo interno)
		typedef set<string> TeDominantFlowIsolated;

		vector<string> flowType_;

	protected:

		string errorMessage_;			//!< Armazena as mensagens de erro durante o processo de classificacao dos fluxos

		TeTheme* referenceTheme_;		//!< Ponteiro para o tema que sera utilizado com referencia, durante a classificacao dos fluxos: este tema aponta para uma camada de informacao cujos objetos representam o no de nossa rede hierarquica
		string referenceTableName_;		//!< Nome da tabela de atributos utilizada originalmente para criacaodos fluxos
		string dominanceColumnTable_;	//!< Nome da tabela de atributos que contem a coluna com o peso
		string dominanceColumn_;		//!< Armazena o nome da coluna que contem as informacoes de dominancia dos nohs, ou seja, o nome da coluna da tabela de atributos do tema de referencia que possui a informacao de dominancia: contera apenas o nome da coluna
		
		
		TeTheme* flowTheme_;			//!< Ponteiro para o tema que possui o fluxo (criado pela classe TeFlowDataGenerator)
		string flowTableName_;			//!< Armazena o nome da tabela de fluxo
		string weightColumnTableName_;	//!< Nome da tabela que contem o peso usado pelas arestas
		string weightColumnName_;		//!< Armazena o nome da coluna que possui o peso dos fluxos (alguma coluna de uma tabela de atributo do tema flowTheme_): contera apenas o nome da coluna

		TeDominantFlowTree flow_;			//!< Representa a estrutura da rede hierarquica dos fluxos
		TeDominantFlowRoots roots_;			//!< Armazena o identificador dos nohs que compoem as raizes da rede: precisamos destes valores para percorrer a rede (ou seja, ela pode ser desconectada)
		TeDominantFlowIsolated isolateds_;	//!< Armazena o identificador dos nohs que estao isolados da rede: esses nohs nao devem ser classificados como roots.

		TeFlowMetadata* flowMetadata_;	//!< Armazena os metadados da tabela de fluxo

	public:

		//! Default constructor
		/*!
			\param flowTheme	Ponteiro para o tema que contem as linhas de fluxo
			\param refTheme		Ponteiro para o tema que contem as referencias das extremidades das linhas de fluxos
		*/
		TeDominantFlowNetwork(TeTheme* flowTheme, TeTheme* refTheme);

		//! Destructor
		~TeDominantFlowNetwork();

		//! Retorna uma mensagem com o ultimo erro ocorrido
		string errorMessage(void) const { return errorMessage_; }

		//! Retorna o nome da tabela de fluxo
		string getFlowTableName() { return flowTableName_; }

		//! Monta em memoria uma rede hierarquica baseada nos fluxos dominantes
		/*!
			\param	 weightColumnName			Nome da coluna de uma tabela de atributos do tema com fluxo que contem as informacoes de peso dos fluxos (arestas da rede): DEVE ESTAR NO FORMATO NOME_TABELA.NOME_COLUNA
			\param   dominanceColumnName		Nome da coluna de uma de atributos do tema de referencia ou do tema de fluxo que contem as informacoes de impedancia dos nos da rede: DEVE ESTAR NO FORMATO NOME_TABLEA.NOME_COLUNA
			\param   dominaceRefersToOriginCol	Diz se a coluna de impedancia refere-se a coluna de origem ou destino, caso ela esteja associada a alguma tabela do layer de fluxo
			\return								Retorna verdadeiro caso a rede tenha sido construida corretamente, e falso caso contrario
		*/
		bool buildNetwork(const string& weightColumnName, const string& dominanceColumnName, const bool& dominaceRefersToOriginCol = false, const bool& calculateDominance = false, const bool& tot1 = true, const double& dominanceRelation = 0.,
			const bool& checkLocalDominance = false, const double& localDominanceValue = 0.);

		//! 
		/*!
			\param baseColumnName		Nome base das colunas a serem criadas nos temas (mainFlowTheme e networkLevelTheme) que irao possuir as informacoes dos fluxos (_main_flow, _net_level, _in, _out, _sum_in, _sum_out)
			\param sumColumns			Se verdadeiro indica que as colunas de totalizacao: _in, _out, _sum_in, _sum_out serao acrescidas aa camada de referencia e caso seja falso, indica que nao sera necessaria as colunas
			\param treeColumns			Se verdadeiro indica que as colunas de discretização da arvore serao geradas em colunas, os numeros de colunas variam com o numero de niveis da rede
			\return						Retorna verdadeiro caso a rede tenha sido salva corretamente, e falso caso contrario
		*/
		bool saveNetwork(const string& columnName, const bool& sumColumns = true, const bool& treeColumns = true);

		//! Classifica a rede
		bool classifyNetwork(const unsigned int& cutLevel, const string& outputColumnName);
		
		//! Verifica se houve algum noh da rede que nao esta conecatado: ou seja, nao tem um destino e consequentemente todos os nos pendurados neste nao serao classificados uma vez que ele nao fara parte da raiz da rede hierarquica
		bool checkConnections(vector<string>& disconnecteds);

		//! Marca na rede
		bool setAsRootLevel(vector<string>& disconnecteds);

	protected:

		//! Preenche os atributos de nomes de tabelas e colunas necessarias ao processo de geracao da rede
		/*!
			\param	 weightColumnName		Nome da coluna de uma tabela de atributos do tema com fluxo que contem as informacoes de peso dos fluxos (arestas da rede): DEVE ESTAR NO FORMATO NOME_TABELA.NOME_COLUNA
			\param   dominanceColumnName	Nome da coluna de uma de atributos do tema de referencia que contem as informacoes de impedancia dos nos da rede: DEVE ESTAR NO FORMATO NOME_TABLEA.NOME_COLUNA
			\return							Retorna verdadeiro se for possivel preencher os nomes das tabelas e colunas, e falso caso contrario
		*/
		bool fillColumnAndTableNames(const string& weightColumnName, const string& dominanceColumnName, const bool& calculateDominance);

		//! 
		/*!
			\param joinMap  Array associativo, onde a chave equivale ao atributo de origem ou destino de um fluxo (mesmo valor contino na coluna flowTableOriginColumnName_ da estrutura TeFlowMetadata) e o valor equivale ao objhect_id do layer de referencia
			\return			Retorna verdadeiro se for possivel carregar a tabela de juncao, e falso caso contrario
		*/
		bool loadJoinMap(map<string, string>& joinMap);

		//! Carrega uma tabela com as informacoes de impedancia/dominancia para um no
		/*!
			\param dominanceTable               Array associativo, onde a chave equivale ao atributo de origem ou destino de um fluxo (mesmo valor contino na coluna flowTableOriginColumnName_ da estrutura TeFlowMetadata)
			\param  dominaceRefersToOriginCol	Diz se a coluna de impedancia refere-se a coluna de origem ou destino, caso ela esteja associada a alguma tabela do layer de fluxo
			\return								Retorna verdadeiro se for possivel carregar a tabela de dominancia/impedancia dos nohs dos fluxos, e falso caso contrario
		*/
		bool loadDominanceTable(map<string, double>& dominanceTable, const bool& dominaceRefersToOriginCol = false);

		//! Retorna um portal com a consulta a tabela de fluxo ordenada pela origem e pelo valor do fluxo
		/*!
			\return  Retorna um portal ou vazio caso aconteca algum erro durante a abertura deste portal: este metodo NAO FAZ o fetchrow, este deve ser realizado pelo cliente deste metodo
		*/
		TeDatabasePortal* getFlowPortal();

		//! Retorna um portal com a consulta a tabela de fluxo somente com o fluxo principal
		/*!
			\return  Retorna um portal ou vazio caso aconteca algum erro durante a abertura deste portal: este metodo NAO FAZ o fetchrow, este deve ser realizado pelo cliente deste metodo
		*/
		TeDatabasePortal* getMainFlowPortal();

		bool setMainFlow(const double& dominanceRelation, const bool& checkLocalDominance = false, const double& localDominanceValue = 0.);

		//!	Faz a ligacao dos nohs na rede e realiza a verificacao do noh de origem para saber se ele eh dominante ou dominado, atualizando a estrutura de roots no caso de dominante
		/*!
			\param origin			Identificacao do noh de origem do fluxo
			\param destiny			Identificacao do noh de destino do fluxo
			\param flowId			Identificacao do fluxo
			\param weight			Identificacao do peso do fluxo
			\return					Retorna verdadeiro caso a ligacao dos nosh na rede se faca de modo correto e falso caso contrario
		*/
		bool linkNetwork(const string& origin, const string& destiny, const string& flowId, const double& weight, const double& dominanceRelation, const bool& checkLocalDominance = false, const double& localDominanceValue = 0.);

		//! Verifica se o noh ja esta na rede ou nao, se estiver ele faz a insercao do noh na rede, caso contrario realiza apenas a atualizacao de alguns atributos
		/*!
			\param origin			Identificacao do noh de origem do fluxo
			\param destiny			Identificacao do noh de destino do fluxo
			\param flowId			Identificacao do fluxo
			\param weight			Identificacao do peso do fluxo
			\param dominanceMap		Estrutura que possui o valor de dominancia para cada objeto, tendo como chave o ID do objeto
			\return					Retorna verdadeiro caso a insercao do noh na rede seja correta e falso caso contrario
		*/
		bool insertIntoNetwork(const string& origin, const string& destiny, const string& flowId, const double& weight, map<string, double>& dominanceMap, const bool& calculateDom);

		bool calculateDominanceValue(const bool& tot1);

		//!Faz a determinacao dos niveis dos nohs da rede, comecando pelos nohs roots com o nivel 0
		/*!
			\return		Retorna verdadeiro caso a determinacao dos niveis dos nohs tenha sido realizada corretamente e falso caso contrario
		*/
		bool buildLevel();

		//! Faz a determinacao dos niveis dos nohs da rede utilizando recursividade
		/*!
			\param level		armazena o nivel do noh na rede	
			\param flowNode		estrutura para identificacao e caraccterizacao de cada no que a rede de fluxos dominante possui
			\return				Retorna verdadeiro caso a determinacao dos niveis dos nohs tenha sido realizada corretamente e falso caso contrario
		*/
		bool buildLevel(int level, TeFlowNode& flowNode);

		//! Faz a determinação de cada ramo da árvore
		void labelBranchs(const int& branchId, TeFlowNode& node);

		//! Faz a determinação de cada ramo da árvore utilizando recursividade
		/*!
			\param newRoots		armazena os nohs da rede que sao considerados raizes da rede, ou seja, nivel 0
		*/
		void labelBranchs(vector<string>& newRoots);

		//! Faz a classificação da arvore
		/*!
			\param cutLevel		Armazena o nivel de corte da rede, area a ser analizada, determinado pelo usuario
			\param result		Armazena o codigo da classificacao do fluxo
			\param node_origin	Armazena o noh de origem do fluxo
			\param node_destiny	Armazena o noh de destino do fluxo
			\param flowId		Armazena a identificacao do fluxo que esta sendo classificado
		*/
		void classify(int cutLevel, map<string, int>& result, TeFlowNode& node_origin, TeFlowNode& node_destiny, const string& flowId);
		
		//! Faz a classificação da arvore utilizando recursividade
		/*!
			\param cutLevel		Armazena o nivel de corte da rede, area a ser analizada, determinado pelo usuario
			\param node			Estrutura que armazena as informacoes a respeito do no a ser classificado
			\param result		Armazena o codigo da classificacao do fluxo
		*/
		void classify(int cutLevel, TeFlowNode& node, map<string, int>& result);

		//Verifica se os nos de origem e destino estao em um mesmo ramo
		/*!
			\param node_origin		Armazena o no de origem do fluxo
			\param node_destiny		Armazena o no de destino do fluxo
			\return					Retorna verdadeiro caso o noh de destino e noh de origem pertencam ao mesmo ramo, e falso caso contrario
		*/
		bool checkNode(TeFlowNode& node_origin, TeFlowNode& node_destiny);

		void findIsolatedRoots();

		int getMaxLevel();

		int getNetNodesSize(const std::string& objId);

		void getDomiantedNodesSize(const std::string& objId, int& value);

	private:

		//! No copy allowed
		TeDominantFlowNetwork(const TeDominantFlowNetwork&);

		//! No copy allowed
		TeDominantFlowNetwork& operator=(const TeDominantFlowNetwork&);		
};

#endif	// __TERRALIB_INTERNAL_FLOWDATACLASSIFIER_H

