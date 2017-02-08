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
	\file IsolationIndexM.cpp 
	\brief Class that implements the calculus of Spatial Isolation Index of Group m.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

// TerraSegreg
#include "IsolationIndexM.h"

// TerraLib
#include <TeSTElementSet.h>

IsolationIndexM::IsolationIndexM() :
SpatialIndex()
{
	m_indexAttributeName = "q_m";
} 

IsolationIndexM::IsolationIndexM(TeSTElementSet* objSet) :
SpatialIndex(objSet)
{
	m_indexAttributeName = "q_m";
} 

IsolationIndexM::~IsolationIndexM()
{}

void IsolationIndexM::calculate()
{
	// Initializes the necessary variables
	SpatialIndex::init();

	// Auxiliary variable
	std::string stringValue;
	double doubleValue;

	double globalIndex = 0.0;	
	double invMPopulation = 0.0;
    if(m_groupPopulation[m_groupM] != 0.0)
        invMPopulation = 1 / m_groupPopulation[m_groupM];

	std::string lpiGroupM = m_groups[m_groupM];

	TeSTElementSet::iterator itObj; // Objects iterator
	for(itObj = m_objSet->begin(); itObj != m_objSet->end(); ++itObj) // for each object
	{	
        double localIndex = 0.0;
        if(m_totalPopIntensity[itObj->objectId()] != 0.0)
        {
		    itObj->getPropertyValue(lpiGroupM, stringValue);
		    doubleValue = atof(stringValue.c_str());
		    double pondM = doubleValue / m_totalPopIntensity[itObj->objectId()];

		    itObj->getPropertyValue(m_groupM, stringValue);
		    doubleValue = atof(stringValue.c_str());
		    double f = doubleValue * invMPopulation;

		    localIndex = pondM * f;
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

void IsolationIndexM::setGroupM(const std::string& m)
{
	m_groupM = m;
}
