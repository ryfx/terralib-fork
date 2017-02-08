
#include "kmlmanager.h"
#include "kmldocument.h"
#include "kmlstylesmanager.h"
#include "kmlstyle.h"
#include "kmlimporter.h"
#include "kmlreader.h"
#include "kmlwriter.h"
#include "TeXMLEncoder.h"
#include "TeXMLEncoderFactory.h"

namespace tdk {

KMLManager::KMLManager() :
_stylesMger(NULL)
{
}

KMLManager::~KMLManager() 
{
	map<string, KMLFeature*>::iterator it;

	for(it = _feats.begin(); it != _feats.end(); ++it)
		delete it->second;

	if(_stylesMger != NULL)
		delete _stylesMger;
}

//TDKKML_API
map<string, KMLFeature*> KMLManager::getObjects() 
{
	return _feats;
}

KMLStylesManager * KMLManager::getStylesManager() const 
{
  return _stylesMger;
}

vector<const KMLStyle*> KMLManager::foundStyle(const string & styleId) 
{
	return _stylesMger->getStyles(styleId);
}

void KMLManager::parse(const string & kmlFileName) 
{
	try
	{
		KMLReader reader;
		reader.parse(kmlFileName);

		_hasMultiPolygon = reader.hasMultiPolygon();

		_feats = reader.getObjects();
		_stylesMger = reader.getStylesManager();
	}
	catch(...)
	{

	}
}

bool KMLManager::hasMultiPolygon()
{
	return _hasMultiPolygon;
}

void KMLManager::saveKML(const vector<KMLFeature*> & feats, const string & kml) 
{
	TeOGC::TeXMLEncoder* encod = TeOGC::TeXMLEncoderFactory::make();

	KMLWriter w;
	w.setDocuments(feats);

	w.write(*encod);
	encod->writeToFile(kml);

	delete encod;
}

map<string, KMLFeature*> KMLManager::getFeatures()
{
	return _feats;
}


} // namespace tdk
