/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright  2001-2004 INPE and Tecgraf/PUC-Rio.
This file is part of TerraView. TerraView is free software; you can redistribute it 
and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

You should have received a copy of the GNU General Public License
along with TerraView.
The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The software provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use of
this program and its documentation.
************************************************************************************/

#ifndef FILLCELLOP_H
#define FILLCELLOP_H

#include <PluginParameters.h>
#include <TeAttribute.h>

#include <vector>

/**
* @brief A enum that defines each cell operation strategy
* @author Eric Silva Abreu <eric.abreu@funcate.org.br>
*/   
enum CellOpStrategy
{
	CellOp_None,

	CellOp_MinimumStrategy,
	CellOp_MaximumStrategy,
	CellOp_AverageStrategy,
	CellOp_SumStrategy,
	CellOp_STDevStrategy,
	CellOp_MajorityStrategy,
	CellOp_CategoryPercentageStrategy,

	CellOp_AverageWeighByAreaStrategy,
	CellOp_SumWeighByAreaStrategy,
	CellOp_CategoryMajorityStrategy,
	CellOp_CategoryAreaPercentageStrategy,

	CellOp_MinimumDistanceStrategy,
	CellOp_PresenceStrategy,
	CellOp_CountObjectsStrategy,
	CellOp_TotalAreaPercentageStrategy
};

/**
* @brief A struct that defines a cell operation
* @author Eric Silva Abreu <eric.abreu@funcate.org.br>
*/
typedef struct cellOpType
{
	std::string			_opName;			//!< Cell operation name

	CellOpStrategy		_opStrategy;		//!< Cell operation strategy

	int					_opRep;				//!< All geometry types supported to this strategy

} CellOpType;

/**
* @brief A class for fill cell plugin operation.
* @author Eric Silva Abreu <eric.abreu@funcate.org.br>
*/   
class FillCellOp
{

public:

	/**
	* Default constructor.
	*/    
	FillCellOp();

	/**
	* Default destructor.
	*/    
	~FillCellOp();

public:

	/**
	* Function used to get the cell op map
	*
	* @return A map with all cell operations defineds in generateOperations() function.
	*/ 
	std::map<CellOpStrategy, CellOpType> getCellMapOperations();

	/**
	* Function used to get cell operations for raster geometry
	*
	* @return A list of all cell operations names
	*/ 
	std::vector<std::string> getRasterOperations();

	/**
	* Function used to get cell operations for polygon geometry
	*
	* @return A list of all cell operations names
	*/ 
	std::vector<std::string> getPolygonOperations();

	/**
	* Function used to get cell operations for line geometry
	*
	* @return A list of all cell operations names
	*/ 
	std::vector<std::string> getLineOperations();

	/**
	* Function used to get cell operations for point geometry
	*
	* @return A list of all cell operations names
	*/ 
	std::vector<std::string> getPointOperations();

	/**
	* Function used to get cell operations for cell geometry
	*
	* @return A list of all cell operations names
	*/ 
	std::vector<std::string> getCellOperations();

	/**
	* Function used to get cell operations type given a operation name
	*
	* @return A CellOpType if the operation exist. 
	*/ 
	CellOpType getCellOpType(const std::string& opName);

	/**
	* Function used to get a attribute list compatible with a selected operation.
	* Some operations can handle only one type os attribute, such as Category or Numerical.
	*
	* @return A list with all attributes compatibles with selected operation
	*/ 
	std::vector<std::string> getCompatibleAttributes(const std::string& opName, TeAttributeList& attrList);

protected:

	/**
	* Function used to create a map with all cell operations.
	*/    
	void generateOperations();

protected:

	/**
	* Map with all cell operations and its names
	*/ 
	std::map<CellOpStrategy, CellOpType> _cellOperations;
};


#endif //FILLCELLOP_H

