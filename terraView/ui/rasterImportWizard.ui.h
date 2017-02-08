/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
// includes from Qt
#include <qcheckbox.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qlineedit.h>
#include <qfiledialog.h>

// includes from STL
#include <vector>

// includes from TerraLib
#include "TeImportRaster.h"
#include "TeWaitCursor.h"
#include "TeLayer.h"
#include "TeRaster.h"
#include "TeProjection.h"
#include "TeGUIUtils.h"

// includes from TerraView
#include "projection.h"
#include "terraViewBase.h"
#include "TeQtDatabasesListView.h"
#include "help.h"


void RasterImportWizard::init()
{
	help_ = 0;
	db_ = 0;
	projection_ = 0;
	raster_ = 0;

	compressComboBox->insertItem(tr("None"));
	compressComboBox->insertItem("ZLIB");
	compressComboBox->insertItem("JPEG");
	compressComboBox->setCurrentItem(0);

	inputDummyLineEdit->setText("");
	dummyLineEdit->setText("");

	typeComboBox->insertItem("unsigned char");
	typeComboBox->insertItem("char");
	typeComboBox->insertItem("unsigned short");
	typeComboBox->insertItem("short");
	typeComboBox->insertItem("unsigned long");
	typeComboBox->insertItem("long");
	typeComboBox->insertItem("int");
	typeComboBox->insertItem("float");
	typeComboBox->insertItem("double");

	bhComboBox->clear();
	bhComboBox->insertItem("");
	bhComboBox->insertItem("32");
	bhComboBox->insertItem("64");
	bhComboBox->insertItem("128");
	bhComboBox->insertItem("256");
	bhComboBox->insertItem("512");

	bwComboBox->clear();
	bhComboBox->insertItem("");
	bwComboBox->insertItem("32");
	bwComboBox->insertItem("64");
	bwComboBox->insertItem("128");
	bwComboBox->insertItem("256");
	bwComboBox->insertItem("512");

	layerNameComboBox->clear();
	layerNameComboBox->insertItem("");
	layerNameComboBox->setCurrentItem(0);

	MosaicCheckBox->setChecked(false);
	interComboBox->setEnabled(false);
	swapCheckBox->setEnabled(false);
	openRFileLineEdit->setText(lastOpenDir_ + "/...");
	openRFileLineEdit->setEnabled(false);
	setFinishEnabled(MetadataPage,true);
	setFinishEnabled(GeogParamsPage,true);
	setFinishEnabled(StoreParamsPage,true);
	setFinishEnabled(MultiResPage,true);

	importedLayer_ = 0;
	qfilename_="";
}


