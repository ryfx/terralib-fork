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

#include "TeDatabase.h"
#include "TeAppTheme.h"
#include "TeRasterRemap.h"
#include "TeVectorRemap.h"
#include "TeProgress.h"
#include "TeRasterTransform.h"
#include "projection.h"
#include "TeWaitCursor.h"
#include <qapplication.h>
#include <qmessagebox.h>
#include <qstring.h>
#include <qfiledialog.h>
#include <qfile.h>
#include <qstatusbar.h>
#include "TeQtCanvas.h"
#include <TeQtThemeItem.h>
#include <qdom.h>
#include <qfile.h>

#include <iterator>

void SaveRaster::init( TeQtThemeItem *themeItem )
{
	help_ = 0;
	bbCheckBox->setChecked(false);
	theme_ = themeItem->getAppTheme();
	if (!theme_)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select a theme!"));
		return;
	}
	raster_ = 0;
	proj_ = 0;
	projNameLineEdit->clear();
	viewProjRadioButton->setChecked(true);
	projButtonGroup_clicked(0);
	objComboBox->clear();
	TeLayer* layer = ((TeTheme*)theme_->getTheme())->layer();
	string sql;
	if (layer->hasGeometry(TeRASTER))
		sql = "SELECT object_id FROM " + layer->tableName(TeRASTER);
	else
		sql = "SELECT object_id FROM " + layer->tableName(TeRASTERFILE);
	TeDatabasePortal* portal = db_->getPortal();
	if (portal->query(sql))
	{
		while (portal->fetchRow())
			objComboBox->insertItem(QString(portal->getData(0)));
	}
	portal->freeResult();
	delete portal;
	resXLineEdit->clear();
	resYLineEdit->clear();	
	QDir adir;
	QString outName =  lastOpenDir_ + "/" + ((TeTheme*)theme_->getTheme())->name().c_str();
	openRFileLineEdit->setText(outName);
	objComboBox_activated(0);
}

