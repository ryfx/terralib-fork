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
	\file NeighborhoodSortingIndexWin.h  
	\brief Class that implements an user graphic interface to calculates the Spatial Neighborhood Sorting Index.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

#ifndef __TERRASEGREG_PLUGIN_NEIGHBORHOOD_SORTING_INDEX_WIN_H_
#define __TERRASEGREG_PLUGIN_NEIGHBORHOOD_SORTING_INDEX_WIN_H_
  
// TerraSegreg
#include "SpatialIndexWin.h"

class NeighborhoodSortingIndexWin : public SpatialIndexWin
{
	Q_OBJECT

	public:
   
		/** @name Constructor/Destructor
		*  Initilizer methods.
		*/
		//@{
		
		/*! \brief Constructor. */
		NeighborhoodSortingIndexWin(PluginParameters* pp);
    
		/*! \brief Destructor. */
		~NeighborhoodSortingIndexWin();

		//@}
    
	public slots:

        void verifyPushButton_clicked();

	protected:

		bool configureSpatialIndex();
		void update(TeSTElementSet* objSet);
};

#endif // __TERRASEGREG_PLUGIN_NEIGHBORHOOD_SORTING_INDEX_WIN_H_
