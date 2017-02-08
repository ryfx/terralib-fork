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

#include "TeException.h"
#include "TeProgress.h"
#include "TeDatabase.h"
#include <TeQtLayerItem.h>
#include <qlistbox.h>
#include <vector>
#include <time.h>


void ImportRasterSimple::init()
{
	help_ = 0;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	db_ = mainWindow_->currentDatabase();
	projection_ = 0;
	raster_ = 0;
	elapsedTime_ = 0.0;
	openRFileLineEdit->setText(mainWindow_->lastOpenDir_ + "/...");
	openRFileLineEdit->setEnabled(false);
}

void ImportRasterSimple::openRFileButton_clicked()
{
	QString qfileName = QFileDialog::getOpenFileName(mainWindow_->lastOpenDir_, 
		"GeoTiff Files(*.tif *.TIF);;JPG Files(*.jpg *.JPG);;SPR Grid Files(*.spr *.SPR);;ESRI ASCII Grid Files(*.asc *.ASC *.txt *.TXT *.grd *.GRD )",
	  this, 0, tr( "Open input file"));
	
	if (!qfileName.isEmpty())
		openRFileLineEdit->setText(qfileName);
	else
		return;
	mainWindow_->lastOpenDir_ = TeGetPath(qfileName.latin1()).c_str();

	if (raster_)
	{
		raster_->clear();
		delete raster_;
		raster_= 0;
	}

	if ( projection_)
		delete projection_;
	projection_ = 0;

	char* filename = strdup(qfileName);
	string filePrefix = TeGetName(filename);
	string ext = TeGetExtension(filename);
	if (ext == "SID" || ext == "sid")
		keepFileCheckBox->toggle();

	string layerName = TeGetBaseName(filename);
	layerNameLineEdit->setText(layerName.c_str());

	TeRasterParams par;
	raster_ = new TeRaster(filename);
	par = raster_->params();
	if (raster_->projection())
		projection_  = TeProjectionFactory::make(raster_->projection()->params());
	else
		projection_ = new TeNoProjection();
	string projName = projection_->name() + " / " + (projection_->datum()).name();
	projectionRLineEdit->setText(projName.c_str());

	char fmt[100];
	if (projection_->name() == "NoProjection" ||
		projection_->name() == "LatLong")
		strcpy(fmt,"%.8f");
	else
		strcpy(fmt,"%.3f");

	QString myValue;
	myValue.sprintf(fmt,par.resx_);
	resXLineEdit->setText(myValue);
	resXLineEdit->setCursorPosition(0);

	myValue.sprintf(fmt,par.resy_);
	resYLineEdit->setText(myValue);
	resYLineEdit->setCursorPosition(0);

	nLinesLineEdit->setText(QString("%1").arg(par.nlines_));
	nLinesLineEdit->setCursorPosition(0);

	nColsLineEdit->setText(QString("%1").arg(par.ncols_));
	nColsLineEdit->setCursorPosition(0);
	if (par.nBands() > 0)
		nBandsLineEdit->setText(QString("%1").arg(par.nBands()));

	if (par.box().isValid())
	{
		myValue.sprintf(fmt,par.box().x1_);
		x0LineEdit->setText(myValue);
		x0LineEdit->setCursorPosition(0);

		myValue.sprintf(fmt,par.box().y1_);
		y0LineEdit->setText(myValue);
		y0LineEdit->setCursorPosition(0);

		myValue.sprintf(fmt,par.box().x2_);
		x1LineEdit->setText(myValue);
		x1LineEdit->setCursorPosition(0);

		myValue.sprintf(fmt,par.box().y2_);
		y1LineEdit->setText(myValue);	
		y1LineEdit->setCursorPosition(0);
	}

	if (par.useDummy_)	
	{
		myValue.sprintf(fmt,par.dummy_[0]);
		inputDummyLineEdit->setText(myValue);
		inputDummyLineEdit->setCursorPosition(0);
	}
	else
		inputDummyLineEdit->clear();
	elapsedTime_ = 0.0;
}

