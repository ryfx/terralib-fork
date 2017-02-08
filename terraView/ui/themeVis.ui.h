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

#include <TeAppTheme.h>
#include <TeExternalTheme.h>

void ThemeVisibility::init()
{
	help_ = 0;
	samplesCheckBox->setChecked(false);
	textCheckBox->setChecked(false);
	linesCheckBox->setChecked(false);
	rasterCheckBox->setChecked(false);
	arcsCheckBox->setChecked(false);
	polygonsCheckBox->setChecked(false);
	cellsCheckBox->setChecked(false);
	nodesCheckBox->setChecked(false);
	contoursCheckBox->setChecked(false);
	pointsCheckBox->setChecked(false);
	legendCheckBox->setChecked(false);
	groupingCheckBox->setChecked(false);
	pieChartCheckBox->setChecked(false);
	represOutOfCollectionCheckBox->setChecked(false);
	objRepresWithoutLinkCheckBox->setChecked(false);

	samplesCheckBox->setEnabled(false);
	textCheckBox->setEnabled(false);
	linesCheckBox->setEnabled(false);
	rasterCheckBox->setEnabled(false);
	arcsCheckBox->setEnabled(false);
	polygonsCheckBox->setEnabled(false);
	cellsCheckBox->setEnabled(false);
	nodesCheckBox->setEnabled(false);
	contoursCheckBox->setEnabled(false);
	pointsCheckBox->setEnabled(false);
	legendCheckBox->setEnabled(false);
	groupingCheckBox->setEnabled(false);
	pieChartCheckBox->setEnabled(false);
	represOutOfCollectionCheckBox->setEnabled(false);
	objRepresWithoutLinkCheckBox->setEnabled(false);

	scaleVec_.clear();
	scaleVec_.push_back(0);
	scaleVec_.push_back(1000);
	scaleVec_.push_back(2500);
	scaleVec_.push_back(5000);
	scaleVec_.push_back(10000);
	scaleVec_.push_back(25000);
	scaleVec_.push_back(50000);
	scaleVec_.push_back(100000);
	scaleVec_.push_back(250000);
	scaleVec_.push_back(500000);
	scaleVec_.push_back(1000000);
	scaleVec_.push_back(2500000);
	scaleVec_.push_back(5000000);
	scaleVec_.push_back(10000000);
	scaleVec_.push_back(25000000);
	scaleVec_.push_back(50000000);
	scaleVec_.push_back(100000000);

	minimumScaleComboBox->clear();
	maximumScaleComboBox->clear();


	int i;
	for(i=0; i<(int)scaleVec_.size(); ++i)
	{
		if(scaleVec_[i] == 0)
		{
			minimumScaleComboBox->insertItem("");
			maximumScaleComboBox->insertItem("");
		}
		else
		{
			if(i > 1)
				minimumScaleComboBox->insertItem(Te2String(scaleVec_[i]).c_str());
			if(i < (int)scaleVec_.size()-1)
				maximumScaleComboBox->insertItem(Te2String(scaleVec_[i]).c_str());
		}
	}
}



