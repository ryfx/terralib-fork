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
#include <qregexp.h>
#include <TeCoordAlgorithms.h>

void InsertPoint::init()
{
	help_ = 0;

	layerHasStatic_ = false;
	layerHasEvent_ = false;
	layerHasTemporal_ = false;
	themeHasStatic_ = false;
	themeHasEvent_ = false;
	themeHasTemporal_ = false;
	themeHasExternal_ = false;
}

bool InsertPoint::init( TeCoord2D pt )
{
	mainWindow_= (TerraViewBase*)qApp->mainWidget();
	canvas_ = mainWindow_->getCanvas();
	theme_ = mainWindow_->currentTheme();
	if(theme_->getTheme()->type() != TeTHEME)
		return true;

	grid_ = mainWindow_->getGrid();
	TeLayer* layer = ((TeTheme*)theme_->getTheme())->layer();
	db_ = layer->database();
	p_ = pt;
	TeBox b(p_, p_);
	canvas_->mapCWtoDW(b);
	p_ = b.lowerLeft();

	unsigned int i;
	TeAttrTableVector themeTablesVector; 
	((TeTheme*)theme_->getTheme())->getAttTables(themeTablesVector);

	TeAttrTableVector layerTablesVector;
	if(layer->getAttrTables(layerTablesVector))
	{
		for(i=0; i<layerTablesVector.size(); i++)
		{
			if(layerTablesVector[i].tableType() == TeAttrStatic)
				layerHasStatic_ = true;
			else if(layerTablesVector[i].tableType() == TeAttrEvent)
				layerHasEvent_ = true;
			else if(layerTablesVector[i].tableType() == TeFixedGeomDynAttr)
				layerHasTemporal_ = true;
		}
	}
	else
	{
		QMessageBox::critical(this, tr("Error"),
			tr("Fail to get the infolayer tables!"));
		return false;
	}

	for(i=0; i<themeTablesVector.size(); i++)
	{
		if(themeTablesVector[i].tableType() == TeAttrStatic)
		{
			themeHasStatic_ = true;
			staticTableVec_.push_back(themeTablesVector[i]);
		}
		else if(themeTablesVector[i].tableType() == TeAttrEvent)
		{
			themeHasEvent_ = true;
			eventTable_ = themeTablesVector[i];
		}
		else if(themeTablesVector[i].tableType() == TeFixedGeomDynAttr)
		{
			themeHasTemporal_ = true;
			temporalTable_ = themeTablesVector[i];
		}
		else if(themeTablesVector[i].tableType() == TeAttrExternal)
		{
			themeHasExternal_ = true;
			externalTableVec_.push_back(themeTablesVector[i]);
		}
	}

	QString error;
	if(themeHasExternal_)
	{
		error = tr("Remove the external tables, and try again!");
		QMessageBox::warning(this, tr("Warning"), error);
		return false;
	}

	if(themeHasTemporal_)
	{
		objTemporalInstanceTextLabel->setEnabled(true);
		objectTemporalLineEdit->setEnabled(true);
	}
	else
	{
		objTemporalInstanceTextLabel->setEnabled(false);
		objectTemporalLineEdit->setEnabled(false);
	}

	TeDatabasePortal* portal = db_->getPortal();
	TeTable tt;
	if(themeHasStatic_)
		tt = staticTableVec_[0];
	else if(themeHasEvent_)
		tt = eventTable_;
	else if(themeHasTemporal_)
		tt = temporalTable_;

	string maxId;
	string selMax = "SELECT MAX(" + tt.linkName() + ") FROM " + tt.name();
	if(portal->query(selMax) && portal->fetchRow())
		maxId = portal->getData(0);
	else
	{
		QMessageBox::critical(this, tr("Error"), portal->errorMessage().c_str());
		delete portal;
		return false;
	}

	if(maxId.empty())
		maxId = "A";

	string newId = maxId;
	char cc[2];
	cc[1] = 0;
	const char* c = maxId.c_str() + maxId.size() - 1;
	if(*c <= 0x39)
	{
		if(*c < 0x39)
			cc[0] = (*c) + 1;
		else
			cc[0] = 0x41;
	}
	else if(*c <= 0x5A)
	{
		if(*c < 0x5A)
			cc[0] = (*c) + 1;
		else
			cc[0] = 0;
	}
	else if(*c <= 0x7A)
	{
		if(*c < 0x7A)
			cc[0] = (*c) + 1;
		else
			cc[0] = 0;
	}

	if(cc[0] == 0)
		newId.append("0");
	else
	{
		newId = newId.substr(0, newId.size()-1);
		newId.append(cc);
	}			

	objectIdLineEdit->setText(newId.c_str());

	if(themeHasTemporal_)
	{
		tt = temporalTable_;
		string maxTId;
		selMax = "SELECT MAX(" + tt.uniqueName() + ") FROM " + tt.name();
		portal->freeResult();
		if(portal->query(selMax) && portal->fetchRow())
			maxTId = portal->getData(0);
		else
		{
			QMessageBox::critical(this, tr("Error"), portal->errorMessage().c_str());
			delete portal;
			return false;
		}

		string newTId = maxTId;
		char cc[2];
		cc[1] = 0;
		const char* c = maxTId.c_str() + maxTId.size() - 1;
		if(*c <= 0x39)
		{
			if(*c < 0x39)
				cc[0] = (*c) + 1;
			else
				cc[0] = 0x41;
		}
		else if(*c <= 0x5A)
		{
			if(*c < 0x5A)
				cc[0] = (*c) + 1;
			else
				cc[0] = 0;
		}
		else if(*c <= 0x7A)
		{
			if(*c < 0x7A)
				cc[0] = (*c) + 1;
			else
				cc[0] = 0;
		}

		if(cc[0] == 0)
			newTId.append("0");
		else
		{
			newTId = newTId.substr(0, newTId.size()-1);
			newTId.append(cc);
		}			

		objectTemporalLineEdit->setText(newTId.c_str());
	}
	delete portal;

	if(coordButtonGroup->selected() == geographicalRadioButton)
	{
		plane_ = false;
		layer->projection()->setDestinationProjection(layer->projection());
		TeCoord2D pt(layer->projection()->PC2LL(p_));

		bool neg = false;
		int grau = (int)(pt.x() * TeCRD);
		if(pt.x() < 0)
		{
			grau *= -1;
			pt.x_ *= -1;
			neg = true;
		}
		double d = (pt.x() * TeCRD - grau) * 60.;
		int min = (int)d;
		double sec = (d - min) * 60.;

		string x;
		if(neg)
			x = "w " + Te2String(grau, 0) + " " + Te2String(min, 0) + " " + Te2String(sec, 3);
		else
			x = "e " + Te2String(grau, 0) + " " + Te2String(min, 0) + " " + Te2String(sec, 3);

		neg = false;
		grau = (int)(pt.y() * TeCRD);
		if(pt.y() < 0)
		{
			grau *= -1;
			pt.y_ *= -1;
			neg = true;
		}
		d = (pt.y() * TeCRD - grau) * 60.;
		min = (int)d;
		sec = (d - min) * 60.;

		string y;
		if(neg)
			y = "s " + Te2String(grau, 0) + " " + Te2String(min, 0) + " " + Te2String(sec, 3);
		else
			y = "n " + Te2String(grau, 0) + " " + Te2String(min, 0) + " " + Te2String(sec, 3);

		
		xLineEdit->setText(x.c_str());
		yLineEdit->setText(y.c_str());
	}
	else
	{
		plane_ = true;
		xLineEdit->setText(Te2String(p_.x(),4).c_str());
		yLineEdit->setText(Te2String(p_.y(),4).c_str());
	}
	return true;
}



