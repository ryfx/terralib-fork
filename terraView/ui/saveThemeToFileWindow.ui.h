/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright ? 2001-2007 INPE and Tecgraf/PUC-Rio.
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
#include <TeProgress.h>
#include <TeAsciiFile.h>
#include <TeWaitCursor.h>
#include <TeQtThemeItem.h>
#include <TeAppTheme.h>
#include <TeTheme.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qfile.h>
#include <help.h>

#include <TeDriverSHPDBF.h>
#include <TeDriverMIDMIF.h>
#include <TeDriverSPRING.h>
#include <TeDriverCSV.h>

void SaveThemeToFileWindow::init(TeQtThemeItem* themeItem)
{
    if (!themeItem)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select a theme!"));
		return;
	}
	help_ = 0;
	curFormat_ = "spr";
	catSPRComboBox->setCurrentItem(0);
	attrComboBox->setEnabled(false);
	catObjLineEdit->setEnabled(true);
	catObjLineEdit->setText("Object");
	curTheme_ = 0;
	colRradioButton->setChecked(true);

	TeAppTheme* appTheme = themeItem->getAppTheme();
	TeTheme* theme = (TeTheme*)appTheme->getTheme();
	TeGeomRep rep = (TeGeomRep)theme->visibleGeoRep();
	if (rep & TePOLYGONS || rep & TeLINES || rep & TePOINTS || rep & TeCELLS)
	{
		curTheme_ = appTheme;
		string filename = string(lastOpenDir_.latin1()) + "/" + string(theme->name()) + ".spr"; 
		fileNameLineEdit->setText(filename.c_str());	
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("Theme has no vector representation (Points, Lines, Polygons or Cells)!"));
		return;
	}
	if (rep & TePOINTS)
		csvRadioButton->setEnabled(true);
	else 
		csvRadioButton->setEnabled(false);

	catObjLineEdit->setText(theme->name().c_str());
}

