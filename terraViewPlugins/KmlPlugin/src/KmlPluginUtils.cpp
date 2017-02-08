#include <KmlPluginUtils.h>

#include <TeLayer.h>
#include <TeView.h>
#include <TeTheme.h>
#include <TeDatabase.h>
#include <TeAbstractTheme.h>

KmlPluginUtils::KmlPluginUtils()
{
	
}

KmlPluginUtils::~KmlPluginUtils()
{
	
}

TeAbstractTheme* KmlPluginUtils::getTheme(TeDatabase * database, const std::string & themeName, TeView* view)
{
	TeAbstractTheme* theme = 0;
	
	std::vector<TeViewNode*> nodes = view->themes();

	if(nodes.empty())
		return theme;

	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		if(nodes[i]->type() != TeTREE)
		{
			if(nodes[i]->name() == themeName)
			{
				theme = dynamic_cast<TeAbstractTheme*>(nodes[i]);
				break;
			}
		}
	}
	

	return theme;
}

TeLayer* KmlPluginUtils::getLayer(TeDatabase * database, const std::string & layerName)
{
	TeLayer* layer = 0;

	TeLayerMap layerMap =  database->layerMap();

	if(layerMap.empty())
		return 0;

	TeLayerMap::iterator it = layerMap.begin();

	while(it != layerMap.end())
	{
		if(layerName == it->second->name())
		{
			layer = it->second;
			break;		
		}		

		++it;
	}

	if(layer->hasGeometry(TeRASTER) || layer->hasGeometry(TeRASTERFILE))
		return 0;

	return layer;
}

TeView* KmlPluginUtils::getView(TeDatabase * database, const std::string & viewName)
{
	TeView* view = 0;

	TeViewMap viewMap =  database->viewMap();

	if(viewMap.empty())
		return 0;

	TeViewMap::iterator it = viewMap.begin();

	while(it != viewMap.end())
	{
		if(viewName == it->second->name())
		{			
			view = dynamic_cast<TeView*>(it->second);
			return view;
		
		}		

		++it;
	}

	return view;
}

std::vector<TeView*> KmlPluginUtils::getViews(TeDatabase * database)
{
	std::vector<TeView*> views;

	TeViewMap viewMap = database->viewMap();
	
	if(viewMap.empty())
		return views;

	TeViewMap::iterator it = viewMap.begin();

	while(it != viewMap.end())
	{
		views.push_back(it->second);

		++it;
	}

	return views;
}

bool KmlPluginUtils::correctName(std::string & name)
{

	name = TeReplaceSpecialChars(name);

	name = TeRemoveSpecialChars(name);

	bool isInvalid;
	std::string error;
	TeCheckName(name, isInvalid, error);
	if(isInvalid)
	{
		return false;
	}

	return true;

}

std::vector<TeAbstractTheme*> KmlPluginUtils::getThemes(TeDatabase * database, TeView* view)
{
	std::vector<TeAbstractTheme*> themes;

	std::vector<TeViewNode*> nodes = view->themes();

	for(unsigned int i = 0; i < nodes.size(); i++)
	{
		if(nodes[i]->type() != TeTREE)
		{
			themes.push_back(dynamic_cast<TeAbstractTheme*>(nodes[i]));
		}		
	}

	return themes;
}

std::vector<TeLayer*> KmlPluginUtils::getLayers(TeDatabase * database)
{
	std::vector<TeLayer*> layers;

	TeLayerMap layerMap =  database->layerMap();

	if(layerMap.empty())
		return layers;

	TeLayerMap::iterator it = layerMap.begin();

	while(it != layerMap.end())
	{
		
		if(!it->second->hasGeometry(TeRASTER) && !it->second->hasGeometry(TeRASTERFILE))
		{
			layers.push_back(it->second);
		}
		
		++it;		
	}

	return layers;
}

TeTheme* KmlPluginUtils::getTeTheme(TeDatabase * database, const std::string & themeName, TeView* view)
{
	std::vector<TeViewNode*>& vecViewNode = view->themes();
	for(unsigned int i = 0; i < vecViewNode.size(); ++i)
	{
		if(vecViewNode[i]->type() == TeTHEME)
		{
			TeTheme* theme = dynamic_cast<TeTheme*>(vecViewNode[i]);
			if(theme->layer()->hasGeometry(TeRASTER) || theme->layer()->hasGeometry(TeRASTERFILE))
			{
				return 0;
			}
			return theme;
		}
	}

	return 0;

	/*TeThemeMap themeMap =  database->themeMap();

	if(themeMap.empty())
		return 0;

	TeThemeMap::iterator it = themeMap.begin();

	while(it != themeMap.end())
	{
		if(themeName == it->second->name())
		{
			if(it->second->type() == TeTHEME)
			{
				theme = dynamic_cast<TeTheme*>(it->second);
				return theme;
			}
			else
			{
				theme = 0;
				break;
			}				
			
		}		

		++it;
	}

	if(theme == 0)
	{
		return 0;
	}
	if(theme->layer()->hasGeometry(TeRASTER) || theme->layer()->hasGeometry(TeRASTERFILE))
	{
		return 0;
	}*/
}