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
#include <qapplication.h>
#include <TeQtCanvas.h>
#include <qstring.h>
#include <qlineedit.h>
#include <help.h>


void DisplaySize::init()
{
	help_ = 0;
	mainWindow_= (TerraViewBase*)qApp->mainWidget();
	w_ = mainWindow_->cvContentsW_;
	h_ = mainWindow_->cvContentsH_;

	int i;
	for (i = 100; i <= 1500; i+=100)
	{
		widthComboBox->insertItem(Te2String(i).c_str());
		heightComboBox->insertItem(Te2String(i).c_str());
	}

	if(w_ == 0 && h_ == 0)
		defaultCheckBox_toggled(true);
	else
		defaultCheckBox_toggled(false);

	okPushButton->setEnabled(false);
}

void DisplaySize::defaultCheckBox_toggled( bool b)
{
	defaultCheckBox->setChecked(b);
	if(b == true)
	{
		widthTextLabel->setEnabled(false);
		widthComboBox->setEnabled(false);
		heightTextLabel->setEnabled(false);
		heightComboBox->setEnabled(false);
	}
	else
	{
		widthTextLabel->setEnabled(true);
		widthComboBox->setEnabled(true);
		heightTextLabel->setEnabled(true);
		heightComboBox->setEnabled(true);
	}

	int i, w, h;
	if(b || (mainWindow_->cvContentsW_ == 0 && mainWindow_->cvContentsH_ == 0))
	{
		w = mainWindow_->getCanvas()->width();
		h = mainWindow_->getCanvas()->height();
	}
	else
	{
		w = mainWindow_->cvContentsW_;
		h = mainWindow_->cvContentsH_;
	}

	for (i = 0; i < widthComboBox->count(); ++i)
	{
		if(w == widthComboBox->text(i).toInt())
			break;
	}

	if(i >= widthComboBox->count())
		widthComboBox->insertItem(Te2String(w).c_str());
	widthComboBox->setCurrentItem(i);

	for (i = 0; i < heightComboBox->count(); ++i)
	{
		if(h == heightComboBox->text(i).toInt())
			break;
	}

	if(i >= heightComboBox->count())
		heightComboBox->insertItem(Te2String(h).c_str());
	heightComboBox->setCurrentItem(i);
}


void DisplaySize::okPushButton_clicked()
{
	accept();
}


void DisplaySize::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("displaySize.htm");
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


void DisplaySize::widthComboBox_textChanged( const QString & )
{
	okPushButton->setEnabled(true);
}


void DisplaySize::heightComboBox_textChanged( const QString & )
{
	okPushButton->setEnabled(true);
}
