#include <HidroPersister.h>
#include <HidroVertex.h>
#include <HidroEdge.h>
#include <HidroMetadata.h>
#include <HidroGraph.h>

#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeAppTheme.h>
#include <TeApplicationUtils.h>
#include <TeWaitCursor.h>

#include <HidroAttrString.h>
#include <HidroAttrInt.h>
#include <HidroAttrDouble.h>

HidroPersister::HidroPersister(TeDatabase* database) : _db(database), _errorMessage("")
{
	if(_db)
	{
		_hidroMetadata = new HidroMetadata(_db);
	}
}

HidroPersister::~HidroPersister()
{
	if(_hidroMetadata)
	{
		delete _hidroMetadata;
		_hidroMetadata = NULL;
	}
}

std::string HidroPersister::getErrorMessage() const
{
	return _errorMessage;
}

bool HidroPersister::loadGraphFromDatabase(const std::string& graphName, HidroGraph& graph)
{
	_errorMessage = "";

	if(graphName.empty())
	{
		_errorMessage = "Graph name is empty.";
		return false;
	}

	if(!_hidroMetadata->existGraph(graphName))
	{
		_errorMessage = "Graph not exist.";
		return false;
	}

//get layer of points
	TeLayer* layerVertex = _hidroMetadata->getGraphLayerPoints(graphName);

//get layer of lines
	TeLayer* layerEdge = _hidroMetadata->getGraphLayerLines(graphName);

//check layers
	if(!layerVertex || !layerEdge)
	{
		_errorMessage = "Error getting graph layers.";
		return false;
	}
	
//set the graph name
	graph.setGraphName(graphName);

//set the graph vertex
	TeQuerierParams querierVertexParams(true, true);
	querierVertexParams.setParams(layerVertex);

	TeQuerier querierVertex(querierVertexParams);

	if(!querierVertex.loadInstances())
	{
		_errorMessage = "Error getting vertex instances.";
		return false;
	}

	TeSTInstance stiVertex;

	int count = 1;

	int numVertex = querierVertex.numElemInstances();

	if(TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Loading Graph Vertex");
		TeProgress::instance()->setTotalSteps(numVertex);
	}

	while(querierVertex.fetchInstance(stiVertex))
	{
		HidroVertex* vertex = new HidroVertex();

//get vertex attributes
		TePropertyVector vec = stiVertex.getPropertyVector();
		for(unsigned int i=0; i<vec.size(); ++i)
		{
			TeAttrDataType type = vec[i].attr_.rep_.type_;

			if(vec[i].attr_.rep_.name_ == _hidroMetadata->getObjectIdAttrName())
			{
				continue;
			}

//get vertex identification
			if(vec[i].attr_.rep_.name_ == _hidroMetadata->getVertexNameAttrName())
			{
				vertex->setVertexId(vec[i].value_);
			}
			else
			{
				if(type == TeSTRING)
				{
					HidroAttrString* attr = new HidroAttrString();
					attr->setAttrName(vec[i].attr_.rep_.name_);
					attr->setAttrValue(vec[i].value_);

					if(!vertex->addVertexAttr(attr))
					{
						_errorMessage = "Error adding vertex attributes.";
						return false;
					}
				}
				else if(type == TeINT)
				{
					HidroAttrInt* attr = new HidroAttrInt();
					attr->setAttrName(vec[i].attr_.rep_.name_);
					attr->setAttrValue(atoi(vec[i].value_.c_str()));

					if(!vertex->addVertexAttr(attr))
					{
						_errorMessage = "Error adding vertex attributes.";
						return false;
					}
				}
				else if(type == TeREAL)
				{
					HidroAttrDouble* attr= new HidroAttrDouble();
					attr->setAttrName(vec[i].attr_.rep_.name_);
					attr->setAttrValue(atof(vec[i].value_.c_str()));

					if(!vertex->addVertexAttr(attr))
					{
						_errorMessage = "Error adding vertex attributes.";
						return false;
					}
				}
			}
		}

//get vertex geometry
		if(stiVertex.hasPoints())
		{
			TePointSet pointSet;
			stiVertex.getGeometry(pointSet);

			if(pointSet.size() != 1)
			{
				_errorMessage = "Error getting vertex point.";
				return false;
			}

			TePoint point = pointSet[0];

			vertex->setVertexCoord(point.elem());
		}

		vertex->setVertexIdx(count);

		if(!graph.addGraphVertex(vertex))
		{
			_errorMessage = "Error adding vertex in graph.";
			return false;
		}

		if(TeProgress::instance())
		{
			TeProgress::instance()->setProgress(count -1);
		}

		++count;
	}

//set the graph edges
	TeQuerierParams querierEdgeParams(true, true);
	querierEdgeParams.setParams(layerEdge);

	TeQuerier querierEdge(querierEdgeParams);

	if(!querierEdge.loadInstances())
	{
		_errorMessage = "Error getting edge instance.";
		return false;
	}

	TeSTInstance stiEdge;

	int numEdge = querierEdge.numElemInstances();

	if(TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Loading Graph Edge");
		TeProgress::instance()->setTotalSteps(numEdge);
	}

	count = 0;

	while(querierEdge.fetchInstance(stiEdge))
	{
		HidroEdge* edge = new HidroEdge();

		HidroVertex* vertexFrom = NULL;
		HidroVertex* vertexTo   = NULL;

//get edge attributes
		TePropertyVector vec = stiEdge.getPropertyVector();
		for(unsigned int i=0; i<vec.size(); ++i)
		{
			TeAttrDataType type = vec[i].attr_.rep_.type_;

			if(vec[i].attr_.rep_.name_ == _hidroMetadata->getObjectIdAttrName())
			{
				continue;
			}

//get edge identification
			if(vec[i].attr_.rep_.name_ == _hidroMetadata->getEdgeNameAttrName())
			{
				edge->setEdgeId(vec[i].value_);
			}
//get edge vertex from
			else if(vec[i].attr_.rep_.name_ == _hidroMetadata->getEdgeVertexFromAttrName())
			{
				vertexFrom = graph.getGraphVertex(vec[i].value_);

				if(!vertexFrom)
				{
					_errorMessage = "Error getting vertex from.";
					return false;
				}	
			}
//get edge vertex to
			else if(vec[i].attr_.rep_.name_ == _hidroMetadata->getEdgeVertexToAttrName())
			{
				vertexTo = graph.getGraphVertex(vec[i].value_);

				if(!vertexTo)
				{
					_errorMessage = "Error getting vertex to.";
					return false;
				}
			}
//get edge attributes
			else
			{
				if(type == TeSTRING)
				{
					HidroAttrString* attr = new HidroAttrString();
					attr->setAttrName(vec[i].attr_.rep_.name_);
					attr->setAttrValue(vec[i].value_);

					if(!edge->addEdgeAttr(attr))
					{
						_errorMessage = "Error adding edge attributes.";
						return false;
					}
				}
				else if(type == TeINT)
				{
					HidroAttrInt* attr= new HidroAttrInt();
					attr->setAttrName(vec[i].attr_.rep_.name_);
					attr->setAttrValue(atoi(vec[i].value_.c_str()));

					if(!edge->addEdgeAttr(attr))
					{
						_errorMessage = "Error adding edge attributes.";
						return false;
					}
				}
				else if(type == TeREAL)
				{
					HidroAttrDouble* attr= new HidroAttrDouble();
					attr->setAttrName(vec[i].attr_.rep_.name_);
					attr->setAttrValue(atof(vec[i].value_.c_str()));

					if(!edge->addEdgeAttr(attr))
					{
						_errorMessage = "Error adding edges attributes.";
						return false;
					}
				}
			}
		}

		if(vertexFrom && vertexTo)
		{
			edge->setEdgeVertexFrom(vertexFrom);
			edge->setEdgeVertexTo(vertexTo);
		}


		if(!graph.addGraphEdge(edge))
		{
			_errorMessage = "Error adding edge on graph.";
			return false;
		}

		if(TeProgress::instance())
		{
			TeProgress::instance()->setProgress(count);
		}

		++count;
	}

	if(TeProgress::instance())
	{
		TeProgress::instance()->reset();
	}

	return true;
}

