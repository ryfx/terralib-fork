/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright © 2001-2004 INPE and Tecgraf/PUC-Rio.     

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

// Plugin's includes
#include <FillCellDlg.h>
#include <FillCell.h>
#include <help.h>

#include <TeQtGrid.h>

#include <qmessagebox.h>
#include <qwidgetstack.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qdatetimeedit.h>
#include <qvalidator.h>
#include <qradiobutton.h>


///////////////////////////////////////////////////////////////////////////////////
/////// Fill Cell UI - General Functions
///////////////////////////////////////////////////////////////////////////////////

FillCellDlg::FillCellDlg( QWidget* parent ):FillCellUI( parent, "FillCell" , Qt::WType_TopLevel | Qt::WShowModal )
{
	_fillCellUtils = NULL;
	_fillCellOp = NULL;

	_help = NULL;
}

FillCellDlg::~FillCellDlg()
{
	delete _fillCellUtils;

	delete _fillCellOp;

	delete _help;
}

void FillCellDlg::UpdateForm( const PluginParameters& pp )
{
	resetForm();

	plugin_params_ = pp;

	if(!plugin_params_.getCurrentDatabasePtr())
	{
		QMessageBox::critical( this, tr("Error"), tr( "There is no database connected!" ) );
		hide();
		return;
	}

//cell utils
	if(_fillCellUtils)
	{
		delete _fillCellUtils;
	}

	_fillCellUtils = new FillCellUtils(plugin_params_.getCurrentDatabasePtr());


//cell operations
	if(_fillCellOp)
	{
		delete _fillCellOp;
	}

	_fillCellOp = new FillCellOp();


//fill interface fields
	fillForm();

//fill interface layer cell list
	if(!fillLayersCell())
	{
		QMessageBox::critical( this, "Warning", tr( "To use this plugin is required to have at least one layer of cells. See Help." ) );
		hide();
		return;
	}
	
//show interface
	exec();
}

void FillCellDlg::resetForm()
{
	widgetStack->raiseWidget(widgetStack->widget(0));

	nextTwoPushButton->hide();
	okPushButton->hide();

//page one
	themeComboBox->clear();

	themeComboBox_activated("");

//page two
	cellLayerComboBox->clear();

	cellLayerComboBox_activated("");

//page three
	createTimeRadioButton->setChecked(false);

	initialDateEdit->setOrder(QDateEdit::DMY);
	finalDateEdit->setOrder(QDateEdit::DMY);

	initialDateEdit->setEnabled(false);
	initialTimeEdit->setEnabled(false);
	finalDateEdit->setEnabled(false);
	finalTimeEdit->setEnabled(false);

	setHourCheckBox->setChecked(false);

	selectTimeRadioButton->setChecked(false);

	selectTimeComboBox->setEnabled(false);
	
	return;
}

void FillCellDlg::fillForm()
{
	if(!_fillCellUtils)
	{
		return;
	}

//theme list
	std::vector<std::string> themes = _fillCellUtils->getThemeList();

	themeComboBox->clear();
	themeComboBox->insertItem("");

	for(unsigned int i = 0; i < themes.size(); ++i)
	{
		themeComboBox->insertItem(themes[i].c_str());
	}

	std::string themeName = "";

	if(plugin_params_.getCurrentThemeAppPtr())
	{
		themeName = plugin_params_.getCurrentThemeAppPtr()->getTheme()->name();

		themeComboBox->setCurrentText(themeName.c_str());
	}

	themeComboBox_activated(themeName.c_str());

//date/time filds
	fillDateTimeFields();
}

bool FillCellDlg::fillLayersCell()
{

	if(!_fillCellUtils)
	{
		return false;
	}

//layer list
	cellLayerComboBox->clear();

	std::vector<std::string> layers = _fillCellUtils->getCellLayerList();

	if(layers.empty())
	{
		return false;
	}

	cellLayerComboBox->insertItem("");

	for(unsigned int i = 0; i < layers.size(); ++i)
	{
		cellLayerComboBox->insertItem(layers[i].c_str());
	}

	return true;
}