void RasterImportWizard::openRFileButton_clicked()
{
	// check for files of the supported formats
	QString curPath = openRFileLineEdit->text();
	if (!curPath.isEmpty())
	{
		int n = curPath.findRev('/');
		curPath.truncate(n);
	}
	qfilename_ = QFileDialog::getOpenFileName(curPath, 
		"GeoTiff Files(*.tif *.TIF);;JPG Files(*.jpg *.JPG);;SPR Grid Files(*.spr *.SPR);;Binary Raw Files(*.raw *.RAW);;ESRI ASCII Grid Files(*.asc *.ASC *.txt *.TXT *.grd *.GRD)",
	  this, 0, tr( "Open input file"));
	
	if (!qfilename_.isEmpty())
		openRFileLineEdit->setText(qfilename_);
	else
		return;

	// save directory or future operations
	lastOpenDir_ = TeGetPath(qfilename_.latin1()).c_str();

	// release variables used in previous operations
	if (raster_)
	{
		raster_->clear();
		delete raster_;
	}
	raster_= 0;

	if ( projection_)
		delete projection_;
	projection_ = 0;

	// find a valid name for a new layer
	QString newName = TeGetBaseName(qfilename_.latin1()).c_str();
	getValidLayerName(newName);
	lNameP1->setText(newName);
	lNameP2->setText(lNameP1->text());
	setLayerNameComboBox(lNameP1->text());

	// try to initialize a raster from the input file
	raster_ = new TeRaster(qfilename_.latin1());
	TeRasterParams par = raster_->params();

	// fill interface with the parameters defined in the file
	if (raster_->projection())
		projection_  = TeProjectionFactory::make(raster_->projection()->params());
	else
		projection_ = new TeNoProjection();
	string projName = projection_->name() + " / " + (projection_->datum()).name();
	projectionRLineEdit->setText(projName.c_str());

	// set number of decimal characters to be shown in the interface accordingly to the projection
	char fmt[100];
	if (projection_->name() == "NoProjection" ||
		projection_->name() == "LatLong")
		strcpy(fmt,"%.8f");
	else
		strcpy(fmt,"%.3f");
	QString myValue;

	// data characteristics
	nLinesLineEdit->setText(QString("%1").arg(par.nlines_));
	nColsLineEdit->setText(QString("%1").arg(par.ncols_));
	if (par.nBands() > 0)
		nBandsLineEdit->setText(QString("%1").arg(par.nBands()));
	
	if (par.useDummy_)
	{
		myValue.sprintf(fmt,par.dummy_[0]);
		inputDummyLineEdit->setText(myValue);
	}
	else
		inputDummyLineEdit->clear();

	switch (par.dataType_[0])
	{
	case TeUNSIGNEDCHAR:
		typeComboBox->setCurrentItem(0);
		break;
	case TeCHAR:
		typeComboBox->setCurrentItem(1);
		break;
	case TeUNSIGNEDSHORT:
		typeComboBox->setCurrentItem(2);
		break;
	case TeSHORT:
		typeComboBox->setCurrentItem(3);
		break;
	case TeINTEGER:
		typeComboBox->setCurrentItem(4);
		break;
	case TeUNSIGNEDLONG:
		typeComboBox->setCurrentItem(5);
		break;
	case TeLONG:
		typeComboBox->setCurrentItem(6);
		break;
	case TeFLOAT:
		typeComboBox->setCurrentItem(7);
		break;
	case TeDOUBLE:
		typeComboBox->setCurrentItem(8);
        break;
    default:
        break;
	}
	
	if (par.photometric_[0] == TeRasterParams::TePallete)
		palleteCheckBox->setChecked(true);
	else
		palleteCheckBox->setChecked(false);

	// geographical characteristics
	myValue.sprintf(fmt,par.resx_);
	resXLineEdit->setText(myValue);
	myValue.sprintf(fmt,par.resy_);
	resYLineEdit->setText(myValue);

	if (par.box().isValid())
	{
		myValue.sprintf(fmt,par.box().x1_);
		x0LineEdit->setText(myValue);
		myValue.sprintf(fmt,par.box().y1_);
		y0LineEdit->setText(myValue);
		myValue.sprintf(fmt,par.box().x2_);
		x1LineEdit->setText(myValue);
		myValue.sprintf(fmt,par.box().y2_);
		y1LineEdit->setText(myValue);
	}

	compressComboBox->setCurrentItem(0);

	// fill specific information according to the raster format
	string ext = TeGetExtension(qfilename_.latin1());
	if (ext == "raw" || ext == "RAW")
	{
		// raw files don't have any information about the data
		// open the interface for the user to enter it
		nLinesLineEdit->setEnabled(true);
		nColsLineEdit->setEnabled(true);
		nBandsLineEdit->setEnabled(true);
		swapCheckBox->setEnabled(true);
		typeComboBox->setEnabled(true);
		interComboBox->setEnabled(true);
		interComboBox->setCurrentItem(0);
		return;
	}
	else
	{
		// users can not change it for other cases
		nLinesLineEdit->setEnabled(false);
		nColsLineEdit->setEnabled(false);
		nBandsLineEdit->setEnabled(false);
		swapCheckBox->setEnabled(false);
		typeComboBox->setEnabled(false);
		interComboBox->setEnabled(false);
	}

	if (typeComboBox->currentItem() != 0)
		compressComboBox->setCurrentItem(0);
	if (par.swap_)
		swapCheckBox->setChecked(true);
	else
		swapCheckBox->setChecked(false);

	if (par.nBands() > 1)
	{
		if (par.interleaving_ == TeRasterParams::TePerBand)
			interComboBox->setCurrentItem(0);
		else if (par.interleaving_ == TeRasterParams::TePerPixel)
			interComboBox->setCurrentItem(1);
		else
			interComboBox->setCurrentItem(2);
	}

	// assume is a layer
	layerNameComboBox->setCurrentItem(0);	
	objectIdcomboBox->setCurrentItem(0);
	bhComboBox->setCurrentItem(0);
	bwComboBox->setCurrentItem(0);
	curLayerPtr_ = 0;
	return;
}


void RasterImportWizard::projectionRButton_clicked()
{
	ProjectionWindow *projectionWindow = new ProjectionWindow(this, "projection", true);
	if (projection_)
		projectionWindow->loadProjection(projection_);
	if (projectionWindow->exec() == QDialog::Accepted)
	{
		projection_ = projectionWindow->getProjection();
		string projName = projection_->name() + " / " + (projection_->datum()).name();
		projectionRLineEdit->setText(projName.c_str());
	}
	delete projectionWindow;
}

