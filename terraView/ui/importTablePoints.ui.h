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

#include <projection.h>
#include <TeQtDatabasesListView.h>
#include <TeQtLayerItem.h>
#include <TeDriverSHPDBF.h>
#include <TeDriverCSV.h>

void ImportTablePoints::init()
{
	help_ = 0;
	curLayer_ = 0;
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	db_ = mainWindow_->currentDatabase();
	dataTable->setReadOnly(true);
	dataTable->setSorting(false);
	attList_.clear();
	hasHeader_ = false;
	sep_ = ';';
	curCol_ = -1;
	colNameLineEdit->clear();
	textSizeLineEdit->clear();
	dataTable->setNumRows(11);
	dataTable->setReadOnly(true);
	dataTable->setSorting(false);
	autolinkRadioButton->setChecked(true);
	linkComboBox->clear();
    linkComboBox->setEnabled(false);
	projection_ = 0;
	dateTimeGroup->setEnabled(false);
	textTypeGroup->setEnabled(false);
	colDefApplyPushButton->setEnabled(false);
	connect( dataTable->horizontalHeader(),SIGNAL( clicked(int) ),  this, SLOT(horizontalHeader_clicked(int) ));
	StaticRadioButton->setChecked(true);
	tiComboBox->setEnabled(false);
	tfComboBox->setEnabled(false);
	fileNameLineEdit->setText(mainWindow_->lastOpenDir_ + "/...");
	fileNameLineEdit->setEnabled(false);
}


