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

#include <TeWaitCursor.h>
#include <TeQtDatabaseItem.h>
#include <TeQtLayerItem.h>

#include <importTbl.h>

#include <addressNameSeparator.h>

#include <TeAddressNotifyFunctorWindow.h>

void SpatializationWindow::init()
{
	help_ = 0;

	TeWaitCursor wait;

	spatializationStack->raiseWidget(0);

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();

	//Find the current database pointer
	db_ = mainWindow_->currentDatabase();

	// Creates an address locator
	addressLocator_ = new TeAddressLocator(db_);
	
	addressLocator_->createAddressMetadataTable();
	
	// Pega o layer ativo, listando todos os layers configurados como referência para geocodificação e marca como o ativo
	TeLayer* currentLayer = mainWindow_->currentLayer();

	cMetadata_ = new TeCentroidAddressMetadata();
	
	int layerId = (currentLayer ? currentLayer->id() : -1);
	
	listLayers(layerId);			

	listGeocodingTables("");

	listAlternativeLayers();
	
	importTablePushButton->setEnabled(true);
	
	return;
}

void SpatializationWindow::nextPushButton_clicked()
{
	previousPushButton->setEnabled(TRUE);

	if(spatializationStack->id(spatializationStack->visibleWidget()) == 7)
	{
		// Faz o processamento
		if(findLocations())
		{
			// desabilita os botões e avisa do fim.....
			nextPushButton->setEnabled(FALSE);
			previousPushButton->setEnabled(FALSE);

			QString result1 = tr("The address geocoding was accomplished successfully!");

			resultGeocodingTextLabel->setText(result1);

			QString result2 = tr("A new infolayer was created with the name: ");
			        result2 += layerNameLineEdit->text();

			resultLayerTextLabel->setText(result2);

			QString result3  = tr("The reports containing the result of geocoding can be found in:\n");
				    result3 += reportDirectoryLineEdit->text();
	                result3 += "/";
	                result3 += reportNameLineEdit->text();
				    result3 += ".txt\n";

				    result3 += reportDirectoryLineEdit->text();
	                result3 += "/";
	                result3 += reportNameLineEdit->text();
				    result3 += "_summary.txt\n";

			resultReportTextLabel->setText(result3);

			TeQtDatabasesListView *databasesListView = mainWindow_->getDatabasesListView();
			TeQtDatabaseItem *databaseItem = databasesListView->currentDatabaseItem();

			TeDatabase* db = databaseItem->getDatabase();
			
			// Carrega o layer criado na janela de layers
			TeLayer* newLayer = new TeLayer(layerNameLineEdit->text().latin1(), db);

			TeQtLayerItem *layerItem = new TeQtLayerItem(databaseItem, layerNameLineEdit->text().latin1(), newLayer);
			layerItem->setEnabled(true);
		
			if(databasesListView->isOpen(layerItem->parent()) == false)
				databasesListView->setOpen(layerItem->parent(),true);

			mainWindow_->checkWidgetEnabling();
		}
		else
			return;
	}

	QWidget* w = spatializationStack->widget(spatializationStack->id(spatializationStack->visibleWidget ()) + 1);

	spatializationStack->raiseWidget(w);	
	
	if(spatializationStack->id(spatializationStack->visibleWidget()) == 7)
		nextPushButton->setText(tr("&Finish !"));
	else
		nextPushButton->setText(tr("&Next >"));
}

void SpatializationWindow::previousPushButton_clicked()
{
	QWidget* w = spatializationStack->widget(spatializationStack->id(spatializationStack->visibleWidget ()) - 1);

	spatializationStack->raiseWidget(w);

	if(spatializationStack->id(spatializationStack->visibleWidget()) == 0)
		previousPushButton->setEnabled(FALSE);

	nextPushButton->setText(tr("&Next >"));
}

void SpatializationWindow::selectTable()
{

}

void SpatializationWindow::selectColumns()
{

}

void SpatializationWindow::selectReport()
{
}

