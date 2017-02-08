#include <FillCellOp.h>
#include <qobject.h>

FillCellOp::FillCellOp()
{
	generateOperations();
}

FillCellOp::~FillCellOp()
{
	_cellOperations.clear();
}

void FillCellOp::generateOperations()
{
	_cellOperations.clear();

//CellOp_MinimumStrategy
	CellOpType minType;
//	minType._opName		= "Valor Mínimo";
	minType._opName		= QObject::tr("Minimum Value").latin1();
	minType._opRep		= TeRASTER | TePOLYGONS | TeLINES | TePOINTS | TeCELLS;
	minType._opStrategy = CellOp_MinimumStrategy;
	_cellOperations.insert(std::map<CellOpStrategy, CellOpType>::value_type(CellOp_MinimumStrategy, minType));

//CellOp_MaximumStrategy,
	CellOpType maxType;
//	maxType._opName		= "Valor Máximo";
	maxType._opName		= QObject::tr("Maximum Value").latin1();
	maxType._opRep		= TeRASTER | TePOLYGONS | TeLINES | TePOINTS | TeCELLS;
	maxType._opStrategy = CellOp_MaximumStrategy;
	_cellOperations.insert(std::map<CellOpStrategy, CellOpType>::value_type(CellOp_MaximumStrategy, maxType));

//CellOp_AverageStrategy,
	CellOpType avgType;
//	avgType._opName		= "Valor Médio";
	avgType._opName		= QObject::tr("Average Value").latin1();
	avgType._opRep		= TeRASTER | TePOLYGONS | TeLINES | TePOINTS | TeCELLS;
	avgType._opStrategy = CellOp_AverageStrategy;
	_cellOperations.insert(std::map<CellOpStrategy, CellOpType>::value_type(CellOp_AverageStrategy, avgType));

//CellOp_SumStrategy,
	CellOpType sumType;
//	sumType._opName		= "Soma dos Valores";
	sumType._opName		= QObject::tr("Sum of Values").latin1();
	sumType._opRep		= TeRASTER | TePOLYGONS | TeLINES | TePOINTS | TeCELLS;
	sumType._opStrategy = CellOp_SumStrategy;
	_cellOperations.insert(std::map<CellOpStrategy, CellOpType>::value_type(CellOp_SumStrategy, sumType));

//CellOp_STDevStrategy,
	CellOpType stdDevType;
//	stdDevType._opName		= "Desvio Padrão";
	stdDevType._opName		= QObject::tr("Standard Deviation").latin1();
	stdDevType._opRep		= TeRASTER | TePOLYGONS | TeLINES | TePOINTS | TeCELLS;
	stdDevType._opStrategy	= CellOp_STDevStrategy;
	_cellOperations.insert(std::map<CellOpStrategy, CellOpType>::value_type(CellOp_STDevStrategy, stdDevType));

//CellOp_MajorityStrategy,
	CellOpType majorType;
//	majorType._opName		= "Classe Majoritária";
	majorType._opName		= QObject::tr("Major Class").latin1();
	majorType._opRep		= TeRASTER | TePOLYGONS | TeLINES | TePOINTS | TeCELLS;
	majorType._opStrategy	= CellOp_MajorityStrategy;
	_cellOperations.insert(std::map<CellOpStrategy, CellOpType>::value_type(CellOp_MajorityStrategy, majorType));

//CellOp_CategoryPercentageStrategy,
	CellOpType categPercentageType;
//	categPercentageType._opName		= "Porcentagem de cada Classe";
	categPercentageType._opName		= QObject::tr("Percentage per Class").latin1();
	categPercentageType._opRep		= TeRASTER | TePOLYGONS | TeLINES | TePOINTS | TeCELLS;
	categPercentageType._opStrategy = CellOp_CategoryPercentageStrategy;
	_cellOperations.insert(std::map<CellOpStrategy, CellOpType>::value_type(CellOp_CategoryPercentageStrategy, categPercentageType));



//CellOp_AverageWeighByAreaStrategy,
	CellOpType avgWeightType;
//	avgWeightType._opName		= "Média Ponderada por Área";
	avgWeightType._opName		= QObject::tr("Weighted by Area").latin1();
	avgWeightType._opRep		= TePOLYGONS;
	avgWeightType._opStrategy	= CellOp_AverageWeighByAreaStrategy;
	_cellOperations.insert(std::map<CellOpStrategy, CellOpType>::value_type(CellOp_AverageWeighByAreaStrategy, avgWeightType));

//CellOp_SumWeighByAreaStrategy,
	CellOpType sumWeightType;
//	sumWeightType._opName		= "Soma Ponderada por Área";
	sumWeightType._opName		= QObject::tr("Weighted Sum by Area").latin1();
	sumWeightType._opRep		= TePOLYGONS;
	sumWeightType._opStrategy	= CellOp_SumWeighByAreaStrategy;
	_cellOperations.insert(std::map<CellOpStrategy, CellOpType>::value_type(CellOp_SumWeighByAreaStrategy, sumWeightType));

//CellOp_CategoryMajorityStrategy,
	//CellOpType categMajorType;
	//categMajorType._opName		= "Classe Majoritária por Área";
	//categMajorType._opRep		= TePOLYGONS;
	//categMajorType._opStrategy	= CellOp_CategoryMajorityStrategy;
	//_cellOperations.insert(std::map<CellOpStrategy, CellOpType>::value_type(CellOp_CategoryMajorityStrategy, categMajorType));

//CellOp_CategoryAreaPercentageStrategy,
	CellOpType categAreaPercentageType;
//	categAreaPercentageType._opName		= "Porcentagem de cada Classe por Área";
	categAreaPercentageType._opName		= QObject::tr("Percentage of each Class by Area").latin1();
	categAreaPercentageType._opRep		= TePOLYGONS;
	categAreaPercentageType._opStrategy = CellOp_CategoryAreaPercentageStrategy;
	_cellOperations.insert(std::map<CellOpStrategy, CellOpType>::value_type(CellOp_CategoryAreaPercentageStrategy, categAreaPercentageType));



//CellOp_MinimumDistanceStrategy,
	CellOpType minDistType;
//	minDistType._opName		= "Distância Mínima";
	minDistType._opName		= QObject::tr("Minimum Distance").latin1();
	minDistType._opRep		= TePOLYGONS | TeLINES | TePOINTS;
	minDistType._opStrategy = CellOp_MinimumDistanceStrategy;
	_cellOperations.insert(std::map<CellOpStrategy, CellOpType>::value_type(CellOp_MinimumDistanceStrategy, minDistType));

//CellOp_PresenceStrategy,
	CellOpType presenceType;
//	presenceType._opName		= "Presença";
	presenceType._opName		= QObject::tr("Presence").latin1();
	presenceType._opRep			= TePOLYGONS | TeLINES | TePOINTS;
	presenceType._opStrategy	= CellOp_PresenceStrategy;
	_cellOperations.insert(std::map<CellOpStrategy, CellOpType>::value_type(CellOp_PresenceStrategy, presenceType));

//CellOp_CountObjectsStrategy,
	CellOpType countType;
//	countType._opName		= "Contagem";
	countType._opName		= QObject::tr("Count").latin1();
	countType._opRep		= TePOLYGONS | TeLINES | TePOINTS;
	countType._opStrategy	= CellOp_CountObjectsStrategy;
	_cellOperations.insert(std::map<CellOpStrategy, CellOpType>::value_type(CellOp_CountObjectsStrategy, countType));

//CellOp_TotalAreaPercentageStrategy
	CellOpType totalAreaPercengateType;
//	totalAreaPercengateType._opName		= "Porcentagem Total por Área";
	totalAreaPercengateType._opName		= QObject::tr("Percentage of Total Area").latin1();
	totalAreaPercengateType._opRep		= TePOLYGONS;
	totalAreaPercengateType._opStrategy = CellOp_TotalAreaPercentageStrategy;
	_cellOperations.insert(std::map<CellOpStrategy, CellOpType>::value_type(CellOp_TotalAreaPercentageStrategy, totalAreaPercengateType));
}

