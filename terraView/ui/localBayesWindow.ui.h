#include <qapplication.h>
#include <TeUtils.h>
#include <TeQtTerraStat.h>
#include <TeBayesFunctions.h>
#include <TeSTElementSet.h>
#include <display.h>
#include "TeQtGrid.h"
#include "TeQtViewsListView.h"
#include "TeQtViewItem.h"
#include "TeQtThemeItem.h"
#include "TeDatabaseUtils.h"


void LocalBayesWindow::init()
{
	help_ = 0;

   // qApp - vari�vel global que representa a aplica��o   
    mainWindow_ = (TerraViewBase*)qApp->mainWidget();
    fillThemeCombos(ThemeComboBox, TePOLYGONS);
    fillTableCombos(TableComboBox, ThemeComboBox , "");
    fillTableCombos(TableComboBox2, ThemeComboBox , "");
    fillColumnCombos( ThemeComboBox, TableComboBox, ColumnComboBox, "" );
    fillColumnCombos( ThemeComboBox, TableComboBox2, ColumnComboBox2, "" );
}


void LocalBayesWindow::TableComboBox_activated( const QString&)
{
   fillColumnCombos( ThemeComboBox, TableComboBox, ColumnComboBox, "" );
 }


void LocalBayesWindow::ColumnComboBox_activated( const QString&)
{

}


bool LocalBayesWindow::fillThemeCombos( QComboBox * cBox, TeGeomRep rep )
{	
	cBox->clear();
	int idx = 0;
	int count = 0;
	bool done = false;

	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> curThemeItemVec = curViewItem->getThemeItemVec();
	for (unsigned int i = 0; i < curThemeItemVec.size(); ++i)
	{
		TeTheme* theme = (TeTheme*)curThemeItemVec[i]->getAppTheme()->getTheme();
		if(theme->type() != TeTHEME)
			continue;

		if ((rep == TeGEOMETRYNONE) || theme->layer()->hasGeometry(rep))
		{
			//fill combo with all themes frow view
			cBox->insertItem(theme->name().c_str());
			++count;
		}
	}
	cBox->setCurrentItem(idx);
	return done;
}


bool LocalBayesWindow::fillColumnCombos( QComboBox * cThemes, QComboBox * cTables, QComboBox * cColumns, string prevName )
{
	if(cThemes->currentText().isEmpty())
	{
		return false;
	}

	bool done = false;
	TeTheme* currTheme = mainWindow_->currentView()->get(string(cThemes->currentText().ascii()));

	if (currTheme == 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("There is no selected theme!"));
		return false;
	}

	TeAttributeList columns;
	TeDatabase* db = currTheme->layer()->database();
	TeAttrTableVector atvec;
	currTheme->getAttTables(atvec);

	TeTable ttable = atvec[cTables->currentItem()];
	db->getAttributeList(ttable.name(), columns);

	cColumns->clear();
	int idx = 0;
	int count = 0;
	for (unsigned int i=0; i<columns.size(); i++)
	{
		TeAttributeRep rep = columns[i].rep_;
		int type = rep.type_;
		if(type == TeREAL || type == TeINT)
		{
			cColumns->insertItem(rep.name_.c_str());
			if (!prevName.compare(rep.name_))
			{
				idx = count;
				done = true;
			}
			count++;
		}
	}
	cColumns->setCurrentItem(idx);
	return done;
}


bool LocalBayesWindow::fillTableCombos( QComboBox *cTableCombo, QComboBox *cThemeCombo , string prevName )
{
	if(cThemeCombo->currentText().isEmpty())
	{
		return false;
	}

    bool done = false;
  unsigned int i;
  TeTable ttable;
  string tableName;
  TeTheme* currTheme = mainWindow_->currentView()->get(string(cThemeCombo->currentText().ascii()));
  if (currTheme == NULL) {
     QMessageBox::warning(this, tr("Warning"), tr("There is no selected theme!"));
	 return false;
  }
  TeAttrTableVector atvec;
  currTheme->getAttTables(atvec);
  
  cTableCombo->clear();
  
  int idx = 0;
  for(i=0; i<atvec.size(); i++)
  {
    TeTable ttable = atvec[i];
    tableName = ttable.name();
    cTableCombo->insertItem(tableName.c_str());
    if (!prevName.compare(tableName)) {
      idx = i;
      done = true;
  }
  }
  cTableCombo->setCurrentItem(idx);
  return done;
}


