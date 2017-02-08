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
#include <windows.h>
#endif //WIN32

#include <TeDatabaseUtils.h>
#include <TeColorUtils.h>
#include <TeUpdateDBVersion.h>
#include <TeQtViewsListView.h>
#include <TeDatabaseFactory.h>
#include <TeApplicationUtils.h>
#include <TeDBConnectionsPool.h>

#include <qfile.h>

void DatabaseWindow::init()
{
	help_ = 0;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	databasesListView_ = mainWindow_->getDatabasesListView();
	
#ifdef ADO_FOUND
	databaseTypeComboBox->insertItem("Access");
	databaseTypeComboBox->insertItem("SQL Server");
	databaseTypeComboBox->insertItem("SQL Server Spatial");
	databaseTypeComboBox->insertItem("Oracle"); 
#endif //ADO_FOUND

#ifdef ORACLE_FOUND
	databaseTypeComboBox->insertItem("OracleOCI"); 
	databaseTypeComboBox->insertItem("OracleSpatial");
#endif //ORACLE_FOUND
	
#ifdef MySQL_FOUND
	databaseTypeComboBox->insertItem("MySQL");
#endif //ORACLE_FOUND

#ifdef PostGIS_FOUND
	databaseTypeComboBox->insertItem("PostgreSQL");
	databaseTypeComboBox->insertItem("PostGIS");
#endif //PostGIS_FOUND

#ifdef FBIRD_FOUND
	databaseTypeComboBox->insertItem("Firebird");
#endif //FBIRD_FOUND
	
//#ifdef WIN32
//	databaseTypeComboBox->setCurrentText("Access");
//	databaseTypeComboBox_activated("Access");
//#else
//	databaseTypeComboBox->setCurrentText("MySQL");
//	databaseTypeComboBox_activated("MySQL");
//#endif

	dbComboBox->setEnabled(false);
	cmbEncoding->setEnabled(false);
	operationButtonGroup_clicked(0); // connect radioButton is initially on

	getLastConnection();
}


void DatabaseWindow::operationButtonGroup_clicked(int buttonId)
{
	cmbEncoding->setEnabled(false);
	if (databaseTypeComboBox->currentText() == "Access")
	{
		hostLineEdit->setEnabled(false);
		portLineEdit->setEnabled(false);
		userLineEdit->setEnabled(false);
		passwordLineEdit->setEnabled(false);
		userLineEdit->clear();
		passwordLineEdit->clear();
		directoryPushButton->setEnabled(true);
		dbComboBox->setEnabled(false);
		if (buttonId == 0)   //connection operation
		{
			directoryPushButton->setText(tr("Select Database..."));
			dbComboBox->clear();
			databaseNameLineEdit->setText("");
			databaseNameLineEdit->setEnabled(false);
		}
		else				 //create operation
		{
			directoryPushButton->setText(tr("Directory..."));
			dbComboBox->clear();
			databaseNameLineEdit->setText("");
			databaseNameLineEdit->setEnabled(true);
		}
	}
	else if(databaseTypeComboBox->currentText() == "Firebird")
	{
		hostTextLabel->setText(tr("Host:"));
		hostLineEdit->setEnabled(true);
		portLineEdit->setEnabled(true);
		userLineEdit->setEnabled(true);
		passwordLineEdit->setEnabled(true);
		directoryPushButton->setEnabled(true);
		dbComboBox->setEnabled(false);
		databaseNameLineEdit->setEnabled(true);
		if (buttonId == 0)   //connection operation
		{
			directoryPushButton->setText(tr("Select Database..."));
			dbComboBox->clear();
			databaseNameLineEdit->setText("");
		}
		else				 //create operation
		{
			directoryPushButton->setText(tr("Directory..."));
			dbComboBox->clear();
			databaseNameLineEdit->setText("");
		}
	}
	else if ((databaseTypeComboBox->currentText() == "OracleSpatial") ||
			 (databaseTypeComboBox->currentText() == "Oracle") || (databaseTypeComboBox->currentText() == "OracleOCI"))
	{
		
		hostTextLabel->setText(tr("Service name:"));
		hostLineEdit->setEnabled(true);
		userLineEdit->setEnabled(true);
		passwordLineEdit->setEnabled(true);
		portLineEdit->setEnabled(false);
		directoryPushButton->setEnabled(false);
		dbComboBox->setEnabled(false);
		databaseNameLineEdit->setEnabled(false);
	}
	else
	{
		hostLineEdit->setEnabled(true);
		userLineEdit->setEnabled(true);
		passwordLineEdit->setEnabled(true);
		directoryPushButton->setText(tr("Select Database"));
		if( (databaseTypeComboBox->currentText() == "SQL Server") || (databaseTypeComboBox->currentText() == "SQL Server Spatial"))
		{
			portLineEdit->setEnabled(false);
			hostTextLabel->setText(tr("Host:"));
			hostLineEdit->setEnabled(true);
		}

		else
			portLineEdit->setEnabled(true);

		dbComboBox->clear();
		if(databaseTypeComboBox->currentText() == "MySQL" || databaseTypeComboBox->currentText() == "PostgreSQL" || databaseTypeComboBox->currentText() == "PostGIS")
		{
			hostTextLabel->setText(tr("Host:"));
			hostLineEdit->setEnabled(true);
			if(buttonId == 0) //connection operation
			{
				directoryPushButton->setEnabled(true);
				dbComboBox->setEnabled(true);
				databaseNameLineEdit->setText("");
				databaseNameLineEdit->setEnabled(false);
			}
			else
			{
				directoryPushButton->setEnabled(false);
				dbComboBox->setEnabled(false);
				databaseNameLineEdit->setText("");
				databaseNameLineEdit->setEnabled(true);
				if( databaseTypeComboBox->currentText() == "PostgreSQL" || databaseTypeComboBox->currentText() == "PostGIS" ) cmbEncoding->setEnabled(true);
			}
		}
		else
		{
			directoryPushButton->setEnabled(false);
			databaseNameLineEdit->setText("");
			databaseNameLineEdit->setEnabled(true);
		}
	}

	if (buttonId == 0)
		dbConsistencyCheckBox->setEnabled(true);
	else
		dbConsistencyCheckBox->setEnabled(false);
}