std::map<CellOpStrategy, CellOpType> FillCellOp::getCellMapOperations()
{
	return _cellOperations;
}

std::vector<std::string> FillCellOp::getRasterOperations()
{
	std::vector<std::string> opVec;

	std::map<CellOpStrategy, CellOpType>::iterator it = _cellOperations.begin();

	while(it != _cellOperations.end())
	{
		if(it->second._opRep & TeRASTER)
		{
			opVec.push_back(it->second._opName);
		}

		++it;
	}

	sort(opVec.begin(), opVec.end());

	return opVec;
}

std::vector<std::string> FillCellOp::getPolygonOperations()
{
	std::vector<std::string> opVec;

	std::map<CellOpStrategy, CellOpType>::iterator it = _cellOperations.begin();

	while(it != _cellOperations.end())
	{
		if(it->second._opRep & TePOLYGONS)
		{
			opVec.push_back(it->second._opName);
		}

		++it;
	}

	sort(opVec.begin(), opVec.end());

	return opVec;
}

std::vector<std::string> FillCellOp::getLineOperations()
{
	std::vector<std::string> opVec;

	std::map<CellOpStrategy, CellOpType>::iterator it = _cellOperations.begin();

	while(it != _cellOperations.end())
	{
		if(it->second._opRep & TeLINES)
		{
			opVec.push_back(it->second._opName);
		}

		++it;
	}

	sort(opVec.begin(), opVec.end());

	return opVec;
}

