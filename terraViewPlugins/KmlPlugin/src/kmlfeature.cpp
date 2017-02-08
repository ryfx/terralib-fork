
#include "kmlfeature.h"

namespace tdk {

KMLFeature::KMLFeature(const KMLType & objType, KMLObject * parent) :
KMLObject(objType, parent)
{
	_visibility = true;
}

KMLFeature::KMLFeature(const KMLFeature & source) :
KMLObject(source)
{
    _name = source._name;
    _visibility = source._visibility;
    _open = source._open;
    _author = source._author;
    _link = source._link;
    _styleURL = source._styleURL;
}

KMLFeature::~KMLFeature() 
{
}

KMLFeature & KMLFeature::operator=(const KMLFeature & source) 
{
    _name = source._name;
    _visibility = source._visibility;
    _open = source._open;
    _author = source._author;
    _link = source._link;
    _styleURL = source._styleURL;

	return *this;
}

const string KMLFeature::getName() const 
{
  return _name;
}

void KMLFeature::setName(const string & value) 
{
  _name = value;
}

const bool KMLFeature::getVisibility() const 
{
  return _visibility;
}

void KMLFeature::setVisibility(const bool & value) 
{
  _visibility = value;
}

const bool KMLFeature::getOpen() const 
{
  return _open;
}

void KMLFeature::setOpen(const bool & value) 
{
  _open = value;
}

const string KMLFeature::getAuthor() const 
{
  return _author;
}

void KMLFeature::setAuthor(const string & value) 
{
  _author = value;
}

const string KMLFeature::getLink() const 
{
  return _link;
}

void KMLFeature::setLink(const string & value) 
{
  _link = value;
}

const string KMLFeature::getStyleURL() const 
{
  return _styleURL;
}

void KMLFeature::setStyleURL(const string & value) 
{
  _styleURL = value;
}

void KMLFeature::getChildFeatures(const KMLType & featureType, vector<KMLObject*> & features) 
{
	vector<KMLStructureItem<KMLObject>*>::iterator it;

	for(it = _children.begin(); it != _children.end(); ++it)
	{
		KMLObject* child = dynamic_cast<KMLObject*>(*it);
		if(child->getKMLType() == featureType)
			features.push_back(child);
	}
}

const string KMLFeature::getDescription() const 
{
  return _description;
}

void KMLFeature::setDescription(const string & value) 
{
  _description = value;
}


} // namespace tdk
