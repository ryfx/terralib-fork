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
#include <terraViewBase.h>
#include <display.h>
#include <qfontdatabase.h>
#include <qstringlist.h>
#include <qcolordialog.h>
#include <qmessagebox.h>


void GeographicalGrid::init()
{
	help_ = 0;

	error_ = false;
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	db_ = mainWindow_->currentDatabase();
	display_ = mainWindow_->getDisplayWindow();
	view_ = mainWindow_->currentView();

	if(view_ == 0 || db_ == 0)
		return;

	int		 i, j;
	bool	 deltaCalc;
	bool	 txLatHorizontal;
	bool	 txLonHorizontal;
	bool	 vis, leftVis, rightVis, topVis, bottomVis;
	int		 xGrausOffset;
	int		 xMinsOffset;
	double	 xSecsOffset;
	int		 yGrausOffset;
	int		 yMinsOffset;
	double	 ySecsOffset;
	int		 precision;


	TeDatabasePortal* portal = db_->getPortal();
	string sel = "SELECT visible FROM app_geogrid WHERE view_id = " + Te2String(view_->id());
	if(portal->query(sel))
	{
		if(portal->fetchRow())
		{
			vis = portal->getInt(0);
			if(vis == false)
			{
				delete portal;
				error_ = true;
				return;
			}
		}
	}

	sel = "SELECT * FROM app_geogrid WHERE view_id = ";
	sel += Te2String(view_->id());
	portal->freeResult();
	if(portal->query(sel) && portal->fetchRow())
	{
		visible_ = (bool)(portal->getInt(8));
		if(visible_ == false)
		{
			portal->freeResult();
			delete portal;
			return;
		}

		string family = portal->getData(1);
		int red = portal->getInt(2);
		int green = portal->getInt(3);
		int blue = portal->getInt(4);
		bool bold = (bool)(portal->getInt(5));
		bool italic = (bool)(portal->getInt(6));
		int size = portal->getInt(7);

		txVisual_.family(family);
		TeColor cor(red, green, blue);
		txVisual_.color(cor);
		txVisual_.bold(bold);
		txVisual_.italic(italic);
		txVisual_.size(size);

		leftVis = (bool)(portal->getInt(9));
		rightVis = (bool)(portal->getInt(10));
		topVis = (bool)(portal->getInt(11));
		bottomVis = (bool)(portal->getInt(12));
		lineCor_ = QColor(portal->getInt(13), portal->getInt(14), portal->getInt(15));
		deltaCalc = (bool)(portal->getInt(16));
		deltaGraus_ = portal->getInt(17);
		deltaMins_ = portal->getInt(18);
		deltaSecs_ = portal->getDouble(19);
		xGrausOffset = portal->getInt(20);
		xMinsOffset = portal->getInt(21);
		xSecsOffset = portal->getDouble(22);
		yGrausOffset = portal->getInt(23);
		yMinsOffset = portal->getInt(24);
		ySecsOffset = portal->getDouble(25);
		txLatHorizontal = (bool)(portal->getInt(26));
		txLonHorizontal = (bool)(portal->getInt(27));
		precision =  portal->getInt(28);
	}
	else
	{
		delete portal;
		error_ = true;
		return;
	}

	delete portal;

// get colors
	TeColor color = txVisual_.color();
	QColor txcor(color.red_, color.green_, color.blue_);
	textColorPushButton->setPaletteBackgroundColor(txcor);
	textColorPushButton->repaint();

	lineColorPushButton->setPaletteBackgroundColor(lineCor_);
	lineColorPushButton->repaint();

// get family
	i = 0;
	QString cfamily = txVisual_.family().c_str();
    QFontDatabase fdb;
    QStringList families = fdb.families();
    for ( QStringList::Iterator f = families.begin(); f != families.end(); ++f, ++i )
	{
        QString family = *f;
		fontComboBox->insertItem(family);
		if(cfamily == family)
			j = i;
    }
	
	fontComboBox->setCurrentItem(j);

	sizeComboBox->insertItem("6");
	sizeComboBox->insertItem("7");
	sizeComboBox->insertItem("8");
	sizeComboBox->insertItem("9");
	sizeComboBox->insertItem("10");
	sizeComboBox->insertItem("11");
	sizeComboBox->insertItem("12");
	sizeComboBox->insertItem("13");
	sizeComboBox->insertItem("14");
	sizeComboBox->insertItem("15");
	sizeComboBox->insertItem("16");
	sizeComboBox->insertItem("18");
	sizeComboBox->insertItem("20");
	sizeComboBox->insertItem("24");
	sizeComboBox->insertItem("30");

	j = -1;
	for(i=0; i<(int)sizeComboBox->count(); ++i)
	{
		int size = sizeComboBox->text(i).toInt();
		int vsize = txVisual_.size();
		if(size == vsize)
			j = i;
	}

	if(j == -1)
	{
		QString s;
		s.setNum(txVisual_.size());
		sizeComboBox->insertItem(s);
		sizeComboBox->setCurrentItem(sizeComboBox->count()-1);
	}
	else
		sizeComboBox->setCurrentItem(j);

	if(txVisual_.bold())
		boldCheckBox->setChecked(true);
	if(txVisual_.italic())
		italicCheckBox->setChecked(true);

// get visibilities
	if(leftVis)
		leftCheckBox->setChecked(true);
	if(rightVis)
		rightCheckBox->setChecked(true);
	if(topVis)
		topCheckBox->setChecked(true);
	if(bottomVis)
		bottomCheckBox->setChecked(true);

// get offsets
	if(xGrausOffset < 0 || xMinsOffset < 0 || xSecsOffset < 0)
		initialLonWestRadioButton->setChecked(true);
	else
		initialLonEastRadioButton->setChecked(true);

	initialLonDegreeComboBox->insertItem("0");
	initialLonDegreeComboBox->insertItem("1");
	initialLonDegreeComboBox->insertItem("2");
	initialLonDegreeComboBox->insertItem("3");
	initialLonDegreeComboBox->insertItem("4");
	initialLonDegreeComboBox->insertItem("5");
	initialLonDegreeComboBox->insertItem("6");
	initialLonDegreeComboBox->insertItem("10");
	initialLonDegreeComboBox->insertItem("20");
	initialLonDegreeComboBox->insertItem("30");
	initialLonDegreeComboBox->insertItem("40");
	initialLonDegreeComboBox->insertItem("50");

	j = -1;
	for(i=0; i<(int)initialLonDegreeComboBox->count(); ++i)
	{
		int a = initialLonDegreeComboBox->text(i).toInt();
		int b = abs(xGrausOffset);
		if(a == b)
			j = i;
	}

	if(j == -1)
	{
		QString s;
		s.setNum(abs(xGrausOffset));
		initialLonDegreeComboBox->insertItem(s);
		initialLonDegreeComboBox->setCurrentItem(initialLonDegreeComboBox->count()-1);
	}
	else
		initialLonDegreeComboBox->setCurrentItem(j);

	initialLonMinuteComboBox->insertItem("0");
	initialLonMinuteComboBox->insertItem("1");
	initialLonMinuteComboBox->insertItem("2");
	initialLonMinuteComboBox->insertItem("3");
	initialLonMinuteComboBox->insertItem("4");
	initialLonMinuteComboBox->insertItem("5");
	initialLonMinuteComboBox->insertItem("6");
	initialLonMinuteComboBox->insertItem("10");
	initialLonMinuteComboBox->insertItem("20");
	initialLonMinuteComboBox->insertItem("30");
	initialLonMinuteComboBox->insertItem("40");
	initialLonMinuteComboBox->insertItem("50");

	j = -1;
	for(i=0; i<(int)initialLonMinuteComboBox->count(); ++i)
	{
		int a = initialLonMinuteComboBox->text(i).toInt();
		int b = abs(xMinsOffset);
		if(a == b)
			j = i;
	}

	if(j == -1)
	{
		QString s;
		s.setNum(abs(xMinsOffset));
		initialLonMinuteComboBox->insertItem(s);
		initialLonMinuteComboBox->setCurrentItem(initialLonMinuteComboBox->count()-1);
	}
	else
		initialLonMinuteComboBox->setCurrentItem(j);

	initialLonSecondComboBox->insertItem("0");
	initialLonSecondComboBox->insertItem("1");
	initialLonSecondComboBox->insertItem("2");
	initialLonSecondComboBox->insertItem("3");
	initialLonSecondComboBox->insertItem("4");
	initialLonSecondComboBox->insertItem("5");
	initialLonSecondComboBox->insertItem("6");
	initialLonSecondComboBox->insertItem("10");
	initialLonSecondComboBox->insertItem("20");
	initialLonSecondComboBox->insertItem("30");
	initialLonSecondComboBox->insertItem("40");
	initialLonSecondComboBox->insertItem("50");

	j = -1;
	for(i=0; i<(int)initialLonSecondComboBox->count(); ++i)
	{
		int a = initialLonSecondComboBox->text(i).toInt();
		int b = abs((int)xSecsOffset);
		if(a == b)
			j = i;
	}

	if(j == -1)
	{
		QString s;
		s.setNum(fabs(xSecsOffset));
		initialLonSecondComboBox->insertItem(s);
		initialLonSecondComboBox->setCurrentItem(initialLonSecondComboBox->count()-1);
	}
	else
		initialLonSecondComboBox->setCurrentItem(j);

	if(yGrausOffset < 0 || yMinsOffset < 0 || ySecsOffset < 0)
		initialLatSouthRadioButton->setChecked(true);
	else
		initialLatNorthRadioButton->setChecked(true);

	initialLatDegreeComboBox->insertItem("0");
	initialLatDegreeComboBox->insertItem("1");
	initialLatDegreeComboBox->insertItem("2");
	initialLatDegreeComboBox->insertItem("3");
	initialLatDegreeComboBox->insertItem("4");
	initialLatDegreeComboBox->insertItem("5");
	initialLatDegreeComboBox->insertItem("6");
	initialLatDegreeComboBox->insertItem("10");
	initialLatDegreeComboBox->insertItem("20");
	initialLatDegreeComboBox->insertItem("30");
	initialLatDegreeComboBox->insertItem("40");
	initialLatDegreeComboBox->insertItem("50");

	j = -1;
	for(i=0; i<(int)initialLatDegreeComboBox->count(); ++i)
	{
		int a = initialLatDegreeComboBox->text(i).toInt();
		int b = abs(yGrausOffset);
		if(a == b)
			j = i;
	}

	if(j == -1)
	{
		QString s;
		s.setNum(abs(yGrausOffset));
		initialLatDegreeComboBox->insertItem(s);
		initialLatDegreeComboBox->setCurrentItem(initialLatDegreeComboBox->count()-1);
	}
	else
		initialLatDegreeComboBox->setCurrentItem(j);

	initialLatMinuteComboBox->insertItem("0");
	initialLatMinuteComboBox->insertItem("1");
	initialLatMinuteComboBox->insertItem("2");
	initialLatMinuteComboBox->insertItem("3");
	initialLatMinuteComboBox->insertItem("4");
	initialLatMinuteComboBox->insertItem("5");
	initialLatMinuteComboBox->insertItem("6");
	initialLatMinuteComboBox->insertItem("10");
	initialLatMinuteComboBox->insertItem("20");
	initialLatMinuteComboBox->insertItem("30");
	initialLatMinuteComboBox->insertItem("40");
	initialLatMinuteComboBox->insertItem("50");

	j = -1;
	for(i=0; i<(int)initialLatMinuteComboBox->count(); ++i)
	{
		int a = initialLatMinuteComboBox->text(i).toInt();
		int b = abs(yMinsOffset);
		if(a == b)
			j = i;
	}

	if(j == -1)
	{
		QString s;
		s.setNum(abs(yMinsOffset));
		initialLatMinuteComboBox->insertItem(s);
		initialLatMinuteComboBox->setCurrentItem(initialLatMinuteComboBox->count()-1);
	}
	else
		initialLatMinuteComboBox->setCurrentItem(j);

	initialLatSecondComboBox->insertItem("0");
	initialLatSecondComboBox->insertItem("1");
	initialLatSecondComboBox->insertItem("2");
	initialLatSecondComboBox->insertItem("3");
	initialLatSecondComboBox->insertItem("4");
	initialLatSecondComboBox->insertItem("5");
	initialLatSecondComboBox->insertItem("6");
	initialLatSecondComboBox->insertItem("10");
	initialLatSecondComboBox->insertItem("20");
	initialLatSecondComboBox->insertItem("30");
	initialLatSecondComboBox->insertItem("40");
	initialLatSecondComboBox->insertItem("50");

	j = -1;
	for(i=0; i<(int)initialLatSecondComboBox->count(); ++i)
	{
		int a = initialLatSecondComboBox->text(i).toInt();
		int b = abs((int)ySecsOffset);
		if(a == b)
			j = i;
	}

	if(j == -1)
	{
		QString s;
		s.setNum(fabs(ySecsOffset));
		initialLatSecondComboBox->insertItem(s);
		initialLatSecondComboBox->setCurrentItem(initialLatSecondComboBox->count()-1);
	}
	else
		initialLatSecondComboBox->setCurrentItem(j);

// get deltas
	if(deltaCalc)
	{
		deltaDegreeComboBox->insertItem("");
		deltaMinuteComboBox->insertItem("");
		deltaSecondComboBox->insertItem("");
		deltaDegreeComboBox->setCurrentItem(deltaDegreeComboBox->count()-1);
		deltaMinuteComboBox->setCurrentItem(deltaMinuteComboBox->count()-1);
		deltaSecondComboBox->setCurrentItem(deltaSecondComboBox->count()-1);

		intervalCalcAutoCheckBox->setChecked(true);
		intervalGroupBox->setEnabled(false);
	}
	else
	{
		intervalGroupBox->setEnabled(true);

		deltaDegreeComboBox->insertItem("0");
		deltaDegreeComboBox->insertItem("1");
		deltaDegreeComboBox->insertItem("2");
		deltaDegreeComboBox->insertItem("3");
		deltaDegreeComboBox->insertItem("4");
		deltaDegreeComboBox->insertItem("5");
		deltaDegreeComboBox->insertItem("6");
		deltaDegreeComboBox->insertItem("10");
		deltaDegreeComboBox->insertItem("20");
		deltaDegreeComboBox->insertItem("30");

		j = -1;
		for(i=0; i<(int)deltaDegreeComboBox->count(); ++i)
		{
			int a = deltaDegreeComboBox->text(i).toInt();
			int b = abs(deltaGraus_);
			if(a == b)
				j = i;
		}

		if(j == -1)
		{
			QString s;
			s.setNum(abs(deltaGraus_));
			deltaDegreeComboBox->insertItem(s);
			deltaDegreeComboBox->setCurrentItem(deltaDegreeComboBox->count()-1);
		}
		else
			deltaDegreeComboBox->setCurrentItem(j);

		deltaMinuteComboBox->insertItem("0");
		deltaMinuteComboBox->insertItem("1");
		deltaMinuteComboBox->insertItem("2");
		deltaMinuteComboBox->insertItem("3");
		deltaMinuteComboBox->insertItem("4");
		deltaMinuteComboBox->insertItem("5");
		deltaMinuteComboBox->insertItem("6");
		deltaMinuteComboBox->insertItem("10");
		deltaMinuteComboBox->insertItem("20");
		deltaMinuteComboBox->insertItem("30");

		j = -1;
		for(i=0; i<(int)deltaMinuteComboBox->count(); ++i)
		{
			int a = deltaMinuteComboBox->text(i).toInt();
			int b = abs(deltaMins_);
			if(a == b)
				j = i;
		}

		if(j == -1)
		{
			QString s;
			s.setNum(abs(deltaMins_));
			deltaMinuteComboBox->insertItem(s);
			deltaMinuteComboBox->setCurrentItem(deltaMinuteComboBox->count()-1);
		}
		else
			deltaMinuteComboBox->setCurrentItem(j);

		deltaSecondComboBox->insertItem("0");
		deltaSecondComboBox->insertItem("1");
		deltaSecondComboBox->insertItem("2");
		deltaSecondComboBox->insertItem("3");
		deltaSecondComboBox->insertItem("4");
		deltaSecondComboBox->insertItem("5");
		deltaSecondComboBox->insertItem("6");
		deltaSecondComboBox->insertItem("10");
		deltaSecondComboBox->insertItem("20");
		deltaSecondComboBox->insertItem("30");

		j = -1;
		for(i=0; i<(int)deltaSecondComboBox->count(); ++i)
		{
			int a = deltaSecondComboBox->text(i).toInt();
			int b = abs((int)deltaSecs_);
			if(a == b)
				j = i;
		}

		if(j == -1)
		{
			QString s;
			s.setNum(fabs(deltaSecs_));
			deltaSecondComboBox->insertItem(s);
			deltaSecondComboBox->setCurrentItem(deltaSecondComboBox->count()-1);
		}
		else
			deltaSecondComboBox->setCurrentItem(j);
	}

// get tx direction
	if(txLatHorizontal)
		txLatHorizontalRadioButton->setChecked(true);
	else
		txLatVerticalRadioButton->setChecked(true);
	if(txLonHorizontal)
		txLonHorizontalRadioButton->setChecked(true);
	else
		txLonVerticalRadioButton->setChecked(true);

	precisionComboBox->insertItem("1");
	precisionComboBox->insertItem("2");
	precisionComboBox->insertItem("3");
	precisionComboBox->insertItem("4");
	precisionComboBox->insertItem("5");
	precisionComboBox->insertItem("6");
	precisionComboBox->insertItem("7");
	precisionComboBox->insertItem("8");

	for(i=0; i<(int)precisionComboBox->count(); ++i)
	{
		int a = precisionComboBox->text(i).toInt();
		if(a == precision)
		{
			precisionComboBox->setCurrentItem(i);
			break;
		}
	}	
}




