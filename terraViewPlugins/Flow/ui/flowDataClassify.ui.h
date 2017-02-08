/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright � 2001-2004 INPE and Tecgraf/PUC-Rio.     

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


#include "TeAppTheme.h" 
#include "TeTable.h"
#include <TeQtViewsListView.h>
#include <TeQtDatabaseItem.h>
#include <TeQtDatabasesListView.h>
#include <TeQtLayerItem.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include "TeFlowDataClassifier.h"
#include "TeFlowMetadata.h"
#include "TeFlowUtils.h"
#include <TeQtGrid.h>
#include <TeApplicationUtils.h>
#include <TeDatabaseFactory.h>
#include <TeDatabaseFactoryParams.h>

#include <qmessagebox.h>
#include <qstring.h>
#include <qsettings.h>
#include <qapplication.h>
#include <qfiledialog.h>

bool FlowClassify::init(PluginParameters * plug_pars_ptr)
{
	plug_pars_ptr_ = plug_pars_ptr;

	updateInterface(plug_pars_ptr_);

	TeFlowUtils flowUtils(local_db_ptr_.nakedPointer());
	
	TeFlowMetadata flowMetadata(local_db_ptr_.nakedPointer());

	
	if(!flowMetadata.loadMetadata(currentTheme_->layer()->id()))
	{
		QMessageBox::warning(this, tr("Warning"), flowMetadata.errorMessage().c_str());

		return false;
	}
	
// Preencher os campos do formul�rio
// Textos de origem e destino
	originLineEdit->setText(flowMetadata.flowTableOriginColumnName_.c_str());
	destinyLineEdit->setText(flowMetadata.flowTableDestinyColumnName_.c_str());

// Campos que servir�o de peso
	TeAttributeList attList = currentTheme_->sqlAttList();

	TeAttributeList::iterator it = attList.begin();

	TeAttrTableVector& attrs = currentTheme_->attrTables();

	unsigned int nTables = attrs.size();

	for(unsigned int i = 0; i < nTables; ++i)
	{
		if(attrs[i].tableType() == TeAttrStatic)
		{
			TeAttributeList& attList = attrs[i].attributeList();

			for(unsigned int j = 0; j < attList.size(); ++j)
			{
				if(attList[j].rep_.type_ == TeINT || attList[j].rep_.type_ == TeREAL)
				{
					string columnName = attrs[i].name() + "." + attList[j].rep_.name_;

					weightComboBox->insertItem(columnName.c_str());
					dominanceColumnComboBox->insertItem(columnName.c_str());
				}
			}
		}
	}

	if(weightComboBox->count() <= 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Current Theme has no attribute!"));
	
		return false;
	}

	weightComboBox->setEnabled(true);	

// Listagem dos temas de refer�ncia
	vector<string> themes;
	
	if(!flowUtils.listReferenceThemes(flowMetadata.referenceLayerId_, themes) || (themes.size() == 0))
	{
		QMessageBox::warning(this, tr("Warning"), flowUtils.errorMessage().c_str());
	
		return false;
	}

	for(unsigned int i = 0; i < themes.size(); ++i)
	{
		flowDominanceThemeComboBox->insertItem(themes[i].c_str());
		flowDominanceThemeComboBox_2->insertItem(themes[i].c_str());
		flowDominanceThemeComboBox_3->insertItem(themes[i].c_str());
	}

	flowDominanceThemeComboBox->setEnabled(true);
	flowDominanceThemeComboBox->setCurrentItem(0);
	flowDominanceThemeComboBox_activated(0);	
	flowDominanceThemeComboBox_2->setEnabled(true);
	flowDominanceThemeComboBox_2->setCurrentItem(0);
	flowDominanceThemeComboBox_3->setEnabled(true);
	flowDominanceThemeComboBox_3->setCurrentItem(0);
	
	if(dominanceColumnComboBox->count() >= 0)
		dominanceColumnComboBox->setEnabled(true);
	return true;
}


void FlowClassify::destroy()
{
	
}


void FlowClassify::flowDominanceThemeComboBox_activated( int )
{
	themeDominanceColumnComboBox->clear();
	themeDominanceColumnComboBox->setEnabled(false);

	TeThemeMap& themes = local_db_ptr_.nakedPointer()->themeMap();
	TeThemeMap::iterator it = themes.begin();
	TeTheme* theme = 0;

	string dominanceThemeName = flowDominanceThemeComboBox->currentText().latin1();

	while(it != themes.end())
	{
		if(it->second->name() == dominanceThemeName)
		{
			theme = (TeTheme*)it->second;
			break;
		}

		++it;
	}

	if(!theme)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Selected Theme not found."));

		return;
	}

	TeAttrTableVector& attrs = theme->attrTables();

	int numCols = theme->sqlAttList().size();

	unsigned int nTables = attrs.size();
	
	for(unsigned int i = 0; i < nTables; ++i)
	{
		if(numCols >= 2)
		{
			for(int j = 0; j < numCols; ++j)
			{
				if(theme->sqlAttList()[j].rep_.type_ == TeREAL || theme->sqlAttList()[j].rep_.type_ == TeINT)
				{
					string name = attrs[i].name() + "." + theme->sqlAttList()[j].rep_.name_;
					themeDominanceColumnComboBox->insertItem(name.c_str());
				}
			}

			if(themeDominanceColumnComboBox->count() > 0)
				themeDominanceColumnComboBox->setEnabled(true);
		}
		else
		{
			QMessageBox::warning(this, tr("Warning"), tr("Selected Theme can not be used!"));

			return;
		}
	}
}


