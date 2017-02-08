/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
*/

/*!
	\file Utils.cpp 
	\brief Utility class for TerraSegreg plugin.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

// TerraSegreg
#include "Utils.h"

// TerraLib
#include <TeAsciiFile.h>
#include <TeColorUtils.h>
#include <TeDatabase.h>
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeSTElementSet.h>

Utils::Utils(TeDatabase* db) :
m_db(db)
{}

Utils::~Utils()
{}

void Utils::getLayers(std::vector<std::string>& layers, const int& geomRep)
{
	TeLayerMap& layerMap = m_db->layerMap();
	TeLayerMap::iterator it;
	for(it = layerMap.begin(); it != layerMap.end(); ++it)
	{
		if(it->second->geomRep() & geomRep)
			layers.push_back(it->second->name());
	}
}

void Utils::getLayers(std::vector<std::string>& layers)
{
	TeLayerMap& layerMap = m_db->layerMap();
	TeLayerMap::iterator it;
	for (it = layerMap.begin(); it != layerMap.end(); ++it)
		layers.push_back(it->second->name());
}

TeLayer* Utils::getLayer(const std::string& name)
{
	TeLayerMap& layerMap = m_db->layerMap();
	TeLayerMap::iterator it;
	for (it = layerMap.begin(); it != layerMap.end(); ++it)
	{
		if(it->second->name() == name)
			return it->second;
	}
	return 0;
}

void Utils::getLayerAttrTables(const std::string& name, std::vector<std::string>& tables)
{
	TeLayer* l = getLayer(name);
	getLayerAttrTables(l, tables);
}

void Utils::getLayerAttrTables(TeLayer* layer, std::vector<std::string>& tables)
{
	if(layer == 0) return;

	for(unsigned int i = 0; i < layer->attrTables().size(); ++i)
	{
		if(layer->attrTables()[i].tableType() != TeAttrMedia)
			tables.push_back(layer->attrTables()[i].name());
	}
}

void Utils::getLayerAttrList(const std::string& name, TeAttributeList& attrList)
{
	TeLayer* l = getLayer(name);
	getLayerAttrList(l, attrList);
}

void Utils::getLayerAttrList(TeLayer* layer, TeAttributeList& attrList)
{
	if(layer == 0)
		return;

	TeAttrTableVector& tables = layer->attrTables();
	for(unsigned int i = 0; i < tables.size(); ++i) // for each table
	{
		TeTable& t = tables[i];
		if(t.tableType() == TeAttrMedia)
			continue;
		
		TeAttributeList& myAttrList = t.attributeList();
		for(unsigned int j = 0; j < myAttrList.size(); ++j) // for each attribute
			attrList.push_back(myAttrList[j]);
	}
}

void Utils::getThemes(std::vector<std::string>& themes, const int& visibleRep)
{
	TeThemeMap& themeMap = m_db->themeMap();
	TeThemeMap::iterator it;
	for (it = themeMap.begin(); it != themeMap.end(); ++it)
	{
		if(it->second->visibleGeoRep() & visibleRep)
			themes.push_back(it->second->name());
	}
}

void Utils::getThemes(std::vector<std::string>& themes)
{
	TeThemeMap& themeMap = m_db->themeMap();
	TeThemeMap::iterator it;
	for (it = themeMap.begin(); it != themeMap.end(); ++it)
		themes.push_back(it->second->name());
}

TeTheme* Utils::getTheme(const std::string& name)
{
	TeThemeMap& themeMap = m_db->themeMap();
	TeThemeMap::iterator it;
	for (it = themeMap.begin(); it != themeMap.end(); ++it)
	{
		if(it->second->name() == name)
		{
			TeTheme* t = dynamic_cast<TeTheme*>(it->second);
			if(t) return t;
		}
	}
	return 0;
}

void Utils::getThemeAttrTables(const std::string& name, std::vector<std::string>& tables)
{
	TeTheme* t = getTheme(name);
	getThemeAttrTables(t, tables);
}

void Utils::getThemeAttrTables(TeTheme* theme, std::vector<std::string>& tables)
{
	if(theme == 0) return;
	getLayerAttrTables(theme->layer(), tables);
}

void Utils::getThemeAttrList(const std::string& name, TeAttributeList& attrList)
{
	TeTheme* t = getTheme(name);
	getThemeAttrList(t, attrList); 
}

void Utils::getThemeAttrList(TeTheme* theme, TeAttributeList& attrList)
{
	if(theme == 0) return;
	getLayerAttrList(theme->layer(), attrList);
}

TeSTElementSet* Utils::getElementSetFromTheme(const std::string& name, const bool& loadGeom, 
											  const std::vector<std::string>& attrs)
{
	TeTheme* t = getTheme(name);
	return getElementSetFromTheme(t, loadGeom, attrs);
}

TeSTElementSet* Utils::getElementSetFromTheme(TeTheme* theme, const bool& loadGeom, 
										      const std::vector<std::string>& attrs)
{
	if(theme == 0)
		return 0;

	TeQuerierParams params(loadGeom, attrs);
	params.setParams(theme);
	
	TeQuerier querier(params);
	if(!querier.loadInstances())
		return 0;

	TeSTElementSet* objSet = new TeSTElementSet(theme, querier.getAttrList());
	TeSTInstance st;
	while(querier.fetchInstance(st))
		objSet->insertSTInstance(st);

	return objSet;
}

void Utils::export2CSV(TeSTElementSet* objSet, const std::string& path, const char& sep)
{
	TeAsciiFile csv(path, "w+");

	TeAttributeList& attrList = objSet->getAttributeList();

    // Header
	std::string line = "ID";
	for(unsigned int i = 0; i < attrList.size(); ++i)
	{
		line += sep;
		line += attrList[i].rep_.name_;
	}

	// Centroid attribute
	line += sep;
	line += "COORDX";
	line += sep;
	line += "COORDY";

	csv.writeString(line);
	csv.writeNewLine();

	double v;
	int doublePrecision = 8;
	TeSTElementSet::iterator it;
	for(it = objSet->begin(); it != objSet->end(); ++it) // for each object
	{
		line = it->getObjectId();
		for(unsigned int i = 0; i < attrList.size(); ++i) // for each attribute
		{
			v = it->operator[](i);
			line += sep;
			line += Te2String(v, doublePrecision);
		}

		// Adds the centroid
		TeCoord2D c = it->getCentroid();
		line += sep;
		line += Te2String(c.x_, doublePrecision);
		line += sep;
		line += Te2String(c.y_, doublePrecision);

		csv.writeString(line);
		csv.writeNewLine();
	}
}

TeSTElementSet* Utils::randomize(TeSTElementSet* objSet)
{
	TeSTElementSet* result = new TeSTElementSet(objSet->getBox(), objSet->getAttributeList());
	
	std::string v;
	TeSTElementSet::iterator it;
	for(it = objSet->begin(); it != objSet->end(); ++it) // for each object
	{	
		std::string id = it->getObjectId();

		// Builds a new random object
		TeSTInstance newObj;
		newObj.setObjectId(id);

		// Adds the geo location
		TePoint p(it->getCentroid());
		p.objectId(id);
		newObj.addGeometry(p);

		// ... randomize the attributes ...
		int n = rand() % objSet->numElements();
		TeSTInstance* theChoosen = objSet->getSTInstance(n);
		for(unsigned int i = 0; i < objSet->getAttributeList().size(); ++i) // for each attribute
		{
			theChoosen->getPropertyValue(v, i);
			newObj.addPropertyValue(v);
		}

		result->insertSTInstance(newObj);
	}
	
	return result;
}

TeSTElementSet* Utils::randomize2(TeSTElementSet* objSet)
{
	TeSTElementSet* result = new TeSTElementSet(objSet->getBox(), objSet->getAttributeList());
	
	std::string v;
	TeSTElementSet::iterator it;
	for(it = objSet->begin(); it != objSet->end(); ++it) // for each object
	{	
		// Builds a new random object
		TeSTInstance newObj;
		newObj.setObjectId(it->getObjectId());
		for(unsigned int i = 0; i < objSet->getAttributeList().size(); ++i) // for each attribute
		{
			it->getPropertyValue(v, i);
			newObj.addPropertyValue(v);
		}

		// ... randomize the centroid ...
		int n = rand() % objSet->numElements();
		TeSTInstance* theChoosen = objSet->getSTInstance(n);

		TePoint p(theChoosen->getCentroid());
		p.objectId(it->getObjectId());
		newObj.addGeometry(p);
		result->insertSTInstance(newObj);
	}
	
	return result;
}

TeSTElementSet* Utils::getElementSetFromCSV(const std::string& path, const char& sep)
{
    TeAsciiFile csv(path);

    // Reads header (attributes)
    std::vector<std::string> attrs;
    csv.readStringListCSV(attrs, sep);
    csv.findNewLine();

    TeAttributeList attrList;
    for(unsigned int i = 0; i < attrs.size() - 2; ++i)
    {
        TeAttribute attr;
        attr.rep_.name_ = attrs[i];
        attr.rep_.type_ = TeSTRING;
        attrList.push_back(attr);
    }
    unsigned int xpos = attrs.size() - 2;
    unsigned int ypos = attrs.size() - 1;

    TeSTElementSet* objSet = new TeSTElementSet(TeBox(), attrList);
    while(csv.isNotAtEOF())
    {   
        std::vector<std::string> values;
        csv.readStringListCSV(values, sep);
        if(values.empty())
            break;
        
        TeSTInstance obj;
        obj.setObjectId(values[0]);
        for(unsigned int i = 0; i < values.size() - 2; ++i)
            obj.addPropertyValue(values[i]);

        TePoint p(atof(values[xpos].c_str()), atof(values[ypos].c_str()));
        p.objectId(values[0]);
        obj.addGeometry(p);

        objSet->insertSTInstance(obj);

        csv.findNewLine();
    }

    return objSet;
}

bool Utils::generateMap(TeTheme* t, const std::string& attrName, std::string& errorMessage)
{
    t->resetGrouping();

    TeAttributeRep rep;
    rep.name_ = t->name() + "." + attrName;
    rep.type_ = TeREAL;

    unsigned int slices = 5;

    TeGrouping quantil;
    quantil.groupAttribute_ = rep;
    quantil.groupMode_ = TeQuantil;
    quantil.groupNumSlices_ = slices;

    if(!t->buildGrouping(quantil))
    {
        errorMessage = "";
        return false;
    }

    // Builds the colors...
    std::vector<TeColor> colors = getColors(TeColor(255, 0, 0), TeColor(255, 255, 255), slices);
    for(unsigned int i = 0; i < slices; ++i)
    {
        TeVisual v(TePOLYGONS);
        v.color(colors[i]);
        v.contourColor(TeColor(100, 100, 100));
        t->setGroupingVisual(i+1, &v, TePOLYGONS);
    }

    if(!t->saveGrouping())
    {
        errorMessage = "";
        return false;
    }

    if(!t->saveLegendInCollection())
    {
        errorMessage = "";
        return false;
    }

    return true;
}
