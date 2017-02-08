#include <HidroOptimizedEdition.h>
#include <HidroMetadata.h>

#include <HidroEdgeEdition.h>

#include <TeDatabase.h>


HidroOptimizedEdition::HidroOptimizedEdition(TeDatabase* database, const std::string& graphName)
{
	_db			= database;
	_graphName	= graphName;
}

HidroOptimizedEdition::~HidroOptimizedEdition()
{
}

std::string HidroOptimizedEdition::getErrorMessage() const
{
	return _errorMessage;
}


bool HidroOptimizedEdition::getValidDirections(const TeCoord2D& coord, HidroOptEditionDirections& optEdDirections)
{
	HidroMetadata metadata(_db);

	double resolution = metadata.getGraphResolution(_graphName);
	double radius = resolution + resolution / 2.;
	double precision = resolution / 2.;

	TeBox box;
	box.x1_ = coord.x() - radius;
	box.x2_ = coord.x() + radius;
	box.y1_ = coord.y() - radius;
	box.y2_ = coord.y() + radius;

	TeLayer* layer = metadata.getGraphLayerPoints(_graphName);

	if(!layer)
	{
		_errorMessage = "Error getting layer";
		return false;
	}

	TeProjection* proj = metadata.getGraphProjection(_graphName);

	TePrecision::instance().setPrecision(precision);

	TePointSet ps;

	layer->getPoints(ps, layer->database()->getSQLBoxWhere(box, TePOINTS, layer->tableName(TePOLYGONS)));  

	if(!ps.empty())
	{
//remove center point
		for(unsigned int i = 0; i < ps.size(); ++i)
		{
			if(TeEquals(ps[i].elem(), coord))
			{
				ps.erase(i);
				break;
			}
		}

//find major y values
		int indexMaxY = 0;

		TePointSet psMaxY;
		while(indexMaxY != -1)
		{
			indexMaxY = getIndexForMaxCooordInY(ps, coord.y(), precision);

			if(indexMaxY != -1)
			{
				psMaxY.add(ps[indexMaxY]);
				ps.erase(indexMaxY);
			}
		}

//find minor y values
		int indexMinY = 0;

		TePointSet psMinY;
		while(indexMinY != -1)
		{
			indexMinY = getIndexForMinCooordInY(ps, coord.y(), precision);

			if(indexMinY != -1)
			{
				psMinY.add(ps[indexMinY]);
				ps.erase(indexMinY);
			}
		}

//find left point
		int indexLeft = getIndexForMinCooordInX(ps, coord.x(), precision);
		if(indexLeft != -1)
		{
			optEdDirections.ptLeft = ps[indexLeft];
			optEdDirections.hasPtLeft = true;
		}

//find right point
		int indexRight = getIndexForMaxCooordInX(ps, coord.x(), precision);
		if(indexRight != -1)
		{
			optEdDirections.ptRight = ps[indexRight];
			optEdDirections.hasPtRight = true;
		}

//find upperleft point
		int indexUpperLeft = getIndexForMinCooordInX(psMaxY, coord.x(), precision);
		if(indexUpperLeft != -1)
		{
			optEdDirections.ptUpperLeft = psMaxY[indexUpperLeft];
			optEdDirections.hasPtUpperLeft = true;
			psMaxY.erase(indexUpperLeft);
		}

//find upperright point
		int indexUpperRight = getIndexForMaxCooordInX(psMaxY, coord.x(), precision);
		if(indexUpperRight != -1)
		{
			optEdDirections.ptUpperRight = psMaxY[indexUpperRight];
			optEdDirections.hasPtUpperRight = true;
			psMaxY.erase(indexUpperRight);
		}

//find up point
		if(!psMaxY.empty())
		{
			optEdDirections.ptUpper = psMaxY[0];
			optEdDirections.hasPtUpper = true;
		}

//find lowerleft point
		int indexLowerLeft = getIndexForMinCooordInX(psMinY, coord.x(), precision);
		if(indexLowerLeft != -1)
		{
			optEdDirections.ptLowerLeft = psMinY[indexLowerLeft];
			optEdDirections.hasPtLowerLeft = true;
			psMinY.erase(indexLowerLeft);
		}

//find lowerright point
		int indexLowerRight = getIndexForMaxCooordInX(psMinY, coord.x(), precision);
		if(indexLowerRight != -1)
		{
			optEdDirections.ptLowerRight = psMinY[indexLowerRight];
			optEdDirections.hasPtLowerRight = true;
			psMinY.erase(indexLowerRight);
		}

//find down point
		if(!psMinY.empty())
		{
			optEdDirections.ptLower = psMinY[0];
			optEdDirections.hasPtLower = true;

		}	
	}

	TePrecision::instance().setPrecision(TeGetPrecision(proj));

	return true;
}

