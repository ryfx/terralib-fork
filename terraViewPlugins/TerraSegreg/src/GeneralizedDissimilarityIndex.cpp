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
	\file GeneralizedDissimilarityIndex.cpp
	\brief Class that implements the calculus of Generalized Spatial Dissimilarity Index.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

// TerraSegreg
#include "GeneralizedDissimilarityIndex.h"

// TerraLib
#include <TeSTElementSet.h>

GeneralizedDissimilarityIndex::GeneralizedDissimilarityIndex() :
SpatialIndex()
{
	m_indexAttributeName = "d_m";
}

GeneralizedDissimilarityIndex::GeneralizedDissimilarityIndex(TeSTElementSet* objSet) :
SpatialIndex(objSet)
{
	m_indexAttributeName = "d_m";
} 

GeneralizedDissimilarityIndex::~GeneralizedDissimilarityIndex()
{}

void GeneralizedDissimilarityIndex::calculate()
{
	// Initializes the necessary variables
	SpatialIndex::init();

	// Auxiliary variable
	std::string stringValue;
	
	double I = calculateI();

    double invTotalPopulation = 0.0;
    if(m_totalPopulation != 0.0)
        invTotalPopulation = 1 / m_totalPopulation;

    double globalIndex = 0.0;
	
	TeSTElementSet::iterator itObj; // Objects iterator
	for(itObj = m_objSet->begin(); itObj != m_objSet->end(); ++itObj) // for each object
	{	
		double localIndex = 0.0;
        if(m_totalPopIntensity[itObj->objectId()] != 0.0 && m_totalPopulation != 0.0 && I != 0.0)
        {
	        std::map<std::string, std::string>::iterator it;
	        for(it = m_groups.begin(); it != m_groups.end(); ++it) // for each group
	        {
		        // Local Population Intensity
		        itObj->getPropertyValue(it->second, stringValue);
		        double lpi = atof(stringValue.c_str());

		        // Equation of Generalized Spatial Dissimilarity Index
		        double f = lpi / m_totalPopIntensity[itObj->objectId()];
		        double h = abs(f - (m_groupPopulation[it->first] * invTotalPopulation));
		        double result = (m_objPopulation[itObj->objectId()] * h) / (2 * m_totalPopulation * I);
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

double GeneralizedDissimilarityIndex::calculateI()
{
    if(m_totalPopulation == 0.0)
        return 0.0;

    double I = 0.0;
    double invTotalPopulation = 1 / m_totalPopulation;   
	std::map<std::string, double>::const_iterator it;
	for(it = m_groupPopulation.begin(); it != m_groupPopulation.end(); ++it)
	{
		double p = it->second * invTotalPopulation;
		I +=  p * (1 - p);
	}
	return I;
}