void DatabaseWindow::databaseTypeComboBox_activated( const QString & )
{
	if (connectRadioButton->isOn() == true)
		operationButtonGroup_clicked(0);
	else
		operationButtonGroup_clicked(1);
}



void DatabaseWindow::directoryPushButton_clicked()
{
	int portNumber = 0;
	if(!portLineEdit->text().isEmpty())
	{
		portNumber = atoi(portLineEdit->text().latin1());
	}

	if (databaseTypeComboBox->currentText() == "Firebird")
	{
		if (connectRadioButton->isOn())
		{
			// Check the contents of the configuration file in the current directory
			string connString, dbName, dbFullPath;
			dbFullPath = QDir::currentDirPath().latin1();
			
			QFileDialog fileDialog;
			QString openFile = fileDialog.getOpenFileName(
				dbFullPath.c_str(),
				"Firebird database (*.fdb)",
				this,
				tr("Select Database File"),
				tr("Choose a Firebird database File"));

			if (!openFile.isEmpty())
			{
				dbComboBox->insertItem(openFile);
				string filename = TeGetBaseName (openFile.latin1()) + ".fdb";
				dbComboBox->setCurrentText(openFile);
				databaseNameLineEdit->setText(filename.c_str());
			}
			else
			{
				QMessageBox::warning(this, tr("Warning"), tr("Select a database file!"));
				return;
			}
		}
		else
		{
			QString dirName = QFileDialog::getExistingDirectory(
				QString::null,
				this,
				tr("Get Existing Directory"),
				tr("Select a directory"),
				true);
		 
			if (!dirName.isEmpty())
				dbComboBox->insertItem(dirName);
			else
			{
				QMessageBox::warning(this, tr("Warning"), tr("Select a directory!"));
				return;
			}
		}
		return;
	}

#ifdef WIN32
	if (databaseTypeComboBox->currentText() == "Access")
	{
		if (connectRadioButton->isOn())
		{
			// Check the contents of the configuration file in the current directory
			string connString, dbName, dbFullPath;
			dbFullPath = QDir::currentDirPath();
			

			QFileDialog fileDialog;
			QString openFile = fileDialog.getOpenFileName(
				dbFullPath.c_str(),
				"Microsoft Access (*.mdb *.accdb)",
				this,
				tr("Select Database File"),
				tr("Choose a Microsoft Access File"));

			if (!openFile.isEmpty())
			{
				dbComboBox->insertItem(openFile);
				string filename;
				if(openFile.find("accdb")!=-1)	filename = TeGetBaseName (openFile.latin1()) + ".accdb";
				else							filename = TeGetBaseName (openFile.latin1()) + ".mdb";
				dbComboBox->setCurrentText(openFile);
				databaseNameLineEdit->setText(filename.c_str());
			}
			else
			{
				QMessageBox::warning(this, tr("Warning"), tr("Select a database file!"));
				return;
			}
		}
		else
		{
			QString dirName = QFileDialog::getExistingDirectory(
				QString::null,
				this,
				tr("Get Existing Directory"),
				tr("Select a directory"),
				true);
		 
			if (!dirName.isEmpty())
			{
				dbComboBox->insertItem(dirName);
				int index=dbComboBox->count() - 1;
				if(index>=0)	dbComboBox->setCurrentItem(index);
			}
			else
			{
				QMessageBox::warning(this, tr("Warning"), tr("Select a directory!"));
				return;
			}
		}
	}
#endif
	if (connectRadioButton->isOn())
	{
		if (databaseTypeComboBox->currentText() == "MySQL" || databaseTypeComboBox->currentText() == "PostgreSQL" || databaseTypeComboBox->currentText() == "PostGIS")
		{
			vector<string> dbNames;

			TeDatabase *db = 0;
			
			if(databaseTypeComboBox->currentText() == "MySQL")
			{
				TeDatabaseFactoryParams params;
				params.dbms_name_ = "MySQL";
				db = TeDatabaseFactory::make(params);
			}
      else if(databaseTypeComboBox->currentText() == "PostgreSQL")
			{   
				TeDatabaseFactoryParams params;
				params.dbms_name_ = "PostgreSQL";
				db = TeDatabaseFactory::make(params);

			}
      else if(databaseTypeComboBox->currentText() == "PostGIS")
			{
				TeDatabaseFactoryParams params;
				params.dbms_name_ = "PostGIS";
				db = TeDatabaseFactory::make(params);
   
			}
			if(db == 0)
			{
				QMessageBox::critical(this, tr("Error"), tr("Fail to get database driver."));
				return;
			}
      
			if (db->showDatabases(hostLineEdit->text().latin1(), userLineEdit->text().latin1(),
							  passwordLineEdit->text().latin1(), dbNames, portNumber) == false)
			{
				QMessageBox::critical(this, tr("Error"), db->errorMessage().c_str());
				delete db;
				dbComboBox->clear();
				return;
			}

			delete db;
			dbComboBox->clear();
			for (unsigned int i = 0; i < dbNames.size(); ++i)
				dbComboBox->insertItem(dbNames[i].c_str());
            dbComboBox_activated(dbComboBox->currentText());
		}		
	}
}


