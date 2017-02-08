#include <HidroUpscale.h>

#include <HidroGraph.h>
#include <HidroVertex.h>
#include <HidroEdge.h>
#include <HidroAttrInt.h>
#include <HidroPersister.h>
#include <HidroUtils.h>
#include <HidroFlowUtils.h>

#include <TeDatabase.h>
#include <TeGeometry.h>
#include <TeOverlay.h>
#include <TeRTree.h>
#include <TeGeometryAlgorithms.h>

HidroUpscale::HidroUpscale(TeDatabase* db) : _db(db), _outputGraph(NULL), _errorMessage("")
{
	_attributeGraphDeep = "attributeGraphDeep__";
	_attributeSubGraphDeep = "attributeSubGraphDeep__";
}

HidroUpscale::~HidroUpscale()
{
	if(_outputGraph)
	{
		delete _outputGraph;
	}
}

bool HidroUpscale::generateUpscale(const std::string& inputGraphName, const std::string& gridLayerName, const int& COTAT, const int& MUFP)
{
//verify input parameters
	_errorMessage = "";

	if(inputGraphName.empty())
	{
		_errorMessage = "Input graph name is empty.";
		return false;
	}

	if(gridLayerName.empty())
	{
		_errorMessage = "Input grid layer name is empty.";
		return false;
	}

//get parameters
	HidroUtils utils(_db);

	TeLayer* inputLayerGrid = utils.getLayerByName(gridLayerName);

	if(!inputLayerGrid)
	{
		_errorMessage = "Grid layer not found.";
		return false;
	}


	HidroPersister persister(_db);

	HidroGraph inputGraph;

	if(!persister.loadGraphFromDatabase(inputGraphName, inputGraph))
	{
		_errorMessage = "Graph not found.";
		return false;
	}

//insert deep attribute into memory graph
	if(!calculateDeepAttribute(inputGraph, _attributeGraphDeep, true))
	{
		_errorMessage = "Error adding Deep Attribute into Graph.";
		return false;
	}

//create RTree
	TeSAM::TeRTree<HidroVertex*> pointTree(inputGraph.getGrahBox());

	std::map<std::string, HidroVertex*>::iterator it = inputGraph.getGraphMapVertex().begin();

	while(it != inputGraph.getGraphMapVertex().end())
	{
		TeBox box(it->second->getVertexCoord(), it->second->getVertexCoord());
		pointTree.insert(box, it->second);

		++it;
	}

//get grid that intersects the graph and input grid
	if(!getIntersectGrid(inputLayerGrid->raster(), inputGraph))
	{
		_errorMessage = "Intersection null.";
		return false;
	}

	TeRaster* rasterGrid = inputLayerGrid->raster();

	TeRasterParams params = rasterGrid->params();
	params.mode_ = 'c';
	params.decoderIdentifier_ = "SMARTMEM";

	TeRaster* rasterOut = new TeRaster(params);

	if(!rasterOut->init())
	{
		_errorMessage = "Error creating output raster DEM";
		return false;
	}


//get the box for each pixel in the raster intersection
	int nLines = rasterGrid->params().nlines_;
	int nCols = rasterGrid->params().ncols_;

	if(TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Upscaling");
		TeProgress::instance()->setTotalSteps(nLines);
	}

	for(int i = 0; i < nLines; ++i)
	{
		for(int j = 0; j < nCols; ++j)
		{
			TeBox cellBox = getCellBox(rasterGrid, i, j);

			std::vector<HidroVertex*> vecVertex;
			
			double precision = TePrecision::instance().precision();
			TePrecision::instance().setPrecision(0.);
			pointTree.search(cellBox, vecVertex);
			TePrecision::instance().setPrecision(precision);

			std::map<std::string, HidroEdge*> edgeOutMap;

			HidroGraph* subGraph = getSubGraph(vecVertex, edgeOutMap);

			if(subGraph->isGraphValid())
			{
				//insert deep attribute into memory sub graph
				if(!calculateDeepAttribute(*subGraph, _attributeSubGraphDeep, false))
				{
					_errorMessage = "Error adding Deep Attribute into Sub Graph.";
					
					delete subGraph;
					delete rasterGrid;

					return false;
				}

				if(!edgeOutMap.empty())
				{
					//find the vertex with bigger deep attribute value  - 1 STEP
					std::string vertexToId = findOutputVertex(subGraph, edgeOutMap, MUFP);

					// find neighbour cell - 2 STEP
					if(!vertexToId.empty())
					{
						HidroVertex* vertexTo = inputGraph.getGraphVertex(vertexToId);
						
						if(vertexTo)
						{
							bool found = false;

							int cotatVal = COTAT;

							while(!found && cotatVal >= 0)
							{
								int lddValue = getCellDirection(vertexTo, rasterGrid, j, i, cotatVal);

								//set ldd value in current position
								if(lddValue != -1)
								{
									rasterOut->setElement(j, i, lddValue);
									found = true;
								}

								cotatVal = cotatVal -1;
							}

							if(!found)
							{
								rasterOut->setElement(j, i, 0.);
							}
						}
					}
					else
					{
						rasterOut->setElement(j, i, 0.);
					}
				}
				else
				{
					rasterOut->setElement(j, i, 0.);
				}
			}

			delete subGraph;
		}

		if(TeProgress::instance())
		{
			TeProgress::instance()->setProgress(i);
		}
	}

	if(TeProgress::instance())
	{
		TeProgress::instance()->reset();
	}


//save graph
	_outputGraph = new HidroGraph("hidroUpscaleGraph");

	if(!utils.generateGraph(rasterOut, *_outputGraph, true))
	{
		delete rasterOut;
		return false;
	}

	delete rasterOut;

// check cross edges - 3 STEP
	if(!checkCrossEdges(_outputGraph))
	{
		delete _outputGraph;
		_outputGraph = NULL;
		
		return false;
	}

	return true;
}

