/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <DatabaseDialog.h>
#include <qdir.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qradiobutton.h>

#include <TeDatabase.h>
#include <TeDatabaseUtils.h>
#include <TeDatabaseFactory.h>
#include <TeWaitCursor.h>
#include <TeDBConnectionsPool.h>
#include <TeUpdateDBVersion.h>

void DatabaseDialog::reset()
{
#ifdef WIN32
	databaseTypeComboBox->setCurrentText("Access");
	databaseTypeComboBox_activated("Access");
#else
	databaseTypeComboBox->setCurrentText("MySQL");
	databaseTypeComboBox_activated("MySQL");
#endif

	dbComboBox->setEnabled(false);
	connectRadioButton_clicked(); // connect radioButton is initially on
}

void DatabaseDialog::connectRadioButton_clicked()
{

}


void DatabaseDialog::createRadioButton_clicked()
{

}


void DatabaseDialog::databaseTypeComboBox_activated( const QString & )
{
	if (connectRadioButton->isOn() == true)
	{
		operationButtonGroup_clicked(0);
	}
	else
	{
		operationButtonGroup_clicked(1);
	}
}


void DatabaseDialog::directoryPushButton_clicked()
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
			
			QDir dir(QDir::currentDirPath());
			QFileInfo fInfo(dir, "application.cfg");
	
			QString fAbsPath = fInfo.absFilePath();
			// Get the connection string
			QFile cfgFile(fAbsPath);

			if (cfgFile.open(IO_ReadOnly))
			{
				QString qConnString;
				cfgFile.readLine(qConnString, 512);
				if (qConnString.isEmpty() == false)
				{
					connString = qConnString.latin1();

					cfgFile.close();

					// Get the database name in the connection string
					dbName = connString.substr(connString.rfind('/') + 1);

					// Get the full database path
					dbFullPath = connString;
					string::size_type pos = dbFullPath.rfind('=');
					dbFullPath = dbFullPath.substr(pos + 1);
				}
			}

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

			// Get the connection string
			QFile cfgFile = QFile(QDir::currentDirPath() + "/application.cfg");
			if (cfgFile.open(IO_ReadOnly))
			{
				QString qConnString;
				cfgFile.readLine(qConnString, 512);
				if (qConnString.isEmpty() == false)
				{
					connString = qConnString.latin1();

					cfgFile.close();

					// Get the database name in the connection string
					dbName = connString.substr(connString.rfind('/') + 1);

					// Get the full database path
					dbFullPath = connString;
					string::size_type pos = dbFullPath.rfind('=');
					dbFullPath = dbFullPath.substr(pos + 1);
				}
			}

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
				dbComboBox->insertItem(dirName);
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

			TeDatabaseFactoryParams params;			

			if(databaseTypeComboBox->currentText() == "MySQL")
				params.dbms_name_ = "MySQL";
			else if(databaseTypeComboBox->currentText() == "PostgreSQL")
				params.dbms_name_ = "PostgreSQL";
			else if(databaseTypeComboBox->currentText() == "PostGIS")
				params.dbms_name_ = "PostGIS";

			TeDatabase *db = TeDatabaseFactory::make(params);

			/*if(databaseTypeComboBox->currentText() == "MySQL")
				db = new TeMySQL();
			else if(databaseTypeComboBox->currentText() == "PostgreSQL")
				db = new TePostgreSQL();
			else if(databaseTypeComboBox->currentText() == "PostGIS")
				db = new TePostGIS();
			*/
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


