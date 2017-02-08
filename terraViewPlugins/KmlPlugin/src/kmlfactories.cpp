
#include "kmlfactories.h"
#include "kmlobject.h"

//KML include files
#include <kmldocument.h>
#include <kmlfolder.h>
#include <kmlplacemark.h>
#include <kmlstyle.h>
#include <kmlstylemap.h>

namespace tdk {

KMLDocumentFactory::KMLDocumentFactory() :
KMLObjectFactory("Document")
{
}

//!  Builds a new product from a set of parameters (should be implemented by descendants)
KMLObject* KMLDocumentFactory::build(KMLObjsParams * params) 
{
	return new KMLDocument(params->_objParent);
}

KMLFolderFactory::KMLFolderFactory() :
KMLObjectFactory("Folder")
{
}

//!  Builds a new product from a set of parameters (should be implemented by descendants)
KMLObject* KMLFolderFactory::build(KMLObjsParams * params) 
{
	return new KMLFolder(params->_objParent);
}

KMLPlacemarkFactory::KMLPlacemarkFactory() :
KMLObjectFactory("Placemark")
{
}

//!  Builds a new product from a set of parameters (should be implemented by descendants)
KMLObject* KMLPlacemarkFactory::build(KMLObjsParams * params) 
{
	return new KMLPlacemark(params->_objParent);
}

KMLStyleFactory::KMLStyleFactory() :
KMLObjectFactory("Style")
{
}

//!  Builds a new product from a set of parameters (should be implemented by descendants)
KMLObject* KMLStyleFactory::build(KMLObjsParams * params) 
{
	return new KMLStyle(params->_objParent);
}

KMLStyleMapFactory::KMLStyleMapFactory() :
KMLObjectFactory("StyleMap")
{
}

//!  Builds a new product from a set of parameters (should be implemented by descendants)
KMLObject* KMLStyleMapFactory::build(KMLObjsParams * params) 
{
	return new KMLStyleMap(params->_objParent);
}


} // namespace tdk
