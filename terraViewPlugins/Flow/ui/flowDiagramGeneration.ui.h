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


#include <TeDatabase.h>
#include "TeAppTheme.h" 
#include <qapplication.h>
#include <importTbl.h>
#include <TeQtViewsListView.h>
#include <TeQtDatabaseItem.h>
#include <TeQtDatabasesListView.h>
#include <TeQtLayerItem.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <TeApplicationUtils.h>
#include <qmessagebox.h>
#include "TeQtCanvas.h"
#include "TeFlowDataGenerator.h"
#include "TeFlowUtils.h"
#include "TeFlowMetadata.h"
#include <TeDatabaseFactory.h>
#include <TeDatabaseFactoryParams.h>

void FlowDiagramGeneration::init(PluginParameters * plug_pars_ptr)	
{
    plug_pars_ptr_ = plug_pars_ptr;

	updateInterface(plug_pars_ptr_);

	TeFlowMetadata metadata(local_db_ptr_.nakedPointer());
	int ret = metadata.createFlowMetadataTable();
	if (ret == 0)
	{
		QMessageBox::warning(this, tr("Warning"), metadata.errorMessage().c_str());
		return;
	}

	TeFlowUtils flowUtils(local_db_ptr_.nakedPointer());

	listFlowDataTables("");

	listColumnLinkName();
	
	string newLayerName = flowUtils.getNewLayerName(plug_pars_ptr_->getCurrentLayerPtr());

	newLayerLineEdit->setText(newLayerName.c_str());

	return;
}



void FlowDiagramGeneration::flowDataTableComboBox_activated( int )	
{	
// Limpa as combobox
	originComboBox->clear();
	destinyComboBox->clear();

// Desabilita as combobox
	originComboBox->setEnabled(FALSE);
	destinyComboBox->setEnabled(FALSE);
	
// Ve a estrutura da tabela selecionada
	TeAttributeList attList;

	if(!local_db_ptr_.nakedPointer()->getAttributeList(flowDataTableComboBox->currentText().latin1(), attList))
	{
		QMessageBox::warning(this, tr("Warning"), local_db_ptr_.nakedPointer()->errorMessage().c_str());
		return;
	}
	

	unsigned int i = 0;

	for(i = 0; i < attList.size(); ++i)
	{
		originComboBox->insertItem(attList[i].rep_.name_.c_str());

		destinyComboBox->insertItem(attList[i].rep_.name_.c_str());
	}

	if(i > 0)
	{
		originComboBox->setEnabled(TRUE);
		destinyComboBox->setEnabled(TRUE);
	}	

	return;	
}


void FlowDiagramGeneration::listFlowDataTables(const string& defaultTable)	
{
	flowDataTableComboBox->setEnabled(false);
	flowDataTableComboBox->clear();	

	vector<string> tables;

	TeFlowUtils flowUtils(local_db_ptr_.nakedPointer());

	if(!flowUtils.listCandidateFlowDataTables(tables))
	{
		QMessageBox::warning(this, tr("Warning"), flowUtils.errorMessage().c_str());
		return;
	}

	unsigned int current = 0;
	
	for(unsigned int i = 0; i < tables.size(); ++i)
	{
		flowDataTableComboBox->insertItem(tables[i].c_str());

		if(tables[i] == defaultTable)
			current = i;
	}

	if(flowDataTableComboBox->count() > 0)
	{
        flowDataTableComboBox->setEnabled(true);	
		flowDataTableComboBox->setCurrentItem(current);	
		flowDataTableComboBox_activated(current);
	}

	return;
}


void FlowDiagramGeneration::importTablePushButton_clicked()	
{
	ImportTbl *importTable = new ImportTbl(this, "importTable", true);

	importTable->tableTypeComboBox->setCurrentItem(0);
	importTable->tableTypeComboBox->setEnabled(true);
	importTable->layerComboBox->setEnabled(false);
	importTable->layerComboBox->hide();
	importTable->layerTextLabel->hide();
	importTable->setParams(local_db_ptr_.nakedPointer(), "");
	
	if(importTable->exec() == QDialog::Accepted)
		listFlowDataTables(importTable->tableNameLineEdit->text().latin1());
	
	delete importTable;
}


