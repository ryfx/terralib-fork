/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <qfiledialog.h>
#include <qmessagebox.h>

void plgGenerator::okBTN_pressed()
{
    if(plgName_LNE_->text().isEmpty() ||
		desc_LNE_->text().isEmpty() ||
		className_LNE_->text().isEmpty() ||
		location_LNE_->text().isEmpty() ||
		plgVendor_LNE_->text().isEmpty() ||
		homePage_LNE_->text().isEmpty() ||
		mail_LNE_->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Plugin generator error"), tr("All parameters must be filled."));
		return;
	}

	accept();
}

void plgGenerator::cancelBTN_pressed()
{
	reject();
}

void plgGenerator::locationBTN_pressed()
{
	QString dirName = QFileDialog::getExistingDirectory(QString::null, this, 0, tr("Select a directory to generate plugin"));

	if(!dirName.isEmpty())
		location_LNE_->setText(dirName);
}
