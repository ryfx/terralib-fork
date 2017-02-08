/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright � 2001-2007 INPE and Tecgraf/PUC-Rio.
This file is part of TerraView. TerraView is free software; you can redistribute it 
and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

You should have received a copy of the GNU General Public License
along with TerraView.
The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The software provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use of
this program and its documentation.
*************************************************************************************/

#include <TeWaitCursor.h>
#include <TeQtGrid.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <TeQtViewsListView.h>
#include <TeQtGrid.h>
#include "TeSTEFunctionsDB.h"
#include "TeProgress.h"
#include "TeSTEvent.h"
#include "TeDatabaseUtils.h"


void Anaesp::init()
{
	help_ = 0;
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	db_ = mainWindow_->currentDatabase();
	theme_ = mainWindow_->currentTheme();
	table_ = 0;

	if(!theme_)
		return; 

	if(theme_->getTheme()->type() != TeTHEME)
		return;

	table_ = mainWindow_->getGrid();
	curLayer_ = ((TeTheme*)theme_->getTheme())->layer();

	//fill only numeric attributes
	TeAttributeList attrVec = ((TeTheme*)theme_->getTheme())->sqlNumAttList();

	AttributeComboBox->clear();
	unsigned int i;
	for(i=0; i<attrVec.size(); i++)
		AttributeComboBox->insertItem(attrVec[i].rep_.name_.c_str());
	
	perm999RadioButton->setEnabled(true);
	Local999RadioButton->setEnabled(true);
	Local9999RadioButton->setEnabled(true);
}

void Anaesp::show()
{
	if(theme_->getTheme()->type() == TeTHEME)
	{
		if(table_!=0)
			QDialog::show();
	}
	else
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		reject();
		hide();
	}
}

