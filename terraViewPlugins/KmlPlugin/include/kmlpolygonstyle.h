#ifndef TDK_KMLPOLYGONSTYLE_H
#define TDK_KMLPOLYGONSTYLE_H


#include "kmlsubstyle.h"

namespace tdk {

class TDKKML_API KMLPolygonStyle : public KMLSubStyle 
{
  public:
    KMLPolygonStyle();

    virtual ~KMLPolygonStyle();

    const bool getFill() const;

    void setFill(const bool & value);

    const bool getOutline() const;

    void setOutline(const bool & value);


  protected:
    bool _fill;

    bool _outline;

};

} // namespace tdk
#endif
