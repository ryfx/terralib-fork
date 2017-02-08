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
	\file LocalPopulationIntensityWin.cpp
	\brief Class that implements a user graphic interface to calculates Local Population Intensity.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

// TerraSegreg
#include "LocalPopulationIntensityWin.h"
#include "TeProxMatrixBiSquaredStrategy.h"
#include "TeProxMatrixGaussianStrategy.h."
#include "Utils.h"

// TerraLib
#include <PluginParameters.h>
#include <TeDatabase.h>
#include <TeGeneralizedProxMatrix.h>
#include <TeProxMatrixConstructionStrategy.h>
#include <TeProxMatrixWeightsStrategy.h>
#include <TeSTEFunctionsDB.h>

// TerraView
#include <help.h>
#include <TeQtGrid.h>

// Qt
#include <qbuttongroup.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qfiledialog.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qmessagebox.h>
#include <qprogressdialog.h> 
#include <qradiobutton.h>
#include <qtabwidget.h>
#include <qvalidator.h>

// STL
#include <string>
#include <vector>

LocalPopulationIntensityWin::LocalPopulationIntensityWin(PluginParameters* pp)
: UILocalPopulationIntensity(0, "LocalPopulationIntensityWin"),
m_pp(pp),
m_utils(0),
m_help(0)
{
	bandWidthLineEdit->setValidator(new QDoubleValidator(this));
} 

LocalPopulationIntensityWin::~LocalPopulationIntensityWin()
{
	delete m_utils;
	delete m_help;
}

void LocalPopulationIntensityWin::showWindow()
{
	TeDatabase* db = m_pp->getCurrentDatabasePtr();
	if(db == 0)
	{
		QMessageBox::critical(0, tr("Error"), tr("Please, connect to a database first."));
		return;
	}

	// Utility class for get some parameters...
	m_utils = new Utils(db);
	
	std::vector<std::string> themes;
	m_utils->getThemes(themes, TePOLYGONS | TeLINES | TePOINTS);

	if(themes.empty())
	{
		QMessageBox::critical(0, tr("Error"), tr("Please, create at least a theme."));
		return;
	}

	// Fills the theme combo box
	themeComboBox->insertItem("");
	for(unsigned int i = 0; i < themes.size(); ++i)
		themeComboBox->insertItem(themes[i].c_str());

	exec();
}

void LocalPopulationIntensityWin::themeComboBox_activated(const QString& name)
{
	TeAttributeList attrList;
	m_utils->getThemeAttrList(name.latin1(), attrList);

	// Updates the attribute list on interface...
	attrsListBox->clear();
	for(unsigned int i = 0; i < attrList.size(); ++i)
	{
		TeAttribute& attr = attrList[i];
		if(!attr.rep_.isPrimaryKey_ && 
		  (attr.rep_.type_ == TeINT || attr.rep_.type_ == TeREAL))
			attrsListBox->insertItem(attr.rep_.name_.c_str());
	}
}

void LocalPopulationIntensityWin::directoryPushButton_clicked()
{
	QString s = QFileDialog::getExistingDirectory(QString::null, this, 
							 "Dir", tr("Select a directory to save the random distributions results..."));

	directoryLineEdit->setText(s);
}