void SaveRaster::okPushButton_clicked()
{
	if (!raster_)
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Select the raster to save!"));
		return;
	}

	QString filename = openRFileLineEdit->text();
	if (filename.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Select a filename to save the raster!"));
		return;
	}

	int answer = 0;
    if (QFile::exists(filename))
	{
        answer = QMessageBox::question(this,
                        tr("Save Raster"),
                        QString( tr("Overwrite") + "\n\'%1\'?" ).
                            arg( filename ),
                        tr("&Yes"), tr("&No"), QString::null, 1, 1 );
		if ( answer == 1 ) 
		{
			openRFileLineEdit->clear();
			return;
		}
	}

	TeRasterParams parIn = raster_->params();
	TeRasterParams par;
	par.fileName_ = string(filename.latin1());
	par.mode_ = 'c';
	int outFormat = formatComboBox->currentItem();
	par.decoderIdentifier_ = "MEMMAP";
	if (outFormat == 1)
		par.decoderIdentifier_ = "TIF";
	if (outFormat == 2)
		par.decoderIdentifier_ = "JPEG";
	if (outFormat == 3)
		par.decoderIdentifier_ = "SPR";if (outFormat == 4)
		par.decoderIdentifier_ = "ASCIIGRID";

	if (!proj_)
		proj_ = TeProjectionFactory::make(raster_->projection()->params());
	par.projection(proj_);
	TeProjection *projIn = raster_->projection();	
	TeProjection *projOut = par.projection();

	TeBox newBox = raster_->params().boundingBox();
	if (bbCheckBox->isChecked())
	{
		newBox = roi_; 
		projIn = projCanvas_; 
		TeProjection* praster = raster_->params().projection();
		TeBox baux = raster_->params().boundingBox();
		if (projIn && praster && !(*projIn == *praster))
			baux = TeRemapBox(baux,praster,projOut);
		TeIntersection(newBox,baux,newBox);
	}

	// default behaviour: maintain the resolution
	double resx = raster_->params().resx_;
	double resy = raster_->params().resy_;
	saveProjection(projOut,filename);
	if (projIn && projOut && !(*projIn == *projOut))
	{
		// here we have to deal with the case where the we are moving from
		// proj to lat/long or lat/long to proj
		TeCoord2D lowerLeft = newBox.lowerLeft();
		TeCoord2D ll = projIn->PC2LL(lowerLeft);
		TeCoord2D upperRight = newBox.upperRight();
		TeCoord2D ur = projIn->PC2LL(upperRight);
		double latM = (ll.y()+ur.y())/2.;				// the mean latitude for the exporting area

		if (projOut->name() == "LatLong") // we are moving from meters to decimal degrees..
		{
			resx = (resx/(projOut->datum().radius()*cos(latM)))*TeCRD;
			resy = (resy/projOut->datum().radius())*TeCRD;
		}
		else if (projIn->name() == "LatLong") // we are moving from decimal degrees to meters...
		{
			resx = resx*TeCDR*projIn->datum().radius()*cos(latM);
			resy = resy*TeCDR*projOut->datum().radius();
		}
		newBox = TeRemapBox(newBox,projIn,projOut);
	}
	if (samplingFactorComboBox->currentItem() > 0)
	{
		resx *= (samplingFactorComboBox->currentItem()+1);
		resy *= (samplingFactorComboBox->currentItem()+1);
	}
	par.boundingBoxResolution(newBox.x1_,newBox.y1_,newBox.x2_,newBox.y2_,resx,resy);
	par.useDummy_ = parIn.useDummy_;
	par.vmax_ = parIn.vmax_;
	par.vmin_ = parIn.vmin_;
	par.date_ = parIn.date_;

	// JPEG, SPRING and ASCIIGRID formats bring whole raster in memory
	// warn user if it requires more than 128 Mb
	if (outFormat == 2 || outFormat == 3 || outFormat == 4)
	{
		long memReq = (par.nlines_*par.ncols_*par.nBands())*(par.nbitsperPixel_[0]/8);
		if (memReq > 134217728)
		{
			double nMbs = memReq/1048576;
			string aux = Te2String(nMbs,3);
			answer = QMessageBox::question(this,
                        tr("Save Raster"),
                        QString( tr("The save raster operation will require %1 MB of RAM. Do you want to continue?")).
                            arg( nMbs ),
                        tr("&Yes"), tr("&No"), QString::null, 1, 1 );
			if (answer == 1)
				return;
		}
	}
	
	// --- sliced or pallete raster  --- //
	TeRasterTransform* transf_ = new TeRasterTransform();
	TeTheme* baseTheme = (TeTheme*)theme_->getTheme();
	if (LutRadioButton->isChecked())				// export palette raster
	{
		int band = 0; 
		if (baseTheme->grouping().groupMode_ == TeRasterSlicing)	// sliced raster
		{	
			band = atoi(baseTheme->grouping().groupAttribute_.name_.c_str());
			int maxrange = 255;
			if (raster_->params().useDummy_)
			{
				par.setDummy(maxrange);
				maxrange -= 1;
			}
			transf_->setLinearTransfParameters(raster_->params().vmin_[band],
											   raster_->params().vmax_[band],0,maxrange);
			transf_->setSrcBand(band);
			transf_->generateLUT(baseTheme->legend(),maxrange+1,baseTheme->defaultLegend().visual(TePOLYGONS)->color());
			if (raster_->params().useDummy_)
			{
				transf_->lutr_.push_back(baseTheme->defaultLegend().visual(TePOLYGONS)->color().red_);
				transf_->lutg_.push_back(baseTheme->defaultLegend().visual(TePOLYGONS)->color().green_);
				transf_->lutb_.push_back(baseTheme->defaultLegend().visual(TePOLYGONS)->color().blue_);
			}
			copy(transf_->lutr_.begin(), transf_->lutr_.end(), back_inserter(par.lutr_));
			copy(transf_->lutg_.begin(), transf_->lutg_.end(), back_inserter(par.lutg_));
			copy(transf_->lutb_.begin(), transf_->lutb_.end(), back_inserter(par.lutb_));
		}
		else 
		{
			if (parIn.photometric_[0] != TeRasterParams::TePallete)
			{
				QMessageBox::warning(this, tr("Warning"),
					tr("There is no LUT associated to raster!"));
				return;
			}
			else
			{
				copy(parIn.lutr_.begin(), parIn.lutr_.end(), back_inserter(par.lutr_));
				copy(parIn.lutg_.begin(), parIn.lutg_.end(), back_inserter(par.lutg_));
				copy(parIn.lutb_.begin(), parIn.lutb_.end(), back_inserter(par.lutb_));
			}
		}

		if (outFormat == 2)		// JPEG => expand pallete to three values
		{
			par.nBands(3);
			par.setDataType(TeUNSIGNEDCHAR);
			par.setPhotometric(TeRasterParams::TeMultiBand);
			if (baseTheme->grouping().groupMode_ == TeRasterSlicing)	// sliced raster
				transf_->setTransfFunction(&TeRasterTransform::LUT2ThreeBand);
			else if (parIn.photometric_[0] == TeRasterParams::TePallete)	// Pallete raster
				transf_->setTransfFunction(&TeRasterTransform::Pallete2ThreeBand);
		}
		else				// GeoTIFF, SPRING, ASCII-GRID or RAW format save one band
		{
			par.nBands(1);
			par.setPhotometric(TeRasterParams::TePallete);
			par.setDataType(TeUNSIGNEDCHAR);
			transf_->setSrcBand(band);
			transf_->setDestBand(0);
			transf_->setTransfFunction(&TeRasterTransform::ExtractBand);
		}
	}	// --- 
	else if (MRadioButton->isChecked())	//  Mono Band
	{
		par.nBands(1);
		short bm = rLineEdit->text().toShort();
		transf_->setSrcBand(bm);
		transf_->setDestBand(0);
		par.setDummy(parIn.dummy_[bm]);
		if (outFormat == 2)
		{
			par.setDataType(TeUNSIGNEDCHAR);
			par.setPhotometric(TeRasterParams::TeMultiBand);
			if (parIn.dataType_[bm] != TeUNSIGNEDCHAR)
				transf_->setLinearTransfParameters(parIn.vmin_[bm], parIn.vmin_[bm], 0, 255);
		}
		else
		{
			par.setDataType(parIn.dataType_[bm]);
			par.setPhotometric(parIn.photometric_[bm]);
		}
		transf_->setTransfFunction(&TeRasterTransform::ExtractBand);
	}
	else						// RGB band
	{
		if (rLineEdit->text().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"),
				tr("Choose a band to go to RED channel!"));
			return;
		}

		if (gLineEdit->text().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"),
				tr("Choose a band to go to GREEN channel!"));
			return;
		}

		if (bLineEdit->text().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"),
				tr("Choose a band to go to BLUE channel!"));
			return;
		}
		
		par.nBands(3);
		if (outFormat == 2)
		{
			par.setDataType(TeUNSIGNEDCHAR);
			par.setPhotometric(TeRasterParams::TeMultiBand);
			if (parIn.dataType_[0] != TeUNSIGNEDCHAR)
				transf_->setLinearTransfParameters(parIn.vmin_[0], parIn.vmin_[0], 0, 255);
		}
		else
		{
			par.setDummy(parIn.dummy_[0]);
			par.setDataType(parIn.dataType_[0]);
			par.setPhotometric(parIn.photometric_[0]);
		}

		short br = rLineEdit->text().toShort();
		short bg = gLineEdit->text().toShort();
		short bb = bLineEdit->text().toShort();
		if (br == 0 && bg == 1 && bb == 2)
		{
			transf_->setTransfFunction(&TeRasterTransform::Band2Band);
		}
		else
		{
			map<TeRasterTransform::TeRGBChannels,short> bmap;
			bmap[TeRasterTransform::TeREDCHANNEL] = br;
			bmap[TeRasterTransform::TeGREENCHANNEL] = bg;
			bmap[TeRasterTransform::TeBLUECHANNEL] = bb;

			transf_->setRGBmap(bmap);
			transf_->setTransfFunction(&TeRasterTransform::ExtractRGB);
		}
	}

	QString mess = tr("Initializing output raster...");
	TeRaster* rasterOut = new TeRaster(par);
	bool res = rasterOut->init();
	if (!res)
	{
		delete rasterOut;
		QString msg = tr("Fail to create the output raster!") + "\n";
		msg += tr("There is not enough memory available or there is no permission to access the output file.");
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}
	if (TeProgress::instance())
	{
		QString caption = tr("Saving");
		TeProgress::instance()->setCaption(caption.latin1());
		QString msg = tr("Saving raster. Please, wait!");
		TeProgress::instance()->setMessage(msg.latin1());
	}
	TeWaitCursor wait;
	res = raster_->fillRaster(rasterOut, transf_, true);
	wait.resetWaitCursor();

	if (TeProgress::instance())
		TeProgress::instance()->reset();

	if (transf_)
		delete transf_;
	if (!res)
		QMessageBox::critical(this, tr("Error"), tr("Fail to save the raster!"));
	else
		QMessageBox::information(this, tr("Information"),
			tr("The raster was saved successfully!"));
	rasterOut->clear();
	delete rasterOut;
	accept();
}


