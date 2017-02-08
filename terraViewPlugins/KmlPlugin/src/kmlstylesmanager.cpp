
#include "kmlstylesmanager.h"
#include "kmlstyle.h"
#include "kmlstylemap.h"

namespace tdk {

KMLStylesManager::KMLStylesManager() 
{
}

KMLStylesManager::~KMLStylesManager() 
{
}

void KMLStylesManager::addStyle(KMLStyle * style) 
{
	_styleMap[style->getId()] = style;
}

//Adds reference to KML style map object
void KMLStylesManager::addStyle(KMLStyleMap * styleMap) 
{
	if(styleMap->getId().empty())
		return;

	map<string, string> sts = styleMap->getStyles();
	map<string, string>::iterator it;

	for(it = sts.begin(); it != sts.end(); ++it)
		_styleMappings.insert(pair<string, string>(styleMap->getId(), it->second));
}

vector<const KMLStyle*> KMLStylesManager::getStyles(const string & id) 
{
	vector<const KMLStyle*> r;

	pair<multimap<string, string>::iterator, multimap<string, string>::iterator>
			sts = _styleMappings.equal_range(id);

	multimap<string, string>::iterator mapIt;

	if(sts.first != sts.second)
	{
		for(mapIt = sts.first; mapIt != sts.second; ++mapIt)
		{
			map<string, KMLStyle*>::iterator it = _styleMap.find(mapIt->second);

			if(it != _styleMap.end())
				r.push_back(it->second);
		}
	}
	else
	{
		map<string, KMLStyle*>::iterator it = _styleMap.find(id);

		if(it != _styleMap.end())
			r.push_back(it->second);
	}

	return r;
}


} // namespace tdk
