/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

// Qt
#include <qmessagebox.h>
#include <qvalidator.h>

void DatumWindow::okPushButton_clicked()
{
	QString datumName = datumNameLineEdit->text();
	if(datumName.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Give the name of the new datum."));
		datumNameLineEdit->setFocus();
		return;
	}

	std::string name = datumName.latin1();
	if(!editMode_)
	{
		TeDatumMap& datumMap = TeDatumFactory::getDatumMap();
		TeDatumMap::iterator it = datumMap.find(name);
		if(it != datumMap.end())
		{
			QMessageBox::warning(this, tr("Warning"), tr("There is already a datum with this name in the database."));
			return;
		}
	}

	QString radius = radiusLineEdit->text();
	if(radius.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Give the radius to the new datum."));
		radiusLineEdit->setFocus();
		return;
	}

	QString flattening = flatteningLineEdit->text();
	if(flattening.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Give the flattening to the new datum."));
		flatteningLineEdit->setFocus();
		return;
	}

	QString dxValue = dxLineEdit->text();
	if(dxValue.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Give the shift Dx to the new datum."));
		dxLineEdit->setFocus();
		return;
	}

	QString dyValue = dyLineEdit->text();
	if(dyValue.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Give the shift Dy to the new datum."));
		dyLineEdit->setFocus();
		return;
	}

	QString dzValue = dzLineEdit->text();
	if(dzValue.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Give the shift Dz to the new datum."));
		dzLineEdit->setFocus();
		return;
	}

	// Converts the values to create the new datum
	double rd = atof(radius.latin1());
	double flt = atof(flattening.latin1());
	double dx = atof(dxValue.latin1());
	double dy = atof(dyValue.latin1());
	double dz = atof(dzValue.latin1());

	// Builds the new datum
	datum_ = TeDatum(name, rd, flt, dx, dy, dz);

	// Gets the current database
	TeDatabase* db = mainWindow_->currentDatabase();
	if(db == 0)
	{
		QMessageBox::critical(this, tr("Error"), tr("There is no current database."));
		return;
	}

	if(!editMode_)
	{
		// Saves to database
		if(!db->insertDatum(datum_))
			QMessageBox::critical(this, tr("Error"), tr("Error saving the datum on database."));
		else
			QMessageBox::information(this, tr("Information"), tr("The datum was created successfully."));
	}
	else
	{
		// Updates datum on database
		if(!db->updateDatum(datum_))
			QMessageBox::critical(this, tr("Error"), tr("Error updating the datum on database."));
		else
			QMessageBox::information(this, tr("Information"), tr("The datum was updated successfully."));
	}

	close();
}

void DatumWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "Help", false);
	help_->init("datum.htm");
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

void DatumWindow::loadDatum( const TeDatum & datum )
{
	datumNameLineEdit->setText(datum.name().c_str());
	radiusLineEdit->setText(Te2String(datum.radius(), 6).c_str());
	flatteningLineEdit->setText(Te2String(datum.flattening(), 10).c_str());
	dxLineEdit->setText(Te2String(datum.xShift(), 6).c_str());
	dyLineEdit->setText(Te2String(datum.yShift(), 6).c_str());
	dzLineEdit->setText(Te2String(datum.zShift(), 6).c_str());
}

TeDatum DatumWindow::getDatum()
{
	return datum_;
}

void DatumWindow::init()
{
	radiusLineEdit->setValidator(new QDoubleValidator(this));
	flatteningLineEdit->setValidator(new QDoubleValidator(this));
	dxLineEdit->setValidator(new QDoubleValidator(this));
	dyLineEdit->setValidator(new QDoubleValidator(this));
	dzLineEdit->setValidator(new QDoubleValidator(this));

	help_= 0 ;
	mainWindow_= (TerraViewBase*)qApp->mainWidget();

	editMode_ = false;
}

void DatumWindow::setEditMode()
{
	editMode_ = true;
	datumNameLineEdit->setEnabled(false);
}
