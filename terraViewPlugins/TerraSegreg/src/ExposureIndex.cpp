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
	\file ExposureIndex.cpp
	\brief Class that implements the calculus of Generalized Spatial Exposure Index.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

// TerraSegreg
#include "ExposureIndex.h"

// TerraLib
#include <TeSTElementSet.h>

ExposureIndex::ExposureIndex() :
SpatialIndex()
{
	m_indexAttributeName = "p_m";
} 


ExposureIndex::ExposureIndex(TeSTElementSet* objSet) :
SpatialIndex(objSet)
{
	m_indexAttributeName = "p_m";
} 

ExposureIndex::~ExposureIndex()
{}

void ExposureIndex::calculate()
{
	// Initializes the necessary variables
	SpatialIndex::init();

	// Auxiliary variable
	std::string stringValue;

	double globalIndex = 0.0;
	double invTotalPopulation = 0.0;
    if(m_totalPopulation != 0.0)
        invTotalPopulation = 1 / m_totalPopulation;

	// Calculates the Groups Proportions
	std::map<std::string, double> groupsProportions;
	std::map<std::string, double>::iterator it;
	for(it = m_groupPopulation.begin(); it != m_groupPopulation.end(); ++it)
		groupsProportions[it->first] = it->second * invTotalPopulation;

	TeSTElementSet::iterator itObj; // Objects iterator
	for(itObj = m_objSet->begin(); itObj != m_objSet->end(); ++itObj) // for each object
	{	
		double pond = m_objPopulation[itObj->objectId()] * invTotalPopulation;
		double totalPopIntensity = m_totalPopIntensity[itObj->objectId()];

		double localIndex = 0.0;
        
        if(totalPopIntensity != 0.0)
        {
		    std::map<std::string, std::string>::iterator it;
		    for(it = m_groups.begin(); it != m_groups.end(); ++it) // for each group
		    {
			    // Local Population Intensity
			    itObj->getPropertyValue(it->second, stringValue);

			    // Equation of Generalized Spatial Exposure Index
			    double num = atof(stringValue.c_str()) / totalPopIntensity;
			    num = groupsProportions[it->first] - num;
			    num *= num;
			    double den = 1 - groupsProportions[it->first];
			    double result = pond * (num / den);
			    localIndex += result;
		    }
        }
		itObj->addPropertyValue(Te2String(localIndex));
		globalIndex += localIndex;
	}
	
	m_globalIndexes.push_back(globalIndex);

	// Adds the local index attribute
	TeAttribute attrIndex;
	attrIndex.rep_.name_ = m_indexAttributeName;
	attrIndex.rep_.type_ = TeREAL;
	m_objSet->addProperty(attrIndex);
}
