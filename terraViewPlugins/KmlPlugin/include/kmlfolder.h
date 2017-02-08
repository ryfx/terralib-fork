#ifndef TDK_KMLFOLDER_H
#define TDK_KMLFOLDER_H


#include "kmlfeature.h"

namespace tdk { class KMLObject; } 
namespace tdk { class KMLAbstractVisitor; } 

namespace tdk {

class TDKKML_API KMLFolder : public KMLFeature {
  public:
    KMLFolder(KMLObject * parent = NULL);

    virtual ~KMLFolder();

    KMLFolder(const KMLFolder & source);

    void accept(KMLAbstractVisitor & v);

};

} // namespace tdk
#endif