void SaveRaster::projPushButton_clicked()
{
	ProjectionWindow *projectionWindow = new ProjectionWindow(this, "projection", true);
	if (proj_)
		projectionWindow->loadProjection(proj_);
	if (projectionWindow->exec() == QDialog::Accepted)
	{
		if (proj_)
			delete proj_;
		proj_ = projectionWindow->getProjection();
		string mess = proj_->name() + "/" + proj_->datum().name();
		projNameLineEdit->setText(mess.c_str());
	}
	delete projectionWindow;
}


void SaveRaster::openRFileButton_clicked()
{
	openRFileLineEdit->clear();
	QString ext;
	int fmt = formatComboBox->currentItem();
	switch (fmt)
	{ 
	case 0:
		ext = ".raw";
		break;
	case 1:
		ext = ".tif";
		break;
	case 2:
		ext = ".jpg";
		break;
	case 3:
		ext = ".spr";
		break;
	case 4:
		ext = ".txt";
	}

	QString filter = "*";
	filter += ext;
	QString curPath = openRFileLineEdit->text();
	if (!curPath.isEmpty())
	{
		int n = curPath.findRev('/');
		if (n != -1)
			curPath.truncate(n);
	}
	string fname;
	QString s = QFileDialog::getSaveFileName(curPath,filter,this,"Save as","File name");
	if (!s.isEmpty())
		fname = TeGetName(s.latin1());
	fname.append(ext.latin1());
	openRFileLineEdit->setText(fname.c_str());
	lastOpenDir_ = TeGetPath(fname.c_str()).c_str();
}


