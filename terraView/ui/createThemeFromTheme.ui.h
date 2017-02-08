/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "TeDatabase.h"
#include "TeAppTheme.h"
#include "TeThemeFunctions.h"
#include "TeQtThemeItem.h"
#include <qmessagebox.h>

void CreateThemeFromTheme::okPushButton_clicked()
{
	if (themeNameLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Inform a name for the new theme!"));
		return;
	}
	if (db_->themeExist(themeNameLineEdit->text().latin1()))
	{
		QMessageBox::warning(this, tr("Warning"), tr("There is already a theme with this name!"));
		return;
	}
	
	int selobj = objectsButtonGroup->selectedId();
	if (!((TeTheme*)theme_->getTheme())->layer()->hasGeometry(TeRASTER))
	{
		// First check if selection will result in an object
		string input; 
		if(selobj == 1) // queried
		{
			input = "(" + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_status = 2";
			input += " OR " + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_status = 3)";
		}
		else if(selobj == 2) // pointed
		{
			input = "(" + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_status = 1";
			input += " OR " + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_status = 3)";
		}
		else if(selobj == 3) // not queried
		{
			input = "(" + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_status <> 2";
			input += " AND " + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_status <> 3)";
		}
		else if(selobj == 4) // not pointed
		{
			input = "(" + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_status <> 1";
			input += " AND " + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_status <> 3)";
		}
		else if(selobj == 5) // pointed and queried
		{
			input = "(" + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_status = 3)";
		}
		else if(selobj == 6) // pointed or queried
		{
			input = "(" + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_status = 2";
			input += " OR " + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_status = 1";
			input += " OR " + ((TeTheme*)theme_->getTheme())->collectionTable() + ".c_object_status = 3)";
		}

		// Initally test if there is any object selected
	
		string sql = "SELECT c_object_id FROM " + ((TeTheme*)theme_->getTheme())->collectionTable();
		if (!input.empty())
			sql += " WHERE " + input;
		TeDatabasePortal* portal = db_->getPortal();
		if (!portal->query(sql))
		{
			delete portal;
			QString msg = tr("Fail to create a theme from the theme") + " \"" + ((TeTheme*)theme_->getTheme())->name().c_str() + "\" !";
			QMessageBox::critical(this, tr("Error"), msg);
			reject();
			return;
		}
		if (!portal->fetchRow())
		{
			delete portal;
			QButton* selB = objectsButtonGroup->selected();
			QString msg = tr("No objects will be selected:") + " " + selB->text();
			QMessageBox::warning(this, tr("Warning"), msg);
			objectsButtonGroup->setButton(0);
			return;
		}
		delete portal;
	}
	
	TeTheme *inTheme = ((TeTheme*)theme_->getTheme());
	newTheme_ = new TeTheme();
	if (TeCreateThemeFromTheme(inTheme, themeNameLineEdit->text().latin1(),selobj, newTheme_))
		accept();
	else
	{
		if(newTheme_)
			delete newTheme_;
		newTheme_ = 0;
		reject();
	}
}



void CreateThemeFromTheme::init(TeQtThemeItem* themeItem)
{
	help_ = 0;
	newTheme_ = 0;
	theme_ = (TeAppTheme*)themeItem->getAppTheme();
	if (!theme_)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select a theme!"));
		return;
	}
	if(theme_->getTheme()->type() != TeTHEME)
		return;
	objectsButtonGroup->setButton(2);
}


TeAppTheme* CreateThemeFromTheme::getNewTheme()
{
	TeAppTheme* t = new TeAppTheme(newTheme_);
	return t;
}


void CreateThemeFromTheme::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("createThemeFromTheme.htm");
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


void CreateThemeFromTheme::setParams( TeDatabase * db )
{
	db_ = db;
}


void CreateThemeFromTheme::show()
{
	if(theme_->getTheme()->type() == TeTHEME)
		QDialog::show();
	else
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		reject();
		hide();
	}
}
