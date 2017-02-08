#ifndef KMLPLUGINUTILS_H
#define KMLPLUGINUTILS_H

#include <string>
#include <vector>

class TeDatabase;
class TeLayer;
class TeTheme;
class TeView;
class TeAbstractTheme;

class KmlPluginUtils
{

public:	

	KmlPluginUtils();
	~KmlPluginUtils();


	static std::vector<TeAbstractTheme*> getThemes(TeDatabase * database, TeView* view);
	
	static TeAbstractTheme* getTheme(TeDatabase * database, const std::string & themeName, TeView* view);
	static TeTheme* getTeTheme(TeDatabase * database, const std::string & themeName, TeView* view);
	
	static TeLayer* getLayer(TeDatabase * database, const std::string & layerName);
	static std::vector<TeLayer*> getLayers(TeDatabase * database);	

	static TeView* getView(TeDatabase * database, const std::string & viewName);
	static std::vector<TeView*> getViews(TeDatabase * database);

	static bool correctName(std::string & name);

};

#endif
