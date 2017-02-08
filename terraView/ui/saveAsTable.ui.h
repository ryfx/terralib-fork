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
#ifndef _MSVS
	#include <teConfig.h>
#endif

#ifdef WIN32
#include <direct.h>
#endif

#include <TeDatabaseFactory.h>
#include <TeDatabaseUtils.h>
#include <TeQtProgress.h>
#include <algorithm>
using namespace std;

void SaveAsTable::init()
{
	help_ = 0;

	mainWindow_= (TerraViewBase*)qApp->mainWidget();
	TeAppTheme* curAppTheme = mainWindow_->currentTheme();
	TeTheme* curTheme = (TeTheme*)curAppTheme->getTheme();
	if(curTheme->type() != TeTHEME)
		return;

	dbOut_ = 0;

	linesComboBox->clear();
	linesComboBox->insertItem(tr("All"));
	linesComboBox->insertItem(tr("Pointed"));
	linesComboBox->insertItem(tr("Not Pointed"));
	linesComboBox->insertItem(tr("Queried"));
	linesComboBox->insertItem(tr("Not Queried"));
	linesComboBox->insertItem(tr("Pointed And Queried"));
	linesComboBox->insertItem(tr("Pointed Or Queried"));
	linesComboBox->setCurrentItem(0);

	columnsComboBox->clear();
	columnsComboBox->insertItem(tr("All"));
	columnsComboBox->insertItem(tr("Visible"));
	columnsComboBox->insertItem(tr("Selected"));
	columnsComboBox->setCurrentItem(0);

	int i = linesComboBox->currentItem();
	QString colMode = columnsComboBox->currentText();
	init(i, colMode);

	operationButtonGroup_clicked(0); // connect radioButton is initially on
}


