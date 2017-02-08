#include "kmlobject.h"

namespace tdk {

KMLObject::KMLObject(const KMLType & objType, KMLObject * parent) :
KMLStructureItem<KMLObject>(parent),
VisitReceiver<KMLAbstractVisitor>()
{
	_kmltype = objType;
}

KMLObject::KMLObject(const KMLObject & source) :
KMLStructureItem<KMLObject>(source._parent),
VisitReceiver<KMLAbstractVisitor>()
{
	_id = source._id;
	_targetId = source._targetId;
	_kmltype = source._kmltype;
}

KMLObject::~KMLObject() 
{
}

KMLObject & KMLObject::operator =(const KMLObject & source) 
{
	_id = source._id;
	_targetId = source._targetId;
	_kmltype = source._kmltype;

	return *this;
}

string KMLObject::getId() const 
{
  return _id;
}

void KMLObject::setId(const string & id) 
{
  _id = id;
}

string KMLObject::getTargetId() const 
{
  	return _targetId;
}

void KMLObject::setTargetId(const string & targetId) 
{
  _targetId = targetId;
}

KMLType KMLObject::getKMLType() const 
{
  return _kmltype;
}

void KMLObject::setKMLType(const KMLType & value) 
{
  _kmltype = value;
}

KMLObject* KMLObject::DefaultObject()

{
	return NULL;
}


} // namespace tdk