void FillCellDlg::fillDateTimeFields()
{
	/*dateFormatComboBox->clear();
	dateFormatComboBox->insertItem("");
	dateFormatComboBox->insertItem("D");
	dateFormatComboBox->insertItem("DsMsY");
	dateFormatComboBox->insertItem("DsYsM");
	dateFormatComboBox->insertItem("M");
	dateFormatComboBox->insertItem("MsDsY");
	dateFormatComboBox->insertItem("MsYsD");
	dateFormatComboBox->insertItem("Y");
	dateFormatComboBox->insertItem("YsMsD");
	dateFormatComboBox->insertItem("YsDsM");

	timeFormatComboBox->clear();
	timeFormatComboBox->insertItem("");
	timeFormatComboBox->insertItem("H");
	timeFormatComboBox->insertItem("HsmsS");
	timeFormatComboBox->insertItem("HsSsm");
	timeFormatComboBox->insertItem("m");
	timeFormatComboBox->insertItem("msSsH");
	timeFormatComboBox->insertItem("msHsS");
	timeFormatComboBox->insertItem("S");
	timeFormatComboBox->insertItem("SsHsm");
	timeFormatComboBox->insertItem("SsmsH");

	ampmComboBox->clear();
	ampmComboBox->insertItem("AM-PM");

	dateSeparatorLineEdit->setText("/");

	timeSeparatorLineEdit->setText(":");*/
}

void FillCellDlg::fillAttrTable()
{
	if(themeComboBox->currentText().isEmpty())
	{
		return;
	}

	if(!_fillCellUtils)
	{
		return;
	}

	std::string themeName = themeComboBox->currentText().latin1();
	
	std::vector<std::string> attrVec = _fillCellUtils->getThemeAttrTables(themeName);

	tableVecComboBox->clear();

	for(unsigned int i = 0; i < attrVec.size(); ++i)
	{
		tableVecComboBox->insertItem(attrVec[i].c_str());
	}

	if(!attrVec.empty())
	{
		tableVecComboBox_activated(attrVec[0].c_str());
	}
}

void FillCellDlg::fillTemporalAttributes()
{
//check parameters
	if(!_fillCellUtils)
	{
		return;
	}
	if(cellLayerComboBox->currentText().isEmpty())
	{
		return;
	}

	if(selectTableComboBox->currentText().isEmpty())
	{
		return;
	}

//get times
	selectTimeComboBox->clear();

	std::vector<std::string> vec = _fillCellUtils->getTableTimes(cellLayerComboBox->currentText().latin1(), selectTableComboBox->currentText().latin1());

	if(vec.empty())
	{
		selectTimeRadioButton->setChecked(false);
		selectTimeRadioButton->setEnabled(false);
		selectTimeComboBox->setEnabled(false);
	}

//fill combo
	
	selectTimeComboBox->insertItem("");

	for(unsigned int i = 0; i < vec.size(); ++i)
	{
		selectTimeComboBox->insertItem(vec[i].c_str());
	}

	selectTimeComboBox->setFocus();
}

