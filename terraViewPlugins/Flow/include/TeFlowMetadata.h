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

/*! \file TeFlowMetadata.h
    This file contains algorithms for flow data generation.
	\author Eric Silva Abreu and Gilberto Ribeiro de Queiroz
*/


#ifndef  __TERRALIB_INTERNAL_FLOWMETADATA_H
#define  __TERRALIB_INTERNAL_FLOWMETADATA_H

#include <string>

using namespace std;

class TeDatabase;

//! Esta classe representa os metadados de uma camada de informacao de fluxos
/*!
    Instancias desta classe representam entradas na tabela te_flow_data_layer.
	Esta classe possui metodos para armazenamento e recuperacao
	desses metadados, e criacao e remocao da tabela de metadados de fluxo.

 \sa
	TeFlowDataGenerator, TeFlowDataClassifier
 */
class TeFlowMetadata
{
	public:

		int flowLayerId_;					//!< Identificação do Layer gerado pelo fluxo
		int flowTableId_;					//!< Identificação da tabela gerado pelo fluxo
		string flowTableOriginColumnName_;	//!< Nome da coluna de Origem da tabela de fluxo
		string flowTableDestinyColumnName_;	//!< Nome da coluna de Destino da tabela de fluxo
		int referenceLayerId_;				//!< Identificação do Layer de referencia
		int referenceTableId_;				//!< Identificação da tabela de referencia
		string referenceColumnName_;		//!< Nome da coluna da tabela de referencia

	protected:
		
		TeDatabase* db_;					//!< A pointer to a database

		string errorMessage_;				//!< Stores the error message during layer generation

	public:

		//! Default constructor
		TeFlowMetadata(TeDatabase* db);

		//! Cria a tabela de armazenamento de metadados das camadas com fluxos
		/*!
			\return   Retorna 1 caso a tabela seja criada, 0 se ocorrer algum erro e -1 se a tabela ja existir
		*/
		int createFlowMetadataTable();

		//! Remove a tabela de armazenamento de metadados das camadas de fluxo
		/*!
			\return   Retorna 1 caso a tabela seja removida, 0 se ocorrer algum erro e -1 se a tabela nao existir
		*/
		int dropFlowMetadataTable();

		//! Insere os metadados de uma camada de fluxo na tabela te_flow_data_layer
		bool insertMetadata(void);

		//! Carrega os metadados
		bool loadMetadata(const int& flowLayerId);

		//! Remove as informacoes de metadado para uma determinada camada de informacao
		bool deleteMetadata(const int& flowLayerId);

		//! Retorna uma mensagem com o ultimo erro ocorrido
		string errorMessage(void) const { return errorMessage_; }

};

#endif	// __TERRALIB_INTERNAL_FLOWMETADATA_H