HidroGraph* HidroUpscale::getOutputGraph()
{
	return _outputGraph;
}

std::string HidroUpscale::errorMessage()
{
	return _errorMessage;
}

bool HidroUpscale::calculateDeepAttribute(HidroGraph& graph, const std::string& attrName, const bool& showProgress)
{
	int nVertex = graph.getGraphMapVertex().size();

	int count = 0;

	if(showProgress && TeProgress::instance())
	{
		TeProgress::instance()->setCaption("TerraHidro");
		TeProgress::instance()->setMessage("Calculate Deep Attribute");
		TeProgress::instance()->setTotalSteps(nVertex);
	}

	std::map<std::string, HidroVertex*>::iterator it = graph.getGraphMapVertex().begin();

	while(it != graph.getGraphMapVertex().end())
	{
		HidroVertex* vertex = it->second;

		std::string vId = vertex->getVertexId();

		int deepValue = 0;

		std::set<std::string> vertexIdSet;

		vertexIdSet.insert(vId);

		calculateDeepValue(vertex->getVertexId(), &graph, deepValue, vertexIdSet);

		HidroAttrInt* attrInt = new HidroAttrInt(attrName, deepValue);

		if(!vertex->addVertexAttr(attrInt))
		{
			return false;
		}

		if(showProgress && TeProgress::instance())
		{
			TeProgress::instance()->setProgress(count);
		}

		++count;

		++it;
	}

	if(showProgress && TeProgress::instance())
	{
		TeProgress::instance()->reset();
	}
	
	return true;
}

void HidroUpscale::calculateDeepValue(const std::string& vertexId, HidroGraph* graph, int& deepValue, std::set<std::string>& vertexIdSet)
{
	HidroVertex* vertex = graph->getGraphVertex(vertexId);

	if(vertex && !vertex->getMapEdgeIn().empty())
	{
		deepValue += vertex->getMapEdgeIn().size();

		std::map<std::string, HidroEdge*>::iterator it =  vertex->getMapEdgeIn().begin();

		while(it != vertex->getMapEdgeIn().end())
		{
			HidroEdge* edge = it->second;

			HidroVertex* vertexFrom = edge->getEdgeVertexFrom();

			std::string vId = vertexFrom->getVertexId();

			std::set<std::string>::iterator itSet = vertexIdSet.find(vId);

			if(itSet == vertexIdSet.end())
			{
				vertexIdSet.insert(vId);

				calculateDeepValue(vertexFrom->getVertexId(), graph, deepValue, vertexIdSet);
			}

			++it;
		}
	}
}

