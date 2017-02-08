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

#include <terraViewBase.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <TeWaitCursor.h>
#include <TeDatabase.h>
#include <TeDatabaseUtils.h>
#include <qtextedit.h>
#include <qpopupmenu.h>
#include <qfiledialog.h>
#include <mediaDescription.h>

#include <TeApplicationUtils.h>

void TableProperties::init()
{
	edit_ = false;
	row_ = -1;
	col_ = -1;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();    
	db_ = mainWindow_->currentDatabase();
	connect(propTable,SIGNAL(pressed(int, int, int, const QPoint&)), this, SLOT(mousePressedSlot(int, int, int, const QPoint&)));	
	connect(propTable,SIGNAL(doubleClicked(int, int, int, const QPoint&)), this, SLOT(mouseDoubleClickedSlot(int, int, int, const QPoint&)));	
	connect(propTable,SIGNAL(valueChanged(int, int)), this, SLOT(valueChangedSlot(int, int)));	
	connect(propTable,SIGNAL(contextMenuRequested (int, int, const QPoint&)), this, SLOT(contextMenuSlot(int, int, const QPoint&)));	
}


void TableProperties::mousePressedSlot( int row, int col, int /* button */, const QPoint & )
{
	if(edit_)
	{
		if(row == row_ && col == col_)
			return;
		propTable->endEdit(row_, col_, true, true);
		row_ = -1;
		col_ = -1;
	}
	else
	{
		if(row == 2 || row == 4)
		{
			row_ = -1;
			col_ = -1;
		}
		else if(row <= 7 && col == 1)
		{
			row_ = row;
			col_ = col;
		}
	}
	edit_ = false;
}


void TableProperties::mouseDoubleClickedSlot( int row, int col, int /* button */, const QPoint & )
{
	if(col == 0)
		return;

	if(edit_)
	{
		propTable->endEdit(row_, col_, true, true);
		updateMetadata(row_, col_);
		row_ = -1;
		col_ = -1;
		edit_ = false;
	}
	if(row == 2 || row == 4)
	{
		MediaDescription* editWidget;
		if(row == 2)
		{
			editWidget= new MediaDescription(this, tr("Source"), true);
			editWidget->setCaption(tr("Source"));
		}
		else
		{
			editWidget= new MediaDescription(this, tr("Description"), true);
			editWidget->setCaption(tr("Description"));
		}

		string val = propTable->text(row, col).latin1();
		editWidget->descriptionTextEdit->setText(val.c_str());
		editWidget->exec();

		string newVal = editWidget->descriptionTextEdit->text().latin1();
		delete editWidget;
		if(newVal.size() > 255)
		{
			QMessageBox::warning(this, tr("Warning"),
				tr("The text is longer than 255 caracteres!\nIt will be truncated."));
			newVal = newVal.substr(0, 255);
		}
		if(newVal.empty())
			newVal = " ";

		propTable->setText(row, col, newVal.c_str());
		propTable->adjustRow(row);
		raise();

		updateMetadata(row, col);
		row_ = -1;
		col_ = -1;
		edit_ = false;
	}
	else if(row <= 7)
	{
		row_ = row;
		col_ = col;
		edit_ = true;
	}
}


void TableProperties::valueChangedSlot( int /* row */, int /* col */ )
{
	updateMetadata(row_, col_);
	edit_ = false;
}



void TableProperties::updateMetadata( int row, int col )
{
	string up = "UPDATE te_table_metadata SET ";
	string val = propTable->text(row, col).latin1();

	if(row == 0)
		up += "name = '" + val + "'";
	else if(row == 1)
		up += "author = '" + val + "'";
	else if(row == 2)
		up += "source = '" + val + "'";
	else if(row == 3)
		up += "quality = '" + val + "'";
	else if(row == 4)
		up += "description = '" + val + "'";
	else if(row == 5)
		up += "date_ = '" + val + "'";
	else if(row == 6)
		up += "hour_ = '" + val + "'";
	else if(row == 7)
		up += "transf = " + val;

	up += " WHERE table_id = " + Te2String(table_id_);

	if(db_->execute(up) == false)
	{
		QMessageBox::critical(this, tr("Error"),
			tr("Fail to update the \"te_table_metadata\" table!"));
	}
}