void SaveRaster::RMButtonGroup_clicked(int i)
{
	if (!raster_)
		return;

	int nc = bandListBox->count();
	if (nc == 0)
		return;

	if (i==1 && formatComboBox->currentItem() == 0)
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Can not save RGB raster in raw format!"));
		RMButtonGroup->setButton(0);
		RMButtonGroup_clicked(0);
		return;
	}

	if (i==1 && formatComboBox->currentItem() == 3)
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Can not save RGB raster in Spring format!"));
		RMButtonGroup->setButton(0);
		RMButtonGroup_clicked(0);
		return;
	}

	if (i==1 && formatComboBox->currentItem() == 4)
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Can not save RGB raster in ASCII-GRID format!"));
		RMButtonGroup->setButton(0);
		RMButtonGroup_clicked(0);
		return;
	}
	if (i==0 || i == 2)
	{
		rRadioButton->setText("M");
		rRadioButton->setChecked(true);
		gRadioButton->setEnabled(false);
		bRadioButton->setEnabled(false);
		gLineEdit->setEnabled(false);
		bLineEdit->setEnabled(false);
		bandListBox->setCurrentItem(0);
		rLineEdit->setText(bandListBox->currentText());
		gLineEdit->clear();
		bLineEdit->clear();
	}
	else
	{
		rRadioButton->setText("R");
		gRadioButton->setEnabled(true);
		bRadioButton->setEnabled(true);
		gLineEdit->setEnabled(true);
		bLineEdit->setEnabled(true);
		rLineEdit->clear();
		gLineEdit->clear();
		bLineEdit->clear();
		rRadioButton->setChecked(true);
		bandListBox->setCurrentItem(0);
		if (nc > 1)
		{
			gRadioButton->setChecked(true);
			bandListBox->setCurrentItem(1);
			if (nc > 2)
			{
				bRadioButton->setChecked(true);
				bandListBox->setCurrentItem(2);
			}
		}
	}
}


