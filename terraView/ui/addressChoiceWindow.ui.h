/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/


void AddressChoiceWindow::init()
{
	help_ = 0;
	choosedAddress_ = -1;

	addressTable->setSelectionMode(QTable::SingleRow);
}


void AddressChoiceWindow::listFoundedAddressListBox(vector<TeAddressDescription>& addresses)
{
    addressTable->setNumRows(addresses.size());

	for(unsigned int i = 0; i < addresses.size(); ++i)
    {		
	    addressTable->setText(i, 0, addresses[i].locationLink_.c_str());

		addressTable->setText(i, 1, addresses[i].locationType_.c_str());

		addressTable->setText(i, 2, addresses[i].locationTitle_.c_str());

		addressTable->setText(i, 3, addresses[i].locationPreposition_.c_str());

		addressTable->setText(i, 4, addresses[i].locationName_.c_str());

		string aux = "";

		aux = Te2String(addresses[i].initialLeftNumber_);
		addressTable->setText(i, 5, aux.c_str());

		aux = Te2String(addresses[i].finalLeftNumber_);
		addressTable->setText(i, 6, aux.c_str());

		aux = Te2String(addresses[i].initialRightNumber_);
		addressTable->setText(i, 7, aux.c_str());
		
		aux = Te2String(addresses[i].finalRightNumber_);
		addressTable->setText(i, 8, aux.c_str());		

		addressTable->setText(i, 9, addresses[i].leftNeighborhood_.c_str());

		addressTable->setText(i, 10, addresses[i].rightNeighborhood_.c_str());

		addressTable->setText(i, 11, addresses[i].leftZipCode_.c_str());

		addressTable->setText(i, 12, addresses[i].rightZipCode_.c_str());
	}
}


void AddressChoiceWindow::listSearchedAddress(const TeAddress& add)
{
	typeLineEdit->setText(add.locationType_.c_str());
	titleLineEdit->setText(add.locationTitle_.c_str());
	prepositionLineEdit->setText(add.locationPreposition_.c_str());
	locationNameLineEdit->setText(add.locationName_.c_str());
	numberLineEdit->setText(Te2String(add.locationNumber_).c_str());
	neighborhoodLineEdit->setText(add.neighborhood_.c_str());
	zipCodeLineEdit->setText(add.zipCode_.c_str());
}


int AddressChoiceWindow::getChoosedAddress()
{
	return choosedAddress_;
}


void AddressChoiceWindow::confirmPushButton_clicked()
{
   choosedAddress_ = addressTable->currentRow();
   
   this->accept();
    
}


void AddressChoiceWindow::selectAllCheckBoxVisible( const bool & visible )
{
	if(visible)
		selectAllCheckBox->show();
	else
		selectAllCheckBox->hide();
}


void AddressChoiceWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("addressChoiceWindow.htm");
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
