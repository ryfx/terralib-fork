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


void ChangeColumnName::init()
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

	oldNameLineEdit->setText(colName.c_str());

}


void ChangeColumnName::okPushButton_clicked()
{
	TeWaitCursor wait;
	QString newName = newNameLineEdit->text();
	if(newName.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"),tr("Type new column name!"));
		return;
	}

	TeDatabase *db = mainWindow_->currentDatabase();
	string name = newName.latin1();

	TeAttrTableVector atvec; 
	((TeTheme*)theme_->getTheme())->getAttTables(atvec);
	TeTable table = atvec[tableIndex_];
	string oldName = table.attributeList()[colIndex_].rep_.name_;
	table.attributeList()[colIndex_].rep_.name_ = name;

	if(db->validTable(table))
	{
		string validColumnName = table.attributeList()[colIndex_].rep_.name_;

		QString msg = tr("This name is not valid! The system will convert to:") + " ";
		msg += validColumnName.c_str();
		msg += "\n" + tr("Continue?");
		int response = QMessageBox::question(this, tr("Convert Automatically"), msg, tr("Yes"), tr("No"));

		if(response == 0)
			name = validColumnName;
		else
			return;
	}

	string tableName = table.name();
	TeAttributeRep rep = table.attributeList()[colIndex_].rep_;
	rep.name_ = name;

	TeQtGrid* grid = mainWindow_->getGrid();

	int x = grid->contentsX();
	int y = grid->contentsY();

	grid->clearPortal();

	if(db->alterTable(tableName, rep, oldName) == false)
	{
		QMessageBox::information(this, tr("Error"), tr("Column name not changed!"));
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

	//TeViewMap& viewMap = db->viewMap();
	//TeViewMap::iterator it;
	//for (it = viewMap.begin(); it != viewMap.end(); ++it)
	//{
	//	TeView *view = it->second;
	//	vector<TeViewNode*>& themesVector = view->themes();
	//	for (unsigned int i = 0; i < themesVector.size(); ++i)
	//	{
	//		TeTheme *theme = (TeAppTheme*)themesVector[i];
	//		if (theme->isThemeTable(table.id()) == true)
	//		{
	//			theme->layer()->loadLayerTables();
	//			theme->loadThemeTables();
	//		}
	//	}
	//}

	grid->initPortal();
	grid->setContentsPos(x, y);
	grid->refresh();

	accept();
}



void ChangeColumnName::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("changeColumnName.htm");
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




void ChangeColumnName::show()
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