void SaveRaster::rgbButtonGroup_clicked(int i)
{
	if (!raster_)
		return;

	if (i==0)
	{
		rLineEdit->setText(bandListBox->currentText());
	}
	else if (i==1)
	{
		gLineEdit->setText(bandListBox->currentText());
	}
	else 
	{
		bLineEdit->setText(bandListBox->currentText());
	}
}


void SaveRaster::projButtonGroup_clicked( int i)
{
	if (!theme_)
		return;
	if (proj_)
	{
		delete proj_;
		proj_ = 0;
	}

	TeTheme* baseTheme = (TeTheme*)theme_->getTheme();
	TeLayer* layer_ = baseTheme->layer();
	if (i==0)  // view projection
	{
		int vid = baseTheme->view();
		TeView* view_ = 0;
		TeViewMap& viewMap = db_->viewMap();
		TeViewMap::iterator it;
		for (it = viewMap.begin(); it != viewMap.end(); ++it)
		{
			if (it->second->id() == vid)
			{
				view_ = it->second;
				break;
			}
		}
		proj_ = TeProjectionFactory::make(view_->projection()->params());
	}
	else if (i==1)	// layer projection
	{
		proj_ = TeProjectionFactory::make(layer_->projection()->params());
	}
	else
	{
		proj_ = TeProjectionFactory::make(layer_->projection()->params());
		projPushButton_clicked();
		if (proj_->name() == "NoProjection" && 
			layer_->projection()->name() != "NoProjection")
		{
			QMessageBox::warning(this, tr("Warning"),
				tr("It is not possible to export a raster \"No Projection\" from a raster with projection!"));
			delete proj_;
			proj_ = TeProjectionFactory::make(layer_->projection()->params());
		}
	}
	string pname = proj_->name() + "/" + proj_->datum().name();
	projNameLineEdit->setText(pname.c_str());
	samplingFactorComboBox->setCurrentItem(0);
	samplingFactorComboBox_highlighted(0);
}