bool SaveAsTable::init( int lineMode, QString colMode )
{
	unsigned int i;
	TeAppTheme* curAppTheme = mainWindow_->currentTheme();
	TeTheme* curTheme = (TeTheme*)curAppTheme->getTheme();
	//databasesListView_ = mainWindow_->getDatabasesListView();
	dbIn_ = mainWindow_->currentDatabase();

	if (dbIn_->dbmsName() != "Ado" && dbIn_->dbmsName() != "OracleAdo" && dbIn_->dbmsName() != "MySQL")
	{
		QString msg = tr("This module was only implemented for") + "\n";
		msg += tr("Microsoft ACCESS and MySQL databases!");
		QMessageBox::warning(this, tr("Warning"), msg);
		return false;
	}

	dbComboBox->setEnabled(false);
//	operationButtonGroup_clicked(0); // connect radioButton is initially on

	string CTE = curTheme->collectionTable();
	string input;
	if(lineMode == 1) // pointed
	{
		input_ = " WHERE (grid_status = 1 OR grid_status = 3";
		input_ += " OR (grid_status is null AND (c_object_status = 1 OR c_object_status = 3)))";
	}
	else if(lineMode == 2) // not pointed
	{
		input_ = " WHERE (grid_status = 0 OR grid_status = 2";

		input_ += " OR (grid_status is null AND (c_object_status = 0 OR c_object_status = 2)))";
	}
	if(lineMode == 3) // queried
	{
		input_ = " WHERE (grid_status = 2 OR grid_status = 3";
		input_ += " OR (grid_status is null AND (c_object_status = 2 OR c_object_status = 3)))";
	}
	else if(lineMode == 4) // not queried
	{
		input_ = " WHERE (grid_status = 0 OR grid_status = 1";
		input_ += " OR (grid_status is null AND (c_object_status = 0 OR c_object_status = 1)))";
	}
	else if(lineMode == 5) // pointed and queried
	{
		input_ = " WHERE (grid_status = 3";
		input_ += " OR (grid_status is null AND c_object_status = 3))";
	}
	else if(lineMode == 6) // pointed or queried
	{
		input_ = " WHERE (grid_status <> 0";
		input_ += " OR (grid_status is null AND c_object_status <> 0))";
	}

	TeAttributeList tatl = curTheme->sqlAttList();
	atOut_.clear();

	TeQtGrid* grid = mainWindow_->getGrid();
	attributesIn_.clear();
	attributesOut_.clear();
	if(colMode == tr("All"))
	{
		for(i=0; i<tatl.size(); i++)
		{
			int j = grid->getColumn(i);
			TeAttribute t = tatl[j];
			string name = t.rep_.name_;
			attributesIn_ += name + ", ";
			std::string::size_type k = name.find(".");
			while(k != string::npos)
			{
				name.replace(k, 1, "_");
				k = name.find(".");
			}
			attributesOut_ += name + ", ";
			t.rep_.name_ = name;
			t.rep_.isPrimaryKey_ = false;
			atOut_.push_back(t);
		}
	}
	else if(colMode == tr("Visible"))
	{
		for(i=0; i<tatl.size(); i++)
		{
			if(grid->getVisColumn(i))
			{
				int j = grid->getColumn(i);
				TeAttribute t = tatl[j];
				string name = t.rep_.name_;
				attributesIn_ += name + ", ";
				std::string::size_type k = name.find(".");
				while(k != string::npos)
				{
					name.replace(k, 1, "_");
					k = name.find(".");
				}
				attributesOut_ += name + ", ";
				t.rep_.name_ = name;
				t.rep_.isPrimaryKey_ = false;
				atOut_.push_back(t);
			}
		}
	}
	else // selected columns
	{
		for(i=0; i<tatl.size(); i++)
		{
			if(grid->isColumnSelected(i, true))
			{
				int j = grid->getColumn(i);
				TeAttribute t = tatl[j];
				string name = t.rep_.name_;
				attributesIn_ += name + ", ";
				std::string::size_type k = name.find(".");
				while(k != string::npos)
				{
					name.replace(k, 1, "_");
					k = name.find(".");
				}
				attributesOut_ += name + ", ";
				t.rep_.name_ = name;
				t.rep_.isPrimaryKey_ = false;
				atOut_.push_back(t);
			}
		}
	}
	attributesIn_ = attributesIn_.substr(0, attributesIn_.size()-2);	
	attributesOut_ = attributesOut_.substr(0, attributesOut_.size()-2);	

	selCount_ = "SELECT COUNT(*)" + curTheme->sqlGridFrom();
	sel_ = "SELECT " + attributesIn_ + curTheme->sqlGridFrom();
	if(input_.empty() == false)
	{
		sel_ += input_;
		selCount_ += input_;
	}

	QString	user = dbIn_->user().c_str(); 
	QString	host = dbIn_->host().c_str(); 
	QString	password = dbIn_->password().c_str(); 
	QString	port = (Te2String(dbIn_->portNumber())).c_str();
	QString dbName = dbIn_->databaseName().c_str();
	
	userLineEdit->setText(user);
//	passwordLineEdit->setText(password);
	hostLineEdit->setText(host);
	portLineEdit->setText(port);
	if (connectRadioButton->isOn()) //connect
		databaseNameLineEdit->setText(dbName);

	if(dbIn_->dbmsName() == "Ado")
	{
		hostLineEdit->setEnabled(false);
		portLineEdit->setEnabled(false);
		userLineEdit->setEnabled(false);
		passwordLineEdit->setEnabled(false);
		dbPushButton->setEnabled(true);
		dbComboBox->setEnabled(false);
	}
	else if(dbIn_->dbmsName() == "OracleAdo")
	{
		hostLineEdit->setEnabled(true);
		userLineEdit->setEnabled(true);
		passwordLineEdit->setEnabled(true);
		databaseNameLineEdit->setEnabled(true);
		dbPushButton->setEnabled(false);
		dbComboBox->setEnabled(false);
		createRadioButton->setEnabled(false);
	}
	else
	{
		hostLineEdit->setEnabled(true);
		userLineEdit->setEnabled(true);
		passwordLineEdit->setEnabled(true);
		dbPushButton->setText(tr("Select Database"));
	}

	return true;
}