bool HidroPersister::saveGraphInDatabase(HidroGraph& graph, TeProjection* proj, const double& res)
{
	if(!graph.isGraphValid())
	{
		_errorMessage = "Graph is not valid to be saved.";
		return false;
	}

	if(_hidroMetadata->existGraph(graph.getGraphName()))
	{
		_errorMessage = "Graph already exist.";
		return false;
	}

//set database objects names
	std::string layerVertexName	= graph.getGraphName() + "_VertexLayer";
	std::string layerEdgeName	= graph.getGraphName() + "_EdgeLayer";
	std::string tableVertexName	= graph.getGraphName() + "_VertexTable";
	std::string tableEdgeName	= graph.getGraphName() + "_EdgeTable";

//creating objects for graph vertex
	TeAttributeList vertexAttrList = this->getVertexAttributeList(graph);
	
	TeTable vertexTable;
	if(!this->createTable(tableVertexName, vertexAttrList, vertexTable, false))
	{
		return false;
	}

	if(!this->createAttributeTable(tableVertexName, vertexAttrList))
	{
		return false;
	}

	TeLayer* vertexLayer = new TeLayer(layerVertexName, _db, proj);
	if(!vertexLayer)
	{
		return false;
	}

	TePointSet ps;

//creating objects for graph edge
	TeAttributeList edgeAttrList = this->getEdgeAttributeList(graph);

	TeTable edgeTable;
	if(!this->createTable(tableEdgeName, edgeAttrList, edgeTable, true))
	{
		return false;
	}

	if(!this->createAttributeTable(tableEdgeName, edgeAttrList))
	{
		return false;
	}

	TeLayer* edgeLayer = new TeLayer(layerEdgeName, _db, proj);
	if(!edgeLayer)
	{
		return false;
	}

	TeLineSet ls;

//saving graph vertex data
	int count = 1;
	int nRec  = 0;

	std::map<std::string, HidroVertex*>::iterator itVertex = graph.getGraphMapVertex().begin();

	if(TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Saving Graph Vertex");
		TeProgress::instance()->setTotalSteps(graph.getGraphMapVertex().size());
	}

	while(itVertex != graph.getGraphMapVertex().end())
	{
		TeTableRow row;

		HidroVertex* vertex = itVertex->second;

//vertex without edge has to be ignored ??
    // No, hasn't to be ignored.
		/*if(vertex->getMapEdgeIn().empty() && vertex->getMapEdgeOut().empty())
		{
			if(TeProgress::instance())
			{
				TeProgress::instance()->setProgress(count);
			}

			++count;
			++itVertex;

			continue;
		}*/

		for(unsigned int j = 0; j < vertex->getVertexVecAttr().size(); ++j)
		{
			HidroAttr* attr = vertex->getVertexVecAttr()[j];

			if(attr->getAttrType() == TeSTRING)
			{
				HidroAttrString* attrString = (HidroAttrString*)vertex->getVertexVecAttr()[j];
				row.push_back(attrString->getAttrValue());
			}
			else if(attr->getAttrType() == TeINT)
			{
				HidroAttrInt* attrInt = (HidroAttrInt*)vertex->getVertexVecAttr()[j];
				row.push_back(Te2String(attrInt->getAttrValue()));
			}
			else if(attr->getAttrType() == TeREAL)
			{
				HidroAttrDouble* attrDouble = (HidroAttrDouble*)vertex->getVertexVecAttr()[j];
				row.push_back(Te2String(attrDouble->getAttrValue()));
			}
		}

//add the attribute for object name
		row.push_back(vertex->getVertexId());
		
//add the attribute for object id
		row.push_back(Te2String(count));

		vertexTable.add(row);

		TePoint pt(vertex->getVertexCoord());

//set the geometry object id
		pt.objectId(Te2String(count));

		ps.add(pt);

		if((nRec % 1000) == 0)
		{
			if(!_db->insertTable(vertexTable))
			{
				return false;
			}
			vertexTable.clear();

			if(!vertexLayer->addPoints(ps))
			{
				return false;
			}
			ps.clear();

			nRec = 0;
		}

		if(TeProgress::instance())
		{
			TeProgress::instance()->setProgress(count);
		}

		++nRec;
		++count;
		++itVertex;
	}

	if(TeProgress::instance())
	{
		TeProgress::instance()->reset();
	}

	if(vertexTable.size() > 0)
	{
		if(!_db->insertTable(vertexTable))
		{
			return false;
		}
		vertexTable.clear();

		if(!vertexLayer->addPoints(ps))
		{
			return false;
		}
		ps.clear();
	}

	if(!this->saveLayer(vertexLayer, vertexTable, TePOINTS))
	{
		return false;
	}

//saving edge graph data
	count = 0;
	nRec  = 0;
	std::map<std::string, HidroEdge*>::iterator itEdge = graph.getGraphMapEdge().begin();

	if(TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Saving Graph Edges");
		TeProgress::instance()->setTotalSteps(graph.getGraphMapEdge().size());
	}

	while(itEdge != graph.getGraphMapEdge().end())
	{
		TeTableRow row;

		HidroEdge* edge = itEdge->second;
		for(unsigned int j = 0; j < edge->getEdgeVecAttr().size(); ++j)
		{
			HidroAttr* attr = edge->getEdgeVecAttr()[j];

			if(attr->getAttrType() == TeSTRING)
			{
				HidroAttrString* attrString = (HidroAttrString*)edge->getEdgeVecAttr()[j];
				row.push_back(attrString->getAttrValue());
			}
			else if(attr->getAttrType() == TeINT)
			{
				HidroAttrInt* attrInt = (HidroAttrInt*)edge->getEdgeVecAttr()[j];
				row.push_back(Te2String(attrInt->getAttrValue()));
			}
			else if(attr->getAttrType() == TeREAL)
			{
				HidroAttrDouble* attrDouble = (HidroAttrDouble*)edge->getEdgeVecAttr()[j];
				row.push_back(Te2String(attrDouble->getAttrValue()));
			}
		}

//add the attribute for vertex from
		row.push_back(edge->getEdgeVertexFrom()->getVertexId());

//add the attribute for vertex from
		row.push_back(edge->getEdgeVertexTo()->getVertexId());

//add the attribute for edge name
		row.push_back(edge->getEdgeId());
		
//add the attribute for object id
		row.push_back(Te2String(count));

		edgeTable.add(row);

		TeLine2D line(edge->getEdgeLine());

//set the geometry object id
		line.objectId(Te2String(count));

		ls.add(line);

		if((nRec % 1000) == 0)
		{
			if(!_db->insertTable(edgeTable))
			{
				return false;
			}
			edgeTable.clear();

			if(!edgeLayer->addLines(ls))
			{
				return false;
			}
			ls.clear();

			nRec = 0;
		}

		if(TeProgress::instance())
		{
			TeProgress::instance()->setProgress(count);
		}

		++nRec;
		++count;
		++itEdge;
	}

	if(TeProgress::instance())
	{
		TeProgress::instance()->reset();
	}

	if(edgeTable.size() > 0)
	{
		if(!_db->insertTable(edgeTable))
		{
			return false;
		}
		edgeTable.clear();

		if(!edgeLayer->addLines(ls))
		{
			return false;
		}
		ls.clear();
	}

	if(!this->saveLayer(edgeLayer, edgeTable, TeLINES))
	{
		return false;
	}

//saving graph entry
	if(!_hidroMetadata->addGraphEntry(graph.getGraphName(), vertexLayer->id(), edgeLayer->id(), res))
	{
		_errorMessage = _hidroMetadata->getErrorMessage();
		return false;
	}

	return true;
}

