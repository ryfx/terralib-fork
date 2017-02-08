#include <HidroAttr.h>

HidroAttr::HidroAttr(): _attrName(""), _attrType(TeUNKNOWN)
{
}

HidroAttr::~HidroAttr()
{
}

TeAttrDataType HidroAttr::getAttrType() const
{
	return _attrType;
}

std::string HidroAttr::getAttrName() const
{
	return _attrName;
}

void HidroAttr::setAttrName(const std::string& attrName)
{
	_attrName = attrName;
}
