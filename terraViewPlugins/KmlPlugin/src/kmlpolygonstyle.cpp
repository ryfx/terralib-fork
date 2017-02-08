
#include "kmlpolygonstyle.h"

namespace tdk {

KMLPolygonStyle::KMLPolygonStyle() :
KMLSubStyle(KMLSubStyle::POLYGON)
{
}

KMLPolygonStyle::~KMLPolygonStyle() 
{
}

const bool KMLPolygonStyle::getFill() const 
{
  return _fill;
}

void KMLPolygonStyle::setFill(const bool & value) 
{
  _fill = value;
}

const bool KMLPolygonStyle::getOutline() const 
{
  return _outline;
}

void KMLPolygonStyle::setOutline(const bool & value) 
{
  _outline = value;
}


} // namespace tdk
