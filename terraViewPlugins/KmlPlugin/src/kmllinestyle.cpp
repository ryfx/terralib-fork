
#include "kmllinestyle.h"

namespace tdk {

KMLLineStyle::KMLLineStyle() :
KMLSubStyle(LINE)
{
}

KMLLineStyle::~KMLLineStyle() 
{
}

const int KMLLineStyle::getWidth() const 
{
  return _width;
}

void KMLLineStyle::setWidth(const int & value) 
{
  _width = value;
}


} // namespace tdk
