#include "kmldocument.h"
#include "kmlobject.h"
#include "kmlabstractvisitor.h"

namespace tdk {

KMLDocument::KMLDocument(KMLObject * parent) :
KMLFeature(DOCUMENT, parent)
{
	setKMLType(DOCUMENT);
}

KMLDocument::KMLDocument(const KMLDocument & source) :
KMLFeature(source)
{
	setKMLType(DOCUMENT);
}

KMLDocument::~KMLDocument() 
{
//	map<string, TeVisual*>::iterator it;
//
//	for(it = _visuals.begin(); it != _visuals.end(); ++it)
//		delete it->second;
}

KMLDocument KMLDocument::operator =(const KMLDocument & source) 
{
	KMLObject::operator =(source);

	return *this;
}

void KMLDocument::accept(KMLAbstractVisitor & v) 
{
	v.visit(*this);
}


} // namespace tdk
