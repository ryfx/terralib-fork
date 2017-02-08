#include <HidroAttrDouble.h>

HidroAttrDouble::HidroAttrDouble() : HidroAttr()
{
	_attrType = TeREAL;
}

HidroAttrDouble::HidroAttrDouble(const std::string& attrName, const double& value) : HidroAttr()
{
	_attrType	= TeREAL;
	_attrName	= attrName;
	_attrValue	= value;
}

HidroAttrDouble::~HidroAttrDouble()
{
}

double HidroAttrDouble::getAttrValue() const
{
	return _attrValue;
}

void HidroAttrDouble::setAttrValue(const double& attrValue)
{
	_attrValue = attrValue;
}