void InsertPoint::coordButtonGroup_clicked( int i)
{
	TeLayer* layer = ((TeTheme*)theme_->getTheme())->layer();

	if (i == 0)	// Plane Coordinates
	{
		if(plane_ == false)
		{
			xTextLabel->setText("X:");
			yTextLabel->setText("Y:");
			double vx, vy;
			QString str = xLineEdit->text();
			longValidator(str, vx);
			str = yLineEdit->text();
			latValidator(str, vy);
			TeCoord2D pt(vx*TeCDR, vy*TeCDR);
			pt = layer->projection()->LL2PC(pt);
			
			xLineEdit->setText(Te2String(pt.x(),2).c_str());
			yLineEdit->setText(Te2String(pt.y(),2).c_str());
		}
		plane_ = true;
	}
	else //Geographical Coordinates
	{
		if(plane_)
		{
			xTextLabel->setText("Long:");
			yTextLabel->setText("Lat:");
			double vx = xLineEdit->text().toDouble();
			double vy = yLineEdit->text().toDouble();
			TeCoord2D pt(vx, vy);
			layer->projection()->setDestinationProjection(layer->projection());
			pt = layer->projection()->PC2LL(pt);

			bool neg = false;
			int grau = (int)(pt.x() * TeCRD);
			if(pt.x() < 0)
			{
				grau *= -1;
				pt.x_ *= - 1;
				neg = true;
			}
			double d = (pt.x() * TeCRD - grau) * 60.;
			int min = (int)d;
			double sec = (d - min) * 60.;

			string x;
			if(neg)
				x = "w " + Te2String(grau, 0) + " " + Te2String(min, 0) + " " + Te2String(sec, 3);
			else
				x = "e " + Te2String(grau, 0) + " " + Te2String(min, 0) + " " + Te2String(sec, 3);

			neg = false;
			grau = (int)(pt.y() * TeCRD);
			if(pt.y() < 0)
			{
				grau *= -1;
				pt.y_ *= -1;
				neg = true;
			}
			d = (pt.y() * TeCRD - grau) * 60.;
			min = (int)d;
			sec = (d - min) * 60.;

			string y;
			if(neg)
				y = "s " + Te2String(grau, 0) + " " + Te2String(min, 0) + " " + Te2String(sec, 3);
			else
				y = "n " + Te2String(grau, 0) + " " + Te2String(min, 0) + " " + Te2String(sec, 3);

			
			xLineEdit->setText(x.c_str());
			yLineEdit->setText(y.c_str());
		}
		plane_ = false;
	}
}