std::vector<std::string> FillCellOp::getPointOperations()
{
	std::vector<std::string> opVec;

	std::map<CellOpStrategy, CellOpType>::iterator it = _cellOperations.begin();

	while(it != _cellOperations.end())
	{
		if(it->second._opRep & TePOINTS)
		{
			opVec.push_back(it->second._opName);
		}

		++it;
	}

	sort(opVec.begin(), opVec.end());

	return opVec;
}

std::vector<std::string> FillCellOp::getCellOperations()
{
	std::vector<std::string> opVec;

	std::map<CellOpStrategy, CellOpType>::iterator it = _cellOperations.begin();

	while(it != _cellOperations.end())
	{
		if(it->second._opRep & TeCELLS)
		{
			opVec.push_back(it->second._opName);
		}

		++it;
	}

	sort(opVec.begin(), opVec.end());

	return opVec;
}

CellOpType FillCellOp::getCellOpType(const std::string& opName)
{
	CellOpType type;

	std::map<CellOpStrategy, CellOpType>::iterator it = _cellOperations.begin();

	while(it != _cellOperations.end())
	{
		if(it->second._opName == opName)
		{
			type = it->second;
			break;
		}

		++it;
	}

	return type;
}

std::vector<std::string> FillCellOp::getCompatibleAttributes(const std::string& opName, TeAttributeList& attrList)
{
	std::vector<std::string> vecList;

	CellOpType opType = getCellOpType(opName);

	TeAttributeList::iterator it = attrList.begin();
	
	while (it != attrList.end())
	{
		TeAttrDataType type = it->rep_.type_;
		
		if( opType._opStrategy == CellOp_MajorityStrategy || 
			opType._opStrategy == CellOp_CategoryPercentageStrategy || 
			opType._opStrategy == CellOp_CategoryMajorityStrategy || 
			opType._opStrategy == CellOp_CategoryAreaPercentageStrategy )
		{
			if(type == TeSTRING || type == TeINT)
			{
				vecList.push_back(it->rep_.name_);
			}
		}
		else
		{
			if(type == TeREAL || type == TeINT)
			{
				vecList.push_back(it->rep_.name_);
			}
		}

		++it;
	}

	return vecList;
}
