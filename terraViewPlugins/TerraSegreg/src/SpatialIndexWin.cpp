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
	\file SpatialIndexWin.cpp
	\brief Base class that implements an user graphic interface to calculates the Spatial Indexes.
	\author Douglas Uba <douglas@dpi.inpe.br>
*/

// TerraSegreg
#include "SpatialIndex.h"
#include "SpatialIndexWin.h"
#include "Utils.h"

// TerraLib
#include <PluginParameters.h>
#include <TeDatabase.h>
#include <TeSTElementSet.h>
#include <TeSTEFunctionsDB.h>

// TerraView
#include <help.h>
#include <TeWaitCursor.h>

// Qt
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qfiledialog.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qmessagebox.h>
#include <qprogressdialog.h> 
#include <qtable.h>
#include <qtabwidget.h>

// STL
#include <string>
#include <vector>

SpatialIndexWin::SpatialIndexWin(PluginParameters* pp)
: UISpatialIndex(0, "SpatialIndex"),
m_pp(pp),
m_utils(0),
m_help(0),
m_spatialIndex(0)
{} 

SpatialIndexWin::~SpatialIndexWin()
{
	delete m_utils;
	delete m_help;
	delete m_spatialIndex;
}

void SpatialIndexWin::showWindow()
{
    TeDatabase* db = m_pp->getCurrentDatabasePtr();
	if(db == 0)
	{
		QMessageBox::critical(0, tr("Error"), tr("Please, connect to a database first."));
		return;
	}

	m_utils = new Utils(db);
	
	std::vector<std::string> themes;
	m_utils->getThemes(themes, TePOLYGONS | TeLINES | TePOINTS);

	if(themes.empty())
	{
		QMessageBox::critical(0, tr("Error"), tr("Please, create at least a theme."));
		return;
	}

	// Fill the theme combo box
	themeComboBox->insertItem("");
	for(unsigned int i = 0; i < themes.size(); ++i)
		themeComboBox->insertItem(themes[i].c_str());

	// Sets default attribute name
	attrNameLineEdit->setText(m_spatialIndex->getIndexAttributeName().c_str());

	exec();
}

void SpatialIndexWin::verifyPushButton_clicked()
{	
    // Verifies input theme
    std::string themeName = themeComboBox->currentText().latin1();
	if(themeName.empty())
	{
		QMessageBox::information(this, tr("Information"), tr("Please, select a theme first."));
		themeComboBox->setFocus();
		return;
	}

    // Verifies local population intensity suffix
    std::string suffix = attrSuffixLineEdit->text().latin1();
    if(suffix.empty())
    {
        QMessageBox::information(this, tr("Information"), tr("Please, define the local population intensity suffix first."));
        attrSuffixLineEdit->setFocus();
		return;
    }

    // Gets the theme
    TeTheme* theme = m_utils->getTheme(themeName);
	if(theme == 0)
	{
		QMessageBox::critical(this, tr("Error"), tr("Error getting the selected theme."));
		return;
	}

    // Gets the attribute list
    TeAttributeList attrList;
    m_utils->getThemeAttrList(themeName, attrList);

    // Try match the population groups with its local population intensity
    m_groups.clear();
    for(unsigned int i = 0; i < attrList.size(); ++i)
	{
        TeAttribute& attr = attrList[i];
        std::string nameToSearch = attr.rep_.name_ + suffix;
        for(unsigned int j = i; j < attrList.size(); ++j)
        {
            TeAttribute& currentAttr = attrList[j];
            if(currentAttr.rep_.name_ == nameToSearch)
                m_groups[attr.rep_.name_] = currentAttr.rep_.name_;
        }
	}

    // Verifies population groups
    if(m_groups.empty())
	{
		QMessageBox::information(this, tr("Information"), tr("No population groups found."));
		return;
	}
}

