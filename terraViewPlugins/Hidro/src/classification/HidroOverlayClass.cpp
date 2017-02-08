#include <HidroOverlayClass.h>
#include <HidroOverlayRuleGroup.h>

#include <TeUtils.h>


HidroOverlayClass::HidroOverlayClass(const int& idx , const std::string& className): _className(className), _idx(idx), _isValid(false)
{
	_count = 0;

	_ruleGroupMap.clear();
}

HidroOverlayClass::~HidroOverlayClass()
{
	_ruleGroupMap.clear();
}

void HidroOverlayClass::setClassName(const std::string& className)
{
	_className = className;
}

std::string HidroOverlayClass::getClassName()
{
	return _className;
}

int HidroOverlayClass::getClassIdx()
{
	 return _idx;
}

void HidroOverlayClass::setClassColor(const TeColor& color)
{
	_color = color;
}

TeColor HidroOverlayClass::getClassColor()
{
	return _color;
}

bool HidroOverlayClass::ruleGroupIsValid()
{
	return _isValid;
}

std::map<std::string, HidroOverlayRuleGroup*>& HidroOverlayClass::getRuleGroups()
{
	return _ruleGroupMap;
}

bool HidroOverlayClass::addNewRuleGroup(HidroOverlayRuleGroup* ruleGroup)
{
	std::map<std::string, HidroOverlayRuleGroup*>::iterator it = _ruleGroupMap.find(ruleGroup->getRuleGroupId());

	if(it != _ruleGroupMap.end())
	{
		return false;
	}

	_ruleGroupMap.insert(std::map<std::string, HidroOverlayRuleGroup*>::value_type(ruleGroup->getRuleGroupId(), ruleGroup));

	++_count;

	return true;
}

void HidroOverlayClass::removeRuleGroup(const std::string& ruleGroupId)
{
	std::map<std::string, HidroOverlayRuleGroup*>::iterator it = _ruleGroupMap.begin();

	while(it != _ruleGroupMap.end())
	{
		if(it->second->getRuleGroupId() == ruleGroupId)
		{
			_ruleGroupMap.erase(it);

			return;
		}
		++it;
	}
}

std::string HidroOverlayClass::getNewGroupName()
{
	std::string groupName = "ruleGroup_" + Te2String(_count);

	return groupName;
}

HidroOverlayRuleGroup* HidroOverlayClass::getRuleGroup(const std::string& ruleGroupId)
{
	std::map<std::string, HidroOverlayRuleGroup*>::iterator it = _ruleGroupMap.begin();

	while(it != _ruleGroupMap.end())
	{
		if(it->second->getRuleGroupId() == ruleGroupId)
		{
			return it->second;
		}
		++it;
	}

	return NULL;
}