void FillCellDlg::execute(const bool& isTemporal)
{
//check parameters
	if(!_fillCellUtils)
	{
		return;
	}

	if(!_fillCellOp)
	{
		return;
	}

	FillCell fc(plugin_params_.getCurrentDatabasePtr());

//set parameters

	//first page
	fc._inputThemeName = themeComboBox->currentText().latin1();

	if(polyRadioButton->isChecked())
	{
		fc._rep = TePOLYGONS;
	}
	else if(lineRadioButton->isChecked())
	{
		fc._rep = TeLINES;
	}
	else if(pointRadioButton->isChecked())
	{
		fc._rep = TePOINTS;
	}
	else if(cellRadioButton->isChecked())
	{
		fc._rep = TeCELLS;
	}
	else if(rasterRadioButton->isChecked())
	{
		fc._rep = TeRASTER;
	}

	fc._strategy = _fillCellOp->getCellOpType(opComboBox->currentText().latin1())._opStrategy;

	if(rasterRadioButton->isChecked())
	{
		fc._useDummy = ignoreCheckBox->isChecked();

		if(fc._useDummy)
		{
			fc._dummy = ignoreLineEdit->text().toDouble();	
		}
	}
	else
	{
		if( !(fc._strategy == CellOp_MinimumDistanceStrategy ||
			fc._strategy == CellOp_PresenceStrategy ||
			fc._strategy == CellOp_CountObjectsStrategy ||
			fc._strategy == CellOp_TotalAreaPercentageStrategy) )
		{
			fc._dataTableName = tableVecComboBox->currentText().latin1();
			fc._dataAttrTableName = attrTableVecComboBox->currentText().latin1();
			
			fc._ignoreValue = ignoreCheckBox->isChecked();

			if(fc._ignoreValue)
			{
				fc._valueIgnored = ignoreLineEdit->text().latin1();
			}
		}
	}
	 
	//second page
	fc._layerCellName = cellLayerComboBox->currentText().latin1();

	if(selectTableRadioButton->isChecked())
	{
		fc._cellTableName = selectTableComboBox->currentText().latin1();
	}

	if(createTableRadioButton->isChecked())
	{
		fc._cellTableName = createTableLineEdit->text().latin1();
	}

	if(attrButtonGroup->isEnabled())
	{
		if(createAttrRadioButton->isChecked())
		{
			fc._cellAttrTableName = createAttrNameLineEdit->text().latin1();

			if(selectTableRadioButton->isChecked())
			{
				fc._cellTableName = selectTableComboBox->currentText().latin1();

				std::string errorMessage = "";

				if(!_fillCellUtils->checkAttributeName(	cellLayerComboBox->currentText().latin1(), 
														selectTableComboBox->currentText().latin1(), 
														createAttrNameLineEdit->text().latin1(), errorMessage))
				{
					QMessageBox::warning(this, tr("Warning"), tr("Attribute Error: ") + errorMessage.c_str());
					return;
				}
			}

			fc._usecellDefaultValue = _useDefaultValue;

			if(fc._usecellDefaultValue)
			{
				fc._cellDefaultValue = createAttrDefaultValueLineEdit->text().latin1();
			}
		}

		if(useAttrRadioButton->isChecked())
		{
			fc._cellAttrTableName = useAttrComboBox->currentText().latin1();
		}
	}

	//third page
	fc._useTemporalTable = isTemporal;

	if(fc._useTemporalTable)
	{
		if(createTimeRadioButton->isChecked())
		{

			if(initialDateEdit->date() > finalDateEdit->date())
			{
				QMessageBox::warning(this, tr("Warning"), tr("Initial date is greater than final date."));
				return;
			}

			QString dateFormat("dd/MM/yyyy");
			QString timeFormat("hh:mm:ss");

			std::string maskText = "";
			std::string initialTime = "";
			std::string finalTime = "";

			TeChronon c;

			if(setHourCheckBox->isChecked())
			{
				if(initialDateEdit->date() == finalDateEdit->date())
				{
					if(initialTimeEdit->time() > finalTimeEdit->time())
					{
						QMessageBox::warning(this, tr("Warning"), tr("Initial time is greater than final time."));
						return;
					}
				}

				maskText = "DDsMMsYYYY HHsmmsSS";

				initialTime = initialDateEdit->date().toString(dateFormat).latin1();
				initialTime += " ";
				initialTime += initialTimeEdit->time().toString(timeFormat).latin1();

				finalTime = finalDateEdit->date().toString(dateFormat).latin1();
				finalTime += " ";
				finalTime += finalTimeEdit->time().toString(timeFormat).latin1();

				c = TeSECOND;
			}
			else
			{
				maskText = "DDsMMsYYYY";

				initialTime = initialDateEdit->date().toString(dateFormat).latin1();
				finalTime = finalDateEdit->date().toString(dateFormat).latin1();

				c = TeDAY;
			}

			std::string dataSeparator = "/";
			std::string timeSeparator = ":";
			std::string ampmFormat = "AM-PM";

			TeTimeInterval t(initialTime, finalTime, c, maskText, dataSeparator, timeSeparator, ampmFormat);

			if(!t.isValid())
			{
				QMessageBox::warning(this, tr("Warning"), tr("Time format is not right."));
				return;
			}

			fc._timeInterval = t;
		}

		if(selectTimeRadioButton->isChecked())
		{
			std::string strTimeInterval = selectTimeComboBox->currentText().latin1();

			TeTimeInterval t;

			t = _fillCellUtils->getTimeInterval(fc._layerCellName, fc._cellTableName, strTimeInterval);

			if(!t.isValid())
			{
				QMessageBox::warning(this, tr("Warning"), tr("Time selected is not right."));
				return;
			}

			fc._timeInterval = t;

		}
	}

	plugin_params_.teqtgrid_ptr_->clearPortal();

//execute operation
	if(!fc.executeOperation())
	{
		std::string error = fc.getErrorMessage();

		QMessageBox::critical(this, tr("Error"), error.c_str());
		return;
	}
	else
	{
		if(createTimeRadioButton->isChecked())
		{
			if(!_fillCellUtils->updateTheme(cellLayerComboBox->currentText().latin1()))
			{
				QMessageBox::warning(this, tr("Warning"), tr("Update theme error."));
			}
		}

		QMessageBox::information(this, tr("Information"), tr("Operation done."));

		plugin_params_.updateTVInterface();

		accept();
	}
}

