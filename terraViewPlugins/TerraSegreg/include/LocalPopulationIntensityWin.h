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
	\file LocalPopulationIntensityWin.h  
	\brief Class that implements a user graphic interface to calculates Local Population Intensity.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

#ifndef __TERRASEGREG_PLUGIN_LOCAL_POPULATION_INTENSITY_WIN_H_
#define __TERRASEGREG_PLUGIN_LOCAL_POPULATION_INTENSITY_WIN_H_
  
#include "UILocalPopulationIntensity.h"
#include "LocalPopulationIntensity.h"

// TerraLib
#include <TeDataTypes.h>

// forward declarations
class Help;
class PluginParameters;
class TeProxMatrixWeightsStrategy;
class TeSTElementSet;
class Utils;

class LocalPopulationIntensityWin : public UILocalPopulationIntensity
{
	Q_OBJECT

	/*!
      \enum Page
      \brief User interface pages to Local Population Intensity.
     */
	enum Page
	{
		Input   = 0,
		Kernel  = 1, 
		Options = 2
	};

	public:
   
		/** @name Constructor/Destructor
		*  Initilizer methods.
		*/
		//@{
		
		/*! \brief Constructor. */
		LocalPopulationIntensityWin(PluginParameters* pp);
    
		/*! \brief Destructor. */
		~LocalPopulationIntensityWin();

		//@}
    
	public slots:

		void showWindow();
		void themeComboBox_activated(const QString& name);
		void directoryPushButton_clicked();
		void okPushButton_clicked();
		void helpPushButton_clicked();
		void randomDistributionsCheckBox_clicked();

	private:

		LocalPopulationIntensity::ProxFunction getWStrategy();

		unsigned int numberOfRandomDistributions();

	private:

		PluginParameters* m_pp;
		Utils* m_utils;
		Help* m_help;
};

#endif // __TERRASEGREG_PLUGIN_LOCAL_POPULATION_INTENSITY_WIN_H_