bool HidroOptimizedEdition::createEdge(TeTheme* edgeTheme, TeTheme* vertexTheme, TeQtGrid* grid, 
						const std::string& vertexFromId, const std::string& vertexToId, const std::string& name, const bool& autoCreateAttrs)
{
	HidroEdgeEdition edgeEdition(edgeTheme, vertexTheme, grid);

//verify if this vertex has any edge
	bool hasOutputEdges			= false;
	std::string outputEdgeName	= "";

	if(hasVertexOutputEdges(vertexFromId, hasOutputEdges, outputEdgeName))
	{
		if(hasOutputEdges)
		{
//delete existing edge
			if(!edgeEdition.removeEdge(outputEdgeName))
			{
				return false;
			}
		}
	}
	
//verify if detiny vertex has an edge to this vertex
	bool hasInputEdges			= false;
	std::string inputEdgeName	= "";

	if(hasVertexInputEdges(vertexToId, vertexFromId, hasInputEdges, inputEdgeName))
	{
		if(hasInputEdges)
		{
//delete existing edge
			if(!edgeEdition.removeEdge(inputEdgeName))
			{
				return false;
			}
		}
	}

//verify if has to create automatic attributes
	std::vector<std::string> attrs;

	if(autoCreateAttrs)
	{
	}

//create new edge
	if(!edgeEdition.addEdge(vertexFromId, vertexToId, attrs, name))
	{
		return false;
	}

	return true;
}

bool HidroOptimizedEdition::hasEdge(const std::string& vertexNameFrom, const std::string& vertexNameTo, bool& hasEdge)
{
	_errorMessage = "";

	if(vertexNameFrom.empty() || vertexNameTo.empty())
	{
		_errorMessage = "Invalid input parameters.";
		return false;
	}

	HidroMetadata metadata(_db);

	TeLayer* layer = metadata.getGraphLayerLines(_graphName);

	if(layer && !layer->attrTables().empty())
	{		
		std::string sql  = "SELECT ";
					sql += " * FROM ";
					sql += layer->attrTables()[0].name();
					sql += " WHERE ";
					sql += metadata.getEdgeVertexFromAttrName();
					sql += " = '";
					sql += vertexNameFrom;
					sql += "'";
					sql += " AND ";
					sql += metadata.getEdgeVertexToAttrName();
					sql += " = '";
					sql += vertexNameTo;
					sql += "'";


//get database portal
		TeDatabasePortal* portal = _db->getPortal();

		if(!portal)
		{
			_errorMessage = "Database Portal Internal Error.";
			return false;
		}

		if(!portal->query(sql))
		{
			delete portal;

			_errorMessage = "Error getting edge informations.";
			return false;
		}

//verify if exist edges from this vertex
		if(portal->fetchRow())
		{
			hasEdge = true;
		}
		else
		{
			hasEdge = false;	
		}

		delete portal;
	}

	return true;
}

bool HidroOptimizedEdition::hasVertexOutputEdges(const std::string& vertexName, bool& hasEdges, std::string& edgeName)
{
	_errorMessage = "";

	if(vertexName.empty())
	{
		_errorMessage = "Vertex name is empty.";
		return false;
	}

	HidroMetadata metadata(_db);

	TeLayer* layer = metadata.getGraphLayerLines(_graphName);

	if(layer && !layer->attrTables().empty())
	{		
		std::string sql  = "SELECT ";
					sql += metadata.getEdgeNameAttrName();
					sql += " FROM ";
					sql += layer->attrTables()[0].name();
					sql += " WHERE ";
					sql += metadata.getEdgeVertexFromAttrName();
					sql += " = '";
					sql += vertexName;
					sql += "'";


//get database portal
		TeDatabasePortal* portal = _db->getPortal();

		if(!portal)
		{
			_errorMessage = "Database Portal Internal Error.";
			return false;
		}

		if(!portal->query(sql))
		{
			delete portal;

			_errorMessage = "Error getting edge informations.";
			return false;
		}

//verify if exist edges from this vertex
		if(portal->fetchRow())
		{
			hasEdges = true;
			edgeName = portal->getData(0);
		}
		else
		{
			hasEdges = false;	
		}

		delete portal;
	}

	return true;
}

