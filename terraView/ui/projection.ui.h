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

extern const int NUM_PROJ;

#include <datum.h>

#include <cstdlib>

void ProjectionWindow::init()
{
	help_ = 0;

	int i;
	projection_ = 0;

	const char** teProjInfo = TeGetProjInfo();
	for (i = 0; i < NUM_PROJ; ++i)
		projectionComboBox->insertItem(teProjInfo[i*9]);

	fillDatumList();

	unitComboBox->clear();
	unitComboBox->insertItem("Meters");
	unitComboBox->insertItem("Kilometers");
	unitComboBox->insertItem("Centimeters");
	unitComboBox->insertItem("Millimeters");
	unitComboBox->insertItem("Feet");
	unitComboBox->insertItem("Inches");
	unitComboBox->insertItem("DecimalDegrees");
	
	//unitComboBox->setEnabled(false);
	for (i = 1; i < 61; ++i)
	{
        QString zone = tr("Zone") + QString(" %1").arg(i);
	    ZoneComboBox->insertItem(zone);
	 }
    originLongLineEdit->clear();
	UtmOffsetX_ = 500000.0;
	UtmOffsetY_ = 10000000.0;
	ZoneComboBox->setEnabled(false);
	ZoneComboBox->setCurrentItem(0);

	advancedButtonGroup->hide();
}

void ProjectionWindow::projectionComboBox_activated( const QString &projName )
{
	hemisphereButtonGroup->setEnabled(false);
	if (projName == "UTM" || projName == "PolarStereographic")
		hemisphereButtonGroup->setEnabled(true);

	QString aux;
	if (projName != "NoProjection")
	{
		datumComboBox->setEnabled(true);
		parametersGroupBox->setEnabled(true);
	}
	else
	{
		datumComboBox->setEnabled(false);
		datumComboBox->setCurrentText("Spherical");
		clearParametersGroup();
	}

	TeProjInfo pjInfo = TeProjectionInfo(projName.latin1());

	if ( pjInfo.hasLon0 )
	{
		originLongTextLabel->setEnabled(true);
		originLongLineEdit->setEnabled(true);
		if (projName == "UTM")
		{
			aux.sprintf("%.3f",UtmOffsetY_);
			offsetYLineEdit->setText(aux);
			aux.sprintf("%.3f",UtmOffsetX_);
			offsetXLineEdit->setText(aux);
			ZoneComboBox->setEnabled(true);
			ZoneComboBox_activated(23);	
		}
		else
		{
			ZoneComboBox->setEnabled(false);
			offsetYLineEdit->clear();
			offsetXLineEdit->clear();
		}
	}
	else
	{
		ZoneComboBox->setEnabled(false);
		originLongTextLabel->setEnabled(false);
		originLongLineEdit->setEnabled(false);
		originLongLineEdit->clear();
	}		

	if ( pjInfo.hasLat0 )
	{
		originLatTextLabel->setEnabled(true);
		originLatLineEdit->setEnabled(true);
	}
	else
	{
		originLatTextLabel->setEnabled(false);
		originLatLineEdit->setEnabled(false);
		originLatLineEdit->clear();
		if (projName == "UTM")
			originLatLineEdit->setText("0.000000");
	}

	if ( pjInfo.hasStlat1 )
	{
		stdParallel1TextLabel->setEnabled(true);
		stdParallel1LineEdit->setEnabled(true);
	}
	else
	{
		stdParallel1TextLabel->setEnabled(false);
		stdParallel1LineEdit->setEnabled(false);
		stdParallel1LineEdit->clear();
	}
	
	if ( pjInfo.hasStlat2 )
	{
		stdParallel2TextLabel->setEnabled(true);
		stdParallel2LineEdit->setEnabled(true);
	}
	else
	{
		stdParallel2TextLabel->setEnabled(false);
		stdParallel2LineEdit->setEnabled(false);
		stdParallel2LineEdit->clear();
	}

	if ( pjInfo.hasScale )
	{
		scaleTextLabel->setEnabled(true);
		scaleLineEdit->setEnabled(true);
		if ( projName == "UTM" )
		{
			scaleLineEdit->setText("0.9996");
			scaleLineEdit->setEnabled(false);
		}
	}
	else
	{
		scaleTextLabel->setEnabled(false);
		scaleLineEdit->setEnabled(false);
		scaleLineEdit->clear();
	}

	if ( pjInfo.hasUnits)
	{
		//unitComboBox->setEnabled(true);
		if ( projName == "LatLong")
			unitComboBox->setCurrentText("DecimalDegrees");
		else
			unitComboBox->setCurrentText("Meters");

		currentUnit_ = unitComboBox->currentText().latin1();
	}
	else
		unitComboBox->setEnabled(false);

	if (pjInfo.hasOffx)
		offsetXLineEdit->setEnabled(true);
	else
	{
		offsetXLineEdit->clear();
		offsetXLineEdit->setEnabled(false);
	}

	if (pjInfo.hasOffy)
		offsetYLineEdit->setEnabled(true);
	else
	{
		offsetYLineEdit->clear();
		offsetYLineEdit->setEnabled(false);
	}

}



