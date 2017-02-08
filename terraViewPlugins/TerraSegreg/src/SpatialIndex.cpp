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
	\file SpatialIndex.cpp
	\brief Base class for calculus of TerraSegreg indexes.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

// TerraSegreg
#include "SpatialIndex.h"

// TerraLib
#include <TeSTElementSet.h>

SpatialIndex::SpatialIndex() :
m_objSet(0),
m_totalPopulation(0.0),
m_sumPopIntensity(0.0)
{}

SpatialIndex::SpatialIndex(TeSTElementSet* objSet) :
m_objSet(objSet),
m_totalPopulation(0.0),
m_sumPopIntensity(0.0)
{} 

SpatialIndex::~SpatialIndex()
{}

void SpatialIndex::init()
{
    clear();

	// Auxiliaries variables
	std::string stringValue; double doubleValue;

	TeSTElementSet::iterator itObj; // Objects iterator
	std::map<std::string, std::string>::iterator it; // Attributes iterator

	for(it = m_groups.begin(); it != m_groups.end(); ++it) // for each group
	{
		double gp = 0.0;	
		for(itObj = m_objSet->begin(); itObj != m_objSet->end(); ++itObj) // for each object
		{	
			// Gets the group value
			itObj->getPropertyValue(it->first, stringValue);
			doubleValue = atof(stringValue.c_str());

			// Object population
			m_objPopulation[itObj->objectId()] += doubleValue;
			// Group population
			gp += doubleValue;
			// Total population
			m_totalPopulation += doubleValue;
			
			// Gets the group Local Population Intensity
			itObj->getPropertyValue(it->second, stringValue);
			doubleValue = atof(stringValue.c_str());
			
			// Local Population Intensity for each object.
			m_totalPopIntensity[itObj->objectId()] += doubleValue;
			m_totalGroupPopIntensity[it->second] += doubleValue;
			m_sumPopIntensity += doubleValue;		
		}
		m_groupPopulation[it->first] = gp;
	}
}

void SpatialIndex::addGroup(const std::string& group, const std::string& groupLocalPopulationIntenstity)
{
	m_groups[group] = groupLocalPopulationIntenstity;
}

void SpatialIndex::clear()
{
    m_objPopulation.clear();
    m_totalPopIntensity.clear();
    m_groupPopulation.clear();
    m_totalPopulation = 0.0;
}

void SpatialIndex::setObjSet(TeSTElementSet* objSet)
{
    m_objSet = objSet;
}

void SpatialIndex::setIndexAttributeName(const std::string& name)
{
	if(name.empty())
		return;
	m_indexAttributeName = name;
}

std::string SpatialIndex::getIndexAttributeName()
{
	return m_indexAttributeName;
}

double SpatialIndex::getGlobalIndex()
{
    return m_globalIndexes[0];
}

double SpatialIndex::getpValue()
{
    double g = m_globalIndexes[0];
    unsigned int gratherIndexes = 0;
    for(unsigned int i = 1; i < m_globalIndexes.size(); ++i)
    {
        if(m_globalIndexes[i] >= g)
            gratherIndexes++;
    }

    double pvalue = (gratherIndexes + 1) / (double)((m_globalIndexes.size() - 1) + 1);

    return pvalue;
}