void SaveThemeToFileWindow::exportButton_clicked()
{	
	if (fileNameLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Select the name of the output file!"));
		return;
	}

	//Verify the name
	std::string baseName = TeGetBaseName(fileNameLineEdit->text());

	bool changed = false;
	std::string invalidChar = "";
	TeCheckName(baseName, changed, invalidChar);

	if(changed)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Output file name invalid!"));
		return;
	}
	//End

	QString fname = fileNameLineEdit->text();
	int i;

	TeTheme* curBaseTheme = (TeTheme*)curTheme_->getTheme();

	if (formatButtonGroup->selectedId() == 2 )
	{
		i = fileNameLineEdit->text().findRev('.');
		if (i != -1)
			fname = fileNameLineEdit->text().left(i);
		else
			fname = fileNameLineEdit->text();
		TeGeomRep rep = (TeGeomRep)curBaseTheme->visibleGeoRep();
		if (rep & TePOLYGONS || rep & TeCELLS)
			fname += "_pol.shp";
		if (rep & TeLINES)
			fname += "_lin.shp";
		if (rep & TeLINES)
			fname += "_point.shp";
	}

	int answer = 0;
    if (QFile::exists(fname))
	{
        answer = QMessageBox::question(this,
                        tr("Vectorial Export"),
                        QString( tr("Overwrite") + "\n\'%1\'?" ).
                            arg( fname ),
                        tr("&Yes"), tr("&No"), QString::null, 1, 1 );
		if ( answer == 1 ) 
		{
			fileNameLineEdit->clear();
			return;
		}
	}
	i = fileNameLineEdit->text().findRev('.');
	if (i != -1)
		fname = fileNameLineEdit->text().left(i);
	else
		fname = fileNameLineEdit->text();

	TeWaitCursor wait;
	bool res=false;

	if (formatButtonGroup->selectedId() == 0)
	{
		if (catSPRComboBox->currentItem()==3 && !(curBaseTheme->visibleGeoRep() & TeLINES || curBaseTheme->visibleGeoRep() & TePOINTS))
		{
			wait.resetWaitCursor();
			QMessageBox::warning(this, tr("Warning"),tr("The Theme doesn't have lines or points to be exported to a ASCII-SPRING MNT model."));
			return;
		}
 		
		if (catSPRComboBox->currentItem()==3 && attrComboBox->count()==0)
		{
			wait.resetWaitCursor();
			QMessageBox::warning(this, tr("Warning"),tr("The Theme doesn't have a numeric attribute to be used as quote values in a ASCII-SPRING MNT model."));
			return;
		}

		if (catSPRComboBox->currentItem()==2 && attrComboBox->count()==0)
		{
			wait.resetWaitCursor();
			QMessageBox::warning(this, tr("Warning"),tr("The Theme doesn't have an integral attribute to be used as the class information in a ASCII-SPRING Thematic model"));
			return;
		}

		string colName = "";
		if (catSPRComboBox->currentItem() < 2)
		{
			colName =  catObjLineEdit->text().latin1();
			if (colName.empty())
				colName = "Objeto";
		}
		else
			colName = attrComboBox->currentText().latin1();


		if(TeProgress::instance())
		{
			QString caption = tr("Exporting");
			TeProgress::instance()->setCaption(caption.latin1());
			QString msg = tr("Exporting vector to ASCII-SPRING format. Please, wait!");
			TeProgress::instance()->setMessage(msg.latin1());
		}
		res = TeExportThemeToSPRING(curBaseTheme, TeSpringModels(catSPRComboBox->currentItem()), colName, static_cast<TeSelectedObjects>(objectsButtonGroup->selectedId()),fname.latin1());
	}
	else if (formatButtonGroup->selectedId() == 1)
	{
		if(TeProgress::instance())
		{
			QString caption = tr("Exporting");
			TeProgress::instance()->setCaption(caption.latin1());
			QString msg = tr("Exporting vector to MID/MIF format. Please, wait!");
			TeProgress::instance()->setMessage(msg.latin1());
		}
		res = TeExportThemeToMIF(curBaseTheme,static_cast<TeSelectedObjects>(objectsButtonGroup->selectedId()),fname.latin1());
	}
	else if (formatButtonGroup->selectedId() == 2)
	{
		if(TeProgress::instance())
		{
			QString caption = tr("Exporting");
			TeProgress::instance()->setCaption(caption.latin1());
			QString msg = tr("Exporting vector to Shapefile format. Please, wait!");
			TeProgress::instance()->setMessage(msg.latin1());
		}
		res = TeExportThemeToShapefile(curBaseTheme,static_cast<TeSelectedObjects>(objectsButtonGroup->selectedId()),fname.latin1());
	}
	else if (formatButtonGroup->selectedId() == 3)
	{
		if(TeProgress::instance())
		{
			QString caption = tr("Exporting");
			TeProgress::instance()->setCaption(caption.latin1());
			QString msg = tr("Exporting vector to ASCII format. Please, wait!");
			TeProgress::instance()->setMessage(msg.latin1());
		}
		fname += ".txt";
		res = TeExportThemeToCSV(curBaseTheme,static_cast<TeSelectedObjects>(objectsButtonGroup->selectedId()),fname.latin1(),';');
	}
	else
	{
		wait.resetWaitCursor();
		QMessageBox::warning(this, tr("Warning"), tr("Select a format!"));
	}
	wait.resetWaitCursor();
	if(TeProgress::instance())
		TeProgress::instance()->reset();

	if (res)
	{
		QMessageBox::information(this,tr("Information"),tr("The theme was successfully exported!"));
		accept();
	}
	else
	{
		QMessageBox::critical(this, tr("Error"), tr("Fail to export the data!"));
		reject();
	}
}

void SaveThemeToFileWindow::formatComboBox_activated( int fmt)
{
	if (fmt == 0)
	{
		catSPRComboBox->setEnabled(true);
		catSPRComboBox->setCurrentItem(0);
		catObjLineEdit->setEnabled(true);
		attrComboBox->setEnabled(false);
	}
	else
	{
		catSPRComboBox->setEnabled(false);
		attrComboBox->setEnabled(false);
		catObjLineEdit->setEnabled(false);
	}
}


