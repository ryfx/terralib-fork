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
	\file LocalPopulationIntensity.cpp
	\brief Class that implements the calculus of Local Population Intensity based on neighborhood.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

// TerraSegreg
#include "LocalPopulationIntensity.h"
#include "TeProxMatrixBiSquaredStrategy.h"
#include "TeProxMatrixGaussianStrategy.h."

// TerraLib
#include <TeGeneralizedProxMatrix.h>
#include <TeProxMatrixConstructionStrategy.h>
#include <TeProxMatrixWeightsStrategy.h>

LocalPopulationIntensity::LocalPopulationIntensity(const double& bandwidth,
												   const ProxFunction& f, 
												   const std::string& suffix) :
m_proxMatrix(0),
m_cs(0),
m_ws(0),
m_bandwidth(bandwidth),
m_f(f),
m_suffix(suffix)
{
	initWeightStrategy();
} 

LocalPopulationIntensity::~LocalPopulationIntensity()
{
	delete m_proxMatrix;
}

void LocalPopulationIntensity::init(TeSTElementSet* objSet, const double& distance, const TeGeomRep& geomRep)
{
	// Matrix Construction Strategy using distance
	m_cs = new TeProxMatrixLocalDistanceStrategy<TeSTElementSet>(objSet, geomRep, distance);

	// Generalized Proximity Matrix
	m_proxMatrix = new TeGeneralizedProxMatrix<TeSTElementSet>(m_cs, m_ws); 

	// Calculates the neighbours
	m_proxMatrix->constructMatrix();
}

void LocalPopulationIntensity::calculate(TeSTElementSet* objSet)
{
	TeAttributeList attrList = objSet->getAttributeList();
	for(unsigned int i = 0; i < attrList.size(); ++i) // for each attribute
	{
		TeAttribute attw(attrList[i]);
		attw.rep_.name_ += m_suffix;
		attw.rep_.type_ = TeREAL;
		objSet->addProperty(attw);

		TeSTElementSet::iterator it;
		for(it = objSet->begin(); it != objSet->end(); ++it) // for each object
		{
			double v = (*it)[i];
			// Calculates the Local Population Intensity based on neighbours
			TeNeighbours neighbours = m_proxMatrix->getNeighbours(it->getObjectId());
			for(unsigned int j = 0; j < neighbours.size(); ++j) // for each neighbour
			{
				TeProxMatrixAttributes attr = neighbours.Attributes(j);
				double w = attr.Weight();
				TeSTInstance* neighbourObj = objSet->getSTInstance(neighbours[j]);
				v += w * neighbourObj->operator[](i);
			}
			it->addPropertyValue(Te2String(v));
		}
	}
}

void LocalPopulationIntensity::getDescription(std::string& description)
{
	// Adding function name
	switch(m_f)
	{
		case MovingWindow:
			description = "movingwindow";
		break;

		case Gaussian:
			description = "gaussian";
		break;	

		case BiSquared:
			description = "bisquared";
		break;
	}
	
	// Adding m_bandwidth
	description += "_" + Te2String(m_bandwidth, 0);

	TeTime t;
	t.now();

	// Adding Date
	description += "_" + t.getDate();
	// Adding Time
	description += "_" + Te2String(t.hour());
	description += "-" + Te2String(t.minute());
}

void LocalPopulationIntensity::initWeightStrategy()
{
	switch(m_f)
	{
		case MovingWindow:
			m_ws = new TeProxMatrixNoWeightsStrategy();
			m_ws->weightsParams().max_local_dist_ = m_bandwidth;
		break;

		case Gaussian:
			m_ws = new TeProxMatrixGaussianStrategy(m_bandwidth);
		break;

		case BiSquared:
			m_ws = new TeProxMatrixBiSquaredStrategy(m_bandwidth);
		break;
	}
}
