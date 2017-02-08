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
	\file NeighborhoodSortingIndex.h  
	\brief Class that implements the calculus of Spatial Neighborhood Sorting Index.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

#ifndef __TERRASEGREG_PLUGIN_NEIGHBORHOOD_SORTING_INDEX_H_
#define __TERRASEGREG_PLUGIN_NEIGHBORHOOD_SORTING_INDEX_H_

// TerraSegreg
#include "SpatialIndex.h"

// STL
#include <map>
#include <string>

// forward declarations
class TeSTElementSet;

class NeighborhoodSortingIndex : public SpatialIndex
{	
	public:
   
		/** @name Constructor/Destructor
		*  Initilizer methods.
		*/
		//@{
		
		/*! \brief Constructor. */
		NeighborhoodSortingIndex();

		/*! \brief Constructor. */
		NeighborhoodSortingIndex(TeSTElementSet* objSet);
    
		/*! \brief Destructor. */
		virtual ~NeighborhoodSortingIndex();

		//@}
		
		/** @name Methods related with the calculus of TerraSegreg Indexes.
		*/
		//@{
		
		virtual void calculate();
		
		//@}

		/** @name Access Methods
         *  Methods for getting/setting Spatial Neighborhood Sorting Index attributes.
         */
        //@{

		void setGroupValue(const std::string& group, const double& v);

		//@}
	
	protected:
		
		std::map<std::string, double> m_groupsValues; //<! The values for each population group given by user.
};

#endif // __TERRASEGREG_PLUGIN_NEIGHBORHOOD_SORTING_INDEX_H_