void TableProperties::init( string table )
{
	table_ = table;

	QString	cap = tr("Table Properties: ") + table.c_str();
	setCaption(cap);

	propTable->setNumCols(2);
	propTable->horizontalHeader()->setLabel(0, tr("Parameter"));
	propTable->horizontalHeader()->setLabel(1, tr("Value"));

	vector<string> param;
	vector<string> val;
	string s;

	if(db_->tableExist("te_table_metadata") == false)
	{
		int ret = createTeTableMetadata(db_);
		if(ret == 1)
		{
			QMessageBox::critical(this, tr("Error"), 
				tr("Fail to create the \"te_table_metadata\" table!"));
			return;
		}
		else if (ret == 2)
		{
			QMessageBox::information(this, tr("Error"),
				tr("Fail to create the integrity between the tables \"te_table_metadata\" and \"te_layer\" !"));
			return;
		}
	}

	TeDatabasePortal* portal = db_->getPortal();

	string sel = "SELECT table_id FROM te_layer_table WHERE attr_table = '" + table_ + "'";
	portal->freeResult();
	if(portal->query(sel) && portal->fetchRow())
		table_id_ = portal->getInt(0);
	else
	{
		delete portal;
		QString msg = tr("The table") + " \"" + table_.c_str() + "\" ";
		msg += tr("is not registered in the \"te_layer_table\" table!");
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}

	sel = "SELECT * FROM te_table_metadata WHERE table_id = " + Te2String(table_id_);
	portal->freeResult();
	if(portal->query(sel))
	{
		param.push_back(tr("Name").latin1());
		param.push_back(tr("Author").latin1());
		param.push_back(tr("Source").latin1());
		param.push_back(tr("Quality").latin1());
		param.push_back(tr("Description").latin1());
		param.push_back(tr("Date").latin1());
		param.push_back(tr("Hour").latin1());
		param.push_back(tr("Transf").latin1());
		if(portal->fetchRow())
		{
			string name = portal->getData(1);			
			val.push_back(name);
			string author = portal->getData(2);			
			val.push_back(author);
			string source = portal->getData(3);			
			val.push_back(source);
			string quality = portal->getData(4);			
			val.push_back(quality);
			string description = portal->getData(5);
			val.push_back(description);
			string date = portal->getData(6);			
			val.push_back(date);
			string hour = portal->getData(7);			
			val.push_back(hour);
			string transf = portal->getData(8);			
			val.push_back(transf);
		}
		else
		{
			portal->freeResult();
			string ins = "INSERT INTO te_table_metadata (table_id) VALUES (" + Te2String(table_id_) + ")";
			db_->execute(ins);
			val.push_back("");
			val.push_back("");
			val.push_back("");
			val.push_back("");
			val.push_back("");
			val.push_back("");
			val.push_back("");
			val.push_back("");
		}
	}
	delete portal;

	param.push_back(tr("Table Name").latin1());
	val.push_back(table_);
	param.push_back(tr("Table Id").latin1());
	val.push_back(Te2String(table_id_));
	param.push_back("");
	val.push_back("");


	TeAttributeList attList;
	db_->getAttributeList(table_, attList);

	unsigned int i;
	for(i=0; i<attList.size(); i++)
	{
		param.push_back(tr("Name").latin1());
		val.push_back(attList[i].rep_.name_);
		param.push_back(tr("Type").latin1());
		if(attList[i].rep_.type_ == TeINT)
			val.push_back("TeINT");
		else if(attList[i].rep_.type_ == TeSTRING)
		{
			val.push_back("TeSTRING");
			param.push_back(tr("Number of  Characters").latin1());
			val.push_back(Te2String(attList[i].rep_.numChar_));
		}
		else if(attList[i].rep_.type_ == TeREAL)
		{
			val.push_back("TeREAL");
//			param.push_back(tr("Decimals").latin1());
//			val.push_back(Te2String(attList[i].rep_.decimals_));
		}
		else
			val.push_back(Te2String(attList[i].rep_.type_));

		param.push_back("");
		val.push_back("");
	}

	propTable->setNumRows(param.size());
	for(i=0; i<(unsigned int)param.size(); i++)
	{
		propTable->setText(i, 0, param[i].c_str());
		propTable->setText(i, 1, val[i].c_str());
		if(i == 2 || i == 4 || i >= 8)
			propTable->setRowReadOnly(i, true);
	}
	propTable->adjustColumn(0);
	propTable->adjustColumn(1);
	propTable->setColumnStretchable(0, false);
	propTable->setColumnReadOnly(0, true);

	propTable->adjustRow(2);
	propTable->adjustRow(4);
}


void TableProperties::contextMenuSlot( int, int, const QPoint &p )
{
	QPopupMenu* popup = new QPopupMenu;
	popup->insertItem(tr("Save As txt..."), this, SLOT(saveTxtSlot()));
	popup->move(p.x(), p.y());
	popup->exec();
}


void TableProperties::saveTxtSlot()
{
    QString s = QFileDialog::getSaveFileName(
                    "",
                    tr("File (*.txt)"),
                    this,
                    tr("Save File Dialog"),
                    tr("Choose a filename to save under"));

	if(s.isNull() || s.isEmpty())
		return;

	if(s.findRev(".txt", -1, false) == -1)
		s.append(".txt");

	string file = s.latin1();
	FILE* fp = fopen(file.c_str(), "r");
	if(fp)
	{
		fclose(fp);
		int response = QMessageBox::question(this, tr("File already exists"),
				 tr("Do you want to overwrite the file?"),
				 tr("Yes"), tr("No"));

		if (response != 0)
			return;
	}
	if((fp = fopen(file.c_str(), "w")) == 0)
	{
		QMessageBox::critical(this, tr("Error"), tr("The file cannot be opened!"));
		return;
	}

	string ss = tr("Properties of the Table:").latin1();
	ss += " ";
	ss += table_ + "\n\n";
	if(fwrite(ss.c_str(), sizeof( char ), ss.size(), fp) < ss.size())
	{
		fclose(fp);
		QMessageBox::critical(this, tr("Error"), tr("Fail to save the table properties!"));
		return;
	}

	int	i;
	for(i=0; i<propTable->numRows(); i++)
	{
		ss = propTable->text(i, 0).latin1();
		while(ss.size() < 16)
			ss += " ";
		string sa = propTable->text(i, 1).latin1();
		int f = sa.find("\n");
		while(f >= 0)
		{
			sa.insert(f+1, "                ");
			f += 16;
			f = sa.find("\n", f);
		}
		ss += sa;
		ss += "\n";
		if(i < 8)
			ss += "\n";

		if(fwrite(ss.c_str(), sizeof( char ), ss.size(), fp) < ss.size())
		{
			fclose(fp);
			QMessageBox::critical(this, tr("Error"), tr("Fail to save the table properties!"));
			return;
		}
	}
	fclose(fp);

	QMessageBox::information(this, tr("Information"),
		tr("The save file operation was executed successfully!"));
}