void DatabaseWindow::dbComboBox_activated(const QString &dbName)
{
	databaseNameLineEdit->setText(dbName);	
}


void DatabaseWindow::okPushButton_clicked()
{
 	TeWaitCursor wait;

	TeDatabase* tdb = 0;
	bool status = false;
	bool integrity = true;
	std::string	charSet="";

	string hostName = hostLineEdit->text().latin1();
	string userName = userLineEdit->text().latin1();
	string password = passwordLineEdit->text().latin1();
	string dirName = "";

	int portNumber = 0;
	if(!portLineEdit->text().isEmpty())
	{
		portNumber = atoi(portLineEdit->text().latin1());
	}

	if (dbComboBox->count() != 0)
		dirName  = dbComboBox->currentText().latin1();
	string dbName = databaseNameLineEdit->text().latin1();

	string dbType = databaseTypeComboBox->currentText().latin1();

	//verify what is the decimal separator
#ifdef ADO_FOUND
    if (dbType == "Access")
	{
		HKEY    hk;
		DWORD	DataSize = 2;
		DWORD   Type = REG_SZ;
		char    buf[2];

		string key = "Control Panel\\International";
		string sepDecimal = "sDecimal";	
		string sepDecimalResult = "";

		if (RegOpenKeyExA(HKEY_CURRENT_USER, key.c_str(), 0, KEY_READ, &hk) == ERROR_SUCCESS)
		{
			memset (buf, 0, 2);
			DataSize = 2;
			//decimal separator
			if (RegQueryValueExA(hk, sepDecimal.c_str(), NULL, &Type, (LPBYTE)buf, &DataSize) == ERROR_SUCCESS)
				sepDecimalResult = buf;
			
			RegCloseKey (hk);
		}

		if((!sepDecimalResult.empty()) && (sepDecimalResult==","))
		{
			mainWindow_->origDecimalSepIsPoint(false);
			if (RegOpenKeyExA(HKEY_CURRENT_USER, key.c_str(), 0, KEY_SET_VALUE, &hk) == ERROR_SUCCESS)
			{				
				memset (buf, 0, 2);
				buf[0] = '.';
				DataSize = 2;
					
				RegSetValueExA(hk, sepDecimal.c_str(), NULL, Type, (LPBYTE)buf, DataSize);  				
				RegCloseKey (hk);
			}
		}
	}
#endif //ADO_FOUND

	// Verify creation of databases that are not allowed (Oracle and SQL Server)
	if (createRadioButton->isOn())
	{
		if ((dbType == "OracleSpatial") || (dbType == "OracleOCI") || (dbType == "Oracle"))
		{
			wait.resetWaitCursor();
			QString mess = tr("TerraView cannot create a new Oracle database!");
			mess += "\n" + tr(" Only a DBA can create a new database. TerraView only can");
			mess += "\n" + tr(" connect to previously created database. ");
			QMessageBox::warning(this, tr("Warning"),mess);
			return;
		}
		else if ( (dbType == "SQL Server") || (dbType == "SQL Server Spatial") )
		{
			wait.resetWaitCursor();
			QString mess = tr("TerraView cannot create a new SQL Server database!");
			mess += "\n" + tr(" Only a DBA can create a new database. TerraView only can");
			mess += "\n" + tr(" connect to previously created database. ");
			QMessageBox::warning(this, tr("Warning"),mess);
			return;	
		}
	}

// Check the data provided by the user
	if (dbType == "Access" && dirName.empty() == true)
	{
		QString qs;
		if (createRadioButton->isOn()) // database creation
			qs = tr("Select the directory where the database file will be created!");
		else
			qs = tr("Select the database file to be connected!");
		wait.resetWaitCursor();
		QMessageBox::warning( this, tr("Warning"), qs);
		return;
	}

	if (dbType != "Access" && userName.empty())
	{
		wait.resetWaitCursor();
		QMessageBox::warning(this, tr("Warning"), tr("Enter the user name!"));
		return;
	}

	if (dbName.empty() && (dbType != "OracleSpatial") && (dbType != "OracleOCI") && (dbType != "Oracle"))
	{
		wait.resetWaitCursor();
		QString mess;
		if (createRadioButton->isOn())
			mess = tr("Give the name of the database that will be created!");
		else
			mess = tr("Select the database to be connected!");
		QMessageBox::warning(this, tr("Warning"),mess);
		return;
	}

// Creation or connection to the several types of databases
	if (createRadioButton->isOn())
	{

		//Verify the database name
		bool changed = false;
		std::string invalidChar = "";
		TeCheckName(dbName, changed, invalidChar);	
		if(changed)
		{
			wait.resetWaitCursor();
			QMessageBox::warning(this, tr("Warning"),tr("Invalid database name!"));
			databaseNameLineEdit->setFocus();
			return;
		}
		//End

		// operation is set to create a database
		if (dbType == "Access")
		{
			TeDatabaseFactoryParams params;
			params.dbms_name_ = "Ado";
			tdb = TeDatabaseFactory::make(params);

			std::string::size_type k = dbName.find(".accdb", 0, 4);
			if (k == string::npos)
			{
				k = dbName.find(".mdb", 0, 4);
				if (k == string::npos)
					dbName += ".mdb";
			}
			string fullName = dirName + "\\" + dbName;
			status =  tdb->newDatabase(fullName,userName,password,hostName, portNumber);
			if (status)
			{
				status = tdb->connect(fullName);
				
			}

		}
		else if (dbType == "MySQL")
		{
			TeDatabaseFactoryParams params;
			params.dbms_name_ = "MySQL";
			tdb = TeDatabaseFactory::make(params);

			status =  tdb->newDatabase(dbName,userName,password,hostName, portNumber);
			if (status)
				status = tdb->connect(hostName,userName,password,dbName, portNumber);
		}
		else if (dbType == "PostgreSQL")
		{
   
			TeDatabaseFactoryParams params;
			params.dbms_name_ = "PostgreSQL";
			tdb = TeDatabaseFactory::make(params);
			if(!cmbEncoding->currentText().isEmpty())
				charSet=cmbEncoding->currentText().latin1();
			status = tdb->newDatabase(dbName, userName, password, hostName, portNumber,true,charSet);
			
			if(status)
				status = tdb->connect(hostName, userName, password, dbName, portNumber);

		}
		else if (dbType == "PostGIS")
		{

			TeDatabaseFactoryParams params;
			params.dbms_name_ = "PostGIS";
			tdb = TeDatabaseFactory::make(params);
			if(!cmbEncoding->currentText().isEmpty())
				charSet=cmbEncoding->currentText().latin1();
			status = tdb->newDatabase(dbName, userName, password, hostName, portNumber,true,charSet);
			
			if(status)
				status = tdb->connect(hostName, userName, password, dbName, portNumber);
   
		}
		else if (dbType == "Firebird")
		{
  
			int pos = dbName.find(".fdb");
			
			if(pos < 0)
			{
				pos = dbName.find(".FDB");
				if(pos < 0)
				{
					dbName += ".fdb";
				}
			}
			
			QString qDir(dirName.c_str());
			QDir dir(qDir);
			QFileInfo info(qDir, dbName.c_str());
			std::string fullName = info.absFilePath().latin1();

			if(hostName.empty())
				hostName = "localhost";

			TeDatabaseFactoryParams params;
			params.dbms_name_ = "Firebird";
			tdb = TeDatabaseFactory::make(params);
			status = tdb->newDatabase(fullName, userName, password, hostName, portNumber);
   
		}
		
		if (status == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"), tdb->errorMessage().c_str());
			return;
		}
		else
		{
			TeQtDatabaseItem *dbItem = new TeQtDatabaseItem(databasesListView_, dbName.c_str(), tdb);
			databasesListView_->selectItem(dbItem);
			databasesListView_->show();
			// try to create a default color scheme table
			map<string,string> cramps;
			QString defaultColorRampName =  qApp->applicationDirPath() + "/tv_color_ramps.txt" ;
			if (TeReadColorRampTextFile(defaultColorRampName.latin1(), cramps))
			{
				TeAttributeList atl;
				TeAttribute		at;

				at.rep_.name_ = "name";
				at.rep_.type_ = TeSTRING;
				at.rep_.numChar_ = 150;
				at.rep_.isPrimaryKey_ = true;
				atl.push_back(at);

				at.rep_.isPrimaryKey_ = false;
				at.rep_.name_ = "colorbar";
				at.rep_.numChar_ = 255;
				atl.push_back(at);

				if(tdb->createTable("tv_colorbar", atl))
				{
					string sql;
					map<string,string>::const_iterator it = cramps.begin();
					while (it != cramps.end())
					{
						sql = "INSERT INTO tv_colorbar VALUES ('" + it->first + "', '" + it->second + "')";
						tdb->execute(sql);
						++it;
					}
				}
			}
			createAppThemeTable(tdb,integrity);
			createChartParamsTable(tdb,integrity);
		}

		TeProject project;
		project.setName("TV_Project");
		project.setDescription("TerraView_Default_Project");
		project.setCurrentViewId(-1);
		tdb->insertProject(&project);
		tdb->projectMap().clear(); // why I need ?
		if(!tdb->loadProjectSet())
		{
			QMessageBox::critical(this, tr("Error"), tr("Fail to load project!")); 
			return;
		}
	}
	else 
	{
		if(dirName.empty())
		{
			dirName = dbName;
		}

        std::string auxDbName = (dbType.compare("Access") == 0 || dbType.compare("Firebird") == 0) ? dirName : dbName;
		if(dbType.compare("Oracle") == 0 ) auxDbName=hostName;

        tdb = TeDBConnectionsPool::instance().getDatabase(dbType, auxDbName, hostName, userName, 
                                                          password, portNumber);

        status = (tdb->isConnected()) ? true :
                                        tdb->connect(tdb->host(), tdb->user(), tdb->password(), 
                                            tdb->databaseName(), tdb->portNumber());

        if(dbType.compare("OracleSpatial") == 0 || dbType.compare("OracleAdo") == 0)
        {
            integrity = true;
        }

        if (status == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"),
			tdb->errorMessage().c_str());
			return;
		}

		if(dbType == "OracleSpatial" || dbType == "OracleOCI" || dbType == "OracleAdo")
		{
			dbName = hostName;
		}

		//verify if this dabatase must be updated
		string DBver;
		if(needUpdateDB(tdb, DBver))
		{
			wait.resetWaitCursor();
			string dbVersion = TeDBVERSION;

			if(isLowerVersion(dbVersion, DBver))
			{
				tdb->close();
				delete tdb;

				QMessageBox::warning(this, tr("Warning"), tr("Cannot connect to database because the version of Terraview is lower than the version of the database!"));
				return;
			}

			QString msg = tr("The database must be converted to the model ") + dbVersion.c_str() + "! \n";

			if(tdb->dbmsName() == "PostgreSQL" && (DBver.empty() || DBver=="3.0.1" || DBver=="3.0" || DBver=="3.0.2" || DBver=="3.1.0"))			
			{
				msg += tr("This TerraLib and TerraView version (only for PostgreSQL) will update the geometry tables (vector and raster) to use R-Tree GiST. ") + "\n";
				msg += tr("This process may take a while. Be sure to make a backup before going on with this conversion.") + "\n";
			}
			else if(tdb->dbmsName() == "PostGIS" && (DBver.empty() || DBver=="3.0.1" || DBver=="3.0" || DBver=="3.0.2" || DBver=="3.1.0"))
			{
				msg += tr("This TerraLib and TerraView version (only for PostGIS) will update the geometry tables for raster type to use R-Tree GiST. ") + "\n";
				msg += tr("This process may take a while. Be sure to make a backup before going on with this conversion.") + "\n";
			}
			else
			{
				 if(tdb->dbmsName() != "Access" && tdb->dbmsName() != "Ado")
				 {
					msg += tr("Be sure to make a backup before going on with this conversion.") + "\n";
				}
			}
			
			msg += tr("Do you want to convert it?");
			int response = QMessageBox::question(this, tr("Database Conversion"),
				msg, tr("Yes"), tr("No"));
			if(response == 0)
			{
				if(tdb->dbmsName() == "Access" || tdb->dbmsName() == "Ado")
				{
					QString msgAccess = tr("Would you like to create a backup?");
					int resp = QMessageBox::question(this, tr("Database Conversion"), msgAccess, tr("Yes"), tr("No"));
					if(resp == 0)
					{
						//close connection
						tdb->close();
						delete tdb;

						//create a copy of a database with a new name added by the database version
#ifdef WIN32
							std::string fullName = TeGetName(auxDbName.c_str());
							std::string extName = TeGetExtension(auxDbName.c_str());
							std::string oldVersion = TeRemoveSpecialChars(DBver);
							std::string newName = fullName + "_" + oldVersion + "." + extName;

							std::string cmd = "copy \"" + auxDbName + "\" \"" + newName + "\"";

							//fix string to execute the command operation
							string::iterator it;
							for(it=cmd.begin();it!=cmd.end();it++)
							{
								if(*it == '/')
									*it = '\\';
							}
							

							int res = system(cmd.c_str());

							if(res == 0)
							{
								QString m = tr("Backup generated. File created: ") +  newName.c_str();

								QMessageBox::information(this, tr("Information"), m);
							}
							else
							{
								QMessageBox::warning(this, tr("Warning"), tr("Error creating backup file."));

								return;
							}
#endif

						//restart connection
						tdb = TeDBConnectionsPool::instance().getDatabase(dbType, auxDbName, hostName, userName, 
																  password, portNumber);

						status = (tdb->isConnected()) ? true :
												tdb->connect(tdb->host(), tdb->user(), tdb->password(), 
													tdb->databaseName(), tdb->portNumber());
					}
				}

				string error; 
				wait.setWaitCursor();
				if(!updateDBVersion(tdb, DBver, error))
				{
					wait.resetWaitCursor();
					QString msg = tr("Fail to convert the database to the new model!") + "\n";
					msg += error.c_str();
					QMessageBox::critical(this, tr("Error"), msg);
					return;		
				}
				else
				{
					wait.resetWaitCursor();
					QString msg = tr("Database was converted successfully!") + "\n";
					msg += error.c_str();
					QMessageBox::information(this, tr("Information"), msg);
				}
			}
			else
				return;
		}

		if(!isTerralibModel(tdb))
		{
			wait.resetWaitCursor();
			QString msg;
			msg += tr("This database do not contains the tables of TerraLib Conceptual Model. \n");
			msg += tr("If you connect, these tables will be created. Do you want continue?");
			if(QMessageBox::question(this, tr("Database Connection"), msg, tr("Yes"), tr("No")) == 1)
			{
				tdb->close();
				delete tdb;
				return;
			}
			wait.setWaitCursor();
		}

		//create conceptual model 
		tdb->createConceptualModel(integrity, false);
		if (!tdb->tableExist("te_theme_application"))
			createAppThemeTable(tdb,integrity);
		if (!tdb->tableExist("te_chart_params"))
			createChartParamsTable(tdb,integrity);

		// alter grouping_color field of the te_theme_application table to 255 characters
		TeAttributeList appAttList;
		if(tdb->getAttributeList("te_theme_application", appAttList))
		{
			TeAttribute at;
			at.rep_.type_ = TeSTRING;
			at.rep_.numChar_ = 255;
			at.rep_.name_ = "grouping_color";

			int i;
			for(i=0; i<(int)appAttList.size(); ++i)
			{
				if(appAttList[i].rep_.name_ == "grouping_color")
				{
					if(appAttList[i].rep_.numChar_ != 255)
						tdb->alterTable("te_theme_application", at.rep_, "");
					break;
				}
			}
		}
	
		//populate the te_theme_application when the theme wasn�t generated by TerraView
		if(!populateAppThemeTable(tdb))
			return;

		wait.setWaitCursor();		
		//////////////////////////////////////////////////////
		// Check the consistency of the tables in the database
		//////////////////////////////////////////////////////
		if (dbConsistencyCheckBox->isOn() == true)
		{
			// Get layer tables	that doesn�t exist in the database
			set<int> nonExistentTablesSet;
			set<int> relationIdSet;
			set<int>::iterator setIt;
			TeDatabasePortal *pt = tdb->getPortal();
			string tableName;
			int tableId;

			if (pt->query("SELECT * FROM te_layer_table") == false)
			{	
				delete pt;
				return;
			}

			while (pt->fetchRow())
			{
				tableName = pt->getData("attr_table");
				tableId = atoi(pt->getData("table_id"));
				if (tdb->tableExist(tableName) == false)
					nonExistentTablesSet.insert(tableId);	
			}

			// Check the te_theme_table whether there are themes using tables that doesn�t exist
			for (setIt = nonExistentTablesSet.begin(); setIt != nonExistentTablesSet.end(); ++setIt)
			{
				tableId = *setIt;
				pt->freeResult();
				string q = "SELECT relation_id FROM te_theme_table WHERE table_id = " + Te2String(tableId);
				q += " AND relation_id IS NOT NULL";
				if (pt->query(q) == false)
				{	
					delete pt;
					return;
				}
				while (pt->fetchRow())
					relationIdSet.insert(atoi(pt->getData(0)));
			}

			delete pt;

			// Delete the entries in the te_tables_relation corresponding 
			// to the table that doesn�t exist in the database
			for (setIt = relationIdSet.begin(); setIt != relationIdSet.end(); ++setIt)
				tdb->execute("DELETE FROM te_tables_relation WHERE relation_id = " + Te2String(*setIt));

			for (setIt = nonExistentTablesSet.begin(); setIt != nonExistentTablesSet.end(); ++setIt)
			{
				// Delete the entries in the te_layer_table and te_theme_table corresponding 
				// to the table that doesn�t exist in the database
				tdb->execute("DELETE FROM te_theme_table WHERE table_id = " + Te2String(*setIt));
				tdb->execute("DELETE FROM te_layer_table WHERE table_id = " + Te2String(*setIt));
			}
		}
		///////End of the check of consistency of the tables in the database /////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////
		
		// Load the database connections 
		//TeDBConnectionsPool::instance().clear();
        TeDBConnectionsPool::instance().loadExternalDBConnections(tdb);
		
		// Get datums from the database
		if (!tdb->loadDatumSet())
		{
			wait.resetWaitCursor();
			QString msg = tr("Fail to get the datums from the database!") + "\n";
			msg += tdb->errorMessage().c_str();
			QMessageBox::critical(this, tr("Error"), msg);
			return;		
		}

		// Get layers from the database
		if (!tdb->loadLayerSet())
		{
			wait.resetWaitCursor();
			QString msg = tr("Fail to get the layers from the database!") + "\n";
			msg += tdb->errorMessage().c_str();
			QMessageBox::critical(this, tr("Error"), msg);
			return;		
		}

		if (!tdb->loadViewSet(userLineEdit->text().latin1()))
		{
			wait.resetWaitCursor();
			QString msg = tr("Fail to get the views from the database!") + "\n";
			msg += tdb->errorMessage().c_str();
			QMessageBox::critical(this, tr("Error"), msg);
			return;
		}

		tdb->projectMap().clear(); // why I need ?
		if(!tdb->loadProjectSet())
		{
			QMessageBox::critical(this, tr("Error"), tr("Fail to load project!")); 
			return;
		}

		// Verify if text table has visual
		vector<string> txVec;
		TeDatabasePortal* portal = tdb->getPortal();
		if(portal->query("SELECT geom_table FROM te_representation WHERE geom_type = 128"))
		{
			while(portal->fetchRow())
			{
				string tx = portal->getData(0);
				string tvis = tx + "_txvisual";
				if(tdb->tableExist(tvis) == false)
					txVec.push_back(tx);
			}
		}
		delete portal;

		int n;
		for(n = 0; n < (int)txVec.size(); ++n)
		{
			string tx = txVec[n];
			string tvis = tx + "_txvisual";
			if(tdb->tableExist(tvis) == false)
			{
				TeAttributeList atl;
				TeAttribute at;

				at.rep_.name_ = "geom_id";
				at.rep_.type_ = TeINT;
				at.rep_.numChar_ = 0;
				at.rep_.isPrimaryKey_ = true;
				atl.push_back(at);

				at.rep_.isPrimaryKey_ = false;
				at.rep_.name_ = "dot_height";
				at.rep_.type_ = TeINT;
				atl.push_back(at);

				at.rep_.name_ = "fix_size";
				at.rep_.type_ = TeINT;
				atl.push_back(at);

				at.rep_.name_ = "color";
				at.rep_.type_ = TeINT;
				atl.push_back(at);

				at.rep_.name_ = "family";
				at.rep_.type_ = TeSTRING;
				at.rep_.numChar_ = 128;
				atl.push_back(at);

				at.rep_.name_ = "bold";
				at.rep_.type_ = TeINT;
				at.rep_.numChar_ = 0;
				atl.push_back(at);

				at.rep_.name_ = "italic";
				at.rep_.type_ = TeINT;
				atl.push_back(at);

				tdb->createTable(tvis, atl);
				string fk = "fk_" + tvis;
				tdb->createRelation(fk, tvis, "geom_id", tx, "geom_id", true);
				string ins = "INSERT INTO " + tvis + " (geom_id) SELECT geom_id FROM " + tx;
				tdb->execute(ins);

				string popule = "UPDATE " + tvis;
				popule += " SET dot_height = 12";
				popule += ", fix_size = 0";
				popule += ", color = 0";
				popule += ", family = 'Verdana'";
				popule += ", bold = 0";
				popule += ", italic = 0";
				if(tdb->execute(popule) == false)
				{
					wait.resetWaitCursor();
					QMessageBox::critical(this, tr("Error"),
						tr("Fail to generate the visual table of the text!"));
					return;
				}
			}
		}

		TeQtDatabaseItem *dbItem = new TeQtDatabaseItem(databasesListView_, dbName.c_str(), tdb);

		// Insert layers into the tree
		TeLayerMap& layerMap = tdb->layerMap();
		TeLayerMap::iterator itlay = layerMap.begin();
		TeQtLayerItem *layerItem;
		while ( itlay != layerMap.end() )
		{
			TeLayer *layer = (*itlay).second;
			string name = layer->name();
			layerItem = new TeQtLayerItem(dbItem, name.c_str(), layer);
			if (databasesListView_->isOpen(dbItem) == false)
				databasesListView_->setOpen(dbItem,true);
			itlay++;
		}

		mainWindow_->getDatabasesListView()->selectItem(dbItem);
		databasesListView_->show();
	}

	setLastConnection();
	hide();
}


void DatabaseWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("database.htm");
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



void DatabaseWindow::setLastConnection()
{
	QSettings settings;
	
	settings.beginGroup("TerraView");
	settings.beginGroup("LastConnection");
	
	settings.writeEntry("dbType", databaseTypeComboBox->currentText());
	settings.writeEntry("dbHost", hostLineEdit->text());
	settings.writeEntry("dbPort", portLineEdit->text());
	settings.writeEntry("dbUser", userLineEdit->text());
	settings.writeEntry("dbDirectory", dbComboBox->currentText());
	settings.writeEntry("dbName", databaseNameLineEdit->text());
	
	settings.endGroup();
	settings.endGroup();
	
}


void DatabaseWindow::getLastConnection()
{
	QSettings settings;
	settings.beginGroup("TerraView");
	settings.beginGroup("LastConnection");
	
	if(!settings.readEntry("dbType").isEmpty())
	{
		databaseTypeComboBox->setCurrentText(settings.readEntry("dbType"));
		databaseTypeComboBox_activated("");
	}
		
	hostLineEdit->setText(settings.readEntry("dbHost"));
	portLineEdit->setText(settings.readEntry("dbPort"));
	userLineEdit->setText(settings.readEntry("dbUser"));

	if(dbComboBox->count() > 0)	
		dbComboBox->setCurrentText(settings.readEntry("dbDirectory"));
	else
		dbComboBox->insertItem(settings.readEntry("dbDirectory"));

	databaseNameLineEdit->setText(settings.readEntry("dbName"));
		
	settings.endGroup();
	settings.endGroup();

	passwordLineEdit->setFocus();
}
