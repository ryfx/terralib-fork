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
#include "importTbl.h"
#include "TeAsciiFile.h"
#include "TeWaitCursor.h"
#include "TeDatabase.h"
#include <TeDriverSHPDBF.h>
#include <TeDriverCSV.h>
#include <TeDatabaseFactory.h>

#include <qfile.h>
#include <qmessagebox.h>

void ImportTbl::init()
{
	help_ = 0;
	db_ = 0;
	lastOpenDir_ = ".";
	dataTable->setReadOnly(true);
	dataTable->setSorting(false);
	attList_.clear();
	attTable_.clear();
	fileNameLineEdit->clear();
	tableComboBox->clear();
	hasHeader_ = false;
	sep_ = ';';
	curCol_ = -1;
	colNameLineEdit->clear();
	textSizeLineEdit->clear();
	colDefApplyPushButton->setEnabled(false);
	dataTable->setNumRows(5);
	connect( dataTable->horizontalHeader(),SIGNAL( clicked(int) ),  this, SLOT(horizontalHeader_clicked(int)));
	dateTimeGroup->setEnabled(false);
	textTypeGroup->setEnabled(false);
	layerComboBox->clear();
	fileNameLineEdit->setEnabled(false);
	temporalparams->setEnabled(false);
}

void ImportTbl::filePushButton_clicked()
{
	attList_.clear();
	attTable_.clear();
	fileNameLineEdit->clear();
	tableComboBox->clear();

	QString extAllowed;
	#ifdef WIN32
		extAllowed = tr("DBF files (*.dbf);;ASCII Files(*.csv *.CSV *.txt *.TXT);;SPRING Files(*.spr);;MDB Files(*.mdb)");
	#else
		extAllowed = tr("DBF files (*.dbf *.DBF);;ASCII Files(*.csv *.CSV *.txt *.TXT);;SPRING Files(*.spr *.SPR)");
	#endif

	QString f = QFileDialog::getOpenFileName(lastOpenDir_, extAllowed, this, 0, tr("Open Table Source"));
	if (!f.isEmpty())
		fileNameLineEdit->setText(f);
	else
		return;
	lastOpenDir_ = TeGetPath(f.latin1()).c_str();

	string filename = string(f.latin1()).c_str();
	string ext = TeGetExtension(filename.c_str());
	tableNameLineEdit->clear();
	tableComboBox->setEnabled(false);
	tableTextLabel->setEnabled(false);

	if (TeStringCompare(ext,"spr"))
	{
		showTabTable(f);
	}
	else if (TeStringCompare(ext,"dbf"))
	{
		showDBFTable(f);
	}
#ifdef WIN32
#ifndef __GNUC__
	else if (ext=="mdb" || ext=="MDB")
	{
		TeDatabaseFactoryParams params;
		params.dbms_name_ = "Ado";
		TeDatabase* mdb_ = TeDatabaseFactory::make(params);

		if (!mdb_ || !mdb_->connect("","","",filename, 0))
		{
			QMessageBox::critical(this, tr("Error"),
				tr("Fail to connect to the database!"));
			if (mdb_)
				delete mdb_;
			return;
		}
		vector<string> tList;
		if (!mdb_->listTables(tList) || tList.empty())
		{
			delete mdb_;
			db_ = 0;
			QMessageBox::warning(this, tr("Warning"),
				tr("There are no tables in the database!"));
			fileNameLineEdit->clear();
			return;
		}
		else
		{
			tableComboBox->clear();
			TeDatabasePortal* portal = mdb_->getPortal();
			for (unsigned int i=0; i<tList.size(); ++i)
			{
				string s = tList[i].substr(0,4);
				if ( s == "MSys" )
					continue;
				string sql = "SELECT COUNT(*) FROM " + tList[i];
				if (portal->query(sql) && portal->fetchRow())
				{
					tableComboBox->insertItem(QString(tList[i].c_str()));
				}
				portal->freeResult();
			}
			delete portal;
			if (tableComboBox->count() > 0)
			{
				tableComboBox->setEnabled(true);
				tableTextLabel->setEnabled(true);
				tableComboBox->setCurrentItem(0);
				showMDBTable(tableComboBox->currentText());
			}
			else
			{
				QMessageBox::warning(this, tr("Warning"),
					tr("There are no accessible tables in the database!"));
				fileNameLineEdit->clear();
				return;
			}
		}
	}
#endif
#endif
	else if (TeStringCompare(ext, "csv") || TeStringCompare(ext, "txt"))
	{
		showCSVTable(f);
	}
	else
	{
		return;
	}
	displaySampleTable();
	fillFieldCombos();
	horizontalHeader_clicked(0);
}

void ImportTbl::horizontalHeader_clicked( int col )
{
	if (attList_.empty())
		return;

	curCol_ = col;
	colNameLineEdit->setText(attList_[curCol_].rep_.name_.c_str());
	dateTimeGroup->setEnabled(false);
	textTypeGroup->setEnabled(false);
	if (attList_[curCol_].rep_.type_ == TeSTRING)
	{
		textTypeGroup->setEnabled(true);
		textRadioButton->setChecked(true);
		textSizeLineEdit->setText(Te2String(attList_[curCol_].rep_.numChar_).c_str());
	}
	else if (attList_[curCol_].rep_.type_ == TeINT)
	{
		intRadioButton->setChecked(true);
	}
	else if (attList_[curCol_].rep_.type_ == TeREAL)
	{
		realRadioButton->setChecked(true);
	}
	else
	{
		dateTimeGroup->setEnabled(true);
		dateFormatComboBox->insertItem(attList_[curCol_].dateTimeFormat_.c_str());
		dateFormatComboBox->setCurrentText(attList_[curCol_].dateTimeFormat_.c_str());
		timeRadioButton->setChecked(true);
	}
	colNameLineEdit->setFocus();
}