void GeographicalGrid::intervalCalcAutoCheckBox_toggled( bool deltaCalc)
{
	int	i, j;

	if(deltaCalc)
	{
		for(i=0; i<(int)deltaDegreeComboBox->count(); ++i)
		{
			QString s = deltaDegreeComboBox->text(i);
			if(s.isEmpty())
			{
				deltaDegreeComboBox->setCurrentItem(i);
				break;
			}
		}
		if(i == deltaDegreeComboBox->count())
		{
			deltaDegreeComboBox->insertItem("");
			deltaDegreeComboBox->setCurrentItem(deltaDegreeComboBox->count()-1);
		}

		for(i=0; i<(int)deltaMinuteComboBox->count(); ++i)
		{
			QString s = deltaMinuteComboBox->text(i);
			if(s.isEmpty())
			{
				deltaMinuteComboBox->setCurrentItem(i);
				break;
			}
		}
		if(i == deltaMinuteComboBox->count())
		{
			deltaMinuteComboBox->insertItem("");
			deltaMinuteComboBox->setCurrentItem(deltaMinuteComboBox->count()-1);
		}

		for(i=0; i<(int)deltaSecondComboBox->count(); ++i)
		{
			QString s = deltaSecondComboBox->text(i);
			if(s.isEmpty())
			{
				deltaSecondComboBox->setCurrentItem(i);
				break;
			}
		}
		if(i == deltaSecondComboBox->count())
		{
			deltaSecondComboBox->insertItem("");
			deltaSecondComboBox->setCurrentItem(deltaSecondComboBox->count()-1);
		}

		intervalGroupBox->setEnabled(false);
	}
	else
	{
		intervalGroupBox->setEnabled(true);

		deltaDegreeComboBox->insertItem("0");
		deltaDegreeComboBox->insertItem("1");
		deltaDegreeComboBox->insertItem("2");
		deltaDegreeComboBox->insertItem("3");
		deltaDegreeComboBox->insertItem("4");
		deltaDegreeComboBox->insertItem("5");
		deltaDegreeComboBox->insertItem("6");
		deltaDegreeComboBox->insertItem("10");
		deltaDegreeComboBox->insertItem("20");
		deltaDegreeComboBox->insertItem("30");

		j = -1;
		for(i=0; i<(int)deltaDegreeComboBox->count(); ++i)
		{
			int a = deltaDegreeComboBox->text(i).toInt();
			int b = abs(deltaGraus_);
			if(a == b)
				j = i;
		}

		if(j == -1)
		{
			QString s;
			s.setNum(abs(deltaGraus_));
			deltaDegreeComboBox->insertItem(s);
			deltaDegreeComboBox->setCurrentItem(deltaDegreeComboBox->count()-1);
		}
		else
			deltaDegreeComboBox->setCurrentItem(j);

		deltaMinuteComboBox->insertItem("0");
		deltaMinuteComboBox->insertItem("1");
		deltaMinuteComboBox->insertItem("2");
		deltaMinuteComboBox->insertItem("3");
		deltaMinuteComboBox->insertItem("4");
		deltaMinuteComboBox->insertItem("5");
		deltaMinuteComboBox->insertItem("6");
		deltaMinuteComboBox->insertItem("10");
		deltaMinuteComboBox->insertItem("20");
		deltaMinuteComboBox->insertItem("30");

		j = -1;
		for(i=0; i<(int)deltaMinuteComboBox->count(); ++i)
		{
			int a = deltaMinuteComboBox->text(i).toInt();
			int b = abs(deltaMins_);
			if(a == b)
				j = i;
		}

		if(j == -1)
		{
			QString s;
			s.setNum(abs(deltaMins_));
			deltaMinuteComboBox->insertItem(s);
			deltaMinuteComboBox->setCurrentItem(deltaMinuteComboBox->count()-1);
		}
		else
			deltaMinuteComboBox->setCurrentItem(j);

		deltaSecondComboBox->insertItem("0");
		deltaSecondComboBox->insertItem("1");
		deltaSecondComboBox->insertItem("2");
		deltaSecondComboBox->insertItem("3");
		deltaSecondComboBox->insertItem("4");
		deltaSecondComboBox->insertItem("5");
		deltaSecondComboBox->insertItem("6");
		deltaSecondComboBox->insertItem("10");
		deltaSecondComboBox->insertItem("20");
		deltaSecondComboBox->insertItem("30");

		j = -1;
		for(i=0; i<(int)deltaSecondComboBox->count(); ++i)
		{
			int a = deltaSecondComboBox->text(i).toInt();
			int b = abs((int)deltaSecs_);
			if(a == b)
				j = i;
		}

		if(j == -1)
		{
			QString s;
			s.setNum(fabs(deltaSecs_));
			deltaSecondComboBox->insertItem(s);
			deltaSecondComboBox->setCurrentItem(deltaSecondComboBox->count()-1);
		}
		else
			deltaSecondComboBox->setCurrentItem(j);
	}
}