void SaveAsTable::operationButtonGroup_clicked( int buttonId )
{
	if(dbOut_)
	{
		dbOut_->close();
		delete dbOut_;
		dbOut_ = 0;
	}

	if (dbIn_->dbmsName() == "Ado")
	{
		if (buttonId == 0)   //connection operation
		{
			dbPushButton->setText(tr("Select Database..."));
			dbComboBox->clear();
			databaseNameLineEdit->clear();
			databaseNameLineEdit->setEnabled(false);
		}
		else				 //create operation
		{
			dbPushButton->setText(tr("Directory..."));
			dbComboBox->clear();;
			databaseNameLineEdit->clear();
			databaseNameLineEdit->setEnabled(true);
		}
	}
	else
	{
		if(dbIn_->dbmsName() == "SQL Server")
			portLineEdit->setEnabled(false);
		else
			portLineEdit->setEnabled(true);

		dbComboBox->clear();
		if (dbIn_->dbmsName() == "MySQL")
		{
			if(buttonId == 0) //connection operation
			{
				dbPushButton->setEnabled(true);
				dbComboBox->setEnabled(true);
				databaseNameLineEdit->clear();
				databaseNameLineEdit->setEnabled(false);
			}
			else
			{
				dbPushButton->setEnabled(false);
				dbComboBox->setEnabled(false);
				databaseNameLineEdit->clear();
				databaseNameLineEdit->setEnabled(true);
			}
		}
		else
		{
			dbPushButton->setEnabled(false);
			dbComboBox->setEnabled(false);
			databaseNameLineEdit->clear();
			databaseNameLineEdit->setEnabled(true);
		}
	}
}


void SaveAsTable::dbPushButton_clicked()
{
	if(dbOut_)
	{
		dbOut_->close();
		delete dbOut_;
		dbOut_ = 0;
	}

	if (dbIn_->dbmsName() == "Ado")
	{
		if (connectRadioButton->isOn())
		{
			string startDir = QDir::currentDirPath();
			QFileDialog fileDialog;
			QString openFile = fileDialog.getOpenFileName(
				startDir.c_str(),
				"Microsoft Access (*.mdb *.accdb)",
				this,
				tr("Select Database File"),
				tr("Choose a Microsoft Access File"));

			if (!openFile.isEmpty())
			{
				dbComboBox->insertItem(openFile);
				string filename;
				if(openFile.find(".accdb",0)<0)
				{
					filename = TeGetBaseName (openFile.latin1()) + ".mdb";
				}else
				{
					filename = TeGetBaseName (openFile.latin1()) + ".accdb";
				}
				databaseNameLineEdit->setText(filename.c_str());

				 
				TeDatabaseFactoryParams params;
				params.dbms_name_ = "Ado";
				dbOut_ = TeDatabaseFactory::make(params);

				std::string::size_type k = filename.find(".accdb", 0, 4);
				if (k == string::npos)
				{
					k = filename.find(".mdb", 0, 4);
					if (k == string::npos)
						filename += ".mdb";
				}
				string fullName = dbComboBox->currentText().latin1();
				string userName = userLineEdit->text().latin1();
				string password = passwordLineEdit->text().latin1();
				string hostName = hostLineEdit->text().latin1();
				int	port = atoi(portLineEdit->text().latin1());
//				bool status = dbOut_->connect(hostName, userName, password, fullName, port);
				dbOut_->connect(hostName, userName, password, fullName, port);
			}
			else
			{
				QMessageBox::warning(this, tr("Warning"),
					tr("Select a database file!"));
				databaseNameLineEdit->clear();
				return;
			}
		}
		else
		{
			QString dirName = QFileDialog::getExistingDirectory(
				QString::null,
				this,
				tr("Get existing directory"),
				tr("Select a directory"),
				true);
		 
			if (!dirName.isEmpty())
				dbComboBox->insertItem(dirName);
			else
			{
				QMessageBox::warning(this, tr("Warning"),
					tr("Select a directory!"));
				return;
			}
		}

		if(dbOut_ && isTerralibModel(dbOut_))
		{
			QString msg = tr("The database selected is a TerraLib database!") + "\n";
			msg += tr("Select another database or create a new one!");
			QMessageBox::warning(this, tr("Warning"), msg);

			tableComboBox->clear();
			tableComboBox->clear();
			databaseNameLineEdit->clear();
			dbOut_->close();
			delete dbOut_;
			dbOut_ = 0;
			dbPushButton_clicked();
		}
		if(dbOut_ == 0) // protection for recursive case
			return;

		vector<string> tables;
		dbOut_->listTables(tables);

		tableComboBox->clear();
		for(unsigned int i = 0; i<tables.size(); i++)
		{
			string s = tables[i];
			std::string::size_type k = s.find("MSys", 0, 4);
			if (k == string::npos)
				tableComboBox->insertItem(s.c_str());
		}
	}

	if (connectRadioButton->isOn())
	{
		if (dbIn_->dbmsName() == "MySQL")
		{
			vector<string> dbNames;
			TeDatabaseFactoryParams params;
			params.dbms_name_ = "MySQL";
			dbOut_ = TeDatabaseFactory::make(params);
			dbOut_->showDatabases(hostLineEdit->text().latin1(), userLineEdit->text().latin1(),
							  passwordLineEdit->text().latin1(), dbNames, 0);

			delete dbOut_;
			dbOut_ = 0;
			databaseNameLineEdit->clear();
			dbComboBox->clear();
			for (unsigned int i = 0; i < dbNames.size(); ++i)
				dbComboBox->insertItem(dbNames[i].c_str());
		}
	}
}


