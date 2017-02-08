/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <qapplication.h>
#include <qmessagebox.h>

#include "TeAppTheme.h"
#include "terraViewBase.h"
#include "TeViewNode.h"
#include "TeRasterTransform.h"
#include "TeRasterParams.h"
#include "TeQtThemeItem.h"
#include <vector>

void RasterColorCompositionWindow::okPushButton_clicked()
{
	if (!theme_ || !raster_)
		reject();

	((TeTheme*)theme_->getTheme())->removeRasterVisual();

	TeRasterTransform* vis = new TeRasterTransform();

	short br = R_comboBox->currentItem();
	short bg = G_comboBox->currentItem();
	short bb = B_comboBox->currentItem();
	short bm = M_comboBox->currentItem();

	if (br==0 && bg == 0 && bb == 0 && bm == 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("You must select a band to the one of the channels or a color composition !"));
		return;
	}

	if (br != 0 && bg != 0 && bb != 0)
	{
		if (br == 1 && bg == 2  && bb == 3)
		{
			vis->setTransfFunction(&TeRasterTransform::Band2Band);
		}
		else
		{
			map<TeRasterTransform::TeRGBChannels,short> bmap;
			bmap[TeRasterTransform::TeREDCHANNEL] = br-1;
			bmap[TeRasterTransform::TeGREENCHANNEL] = bg-1;
			bmap[TeRasterTransform::TeBLUECHANNEL] = bb-1;
			vis->setRGBmap(bmap);
			vis->setTransfFunction(&TeRasterTransform::ExtractRGB);
		}
	}
	else
	{
		vis->clearRGBMap();
		if (bm != 0)
		{
			vis->setSrcBand(bm-1);
			vis->setDestBand(0);
			vis->setTransfFunction(&TeRasterTransform::Mono2ThreeBand);
		}
		else
		{
			if (bg == 0 && bb == 0 && br != 0)
			{
				vis->setSrcBand(br-1);
				vis->setDestBand(0);
				vis->setBChannelMapping(br-1,TeRasterTransform::TeREDCHANNEL);
				vis->setTransfFunction(&TeRasterTransform::ExtractBand);
			}
			else if (br == 0 && bb == 0 && bg != 0)
			{
				vis->setSrcBand(bg-1);
				vis->setDestBand(1);
				vis->setBChannelMapping(bg-1,TeRasterTransform::TeGREENCHANNEL);
				vis->setTransfFunction(&TeRasterTransform::ExtractBand);
			}
			else if (br == 0 && bg == 0 && bb != 0)
			{
				vis->setSrcBand(bb-1);
				vis->setDestBand(2);
				vis->setBChannelMapping(bb-1,TeRasterTransform::TeBLUECHANNEL);
				vis->setTransfFunction(&TeRasterTransform::ExtractBand);
			}
			else
			{
				map<TeRasterTransform::TeRGBChannels,short> bmap;
				if (br>0)
					bmap[TeRasterTransform::TeREDCHANNEL] = br-1;
				else
					bmap[TeRasterTransform::TeREDCHANNEL] = -1;

				if (bg>0)
					bmap[TeRasterTransform::TeGREENCHANNEL] = bg-1;
				else
					bmap[TeRasterTransform::TeGREENCHANNEL] = -1;

				if (bb>0)
					bmap[TeRasterTransform::TeBLUECHANNEL] = bb-1;
				else
					bmap[TeRasterTransform::TeBLUECHANNEL] = -1;
				
				vis->setRGBmap(bmap);
				vis->setTransfFunction(&TeRasterTransform::ExtractBands);
			}
		}
	}
	((TeTheme*)theme_->getTheme())->rasterVisual(vis);
	accept();
	return;
}