void FlowClassify::generatePushButton_clicked()
{
	//////////////////////////////////////////////////

	std::string refTheme = "";
	std::string dominanceColumn = "";
	std::string domValueType = "";

	std::string outputThemeName = dominanceThemeLineEdit->text().latin1();
	std::string weightColumn = (weightComboBox->currentText().isEmpty()? "": weightComboBox->currentText().latin1());

	std::string dominanceInfo_flow = (flowTableRadioButton->isChecked()? "YES": "NO");

	if(flowTableRadioButton->isChecked())
	{
		refTheme = flowDominanceThemeComboBox_2->currentText().latin1();
		dominanceColumn = dominanceColumnComboBox->currentText().latin1();
	}

	std::string dominanceInfo_ref = (referenceThemeRadioButton->isChecked()? "YES": "NO");

	if(referenceThemeRadioButton->isChecked())
	{
		refTheme = flowDominanceThemeComboBox->currentText().latin1();
		dominanceColumn = themeDominanceColumnComboBox->currentText().latin1();
	}

	std::string dominanceInfo_calc = (calcDomRadioButton->isChecked()? "YES": "NO");

	if(calcDomRadioButton->isChecked())
	{
		refTheme = flowDominanceThemeComboBox_3->currentText().latin1();
		dominanceColumn = "";
		domValueType = (tot1RadioButton->isChecked()? "YES": "NO");
	}

	std::string checkLocalDominanceStr = (verifyLocalRelCheckBox->isChecked()? "YES": "NO");

	bool checkLocalDominance = verifyLocalRelCheckBox->isChecked();

	std::string localDom = "";

	if(checkLocalDominance)
	{
		localDom = domLocalRelLineEdit->text().latin1();
	}

	std:string otherFlowDom = domRelLineEdit->text().latin1();


	//generate LOG file
	QString msg = tr("Save Log File?");
	QString caption = tr("Terra View - FLow Plugin.");
	

	int response = QMessageBox::question(this, caption, msg, tr("Yes"), tr("No"));

	if(response == 0)
	{
		QString qfileName = QFileDialog::getSaveFileName("", "Text File(*.txt)", this, 0, tr( "Set Log File Name"));

		if (qfileName.isEmpty())
		{
			return;
		}

		std::string fileName = qfileName.latin1();

		std::string ext = TeGetExtension(fileName.c_str());

		if(ext.empty())
		{
			fileName += ".txt";
		}

		TeWriteToFile(fileName, "TerraView Flow Plugin - Generate Flow Network - LOG\n\n", "w");
		TeWriteToFile(fileName, "Reference theme: " + refTheme + "\n", "a+");
		TeWriteToFile(fileName, "Output theme name: " + outputThemeName + "\n", "a+");
		TeWriteToFile(fileName, "Weight column name: " + weightColumn + "\n", "a+");
		TeWriteToFile(fileName, "Dominance information:\n", "a+");
		TeWriteToFile(fileName, "\tIs from Flow table: " + dominanceInfo_flow + "\n", "a+");
		TeWriteToFile(fileName, "\tIs from Reference theme: " + dominanceInfo_ref + "\n", "a+");
		TeWriteToFile(fileName, "\tIs calculated: " + dominanceInfo_calc + "\n", "a+");
		TeWriteToFile(fileName, "\tDominance column: " + dominanceColumn + "\n", "a+");
		TeWriteToFile(fileName, "\tUse output flow: " + domValueType + "\n", "a+");
		TeWriteToFile(fileName, "Check local dominance: " + checkLocalDominanceStr + "\n", "a+");
		TeWriteToFile(fileName, "Local dominance value: " + localDom + "\n", "a+");
		TeWriteToFile(fileName, "Other Flows dominance value: " + otherFlowDom + "\n", "a+");
	}

	//generate register
	QSettings settings;

	settings.beginGroup("TerraView");
	settings.beginGroup("Flow Plugin");

	settings.writeEntry("ref_theme", refTheme.c_str());
	settings.writeEntry("weight_column", weightColumn.c_str());
	settings.writeEntry("dom_from_flow", dominanceInfo_flow.c_str());
	settings.writeEntry("dom_from_ref", dominanceInfo_ref.c_str());
	settings.writeEntry("dom_calc", dominanceInfo_calc.c_str());
	settings.writeEntry("dom_column", dominanceColumn.c_str());
	settings.writeEntry("use_out_flow", domValueType.c_str());
	settings.writeEntry("check_local_flow", checkLocalDominanceStr.c_str());
	settings.writeEntry("local_dom_value", localDom.c_str());
	settings.writeEntry("others_dom_value", otherFlowDom.c_str());
	
	settings.endGroup();
	settings.endGroup();

	//////////////////////////////////////////////////

	TeFlowMetadata flowMetadata(local_db_ptr_.nakedPointer());

	TeFlowUtils flowUtils(local_db_ptr_.nakedPointer());

	weightColumn = (weightComboBox->currentText().isEmpty()? "": weightComboBox->currentText().latin1());

	if(weightColumn.empty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Weight attribute not defined!"));

		return;
	}

	if(dominanceThemeLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Output theme name not defined!"));

		return;
	}

	string baseThemeName = dominanceThemeLineEdit->text().latin1();
	string theme1Name = baseThemeName + "_main_flow";		       
	string theme2Name = baseThemeName + "_level_0";		       

	if(local_db_ptr_.nakedPointer()->themeExist(theme1Name) || local_db_ptr_.nakedPointer()->themeExist(theme2Name)) 
	{
		QMessageBox::warning(this, tr("Warning"), tr("Invalid theme name!"));

		return;
	}

	bool calculateDominance = false;
	bool tot1 = true;

	if(flowTableRadioButton->isChecked())
	{
		refTheme = flowDominanceThemeComboBox_2->currentText().latin1();

		dominanceColumn = dominanceColumnComboBox->currentText().latin1();

		if(dominanceColumn.empty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Dominance Column not defined!"));

			return;
		}

	}
	else if(referenceThemeRadioButton->isChecked())
	{
		refTheme = flowDominanceThemeComboBox->currentText().latin1();

		dominanceColumn = themeDominanceColumnComboBox->currentText().latin1();

		if(refTheme.empty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Reference theme not defined!"));

			return;
		}

		if(dominanceColumn.empty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Dominance Column not defined!"));

			return;
		}
	}
	else if(calcDomRadioButton->isChecked())
	{
		refTheme = flowDominanceThemeComboBox_3->currentText().latin1();

		calculateDominance = true;

		tot1 = tot1RadioButton->isChecked();

		dominanceColumn = "";

		if(refTheme.empty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Reference theme not defined!"));

			return;
		}

	}

	bool ok = true;

	checkLocalDominance = verifyLocalRelCheckBox->isChecked();

	double dominanceLocalRelation = 0.;

	if(checkLocalDominance)
	{
		dominanceLocalRelation = domLocalRelLineEdit->text().toDouble(&ok);

		if(!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Invalid value for Local Dominance Relation!"));
			return;
		}
	}

	double dominanceRelation = domRelLineEdit->text().toDouble(&ok);

	if(!ok)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Invalid value for Dominance Relation!"));
		return;
	}

	
	TeTheme* referenceTheme = flowUtils.getReferenceTheme(refTheme);
	
	TeDominantFlowNetwork flowDataClassifier(currentTheme_, referenceTheme);

	if(!flowDataClassifier.buildNetwork(weightColumn, dominanceColumn, originCheckBox->isChecked(), calculateDominance, tot1, dominanceRelation, checkLocalDominance, dominanceLocalRelation))
	{
		QMessageBox::warning(this, tr("Warning"), flowDataClassifier.errorMessage().c_str());
		return;
	}
	else
	{
		vector<string> disconnecteds;
		if(flowDataClassifier.checkConnections(disconnecteds))
		{
			    QString msg1 = tr("Exist disconnecteds nodes in network!");
				QString msg2 = tr("Nodes:") + " ";
					    msg2 += disconnecteds[0].c_str();
				if (disconnecteds.size() > 2)
				{
					msg2 += ", ";
					msg2 += disconnecteds[1].c_str();
					msg2 += ("...");
			}
			
				msg2 += " " + tr("are not connected to the network!") + "\n";
				msg2 +=	tr("Connect them?");

				int response = QMessageBox::question(this, msg1, msg2, tr("Yes"), tr("No"));

				if(response == 0)
				{
					flowDataClassifier.setAsRootLevel(disconnecteds);
					
				}
				else
				{
					
				}
		}
		
		TeQtGrid* grid = plug_pars_ptr_->teqtgrid_ptr_;
		if(grid == 0)
		{
			QMessageBox::critical(this, tr("Error"), tr("Grid is not initialized!"));
			return;
		}

		TeAttrTableVector ta; 
		currentTheme_->getAttTables(ta);
		int cpos = 0;
		unsigned int i = 0;  
		
		for(i = 0; i < ta.size(); i++)
		{
			cpos += ta[i].attributeList().size();

			if(TeConvertToUpperCase(ta[i].name()) == TeConvertToUpperCase(flowDataClassifier.getFlowTableName()))
				break;
		}

		grid->clearPortal();
		
		if(!flowDataClassifier.saveNetwork(dominanceOutputColumnNameLineEdit->text().latin1(), sumCheckBox->isChecked(), treeCheckBox->isChecked()))
		{
			QMessageBox::critical(this, tr("Error"), flowDataClassifier.errorMessage().c_str());

			grid->initPortal();
			grid->refresh();
			grid->goToLastColumn();
	
			return;
		}
	
		TeView* view = currentView_;
		TeAttrTableVector tablesVector;
		currentTheme_->getAttTables(tablesVector);
		string whereClause  = " ";
		       whereClause += flowDataClassifier.getFlowTableName();
			   whereClause += ".";
			   whereClause += string(dominanceOutputColumnNameLineEdit->text().latin1()) + "_main_flow";
		       whereClause += " >= 0 ";
/////////////////////////////////////////////////////////

		TeTheme* theme = new TeTheme(theme1Name);
		theme->layer (currentTheme_->layer());
		theme->visibleRep(currentTheme_->layer()->geomRep());
		view->add(theme);
		theme->visibility(true);
		theme->attributeRest(whereClause);

		TeAppTheme* themeClassify = new TeAppTheme(theme);

		themeClassify->canvasLegUpperLeft(currentTheme_->layer()->box().center());
			
		for (unsigned int i = 0; i < tablesVector.size(); ++i)
			theme->addThemeTable(tablesVector[i]);

		if(!insertAppTheme(local_db_ptr_.nakedPointer(),themeClassify))
		{
			QMessageBox::critical(this, tr("Error"),
			tr("Failed to insert new theme!"));
			return;
		}

		if (!theme->save())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error saving theme!"));
			return;
		}

		if (!theme->buildCollection())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error generating collection!"));
			return;
		}

		if(!theme->generateLabelPositions())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error generating label positions!"));
		}

		TeBox tbox = local_db_ptr_.nakedPointer()->getThemeBox(theme);
		theme->setThemeBox(tbox);
		local_db_ptr_.nakedPointer()->updateTheme(theme);

		std::string themeId = Te2String(theme->id()),
					sql = "UPDATE te_theme SET parent_id = " + themeId + " WHERE theme_id = " + themeId;   

		if(!local_db_ptr_.nakedPointer()->execute(sql))
		{
			return;
		}

		
