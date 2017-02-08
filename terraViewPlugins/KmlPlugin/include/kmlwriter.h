#ifndef TDK_KMLWRITER_H
#define TDK_KMLWRITER_H


#include "kmlabstractvisitor.h"
#include <vector>
using namespace std;

namespace tdk { class KMLDocument; } 
namespace tdk { class KMLFolder; } 
namespace tdk { class KMLPlacemark; } 
namespace tdk { class KMLStyle; } 
namespace tdk { class KMLStyleMap; } 
namespace TeOGC { class TeXMLEncoder; } 
namespace tdk { class KMLObject; } 
namespace tdk { class KMLFeature; } 
class TePoint;
class TeLine2D;
class TePolygon;
namespace tdk { class KMLSubStyle; } 
namespace tdk { class KMLLineStyle; } 

namespace tdk {

class TDKKML_API KMLWriter : public KMLAbstractVisitor 
{
  public:
    KMLWriter();

    ~KMLWriter();

    void visit(KMLDocument & doc);

    void visit(KMLFolder & folder);

    void visit(KMLPlacemark & mark);

    void visit(KMLStyle & style);

    void visit(KMLStyleMap & style);

    void setDocuments(const vector< KMLFeature *> & feats);

    void write(TeOGC::TeXMLEncoder & encoder);


  protected:
    void writeObject(const KMLObject & object);

    void writeFeature(const KMLFeature & feature);

    void writeGeometry(const ::TePoint & geom);

    void writeGeometry(const ::TeLine2D & geom);

    void writeGeometry(const ::TePolygon & geom);

    void writeStyle(const KMLSubStyle & style);

    //Vector with kml documents.
    vector<KMLFeature *> _feats;

    //XML encoder.
    TeOGC::TeXMLEncoder * _xmlEncoder;

};

} // namespace tdk
#endif
