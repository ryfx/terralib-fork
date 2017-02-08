#include <qapplication.h>
#include <TeUtils.h>
#include <TeQtTerraStat.h>
#include <TeBayesFunctions.h>
#include <display.h>
#include "TeQtGrid.h"
#include "TeQtViewsListView.h"
#include "TeQtViewItem.h"
#include "TeQtThemeItem.h"


void BayesWindow::init()
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


void BayesWindow::TableComboBox_activated( const QString&)
{
   fillColumnCombos( ThemeComboBox, TableComboBox, ColumnComboBox, "" );
 }


void BayesWindow::ColumnComboBox_activated( const QString&)
{

}


bool BayesWindow::fillThemeCombos( QComboBox * cBox, TeGeomRep rep )
{	
	cBox->clear();
	int idx = 0;
	int count = 0;
	bool done = false;

	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	for (unsigned int i = 0; i < themeItemVec.size(); ++i)
	{
		TeAppTheme* appTheme = themeItemVec[i]->getAppTheme();
		TeTheme* theme = (TeTheme*)appTheme->getTheme();
		if ((rep == TeGEOMETRYNONE) || theme->layer()->hasGeometry(rep))
		{	
			//fill combo with all themes frow view
			cBox->insertItem(theme->name().c_str());
			count++;
		}
	}
	cBox->setCurrentItem(idx);
	return done;
}


bool BayesWindow::fillColumnCombos( QComboBox * cThemes, QComboBox * cTables, QComboBox * cColumns, string prevName )
{
	if(cThemes->currentText().isEmpty())
	{
		return false;
	}

	bool done = false;
	TeTheme* currTheme = mainWindow_->currentView()->get(
	string(cThemes->currentText().ascii()));
	if (currTheme == NULL) {
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
	for(unsigned int i=0; i<columns.size(); i++)
	{
		TeAttributeRep rep = columns[i].rep_;
		int type = rep.type_;
		if(type == TeREAL || type == TeINT) 
		{
			cColumns->insertItem(rep.name_.c_str());
			if (!prevName.compare(rep.name_)) {
				idx = count;
				done = true;
			}
			count++;
		}
	}
	cColumns->setCurrentItem(idx);
	return done;
}


bool BayesWindow::fillTableCombos( QComboBox *cTableCombo, QComboBox *cThemeCombo , string prevName )
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


void BayesWindow::TableComboBox2_activated(const QString &)
{
  	fillColumnCombos( ThemeComboBox, TableComboBox2, ColumnComboBox2, ""); 
}


void BayesWindow::ThemeComboBox_activated( const QString & )
{
    fillTableCombos(TableComboBox, ThemeComboBox , "");
    fillTableCombos(TableComboBox2, ThemeComboBox , "");
    fillColumnCombos( ThemeComboBox, TableComboBox, ColumnComboBox, "" );
    fillColumnCombos( ThemeComboBox, TableComboBox2, ColumnComboBox2, "" );
}

/***************************************************************
** Execucao do Global Empirical Bayes
***************************************************************/

void BayesWindow::executeGlobalEmpiricalBayes()
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

	TeTheme* tempTheme = 0;
	TeAppTheme* tempAppTheme = 0;
	TeQtThemeItem* themeItem = 0;
	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	for (unsigned int i=0; i<themeItemVec.size(); ++i)
	{
		TeTheme* theme = (TeTheme*)(themeItemVec[i]->getAppTheme())->getTheme();
		if (theme->name() == Theme_name)
		{
			themeItem = themeItemVec[i];
			tempTheme = theme;
			tempAppTheme = themeItemVec[i]->getAppTheme();
			break;
		}
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

	TeSTStatInstanceSet StoSet;
	if (!createSTOStatSetFromTheme(tempTheme, att,1, &StoSet)) {
		QMessageBox::warning(this, tr("Error"), tr("Can't load data!"));
		return;
	}

	mainWindow_->getGrid()->clear();

	// The Global Empirical Bayes Method
	if (TeGlobalEmpiricalBayes(&StoSet,MRate) )
	{
		// InsertPropertyColumn
		if(!insertPropertyColumn(StoSet, 2,tempTheme, updateTable, New_column))
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
	TeViewMap::iterator it;
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
	mainWindow_->getViewsListView()->selectItem(themeItem);
	
	// ---------------------------------------------- grid 
	if(mainWindow_->getGrid()->getPortal() == 0)
		mainWindow_->getGrid()->init(tempAppTheme);
	mainWindow_->getGrid()->goToLastColumn();

	close(); 
}


void BayesWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("bayesWindow.htm");
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