///////////////////////////////////////////////////////////////////////////////////
/////// Fill Cell UI - First Page
///////////////////////////////////////////////////////////////////////////////////

void FillCellDlg::themeComboBox_activated(const QString& themeName)
{
//adjust interface
	polyRadioButton->setChecked(false); polyRadioButton->setEnabled(false);
	lineRadioButton->setChecked(false); lineRadioButton->setEnabled(false);
	pointRadioButton->setChecked(false); pointRadioButton->setEnabled(false);
	cellRadioButton->setChecked(false); cellRadioButton->setEnabled(false);
	rasterRadioButton->setChecked(false); rasterRadioButton->setEnabled(false);

	opGroupBox->setEnabled(false);
	opComboBox->clear();

	vecAttrGroupBox->setEnabled(false);
	tableVecComboBox->clear();
	attrTableVecComboBox->clear();

	ignoreGroupBox->setEnabled(false);
	ignoreCheckBox->setChecked(false);
	ignoreLineEdit->clear();
	
//

	if(themeName.isEmpty())
	{
		return;
	}

	if(!_fillCellUtils)
	{
		return;
	}

	TeTheme* theme = _fillCellUtils->getTheme(themeName.latin1());

	if(theme)
	{
		bool autoSelect = false;


		if(theme->layer()->vectRepres().size() == 1)
		{
			autoSelect = true;
		}

		if(theme->layer()->hasGeometry(TeRASTER))
		{
			rasterRadioButton->setEnabled(true);

			if(autoSelect)
			{
				rasterRadioButton->setChecked(true);

				geomRepButtonGroup_clicked(geomRepButtonGroup->selectedId());
			}
		}
		
		if(theme->layer()->hasGeometry(TePOLYGONS))
		{
			polyRadioButton->setEnabled(true);

			if(autoSelect)
			{
				polyRadioButton->setChecked(true);

				geomRepButtonGroup_clicked(geomRepButtonGroup->selectedId());
			}
		}

		if(theme->layer()->hasGeometry(TeLINES))
		{
			lineRadioButton->setEnabled(true);

			if(autoSelect)
			{
				lineRadioButton->setChecked(true);

				geomRepButtonGroup_clicked(geomRepButtonGroup->selectedId());
			}
		}

		if(theme->layer()->hasGeometry(TePOINTS))
		{
			pointRadioButton->setEnabled(true);

			if(autoSelect)
			{
				pointRadioButton->setChecked(true);

				geomRepButtonGroup_clicked(geomRepButtonGroup->selectedId());
			}
		}

		if(theme->layer()->hasGeometry(TeCELLS))
		{
			cellRadioButton->setEnabled(true);

			if(autoSelect)
			{
				cellRadioButton->setChecked(true);

				geomRepButtonGroup_clicked(geomRepButtonGroup->selectedId());
			}
		}
	}
}