///////////////////////////////////////////
		TeAttrTableVector tablesVector2;
		
		if(!flowMetadata.loadMetadata(currentTheme_->layer()->id()))
		{
			QMessageBox::warning(this, tr("Warning"), flowMetadata.errorMessage().c_str());

			return;
		}
				
		(local_db_ptr_->layerMap()[flowMetadata.referenceLayerId_])->getAttrTables(tablesVector2);
		
		std::string tableName = "";

		for(unsigned int i = 0; i < tablesVector2.size(); ++i)
		{
			if(tablesVector2[i].id() == flowMetadata.referenceTableId_)
			{
				tableName = tablesVector2[i].name();
			}
				break;
		}

		whereClause  = " ";
		whereClause += tableName;
		whereClause += ".";
		whereClause += string(dominanceOutputColumnNameLineEdit->text().latin1()) + "_net_level";
		whereClause += " = 0 ";

//////////////////////////////////////////

		TeTheme* theme2 = new TeTheme(theme2Name);
		theme2->layer (local_db_ptr_.nakedPointer()->layerMap()[flowMetadata.referenceLayerId_]);
		theme2->visibleRep(local_db_ptr_.nakedPointer()->layerMap()[flowMetadata.referenceLayerId_]->geomRep());
		view->add(theme2);
		theme2->visibility(true);
		theme2->attributeRest(whereClause);

		TeAppTheme* themeClassify2 = new TeAppTheme(theme2);

		themeClassify2->canvasLegUpperLeft(local_db_ptr_.nakedPointer()->layerMap()[flowMetadata.referenceLayerId_]->box().center());
			
		for (unsigned int i = 0; i < tablesVector2.size(); ++i)
			theme2->addThemeTable(tablesVector2[i]);

		if(!insertAppTheme(local_db_ptr_.nakedPointer(),themeClassify2))
		{
			QMessageBox::critical(this, tr("Error"),
			tr("Failed insert new theme!"));
			return;
		}

		if (!theme2->save())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error saving theme!"));
			return;
		}

		if (!theme2->buildCollection())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error generating collection!"));
			return;
		}

		if(!theme2->generateLabelPositions())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error generating label positions!"));
		}

		TeBox tbox2 = local_db_ptr_.nakedPointer()->getThemeBox(theme2);
		theme2->setThemeBox(tbox2);
		local_db_ptr_.nakedPointer()->updateTheme(theme2);


		std::string themeId2 = Te2String(theme2->id()),
					sql2 = "UPDATE te_theme SET parent_id = " + themeId2 + " WHERE theme_id = " + themeId2;   

		if(!local_db_ptr_.nakedPointer()->execute(sql2))
		{
			return;
		}

