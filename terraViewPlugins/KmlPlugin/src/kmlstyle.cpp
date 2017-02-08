#include "kmlstyle.h"
#include "kmlabstractvisitor.h"

namespace tdk {

KMLStyle::KMLStyle(KMLObject * parent) :
KMLObject(STYLE, parent)
{
}

KMLStyle::KMLStyle(const KMLStyle & source) :
KMLObject(source)
{
}

KMLStyle::~KMLStyle() 
{
	map<KMLSubStyle::KMLStyleType, KMLSubStyle*>::iterator it;

	for(it = _visuals.begin(); it != _visuals.end(); ++it)
		delete it->second;
}

KMLStyle& KMLStyle::operator=(const KMLStyle & source) 
{
	KMLObject::operator=(source);

	return *this;
}

void KMLStyle::addChild(KMLObject * child) 
{
}

void KMLStyle::accept(KMLAbstractVisitor & v) 
{
	v.visit(*this);
}

void KMLStyle::setVisual(const KMLSubStyle::KMLStyleType & type, KMLSubStyle * visual) 
{
	_visuals[type] = visual;
}

KMLSubStyle* KMLStyle::getVisual(const KMLSubStyle::KMLStyleType & visualType) const 
{
	map<KMLSubStyle::KMLStyleType, KMLSubStyle*>::const_iterator it =
			_visuals.find(visualType);

	return (it == _visuals.end()) ? NULL : it->second;
}


} // namespace tdk
