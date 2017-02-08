/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <qmessagebox.h>


void AddressNameSeparator::init()
{
	help_ = 0;
}

void AddressNameSeparator::closePushButton_clicked()
{
    numberLineEdit->clear();
    this->reject();
}


void AddressNameSeparator::confirmPushButton_clicked()
{
    if(numberLineEdit->text().isEmpty())
    {
		QString msg = tr("You should enter a character!");
		QMessageBox::warning( this, tr("Warning"), msg);
		return;
    }
    this->accept();
}


void AddressNameSeparator::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("addressNameSeparator.htm");
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