bool HidroPersister::deleteGraphFromDatabase(const std::string& graphName)
{
	if(!_hidroMetadata->existGraph(graphName))
	{
		_errorMessage = "Graph not exist.";
		return false;
	}

//get layer with lines from graph
	TeLayer* layerLines = _hidroMetadata->getGraphLayerLines(graphName);

	if(layerLines)
	{
		_db->deleteLayer(layerLines->id());
	}

//get layer with points from graph
	TeLayer* layerPoints = _hidroMetadata->getGraphLayerPoints(graphName);

	if(layerPoints)
	{
		_db->deleteLayer(layerPoints->id());
	}

//remove attribute tables
	std::string vertexAttrTableName = _hidroMetadata->getGraphVertexAttrTableName(graphName);
	if(!_db->deleteTable(vertexAttrTableName))
	{
		_errorMessage = _db->errorMessage();
		return false;
	}

	std::string edgeAttrTableName = _hidroMetadata->getGraphEdgeAttrTableName(graphName);
	if(!_db->deleteTable(edgeAttrTableName))
	{
		_errorMessage = _db->errorMessage();
		return false;
	}

//remove graph entry
	if(!_hidroMetadata->removeGraphEntry(graphName))
	{
		_errorMessage = _hidroMetadata->getErrorMessage();
		return false;
	}

	return true;
}

