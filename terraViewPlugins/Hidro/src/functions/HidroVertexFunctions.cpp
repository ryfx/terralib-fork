#include <HidroVertexFunctions.h>

#include <HidroGraph.h>
#include <HidroVertex.h>
#include <HidroMetadata.h>
#include <HidroPersister.h>

#include <TeDatabase.h>
#include <TeVectorRemap.h>

HidroVertexFunctions::HidroVertexFunctions(TeDatabase* database) : _db(database), _errorMessage("")
{
}

HidroVertexFunctions::~HidroVertexFunctions()
{
}

bool HidroVertexFunctions::createAttributeFromRaster(const std::string& graphName, TeLayer* layer, const std::string& attrName)
{
	HidroMetadata metadata(_db);

//verify input parameters
	if(graphName.empty() || !metadata.existGraph(graphName))
	{
		_errorMessage = "Invalid Graph.";
		return false;
	}

	if(!layer || !layer->raster())
	{
		_errorMessage = "Invalid Layer.";
		return false;
	}

	if(attrName.empty() || metadata.existGraphAttrEntry(graphName, attrName, true))
	{
		_errorMessage = "Invalid Attribute.";
		return false;
	}

	TeProjection* graphProj = metadata.getGraphProjection(graphName);
	TeProjection* layerProj = layer->projection();

	bool hasToRemap = false;

	if(graphProj && layerProj && !(*graphProj == *layerProj))
	{
		hasToRemap = true;
	}

//get graph from database
	HidroGraph graph;

	HidroPersister persister(_db);

	if(!persister.loadGraphFromDatabase(graphName, graph))
	{
		_errorMessage = persister.getErrorMessage();
		return false;
	}

//create the attribute in database table
	if(!metadata.addGraphAttr(graphName, attrName, false, true))
	{
		_errorMessage = metadata.getErrorMessage();
		return false;
	}

//get attribute table name
	TeLayer* layerVertex = metadata.getGraphLayerPoints(graphName);
	
	if(!layerVertex || layerVertex->attrTables().empty())
	{
		_errorMessage = metadata.getErrorMessage();
		return false;
	}

	std::string tableName = layerVertex->attrTables()[0].name();


//get vertex from graph
	std::map<std::string, HidroVertex*>::iterator itVertex = graph.getGraphMapVertex().begin();

	if(TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Generating Vertex Attribute");
		TeProgress::instance()->setTotalSteps(graph.getGraphMapVertex().size());
	}

	int count = 0;

	while(itVertex != graph.getGraphMapVertex().end())
	{
//get vertex
		HidroVertex* vertex = itVertex->second;

		std::string vertexObjId = vertex->getVertexId();

		TeCoord2D coordVertex = vertex->getVertexCoord();

//verify if has to remap
		if(hasToRemap)
		{
			TeVectorRemap(vertex->getVertexCoord(), graphProj, coordVertex, layerProj);
		}

//convert coord to index
		TeCoord2D coord = layer->raster()->coord2Index(coordVertex);

		double value = 0.;
		
//get raster element
		layer->raster()->getElement(coord.x(), coord.y(), value);

//save attribute in database table
		std::string sql = "UPDATE ";
				sql+= tableName;
				sql+= " SET ";
				sql+= attrName;
				sql+= " = ";
				sql+= Te2String(value, 6);
				sql+= " WHERE ";
				sql+= metadata.getVertexNameAttrName();
				sql+= " = '";
				sql+= vertexObjId;
				sql+= "'";

		if(!_db->execute(sql))
		{
			_errorMessage = _db->errorMessage();
			return false;
		}

		if(TeProgress::instance())
		{
			TeProgress::instance()->setProgress(count);
		}

		++count;
		++itVertex;
	}

	if(TeProgress::instance())
	{
		TeProgress::instance()->reset();
	}

	return true;
}

