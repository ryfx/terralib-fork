#ifndef TDK_KMLSTYLEMAP_H
#define TDK_KMLSTYLEMAP_H


#include <map>
#include "kmlobject.h"
#include <string>
using namespace std;
#include <map>
using namespace std;

namespace tdk { class KMLAbstractVisitor; } 

namespace tdk {

class TDKKML_API KMLStyleMap : public KMLObject 
{
  public:
    KMLStyleMap(KMLObject * parent = NULL);

    virtual ~KMLStyleMap();

    virtual void accept(KMLAbstractVisitor & v);

    void mapStyle(const string & id, const string & value);

    map<string, string> getStyles() const;


  protected:
    map<string, string> _styles;


  private:
    KMLStyleMap(const KMLStyleMap & source);

    KMLStyleMap & operator=(const KMLStyleMap & source);

};

} // namespace tdk
#endif