TeView* HidroPersister::createGraphView(HidroGraph* graph, TeProjection* proj)
{
	_errorMessage = "";

//get layer with lines from graph
	TeLayer* layerLines = _hidroMetadata->getGraphLayerLines(graph->getGraphName());

//get layer with points from graph
	TeLayer* layerPoints = _hidroMetadata->getGraphLayerPoints(graph->getGraphName());


	if(!layerLines || !layerPoints)
	{
		_errorMessage = "Error getting graph layers.";
		return NULL;
	}

	TeWaitCursor wait;
	wait.setWaitCursor();

//define object names
	std::string viewName		= graph->getGraphName() + "_view";
	std::string themeVertexName = graph->getGraphName() + "_VertexTheme";
	std::string themeEdgeName	= graph->getGraphName() + "_EdgeTheme";

//create view
	TeView* view = new TeView(viewName, _db->user());
	view->projection(proj);

	if(!_db->insertView(view))
	{
		delete view;

		wait.resetWaitCursor();
		_errorMessage = _db->errorMessage();

		return NULL;
	}

//create theme for vertex layer
	if(!this->createTheme(themeVertexName, "",view, layerPoints))
	{
		_db->deleteView(view->id());
		delete view;

		wait.resetWaitCursor();
		return NULL;
	}

//create theme for edge layer
	if(!this->createTheme(themeEdgeName, "", view, layerLines))
	{
		_db->deleteView(view->id());
		delete view;

		wait.resetWaitCursor();
		return NULL;
	}

	wait.resetWaitCursor();

	return view;
}