void FillCellDlg::geomRepButtonGroup_clicked(int value)
{
//adjust interface
	vecAttrGroupBox->setEnabled(false);
	tableVecComboBox->clear();
	attrTableVecComboBox->clear();

	ignoreGroupBox->setEnabled(false);
	ignoreCheckBox->setChecked(false);
	ignoreLineEdit->clear();
//

	if(!_fillCellOp)
	{
		return;
	}

	defaultValueTextLabel->setText(tr("Default Value"));

	std::vector<std::string> vecOp;

	if(value == 0) //polygon
	{
		vecOp = _fillCellOp->getPolygonOperations();
	}
	else if(value == 1) //line
	{
		vecOp = _fillCellOp->getLineOperations();
	}
	else if(value == 2) //point
	{
		vecOp = _fillCellOp->getPointOperations();
	}
	else if(value == 3) //cell
	{
		vecOp = _fillCellOp->getCellOperations();
	}
	else if(value == 4) //raster
	{
		vecOp = _fillCellOp->getRasterOperations();
		defaultValueTextLabel->setText(tr("Dummy Value"));
	}

	opComboBox->clear();
	opComboBox->insertItem("");

	for(unsigned int i = 0; i < vecOp.size(); ++i)
	{
		opComboBox->insertItem(vecOp[i].c_str());
	}

	opGroupBox->setEnabled(true);
}

void FillCellDlg::opComboBox_activated(const QString& operation)
{
//adjust interface
	vecAttrGroupBox->setEnabled(false);
	tableVecComboBox->clear();
	attrTableVecComboBox->clear();

	ignoreGroupBox->setEnabled(false);
	ignoreCheckBox->setChecked(false);
	ignoreLineEdit->clear();
//

//check parameters
	if(operation.isEmpty())
	{
		return;
	}

	if(themeComboBox->currentText().isEmpty())
	{
		return;
	}

	if(!_fillCellUtils)
	{
		return;
	}

	if(!_fillCellOp)
	{
		return;
	}

//get parameters

	//theme
	std::string themeName = themeComboBox->currentText().latin1();
	
	TeTheme* theme = _fillCellUtils->getTheme(themeName);

	if(!theme)
	{
		return;
	}

	//geom rep
	TeGeomRep curGeomRep;

	if(polyRadioButton->isChecked())
	{
		curGeomRep = TePOLYGONS;
	}
	else if(lineRadioButton->isChecked())
	{
		curGeomRep = TeLINES;
	}
	else if(pointRadioButton->isChecked())
	{
		curGeomRep = TePOINTS;
	}
	else if(cellRadioButton->isChecked())
	{
		curGeomRep = TeCELLS;
	}
	else if(rasterRadioButton->isChecked())
	{
		curGeomRep = TeRASTER;
	}
	else
	{
		return;
	}

	//cell operation
	CellOpType opType = _fillCellOp->getCellOpType(operation.latin1());

//

	if(curGeomRep & TeRASTER)
	{
		attrButtonGroup->setEnabled(true); 

		if(opType._opStrategy == CellOp_CategoryPercentageStrategy) //porcentagem da classe
		{
			ignoreGroupBox->setEnabled(false);
			ignoreCheckBox->setChecked(false);
			ignoreLineEdit->clear();

			createAttrDefaultValueLineEdit->clear();
			createAttrDefaultValueLineEdit->setEnabled(false);

			_useDefaultValue = false;
		}
		else
		{
			ignoreGroupBox->setEnabled(true);
			ignoreCheckBox->setChecked(true);
			ignoreLineEdit->clear();

			ignoreCheckBox_clicked();
			
			createAttrDefaultValueLineEdit->clear();
			createAttrDefaultValueLineEdit->setEnabled(true);

			_useDefaultValue = true;
		}
	}
	else
	{
		attrButtonGroup->setEnabled(true);

		vecAttrGroupBox->setEnabled(true);

		fillAttrTable();

		if( opType._opStrategy == CellOp_MinimumDistanceStrategy ||
			opType._opStrategy == CellOp_PresenceStrategy ||
			opType._opStrategy == CellOp_CountObjectsStrategy ||
			opType._opStrategy == CellOp_TotalAreaPercentageStrategy )
		{
			vecAttrGroupBox->setEnabled(false);
			tableVecComboBox->clear();
			attrTableVecComboBox->clear();

			ignoreGroupBox->setEnabled(false);
			ignoreCheckBox->setChecked(false);
			ignoreLineEdit->clear();

			createAttrDefaultValueLineEdit->clear();
			createAttrDefaultValueLineEdit->setEnabled(false);

			_useDefaultValue = false;
		}
		else
		{
			if( opType._opStrategy == CellOp_CategoryPercentageStrategy ||
			    opType._opStrategy == CellOp_AverageWeighByAreaStrategy ||
				opType._opStrategy == CellOp_SumWeighByAreaStrategy)
			{
				ignoreGroupBox->setEnabled(false);
				ignoreCheckBox->setChecked(false);
				ignoreLineEdit->clear();

				createAttrDefaultValueLineEdit->clear();
				createAttrDefaultValueLineEdit->setEnabled(false);

				_useDefaultValue = false;
			}
			else
			{
				if(opType._opStrategy == CellOp_CategoryAreaPercentageStrategy)
				{
					attrButtonGroup->setEnabled(false);
				}

				ignoreGroupBox->setEnabled(true);
				ignoreCheckBox->setChecked(false);
				ignoreLineEdit->setEnabled(false);
				ignoreLineEdit->clear();

				createAttrDefaultValueLineEdit->clear();
				createAttrDefaultValueLineEdit->setEnabled(true);

				_useDefaultValue = true;
			}

		}
	}
	
}

