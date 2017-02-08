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
#include <createTextRepresentation.h>
#include <selectTextRepresentation.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <TeAppTheme.h>

void InsertTextWindow::init()
{
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	TeAppTheme* appTheme = mainWindow_->currentTheme();
	TeTheme* theme = (TeTheme*)appTheme->getTheme();
	if(theme->type() != TeTHEME)
		return;
	cancel_ = false;

	if((theme->layer()->geomRep() & TeTEXT) == 0)
	{
		int response = QMessageBox::question(mainWindow_, tr("Create Text Representation"),
				tr("To insert text you need to create a text representation. Continue?"),
				tr("Yes"), tr("No"));

		if (response != 0)
		{
			cancel_ = true;
			return;
		}

		CreateTextRepresentation w(mainWindow_, tr("Create text representation"), true);
		if (w.exec() == QDialog::Rejected)
		{
			cancel_ = true;
			return;
		}
	}
	else if(appTheme->textTable().empty())
	{
		int response = QMessageBox::question(mainWindow_, tr("Select Text Representation"),
				tr("To insert text you need to select a text representation. Continue?"),
				tr("Yes"), tr("No"));

		if (response != 0)
		{
			cancel_ = true;
			return;
		}

		SelectTextRepresentation w(mainWindow_, "select text representation", true);
		w.exec();

		if(appTheme->textTable().empty())
		{
			cancel_ = true;
			return;
		}
	}

	if((theme->visibleRep() & TeTEXT) == 0)
		theme->visibleRep(theme->visibleRep() | TeTEXT);
}

void InsertTextWindow::insertLineEdit_returnPressed()
{
	cancel_ = false;
	hide();
}


void InsertTextWindow::closeEvent( QCloseEvent * )
{
	cancel_ = true;
	hide();
}


void InsertTextWindow::show()
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