void DatabaseDialog::getEncodingListPushButton_clicked()
{
	encodingComboBox->clear();

	if (createRadioButton->isOn())
	{
		if (databaseTypeComboBox->currentText() == "MySQL" || databaseTypeComboBox->currentText() == "PostgreSQL" || databaseTypeComboBox->currentText() == "PostGIS")
		{
			vector<string> dbNames;

			TeDatabaseFactoryParams params;			

			if(databaseTypeComboBox->currentText() == "MySQL")
				params.dbms_name_ = "MySQL";
			else if(databaseTypeComboBox->currentText() == "PostgreSQL")
				params.dbms_name_ = "PostgreSQL";
			else if(databaseTypeComboBox->currentText() == "PostGIS")
				params.dbms_name_ = "PostGIS";

			TeDatabase *db = TeDatabaseFactory::make(params);
			
			/*if(databaseTypeComboBox->currentText() == "MySQL")
			{
#ifdef HAS_MYSQL
				db = new TeMySQL();
#endif
			}
			else if(databaseTypeComboBox->currentText() == "PostgreSQL")
			{
#ifdef HAS_POSTGRESQL
				db = new TePostgreSQL();
#endif
			}
			else if(databaseTypeComboBox->currentText() == "PostGIS")
			{
#ifdef HAS_POSTGRESQL
				db = new TePostGIS();
#endif
			}*/

			if(db == NULL)
			{
				QMessageBox::warning(this, tr("Warning"), tr("Unable to init database. Maybe its was not compilied with PostgreSQL ou MySQL support!"));
				return;
			}

			int port = -1;
			if(portLineEdit->text() != "")
			{
				bool ok = false;
				port = portLineEdit->text().toInt(&ok);

				if(!ok)
				{
					QMessageBox::warning(this, tr("Warning"), tr("Port must be a number!"));
					return;
				}
			}
			
			std::vector<std::string> vecList;
			if (db->getEncodingList(hostLineEdit->text().latin1(), userLineEdit->text().latin1(), passwordLineEdit->text().latin1(), port, vecList) == false)
			{
				QMessageBox::critical(this, tr("Error"), db->errorMessage().c_str());
				delete db;
				return;
			}

			delete db;

			encodingComboBox->insertItem("");
			for(unsigned int i = 0; i < vecList.size(); ++i)
			{
				encodingComboBox->insertItem(vecList[i].c_str());
			}
			if(encodingComboBox->count() > 1)
			{
				encodingComboBox->setCurrentItem(1);
			}

			//TaInterfaceUtils::sortComboBox(encodingComboBox, "");
		}
	}
}


void DatabaseDialog::init()
{
	#ifdef WIN32
	databaseTypeComboBox->insertItem("Access");
	databaseTypeComboBox->insertItem("SQL Server");
	databaseTypeComboBox->insertItem("Oracle"); 
#endif

	databaseTypeComboBox->insertItem("OracleOCI"); 
	databaseTypeComboBox->insertItem("OracleSpatial");
	
	databaseTypeComboBox->insertItem("MySQL");

	databaseTypeComboBox->insertItem("PostgreSQL");
	databaseTypeComboBox->insertItem("PostGIS");
	databaseTypeComboBox->insertItem("Firebird");
	
#ifdef WIN32
	databaseTypeComboBox->setCurrentText("Access");
	databaseTypeComboBox_activated("Access");
#else
	databaseTypeComboBox->setCurrentText("MySQL");
	databaseTypeComboBox_activated("MySQL");
#endif

	databaseNameLineEdit->setMaxLength(200); //previne nomes de banco grandes demais 

	dbComboBox->setEnabled(false);
	connectRadioButton_clicked(); // connect radioButton is initially on
}

void DatabaseDialog::init(const bool& allowCreation, const bool& allowConnection)
{
	createRadioButton->setChecked(false);
	connectRadioButton->setChecked(false);
	createRadioButton->setDisabled(!allowCreation);
	connectRadioButton->setDisabled(!allowConnection);
	if(allowCreation == true)
	{
		createRadioButton->setChecked(true);
		operationButtonGroup_clicked(1);
	}
	if(allowConnection == true)
	{
		connectRadioButton->setChecked(true);
		operationButtonGroup_clicked(0);
	}
}


void DatabaseDialog::destroy()
{

}


void DatabaseDialog::operationButtonGroup_clicked( int buttonId)
{
	hostTextLabel->setText(tr("Host:"));
	if (databaseTypeComboBox->currentText() == "Access")
	{
		hostLineEdit->setText("");
		portLineEdit->setText("");
		userLineEdit->setText("");
		passwordLineEdit->setText("");
		hostLineEdit->setEnabled(false);
		portLineEdit->setEnabled(false);
		userLineEdit->setEnabled(false);
		passwordLineEdit->setEnabled(false);
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
		if(databaseTypeComboBox->currentText() == "SQL Server")
			portLineEdit->setEnabled(false);
		else
			portLineEdit->setEnabled(true);

		dbComboBox->clear();
		if(databaseTypeComboBox->currentText() == "MySQL" || databaseTypeComboBox->currentText() == "PostgreSQL" || databaseTypeComboBox->currentText() == "PostGIS")
		{
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
			}
		}
		else
		{
			directoryPushButton->setEnabled(false);
			databaseNameLineEdit->setText("");
			databaseNameLineEdit->setEnabled(true);
		}
	}

	bool create = false;
	if(buttonId != 0)
	{
		getEncodingListPushButton->setEnabled(create);
		create = true;
	}

	encodingComboBox->clear();
	encodingComboBox->setEnabled(create);
	getEncodingListPushButton->setEnabled(create);
}


