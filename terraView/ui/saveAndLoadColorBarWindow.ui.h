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
#include <TeAttribute.h>
#include <terraViewBase.h>
#include <qmessagebox.h>
#include <qapplication.h>

void SaveAndLoadColorBarWindow::setColor( const QString& color)
{
	color_ = color;
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	db_ = mainWindow_->currentDatabase();
	TeDatabasePortal* portal = db_->getPortal();

	saveNamesComboBox->clear();
	saveNameLineEdit->setText("");
	saveGroupBox->setEnabled(true);
	saveNamesComboBox->insertItem("");

	if(db_->tableExist("tv_colorbar") == false)
	{
		TeAttributeList atl;
		TeAttribute		at;

		at.rep_.name_ = "name";
		at.rep_.type_ = TeSTRING;
		at.rep_.numChar_ = 150;
		at.rep_.isPrimaryKey_ = true;
		atl.push_back(at);

		at.rep_.isPrimaryKey_ = false;
		at.rep_.name_ = "colorbar";
		at.rep_.numChar_ = 255;
		atl.push_back(at);

		if(db_->createTable("tv_colorbar", atl) == false)
		{
			QMessageBox::warning(this, tr("Error"),
				tr("Fail to create tv_colorbar table!"));
		}
	}

	if(portal->query("SELECT name FROM tv_colorbar ORDER BY name"))
	{
		while(portal->fetchRow())
		{
			QString name = portal->getData(0);
			saveNamesComboBox->insertItem(name);
		}
	}
	delete portal;

	saveNamesComboBox->setCurrentItem(0);
	savedColorName_ = "";
}

void SaveAndLoadColorBarWindow::loadNamesComboBox_activated( const QString & )
{

}

void SaveAndLoadColorBarWindow::applyPushButton_clicked()
{
	QString name = saveNameLineEdit->text();
	if(name.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Type color bar name and try again!"));
		return;
	}

	if(name != saveNamesComboBox->currentText())
	{
		int i;
		for(i=0; i<saveNamesComboBox->count(); ++i)
		{
			QString s = saveNamesComboBox->text(i);
			if(s == name)
			{
				int response = QMessageBox::question(this, tr("name already exist"),
						tr("Do you to modify the color bar?"),
						tr("Yes"), tr("No"));

				if (response != 0)
					return;
			}
		}
	}

	bool jatem = false;
	TeDatabasePortal* portal = db_->getPortal();
	QString sel = "SELECT name FROM tv_colorbar WHERE name = '" + name + "'";
	if(portal->query(sel.latin1()))
	{
		if(portal->fetchRow())
			jatem = true;
	}
	delete portal;
	
	string ss;
	if(jatem)
	{
		ss = "UPDATE tv_colorbar set colorbar = '";
		ss += color_.latin1();
		ss += "' WHERE name = '";
		ss += name.latin1();
		ss += "'";
	}
	else
	{
		ss = "INSERT INTO tv_colorbar (name, colorbar) VALUES ('";
		ss += name.latin1();
		ss += "', '";
		ss += color_.latin1();
		ss += "')";
	}
	db_->execute(ss);
	savedColorName_ = name;
	accept();
}

void SaveAndLoadColorBarWindow::reject()
{
	QDialog::reject();
}


QString SaveAndLoadColorBarWindow::getColorName()
{
    return savedColorName_;
}