void FlowDiagramGeneration::listColumnLinkName()	
{
	objectIdComboBox->setEnabled(false);
	objectIdComboBox->clear();

	TeAttrTableVector& attrs = currentTheme_->attrTables();

	unsigned int current = -1;
	unsigned int nTables = attrs.size();

	for(unsigned int i = 0; i < nTables; ++i)
	{
		if(attrs[i].tableType() == TeAttrStatic)
		{
			TeAttributeList& attList = attrs[i].attributeList();

			for(unsigned int j = 0; j < attList.size(); ++j)
			{
				string columnName = attrs[i].name() + "." + attList[j].rep_.name_;

				objectIdComboBox->insertItem(columnName.c_str());

// marca a primeira coluna que for de ligacao com as geometrias como a corrente => tentativa de ajudar
// o usuario
				if(current == -1)
					if(attList[j].rep_.name_ == attrs[i].linkName())
						current = objectIdComboBox->count() - 1;
			}
		}
	}	

	if(objectIdComboBox->count() > 0)
	{
		if(current != -1)
			objectIdComboBox->setCurrentItem(current);
		
		objectIdComboBox->setEnabled(true);
	}

	return;
}


void FlowDiagramGeneration::generatePushButton_clicked()
{
	
	if(tabWinCheckBox->isChecked())
	{
		if(codSizeLineEdit->text().latin1() == "")
		{
			QMessageBox::warning(this, tr("Warning"), "Code size is empty!");
			return;
		}

		if(!updateTableFromTabWin())
			return;
	}

// verifica se todos os campos necessarios estao preenchidos!

	if(flowDataTableComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Flow data table not defined!"));

		return;
	}

	if(originComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Origin column name not defined!"));

		return;
	}

	if(destinyComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Destiny column name not defined!"));

		return;
	}

	if(objectIdComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Link column name not defined!"));

		return;
	}

	if(newLayerLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Output layer name not defined!"));

		return;
	}

	if(originComboBox->currentText() == destinyComboBox->currentText())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Origin and destiny column names are the same!"));

		return;
	}

// verifica se o nome do layer e valido

	QString ss = newLayerLineEdit->text();
	
	if(isValidName(ss) == false)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Output layer name is not valid!"));

		return;
	}

	TeFlowDataGenerator flowGenerator(currentTheme_);
	
	TeLayer* newLayer = flowGenerator.buildFlowData(flowDataTableComboBox->currentText().latin1(),
												    originComboBox->currentText().latin1(),
												    destinyComboBox->currentText().latin1(),
												    objectIdComboBox->currentText().latin1(),
												    ss.latin1());

	if(!newLayer)
	{
		QMessageBox::warning(this, tr("Warning"), flowGenerator.errorMessage().c_str());
		return;
	}

// se chegou ate aqui e porque o layer foi criado
// vamos criar um tema e exibi-lo
    QString msg1 = tr("Flow Diagram generated!");
	QString msg2 = tr("Layer:") + " ";
    		msg2 += newLayer->name().c_str();
            msg2 += " " + tr("was created!") + "\n";
            msg2 +=	tr("Show data?");

    int response = QMessageBox::question(this, msg1, msg2, tr("Yes"), tr("No"));

    if(response != 0)
        return;

	string themeName = getNewThemeName(newLayer->name());
	TeView* view = currentView_;
	TeAttrTableVector tablesVector;
	newLayer->getAttrTables(tablesVector);

	TeTheme* theme = new TeTheme(themeName);
	theme->layer (newLayer);
	theme->visibleRep(newLayer->geomRep());
	view->add(theme);
	theme->visibility(true);

	TeAppTheme* themeDiagram = new TeAppTheme(theme);

	themeDiagram->canvasLegUpperLeft(newLayer->box().center());
		
	for (unsigned int i = 0; i < tablesVector.size(); ++i)
		theme->addThemeTable(tablesVector[i]);

	if(!insertAppTheme(local_db_ptr_.nakedPointer(),themeDiagram))
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

	plug_pars_ptr_->updateTVInterface(*plug_pars_ptr_);

	accept();
}


