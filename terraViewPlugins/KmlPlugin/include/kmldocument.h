//This file contains a definition for a KML document.
#ifndef TDK_KMLDOCUMENT_H
#define TDK_KMLDOCUMENT_H

#include "kmlfeature.h"

namespace tdk { class KMLObject; } 
namespace tdk { class KMLAbstractVisitor; } 

namespace tdk {

class TDKKML_API KMLDocument : public KMLFeature {
  public:
    //Default constructor.
    KMLDocument(KMLObject * parent = NULL);

    //Copy constructor.
    KMLDocument(const KMLDocument & source);

    //Virtual destructor.
    virtual ~KMLDocument();

    //Copy operator.
    KMLDocument operator =(const KMLDocument & source);

    //Used by visitor pattern. This method is called when visiting the Document.
    void accept(KMLAbstractVisitor & v);

};

} // namespace tdk
#endif
