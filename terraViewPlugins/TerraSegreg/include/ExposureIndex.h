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
	\file ExposureIndex.h  
	\brief Class that implements the calculus of Generalized Spatial Exposure Index.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

#ifndef __TERRASEGREG_PLUGIN_EXPOSURE_INDEX_H_
#define __TERRASEGREG_PLUGIN_EXPOSURE_INDEX_H_

// TerraSegreg
#include "SpatialIndex.h"

// STL
#include <map>
#include <string>

// forward declarations
class TeSTElementSet;

class ExposureIndex : public SpatialIndex
{	
	public:
   
		/** @name Constructor/Destructor
		*  Initilizer methods.
		*/
		//@{
		
		/*! \brief Constructor. */
		ExposureIndex();

		/*! \brief Constructor. */
		ExposureIndex(TeSTElementSet* objSet);
    
		/*! \brief Destructor. */
		virtual ~ExposureIndex();

		//@}
		
		/** @name Methods related with the calculus of TerraSegreg Indexes.
		*/
		//@{
		
		virtual void calculate();
		
		//@}
		
		/** @name Access Methods
         *  Methods for getting/setting Generalized Spatial Exposure Index attributes.
         */
        //@{
		
		//@}	
};

#endif // __TERRASEGREG_PLUGIN_EXPOSURE_INDEX_H_