void FillCellDlg::tableVecComboBox_activated(const QString& tableName)
{
//check parameters
	if(tableName.isEmpty())
	{
		return;
	}

	if(opComboBox->currentText().isEmpty())
	{
		return;
	}

	if(themeComboBox->currentText().isEmpty())
	{
		return;
	}

	if(!_fillCellUtils)
	{
		return;
	}

	if(!_fillCellOp)
	{
		return;
	}

//get parameters
	std::string themeName = themeComboBox->currentText().latin1();
	std::string opName = opComboBox->currentText().latin1();

	TeAttributeList list = _fillCellUtils->getThemeAttrList(themeName, tableName.latin1());

	std::vector<std::string> vecList = _fillCellOp->getCompatibleAttributes(opName, list);

//fill attr combo
	attrTableVecComboBox->clear();

	for(unsigned int i = 0; i < vecList.size(); ++i)
	{
		attrTableVecComboBox->insertItem(vecList[i].c_str());
	}
}

void FillCellDlg::ignoreCheckBox_clicked()
{
	ignoreLineEdit->setEnabled(ignoreCheckBox->isChecked());

	if(ignoreCheckBox->isChecked() && rasterRadioButton->isChecked() && _fillCellUtils)
	{
		std::string themeName = themeComboBox->currentText().latin1();
	
		TeTheme* theme = _fillCellUtils->getTheme(themeName);

		if(!theme)
		{
			return;
		}

		if(theme->layer() && theme->layer()->raster())
		{
			double value = theme->layer()->raster()->params().dummy_[0];

			QString dummyStr;
			dummyStr.setNum(value);

			ignoreLineEdit->setText(dummyStr);
		}
	}
}

void FillCellDlg::nextOnePushButton_clicked()
{
//check interface
	if(themeComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Theme not defined."));
		return;
	}

	if(opComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Operation not defined."));
		return;
	}

	if(ignoreCheckBox->isChecked() && ignoreLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Value to ignore not defined."));
		return;
	}

	if(attrTableVecComboBox->isEnabled())
	{
		if(attrTableVecComboBox->currentText().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Attribute not defined."));
			return;
		}
	}

//automatic activate the cell layer if only one exist
	if(cellLayerComboBox->currentText().isEmpty() && cellLayerComboBox->count() == 2)
	{
		cellLayerComboBox->setCurrentItem(1);

		cellLayerComboBox_activated(cellLayerComboBox->currentText());
	}

	widgetStack->raiseWidget(widgetStack->widget(1));
}

///////////////////////////////////////////////////////////////////////////////////
/////// Fill Cell UI - Second Page
///////////////////////////////////////////////////////////////////////////////////