void InsertPoint::okPushButton_clicked()
{
	QString erro;
	TeLayer* layer = ((TeTheme*)theme_->getTheme())->layer();

	if(objectIdLineEdit->text().isEmpty())
		erro = tr("Type object identifier!");
	if(erro.isEmpty() && themeHasTemporal_)
	{
		if(objectTemporalLineEdit->text().isEmpty())
			erro = tr("Type object temporal instance!");
	}

	if(erro.isEmpty() == false)
	{
		QMessageBox::warning(this, tr("Warning"), erro);
		return;
	}

	string newId = objectIdLineEdit->text().latin1();
	string newTId;
	if(themeHasTemporal_)
		newTId = objectTemporalLineEdit->text().latin1();

	QString qx = xLineEdit->text();
	QString qy = yLineEdit->text();
	QString qerro;
	double	xx, yy;
	if(plane_ == false)
	{
		if(longValidator(qx, xx) == false)
			qerro = tr("x coordinate error");
		if(latValidator(qy, yy) == false)
		{
			if(qerro.isEmpty() == false)
				qerro += " " + tr("and y coordinate error");
			else
			qerro = tr("y coordinate error");
		}
	}
	else
	{
		if(xValidator(qx, xx) == false)
			qerro = tr("x coordinate error");
		if(yValidator(qy, yy) == false)
		{
			if(qerro.isEmpty() == false)
				qerro += " " + tr("and y coordinate error");
			else
			qerro = tr("y coordinate error");
		}
	}

	if(qerro.isEmpty() == false)
	{
		QMessageBox::warning(this, tr("Warning"), qerro);
		return;
	}

	if(plane_)
	{
		p_.x_ = xx;
		p_.y_ = yy;
	}
	else
	{
		TeCoord2D pt(xx*TeCDR, yy*TeCDR);
		p_ = layer->projection()->LL2PC(pt);
	}

	TePointSet pointSet;
	TePoint	point;
	point.add(p_);
	point.objectId(newId);
	pointSet.add(point);
	layer->addPoints(pointSet);

	grid_->removePointingColorSlot();
	string erroIns = insertObjectIntoCollection(((TeTheme*)theme_->getTheme()), p_, newId, newTId);
	if(erroIns.empty() == false)
	{
		if(erroIns != "There is already")
		{
			QMessageBox::critical(this, tr("Error"), erroIns.c_str());
			return;
		}
	}
	else
	{
		grid_->getPortal()->freeResult();
		grid_->insertObjectIntoCollection(newId, newTId);
		grid_->updatePortalContents();

		set<string> uidSet, oidSet;
		oidSet.insert(newId);
		if(newTId.empty() == false)
			uidSet.insert(newTId);
		grid_->putColorOnGrid(uidSet, oidSet, "newPointing"); 
//		grid_->goToLastLine();
		grid_->setVerticalEdition(false);
	}

	DisplayWindow* display = mainWindow_->getDisplayWindow();
	set<string> drawSet;
	drawSet.insert(newId);
	display->plotData(theme_, drawSet);

	hide();
}




bool InsertPoint::longValidator( const QString str, double & val )
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
			val = grauDec;
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
		val = grauDec;
		return true;
	}
	return false;
}


bool InsertPoint::latValidator( const QString str, double & val )
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
			val = grauDec;
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
		val = grauDec;
		return true;
	}
	return false;
}


void InsertPoint::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("insertPoint.htm");
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


bool InsertPoint::xValidator( QString str, double & val )
{
	int mat;
	QRegExp exp("^[+-]?\\d\\d?\\d?\\.?[0-9]?" ); 
	if((mat=exp.match( str )) != -1 )
	{
		val = str.toDouble();
		return true;
	}
	return false;
}


bool InsertPoint::yValidator( QString str, double & val )
{
	int mat;
	QRegExp exp("^[+-]?\\d\\d?\\d?\\.?[0-9]?" ); 
	if((mat=exp.match( str )) != -1 )
	{
		val = str.toDouble();
		return true;
	}
	return false;
}


void InsertPoint::show()
{
	if(theme_->getTheme()->type() == TeTHEME)
		QDialog::show();
	else
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		reject();
		hide();
	}
}
