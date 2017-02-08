/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright © 2001-2007 INPE and Tecgraf/PUC-Rio.
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

#include <TeDatabase.h>
#include <qmessagebox.h>


void CountComputing::init( TeAppTheme * theme )
{
	help_ = 0;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	theme_ = theme;
}

void CountComputing::okPushButton_clicked()
{
	QString prefix = prefixLineEdit->text();
	if(prefix.isEmpty())
	{
		QString msg = tr("Enter with the column prefix!");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}
	else
	{
		// verify if exist column name
		TeDatabase* db = ((TeTheme*)theme_->getTheme())->layer()->database();
		TeAttrTableVector attrTableVector; 
		((TeTheme*)theme_->getTheme())->getAttTables(attrTableVector);
		TeTable& table = attrTableVector[0];

		string pref = prefix.latin1();
		string colName = prefix.latin1();
		colName += "_COUNT";

		string errorMessage;
		bool changed;
		string newName = TeCheckName(colName, changed, errorMessage); 

		if(changed)
		{
			QString mess = tr("The prefix name is invalid: ") + errorMessage.c_str();
					mess += "\n" + tr("Change current name and try again.");
					QMessageBox::warning(this, tr("Warning"), mess);

				return;
		}

		colName = newName;

		TeAttribute attr;
		if(db->columnExist(table.name(), colName, attr) == true)
		{
			QString msg = tr("The column already exists! Enter with another column prefix.");
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}

		mainWindow_->countingPrefix_ = pref;
		hide();
	}
}


void CountComputing::closeEvent( QCloseEvent * )
{
	mainWindow_->countingPrefix_.clear();
	hide();
}


void CountComputing::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("countComputing.htm");
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


void CountComputing::show()
{
	TeAppTheme* curAppTheme = mainWindow_->currentTheme();
	TeTheme* curTheme = (TeTheme*)curAppTheme->getTheme();
	if(curTheme->type() == TeTHEME)
		QDialog::show();
	else
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		reject();
		hide();
	}
}
