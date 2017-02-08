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

#include "TeProgress.h"
#include "TeGeometryAlgorithms.h"
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <TeQtViewsListView.h>
#include <qvalidator.h>

void CreateCells::init()
{
	help_ = 0;
	layersComboBox->setEnabled(true);
	themesComboBox->setEnabled(false);
	polygonsCheckBox->setEnabled(true);
	polygonsCheckBox->setChecked(false);
	layersComboBox->clear();

	mainWindow_ = (TerraViewBase*) qApp->mainWidget();
	db_ = mainWindow_->currentDatabase();

	TeLayerMap& layerMap = db_->layerMap();
	if (layerMap.empty())
	{
	    QMessageBox::critical(this, tr("Error"),
			tr("The database has no layers!"));
	    return;
	}
	
	curLayer_ = mainWindow_->currentLayer();
	TeLayerMap::iterator itlay = layerMap.begin();
	TeLayerMap::iterator itCurLayer = layerMap.begin();
	int i = 0, j = 0;
	while (itlay != layerMap.end())
	{
		layersComboBox->insertItem((*itlay).second->name().c_str());
		if (curLayer_ && curLayer_->name() == (*itlay).second->name().c_str())
		{	
			itCurLayer = itlay;
			j = i;
		}
		++itlay;
	    ++i;
	}
	layersComboBox->setCurrentItem(j);
	curLayer_ = (*itCurLayer).second;

	vector<TeQtViewItem*> viewItemVec = mainWindow_->getViewsListView()->getViewItemVec();
	if (viewItemVec.empty() == true)
	{
		themesComboBox->setEnabled(false);
		temeRadioButton->setDown(false);
		temeRadioButton->setEnabled(false);
		curTheme_ = 0;
	    return;
	}
	curTheme_ = mainWindow_->currentTheme();
	themesComboBox->clear();
	i=0;
	unsigned int k;
	for (k=0; k<viewItemVec.size(); ++k)
	{
		vector<TeQtThemeItem*> themeItemVec = viewItemVec[k]->getThemeItemVec();
		for (unsigned int n = 0; n < themeItemVec.size(); ++n)
		{
			TeAbstractTheme* theme = themeItemVec[n]->getAppTheme()->getTheme();
			if(theme->type() != TeTHEME)
				continue;
			if (theme->type() < 3 && theme->visibleGeoRep() /*& TePOLYGONS*/)
			{
				if (curTheme_ && theme->name() == curTheme_->getTheme()->name())
					j = i;
			    themesComboBox->insertItem(theme->name().c_str());
			    ++i;
			}
		}
	}
	temeRadioButton->setDown(false);
	if (k>0)
	{
		themesComboBox->setCurrentItem(j);
		temeRadioButton->setEnabled(true);
	}
	else
		temeRadioButton->setEnabled(false);
	curTheme_ = 0;

	layerRadioButton->setChecked(true);
	layerRadioButton_clicked();
	
	//Validator
	xLLLineEdit->setValidator(new QDoubleValidator(this));
	yLLLineEdit->setValidator(new QDoubleValidator(this));
	xURLineEdit->setValidator(new QDoubleValidator(this));
	yURLineEdit->setValidator(new QDoubleValidator(this));
	resXLineEdit->setValidator(new QDoubleValidator(this));
	resYLineEdit->setValidator(new QDoubleValidator(this));

	//only used as information
	xLLLineEdit->setEnabled(false);
	yLLLineEdit->setEnabled(false);
	xURLineEdit->setEnabled(false);
	yURLineEdit->setEnabled(false);

	columnsLineEdit->setEnabled(false);
	linesLineEdit->setEnabled(false);

	//fill unit combo box
	//enum TeUnits { TeUndefinedUnit, TeMillimeters, TeCentimeters, TeMeters, TeKilometers, TeMiles, TeInches, TeFeet, TeDecimalDegress };
	unitComboBox->clear();
	unitComboBox->insertItem("Meters");
	unitComboBox->insertItem("Kilometers");
	unitComboBox->insertItem("Centimeters");
	unitComboBox->insertItem("Millimeters");
	unitComboBox->insertItem("Feet");
	unitComboBox->insertItem("Inches");
	unitComboBox->insertItem("Miles");
	unitComboBox->insertItem("DecimalDegrees");

	if(curLayer_)
	{
		std::string curUnit = curLayer_->projection()->params().units;

		unitComboBox->setCurrentText(curUnit.c_str());
	}
}


