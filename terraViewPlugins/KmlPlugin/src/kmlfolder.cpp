
#include "kmlfolder.h"
#include "kmlobject.h"
#include "kmlabstractvisitor.h"

namespace tdk {

KMLFolder::KMLFolder(KMLObject * parent) :
KMLFeature(FOLDER, parent)
{
	setKMLType(FOLDER);
}

KMLFolder::~KMLFolder(){
  	//std::vector<KMLGeographicObject*>::iterator it;
 
  	//for(it = _geoObjects.begin(); it != _geoObjects.end(); ++it)
  	//	delete *it;
}

KMLFolder::KMLFolder(const KMLFolder & source) :
KMLFeature(source)
{
}

void KMLFolder::accept(KMLAbstractVisitor & v) 
{
	v.visit(*this);
}


} // namespace tdk