void FillCellDlg::cellLayerComboBox_activated(const QString& cellLayerName)
{
//reset interface
	tableButtonGroup->setEnabled(false);
	selectTableRadioButton->setChecked(false);
	selectTableComboBox->clear();
	createTableRadioButton->setChecked(false);
	createTableLineEdit->clear();

//	attrButtonGroup->setEnabled(false);
	createAttrRadioButton->setChecked(false);
	createAttrNameLineEdit->clear();
	createAttrDefaultValueLineEdit->clear();
	useAttrRadioButton->setChecked(false);
	useAttrComboBox->clear();

//check parameters
	if(cellLayerName.isEmpty())
	{
		return;
	}

	if(!_fillCellUtils)
	{
		return;
	}

//get cell layer attr tables
	std::vector<std::string> vecAttr = _fillCellUtils->getLayerAttrTables(cellLayerName.latin1());

//fill cell attr tables combo box
	selectTableComboBox->clear();

	for(unsigned int i = 0; i < vecAttr.size(); ++i)
	{
		selectTableComboBox->insertItem(vecAttr[i].c_str());
	}

//by default
	tableButtonGroup->setEnabled(true);
	selectTableRadioButton->setChecked(true);
	selectTableRadioButton_clicked();

	if(!selectTableComboBox->currentText().isEmpty())
	{
		selectTableComboBox_activated(selectTableComboBox->currentText());
	}
}

void FillCellDlg::selectTableRadioButton_clicked()
{
	selectTableComboBox->setEnabled(true);
	createTableLineEdit->setEnabled(false);

	if(!selectTableComboBox->currentText().isEmpty())
	{
		selectTableComboBox_activated(selectTableComboBox->currentText());
	}
}

void FillCellDlg::selectTableComboBox_activated(const QString& tableName)
{
//check parameters
	if(cellLayerComboBox->currentText().isEmpty())
	{
		return;
	}

	if(tableName.isEmpty())
	{
		return;
	}

	if(!_fillCellUtils)
	{
		return;
	}

//get cell layer attr lsit
	TeAttributeList list = _fillCellUtils->getLayerAttrList(cellLayerComboBox->currentText().latin1(), tableName.latin1());

//fill cell attr list combo box
	useAttrComboBox->clear();

	TeAttributeList::iterator it;
	for(it = list.begin(); it != list.end(); ++it)
	{
		useAttrComboBox->insertItem(it->rep_.name_.c_str());
	}

	bool isTemporal = _fillCellUtils->tableIsTemporal(cellLayerComboBox->currentText().latin1(), tableName.latin1());

	//attrButtonGroup->setEnabled(true);

	if(isTemporal)
	{
		fillTemporalAttributes();

		useAttrRadioButton->setEnabled(true);
		useAttrRadioButton->setChecked(true);
		useAttrRadioButton_clicked();

		nextTwoPushButton->show();
		okPushButton->hide();
	}
	else
	{
		useAttrRadioButton->setEnabled(true);

		createAttrRadioButton->setChecked(true);
		createAttrRadioButton_clicked();

		nextTwoPushButton->hide();
		okPushButton->show();
	}	
}

void FillCellDlg::createTableRadioButton_clicked()
{
	createTableLineEdit->setEnabled(true);
	selectTableComboBox->setEnabled(false);

	attrButtonGroup->setEnabled(true);
	
	createAttrRadioButton->setChecked(true);
	createAttrRadioButton_clicked();
	useAttrRadioButton->setEnabled(false);

	createTableLineEdit->setFocus();

	selectTimeRadioButton->setChecked(false);
	selectTimeRadioButton->setEnabled(false);
	selectTimeComboBox->setEnabled(false);

	nextTwoPushButton->show();
	okPushButton->hide();
}

void FillCellDlg::createAttrRadioButton_clicked()
{
	createAttrNameLineEdit->setEnabled(true);

	if(_useDefaultValue)
	{
		createAttrDefaultValueLineEdit->setEnabled(true);
	}

	createAttrNameLineEdit->setFocus();

	useAttrComboBox->setEnabled(false);
}

void FillCellDlg::useAttrRadioButton_clicked()
{
	useAttrComboBox->setEnabled(true);

	useAttrComboBox->setFocus();
	
	createAttrNameLineEdit->setEnabled(false);
	createAttrDefaultValueLineEdit->setEnabled(false);
}

void FillCellDlg::previousTwoPushButton_clicked()
{
	widgetStack->raiseWidget(widgetStack->widget(0));
}

