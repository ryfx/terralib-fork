#include <HidroOverlayClassGroup.h>
#include <HidroOverlayClass.h>


HidroOverlayClassGroup::HidroOverlayClassGroup()
{
	_count = 1;

	_classMap.clear();
}

HidroOverlayClassGroup::~HidroOverlayClassGroup()
{
	_classMap.clear();
}

std::map<std::string, HidroOverlayClass*>& HidroOverlayClassGroup::getClasses()
{
	return _classMap;
}

bool HidroOverlayClassGroup::addNewClass(HidroOverlayClass* overlayClass)
{
	std::map<std::string, HidroOverlayClass*>::iterator it = _classMap.find(overlayClass->getClassName());

	if(it != _classMap.end())
	{
		return false;
	}

	_classMap.insert(std::map<std::string, HidroOverlayClass*>::value_type(overlayClass->getClassName(), overlayClass));
	
	++_count;

	return true;
}

void HidroOverlayClassGroup::removeClass(const std::string& className)
{
	std::map<std::string, HidroOverlayClass*>::iterator it = _classMap.begin();

	while(it != _classMap.end())
	{
		if(it->second->getClassName() == className)
		{
			_classMap.erase(it);

			return;
		}
		++it;
	}
}

HidroOverlayClass* HidroOverlayClassGroup::getClass(const std::string& className)
{
	std::map<std::string, HidroOverlayClass*>::iterator it = _classMap.begin();

	while(it != _classMap.end())
	{
		if(it->second->getClassName() == className)
		{
			return it->second;
		}
		++it;
	}

	return NULL;
}

std::vector<TeColor> HidroOverlayClassGroup::getClassGroupLut()
{
	std::vector<TeColor> colorVec;

	int idxValue = 1;

	while(colorVec.size() < _classMap.size())
	{
		std::map<std::string, HidroOverlayClass*>::iterator it = _classMap.begin();

		while(it != _classMap.end())
		{
			if(it->second->getClassIdx() == idxValue)
			{
				TeColor color(it->second->getClassColor().red_, it->second->getClassColor().green_, it->second->getClassColor().blue_);

				colorVec.push_back(color);

				++idxValue;

				break;
			}
			++it;
		}
	}

	return colorVec;
}

int HidroOverlayClassGroup::getNewIndex()
{
	return _count;
}