TeAttributeList HidroPersister::getVertexAttributeList(HidroGraph& graph)
{
	TeAttributeList attrList;


	if(!graph.getGraphMapVertex().empty())
	{
		HidroVertex* vertex = graph.getGraphMapVertex().begin()->second;

		for(unsigned int i = 0 ; i < vertex->getVertexVecAttr().size(); ++i)
		{
			HidroAttr* hidroAt = vertex->getVertexVecAttr()[i];

			TeAttribute at;
			at.rep_.name_ = hidroAt->getAttrName();
			at.rep_.type_ = hidroAt->getAttrType();
			at.rep_.isPrimaryKey_ = false;

			if(at.rep_.type_ == TeSTRING)
			{
				at.rep_.numChar_ = 100;
			}
			else if(at.rep_.type_ == TeINT)
			{
				at.rep_.isAutoNumber_ = false;
			}

			attrList.push_back(at);
		}
	}

	return attrList;
}

TeAttributeList HidroPersister::getEdgeAttributeList(HidroGraph& graph)
{
	TeAttributeList attrList;

	if(!graph.getGraphMapEdge().empty())
	{
		HidroEdge* edge = graph.getGraphMapEdge().begin()->second;

		for(unsigned int i = 0 ; i < edge->getEdgeVecAttr().size(); ++i)
		{
			HidroAttr* hidroAt = edge->getEdgeVecAttr()[i];

			TeAttribute at;
			at.rep_.name_ = hidroAt->getAttrName();
			at.rep_.type_ = hidroAt->getAttrType();
			at.rep_.isPrimaryKey_ = false;

			if(at.rep_.type_ == TeSTRING)
			{
				at.rep_.numChar_ = 100;
			}
			else if(at.rep_.type_ == TeINT)
			{
				at.rep_.isAutoNumber_ = false;
			}

			attrList.push_back(at);
		}
	}

	return attrList;
}

