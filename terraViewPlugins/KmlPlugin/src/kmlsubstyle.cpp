#include "kmlsubstyle.h"

namespace tdk {

KMLSubStyle::~KMLSubStyle() 
{
}

const string KMLSubStyle::getId() const 
{
  return _id;
}

void KMLSubStyle::setId(const string & value) 
{
  _id = value;
}

const KMLSubStyle::KMLStyleType KMLSubStyle::getStyleType() const 
{
  return _styleType;
}

void KMLSubStyle::setStyleType(const KMLSubStyle::KMLStyleType & value) 
{
  _styleType = value;
}

const KMLSubStyle::KMLColorMode KMLSubStyle::getColorMode() const 
{
  return _colorMode;
}

void KMLSubStyle::setColorMode(const KMLSubStyle::KMLColorMode & value) 
{
  _colorMode = value;
}

const ::TeColor KMLSubStyle::getColor() const 
{
  return _color;
}

void KMLSubStyle::setColor(const ::TeColor & value) 
{
  _color = value;
}

const int KMLSubStyle::getAlpha() const 
{
  return _alpha;
}

void KMLSubStyle::setAlpha(const int & value) 
{
  _alpha = value;
}

KMLSubStyle::KMLSubStyle(const KMLSubStyle::KMLStyleType & type) 
{
	_styleType = type;
}


} // namespace tdk