void SaveAsTable::dbComboBox_activated( const QString& dbName)
{
	if (dbIn_->dbmsName() == "Ado")
		databaseNameLineEdit->setText(dbName);
	else
	{
		string dbName = dbComboBox->currentText().latin1();
		string userName = userLineEdit->text().latin1();
		string password = passwordLineEdit->text().latin1();
		string hostName = hostLineEdit->text().latin1();
//		int	port = atoi(portLineEdit->text().latin1());
		TeDatabaseFactoryParams params;
		params.dbms_name_ = "MySQL";
		dbOut_ = TeDatabaseFactory::make(params);
		bool status = dbOut_->connect(hostName,userName,password,dbName,0);
		if(status == false)
		{
			QString msg = tr("Fail to make the connection to the database!") + "\n";
			msg += dbOut_->errorMessage().c_str();
			QMessageBox::critical(this, tr("Error"), msg);
			delete dbOut_;
			dbOut_ = 0;
			return;
		}

		if(dbOut_ && isTerralibModel(dbOut_))
		{
			QString msg = tr("The database selected is a TerraLib database!") + "\n";
			msg += tr("Select another database or create a new one!");
			QMessageBox::warning(this, tr("Warning"), msg);

			tableComboBox->clear();
			databaseNameLineEdit->clear();
			dbOut_->close();
			delete dbOut_;
			dbOut_ = 0;
			dbPushButton_clicked();
		}
		if(dbOut_ == 0) // protection for recursive case
			return;

		databaseNameLineEdit->setText(dbComboBox->currentText());
		vector<string> tables;
		dbOut_->listTables(tables);

		tableComboBox->clear();
		for(unsigned int i = 0; i<tables.size(); i++)
		{
			string s = tables[i];
			std::string::size_type k = s.find("MSys", 0, 4);
			if (k == string::npos)
				tableComboBox->insertItem(s.c_str());
		}
	}
}


void SaveAsTable::tablePushButton_clicked()
{

}


void SaveAsTable::tableComboBox_activated( const QString & )
{

}