////////////////////////////////////////
		
		plug_pars_ptr_->updateTVInterface(*plug_pars_ptr_);

		accept();
	}
	
	
	return;
}



void FlowClassify::referenceThemeRadioButton_clicked()
{
	dominanceDataWidgetStack->raiseWidget(0);
}

void FlowClassify::flowTableRadioButton_clicked()
{
	dominanceDataWidgetStack->raiseWidget(1);
}

void FlowClassify::calcDomRadioButton_clicked()
{
	dominanceDataWidgetStack->raiseWidget(2);
}

void FlowClassify::classifyPushButton_clicked()
{

	//////////////////////////////////////////////////

	std::string refTheme = "";
	std::string dominanceColumn = "";
	std::string domValueType = "";

	std::string outputThemeName = dominanceThemeLineEdit->text().latin1();
	std::string weightColumn = (weightComboBox->currentText().isEmpty()? "": weightComboBox->currentText().latin1());

	std::string cutLevel = (cutLevelLineEdit->text().isEmpty()? "": cutLevelLineEdit->text().latin1());


	std::string dominanceInfo_flow = (flowTableRadioButton->isChecked()? "YES": "NO");

	if(flowTableRadioButton->isChecked())
	{
		refTheme = flowDominanceThemeComboBox_2->currentText().latin1();
		dominanceColumn = dominanceColumnComboBox->currentText().latin1();
	}

	std::string dominanceInfo_ref = (referenceThemeRadioButton->isChecked()? "YES": "NO");

	if(referenceThemeRadioButton->isChecked())
	{
		refTheme = flowDominanceThemeComboBox->currentText().latin1();
		dominanceColumn = themeDominanceColumnComboBox->currentText().latin1();
	}

	std::string dominanceInfo_calc = (calcDomRadioButton->isChecked()? "YES": "NO");

	if(calcDomRadioButton->isChecked())
	{
		refTheme = flowDominanceThemeComboBox_3->currentText().latin1();
		dominanceColumn = "";
		domValueType = (tot1RadioButton->isChecked()? "YES": "NO");
	}

	std::string checkLocalDominanceStr = (verifyLocalRelCheckBox->isChecked()? "YES": "NO");

	bool checkLocalDominance = verifyLocalRelCheckBox->isChecked();

	std::string localDom = "";

	if(checkLocalDominance)
	{
		localDom = domLocalRelLineEdit->text().latin1();
	}

	std:string otherFlowDom = domRelLineEdit->text().latin1();


	//generate LOG file
	QString msg = tr("Save Log File?");
	QString caption = tr("Terra View - FLow Plugin.");
	

	int response = QMessageBox::question(this, caption, msg, tr("Yes"), tr("No"));

	if(response == 0)
	{
		QString qfileName = QFileDialog::getSaveFileName("", "Text File(*.txt)", this, 0, tr( "Set Log File Name"));

		if (qfileName.isEmpty())
		{
			return;
		}

		std::string fileName = qfileName.latin1();

		std::string ext = TeGetExtension(fileName.c_str());

		if(ext.empty())
		{
			fileName += ".txt";
		}

		TeWriteToFile(fileName, "TerraView Flow Plugin - Classify Data Flow - LOG\n\n", "w");
		TeWriteToFile(fileName, "Reference theme: " + refTheme + "\n", "a+");
		TeWriteToFile(fileName, "Output theme name: " + outputThemeName + "\n", "a+");
		TeWriteToFile(fileName, "Weight column name: " + weightColumn + "\n", "a+");
		TeWriteToFile(fileName, "Dominance information:\n", "a+");
		TeWriteToFile(fileName, "\tIs from Flow table: " + dominanceInfo_flow + "\n", "a+");
		TeWriteToFile(fileName, "\tIs from Reference theme: " + dominanceInfo_ref + "\n", "a+");
		TeWriteToFile(fileName, "\tIs calculated: " + dominanceInfo_calc + "\n", "a+");
		TeWriteToFile(fileName, "\tDominance column: " + dominanceColumn + "\n", "a+");
		TeWriteToFile(fileName, "\tUse output flow: " + domValueType + "\n", "a+");
		TeWriteToFile(fileName, "Check local dominance: " + checkLocalDominanceStr + "\n", "a+");
		TeWriteToFile(fileName, "Local dominance value: " + localDom + "\n", "a+");
		TeWriteToFile(fileName, "Other Flows dominance value: " + otherFlowDom + "\n", "a+");
		TeWriteToFile(fileName, "Cut Level: " + cutLevel + "\n", "a+");

	}

	//generate register
	QSettings settings;

	settings.beginGroup("TerraView");
	settings.beginGroup("Flow Plugin");

	settings.writeEntry("ref_theme", refTheme.c_str());
	settings.writeEntry("weight_column", weightColumn.c_str());
	settings.writeEntry("dom_from_flow", dominanceInfo_flow.c_str());
	settings.writeEntry("dom_from_ref", dominanceInfo_ref.c_str());
	settings.writeEntry("dom_calc", dominanceInfo_calc.c_str());
	settings.writeEntry("dom_column", dominanceColumn.c_str());
	settings.writeEntry("use_out_flow", domValueType.c_str());
	settings.writeEntry("check_local_flow", checkLocalDominanceStr.c_str());
	settings.writeEntry("local_dom_value", localDom.c_str());
	settings.writeEntry("others_dom_value", otherFlowDom.c_str());
	settings.writeEntry("cut_level", cutLevel.c_str());

	settings.endGroup();
	settings.endGroup();

	//////////////////////////////////////////////////

	TeFlowUtils flowUtils(local_db_ptr_.nakedPointer());
	
	weightColumn = weightComboBox->currentText().latin1();

	if(weightColumn.empty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Weight colung not defined!"));

		return;
	}


	if(cutLevelLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Cut level not defined!"));

		return;
	}

	if(outputColumnNameLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Output column name not defined!"));

		return;
	}

	bool calculateDominance = false;
	bool tot1 = true;

	if(flowTableRadioButton->isChecked())
	{
		refTheme = flowDominanceThemeComboBox_2->currentText().latin1();

		dominanceColumn = dominanceColumnComboBox->currentText().latin1();

		if(dominanceColumn.empty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Dominance Column not defined!"));

			return;
		}

	}
	else if(referenceThemeRadioButton->isChecked())
	{
		refTheme = flowDominanceThemeComboBox->currentText().latin1();

		dominanceColumn = themeDominanceColumnComboBox->currentText().latin1();

		if(refTheme.empty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Reference theme not defined!"));

			return;
		}

		if(dominanceColumn.empty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Dominance Column not defined!"));

			return;
		}
	}
	else if(calcDomRadioButton->isChecked())
	{
		refTheme = flowDominanceThemeComboBox_3->currentText().latin1();

		calculateDominance = true;

		tot1 = tot1RadioButton->isChecked();

		dominanceColumn = "";

		if(refTheme.empty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Reference theme not defined!"));

			return;
		}

	}

	bool ok;

	checkLocalDominance = verifyLocalRelCheckBox->isChecked();

	double dominanceLocalRelation = 0.;

	if(checkLocalDominance)
	{
		dominanceLocalRelation = domLocalRelLineEdit->text().toDouble(&ok);

		if(!ok)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Invalid value for Local Dominance Relation!"));
			return;
		}
	}

	double dominanceRelation = domRelLineEdit->text().toDouble(&ok);

	if(!ok)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Invalid value for Dominance Relation!"));
		return;
	}

	TeTheme* referenceTheme = flowUtils.getReferenceTheme(refTheme);
	TeDominantFlowNetwork flowDataClassifier(currentTheme_, referenceTheme);

	if(!flowDataClassifier.buildNetwork(weightColumn, dominanceColumn, originCheckBox->isChecked(), calculateDominance, tot1, dominanceRelation, checkLocalDominance, dominanceLocalRelation))
	{
		QMessageBox::warning(this, tr("Warning"), flowDataClassifier.errorMessage().c_str());
		return;
	}
	else
	{
		vector<string> disconnecteds;
		if(flowDataClassifier.checkConnections(disconnecteds))
		{
			    QString msg1 = tr("Exist disconnecteds nodes in network!");
				QString msg2 = tr("Nodes:") + " ";
					    msg2 += disconnecteds[0].c_str();
				if (disconnecteds.size() > 2)
				{
					msg2 += ", ";
					msg2 += disconnecteds[1].c_str();
					msg2 += ("...");
			}
			
				msg2 += " " + tr("are not connected to the network!") + "\n";
				msg2 +=	tr("Connect them?");

				int response = QMessageBox::question(this, msg1, msg2, tr("Yes"), tr("No"));

				if(response == 0)
				{
					flowDataClassifier.setAsRootLevel(disconnecteds);
					
				}
				else
				{
					
				}
		}
		
		TeQtGrid* grid = plug_pars_ptr_->teqtgrid_ptr_;
		if(grid == 0)
		{
			QMessageBox::critical(this, tr("Error"), tr("Error initing grid!"));
			return;
		}

		TeAttrTableVector ta; 
		currentTheme_->getAttTables(ta);
		int cpos = 0;
		unsigned int i;
		for(i = 0; i < ta.size(); i++)
		{
			cpos += ta[i].attributeList().size();

			if(TeConvertToUpperCase(ta[i].name()) == TeConvertToUpperCase(flowDataClassifier.getFlowTableName()))
				break;
		}

		grid->clearPortal();

		if(!flowDataClassifier.classifyNetwork(atoi(cutLevelLineEdit->text().latin1()), outputColumnNameLineEdit->text().latin1()))
		{
			QMessageBox::critical(this, tr("Error"), flowDataClassifier.errorMessage().c_str());

			grid->initPortal();
			grid->refresh();
			grid->goToLastColumn();
	
			return;
		}

		plug_pars_ptr_->updateTVInterface(*plug_pars_ptr_);

		accept();
	}
	
	return;
}