void ImportTbl::colTypeButtonGroup_clicked(int n)
{
 	if (attList_.empty())
		return;
	
	if (curCol_ >= 0 && curCol_ < (int)attList_.size())
	{
		if (n==0)	//text type
		{
			dateTimeGroup->setEnabled(false);
			textTypeGroup->setEnabled(true);
			textSizeLineEdit->setText(Te2String(attList_[curCol_].rep_.numChar_).c_str());
		}
		else if (n==2) //date/time type
		{
			dateTimeGroup->setEnabled(true);
			textTypeGroup->setEnabled(false);
		}
		else
		{
			dateTimeGroup->setEnabled(false);
			textTypeGroup->setEnabled(false);
		}
	}
}

void ImportTbl::colDefApplyPushButton_clicked()
{
 	if (attList_.empty())
		return;
	
	if (curCol_ >= 0 && curCol_ < (int)attList_.size())
    {
		string name = colNameLineEdit->text().latin1();
		bool changed;
		string invalidChar;
		string newName = TeCheckName(name, changed, invalidChar);
		if (changed)
		{
			QString mess = tr("Field name contain one or more invalid characters: \"");
			mess += invalidChar.c_str();
			mess += "\".\n";
			mess += tr("Change current name and try again.");
			QMessageBox::warning(this, tr("Warning"), mess);
			name = newName;
			colNameLineEdit->setText(name.c_str());
			return;
		}
		if (!name.empty())
		{
			attList_[curCol_].rep_.name_ = name;
			QHeader *header = dataTable->horizontalHeader();
			header->setLabel( curCol_, QObject::tr(attList_[curCol_].rep_.name_.c_str()));
		}
		if (intRadioButton->isChecked())
			attList_[curCol_].rep_.type_ = TeINT;
		else if (realRadioButton->isChecked())
			attList_[curCol_].rep_.type_ = TeREAL;
		else if (timeRadioButton->isChecked())
		{
			string dateMask, timeMask, indMask;
			if(dateFormatComboBox->currentItem()>0)
				dateMask = dateFormatComboBox->currentText().latin1();
			if(timeFormatComboBox->currentItem()>0)
				timeMask = timeFormatComboBox->currentText().latin1();
			if(PMComboBox->currentItem()>0)
				indMask = PMComboBox->currentText().latin1();

			string maskText = ""; 	
			if(!dateMask.empty())
			{
				if (dateSepLineEdit->text().isEmpty())
				{// DsMsY DsYsM MsDsY MsYsD YsMsD YsDsM
					if (dateMask=="DsMsY") 
						dateMask="DDMMYYYY";
					else if (dateMask=="DsYsM")
						dateMask="DDYYYYMM";
					else if (dateMask=="MsDsY")
						dateMask="MMDDYYYY";
					else if (dateMask=="MsDsY")
						dateMask="MMDDYYYY";
					else if (dateMask=="YsMsD")
						dateMask="YYYYMMDD";
					else if (dateMask=="YsDsM")
						dateMask="YYYYDDMM";
				}
			    maskText += dateMask; 
			}

			if(!dateMask.empty())
				maskText += dateMask; 

			if(!timeMask.empty())
			{
				if(maskText.empty())
				{
					QMessageBox::warning(this, tr("Warning"),
					tr("It is not possible to define a Date/Time field composed by the time information only."));
					attList_[curCol_].rep_.type_ = TeSTRING;
					attList_[curCol_].rep_.numChar_ = atoi(textSizeLineEdit->text().latin1());
					textRadioButton->setChecked(true);
					dateTimeGroup->setEnabled(false);
					timeFormatComboBox->setCurrentItem(0);
					dateSepLineEdit->setText("/");
					timeSepLineEdit->setText(":");
					dateFormatComboBox->setCurrentItem(0);
					fillFieldCombos();
					return;
				}
				maskText += "s";
				maskText +=timeMask;

				if(!indMask.empty())
					maskText += "sTT";
				attList_[curCol_].dateChronon_ = TeSECOND;
			}
			else
			{
				if (dateMask == "Y")
					attList_[curCol_].dateChronon_ = TeYEAR;
				else if (dateMask == "M")
					attList_[curCol_].dateChronon_ = TeMONTH;
				else
					attList_[curCol_].dateChronon_ = TeDAY;
			}

			attList_[curCol_].rep_.type_ = TeDATETIME;
			attList_[curCol_].dateTimeFormat_ = maskText;
			attList_[curCol_].indicatorAM_ = "AM";
			attList_[curCol_].indicatorPM_ = "PM";
			attList_[curCol_].dateSeparator_ = dateSepLineEdit->text().latin1();
			attList_[curCol_].timeSeparator_ = timeSepLineEdit->text().latin1();
		}
		else
		{
			attList_[curCol_].rep_.type_ = TeSTRING;
			attList_[curCol_].rep_.numChar_ = atoi(textSizeLineEdit->text().latin1());
		}
		fillFieldCombos();
    }
}