void SaveThemeToFileWindow::catSPRComboBox_activated( int cat)
{
	if (!curTheme_)
		return;

	if (cat <2)
	{
		catObjLineEdit->setEnabled(true);
		attrComboBox->setEnabled(false);
	}
	else 
	{
		catObjLineEdit->setEnabled(false);
		attrComboBox->setEnabled(true);
		attrComboBox->clear();

		bool showMess = false;
		TeAttrTableVector& themeTables = ((TeTheme*)curTheme_->getTheme())->attrTables();
		TeAttrTableVector::const_iterator it = themeTables.begin();
		TeAttrTableVector::const_iterator itend = themeTables.end();
		while (it != itend)
		{
			TeTable table = *it;
			if (table.tableType()!= TeAttrStatic && 
				table.tableType() != TeAttrEvent && 
				table.tableType() != TeFixedGeomDynAttr) 
			{
				++it;
				showMess = true;
				continue;
			}
			unsigned int i;
			if (cat == 3)		// only numerical attributes
			{	
				attrTextLabel->setText(tr("Elevation:"));		
				for (i=0; i<table.attributeList().size(); ++i)
				{   
					if (table.attributeList()[i].rep_.type_ == (TeAttrDataType)TeREAL  || table.attributeList()[i].rep_.type_ == (TeAttrDataType)TeDOUBLE ||
					    table.attributeList()[i].rep_.type_ == (TeAttrDataType)TeFLOAT || table.attributeList()[i].rep_.type_ == (TeAttrDataType)TeINT||
					    table.attributeList()[i].rep_.type_ == (TeAttrDataType)TeUNSIGNEDINT)
						attrComboBox->insertItem(table.attributeList()[i].rep_.name_.c_str());
				}
			}
			else
			{
				attrTextLabel->setText(tr("Class:"));		
				for (i=0; i<table.attributeList().size(); ++i)
				{
					if (table.attributeList()[i].rep_.type_ == (TeAttrDataType)TeSTRING || 
						table.attributeList()[i].rep_.type_ == (TeAttrDataType)TeINT ||
						table.attributeList()[i].rep_.type_ == (TeAttrDataType)TeUNSIGNEDINT)
						attrComboBox->insertItem(table.attributeList()[i].rep_.name_.c_str());
				}
			}
			++it;
		}
		if (showMess)
			QMessageBox::warning(this, tr("Warning"), tr("External tables won't be exported."));
	}
}


void SaveThemeToFileWindow::outputFileButton_clicked()
{
	QString ext;
	int fmt = formatButtonGroup->selectedId();
	if (fmt == 0 )
		ext = ".spr";
	else if (fmt == 1)
		ext = ".mid";
	else if (fmt == 2)
		ext = ".shp";
	else 
		ext = ".txt";

	QString filter = "*";
	filter += ext;
	QString curPath = fileNameLineEdit->text();
	if (!curPath.isEmpty())
	{
		int n = curPath.findRev('/');
		if (n != -1)
			curPath.truncate(n);
	}
	QString s = QFileDialog::getSaveFileName( curPath,filter,this,tr("Save As"), tr("File Name"));
	if (s.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Choose a valid name and path for the output file!"));
	}
	else
	{
		QString fname;
		int n = s.findRev('.');
		int m = s.findRev('/');
		if ((n != -1) && (n > m))
			fname = s.left(n);
		else
			fname = s;
		if (m != -1)
			lastOpenDir_ = fname.left(m);
		else
			lastOpenDir_ = fname;
		fname = fname + ext;
		fileNameLineEdit->setText(fname);
	}
}

void SaveThemeToFileWindow::formatButtonGroup_clicked( int n)
{
    if (n != 0)
	    sprGroupBox->setEnabled(false);
	else
	    sprGroupBox->setEnabled(true);
	if (!fileNameLineEdit->text().isEmpty())
	{
		QString curFilename = fileNameLineEdit->text();
		curFilename.truncate(curFilename.length()-3);
		if (n == 0)
			curFilename += "spr";
		else if (n == 1)
			curFilename += "mif";
		else if (n == 2)
			curFilename += "shp";
		else
			curFilename += "txt";

		fileNameLineEdit->setText(curFilename);
	}
}

void SaveThemeToFileWindow::helpButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("salvar_tema_arquivo.htm");
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


void SaveThemeToFileWindow::setParams( const QString & lastOpenDir )
{
	lastOpenDir_ = lastOpenDir;
}


QString SaveThemeToFileWindow::getLastOpenDir()
{
	return lastOpenDir_;
}


void SaveThemeToFileWindow::helpButtonSlot()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("SaveThemeToFileWindow.htm");
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