/*

Tipo 1: Matricial, Celular e Vetorial (somente valor de atributo)		Tipo do atributo								Repres.								Estrategia									Preenchimento

Valor mínimo															Numérico (inteiro ou real)						matricial, vetorial, celular		TeMinimumStrategy							TeFillCellNonSpatialRasterOperation / TeFillCellNonSpatialOperation

Valor máximo															Numérico (inteiro ou real)						matricial, vetorial, celular		TeMaximumStrategy							TeFillCellNonSpatialRasterOperation / TeFillCellNonSpatialOperation

Valor médio																Numérico (inteiro ou real)						matricial, vetorial, celular		TeAverageStrategy							TeFillCellNonSpatialRasterOperation / TeFillCellNonSpatialOperation

Soma dos valores														Numérico (inteiro ou real)						matricial, vetorial, celular		TeSumStrategy								TeFillCellNonSpatialRasterOperation / TeFillCellNonSpatialOperation

Desvio padrão															Numérico (inteiro ou real)						matricial, vetorial, celular		TeSTDevStrategy								TeFillCellNonSpatialRasterOperation / TeFillCellNonSpatialOperation

Classe majoritária (número de elementos)								Categórico, nominal (inteiro ou string)			matricial, vetorial, celular		TeMajorityStrategy							TeFillCellNonSpatialRasterOperation / TeFillCellNonSpatialOperation

Porcentagem de cada classe (quantidade)									Categórico, nominal (inteiro, string)			matricial, vetorial, celular		TeCategoryPercentageStrategy				TeFillCellCategoryCountPercentageRasterOperation


Tipo 2: Vetorial poligonal (área de intersecção e valor do atributo)

Média ponderada pela intersecção										Numérico (inteiro ou real)						polígonos							TeAverageWeighByAreaStrategy				TeFillCellSpatialOperation

Soma ponderada pela interseção											Numérico (inteiro ou real)						polígonos							TeSumWeighByAreaStrategy					TeFillCellSpatialOperation

Classe majoritária (em área)											Categórico, nominal (inteiro, string)			polígonos							TeCategoryMajorityStrategy					TeFillCellSpatialOperation

Porcentagem da classe majoritária (área)								Categórico, nominal (inteiro, string)			polígonos							N/A											TeFillCellCategoryAreaPercentageOperation				


Tipo 3: Vetorial (apenas geometria)

Distância Mínima														N/A												pontos, linhas, polígonos			N/A	(TeMinimumDistance****Strategy)			TeFillCellDistanceOperation

Presença																N/A												pontos, linhas, polígonos			TePresenceStrategy							TeFillCellNonSpatialOperation

Contagem																N/A												pontos, linhas, polígonos			TeCount****ObjectsStrategy					TeFillCellSpatialOperation

Porcentagem total de interseção											N/A												polígonos							TeTotalAreaPercentageStrategy				TeFillCellSpatialOperation


SOURCE: http://www.dpi.inpe.br/~anapaula/plugin_celulas/help.htm

TeComputeAttrStrategy
	TeAverageStrategy
	TeSTDevStrategy
	TeSumStrategy
	TeMinimumStrategy
	TeMaximumStrategy
	TeCategoryPercentageStrategy
	TePresenceStrategy
	TeMajorityStrategy
	TeMajorityCategoryStrategy

TeComputeSpatialStrategy
	TeAverageWeighByAreaStrategy
	TeSumWeighByAreaStrategy
	TeCategoryMajorityStrategy
	TeCountObjectsStrategy
		TeCountPolygonalObjectsStrategy
		TeCountLineObjectsStrategy
		TeCountPointObjectsStrategy
	TeLineLengthStrategy
	TeTotalAreaPercentageStrategy
	TeMinimumDistanceStrategy
		TeMinimumDistancePolygonsStrategy
		TeMinimumDistanceLinesStrategy
		TeMinimumDistancePointsStrategy



//especificação retirada da antiga versao do plugin

if(geometria=="RASTER")
{
	 Classe Majoritária (número de elementos)	=> CellOp_MajorityStrategy
	 Desvio Padrão								=> CellOp_STDevStrategy
	 Porcentagem de Cada Classe					=> CellOp_CategoryPercentageStrategy
	 Soma										=> CellOp_SumStrategy
	 Valor Máximo								=> CellOp_MaximumStrategy
	 Valor Médio								=> CellOp_AverageStrategy
	 Valor Mínimo								=> CellOp_MinimumStrategy
}

if(geometria=="PONTOS")
{		
	 Classe Majoritária (número de elementos)	=> CellOp_MajorityStrategy
	 Contagem									=> CellOp_CountObjectsStrategy
	 Desvio Padrão								=> CellOp_STDevStrategy
	 Distância Mínima							=> CellOp_MinimumDistanceStrategy
	 Presença									=> CellOp_PresenceStrategy
	 Soma dos Valores							=> CellOp_SumStrategy
	 Valor Máximo								=> CellOp_MaximumStrategy
	 Valor Médio								=> CellOp_AverageStrategy
	 Valor Mínimo								=> CellOp_MinimumStrategy	
}
if(geometria=="LINHAS")
{
	 Classe Majoritária (número de elementos)	=> CellOp_MajorityStrategy
	 Contagem									=> CellOp_CountObjectsStrategy
	 Desvio Padrão								=> CellOp_STDevStrategy	
	 Distância Mínima							=> CellOp_MinimumDistanceStrategy
	 Presença									=> CellOp_PresenceStrategy	
	 Soma dos Valores							=> CellOp_SumStrategy
	 Valor Máximo								=> CellOp_MaximumStrategy
	 Valor Médio								=> CellOp_AverageStrategy
	 Valor Mínimo								=> CellOp_MinimumStrategy
}
if(geometria=="POLÍGONOS")
{
	 Classe Majoritária (área)					=> CellOp_CategoryMajorityStrategy *
	 Classe Majoritária (número de elementos)	=> CellOp_MajorityStrategy
	 Contagem									=> CellOp_CountObjectsStrategy
	 Desvio Padrão								=> CellOp_STDevStrategy
	 Distância Mínima							=> CellOp_MinimumDistanceStrategy	
	 Presença									=> CellOp_PresenceStrategy
	 Porcentagem de Cada Classe					=> CellOp_CategoryPercentageStrategy
	 Porcentagem Total de Interseção			=> CellOp_TotalAreaPercentageStrategy
	 Porcentagem da Classe Majoritária			=> CellOp_CategoryAreaPercentageStrategy
	 Soma dos Valores							=> CellOp_SumStrategy
	 Soma Ponderada Pela Área da Célula			=> CellOp_AverageWeighByAreaStrategy			
	 Soma Ponderada Pela Área do Polígono		=> CellOp_SumWeighByAreaStrategy
	 Valor Máximo								=> CellOp_MaximumStrategy
	 Valor Médio								=> CellOp_AverageStrategy
	 Valor Mínimo								=> CellOp_MinimumStrategy
}

if(geometria=="CÉLULAS")
{
	 Classe Majoritária (número de elementos)	=> CellOp_MajorityStrategy
	 Porcentagem de Cada Classe					=> CellOp_CategoryPercentageStrategy
	 Valor Máximo								=> CellOp_MaximumStrategy
	 Valor Médio								=> CellOp_AverageStrategy
	 Valor Mínimo								=> CellOp_MinimumStrategy
}
*/