void Anaesp::okPushButton_clicked()
{
	MoranGlobalLineEdit->clear();
	SignificanceLineEdit->clear();

	if ((MoranGlobalCheckBox->isChecked() || MoranLocalCheckBox->isChecked() || LocalMeanCheckBox->isChecked() ||
		GCheckBox->isChecked() || GStarCheckBox->isChecked()) == 0 )
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select at least one statistic to be calculated."));
		return;
	}	
	
	if (AttributeComboBox->currentText().isEmpty()) 
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select an attribute to calculate the statistics."));
		return;
	}

	// filter theme that use layer with only one type of geometric representation
	TeGeomRep geomRep = (TeGeomRep)(theme_->getTheme()->visibleGeoRep() & ~TeTEXT); // TeTEXT not is considered
	if (!(geomRep == TePOLYGONS || geomRep == TeLINES || geomRep == TePOINTS || geomRep == TeCELLS))
	{
		QString msg = tr("The themes must have only one visible vector representation (polygons, points, lines, or cells).") + "\n";
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

	TeTable resultTable;
	if (MoranLocalCheckBox->isChecked() || 	// local statistics will be stored in the first of the static tables
		LocalMeanCheckBox->isChecked() ||	// check if there is one to be used
		GCheckBox->isChecked() || 
		GStarCheckBox->isChecked())
	{
		TeAttrTableVector attrs; 
		((TeTheme*)theme_->getTheme())->getAttTables(attrs, TeAttrStatic);
		if (attrs.empty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("The theme has no static tables to save the local statistics."));
			return; 
		}
		resultTable = attrs[0];
	}

	// Build a STOSet to retrieve the input data and calculate or retrieve the proximity matrix 
	string attr = AttributeComboBox->currentText().latin1();
	bool status = false;
	TeSTElementSet	stoSet ((TeTheme*)theme_->getTheme()); 
	TeGroupingAttr groupAttr;								// for temporal data
	TeAttributeRep rep;										// we used the max value of the choosen attribute
	rep.name_ = attr;
	rep.type_ = TeREAL;
	rep.decimals_ = 15;
	groupAttr.push_back(pair<TeAttributeRep, TeStatisticType>(rep, TeMAXVALUE));
	TeWaitCursor wait;
	status = TeSTOSetBuildDB (&stoSet, groupAttr, true);
	wait.resetWaitCursor();
	if (!status)
	{
		if(TeProgress::instance())
			TeProgress::instance()->reset();
		QMessageBox::critical(this, tr("Error"), tr("Fails retrieving the object set from database."));
		return;
	}

	// From the object set construct and event set
	TeSTEventSet evSet((TeTheme*)theme_->getTheme());

	//insert a first attribute of the stoSet in the the event set
	vector<string> objIds;
	TeAttribute att = stoSet.getAttributeList()[0];
	evSet.addProperty(att);
	TeSTElementSet::iterator itObjs = stoSet.begin();
	while (itObjs!=stoSet.end())
	{
		TeSTEvent ev;
		string val;
		(*itObjs).getPropertyValue(val, 0);
		ev.objectId((*itObjs).objectId());
		ev.uniqueId((*itObjs).uniqueId());
		ev.addPropertyValue(val); 
		evSet.insertSTInstance(ev);
		objIds.push_back((*itObjs).objectId());
		++itObjs;
	}

	
	//  Load an existing proximity matrix or create a new one
	double tol = TeGetPrecision(((TeTheme*)theme_->getTheme())->layer()->projection());
	TePrecision::instance().setPrecision(tol);
	TeProxMatrixConstructionStrategy<TeSTElementSet>*   constStrategy=0;
	TeGeneralizedProxMatrix<TeSTElementSet>* proxMat=0;
	if (!loadDefaultGPM(db_, ((TeTheme*)theme_->getTheme())->id(), proxMat))
	{
		if (!(geomRep == TePOLYGONS || geomRep == TeCELLS))
		{
			QString msg = tr("The theme must have a vector representation of polygons") + "\n";
			msg += tr("or cells to create the contiguity matrix!");
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}
		
		if(TeProgress::instance())
		{
			string caption = tr("Spatial Statistics").latin1();
			TeProgress::instance()->setCaption(caption.c_str());
			string msg = tr("Building the Proximity Matrix... Please, wait!").latin1();
			TeProgress::instance()->setMessage(msg);
		}
		
		//create matrix and save in the database
		constStrategy = new TeProxMatrixLocalAdjacencyStrategy (&stoSet, geomRep); 
		TeProxMatrixNoWeightsStrategy sw_no;
		TeProxMatrixNoSlicingStrategy ss_no;
		proxMat = new TeGeneralizedProxMatrix<TeSTElementSet>(constStrategy, &sw_no, &ss_no);
		status = proxMat->constructMatrix();
		if (!status)
		{
			if(TeProgress::instance())
				TeProgress::instance()->reset();
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"), tr("Fail building the proximity matrix!"));
			if(proxMat)
				delete proxMat;
			return;
		}

		//save in the database
		bool savedMatrix = true;
		if(!db_->tableExist("te_gpm"))
		{
			if(!createGPMMetadataTable(db_))
			{
				if(TeProgress::instance())
					TeProgress::instance()->reset();
				wait.resetWaitCursor();
				QMessageBox::warning(this, tr("Error"), 
					tr("It was not possible to save proximity matrix in database!"));
				savedMatrix = false;
			}
		}

		string tableName = "te_neighbourhood_"+ Te2String(((TeTheme*)theme_->getTheme())->id());
		string tempTable = tableName;
		int count = 1;
		while(savedMatrix && db_->tableExist(tempTable))
		{
			tempTable = tableName+"_"+Te2String(count);
			++count;
		}

		if(savedMatrix && (!createNeighbourhoodTable(db_, tempTable)))
		{
			if(TeProgress::instance())
				TeProgress::instance()->reset();
			wait.resetWaitCursor();
			QMessageBox::warning(this, tr("Error"), 
				tr("It was not possible to save proximity matrix in database!"));
			savedMatrix=false;
		}

		proxMat->neighbourhood_table_=tempTable;

		if(savedMatrix && (!insertGPM(db_, proxMat, objIds)))
		{
			if(TeProgress::instance())
				TeProgress::instance()->reset();
			wait.resetWaitCursor();
			QMessageBox::warning(this, tr("Error"), 
				tr("It was not possible to save proximity matrix in database!"));
			savedMatrix=false;		
		}
	}
	stoSet.clear();

	 if(proxMat->numberOfObjects()<1)
	 {
		if(TeProgress::instance())
			TeProgress::instance()->reset();
		wait.resetWaitCursor();
		QMessageBox::critical(this, tr("Error"), 
			tr("The proximity matrix is empty! No neighbourhood found!"));
		if(proxMat)
			delete proxMat;
		return;
	}

	 //statisctics
	int indexAttr=0, indexZ=99, indexG=99, indexGStar=99, indexWZ=99, indexNumberNeig=99, indexLocalMean=99;
	int indexLocalMoran=99, indexBoxMap=99, indexProbLISA=99, indexLISAMap=99, indexMoranMap=99; 
	int nextIndex = 1;
	
	if(TeProgress::instance())
	{
		string caption = tr("Spatial Statistics").latin1();
		TeProgress::instance()->setCaption(caption.c_str());
		string msg = tr("Calculating statistics. Please, wait!").latin1();
		TeProgress::instance()->setMessage(msg);
	}
	
	if (GCheckBox->isChecked() || GStarCheckBox->isChecked() )//G and G*
	{
		if (!TeGStatistics(&evSet, proxMat))
		{
			if(TeProgress::instance())
				TeProgress::instance()->reset();
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"), 
				tr("Fail to calculate G statistics!\n Verify if there are objects without attribute value!"));
			if(proxMat)
				delete proxMat;
			evSet.clear();
			return;
		}
		indexG = nextIndex;
		++nextIndex;
		indexGStar = nextIndex;
		++nextIndex;
	}

	//if(LocalMeanCheckBox->isChecked())
	//{
		status = TeLocalMean (&evSet, proxMat, indexAttr);
		if (!status)
		{
			if(TeProgress::instance())
				TeProgress::instance()->reset();
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"), tr("Fail to calculate Local Mean statistics!"));
			evSet.clear();
			return;
		}		
		
		indexLocalMean = nextIndex;			
		++nextIndex;
		indexNumberNeig = nextIndex;	
		++nextIndex;
	//}

	if (MoranLocalCheckBox->isChecked() || MoranGlobalCheckBox->isChecked())
	{	
		//standard deviation Z and local mean of the desviation Z (WZ)
		status = TeZAndWZ(&evSet, proxMat, indexAttr);
		if (!status)
		{
			if(TeProgress::instance())
				TeProgress::instance()->reset();
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"), 
				tr("Fail to calculate statistics!\n Verify if there are objects without attribute value!"));
			if(proxMat)
				delete proxMat;
			evSet.clear();
			return;
		}
		indexZ = nextIndex;
		++nextIndex;
		indexWZ = nextIndex;			
		++nextIndex;

		//local moran (LISA) and return global moran index
		double moran = TeMoranIndex (&evSet, indexZ, indexWZ);  
		indexLocalMoran = nextIndex;
		++nextIndex;

		//plot global moran index in the interface 
		if(MoranGlobalCheckBox->isChecked())
		{
			QString tmoran;
			tmoran.setNum(moran);
			MoranGlobalLineEdit->setText (tmoran);
		}

		// evaluating the significance of the global index.
		double globalMoranSignificance = 0.;
		if ((MoranGlobalCheckBox->isChecked()) && (!NotRadioButton->isChecked()))
		{	
			if(TeProgress::instance())
			{
				string caption = tr("Spatial Statistics").latin1();
				TeProgress::instance()->setCaption(caption.c_str());
				string msg = tr("Calculating the Moran Significance. Please, wait!").latin1();
				TeProgress::instance()->setMessage(msg);
			}
					
			int permNumber = 0;
			if ( perm999RadioButton->isChecked() )	
				permNumber = 999;
			else if ( perm99RadioButton->isChecked() )	
				permNumber = 99;

			//significance of the global moran index
			globalMoranSignificance =  TeGlobalMoranSignificance (&evSet, proxMat,
							indexAttr, permNumber, moran);

			//plot significance of the global moran index
			QString moranSignificance;
			moranSignificance.setNum(globalMoranSignificance);
			SignificanceLineEdit->setText (moranSignificance);
		}
		// box map 
		TeBoxMap (&evSet, indexZ, indexWZ, 0);   
		indexBoxMap = nextIndex;
		++nextIndex;
		if ((MoranLocalCheckBox->isChecked()) && (!notLocalRadioButton->isChecked()))
		{
			//evaluating LISA's significance	
			if(TeProgress::instance())
			{
				string caption = tr("Spatial Statistics").latin1();
				TeProgress::instance()->setCaption(caption.c_str());
				string msg = tr("Calculating the LISA significance. Please, wait!").latin1();
				TeProgress::instance()->setMessage(msg);
			}
			
			int permNumber = 0;
			if ( Local9999RadioButton->isChecked() )	
				permNumber = 9999;
			else if ( Local999RadioButton->isChecked() )	
				permNumber = 999;
			else if ( Local99RadioButton->isChecked() )	
				permNumber = 99;

			// LISA significance = probabilities
			status = TeLisaStatisticalSignificance (&evSet, indexZ,  indexLocalMoran, indexNumberNeig, permNumber); 
			if (!status)
			{
				if(TeProgress::instance())
					TeProgress::instance()->reset();
				wait.resetWaitCursor();
				QMessageBox::critical(this, tr("Error"), tr("Fail to calculate LISA Statistical Significance!"));
				if(proxMat)
					delete proxMat;
				evSet.clear();
				return;
			}
			indexProbLISA = nextIndex;
			++nextIndex;

			//LISA map
			TeLisaMap (&evSet, indexProbLISA, permNumber); 
			indexLISAMap = nextIndex;
			++nextIndex; 
			
			//Moran map
			TeMoranMap(&evSet, indexLISAMap, indexBoxMap); 
			indexMoranMap = nextIndex;
			++nextIndex; 
		}
	}

	//update database
	if(TeProgress::instance())
	{
		string caption = tr("Spatial Statistics").latin1();
		TeProgress::instance()->setCaption(caption.c_str());
		string msg = tr("Inserting into the database. Please, wait!").latin1();
		TeProgress::instance()->setMessage(msg);
	}

	//update attribute names to add a prefix
	string prefix = ColNameLineEdit->text().latin1();
	TeAttributeList& attrList = evSet.getAttributeList();
	for(unsigned int i=0; i<(attrList.size()); ++i)
	{
		string n; 
		if(i==(unsigned int)indexWZ)
			n= prefix + "Wz";
		else
			n= prefix + (attrList[i].rep_.name_);
		attrList[i].rep_.name_ = n;
	}
	table_->clear();

	//------------------------------------- Update indexes to database
	
	vector<int> indexesToUpdate;

	// update G in the table 
	if (GCheckBox->isChecked())
		indexesToUpdate.push_back(indexG);

	// update G* in the table 
	if (GStarCheckBox->isChecked())
		indexesToUpdate.push_back(indexGStar);

	// update local mean in the table 
	if(LocalMeanCheckBox->isChecked())
		indexesToUpdate.push_back(indexLocalMean);

	// update LISA and BoxMap in the table
	if (MoranLocalCheckBox->isChecked())
	{
		indexesToUpdate.push_back(indexLocalMoran);
		indexesToUpdate.push_back(indexBoxMap);
		indexesToUpdate.push_back(indexZ);
		indexesToUpdate.push_back(indexWZ);
	}

	// update Probabilities in the table  
	if (MoranLocalCheckBox->isChecked() && (!notLocalRadioButton->isChecked()))
	{
		indexesToUpdate.push_back(indexProbLISA);
		// update LisaMap in the table 
		indexesToUpdate.push_back(indexLISAMap);
		// update table: MoranMap
		indexesToUpdate.push_back(indexMoranMap);
	}

	bool insIndex;
	if(indexesToUpdate.empty())
	{
		if(TeProgress::instance())
			TeProgress::instance()->reset();
		wait.resetWaitCursor();
		QString msg = tr("The statistics were calculated successfully!");
		QMessageBox::information(this, tr("Information"), msg);
		if(proxMat)
			delete proxMat;
		table_->init(theme_);
		return;
	}
	else
		insIndex = TeUpdateDBFromSet(&evSet, resultTable.name(),&indexesToUpdate);
	

	// Update all the themes that uses this table
	vector<TeQtViewItem*> viewItemVec = mainWindow_->getViewsListView()->getViewItemVec();
	for (unsigned int i = 0; i < viewItemVec.size(); ++i)
	{
		vector<TeQtThemeItem*> themeItemVec = viewItemVec[i]->getThemeItemVec();
		for (unsigned int j = 0; j < themeItemVec.size(); ++j)
		{
			TeTheme* theme = (TeTheme*)(themeItemVec[j]->getAppTheme())->getTheme();
			if(theme->type() == TeTHEME)
			{
				if (theme->isThemeTable(resultTable.name()) == true)
				{
					((TeTheme*)theme_->getTheme())->layer()->loadLayerTables();
					((TeTheme*)theme_->getTheme())->loadThemeTables();
				}
			}
		}
	}

	((TeTheme*)theme_->getTheme())->loadAttrLists();
	table_->init(theme_);
	
	if(insIndex)
	{
		TeProgress::instance()->reset();
		wait.resetWaitCursor();
		QString msg = tr("The statistics were created successfully!");
		QMessageBox::information(this, tr("Information"), msg);
	}
	else
	{
		TeProgress::instance()->reset();
		wait.resetWaitCursor();
		QString msg = tr("Error storing statistics into the database!");
		QMessageBox::critical(this, tr("Error"), msg);
	}

	table_->goToLastColumn(); // deixe depois do message para dar tempo ao portal do grid se atualizar
	table_->refresh();
	if(proxMat)
		delete proxMat;
	return;
}


void Anaesp::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("anaesp.htm");
	if(help_->erro_ == false)
	{
		help_->show();
		help_->raise();
	}
	else
	{
		delete help_;
		help_ = 0;
	}
}



void Anaesp::changeAttributeComboSlot()
{
    MoranGlobalLineEdit->clear();
	SignificanceLineEdit->clear();
	ColNameLineEdit->clear();

}
