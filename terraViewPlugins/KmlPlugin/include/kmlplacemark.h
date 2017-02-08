#ifndef TDK_KMLPLACEMARK_H
#define TDK_KMLPLACEMARK_H


#include "kmlfeature.h"

namespace tdk { class KMLObject; } 
namespace tdk { class KMLAbstractVisitor; } 
class TeGeometry;

namespace tdk {

///**	@class KMLPlacemark
// *	@brief ${comments}
// **/
class TDKKML_API KMLPlacemark : public KMLFeature {
  public:
    //Defines the type of geometry defined in a placemark
    enum KMLGeometryType {
      POINT,
      LINE,
      POLYGON,
      UNKNOWN

    };

    //TDKKML_API
    //Deafault constructor.
    KMLPlacemark(KMLObject * parent = NULL);

    //Copy constructor.
    KMLPlacemark(const KMLPlacemark & source);

    //Virtual destructor.
    virtual ~KMLPlacemark();

    //Copy operator.
    KMLPlacemark & operator =(const KMLPlacemark & source);

    void addChild(KMLObject * child);

    virtual void accept(KMLAbstractVisitor & v);

    const TeGeometry & getGeometry() const;

    void setGeometry(const TeGeometry & geom);

    const KMLGeometryType getGeometryType() const;


  protected:
    void copyGeometry(const TeGeometry & geom);

    TeGeometry * _geometry;

    KMLGeometryType _geomType;

};

} // namespace tdk
#endif