// This method fills the interface with the values of a given projection
void ProjectionWindow::loadProjection( TeProjection *projection )
{
	string& projectionName = projection->name();
	projectionComboBox->setCurrentText(projectionName.c_str());
	if (projection->name() != "NoProjection")
	{
		datumComboBox->setCurrentText(projection->datum().name().c_str());
		datumComboBox->setEnabled(true);
		parametersGroupBox->setEnabled(true);
	}
	else
	{
		datumComboBox->setCurrentText("Spherical");
		datumComboBox->setEnabled(false);
		clearParametersGroup();
	}

	QString aux;
	TeProjInfo pjInfo = TeProjectionInfo(projectionName);
	if (pjInfo.hasUnits)
	{
		unitComboBox->insertItem(QString(projection->units().c_str()));
		unitComboBox->setCurrentText(projection->units().c_str());
		currentUnit_ = unitComboBox->currentText().latin1();
		//unitComboBox->setEnabled(true);
	}

	if ( pjInfo.hasLon0 )
	{
		originLongTextLabel->setEnabled(true);
		originLongLineEdit->setEnabled(true);
		aux.sprintf("%.6f",projection->lon0()*TeCRD);
		originLongLineEdit->setText(aux);
		if (projection->name() == "UTM")
		{
			ZoneComboBox->setEnabled(true);
			ZoneComboBox->setCurrentItem((int)((projection->lon0()*TeCRD+183.0)/6.0)-1);
		}
		else
			ZoneComboBox->setEnabled(false);
	}
	else
	{
		ZoneComboBox->setEnabled(false);
		originLongTextLabel->setEnabled(false);
		originLongLineEdit->setEnabled(false);
		originLongLineEdit->clear();
	}		

	if ( pjInfo.hasLat0 )
	{
		originLatTextLabel->setEnabled(true);
		originLatLineEdit->setEnabled(true);
		aux.sprintf("%.6f",projection->lat0()*TeCRD);
		originLatLineEdit->setText(aux);
	}
	else
	{
		originLatTextLabel->setEnabled(false);
		originLatLineEdit->setEnabled(false);
		originLatLineEdit->clear();
		if (projectionName == "UTM")
			originLatLineEdit->setText("0.0");
	}

	if ( pjInfo.hasStlat1 )
	{
		stdParallel1TextLabel->setEnabled(true);
		stdParallel1LineEdit->setEnabled(true);
		aux.sprintf("%.6f",projection->stLat1()*TeCRD);
		stdParallel1LineEdit->setText(aux);
	}
	else
	{
		stdParallel1TextLabel->setEnabled(false);
		stdParallel1LineEdit->setEnabled(false);
		stdParallel1LineEdit->clear();
	}
	
	if ( pjInfo.hasStlat2 )
	{
		stdParallel2TextLabel->setEnabled(true);
		stdParallel2LineEdit->setEnabled(true);
		aux.sprintf("%.6f",projection->stLat2()*TeCRD);
		stdParallel2LineEdit->setText(aux);
	}
	else
	{
		stdParallel2TextLabel->setEnabled(false);
		stdParallel2LineEdit->setEnabled(false);
		stdParallel2LineEdit->clear();
	}

	if ( pjInfo.hasScale )
	{
		scaleTextLabel->setEnabled(true);
		scaleLineEdit->setEnabled(true);
		aux.sprintf("%.6f",projection->scale());
		scaleLineEdit->setText(aux);

	}
	else
	{
		scaleTextLabel->setEnabled(false);
		scaleLineEdit->setEnabled(false);
		scaleLineEdit->clear();
	}

	if (pjInfo.hasOffx)
	{
		aux.sprintf("%.1f",projection->offX());
		offsetXLineEdit->setText(aux);
	}
	else
	{
		offsetXLineEdit->clear();
		offsetXLineEdit->setEnabled(false);
	}

	if (pjInfo.hasOffy)
	{
		aux.sprintf("%.1f",projection->offY());
		offsetYLineEdit->setText(aux);
	}
	else
	{
		offsetYLineEdit->clear();
		offsetYLineEdit->setEnabled(false);
	}	
	hemisphereButtonGroup->setButton(projection->hemisphere());
}



