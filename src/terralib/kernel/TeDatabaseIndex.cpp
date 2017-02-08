#include "TeDatabaseIndex.h"

TeDatabaseIndex::TeDatabaseIndex()
{
}

TeDatabaseIndex::~TeDatabaseIndex()
{
}

void TeDatabaseIndex::setIndexName(const std::string& name)
{
	_indexName = name;
}

std::string TeDatabaseIndex::getIndexName()
{
	return _indexName;
}

void TeDatabaseIndex::setColumns(const std::vector<std::string>& columns)
{
	_indexColumns = columns;
}

std::vector<std::string> TeDatabaseIndex::getColumns()
{
	return _indexColumns;
}

void TeDatabaseIndex::setIsPrimaryKey(bool isPk)
{
	_isPrimaryKey = isPk;
}

bool TeDatabaseIndex::isPrimaryKey()
{
	return _isPrimaryKey;
}
