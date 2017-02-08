#include <HidroAttrString.h>

HidroAttrString::HidroAttrString() : HidroAttr()
{
	_attrType = TeSTRING;
}

HidroAttrString::HidroAttrString(const std::string& attrName, const std::string& value) : HidroAttr()
{
	_attrType	= TeSTRING;
	_attrName	= attrName;
	_attrValue	= value;
}

HidroAttrString::~HidroAttrString()
{
}

std::string HidroAttrString::getAttrValue() const
{
	return _attrValue;
}

void HidroAttrString::setAttrValue(const std::string& attrValue)
{
	_attrValue = attrValue;
}