void ImportTablePoints::filePushButton_clicked()
{
	QString f = QFileDialog::getOpenFileName(mainWindow_->lastOpenDir_, 
	  tr("DBF files (*.dbf *.DBF);;ASCII Files (*.csv *.CSV *.txt *.TXT)"),
	  this, 0, tr("Open point table"));
	if (!f.isEmpty())
		fileNameLineEdit->setText(f);
	else
		return;
	mainWindow_->lastOpenDir_ = TeGetPath(f.latin1()).c_str();
	string filename = fileNameLineEdit->text().latin1();
	string layerName = TeGetBaseName(string(f.latin1()).c_str());
	layerNameLineEdit->setText(layerName.c_str());

	attList_.clear();
	colDefApplyPushButton->setEnabled(false);
	colNameLineEdit->clear();
	textSizeLineEdit->clear();

	TeTable attTable(layerName+"Attr");
	attTable.setTableType(TeAttrStatic);

	string fileExt = TeConvertToUpperCase(TeGetExtension (filename.c_str()));
	if (fileExt == "TXT" || fileExt == "CSV" )
	{
		TeAsciiFile csvFile(filename);

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
		if ( ok ) 
		{
			if (text == "Comma" || text == "Vírgula")
				sep_= ',';
			else if (text == "Semicolon" || text == "Ponto e Vírgula")
				sep_ = ';';
			else if (text == "Space" || text == "Espaço")
  				sep_ = ' ';
			else  if (text == "Tabulation" || text == "Tabulação")
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

		// check if first row is contains the name of the columns
		int response = QMessageBox::question(this, tr("Question"),
             tr("Is the first line a header?"),
             tr("Yes"), tr("No"));

		if (response == 0)
			hasHeader_ = true;
		else
			hasHeader_ = false;

		curCol_ = -1;
		unsigned int i;

		vector<string> row;
		TeTrim(aux1);
		string separator(1,sep_);
		TeSplitString(aux1,separator,row);
		for (i=0; i<row.size(); ++i)
		{
			string colName;
			if (hasHeader_ == true)
				colName = row[i];
			else
				colName  = "Col" + Te2String(i);
			TeAttribute at;
			at.rep_.name_= colName;
			at.rep_.type_ = TeSTRING;
			at.rep_.numChar_ = 100;
			attList_.push_back(at);
		}
		attTable.setAttributeList(attList_);

		int aux = 0;	
		if ((hasHeader_ == false)) // first row in not header
		{
			attTable.add(row);
			aux=1;
		}
		for (i=aux; i<10 && csvFile.isNotAtEOF(); i++)	// reads the first 10 rows
		{
			TeTableRow trow;
			csvFile.readStringListCSV(trow,sep_);
			if (!trow.empty())
				attTable.add(trow);
			csvFile.findNewLine();
		}

		colDefApplyPushButton->setEnabled(true);
		QMessageBox::warning(this, tr("Warning"),
			tr("Check the name and the type of the columns!"));
	}
	else
	{
		colDefApplyPushButton->setEnabled(false);
		try
		{
			TeReadDBFAttributeList(filename,attList_);
			attTable.setAttributeList(attList_);

			if (!TeReadDBFAttributes(filename, attTable, 10,0))
				return;
		}
		catch(...)
		{
			QMessageBox::critical( this, tr("Error"),
				tr("Fail to read the DBF table!"));
			return;
		}
	}
	// shows the columns header
	dataTable->setNumCols(attList_.size());
	unsigned int i,j;
	QHeader *header = dataTable->horizontalHeader();
	for (i=0; i<attList_.size();i++)
		 header->setLabel( i, QObject::tr(attList_[i].rep_.name_.c_str()));

	// show 10 first records
	for (i = 0; i < attTable.size(); ++i ) 
	{
		if (!attTable[i].empty())
		{
			for (j = 0; j < attList_.size(); j++ ) 
			{
				if (!attTable[i][j].empty())
					dataTable->setText(i,j,attTable[i][j].c_str());
			}
		}
	}
	fillFieldCombos();
}

void ImportTablePoints::okPushButton_clicked()
{
	string filename = fileNameLineEdit->text().latin1();
	if (filename.empty())
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Select a file!"));
		return;
	}

	if (attList_.empty())
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Attibute list cannot be empty! Maybe you haven't selected a file to import"));
		return;
	}

	if (coordXComboBox->currentText().isEmpty() || coordYComboBox->currentText().isEmpty())
	{
		QMessageBox::critical(this, tr("Error"),
			tr("Please choose the fields that contain the coordinates X and Y of the points."));
		return;
	}
	if (coordXComboBox->currentText() == coordYComboBox->currentText())
	{
		int response = QMessageBox::question(this, tr("Question"),
             tr("The coordinates X and Y of the points are given by the same field!\n Do you want to change them?"),
             tr("Yes"), tr("No"));
		if (response == 0)
			return;
	}

	string coordx = coordXComboBox->currentText().latin1();
	string coordy = coordYComboBox->currentText().latin1();
	unsigned int indexx, indexy;
	unsigned int i;
	for (i=0; i<attList_.size();++i)
	{
		if (TeConvertToUpperCase(attList_[i].rep_.name_) == TeConvertToUpperCase(coordx))
			indexx = i;
		if (TeConvertToUpperCase(attList_[i].rep_.name_) == TeConvertToUpperCase(coordy))
			indexy = i;
	}

	vector<unsigned int> coln;
	vector<string> invChars;
	for (i=0; i<attList_.size();++i)
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
		for (i=0; i<coln.size();++i)
			errorMess += tr("Column ").latin1() + Te2String(coln[i]) + ": " + invChars[i] + "\n";
		mess += errorMess.c_str();
		mess += "\n";
		mess += "Change field names and try again.";

		coln.clear();
		invChars.clear();

		return;
	}


	// Creating a new layer
	// Check layer name validity
	string layerName = layerNameLineEdit->text().latin1();
	if(layerName.empty())
	{
		QMessageBox::warning(this,tr("Warning"),
		tr("Enter output layer name and try again!"));
		return;
	}
	string errorMessage;
	bool changed;
	string newName = TeCheckName(layerName, changed, errorMessage); 
	if(changed)
	{
		QString mess = tr("The output layer name is invalid: ") + errorMessage.c_str();
		mess += "\n" + tr("Change current name and try again.");
		QMessageBox::warning(this, tr("Warning"), mess);
		layerNameLineEdit->setText(newName.c_str());
		return;
	}
	layerName = newName;
	if (db_->layerExist(layerName))
	{
		QMessageBox::warning(this,tr("Warning"),
		tr("Output layer name already exist, replace and try again!"));
		return;
	}

