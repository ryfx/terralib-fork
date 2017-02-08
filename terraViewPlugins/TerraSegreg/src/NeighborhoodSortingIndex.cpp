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
	\file NeighborhoodSortingIndex.cpp
	\brief Class that implements the calculus of Spatial Neighborhood Sorting Index.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

// TerraSegreg
#include "NeighborhoodSortingIndex.h"

// TerraLib
#include <TeSTElementSet.h>

NeighborhoodSortingIndex::NeighborhoodSortingIndex() :
SpatialIndex()
{} 

NeighborhoodSortingIndex::NeighborhoodSortingIndex(TeSTElementSet* objSet) :
SpatialIndex(objSet)
{} 

NeighborhoodSortingIndex::~NeighborhoodSortingIndex()
{}

void NeighborhoodSortingIndex::calculate()
{
	// Initializes the necessary variables
	SpatialIndex::init();

	// Auxiliary variable
	std::string stringValue;
	double doubleValue;

	double invTotalPopIntensity = 0.0;
    if(m_sumPopIntensity != 0.0)
        invTotalPopIntensity = 1 / m_sumPopIntensity;

	double totalMean = 0.0;
	std::map<std::string, double>::iterator it;
	for(it = m_totalGroupPopIntensity.begin(); it != m_totalGroupPopIntensity.end(); ++it) // for each group
		totalMean += it->second * m_groupsValues[it->first];
	totalMean *= invTotalPopIntensity;

	double totalVar = 0.0;
	for(it = m_totalGroupPopIntensity.begin(); it != m_totalGroupPopIntensity.end(); ++it) // for each group
	{
		double f = (m_groupsValues[it->first] - totalMean) * 
				   (m_groupsValues[it->first] - totalMean);
		
		totalVar += it->second * f;
	}
	totalVar *= invTotalPopIntensity;
	totalMean *= totalMean;

    double globalIndex = 0.0;
    if(totalVar == 0.0)
    {
        m_globalIndexes.push_back(globalIndex);
        return;
    }

	double sumVentrePond = 0.0;
	TeSTElementSet::iterator itObj; // Objects iterator
	for(itObj = m_objSet->begin(); itObj != m_objSet->end(); ++itObj) // for each object
	{	
        if(m_totalPopIntensity[itObj->objectId()] == 0.0)
            continue;

		double groupValue;
		double mean = 0.0;
		std::map<std::string, std::string>::iterator it;
		for(it = m_groups.begin(); it != m_groups.end(); ++it) // for each group
		{
			// Gets the group value given by user
			groupValue = m_groupsValues[it->second];
			
			// Local Population Intensity
			itObj->getPropertyValue(it->second, stringValue);
			doubleValue = atoi(stringValue.c_str());

			mean += doubleValue * groupValue;
		}
		mean /= m_totalPopIntensity[itObj->objectId()];
		double pond = m_totalPopIntensity[itObj->objectId()] * invTotalPopIntensity;
		double ventre = (mean * mean) - totalMean;
		double ventrePond = ventre * pond;
		sumVentrePond += ventrePond;
	}

	globalIndex = sumVentrePond / totalVar;
	m_globalIndexes.push_back(globalIndex);
}

void NeighborhoodSortingIndex::setGroupValue(const std::string& group, const double& v)
{
	m_groupsValues[group] = v;
}
