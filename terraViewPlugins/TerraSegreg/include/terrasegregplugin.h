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
	\brief TerraSegreg plugin.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

#ifndef __TERRASEGREG_PLUGIN_H_
#define __TERRASEGREG_PLUGIN_H_

#include <TViewAbstractPlugin.h>

// Qt include files
#include <qobject.h>
#include <qstring.h>

// forward declarations
class QPopupMenu;

class TerraSegregPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:

	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	TerraSegregPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~TerraSegregPlugin();

	/**	\brief Initializer method.
	 */
	void init();

	/**	\brief Finalizer method.
	 */
	void end();

protected:

	/*! \brief Creates a new action setting the properties.
		\param  text Defines the text of the action.
		\param  w	 Defines the widget that will contains the action.
		\param  m	 Defines the method that will be called by the action.
    */
	void createAction(const QString& text, QWidget* w, const char* m);

protected slots:

	void showLocalPopWindow();
	void showDissimilarityIndexWindow();
	void showNeighborhoodSortingIndexWindow();
	void showExposureIndexWindow();
	void showExposureIndexMNWindow();
	void showIsolationIndexMWindow();
    void showAbout();

protected:

	// Menus
	QString m_preProcessingText;
	QString m_clusteringText;
	QString m_isolationText;

	// Local Population Intensity
	QString m_localPopText;

	// Generalized Spatial Dissimilarity Index
	QString	m_dissimilarityIndexText;

	// Spatial Neighborhood Sorting Index
	QString m_neighborhoodIndexText;

	// Generalized Spatial Exposure Index
	QString m_exposureIndexText;

	// Spatial Exposure Index of Group m to Group n
	QString m_exposureIndexMNText;

	// Spatial Isolation Index of Group m
	QString m_isolationIndexMText;

    // About
    QString m_aboutText;
};

#endif //__TERRASEGREG_PLUGIN_H_