bool ProjectionWindow::longValidator( const QString str, double & val )
{
	int mat;
	QStringList myList;
	QRegExp exp1("^[oOwWeElL][ ]\\d\\d?\\d?[ ]?\\d\\d?[ ]?\\d?\\d?\\.?[0-9]?" );
	QRegExp exp2("^[oOwWeElL][;]\\d\\d?\\d?[;]?\\d\\d?[;]?\\d?\\d?\\.?[0-9]?" ); 
	QRegExp exp3("^[oOwWeElL][:]\\d\\d?\\d?[:]?\\d\\d?[:]?\\d?\\d?\\.?[0-9]?" ); 
	QRegExp exp4("^[+-]?\\d\\d?\\d?\\.?[0-9]?" ); 

	double grauDec;

    if ( (mat=exp1.match( str )) != -1 )
	{
		myList = QStringList::split(" ",str);
	}
	else if ((mat=exp2.match( str )) != -1 )
	{
		myList = QStringList::split(";",str);
	}
	else if ((mat=exp3.match( str )) != -1 )
	{
		myList = QStringList::split(":",str);
	}
	else if ((mat=exp4.match( str )) != -1 )
	{
		grauDec = str.toDouble();
		if ((grauDec >= -180.0 && grauDec <=180.0))
		{
			val = grauDec * TeCDR;
			return true;
		}
		else
			return false;
	}
	else
		return false;

	int nelementos = myList.count();
	short graus, minutos = 0;
	float segundos = 0.0;
	
	graus = myList[1].toShort();

	if (nelementos > 2)
		minutos = myList[2].toShort();
	
	if (nelementos > 3)
		segundos = myList[3].toFloat();

	char* lado = strdup(myList[0]);
	if (TeLongDMS2DD(lado[0],graus, minutos, segundos, grauDec))
	{
		val = grauDec * TeCDR;
		return true;
	}
	else
		return false;
}


