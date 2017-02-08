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
#include <terraViewBase.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <TeQtViewsListView.h>

#include <help.h>

void AddColumn::init()
{
	help_ = 0;
	unsigned int i;

	mainWindow_= (TerraViewBase*)qApp->mainWidget();
	theme_ = mainWindow_->currentTheme();
	if(theme_->getTheme()->type() != TeTHEME)
		return;

	int col = mainWindow_->gridColumn();

	TeAttrTableVector atvec; 
	((TeTheme*)theme_->getTheme())->getAttTables(atvec);

	int	k = -1, n = 0;
	for(i=0; i<atvec.size(); i++)
	{
		tableComboBox->insertItem(atvec[i].name().c_str());
		n += atvec[i].attributeList().size();
		if(n>col && k==-1)
			k = i;
	}
	tableComboBox->setCurrentItem(k);

	dataTypeComboBox->insertItem(tr("Real"));
	dataTypeComboBox->insertItem(tr("Integer"));
	dataTypeComboBox->insertItem("String");
	dataTypeComboBox->insertItem(tr("Date"));
	dataTypeComboBox->setCurrentItem(0);

	sizeComboBox->insertItem("10");
	sizeComboBox->insertItem("20");
	sizeComboBox->insertItem("30");
	sizeComboBox->insertItem("40");
	sizeComboBox->insertItem("50");
	sizeComboBox->insertItem("100");
	sizeComboBox->insertItem("150");
	sizeComboBox->insertItem("200");
	sizeComboBox->insertItem("250");
	sizeComboBox->setCurrentItem(2);

	dataTypeComboBox_activated("");
}


void AddColumn::dataTypeComboBox_activated( const QString & )
{
	QString qs = dataTypeComboBox->currentText();
	if(qs.isEmpty())
		return;
	if(qs == "String")
		sizeComboBox->setEnabled(true);
	else
		sizeComboBox->setEnabled(false);
}


void AddColumn::okPushButton_clicked()
{
	QString qs = columnNameLineEdit->text();
	if(qs.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Enter with the column name!"));
		return;
	}

	string colName = qs.latin1();
	string tableName = tableComboBox->currentText().latin1();
	TeDatabase* db = mainWindow_->currentDatabase();

	TeWaitCursor wait;
	TeAttribute at;
	if(db->columnExist(tableName, colName, at) == true)
	{
		wait.resetWaitCursor();
		QString msg = tr("The table") + " \"" + tableName.c_str() + "\" ";
		msg += tr("already has a column with this name!") + "\n";
		msg += tr("Enter with another column name.");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}
	
	string errorMessage;
	bool changed;

	string newName = TeCheckName(colName, changed, errorMessage); 
	wait.resetWaitCursor();

	if(changed)
	{
		QString mess = tr("The column name is invalid: ") + errorMessage.c_str();
				mess += "\n" + tr("Change current name and try again.");
				QMessageBox::warning(this, tr("Warning"), mess);

			return;
	}

	// update column
	TeAttributeRep atrep;

	if(dataTypeComboBox->currentText() == tr("Real"))
	{
		atrep.type_ = TeREAL;
		atrep.decimals_ = 10;
	}
	else if(dataTypeComboBox->currentText() == tr("Integer"))
		atrep.type_ = TeINT;
	else if(dataTypeComboBox->currentText() == "String")
	{
		atrep.type_ = TeSTRING;
		atrep.numChar_ = atoi(sizeComboBox->currentText().latin1());
	}
	else
		atrep.type_ = TeDATETIME;

	atrep.name_ = newName;

	TeQtGrid* grid = mainWindow_->getGrid();
	if(grid == 0)
	{
		wait.resetWaitCursor();
		QMessageBox::critical(this, tr("Error"), tr("The grid was not initialized!"));
		return;
	}

	TeAttrTableVector ta; 
	((TeTheme*)theme_->getTheme())->getAttTables(ta);
	int cpos = 0;
	for(unsigned int i = 0; i < ta.size(); ++i)
	{
		cpos += ta[i].attributeList().size();

		if(TeConvertToUpperCase(ta[i].name()) == TeConvertToUpperCase(tableName))
			break;
	}

	grid->clearPortal();

	if(db->addColumn(tableName, atrep) == false)
	{
		grid->initPortal();
		grid->refresh();
		wait.resetWaitCursor();
		QString msg = tr("The column could not be appended!") + "\n";
		msg += db->errorMessage().c_str();		
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}

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
				if (theme->isThemeTable(tableName) == true)
				{
					theme->layer()->loadLayerTables();
					theme->loadThemeTables();
				}
			}
		}
	}

	grid->initPortal();
	grid->refresh();
	grid->goToLastColumn();
	wait.resetWaitCursor();
	QMessageBox::information(this, tr("Information"), tr("The column was appended successfully!"));	
	hide();
	grid->goToLastColumn();
}


void AddColumn::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("addColumn.htm");
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



void AddColumn::show()
{
	if(theme_->getTheme()->type() == TeTHEME)
		QDialog::show();
	else
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		reject();
		hide();
	}
}
