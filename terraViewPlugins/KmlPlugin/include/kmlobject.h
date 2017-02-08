
#ifndef TDK_KMLOBJECT_H
#define TDK_KMLOBJECT_H

#include "visitreceiver.h"
#include "kmlstructureitem.h"
#include "kmldefines.h"
#include <string>
using namespace std;
#include "kmlabstractvisitor.h"

namespace tdk {

//Basic KML object
class TDKKML_API KMLObject : public VisitReceiver<KMLAbstractVisitor>, public KMLStructureItem<KMLObject> 
{
  public:
    //Default constructor.
    KMLObject(const KMLType & objType, KMLObject * parent = NULL);

    //Copy constructor.
    KMLObject(const KMLObject & source);

    //Virtual destructor.
    virtual ~KMLObject();

    //Copy operator.
    KMLObject & operator =(const KMLObject & source);

    //Returns the object identifier.
    string getId() const;

    //Sets the object identifier.
    void setId(const string & id);

    //Returns TargetId attribute.
    string getTargetId() const;

    //Sets target id attribute.
    void setTargetId(const string & targetId);

    KMLType getKMLType() const;

    void setKMLType(const KMLType & value);

    static KMLObject* DefaultObject();


  protected:
    string _id;

    string _targetId;

    KMLType _kmltype;

};

} // namespace tdk
#endif