bool HidroOptimizedEdition::hasVertexInputEdges(const std::string& vertexNameFrom, const std::string& vertexNameTo, bool& hasEdges, std::string& edgeName)
{
	_errorMessage = "";

	if(vertexNameFrom.empty() || vertexNameTo.empty())
	{
		_errorMessage = "Invalid input parameters.";
		return false;
	}

	HidroMetadata metadata(_db);

	TeLayer* layer = metadata.getGraphLayerLines(_graphName);

	if(layer && !layer->attrTables().empty())
	{		
		std::string sql  = "SELECT ";
					sql += metadata.getEdgeNameAttrName();
					sql += " FROM ";
					sql += layer->attrTables()[0].name();
					sql += " WHERE ";
					sql += metadata.getEdgeVertexFromAttrName();
					sql += " = '";
					sql += vertexNameFrom;
					sql += "'";
					sql += " AND ";
					sql += metadata.getEdgeVertexToAttrName();
					sql += " = '";
					sql += vertexNameTo;
					sql += "'";


//get database portal
		TeDatabasePortal* portal = _db->getPortal();

		if(!portal)
		{
			_errorMessage = "Database Portal Internal Error.";
			return false;
		}

		if(!portal->query(sql))
		{
			delete portal;

			_errorMessage = "Error getting edge informations.";
			return false;
		}

//verify if exist edges from this vertex
		if(portal->fetchRow())
		{
			hasEdges = true;
			edgeName = portal->getData(0);
		}
		else
		{
			hasEdges = false;	
		}

		delete portal;
	}

	return true;
}

int HidroOptimizedEdition::getIndexForMaxCooordInY(TePointSet& ps, const double& yReference, const double& precision)
{
	int index = -1;

	double yValue = yReference;

	for(unsigned int i = 0; i < ps.size(); ++i)
	{
		double yCurValue = ps[i].elem().y();

		if(yCurValue > yReference && yCurValue > yValue)
		{
			double dif= yCurValue - yReference;

			if(dif > precision)
			{
				yValue = yCurValue;
				index = i;
			}
		}
	}

	return index;
}

int HidroOptimizedEdition::getIndexForMinCooordInY(TePointSet& ps, const double& yReference, const double& precision)
{
	int index = -1;

	double yValue = yReference;

	for(unsigned int i = 0; i < ps.size(); ++i)
	{
		double yCurValue = ps[i].elem().y();

		if(yCurValue < yReference && yCurValue < yValue)
		{
			double dif= yReference - yCurValue;

			if(dif > precision)
			{
				yValue = yCurValue;
				index = i;
			}
		}
	}

	return index;
}

int HidroOptimizedEdition::getIndexForMaxCooordInX(TePointSet& ps, const double& xReference, const double& precision)
{
	int index = -1;

	double xValue = xReference;

	for(unsigned int i = 0; i < ps.size(); ++i)
	{
		double xCurValue = ps[i].elem().x();

		if(xCurValue > xReference && xCurValue > xValue)
		{
			double dif= xCurValue - xReference;

			if(dif > precision)
			{
				xValue = xCurValue;
				index = i;
			}
		}
	}

	return index;
}
	
int HidroOptimizedEdition::getIndexForMinCooordInX(TePointSet& ps, const double& xReference, const double& precision)
{
	int index = -1;

	double xValue = xReference;

	for(unsigned int i = 0; i < ps.size(); ++i)
	{
		double xCurValue = ps[i].elem().x();

		if(xCurValue < xReference && xCurValue < xValue)
		{
			double dif= xReference - xCurValue;

			if(dif > precision)
			{
				xValue = xCurValue;
				index = i;
			}
		}
	}

	return index;
}