bool SpatializationWindow::findLocations()
{	
	string reportDir  = reportDirectoryLineEdit->text().latin1();
    string reportName = reportNameLineEdit->text().latin1();

	TeTrim(reportName);

	if(reportName == "")
	{
		QMessageBox::warning(this, tr("Warning"), tr("You should enter a name for the reports that will be generated!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return false;
	}

	TeTrim(reportDir);

	if(reportDir == "")
	{
		QMessageBox::warning(this, tr("Warning"), tr("You should enter a directory name for the reports that will be generated!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return false;
	}

	if(attributeTableComboBox->count() <= 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please, choose a reference layer!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
		
		return false;
	}

	if(!addressLocator_->loadAddressMetadata(currentTableIds_[attributeTableComboBox->currentItem()]))
	{
		QMessageBox::warning(this, tr("Warning"), tr("The attribute table information for the layer used as base for geocoding could not be found."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
		
		return false;
	}

	if(tableComboBox->count() <= 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("You should select a table for geocoding!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return false;

	}

	TeGeocodeTableMetadata tableMetadata;
	
	tableMetadata.tableId_ = currentGeocodingTables_[tableComboBox->currentItem()];
	tableMetadata.locationNameColumnName_ = locationNameColumnComboBox->currentText().latin1();
	tableMetadata.locationNumberColumnName_ = numberColumnComboBox->currentText().latin1();
	tableMetadata.locationPrepositionColumnName_ = locationPrepositionColumnComboBox->currentText().latin1();
	tableMetadata.locationTitleColumnName_ = locationTitleColumnComboBox->currentText().latin1();
	tableMetadata.locationTypeColumnName_ = locationTypeColumnComboBox->currentText().latin1();
	tableMetadata.neighborhoodColumnName_ = neighborhoodColumnComboBox->currentText().latin1();
	tableMetadata.tableName_ = tableComboBox->currentText().latin1();
	tableMetadata.zipCodeColumnName_ = zipCodeColumnComboBox->currentText().latin1();

	if(tableMetadata.locationNameColumnName_ == "")
	{
		QMessageBox::warning(this, tr("Warning"), tr("You should select the name location field!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return false;

	}

	// See if separator char for number an name will be need
	if((tableMetadata.locationNumberColumnName_ == "") || (tableMetadata.locationNumberColumnName_ == tableMetadata.locationNameColumnName_))
	{
		tableMetadata.locationNumberColumnName_ = "";

		AddressNameSeparator *sepWindow = new AddressNameSeparator(this, "sepWindow", true);

		if(sepWindow->exec() != QDialog::Accepted)
			return false;		

		string sep = sepWindow->numberLineEdit->text().latin1();
		
		addressLocator_->setSplitChar(sep);

		delete sepWindow;
	}	

	string layerName = layerNameLineEdit->text().latin1();

	if(layerName == "")
	{
		QMessageBox::warning(this, tr("Warning"), tr("You should enter a name for the new infolayer!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return false;

	}

//	size_t pos = layerName.find(" ", string::npos, 1);
	size_t pos = layerName.find(" ", 0, 1);
	if(pos != string::npos)
	{
		QMessageBox::warning(this, tr("Warning"), tr("The infolayer name cannot have a blank character!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return false;
	}	
	
//	pos = layerName.find("-", string::npos, 1);
	pos = layerName.find("-", 0, 1);

	if(pos != string::npos)
	{
		QMessageBox::warning(this, tr("Warning"), tr("The infolayer name cannot have a \"-\" character!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return false;
	}

	char ch = layerName.at(0);

	if(isdigit(ch))
	{
		QMessageBox::warning(this, tr("Warning"), tr("The name of the infolayer or the table cannot begin with a numeric character!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return false;
	}	

	TeAbstractAddressNotifyFunctor* notifyWindow = new TeAdressNotifyFunctorWindow(this, true);

	if(neighborRadioButton->isChecked())
		cMetadata_->fieldType_ = TeNEIGHBORHOOD;
	else
		cMetadata_->fieldType_ = TeZIPCODE;	

	addressLocator_->setCentroidAddressMetadata(*cMetadata_);

	addressLocator_->setReplaceSpecialChars(specialCharsCheckBox->isOn());


	try
	{
		if(!addressLocator_->addressGeocode(tableMetadata, layerNameLineEdit->text().latin1(),
											reportName, reportDir, exaustiveSearchCheckBox->isOn(), notifyWindow))
		{
			QMessageBox::warning(this, tr("Warning"), addressLocator_->errorMessage().c_str(), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

			delete notifyWindow;

			return false;
		}
	}
	catch(...)
	{
		QMessageBox::warning(this, tr("Warning"), "Error during geocode! Please try again!", QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		delete notifyWindow;

		return false;

	}

	delete notifyWindow;

	return true;
}

void SpatializationWindow::destroy()
{
	if(addressLocator_)
		delete addressLocator_;

	if(cMetadata_)
		delete cMetadata_;

	return;
}

void SpatializationWindow::layerComboBox_activated()
{
	attributeTableComboBox->setEnabled(FALSE);
	attributeTableComboBox->hide();
	textLabel1_4->hide();

	currentTableIds_.clear();

	// Limpa todas as combobox
	attributeTableComboBox->clear();

	vector<pair<int, string> > layerTables;

	addressLocator_->getAddressLocatorLayerTables(layerTables, currentLayerIds_[layerComboBox->currentItem()]);

	// Lista as tabelas de atributos
	unsigned int i = 0;	

	for(i = 0; i < layerTables.size(); ++i)
	{
		attributeTableComboBox->insertItem(layerTables[i].second.c_str());
		currentTableIds_.push_back(layerTables[i].first);
	}

	if(!i)
	{
		QMessageBox::warning(this, tr("Warning"), tr("The attribute tables of the infolayer could not be found."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return;
	}

	if(i > 1u)
	{
		attributeTableComboBox->show();
		textLabel1_4->show();
	}

	// Habilita a combobox de tabelas de atributos
	attributeTableComboBox->setEnabled(TRUE);

	// Marca a primeira tabela de atributos como a corrente se ela já estiver registrada
	attributeTableComboBox->setCurrentItem(0);	

	attributeTableComboBox_activated();

	return;
}


void SpatializationWindow::attributeTableComboBox_activated()
{
	return;
}


void SpatializationWindow::tableComboBox_activated()
{
	// Limpa as combobox
	numberColumnComboBox->clear();
	locationTypeColumnComboBox->clear();
	locationTitleColumnComboBox->clear();
	locationPrepositionColumnComboBox->clear();
	locationNameColumnComboBox->clear();
	neighborhoodColumnComboBox->clear();
	zipCodeColumnComboBox->clear();	
	//primaryKeyColumnComboBox->clear();

	// Insere um item vazio em cada combobox
	numberColumnComboBox->insertItem("");
	locationTypeColumnComboBox->insertItem("");
	locationTitleColumnComboBox->insertItem("");
	locationPrepositionColumnComboBox->insertItem("");
	locationNameColumnComboBox->insertItem("");
	neighborhoodColumnComboBox->insertItem("");
	zipCodeColumnComboBox->insertItem("");	
	//primaryKeyColumnComboBox->insertItem("");
	
	// Desabilita as combobox
	numberColumnComboBox->setEnabled(FALSE);
	locationTypeColumnComboBox->setEnabled(FALSE);
	locationTitleColumnComboBox->setEnabled(FALSE);
	locationPrepositionColumnComboBox->setEnabled(FALSE);
	locationNameColumnComboBox->setEnabled(FALSE);
	neighborhoodColumnComboBox->setEnabled(FALSE);
	zipCodeColumnComboBox->setEnabled(FALSE);	
	//primaryKeyColumnComboBox->setEnabled(FALSE);


	// Vê a estrutura da tabela selecionada
	TeAttributeList attList;

	if(!db_->getAttributeList(tableComboBox->currentText().latin1(), attList))
		return;

	unsigned int i = 0;

	for(i = 0; i < attList.size(); ++i)
	{
		if(attList[i].rep_.type_ == TeINT || attList[i].rep_.type_ == TeREAL)
			numberColumnComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeSTRING)
			locationTypeColumnComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeSTRING)
			locationTitleColumnComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeSTRING)
			locationPrepositionColumnComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeSTRING)
			locationNameColumnComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeSTRING)
		neighborhoodColumnComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeSTRING)
			zipCodeColumnComboBox->insertItem(attList[i].rep_.name_.c_str());	
		//primaryKeyColumnComboBox->insertItem(attList[i].rep_.name_.c_str());
	}

	//if(i > 0)
	//{
		numberColumnComboBox->setEnabled(TRUE);
		locationTypeColumnComboBox->setEnabled(TRUE);
		locationTitleColumnComboBox->setEnabled(TRUE);
		locationPrepositionColumnComboBox->setEnabled(TRUE);
		locationNameColumnComboBox->setEnabled(TRUE);
		neighborhoodColumnComboBox->setEnabled(TRUE);
		zipCodeColumnComboBox->setEnabled(TRUE);	
		//primaryKeyColumnComboBox->setEnabled(TRUE);
	//}

	return;
}



void SpatializationWindow::configPushButton_clicked()
{
	int id = -1;	

	if(layerComboBox->count() > 0)
		id = currentLayerIds_[layerComboBox->currentItem()];

    LBSDefinition *lbsWindow = new LBSDefinition(this, "lbsWindow", true);

	lbsWindow->listLayers(id);

	lbsWindow->exec();

	if(lbsWindow->getLayer())
		listLayers(lbsWindow->getLayer()->id());
	else
		listLayers(-1);

	delete lbsWindow;
}


void SpatializationWindow::listLayers( const int & currentLayerId )
{
	// Fill the layer combobox with the list of layers of the current database
	layerComboBox->clear();
	attributeTableComboBox->clear();
	attributeTableComboBox->hide();
	textLabel1_4->hide();

	layerComboBox->setEnabled(FALSE);
	attributeTableComboBox->setEnabled(FALSE);

	vector<pair<int, string> > layers;
	
	addressLocator_->getAddressLocatorLayers(layers);

	if(layers.size() == 0)
	{
		previousPushButton->setEnabled(FALSE);
		nextPushButton->setEnabled(FALSE);

		return;
	}
	else
	{
		previousPushButton->setEnabled(TRUE);
		nextPushButton->setEnabled(TRUE);
	}

	unsigned int i;

	currentLayerIds_.clear();

	for(i = 0; i < layers.size(); ++i)
	{
		layerComboBox->insertItem(layers[i].second.c_str());
		currentLayerIds_.push_back(layers[i].first);
	}

	// Só habilita a lista de layer se a lista contiver pelo menos 1 layer na lista.
	if(layerComboBox->count() > 0)
		layerComboBox->setEnabled(TRUE);
	else
		return;

	if(currentLayerId != -1)
	{
		for(i = 0; i < currentLayerIds_.size(); ++i)
		{
			if(currentLayerIds_[i] == currentLayerId)
			{
				layerComboBox->setCurrentItem(i);
				break;
			}
		}

		if(i == (unsigned int)layerComboBox->count())
			layerComboBox->setCurrentItem(0);
	}
	else
		layerComboBox->setCurrentItem(0);

	layerComboBox_activated();

	return;
}


void SpatializationWindow::importTablePushButton_clicked()
{

	ImportTbl *importTable = new ImportTbl(this, "importTable", true);
	importTable->setParams(db_,mainWindow_->lastOpenDir_);
	importTable->tableTypeComboBox->setCurrentItem(3);
	importTable->tableTypeComboBox_activated(3);
	importTable->tableTypeComboBox->setEnabled(false);
	importTable->layerComboBox->setEnabled(false);
	importTable->layerComboBox->hide();
	importTable->layerTextLabel->hide();
	if(importTable->exec() == QDialog::Accepted)
	{
		listGeocodingTables(importTable->tableNameLineEdit->text().latin1());
		mainWindow_->lastOpenDir_ = importTable->getLastOpenDir();
	}
	
	delete importTable;

}

void SpatializationWindow::listGeocodingTables(const string & defaultTable)
{
	tableComboBox->setEnabled(false);
	tableComboBox->clear();

	// Pegar todas as tabelas de um banco de dados terralib e listar
	vector<pair<int, string> > tables;
	
	unsigned int i = 0u;	

	unsigned int currentItem = 0u;

	currentGeocodingTables_.clear();

	if(addressLocator_->findGeocodingTables(tables))
	{
		for(i = 0; i < tables.size(); ++i)
		{
			// MANTER UM VETOR COM OS IDS
			currentGeocodingTables_.push_back(tables[i].first);

			// Insere o item na list box
			tableComboBox->insertItem(tables[i].second.c_str());

			// Verifica se existe um nome default
			if(!defaultTable.empty())
			{
				if(tables[i].second == defaultTable)
					currentItem = i;
			}
		}

		if(tableComboBox->count() > 0)
		{
			tableComboBox->setEnabled(true);
	
			tableComboBox->setCurrentItem(currentItem);
	
			tableComboBox_activated();
		}
	}

	return;
}


void SpatializationWindow::dictionaryPushButton_clicked()
{
	QString extAllowed = tr("All Files (*.*)");

	QString dirName = QFileDialog::getOpenFileName(mainWindow_->lastOpenDir_, extAllowed, this, 0, tr("Select a dictionary file"));

	if(!dirName.isEmpty())
	{
		dictionaryLineEdit->setText(dirName);
		string fName = dictionaryLineEdit->text().latin1();
		this->addressLocator_->loadDictionary(fName);
	}
	else
	{
		QString msg = tr("Select a dictionary file!");
		QMessageBox::warning( this, tr("Warning"), msg);
		return;
	}
	mainWindow_->lastOpenDir_ = dirName;
}

void SpatializationWindow::listAlternativeLayers()
{

	TeLayerMap& layerMap = db_->layerMap();

	if(layerMap.empty())
		return;

	alternativeLayerComboBox->setEnabled(true);
	alternativeLayerComboBox->clear();

	alternativeAttrTableComboBox->setEnabled(false);
	alternativeAttrTableComboBox->clear();

	alternativeFieldComboBox->setEnabled(false);
	alternativeFieldComboBox->clear();


	alternativeLayerComboBox->insertItem("");

	TeLayerMap::iterator layerIt;

	for(layerIt = layerMap.begin(); layerIt != layerMap.end(); ++layerIt)
	{
		QString layerName = layerIt->second->name().c_str();

		alternativeLayerComboBox->insertItem(layerName);
	}
}


void SpatializationWindow::alternativeLayerComboBox_clicked()
{
	TeLayerMap& layerMap = db_->layerMap();

	if(layerMap.empty())
		return;

	alternativeAttrTableComboBox->setEnabled(false);
	alternativeAttrTableComboBox->clear();

	alternativeFieldComboBox->setEnabled(false);
	alternativeFieldComboBox->clear();

	cMetadata_->centroidLayer_ = 0;

	TeLayerMap::iterator layerIt;

	string alternativeLayer = alternativeLayerComboBox->currentText().latin1();

	for(layerIt = layerMap.begin(); layerIt != layerMap.end(); ++layerIt)
	{
		
		if(alternativeLayer == layerIt->second->name().c_str())
		{
			TeLayer* layer = layerIt->second;

			cMetadata_->centroidLayer_ = layer;

			TeAttrTableVector attVec;
			layer->getAttrTables(attVec, TeAttrStatic);

			for(unsigned int i = 0; i < attVec.size(); ++i)
			{
				QString attName = attVec[i].name().c_str();

				alternativeAttrTableComboBox->insertItem(attName);
			}

			alternativeAttrTableComboBox->setEnabled(TRUE);

			alternativeAttrTableComboBox->setCurrentItem(0);			
			
			alternativeAttrTableComboBox_clicked();
		}
	}
}


void SpatializationWindow::alternativeAttrTableComboBox_clicked()
{
	TeLayerMap& layerMap = db_->layerMap();

	if(layerMap.empty())
		return;

	alternativeFieldComboBox->setEnabled(false);
	alternativeFieldComboBox->clear();

	cMetadata_->attributeTableName_ = "";

	TeLayerMap::iterator layerIt;

	string alternativeLayer = alternativeLayerComboBox->currentText().latin1();

	string attTableName = alternativeAttrTableComboBox->currentText().latin1();

	cMetadata_->attributeTableName_ = attTableName;

	for(layerIt = layerMap.begin(); layerIt != layerMap.end(); ++layerIt)
	{
		
		if(alternativeLayer == layerIt->second->name().c_str())
		{
			TeLayer* layer = layerIt->second;

			TeAttrTableVector attVec;
			layer->getAttrTables(attVec, TeAttrStatic);			

			for(unsigned int i = 0; i < attVec.size(); ++i)
			{
				if(attVec[i].name() == attTableName)
				{
					TeAttributeList& attList = attVec[i].attributeList();

					for(unsigned int j = 0; j < attList.size(); ++j)
					{
						QString fName = attList[j].rep_.name_.c_str();

						alternativeFieldComboBox->insertItem(fName);
					}

					alternativeFieldComboBox->setEnabled(TRUE);
					
					alternativeFieldComboBox->setCurrentItem(0);

					
				}				
			}
		}
	}
}


void SpatializationWindow::alternativeFieldComboBox_clicked()
{
	string fieldName = alternativeFieldComboBox->currentText().latin1();
	cMetadata_->centroidName_ = fieldName;
}


void SpatializationWindow::clearDictionaryPushButton_clicked()
{
	QString s = "";
	
	dictionaryLineEdit->setText(s);

	addressLocator_->loadDictionary("");

	
}


void SpatializationWindow::locateDirPushButton_clicked()
{
	QString dirName = QFileDialog::getExistingDirectory(QString::null, this, tr("Get existing directory"), tr("Select a directory"), true);

	if(!dirName.isEmpty())
		reportDirectoryLineEdit->setText(dirName);
	else
	{
		QString msg = tr("Select a directory!");
		QMessageBox::warning( this, tr("Warning"), msg);
		return;
	}
}


void SpatializationWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("spatializationWindow.htm");
	if(help_->erro_ == false)
	{
		help_->show();
		help_->raise();

		if(spatializationStack->id(spatializationStack->visibleWidget()) == 1)
			help_->scrollToAnchor("spatializationStack1");
		else if(spatializationStack->id(spatializationStack->visibleWidget()) == 2)
			help_->scrollToAnchor("spatializationStack2");
		else if(spatializationStack->id(spatializationStack->visibleWidget()) == 3)
			help_->scrollToAnchor("spatializationStack3");
		else if(spatializationStack->id(spatializationStack->visibleWidget()) == 4)
			help_->scrollToAnchor("spatializationStack4");
		else if(spatializationStack->id(spatializationStack->visibleWidget()) == 5)
			help_->scrollToAnchor("spatializationStack5");
		else if(spatializationStack->id(spatializationStack->visibleWidget()) == 6)
			help_->scrollToAnchor("spatializationStack6");
		else if(spatializationStack->id(spatializationStack->visibleWidget()) == 7)
			help_->scrollToAnchor("spatializationStack7");	
	}
	else
	{
		delete help_;
		help_ = 0;
	}
}


void SpatializationWindow::show()
{
	TeLayerMap& layerMap = db_->layerMap();
	if(layerMap.empty())
	{
		QMessageBox::information(this, tr("Information"), tr("There are no layers to be used as base to geocoding!"));	
		reject();
		hide();
	}
	else
		QDialog::show();
}