void RasterImportWizard::validateBoxPushButton_clicked()
{
	// Coordinates should be relative to BOX (centre of the pixels)
	double resx = resXLineEdit->text().toDouble();
	double resy = resYLineEdit->text().toDouble();

	int nlines = nLinesLineEdit->text().toInt();
	int ncols = nColsLineEdit->text().toInt();

	if (nlines <=0 || ncols <= 0 || resx <= 0 || resy <= 0)
	{
		QMessageBox::warning( this, tr("Warning"),
			tr("Invalid number of lines or columns or x and y resolutions invalid!"));
		return;
	}
	
	QString x0T = x0LineEdit->text();
	QString y0T = y0LineEdit->text();
	QString x1T = x1LineEdit->text();
	QString y1T = y1LineEdit->text();

	TeBox box;
	double lx, rx, ly, uy; 
	if (!x0T.isEmpty())
	{
		lx = x0T.toDouble();
		if (!y0T.isEmpty())			// lower-left coordinate given
		{
			ly = y0T.toDouble();
			box = TeBox(lx,ly,lx+(ncols-1)*resx,ly+(nlines-1)*resy);
		}
		else if (!y1T.isEmpty())		// upper-left coordinate given
		{
			uy = y1T.toDouble();
			box = TeBox(lx,uy-((nlines-1)*resy),lx+(ncols-1)*resx,uy);
		}
		else
		{
			QMessageBox::warning( this, tr("Warning"),
				tr("Provide a Y coordinate!"));
			return;
		}
	}
	else if (!x1T.isEmpty())
	{
		rx = x1T.toDouble();
		if (!y0T.isEmpty())			// lower-right coordinate given
		{
			ly = y0T.toDouble();
			box = TeBox(rx-(ncols-1)*resx,ly,rx,ly+(nlines-1)*resy);
		} 
		else if (!y1T.isEmpty())		// upper-right coordinate given
		{
			uy = y1T.toDouble();
			box = TeBox(rx-(ncols-1)*resx,uy-(nlines-1)*resy,rx,uy);
		}
		else
		{
			QMessageBox::warning( this, tr("Warning"),
				tr("Provide a Y coordinate!"));
			return;
		}
	}
	else
	{
		QMessageBox::warning( this, tr("Warning"),
			tr("Provide a X coordinate!"));
		return;
	}
	if (!box.isValid())
	{
		QMessageBox::warning( this, tr("Warning"),
			tr("Invalid box coordinates!"));
		return;
	}
	char fmt[100];
	if (projection_->name() == "NoProjection" ||
		projection_->name() == "LatLong")
		strcpy(fmt,"%.8f");
	else
		strcpy(fmt,"%.3f");
	QString myValue;
	myValue.sprintf(fmt,box.x1_);
	x0LineEdit->setText(myValue);
	myValue.sprintf(fmt,box.y1_);
	y0LineEdit->setText(myValue);
	myValue.sprintf(fmt,box.x2_);
	x1LineEdit->setText(myValue);
	myValue.sprintf(fmt,box.y2_);
	y1LineEdit->setText(myValue);
}


void RasterImportWizard::layerNameComboBox_activated( int i )
{
	if (i==0)
	{
		objectIdcomboBox->clear();
		objectIdcomboBox->setCurrentItem(0);
		objectIdcomboBox_activated(0);
		dummyLineEdit->clear();
		MosaicCheckBox->setEnabled(false);
		bwComboBox->setEnabled(true);
		bhComboBox->setEnabled(true);
		dummyLineEdit->setEnabled(true);
		return;
	}

	// if an existing layer fill the interface with allowed parameters
	// get the choosen layer
	TeLayerMap& layerMap = db_->layerMap();
	string layername = layerNameComboBox->currentText().latin1();
	TeLayerMap::iterator itlay = layerMap.begin();
	curLayerPtr_=0;
	while ( itlay != layerMap.end() )
	{
		curLayerPtr_ = (*itlay).second;
		if (curLayerPtr_->name() == layername)
			break;
		++itlay;
	}
	// name in the combo box is not a valid layer
	if (itlay == layerMap.end())
	{
		objectIdcomboBox->clear();
		objectIdcomboBox->insertItem("");
		return;
	}

	// check for raster representation that are extensible in this layer
	unsigned int nn;
	objectIdcomboBox->clear();
	objectIdcomboBox->insertItem("");
	vector<string> objs;
	if (curLayerPtr_->getRasterGeometries(objs,1))
	{
		for (nn=0; nn<objs.size(); ++nn)		
			objectIdcomboBox->insertItem(objs[nn].c_str());
	}
	objectIdcomboBox->setCurrentItem(0);
	if (objectIdcomboBox->count()>1)
	{
		objectIdcomboBox->setCurrentItem(1);
		objectIdcomboBox_activated(1);
	}
}


