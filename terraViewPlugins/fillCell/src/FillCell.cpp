#include <FillCell.h>
#include <FillCellUtils.h>

#include <TeCellAlgorithms.h>
#include <TeComputeAttributeStrategies.h>
#include <TeUtils.h>

FillCell::FillCell(TeDatabase* db) :
_errorMessage(""),
_inputThemeName(""),
_dataTableName(""),
_dataAttrTableName(""),
_layerCellName(""),
_cellTableName(""),
_cellAttrTableName(""),
_usecellDefaultValue(false),
_cellDefaultValue(""),
_useDummy(false),
_dummy(TeMAXFLOAT),
_ignoreValue(false),
_valueIgnored(""),
_rep(TeGEOMETRYNONE),
_strategy(CellOp_None),
_useTemporalTable(false)
{
	_db = db;
	_inputTheme = NULL;
	_cellLayer = NULL;
}

FillCell::~FillCell()
{
}

bool FillCell::executeOperation()
{
	if(!checkParameters())
	{
		return false;
	}

	_errorMessage = "";

	bool res = false;

	if(_strategy == CellOp_MinimumStrategy)
	{
		res = executeMinimumStrategy();
	}
	else if(_strategy == CellOp_MaximumStrategy)
	{
		res = executeMaximumStrategy();
	}
	else if(_strategy == CellOp_AverageStrategy)
	{
		res = executeAverageStrategy();
	}
	else if(_strategy == CellOp_SumStrategy)
	{
		res = executeSumStrategy();
	}
	else if(_strategy == CellOp_STDevStrategy)
	{
		res = executeSTDevStrategy();
	}
	else if(_strategy == CellOp_MajorityStrategy)
	{
		res = executeMajorityStrategy();
	}
	else if(_strategy == CellOp_CategoryPercentageStrategy)
	{
		res = executeCategoryPercentageStrategy();
	}
	else if(_strategy == CellOp_AverageWeighByAreaStrategy)
	{
		res = executeAverageWeighByAreaStrategy();
	}
	else if(_strategy == CellOp_SumWeighByAreaStrategy)
	{
		res = executeSumWeighByAreaStrategy();
	}
	else if(_strategy == CellOp_CategoryMajorityStrategy)
	{
		res = executeCategoryMajorityStrategy();
	}
	else if(_strategy == CellOp_CategoryAreaPercentageStrategy)
	{
		res = executeCategoryAreaPercentageStrategy();
	}
	else if(_strategy == CellOp_MinimumDistanceStrategy)
	{
		res = executeMinimumDistanceStrategy();
	}
	else if(_strategy == CellOp_PresenceStrategy)
	{
		res = executePresenceStrategy();
	}
	else if(_strategy == CellOp_CountObjectsStrategy)
	{
		res = executeCountObjectsStrategy();
	}
	else if(_strategy == CellOp_TotalAreaPercentageStrategy)
	{
		res = executeTotalAreaPercentageStrategy();
	}

	if(!res)
	{
		_errorMessage = "Internal Error. " + _db->errorMessage();
	}

	return res;
}

std::string FillCell::getErrorMessage()
{
	return _errorMessage;
}

bool FillCell::checkParameters()
{
	_errorMessage = "";

//check database
	if(!_db)
	{
		_errorMessage = "Database not found.";
		return false;
	}

	FillCellUtils utils(_db);

//check input theme name
	if(_inputThemeName.empty())
	{
		_errorMessage = "Input theme not defined.";
		return false;
	}

	_inputTheme = utils.getTheme(_inputThemeName);

	if(!_inputTheme)
	{
		_errorMessage = "Input theme not found.";
		return false;
	}

	if(!(_rep & TeRASTER))
	{
		if((_strategy != CellOp_MinimumDistanceStrategy) &&
		(_strategy != CellOp_CountObjectsStrategy) &&
		(_strategy != CellOp_TotalAreaPercentageStrategy) &&
		(_strategy != CellOp_PresenceStrategy))
		{
	//check data table name
			if(_dataTableName.empty())
			{
				_errorMessage = "Table from Theme not defined.";
				return false;
			}

	//check data attr table name
			if(_dataAttrTableName.empty())
			{
				_errorMessage = "Attr Table from Theme not defined.";
				return false;
			}
		}
	}

//check layer cell
	if(_layerCellName.empty())
	{
		_errorMessage = "Layer Cell not defined.";
		return false;
	}

	_cellLayer = utils.getLayer(_layerCellName);

	if(!_cellLayer)
	{
		_errorMessage = "Layer Cell not found.";
		return false;
	}

//check cell table name
	if(_cellTableName.empty())
	{
		_errorMessage = "Table from Cell Layer not defined.";
		return false;
	}

	if(_strategy != CellOp_CategoryAreaPercentageStrategy)
	{
//check cell attr table name
		if(_cellAttrTableName.empty())
		{
			_errorMessage = "Attr Table from Cell Layer not defined.";
			return false;
		}
	}

	if(_usecellDefaultValue)
	{
		if(_cellDefaultValue.empty())
		{
			_errorMessage = "Default value for attr Table from Cell Layer not defined.";
			return false;
		}
	}

//check geom rep
	if(_rep & TeGEOMETRYNONE)
	{
		_errorMessage = "Geometry type not defined.";
		return false;
	}

//check strategy
	if(_strategy == CellOp_None)
	{
		_errorMessage = "Strategy not defined.";
		return false;
	}

//check time interval
	if(_useTemporalTable)
	{
		if(!_timeInterval.isValid())
		{
			_errorMessage = "Time intervals is not Valid.";
			return false;
		}
	}

	return true;
}