bool HidroVertexFunctions::createAttributeFromPolygons(const std::string& graphName, TeLayer* layer, const std::string& className, const std::string& attrName)
{
	HidroMetadata metadata(_db);

//verify input parameters
	if(graphName.empty() || !metadata.existGraph(graphName))
	{
		_errorMessage = "Invalid Graph.";
		return false;
	}

	if(!layer)
	{
		_errorMessage = "Invalid Layer.";
		return false;
	}

	if(attrName.empty() || metadata.existGraphAttrEntry(graphName, attrName, true))
	{
		_errorMessage = "Invalid Attribute.";
		return false;
	}

	TeProjection* graphProj = metadata.getGraphProjection(graphName);
	TeProjection* layerProj = layer->projection();

	bool hasToRemap = false;

	if(graphProj && layerProj && !(*graphProj == *layerProj))
	{
		hasToRemap = true;
	}

//get graph from database
	HidroGraph graph;

	HidroPersister persister(_db);

	if(!persister.loadGraphFromDatabase(graphName, graph))
	{
		_errorMessage = persister.getErrorMessage();
		return false;
	}

//create the attribute in database table

	TeAttributeList attrList = layer->attrTables()[0].attributeList();

	bool isStringAttr = true;

	for(unsigned int i = 0; i < attrList.size(); ++i)
	{
		if(attrList[i].rep_.name_ == className)
		{
			if(attrList[i].rep_.type_ != TeSTRING)
			{
				isStringAttr = false;
			}
			
			break;
		}
	}

	if(!metadata.addGraphAttr(graphName, attrName, isStringAttr, true))
	{
		_errorMessage = metadata.getErrorMessage();
		return false;
	}

//get attribute table name
	TeLayer* layerVertex = metadata.getGraphLayerPoints(graphName);
	
	if(!layerVertex || layerVertex->attrTables().empty())
	{
		_errorMessage = metadata.getErrorMessage();
		return false;
	}

	std::string tableName = layerVertex->attrTables()[0].name();


//get vertex from graph
	std::map<std::string, HidroVertex*>::iterator itVertex = graph.getGraphMapVertex().begin();

	if(TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Generating Vertex Attribute");
		TeProgress::instance()->setTotalSteps(graph.getGraphMapVertex().size());
	}

	int count = 0;

	while(itVertex != graph.getGraphMapVertex().end())
	{
//get vertex
		HidroVertex* vertex = itVertex->second;

		std::string vertexObjId = vertex->getVertexId();

		TeCoord2D coordVertex = vertex->getVertexCoord();

//verify if has to remap
		if(hasToRemap)
		{
			TeVectorRemap(vertex->getVertexCoord(), graphProj, coordVertex, layerProj);
		}

		TeBox box(coordVertex, coordVertex);

//get attribute class from layer
		std::string sqlSelect = "SELECT " + layer->tableName(TePOLYGONS) + ".*," + className;
					sqlSelect += " FROM " + layer->tableName(TePOLYGONS);
					sqlSelect += " LEFT JOIN " + layer->attrTables()[0].name();
					sqlSelect += " ON " + layer->tableName(TePOLYGONS) + ".object_id = " + layer->attrTables()[0].name() + "." + layer->attrTables()[0].linkName();
					sqlSelect += " WHERE " + layer->database()->getSQLBoxWhere( box, TePOLYGONS, layer->tableName(TePOLYGONS) );
					sqlSelect += " ORDER BY object_id asc, parent_id asc, num_holes desc";
		
		TeDatabasePortal* portal = _db->getPortal();

		if(!portal->query(sqlSelect, TeSERVERSIDE, TeUNIDIRECTIONAL, TeREADONLY, TeBINARYCURSOR))
		{			
			delete portal;
			return false;
		}

		if(portal->fetchRow())
		{
			std::string value = portal->getData(className);

			delete portal;

	//save attribute in database table
			std::string sql = "UPDATE ";
					sql+= tableName;
					sql+= " SET ";
					sql+= attrName;
					sql+= " = ";

			if(isStringAttr)
			{
				sql+= "'" + value + "'";
			}
			else
			{
				sql+= value;
			}

			sql+= " WHERE ";
			sql+= metadata.getVertexNameAttrName();
			sql+= " = '";
			sql+= vertexObjId;
			sql+= "'";

			if(!_db->execute(sql))
			{
				_errorMessage = _db->errorMessage();
				return false;
			}
		}

		if(TeProgress::instance())
		{
			TeProgress::instance()->setProgress(count);
		}

		++count;
		++itVertex;
	}

	if(TeProgress::instance())
	{
		TeProgress::instance()->reset();
	}

	return true;
}

std::string HidroVertexFunctions::getErrorMessage() const
{
	return _errorMessage;
}