void RasterImportWizard::objectIdcomboBox_activated( int )
{
	if (layerNameComboBox->currentText().isEmpty() || !curLayerPtr_ ||
		objectIdcomboBox->currentText().isEmpty())
		return;

	QString objid = objectIdcomboBox->currentText();
	// if an expansible representation was choosen
	TeRaster* rst = curLayerPtr_->raster(objid.latin1());
	if (rst->params().tiling_type_ == TeRasterParams::TeNoExpansible) // unusual error
	{
		QString mess = tr("The raster with object id \'");
		mess += objid;
		mess += tr("\' of the layer \'");
		mess += curLayerPtr_->name().c_str();
		mess += tr("\' was stored as non expansible!");
		QMessageBox::warning(this, tr("Warning"),mess);
		objectIdcomboBox->setCurrentItem(0);
		return;
	}

	MosaicCheckBox->setChecked(true);
	MosaicCheckBox->setEnabled(false);
	setBlockWidthComboValue(rst->params().blockWidth_);
	setBlockHeightComboValue(rst->params().blockHeight_);
	bwComboBox->setEnabled(false);
	bhComboBox->setEnabled(false);
	dummyLineEdit->setText(QString("%1").arg(rst->params().dummy_[0]));
	dummyLineEdit->setEnabled(false);
	if (rst->params().compression_[0] == TeRasterParams::TeZLib)
		compressComboBox->setCurrentItem(1);
	else if (rst->params().compression_[0] == TeRasterParams::TeJpeg)
		compressComboBox->setCurrentItem(2);
	else
		compressComboBox->setCurrentItem(0);
	rst->clear();
}


QString RasterImportWizard::getLastOpenDir()
{
	return lastOpenDir_;
}


void RasterImportWizard::setParams( TeDatabase * curDb, QString lastOpenDir )
{
	db_ = curDb;
	lastOpenDir_ = lastOpenDir;
	openRFileLineEdit->setText(lastOpenDir_ + "/...");
	fillLayerCombo(db_, TeRASTER, layerNameComboBox, "", false);
	layerNameComboBox->setCurrentItem(0);
}

void RasterImportWizard::numberLevelsComboBox_highlighted( int i )
{
	for (int ii=0; ii<=i; ++i)
		extraResListBox->setSelected(ii,true);
}


void RasterImportWizard::pageChangedSlot(const QString  & /*pageName*/ )
{
	int id = indexOf(currentPage());
	switch (id)
	{
	case 1:

		if(!raster_)
		{
			QString mess = tr("Select a file to import first. ");
			QMessageBox::warning(this, tr("Warning"), mess);
			back();
			return;
		}

		if (!lNameP1->text().isEmpty())
		{
			string errorMessage;
			bool changed;
			string newName = TeCheckName(lNameP1->text().latin1(), changed, errorMessage); 
			if(changed)
			{
				QString mess = tr("The layer name is invalid: ") + errorMessage.c_str();
				mess += "\n" + tr("Change current name and try again.");
				QMessageBox::warning(this, tr("Warning"), mess);
				lNameP1->setText(newName.c_str());
				lNameP2->setText(lNameP1->text());
				back();
				return;
			}
		}
		else
		{
			QString mess = tr("The layer name can not be empty. ");
			QMessageBox::warning(this, tr("Warning"), mess);
			back();
			return;
		}

		lNameP2->setText(lNameP1->text());

		if (nColsLineEdit->text().toInt() < 1 || 
			nLinesLineEdit->text().toInt() < 1 ||
			nBandsLineEdit->text().toInt() < 1)
		{
			QString mess = tr("Number of lines, columns and bands should be greater than 0.");
			QMessageBox::warning(this, tr("Warning"),mess);
			back();
			return;
		}
		if (x0LineEdit->text().isEmpty() && x1LineEdit->text().isEmpty() &&
			y0LineEdit->text().isEmpty() && y1LineEdit->text().isEmpty() )
		{
			x0LineEdit->setText("0.0");
			y0LineEdit->setText("0.0");
			validateBoxPushButton_clicked();
		}

		// bypass the input dummy to output
		if (!inputDummyLineEdit->text().isEmpty())
			dummyLineEdit->setText(inputDummyLineEdit->text());
		break;
	case 2:
		if (layerNameComboBox->currentText().isEmpty())
		{// the default behaviour => importing data to a new layer
			if (!lNameP1->text().isEmpty())
				setLayerNameComboBox(lNameP1->text());
		}
		if (bhComboBox->currentItem()==0 || bhComboBox->currentItem())
		{
			int bw, bh;
			if (calculateDefaultBlockDimValues(bw,bh))
			{
				setBlockHeightComboValue(bh);
				setBlockWidthComboValue(bw);
			}
		}
		break;
	case 3:
		if (!inputDummyLineEdit->text().isEmpty())
			dummyLineEdit->setText(inputDummyLineEdit->text());
		setCurrentResolution();
		nextButton()->hide();

		break;
	}
}