bool ProjectionWindow::latValidator( const QString str, double & val )
{
	int mat;
	QStringList myList;
	QRegExp exp1("^[nNsS][ ]\\d\\d?[ ]?\\d?[ ]?\\d?\\d?\\.?[0-9]?" );
	QRegExp exp2("^[nNsS][;]\\d\\d?[;]?\\d?[;]?\\d?\\d?\\.?[0-9]?" ); 
	QRegExp exp3("^[nNsS][:]\\d\\d?[:]?\\d?[:]?\\d?\\d?\\.?[0-9]?" ); 
	QRegExp exp4("^[+-]?\\d\\d?\\d?\\.?[0-9]?" ); 

	double grauDec;
	val = 0;

    if ( (mat=exp1.match( str )) != -1 )
	{
		myList = QStringList::split(" ",str);
	}
	else if ((mat=exp2.match( str )) != -1 )
	{
		myList = QStringList::split(";",str);
	}
	else if ((mat=exp3.match( str )) != -1 )
	{
		myList = QStringList::split(":",str);
	}
	else if ((mat=exp4.match( str )) != -1 )
	{
		grauDec = str.toDouble();
		if ((grauDec >= -90.0 && grauDec <=90.0))
		{
			val = grauDec * TeCDR;
			return true;
		}
		else
			return false;
	}
	else
	{
		return false;
	}
	int nelementos = myList.count();
	short graus, minutos = 0;
	float segundos = 0.0;
	
	graus = myList[1].toShort();

	if (nelementos > 2)
		minutos = myList[2].toShort();
	
	if (nelementos > 3)
		segundos = myList[3].toFloat();

	char* lado = strdup(myList[0]);
	if (TeLatDMS2DD(lado[0],graus, minutos, segundos, grauDec))
	{
		val = grauDec * TeCDR;
		return true;
	}
	else
		return false;
}


void ProjectionWindow::okPushButton_clicked()
{
	bool projOK = true;
	double plon0 = 0., plat0 = 0.;
	double poffx = 0., poffy = 0.;
	double pstlat1 = 0., pstlat2 = 0.;
	double	pscale = 1.;
	double val;
	TeHemisphere phem;
	TeProjectionParams pa;
	QString txt;

	string pname = projectionComboBox->currentText().latin1();
	TeProjInfo pjInfo = TeProjectionInfo (pname);

	string datumname = datumComboBox->currentText().latin1();
	TeDatum pdatum = TeDatumFactory :: make(datumname);

	const string punits = unitComboBox->currentText().latin1();

	if (northRadioButton->isOn())
		phem = TeNORTH_HEM;
	else
		phem = TeSOUTH_HEM;

	if (pjInfo.hasLon0)
	{
		if (longValidator(originLongLineEdit->text(), val))
			plon0 = val;
		else
		{
			projOK = false;
			txt = tr("Origin Longitude") + " ";
		}		
	}

	if (pjInfo.hasLat0)
	{
		if (latValidator(originLatLineEdit->text(), val))
			plat0 = val;
		else
		{
			projOK = false;
			txt += tr("Origin Latitude");
		}
	}

	if (pjInfo.hasOffx)
		poffx = atof(offsetXLineEdit->text().latin1());

	if (pjInfo.hasOffy)
		poffy = atof(offsetYLineEdit->text().latin1());

	if (pjInfo.hasStlat1)
	{
		if (latValidator(stdParallel1LineEdit->text(), val))
			pstlat1 = val;
		else
		{
			projOK = false;
			txt += tr("Standard Parallel 1");
		}
	}

	if (pjInfo.hasStlat2)
	{
		if (latValidator(stdParallel2LineEdit->text(), val))
			pstlat2 = val;
		else
		{
			projOK = false;
			txt += tr("Standard Parallel 2");
		}
	}

	if (pjInfo.hasScale)
		pscale = atof(scaleLineEdit->text().latin1());

	if (projOK)
	{
		pa.name = pname;	   
		pa.datum = pdatum;     
		pa.lon0 = plon0;	
		pa.lat0 = plat0;	
		pa.offx = poffx;	   
		pa.offy = poffy;	   
		pa.stlat1 = pstlat1;	   
		pa.stlat2 = pstlat2;     
		pa.units = punits;      
		pa.scale = pscale;      
		pa.hemisphere = phem; 

		projection_ = TeProjectionFactory::make(pa);
	}
	else
	{
		QString str = tr("Invalid value for the following parameter:") + " " + txt;
		QMessageBox::critical(this, tr("Error"), tr(str));
		return;
	}
	accept();
}