bool FillCell::executeMinimumStrategy()
{
	bool res = false;

	if(_rep & TeRASTER)
	{
		TeComputeAttrStrategy<TeBoxRasterIterator>* op = new TeMinimumStrategy<TeBoxRasterIterator>(_dummy, atof(_cellDefaultValue.c_str()));

		res = TeFillCellNonSpatialRasterOperation(_db, _inputTheme->layer()->name(), op, _layerCellName, _cellTableName, _cellAttrTableName, _timeInterval);	
	}
	else if(_rep & TePOLYGONS || _rep & TeLINES || _rep & TePOINTS || _rep & TeCELLS)
	{
		TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* op = new TeMinimumStrategy<TeSTElementSet::propertyIterator>(atof(_valueIgnored.c_str()), atof(_cellDefaultValue.c_str()));

		std::string attrName = _dataTableName +"."+ _dataAttrTableName;

		res = TeFillCellNonSpatialOperation(_inputTheme, _rep, attrName, op, _cellLayer, _cellTableName, _cellAttrTableName, _timeInterval);
	}

	return res;
}

bool FillCell::executeMaximumStrategy()
{
	bool res = false;

	if(_rep & TeRASTER)
	{
		TeComputeAttrStrategy<TeBoxRasterIterator>* op = new TeMaximumStrategy<TeBoxRasterIterator>(_dummy, atof(_cellDefaultValue.c_str()));

		res = TeFillCellNonSpatialRasterOperation(_db, _inputTheme->layer()->name(), op, _layerCellName, _cellTableName, _cellAttrTableName, _timeInterval);	
	}
	else if(_rep & TePOLYGONS || _rep & TeLINES || _rep & TePOINTS || _rep & TeCELLS)
	{
		TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* op = new TeMaximumStrategy<TeSTElementSet::propertyIterator>(atof(_valueIgnored.c_str()), atof(_cellDefaultValue.c_str()));

		std::string attrName = _dataTableName +"."+ _dataAttrTableName;

		res = TeFillCellNonSpatialOperation(_inputTheme, _rep, attrName, op, _cellLayer, _cellTableName, _cellAttrTableName, _timeInterval);
	}

	return res;
}

bool FillCell::executeAverageStrategy()
{
	bool res = false;

	if(_rep & TeRASTER)
	{
		TeComputeAttrStrategy<TeBoxRasterIterator>* op = new TeAverageStrategy<TeBoxRasterIterator>(_dummy, atof(_cellDefaultValue.c_str()));

		res = TeFillCellNonSpatialRasterOperation(_db, _inputTheme->layer()->name(), op, _layerCellName, _cellTableName, _cellAttrTableName, _timeInterval);	
	}
	else if(_rep & TePOLYGONS || _rep & TeLINES || _rep & TePOINTS || _rep & TeCELLS)
	{
		TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* op = new TeAverageStrategy<TeSTElementSet::propertyIterator>(atof(_valueIgnored.c_str()), atof(_cellDefaultValue.c_str()));

		std::string attrName = _dataTableName +"."+ _dataAttrTableName;

		res = TeFillCellNonSpatialOperation(_inputTheme, _rep, attrName, op, _cellLayer, _cellTableName, _cellAttrTableName, _timeInterval);
	}

	return res;
}

