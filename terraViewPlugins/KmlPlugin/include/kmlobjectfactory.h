#ifndef TDK_KMLOBJECTFACTORY_H
#define TDK_KMLOBJECTFACTORY_H


#include <string>
using namespace std;
#include "kmlobject.h"
#include "TeAbstractFactory.h"

namespace tdk {

struct KMLObjsParams 
{
    KMLObjsParams(const string & factoryName, KMLObject * parent);

    string getProductId();

    //Name of the factory of kml objects to be used.
    string _objFactoryName;

    //Parent of the object to be constructed.
    KMLObject * _objParent;

};
inline KMLObjsParams::KMLObjsParams(const string & factoryName, KMLObject * parent) 
{
	_objFactoryName = factoryName;
	_objParent = parent;
}

inline string KMLObjsParams::getProductId() 
{
	return _objFactoryName;
}

//Defines a KML abstract factory.
class TDKKML_API KMLObjectFactory : public ::TeAbstractFactory<KMLObject, KMLObjsParams, string> 
{
  public:
    KMLObjectFactory(const string & factoryName);

    virtual ~KMLObjectFactory();

};
inline KMLObjectFactory::KMLObjectFactory(const string & factoryName) :
TeAbstractFactory<KMLObject, KMLObjsParams, string>(factoryName)
{
}

inline KMLObjectFactory::~KMLObjectFactory() 
{
}


} // namespace tdk
#endif