void ImportTbl::okPushButton_clicked()
{
	if (!db_)
	{
		QMessageBox::critical(this, tr("Error"), tr("There is no active database!"));
		reject();
	}

	if (attList_.empty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select a table to import!"));
		return;
	}

	if (fileNameLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select a file!"));
		return;
	}

	// check the table name validity 
	string tableName = tableNameLineEdit->text().latin1();
	string errorMessage;
	QString mess;
	bool changed;
	string newName = TeCheckName(tableName, changed, errorMessage); 
	if(changed)
	{
		mess = tr("The table name is invalid: ") + errorMessage.c_str();
		mess += "\n" + tr("Change current name and try again.");
		QMessageBox::warning(this, tr("Warning"), mess);
		tableNameLineEdit->setText(newName.c_str());
		return;
	}
	tableName = newName;
	if(db_->tableExist(tableName) == true)
	{
		QMessageBox::warning(this, tr("Warning"), tr("A table with this name already exists in the database! Please change it."));
		return;
	}

	// check the validity of column names
	vector<unsigned int> coln;
	vector<string> invChars;
	for (unsigned int i=0; i<attList_.size();++i)
	{
		bool changed;
		string invalidChar;
		string newName = TeCheckName(attList_[i].rep_.name_, changed, invalidChar);
		if (changed)
		{
			coln.push_back(i);
			invChars.push_back(invalidChar);
		}
	}
	if (!coln.empty())
	{
		QString mess = tr("One or more fields have invalid character(s) in its names: ");
		mess += "\n";
		string errorMess;
		for (unsigned int i=0; i<coln.size();++i)
			errorMess += tr("Column ").latin1() + Te2String(coln[i]) + ": " + invChars[i] + "\n";
		mess += errorMess.c_str();
		mess += "\n";
		mess += "Change field names and try again.";

		coln.clear();
		invChars.clear();
		QMessageBox::warning(this, tr("Warning"),mess);
		return;
	}

	TeLayer* layer = 0;
	int lid = 0;
	if (tableTypeComboBox->currentItem()==0)		// external tables
	{
		if (attrComboBox->currentItem() == 0)
		{
			TeAttribute at;
			at.rep_.type_ = TeSTRING;
			at.rep_.numChar_ = 16;
			at.rep_.isPrimaryKey_ = true;
			at.rep_.name_ = "unique_id_";
			attList_.push_back(at);
			attTable_.setLinkName("unique_id_");
			attTable_.setUniqueName("unique_id_");
		}
		else
		{
			attTable_.setLinkName(attrComboBox->currentText().latin1());
			attTable_.setUniqueName(attrComboBox->currentText().latin1());
		}
		attTable_.setTableType(TeAttrExternal);
	}
	else if (tableTypeComboBox->currentItem()==1)	// static tables
	{
		if (layerComboBox->currentText().isEmpty()) 
		{
			QMessageBox::warning(this, tr("Error"), tr("Static tables should be associated to an existing layer."));
			return;
		}
		if (attrComboBox->currentItem() == 0)
		{
			TeAttribute at;
			at.rep_.type_ = TeSTRING;
			at.rep_.numChar_ = 16;
			at.rep_.isPrimaryKey_ = true;
			at.rep_.name_ = "object_id_";
			attList_.push_back(at);
			attTable_.setLinkName("object_id_");
			attTable_.setUniqueName("object_id_");
		}
		else
		{
			attTable_.setLinkName(attrComboBox->currentText().latin1());
			attTable_.setUniqueName(attrComboBox->currentText().latin1());
		}
		attTable_.setTableType(TeAttrStatic);
		TeLayerMap::iterator itlay = db_->layerMap().begin();
		TeLayerMap::iterator itlayend = db_->layerMap().end();
		while (itlay != itlayend)
		{
			if (layerComboBox->currentText().latin1() == (*itlay).second->name())
			{
				lid = (*itlay).second->id();
				layer = (*itlay).second;
				break;
			}
			++itlay;
		}
	}
	else if (tableTypeComboBox->currentItem()==2)	// temporal tables
	{
		if (layerComboBox->currentText().isEmpty()) 
		{
			QMessageBox::warning(this, tr("Error"), tr("Temporal tables should be associated to an existing layer."));
			return;
		}
		// temporal tables should have 
		if (initTimeComboBox->currentText().isEmpty() ||
			finalTimeComboBox->currentText().isEmpty())
		{
			QMessageBox::warning(this, tr("Error"), tr("A temporal table should have date/time fields that describe the initial and final time\n associated to the instance of attributes."));
			return;
		}
		if (attrComboBox->currentText().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Temporal tables should have an attribute to link with the geometries of the layer."));
			return;
		}
		else
		{
			if (attrComboBox->currentText() == uniqueIdComboBox->currentText())
			{
				int response = QMessageBox::question(this, tr("Question"),
				 tr("The fields that represent the link to geometries and the unique identifier of each instance of attributes are the same. Is this correct?"), tr("Yes"), tr("No"));
				if (response == 1)
					return;
			}
			else
				attTable_.setLinkName(attrComboBox->currentText().latin1());
		}
		if (uniqueIdComboBox->currentItem() == 0)
		{
			TeAttribute at;
			at.rep_.type_ = TeSTRING;
			at.rep_.numChar_ = 16;
			at.rep_.isPrimaryKey_ = true;
			at.rep_.name_ = "unique_id_";
			attList_.push_back(at);
			attTable_.setUniqueName("unique_id_");
		}
		else
		{
			attTable_.setUniqueName(attrComboBox->currentText().latin1());
		}
		attTable_.setTableType(TeFixedGeomDynAttr);
		attTable_.attFinalTime(finalTimeComboBox->currentText().latin1());
		attTable_.attInitialTime(initTimeComboBox->currentText().latin1());
		TeLayerMap::iterator itlay = db_->layerMap().begin();
		TeLayerMap::iterator itlayend = db_->layerMap().end();
		while (itlay != itlayend)
		{
			if (layerComboBox->currentText().latin1() == (*itlay).second->name())
			{
				lid = (*itlay).second->id();
				layer = (*itlay).second;
				break;
			}
			++itlay;
		}
	}
	else			// geocoding tables
	{
		if (attrComboBox->currentItem() == 0)
		{
			TeAttribute at;
			at.rep_.type_ = TeSTRING;
			at.rep_.numChar_ = 16;
			at.rep_.isPrimaryKey_ = true;
			at.rep_.name_ = "object_id_";
			attList_.push_back(at);
			attTable_.setUniqueName("object_id_");
			attTable_.setLinkName("object_id_");
		}
		else
		{
			attTable_.setLinkName(attrComboBox->currentText().latin1());
			attTable_.setUniqueName(attrComboBox->currentText().latin1());
		}
		attTable_.setTableType(TeGeocodingData);
	}
	TeAttributeList::iterator ita = attList_.begin();
	TeAttributeList::iterator itaend = attList_.end();
	string filename = fileNameLineEdit->text().latin1();
	string ext = TeGetExtension(filename.c_str());

	while (ita != itaend)
	{
		if ((*ita).rep_.name_ == attrComboBox->currentText().latin1())
		{
			(*ita).rep_.isPrimaryKey_ = true;

			if (!(ext=="csv" || ext=="CSV" || ext=="txt" || ext == "TXT")) // not for csv/txt files
			{
				if ((*ita).rep_.type_ != TeSTRING)
				{
					(*ita).rep_.type_ = TeSTRING;
					(*ita).rep_.numChar_ = 255;
				}
			}
			break;
		}
		++ita;
	}

	attTable_.setAttributeList(attList_);  
	attTable_.name(tableName);
	db_->validTable(attTable_);
	attList_ = attTable_.attributeList();
		
	bool res = false;
	bool insertMetadata = true;
	QString messres;
	setClientEncoding();
#ifdef WIN32
	if (ext=="mdb" || ext=="MDB")
	{
		if(!createAttributeTable())
		{
			restoreClientEncoding();
			return;
		}
		res = importMDBTable(messres);
	}
	else if (ext=="csv" || ext=="CSV" || ext=="txt" || ext == "TXT")
#else
	if (ext=="csv" || ext=="CSV" || ext=="txt" || ext == "TXT")
#endif
	{
		res = importCSVTable(messres, layer);
		insertMetadata = false;
	}
	else if (ext=="dbf" || ext=="DBF")
	{
		if(!createAttributeTable())
		{
			restoreClientEncoding();
			return;
		}
		res = importDBFTable(messres);
	}
	else if (TeStringCompare(ext,"tab") || 
			 TeStringCompare(ext,"spr"))
	{
		if(!createAttributeTable())
		{
			restoreClientEncoding();
			return;
		}
		res = importTabTable(messres);
	}
	else
	{
		restoreClientEncoding();
		return;	
	}

	if (!res)
	{
		QMessageBox::critical(this, tr("Error"), messres);
		restoreClientEncoding();
		return;
	}
	
	if (insertMetadata)
	{
		if (tableTypeComboBox->currentItem() == 1)
		{
			db_->insertTableInfo(lid,attTable_);
			if (layer)
				layer->loadLayerTables();
		}
		else
		{
			db_->insertTableInfo(0,attTable_);
		}
	}
	attList_.clear();
	attTable_.clear();
	restoreClientEncoding();
	accept();
}

