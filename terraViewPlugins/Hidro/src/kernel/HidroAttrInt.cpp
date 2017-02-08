#include <HidroAttrInt.h>

HidroAttrInt::HidroAttrInt() : HidroAttr()
{
	_attrType = TeINT;
}

HidroAttrInt::HidroAttrInt(const std::string& attrName, const int& value) : HidroAttr()
{
	_attrType	= TeINT;
	_attrName	= attrName;
	_attrValue	= value;
}

HidroAttrInt::~HidroAttrInt()
{
}

int HidroAttrInt::getAttrValue() const
{
	return _attrValue;
}

void HidroAttrInt::setAttrValue(const int& attrValue)
{
	_attrValue = attrValue;
}