void FlowClassify::setLayout( const bool & network )
{
	if(network)
	{
		outPutWidgetStack->raiseWidget(0);
		classifyPushButton->hide();
		generatePushButton->show();

		//get info from register

		QSettings settings;
		settings.beginGroup("TerraView");
		settings.beginGroup("Flow Plugin");

		
		std::string refTheme = "";
		
		if(!settings.readEntry("ref_theme").isEmpty())
		{
			refTheme = settings.readEntry("ref_theme").latin1();
		}

		std::string weightColumn = "";

		if(!settings.readEntry("weight_column").isEmpty())
		{
			weightColumn = settings.readEntry("weight_column").latin1();
		}

		std::string domFromFlow = "";

		if(!settings.readEntry("dom_from_flow").isEmpty())
		{
			domFromFlow = settings.readEntry("dom_from_flow").latin1();
		}

		std::string domFromRef = "";

		if(!settings.readEntry("dom_from_ref").isEmpty())
		{
			domFromRef = settings.readEntry("dom_from_ref").latin1();
		}
		
		std::string domCalc = "";

		if(!settings.readEntry("dom_calc").isEmpty())
		{
			domCalc = settings.readEntry("dom_calc").latin1();
		}

		std::string domColumn = "";

		if(!settings.readEntry("dom_column").isEmpty())
		{
			domColumn = settings.readEntry("dom_column").latin1();
		}

		std::string useOutFlow = "";

		if(!settings.readEntry("use_out_flow").isEmpty())
		{
			useOutFlow = settings.readEntry("use_out_flow").latin1();
		}

		std::string checkLocalFlow = "";

		if(!settings.readEntry("check_local_flow").isEmpty())
		{
			checkLocalFlow = settings.readEntry("check_local_flow").latin1();
		}

		std::string localDomValue = "10";

		if(!settings.readEntry("local_dom_value").isEmpty())
		{
			localDomValue = settings.readEntry("local_dom_value").latin1();
		}
		
		std::string othersDomValue = "10";

		if(!settings.readEntry("others_dom_value").isEmpty())
		{
			othersDomValue = settings.readEntry("others_dom_value").latin1();
		}
		
		
		settings.endGroup();
		settings.endGroup();

		weightComboBox->setCurrentText(weightColumn.c_str());

		if(domFromFlow == "YES")
		{
			referenceThemeRadioButton->setChecked(true);
			dominanceDataWidgetStack->raiseWidget(1);
			flowDominanceThemeComboBox->setCurrentText(refTheme.c_str());
			themeDominanceColumnComboBox->setCurrentText(domColumn.c_str());
		}

		if(domFromRef == "YES")
		{
			flowTableRadioButton->setChecked(true);
			dominanceDataWidgetStack->raiseWidget(0);
			flowDominanceThemeComboBox_2->setCurrentText(refTheme.c_str());
			dominanceColumnComboBox->setCurrentText(domColumn.c_str());
		}

		if(domCalc == "YES")
		{
			calcDomRadioButton->setChecked(true);
			dominanceDataWidgetStack->raiseWidget(2);
			flowDominanceThemeComboBox_3->setCurrentText(refTheme.c_str());


			if(useOutFlow == "YES")
			{
				tot1RadioButton->setChecked(true);
			}
			else
			{
				tot2RadioButton->setChecked(true);
			}
		}

		if(checkLocalFlow == "YES")
		{
			verifyLocalRelCheckBox->setChecked(true);
			domLocalRelLineEdit->setText(localDomValue.c_str());
		}
		else
		{
			verifyLocalRelCheckBox->setChecked(false);
		}

		domRelLineEdit->setText(othersDomValue.c_str());
	}
	else
	{
		outPutWidgetStack->raiseWidget(1);
		generatePushButton->hide();
		classifyPushButton->show();

		//get info from register

		QSettings settings;
		settings.beginGroup("TerraView");
		settings.beginGroup("Flow Plugin");

		
		std::string refTheme = "";
		
		if(!settings.readEntry("ref_theme").isEmpty())
		{
			refTheme = settings.readEntry("ref_theme").latin1();
		}

		std::string weightColumn = "";

		if(!settings.readEntry("weight_column").isEmpty())
		{
			weightColumn = settings.readEntry("weight_column").latin1();
		}

		std::string domFromFlow = "";

		if(!settings.readEntry("dom_from_flow").isEmpty())
		{
			domFromFlow = settings.readEntry("dom_from_flow").latin1();
		}

		std::string domFromRef = "";

		if(!settings.readEntry("dom_from_ref").isEmpty())
		{
			domFromRef = settings.readEntry("dom_from_ref").latin1();
		}
		
		std::string domCalc = "";

		if(!settings.readEntry("dom_calc").isEmpty())
		{
			domCalc = settings.readEntry("dom_calc").latin1();
		}

		std::string domColumn = "";

		if(!settings.readEntry("dom_column").isEmpty())
		{
			domColumn = settings.readEntry("dom_column").latin1();
		}

		std::string useOutFlow = "";

		if(!settings.readEntry("use_out_flow").isEmpty())
		{
			useOutFlow = settings.readEntry("use_out_flow").latin1();
		}

		std::string checkLocalFlow = "";

		if(!settings.readEntry("check_local_flow").isEmpty())
		{
			checkLocalFlow = settings.readEntry("check_local_flow").latin1();
		}

		std::string localDomValue = "10";

		if(!settings.readEntry("local_dom_value").isEmpty())
		{
			localDomValue = settings.readEntry("local_dom_value").latin1();
		}
		
		std::string othersDomValue = "10";

		if(!settings.readEntry("others_dom_value").isEmpty())
		{
			othersDomValue = settings.readEntry("others_dom_value").latin1();
		}

		std::string cutLevel = "";

		if(!settings.readEntry("cut_level").isEmpty())
		{
			cutLevel = settings.readEntry("cut_level").latin1();
		}

		settings.endGroup();
		settings.endGroup();

		weightComboBox->setCurrentText(weightColumn.c_str());

		if(domFromFlow == "YES")
		{
			referenceThemeRadioButton->setChecked(true);
			dominanceDataWidgetStack->raiseWidget(1);
			flowDominanceThemeComboBox->setCurrentText(refTheme.c_str());
			themeDominanceColumnComboBox->setCurrentText(domColumn.c_str());
		}

		if(domFromRef == "YES")
		{
			flowTableRadioButton->setChecked(true);
			dominanceDataWidgetStack->raiseWidget(0);
			flowDominanceThemeComboBox_2->setCurrentText(refTheme.c_str());
			dominanceColumnComboBox->setCurrentText(domColumn.c_str());
		}

		if(domCalc == "YES")
		{
			calcDomRadioButton->setChecked(true);
			dominanceDataWidgetStack->raiseWidget(2);
			flowDominanceThemeComboBox_3->setCurrentText(refTheme.c_str());


			if(useOutFlow == "YES")
			{
				tot1RadioButton->setChecked(true);
			}
			else
			{
				tot2RadioButton->setChecked(true);
			}
		}

		if(checkLocalFlow == "YES")
		{
			verifyLocalRelCheckBox->setChecked(true);
			domLocalRelLineEdit->setText(localDomValue.c_str());
		}
		else
		{
			verifyLocalRelCheckBox->setChecked(false);
		}

		domRelLineEdit->setText(othersDomValue.c_str());

		cutLevelLineEdit->setText(cutLevel.c_str());
	}
}