void ImportTbl::showCSVTable( QString& f )
{
	intRadioButton->setDisabled(false);
	realRadioButton->setDisabled(false);
	timeRadioButton->setDisabled(false);
	textRadioButton->setDisabled(false);
	textSizeLineEdit->setDisabled(false);


	TeAsciiFile csvFile(f.latin1());

	// try to guess the separator
	string aux1 = csvFile.readLine();
	int sep = 4;
	int pos = aux1.find(',');
	if (pos != -1)
		sep = 0;
	else
	{
		pos = aux1.find(';');
		if (pos != -1)
			sep = 1;
		else
		{
			pos = aux1.find(' ');
			if (pos != -1)
				sep = 2;
			else
			{
				pos = aux1.find('\t');
				if (pos != -1)
					sep = 3;
			}
		}
	}

	bool ok;
	// find fields separator
	QStringList lst;
	lst << tr("Comma") << tr("Semicolon") << tr("Space") << tr("Tabulation") << tr("Other Character");
	QString text = QInputDialog::getItem(tr("Import Table"),  tr("Enter with the separator character:"), lst, sep, true, &ok, this );
	if ( ok && !text.isEmpty() ) 
	{
		std::string selectText=text.latin1();
		
		if (selectText.compare(tr("Comma").latin1()) == 0)
			sep_= ',';
		else if (selectText.compare(tr("Semicolon").latin1()) == 0)
			sep_ = ';';
		else if (selectText.compare(tr("Space").latin1()) == 0)
  			sep_ = ' ';
		else  if (selectText.compare(tr("Tabulation").latin1()) == 0)
  			sep_ = '\t';
		else
		{
			QString text = QInputDialog::getText( tr("Import Table"), 
													tr("Enter with the separator character:"), 
														QLineEdit::Normal,"", &ok, this );

				if ( ok && !text.isEmpty() ) 
					sep_ = (char)text.at(0).latin1();
				else 
				{
					QMessageBox::warning(this, tr("Warning"),
						tr("Please select a separator character!"));
					return;
				}
		}
	} 
	else 
	{
		fileNameLineEdit->clear();
		QMessageBox::warning(this, tr("Warning"),
			tr("Please select a separator character!"));
		return;
	}

	// check if the first row is a header
	int response = QMessageBox::question(this, tr("Question"),
         tr("Is the first line a header?"),
		 tr("Yes"), tr("No"));

	if (response == 0)
		hasHeader_ = true;
	else
		hasHeader_ = false;

	curCol_ = -1;
	unsigned int i;
	try
	{
		attList_.clear();

		// find the attribute list
		vector<string> row;
		TeTrim(aux1);
		string separator(1,sep_);
		TeSplitString(aux1,separator,row);
		for (i=0; i<row.size(); ++i)
		{
			string colName;
			if (hasHeader_ == true)  //first line is a header
				colName = row[i];
			else
				colName  = "Col" + Te2String(i);
			TeAttribute at;
			at.rep_.name_= colName;
			at.rep_.type_ = TeSTRING;
			at.rep_.numChar_ = 100;
			attList_.push_back(at);
		}
		attTable_.clear();
		attTable_.setAttributeList(attList_);

		int aux = 0;	
		if (hasHeader_ == false) // first row is not header
		{
			attTable_.add(row);
			aux=1;
		}
		for (i=aux; i<5 && csvFile.isNotAtEOF(); ++i)	// reads the first 5 rows
		{
			TeTableRow trow;
			csvFile.readStringListCSV(trow,sep_);
			if (!trow.empty())
				attTable_.add(trow);
			csvFile.findNewLine();
		}
	}
	catch(...)
	{
		QMessageBox::critical( this, tr("Error"),tr("The file could not be read!"));
		return;
	}
	string tName = TeGetBaseName(f.latin1());
	tableNameLineEdit->setText(tName.c_str());
	colDefApplyPushButton->setEnabled(true);
	QMessageBox::warning( this, tr("Warning"),tr("Check the name and the type of the columns!"));
}


