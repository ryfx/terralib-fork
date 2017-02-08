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
	\file NeighborhoodSortingIndexWin.cpp
	\brief Class that implements an user graphic interface to calculates the Spatial Neighborhood Sorting Index.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

// TerraSegreg
#include "NeighborhoodSortingIndexWin.h"
#include "NeighborhoodSortingIndex.h"
#include "Utils.h"

// TerraLib
#include <PluginParameters.h>
#include <TeUtils.h>

// TerraView
#include <help.h>

// Qt
#include <qbuttongroup.h>
#include <qmessagebox.h>
#include <qtable.h>

NeighborhoodSortingIndexWin::NeighborhoodSortingIndexWin(PluginParameters* pp)
: SpatialIndexWin(pp)
{
	// Configures the user interface
	groupMButtonGroup->hide();
	groupNButtonGroup->hide();
	generalOptionsButtonGroup->hide();

	// Builds the group table
	groupsTable->setNumCols(2);
	QStringList colsNames;
	colsNames.push_back(tr("Local Population Intensity"));
	colsNames.push_back(tr("Group Value"));
	groupsTable->setColumnLabels(colsNames);
    
	// Adjusts groups table
    groupsTable->setColumnStretchable(0, true);
    groupsTable->setColumnStretchable(1, true);

	// Specified Spatial Index
	m_spatialIndex = new NeighborhoodSortingIndex();
	m_helpPath = "TerraSegreg/NeighborhoodSortingIndex.html";
} 

NeighborhoodSortingIndexWin::~NeighborhoodSortingIndexWin()
{}

void NeighborhoodSortingIndexWin::verifyPushButton_clicked()
{
	SpatialIndexWin::verifyPushButton_clicked();

    if(m_groups.empty())
		return;

    // Builds the population groups table
    groupsTable->setNumRows(m_groups.size());
    int row = 0;
    std::map<std::string, std::string>::iterator it;
    for(it = m_groups.begin(); it != m_groups.end(); ++it)
    {
        groupsTable->setText(row, 0, it->second.c_str());
        groupsTable->setText(row, 1, tr("< choose... >"));
        row++;
    }
}

bool NeighborhoodSortingIndexWin::configureSpatialIndex()
{
	// Configures Spatial Neighborhood Sorting Index.
	NeighborhoodSortingIndex* nsi = dynamic_cast<NeighborhoodSortingIndex*>(m_spatialIndex);

	// Verifies the group values
	bool isOk;
	std::map<std::string, std::string>::iterator it = m_groups.begin();
	for(unsigned int i = 0; i < groupsTable->numRows(); ++i)
	{
		QString v = groupsTable->text(i, 1);
		std::string t = v.latin1();
		double value = v.toDouble(&isOk);
		if(!isOk)
		{
			QString msg = tr("The choosen value for group ");
			msg += Te2String(i+1).c_str();
			msg += tr(" is invalid. Choose another!");
			QMessageBox::information(this, tr("Information"), msg);
			return false;
		}
		nsi->setGroupValue(it->second, value);
		++it;
	}

	return true;
}

void NeighborhoodSortingIndexWin::update(TeSTElementSet* /*objSet*/)
{
	QMessageBox::information(this, tr("Information"), tr("Spatial Neighborhood Sorting Index calculated with successful."));
}

	