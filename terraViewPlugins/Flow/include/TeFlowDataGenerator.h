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

/*! \file TeFlowDataGenerator.h
    This file contains algorithms for flow data generation.
	\author Eric Silva Abreu and Gilberto Ribeiro de Queiroz
*/

#ifndef  __TERRALIB_INTERNAL_FLOWDATAGENERATOR_H
#define  __TERRALIB_INTERNAL_FLOWDATAGENERATOR_H

#include <string>
#include <map>
#include <TeCoord2D.h>
#include <TeGeometry.h>

using namespace std;

class TeTheme;
class TeTable;
class TeLayer;
class TeDatabase;
class TeFlowMetadata;


//! Classe de suporte a criacao de camadas de informacao de fluxos
/*!
	Esta classe fornece o suporte necessario para a criacao de
	camadas de informacao que representam fluxos entre objetos 
	de um tema de referencia.
	A camada de informacao gerada para representar os fluxos 
	tera a representacao de linhas e poligonos (este ultimo
	sera utilizado no caso de fluxos internos, isto e, fluxos
	onde origem e destino sao o mesmo objeto).
	Para a geracao da camada de representacao dos fluxos e 
	necessario:
	1. informar uma tabela externa (previamente 
	   importada para um banco TerraLib) que nao esteja ligada 
	   a nenhuma outra tabela do banco;
	2. especificar quais as colunas desta tabela que contém 
	   os identificadores de origem e destino dos fluxos;
    3. especificar um tema de referencia e a coluna deste tema 
	   que possui os valores que ligam com as colunas do item 2;
   A nova camada de informacao criada sera registrada em uma tabela 
   especifica, chamada te_flow_data_layer, que aramazenara os metadados 
   necessarios para operacoes ........
   Alem disso, na tabela de entrada, sera acrescentada uma coluna 
   com a distancia geodesica entre as extremidades dos fluxos.
   Obs.: A tabela externa informada, fara parte do novo layer criado,
         sendo a sua entrada na tabela TE_LAYER alterada (ela sera estatica no
		 novo layer criado).

  \sa
  TeFlowMetadata, TeDominantFlowNetwork
*/
class TeFlowDataGenerator
{
	protected:

		typedef map<string, TeCoord2D> TeCentroidMap;	//!< definindo estrutura MAP
		
		TeTheme* referenceTheme_;		//!< Ponteiro para o tema que sera utilizado com referencia, durante a criacao da camada de informacao com os fluxos

		string errorMessage_;			//!< Stores the error message during layer generation		

	public:

		//! Default constructor
		TeFlowDataGenerator(TeTheme* theme);

		//! Retorna uma mensagem com o ultimo erro ocorrido
		string errorMessage(void) const { return errorMessage_; }		

		//! Gera uma camada de informacao com o fluxo, a partir de um tema de referencia e uma tabela externa que nao esteja conectada a nenhuma outra tabela
		/*!
			\param flowTableName					Nome da tabela externa que contem os campos de origem e destino dos fluxos, e que fara parte do novo layer com os fluxos
			\param strOrigin						Nome da coluna da tabela externa (flowTableName) que contem valores que permitem ligar com os objetos do tema de referencia, que formarao a origem dos fluxos
			\param strDestiny						Nome da coluna da tabela externa (flowTableName) que contem valores que permitem ligar com os objetos do tema de referencia, que formarao o destino dos fluxos
			\param referenceThemeLinkColumnName		Nome da coluna (no formato NOME_TABELA.NOME_COLUNA)do tema de referencia que liga com as colunas de origem e destino da tabela externa
			\param newLayerName						Nome do layer que sera criado (a tabela de fluxo fara parte deste novo layer)
			\return									Retorna um ponteiro para o novo layer criado ou vazio caso ocorra algum erro
		*/
		TeLayer* buildFlowData(const string& flowTableName,
							   const string& strOrigin,
							   const string& strDestiny,
							   const string& referenceThemeLinkColumnName,
							   const string& newLayerName);

		//! Recupera as informacoes da tabela de fluxo que esta sendo utilizada para geracao dos fluxos, alem disso verifica se a tabela nao esta sendo utilizada em alguma associacao
		/*!
		    \param flowTable	Tabela contendo apenas o nome da tabela externa com as informacoes de fluxos onde sera armazenado os metadados
			\return				Retorna verdadeiro se for possivel recuperar as informacoes da tabela e tambem se ela nao estiver associada, retorna falso caso contrario
		*/
		bool getFlowTableInfo(TeTable& flowTable);

	protected:

		//! Carrega um map com os centroides dos objetos que poderao participar dos fluxos
		/*!
			\param referenceThemeLinkColumnName	Coluna do Theme de referencia que sera utilizada de link com a tabela externa
			\param centerMap					Sera gerado um mapa de centroides que sera composto de uma chave que fara a conexao da tabela externa
												com o Theme e um outro dado que sera a representacao do objeto
			\return								Retorna verdadeiro caso os centroides sejam carregados corretamentes, e falso caso contrario
		*/
		bool loadCentroidMap(const string& referenceThemeLinkColumnName, TeFlowDataGenerator::TeCentroidMap& centerMap);

		//! Adiciona uma coluna para armazenar a distancia entre a origem e o destino dos fluxos, na tabela externa de entrada
		/*!
		    \param flowTableName		Nome da tabela externa com os fluxos
			\param distanceColumnName	Parametro de retorno, contendo o nome da coluna adicionada a tabela (flowTableName)
			\return						Retorna verdadeiro se for possivel adiconar uma coluna e falso caso contrario
		*/
		bool addDistanceColumn(const string& flowTableName, string& distanceColumnName);

		//! Cria as linhas de fluxo do novo layer e automaticamente atualiza as informacoes de distancia
		/*!
		    \param newLayer				Criado um novo layer  com as informações do fluxo gerado
			\param flowTable			Tabela contendo apenas o nome da tabela externa com as informacoes de fluxos onde sera armazenado os metadados
			\param distanceColumnName	Nome da coluna a ser criada onde armazenara as distancias geodesicas entre os fluxos
			\param flowMetadata			Estrutura que contem informações a respeito do fluxo, por exemplo, origem, destino, id fluxo, id tabela, coluna de referencia
			\param centerMap			Mapa de centroides utilizado para conexão dos fluxos entre os objetos de origem e destino
			\return						Retorna verdadeiro se foi possivel carregar as informacoes da tabela de fluxo e falso caso contrario
		*/
		bool createFlow(TeLayer* newLayer, TeTable& flowTable, const string& distanceColumnName, const TeFlowMetadata& flowMetadata, TeFlowDataGenerator::TeCentroidMap& centerMap);

		//! Registra a tabela de fluxo no novo layer criado, caso ocorra um erro, a tabela de fluxo ira ficar com os metadados originais
		/*!
			\param newLayer		Layer criado anteriormente que possui as informações dos fluxos gerados
			\param flowTable	Tabela contendo apenas o nome da tabela externa com as informacoes de fluxos onde sera armazenado os metadados
			\return				Retorna verdadeiro caso seja feito corretamente o registro dos fluxos, e falso caso contrario		
		*/
		bool registerFlowTable(TeLayer* newLayer, TeTable& flowTable);

		//! Salva o metadado da tabela de fluxo do novo layer criado
		/*!
			\param flowMetadata				Estrutura que contem informações a respeito do fluxo, por exemplo, origem, destino, id fluxo, id tabela, coluna de referencia								
			\param referenceAttTableName	Nome da tabela do tema de referencia que possui a coluna de ligacao com a tabela externa	
			\return							Retorna verdadeiro caso o o layer com os fluxo tenha sido salvo completamente, e falso caso contrario
		*/
		bool saveFlowLayerMetadata(TeFlowMetadata& flowMetadata, const string& referenceAttTableName);

		//! Insere as informacoes de distancia referente aos fluxo em uma determinada coluna (columnName) na tabela (flowTable)
		/*!
			\param columnName		Nome da coluna de distancia que ira ser atualizada no layer
			\param dist				Distancia geodesica calculada entre a origem e destino de dois objetos da tabela de fluxo
			\param flowTable		Nome da tabela na qual ira ser adicionada a coluna de distancia e atualizada as informacoes de distancia de cada fluxo
			\param objId			Identificador de cada entrada na tabela referente ao calculo de cada distancia calculada
			\return					Retorna verdadeiro caso a atualizacao seja realizada corretamente, e falso caso contrario
		*/
		bool updateColumn(const string& columnName, const double& dist, TeTable& flowTable, const string& objId);

		//! Remove as informacoes de metadado da tabela de atributos em te_layer_table e seta as informacoes necessarias para voltar a tabela como externa
		/*!
			\param flowTable	Tabela contendo apenas o nome da tabela externa com as informacoes de fluxos onde sera armazenado os metadados 		
			\return 
		*/
		bool rollbackFlowTableInfo(TeTable& flowTable);

		void drawArrow(TeLine2D& line, TeCoord2D coordInit, TeCoord2D coordEnd);
};

#endif	// __TERRALIB_INTERNAL_FLOWDATAGENERATOR_H

