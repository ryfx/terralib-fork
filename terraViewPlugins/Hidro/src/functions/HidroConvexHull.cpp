#include <HidroConvexHull.h>
#include <HidroGraph.h>
#include <HidroVertex.h>
#include <HidroEdge.h>
#include <HidroMetadata.h>
#include <HidroPersister.h>
#include <HidroUtils.h>

#include <TeDatabase.h>
#include <TeLayer.h>
#include <TeGeometryAlgorithms.h>
#include <TeBufferRegion.h>
#include <TeOverlay.h>

#include <TePDIRaster2Vector.hpp>
#include <TePDIParameters.hpp>

HidroConvexHull::HidroConvexHull(TeDatabase* database) : 
_database(database), _errorMessage("")
{
}

HidroConvexHull::~HidroConvexHull()
{
	_errorMessage = "";
}

TeLayer* HidroConvexHull::generateConvexHull(HidroGraph* graph, const double& bufferDistance)
{
	if(!graph->isGraphValid())
	{
		_errorMessage = "Graph is not valid.";
		return NULL;
	}

	std::map<std::string, HidroEdge*>::iterator it = graph->getGraphMapEdge().begin();

	TeLineSet lineSet;
	while(it != graph->getGraphMapEdge().end())
	{
		lineSet.add(it->second->getEdgeLine());

		++it;
	}

	HidroMetadata metadata(_database);

	TeProjection* proj = metadata.getGraphProjection(graph->getGraphName());

	int nPoints = 9;

	std::vector<TePolygonSet> outputVecPolSet;

	TePrecision::instance().setPrecision(TeGetPrecision(proj));

	if(!TeBUFFERREGION::TeBufferRegion(lineSet, bufferDistance, nPoints, 1, outputVecPolSet))
	{
		_errorMessage = "Error generating buffer region.";
		return false;
	}

	if(outputVecPolSet.empty())
	{
		_errorMessage = "Error generating buffer region.";
		return false;
	}

	TePolygonSet inputPolSet = outputVecPolSet[0];
	TePolygonSet outputPolSet;

	std::string layerName = graph->getGraphName() + "_Buffer";

	TeLayer* outputLayer = new TeLayer(layerName, _database, proj);
	
	if(!createOutputLayer(outputLayer, inputPolSet))
	{
		return NULL;
	}

	return outputLayer;
}

TeLayer* HidroConvexHull::generateMask(HidroGraph* graph, TeRaster* raster, const std::string& layerName)
{
	if(!graph->isGraphValid())
	{
		_errorMessage = "Graph is not valid.";
		return NULL;
	}

	TeRasterParams params = raster->params();
	params.decoderIdentifier_ = "SMARTMEM";
	params.setDummy(255, 0);
	params.useDummy_ = true;
	params.setDataType(TeUNSIGNEDCHAR);
	params.mode_ = 'c';
	params.nBands(1);

	TeRaster* rasterOut = new TeRaster(params);

	if(!rasterOut->init())
	{
		return NULL;
	}

	std::map<std::string, HidroVertex*> mapVertex = graph->getGraphMapVertex();

	std::map<std::string, HidroVertex*>::iterator it = mapVertex.begin();

	while(it != mapVertex.end())
	{
		TeCoord2D coord = it->second->getVertexCoord();

		TeCoord2D coordMatrix = rasterOut->coord2Index(coord);

		rasterOut->setElement( TeRound(coordMatrix.x()), TeRound(coordMatrix.y()), 0, 0);

		++it;
	}

	std::string layerMaskName = layerName + "_Mask";

	HidroUtils utils(_database);

	if(!utils.saveOutputRaster(layerMaskName, rasterOut))
	{
		delete rasterOut;
		return NULL;
	}

	return utils.getLayerByName(layerMaskName);
}

TeLayer* HidroConvexHull::generatePols(TeRaster* raster, const std::string& layerName)
{
	//////////////////////////////////////////////////////////////////////////////////// PDI
	TePDITypes::TePDIRasterPtrType inputRaster(raster, true);
	TePDITypes::TePDIPolSetMapPtrType outputPolSet( new TePDITypes::TePDIPolSetMapType);

	TePDIParameters params;
	params.SetParameter("rotulated_image", inputRaster);
	params.SetParameter("channel", (unsigned int)0);
	params.SetParameter("output_polsets", outputPolSet);

	TePDIRaster2Vector raster2Vector;

	if(!raster2Vector.Reset(params))
	{
		return NULL;
	}

	if(!raster2Vector.Apply())
	{
		return NULL;
	}

	TePolygonSet inputPolSet;

	TePDITypes::TePDIPolSetMapType::iterator it = outputPolSet->begin();
	TePDITypes::TePDIPolSetMapType::iterator itEnd = outputPolSet->end();

	int count = 0;

	while(it != itEnd)
	{
		TePolygonSet ps = it->second;
		
		TePolygonSet::iterator itPol = ps.begin();
		TePolygonSet::iterator itPolEnd = ps.end();

		while(itPol != itPolEnd)
		{
			TePolygon poly;
			poly.copyElements(*itPol);
			poly.objectId(Te2String(count));

			inputPolSet.add(poly);

			++count;
			++itPol;
		}

		++it;
	}
	//////////////////////////////////////////////////////////////////////////////////// /PDI

	TeProjection* proj = TeProjectionFactory::make(raster->projection()->params());

	std::string layerPolsName = layerName + "_Pols";

	TeLayer* outputLayer = new TeLayer(layerName, _database, proj);

	if(!outputLayer)
	{
		return NULL;
	}

	if(!createOutputLayer(outputLayer, inputPolSet))
	{
		return NULL;
	}

	return outputLayer;
}

std::string HidroConvexHull::errorMessage()
{
	return _errorMessage;
}

bool HidroConvexHull::createOutputLayer(TeLayer* layer, TePolygonSet& polSet)
{
	if(polSet.empty() /*|| polSet.size() != 1u */)
	{
		_errorMessage = "Invalid Polygon Set.";
		return false;
	}

	if(layer == NULL)
	{
		_errorMessage = "Invalid layer.";
		return false;
	}

	std::string attrTableName = layer->name() + "_attrTable";

	TeTable table;
	if(!createTable(attrTableName, table))
	{
		return false;
	}

	if(!layer->addPolygons(polSet))
	{
		return false;
	}

	int geomId = polSet[0].geomId();

	TeTableRow row;
	row.push_back(Te2String(geomId));

	table.add(row);

	if(!_database->insertTable(table))
	{
		_errorMessage = _database->errorMessage();
		return false;
	}

	HidroPersister persister(_database);
	if(!persister.saveLayer(layer, table, TePOLYGONS))
	{
		return false;
	}

	return true;
}

bool HidroConvexHull::createTable(const std::string& tableName, TeTable& table)
{
	if(tableName.empty())
	{
		_errorMessage = "Attribute table name is empty.";
		return false;
	}

	TeAttribute at;
	at.rep_.type_ = TeSTRING;
	at.rep_.numChar_ = 16;
	at.rep_.isPrimaryKey_ = true;
	at.rep_.name_ = "linkColumn";

	TeAttributeList attrList;

	attrList.push_back(at);

	table.name(tableName);
	table.setAttributeList(attrList);  
	table.setLinkName("linkColumn");
	table.setUniqueName("linkColumn");

	_database->validTable(table);

	if(_database->tableExist(tableName))
	{
		_errorMessage = "Attribute table already exist.";
		return false;
	}

	if(!_database->createTable(tableName, attrList))
	{
		_errorMessage = _database->errorMessage();
		return false;
	}
	
	return true;
}
