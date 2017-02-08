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

/*! \file TeKernelInterpolation.h
    This file contains routines for interpolation of raster themes.	
*/

#ifndef  __MERCADO_INTERPOLATION_H
#define  __MERCADO_INTERPOLATION_H

#include "TeTheme.h"


/*
 * NOTE: You should call TeCellInterpolate or TeRasterInterpolate with the required parameters. Don't use
 *       the class TeInterpolationAlgorithms, it is for internal use only!
 *
 */


/** @defgroup InterpolationDataStructure Interpolation Algorithms and Data Structures
 *  TerraLib Interpolation Data Structure.
 *  @{
 */

/*! \enum TeKernelInterpolationAlgorithm

	Algorithms of interpolation, may be:
    - TeDistWeightAvgInterpolation       Interpolation with weight average (inverse of square distance or other) of k-nearest neighbors values
	- TeDistWeightAvgInBoxInterpolation  Interpolation with weight average of elements in box
*/ 
enum TeKernelInterpolationAlgorithm {TeDistWeightAvgInterpolation, TeDistWeightAvgInBoxInterpolation};

/*! \enum TeKernelInterpolationMethod

	Methods of interpolation, may be:
    - TeQuarticKernelMethod
    - TeNormalKernelMethod
	- TeUniformKernelMethod
	- TeTriangularKernelMethod
	- TeNegExpKernelMethod
*/ 
enum TeKernelInterpolationMethod {TeQuarticKernelMethod, TeNormalKernelMethod, TeUniformKernelMethod, TeTriangularKernelMethod, TeNegExpKernelMethod};