void SaveRaster::objComboBox_activated( int )
{
	bandListBox->clear();
	TeTheme* baseTheme = (TeTheme*)theme_->getTheme();
	TeLayer* layer = baseTheme->layer();
	if (raster_)
		raster_ = 0;

	raster_ = layer->raster(objComboBox->currentText().latin1());
	if (!raster_)
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("It is not possible to access the raster!"));
		return;
	}
	int nb = raster_->params().nBands();
	for (int n=0; n<nb; n++)
	{
		QString bname =  QString("%1").arg( n );
		bandListBox->insertItem(bname);
	}

	int RGBMLUTButton = 0;
	int formatButton = 2;

	if (baseTheme->grouping().groupMode_ == TeRasterSlicing)		// sliced raster
	{												
		LutRadioButton->setEnabled(true);			// save it sliced
		LutRadioButton->setChecked(true);
		MRadioButton->setEnabled(true);				// or save the original value
		RGBRadioButton->setEnabled(false);
		RGBMLUTButton = 2;
	}
	else if (raster_->params().photometric_[0] == TeRasterParams::TePallete)  // raster with LUT
	{
		LutRadioButton->setEnabled(true);					// save it only with palette
		LutRadioButton->setChecked(true);
		RGBRadioButton->setEnabled(false);
		MRadioButton->setEnabled(false);
		RGBMLUTButton = 2;
	}
	else											// non sliced and non LUT
	{
		LutRadioButton->setEnabled(false);
		MRadioButton->setEnabled(true);				// or save the original value
		if (raster_->params().dataType_[0] != TeUNSIGNEDCHAR)
			formatButton = 1;
		if (raster_->nBands() >= 3)
		{
			RGBRadioButton->setEnabled(true);
			RGBMLUTButton = 1;
		}
		else
			RGBRadioButton->setEnabled(false);
	}
	RMButtonGroup->setButton(RGBMLUTButton);
	formatComboBox->setCurrentItem(formatButton);
	RMButtonGroup_clicked(RGBMLUTButton);
	formatComboBox_activated(formatButton);
	samplingFactorComboBox->setCurrentItem(0);
	samplingFactorComboBox_highlighted(0);
}


void SaveRaster::formatComboBox_activated(int i)
{
	if (!raster_)
		return;

	QString filename = openRFileLineEdit->text();
	string fname = filename.latin1();
	if (!filename.isEmpty())
	{
		fname = TeGetName(filename);
		switch (i)
		{ 
		case 0:
			if (RMButtonGroup->selectedId() == 1)
			{
				QMessageBox::warning(this, tr("Warning"),
					tr("Can not save RGB raster in raw format!"));
				formatComboBox->setCurrentItem(1);
				fname.append(".tif");
			}
			else
				fname.append(".raw");
			break;
		case 1:
			fname.append(".tif");
			break;
		case 2:
			fname.append(".jpg");
			break;
		case 3:
			if (RMButtonGroup->selectedId() == 1)
			{
				QMessageBox::warning(this, tr("Warning"),
					tr("Can not save RGB raster in Spring format!"));
				formatComboBox->setCurrentItem(1);
				fname.append(".tif");
			}
			else
				fname.append(".spr");
			break;
		case 4:
			fname.append(".txt");
			if (RMButtonGroup->selectedId() == 1)
			{
				QMessageBox::warning(this, tr("Warning"),
					tr("ASCII-GRID format only supports rasters with one band."));
				RMButtonGroup->setButton(0);
				RMButtonGroup_clicked(0);
			}
		}
	}
	openRFileLineEdit->setText(fname.c_str());
}


void SaveRaster::samplingFactorComboBox_highlighted( int s)
{
	if (!raster_)
		return;

	TeProjection* projIn = raster_->projection();

	double resx = raster_->params().resx_;
	double resy = raster_->params().resy_;

	if (proj_ && projIn && !(*proj_== *projIn))
	{
		TeBox boxIn = raster_->params().box();
		TeBox newBox = TeRemapBox(boxIn,projIn,proj_);
		resx = newBox.width()/raster_->params().ncols_;		// recalculates resolutions 
		resy = newBox.height()/raster_->params().nlines_;	// for the new projection
	}
	string aux;
	aux = Te2String(resx*(s+1),6);
	resXLineEdit->setText(aux.c_str());
	resXLineEdit->setCursorPosition(0);

	aux = Te2String(resy*(s+1),6);
	resYLineEdit->setText(aux.c_str());
	resYLineEdit->setCursorPosition(0);
}

void SaveRaster::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("saveRaster.htm");
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



QString SaveRaster::getLastOpenDir()
{
	return lastOpenDir_;
}


void SaveRaster::setParams( TeDatabase * db, TeBox bCanvas, TeProjection * projCanvas, QString lastOpenDir )
{
	db_ = db;
	roi_ = bCanvas;
	projCanvas_ = projCanvas;
	lastOpenDir_ = lastOpenDir;
}


