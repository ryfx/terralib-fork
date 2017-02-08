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
#include <qmessagebox.h>

void GraphicParams::init()
{
	help_ = 0;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	TeTheme* theme = (TeTheme*)mainWindow_->currentTheme()->getTheme();
	if(!(theme->type() == TeTHEME || theme->type() == TeEXTERNALTHEME))
		return;

	graphicTypeComboBox->setCurrentItem(GraphicParams::Histogram);
	inputObjectsComboBox->setCurrentItem(0);	// All
	graphicSlicesComboBox->setCurrentItem(5);  // slices = 30

	graphicTypeComboBox_activated(GraphicParams::Histogram);
	xFunctionComboBox->setEnabled(false);
	yFunctionComboBox->setEnabled(false);

	TeQtGrid *ptrGrid = mainWindow_->getGrid();
	connect((const QObject*)ptrGrid,SIGNAL(gridChangedSignal()),
		this, SLOT(updateCombosSlot()));
}


void GraphicParams::updateCombosSlot()
{
	if(isShown())
	{
		hide();
		show();
	}
}

void GraphicParams::show()
{
	TeTheme* theme = (TeTheme*)mainWindow_->currentTheme()->getTheme();
	if(!(theme->type() == TeTHEME || theme->type() == TeEXTERNALTHEME))
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		reject();
		hide();
		return;
	}

	db_ = mainWindow_->currentDatabase();
	theme_ = mainWindow_->currentTheme();
	graphic_ = mainWindow_->getGraphicWindow();

	if(graphic_->isHidden())
	{
		attributeXComboBox->clear();
		attributeYComboBox->clear();
		TeAttributeList	attrVec = ((TeTheme*)theme_->getTheme())->sqlNumAttList();
		unsigned int i;
		for(i=0; i<attrVec.size(); i++)
		{
			QString	name(attrVec[i].rep_.name_.c_str());
			attributeXComboBox->insertItem(name);
			attributeYComboBox->insertItem(name);
		}
	}

	QDialog::show();
}


void GraphicParams::graphicTypeComboBox_activated(int graphicType)
{
	if (graphicType == GraphicParams::Histogram)
	{
		graphicSlicesTextLabel->setEnabled(true);
		graphicSlicesComboBox->setEnabled(true);
		attributeYTextLabel->setEnabled(false);
		attributeYComboBox->setEnabled(false);
		yFunctionComboBox->setEnabled(false);
	}
	if (graphicType == GraphicParams::NormalProbability)
	{
		graphicSlicesTextLabel->setEnabled(true);
		graphicSlicesComboBox->setEnabled(true);
		attributeYTextLabel->setEnabled(false);
		attributeYComboBox->setEnabled(false);
		yFunctionComboBox->setEnabled(false);
	}
	if (graphicType == GraphicParams::Dispersion)
	{
		graphicSlicesTextLabel->setEnabled(false);
		graphicSlicesComboBox->setEnabled(false);
		attributeYTextLabel->setEnabled(true);
		attributeYComboBox->setEnabled(true);
	}
}





void GraphicParams::okPushButton_clicked()
{
	graphic_ = mainWindow_->getGraphicWindow();
	graphic_->show();
}


void GraphicParams::reinit()
{
	db_ = mainWindow_->currentDatabase();
	theme_ = mainWindow_->currentTheme();

	attributeXComboBox->clear();
	attributeYComboBox->clear();
	TeAttributeList	attrVec = ((TeTheme*)theme_->getTheme())->sqlNumAttList();
	unsigned int i;
	for(i=0; i<attrVec.size(); i++)
	{
		QString	name(attrVec[i].rep_.name_.c_str());
		attributeXComboBox->insertItem(name);
		attributeYComboBox->insertItem(name);
	}
}


void GraphicParams::applyGraphic( int range, string fieldx, string fieldy, int graphType )
{
	functionCheckBox->setChecked(false);
	functionCheckBox_toggled(false);
	graphicTypeComboBox->setCurrentItem(graphType);
	graphicTypeComboBox_activated(graphType);

	inputObjectsComboBox->setCurrentItem(range);

	int	i;
	for(i=0; i<attributeXComboBox->count(); i++)
	{
		string s = attributeXComboBox->text(i).latin1();
		if(s == fieldx)
			break;
	}
	if(i < attributeXComboBox->count())
		attributeXComboBox->setCurrentItem(i);

	if(fieldy.empty() == false)
	{
		for(i=0; i<attributeYComboBox->count(); i++)
		{
			string s = attributeYComboBox->text(i).latin1();
			if(s == fieldy)
				break;
		}
		if(i < attributeYComboBox->count())
			attributeYComboBox->setCurrentItem(i);
	}

	okPushButton_clicked();
}


void GraphicParams::functionCheckBox_toggled(bool b)
{
	int gType = graphicTypeComboBox->currentItem();
	if (b)
	{
		xFunctionComboBox->setEnabled(true);
		if (gType == GraphicParams::Dispersion)
			yFunctionComboBox->setEnabled(true);
		else
			yFunctionComboBox->setEnabled(false);
	}
	else
	{
		xFunctionComboBox->setEnabled(false);
		yFunctionComboBox->setEnabled(false);
	}
}

void GraphicParams::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("graphicParams.htm");
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


