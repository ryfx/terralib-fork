
#include "kmlpointstyle.h"

namespace tdk {

KMLPointStyle::KMLPointStyle() :
KMLSubStyle(ICON)
{
}

KMLPointStyle::~KMLPointStyle() 
{
}

const double KMLPointStyle::getScale() const 
{
  return _scale;
}

void KMLPointStyle::setScale(const double & value) 
{
  _scale = value;
}

const double KMLPointStyle::getHeading() const 
{
  return _heading;
}

void KMLPointStyle::setHeading(const double & value) 
{
  _heading = value;
}

const ::TeCoord2D KMLPointStyle::getHotspot() const 
{
  return _hotspot;
}

void KMLPointStyle::setHotspot(const ::TeCoord2D & value) 
{
  _hotspot = value;
}

void KMLPointStyle::getHotspotUnit(KMLPointStyle::OffsetUnit & xUnit, KMLPointStyle::OffsetUnit & yUnit) 
{
	xUnit = _xUnit;
	yUnit = _yUnit;
}

void KMLPointStyle::setHotspotUnit(const KMLPointStyle::OffsetUnit & xUnit, const KMLPointStyle::OffsetUnit & yUnit) 
{
	_xUnit = xUnit;
	_yUnit = yUnit;
}

const string KMLPointStyle::getIcon() const 
{
  return _icon;
}

void KMLPointStyle::setIcon(const string & value) 
{
  _icon = value;
}


} // namespace tdk
