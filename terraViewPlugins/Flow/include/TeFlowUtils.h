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

/*! \file TeFlowUtils.h
    This file contains algorithms for flow data classifier.
	\author Eric Silva Abreu and Gilberto Ribeiro de Queiroz
*/

#ifndef  __TERRALIB_INTERNAL_FLOWUTILS_H
#define  __TERRALIB_INTERNAL_FLOWUTILS_H

#include<string>

class TeDatabase;
class TeLayer;

//! Classe utilitaria que fornece operacoes para alimentar as classes TeFlowMetadata, TeFlowDataGenerator e TeFlowDataClassifier
/*!
    Instancias desta classe podem realizar consultas e operacoes tipicamente necessarias
	para as classes de geracao e classificacao de fluxo, como por exemplo, listagem de tabelas,
	criacao e remocao de tabelas de metadados especiais.

 \sa
	TeFlowMetadata, TeFlowDataGenerator, TeFlowDataClassifier
 */
class TeFlowUtils
{
	protected:
		
		string errorMessage_;		//!< Armazena as mensagens de erro durante o processo de classificacao dos fluxos
		TeDatabase* db_;			//!< Ponteiro para o banco de dados em utilizacao
		
	public:

		//! Construtor default
		/*!
			\param db Ponteiro para um banco de dados			
		*/
		TeFlowUtils(TeDatabase* db)
			: db_(db)
		{
		}

		//! Retorna uma mensagem com o ultimo erro ocorrido
		string errorMessage(void) const { return errorMessage_; }

		//! Lista as possiveis tabelas externas que podem ser utilizdas para a criacao de uma camada de informacao com fluxos
		/*!
			\param tables	Lista de saida com os nomes de tabelas externas que nao estao associadas a nenhuma outra tabela do banco
			\return			Retorna verdadeiro se for possivel localizar tabelas e falso em caso de erro
		*/
		bool listCandidateFlowDataTables(vector<string>& tables);

		
			
		//! Retorna uma lista com todos os temas de uma camada de informacao: util pra listagem dos temas de referencia associado a um layer de referencia
		/*!
			\param layerId  Identificador da camada que serviu de referencia para alguma outra camada com fluxos
			\param themes	Vetor de string que ira conter os nomes dos temas do layer
			\return			Retorna verdadeiro se foi possivel listar os temas, e falso caso contrario
		*/
		bool listReferenceThemes(const int& layerId, vector<string>& themes);

		//! Carrega o tema que possui as informacoes de dominancia selecionado pelo usuario
		/*!
			\param dominanceThemeName	Nome do tema de referencia selecionado que possui as informacoes de dominancia dos fluxos (informado pelo usuario)
			\return						Retorna um ponteiro para o tema carregado ou NULL em caso de erro
		*/
		TeTheme* getReferenceTheme(const string& dominanceThemeName);
		
		//! Armazena o nome do novo layer a ser criado
		/*!
			\param newLayer	Novo layer a ser criado.
			\return			Retorna o layer.
		*/
		string getNewLayerName(TeLayer* newLayer);


		//!  Update column table getting just part of a string from this column
		/*!
			\param flowTableName	table name to be updated
			\param columnTableName	column name to get the string
			\param length			string size to be cut
			\return					True if the table has been update correctly or false in other case
		*/
		bool getLeftString(const string flowTableName, const string columnTableName, const int& length);


};
#endif	// __TERRALIB_INTERNAL_FLOWUTILS_H