void ImportTbl::showDBFTable( QString& f )
{
	string filename = f.latin1();
	colDefApplyPushButton->setEnabled(true);


	intRadioButton->setDisabled(true);
	realRadioButton->setDisabled(true);
	timeRadioButton->setDisabled(true);
	textRadioButton->setDisabled(true);
	textSizeLineEdit->setDisabled(true);

	try
	{
		attList_.clear();
		TeReadDBFAttributeList(filename,attList_);
		attTable_.clear();
		attTable_.setAttributeList(attList_);
		if (!TeReadDBFAttributes(filename, attTable_,5,0))
			return;
	}
	catch(...)
	{
		QMessageBox::critical( this, tr("Error"),
			tr("Fail to read the DBF table!"));
		return;
	}
	string tName = TeGetBaseName(filename.c_str());
	tableNameLineEdit->setText(tName.c_str());
}


void ImportTbl::tableTypeComboBox_activated( int i )
{
	if (i==2)
		temporalparams->setEnabled(true);
	else
		temporalparams->setEnabled(false);

	if (i==1 || i==2)
	{
		if (layerComboBox->count() > 0)
		{
			layerTextLabel->setEnabled(true);
			layerComboBox->setEnabled(true);
			attrTextLabel->setText(tr("Geometry link:"));
		}
		else
		{
			QMessageBox::warning( this, tr("Warning"),tr("The database has no layers!"));
			tableTypeComboBox->setCurrentItem(0);
		}
	}
	else
	{
		layerTextLabel->setEnabled(false);
		layerComboBox->setEnabled(false);
 		attrTextLabel->setText(tr("Primary key:"));
   }
   fillFieldCombos();
}


void ImportTbl::tableComboBox_activated( int )
{
#ifdef WIN32
#ifndef __GNUC__
	if (tableComboBox->currentText().isEmpty())
		return;
	showMDBTable(tableComboBox->currentText());
	displaySampleTable();
	fillFieldCombos();
	horizontalHeader_clicked(0);
#endif
#endif
}


