
#ifndef TDK_KMLSTYLE_H
#define TDK_KMLSTYLE_H

#include "kmldefines.h"
#include "kmlobject.h"
#include "kmlsubstyle.h"
#include <map>
using namespace std;

namespace tdk { class KMLAbstractVisitor; } 

namespace tdk {

class TDKKML_API KMLStyle : public KMLObject 
{
  public:
    KMLStyle(KMLObject * parent = NULL);


  private:
    KMLStyle(const KMLStyle & source);


  public:
    virtual ~KMLStyle();


  private:
    KMLStyle& operator=(const KMLStyle & source);


  public:
    void addChild(KMLObject * child);

    virtual void accept(KMLAbstractVisitor & v);

    void setVisual(const KMLSubStyle::KMLStyleType & type, KMLSubStyle * visual);

    KMLSubStyle* getVisual(const KMLSubStyle::KMLStyleType & visualType) const;


  protected:
    //Visuals for every type supported. Note that if one of the styles does not exists, a NULL pointer will be setted.
    map<KMLSubStyle::KMLStyleType, KMLSubStyle*> _visuals;

};

} // namespace tdk
#endif