void LocalPopulationIntensityWin::okPushButton_clicked()
{
	// Verifies input theme
	std::string themeName = themeComboBox->currentText().latin1();
	if(themeName.empty())
	{
		QMessageBox::information(this, tr("Information"), tr("Please, select a theme first."));
		mainTabWidget->setCurrentPage(Input);
		themeComboBox->setFocus();
		return;
	}

	// Verifies the selected attributes
	std::vector<std::string> attrs;
	for(unsigned int i = 0; i < attrsListBox->numRows(); ++i)
	{
		if(attrsListBox->isSelected(i))
			attrs.push_back(attrsListBox->text(i).latin1());
	}

	if(attrs.empty())
	{
		QMessageBox::information(this, tr("Information"), tr("Please, select at least an attribute."));
		mainTabWidget->setCurrentPage(Input);
		attrsListBox->setFocus();
		return;
	}
	
	// Verifies the band width
	QString b = bandWidthLineEdit->text();
	if(b.isEmpty())
	{
		QMessageBox::information(this, tr("Information"), tr("Please, define the band width value."));
		mainTabWidget->setCurrentPage(Kernel);
		bandWidthLineEdit->setFocus();
		return;
	}

	// Verifies the random distributions
	bool generateRandomDistributions = randomDistributionsCheckBox->isChecked();
	if(generateRandomDistributions)
	{
		QString dir = directoryLineEdit->text();
		if(dir.isEmpty())
		{
			QMessageBox::information(this, tr("Information"), tr("Please, select a directory to save the random distributions results."));
			mainTabWidget->setCurrentPage(Options);
			directoryLineEdit->setFocus();
			return;
		}
	}

	// Getting the necessary parameters...
	TeTheme* theme = m_utils->getTheme(themeName);
	if(theme == 0)
	{
		QMessageBox::critical(this, tr("Error"), tr("Error getting the selected theme."));
		return;
	}

    // Builds the element set
	TeSTElementSet* objSet = m_utils->getElementSetFromTheme(theme, true, attrs);
	if(objSet == 0)
	{
		QMessageBox::critical(this, tr("Error"), tr("Error building element set from the selected theme."));
		return;
	}

	double bandwidth = bandWidthLineEdit->text().toDouble();
	
	// Proximity Function choosen
	LocalPopulationIntensity::ProxFunction f = getWStrategy();
	
	// Verifies the suffix
	std::string suffix = attrSuffixLineEdit->text().latin1();
	if(suffix.empty()) suffix = "_W";
	
	// Class to calculates the Local Population Intensity
	LocalPopulationIntensity lps(bandwidth, f, suffix);

	double distance = bandwidth;
	if(f == LocalPopulationIntensity::Gaussian)
		distance = TeMAXFLOAT;

	// Initializing the parameters...
	lps.init(objSet, distance, (TeGeomRep)theme->visibleGeoRep());
	
	if(generateRandomDistributions)
	{
		// Creates the directory to save the distributions
		QString path = directoryLineEdit->text();
		QDir dir(path);
		std::string description;
		// Gets the string description
		lps.getDescription(description);
		// Creates a directory to store the random distributions results
		dir.mkdir(description.c_str());
		dir.cd(description.c_str());
		
		int n = numberOfRandomDistributions();
		QProgressDialog pb(tr("Generating random distributions..."), tr("Cancel"), n, this, "", true);
		for(unsigned int i = 0; i < n; ++i)
		{
			// Randomize the originals objects...
			TeSTElementSet* random = m_utils->randomize(objSet);
			// Performs the calculus...
			lps.calculate(random);
			// Save the result to a file
			std::string filename = "/random_" + description + "_" + Te2String(i+1) + ".csv";
			std::string fullPath = dir.absPath().latin1() + filename; 
			m_utils->export2CSV(random, fullPath);
			
			delete random;
			
			// Updates the progress bar...
			pb.setProgress(i);
			if(pb.wasCanceled())
				break;
		}
		pb.reset();
	}

	// Performs the calculus for originals objects...
	lps.calculate(objSet);

	// Removes originals attributes in order to optimize the table update...
	for(unsigned int i = 0; i < attrs.size(); ++i)
	{
		TeAttributeRep attrRep;
		attrRep.name_ = attrs[i];
		objSet->removeProperty(attrRep);
	}

	m_pp->teqtgrid_ptr_->clearPortal();

	// Updates the database
	if(!TeUpdateDBFromSet(objSet, theme->layer()->attrTables()[0].name()))
	{
		QString msg = tr("Error updating the layer table.") + "\n";
		msg += tr("Details:") + " " + m_pp->getCurrentDatabasePtr()->errorMessage().c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}
	
	QMessageBox::information(this, tr("Information"), tr("Local Population Intensity calculated with successful."));

	delete objSet;

	close();

	m_pp->updateTVInterface();
}

void LocalPopulationIntensityWin::helpPushButton_clicked()
{
	delete m_help;
	m_help = new Help(this, tr("Help"));
	QString helpFile = "TerraSegreg/LocalPopulationIntensity.html";
	m_help->init(helpFile);
	m_help->show();
}

void LocalPopulationIntensityWin::randomDistributionsCheckBox_clicked()
{
	bool on = randomDistributionsCheckBox->isChecked();
	n99RadioButton->setEnabled(on);
	n499RadioButton->setEnabled(on);
	n999RadioButton->setEnabled(on);
	directoryButtonGroup->setEnabled(on);
}

LocalPopulationIntensity::ProxFunction LocalPopulationIntensityWin::getWStrategy()
{	
	switch(weightingFunctionButtonGroup->selectedId())
	{
		case 0:
			return LocalPopulationIntensity::MovingWindow;

		case 1:
			return LocalPopulationIntensity::Gaussian;

		default:
			return LocalPopulationIntensity::BiSquared;
	}
}

unsigned int LocalPopulationIntensityWin::numberOfRandomDistributions()
{
	if(n99RadioButton->isOn())
		return 99;
	else if(n499RadioButton->isOn())
		return 499;
	return 999;
}