bool HidroUpscale::getIntersectGrid(TeRaster* inputGrid, HidroGraph& inputGraph)
{
	_errorMessage = "";

//check input parameters
	if(!inputGrid)
	{
		_errorMessage = "Input grid not valid.";
		return NULL;
	}

	if(!inputGraph.isGraphValid())
	{
		_errorMessage = "Input graph is not valid.";
		return NULL;
	}

	return TeIntersects(inputGrid->box(), inputGraph.getGrahBox());
}

TeBox HidroUpscale::getCellBox(TeRaster* raster, const int& lineCur, const int& colCur)
{
	TeBox box = TeBox();

	if(raster)
	{
		if(lineCur >= 0 && colCur >= 0 && lineCur < raster->params().nlines_ && colCur < raster->params().ncols_)
		{
			TeCoord2D upperLeftCoord(raster->params().boundingBox().x1(), raster->params().boundingBox().y2());

			box.x1_ = upperLeftCoord.x() + colCur * raster->params().resx_;
			box.x2_ = upperLeftCoord.x() + ((colCur + 1) * raster->params().resx_);
			box.y1_ = upperLeftCoord.y() - ((lineCur + 1) * raster->params().resy_);
			box.y2_ = upperLeftCoord.y() - lineCur * raster->params().resy_;
		}
	}

	return box;
}

HidroGraph* HidroUpscale::getSubGraph(const std::vector<HidroVertex*>& vecVertex, std::map<std::string, HidroEdge*>& edgeOutMap)
{
	std::map<std::string, HidroVertex*> subGraphVertexMap;

	for(unsigned int i = 0; i < vecVertex.size(); ++ i)
	{
		subGraphVertexMap.insert(std::map<std::string, HidroVertex*>::value_type(vecVertex[i]->getVertexId(), vecVertex[i]));
	}

	std::string graphName = "cellSubGraph";

	HidroGraph* subGraph = new HidroGraph(graphName);

	for(unsigned int i = 0; i < vecVertex.size(); ++ i)
	{
		//add vertex from
		HidroVertex* subVertexFrom = subGraph->getGraphVertex(vecVertex[i]->getVertexId());

		if(!subVertexFrom)
		{
			subVertexFrom = new HidroVertex(vecVertex[i]->getVertexId(), vecVertex[i]->getVertexCoord());

			HidroAttrInt* inputAttrFrom = (HidroAttrInt*)vecVertex[i]->getVertexAttr(_attributeGraphDeep);
			HidroAttrInt* subAttrIntFrom = new HidroAttrInt(_attributeGraphDeep, inputAttrFrom->getAttrValue());

			subVertexFrom->addVertexAttr(subAttrIntFrom);

			subGraph->addGraphVertex(subVertexFrom);

		}
		//add edges that come out from this vertex
		std::map<std::string, HidroEdge*>::iterator it = vecVertex[i]->getMapEdgeOut().begin();

		while(it != vecVertex[i]->getMapEdgeOut().end())
		{
			HidroEdge* edge = it->second;

			std::string vertexToId = edge->getEdgeVertexTo()->getVertexId();

			std::map<std::string, HidroVertex*>::iterator itVertex = subGraphVertexMap.find(vertexToId);

			if(itVertex != subGraphVertexMap.end())
			{
				//vertex to inside sub graph

				HidroVertex* vertexTo = edge->getEdgeVertexTo();

				//add vertex To
				HidroVertex* subVertexTo = subGraph->getGraphVertex(vertexTo->getVertexId());

				if(!subVertexTo)
				{
					subVertexTo = new HidroVertex(vertexTo->getVertexId(), vertexTo->getVertexCoord());

					HidroAttrInt* inputAttrTo = (HidroAttrInt*)vertexTo->getVertexAttr(_attributeGraphDeep);

					HidroAttrInt* subAttrIntTo = new HidroAttrInt(_attributeGraphDeep, inputAttrTo->getAttrValue());

					subVertexTo->addVertexAttr(subAttrIntTo);

					subGraph->addGraphVertex(subVertexTo);
				}
				
				//add edge
				HidroEdge* subEdge = new HidroEdge(edge->getEdgeId());
				subEdge->setEdgeVertexFrom(subVertexFrom);
				subEdge->setEdgeVertexTo(subVertexTo);

				subGraph->addGraphEdge(subEdge);
			}
			else
			{
				//edge come out to sub graph
				edgeOutMap.insert(std::map<std::string, HidroEdge*>::value_type(edge->getEdgeId(), edge));
			}

			++it;
		}
	}

	return subGraph;
}