//	bool status = true;
	TeBox box;
	if (!projection_)
		projection_ = new TeNoProjection();

	int nrows = 100;

	string colSelected;
	//creates a new layer
	curLayer_ = new TeLayer(layerName, db_, projection_);
	if (!curLayer_ || curLayer_->id() <= 0 )
	{
		QString msg = tr("Fail to create the new layer!") + "\n";
		msg += db_->errorMessage().c_str();
		QMessageBox::critical( this, tr("Error"), msg);
		return;
	}
	int layerId = curLayer_->id();
	int indexid=-1;
	bool autoNum = false;
	string tName = layerName;
	int n=1;
	while (db_->tableExist(tName))
	{
		tName = layerName + "_" + Te2String(n);
		n++;
	}

	TeTable table(tName);
	colSelected = linkComboBox->currentText().latin1();
	if (!colLinkRadioButton->isChecked())
	{
		TeAttribute at;
		at.rep_.type_ = TeSTRING;
		at.rep_.numChar_ = 16;
		at.rep_.name_ = "object_id_";
		at.rep_.isPrimaryKey_ = true;
		attList_.push_back(at);
		colSelected = "object_id_";
		autoNum = true;
		indexid = attList_.size()-1;
	}
	else
	{
		for (i=0; i<attList_.size();i++)
			if (attList_[i].rep_.name_ == colSelected)
			{
				attList_[i].rep_.isPrimaryKey_ = true;
				indexid = i;
				if (attList_[i].rep_.type_ != TeSTRING)
				{
					attList_[i].rep_.type_ = TeSTRING;
					attList_[i].rep_.numChar_ = 255;
				}
				break;
			}
	}
	table.setLinkName(colSelected);
	table.setUniqueName(colSelected);
	table.setAttributeList(attList_);

	if(EventRadioButton->isChecked())
	{
	    string ti = tiComboBox->currentText().latin1();
	    string tf = tfComboBox->currentText().latin1();
	    
	    if(ti.empty() || tf.empty())
	    {
			QMessageBox::warning(this, tr("Warning"),
				tr("Select the initial and final time!"));
			return;
	    }
	    else
	    {
			table.setTableType(TeAttrEvent);
			table.attInitialTime(ti);
			table.attFinalTime(tf);
	    }
	}
	else
	    table.setTableType(TeAttrStatic);

	db_->validTable(table);
	attList_ = table.attributeList();

	string fileExt = TeGetExtension (filename.c_str());

	//progress bar
	if (TeProgress::instance())
	{
		string caption = string(tr("Importing table of points").latin1());
		string mess = string(tr("Importing table of points. Please wait...").latin1());
		TeProgress::instance()->setCaption(caption);
		TeProgress::instance()->setMessage(mess);
	}
	TeWaitCursor wait;

	//csv
	if(TeConvertToUpperCase(fileExt) == "CSV" || TeConvertToUpperCase(fileExt) == "TXT")
	{
		if(!TeImportCSVFile(filename, indexx, indexy, table, db_, curLayer_, 
							sep_,  hasHeader_, 100,  autoNum))
		{
			QMessageBox::warning ( this, tr("Error"),
			tr("Error importing CSV file!"));
			db_->deleteLayer(layerId); //delete geometry table too...
			//delete o que for preciso
			wait.resetWaitCursor();
			return;
		}
	}
	else //.dbf
	{
		// try to create a table in the database
		if (!db_->createTable(table.name(), attList_))
		{
			QString msg = tr("Fail to create the table") + " \"";
			msg += table.name().c_str();
			msg + "\" !";
			if (!db_->errorMessage().empty())
			{
				msg += "\n";
				msg += db_->errorMessage().c_str();
			}
			QMessageBox::critical( this, tr("Error"), msg);
			db_->deleteLayer(layerId);
			wait.resetWaitCursor();
			return;
		}
	
		//Read DBF file
		int rinitial = 0;
		bool endD = false;
		TePointSet ps;
		while (!endD)
		{
			bool status = TeReadDBFAttributes(filename, table, nrows,rinitial,autoNum);
			endD = !status;
			//int nrec = 0;
			rinitial += table.size();

			if (status)
			{
				if (!db_->insertTable(table))
				{
					wait.resetWaitCursor();
					QString msg = tr("Fail to save the table") + " \"";
					msg += table.name().c_str();
					msg += "\" " + tr("in the database!");
					if (!db_->errorMessage().empty())
					{
						msg += "\n";
						msg += db_->errorMessage().c_str();
					}
					QMessageBox::critical(this, tr("Error"), msg);
					db_->deleteLayer(layerId);
					return;
				}
				for (i=0; i<table.size(); i++)
				{
					TePoint p;
					string x = table(i,indexx);
					string y = table(i,indexy);
					
					if(x.empty() || y.empty())
						continue;

					TeCoord2D xy(atof(x.c_str()),atof(y.c_str()));
					p.add(xy);
					p.objectId(table(i,indexid));
					ps.add(p);
				}
				if (!curLayer_->addPoints(ps))
				{
					wait.resetWaitCursor();
					QString msg = tr("Fail to save the points in the database!");
					if (!db_->errorMessage().empty())
					{
						msg += "\n";
						msg+= db_->errorMessage().c_str();
					}
					QMessageBox::critical(this, tr("Error"), msg);
					db_->deleteLayer(layerId);
					return;
				}
			}
			table.clear();
			ps.clear();
		}

		if (table.size() > 0)
		{
			if (!db_->insertTable(table))
			{
				wait.resetWaitCursor();
				QString msg = tr("Fail to save the table") + " \"";
				msg += table.name().c_str();
				msg += "\" " + tr("in the database!");
				if (!db_->errorMessage().empty())
				{
					msg += "\n";
					msg += db_->errorMessage().c_str();
				}
				QMessageBox::critical( this, tr("Error"), msg);
				db_->deleteLayer(layerId);
				return;
			}
			for (i=0; i<table.size(); i++)
			{
				TePoint p;
				string x = table(i,indexx);
				string y = table(i,indexy);
				TeCoord2D xy(atof(x.c_str()),atof(y.c_str()));
				p.add(xy);
				p.objectId(table(i,indexid));
				ps.add(p);
			}
			if (!curLayer_->addPoints(ps))
			{
				wait.resetWaitCursor();
				QString msg = tr("Fail to save the points in the database!");
				if (!db_->errorMessage().empty())
				{
					msg += "\n";
					msg+= db_->errorMessage().c_str();
				}
				QMessageBox::critical(this, tr("Error"), msg);
				db_->deleteLayer(layerId);
				return;
			}
		}
		table.clear();
		ps.clear();

		if(!curLayer_->database()->insertMetadata(curLayer_->tableName(TePOINTS),curLayer_->database()->getSpatialIdxColumn(TePOINTS), 0.0005,0.0005,curLayer_->box()))
		{
			wait.resetWaitCursor();
			QString msg = tr((db_->errorMessage()).c_str());
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}

		if(!curLayer_->database()->createSpatialIndex(curLayer_->tableName(TePOINTS),curLayer_->database()->getSpatialIdxColumn(TePOINTS), (TeSpatialIndexType)TeRTREE))
		{
			wait.resetWaitCursor();
			QString msg = tr((db_->errorMessage()).c_str());
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}
	
		if (!db_->insertTableInfo(layerId,table))
		{
			QMessageBox::critical( this, tr("Error"),
				tr("Fail to insert table information!"));
		}	
		curLayer_->addAttributeTable(table);
	} // end of DBF file

	wait.resetWaitCursor();

	//show layer in the layer list
	TeQtDatabasesListView *databasesListView = mainWindow_->getDatabasesListView();
	TeQtDatabaseItem *databaseItem = databasesListView->currentDatabaseItem();

	TeQtLayerItem *layerItem = new TeQtLayerItem((QListViewItem*)databaseItem, curLayer_->name().c_str(), curLayer_);
	layerItem->setEnabled(true);
	if (databasesListView->isOpen(layerItem->parent()) == false)
		databasesListView->setOpen(layerItem->parent(),true);
	mainWindow_->checkWidgetEnabling();
	accept();
	hide();
}