void DatabaseDialog::dbComboBox_activated( const QString & dbName)
{
	databaseNameLineEdit->setText(dbName);	
}


bool DatabaseDialog::isEqualTo(TeDatabase* database)
{
	if(database == 0)
	{
		return false;
	}

	std::string userName = "";
	std::string hostName = "";
	std::string dbName = "";
	std::string dirName = "";
	std::string dbmsName = "";

	std::string dbType = databaseTypeComboBox->currentText().latin1();
	dbmsName = dbType;

	if(!userLineEdit->text().isEmpty())
	{
		userName = userLineEdit->text().latin1();
	}
	if(!hostLineEdit->text().isEmpty())
	{
		hostName = hostLineEdit->text().latin1();
	}
	if(!databaseNameLineEdit->text().isEmpty())
	{
		dbName = databaseNameLineEdit->text().latin1();
	}	
	if (dbComboBox->count() != 0)
	{
		dirName  = dbComboBox->currentText().latin1();
	}

	if(dirName.empty())
	{
		dirName = dbName;
	}

	if (dbType == "Access")
	{
		dbmsName = "Ado";

		std::string::size_type k = dbName.find(".accdb", 0, 4);
		if (k == string::npos)
		{
			k = dbName.find(".mdb", 0, 4);
			if (k == string::npos)
				dbName += ".mdb";
		}
		dbName = dirName + "\\" + dbName;
	}
	else if(dbmsName == "SQL Server")
    {
        dbmsName = "SqlServerAdo";
    }else if(dbmsName == "SQL Server Spatial")
	{
		dbmsName="SqlServerAdoSpatial";
	}
	else if(dbmsName == "Oracle")
	{
		dbmsName = "OracleAdo";
	}
	else if(dbmsName == "OracleOCI")
	{
		dbmsName = "OracleOCI";
	}
    else 
	{
		/*if(hostName.empty())
		{
			hostName = "localhost";
		}*/
	}

    std::string auxDbName = (dbType.compare("Ado") == 0 || dbType.compare("Firebird") == 0) ? dirName : dbName;
	if(dbType.compare("Oracle") == 0 ) 
	{
		auxDbName = hostName;
	}

	if(database->user() != userName)
	{
		return false;
	}
	if(database->host() != hostName)
	{
		return false;
	}
	if(database->databaseName() != auxDbName)
	{
		return false;
	}

	return true;
}


TeDatabase* DatabaseDialog::getConnection(const bool& loadLayerSet, const bool& loadViewSet)
{
	TeWaitCursor wait;

	TeDatabase* tdb = 0;
	bool status = true;
	bool integrity = true;

	string hostName = hostLineEdit->text().latin1();
	string userName = userLineEdit->text().latin1();
	string password = passwordLineEdit->text().latin1();
	string dirName = "";
    std::string encoding = "";
	
	
	if (encodingComboBox->count() != 0)
	{
		encoding = encodingComboBox->currentText().latin1();
	}

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
#ifdef WIN32
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
			/*
			mainWindow_->origDecimalSepIsPoint(false);
			*/
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
#endif
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
		return NULL;
	}

	if (dbType != "Access" && userName.empty())
	{
		wait.resetWaitCursor();
		QMessageBox::warning(this, tr("Warning"), tr("Enter the user name!"));
		return NULL;
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
		return NULL;
	}