bool HidroPersister::createAttributeTable(const std::string& tableName, TeAttributeList& attrList)
{
	if(tableName.empty())
	{
		_errorMessage = "Attribute table name is empty.";
		return false;
	}

	if(_db->tableExist(tableName))
	{
		_errorMessage = "Attribute table already exist.";
		return false;
	}

	if(attrList.empty())
	{
		_errorMessage = "Attribute table is empty.";
		return false;
	}

	if(!_db->createTable(tableName, attrList))
	{
		_errorMessage = _db->errorMessage();
		return false;
	}

	return true;
}

bool HidroPersister::createTable(const std::string& tableName, TeAttributeList& attrList, TeTable& table, const bool& edgeTable)
{
	if(tableName.empty())
	{
		_errorMessage = "Attribute table name is empty.";
		return false;
	}

	if(edgeTable)
	{
		TeAttribute atFrom;
		atFrom.rep_.type_ = TeSTRING;
		atFrom.rep_.numChar_ = 100;
		atFrom.rep_.isPrimaryKey_ = false;
		atFrom.rep_.name_ = _hidroMetadata->getEdgeVertexFromAttrName();
		attrList.push_back(atFrom);

		TeAttribute atTo;
		atTo.rep_.type_ = TeSTRING;
		atTo.rep_.numChar_ = 100;
		atTo.rep_.isPrimaryKey_ = false;
		atTo.rep_.name_ = _hidroMetadata->getEdgeVertexToAttrName();
		attrList.push_back(atTo);
	}

	TeAttribute atName;
	atName.rep_.type_ = TeSTRING;
	atName.rep_.numChar_ = 100;
	atName.rep_.isPrimaryKey_ = false;
	if(edgeTable)
	{
		atName.rep_.name_ = _hidroMetadata->getEdgeNameAttrName();
	}
	else
	{
		atName.rep_.name_ = _hidroMetadata->getVertexNameAttrName();
	}

	attrList.push_back(atName);

	TeAttribute at;
	at.rep_.type_ = TeSTRING;
	at.rep_.numChar_ = 16;
	at.rep_.isPrimaryKey_ = true;
	at.rep_.name_ = _hidroMetadata->getObjectIdAttrName();

	attrList.push_back(at);

	table.name(tableName);
	table.setLinkName(_hidroMetadata->getObjectIdAttrName());
	table.setUniqueName(_hidroMetadata->getObjectIdAttrName());
	table.setAttributeList(attrList);  

	_db->validTable(table);
	
	return true;
}

bool HidroPersister::saveLayer(TeLayer* layer, TeTable& table, const TeGeomRep& rep)
{
	if(layer == NULL)
	{
		_errorMessage = "Layer pointer is NULL.";
		return false;
	}

	if(!layer->database()->insertMetadata(layer->tableName(rep),layer->database()->getSpatialIdxColumn(rep), 0.0005,0.0005,layer->box()))
	{
		_errorMessage = _db->errorMessage();
		return false;
	}

	if(!layer->database()->createSpatialIndex(layer->tableName(rep),layer->database()->getSpatialIdxColumn(rep), (TeSpatialIndexType)TeRTREE))
	{
		_errorMessage = _db->errorMessage();
		return false;
	}

	if (!_db->insertTableInfo(layer->id(), table))
	{
		_errorMessage = _db->errorMessage();
		return false;
	}

	layer->addAttributeTable(table);

	return true;
}