void ImportTablePoints::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("importTablePoints.htm");
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


void ImportTablePoints::colTypeButtonGroup_clicked(int n)
{
		//n==0: integer
		//n==1: real
		//n==2: string
		//n==3: date	
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

void ImportTablePoints::AttrTableTypeButtonGroup_clicked(int n)
{
   	if (n==0)
	{
		tiComboBox->setEnabled(false);
		tfComboBox->setEnabled(false);
	}
	else if (n==1)
	{
		tiComboBox->setEnabled(true);
		tfComboBox->setEnabled(true);
	}
}


void ImportTablePoints::horizontalHeader_clicked( int col )
{
	if (attList_.empty())
		return;
	
	curCol_ = col;
	colNameLineEdit->setText(attList_[curCol_].rep_.name_.c_str());
	dateTimeGroup->setEnabled(false);
	textTypeGroup->setEnabled(false);
	if (attList_[curCol_].rep_.type_ == TeSTRING)
	{
		textRadioButton->setChecked(true);
		textTypeGroup->setEnabled(true);
		textSizeLineEdit->setText(Te2String(attList_[curCol_].rep_.numChar_).c_str());
	}
	else if (attList_[curCol_].rep_.type_ == TeINT)
	{
		intRadioButton->setChecked(true);
		textSizeLineEdit->setText(Te2String(attList_[curCol_].rep_.numChar_).c_str());
	}
	else if (attList_[curCol_].rep_.type_ == TeREAL)
	{
		realRadioButton->setChecked(true);
	}
	else
	{
		timeRadioButton->setChecked(true);
		dateTimeGroup->setEnabled(true);
		dateFormatComboBox->insertItem(attList_[curCol_].dateTimeFormat_.c_str());
		dateFormatComboBox->setCurrentText(attList_[curCol_].dateTimeFormat_.c_str());
	}
	colNameLineEdit->setFocus();
}


void ImportTablePoints::autolinkRadioButton_clicked()
{
    linkComboBox->setEnabled(false);
}


void ImportTablePoints::colLinkRadioButton_clicked()
{
    linkComboBox->setEnabled(true);
}


void ImportTablePoints::projectionPushButton_clicked()
{
	ProjectionWindow *projectionWindow = new ProjectionWindow(this, "projection", true);
	if (projection_)
		projectionWindow->loadProjection(projection_);
	if (projectionWindow->exec() == QDialog::Accepted)
	{
		projection_ = projectionWindow->getProjection();
		string pname = projection_->name() + " / " + projection_->datum().name();
		projectionLineEdit->setText(pname.c_str());
	}
	delete projectionWindow;
}



TeLayer* ImportTablePoints::getLayer()
{
    return curLayer_;
}


void ImportTablePoints::fillFieldCombos()
{
	if (attList_.empty())
		return;

	unsigned int na = attList_.size();
	unsigned int nn;

	string curField;	
	if (linkComboBox->count()>0)
		curField = linkComboBox->currentText().latin1();

	unsigned int curItem = 0;
	unsigned int curItemVal = 0;
	linkComboBox->clear();
	for (nn=0;nn<na;++nn)
	{
		if (attList_[nn].rep_.type_ == TeSTRING || attList_[nn].rep_.type_ == TeINT || 
			attList_[nn].rep_.type_ == TeUNSIGNEDINT || attList_[nn].rep_.type_ == TeREAL )
		{
			linkComboBox->insertItem(attList_[nn].rep_.name_.c_str());
			if (attList_[nn].rep_.isPrimaryKey_ || attList_[nn].rep_.name_ == curField)
				curItem = curItemVal;
			++curItemVal;
		}
	}
	linkComboBox->setCurrentItem(curItem);

	curItem = 0;
	curItemVal = 0;
	if (tiComboBox->count()>0)
		curField = tiComboBox->currentText().latin1();
	tiComboBox->clear();
	for (nn=0;nn<na;++nn)
	{
		if (attList_[nn].rep_.type_ == TeDATETIME)
		{
			tiComboBox->insertItem(attList_[nn].rep_.name_.c_str());
			if (attList_[nn].rep_.name_ == curField)
				curItem = curItemVal;
			++curItemVal;
		}
	}
	tiComboBox->setCurrentItem(curItem);

	curItem = 0;
	curItemVal = 0;
	if (tfComboBox->count()>0)
		curField = tfComboBox->currentText().latin1();
	tfComboBox->clear();
	for (nn=0;nn<na;++nn)
	{
		if (attList_[nn].rep_.type_ == TeDATETIME)
		{
			tfComboBox->insertItem(attList_[nn].rep_.name_.c_str());
			if (attList_[nn].rep_.name_ == curField)
				curItem = curItemVal;
			++curItemVal;
		}
	}
	tfComboBox->setCurrentItem(curItem);

	curItem = 0;
	curItemVal = 0;
	if (coordYComboBox->count()>0)
		curField = coordYComboBox->currentText().latin1();
	coordYComboBox->clear();
	for (nn=0;nn<na;++nn)
	{
		if (attList_[nn].rep_.type_ == TeREAL || attList_[nn].rep_.type_ == TeINT || 
			attList_[nn].rep_.type_ == TeUNSIGNEDINT)
		{
			coordYComboBox->insertItem(attList_[nn].rep_.name_.c_str());
			if (attList_[nn].rep_.name_ == curField)
				curItem = curItemVal;
			++curItemVal;
		}
	}
	coordYComboBox->setCurrentItem(curItem);

	curItem = 0;
	curItemVal = 0;
	if (coordXComboBox->count()>0)
		curField = coordXComboBox->currentText().latin1();
	coordXComboBox->clear();
	for (nn=0;nn<na;++nn)
	{
		if (attList_[nn].rep_.type_ == TeREAL || attList_[nn].rep_.type_ == TeINT || 
			attList_[nn].rep_.type_ == TeUNSIGNEDINT)
		{
			coordXComboBox->insertItem(attList_[nn].rep_.name_.c_str());
			if (attList_[nn].rep_.name_ == curField)
				curItem = curItemVal;
			++curItemVal;
		}
	}
	coordXComboBox->setCurrentItem(curItem);
}


void ImportTablePoints::colDefApplyPushButton_clicked()
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
			mess += tr("Change field name and try again");
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
				maskText += timeMask;

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
			attList_[curCol_].rep_.numChar_ = textSizeLineEdit->text().toInt();
		}
    }
	fillFieldCombos();
}