void RasterImportWizard::setBlockWidthComboValue( int bw )
{
	// change interface to the default values
	int ii, cid = -1;
	for (ii=0; ii<bwComboBox->count(); ++ii)
	{
		if (bwComboBox->text(ii).toInt() == bw)
		{
			cid = ii;
			break;
		}
	}
	if (cid == -1)
	{
		bwComboBox->insertItem(QString("%1").arg(bw), bwComboBox->count());
		bwComboBox->setCurrentItem(bwComboBox->count()-1);
	}
	else
		bwComboBox->setCurrentItem(cid);
}


void RasterImportWizard::setBlockHeightComboValue( int bh )
{
	int ii, cid = -1;
	for (ii=0; ii<bhComboBox->count(); ++ii)
	{
		if (bhComboBox->text(ii).toInt() == bh)
		{
			cid = ii;
			break;
		}
	}
	if (cid == -1)
	{
		bhComboBox->insertItem(QString("%1").arg(bh), bhComboBox->count());
		bhComboBox->setCurrentItem(bhComboBox->count()-1);
	}
	else
		bhComboBox->setCurrentItem(cid);
}


bool RasterImportWizard::calculateDefaultBlockDimValues( int & bw, int & bh )
{
	// default behaviour for new layers: no expansible, suggest some values to
	// block dimensions
	bw = 512;
	bh = 512;		// an initial guess...

	if (nLinesLineEdit->text().isEmpty() || nColsLineEdit->text().isEmpty())
		return false;

	// if raster is up to 512x512 store it in one block
	if (nLinesLineEdit->text().toInt() < 512 && nColsLineEdit->text().toInt() < 512)
	{
		bw = nColsLineEdit->text().toInt();
		bh = nLinesLineEdit->text().toInt();
	}
	// if ncols<1024 build blocks of 1 line
	else if (nColsLineEdit->text().toInt() < 1024)  
	{
		bw = nColsLineEdit->text().toInt();
		bh = 1;
	}
	return true;
}