template<class ADAPTATIVEKDTREE> class TeKernelInterpolationAlgorithms
{
	protected:

		typename typedef ADAPTATIVEKDTREE::kdKey kdKey;
		typename typedef ADAPTATIVEKDTREE::kdDataItem kdDataItem;
		typename typedef ADAPTATIVEKDTREE::kdNode kdNode;

		//! reference to an adptative kdtree 
		const ADAPTATIVEKDTREE& kd_;

	public:		

		//! Constructor
		TeKernelInterpolationAlgorithms(const ADAPTATIVEKDTREE& kd)
			: kd_(kd)
		{
		}

		//! Destructor
		~TeKernelInterpolationAlgorithms()
		{
		}

		//! Weight Average of Nearest Neighbors. If an error occur returns -TeMAXFLOAT
		double distWeightAvgNearestNeighbor(const kdKey& key, const unsigned int& numberOfNeighbors, const TeKernelInterpolationMethod& method)
		{
			vector<kdDataItem> reportItem;
			vector<double> sqrDists;

			fillNNVector(reportItem, numberOfNeighbors);

			kd_.nearestNeighborSearch(key, reportItem, sqrDists, numberOfNeighbors);

			double adaptativeRatio = 0.;
			for(unsigned int i = 0; i < sqrDists.size(); ++i)
			{
				if(sqrDists[i] > adaptativeRatio)
				{
					adaptativeRatio = sqrDists[i];
				}
			}

			TeBox box(key.x_ - adaptativeRatio, key.y_ - adaptativeRatio, key.x_ + adaptativeRatio, key.y_ + adaptativeRatio);

			vector<kdNode*> report;

			kd_.search(box, report);

			unsigned int numberOfNodes = report.size();

			double value = 0.;

			for(unsigned int i = 0; i < numberOfNodes; ++i)
			{
				unsigned int nodeSize = report[i]->getData().size();

				for(unsigned int j = 0; j < nodeSize; ++j)
				{
					if(TeIntersects(report[i]->getData()[j].location(), box))
					{
						double distance = TeDistance(key, report[i]->getData()[j].location());

						if(method == TeQuarticKernelMethod)
						{
							value += TeKernelQuartic(adaptativeRatio, distance, report[i]->getData()[j].value());
						}
						else if(method == TeNormalKernelMethod)
						{
							value += TeKernelNormal(adaptativeRatio, distance, report[i]->getData()[j].value());
						}
						else if(method == TeUniformKernelMethod)
						{
							value += TeKernelUniform(adaptativeRatio, distance, report[i]->getData()[j].value());
						}
						else if(method == TeTriangularKernelMethod)
						{
							value += TeKernelTriangular(adaptativeRatio, distance, report[i]->getData()[j].value());
						}
						else if(method == TeNegExpKernelMethod)
						{
							value += TeKernelNegExponential(adaptativeRatio, distance, report[i]->getData()[j].value());
						}
					}
				}				
			}

			return value;
		}

		//! Distance Weight Average of Elements in Box. If an error occur returns -TeMAXFLOAT
		double boxDistWeightAvg(const kdKey& key, const TeBox& box, const TeKernelInterpolationMethod& method)
		{
			vector<kdNode*> report;

			kd_.search(box, report);

			unsigned int numberOfNodes = report.size();

			double value = 0.;

			for(unsigned int i = 0; i < numberOfNodes; ++i)
			{
				unsigned int nodeSize = report[i]->getData().size();

				for(unsigned int j = 0; j < nodeSize; ++j)
				{
					double ratio = box.height() / 2;

					if(TeIntersects(report[i]->getData()[j].location(), box))
					{
						double distance = TeDistance(key, report[i]->getData()[j].location());

						if(method == TeQuarticKernelMethod)
						{
							value += TeKernelQuartic(ratio, distance, report[i]->getData()[j].value());
						}
						else if(method == TeNormalKernelMethod)
						{
							value += TeKernelNormal(ratio, distance, report[i]->getData()[j].value());
						}
						else if(method == TeUniformKernelMethod)
						{
							value += TeKernelUniform(ratio, distance, report[i]->getData()[j].value());
						}
						else if(method == TeTriangularKernelMethod)
						{
							value += TeKernelTriangular(ratio, distance, report[i]->getData()[j].value());
						}
						else if(method == TeNegExpKernelMethod)
						{
							value += TeKernelNegExponential(ratio, distance, report[i]->getData()[j].value());
						}
					}
				}				
			}

			return value;
		}

		double TeKernelQuartic(double tau, double distance, double intensity) 
		{
			if (distance > tau)
				return 0.0;

			return intensity * (3.0 / (tau * tau * TePI)) *
				pow(1 - ((distance * distance)/ (tau * tau)),2.0);
		}

		double TeKernelNormal(double tau, double distance, double intensity)
		{
			if (distance > tau)
				return 0.0;

			return intensity * (1.0 / (tau * tau * 2 * TePI)) *
				exp(-1.0 * (distance * distance)/ (2 * tau * tau));
		}

		double TeKernelUniform(double tau, double distance, double intensity)
		{
			if (distance > tau)
				return 0.0;

			return intensity;
		}

		double TeKernelTriangular(double tau, double distance, double intensity)
		{
			if (distance > tau)
				return 0.0;

			return intensity * (1.0 - 1.0/tau) * distance;
		}

		double TeKernelNegExponential(double tau, double distance, double intensity)
		{
			if (distance > tau)
				return 0.0;

			return intensity * exp(-3.0 * distance);
		}

	protected:

		//! Fills the nearest neighbour vector with default values
		void fillNNVector(vector<kdDataItem>& report, const unsigned int& numberOfNeighbors) const
		{
			for(unsigned int i = 0; i < numberOfNeighbors; ++i)
			{
				TeCoord2D c(TeMAXFLOAT, TeMAXFLOAT);
				kdDataItem item(c, -TeMAXFLOAT);

				report.push_back(item);
			}
		}

	private:

		//! No copy allowed
		TeKernelInterpolationAlgorithms(const TeKernelInterpolationAlgorithms& rhs);

		//! No copy allowed
		TeKernelInterpolationAlgorithms& operator=(const TeKernelInterpolationAlgorithms& rhs);		
};

//! Interpolates a a raster
/*
		\param inputTheme			The theme with samples to be used: samples are treated as a layer of points with a column with a value (double or integer)
		\param inputAttrTableName   Theme table with samples
		\param sampleColumnName     Table column with sample values
		\param outputRaster         A pointer to a raster where interpolated data will be stored: this raster mus already exist
		\param band                 The band to be interpolated
		\param algorithm		    Type of algorithm to be used: see enum TeInterpolationAlgorithm
		\param numberOfNeighbors    Number of nearest neighbor to be used in interpolation
		\param boxRatio				Ratio of the box used to search samples
		\return TRUE if there isn't an error during interpolation otherwise return FALSE
 */
bool TeRasterInterpolate(TeTheme* inputTheme, const string& inputAttrTableName, const string& sampleColumnName,
                         TeRaster* outputRaster, const int& band = 0,
				         const TeKernelInterpolationAlgorithm& algorithm = TeDistWeightAvgInBoxInterpolation,
						 const TeKernelInterpolationMethod& method = TeQuarticKernelMethod,
                         const unsigned int& numberOfNeighbors = 1, const double& boxRatio = 1.0);

		

/** @} */ 




#endif	// __MERCADO_INTERPOLATION_H




