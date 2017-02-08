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

#include <qmessagebox.h>
#include <qapplication.h>
#include <TeAppTheme.h>
#include <terraViewBase.h>
#include <TeQtGrid.h>
#include <TeWaitCursor.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <TeQtViewsListView.h>

void ChangeColumnType::init()
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

	int	n = 0, nn = 0;
	for(i=0; i<atvec.size(); i++)
	{
		n += atvec[i].attributeList().size();
		if(n > col)
		{
			colIndex_ = col - nn;
			break;
		}
		nn = n;
	}
	tableIndex_ = i;

	string table = atvec[tableIndex_].name();
	string colName = atvec[tableIndex_].attributeList()[colIndex_].rep_.name_;

	oldTableNameLineEdit->setText(table.c_str());
	oldColumnNameLineEdit->setText(colName.c_str());

	int type = atvec[tableIndex_].attributeList()[colIndex_].rep_.type_;
	if(type == TeINT)
		oldColumnTypeLineEdit->setText("INT");
	else if(type == TeREAL)
		oldColumnTypeLineEdit->setText("REAL");
	else if(type == TeSTRING)
		oldColumnTypeLineEdit->setText("STRING");
	else if(type == TeDATETIME)
		oldColumnTypeLineEdit->setText("DATETIME");
	else
		oldColumnTypeLineEdit->setText("UNKNOWN");

	if(type == TeSTRING)
	{
		int size = atvec[tableIndex_].attributeList()[colIndex_].rep_.numChar_;
		oldColumnSizeLineEdit->setText(Te2String(size).c_str());
	}
	else
		oldColumnSizeLineEdit->setText("");

	newColumnTypeComboBox->insertItem("INT");
	newColumnTypeComboBox->insertItem("REAL");
	newColumnTypeComboBox->insertItem("STRING");
	newColumnTypeComboBox->insertItem("DATETIME");

	newSizeComboBox->insertItem("1");
	newSizeComboBox->insertItem("5");
	newSizeComboBox->insertItem("10");
	newSizeComboBox->insertItem("15");
	newSizeComboBox->insertItem("20");
	newSizeComboBox->insertItem("25");
	newSizeComboBox->insertItem("30");
	newSizeComboBox->insertItem("50");
	newSizeComboBox->insertItem("100");
	newSizeComboBox->insertItem("150");
	newSizeComboBox->insertItem("200");
	newSizeComboBox->insertItem("255");
	newSizeComboBox->insertItem("");

	newColumnTypeComboBox_activated("");
}


void ChangeColumnType::okPushButton_clicked()
{
	TeWaitCursor wait;

	QString newType = newColumnTypeComboBox->currentText();
	QString oldType = oldColumnTypeLineEdit->text();
	QString newSize = newSizeComboBox->currentText();
	QString oldSize = oldColumnTypeLineEdit->text();

	if(newType == oldType)
	{
		if(newType == "STRING")
		{
			if(newSize == oldSize)
			{
				QMessageBox::warning(this, tr("Warning"),tr("Type not modified!"));
				return;
			}
		}
		else
		{
			QMessageBox::warning(this, tr("Warning"),tr("Type not modified!"));
			return;
		}
	}

	TeDatabase *db = mainWindow_->currentDatabase();

	TeAttrTableVector atvec; 
	((TeTheme*)theme_->getTheme())->getAttTables(atvec);
	TeTable table = atvec[tableIndex_];
	string oldName = table.attributeList()[colIndex_].rep_.name_;

	if ((db->dbmsName()=="OracleAdo") || (db->dbmsName()=="OracleSpatial"))
	{
		string sel = "SELECT COUNT(*) FROM " + table.name() + " WHERE " + oldName + " is not null";
		TeDatabasePortal *portal = db->getPortal();
		if(portal->query(sel) && portal->fetchRow())
		{
			int c = atoi(portal->getData(0));
			if(c > 0)
			{
				QMessageBox::warning(this, tr("Warning"),tr("Column must be empty to change the type!") + "\n" + tr("Make it empty and try again."));
				delete portal;
				return;
			}
		}
		delete portal;
	}

	table.attributeList()[colIndex_].rep_.numChar_ = 0;
	if(newType == "INT")
		table.attributeList()[colIndex_].rep_.type_ = TeINT;
	else if(newType == "REAL")
	{
		table.attributeList()[colIndex_].rep_.type_ = TeREAL;
		table.attributeList()[colIndex_].rep_.decimals_ = 10;
	}
	else if(newType == "DATETIME")
		table.attributeList()[colIndex_].rep_.type_ = TeDATETIME;
	else if(newType == "STRING")
	{
		table.attributeList()[colIndex_].rep_.type_ = TeSTRING;
		table.attributeList()[colIndex_].rep_.numChar_ = newSize.toInt();
	}

	db->validTable(table);

	string tableName = table.name();
	TeAttributeRep rep = table.attributeList()[colIndex_].rep_;

	TeQtGrid* grid = mainWindow_->getGrid();

	int x = grid->contentsX();
	int y = grid->contentsY();

	grid->clearPortal();

	if(db->alterTable(tableName, rep, "") == false)
	{
		QString msg = tr("Column type not changed!") + "\n";
		msg += db->errorMessage().c_str();
		QMessageBox::information(this, tr("Error"), msg);
		grid->initPortal();
		grid->setContentsPos(x, y);
		grid->refresh();
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
				if (theme->isThemeTable(table.id()) == true)
				{
					theme->layer()->loadLayerTables();
					theme->loadThemeTables();
				}
			}
		}
	}

	grid->initPortal();
	grid->setContentsPos(x, y);
	grid->refresh();

	QMessageBox::information(this, tr("Success"), tr("Column changed!"));
	accept();

}


void ChangeColumnType::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("changeColumnType.htm");
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


void ChangeColumnType::newColumnTypeComboBox_activated( const QString &s )
{
	if(s != "STRING")
	{
		newSizeComboBox->setCurrentItem(newSizeComboBox->count()-1);
		newSizeComboBox->setEnabled(false);
	}
	else
	{
		newSizeComboBox->setCurrentItem(5);
		newSizeComboBox->setEnabled(true);
	}
}


void ChangeColumnType::newSizeComboBox_activated( const QString & )
{
	QString s = newColumnTypeComboBox->currentText();

	if(s.isEmpty())
		newSizeComboBox->setCurrentItem(newSizeComboBox->count()-2);
}


void ChangeColumnType::show()
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