bool FillCell::executeSumStrategy()
{
	bool res = false;

	if(_rep & TeRASTER)
	{
		TeComputeAttrStrategy<TeBoxRasterIterator>* op = new TeSumStrategy<TeBoxRasterIterator>(_dummy, atof(_cellDefaultValue.c_str()));

		res = TeFillCellNonSpatialRasterOperation(_db, _inputTheme->layer()->name(), op, _layerCellName, _cellTableName, _cellAttrTableName, _timeInterval);	
	}
	else if(_rep & TePOLYGONS || _rep & TeLINES || _rep & TePOINTS || _rep & TeCELLS)
	{
		TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* op = new TeSumStrategy<TeSTElementSet::propertyIterator>(atof(_valueIgnored.c_str()), atof(_cellDefaultValue.c_str()));

		std::string attrName = _dataTableName +"."+ _dataAttrTableName;

		res = TeFillCellNonSpatialOperation(_inputTheme, _rep, attrName, op, _cellLayer, _cellTableName, _cellAttrTableName, _timeInterval);
	}

	return res;
}

bool FillCell::executeSTDevStrategy()
{
	bool res = false;

	if(_rep & TeRASTER)
	{
		TeComputeAttrStrategy<TeBoxRasterIterator>* op = new TeSTDevStrategy<TeBoxRasterIterator>(_dummy, atof(_cellDefaultValue.c_str()));

		res = TeFillCellNonSpatialRasterOperation(_db, _inputTheme->layer()->name(), op, _layerCellName, _cellTableName, _cellAttrTableName, _timeInterval);	
	}
	else if(_rep & TePOLYGONS || _rep & TeLINES || _rep & TePOINTS || _rep & TeCELLS)
	{
		TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* op = new TeSTDevStrategy<TeSTElementSet::propertyIterator>(atof(_valueIgnored.c_str()), atof(_cellDefaultValue.c_str()));

		std::string attrName = _dataTableName +"."+ _dataAttrTableName;

		res = TeFillCellNonSpatialOperation(_inputTheme, _rep, attrName, op, _cellLayer, _cellTableName, _cellAttrTableName, _timeInterval);
	}

	return res;
}

bool FillCell::executeMajorityStrategy()
{
	bool res = false;

	if(_rep & TeRASTER)
	{
		TeComputeAttrStrategy<TeBoxRasterIterator>* op = new TeMajorityStrategy<TeBoxRasterIterator>(TeREAL, _cellDefaultValue, Te2String(_dummy));

		res = TeFillCellNonSpatialRasterOperation(_db, _inputTheme->layer()->name(), op, _layerCellName, _cellTableName, _cellAttrTableName, _timeInterval);	
	}
	else if(_rep & TePOLYGONS || _rep & TeLINES || _rep & TePOINTS || _rep & TeCELLS)
	{
		FillCellUtils utils(_db);

		TeAttrDataType type = utils.getAttrDataType(_inputTheme->layer()->name(), _dataTableName, _dataAttrTableName);

		TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* op = new TeMajorityStrategy<TeSTElementSet::propertyIterator>(type, _cellDefaultValue, _valueIgnored);

		std::string attrName = _dataTableName +"."+ _dataAttrTableName;

		res = TeFillCellNonSpatialOperation(_inputTheme, _rep, attrName, op, _cellLayer, _cellTableName, _cellAttrTableName, _timeInterval);
	}

	return res;
}

bool FillCell::executeCategoryPercentageStrategy()
{
	bool res = false;

	if(_rep & TeRASTER)
	{
		res = TeFillCellCategoryCountPercentageRasterOperation(_db, _inputTheme->layer()->name(), _layerCellName, _cellTableName, _cellAttrTableName, _timeInterval);
	}
	else if(_rep & TePOLYGONS || _rep & TeLINES || _rep & TePOINTS || _rep & TeCELLS)
	{
		std::string attrName = _dataTableName +"."+ _dataAttrTableName;

		map<string, string> classesMap;
		vector< string > atts;
		TeQuerier* querier;
		TeSTInstance sti;

		atts.push_back(attrName);

 		TeQuerierParams params (false, atts);
		params.setParams (_inputTheme);

		querier = new TeQuerier(params);

		querier->loadInstances();

		while( querier->fetchInstance(sti) )
		{
			std::string value = sti.getPropertyVector()[0].value_;

			std::string replaceChars = TeReplaceSpecialChars(value);

			std::string removeChars = TeRemoveSpecialChars(replaceChars);

			classesMap[sti.getPropertyVector()[0].value_] = removeChars;

		}
		delete querier;

		TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* op = new TeCategoryPercentageStrategy<TeSTElementSet::propertyIterator>(classesMap, _usecellDefaultValue);
		
		//TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* op = new TeCategoryPercentageStrategy<TeSTElementSet::propertyIterator>;

		res = TeFillCellNonSpatialOperation(_inputTheme, _rep, attrName, op, _cellLayer, _cellTableName, _cellAttrTableName, _timeInterval);
	}

	return res;
}