void ImportTbl::showMDBTable( QString& f )
{
	intRadioButton->setDisabled(false);
	realRadioButton->setDisabled(false);
	timeRadioButton->setDisabled(false);
	textRadioButton->setDisabled(false);
	textSizeLineEdit->setDisabled(false);

#ifdef WIN32
#ifndef __GNUC__
	if (tableComboBox->currentText().isEmpty())
		return;
	colDefApplyPushButton->setEnabled(false);
	
	TeDatabaseFactoryParams params;
	params.dbms_name_ = "Ado";
	TeDatabase* mdb_ = TeDatabaseFactory::make(params);

	string filename = fileNameLineEdit->text().latin1();
	if (!mdb_ || !mdb_->connect("","","",filename, 0))
	{
		QMessageBox::critical(this, tr("Error"),
			tr("Fail to connect to the database!"));
		if (mdb_)
			delete mdb_;
		return;
	}
	attList_.clear();
	attTable_.clear();
	string tName = f.latin1();
	TeDatabasePortal* portal = mdb_->getPortal();
	string sql = "SELECT * FROM " + tName;
	if (!portal->query(sql))
	{
		delete portal;
		delete mdb_;
		QMessageBox::information(this, tr("Information"),tr("The table could not be read!"));
		return;
	}
	if (!portal->fetchRow())
	{
		delete portal;
		delete mdb_;
		QMessageBox::information(this, tr("Information"),tr("The table contains no data!"));
	}
	attList_ = portal->getAttributeList();
	TeAttributeList::iterator it = attList_.begin();
	while (it != attList_.end())
	{
		if ((*it).rep_.type_ == TeBLOB)
		{
			(*it).rep_.type_ = TeSTRING;
			(*it).rep_.numChar_ = 255;
			QString mess = tr("The field \'") + (*it).rep_.name_.c_str() + tr("\' is blob!\nIt will not be read.");
			QMessageBox::information(this, tr("Information"),mess);
		}
		else if ((*it).rep_.type_ == TeUNKNOWN)
		{
			(*it).rep_.type_ = TeSTRING;
			(*it).rep_.numChar_ = 255;
			QString mess = tr("The field \'") + (*it).rep_.name_.c_str() + tr("\' has an unknown type!\nIt will not be read.");
			QMessageBox::information(this, tr("Information"),mess);
		}
		++it;
	}
	attTable_.setAttributeList(attList_);
	int n = 0;
	do
	{
		TeTableRow trow;
		for (int nf=0; nf<portal->numFields(); nf++)
			trow.push_back(portal->getData(nf));
		attTable_.add(trow);
		trow.clear();
		n++;
	}while (portal->fetchRow() && n < 5);
	portal->freeResult();
	delete portal;
	delete mdb_;
	tableNameLineEdit->setText(tName.c_str());
#else
	f = "";  // dummy statement to avoid warning in Linux
#endif
#endif
}


void ImportTbl::displaySampleTable()
{
    if (attTable_.size() <= 0)
		return;
	// shows the columns header
	dataTable->setNumCols(attList_.size());
	int i;
	QHeader *header = dataTable->horizontalHeader();
	for (i=0; i<(int)attList_.size();i++)
		 header->setLabel( i, QObject::tr(attList_[i].rep_.name_.c_str()));

	// show 10 first records
	int nrec = (int)attTable_.size();
	for ( int row = 0; row < nrec; row++ ) 
	{
		TeTableRow trow = attTable_[row];
		int numAttrs = trow.size ();
        for ( int col = 0; col < numAttrs; col++ ) 
		{
			string celldata = trow[col];
            if (!celldata.empty())
				dataTable->setText(row,col,celldata.c_str());
        }
		trow.clear();
	}
}


bool ImportTbl::importMDBTable( QString & m )
{
#ifdef WIN32
#ifndef __GNUC__
	string filename = fileNameLineEdit->text().latin1();
	bool autoNum = (attrComboBox->currentItem() == 0);
	if (tableComboBox->currentText().isEmpty())
		return false;
	
	TeDatabaseFactoryParams params;
	params.dbms_name_ = "Ado";
	TeDatabase* mdb_ = TeDatabaseFactory::make(params);

	if (!mdb_ || !mdb_->connect("","","",filename, 0))
	{
		QMessageBox::critical(this, tr("Error"),
			tr("Fail to connect to the database!"));
		if (mdb_)
			delete mdb_;
		return false;
	}
	attTable_.clear();
	string tName = tableComboBox->currentText().latin1();
	TeDatabasePortal* portal = mdb_->getPortal();
	string sql = "SELECT * FROM " + tName;
	if (!portal->query(sql))
	{
		delete portal;
		delete mdb_;
		m = tr("The table could not be read!");
		return false;
	}
	if (!portal->fetchRow())
	{
		delete portal;
		delete mdb_;
		m = tr("The table contains no data!");
		return false;
	}
	int n = 1;
	do
	{
		TeTableRow trow;
		for (int nf=0; nf<portal->numFields(); nf++)
			trow.push_back(portal->getData(nf));
		if (autoNum)
			trow.push_back(Te2String(n));
		attTable_.add(trow);
		trow.clear();
		if ((n % 100) == 0)
		{
			if (!db_->insertTable(attTable_))
			{
				if (!db_->errorMessage().empty())
					m = db_->errorMessage().c_str();
				delete portal;
				delete mdb_;
				return false;
			}
			attTable_.clear();
		}
		n++;
	}while (portal->fetchRow());
	delete portal;
	delete mdb_;
	if (attTable_.size() > 0)
	{
		if (!db_->insertTable(attTable_))
		{
			if (!db_->errorMessage().empty())
				m = db_->errorMessage().c_str();
			delete portal;
			delete mdb_;
			return false;
		}
		attTable_.clear();
	}
	return true;
#else
	m = "";  // dummy statement to avoid warning in Linux
	return true;
#endif
#endif
}


bool ImportTbl::importCSVTable(QString & m, TeLayer* layer )
{
	string filename = fileNameLineEdit->text().latin1();
	bool autoNum = ((tableTypeComboBox->currentItem() == 2 && uniqueIdComboBox->currentItem()== 0) ||
		attrComboBox->currentItem() == 0);
	TeWaitCursor wait;
	
	if(!TeImportCSVFile(filename, attTable_, db_, layer, sep_, hasHeader_, 100, autoNum))
	{
		m = tr("Error importing ASCII file!");
		m += ":" + QString(db_->errorMessage().c_str());
		wait.resetWaitCursor();
		return false;
	}
	return true;
}


