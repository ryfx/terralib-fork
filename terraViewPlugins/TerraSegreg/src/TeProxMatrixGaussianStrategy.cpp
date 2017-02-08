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
	\file TeProxMatrixGaussianStrategy.cpp
	\brief Implementation of Gaussian weight strategy to proximity matrix.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

// TerraSegreg
#include <TeProxMatrixGaussianStrategy.h>

// Math
#include <cmath>

TeProxMatrixGaussianStrategy::TeProxMatrixGaussianStrategy(const double& b) 
	: TeProxMatrixWeightsStrategy()
{
	params_.max_local_dist_ = b;
}
    
TeProxMatrixGaussianStrategy::~TeProxMatrixGaussianStrategy()
{}

bool TeProxMatrixGaussianStrategy::ComputeWeigths(TeProxMatrixImplementation* imp)
{
	if(imp == 0) 
		return false;

	double b = params_.max_local_dist_; // band width
	double d = 0.0; // to get the centroids distances...
	
	int n = imp->numberOfObjects();
	for(int i = 0; i < n; ++i) // for each object
	{
		TeNeighbours neighbours;
		std::string objectid;
		if(imp->getNeighbours(i, objectid, neighbours))
		{
			int size = neighbours.size();
			for(int j = 0; j < size; ++j) // for each neighbour
			{
				TeProxMatrixAttributes attr = neighbours.Attributes(j);
				if(!attr.WasCentroidDistanceComputed())
					continue;
				
				// Gets the distance
				d = attr.CentroidDistance();
				if(d == 0.0) continue;

				// Calculates the weight from gaussian strategy
				double w = exp(-0.5 * pow((d / b), 2));
				// ... and updates the neighbour
				attr.Weight(w);
				imp->setConnectionAttributes(objectid, neighbours[j], attr);
			}
		}
	}
	return true;
}