// Creation or connection to the several types of databases
	if (createRadioButton->isOn())
	{
		bool callNewDatabaseFunction = true; //temporary. will be removed when connection option be enabled

		std::string dbmsName = dbType;
		if (dbType == "Access")
		{
			dbmsName = "Ado";

			std::string::size_type k = dbName.find(".accdb", 0, 4);
			if (k == string::npos)
			{
				k = dbName.find(".mdb", 0, 4);
				if (k == string::npos)
					dbName += ".mdb";
			}
			dbName = dirName + "\\" + dbName;
		}
		else if(dbType == "SQL Server")
		{
			wait.resetWaitCursor();
			QString mess = tr("A new SQLServer database can not be created !");
			mess += "\n" + tr(" Only a DBA can create a new database. TerraView only can");
			mess += "\n" + tr(" connect to previously created database. Make sure its an empty database in order to copy data to it!");
			QMessageBox::warning(this, tr("Warning"),mess);
			wait.resetWaitCursor();

			dbmsName = "SqlServerAdo";
			//callNewDatabaseFunction = false;
		}
		else if (dbType == "OracleSpatial" || dbType == "Oracle" || dbType == "OracleOCI")
		{
			wait.resetWaitCursor();
			QString mess = tr("A new Oracle database can not be created !");
			mess += "\n" + tr(" Only a DBA can create a new database. TerraView only can");
			mess += "\n" + tr(" connect to previously created database. Make sure its an empty database in order to copy data to it!");
			QMessageBox::warning(this, tr("Warning"),mess);
			wait.resetWaitCursor();
			/*wait.resetWaitCursor();
			QString mess = tr("A new Oracle database can not be created !");
			mess += "\n" + tr(" Only a DBA can create a new database. TerraView only can");
			mess += "\n" + tr(" connect to previously created database. ");
			QMessageBox::warning(this, tr("Warning"),mess);
			return NULL;*/
			if(dbType == "Oracle")
			{
				dbmsName = "OracleAdo";
			}
			if(dbType == "OracleOCI" || dbType == "OracleSpatial") 
			{
				dbName = hostName;
			}
			callNewDatabaseFunction = false;
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
			dbName = info.absFilePath().latin1();

			if(hostName.empty())
				hostName = "localhost";
		}
		
		/*
		// operation is set to create a database
		if (dbType == "Access")
		{
#ifdef WIN32
			tdb = new TeAdo();

			std::string::size_type k = dbName.find(".accdb", 0, 4);
			if (k == string::npos)
			{
				k = dbName.find(".mdb", 0, 4);
				if (k == string::npos)
					dbName += ".mdb";
			}
			string fullName = dirName + "\\" + dbName;
			status =  tdb->newDatabase(fullName,userName,password,hostName, portNumber);
			//if (status)
			//{
			//	status = tdb->connect(fullName);
			//	// save the configuration file in the current directory
			//	char myConnectString[512] = {0};
			//	char myCfgFile[512] = {0};
			//	FILE* myCfg;
			//	TeAdo *ado = (TeAdo*)tdb;
			//	_bstr_t	bstr_myConnectString = ado->connectionString().c_str();
			//	_getcwd(myCfgFile, 512 );
			//	strcat (myCfgFile,"\\application.cfg");
			//	myCfg = fopen(myCfgFile,"w");
			//	if (myCfg)
			//	{
			//		strcpy (myConnectString,(char*) bstr_myConnectString);
			//		fwrite (myConnectString , 1, strlen(myConnectString), myCfg  );
			//		fclose (myCfg);
			//	}
			//}
#endif
		}
        else if(dbType == "SQL Server")
		{
#ifdef WIN32
			tdb = new TeSqlServer();
			status = tdb->connect(hostName,userName,password,dbName, portNumber);
#endif
		}
		else if (dbType == "MySQL")
		{
			tdb = new TeMySQL();
			status =  tdb->newDatabase(dbName,userName,password,hostName, portNumber);
			if (status)
				status = tdb->connect(hostName,userName,password,dbName, portNumber);
		}
		else if (dbType == "PostgreSQL")
		{

			tdb = new TePostgreSQL();

			status = tdb->newDatabase(dbName, userName, password, hostName, portNumber);
			
			if(status)
				status = tdb->connect(hostName, userName, password, dbName, portNumber);
		}
		else if (dbType == "PostGIS")
		{

			tdb = new TePostGIS();

			status = tdb->newDatabase(dbName, userName, password, hostName, portNumber);
			
			if(status)
				status = tdb->connect(hostName, userName, password, dbName, portNumber);
		}
		else if (dbType == "OracleSpatial" || dbType == "Oracle")
		{
			wait.resetWaitCursor();
			QString mess = tr("TerraView can not create a new Oracle database!");
			mess += "\n" + tr(" Only a DBA can create a new database. TerraView only can");
			mess += "\n" + tr(" connect to previously created database. ");
			QMessageBox::warning(this, tr("Warning"),mess);
			return false;
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

			tdb = new TeFirebird();
			status = tdb->newDatabase(fullName, userName, password, hostName, portNumber);
		}*/

		TeDatabaseFactoryParams params;
		params.dbms_name_ = dbmsName;
		tdb = TeDatabaseFactory::make(params);

		if(tdb != NULL)
		{
			if(callNewDatabaseFunction == true)
			{
				status = tdb->newDatabase(dbName, userName, password, hostName, portNumber, true, encoding);
			}
			if(status == true && !tdb->isConnected())
			{
				status = tdb->connect(dbName, userName, password, hostName, portNumber);
			}
		}		
		
		if (status == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"), tdb->errorMessage().c_str());
			return NULL;
		}
		else
		{
			tdb->createConceptualModel(integrity, false);


			/*TeQtDatabaseItem *dbItem = new TeQtDatabaseItem(databasesListView_, dbName.c_str(), tdb);
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
			*/
			/*
			createAppThemeTable(tdb,integrity);
			createChartParamsTable(tdb,integrity);
			*/
		}

		/*TeProject project;
		project.setName("TV_Project");
		project.setDescription("TerraView_Default_Project");
		project.setCurrentViewId(-1);
		tdb->insertProject(&project);
		tdb->projectMap().clear(); // why I need ?
		if(!tdb->loadProjectSet())
		{
			QMessageBox::critical(this, tr("Error"), tr("Fail to load project!")); 
			return;
		}*/
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
			return NULL;
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
				return NULL;
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

			msg += tr("Do you want to convert it?");
			int response = QMessageBox::question(this, tr("Database Conversion"),
				msg, tr("Yes"), tr("No"));
			if(response == 0)
			{
				string error; 
				wait.setWaitCursor();
				if(!updateDBVersion(tdb, DBver, error))
				{
					wait.resetWaitCursor();
					QString msg = tr("Fail to convert the database to the new model!") + "\n";
					msg += error.c_str();
					QMessageBox::critical(this, tr("Error"), msg);
					return NULL;
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
				return NULL;
		}

		if(!isTerralibModel(tdb))
		{
			QString msg;
			msg += tr("This database do not contains the tables of TerraLib Conceptual Model. \n");
			msg += tr("If you connect, these tables will be created. Do you want continue?");
			if(QMessageBox::question(this, tr("Database Connection"), msg, tr("Yes"), tr("No")) == 1)
			{
				tdb->close();
				delete tdb;
				return 0;
			}
		}

		//create conceptual model 
		tdb->createConceptualModel(integrity, false);
		/*if (!tdb->tableExist("te_theme_application"))
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
		}*/
	
		//populate the te_theme_application when the theme wasn't generated by TerraView
		/*
		if(!populateAppThemeTable(tdb))
			return;
		*/

		wait.setWaitCursor();		
		//////////////////////////////////////////////////////
		// Check the consistency of the tables in the database
		//////////////////////////////////////////////////////
		/*if (dbConsistencyCheckBox->isOn() == true)
		{
			// Get layer tables	that doesn't exist in the database
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

			// Check the te_theme_table whether there are themes using tables that doesn't exist
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
			// to the table that doesn't exist in the database
			for (setIt = relationIdSet.begin(); setIt != relationIdSet.end(); ++setIt)
				tdb->execute("DELETE FROM te_tables_relation WHERE relation_id = " + Te2String(*setIt));

			for (setIt = nonExistentTablesSet.begin(); setIt != nonExistentTablesSet.end(); ++setIt)
			{
				// Delete the entries in the te_layer_table and te_theme_table corresponding 
				// to the table that doesn't exist in the database
				tdb->execute("DELETE FROM te_theme_table WHERE table_id = " + Te2String(*setIt));
				tdb->execute("DELETE FROM te_layer_table WHERE table_id = " + Te2String(*setIt));
			}
		}*/
		///////End of the check of consistency of the tables in the database /////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////
		
		// Load the database connections 
		/*
		TeDBConnectionsPool::instance().clear();
        TeDBConnectionsPool::instance().loadExternalDBConnections(tdb);
		*/
		
		// Get layers from the database
		if(loadLayerSet == true)
		{
			if (!tdb->loadLayerSet())
			{
				wait.resetWaitCursor();
				QString msg = tr("Fail to get the layers from the database!") + "\n";
				msg += tdb->errorMessage().c_str();
				QMessageBox::critical(this, tr("Error"), msg);
				return NULL;
			}
		}

		if(loadViewSet == true)
		{
			if (!tdb->loadViewSet(userLineEdit->text().latin1()))
			{
				wait.resetWaitCursor();
				QString msg = tr("Fail to get the views from the database!") + "\n";
				msg += tdb->errorMessage().c_str();
				QMessageBox::critical(this, tr("Error"), msg);
				return NULL;
			}
		}

		/*tdb->projectMap().clear(); // why I need ?
		if(!tdb->loadProjectSet())
		{
			QMessageBox::critical(this, tr("Error"), tr("Fail to load project!")); 
			return;
		}*/

		// Verify if text table has visual
		/*vector<string> txVec;
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
		delete portal;*/

		/*int n;
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
		}*/

		/*TeQtDatabaseItem *dbItem = new TeQtDatabaseItem(databasesListView_, dbName.c_str(), tdb);

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
		databasesListView_->show();*/
	}


	return tdb;
}