bool ImportTbl::importDBFTable( QString & m )
{
	string filename = fileNameLineEdit->text().latin1();
	bool autoNum = (attrComboBox->currentItem() == 0);
	int rinitial = 0;
	int nrows = 100;
	attTable_.clear();
	TeWaitCursor wait;
	while (TeReadDBFAttributes(filename, attTable_, nrows,rinitial,autoNum))
	{
		TeTableRow row = attTable_[0];
		if (!db_->insertTable(attTable_))
		{
			if (!db_->errorMessage().empty())
				m = db_->errorMessage().c_str();
			return false;
		}
		rinitial += attTable_.size();
		attTable_.clear();
	}
	return true;
}


void ImportTbl::showTabTable( QString& f)
{
	intRadioButton->setDisabled(false);
	realRadioButton->setDisabled(false);
	timeRadioButton->setDisabled(false);
	textRadioButton->setDisabled(false);
	textSizeLineEdit->setDisabled(false);

	try
	{
		TeAsciiFile sprFile(f.latin1());
		attList_.clear();
		
		string name = sprFile.readString();
		if (!TeStringCompare(name,"TABLE"))
		{
			QMessageBox::critical( this, tr("Error"),tr("The file is not in the correct ASCII SPRING format for tables!"));
			return;
		}

		// try to read the separator char
		while (sprFile.isNotAtEOF() && !TeStringCompare(name,"SEPARATOR"))
			name = sprFile.readString();

		if (!sprFile.isNotAtEOF())
		{
			QMessageBox::critical( this, tr("Error"),tr("The file is not in the correct ASCII SPRING format for tables!"));
			return;
		}
		sep_ = sprFile.readChar();

		while (sprFile.isNotAtEOF() && !TeStringCompare(name,"CATEGORY_OBJ") && !TeStringCompare(name,"CATEGORY_NOSPATIAL"))
			name = sprFile.readString();

		// try to read the list of attributes
		sprFile.findNewLine();
		name = sprFile.readStringCSVNoSpace (sep_);
		TeAttribute attribute;
		attList_.clear();
		while (sprFile.isNotAtEOF() && !TeStringCompare(name,"INFO_END"))
		{
			attribute.rep_.name_  = name;
			string attType = sprFile.readStringCSVNoSpace (sep_);
			if (TeStringCompare(attType,"INTEGER"))
			{
				attribute.rep_.type_ = TeINT;
			}
			else if (TeStringCompare(attType,"REAL"))
			{
				attribute.rep_.type_ = TeREAL;
			}
			else
			{
				attribute.rep_.type_  = TeSTRING;
				attribute.rep_.numChar_  = sprFile.readIntCSV(sep_);
			}
			attList_.push_back ( attribute );
			sprFile.findNewLine();
			name = sprFile.readStringCSVNoSpace (sep_);
		}
		if (!sprFile.isNotAtEOF())
		{
			QMessageBox::critical( this, tr("Error"),tr("The file is not in the correct ASCII SPRING format for tables!"));
			return;
		}
		sprFile.findNewLine();
		attTable_.clear();
		attTable_.setAttributeList(attList_);
		unsigned int nfields = attList_.size();
		for (unsigned int i=0; i<5; i++)	// reads the first 10 rows
		{
			TeTableRow trow;
			if (!sprFile.isNotAtEOF())
				break;		
			sprFile.readNStringCSV (trow, nfields, sep_);
			sprFile.findNewLine();
			attTable_.add(trow);
		}
	}
	catch(...)
	{
		QMessageBox::critical( this, tr("Error"),tr("The file could not be read!"));
		return;
	}
	string tName = TeGetBaseName(f.latin1());
	tableNameLineEdit->setText(tName.c_str());
	colDefApplyPushButton->setEnabled(false);
}