void RasterImportWizard::accept()
{
	if (qfilename_.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please select a file!"));
		return;
	}
	string ext = TeGetExtension(qfilename_.latin1());
	if (ext != "spr" && !raster_)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select a file!"));
		return;
	}
	
	importedLayer_ = 0;
	bool isNewLayer = true;
	int id = indexOf(currentPage());
	string layerName;
	if (id == 0 || id == 1 || layerNameComboBox->currentText().isEmpty())
	{
		if (lNameP1->text().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Enter with the layer name!"));
			return;
		}
		layerName = lNameP1->text().latin1();
	}
	else
		layerName =  layerNameComboBox->currentText().latin1();

	// if a new layer will be created check if its name is valid
	if (!db_->layerExist(layerName))
	{
		QString lname = layerName.c_str();
		if (getValidLayerName(lname))
		{
			QString mess = tr("The output layer name is invalid.");
			mess += "\n" + tr("Check in the interface a suggestion for a new name.");
			QMessageBox::warning(this, tr("Warning"), mess);
			layerNameComboBox->setCurrentText(lname);
			lNameP1->setText(lname);
			lNameP2->setText(lname);
			return;
		}
		if (!projection_)
		{
			QMessageBox::warning(this, tr("Warning"),
				tr("Enter with the raster projection!"));
			return;
		}
	}
	else
	{
		// check whether there is a layer with this name in the database
		TeLayerMap& layerMap = db_->layerMap();
		TeLayerMap::iterator it;
		for (it = layerMap.begin(); it != layerMap.end(); ++it)
		{
			if (it->second->name() == layerName)
			{
				importedLayer_ = it->second;
				isNewLayer = false;
				break;
			}
		}
	}
	// update input raster parameters with the values of the interface 
	TeRasterParams par;
	par.fileName_ = qfilename_.latin1();
	
	if (bwComboBox->currentText().isEmpty() ||
		bhComboBox->currentText().isEmpty())
		calculateDefaultBlockDimValues(par.blockWidth_ ,par.blockHeight_);
	else
	{
		par.blockWidth_ = bwComboBox->currentText().toInt();
		par.blockHeight_ = bhComboBox->currentText().toInt();
	}
	if (par.blockWidth_ <=0 || par.blockHeight_ <= 0)
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Invalid block size!"));
		return;
	}

	if (compressComboBox->currentText() == "ZLIB")
		par.setCompressionMode(TeRasterParams::TeZLib);
	else if (compressComboBox->currentText() == "JPEG")
		par.setCompressionMode(TeRasterParams::TeJpeg);
	else
		par.setCompressionMode(TeRasterParams::TeNoCompression);

	par.nBands(nBandsLineEdit->text().toInt());
	if (par.nBands() <= 0)
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Invalid number of bands!"));
		return;
	}
	par.ncols_ = nColsLineEdit->text().toInt();
	if (par.ncols_ <= 0)
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Invalid number of columns!"));
		return;
	}
	par.nlines_ = nLinesLineEdit->text().toInt();
	if (par.nlines_ <= 0)
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Invalid number of rows!"));
		return;
	}		
	par.resx_ = resXLineEdit->text().toDouble();
	if (par.resx_ < 0)
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Invalid horizontal resolution!"));
		return;
	}
	par.resy_ = resYLineEdit->text().toDouble();
	if (par.resy_ < 0)
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Invalid vertical resolution!"));
		return;
	}
	TeBox box;
    if (x0LineEdit->text().isEmpty())
		box.x1_ = 0.5;
	else
		box.x1_ = x0LineEdit->text().toDouble();

    if (y0LineEdit->text().isEmpty())
		box.y1_ = 0.5;
	else
		box.y1_ = y0LineEdit->text().toDouble();

    if (x1LineEdit->text().isEmpty())
		box.x2_ = par.ncols_-0.5;
	else
		box.x2_ = x1LineEdit->text().toDouble();

    if (y1LineEdit->text().isEmpty())
		box.y2_ = par.nlines_-0.5;
	else
		box.y2_ = y1LineEdit->text().toDouble();

	if (!box.isValid())
	{
		QMessageBox::warning( this, tr("Warning"),
			tr("Invalid box coordinates!"));
		return;
	}
	par.boxResolution(box.x1_,box.y1_,box.x2_,box.y2_,par.resx_,par.resy_);

	if (typeComboBox->currentText() == "unsigned char")
		par.setDataType(TeUNSIGNEDCHAR);
	else 
	{
		if (typeComboBox->currentText() == "char")
			par.setDataType(TeCHAR);
		else if (typeComboBox->currentText() == "unsigned short")
			par.setDataType(TeUNSIGNEDSHORT);
		else if (typeComboBox->currentText() == "short")
			par.setDataType(TeSHORT);
		else if (typeComboBox->currentText() == "int")
			par.setDataType(TeDataType(TeINT));
		else if (typeComboBox->currentText() == "unsigned long")
			par.setDataType(TeUNSIGNEDLONG);
		else if (typeComboBox->currentText() == "long")
			par.setDataType(TeLONG);
		else if (typeComboBox->currentText() == "float")
			par.setDataType(TeFLOAT);
		else if (typeComboBox->currentText() == "double")
			par.setDataType(TeDOUBLE);

		if (compressComboBox->currentItem() == 2)
		{
			QMessageBox::warning(this, tr("Warning"),
				tr("The JPEG compression can only be applied to an \'unsigned char\' raster!\nChoose another compression type."));
			compressComboBox->setCurrentItem(1);
			return;
		}
	}

	par.projection(projection_);
	par.mode_ = 'r';
	par.swap_ = swapCheckBox->isChecked();
	if (palleteCheckBox->isChecked())
	{
		par.setPhotometric(TeRasterParams::TePallete);
		if (compressComboBox->currentItem() == 2)
		{
			compressComboBox->setCurrentItem(1);
			QString msg = tr("The raster palette compressed by JPEG will be corrupted!\nDo you wish to change the compression type?");
			int response = QMessageBox::critical(this, tr("Error"),
				msg, tr("Yes"), tr("No"));
			if (response == 0)
				return;
		}
	}
	else
		par.setPhotometric(TeRasterParams::TeMultiBand);

	if (par.dataType_[0] != TeUNSIGNEDCHAR && compressComboBox->currentItem() == 2)
	{
		QString msg = tr("Raster with different values of \'unsigned char\' cannot be compressed by JPEG!\nSelect another compression algorithm.");
		QMessageBox::warning( this, tr("Warning"), msg);
		return;
	}

	if (!inputDummyLineEdit->text().isEmpty())
	{	
		par.useDummy_ = true;
		par.setDummy(inputDummyLineEdit->text().toDouble());
	}
	else
	{
		par.useDummy_ = false;
		par.setDummy(pow(2.,par.nbitsperPixel_[0])-1);
	}

	// check storing parameters
	// check object identification
	string objectid = objectIdcomboBox->currentText().latin1();
	if (objectid.empty())
		objectid = "O1";	// if empty, give a default value

	if (importedLayer_)
	{
		TeRaster* rst = importedLayer_->raster(objectid);
		if (rst && rst->params().tiling_type_ == TeRasterParams::TeNoExpansible)
		{
			QString msg = tr("A new raster data cannot be imported to a non extensible representation already stored!");
			QMessageBox::critical(this, tr("Error"), msg);
			return;
		}
		if (rst)
			isNewLayer = false;
		else
			isNewLayer = true;  // same layer but different objects
	}

	TeRasterParams::TeRasterTilingType indext = TeRasterParams::TeNoExpansible;
	if (MosaicCheckBox->isChecked())
		indext = TeRasterParams::TeExpansible;

	double outDummy = pow(2.,par.nbitsperPixel_[0])-1;
	bool outHasDummy = false;
	if (!dummyLineEdit->text().isEmpty())
	{
		outDummy = dummyLineEdit->text().toDouble();
		outHasDummy = true;
	}
	else if (indext==TeRasterParams::TeExpansible)
	{
		dummyLineEdit->setText(QString("%1").arg(outDummy));
		QString msg = tr("A dummy value should be provided to create an extensible raster!\nCheck the default value suggested.");
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}
	par.mode_ = 'r';
	raster_->updateParams(par);
	raster_->init();

	if (!raster_->status())			// check if raster is ready to be read
	{
		QString mess;
		mess = tr("Input raster cannot be read!");
		QMessageBox::critical(this, tr("Error"),mess);
		return;
	}

	TeProjection* inputProj = raster_->projection();
	if (projection_ && inputProj && !(*projection_ == *inputProj))
		raster_->params().projection(projection_);
	
	vector<int> extraRes;          // check if it is possible to do multi-resolution
	unsigned int i;
	for (i = 0; i < extraResListBox->count(); i++ ) 
	{
		QListBoxItem *item = extraResListBox->item( i );
		if ( item->isSelected() ) 
			extraRes.push_back(i+1);
    }
	if (extraRes.size() > 0)
	{
		if (raster_->params().compression_[0] == TeRasterParams::TeTiffCompression)
		{
			QString msg = tr("The TIFF/GeoTiff input data is compressed!\nExtra resolutions cannot be done.\nDo you want to redefine the parameters?");
			int response = QMessageBox::question(this, tr("Redefine Parameters"), msg, tr("Yes"), tr("No"));
			
			for (i=0; i<extraRes.size();i++)
				extraResListBox->setSelected(i-1,false);
			extraRes.clear();
			if(response == 0)
				return;
		}
	}
	
	if (!importedLayer_)        // if it is a new layer, create it
	{
		importedLayer_ = new TeLayer(layerName,db_,projection_);
		isNewLayer = true;
	}

	if(TeProgress::instance())
	{
		QString caption = tr("Importing");
		TeProgress::instance()->setCaption(caption.latin1());
		QString msg = tr("Importing raster. Please, wait!");
		TeProgress::instance()->setMessage(msg.latin1());
	}

	TeWaitCursor wait;
	// try to import raster
	bool res = TeImportRaster(importedLayer_,raster_,par.blockWidth_,par.blockHeight_, 
		par.compression_[0], objectid,outDummy,outHasDummy,indext);
	wait.resetWaitCursor();

	if (TeProgress::instance())
		TeProgress::instance()->reset();
	if (res)
	{
		// build extra resolutions
		for (i=0; i<extraRes.size();i++)
		{
			if(TeProgress::instance())
			{
				QString caption = tr("Creating multiresolution");
				TeProgress::instance()->setCaption(caption.latin1());
				QString msg = tr("Level") + " " + QString("%1").arg(extraRes[i]);
				msg += ". " + tr("Please, wait!");
				TeProgress::instance()->setMessage(msg.latin1());
			}
			int resFac = (int)pow(2.0,extraRes[i]);
			if (!TeBuildLowerResolution(importedLayer_,raster_,resFac,objectid))
			{
				QMessageBox::warning(this, tr("Warning"),
					tr("Fail to build the resolution!"));
				break;
			}
		}
		if (TeProgress::instance())
			TeProgress::instance()->reset();

		projectionRLineEdit->setText("");
		raster_->clear();
		delete raster_;
		raster_ = 0;
		if (isNewLayer)			
			QDialog::accept();
		else
		{
			QMessageBox::information(this, tr("Information"),
				tr("Layer updated!"));
			reject();
		}
	}
	else	// importing failed
	{
		if (isNewLayer)
			db_->deleteLayer(importedLayer_->id());	// remove bad layer
		QMessageBox::critical(this, tr("Error"), tr("Import failed!"));
		raster_->clear();
		delete raster_;
		raster_ = 0;
		delete projection_;
		projection_ = 0;
		reject();
	}

