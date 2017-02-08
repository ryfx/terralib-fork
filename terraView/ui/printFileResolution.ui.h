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
#include <qpaintdevicemetrics.h>
#include <TeQtCanvas.h>

void PrintFileResolution::init()
{
	mainWindow_= (TerraViewBase*)qApp->mainWidget();
	int i;
	for (i = 0; i < 12; i++)
		resolutionComboBox->insertItem(Te2String(50+i*50).c_str());

	displayResolutionCheckBox_toggled(mainWindow_->printWithDisplayResolution_);
}

void PrintFileResolution::okPushButton_clicked()
{
	accept();
}

void PrintFileResolution::helpPushButton_clicked()
{

}

void PrintFileResolution::displayResolutionCheckBox_toggled( bool b)
{
	displayResolutionCheckBox->setChecked(b);

	if(b)
	{
		resolutionTextLabel->setEnabled(false);
		resolutionComboBox->setEnabled(false);
		dpiTextLabel->setEnabled(false);
	}
	else
	{
		resolutionTextLabel->setEnabled(true);
		resolutionComboBox->setEnabled(true);
		dpiTextLabel->setEnabled(true);
	}

	int i, dpi;
	if(b)
	{
		TeQtCanvas* canvas = mainWindow_->getCanvas();
		QPaintDeviceMetrics pdm( canvas->viewport() );
		int w = pdm.width();
		int wmm = pdm.widthMM ();
		dpi = (int)((double)w / ((double)wmm / 25.4) + .5);
	}
	else
		dpi = mainWindow_->printFileResolution_;

	for (i = 0; i < resolutionComboBox->count(); ++i)
	{
		if(dpi == resolutionComboBox->text(i).toInt())
			break;
	}

	if(i >= resolutionComboBox->count())
		resolutionComboBox->insertItem(Te2String(dpi).c_str());
	resolutionComboBox->setCurrentItem(i);
}



void PrintFileResolution::resolutionComboBox_textChanged( const QString & )
{
}
