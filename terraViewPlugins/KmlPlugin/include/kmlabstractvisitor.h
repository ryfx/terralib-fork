//This files contains the definition of a KML visitor object.
#ifndef TDK_KMLABSTRACTVISITOR_H
#define TDK_KMLABSTRACTVISITOR_H

#include "kmldefines.h"
namespace tdk { class KMLDocument; } 
namespace tdk { class KMLFolder; } 
namespace tdk { class KMLPlacemark; } 
namespace tdk { class KMLStyle; } 
namespace tdk { class KMLStyleMap; } 

namespace tdk {

//Defines an interface for visitors that operates KML objects. Each visitor must implement the methods for visiting each kml object. Each visitor must execute different operations when visiting a KML object.
class TDKKML_API KMLAbstractVisitor 
{
  public:
    //Executes the operation of visiting a KMLDocument object.
    virtual void visit(KMLDocument & doc) = 0;

    //Executes the operation of visiting a KMLFolder object.
    virtual void visit(KMLFolder & folder) = 0;

    //Executes the operation of visiting a KMLPlacemark object.
    virtual void visit(KMLPlacemark & mark) = 0;

    //Executes the operation of visiting a KMLStyle object.
    virtual void visit(KMLStyle & style) = 0;

    //Executes the operation of visiting a KMLStyleMap object.
    virtual void visit(KMLStyleMap & style) = 0;


  protected:
    //Default constructor.
    KMLAbstractVisitor();

};
inline KMLAbstractVisitor::KMLAbstractVisitor() 
{
}


} // namespace tdk
#endif