void FlowClassify::updateInterface( PluginParameters * plug_pars_ptr )
{
    plug_pars_ptr_ = plug_pars_ptr;

	TeDatabaseFactoryParams dbf_params;
	dbf_params.database_ = plug_pars_ptr_->getCurrentDatabasePtr()->databaseName();
	dbf_params.dbms_name_ = plug_pars_ptr_->getCurrentDatabasePtr()->dbmsName();
	dbf_params.host_ = plug_pars_ptr_->getCurrentDatabasePtr()->host();
	dbf_params.password_ = plug_pars_ptr_->getCurrentDatabasePtr()->password();
	dbf_params.port_ = plug_pars_ptr_->getCurrentDatabasePtr()->portNumber();
	dbf_params.user_ = plug_pars_ptr_->getCurrentDatabasePtr()->user();

	local_db_ptr_.reset( TeDatabaseFactory::make( dbf_params ) );
	
	if( ! local_db_ptr_.isActive() ) 
	{
		QMessageBox::critical( this, "TerraView", 
			tr( "Error creating connection." ) );    
	    return;     
	}

	if( ! local_db_ptr_->isConnected() ) 
	{
		QMessageBox::critical( this, "TerraView", 
			tr( "Error connecting database." ) );    
	    return;     
	} 

	if(!local_db_ptr_.nakedPointer()->loadViewSet(plug_pars_ptr_->getCurrentDatabasePtr()->user()))
	{
		QString msg = tr("Error loading view set!") + "\n";
		msg += local_db_ptr_->errorMessage().c_str();
		QMessageBox::critical(this, "Error", "Error loading view set!");
		return;
	}

	currentTheme_ = 0;

	string themeName = plug_pars_ptr_->getCurrentThemeAppPtr()->getTheme()->name();
	TeThemeMap& themeMap = local_db_ptr_->themeMap();
	TeThemeMap::iterator itTheme = themeMap.begin();
	while(itTheme != themeMap.end())
	{
		if(itTheme->second->name() == themeName)
		{
			currentTheme_ = (TeTheme*)itTheme->second;
			break;
		}

		++itTheme;
	}

	if(currentTheme_ == 0)
	{
		QMessageBox::critical( this, "TerraView", 
			tr( "Current theme not found." ) );    
	    return;
	}

	currentView_ = 0;

	string viewName = plug_pars_ptr_->getCurrentViewPtr()->name();
	TeViewMap& viewMap = local_db_ptr_->viewMap();
	TeViewMap::iterator itView = viewMap.begin();
	while(itView != viewMap.end())
	{
		if(itView->second->name() == viewName)
		{
			currentView_ = itView->second;
			break;
		}

		++itView;
	}

	if(currentView_ == 0)
	{
		QMessageBox::critical( this, "TerraView", 
			tr( "Current view not found." ) );    
	    return;
	}
}