void ThemeVisibility::loadTheme(TeAppTheme *theme)
{
	bool bNotEnable=false;

	theme_ = theme;
	TeTheme* baseTheme = (TeTheme*)theme_->getTheme();
	previousVisibleRep_ = baseTheme->visibleRep();
	unsigned long possibleVis = baseTheme->visibleGeoRep();

	if (baseTheme->type()== TeTHEME)
		possibleVis = baseTheme->layer()->geomRep();
	else if (baseTheme->type() == TeEXTERNALTHEME)
		possibleVis = ((TeExternalTheme*)baseTheme)->getRemoteTheme()->layer()->geomRep();
	else if (baseTheme->type() == TeFILETHEME)
		bNotEnable=true;

	if(!theme_->chartAttributes_.empty())
		possibleVis |= 0x80000000;

	if(!baseTheme->legend().empty())
	{
		possibleVis |= 0x40000000; // grouping visibility
		if(theme_->showCanvasLegend())
			possibleVis |= 0x20000000; // legend visibility
	}

	if(!bNotEnable)
	{
		if((possibleVis & TeSAMPLES))
			samplesCheckBox->setEnabled(true);
		if((possibleVis & TeTEXT) && theme_->textTable().empty() == false)
			textCheckBox->setEnabled(true);
		if((possibleVis & TeLINES))
			linesCheckBox->setEnabled(true);
		if((possibleVis & TeRASTER || possibleVis & TeRASTERFILE))
			rasterCheckBox->setEnabled(true);
		if((possibleVis & TeARCS))
			arcsCheckBox->setEnabled(true);
		if((possibleVis & TePOLYGONS))
			polygonsCheckBox->setEnabled(true);
		if((possibleVis & TeCELLS))
			cellsCheckBox->setEnabled(true);
		if((possibleVis & TeNODES))
			nodesCheckBox->setEnabled(true);
		if((possibleVis & TeCONTOURS))
			contoursCheckBox->setEnabled(true);
		if((possibleVis & TePOINTS))
			pointsCheckBox->setEnabled(true);
	}

	if((possibleVis & 0x80000000) && !theme_->chartAttributes_.empty())
	{
		pieChartCheckBox->setEnabled(true);
		if(baseTheme->type() == TeTHEME)
			legendCheckBox->setEnabled(true);
	}
	if((possibleVis & 0x40000000) && baseTheme->grouping().groupMode_ != TeNoGrouping)
	{
		groupingCheckBox->setEnabled(true);
		if(baseTheme->type() == TeTHEME)
			legendCheckBox->setEnabled(true);
	}
	if(possibleVis & TeRASTER)
		if(baseTheme->type() == TeTHEME)
			legendCheckBox->setEnabled(true);

	if((previousVisibleRep_ & TeSAMPLES))
		samplesCheckBox->setChecked(true);
	if((previousVisibleRep_ & TeTEXT) && theme_->textTable().empty() == false)
		textCheckBox->setChecked(true);
	if((previousVisibleRep_ & TeLINES))
		linesCheckBox->setChecked(true);
	if((previousVisibleRep_ & TeRASTER || possibleVis & TeRASTERFILE))
		rasterCheckBox->setChecked(true);
	if((previousVisibleRep_ & TeARCS))
		arcsCheckBox->setChecked(true);
	if((previousVisibleRep_ & TePOLYGONS))
		polygonsCheckBox->setChecked(true);
	if((previousVisibleRep_ & TeCELLS))
		cellsCheckBox->setChecked(true);
	if((previousVisibleRep_ & TeNODES))
		nodesCheckBox->setChecked(true);
	if((previousVisibleRep_ & TeCONTOURS))
		contoursCheckBox->setChecked(true);
	if((previousVisibleRep_ & TePOINTS))
		pointsCheckBox->setChecked(true);
	if((previousVisibleRep_ & 0x80000000) && !theme_->chartAttributes_.empty())
		pieChartCheckBox->setChecked(true);
	if((previousVisibleRep_ & 0x40000000) && baseTheme->grouping().groupMode_ != TeNoGrouping)
		groupingCheckBox->setChecked(true);
	if(previousVisibleRep_ & 0x20000000)
		if(baseTheme->type() == TeTHEME)
			legendCheckBox->setChecked(true);

	int min = (int)baseTheme->minScale();
	int max = (int)baseTheme->maxScale();

	if(min == 0)
		minimumScaleComboBox->setCurrentItem(0);
	else if(min == 2500)
		minimumScaleComboBox->setCurrentItem(1);
	else if(min == 5000)
		minimumScaleComboBox->setCurrentItem(2);
	else if(min == 10000)
		minimumScaleComboBox->setCurrentItem(3);
	else if(min == 25000)
		minimumScaleComboBox->setCurrentItem(4);
	else if(min == 50000)
		minimumScaleComboBox->setCurrentItem(5);
	else if(min == 100000)
		minimumScaleComboBox->setCurrentItem(6);
	else if(min == 250000)
		minimumScaleComboBox->setCurrentItem(7);
	else if(min == 500000)
		minimumScaleComboBox->setCurrentItem(8);
	else if(min == 1000000)
		minimumScaleComboBox->setCurrentItem(9);
	else if(min == 2500000)
		minimumScaleComboBox->setCurrentItem(10);
	else if(min == 5000000)
		minimumScaleComboBox->setCurrentItem(11);
	else if(min == 10000000)
		minimumScaleComboBox->setCurrentItem(12);
	else if(min == 25000000)
		minimumScaleComboBox->setCurrentItem(13);
	else if(min == 50000000)
		minimumScaleComboBox->setCurrentItem(14);
	else if(min == 100000000)
		minimumScaleComboBox->setCurrentItem(15);
	else
	{
		minimumScaleComboBox->insertItem(Te2String(min, 0).c_str());
		minimumScaleComboBox->setCurrentItem(16);
	}

	if(max == 0)
		maximumScaleComboBox->setCurrentItem(0);
	else if(max == 1000)
		maximumScaleComboBox->setCurrentItem(1);
	else if(max == 2500)
		maximumScaleComboBox->setCurrentItem(2);
	else if(max == 5000)
		maximumScaleComboBox->setCurrentItem(3);
	else if(max == 10000)
		maximumScaleComboBox->setCurrentItem(4);
	else if(max == 25000)
		maximumScaleComboBox->setCurrentItem(5);
	else if(max == 50000)
		maximumScaleComboBox->setCurrentItem(6);
	else if(max == 100000)
		maximumScaleComboBox->setCurrentItem(7);
	else if(max == 250000)
		maximumScaleComboBox->setCurrentItem(8);
	else if(max == 500000)
		maximumScaleComboBox->setCurrentItem(9);
	else if(max == 1000000)
		maximumScaleComboBox->setCurrentItem(10);
	else if(max == 2500000)
		maximumScaleComboBox->setCurrentItem(11);
	else if(max == 5000000)
		maximumScaleComboBox->setCurrentItem(12);
	else if(max == 10000000.)
		maximumScaleComboBox->setCurrentItem(13);
	else if(max == 25000000)
		maximumScaleComboBox->setCurrentItem(14);
	else if(max == 50000000)
		maximumScaleComboBox->setCurrentItem(15);
	else
	{
		maximumScaleComboBox->insertItem(Te2String(max, 0).c_str());
		maximumScaleComboBox->setCurrentItem(16);
	}

}