void CreateCells::layerRadioButton_clicked()
{
	layersComboBox->setEnabled(true);
	themesComboBox->setEnabled(false);
	TeBox bb = curLayer_->box();
	if (curLayer_->hasGeometry(TePOLYGONS))
	{
		polygonsCheckBox->setEnabled(true);
		polygonsCheckBox->setChecked(true);
		bb = curLayer_->getRepresentation(TePOLYGONS)->box_;
	}
	else
	{
		polygonsCheckBox->setEnabled(false);
		polygonsCheckBox->setChecked(false);
	}
	showBox(bb);
}

void CreateCells::layersComboBox_activated( const QString & layername )
{
	TeLayerMap& layerMap = db_->layerMap();
	TeLayerMap::iterator itlay = layerMap.begin();
	while (itlay != layerMap.end())
	{
		if ((*itlay).second->name() == string(layername.latin1()))
		{
			curLayer_ = (*itlay).second;
			break;
		}
		++itlay;
	}
	if (curLayer_->hasGeometry(TePOLYGONS))
	{
		polygonsCheckBox->setEnabled(true);
		polygonsCheckBox->setChecked(true);
	}
	else
	{
		polygonsCheckBox->setEnabled(false);
		polygonsCheckBox->setChecked(false);
	}
	showBox(curLayer_->box());
	layerRadioButton->setChecked(true);
}

void CreateCells::themeRadioButton_clicked()
{
	if(curTheme_)
	{
		TeTheme* theme = (TeTheme*)(curTheme_->getTheme());
		TeLayer* inputLayer = theme->layer();

		if (inputLayer->hasGeometry(TePOLYGONS))
		{
			polygonsCheckBox->setEnabled(true);
			polygonsCheckBox->setChecked(true);

			showBox(curTheme_->getTheme()->getThemeBox());
		}
		else
		{
			polygonsCheckBox->setEnabled(false);
			polygonsCheckBox->setChecked(false);
		}
	}
	else
	{
		polygonsCheckBox->setEnabled(false);
		polygonsCheckBox->setChecked(false);
	}

	temeRadioButton->setChecked(true);
	layersComboBox->setEnabled(false);
	themesComboBox->setEnabled(true);
	themesComboBox_activated(themesComboBox->currentText());
}

void CreateCells::themesComboBox_activated(const QString & themename)
{
	vector<TeQtViewItem*> viewItemVec = mainWindow_->getViewsListView()->getViewItemVec();
	unsigned int k;
	for (k=0; k<viewItemVec.size(); ++k)
	{
		vector<TeQtThemeItem*> themeItemVec = viewItemVec[k]->getThemeItemVec();
		for (unsigned int n = 0; n < themeItemVec.size(); ++n)
		{
			TeTheme* theme = (TeTheme*)(themeItemVec[n]->getAppTheme())->getTheme();
			if (theme->name().c_str() == themename)
			{
				curTheme_ = themeItemVec[n]->getAppTheme();
				showBox(curTheme_->getTheme()->getThemeBox());

				TeTheme* theme = (TeTheme*)(curTheme_->getTheme());
				TeLayer* inputLayer = theme->layer();

				if (inputLayer->hasGeometry(TePOLYGONS))
				{
					polygonsCheckBox->setEnabled(true);
					polygonsCheckBox->setChecked(true);
				}
				else
				{
					polygonsCheckBox->setEnabled(false);
					polygonsCheckBox->setChecked(false);
				}

				return;
			}
		}
	}
}