void ImportRasterSimple::importRButton_clicked()
{
	QString f = openRFileLineEdit->text();
	if (f.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select a file!"));
		return;
	}
	char* filename = strdup(f);
	string ext = TeGetExtension(filename);
	if ((ext == "SID" || ext == "sid") &&
		!keepFileCheckBox->isChecked())
	{
		QString mess;
		mess = tr("MrSID files can be used only when maintained in file.");
		QMessageBox::critical(this, tr("Error"),mess);
		keepFileCheckBox->setDown(true);
		return;
	}

	string layerName = layerNameLineEdit->text().latin1();
	if(layerName.empty())
	{
		QMessageBox::warning(this,tr("Warning"),
		tr("Enter output layer name and try again!"));
		return;
	}
	string errorMessage;
	bool changed;
	string newName = TeCheckName(layerName, changed, errorMessage); 
	if(changed)
	{
		QString mess = tr("The output layer name is invalid: ") + errorMessage.c_str();
		mess += "\n" + tr("Change current name and try again.");
		QMessageBox::warning(this, tr("Warning"), mess);
		layerNameLineEdit->setText(newName.c_str());
		return;
	}
	layerName = newName;
	if (db_->layerExist(layerName))
	{
		QMessageBox::warning(this,tr("Warning"),
		tr("Output layer name already exist, replace and try again!"));
		return;
	}
	TeWaitCursor wait;
	raster_->init();
	wait.resetWaitCursor();
	if (!raster_->status())			// check if raster is ready to be read
	{
		QString mess;
		mess = tr("Input raster cannot be read!");
		QMessageBox::critical(this, tr("Error"),mess);
		return;
	}
	importedLayer_ = new TeLayer(layerName,db_,projection_);
	if (!importedLayer_->id())
	{
		importedLayer_ = 0;
	    return;
	}
	bool res = false;
	// deal with the case where the raster will be kept outside the database in a file
	if (keepFileCheckBox->isChecked())
	{
		raster_->params().tiling_type_ = TeRasterParams::TeNoExpansible;
		if (raster_->params().dataType_[0] == TeUNSIGNEDCHAR)
		{
			raster_->params().vmax_.assign(raster_->params().nBands(), 255);
			raster_->params().vmin_.assign(raster_->params().nBands(), 0);
		}
		else 
		{
			// when the type of the data is not unsigned char and min and max values where not read
			// before, visualization might not be possible. Let's read it once during the importing
			if (raster_->params().vmax_[0] == -TeMAXFLOAT && raster_->params().vmin_[0] == TeMAXFLOAT)
			{
				if(TeProgress::instance())
				{
					QString caption = tr("Importing");
					TeProgress::instance()->setCaption(caption.latin1());
					QString msg = tr("Reading data information. Please, wait!");
					TeProgress::instance()->setMessage(msg.latin1());
					TeProgress::instance()->setTotalSteps(raster_->params().nlines_);
				}
				int l, c, b;
				double val;
				int nb = raster_->params().nBands();
				for (l=0; l<raster_->params().nlines_;++l)
				{
					for (c=0; c<raster_->params().ncols_;++c)
					{
						for (b=0; b<nb;++b)
						{
							if (raster_->getElement(c,l,val,b))
							{
								if (val>raster_->params().vmax_[b])
									raster_->params().vmax_[b] = val;
								if (val<raster_->params().vmin_[b])
									raster_->params().vmin_[b] = val;
							}
						}
					}
					TeProgress::instance()->setProgress(l);
				}
				TeProgress::instance()->reset();
			}
		}
		res = importedLayer_->addRasterFileGeometry(raster_);
	}
	else
	{
		int bh = nLinesLineEdit->text().toInt();
		int bw = nColsLineEdit->text().toInt();
		// if raster is up to 512x512 store it in one block
		if (bh > 512 && bw > 512)
		{
			if (bw <= 1024)
				bh = 1;
			else
			{
				bw = (bw%512 <= bw%256)? 512 : 256;
				bh = (bh%512 <= bh%256)? 512 : 256;
			}
		}
		double outDummy = pow(2.,raster_->params().nbitsperPixel_[0])-1;
		bool outHasDummy = false;
		if (!inputDummyLineEdit->text().isEmpty())
		{
			outDummy = inputDummyLineEdit->text().toDouble();
			outHasDummy = true;
		}

		if(TeProgress::instance())
		{
			QString caption = tr("Importing");
			TeProgress::instance()->setCaption(caption.latin1());
			QString msg = tr("Importing raster. Please, wait!");
			TeProgress::instance()->setMessage(msg.latin1());
		}

		clock_t	ti;
		ti = clock();
		// try to import raster
		res = TeImportRaster(importedLayer_,raster_, bw, bh, TeRasterParams::TeNoCompression,"O1",outDummy,outHasDummy,TeRasterParams::TeNoExpansible);
		elapsedTime_ = double((clock()-ti)/CLOCKS_PER_SEC);
		if (TeProgress::instance())
			TeProgress::instance()->reset();
	}

	if (res)
	{
		projectionRLineEdit->setText("");
		raster_->clear();
		delete raster_;
		raster_ = 0;
		delete projection_;
		projection_ = 0;
		TeQtDatabasesListView *databasesListView = mainWindow_->getDatabasesListView();
		TeQtDatabaseItem *databaseItem = databasesListView->currentDatabaseItem();
			TeQtLayerItem *layerItem = new TeQtLayerItem((QListViewItem*)databaseItem, importedLayer_->name().c_str(), importedLayer_);
		layerItem->setEnabled(true);
		if (databasesListView->isOpen(layerItem->parent()) == false)
			databasesListView->setOpen(layerItem->parent(),true);
		mainWindow_->checkWidgetEnabling();
		accept();
	}
	else	// importing failed
	{
	    db_->deleteLayer(importedLayer_->id());	// remove bad layer
		QMessageBox::critical(this, tr("Error"), tr("Import failed!"));
		raster_->clear();
		delete raster_;
		raster_ = 0;
		delete projection_;
		projection_ = 0;
		reject();
	}
}


TeLayer* ImportRasterSimple::getLayer()
{
    return importedLayer_;
}



void ImportRasterSimple::helpButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("importRasterSimple.htm");
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


double ImportRasterSimple::getElapsedTime()
{
	return elapsedTime_;
}