void ThemeVisibility::okPushButton_clicked()
{
	TeAbstractTheme* baseTheme = theme_->getTheme();

	int rep = 0;
	int showLegOnCanvas = 0;
	if(samplesCheckBox->isChecked())
		rep |= TeSAMPLES;
	if(textCheckBox->isChecked())
		rep |= TeTEXT;
	if(linesCheckBox->isChecked())
		rep |= TeLINES;
	if(rasterCheckBox->isChecked())
		rep |= TeRASTER;
	if(arcsCheckBox->isChecked())
		rep |= TeARCS;
	if(polygonsCheckBox->isChecked())
		rep |= TePOLYGONS;
	if(cellsCheckBox->isChecked())
		rep |= TeCELLS;
	if(nodesCheckBox->isChecked())
		rep |= TeNODES;
	if(contoursCheckBox->isChecked())
		rep |= TeCONTOURS;
	if(pointsCheckBox->isChecked())
		rep |= TePOINTS;
	if(pieChartCheckBox->isChecked())
		rep |= 0x80000000;
	if (baseTheme->grouping().groupMode_ == TeRasterSlicing)
		baseTheme->removeRasterVisual();
	if(groupingCheckBox->isChecked())
		rep |= 0x40000000;
	if(legendCheckBox->isChecked())
	{
		rep |= 0x20000000; // legend visible
		showLegOnCanvas = 1;
	}

	if(baseTheme->visibleRep() != rep)
	{
		baseTheme->visibleRep(rep);

		TeDatabase* db = theme_->getLocalDatabase();
		
		string q = "UPDATE te_theme SET visible_rep = " + Te2String(rep);
		q += " WHERE theme_id = " + Te2String(baseTheme->id());
		db->execute(q);

		q = "UPDATE te_theme_application SET show_canvas_leg = " + Te2String(showLegOnCanvas);
		q += " WHERE theme_id = " + Te2String(baseTheme->id());
		db->execute(q);

		theme_->showCanvasLegend((bool)showLegOnCanvas);
	}
	accept();
}

unsigned long ThemeVisibility::previousVisibleRep()
{
    return previousVisibleRep_;
}


void ThemeVisibility::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("themeVis.htm");
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