//update theme box
	TeBox layerBox = importedLayer_->box();

	TeThemeMap::iterator itMap = db_->themeMap().begin();

	while(itMap != db_->themeMap().end())
	{
		TeTheme* theme = dynamic_cast<TeTheme*>(itMap->second);

		if(theme && theme->layer() == importedLayer_)
		{
			theme->setThemeBox(layerBox);

			db_->updateTheme(theme);
		}

		++itMap;
	}
}


void RasterImportWizard::setLayerNameComboBox(const QString& layername)
{
	int ii, cid = -1;
	for (ii=0; ii<layerNameComboBox->count(); ++ii)
	{
		if (layerNameComboBox->text(ii) == layername)
		{
			cid = ii;
			break;
		}
	}
	if (cid == -1)
	{
		layerNameComboBox->insertItem(lNameP1->text(), layerNameComboBox->count());
		layerNameComboBox->setCurrentItem(layerNameComboBox->count()-1);
		int bw, bh;		
		if (calculateDefaultBlockDimValues(bw,bh))
		{
			setBlockWidthComboValue(bw);
			setBlockHeightComboValue(bh);
		}
	}
	else
	{
		layerNameComboBox->setCurrentItem(cid);
		layerNameComboBox_activated(cid);
	}
}


bool RasterImportWizard::getValidLayerName( QString & name )
{
	string errorMessage;
	bool changed = false;
	string newName = TeCheckName(name.latin1(), changed, errorMessage); 
	int nv = 1;
	string oldName = newName;
	while (db_->layerExist(newName))
	{
		changed = true;
		newName = oldName + "_" + Te2String(nv);
		++nv;
	}
	name = newName.c_str();
	return changed;
}

