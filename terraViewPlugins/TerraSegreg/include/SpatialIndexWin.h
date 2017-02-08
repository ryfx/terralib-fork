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
	\file SpatialIndexWin.h  
	\brief Base class that implements an user graphic interface to calculates the Spatial Indexes.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

#ifndef __TERRASEGREG_PLUGIN_SPATIAL_INDEX_WIN_H_
#define __TERRASEGREG_PLUGIN_SPATIAL_INDEX_WIN_H_
  
#include "UISpatialIndex.h"

// STL
#include <map>

// forward declarations
class Help;
class PluginParameters;
class Utils;
class SpatialIndex;
class TeSTElementSet;

class SpatialIndexWin : public UISpatialIndex
{
	Q_OBJECT

	/*!
      \enum Page
      \brief User interface pages to Spatial Indexes.
     */
	enum Page
	{
		Input   = 0,
		Options = 1,
		Results = 2
	};

	public:
   
		/** @name Constructor/Destructor
		*  Initilizer methods.
		*/
		//@{
		
		/*! \brief Constructor. */
		SpatialIndexWin(PluginParameters* pp);
    
		/*! \brief Destructor. */
		virtual ~SpatialIndexWin();

		//@}
    
	public slots:
	
		virtual void showWindow();
        virtual void verifyPushButton_clicked();
		virtual void okPushButton_clicked();
        virtual void themeComboBox_activated(const QString& name);
        virtual void directoryPushButton_clicked();
        virtual void pvalueCheckBox_clicked();
		virtual void helpPushButton_clicked();
	
	protected:

		/*! \brief Pure virtual method that should be implemented by specialized indexes user interfaces. 
				   Basically, this method is responsable to verifiy and set the specified 
				   attributes for each Spatial Index. 
		*/
		virtual bool configureSpatialIndex() = 0;
		
		/*! \brief Pure virtual method that should be implemented by specialized indexes user interfaces. 
				   Basically, this method is responsable to persist the results.
				   For example, updates the theme table, generates the map of local indexes, etc.
		*/
		virtual void update(TeSTElementSet* objSet) = 0;

	protected:
	
		PluginParameters* m_pp;						 //<! TerraView plugin parameters.
		Utils* m_utils;							     //<! Utility class.
		SpatialIndex* m_spatialIndex;				 //<! The spatial index that will be calculated.
		std::map<std::string, std::string> m_groups; //<! The population groups that will be used.
		Help* m_help;							     //<! Help interface.
		std::string m_helpPath;						 //<! The path to help file.
};

#endif // __TERRASEGREG_PLUGIN_SPATIAL_INDEX_WIN_H_
