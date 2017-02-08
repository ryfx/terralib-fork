/************************************************************************************
TerraLib - a library for developing GIS applications.
Copyright © 2001-2004 INPE and Tecgraf/PUC-Rio.

This code is part of the TerraLib library.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

You should have received a copy of the GNU Lesser General Public
License along with this library.

The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The library provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use
of this library and its documentation.
*************************************************************************************/

#include "TeKernelInterpolation.h"

#include "TeKdTree.h"
#include "TeSTElementSet.h"
#include "TeSTEFunctionsDB.h"
#include "TeQuerierParams.h"
#include "TeQuerier.h"
#include "TeVectorRemap.h"


//! Builds a KDTree from a theme with samples: the theme must have a point representation
/*
		\param sampleTheme          The theme with samples
		\param sampleAttrTableName  Theme table with samples
		\param sampleColumnName     Table column with sample values
		\param tree				    The tree to store samples
		\param destProjection		Destination projection
		\return TRUE if the tree is built correct, otherwise returns FALSE
 */
template<class KDTREE> bool TeBuildKDTree(TeTheme* sampleTheme, const string& sampleAttrTableName,
										  const string& sampleColumnName, KDTREE& tree, TeProjection* destProjection)
{
// verifies if we need to reproject the samples
	bool doReprojection = false;

	TeProjection* sampleProj = sampleTheme->layer()->projection();

	if(sampleProj && destProjection && (!((*sampleProj) == (*destProjection))))
		doReprojection = true;

// if it is necessary to reproject, reproject the tree's box first
    if(doReprojection)
	{
		TeBox baux = tree.getBox();
		TeBox b = TeRemapBox(baux, sampleProj, destProjection);

		tree.setBox(b);
	}


	vector<pair<TeCoord2D, TeSample> > dataSet;

// load sample points
	bool loadGeometries = true;

    vector<string> attributes;

	string attName = sampleAttrTableName + "." + sampleColumnName;

    attributes.push_back(attName);

	TeQuerierParams querierParams(loadGeometries, attributes);

	querierParams.setParams(sampleTheme);

	TeQuerier querier(querierParams);

// load instances from theme based in the querier parameters
	if(!querier.loadInstances())
		return false;
	
	TeSTInstance sti;	

// traverse all the instances
	while(querier.fetchInstance(sti))	
	{
		TeProperty prop;

// retrieves sample values
		sti.getProperty(prop, attName);    
		
		if(sti.hasPoints())
		{
			TePointSet pointSet;

			sti.getGeometry(pointSet);

			TeCoord2D c = pointSet[0].location();

// if it is necessary, do reprojection
			if(doReprojection)
			{
				TeCoord2D caux;
				TeVectorRemap(c, sampleProj, caux, destProjection);
				c = caux;
			}

			TeSample sample(c, atof(prop.value_.c_str()));

// put samples into auxiliary vector to build the tree
			dataSet.push_back(pair<TeCoord2D, TeSample>(sample.location(), sample));
		}
	}

	tree.build(dataSet);

	return true;
}

bool TeRasterInterpolate(TeTheme* inputTheme, const string& inputAttrTableName, const string& sampleColumnName,
                         TeRaster* outputRaster, const int& band,
				         const TeKernelInterpolationAlgorithm& algorithm,
						 const TeKernelInterpolationMethod& method,
                         const unsigned int& numberOfNeighbors, const double& boxRatio)
{
	if((inputTheme == 0) || (outputRaster == 0))
		return false;

	if(inputAttrTableName.empty())
		return false;

	if(sampleColumnName.empty())
		return false;	

// load tree with samaples
	typedef TeSAM::TeAdaptativeKdTreeNode<TeCoord2D, vector<TeSample>, TeSample> KDNODE;
	typedef TeSAM::TeAdaptativeKdTree<KDNODE> KDTREE;

// A minimum of MINBUCKETSIZE elements in each bucket
	unsigned int bucketSize = 2 * numberOfNeighbors;

	TeBox mbr = inputTheme->layer()->box();

	KDTREE tree(mbr, bucketSize);

	if(!TeBuildKDTree(inputTheme, inputAttrTableName, sampleColumnName, tree, outputRaster->projection()))
		return false;

	if(tree.size() == 0)
		return false;

	TeKernelInterpolationAlgorithms<KDTREE> interpolationObj(tree);
	
	int nLines = outputRaster->params().nlines_;
	int nCols = outputRaster->params().ncols_;

	double value = -TeMAXFLOAT;

	if(algorithm == TeDistWeightAvgInterpolation)
	{
		for(int i = 0; i < nLines; ++i)
			for(int j = 0; j < nCols; ++j)
			{
				TeCoord2D cl(j, i);

				TeCoord2D c = outputRaster->index2Coord(cl);

				value = interpolationObj.distWeightAvgNearestNeighbor(c, numberOfNeighbors, method);

                outputRaster->setElement(j, i, value, band);
			}
	}
	else if(algorithm == TeDistWeightAvgInBoxInterpolation)
	{
		for(int i = 0; i < nLines; ++i)
			for(int j = 0; j < nCols; ++j)
			{
				TeCoord2D cl(j, i);

				TeCoord2D c = outputRaster->index2Coord(cl);

				TeBox b(c.x_ - boxRatio, c.y_ - boxRatio, c.x_ + boxRatio, c.y_ + boxRatio);

				value = interpolationObj.boxDistWeightAvg(c, b, method);

	            outputRaster->setElement(j, i, value, band);
			}
	}
	else
		return false;


    return true;
}
