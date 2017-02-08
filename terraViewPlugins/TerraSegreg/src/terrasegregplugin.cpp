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

// TerraSegreg
#include "terrasegregplugin.h"
#include "Utils.h"

// TerraSegreg UI
#include "GeneralizedDissimilarityIndexWin.h"
#include "ExposureIndexWin.h"
#include "ExposureIndexMNWin.h"
#include "IsolationIndexMWin.h"
#include "LocalPopulationIntensityWin.h"
#include "NeighborhoodSortingIndexWin.h"
#include "UIAbout.h"

// Qt
#include <qaction.h>
#include <qmessagebox.h>

TerraSegregPlugin::TerraSegregPlugin(PluginParameters* params):
QObject(),
TViewAbstractPlugin(params)
{
	// Translation files
	loadTranslationFiles("terrasegreg_");
	
	// Initializing menu texts...
	m_preProcessingText = tr("Pre-Processing");
	m_localPopText = tr("Local Population Intensity");

	m_clusteringText = tr("Dimension Evenness / Clustering");
	m_dissimilarityIndexText = tr("Generalized Spatial Dissimilarity Index");
	m_neighborhoodIndexText = tr("Spatial Neighborhood Sorting Index");
	
	m_isolationText = tr("Dimension Exposure / Isolation");
	m_exposureIndexText = tr("Generalized Spatial Exposure Index");
	m_exposureIndexMNText = tr("Spatial Exposure Index of Group m to Group n");
	m_isolationIndexMText = tr("Spatial Isolation Index of Group m");
    
	m_aboutText = tr("About...");
}

TerraSegregPlugin::~TerraSegregPlugin()
{
	end();
}

void TerraSegregPlugin::init()
{
	// Gets plugin menu and creates actions for TerraSegreg User Interfaces
	std::string baseMenu = "TerraViewPlugins.TerraSegreg.";
	
	// Pre-Processing
	QPopupMenu* pmenu  = getPluginsMenu(baseMenu + m_preProcessingText.latin1());
	createAction(m_localPopText, pmenu, SLOT(showLocalPopWindow()));
	
	// Clustering
	QPopupMenu* cmenu  = getPluginsMenu(baseMenu + m_clusteringText.latin1());
	createAction(m_dissimilarityIndexText, cmenu, SLOT(showDissimilarityIndexWindow()));
	createAction(m_neighborhoodIndexText, cmenu, SLOT(showNeighborhoodSortingIndexWindow()));
	
	// Isolation
	QPopupMenu* imenu  = getPluginsMenu(baseMenu + m_isolationText.latin1());
	createAction(m_exposureIndexText, imenu, SLOT(showExposureIndexWindow()));
	createAction(m_exposureIndexMNText, imenu, SLOT(showExposureIndexMNWindow()));
	createAction(m_isolationIndexMText, imenu, SLOT(showIsolationIndexMWindow()));
	
	// --
	QPopupMenu* menu = getPluginsMenu(baseMenu);
	menu->insertSeparator();
	
	// About
	createAction(m_aboutText, menu, SLOT(showAbout()));
}

void TerraSegregPlugin::end()
{}

void TerraSegregPlugin::createAction(const QString& text, QWidget* w, const char* m)
{
	QAction* action = new QAction(this);
	action->setMenuText(text);
	action->setText(text);
	action->addTo(w);
	connect(action, SIGNAL(activated()), this, m);
}

void TerraSegregPlugin::showLocalPopWindow()
{
	LocalPopulationIntensityWin lpw(params_);
	lpw.showWindow();
}

void TerraSegregPlugin::showDissimilarityIndexWindow()
{
	GeneralizedDissimilarityIndexWin gdi(params_);
	gdi.setCaption(m_dissimilarityIndexText);
	gdi.showWindow();
}

void TerraSegregPlugin::showNeighborhoodSortingIndexWindow()
{
	NeighborhoodSortingIndexWin nsi(params_);
	nsi.setCaption(m_neighborhoodIndexText);
	nsi.showWindow();
}

void TerraSegregPlugin::showExposureIndexWindow()
{
	ExposureIndexWin ei(params_);
	ei.setCaption(m_exposureIndexText);
	ei.showWindow();
}

void TerraSegregPlugin::showExposureIndexMNWindow()
{
	ExposureIndexMNWin eimn(params_);
	eimn.setCaption(m_exposureIndexMNText);
	eimn.showWindow();
}

void TerraSegregPlugin::showIsolationIndexMWindow()
{
	IsolationIndexMWin iim(params_);
	iim.setCaption(m_isolationIndexMText);
	iim.showWindow();
}

void TerraSegregPlugin::showAbout()
{
	UIAbout about;
	about.exec();
}
