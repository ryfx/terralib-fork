/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <qapplication.h>
#include <terraViewBase.h>
#include <display.h>
#include "TeRasterTransform.h"
#include <TeAppTheme.h>
#include <help.h>



void RasterTransparency::init()
{
	help_ = 0;
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	displayWindow_ = mainWindow_->getDisplayWindow();
	transpSlider->setMinValue(0);
	transpSlider->setMaxValue(100);
	TeTheme* curTheme = (TeTheme*)mainWindow_->currentTheme()->getTheme();
	TeRasterTransform* vis = curTheme->rasterVisual();
	if (!vis)
	{
		curTheme->createRasterVisual(curTheme->layer()->raster());
		vis = curTheme->rasterVisual();
	}
	unsigned int v = vis->getTransparency();
	v = 255 - v;
	v = (unsigned int)((v * 0.392)+.5);
	transpSlider->setValue(static_cast<int>(v));
	transpSlider_sliderMoved( v );
}


void RasterTransparency::resetPushButton_clicked()
{
	transpSlider->setValue(0);
	
	TeTheme* curTheme = (TeTheme*)mainWindow_->currentTheme()->getTheme();
	TeRasterTransform* vis = curTheme->rasterVisual();
	if (!vis)
	{
		curTheme->createRasterVisual(curTheme->layer()->raster());
		vis = curTheme->rasterVisual();
	}
	vis->setTransparency(255);
	displayWindow_->plotData();
}

void RasterTransparency::helpPushButton_clicked()
{
   if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("raster_transparency.htm");
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


void RasterTransparency::transpSlider_sliderReleased()
{
	TeTheme* curTheme = (TeTheme*)mainWindow_->currentTheme()->getTheme();
	TeRasterTransform* vis = curTheme->rasterVisual();
	if (!vis)
	{
		curTheme->createRasterVisual(curTheme->layer()->raster());
		vis = curTheme->rasterVisual();
	}
	int v = transpSlider->value();
	v = (int)(v * 2.55);
	vis->setTransparency(static_cast<unsigned int>(255-v));
	displayWindow_->plotData();
}


void RasterTransparency::transpSlider_sliderMoved( int v )
{
	sliderValueTextLabel->setText(QString("%1").arg(v)+"%");
}



void RasterTransparency::RasterTransparency_destroyed( QObject * )
{

}
