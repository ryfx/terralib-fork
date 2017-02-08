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
	\file SpatialIndex.h  
	\brief Base class for calculus of TerraSegreg indexes.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

#ifndef __TERRASEGREG_PLUGIN_SPATIAL_INDEX_H_
#define __TERRASEGREG_PLUGIN_SPATIAL_INDEX_H_

// TerraLib
#include <TeDataTypes.h>

// STL
#include <map>
#include <string>

// forward declarations
class TeSTElementSet;

class SpatialIndex
{	
	public:
   
		/** @name Constructor/Destructor
		*  Initilizer methods.
		*/
		//@{
		
		/*! \brief Constructor. */
		SpatialIndex();

		/*! \brief Constructor. */
		SpatialIndex(TeSTElementSet* objSet);
    
		/*! \brief Virtual destructor. */
		virtual ~SpatialIndex();

		//@}
		
		/** @name Methods related with the calculus of TerraSegreg Indexes.
		*/
		//@{
		
		/*! \brief Adds a population group and its respective Local Population Intensity. */
		virtual void addGroup(const std::string& group, const std::string& groupLocalPopulationIntenstity);

		/*! \brief Initializes and calculates the common parameters for all TerraSegreg indexes. */
		virtual void init();

        /*! \brief Clear the the common parameters. */
        virtual void clear();

		/*! \brief Pure virtual method that should be implemented by specialized indexes. */
		virtual void calculate() = 0;
		
		//@}

		/** @name Access Methods
         *  Methods for getting/setting Spatial indexes attributes.
         */
        //@{

		/*! \brief Sets the objects that will be used on indexes calculus. */
        virtual void setObjSet(TeSTElementSet* objSet);

		/*! \brief Sets the attribute name that will store the local index values for each object. */
		virtual void setIndexAttributeName(const std::string& name);

		/*! \brief Gets the attribute name that will store the local index values for each object. */
		std::string getIndexAttributeName();

		/*! \brief Return the global index value. */
        virtual double getGlobalIndex();

		/*! \brief Calculates the p-value based on global indexes. */
        virtual double getpValue();
		
		//@}
		
	protected:

		TeSTElementSet* m_objSet; //<! The set of objects that will be used.

		//<! A map that associates a specific group with its Local Population Intensity attribute.
		std::map<std::string, std::string> m_groups;

		//<! The attribute name that will store the local index values for each object.
		std::string m_indexAttributeName;
		
		std::map<std::string, double> m_objPopulation;     //<! The total population for each object. 
		std::map<std::string, double> m_totalPopIntensity; //<! The total Local Population Intensity for each object.	
		
		std::map<std::string, double> m_groupPopulation;   //<! The total population for each group.

		std::map<std::string, double> m_totalGroupPopIntensity; //<! The total Local Population Intensity for each group.
		
		double m_totalPopulation;						   //<! The total population.
		double m_sumPopIntensity;						   //<! The total Local Population Intensity.

		std::vector<double> m_globalIndexes; //<! The Global Indexes.
};

#endif // __TERRASEGREG_PLUGIN_SPATIAL_INDEX_H_
