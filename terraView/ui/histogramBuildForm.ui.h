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
#include <graphic.h>
#include <qapplication.h>
#include <TePDIUtils.hpp>
#include <TeRasterTransform.h>
#include <help.h>

void HistogramBuildForm::init()
{
    help_ = 0;
	//  histwin_.reset( new HistogramPlotForm( this, "Histogram Window", false, 0 ), true );
   //  histwin_->setCaption( "TerraView - Raster Histogram" );
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
  
  in_raster_ = 0;
}




void HistogramBuildForm::setRaster( TeRaster * in_raster )
{
  if( in_raster == 0 )  {
    PBgenerate->setEnabled( false );
    in_raster_ = 0;
    return;
  } else {
    in_raster_ = in_raster;
    TeRasterParams& pars = in_raster->params();
    SBRasterBand->setMaxValue( (int) pars.nBands() - 1 );
    bandChanged( 0 );
    PBgenerate->setEnabled( true );
  }
}


void HistogramBuildForm::PBgenerateClick()
{
//  histwin_->hide();

  hist_.clear();
  
  /*
  TePDITypes::TePDIRasterPtrType RAMRaster;
  
  TeRasterParams RAMRaster_params = in_raster_->params();
  RAMRaster_params.nBands( 1 );
  if( in_raster_->params().useDummy_ ) {
    RAMRaster_params.setDummy( 
      in_raster_->params().dummy_[ SBRasterBand->value() ] );
  }
  
  if( ! TePDIUtils::TeAllocRAMRaster( RAMRaster, RAMRaster_params, false ) ) {
    
    QMessageBox::critical(this, tr("Error"),
      tr("Insuficient memory to load image data from database."));
      
    return;
  }
  
  TeRasterTransform transf( in_raster_, RAMRaster.NakedPointer() );
  transf.setTransfFunction( TeRasterTransform::TeExtractBand );
  transf.setSrcBand( SBRasterBand->value() );
  
  if( ! in_raster_->fillRaster( RAMRaster.NakedPointer(), &transf, true ) ) {
    QMessageBox::critical(this, tr("Error"),
      tr("Unable to load image data from database."));
      
    return;     
  }
  */
  
  unsigned int levels = ( CBIntHistAutoGuess->isOn() ? 0 :
                        (unsigned int) SBIntHistLevels->value() );

  TeSharedPtr< TeRaster > in_raster_sptr( in_raster_, true );
  
  if( ! hist_.reset( in_raster_sptr, SBRasterBand->value(), 
    levels, TeBoxPixelIn, 
    TeSharedPtr< TePolygonSet>() ) ) {
    
    QMessageBox::critical(this, tr("Error"),
      tr("Unable to create histogram. Please check all parameters"));
      
    return;
    
  }
  
  if( CBIntHistDiscritize->isOn() ) {
    if( ! hist_.Discretize() ) {
    
      QMessageBox::critical(this, tr("Error"),
        tr("Unable to discretize histogram. Please check all parameters" ));
        
      return;
    }
  }

	mainWindow_->getGraphicWindow();
	mainWindow_->getGraphicWindow()->show();
	mainWindow_->getGraphicWindow()->plotRasterHistogram(hist_);
  
//  histwin_->setHistogram( hist_ );
    
//  histwin_->show();
}


TePDIHistogram HistogramBuildForm::getHistogram()
{
  return hist_;
}



void HistogramBuildForm::helpPushButton_clicked()
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



void HistogramBuildForm::bandChanged( int )
{
  TeRasterParams& pars = in_raster_->params();
    
  if( ( pars.vmax_[ 0 ] - pars.vmin_[ 0 ] ) > 1 ) {
    CBIntHistAutoGuess->setEnabled( true );
    CBIntHistAutoGuess->setChecked( true );
    
    CBIntHistDiscritize->setEnabled( true );
    CBIntHistDiscritize->setChecked( true );
    
    SBIntHistLevels->setEnabled( false );
  } else {
    CBIntHistAutoGuess->setEnabled( false );
    CBIntHistAutoGuess->setChecked( false );
    
    CBIntHistDiscritize->setEnabled( false );
    CBIntHistDiscritize->setChecked( false );
    
    SBIntHistLevels->setEnabled( true );
  }
}


void HistogramBuildForm::destroy()
{
//  histwin_.reset();
}