std::string HidroUpscale::findOutputVertex(HidroGraph* subGraph, std::map<std::string, HidroEdge*>& edgeOutMap, const int& MUFP)
{
	std::map<std::string, HidroEdge*>::iterator it = edgeOutMap.begin();

	std::vector< DeepEdgeVertex > deepEdgeVecOut;

	while(it != edgeOutMap.end())
	{
		HidroAttrInt* attr = (HidroAttrInt*)it->second->getEdgeVertexTo()->getVertexAttr(_attributeGraphDeep);

		DeepEdgeVertex dev;
		dev.vertexFromId = it->second->getEdgeVertexFrom()->getVertexId();
		dev.vertexToId = it->second->getEdgeVertexTo()->getVertexId();
		dev.deepValue = attr->getAttrValue();

		deepEdgeVecOut.push_back(dev);

		++it;
	}

	sort(deepEdgeVecOut.begin(), deepEdgeVecOut.end(), OrderByVal());

	////////////////////////////////////////////////////////////////////////////////////////
	// REFAZER DEPOIS
	for(int i = deepEdgeVecOut.size() - 1; i >= 0; --i)
	{
		HidroVertex* vf = subGraph->getGraphVertex(deepEdgeVecOut[i].vertexFromId);

		double distValue = 0.;

		getDistValue(vf, distValue, MUFP);
		
		if((int)distValue >= MUFP)
		{
			return deepEdgeVecOut[i].vertexToId;
		}

	//	HidroAttrInt* attr = (HidroAttrInt*)vf->getVertexAttr(_attributeSubGraphDeep);
	//	int subDeepVal = attr->getAttrValue();

	////NAO UTILIZAR SUBDEEPATTRIBUTE E SIM O TAMANHO DO CAMINHO DENTRO DO SUBGRAPH
	//	if(subDeepVal >= MUFP)
	//	{
	//		return deepEdgeVecOut[i].vertexToId;
	//	}
	}

	return "";
}

void HidroUpscale::getDistValue(HidroVertex* vertex, double& dist, const int& MUFP)
{
	if((int)dist >= MUFP)
	{
		return;
	}

	std::map<std::string, HidroEdge*> mapEdgeIn = vertex->getMapEdgeIn();

	std::map<std::string, HidroEdge*>::iterator it = mapEdgeIn.begin();

	HidroVertex* vFrom = NULL;
	int deepValue = -1;

	while(it != mapEdgeIn.end())
	{
		HidroVertex * v = it->second->getEdgeVertexFrom();

		HidroAttrInt* attr = (HidroAttrInt*)v->getVertexAttr(_attributeSubGraphDeep);
		if(attr->getAttrValue() > deepValue)
		{
			deepValue = attr->getAttrValue();
			vFrom = v;
		}

		++it;
	}
	
	if(vFrom)
	{
		dist += calculateEdgeDistValue(vertex, vFrom);

		getDistValue(vFrom, dist, MUFP);
	}

}