bool ImportTbl::importTabTable( QString & m)
{
	try
	{
		bool autoNum = (attrComboBox->currentItem() == 0);
		TeWaitCursor wait;
		string name = "";
		TeAsciiFile sprFile(fileNameLineEdit->text().latin1());
		while (sprFile.isNotAtEOF() && !TeStringCompare(name,"INFO_END"))
			name = sprFile.readString();
		sprFile.findNewLine();

		if (!sprFile.isNotAtEOF())
		{
			m = tr("The file is not in the correct ASCII SPRING format for tables!");
			return false;
		}
		
		attTable_.clear();
		int nrec = 0;
		int count = 0;
		unsigned int nfields = attList_.size();
		if (autoNum)
			--nfields;
		while (sprFile.isNotAtEOF())
		{
			TeTableRow trow;
			sprFile.readNStringCSV (trow, nfields, sep_);
			if (trow.empty() || TeStringCompare(trow[0],"END"))
				break;

			// if create a primary key
			if(autoNum)
				trow.push_back (Te2String(count));

			sprFile.findNewLine();
			attTable_.add(trow);
			trow.clear();
			++nrec;
			++count;

			// save a chunk of registers
			if ((nrec % 100) == 0)
			{
				if (!db_->insertTable(attTable_))
				{
					if (!db_->errorMessage().empty())
						m = db_->errorMessage().c_str();
					return false;
				}
				attTable_.clear();
				nrec = 0;
			}
		}
		if (attTable_.size() > 0)
		{
			if (!db_->insertTable(attTable_))
			{
				if (!db_->errorMessage().empty())
					m = db_->errorMessage().c_str();
				return false;
			}
			attTable_.clear();
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}


void ImportTbl::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("importTable.htm");
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


bool ImportTbl::createAttributeTable()
{
    if (!db_->createTable(attTable_.name(),attList_))	// create table
	{
		QString msg = tr("Fail to create the table") + " " + attTable_.name().c_str();
		if (!db_->errorMessage().empty())
		{
			msg += ".\n";
			msg += db_->errorMessage().c_str();
		}
		QMessageBox::critical(this, tr("Error"), msg);
		return false;
	}
	return true;
}


QString ImportTbl::getLastOpenDir()
{
	return lastOpenDir_;
}


void ImportTbl::setParams( TeDatabase * appDatabase, const QString & lastOpenDir )
{
	db_ = appDatabase;
	lastOpenDir_ = lastOpenDir;
	TeLayerMap& layerMap = db_->layerMap();
	TeLayerMap::iterator itlay = layerMap.begin();
	while ( itlay != layerMap.end() )
	{
		layerComboBox->insertItem((*itlay).second->name().c_str());
		++itlay;
	}
	fileNameLineEdit->setText(lastOpenDir_ + "/...");
	loadInitialPostgresEncoding();
}


void ImportTbl::fillFieldCombos()
{
	if (attList_.empty())
		return;

	unsigned int na = attList_.size();
	unsigned int nn;

	string curField = attrComboBox->currentText().latin1();
	unsigned int curItem = 0;
	unsigned int curItemVal = 0;
	attrComboBox->clear();

	if (tableTypeComboBox->currentItem() != 2)
	{
		attrComboBox->insertItem(tr("--- create ---"));
		curItemVal = 1;
	}
	for (nn=0;nn<na;++nn)
	{
		if (attList_[nn].rep_.type_ == TeSTRING || attList_[nn].rep_.type_ == TeINT || 
			attList_[nn].rep_.type_ == TeUNSIGNEDINT || attList_[nn].rep_.type_ == TeREAL)
		{
			attrComboBox->insertItem(attList_[nn].rep_.name_.c_str());
			if (attList_[nn].rep_.isPrimaryKey_ || attList_[nn].rep_.name_ == curField)
				curItem = curItemVal;
			++curItemVal;
		}
	}
	attrComboBox->setCurrentItem(curItem);

	curItem = 0;
	curItemVal = 0;
	curField = uniqueIdComboBox->currentText().latin1();
	uniqueIdComboBox->clear();
	uniqueIdComboBox->insertItem(tr("--- create ---"));
	for (nn=0;nn<na;++nn)
	{
		if (attList_[nn].rep_.type_ == TeSTRING || attList_[nn].rep_.type_ == TeINT || 
			attList_[nn].rep_.type_ == TeUNSIGNEDINT || attList_[nn].rep_.type_ == TeREAL)
		{
			++curItemVal;
			uniqueIdComboBox->insertItem(attList_[nn].rep_.name_.c_str());
			if (attList_[nn].rep_.isPrimaryKey_ || attList_[nn].rep_.name_ == curField)
				curItem = curItemVal;
		}
	}
	uniqueIdComboBox->setCurrentItem(curItem);

	curItem = 0;
	curItemVal = 0;
	if (initTimeComboBox->count()>0)
		curField = initTimeComboBox->currentText().latin1();
	initTimeComboBox->clear();
	for (nn=0;nn<na;++nn)
	{
		if (attList_[nn].rep_.type_ == TeDATETIME)
		{
			initTimeComboBox->insertItem(attList_[nn].rep_.name_.c_str());
			if (attList_[nn].rep_.name_ == curField)
				curItem = curItemVal;
			++curItemVal;
		}
	}
	initTimeComboBox->setCurrentItem(curItem);

	curItem = 0;
	curItemVal = 0;
	if (finalTimeComboBox->count()>0)
		curField = finalTimeComboBox->currentText().latin1();
	finalTimeComboBox->clear();
	for (nn=0;nn<na;++nn)
	{
		if (attList_[nn].rep_.type_ == TeDATETIME)
		{
			finalTimeComboBox->insertItem(attList_[nn].rep_.name_.c_str());
			if (attList_[nn].rep_.name_ == curField)
				curItem = curItemVal;
			++curItemVal;
		}
	}
	finalTimeComboBox->setCurrentItem(curItem);
}


void ImportTbl::loadInitialPostgresEncoding()
{
	std::vector<std::string> encodingList;
	std::vector<std::string>::iterator it;

	cmbEncoding->setEnabled(false);
	if(db_ == 0) return;
	if((db_->dbmsName() != "PostgreSQL") && (db_->dbmsName() != "PostGIS")) return;

	db_->getEncodingList(encodingList);

	cmbEncoding->clear();
	cmbEncoding->insertItem("");
	for(it=encodingList.begin();it!=encodingList.end();it++)
	{
		cmbEncoding->insertItem((*it).c_str());
	}
	if(!encodingList.empty()) cmbEncoding->setEnabled(true);
}


void ImportTbl::setClientEncoding()
{
	std::string		encoding;

	if(db_ == 0) return;
	if((db_->dbmsName() != "PostgreSQL") && (db_->dbmsName() != "PostGIS")) return; // only Postgres and Postgis

	if(!cmbEncoding->currentText().isEmpty()) encoding=cmbEncoding->currentText().latin1();
	_clientEncoding=db_->getClientEncoding();
	db_->setClientEncoding(encoding);
}


void ImportTbl::restoreClientEncoding()
{
	if(db_ == 0) return;
	if((db_->dbmsName() != "PostgreSQL") && (db_->dbmsName() != "PostGIS")) return; // only Postgres and Postgis
	db_->setClientEncoding(_clientEncoding);
}