void RasterColorCompositionWindow::init(TeQtThemeItem *themeItem)
{
	help_ = 0;
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	theme_ = (TeAppTheme*)themeItem->getAppTheme();
	raster_ =  ((TeTheme*)theme_->getTheme())->layer()->raster();
	if (!theme_)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select a theme!"));
		return;
	}
	R_comboBox->clear();
	G_comboBox->clear();
	B_comboBox->clear();
	M_comboBox->clear();

	R_comboBox->insertItem(tr("None"));
	G_comboBox->insertItem(tr("None"));
	B_comboBox->insertItem(tr("None"));
	M_comboBox->insertItem(tr("None"));

	QString prefix = tr("Band ");
	unsigned int nb = raster_->params().nBands();
	for (unsigned int i = 0; i<nb; ++i)
	{
		QString band = prefix + QString("%1").arg(i);
		R_comboBox->insertItem(band);
		G_comboBox->insertItem(band);
		B_comboBox->insertItem(band);
		M_comboBox->insertItem(band);
	}

	TeRasterTransform* vis = ((TeTheme*)theme_->getTheme())->rasterVisual();
	if (vis)
	{
		if (vis->getTransfFunction() == TeRasterTransform::TeBand2Band)
		{
			M_comboBox->setCurrentItem(0);	

			R_comboBox->setCurrentItem(1);	
			G_comboBox->setCurrentItem(2);	
			B_comboBox->setCurrentItem(3);
		}
		else if (vis->getTransfFunction() == TeRasterTransform::TeExtractBand)
		{
			short db = vis->getDestBand();
			if (db == TeRasterTransform::TeREDCHANNEL)
			{
				M_comboBox->setCurrentItem(0);	
				R_comboBox->setCurrentItem(vis->getSrcBand()+1);	
				G_comboBox->setCurrentItem(0);	
				B_comboBox->setCurrentItem(0);
			}
			else if (db == TeRasterTransform::TeGREENCHANNEL)
			{
				M_comboBox->setCurrentItem(0);	
				R_comboBox->setCurrentItem(0);	
				G_comboBox->setCurrentItem(vis->getSrcBand()+1);	
				B_comboBox->setCurrentItem(0);
			}
			else if (db == TeRasterTransform::TeBLUECHANNEL)
			{
				M_comboBox->setCurrentItem(0);	
				R_comboBox->setCurrentItem(0);	
				G_comboBox->setCurrentItem(0);	
				B_comboBox->setCurrentItem(vis->getSrcBand()+1);
			}
		}

		else if (vis->getTransfFunction() == TeRasterTransform::TeMono2Three)
		{
			M_comboBox->setCurrentItem(vis->getSrcBand()+1);	
			R_comboBox->setCurrentItem(0);	
			G_comboBox->setCurrentItem(0);	
			B_comboBox->setCurrentItem(0);
		}

		else if (vis->getTransfFunction() == TeRasterTransform::TeExtractRGB)
		{
			M_comboBox->setCurrentItem(0);	

			map<TeRasterTransform::TeRGBChannels,short>& RGBmap = vis->getRGBMap();
			R_comboBox->setCurrentItem(RGBmap[TeRasterTransform::TeREDCHANNEL]+1);	
			G_comboBox->setCurrentItem(RGBmap[TeRasterTransform::TeGREENCHANNEL]+1);	
			B_comboBox->setCurrentItem(RGBmap[TeRasterTransform::TeBLUECHANNEL]+1);
		}
		else if (vis->getTransfFunction() == TeRasterTransform::TeThreeBand2RGB)
		{
			M_comboBox->setCurrentItem(0);	

			R_comboBox->setCurrentItem(1);	
			G_comboBox->setCurrentItem(2);	
			B_comboBox->setCurrentItem(3);
		}
		else if (vis->getTransfFunction() == TeRasterTransform::TeExtractBands)
		{
			M_comboBox->setCurrentItem(0);	
			
			map<TeRasterTransform::TeRGBChannels,short>& RGBmap = vis->getRGBMap();
			if (RGBmap[TeRasterTransform::TeREDCHANNEL] != -1)
				R_comboBox->setCurrentItem(RGBmap[TeRasterTransform::TeREDCHANNEL]+1);
			if (RGBmap[TeRasterTransform::TeGREENCHANNEL] != -1)
				G_comboBox->setCurrentItem(RGBmap[TeRasterTransform::TeGREENCHANNEL]+1);	
			if (RGBmap[TeRasterTransform::TeBLUECHANNEL] != -1)
				B_comboBox->setCurrentItem(RGBmap[TeRasterTransform::TeBLUECHANNEL]+1);
		}
	}
}

void RasterColorCompositionWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("rasterColorCompositionWindow.htm");
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

void RasterColorCompositionWindow::M_comboBox_activated( int i )
{
	if (i > 0)
	{
		R_comboBox->setCurrentItem(0);	
		G_comboBox->setCurrentItem(0);	
		B_comboBox->setCurrentItem(0);
	}
}


void RasterColorCompositionWindow::R_comboBox_activated( int i )
{
	if (i > 0)
		M_comboBox->setCurrentItem(0);	
}


void RasterColorCompositionWindow::G_comboBox_activated( int i )
{
	if (i > 0)
		M_comboBox->setCurrentItem(0);	
}


void RasterColorCompositionWindow::B_comboBox_activated( int i )
{
	if (i > 0)
		M_comboBox->setCurrentItem(0);	
}