double HidroUpscale::calculateEdgeDistValue(HidroVertex* vA, HidroVertex* vB)
{
	double dist = 0.;

	if(vA && vB)
	{
		TeCoord2D cA = vA->getVertexCoord();
		TeCoord2D cB = vB->getVertexCoord();

		double thetaVal = theta(cA, cB);
		
		if(thetaVal == 0.0 || thetaVal == 90.0 || thetaVal == 180.0 || thetaVal == 270.0)
		{
			dist = 1.0;
		}
		else
		{
			dist = 1.4;
		}
	}

	return dist;
}

double HidroUpscale::theta(const TeCoord2D& c1, const TeCoord2D& c2)
{
	register double dx = c2.x() - c1.x();
	register double ax = fabs(dx);
	register double dy = c2.y() - c1.y();
	register double ay = fabs(dy);
	register double t = 0.0;

	if((dx == 0.0) && (dy == 0.0))
		t = 0.0;
	else
		t = dy / (ax + ay);

	if(dx < 0.0)
		t = 2 - t;
	else
		if(dy < 0.0)
			t = 4.0 + t;

	return t * 90.0;
}

int HidroUpscale::getCellDirection(HidroVertex* vertex, TeRaster* raster, const int& colCur, const int& lineCur, const int& COTAT)
{
	int val = -1;

	HidroAttrInt* attrFrom = (HidroAttrInt*)vertex->getVertexAttr(_attributeGraphDeep);
	int deepValFrom = attrFrom->getAttrValue();

	while (val < COTAT)
	{
		if(vertex->getMapEdgeOut().empty())
		{
			if(COTAT == 0)
			{
				break;
			}
			else
			{
				return -1;
			}
		}

		HidroVertex* vertexTo = vertex->getMapEdgeOut().begin()->second->getEdgeVertexTo();

		HidroAttrInt* attrTo = (HidroAttrInt*)vertexTo->getVertexAttr(_attributeGraphDeep);
		int deepValTo = attrTo->getAttrValue();

		val = deepValTo - deepValFrom;

		vertex = vertexTo;
	}

	TeBox boxCoordDest = TeBox(vertex->getVertexCoord(), vertex->getVertexCoord());

	bool found		= false;
	int lddValue	= 0;
	
	//verify right cell
	TeBox cellBoxRight = getCellBox(raster, lineCur, colCur + 1);

	if(cellBoxRight.isValid() && !found)
	{
		if(TeWithin(boxCoordDest, cellBoxRight))
		{
			found = true;
			lddValue = 1;
		}
	}

	//verify lower right cell
	TeBox cellBoxLowerRight = getCellBox(raster, lineCur + 1, colCur + 1);

	if(cellBoxLowerRight.isValid() && !found)
	{
		if(TeWithin(boxCoordDest, cellBoxLowerRight))
		{
			found = true;
			lddValue = 2;
		}
	}

	//verify lower cell
	TeBox cellBoxLower = getCellBox(raster, lineCur + 1, colCur);

	if(cellBoxLower.isValid() && !found)
	{
		if(TeWithin(boxCoordDest, cellBoxLower))
		{
			found = true;
			lddValue = 4;
		}
	}

	//verify lower left cell
	TeBox cellBoxLowerLeft	= getCellBox(raster, lineCur + 1, colCur - 1);

	if(cellBoxLowerLeft.isValid() && !found)
	{
		if(TeWithin(boxCoordDest, cellBoxLowerLeft))
		{
			found = true;
			lddValue = 8;
		}
	}

	//verify left cell
	TeBox cellBoxLeft		= getCellBox(raster, lineCur, colCur - 1);

	if(cellBoxLeft.isValid() && !found)
	{
		if(TeWithin(boxCoordDest, cellBoxLeft))
		{
			found = true;
			lddValue = 16;
		}
	}

	//verify upper left cell
	TeBox cellBoxUpperLeft	= getCellBox(raster, lineCur - 1, colCur - 1);

	if(cellBoxUpperLeft.isValid() && !found)
	{
		if(TeWithin(boxCoordDest, cellBoxUpperLeft))
		{
			found = true;
			lddValue = 32;
		}
	}

	//veirfy upper cell
	TeBox cellBoxUpper		= getCellBox(raster, lineCur - 1, colCur);

	if(cellBoxUpper.isValid() && !found)
	{
		if(TeWithin(boxCoordDest, cellBoxUpper))
		{
			found = true;
			lddValue = 64;
		}
	}
	
	//verify upper right cell
	TeBox cellBoxUpperRight = getCellBox(raster, lineCur - 1, colCur + 1);

	if(cellBoxUpperRight.isValid() && !found)
	{
		if(TeWithin(boxCoordDest, cellBoxUpperRight))
		{
			found = true;
			lddValue = 128;
		}
	}


	if(found)
	{
		return lddValue;
	}

	return -1;
}

