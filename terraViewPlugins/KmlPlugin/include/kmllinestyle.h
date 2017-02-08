#ifndef TDK_KMLLINESTYLE_H
#define TDK_KMLLINESTYLE_H


#include "kmlsubstyle.h"

namespace tdk {

class TDKKML_API KMLLineStyle : public KMLSubStyle 
{
  public:
    KMLLineStyle();

    ~KMLLineStyle();

    const int getWidth() const;

    void setWidth(const int & value);


  protected:
    int _width; //!< Defines the line width.    

};

} // namespace tdk
#endif
