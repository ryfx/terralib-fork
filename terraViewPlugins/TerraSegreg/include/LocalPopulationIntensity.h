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
	\file LocalPopulationIntensity.h  
	\brief Class that implements the calculus of Local Population Intensity based on neighborhood.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

#ifndef __TERRASEGREG_PLUGIN_LOCAL_POPULATION_INTENSITY_H_
#define __TERRASEGREG_PLUGIN_LOCAL_POPULATION_INTENSITY_H_

// TerraLib
#include <TeDataTypes.h>
#include <TeGeneralizedProxMatrix.h>
#include <TeProxMatrixConstructionStrategy.h>
#include <TeSTElementSet.h>

// STL
#include <string>

// forward declarations
class TeProxMatrixWeightsStrategy;

class LocalPopulationIntensity
{
	public:
		
		/*!
		  \enum ProxFunction
		  \brief Proximity functions type.
		 */
		enum ProxFunction
		{
			MovingWindow   = 0,
			Gaussian	   = 1, 
			BiSquared      = 2
		};
	
	public:
   
		/** @name Constructor/Destructor
		*  Initilizer methods.
		*/
		//@{
		
		/*! \brief Constructor. */
		LocalPopulationIntensity(const double& bandwidth, const ProxFunction& f, const std::string& suffix = "_W");
    
		/*! \brief Destructor. */
		~LocalPopulationIntensity();

		//@}
		
		/** @name Methods related with calculus of Local Population Intensity.
		*/
		//@{
		
		/*! \brief Initializes and builds the matrices given a set of geographic objects. */
		void init(TeSTElementSet* objSet, const double& distance, const TeGeomRep& geomRep);
		
		/*! \brief Calculates Local Population Intensity given a set of geographic objects. */
		void calculate(TeSTElementSet* objSet);

		/*! \brief Builds a well known string with Local Population Intensity configuration. 
				   Format:   proxFunctionName_bandwidth_yyyy-mm-dd_hh_mm
				   Examples: gaussian_5000_2011-02-19_18_37 / bisquared_3000_2011-02-19_23_59
		*/
		void getDescription(std::string& description);
		
		//@}

	private:

		void initWeightStrategy();
		
	private:
	
		TeGeneralizedProxMatrix<TeSTElementSet>* m_proxMatrix;		//<! Generalized Proximity Matrix.
		TeProxMatrixConstructionStrategy<TeSTElementSet>* m_cs; 	//<! Matrix Construction Strategy that uses distance.
		TeProxMatrixWeightsStrategy* m_ws; 							//<! Matrix Weight Strategy.
		
		double m_bandwidth;	  //<! The bandwidth used to define the neighborhood and weights.
		ProxFunction m_f;	  //<! Proximity function type.
		
		std::string m_suffix; //<! Suffix that will be added to generated attributes.
};

#endif // __TERRASEGREG_PLUGIN_LOCAL_POPULATION_INTENSITY_H_
