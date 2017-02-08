
#include "kmlstylemap.h"
#include "kmlabstractvisitor.h"

namespace tdk {

KMLStyleMap::KMLStyleMap(KMLObject * parent) :
KMLObject(STYLEMAP, parent)
{
}

KMLStyleMap::~KMLStyleMap() 
{
}

void KMLStyleMap::accept(KMLAbstractVisitor & v) 
{
	v.visit(*this);
}

void KMLStyleMap::mapStyle(const string & id, const string & value) 
{
	_styles[id] = value;
}

map<string, string> KMLStyleMap::getStyles() const 
{
	return _styles;
}


} // namespace tdk