void LocalBayesWindow::TableComboBox2_activated(const QString &)
{
  fillColumnCombos( ThemeComboBox, TableComboBox2, ColumnComboBox2, ""); 
}


void LocalBayesWindow::ThemeComboBox_activated( const QString & )
{
    fillTableCombos(TableComboBox, ThemeComboBox , "");
    fillTableCombos(TableComboBox2, ThemeComboBox , "");
    fillColumnCombos( ThemeComboBox, TableComboBox, ColumnComboBox, "" );
    fillColumnCombos( ThemeComboBox, TableComboBox2, ColumnComboBox2, "" );
}

/***************************************************************
** Execucao do Local Empirical Bayes
***************************************************************/

void LocalBayesWindow::executeGlobalEmpiricalBayes()
{
	if(ThemeComboBox->currentText().isEmpty())
	{
		return;
	}

	okPushButton->setEnabled(FALSE);

	vector <string> att; //Used to storage text
	double MRate = 1;

	// get the current ComboBox names
	string Theme_name   = string(ThemeComboBox->currentText().ascii());
	string Pop_table    = string(TableComboBox->currentText().ascii());
	string Pop_Column   = string(ColumnComboBox->currentText().ascii());
	string Cases_table  = string(TableComboBox2->currentText().ascii());
	string Cases_column = string(ColumnComboBox2->currentText().ascii());
	string New_column   = string(gridResultLineEdit->text().ascii());

	switch( RatebuttonGroup->selectedId() )
	{
	case 2:
		MRate = 100;
		break;
	case 3:
		MRate = 1000;
		break;
	case 4:
		MRate = 100000;
		break;
	}

	att.push_back(Pop_table+"."+Pop_Column);
    att.push_back(Cases_table+"."+Cases_column);
	
	// points to the desired Theme
	TeTheme* tempTheme = (TeTheme*)mainWindow_->currentView()->get(Theme_name);
  
	TeSTElementSet StoSet;
	if (!createSTOSetFromTheme(tempTheme, att,&StoSet)) 
	{
		QMessageBox::warning(this, tr("Error"), tr("Can't load data!"));
		return;
	}

	//get the first static table of this theme to generate the new column
	TeAttrTableVector tables;
	string updateTable; 
	if(tempTheme->getAttTables(tables, TeAttrStatic))
	{
        TeAttrTableVector::iterator attrIt = tables.begin();
		updateTable = (attrIt->name()).c_str(); 
	}
	else
		updateTable = Pop_table;
	
	if(updateTable.empty())
	{
		QMessageBox::warning(this, tr("Error"), tr("Can't load table name!"));
		return;
	}
	
	mainWindow_->getGrid()->clear();

	double tol = TeGetPrecision(tempTheme->layer()->projection());
	TePrecision::instance().setPrecision(tol);

	//load prox matrix or construct one
	TeGeneralizedProxMatrix<TeSTElementSet>*			proxMat=0;
	TeProxMatrixConstructionStrategy<TeSTElementSet>*	constStrategy=0;

	//-------------- Load prox matrix or create new
	if(!loadDefaultGPM(tempTheme->layer()->database(), tempTheme->id(), proxMat))
	{
		TeGeomRep geomRep = TeGEOMETRYNONE;
		if(tempTheme->layer()->hasGeometry (TePOLYGONS))
			geomRep = TePOLYGONS;
		else if (tempTheme->layer()->hasGeometry (TeCELLS))
			geomRep = TeCELLS;

		if(geomRep==TeGEOMETRYNONE)
		{
			QString msg = tr("The theme must have a geometric representation of polygons") + "\n";
			msg += tr("or cells to create contiguity matrix!");
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}
		
		if(TeProgress::instance())
		{
			string caption = tr("Local Bayes").latin1();
			TeProgress::instance()->setCaption(caption.c_str());
			string msg = tr("Building the Proximity Matrix. Please, wait!").latin1();
			TeProgress::instance()->setMessage(msg);
		}
		
		//create matrix and save in the database
		constStrategy = new TeProxMatrixLocalAdjacencyStrategy (&StoSet, geomRep); 
		TeProxMatrixNoWeightsStrategy sw_no;
		TeProxMatrixNoSlicingStrategy ss_no;
		proxMat = new TeGeneralizedProxMatrix<TeSTElementSet>(constStrategy, &sw_no, &ss_no);
		
		if (!proxMat->constructMatrix())
		{
			QMessageBox::critical(this, tr("Error"), tr("Fail building the proximity matrix!"));
			if(proxMat)
				delete proxMat;
			return;
		}

		//save in the database
		bool savedMatrix = true;
		if(!tempTheme->layer()->database()->tableExist("te_gpm"))
		{
			if(!createGPMMetadataTable(tempTheme->layer()->database()))
			{
				QMessageBox::warning(this, tr("Error"), 
					tr("It was not possible to save proximity matrix in database!"));
				savedMatrix = false;
			}
		}

		string tableName = "te_neighbourhood_"+ Te2String(tempTheme->id());
		string tempTable = tableName;
		int count = 1;
		while(tempTheme->layer()->database()->tableExist(tempTable))
		{
			tempTable = tableName+"_"+Te2String(count);
			++count;
		}

		if(savedMatrix && (!createNeighbourhoodTable(tempTheme->layer()->database(), tempTable)))
		{
			QMessageBox::warning(this, tr("Error"), 
					tr("It was not possible to save proximity matrix in database!"));
			savedMatrix = false;
		}

		proxMat->neighbourhood_table_=tempTable;

		//load object ids
		vector<string> objIds;
		TeSTElementSet::iterator itAux = StoSet.begin();
		while(itAux!=StoSet.end())
		{
			objIds.push_back(itAux->objectId());
			++itAux;
		}
		
		if(savedMatrix && (!insertGPM(tempTheme->layer()->database(), proxMat, objIds)))
		{
			QMessageBox::warning(this, tr("Error"), 
					tr("It was not possible to save proximity matrix in database!"));
			savedMatrix = false;
		}
	}
	
	// The Local Empirical Bayes Method
	if(	TeLocalEmpiricalBayes(&StoSet, proxMat, New_column, MRate) )
	{
		// InsertPropertyColumn
		if(!insertPropertyColumn(StoSet, updateTable, New_column))
			QMessageBox::warning(this, tr("Error"), tr("Fail to store data into the database!"));
		else
			QMessageBox::information(this, tr("Information"), tr("The column was created!"));
	}
	else
	{
		QString info = tr(" Non-consistent data set! \n");
		info += tr(" Verify if there are null or zero values in the population attribute."); 
		QMessageBox::warning(this, tr("Error"), info);
	}

	
	// ---------------------------------------------- load updated table from database 
	// Update all the themes that uses this table
	set<TeLayer*> layerSet;
	vector<TeQtViewItem*> viewItemVec = mainWindow_->getViewsListView()->getViewItemVec();

	for (unsigned int i = 0; i < viewItemVec.size(); ++i)
	{
		vector<TeQtThemeItem*> themeItemVec = viewItemVec[i]->getThemeItemVec();
		for (unsigned int j = 0; j < themeItemVec.size(); ++j)
		{
			TeTheme* theme = (TeTheme*)(themeItemVec[j]->getAppTheme())->getTheme();
			if(theme->type() == TeTHEME)
			{
				if (theme->isThemeTable(Pop_table) == true)
				{
					theme->loadThemeTables();
					layerSet.insert(theme->layer());
				}
			}
		}
	}

	// Update the layer tables affected
	set<TeLayer*>::iterator setIt;
	for (setIt = layerSet.begin(); setIt != layerSet.end(); ++setIt)
		(*setIt)->loadLayerTables();

	// ---------------------------------------------- set new theme tree  

	// Get the tempAppTheme from tempTheme
	TeAppTheme* tempAppTheme;
	TeQtViewsListView* viewsListView = mainWindow_->getViewsListView();
	vector<TeQtThemeItem*> themeItemVec = viewsListView->getThemeItemVec();
	for (unsigned int i = 0; i < themeItemVec.size(); ++i)
	{
		tempAppTheme = themeItemVec[i]->getAppTheme();
		if(tempAppTheme->getTheme()->type() == TeTHEME)
		{
			if (tempAppTheme->getTheme() == tempTheme)
				break;
		}
	}

	TeQtThemeItem* themeItem = viewsListView->getThemeItem(tempAppTheme);
	viewsListView->selectItem ((QListViewItem*) themeItem);
	
	// ---------------------------------------------- grid 
	if(mainWindow_->getGrid()->getPortal() == 0)
		mainWindow_->getGrid()->init(tempAppTheme);
	mainWindow_->getGrid()->goToLastColumn();
	
	if(proxMat)
		delete proxMat;
	close(); 
}


void LocalBayesWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("localBayesWindow.htm");
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
