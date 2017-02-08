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

#include <qapplication.h>
#include <qpixmap.h>
#include <terraViewBase.h>
#include <display.h>
#include "TeRasterTransform.h"

#include <TeAppTheme.h>


void ContrastWindow::init()
{
	help_ = 0;
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	displayWindow_ = mainWindow_->getDisplayWindow();
	TeTheme* theme = (TeTheme*)mainWindow_->currentTheme()->getTheme();
	TeRasterTransform* vis = theme->rasterVisual();
	if (!vis)
	{
		theme->createRasterVisual(theme->layer()->raster());
		vis = theme->rasterVisual();
	}
	else
	{
		monoSlider->setValue(int((vis->getContrastM()-1.0)*100.));
		redSlider->setValue(int((vis->getContrastR()-1.0)*100.));
		greenSlider->setValue(int((vis->getContrastG()-1.0)*100.));
		blueSlider->setValue(int((vis->getContrastB()-1.0)*100.));
	}

	TeRasterTransform::TeRasterTransfFunctions tf = vis->getTransfFunction();
	redSlider->setEnabled(false);
	redTextLabel->setEnabled(false);
	redValueTextLabel->setEnabled(false);
	greenSlider->setEnabled(false);
	greenTextLabel->setEnabled(false);
	greenValueTextLabel->setEnabled(false);
	blueSlider->setEnabled(false);
	blueTextLabel->setEnabled(false);
	blueValueTextLabel->setEnabled(false);
	monoSlider->setEnabled(false);
	monoTextLabel->setEnabled(false);
	monoValueTextLabel->setEnabled(false);
	if (tf == TeRasterTransform::TeMono2Three)
	{
		monoSlider->setEnabled(true);
		monoTextLabel->setEnabled(true);
		monoValueTextLabel->setEnabled(true);

		okPushButton->setEnabled(false);
	}
	else if (tf == TeRasterTransform::TeExtractBand)
	{
		if (vis->getDestBand() == 0)
		{
			redSlider->setEnabled(true);
			redTextLabel->setEnabled(true);
			redValueTextLabel->setEnabled(true);
		}
		else if (vis->getDestBand() == 1)
		{
			greenSlider->setEnabled(true);
			greenTextLabel->setEnabled(true);
			greenValueTextLabel->setEnabled(true);
		}
		else if (vis->getDestBand() == 2)
		{
			blueSlider->setEnabled(true);
			blueTextLabel->setEnabled(true);
			blueValueTextLabel->setEnabled(true);
		}
	}
	else if (tf == TeRasterTransform::TeBand2Band || 
				tf == TeRasterTransform::TeExtractRGB ||
				tf == TeRasterTransform::TeThreeBand2RGB)
	{
		redSlider->setEnabled(true);
		redTextLabel->setEnabled(true);
		redValueTextLabel->setEnabled(true);
		greenSlider->setEnabled(true);
		greenTextLabel->setEnabled(true);
		greenValueTextLabel->setEnabled(true);
		blueSlider->setEnabled(true);
		blueTextLabel->setEnabled(true);
		blueValueTextLabel->setEnabled(true);
	}
	else if (tf == TeRasterTransform::TeExtractBands)
	{
		map<TeRasterTransform::TeRGBChannels,short>& RGBmap = vis->getRGBMap();
		map<TeRasterTransform::TeRGBChannels,short>::iterator it = RGBmap.begin();
		while (it != RGBmap.end())
		{
			if (it->first == TeRasterTransform::TeREDCHANNEL)
			{
				redSlider->setEnabled(true);
				redTextLabel->setEnabled(true);
				redValueTextLabel->setEnabled(true);
			}
			else if (it->first == TeRasterTransform::TeGREENCHANNEL)
			{
				greenSlider->setEnabled(true);
				greenTextLabel->setEnabled(true);
				greenValueTextLabel->setEnabled(true);
			}
			else
			{
				blueSlider->setEnabled(true);
				blueTextLabel->setEnabled(true);
				blueValueTextLabel->setEnabled(true);
			}
			++it;
		}
	}	

	QString s;

	s = Te2String(monoSlider->value()).c_str();
	s += "%";
	monoValueTextLabel->setText(s);

	s = Te2String(redSlider->value()).c_str();
	s += "%";
	redValueTextLabel->setText(s);

	s = Te2String(greenSlider->value()).c_str();
	s += "%";
	greenValueTextLabel->setText(s);

	s = Te2String(blueSlider->value()).c_str();
	s += "%";
	blueValueTextLabel->setText(s);	
}


void ContrastWindow::monoSlider_sliderMoved(int value)
{
	QString s;

	s = Te2String(value).c_str();
	s += "%";
	monoValueTextLabel->setText(s);
}


void ContrastWindow::monoSlider_sliderReleased()
{
	if (((TeTheme*)mainWindow_->currentTheme()->getTheme())->rasterVisual())
		((TeTheme*)mainWindow_->currentTheme()->getTheme())->rasterVisual()->setContrastM(1. + (monoSlider->value())/100.);
	displayWindow_->plotData();
}


void ContrastWindow::redSlider_sliderMoved(int value)
{
	QString s;

	s = Te2String(value).c_str();
	s += "%";
	redValueTextLabel->setText(s);
}


void ContrastWindow::redSlider_sliderReleased()
{
	if (((TeTheme*)mainWindow_->currentTheme()->getTheme())->rasterVisual())
		((TeTheme*)mainWindow_->currentTheme()->getTheme())->rasterVisual()->setContrastR(1. + (redSlider->value())/100.);
	displayWindow_->plotData();
}


void ContrastWindow::greenSlider_sliderMoved(int value)
{
	QString s;

	s = Te2String(value).c_str();
	s += "%";
	greenValueTextLabel->setText(s);
}

void ContrastWindow::greenSlider_sliderReleased()
{
	if (((TeTheme*)mainWindow_->currentTheme()->getTheme())->rasterVisual())
		((TeTheme*)mainWindow_->currentTheme()->getTheme())->rasterVisual()->setContrastG(1. + (greenSlider->value())/100.);
	displayWindow_->plotData();
}


void ContrastWindow::blueSlider_sliderMoved(int value)
{
	QString s;

	s = Te2String(value).c_str();
	s += "%";
	blueValueTextLabel->setText(s);
}


void ContrastWindow::blueSlider_sliderReleased()
{
	if (((TeTheme*)mainWindow_->currentTheme()->getTheme())->rasterVisual())
		((TeTheme*)mainWindow_->currentTheme()->getTheme())->rasterVisual()->setContrastB(1. + (blueSlider->value())/100.);
	displayWindow_->plotData();
}



void ContrastWindow::okPushButton_clicked()
{
	if (((TeTheme*)mainWindow_->currentTheme()->getTheme())->rasterVisual())
		((TeTheme*)mainWindow_->currentTheme()->getTheme())->rasterVisual()->setContrastB(1. + (blueSlider->value())/100.);
	if (((TeTheme*)mainWindow_->currentTheme()->getTheme())->rasterVisual())
		((TeTheme*)mainWindow_->currentTheme()->getTheme())->rasterVisual()->setContrastR(1. + (redSlider->value())/100.);
	if (((TeTheme*)mainWindow_->currentTheme()->getTheme())->rasterVisual())
		((TeTheme*)mainWindow_->currentTheme()->getTheme())->rasterVisual()->setContrastG(1. + (greenSlider->value())/100.);
	displayWindow_->plotData();
}


void ContrastWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("contrast.htm");
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




