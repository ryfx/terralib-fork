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

#include <terraViewBase.h>
#include <TeAppTheme.h>
#include <TeLayer.h>
#include <TeDatabase.h>
#include <qapplication.h>
#include <qmessagebox.h>

void CreateTextRepresentation::init()
{
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	theme_ = mainWindow_->currentTheme();
	layer_ = ((TeTheme*)theme_->getTheme())->layer();
	db_ = layer_->database();
}


void CreateTextRepresentation::OKPushButton_clicked()
{
	if(tableLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Enter with the name of the representation table!"));
		return;
	}
	if (db_->tableExist(tableLineEdit->text().latin1()))
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("There is already a table with this name in the database! Give another name."));
		return;
	}

	string table = tableLineEdit->text().latin1();

	if(layer_->addGeometry(TeTEXT, table, "") == false)
	{
		QMessageBox::critical(this, tr("Error"),
			tr("Fail to create the text representation!"));
		return;
	}


	string tvis = table + "_txvisual";
	TeAttributeList atl;
	TeAttribute at;

	at.rep_.name_ = "geom_id";
	at.rep_.type_ = TeINT;
	at.rep_.numChar_ = 0;
	at.rep_.isPrimaryKey_ = true;
	atl.push_back(at);

	at.rep_.isPrimaryKey_ = false;
	at.rep_.name_ = "dot_height";
	at.rep_.type_ = TeINT;
	atl.push_back(at);

	at.rep_.name_ = "fix_size";
	at.rep_.type_ = TeINT;
	atl.push_back(at);

	at.rep_.name_ = "color";
	at.rep_.type_ = TeINT;
	atl.push_back(at);

	at.rep_.name_ = "family";
	at.rep_.type_ = TeSTRING;
	at.rep_.numChar_ = 128;
	atl.push_back(at);

	at.rep_.name_ = "bold";
	at.rep_.type_ = TeINT;
	at.rep_.numChar_ = 0;
	atl.push_back(at);

	at.rep_.name_ = "italic";
	at.rep_.type_ = TeINT;
	atl.push_back(at);

	db_->createTable(tvis, atl);
	string fk = "fk_" + tvis;
	db_->createRelation(fk, tvis, "geom_id", table, "geom_id", true);

	long vrep = ((TeTheme*)theme_->getTheme())->visibleRep();
	vrep |= TeTEXT;
	((TeTheme*)theme_->getTheme())->visibleRep(vrep);

	string s = "UPDATE te_theme SET visible_rep = " + Te2String(((TeTheme*)theme_->getTheme())->visibleRep());
	s += " WHERE theme_id = " + Te2String(((TeTheme*)theme_->getTheme())->id());
	db_->execute(s);

	theme_->textTable(table);

	s = "UPDATE te_theme_application SET text_table = '" + table + "'";
	s += " WHERE theme_id = " + Te2String(((TeTheme*)theme_->getTheme())->id());
	db_->execute(s);

	accept();
}

void CreateTextRepresentation::reject()
{
	QDialog::reject();
}


void CreateTextRepresentation::HelpPushButton_clicked()
{

}



void CreateTextRepresentation::show()
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