void CreateCells::okPushButton_clicked()
{
	if (layerRadioButton->isChecked() && !curLayer_)
	{
		QMessageBox::critical( this, tr("Error"), tr("There is no layer selected."));
		return;
	}
	else if (temeRadioButton->isChecked() && !curTheme_)
	{
		QMessageBox::critical( this, tr("Error"), tr("There is no theme selected."));
		return;

	}

	string layerName = newLayerLineEdit->text().latin1();
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
		/* Mudança davido a falta de tradução genérica na Terralib.
		QString mess = tr("The output layer name is invalid: ") + errorMessage.c_str();
		mess += "\n" + tr("Change current name and try again.");
		*/		
		QString mess = tr("The output layer name is invalid!");
		QMessageBox::warning(this, tr("Warning"), mess);
		newLayerLineEdit->setFocus();
		return;
	}
	layerName = newName;
	if (db_->layerExist(layerName))
	{
		QMessageBox::warning(this,tr("Warning"),
		tr("Output layer name already exist, replace and try again!"));
		return;
	}
	
	TeCoord2D ll(xLLLineEdit->text().toDouble(),yLLLineEdit->text().toDouble());
	TeCoord2D ur(xURLineEdit->text().toDouble(),yURLineEdit->text().toDouble());
	TeBox searchBox(ll,ur);
	if (!searchBox.isValid())
	{
		QMessageBox::warning( this, tr("Warning"), tr("Invalid box coordinates!"));
		return;
	}
	float lWidth = searchBox .width();
	float lHeight = searchBox .height();
	double resX, resY;
	resX = resXLineEdit->text().toFloat();
	resY = resYLineEdit->text().toFloat();

	std::string layerUnit = curLayer_->projection()->params().units;
	std::string curUnit = unitComboBox->currentText().latin1();

	TeUnits enumLayertUnit = TeGetUnit(layerUnit);
	TeUnits enumCurUnit = TeGetUnit(curUnit);

	if(enumLayertUnit != enumCurUnit)
	{
		resX = TeConvertUnits(resX, enumCurUnit, enumLayertUnit);
		resY = TeConvertUnits(resY, enumCurUnit, enumLayertUnit);
	}

	if (resX <= 0. || resX >= lWidth)
	{
		QString msg = tr("Resolution in X must be > 0 and <") + " ";
		msg += Te2String(lWidth,4).c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}
	if (resY <= 0. || resY >= lHeight)
	{
		QString msg = tr("Resolution in Y must be > 0 and <") + " ";
		msg += Te2String(lHeight,4).c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}

	TeBox newBox = adjustToCut(searchBox,resX,resY);
	double x1,x2,y1,y2;
	x1 = newBox.lowerLeft().x();
	y1 = newBox.lowerLeft().y();
	x2 = newBox.upperRight().x();
	y2 = newBox.upperRight().y();

	int maxcols, maxlines;
	maxcols = (int)((y2-y1)/resY);
	maxlines = (int)((x2-x1)/resX);

	//QString msg = tr("The infolayer will be created with a maximum of") + " ";
	//msg += Te2String(maxlines).c_str();
	//msg += " " + tr("columns") + " X ";
	//msg += Te2String(maxcols).c_str();
	//msg += " " + tr("lines") + ".\n" + tr("Continue?");

	//if (QMessageBox::question(this, tr("Infolayer Creation"),
    //         msg, tr("Yes"), tr("No")) != 0)
	//	return;
	
	TeWaitCursor wait;
	if (temeRadioButton->isChecked())
	{
		if (!TeIntersects(newBox,curTheme_->getTheme()->getThemeBox()))
		{
			wait.resetWaitCursor();
			QString msg = tr("Choosen box doesn't intercept the theme box!\n No cells will be created.");
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}
		if(TeProgress::instance())
		{
			string caption = tr("Cells").latin1();
			TeProgress::instance()->setCaption(caption.c_str());
			string msg = tr("Creating the layer of cells. Please, wait!").latin1();
			TeProgress::instance()->setMessage(msg);
		}

		TeTheme* theme = (TeTheme*)(curTheme_->getTheme());
		TeLayer* inputLayer = theme->layer();
		TeRepresentation* pp = inputLayer->getRepresentation(TePOLYGONS);
	
		if (!pp)
		{
			newLayer_ = TeCreateCells(layerName,inputLayer,resX,resY,newBox,polygonsCheckBox->isChecked());
		}
		else
		{
			newLayer_ = TeCreateCells(layerName,(TeTheme*)(curTheme_->getTheme()),resX,resY,newBox);
		}
	}
	else if (layerRadioButton->isChecked())
	{
		if (!TeIntersects(newBox,curLayer_->box()))
		{
			wait.resetWaitCursor();
			QString msg = tr("Choosen box doesn't intercepts layer box.\n No cells will be created.");
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}
		if(TeProgress::instance())
		{
			string caption = tr("Cells").latin1();
			TeProgress::instance()->setCaption(caption.c_str());
			string msg = tr("Creating the layer of cells. Please, wait!").latin1();
			TeProgress::instance()->setMessage(msg);
		}
		newLayer_ = TeCreateCells(layerName,curLayer_,resX,resY,newBox,polygonsCheckBox->isChecked());
		if (newLayer_ == 0)
		{
			wait.resetWaitCursor();
			QMessageBox::critical( this, tr("Error"),
				tr("Fail to create a new infolayer of cells!"));
			return;	    
		}
	}
	//create spatial index
	if(newLayer_)
	{
		if(!newLayer_->database()->insertMetadata(newLayer_->tableName(TeCELLS),newLayer_->database()->getSpatialIdxColumn(TeCELLS), 0.0005,0.0005,newLayer_->box()))
		{
			wait.resetWaitCursor();
			QString msg = tr((db_->errorMessage()).c_str());
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}

		if(!newLayer_->database()->createSpatialIndex(newLayer_->tableName(TeCELLS),newLayer_->database()->getSpatialIdxColumn(TeCELLS), (TeSpatialIndexType)TeRTREE))
		{
			wait.resetWaitCursor();
			QString msg = tr((db_->errorMessage()).c_str());
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}
	}

	accept();
}