bool FillCell::executeAverageWeighByAreaStrategy()
{
	bool res = false;

	if(_rep & TePOLYGONS)
	{
		TeComputeSpatialStrategy* op = new TeAverageWeighByAreaStrategy(_inputTheme, _dataAttrTableName, _rep);

		res = TeFillCellSpatialOperation(_db, op, _layerCellName, _cellTableName, _cellAttrTableName, _timeInterval);
	}

	return res;
}

bool FillCell::executeSumWeighByAreaStrategy()
{
	bool res = false;

	if(_rep & TePOLYGONS)
	{
		TeComputeSpatialStrategy* op = new TeSumWeighByAreaStrategy(_inputTheme, _dataAttrTableName, _rep);

		res = TeFillCellSpatialOperation(_db, op, _layerCellName, _cellTableName, _cellAttrTableName, _timeInterval);
	}

	return res;
}

bool FillCell::executeCategoryMajorityStrategy()
{
	bool res = false;

	if(_rep & TePOLYGONS)
	{
		//TeComputeSpatialStrategy* op = new TeCategoryMajorityStrategy(_inputTheme, _dataAttrTableName, _rep);

		//res = TeFillCellSpatialOperation(_db, op, _layerCellName, _cellTableName, _cellAttrTableName, _timeInterval);
	}

	return res;
}

bool FillCell::executeCategoryAreaPercentageStrategy()
{
	bool res = false;

	if(_rep & TePOLYGONS)
	{
		res = TeFillCellCategoryAreaPercentageOperation(_inputTheme, _dataAttrTableName , _cellLayer, _cellTableName, _timeInterval);
	}

	return res;
}

bool FillCell::executeMinimumDistanceStrategy()
{
	bool res = false;

	if(_rep & TePOLYGONS || _rep & TeLINES || _rep & TePOINTS)
	{
		res = TeFillCellDistanceOperation(_inputTheme, _rep, _cellLayer, _cellTableName, _cellAttrTableName, _timeInterval);
	}

	return res;
}

bool FillCell::executePresenceStrategy()
{
	bool res = false;

	if(_rep & TePOLYGONS || _rep & TeLINES || _rep & TePOINTS)
	{
		TeComputeAttrStrategy<TeSTElementSet::propertyIterator>* op = new TePresenceStrategy<TeSTElementSet::propertyIterator>;

		TeAttrTableVector& attrs = _inputTheme->attrTables();
		std::string attrName = attrs[0].uniqueName();

		res = TeFillCellNonSpatialOperation(_inputTheme, _rep, attrName, op, _cellLayer, _cellTableName, _cellAttrTableName, _timeInterval);
	}

	return res;
}

bool FillCell::executeCountObjectsStrategy()
{
	bool res = false;

	TeComputeSpatialStrategy* op = NULL;

	if(_rep & TePOLYGONS)
	{
		op = new TeCountPolygonalObjectsStrategy(_inputTheme);
	}
	else if(_rep & TeLINES)
	{
		op = new TeCountLineObjectsStrategy(_inputTheme);
	}
	else if(_rep & TePOINTS)
	{
		op = new TeCountPointObjectsStrategy(_inputTheme);
	}
	
	if(op)
	{
		res = TeFillCellSpatialOperation(_db, op, _layerCellName, _cellTableName, _cellAttrTableName, _timeInterval);
	}

	return res;
}

bool FillCell::executeTotalAreaPercentageStrategy()
{
	bool res = false;

	if(_rep & TePOLYGONS)
	{
		TeComputeSpatialStrategy* op = new TeTotalAreaPercentageStrategy(_inputTheme);

		res = TeFillCellSpatialOperation(_db, op, _layerCellName, _cellTableName, _cellAttrTableName, _timeInterval);
	}

	return res;
}