void FillCellDlg::nextTwoPushButton_clicked()
{
//check interface
	if(cellLayerComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Cell Layer not defined."));
		return;
	}

	if(selectTableRadioButton->isChecked())
	{
		if(selectTableComboBox->currentText().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Table not selected."));
			return;
		}
	}

	if(createTableRadioButton->isChecked())
	{
		if(createTableLineEdit->text().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Table name not defined."));
			return;
		}
	}

	if(createAttrRadioButton->isChecked())
	{
		if(createAttrNameLineEdit->text().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Attribute table name not defined."));
			return;
		}
	}

	if(useAttrRadioButton->isChecked())
	{
		if(useAttrComboBox->currentText().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Attribute table name not selected."));
			return;
		}
	}

	createTimeRadioButton->setChecked(true);

	createTimeRadioButton_clicked();

	widgetStack->raiseWidget(widgetStack->widget(2));
}

void FillCellDlg::okPushButton_clicked()
{
//check interface
	if(cellLayerComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Cell Layer not defined."));
		return;
	}

	if(selectTableRadioButton->isChecked())
	{
		if(selectTableComboBox->currentText().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Table not selected."));
			return;
		}
	}

	if(createTableRadioButton->isChecked())
	{
		if(createTableLineEdit->text().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Table name not defined."));
			return;
		}
	}

	if(attrButtonGroup->isEnabled())
	{
		if(createAttrRadioButton->isChecked())
		{
			if(createAttrNameLineEdit->text().isEmpty())
			{
				QMessageBox::warning(this, tr("Warning"), tr("Attribute table name not defined."));
				return;
			}

			if(_useDefaultValue)
			{
				if(createAttrDefaultValueLineEdit->text().isEmpty())
				{
					QMessageBox::warning(this, tr("Warning"), tr("Attribute default value not defined."));
					return;
				}
			}
		}

		if(useAttrRadioButton->isChecked())
		{
			if(useAttrComboBox->currentText().isEmpty())
			{
				QMessageBox::warning(this, tr("Warning"), tr("Attribute table name not selected."));
				return;
			}
		}
	}

	execute(false);
}

///////////////////////////////////////////////////////////////////////////////////
/////// Fill Cell UI - Third Page
///////////////////////////////////////////////////////////////////////////////////

void FillCellDlg::createTimeRadioButton_clicked()
{
	initialDateEdit->setEnabled(true);
	initialTimeEdit->setEnabled(setHourCheckBox->isChecked());
	
	finalDateEdit->setEnabled(true);
	finalTimeEdit->setEnabled(setHourCheckBox->isChecked());

	setHourCheckBox->setEnabled(true);

	selectTimeComboBox->setEnabled(false);
}

void FillCellDlg::selectTimeRadioButton_clicked()
{
    selectTimeComboBox->setEnabled(true);

	initialDateEdit->setEnabled(false);
	initialTimeEdit->setEnabled(false);
	
	finalDateEdit->setEnabled(false);
	finalTimeEdit->setEnabled(false);

	setHourCheckBox->setEnabled(false);
}

void FillCellDlg::setHourCheckBox_clicked()
{
	initialTimeEdit->setEnabled(setHourCheckBox->isChecked());
	finalTimeEdit->setEnabled(setHourCheckBox->isChecked());
}

void FillCellDlg::previousThreePushButton_clicked()
{
	widgetStack->raiseWidget(widgetStack->widget(1));
}

void FillCellDlg::ok2PushButton_clicked()
{
//check interface
	if(createTimeRadioButton->isChecked())
	{
		if(!initialDateEdit->date().isValid())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Initial date not valid."));
			return;
		}

		if(!finalDateEdit->date().isValid())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Final date not valid."));
			return;
		}

		if(setHourCheckBox->isChecked())
		{
			if(!initialTimeEdit->time().isValid())
			{
				QMessageBox::warning(this, tr("Warning"), tr("Initial time not valid."));
				return;
			}

			if(!finalTimeEdit->time().isValid())
			{
				QMessageBox::warning(this, tr("Warning"), tr("Final time not valid."));
				return;
			}
		}
	}

	if(selectTimeRadioButton->isChecked())
	{
		if(selectTimeComboBox->currentText().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Time not selected."));
			return;
		}
	}

	execute(true);
}

void FillCellDlg::helpPushButton_clicked()
{
	if(_help)
	{
		delete _help;
	}

	_help = new Help(this, "help", false);

	_help->init("fillCell.html");

	if(_help->erro_ == false)
	{
		_help->show();
		_help->raise();
	}
	else
	{
		delete _help;
		_help = 0;
	}
}