#include "TeDXFDriver.h"
#include "TeProjection.h"
#include "TeSTElementSet.h"
#include "tedxflayer.h"
#include "tedxfvisitor.h"

TeDXFDriver::TeDXFDriver(const string & fileName) :
TeGeoDataDriver()
{
	fileName_ = fileName;
	numObjs_ = 0;
}

TeDXFDriver::~TeDXFDriver() 
{
}

std::string TeDXFDriver::getFileName() 
{
	return fileName_;
}

bool TeDXFDriver::isDataAccessible() 
{
	return TeCheckFileExistence(fileName_);
}

TeProjection * TeDXFDriver::getDataProjection() 
{
	return NULL;
}

bool TeDXFDriver::getDataInfo(unsigned int & nObjects, TeBox & ext, TeGeomRep & repres) 
{
	nObjects = numObjs_;
	ext = box_;

	repres = geomType_;

	return true;
}

bool TeDXFDriver::getDataAttributesList(TeAttributeList & /*attList*/) 
{
	return true;
}

bool TeDXFDriver::loadData(TeSTElementSet * dataSet) 
{
    TeDXFVisitor* visitor = new TeDXFVisitor(fileName_);
	vector<TeDXFLayer*> layers = visitor->getLayers();
	vector<TeDXFLayer*>::iterator it;

	delete visitor;

	for(it = layers.begin(); it != layers.end(); ++it)
	{
		TeDXFLayer* layer = *it;

		if(!layer->getPoints().empty())
		{
			TePointSet pts = layer->getPoints();
			TePointSet::iterator pIt;
			
			for(pIt = pts.begin(); pIt != pts.end(); ++pIt)
			{
				TeSTInstance obj;
				obj.addGeometry(*pIt);
				dataSet->insertSTInstance(obj);
			}
			updateBox(box_, pts.box());
			numObjs_ += pts.size();
		}
		if(!layer->getLines().empty())
		{
			TeLineSet ls = layer->getLines();
			TeLineSet::iterator lIt;
			
			for(lIt = ls.begin(); lIt != ls.end(); ++lIt)
			{
				TeSTInstance obj;
				obj.addGeometry(*lIt);
				dataSet->insertSTInstance(obj);
			}
			updateBox(box_, ls.box());
			numObjs_ += ls.size();
		}
		if(!layer->getPolygons().empty())
		{
			TePolygonSet pts = layer->getPolygons();
			TePolygonSet ::iterator pIt;
			
			for(pIt = pts.begin(); pIt != pts.end(); ++pIt)
			{
				TeSTInstance obj;
				obj.addGeometry(*pIt);
				dataSet->insertSTInstance(obj);
			}
			updateBox(box_, pts.box());
			numObjs_ += pts.size();
		}
		delete layer;
	}

	return true;
}

