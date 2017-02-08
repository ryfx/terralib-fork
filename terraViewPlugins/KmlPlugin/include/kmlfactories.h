#ifndef TDK_KMLFACTORIES_H
#define TDK_KMLFACTORIES_H


#include "kmlobjectfactory.h"

namespace tdk { class KMLObject; } 
namespace tdk { struct KMLObjsParams; } 

namespace tdk {

class TDKKML_API KMLDocumentFactory : public KMLObjectFactory 
{
  public:
    KMLDocumentFactory();


  protected:
    //!  Builds a new product from a set of parameters (should be implemented by descendants)
    KMLObject* build(KMLObjsParams * params);

};
class TDKKML_API KMLFolderFactory : public KMLObjectFactory 
{
  public:
    KMLFolderFactory();


  protected:
    //!  Builds a new product from a set of parameters (should be implemented by descendants)
    KMLObject* build(KMLObjsParams * params);

};
class TDKKML_API KMLPlacemarkFactory : public KMLObjectFactory 
{
  public:
    KMLPlacemarkFactory();


  protected:
    //!  Builds a new product from a set of parameters (should be implemented by descendants)
    KMLObject* build(KMLObjsParams * params);

};
class TDKKML_API KMLStyleFactory : public KMLObjectFactory 
{
  public:
    KMLStyleFactory();


  protected:
    //!  Builds a new product from a set of parameters (should be implemented by descendants)
    KMLObject* build(KMLObjsParams * params);

};
class TDKKML_API KMLStyleMapFactory : public KMLObjectFactory 
{
  public:
    KMLStyleMapFactory();


  protected:
    //!  Builds a new product from a set of parameters (should be implemented by descendants)
    KMLObject* build(KMLObjsParams * params);

};

} // namespace tdk
#endif