TeLayer * RasterImportWizard::getLayer()
{
	return importedLayer_;
}



void RasterImportWizard::helpButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("importRaster.htm");
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



void RasterImportWizard::numberLevelsComboBox_activated( int /*nl*/)
{
	
	extraResListBox->clearSelection();

	int value = numberLevelsComboBox->currentText().toInt();

	for (int ii=0; ii< value; ++ii)
		extraResListBox->setSelected(ii,true);
}


int RasterImportWizard::getMaxResolution( TeLayer *layer )
{
	TeDatabase			*database=0;
	TeDatabasePortal	*dbPortal=0;
	double				iResolution=0;
	std::string			tableName;
	std::string			SQL;
try
{
	if(layer == 0)										return 0;
	else if(layer->hasGeometry(TeRASTER) == false)		return 0;
	if((database=layer->database()) == 0) return 0;

	if(layer->raster() == 0)	return 0;
	tableName=layer->raster()->params().fileName_;
	if(tableName.empty())		return 0;
	SQL="select max(resolution_factor) from " + tableName;
	if((dbPortal=database->getPortal()) == 0)	return 0;
	if(dbPortal->query(SQL) && dbPortal->fetchRow())
		iResolution=(double)dbPortal->getInt(0);
	delete dbPortal;
}
catch(...)
{
	iResolution=0;
}
return (int)(log(iResolution)/log(2.0));
}


void RasterImportWizard::setCurrentResolution()
{
	std::string					layerName;
	TeLayer						*layer=0;
	TeLayerMap::iterator		it;
	int							resolution;

	if(db_ == 0) return;

	layerName =  layerNameComboBox->currentText().latin1();
	TeLayerMap& layerMap = db_->layerMap();
	for (it = layerMap.begin(); it != layerMap.end(); ++it)
		{
			if (it->second->name() == layerName)
			{
				layer = it->second;
				break;
			}
		}
	if(layer !=0)
	{
		resolution=getMaxResolution(layer);
		if(resolution !=0)
		{
			numberLevelsComboBox->setCurrentItem(resolution - 1);
			numberLevelsComboBox_activated(0);
		}
	}
}