bool HidroPersister::createTheme(const std::string& themeName, const std::string& whereClauseIN, TeView* view, TeLayer* layer)
{
	_errorMessage = "";
	
	if(themeName.empty() || !view || !layer)
	{
		_errorMessage = "Error from input parameters.";
		return false;
	}

//create theme
	TeTheme* theme = new TeTheme(themeName, layer, 0, view->id());
	theme->visibleRep(layer->geomRep());
	view->add(theme);

//apply restriction if exist
	if(!whereClauseIN.empty())
	{
		theme->attributeRest(whereClauseIN);
	}

//create theme application
	TeAppTheme* appTheme = new TeAppTheme(theme);
	appTheme->canvasLegUpperLeft(layer->box().center());

	for(unsigned int i = 0; i < layer->attrTables().size(); ++i)
	{
		theme->addThemeTable(layer->attrTables()[i]);
	}

	if(!insertAppTheme(_db, appTheme))
	{
		_errorMessage = "Error inserting theme application.";
		return false;
	}

	if(!theme->save())
	{
		_errorMessage = "Error saving theme.";
		return false;
	}

	if(!theme->buildCollection())
	{
		_errorMessage = "Error building collection.";

		_db->deleteTheme(theme->id());
		return false;
	}

	if(!theme->generateLabelPositions())
	{
		_errorMessage = "Error generate label positions.";
	}

//update theme box
	TeBox themeBox = _db->getThemeBox(theme);
	theme->setThemeBox(themeBox);
	_db->updateTheme(theme);

//update database theme id
	std::string themeId  = Te2String(theme->id());

	std::string	sql	 = "UPDATE te_theme SET parent_id = ";
				sql += themeId;
				sql	+= " WHERE theme_id = ";
				sql	+= themeId;

	if(!_db->execute(sql))
	{
		_errorMessage = _db->errorMessage();
		return false;
	}

	return true;
}

bool HidroPersister::updateGraphAttributes(HidroGraph* inputGraph, HidroGraph* outputGraph)
{
	if(!inputGraph->isGraphValid() || !outputGraph->isGraphValid())
	{
		_errorMessage = "Invalid input paramters.";
		return false;
	}

//get output graph vertex attributes
	HidroVertex* vertex = outputGraph->getGraphMapVertex().begin()->second;

	for(unsigned int i = 0; i < vertex->getVertexVecAttr().size(); ++i)
	{
		std::string attrName = vertex->getVertexVecAttr()[i]->getAttrName();

		if(!copyAttributeEntry(inputGraph, outputGraph, attrName, true))
		{
			return false;
		}

	}

//get output graph edge attributes
	HidroEdge* edge = outputGraph->getGraphMapEdge().begin()->second;

	for(unsigned int i = 0; i < edge->getEdgeVecAttr().size(); ++i)
	{
		std::string attrName = edge->getEdgeVecAttr()[i]->getAttrName();

		if(!copyAttributeEntry(inputGraph, outputGraph, attrName, false))
		{
			return false;
		}
	}

	return true;
}

bool HidroPersister::copyAttributeEntry(HidroGraph* inputGraph, HidroGraph* outputGraph, const std::string& attrName, const bool& isVertexAttr)
{
//get attribute values from input graph
	std::string tableName = "";

	if(isVertexAttr)
	{
		tableName = _hidroMetadata->getGraphVertexAttrTableName(inputGraph->getGraphName());
	}
	else
	{
		tableName = _hidroMetadata->getGraphEdgeAttrTableName(inputGraph->getGraphName());
	}

	std::string sqlSelect = "SELECT * FROM ";
				sqlSelect+= tableName;
				sqlSelect+= " WHERE ";
				sqlSelect+= "attribute";
				sqlSelect+= " = '";
				sqlSelect+= attrName;
				sqlSelect+= "'";

//verify if attribute exist in input attribute table
	TeDatabasePortal* portal = _db->getPortal();

	if(!portal)
	{
		_errorMessage = "Database Portal Internal Error.";
		return false;
	}

	if(!portal->query(sqlSelect) || !portal->fetchRow())
	{
		delete portal;
		return false;
	}

	std::string inputAttrName		= portal->getData(0);
	std::string inputLayerName		= portal->getData(1);
	std::string inputLayerAttrName	= portal->getData(2);

	delete portal;

	if(isVertexAttr)
	{
		if(!_hidroMetadata->addGraphVertexAttrEntry(outputGraph->getGraphName(), inputAttrName, inputLayerName, inputLayerAttrName))
		{
			_errorMessage = _hidroMetadata->getErrorMessage();
			return false;
		}
	}
	else
	{
		if(!_hidroMetadata->addGraphEdgeAttrEntry(outputGraph->getGraphName(), inputAttrName, inputLayerName, inputLayerAttrName))
		{
			_errorMessage = _hidroMetadata->getErrorMessage();
			return false;
		}
	}

	return true;
}