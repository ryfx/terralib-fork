#include <HidroOverlayRule.h>

HidroOverlayRule::HidroOverlayRule(const std::string& ruleId): _ruleId(ruleId), _layer(NULL), _attr(NULL)
{
}

HidroOverlayRule::~HidroOverlayRule()
{
	if(_attr)
	{
		delete _attr;
	}
}

void HidroOverlayRule::setRuleId(const std::string& ruleId)
{
	_ruleId = ruleId;
}

std::string HidroOverlayRule::getRuleId()
{
	return _ruleId;
}

void HidroOverlayRule::setLayer(TeLayer* layer)
{
	_layer = layer;
}

TeLayer* HidroOverlayRule::getLayer()
{
	return _layer;
}

void HidroOverlayRule::setComparisonOperator(const OverlayComparisonOperator& op)
{
	_op = op;
}

HidroOverlayRule::OverlayComparisonOperator HidroOverlayRule::getComparisonOperator()
{
	return _op;
}

void HidroOverlayRule::setRuleAttr(HidroAttr* attr)
{
	_attr = attr;
}

HidroAttr* HidroOverlayRule::getRuleAttr()
{
	return _attr;
}

HidroOverlayRule::OverlayComparisonOperator HidroOverlayRule::getRuleComparisonOp(const std::string& value)
{
	if(value == "==")
	{
		return OverlayComparisonOperator::Equal;
	}
	else if(value == "<>")
	{
		return OverlayComparisonOperator::NotEqual;
	}
	else if(value == ">")
	{
		return OverlayComparisonOperator::GreaterThan;
	}
	else if(value == "<")
	{
		return OverlayComparisonOperator::LessThan;
	}
	else if(value == ">=")
	{
		return OverlayComparisonOperator::GreaterThanOrEqualTo;
	}
	else if(value == "<=")
	{
		return OverlayComparisonOperator::LessThanOrEqualTo;
	}

	return OverlayComparisonOperator::None;
}

std::string HidroOverlayRule::getRuleStrComparisonOp(const OverlayComparisonOperator& op)
{
	if(op == OverlayComparisonOperator::Equal)
	{
		return "==";
	}
	else if(op == OverlayComparisonOperator::NotEqual)
	{
		return "<>";
	}
	else if(op == OverlayComparisonOperator::GreaterThan)
	{
		return ">";
	}
	else if(op == OverlayComparisonOperator::LessThan)
	{
		return "<";
	}
	else if(op == OverlayComparisonOperator::GreaterThanOrEqualTo)
	{
		return ">=";
	}
	else if(op == OverlayComparisonOperator::LessThanOrEqualTo)
	{
		return "<=";
	}

	return "";
}