TeProjection* ProjectionWindow::getProjection()
{
	return projection_;
}


void ProjectionWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("projection.htm");
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




void ProjectionWindow::originLongLineEdit_lostFocus()
{
 	double val;
	if (longValidator(originLongLineEdit->text(), val))
		ZoneComboBox->setCurrentItem(static_cast<int>(((val*TeCRD+183.0)/6.0)-1));
}


void ProjectionWindow::ZoneComboBox_activated( int z )
{
    originLongLineEdit->clear();
	float lo = -183. + 6.*(z+1);
	QString aux;
	aux.setNum(lo,'f',2);
	originLongLineEdit->setText(aux);
}


void ProjectionWindow::clearParametersGroup()
{
	offsetYLineEdit->clear();
	offsetXLineEdit->clear();
	originLatLineEdit->clear();
	originLongLineEdit->clear();
	stdParallel1LineEdit->clear();
	stdParallel1LineEdit->clear();
	scaleLineEdit->clear();
}


void ProjectionWindow::hemisphereButtonGroup_clicked( int i)
{
    if (projectionComboBox->currentText() != "UTM")
	    return;
    if (i==0)
		offsetYLineEdit->setText("0.0");
    else
		offsetYLineEdit->setText("10000000.0");	
}


void ProjectionWindow::unitComboBox_activated( const QString & qUnit)
{
	if(qUnit.isEmpty())
	{
		currentUnit_ = "";
		return;
	}
	if(currentUnit_.empty())
	{
		return;
	}

	if(offsetXLineEdit->text().isEmpty() == true)
	{			
		return;	
	}
	if(offsetYLineEdit->text().isEmpty() == true)
	{			
		return;	
	}

	std::string newUnit = qUnit.latin1();

	if(currentUnit_ != newUnit)
	{
		double offsetX = 0.;
		double offsetY = 0.;

		bool ok = false;
		offsetX = offsetXLineEdit->text().toDouble(&ok);

		if(ok == false)
		{
			return;
		}

		ok = false;
		offsetY = offsetYLineEdit->text().toDouble(&ok);

		if(ok == false)
		{
			return;
		}

		TeUnits intCurrentUnit = TeGetUnit(currentUnit_);
		TeUnits intNewUnit = TeGetUnit(newUnit);

		offsetX = TeConvertUnits(offsetX, intCurrentUnit, intNewUnit);
		offsetY = TeConvertUnits(offsetY, intCurrentUnit, intNewUnit);

		offsetXLineEdit->setText(Te2String(offsetX).c_str());
		offsetYLineEdit->setText(Te2String(offsetY).c_str());

		currentUnit_ = qUnit.latin1();
	}
}

void ProjectionWindow::newDatumPushButton_clicked()
{
	DatumWindow dw(this);
	dw.init();
	dw.exec();
	fillDatumList();
}

void ProjectionWindow::advancedCheckBox_clicked()
{
	if(advancedCheckBox->isChecked())
		advancedButtonGroup->show();
	else
		advancedButtonGroup->hide();
}

void ProjectionWindow::fillDatumList()
{
	TeDatumMap& datumMap = TeDatumFactory::getDatumMap();
	TeDatumMap::iterator it;
	QString currentDatum = datumComboBox->currentText();
	datumComboBox->clear();
	for(it = datumMap.begin(); it != datumMap.end(); ++it)
		datumComboBox->insertItem(it->second.name().c_str());
	
	if(!currentDatum.isEmpty())
		datumComboBox->setCurrentText(currentDatum);
}


void ProjectionWindow::editDatumPushButton_clicked()
{
	std::string datumName = datumComboBox->currentText().latin1();
	TeDatum datum = TeDatumFactory::make(datumName);

	DatumWindow dw(this);
	dw.init();
	dw.loadDatum(datum);
	dw.setEditMode();
	dw.exec();
}