void ThemeVisibility::minimumScaleComboBox_activated( const QString &s )
{
	int i;

	if(s.isEmpty())
	{
		minimumScaleComboBox->clear();
		maximumScaleComboBox->clear();

		for(i=0; i<(int)scaleVec_.size(); ++i)
		{
			if(scaleVec_[i] == 0)
			{
				minimumScaleComboBox->insertItem("");
				maximumScaleComboBox->insertItem("");
			}
			else
			{
				if(i > 1)
					minimumScaleComboBox->insertItem(Te2String(scaleVec_[i]).c_str());
				if(i < (int)scaleVec_.size()-1)
					maximumScaleComboBox->insertItem(Te2String(scaleVec_[i]).c_str());
			}
		}
		minimumScaleComboBox->setCurrentItem(0);
		maximumScaleComboBox->setCurrentItem(0);
	}
	else
	{
		unsigned int v = maximumScaleComboBox->currentText().toInt();
		unsigned int min = s.toInt();
		bool ins = true;
		maximumScaleComboBox->clear();

		for(i=0; i<(int)scaleVec_.size(); ++i)
		{
			if(scaleVec_[i] == 0)
				maximumScaleComboBox->insertItem("");
			else
			{
				if(scaleVec_[i] >= min)
					break;
				maximumScaleComboBox->insertItem(Te2String(scaleVec_[i]).c_str());
				if(scaleVec_[i] == v)
					ins = false;
			}
		}
		if(v != 0 && v < min)
		{
			if(ins)
				maximumScaleComboBox->insertItem(Te2String(v).c_str());

			for(i=0; i<(int)maximumScaleComboBox->count(); ++i)
			{
				unsigned int vv = maximumScaleComboBox->text(i).toInt();
				if(v == vv)
				{
					maximumScaleComboBox->setCurrentItem(i);
					break;
				}
			}
		}
		else
			maximumScaleComboBox->setCurrentItem(maximumScaleComboBox->count() - 1);
	}
}


void ThemeVisibility::maximumScaleComboBox_activated( const QString &s )
{
	int i;

	if(s.isEmpty())
	{
		minimumScaleComboBox->clear();
		maximumScaleComboBox->clear();

		for(i=0; i<(int)scaleVec_.size(); ++i)
		{
			if(scaleVec_[i] == 0)
			{
				minimumScaleComboBox->insertItem("");
				maximumScaleComboBox->insertItem("");
			}
			else
			{
				if(i > 1)
					minimumScaleComboBox->insertItem(Te2String(scaleVec_[i]).c_str());
				if(i < (int)scaleVec_.size()-1)
					maximumScaleComboBox->insertItem(Te2String(scaleVec_[i]).c_str());
			}
		}
		minimumScaleComboBox->setCurrentItem(0);
		maximumScaleComboBox->setCurrentItem(0);
	}
	else
	{
		unsigned int v = minimumScaleComboBox->currentText().toInt();
		unsigned int max = s.toInt();
		bool ins = true;
		minimumScaleComboBox->clear();

		for(i=0; i<(int)scaleVec_.size(); ++i)
		{
			if(scaleVec_[i] == 0)
				minimumScaleComboBox->insertItem("");
			else
			{
				if(scaleVec_[i] <= max)
					continue;
				minimumScaleComboBox->insertItem(Te2String(scaleVec_[i]).c_str());
				if(scaleVec_[i] == v)
					ins = false;
			}
		}
		if(v != 0 && v > max)
		{
			if(ins)
				minimumScaleComboBox->insertItem(Te2String(v).c_str());

			for(i=0; i<(int)minimumScaleComboBox->count(); ++i)
			{
				unsigned int vv = minimumScaleComboBox->text(i).toInt();
				if(v == vv)
				{
					minimumScaleComboBox->setCurrentItem(i);
					break;
				}
			}
		}
		else
			minimumScaleComboBox->setCurrentItem(0);
	}
}


void ThemeVisibility::legendCheckBox_toggled( bool b)
{
	TeTheme* baseTheme = (TeTheme*)theme_->getTheme();

	if(b)
	{
		if(groupingCheckBox->isChecked()==false && pieChartCheckBox->isChecked()==false)
		{
			if(baseTheme->grouping().groupMode_ != TeNoGrouping)
				groupingCheckBox->setChecked(true);
			if(theme_->chartAttributes_.empty() == false)
				pieChartCheckBox->setChecked(true);
		}
	}
}


void ThemeVisibility::pieChartCheckBox_toggled( bool b)
{
	if(b == false)
		if(groupingCheckBox->isChecked()==false && legendCheckBox->isChecked()==true)
			legendCheckBox->setChecked(false);
}


void ThemeVisibility::groupingCheckBox_toggled( bool b)
{
	if(b == false)
		if(pieChartCheckBox->isChecked()==false && legendCheckBox->isChecked()==true)
			legendCheckBox->setChecked(false);
}