bool HidroUpscale::checkCrossEdges(HidroGraph* graph)
{
	_errorMessage = "";

	if(!graph->isGraphValid())
	{
		_errorMessage = "Graph is not valid.";
		return false;
	}

	//insert deep attribute into memory graph
	if(!calculateDeepAttribute(*graph, _attributeGraphDeep, true))
	{
		_errorMessage = "Error adding Deep Attribute into Graph.";
		return false;
	}

//create RTree
	TeSAM::TeRTree<HidroEdge*> edgeTree(graph->getGrahBox());

	std::map<std::string, HidroEdge*>::iterator it = graph->getGraphMapEdge().begin();

	while(it != graph->getGraphMapEdge().end())
	{
		TeBox box = it->second->getEdgeLine().box();
		edgeTree.insert(box, it->second);

		++it;
	}

//find for cross edges
	it = graph->getGraphMapEdge().begin();

	while(it != graph->getGraphMapEdge().end())
	{
		TeBox box = it->second->getEdgeLine().box();

		std::vector<HidroEdge*> vecEdge;
	
		double precision = TePrecision::instance().precision();
		TePrecision::instance().setPrecision(0.);
		edgeTree.search(box, vecEdge);
		TePrecision::instance().setPrecision(precision);

//verify intersection
		if(!vecEdge.empty())
		{
			TeLine2D lineA = it->second->getEdgeLine();

			for(unsigned int i = 0; i < vecEdge.size(); ++i)
			{
				TeLine2D lineB = vecEdge[i]->getEdgeLine();

				if(TeCrosses(lineA, lineB))
				{
					std::string idLineA = it->second->getEdgeId();
					std::string idLineB = vecEdge[i]->getEdgeId();

					HidroEdge* edgeA = graph->getGraphEdge(idLineA);
					HidroEdge* edgeB = graph->getGraphEdge(idLineB);

					if(edgeA && edgeB)
					{
						HidroVertex* vertexToA = edgeA->getEdgeVertexTo();
						HidroVertex* vertexToB = edgeB->getEdgeVertexTo();

						if(vertexToA && vertexToB)
						{
							HidroAttrInt* attrA = (HidroAttrInt*)vertexToA->getVertexAttr(_attributeGraphDeep);
							HidroAttrInt* attrB = (HidroAttrInt*)vertexToB->getVertexAttr(_attributeGraphDeep);

							if(attrA && attrB)
							{
								int valA = attrA->getAttrValue();
								int valB = attrB->getAttrValue();

								if(valA >= valB)
								{
									fixCrossEdge(edgeB, vertexToA);
								}
								else
								{
									fixCrossEdge(edgeA, vertexToB);
								}

								break;
							}
						}
					}
				}
			}
		}

		++it;
	}

	return true;
}

void HidroUpscale::fixCrossEdge(HidroEdge* edge, HidroVertex* vertex)
{
	if(edge && vertex)
	{
		//set edge id
		std::string edgeName = "edge_" + edge->getEdgeVertexFrom()->getVertexId() + "_" + vertex->getVertexId();
		edge->setEdgeId(edgeName);

		//set edge vertex to
		edge->setEdgeVertexTo(vertex);
	}
}