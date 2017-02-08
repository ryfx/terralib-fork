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
#include <display.h>

void SelectTextRepresentation::init()
{
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	theme_ = mainWindow_->currentTheme();
	if(theme_->getTheme()->type() != TeTHEME)
		return;

	TeTheme* curTheme = (TeTheme*)theme_->getTheme();
	layer_ = curTheme->layer();
	db_ = layer_->database();

	TeRepresPointerVector repv;
	layer_->getRepresentation(TeTEXT, repv);
	tableComboBox->clear();
	int i;
	for(i=0; i<int(repv.size()); i++)
		tableComboBox->insertItem(repv[i]->tableName_.c_str());

	tableComboBox->setCurrentItem(0);
}

void SelectTextRepresentation::OKPushButton_clicked()
{
	TeTheme* curTheme = (TeTheme*)theme_->getTheme();
	if(tableComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Select a representation table!"));
		return;
	}
	string table = tableComboBox->currentText().latin1();

	theme_->textTable(table);
	string s = "UPDATE te_theme_application SET text_table = '" + table + "'";
	s += " WHERE theme_id = " + Te2String(curTheme->id());
	db_->execute(s);

	long vrep = curTheme->visibleRep();
	vrep |= TeTEXT;
	curTheme->visibleRep(vrep);
	mainWindow_->getDisplayWindow()->plotData();

	s = "UPDATE te_theme SET visible_rep = " + Te2String(curTheme->visibleRep());
	s += " WHERE theme_id = " + Te2String(curTheme->id());
	db_->execute(s);

	accept();
}


void SelectTextRepresentation::reject()
{
	QDialog::reject();
}


void SelectTextRepresentation::HelpPushButton_clicked()
{

}



void SelectTextRepresentation::show()
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
