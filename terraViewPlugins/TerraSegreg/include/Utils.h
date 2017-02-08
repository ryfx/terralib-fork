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
	\file Utils.h  
	\brief Utility class for TerraSegreg plugin.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

#ifndef __TERRASEGREG_PLUGIN_UTILS_H
#define __TERRASEGREG_PLUGIN_UTILS_H

// TerraLib
#include <TeAttribute.h>

// STL
#include <vector>
#include <string>

// forward declarations
class TeDatabase;
class TeLayer;
class TeSTElementSet;
class TeTheme;
  
class Utils
{
	public:

		/** @name Constructor/Destructor
		*  Initilizer methods.
		*/
		//@{
		
		/*! \brief Constructor. */
		Utils(TeDatabase* db);

		/*! \brief Destructor. */
		~Utils();
		
		//@}

	public:

		/** @name Methods related with TerraLib Layers.
		* Methods for obtainning information about layers.
		*/
		//@{

		/*!
			\brief Gets a list of layers names given a representation.
			\param layers [output] List of layers name from current database.
			\param geomRep [input] A combination of geometric representations (ex.: TePOLYGONS | TeLINES) 
		*/
		void getLayers(std::vector<std::string>& layers, const int& geomRep);

		/*!
			\brief Gets a list of layers names from current database.
			\param layers [output] List of layers name.
		*/
		void getLayers(std::vector<std::string>& layers);

		/*!
			\brief Gets a layer given a name.
			\param name The layer name.
			\return A valid layer pointer if a layer with this name exist in database, 0 otherwise.
		*/
		TeLayer* getLayer(const std::string& name);

		/*!
			\brief Gets a list of tables name given a layer name.
			\param name [input] The layer name.
			\param tables [output] List of tables name from layer.
		*/
		void getLayerAttrTables(const std::string& name, std::vector<std::string>& tables);
		
		/*!
			\brief Gets a list of tables name given a valid layer pointer.
			\param layer [input] The valid layer pointer.
			\param tables [output] List of tables name from layer.
		*/
		void getLayerAttrTables(TeLayer* layer, std::vector<std::string>& tables);

		/*!
			\brief Gets a list of atrributes given a layer name.
			\param name [input] The layer name.
			\param attrList [output] List of atrributes from layer.
		*/
		void getLayerAttrList(const std::string& name, TeAttributeList& attrList);

		/*!
			\brief Gets a list of atrributes given a valid layer pointer.
			\param layer [input] The valid layer pointer.
			\param attrList [output] List of atrributes from layer.
		*/
		void getLayerAttrList(TeLayer* layer, TeAttributeList& attrList);

		//@}

		/** @name Methods related with TerraLib Themes.
		* Methods for obtainning information about themes.
		*/
		//@{

		/*!
			\brief Gets a list of themes names given a visible representation.
			\param themes [output] List of themes name from current database.
			\param geomRep [input] A combination of geometric representations (ex.: TePOLYGONS | TeLINES) 
		*/
		void getThemes(std::vector<std::string>& themes, const int& visibleRep);

		/*!
			\brief Gets a list of themes names from current database.
			\param themes [output] List of themes name.
		*/
		void getThemes(std::vector<std::string>& themes);

		/*!
			\brief Gets a theme given a name.
			\param name The theme name.
			\return A valid theme pointer if a theme with this name exist in database, 0 otherwise.
		*/
		TeTheme* getTheme(const std::string& name);

		/*!
			\brief Gets a list of tables name given a theme name.
			\param name [input] The theme name.
			\param tables [output] List of tables name from theme.
		*/
		void getThemeAttrTables(const std::string& name, std::vector<std::string>& tables);

		/*!
			\brief Gets a list of tables name given a valid theme pointer.
			\param theme [input] The valid theme pointer.
			\param tables [output] List of tables name from theme.
		*/
		void getThemeAttrTables(TeTheme* theme, std::vector<std::string>& tables);

		/*!
			\brief Gets a list of atrributes given a theme name.
			\param name [input] The theme name.
			\param attrList [output] List of atrributes from theme.
		*/
		void getThemeAttrList(const std::string& name, TeAttributeList& attrList);

		/*!
			\brief Gets a list of atrributes given a valid theme pointer.
			\param theme [input] The valid theme pointer.
			\param attrList [output] List of atrributes from theme.
		*/
		void getThemeAttrList(TeTheme* theme, TeAttributeList& attrList);

		/*!
			\brief Builds a STElementSet given a theme name.
			\param name [input] The theme name.
			\param loadGeom [input] A flag that indicates if the geometries should be loaded.
			\param attrs [input] List of attribute names that will be loaded.
			\return The STElementSet.
		*/
		TeSTElementSet* getElementSetFromTheme(const std::string& name, const bool& loadGeom, 
											   const std::vector<std::string>& attrs);

		/*!
			\brief Builds a STElementSet given a valid theme pointer.
			\param theme [input] The valid theme pointer.
			\param loadGeom [input] A flag that indicates if the geometries should be loaded.
			\param attrs [input] List of attribute names that will be loaded.
			\return The STElementSet.
		*/
		TeSTElementSet* getElementSetFromTheme(TeTheme* theme, const bool& loadGeom, 
											   const std::vector<std::string>& attrs);
		//@}

		/*!
			\brief DOCUMENT ME
		*/
		void export2CSV(TeSTElementSet* objSet, const std::string& path, const char& sep = ';');

		/*!
			\brief DOCUMENT ME: randomize the attributes...
		*/
		TeSTElementSet* randomize(TeSTElementSet* objSet);

		/*!
			\brief DOCUMENT ME: randomize the centroids...
		*/
		TeSTElementSet* randomize2(TeSTElementSet* objSet);

        /*!
			\brief DOCUMENT ME
		*/
        TeSTElementSet* getElementSetFromCSV(const std::string& path, const char& sep = ';');

        /*!
			\brief DOCUMENT ME
		*/
        bool generateMap(TeTheme* t, const std::string& attrName, std::string& errorMessage);

	protected:

		TeDatabase* m_db; //  A pointer to TerraLib Database.
};

#endif //__TERRASEGREG_PLUGIN_UTILS_H
