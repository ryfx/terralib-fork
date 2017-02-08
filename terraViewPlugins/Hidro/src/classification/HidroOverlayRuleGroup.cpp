#include <HidroOverlayRuleGroup.h>
#include <HidroOverlayRule.h>

#include <TeUtils.h>

HidroOverlayRuleGroup::HidroOverlayRuleGroup(const std::string& groupId): _groupId(groupId), _isValid(false)
{
	_count = 0;
}

HidroOverlayRuleGroup::~HidroOverlayRuleGroup()
{
	_ruleMap.clear();
}

void HidroOverlayRuleGroup::setGroupRuleId(const std::string& groupId)
{
	_groupId = groupId;
}

std::string HidroOverlayRuleGroup::getRuleGroupId()
{
	return _groupId;
}

bool HidroOverlayRuleGroup::ruleIsValid()
{
	return _isValid;
}

std::map<std::string, HidroOverlayRule*>& HidroOverlayRuleGroup::getRuleGroup()
{
	return _ruleMap;
}

bool HidroOverlayRuleGroup::addNewRule(HidroOverlayRule* rule)
{
	std::map<std::string, HidroOverlayRule*>::iterator it = _ruleMap.find(rule->getRuleId());

	if(it != _ruleMap.end())
	{
		return false;
	}

	_ruleMap.insert(std::map<std::string, HidroOverlayRule*>::value_type(rule->getRuleId(), rule));

	++_count;

	return true;
}

void HidroOverlayRuleGroup::removeRule(const std::string& ruleId)
{
	std::map<std::string, HidroOverlayRule*>::iterator it = _ruleMap.begin();

	while(it != _ruleMap.end())
	{
		if(it->second->getRuleId() == ruleId)
		{
			_ruleMap.erase(it);

			return;
		}
		++it;
	}
}

HidroOverlayRule* HidroOverlayRuleGroup::getRule(const std::string& ruleId)
{
	std::map<std::string, HidroOverlayRule*>::iterator it = _ruleMap.begin();

	while(it != _ruleMap.end())
	{
		if(it->second->getRuleId() == ruleId)
		{
			return it->second;
		}
		++it;
	}

	return NULL;
}

std::string HidroOverlayRuleGroup::getNewRuleName()
{
	std::string ruleName = "rule_" + Te2String(_count);

	return ruleName;
}