TeLayer* CreateCells::getNewLayer()
{
	return newLayer_;
}


void CreateCells::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("createCells.htm");
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




void CreateCells::polygonsCheckBox_clicked()
{

}




void CreateCells::showBox( TeBox & box )
{
	char txt[40];
	sprintf(txt,"%.8g",box.x1_);
	xLLLineEdit->setText(QString(txt));
	sprintf(txt,"%.8g",box.y1_);
	yLLLineEdit->setText(QString(txt));
	sprintf(txt,"%.8g",box.x2_);
	xURLineEdit->setText(QString(txt));
	sprintf(txt,"%.8g",box.y2_);
	yURLineEdit->setText(QString(txt));
}


void CreateCells::resXLineEdit_textChanged( const QString & value )
{
	if(value.isEmpty())
	{
		linesLineEdit->clear();
		columnsLineEdit->clear();

		return;
	}

	if(resYLineEdit->text().isEmpty())
	{
		return;
	}

	double resX = value.toDouble();
	double resY = resYLineEdit->text().toDouble();

	std::string layerUnit = curLayer_->projection()->params().units;
	std::string curUnit = unitComboBox->currentText().latin1();

	TeUnits enumLayertUnit = TeGetUnit(layerUnit);
	TeUnits enumCurUnit = TeGetUnit(curUnit);

	if(enumLayertUnit != enumCurUnit)
	{
		resX = TeConvertUnits(resX, enumCurUnit, enumLayertUnit);
		resY = TeConvertUnits(resY, enumCurUnit, enumLayertUnit);
	}

	TeCoord2D ll(xLLLineEdit->text().toDouble(),yLLLineEdit->text().toDouble());
	TeCoord2D ur(xURLineEdit->text().toDouble(),yURLineEdit->text().toDouble());
	TeBox searchBox(ll,ur);
	if (!searchBox.isValid())
	{
		QMessageBox::warning( this, tr("Warning"), tr("Invalid box coordinates!"));
		return;
	}

	float lWidth = searchBox .width();
	float lHeight = searchBox .height();

	if (resX < 0. || resX >= lWidth)
	{
		QString msg = tr("Resolution in X must be > 0 and <") + " ";
		msg += Te2String(lWidth,4).c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}

	if (resY < 0. || resY >= lHeight)
	{
		QString msg = tr("Resolution in Y must be > 0 and <") + " ";
		msg += Te2String(lHeight,4).c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}

	TeBox newBox = adjustToCut(searchBox,resX,resY);
	double x1,x2,y1,y2;
	x1 = newBox.lowerLeft().x();
	y1 = newBox.lowerLeft().y();
	x2 = newBox.upperRight().x();
	y2 = newBox.upperRight().y();

	int maxcols, maxlines;
	maxcols = (int)((y2-y1)/resY);
	maxlines = (int)((x2-x1)/resX);

	QString cols;
	cols.setNum(maxcols);

	QString lines;
	lines.setNum(maxlines);

	linesLineEdit->setText(cols);
	columnsLineEdit->setText(lines);
}


