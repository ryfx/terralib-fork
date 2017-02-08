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

#include <visual.h>

#include <lbsDefinition.h>

#include <display.h>

#include <TeAddressNotifyFunctorWindow.h>

#include <qtabwidget.h>


void AddressLocatorWindow::findLocation()
{
	// Pega os valores do formulário
	TeAddress address;
	
	address.locationType_ = (locationTypeComboBox->isEnabled() ? locationTypeComboBox->currentText().latin1() : "");
	address.locationTitle_ = (locationTitleComboBox->isEnabled() ? locationTitleComboBox->currentText().latin1() : "");
	address.locationPreposition_ = (locationPrepositionComboBox->isEnabled() ? locationPrepositionComboBox->currentText().latin1() : "");
	address.locationName_ = locationNameLineEdit->text().latin1();
	address.locationNumber_ = atoi(numberLineEdit->text().latin1()); 
	address.neighborhood_ = neighborhoodLineEdit->text().latin1();
	address.zipCode_ = zipCodeLineEdit->text().latin1();

	if(address.locationName_.empty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("You should type at least the location name!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
		
		return;
	}

	// Tenta localizar o endereço
	vector<TeAddressDescription> addressesFound;

	int status = addressLocator_->findAddress(address, addressesFound);

	QString result = "";

	switch(status)
	{
		case 0    : result = tr("Database connection problem"); break;
		case 1    :	result = tr("Address not found"); break;
		case 2    :	result = tr("Can't do geocoding with an empty address name"); break;
		case 3    :	result = tr("Error on query the database"); break;
	}
			
	if((status >= 4) && (addressesFound.size() > 0))
	{
		TeAddressDescription addDescSimilar = addressesFound[0];
			
		if(addressesFound.size() > 1)
		{
		    TeAbstractAddressNotifyFunctor* notifyWindow = new TeAdressNotifyFunctorWindow(this, false);
		    int chosed =  (*notifyWindow)(address, addressesFound);

			delete notifyWindow;


		    if(chosed >= 0)
				addDescSimilar = addressesFound[chosed];
			else
				return;
		}

		if(status != 9)
			addressLocator_->findCoordinate(address, addDescSimilar);

		if(!addDescSimilar.isValid_)
		{
			hasPoint_ = false;

			result = tr("The segment that comprise the location couldn't be founded");

			QMessageBox::warning(this, "Warning", result, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
		}
		else
		{

			TeProjection* ptProj = currTheme_->layer()->projection();

			TeQtCanvas* canvas = mainWindow_->getCanvas();

			canvas->setDataProjection(ptProj);

			if(hasPoint_)
			{
				double pixelWidth = canvas->mapVtoDW(16);

				double h = pixelWidth/2.0;
				double w = pixelWidth/2.0;

				TeBox b;

				b.x1_ = ptFound_.x() - w;
				b.y1_ = ptFound_.y() - h;

				b.x2_ = ptFound_.x() + w;
				b.y2_ = ptFound_.y() + h;

				mainWindow_->getDisplayWindow()->plotData(b, true);	// Apaga o ponto anterior
			}
			
			ptFound_ = addDescSimilar.pt_;		

			TeBox b = canvas->getWorld();

			TePoint ptaux(ptFound_);
			TeBox pointBox(ptaux.box());		

			canvas->setDataProjection(ptProj);
			
			canvas->mapDWtoCW(pointBox); /// pointBox está na mesma projecao que b

			bool voar = TeDisjoint(pointBox, b);

			if(voar)
			{
				double h = b.height()/2.0;
				double w = b.width()/2.0;

				b.x1_ = pointBox.x1() - w;
				b.y1_ = pointBox.y1() - h;

				b.x2_ = pointBox.x1() + w;
				b.y2_ = pointBox.y1() + h;
				
//				canvas->setWorld(b);
				canvas->setWorld(b, mainWindow_->cvContentsW_, mainWindow_->cvContentsH_);

				hasPoint_ = true;

				mainWindow_->getDisplayWindow()->plotData();
			}
			else
			{
				hasPoint_ = true;
				plotPoint();
				canvas->copyPixmapToWindow();	
			}		
		}
	}
	else
	{
		hasPoint_ = false;

		QMessageBox::warning(this, "Warning", result, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	}

	return;
}


void AddressLocatorWindow::init()
{
	help_ = 0;

	// Pega o ponteiro para a aplicação principal do TerraView
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();

	addressLocator_ = 0;

	refreshForm();

	TeLegendEntry& legendEntry = currTheme_->defaultLegend();

	ptVisual_ = *(legendEntry.visual(TePOINTS));

	return;
}


void AddressLocatorWindow::configurePushButtonClicked()
{
	int id = currTheme_->layer()->id();	

	LBSDefinition *lbsWindow = new LBSDefinition(this, "lbsWindow", true);

	lbsWindow->listLayers(id);

	lbsWindow->exec();

	if(lbsWindow->getLayer())
		listComboBoxes();

	delete lbsWindow;
}


bool AddressLocatorWindow::listComboBoxes()
{
	locationTypeComboBox->clear();
	locationTypeComboBox->setEnabled(FALSE);

	locationTitleComboBox->clear();
	locationTitleComboBox->setEnabled(FALSE);

	locationPrepositionComboBox->clear();
	locationPrepositionComboBox->setEnabled(FALSE);	

	locatePushButton->setEnabled(FALSE);
	
	addVec_.clear();

	addressLocator_->getAllAddressMetadataTheme(currTheme_->id(), addVec_);

	if(addVec_.size() == 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("The active theme doesn't have a geocoding address reference layer!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		return false;
	}

	// Carrega o metadado da tabela de endereçamento
	addressLocator_->loadAddressMetadata(addVec_[0].tableId_);

	//addressTableName_ = "";
	//addressTableLinkName_ = "";
	//layerId_ = -1;

	//if(!addressLocator_->findAddressTableInformation(addressTableName_, addressTableLinkName_, layerId_))
	//{
	//	QMessageBox::warning(this, tr("Warning"), tr("Could not find the information of current theme!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	//
	//	return;
	//}

	// Busca o ponteiro para o layer selecionado
	TeDatabase* db = currTheme_->layer()->database();
	//TeLayerMap& layerMap = db->layerMap();

	//currentLayer_ = layerMap[layerId_];

	//if(currentLayer_ == 0)
	//{
	//	QMessageBox::warning(this, tr("Warning"), tr("Could not find the infolayer metadata for the active theme!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

	//	return;
	//}

	// Preenche as combobox de tipo de logradouro, título e preposição.
	
	//Tipo
	if(addressLocator_->getAddressLocatorMetadata().locationType_ != "")
	{
		TeDatabasePortal *p = db->getPortal();

		string sql  = "SELECT DISTINCT ";
		       sql += addressLocator_->getAddressLocatorMetadata().locationType_;
			   sql += " FROM ";
			   sql += addressLocator_->getAddressLocatorMetadata().tableName_;
			   sql += " ORDER BY ";
			   sql += addressLocator_->getAddressLocatorMetadata().locationType_;

		if(p->query(sql))
		{
			while(p->fetchRow())
				locationTypeComboBox->insertItem(p->getData(addressLocator_->getAddressLocatorMetadata().locationType_));

			if(locationTypeComboBox->count() > 0)
				locationTypeComboBox->setEnabled(TRUE);
		}

		delete p;
	}

	// Título
	if(addressLocator_->getAddressLocatorMetadata().locationTitle_ != "")
	{
		TeDatabasePortal *p = db->getPortal();

		string sql  = "SELECT DISTINCT ";
		       sql += addressLocator_->getAddressLocatorMetadata().locationTitle_;
			   sql += " FROM ";
			   sql += addressLocator_->getAddressLocatorMetadata().tableName_;
			   sql += " ORDER BY ";
			   sql += addressLocator_->getAddressLocatorMetadata().locationTitle_;

		if(p->query(sql))
		{
			while(p->fetchRow())
				locationTitleComboBox->insertItem(p->getData(addressLocator_->getAddressLocatorMetadata().locationTitle_));

			if(locationTitleComboBox->count() > 0)
				locationTitleComboBox->setEnabled(TRUE);
		}

		delete p;
	}

	// Preposição
	if(addressLocator_->getAddressLocatorMetadata().locationPreposition_ != "")
	{
		TeDatabasePortal *p = db->getPortal();

		string sql  = "SELECT DISTINCT ";
		       sql += addressLocator_->getAddressLocatorMetadata().locationPreposition_;
			   sql += " FROM ";
			   sql += addressLocator_->getAddressLocatorMetadata().tableName_;
			   sql += " ORDER BY ";
			   sql += addressLocator_->getAddressLocatorMetadata().locationPreposition_;

		if(p->query(sql))
		{
			while(p->fetchRow())
				locationPrepositionComboBox->insertItem(p->getData(addressLocator_->getAddressLocatorMetadata().locationPreposition_));

			if(locationPrepositionComboBox->count() > 0)
				locationPrepositionComboBox->setEnabled(TRUE);
		}

		delete p;
	}	

	// Habilita as caixas de texto de bairro e CEP
	if(addressLocator_->getAddressLocatorMetadata().rightNeighborhood_ != "" || addressLocator_->getAddressLocatorMetadata().leftNeighborhood_ != "")
		neighborhoodLineEdit->setEnabled(TRUE);

	if(addressLocator_->getAddressLocatorMetadata().rightZipCode_ != "" || addressLocator_->getAddressLocatorMetadata().leftZipCode_ != "")
		zipCodeLineEdit->setEnabled(TRUE);

	locatePushButton->setEnabled(TRUE);

	return true;
}


void AddressLocatorWindow::destroy()
{
	if(addressLocator_)
		delete addressLocator_;

	return;
}


void AddressLocatorWindow::plotPoint()
{
	if(hasPoint_)
    {
		TeQtCanvas* canvas = mainWindow_->getCanvas();
		TeProjection* ptProj = currTheme_->layer()->projection();
		canvas->setDataProjection(ptProj);	
		canvas->plotOnPixmap0();

		TeColor& color = ptVisual_.color();
		
		canvas->setPointColor(color.red_, color.green_, color.blue_);
		canvas->setPointStyle((int)ptVisual_.style(), ptVisual_.size());
		canvas->plotPoint(ptFound_);
    }
}


bool AddressLocatorWindow::refreshForm()
{
	hasPoint_ = false;
	_wasInited = false;
	
	// Seleciona o tema corrente
	currTheme_ = (TeTheme*)mainWindow_->currentTheme()->getTheme();
	

	if(!currTheme_)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Please, select a theme!"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

		locatePushButton->setEnabled(FALSE);

		return false;
	}

	if(addressLocator_)
	{
	    delete addressLocator_;
	    addressLocator_ = 0;
	}

	// Creates an address locator
	addressLocator_ = new TeAddressLocator(currTheme_->layer()->database());

	addressLocator_->createAddressMetadataTable();

	// Manda preencher as informações pré-definidas do formulário
	if(!listComboBoxes())
	{
		return false;
	}

	_wasInited = true;

	return true;
}


void AddressLocatorWindow::visualPushButton_clicked()
{
	TeLegendEntry legend;

	legend.setVisual(ptVisual_.copy(), TePOINTS);

	VisualWindow *visualWindow = new VisualWindow(this, "visualWindow",true);
	visualWindow->setCaption(tr("Default Visual"));
	visualWindow->loadLegend(legend, currTheme_);

	visualWindow->visualTabWidget->setTabEnabled(visualWindow->linePage, false);
	visualWindow->visualTabWidget->setTabEnabled(visualWindow->pointPage, true);
	

	//TeColor color = ptVisual_.color();
	//visualWindow->pointPenColor_.setRgb(color.red_, color.green_, color.blue_);

	int pointSize = ptVisual_.size();
	visualWindow->pointSizeComboBox->setCurrentText(Te2String(pointSize).c_str());

	int pointStyle = ptVisual_.style();
	visualWindow->pointStyleComboBox->setCurrentItem(pointStyle);

	visualWindow->visualTabWidget->setCurrentPage(2);

	visualWindow->exec();

	ptVisual_ = *(legend.visual(TePOINTS));

	delete visualWindow;	
}


void AddressLocatorWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("spatializationWindow.htm");
	if(help_->erro_ == false)
	{
		help_->show();
		help_->raise();
		help_->scrollToAnchor("addressLocatorWindow");
	}
	else
	{
		delete help_;
		help_ = 0;
	}
}


void AddressLocatorWindow::show()
{
	TeAppTheme* curAppTheme = mainWindow_->currentTheme();
	TeTheme* curTheme = (TeTheme*)curAppTheme->getTheme();
	if(curTheme->type() == TeTHEME)
		QDialog::show();
	else
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		reject();
		hide();
	}
}