void SpatialIndexWin::okPushButton_clicked()
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

    // Verifies population groups
    if(m_groups.empty())
	{
		QMessageBox::information(this, tr("Information"), tr("No population groups found."));
		return;
	}

    // Verifies p-value calculus
	bool calculatepValue = pvalueCheckBox->isChecked();
	if(calculatepValue)
	{
		QString dir = directoryLineEdit->text();
		if(dir.isEmpty())
		{
			QMessageBox::information(this, tr("Information"), tr("Please, select the directory that contains the random distributions results."));
			mainTabWidget->setCurrentPage(Options);
			directoryLineEdit->setFocus();
			return;
		}
	}

    // Gets the theme
    TeTheme* theme = m_utils->getTheme(themeName);
	if(theme == 0)
	{
		QMessageBox::critical(this, tr("Error"), tr("Error getting the selected theme."));
		return;
	}

    // Builds the attributes that will be loaded
	std::vector<std::string> attrs;
	std::map<std::string, std::string>::iterator it;
	for(it = m_groups.begin(); it != m_groups.end(); ++it)
	{
		attrs.push_back(it->first);
		attrs.push_back(it->second);
	}

    // Builds the element set
	TeSTElementSet* objSet = m_utils->getElementSetFromTheme(theme, false, attrs);
	if(objSet == 0)
	{
		QMessageBox::critical(this, tr("Error"), tr("Error building element set from the selected theme."));
		return;
	}
	
	// waiting...
	TeWaitCursor wait;

	// Configures the Spatial Index
	m_spatialIndex->setObjSet(objSet);
	for(it = m_groups.begin(); it != m_groups.end(); ++it)
		m_spatialIndex->addGroup(it->first, it->second);

	// Attribute name for the calculted index
	std::string attrName = attrNameLineEdit->text().latin1();
	m_spatialIndex->setIndexAttributeName(attrName);

	// Configures the specified paramaters
	if(!configureSpatialIndex())
	{
		delete objSet;
		return;
	}

	// Calculates the index!
	m_spatialIndex->calculate();
    
    if(calculatepValue)
    {
        // Gets the directory that contains the random distributions results
		QString basePath = directoryLineEdit->text();
        
        QDir dir(basePath);
        QStringList files = dir.entryList("*.csv");
        
        if(files.empty())
            QMessageBox::information(this, tr("Information"), tr("There are not *.csv files on informed directory. The p-value will not be calculated."));
        else
        {
            unsigned int nfiles = files.count();
            QProgressDialog pb(tr("Calculating the global index for random distributions..."), tr("Cancel"), nfiles, this, "", true);
            for(unsigned int i = 0; i < nfiles; ++i)
            {
                std::string file = basePath + "/" + files[i].latin1();
                TeSTElementSet* random = m_utils->getElementSetFromCSV(file);
                m_spatialIndex->setObjSet(random);
                m_spatialIndex->calculate();
                
                delete random;

                // Updates the progress bar...
			    pb.setProgress(i);
			    if(pb.wasCanceled())
				    break;
            }
            pb.reset();
        }
    }

	// Show results
	mainTabWidget->setCurrentPage(Results);
    double globalIndex = m_spatialIndex->getGlobalIndex();
    double pvalue = m_spatialIndex->getpValue();
	globalIndexLineEdit->setText(QString(QVariant(globalIndex).asString()));
	if(calculatepValue)
		pvalueLineEdit->setText(QString(QVariant(pvalue).asString()));
	else
		pvalueLineEdit->setText(tr("Not calculated"));

	wait.resetWaitCursor();

	update(objSet);

	delete objSet;

	m_pp->updateTVInterface();

	okPushButton->setEnabled(false);
}

void SpatialIndexWin::themeComboBox_activated(const QString& name)
{
    groupsTable->setNumRows(0);
    m_groups.clear();
}

void SpatialIndexWin::directoryPushButton_clicked()
{
    QString s = QFileDialog::getExistingDirectory(QString::null, this, 
							 "Dir", tr("Select the directory that contains the random distributions results..."));

	directoryLineEdit->setText(s);
}

void SpatialIndexWin::pvalueCheckBox_clicked()
{
    bool on = pvalueCheckBox->isChecked();
    directoryButtonGroup->setEnabled(on);
}

void SpatialIndexWin::helpPushButton_clicked()
{
	delete m_help;
	m_help = new Help(this, tr("Help"));
	QString helpFile = m_helpPath.c_str();
	m_help->init(helpFile);
	m_help->show();
}

	