void CreateCells::resYLineEdit_textChanged( const QString & value )
{
	if(value.isEmpty())
	{
		linesLineEdit->clear();
		columnsLineEdit->clear();

		return;
	}

	if(resXLineEdit->text().isEmpty())
	{
		return;
	}

	double resY = value.toDouble();
	double resX = resXLineEdit->text().toDouble();

	std::string layerUnit = curLayer_->projection()->params().units;
	std::string curUnit = unitComboBox->currentText().latin1();

	TeUnits enumLayertUnit = TeGetUnit(layerUnit);
	TeUnits enumCurUnit = TeGetUnit(curUnit);

	if(enumLayertUnit != enumCurUnit)
	{
		resX = TeConvertUnits(resX, enumCurUnit, enumLayertUnit);
		resY = TeConvertUnits(resY, enumCurUnit, enumLayertUnit);
	}

	TeCoord2D ll(xLLLineEdit->text().toDouble(),yLLLineEdit->text().toDouble());
	TeCoord2D ur(xURLineEdit->text().toDouble(),yURLineEdit->text().toDouble());
	TeBox searchBox(ll,ur);
	if (!searchBox.isValid())
	{
		QMessageBox::warning( this, tr("Warning"), tr("Invalid box coordinates!"));
		return;
	}

	float lWidth = searchBox .width();
	float lHeight = searchBox .height();

	if (resX < 0. || resX >= lWidth)
	{
		QString msg = tr("Resolution in X must be > 0 and <") + " ";
		msg += Te2String(lWidth,4).c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}

	if (resY < 0. || resY >= lHeight)
	{
		QString msg = tr("Resolution in Y must be > 0 and <") + " ";
		msg += Te2String(lHeight,4).c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}

	TeBox newBox = adjustToCut(searchBox,resX,resY);
	double x1,x2,y1,y2;
	x1 = newBox.lowerLeft().x();
	y1 = newBox.lowerLeft().y();
	x2 = newBox.upperRight().x();
	y2 = newBox.upperRight().y();

	int maxcols, maxlines;
	maxcols = (int)((y2-y1)/resY);
	maxlines = (int)((x2-x1)/resX);

	QString cols;
	cols.setNum(maxcols);

	QString lines;
	lines.setNum(maxlines);

	linesLineEdit->setText(cols);
	columnsLineEdit->setText(lines);
}


void CreateCells::unitComboBox_activated( const QString & value )
{
	if(value.isEmpty())
	{
		linesLineEdit->clear();
		columnsLineEdit->clear();

		return;
	}

	if(!curLayer_)
	{
		return;
	}

	std::string layerUnit = curLayer_->projection()->params().units;
	std::string curUnit = unitComboBox->currentText().latin1();

	TeUnits enumLayertUnit = TeGetUnit(layerUnit);
	TeUnits enumCurUnit = TeGetUnit(curUnit);

	if(resXLineEdit->text().isEmpty() || resYLineEdit->text().isEmpty())
	{
		return;
	}

	double resX = resXLineEdit->text().toDouble();
	double resY = resYLineEdit->text().toDouble();

	resX = TeConvertUnits(resX, enumCurUnit, enumLayertUnit);
	resY = TeConvertUnits(resY, enumCurUnit, enumLayertUnit);

	TeCoord2D ll(xLLLineEdit->text().toDouble(),yLLLineEdit->text().toDouble());
	TeCoord2D ur(xURLineEdit->text().toDouble(),yURLineEdit->text().toDouble());
	TeBox searchBox(ll,ur);
	if (!searchBox.isValid())
	{
		QMessageBox::warning( this, tr("Warning"), tr("Invalid box coordinates!"));
		return;
	}

	float lWidth = searchBox .width();
	float lHeight = searchBox .height();

	if (resX < 0. || resX >= lWidth)
	{
		QString msg = tr("Resolution in X must be > 0 and <") + " ";
		msg += Te2String(lWidth,4).c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}

	if (resY < 0. || resY >= lHeight)
	{
		QString msg = tr("Resolution in Y must be > 0 and <") + " ";
		msg += Te2String(lHeight,4).c_str();
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}

	TeBox newBox = adjustToCut(searchBox,resX,resY);
	double x1,x2,y1,y2;
	x1 = newBox.lowerLeft().x();
	y1 = newBox.lowerLeft().y();
	x2 = newBox.upperRight().x();
	y2 = newBox.upperRight().y();

	int maxcols, maxlines;
	maxcols = (int)((y2-y1)/resY);
	maxlines = (int)((x2-x1)/resX);

	QString cols;
	cols.setNum(maxcols);

	QString lines;
	lines.setNum(maxlines);

	linesLineEdit->setText(cols);
	columnsLineEdit->setText(lines);
}