void SaveRaster::saveProjection( TeProjection *proj, QString fileName )
{
	std::string		sFileName;
	QDomDocument	doc;
	QDomElement		deValue;
	QDomText		deValueNode;
	
	
	if( (proj == 0) || (fileName.isEmpty() == true)) return;
	sFileName=fileName.latin1();
	if(sFileName[sFileName.size() - 4] == '.') sFileName=sFileName.erase(sFileName.size() - 4,4);
	if(sFileName.empty()  == true)	return;
	sFileName+=".xml";

	
	QDomElement root = doc.createElement( "Projection" );
	doc.appendChild(root);

	//Projection Name
	deValue = doc.createElement( "Name" );
	deValueNode = doc.createTextNode("Name");
	deValueNode.setData(proj->name().c_str());
    deValue.appendChild( deValueNode );
	root.appendChild(deValue);
	//Datum
	deValue = doc.createElement( "Datum" );
	deValueNode = doc.createTextNode("Datum");
	deValueNode.setData(proj->datum().name().c_str());
    deValue.appendChild( deValueNode );
	root.appendChild(deValue);
	//Unit
	deValue = doc.createElement( "Unit" );
	deValueNode = doc.createTextNode("Unit");
	deValueNode.setData(proj->units().c_str());
    deValue.appendChild( deValueNode );
	root.appendChild(deValue);
	//Origin_Longitude
	deValue = doc.createElement( "Origin_Longitude" );
	deValueNode = doc.createTextNode("Origin_Longitude");
	deValueNode.setData(Te2String(proj->lon0() * TeCRD,15).c_str());
    deValue.appendChild( deValueNode );
	root.appendChild(deValue);
	//Origin_Latitude
	deValue = doc.createElement( "Origin_Latitude" );
	deValueNode = doc.createTextNode("Origin_Latitude");
	deValueNode.setData(Te2String(proj->lat0() * TeCRD,15).c_str());
    deValue.appendChild( deValueNode );
	root.appendChild(deValue); 
	//Parallel1
	deValue = doc.createElement( "Parallel1" );
	deValueNode = doc.createTextNode("Parallel1");
	deValueNode.setData(Te2String(proj->stLat1(),15).c_str());
    deValue.appendChild( deValueNode );
	root.appendChild(deValue);
	//Parallel2
	deValue = doc.createElement( "Parallel2" );
	deValueNode = doc.createTextNode("Parallel2");
	deValueNode.setData(Te2String(proj->stLat2(),15).c_str());
    deValue.appendChild( deValueNode );
	root.appendChild(deValue);
	//OffsetX
	deValue = doc.createElement( "OffsetX" );
	deValueNode = doc.createTextNode("OffsetX");
	deValueNode.setData(Te2String(proj->offX(),15).c_str());
    deValue.appendChild( deValueNode );
	root.appendChild(deValue);
	//OffsetY
	deValue = doc.createElement( "OffsetY" );
	deValueNode = doc.createTextNode("OffsetY");
	deValueNode.setData(Te2String(proj->offY(),15).c_str());
    deValue.appendChild( deValueNode );
	root.appendChild(deValue);
	//Scale
	deValue = doc.createElement( "Scale" );
	deValueNode = doc.createTextNode("Scale");
	deValueNode.setData(Te2String(proj->scale(),15).c_str());
    deValue.appendChild( deValueNode );
	root.appendChild(deValue);
	//Hemisphere
	deValue = doc.createElement( "Hemisphere" );
	deValueNode = doc.createTextNode("Hemisphere");
	if(proj->hemisphere() == TeNORTH_HEM)	deValueNode.setData("North");
	else									deValueNode.setData("South");
    deValue.appendChild( deValueNode );
	root.appendChild(deValue);

    
	QFile file(sFileName.c_str());
	if (!file.open(IO_ReadWrite))	return ;
	QTextStream out(&file);
	out << doc.toString();
	file.close();
}
