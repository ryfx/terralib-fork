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

Valor m�nimo															Num�rico (inteiro ou real)						matricial, vetorial, celular		TeMinimumStrategy							TeFillCellNonSpatialRasterOperation / TeFillCellNonSpatialOperation

Valor m�ximo															Num�rico (inteiro ou real)						matricial, vetorial, celular		TeMaximumStrategy							TeFillCellNonSpatialRasterOperation / TeFillCellNonSpatialOperation

Valor m�dio																Num�rico (inteiro ou real)						matricial, vetorial, celular		TeAverageStrategy							TeFillCellNonSpatialRasterOperation / TeFillCellNonSpatialOperation

Soma dos valores														Num�rico (inteiro ou real)						matricial, vetorial, celular		TeSumStrategy								TeFillCellNonSpatialRasterOperation / TeFillCellNonSpatialOperation

Desvio padr�o															Num�rico (inteiro ou real)						matricial, vetorial, celular		TeSTDevStrategy								TeFillCellNonSpatialRasterOperation / TeFillCellNonSpatialOperation

Classe majorit�ria (n�mero de elementos)								Categ�rico, nominal (inteiro ou string)			matricial, vetorial, celular		TeMajorityStrategy							TeFillCellNonSpatialRasterOperation / TeFillCellNonSpatialOperation

Porcentagem de cada classe (quantidade)									Categ�rico, nominal (inteiro, string)			matricial, vetorial, celular		TeCategoryPercentageStrategy				TeFillCellCategoryCountPercentageRasterOperation


Tipo 2: Vetorial poligonal (�rea de intersec��o e valor do atributo)

M�dia ponderada pela intersec��o										Num�rico (inteiro ou real)						pol�gonos							TeAverageWeighByAreaStrategy				TeFillCellSpatialOperation

Soma ponderada pela interse��o											Num�rico (inteiro ou real)						pol�gonos							TeSumWeighByAreaStrategy					TeFillCellSpatialOperation

Classe majorit�ria (em �rea)											Categ�rico, nominal (inteiro, string)			pol�gonos							TeCategoryMajorityStrategy					TeFillCellSpatialOperation

Porcentagem da classe majorit�ria (�rea)								Categ�rico, nominal (inteiro, string)			pol�gonos							N/A											TeFillCellCategoryAreaPercentageOperation				


Tipo 3: Vetorial (apenas geometria)

Dist�ncia M�nima														N/A												pontos, linhas, pol�gonos			N/A	(TeMinimumDistance****Strategy)			TeFillCellDistanceOperation

Presen�a																N/A												pontos, linhas, pol�gonos			TePresenceStrategy							TeFillCellNonSpatialOperation

Contagem																N/A												pontos, linhas, pol�gonos			TeCount****ObjectsStrategy					TeFillCellSpatialOperation

Porcentagem total de interse��o											N/A												pol�gonos							TeTotalAreaPercentageStrategy				TeFillCellSpatialOperation


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



//especifica��o retirada da antiga versao do plugin

if(geometria=="RASTER")
{
	 Classe Majorit�ria (n�mero de elementos)	=> CellOp_MajorityStrategy
	 Desvio Padr�o								=> CellOp_STDevStrategy
	 Porcentagem de Cada Classe					=> CellOp_CategoryPercentageStrategy
	 Soma										=> CellOp_SumStrategy
	 Valor M�ximo								=> CellOp_MaximumStrategy
	 Valor M�dio								=> CellOp_AverageStrategy
	 Valor M�nimo								=> CellOp_MinimumStrategy
}

if(geometria=="PONTOS")
{		
	 Classe Majorit�ria (n�mero de elementos)	=> CellOp_MajorityStrategy
	 Contagem									=> CellOp_CountObjectsStrategy
	 Desvio Padr�o								=> CellOp_STDevStrategy
	 Dist�ncia M�nima							=> CellOp_MinimumDistanceStrategy
	 Presen�a									=> CellOp_PresenceStrategy
	 Soma dos Valores							=> CellOp_SumStrategy
	 Valor M�ximo								=> CellOp_MaximumStrategy
	 Valor M�dio								=> CellOp_AverageStrategy
	 Valor M�nimo								=> CellOp_MinimumStrategy	
}
if(geometria=="LINHAS")
{
	 Classe Majorit�ria (n�mero de elementos)	=> CellOp_MajorityStrategy
	 Contagem									=> CellOp_CountObjectsStrategy
	 Desvio Padr�o								=> CellOp_STDevStrategy	
	 Dist�ncia M�nima							=> CellOp_MinimumDistanceStrategy
	 Presen�a									=> CellOp_PresenceStrategy	
	 Soma dos Valores							=> CellOp_SumStrategy
	 Valor M�ximo								=> CellOp_MaximumStrategy
	 Valor M�dio								=> CellOp_AverageStrategy
	 Valor M�nimo								=> CellOp_MinimumStrategy
}
if(geometria=="POL�GONOS")
{
	 Classe Majorit�ria (�rea)					=> CellOp_CategoryMajorityStrategy *
	 Classe Majorit�ria (n�mero de elementos)	=> CellOp_MajorityStrategy
	 Contagem									=> CellOp_CountObjectsStrategy
	 Desvio Padr�o								=> CellOp_STDevStrategy
	 Dist�ncia M�nima							=> CellOp_MinimumDistanceStrategy	
	 Presen�a									=> CellOp_PresenceStrategy
	 Porcentagem de Cada Classe					=> CellOp_CategoryPercentageStrategy
	 Porcentagem Total de Interse��o			=> CellOp_TotalAreaPercentageStrategy
	 Porcentagem da Classe Majorit�ria			=> CellOp_CategoryAreaPercentageStrategy
	 Soma dos Valores							=> CellOp_SumStrategy
	 Soma Ponderada Pela �rea da C�lula			=> CellOp_AverageWeighByAreaStrategy			
	 Soma Ponderada Pela �rea do Pol�gono		=> CellOp_SumWeighByAreaStrategy
	 Valor M�ximo								=> CellOp_MaximumStrategy
	 Valor M�dio								=> CellOp_AverageStrategy
	 Valor M�nimo								=> CellOp_MinimumStrategy
}

if(geometria=="C�LULAS")
{
	 Classe Majorit�ria (n�mero de elementos)	=> CellOp_MajorityStrategy
	 Porcentagem de Cada Classe					=> CellOp_CategoryPercentageStrategy
	 Valor M�ximo								=> CellOp_MaximumStrategy
	 Valor M�dio								=> CellOp_AverageStrategy
	 Valor M�nimo								=> CellOp_MinimumStrategy
}
*/