void SaveAsTable::okPushButton_clicked()
{
	int i = linesComboBox->currentItem();
	QString colMode = columnsComboBox->currentText();
	if(!init(i, colMode))
	{
		return;
	}

	QString msg;

	if(databaseNameLineEdit->text().isEmpty())
	{
		if (connectRadioButton->isOn())
			msg += tr("Select a new destination database");
		else
			msg += tr("Create a new destination database");
	}
	if (dbIn_->dbmsName() == "Ado")
	{
		if(msg.isEmpty() && dbComboBox->currentText().isEmpty())
			msg += tr("Create a new destination database");
	}

	if(tableComboBox->currentText().isEmpty())
	{
		if (connectRadioButton->isOn())
		{
			if(msg.isEmpty() == false)
			{
				msg += "\n";
				msg += tr("and select or enter a new destination table name!");
			}
			else
				msg += tr("Select or enter a new destination table name!");
		}
		else
		{
			if(msg.isEmpty() == false)
			{
				msg += "\n";
				msg += tr("and enter a new destination table name!");
			}
			else
				msg += tr("Enter a new destination table name!");
		}

	}

	if(msg.isEmpty() == false)
	{
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

	string table = tableComboBox->currentText().latin1();

	if (!connectRadioButton->isOn()) //create new database
	{
		if(dbIn_->dbmsName() == "MySQL")
		{
			if(dbOut_)
			{
				dbOut_->close();
				delete dbOut_;
			}
			TeDatabaseFactoryParams params;
			params.dbms_name_ = "MySQL";
			dbOut_ = TeDatabaseFactory::make(params);
			string dbName = databaseNameLineEdit->text().latin1();
			string userName = userLineEdit->text().latin1();
			string password = passwordLineEdit->text().latin1();
			string hostName = hostLineEdit->text().latin1();
//			int	port = atoi(portLineEdit->text().latin1());
			if(dbOut_->newDatabase(dbName,userName,password,hostName, 0, false) == false)
			{
				msg = tr("Fail to create the database") + " \"" + dbName.c_str() + "\"\n";
				msg += dbOut_->errorMessage().c_str();
				QMessageBox::information( this, tr("Error"), msg);
				tableComboBox->clear();
				databaseNameLineEdit->clear();
				delete dbOut_;
				dbOut_ = 0;
				return;
			}
			if(dbOut_->connect(hostName,userName,password,dbName,0) == false)
			{
				msg = tr("Fail to make the connection to the database!") + "\n";
				msg += dbOut_->errorMessage().c_str();
				QMessageBox::critical(this, tr("Error"), msg);
				tableComboBox->clear();
				databaseNameLineEdit->clear();
				delete dbOut_;
				dbOut_ = 0;
				return;
			}
		} //MySQL

		else if (dbIn_->dbmsName() == "Ado")
		{
			string dir = dbComboBox->currentText().latin1();
			string file = databaseNameLineEdit->text().latin1();
			std::string::size_type k;
			if((k = file.find(".accdb"))==std::string::npos)
			{
				k = file.find(".mdb");
				if (k == string::npos)
				file += ".mdb";
			}
			else if((k != file.size() - 4) && (k != file.size() - 6))
			{
				msg = tr("Invalid database name!") + "\n";
				msg += tr("Please, enter with another database name!");
				QMessageBox::warning(this, tr("Warning"), msg);
				tableComboBox->clear();
				databaseNameLineEdit->clear();
				return;
			}

			string absFile;
			k = dir.find("\\");
			if (k == string::npos)
				absFile = dir + "/" + file;
			else
				absFile = dir + "\\" + file;

			if(dbOut_)
			{
				dbOut_->close();
				delete dbOut_;
			}
			TeDatabaseFactoryParams params;
			params.dbms_name_ = "Ado";
			dbOut_ = TeDatabaseFactory::make(params);
			string userName = userLineEdit->text().latin1();
			string password = passwordLineEdit->text().latin1();
			string hostName = hostLineEdit->text().latin1();
			int	port = atoi(portLineEdit->text().latin1());
			if(dbOut_->newDatabase(absFile, userName, password, hostName, 0, false) == false)
			{
				msg = tr("Fail to create the database!") + "\n";
				msg += dbOut_->errorMessage().c_str();
				QMessageBox::critical(this, tr("Error"), msg);
				tableComboBox->clear();
				databaseNameLineEdit->clear();
				delete dbOut_;
				dbOut_ = 0;
				return;
			}
			if(dbOut_->connect(hostName, userName, password, absFile, port) == false)
			{
				msg = tr("Fail to make the connection to the database!") + "\n";
				msg += dbOut_->errorMessage().c_str();
				QMessageBox::critical(this, tr("Error"), msg);
				tableComboBox->clear();
				databaseNameLineEdit->clear();
				delete dbOut_;
				dbOut_ = 0;
				return;
			}
		}

		else if (dbIn_->dbmsName() == "OracleAdo")
		{
			if(dbOut_)
			{
				dbOut_->close();
				delete dbOut_;
			}
			TeDatabaseFactoryParams params;
			params.dbms_name_ = "OracleAdo";
			dbOut_ = TeDatabaseFactory::make(params);
			string dbName = databaseNameLineEdit->text().latin1();
			string userName = userLineEdit->text().latin1();
			string password = passwordLineEdit->text().latin1();
			string hostName = hostLineEdit->text().latin1();
//			int	port = atoi(portLineEdit->text().latin1());
			if(dbOut_->newDatabase(dbName,userName,password,hostName, 0, false) == false)
			{
				msg = tr("Fail to create the database") + " \"" + dbName.c_str() + "\"\n";
				msg += dbOut_->errorMessage().c_str();
				QMessageBox::information( this, tr("Error"), msg);
				tableComboBox->clear();
				databaseNameLineEdit->clear();
				delete dbOut_;
				dbOut_ = 0;
				return;
			}
			if(dbOut_->connect(hostName,userName,password,dbName,0) == false)
			{
				msg = tr("Fail to make the connection to the database!") + "\n";
				msg += dbOut_->errorMessage().c_str();
				QMessageBox::critical(this, tr("Error"), msg);
				tableComboBox->clear();
				databaseNameLineEdit->clear();
				delete dbOut_;
				dbOut_ = 0;
				return;
			}
		}
	

		if(dbOut_->createTable(table, atOut_) == false)
		{
			msg = tr("Fail to create the table") + " \"" + table.c_str();
			msg += "\" " + tr("in the database") + "\n";
			msg += dbOut_->errorMessage().c_str();
			QMessageBox::critical(this, tr("Error"), msg);
			tableComboBox->clear();
			databaseNameLineEdit->clear();
			dbOut_->close();
			delete dbOut_;
			dbOut_ = 0;
			return;
		}
	} //end create new database
	
	else // connect to existing database
	{
		if(!dbOut_) // it must do the connection first
		{
			string dbName = databaseNameLineEdit->text().latin1();
			string userName = userLineEdit->text().latin1();
			string password = passwordLineEdit->text().latin1();
			string hostName = hostLineEdit->text().latin1();

			if(dbIn_->dbmsName() == "MySQL")
			{
				TeDatabaseFactoryParams params;
				params.dbms_name_ = "MySQL";
				dbOut_ = TeDatabaseFactory::make(params);
				if(dbOut_->connect(hostName,userName,password,dbName,0) == false)
				{
					msg = tr("Fail to make the connection to the database!") + "\n";
					msg += dbOut_->errorMessage().c_str();
					QMessageBox::critical(this, tr("Error"), msg);
					tableComboBox->clear();
					databaseNameLineEdit->clear();
					delete dbOut_;
					dbOut_ = 0;
					return;
				}
			}

			else if (dbIn_->dbmsName() == "Ado")
			{
				string dir = dbComboBox->currentText().latin1();
				string file = dbName;
				std::string::size_type k = file.find(".mdb");
				if (k == string::npos)
					file += ".mdb";
				else if(k != file.size() - 4)
				{
					msg = tr("Invalid database name!") + "\n";
					msg += tr("Please, enter with another database name!");
					QMessageBox::warning(this, tr("Warning"), msg);
					tableComboBox->clear();
					databaseNameLineEdit->clear();
					return;
				}

				string absFile;
				k = dir.find("\\");
				if (k == string::npos)
					absFile = dir + "/" + file;
				else
					absFile = dir + "\\" + file;

				TeDatabaseFactoryParams params;
				params.dbms_name_ = "Ado";
				dbOut_ = TeDatabaseFactory::make(params);
				int	port = atoi(portLineEdit->text().latin1());
				if(dbOut_->connect(hostName, userName, password, absFile, port) == false)
				{
					msg = tr("Fail to make the connection to the database!") + "\n";
					msg += dbOut_->errorMessage().c_str();
					QMessageBox::critical(this, tr("Error"), msg);
					tableComboBox->clear();
					databaseNameLineEdit->clear();
					delete dbOut_;
					dbOut_ = 0;
					return;
				}
			}
			
			else if (dbIn_->dbmsName() == "OracleAdo")
			{

				TeDatabaseFactoryParams params;
				params.dbms_name_ = "OracleAdo";
				dbOut_ = TeDatabaseFactory::make(params);
				if(dbOut_->connect(hostName,userName,password,dbName,0) == false)
				{
					msg = tr("Fail to make the connection to the database!") + "\n";
					msg += dbOut_->errorMessage().c_str();
					QMessageBox::critical(this, tr("Error"), msg);
					tableComboBox->clear();
					databaseNameLineEdit->clear();
					delete dbOut_;
					dbOut_ = 0;
					return;
				}
			}
		} //end 
		
		if(dbOut_->tableExist(table))
		{
			if(dbOut_->deleteTable (table) == false)
			{
				msg = tr("The selected table cannot be modified!") + "\n";
				msg += dbOut_->errorMessage().c_str();
				QMessageBox::critical(this, tr("Error"), msg);
				tableComboBox->clear();
				databaseNameLineEdit->clear();
				dbOut_->close();
				delete dbOut_;
				dbOut_ = 0;
				return;
			}
		}
		if(dbOut_->createTable(table, atOut_) == false)
		{
			msg = tr("Fail to create the table") + " \"" + table.c_str();
			msg += "\" " + tr("in the database") + "\n";
			msg += dbOut_->errorMessage().c_str();
			QMessageBox::critical(this, tr("Error"), msg);
			//tableComboBox->clear();
			//databaseNameLineEdit->clear();
			tableComboBox->clear();
			databaseNameLineEdit->clear();
			dbOut_->close();
			delete dbOut_;
			dbOut_ = 0;
			return;
		}
	}

	TeQtProgress progress(this, "ProgressDialog", true);
	TeWaitCursor wait;
	int	total, np = 0;
	TeDatabasePortal* portal = dbIn_->getPortal();

	if(portal->query(selCount_))
	{
		if(portal->fetchRow())
			total = portal->getInt(0);
	}

	portal->freeResult();
	if(portal->query(sel_))
	{
		QString caption = tr("Export Table");
		progress.setCaption(caption.latin1());
		progress.setLabelText(tr("Exporting table. Please, wait!"));
		progress.setTotalSteps(total);
		while(portal->fetchRow())
		{
			string values = "(";
			string atrs = " (";
			TeAttributeList AL = atOut_;
			for(unsigned int i = 0; i < atOut_.size(); i++)
			{
				string s = portal->getData(i);
				s = dbOut_->escapeSequence(s);
				atrs += AL[i].rep_.name_ + ",";
				if(AL[i].rep_.type_ == TeSTRING)
					values += "'"+ s + "',";
				else
				{
					if(s.empty())
						s = "null";
					values += s +",";
				}
			}
			if(values.size() > 2)
				values.replace(values.size()-1, 1, ")");
			if(atrs.size() > 3)
				atrs.replace(atrs.size()-1, 1, ")");

			ins_ = "INSERT INTO " + table + atrs + " VALUES " + values;

			//////////// PAREI AQUI - N�O EXECUTA INS!!!!
			if(dbOut_->execute(ins_) == false)
			{
				wait.resetWaitCursor();
				msg = dbOut_->errorMessage().c_str();
				QMessageBox::critical(this, tr("Error"), msg);
				databaseNameLineEdit->clear();
				dbOut_->close();
				delete dbOut_;
				dbOut_ = 0;
				delete portal;
				return;
			}
			if(progress.wasCancelled())
			{
				dbOut_->close();
				delete dbOut_;
				dbOut_ = 0;
				delete portal;
				hide();
				return;
			}
			progress.setProgress(++np);
		}
	}
	delete portal;

	wait.resetWaitCursor();
	msg = tr("The grid contents was exported successfully!");
	QMessageBox::information(this, tr("Information"), msg);

	dbOut_->close();
	delete dbOut_;
	dbOut_ = 0;
	hide();
}


void SaveAsTable::closeEvent( QCloseEvent *)
{
	if(dbOut_)
	{
		dbOut_->close();
		delete dbOut_;
		dbOut_ = 0;
	}
	hide();
}


void SaveAsTable::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("saveAsTable.htm");
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

void SaveAsTable::linesComboBox_activated(const QString&)
{
	//int i = linesComboBox->currentItem();
	//QString c = columnsComboBox->currentText();
	//init(i, c.latin1());
}


void SaveAsTable::columnsComboBox_activated(const QString& /*s*/)
{
	//int i = linesComboBox->currentItem();
	//init(i, s.latin1());
}


void SaveAsTable::show()
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
