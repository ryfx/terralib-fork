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
	\file GeneralizedDissimilarityIndex.h  
	\brief Class that implements the calculus of Generalized Spatial Dissimilarity Index.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

#ifndef __TERRASEGREG_PLUGIN_GENERALIZED_DISSIMILARITY_INDEX_H_
#define __TERRASEGREG_PLUGIN_GENERALIZED_DISSIMILARITY_INDEX_H_

// TerraSegreg
#include "SpatialIndex.h"

// STL
#include <map>
#include <string>

// forward declarations
class TeSTElementSet;

class GeneralizedDissimilarityIndex : public SpatialIndex
{	
	public:
   
		/** @name Constructor/Destructor
		*  Initilizer methods.
		*/
		//@{
		
		/*! \brief Constructor. */
		GeneralizedDissimilarityIndex();

		/*! \brief Constructor. */
		GeneralizedDissimilarityIndex(TeSTElementSet* objSet);
    
		/*! \brief Destructor. */
		virtual ~GeneralizedDissimilarityIndex();

		//@}
		
		/** @name Methods related with the calculus of TerraSegreg Indexes.
		*/
		//@{
		
		virtual void calculate();
		
		//@}
		
		/** @name Access Methods
         *  Methods for getting/setting Generalized Dissimilarity Index attributes.
         */
        //@{

		//@}
		
	protected:
	
		/** @name Methods related with calculus of Generalized Dissimilarity Index.
		*/
		//@{
		
		/*! \brief Calculates the constant I of Generalized Dissimilarity Index Equation. */
		double calculateI();
		
		//@}
};

#endif // __TERRASEGREG_PLUGIN_GENERALIZED_DISSIMILARITY_INDEX_H_
