
#include "kmlplacemark.h"
#include "kmlobject.h"
#include "kmlabstractvisitor.h"
#include "TeGeometry.h"

namespace tdk {

//TDKKML_API
//Deafault constructor.
KMLPlacemark::KMLPlacemark(KMLObject * parent) :
KMLFeature(PLACEMARK, parent)
{
	_geometry = NULL;
}

//Copy constructor.
KMLPlacemark::KMLPlacemark(const KMLPlacemark & source) :
KMLFeature(source)
{
  	if(source._geometry == NULL)
  		return;
  
  	copyGeometry(*source._geometry);
}

//Virtual destructor.

//Virtual destructor.
KMLPlacemark::~KMLPlacemark(){
	if(_geometry != NULL)
		delete _geometry;
}

//Copy operator.

//Copy operator.
KMLPlacemark & KMLPlacemark::operator =(const KMLPlacemark & source) {
  	KMLObject::operator =(source);
  
  	return *this;
}

void KMLPlacemark::addChild(KMLObject * child) {
}

void KMLPlacemark::accept(KMLAbstractVisitor & v) 
{
	v.visit(*this);
}

const TeGeometry & KMLPlacemark::getGeometry() const {
  	return *_geometry;
}

void KMLPlacemark::setGeometry(const TeGeometry & geom) 
{
	TeGeomRep rep = geom.elemType();
	switch(rep)
	{
		case TePOINTS:
			_geomType = POINT;
		break;

		case TeLINES:
			_geomType = LINE;
		break;

		case TePOLYGONS:
			_geomType = POLYGON;
		break;

		default:
			_geomType = UNKNOWN;
		break;
	}

	if(_geometry != NULL)
		delete _geometry;

	copyGeometry(geom);	
}

const KMLPlacemark::KMLGeometryType KMLPlacemark::getGeometryType() const {
  return _geomType;
}

void KMLPlacemark::copyGeometry(const TeGeometry & geom) {
	switch(_geomType)
	{
		case POINT :
			_geometry = new TePoint(((TePoint&)geom).location().x(), ((TePoint&)geom).location().y());
		break;

		case LINE :
			_geometry = new TeLine2D;
			((TeLine2D*)_geometry)->copyElements((const TeLine2D&)geom);
		break;

		case POLYGON :
			_geometry = new TePolygon;
			((TePolygon*)_geometry)->copyElements((const TePolygon&)geom);
		break;

		default:
			_geometry = NULL;
		break;
	}
}


} // namespace tdk
