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
	\file TeProxMatrixBiSquaredStrategy.h  
	\brief Implementation of Bi-squared weight strategy to proximity matrix.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

#ifndef __TERRASEGREG_PLUGIN_TE_PROX_MATRIX_BISQUARED_STRATEGY_H
#define __TERRASEGREG_PLUGIN_TE_PROX_MATRIX_BISQUARED_STRATEGY_H

// TerraLib
#include <TeProxMatrixWeightsStrategy.h>

class TeProxMatrixBiSquaredStrategy : public TeProxMatrixWeightsStrategy
{
	public:
	
		/** @name Constructor/Destructor
		*  Initilizer methods.
		*/
		//@{

		/*! \brief Constructor. */
		TeProxMatrixBiSquaredStrategy(const double& b = 0.0); 
	    
		/*! \brief Destructor. */
		~TeProxMatrixBiSquaredStrategy();

		//@}

		bool ComputeWeigths(TeProxMatrixImplementation* imp); 	
};

#endif // __TERRASEGREG_PLUGIN_TE_PROX_MATRIX_BISQUARED_STRATEGY_H