string FlowDiagramGeneration::getNewThemeName( string name )
{
	int	i;
	string newName = "Theme_" + name;
	string q = "SELECT name FROM te_theme WHERE name = '" + newName + "'";

	TeDatabasePortal* portal = local_db_ptr_.nakedPointer()->getPortal();
	if(portal && portal->query(q) && portal->fetchRow())
	{
		q = "SELECT name FROM te_theme WHERE name LIKE 'Theme%_" + name + "' ORDER BY name DESC";
		int f = q.find("%");
		// replace % for correct sintax with other driver (KARINE VERIFY, PLEASE...)
		if(local_db_ptr_.nakedPointer()->dbmsName() == "OracleSpatial")
			q.replace(f, 1, "*");
		else if(local_db_ptr_.nakedPointer()->dbmsName() == "PostgreSQL")
			q.replace(f, 1, "*");
		else if(local_db_ptr_.nakedPointer()->dbmsName() == "MySQL")
			q.replace(f, 1, "*");
		else if(local_db_ptr_.nakedPointer()->dbmsName() == "OracleAdo")
			q.replace(f, 1, "*");
		else if(local_db_ptr_.nakedPointer()->dbmsName() == "SqlServerAdo")
			q.replace(f, 1, "*");

		portal->freeResult();
		if(portal && portal->query(q) && portal->fetchRow())
		{
			string s = portal->getData(0);
			if(s == newName)
				newName = "Theme0_" + name;
			else
			{
				s.erase(0, 5);
				int f = s.find("_");
				s.erase(f, s.size() - f);
				for(i=0; i<(int)s.size(); i++)
				{
					if(s[i] < 0x30 || s[i] > 0x39)
						break;
				}
				if(i == (int)s.size())
				{
					int n = atoi(s.c_str());
					n++;
					newName = "Theme" + Te2String(n) + "_" + name;
				}
				else
				{
                                   s = TeConvertToUpperCase(s);
					if(s[s.size()-1] == 0x5A)
						s.append("A");
					else
					{
						char c = s[s.size()-1];
						s[s.size()-1] = ++c;
					}						 
					newName = "Theme" + s + "_" + name;
				}
			}
		}
	}
	delete portal;
	return newName;
}


bool FlowDiagramGeneration::isValidName( QString s )
{
	if(s.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Invalid Layer Name!"));
		return false;
	}
	else // verify if name is valid
	{
		QString msg;
		string layerName = s.latin1();
		int i = layerName.find(" ");
		if(i >= 0)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Invalid Layer Name!"));
			return false;
		}

		if(layerName[0] >= 0x30 && layerName[0] <= 0x39)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Invalid Layer Name!"));
			return false;
		}
		int f1 = layerName.find("*", string::npos);
		int f2 = layerName.find("/", string::npos);
		int f3 = layerName.find("-", string::npos);
		int f4 = layerName.find("+", string::npos);
		int f5 = layerName.find("=", string::npos);
		int f6 = layerName.find("%", string::npos);
		int f7 = layerName.find(">", string::npos);
		int f8 = layerName.find("<", string::npos);
		if(f1>=0 || f2>=0 || f3>=0 || f4>=0 || f5>=0 || f6>=0 || f7>=0 || f8>=0)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Invalid Layer Name!"));
			return false;
		}

		string u = TeConvertToUpperCase(layerName);

		if(u=="OR" || u=="AND" || u=="NOT" || u=="LIKE")
		{
			QMessageBox::warning(this, tr("Warning"), tr("Invalid Layer Name!"));
			return false;
		}

		if(u=="SELECT" || u=="FROM" || u=="UPDATE" || u=="DELETE" ||u=="BY" || u=="GROUP" || u=="ORDER" ||
		   u=="DROP" || u=="INTO" || u=="VALUE" || u=="IN" || u=="ASC" || u=="DESC"|| u=="COUNT" || u=="JOIN" ||
		   u=="LEFT" || u=="RIGHT" || u=="INNER" || u=="UNION" || u=="IS" || u=="NULL" || u=="WHERE" ||
		   u=="BETWEEN" || u=="DISTINCT" || u=="TRY" || u=="IT" || u=="INSERT" || u=="ALIASES" || u=="CREATE" ||
		   u=="ALTER" || u=="TABLE" || u=="INDEX" || u=="ALL" || u=="HAVING")
		{
			QMessageBox::warning(this, tr("Warning"), tr("Invalid Layer Name!"));
			return false;
		}

		if(local_db_ptr_.nakedPointer()->tableExist(layerName))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Invalid Layer Name!"));
			return false;
		}

		s = layerName.c_str();
		return true;
	}
}

bool FlowDiagramGeneration::updateTableFromTabWin()
{
	TeFlowUtils flowUtils(local_db_ptr_.nakedPointer());
	
	string flowTableName = flowDataTableComboBox->currentText().latin1();
	string originColumnName = originComboBox->currentText().latin1();
	string destinyColumnName = destinyComboBox->currentText().latin1();
	int length = atoi(codSizeLineEdit->text().latin1());

	if(!flowUtils.getLeftString(flowTableName, originColumnName, length))
	{
		QMessageBox::warning(this, tr("Warning"), local_db_ptr_.nakedPointer()->errorMessage().c_str());
		return false;
	}
	
	
	if(!flowUtils.getLeftString(flowTableName, destinyColumnName, length))
	{
		QMessageBox::warning(this, tr("Warning"), local_db_ptr_.nakedPointer()->errorMessage().c_str());
		return false;
	}
	
   return true; 
}


void FlowDiagramGeneration::updateInterface( PluginParameters * plug_pars_ptr )
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
