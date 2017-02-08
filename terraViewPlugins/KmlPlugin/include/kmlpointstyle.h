#ifndef TDK_KMLPOINTSTYLE_H
#define TDK_KMLPOINTSTYLE_H


#include "TeCoord2D.h"
#include <string>
using namespace std;
#include "kmlsubstyle.h"

namespace tdk {

class TDKKML_API KMLPointStyle : public KMLSubStyle 
{
  public:
    enum OffsetUnit {
      PIXELS,
      FRACTION,
      INSETPIXELS

    };

    KMLPointStyle();

    virtual ~KMLPointStyle();

    const double getScale() const;

    void setScale(const double & value);

    const double getHeading() const;

    void setHeading(const double & value);

    const ::TeCoord2D getHotspot() const;

    void setHotspot(const ::TeCoord2D & value);

    void getHotspotUnit(OffsetUnit & xUnit, OffsetUnit & yUnit);

    void setHotspotUnit(const OffsetUnit & xUnit, const OffsetUnit & yUnit);

    const string getIcon() const;

    void setIcon(const string & value);


  protected:
    //Defines the icon scale.
    double _scale;

    //Defines the direction as specified by google on KML spec.
    double _heading;

    //Placemark hotspot.
    ::TeCoord2D _hotspot;

    OffsetUnit _xUnit;

    OffsetUnit _yUnit;

    //Defines location of the image to be used as icon.
    string _icon;

};

} // namespace tdk
#endif
