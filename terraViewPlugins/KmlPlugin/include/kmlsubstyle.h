
#ifndef TDK_KMLSUBSTYLE_H
#define TDK_KMLSUBSTYLE_H

#include "kmldefines.h"
#include <string>
using namespace std;
#include "TeVisual.h"

namespace tdk {

class TDKKML_API KMLSubStyle 
{
  public:
    //Style type definition.
    enum KMLStyleType {
      ICON,
      LABEL,
      LINE,
      POLYGON,
      BALLOON,
      LIST

    };

    //Defines the color mode to be used.
    enum KMLColorMode {
      NORMAL,
      RANDOM

    };

    virtual ~KMLSubStyle();

    const string getId() const;

    void setId(const string & value);

    const KMLStyleType getStyleType() const;

    void setStyleType(const KMLStyleType & value);

    const KMLColorMode getColorMode() const;

    void setColorMode(const KMLColorMode & value);

    const ::TeColor getColor() const;

    void setColor(const ::TeColor & value);

    const int getAlpha() const;

    void setAlpha(const int & value);


  protected:
    KMLSubStyle(const KMLStyleType & type);

    //The style identifier.
    string _id;

    KMLStyleType _styleType;

    KMLColorMode _colorMode;

    ::TeColor _color;

    int _alpha;


  private:
    //No copy allowed.
    KMLSubStyle(const KMLSubStyle & source);

    //No copy allowed.
    KMLSubStyle& operator=(const KMLSubStyle & source);

};

} // namespace tdk
#endif
