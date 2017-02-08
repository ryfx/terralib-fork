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

#include <TeWaitCursor.h>
#include <TeQtGrid.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <TeQtViewsListView.h>


void LBSDefinition::init()
{
	help_ = 0;

	TeWaitCursor wait;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();

	//Find the current database pointer
	db_ = mainWindow_->currentDatabase();

	// Creates an address locator
	addressLocator_ = new TeAddressLocator(db_);
	

	return;
}

void LBSDefinition::destroy()
{
	if(addressLocator_)
		delete addressLocator_;
    
	return;
}

void LBSDefinition::layerComboBox_activated(int index)
{

	layerTableComboBox->setEnabled(FALSE);
	layerTableComboBox->hide();
	textLabel1->hide();

	// Limpa todas as combobox
	layerTableComboBox->clear();

	// Pega o nome do layer selecionado
	QString layerName = layerComboBox->text(index);

	// Busca o ponteiro para o layer selecionado
	TeLayerMap& layerMap = db_->layerMap();

	TeLayerMap::iterator it;

	currentLayer_ = 0;

	for(it = layerMap.begin(); it != layerMap.end(); ++it)
	{
		currentLayer_ = it->second;

		if(currentLayer_->name() == layerName.latin1())
			break;
	}

	// Busca as tabelas de atributos do layer: s� as est�ticas ser�o usadas.

	TeAttrTableVector attVec;

	if(!currentLayer_->getAttrTables(attVec, TeAttrStatic))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Didn't find an attribute table for this layer."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return;
	}

	// Lista as tabelas de atributos
	unsigned int i = 0;	

	for(i = 0; i < attVec.size(); ++i)
	{
		layerTableComboBox->insertItem(attVec[i].name().c_str());
	}

	if(!i)
	{
		QMessageBox::warning(this, tr("Warning"), tr("The attribute tables for this layer could not be found."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return;
	}

	if(i > 1)
	{
		layerTableComboBox->show();
		textLabel1->show();
	}

	// Habilita a combobox de tabelas de atributos
	layerTableComboBox->setEnabled(TRUE);

	// Procura ver qual a tabela de endere�amento ser� ativada automaticamente
	vector<TeAddressMetadata> addVec;

	addressLocator_->getAllAddressMetadataLayer(currentLayer_->id(), addVec);

	// Marca a primeira tabela de atributos como a corrente se ela j� estiver registrada
	if(addVec.size())
	{
		for(i = 0; i < attVec.size(); ++i)
			if(attVec[i].id() == addVec[0].tableId_)
			{
				layerTableComboBox->setCurrentItem(i);			

				break;
			}
	}
	else
		layerTableComboBox->setCurrentItem(0);	

	layerTableComboBox_activated(layerTableComboBox->currentItem());	
}


void LBSDefinition::confirmPushButton_clicked()
{
	// Caso o usu�rio entre com duas informa��es p/ o nome completo, emite-se uma mensagem de erro
	if(!completeLocationNameComboBox->currentText().isEmpty() && 
	   !completeLocationNameLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please, choose beteween a new column name and a exiting column for complete address!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return;
	}

	// Se o nome completo n�o for informado, emite uma mensagem de erro
	if(completeLocationNameComboBox->currentText().isEmpty() && 
	   completeLocationNameLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please, select or insert the complete address column!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return;
	}


	if((completeLocationNameComboBox->currentText() == locationComboBox->currentText() ||
	   completeLocationNameLineEdit->text() == locationComboBox->currentText()) && !locationComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please, the complete name must be different from location name!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return;
	}

	// Caso o usu�rio entre com o nome da nova coluna e omita a coluna com o nome
	// � emitido uma mensagem de erro
	if(completeLocationNameComboBox->currentText().isEmpty() && 
	   !completeLocationNameLineEdit->text().isEmpty() &&
	   locationComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("At least the address location must be filled to create a new complete name column!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return;
	}

	if(leftBeginComboBox->currentText().isEmpty() || rightBeginComboBox->currentText().isEmpty() || leftEndComboBox->currentText().isEmpty() || rightEndComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("The fields with initial and final addresses are necessery. Please, fill these fields before confirm operation."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return;
	}

	// Caso o usu�rio entre com os campos de tipo titulo e preposicao e n�o selecione
	// um novo campo para o nome completo, emite uma mensagem de erro
	if((!locationTypeComboBox->currentText().isEmpty() || !locationTitleComboBox->currentText().isEmpty() || !locationPrepositionComboBox->currentText().isEmpty()) &&
		locationComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("The address location is necessery when location name is formed by more than one field. Please, fill this field before confirm operation."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return;
	}

	// Caso o usu�rio entre com os campos de tipo titulo e preposicao e n�o selecione
	// um novo campo para o nome completo, emite uma mensagem de erro
	if((!locationTypeComboBox->currentText().isEmpty() || !locationTitleComboBox->currentText().isEmpty() || !locationPrepositionComboBox->currentText().isEmpty()) &&
		completeLocationNameComboBox->currentText().isEmpty() &&
		completeLocationNameLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("The complete address is necessery when location name is formed by more than one field. Please, fill this field before confirm operation."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return;
	}

	// Verifica se o usu�rio entrou com um nome de coluna que aparece na lista dos poss�veis compos
	// com o nome completo
	if(!completeLocationNameLineEdit->text().isEmpty())
	{
		for(int i = 0; i < completeLocationNameComboBox->count(); ++i)
		{
			if(completeLocationNameComboBox->text(i) == completeLocationNameLineEdit->text())
			{
				QMessageBox::warning(this, tr("Warning"), tr("The complete address in the edition box is already in the list. Please, select it in the list and then confirm again."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

				return;
			}
		}
	}

	TeAddressMetadata m;

	bool newColumn = false;

	m.locationCompleteName_ = completeLocationNameComboBox->currentText().latin1();

	if(m.locationCompleteName_.empty())
	{
		newColumn = true;

		m.locationCompleteName_ = completeLocationNameLineEdit->text().latin1();
	}

	if(newColumn)
	{
		int response = QMessageBox::question(this, tr("Question"),
					   tr("A new column: ") + m.locationCompleteName_.c_str() + 
					   tr(", with all address location will be created. Confirm it?"),
					   tr("Yes"), tr("No"));

		if(response != 0)
			return;

	}

	m.tableId_ = currentTable_;
	m.initialLeftNumber_ = leftBeginComboBox->currentText().latin1();
	m.initialRightNumber_ = rightBeginComboBox->currentText().latin1();
	m.finalLeftNumber_ = leftEndComboBox->currentText().latin1();
	m.finalRightNumber_ = rightEndComboBox->currentText().latin1();
	m.locationType_ = locationTypeComboBox->currentText().latin1();
	m.locationTitle_ = locationTitleComboBox->currentText().latin1();
	m.locationPreposition_ = locationPrepositionComboBox->currentText().latin1();
	m.locationName_ = locationComboBox->currentText().latin1();	
	m.leftNeighborhood_ = bairroLeftComboBox->currentText().latin1();
	m.rightNeighborhood_ = bairroRightComboBox->currentText().latin1();
	m.leftZipCode_ = CEPLeftComboBox->currentText().latin1();
	m.rightZipCode_ = CEPRightComboBox->currentText().latin1();


	bool isInsert = false;

	TeAddressMetadata addAux = m;

	if(addressLocator_->isMetadataLoaded())
	{
		cancelPushButton->setEnabled(TRUE);
		cancelPushButton->setOn(TRUE);
		
		addAux = addressLocator_->getAddressLocatorMetadata();

		if(!addressLocator_->updateAddressMetadata(m))
		{
			QMessageBox::warning(this, tr("Warning"), tr("The register of layer could not be modified."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

			return;			
		}				
	}
	else
	{
		isInsert = true;

		if(!addressLocator_->insertAddressMetadata(m))
		{
			QMessageBox::warning(this, tr("Warning"), tr("Could not register this layer for geocoding reference."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
			return;
		}

		cancelPushButton->setEnabled(TRUE);
		cancelPushButton->setOn(TRUE);
	}

	if(newColumn)
	{
		// Fecha o grid
		TerraViewBase* mainWindow = (TerraViewBase*)qApp->mainWidget();
		TeQtGrid* grid = mainWindow->getGrid();
		grid->clearPortal();

		if(!addressLocator_->addCompleteNameColumn())
		{
			// Em caso de erro somente reabre o grid
			grid->initPortal();
			grid->refresh();
			grid->goToLastColumn();

			QMessageBox::warning(this, tr("Warning"), addressLocator_->errorMessage().c_str(), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

			if(isInsert)
			{
				if(!addressLocator_->deleteAddressMetadata(m.tableId_))
				{

					QMessageBox::warning(this, tr("Warning"), tr("Error! Please, remove this configuration and try again!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

					cancelPushButton->setEnabled(TRUE);
					cancelPushButton->setOn(TRUE);
				}
				else
				{
					cancelPushButton->setEnabled(FALSE);
					cancelPushButton->setOn(FALSE);
				}				
			}
			else
			{
				if(!addressLocator_->updateAddressMetadata(addAux))
				{
					QMessageBox::warning(this, tr("Warning"), tr("Could not undo the operation! Please, remove this configuration and try again!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

					cancelPushButton->setEnabled(TRUE);
					cancelPushButton->setOn(TRUE);
				}
				else
				{
					cancelPushButton->setEnabled(FALSE);
					cancelPushButton->setOn(FALSE);
				}			
			}

			return;
		}

		if(!db_->createIndex(addressLocator_->getAddressLocatorMetadata().tableName_, "att_idx_" + addressLocator_->getAddressLocatorMetadata().locationCompleteName_, addressLocator_->getAddressLocatorMetadata().locationCompleteName_))
			QMessageBox::warning(this, tr("Warning"), tr("Couldn't add an index to specified column!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		// Update all the themes that uses this table
		set<TeLayer*> layerSet;
		vector<TeQtViewItem*> viewItemVec = mainWindow_->getViewsListView()->getViewItemVec();
		for (unsigned int i = 0; i < viewItemVec.size(); ++i)
		{
			vector<TeQtThemeItem*> themeItemVec = viewItemVec[i]->getThemeItemVec();
			for (unsigned int j = 0; j < themeItemVec.size(); ++j)
			{
				TeTheme* theme = (TeTheme*)(themeItemVec[j]->getAppTheme())->getTheme();
				if(theme->type() == TeTHEME)
				{
					if (theme->isThemeTable(addressLocator_->getAddressLocatorMetadata().tableName_) == true)
					{
						theme->loadThemeTables();
						layerSet.insert(theme->layer());
					}
				}
			}
		}

		// Update the layer tables affected
		set<TeLayer*>::iterator setIt;
		for (setIt = layerSet.begin(); setIt != layerSet.end(); ++setIt)
			(*setIt)->loadLayerTables();

		grid->initPortal();
		grid->refresh();
		grid->goToLastColumn();
	}
	else
	{
		if(m.locationCompleteName_ != addAux.locationCompleteName_)
		{
			int response = QMessageBox::question(this, tr("Question"),
			    		   tr("The column:") + m.locationCompleteName_.c_str() + 
					       tr(" must be converted to uppercase in order to do a best geocoding. Do you want to convert it now?"),
					       tr("Yes"), tr("No"));

			if(response == 0 && !addressLocator_->convertColumnNameToUpper())
				QMessageBox::warning(this, tr("Warning"), addressLocator_->errorMessage().c_str(), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
			
		}
	}

	accept();
}


void LBSDefinition::cancelPushButton_clicked()
{
	if(addressLocator_->deleteAddressMetadata(currentTable_))
	{
		cancelPushButton->setEnabled(FALSE);
		cancelPushButton->setOn(FALSE);
	}
	else
		QMessageBox::warning(this, tr("Warning"), tr("Could not be possible to remove the geocoding reference information for this layer."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
    
}


void LBSDefinition::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("spatializationWindow.htm");
	if(help_->erro_ == false)
	{
		help_->show();
		help_->raise();
		help_->scrollToAnchor("lbsDefinition");
	}
	else
	{
		delete help_;
		help_ = 0;
	}
}


void LBSDefinition::layerTableComboBox_activated(int /* index */)
{
	// Limpa as combobox
	leftBeginComboBox->clear();
    rightBeginComboBox->clear();
    leftEndComboBox->clear();
	locationTypeComboBox->clear();
	locationTitleComboBox->clear();
	locationPrepositionComboBox->clear();
    locationComboBox->clear();
	completeLocationNameComboBox->clear();
    rightEndComboBox->clear();
    bairroRightComboBox->clear();
    bairroLeftComboBox->clear();
    CEPLeftComboBox->clear();
    CEPRightComboBox->clear();

	// Desablita os controles
	cancelPushButton->setEnabled(FALSE);
	cancelPushButton->setOn(FALSE);

	columnTableGroupBox->setEnabled(FALSE);


	// Encontra a tabela de atributos corrente
	TeTable attTable;

	if(!currentLayer_->getAttrTablesByName(layerTableComboBox->currentText().latin1(), attTable, TeAttrStatic))
	{
		QMessageBox::warning(this, tr("Warning"), tr("No attribute tables could be found for this layer."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return;
	}

	// Marca a tabela de atributos ativa
	currentTable_ = attTable.id();

	// Preenche os combobox com os nomes das colunas
	TeAttributeList& attList = attTable.attributeList();

	unsigned int i = 0;

	leftBeginComboBox->insertItem("");
	rightBeginComboBox->insertItem("");
	leftEndComboBox->insertItem("");
	locationTypeComboBox->insertItem("");
	locationTitleComboBox->insertItem("");
	locationPrepositionComboBox->insertItem("");
	locationComboBox->insertItem("");
	completeLocationNameComboBox->insertItem("");
	rightEndComboBox->insertItem("");
	bairroRightComboBox->insertItem("");
	bairroLeftComboBox->insertItem("");
	CEPLeftComboBox->insertItem("");
	CEPRightComboBox->insertItem("");

	for(i = 0; i < attList.size(); ++i)
	{
		if(attList[i].rep_.type_ == TeINT || attList[i].rep_.type_ == TeREAL)
			leftBeginComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeINT || attList[i].rep_.type_ == TeREAL)
			rightBeginComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeINT || attList[i].rep_.type_ == TeREAL)
			leftEndComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeSTRING)
			locationTypeComboBox->insertItem(attList[i].rep_.name_.c_str());

		
		if(attList[i].rep_.type_ == TeSTRING)
			locationTitleComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeSTRING)
			locationPrepositionComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeSTRING)
			locationComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeSTRING)
			completeLocationNameComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeINT || attList[i].rep_.type_ == TeREAL)
			rightEndComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeSTRING)
			bairroRightComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeSTRING)
			bairroLeftComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeSTRING)
			CEPLeftComboBox->insertItem(attList[i].rep_.name_.c_str());

		if(attList[i].rep_.type_ == TeSTRING)
			CEPRightComboBox->insertItem(attList[i].rep_.name_.c_str());
	}

	if(!i)
	{
		QMessageBox::warning(this, tr("Warning"), tr("The attributes of the table were not found."), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return;
	}

	confirmPushButton->setEnabled(TRUE);
    confirmPushButton->setOn(TRUE);

	columnTableGroupBox->setEnabled(TRUE);

	if(addressLocator_->loadAddressMetadata(currentTable_))
	{
		leftBeginComboBox->setCurrentText(addressLocator_->getAddressLocatorMetadata().initialLeftNumber_.c_str());
		rightBeginComboBox->setCurrentText(addressLocator_->getAddressLocatorMetadata().initialRightNumber_.c_str());
		leftEndComboBox->setCurrentText(addressLocator_->getAddressLocatorMetadata().finalLeftNumber_.c_str());
		rightEndComboBox->setCurrentText(addressLocator_->getAddressLocatorMetadata().finalRightNumber_.c_str());
		locationTypeComboBox->setCurrentText(addressLocator_->getAddressLocatorMetadata().locationType_.c_str());
		locationTitleComboBox->setCurrentText(addressLocator_->getAddressLocatorMetadata().locationTitle_.c_str());
		locationPrepositionComboBox->setCurrentText(addressLocator_->getAddressLocatorMetadata().locationPreposition_.c_str());
		locationComboBox->setCurrentText(addressLocator_->getAddressLocatorMetadata().locationName_.c_str());
		completeLocationNameComboBox->setCurrentText(addressLocator_->getAddressLocatorMetadata().locationCompleteName_.c_str());
		bairroLeftComboBox->setCurrentText(addressLocator_->getAddressLocatorMetadata().leftNeighborhood_.c_str());
		bairroRightComboBox->setCurrentText(addressLocator_->getAddressLocatorMetadata().rightNeighborhood_.c_str());
		CEPLeftComboBox->setCurrentText(addressLocator_->getAddressLocatorMetadata().leftZipCode_.c_str());
		CEPRightComboBox->setCurrentText(addressLocator_->getAddressLocatorMetadata().rightZipCode_.c_str());

		cancelPushButton->setEnabled(TRUE);
		cancelPushButton->setOn(TRUE);
	}	
}


TeLayer* LBSDefinition::getLayer()
{
    return currentLayer_;

}


void LBSDefinition::listLayers( const unsigned int & layerId )
{
	// Fill the layer combobox with the list of layers of the current database
	layerComboBox->clear();

	currentLayer_ = 0;

	TeLayerMap& layerMap = db_->layerMap();

	TeLayerMap::iterator layerIt;

	for(layerIt = layerMap.begin(); layerIt != layerMap.end(); ++layerIt)
	{
		if(layerIt->second->geomRep() & TeLINES)
		{
			QString layerName = layerIt->second->name().c_str();

			layerComboBox->insertItem(layerName);

			if(layerIt->second->id() == (int)layerId)
				currentLayer_ = layerIt->second;
		}
	}

	// S� habilita a lista de layer se a lista contiver pelo menos 1 layer na lista.
	if(layerComboBox->count() > 0)
		layerComboBox->setEnabled(TRUE);
	else
		return;


	//If there is a current layer, set it as the current item on the layer�s combobox;
	//currentLayer_ = mainWindow_->currentLayer();

	if(currentLayer_)
	{
		int i;
		for(i = 0; i < layerComboBox->count(); ++i)
		{
			if(layerComboBox->text(i).latin1() == currentLayer_->name())
			{
				layerComboBox->setCurrentItem(i);
				break;
			}
		}

		if(i == layerComboBox->count())
			layerComboBox->setCurrentItem(0);
	}
	else
		layerComboBox->setCurrentItem(0);

	layerComboBox_activated(layerComboBox->currentItem());	
}