void GeographicalGrid::textColorPushButton_clicked()
{
	TeColor color = txVisual_.color();
	bool isOK = false;
	QColor qcor(color.red_, color.green_, color.blue_);

	QColor	qc = QColorDialog::getRgba (qcor.rgb(), &isOK, this );
	if (isOK)
	{
		color.init(qc.red(), qc.green(), qc.blue());
		txVisual_.color(color);
		textColorPushButton->setPaletteBackgroundColor(qc);
		textColorPushButton->repaint();
	}
}


void GeographicalGrid::lineColorPushButton_clicked()
{
	bool isOK = false;

	lineCor_ = QColorDialog::getRgba (lineCor_.rgb(), &isOK, this );
	if (isOK)
	{
		lineColorPushButton->setPaletteBackgroundColor(lineCor_);
		lineColorPushButton->repaint();
	}
}

void GeographicalGrid::okPushButton_clicked()
{
	int precision = precisionComboBox->currentText().toInt();
	string family = fontComboBox->currentText().latin1();

	string up = "UPDATE app_geogrid SET font_family = '" + family + "'";
	up += ", red = " + Te2String(txVisual_.color().red_);
	up += ", green = " + Te2String(txVisual_.color().green_);
	up += ", blue = " + Te2String(txVisual_.color().blue_);

	if(boldCheckBox->isChecked())
		up += ", bold = 1";
	else
		up += ", bold = 0";

	if(italicCheckBox->isChecked())
		up += ", italic = 1";
	else
		up += ", italic = 0";

	up += ", font_size = " + Te2String(sizeComboBox->currentText().toInt());

	if(visible_ == true)
		up += ", visible = 1";
	else
		up += ", visible = 0";

	if(leftCheckBox->isChecked() == true)
		up += ", left_vis = 1";
	else
		up += ", left_vis = 0";

	if(rightCheckBox->isChecked() == true)
		up += ", right_vis = 1";
	else
		up += ", right_vis = 0";

	if(topCheckBox->isChecked() == true)
		up += ", top_vis = 1";
	else
		up += ", top_vis = 0";

	if(bottomCheckBox->isChecked() == true)
		up += ", bottom_vis = 1";
	else
		up += ", bottom_vis = 0";

	up += ", line_red = " + Te2String(lineCor_.red());
	up += ", line_green = " + Te2String(lineCor_.green());
	up += ", line_blue = " + Te2String(lineCor_.blue());

	if(intervalCalcAutoCheckBox->isChecked() == true)
		up += ", delta_calc = 1";
	else
		up += ", delta_calc = 0";

	up += ", delta_graus = " + Te2String(deltaDegreeComboBox->currentText().toInt());
	up += ", delta_mins = " + Te2String(deltaMinuteComboBox->currentText().toInt());
	up += ", delta_secs = " + Te2String(deltaSecondComboBox->currentText().toDouble(), precision);

	int grau, min;
	double sec; 
	grau = abs(initialLonDegreeComboBox->currentText().toInt());
	min = abs(initialLonMinuteComboBox->currentText().toInt());
	sec = fabs(initialLonSecondComboBox->currentText().toDouble());
	if(initialLonWestRadioButton->isChecked())
	{
		grau = -abs(grau);
		min = -abs(min);
		sec = -fabs(sec);
	}

	up += ", dx_graus_offset = " + Te2String(grau);
	up += ", dx_mins_offset = " + Te2String(min);
	up += ", dx_secs_offset = " + Te2String(sec, precision);

	grau = abs(initialLatDegreeComboBox->currentText().toInt());
	min = abs(initialLatMinuteComboBox->currentText().toInt());
	sec = fabs(initialLatSecondComboBox->currentText().toDouble());
	if(initialLatSouthRadioButton->isChecked())
	{
		grau = -abs(grau);
		min = -abs(min);
		sec = -fabs(sec);
	}

	up += ", dy_graus_offset = " + Te2String(grau);
	up += ", dy_mins_offset = " + Te2String(min);
	up += ", dy_secs_offset = " + Te2String(sec, precision);

	if(txLatHorizontalRadioButton->isChecked() == true)
		up += ", tx_lat_horizontal = 1";
	else
		up += ", tx_lat_horizontal = 0";

	if(txLonHorizontalRadioButton->isChecked() == true)
		up += ", tx_lon_horizontal = 1";
	else
		up += ", tx_lon_horizontal = 0";

	up += ", sec_precision = " + Te2String(precision);

	up += " WHERE view_id = " + Te2String(view_->id());

	if(db_->execute(up) == false)
	{
		QString msg = tr("Geographical Grid not changed!");
		msg += db_->errorMessage().c_str();
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

	display_->plotData();
//	accept();
}

void GeographicalGrid::verifyValue( QComboBox *combo, const QString &s, bool real, bool negative, double absMax )
{
	int	i;
	bool error = false;
	bool hasPoint = false;
	int size = (int)s.length();

	if(real)
	{
		i = 0;
		while(i < size)
		{
			int f;
			if((f = s.find('.', i+1)) >= 0)
			{
				if(hasPoint == false)
					hasPoint = true;
				else
				{
					QString msg = tr("You already type '.'!");
					QMessageBox::warning(this, tr("Warning"), msg);
					QString ss = s;
					ss.remove(f, 1);
					combo->setCurrentText(ss);
					return;
				}
			}
			i++;
		}
	}

	for(i = 0; i<size; ++i)
	{
		if(i == 0)
		{
			if(real && negative)
			{
				if(s[i].isNumber() || s[i] == '.' || s[i] == '-')
					continue;
				else
					error = true;
			}
			else if(real)
			{
				if(s[i].isNumber() || s[i] == '.')
					continue;
				else
					error = true;
			}
			else if(negative)
			{
				if(s[i].isNumber() || s[i] == '-')
					continue;
				else
					error = true;
			}
			else
			{
				if(s[i].isNumber())
					continue;
				else
					error = true;
			}

			if(error)
			{
				QString ss = s;
				ss.remove(0, 1);
				combo->setCurrentText(ss);
				QString msg = tr("Invalid character typed!");
				QMessageBox::warning(this, tr("Warning"), msg);
				return;
			}		
		}

		if(s[i].isNumber() == false)
		{
			if(real && s[i] == '.')
				continue;

			QString ss = s;
			ss.remove(i, 1);
			combo->setCurrentText(ss);
			QString msg = tr("Invalid character typed!");
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}
	}

	double val = fabs(s.toDouble());
	if(val > absMax)
	{
		QString v;
		v.setNum(absMax);
		QString msg = tr("Max Value is:") + " " + v;
		QMessageBox::warning(this, tr("Warning"), msg);
	}
}

void GeographicalGrid::sizeComboBox_textChanged( const QString &s )
{
	verifyValue(sizeComboBox, s, false, false, 200);
}

void GeographicalGrid::initialLatDegreeComboBox_textChanged( const QString &s )
{
	verifyValue(initialLatDegreeComboBox, s, false, false, 360);
}

void GeographicalGrid::initialLonDegreeComboBox_textChanged( const QString &s )
{
	verifyValue(initialLonDegreeComboBox, s, false, false, 360);
}

void GeographicalGrid::initialLonMinuteComboBox_textChanged( const QString &s )
{
	verifyValue(initialLonMinuteComboBox, s, false, false, 60);
}

void GeographicalGrid::initialLonSecondComboBox_textChanged( const QString &s )
{
	verifyValue(initialLonSecondComboBox, s, true, false, 60);
}

void GeographicalGrid::initialLatMinuteComboBox_textChanged( const QString &s )
{
	verifyValue(initialLatMinuteComboBox, s, false, false, 60);
}

void GeographicalGrid::initialLatSecondComboBox_textChanged( const QString &s )
{
	verifyValue(initialLatSecondComboBox, s, true, false, 60);
}

void GeographicalGrid::precisionComboBox_textChanged( const QString &s )
{
	verifyValue(precisionComboBox, s, false, false, 60);
}

void GeographicalGrid::deltaDegreeComboBox_textChanged( const QString &s )
{
	verifyValue(deltaDegreeComboBox, s, false, false, 60);
}

void GeographicalGrid::deltaMinuteComboBox_textChanged( const QString &s )
{
	verifyValue(deltaMinuteComboBox, s, false, false, 60);
}

void GeographicalGrid::deltaSecondComboBox_textChanged( const QString &s )
{
	verifyValue(deltaSecondComboBox, s, true, false, 60);
}


void GeographicalGrid::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("geographicalGrid.htm");
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
