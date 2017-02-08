
#include <externalDBThemeWindow.h>

#include <TeDatabase.h>
#include <TeDBConnectionsPool.h>
#include <TeDatabaseFactoryParams.h>
#include <TeDatabaseFactory.h>
#include <TeExternalTheme.h>
#include <TeUpdateDBVersion.h>


#include <TeGUIUtils.h> 
#include <TeWaitCursor.h>

#include <qmessagebox.h>
#include <qbuttongroup.h> 
#include <qtabwidget.h>
#include <qtextbrowser.h> 
#include <qspinbox.h> 
#include <qlineedit.h>
#include <qdialog.h>
#include <qfiledialog.h>
#include <qcheckbox.h> 
#include <qwidgetstack.h> 
#include <qmenubar.h>
#include <qpushbutton.h>

#include <TeQtProgress.h>
#include <TeQtThemeItem.h>
#include <TeQtViewItem.h>
#include <TeQtLegendItem.h>
#include <TePlotTheme.h>
#include <TeQtViewsListView.h>
#include <TeQtCanvas.h>

//Qt include files
#include <qtable.h>
#include <qmainwindow.h>

//XPM include files
#include "externalTheme.xpm"

externalDBThemeWindow::externalDBThemeWindow(QWidget* parent) :
plugIngParams_(0),
	externalDBTheme(parent, ""),
	externalDatabaseId_(-1), 
	selectedDb_(false), 
	currentDatabase_(0),
	newDb_(0), 
	viewId_(0), 
	canvas_(0),
	help_(0)
{  
  /* Inserting the tool button into TerraView toolbar */
  
//  QPtrList< QToolBar > tv_tool_bars_list =
//    params->qtmain_widget_ptr_->toolBars( Qt::DockTop );
//
//  if( tv_tool_bars_list.count() > 0 )
//  {
//    pluginAction_ = new QAction(NULL, "plugin action");
////    plugin_action_ptr_.reset( new QAction( tv_tool_bars_list.at( 0 ) ) );
//
//    pluginAction_->setText( tr("External Theme") );
//    pluginAction_->setMenuText( tr("External Theme") );
//    pluginAction_->setToolTip( tr("External Theme") );
//    pluginAction_->setIconSet( QIconSet( QPixmap(externalTheme_xpm)));
//
//	pluginAction_->addTo(params->qtmain_widget_ptr_->toolBars(Qt::DockTop).at(0));
//
//    connect( pluginAction_, SIGNAL( activated() ), this, SLOT( show() ) );
//  }
//
//	QPopupMenu* themeMnu = findThemeMenu();
//
//	if(themeMnu == NULL)
//	{
//		return;
//	}
//
//	mnuIdxs_.push_back(themeMnu->insertSeparator());
//
//	mnuIdxs_.push_back(themeMnu->insertItem(tr("Add External Theme..."), this,
//		SLOT(show())));
}
	

void
externalDBThemeWindow::clearDBAndThemeVector()
{
	//clear database metadata cache
	if(newDb_)
	{
		//this database pointer (newDB_) is in the connection pool and
		//will be deleted when the pool is clear
		newDb_->clear();
		newDb_ = 0;
		selectedDb_ = false;
	}

	if(help_)
		delete help_;

	help_ = 0;
	themeVec_.clear();
}

externalDBThemeWindow::~externalDBThemeWindow()
{
	clearDBAndThemeVector();
}

void 
externalDBThemeWindow::updateForm(PluginParameters* params)
{
	plugIngParams_ = params;

	TeDatabase* db = plugIngParams_->getCurrentDatabasePtr();

	if(	db == NULL )
	{
		QMessageBox::critical(this, tr("Error"), tr("There is no database connected!"));
		return;
	}
	if( !plugIngParams_->getCurrentViewPtr() ||
		!plugIngParams_->teqtviewslistview_ptr_ || 
		!plugIngParams_->teqtviewslistview_ptr_->currentViewItem())
	{
		QMessageBox::warning(this, tr("Warning"), tr("There is not a current view!"));
		return;
	}

	init(db, plugIngParams_->getCurrentViewPtr()->id());
//	QMainWindow::show();
}

void 
externalDBThemeWindow::init(TeDatabase* currentDB, int viewId)
{
	currentDatabase_ = currentDB;
	newDb_ = 0;
	help_ = 0;
	viewId_ = viewId;
	selectedDb_ = false;
	
	previewFrame->setBackgroundColor(Qt::white);
	canvas_ = new TeQtCanvas(previewFrame);
	canvas_->resize(previewFrame->width(), previewFrame->height());
	
	themeVec_.clear();
	
	QHeader* vHeader = databaseTable->verticalHeader();
	connect(vHeader,SIGNAL(clicked(int)),this, SLOT(databaseClickedVHeader(int)));

	previewCheckBox->setChecked(true);

	//verify if there are specific tables to store connections and 
	//external theme and create theme
    TeDBConnectionsPool::createDBConnectionTable(currentDatabase_);
	TeExternalTheme::createExternalThemeTable(currentDatabase_);
	
	fillDatabaseTable();
	//init the interface with the page 0
	remoteWidgetStack->raiseWidget(0);
}


void externalDBThemeWindow::createPushButton_clicked()
{
	databaseTypeComboBox->clear();

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

	dbComboBox->setEnabled(false);

	remoteWidgetStack->raiseWidget(1);
}


void externalDBThemeWindow::nextOnePushButton_clicked()
{
	int col = databaseTable->currentRow();
	if(col==-1)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select a database connection or create a new one!"));
		return;
	}

	if(passLineEdit->text().latin1() == string(""))
	{
		QString msg = tr("Password is empty.\nDo you want to continue?");
		int response = QMessageBox::question(this, tr("External Theme"), msg, tr("Yes"), tr("No"));

		if (response != 0)
			return;
	}

	if(!connectByPool())
		return;

	initTreeView();

	themePropTable->setNumRows(0);

	remoteWidgetStack->raiseWidget(2);
}


void externalDBThemeWindow::databaseTable_clicked( int row, int, int, const QPoint & )
{
	if( row == -1)
	{
		selectedDb_ = false;
		return;
	}
		
	databaseClickedVHeader(row);
}

void externalDBThemeWindow::databaseTypeComboBox_activated( const QString & value )
{
	if (databaseTypeComboBox->currentText() == "Access")
	{
		hostTextLabel->setText(tr("Host"));
		hostLineEdit->setEnabled(false);
		portLineEdit->setEnabled(false);
		userLineEdit->setEnabled(false);
		passwordLineEdit->setEnabled(false);
		directoryPushButton->setEnabled(true);
		dbComboBox->setEnabled(false);

		directoryPushButton->setText(tr("Select Database..."));
		dbComboBox->clear();
		databaseNameLineEdit->setText("");
		databaseNameLineEdit->setEnabled(false);

	}
	else if(databaseTypeComboBox->currentText() == "Firebird")
	{
		hostTextLabel->setText(tr("Host"));
		hostLineEdit->setEnabled(true);
		portLineEdit->setEnabled(true);
		userLineEdit->setEnabled(true);
		passwordLineEdit->setEnabled(true);
		directoryPushButton->setEnabled(true);
		dbComboBox->setEnabled(false);
		databaseNameLineEdit->setEnabled(true);

		directoryPushButton->setText(tr("Select Database..."));
		dbComboBox->clear();
		databaseNameLineEdit->setText("");
	}
	else if ((databaseTypeComboBox->currentText() == "OracleSpatial") ||
			 (databaseTypeComboBox->currentText() == "Oracle") || (databaseTypeComboBox->currentText() == "OracleOCI"))
	{
		
		hostTextLabel->setText(tr("Service name"));
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
		hostTextLabel->setText(tr("Host"));
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
			directoryPushButton->setEnabled(true);
			dbComboBox->setEnabled(true);
			databaseNameLineEdit->setText("");
			databaseNameLineEdit->setEnabled(false);
		}
		else
		{
			directoryPushButton->setEnabled(false);
			databaseNameLineEdit->setText("");
			databaseNameLineEdit->setEnabled(true);
		}
	}
}


void externalDBThemeWindow::dbComboBox_activated( const QString & dbName )
{
    databaseNameLineEdit->setText(dbName);	
}
	

void externalDBThemeWindow::directoryPushButton_clicked()
{
	if (databaseTypeComboBox->currentText() == "Firebird")
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

		return;
	}

#ifdef WIN32
	if (databaseTypeComboBox->currentText() == "Access")
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
			"Microsoft Access (*.mdb)",
			this,
			tr("Select a Database"),
			tr("Choose a Microsoft Access File"));

		if (!openFile.isEmpty())
		{
			dbComboBox->insertItem(openFile);
			string filename = TeGetBaseName (openFile.latin1()) + ".mdb";
			dbComboBox->setCurrentText(openFile);
			databaseNameLineEdit->setText(filename.c_str());
		}
		else
		{
			QMessageBox::warning(this, tr("Warning"), tr("Select a Database!"));
			return;
		}

	}
#endif
	if (databaseTypeComboBox->currentText() == "MySQL" || 
		databaseTypeComboBox->currentText() == "PostgreSQL" || 
		databaseTypeComboBox->currentText() == "PostGIS")
		{
			vector<string> dbNames;

			TeDatabase *db = 0;

			TeDatabaseFactoryParams params;
			params.dbms_name_ = databaseTypeComboBox->currentText().latin1();
			params.host_ = hostLineEdit->text().latin1();
			params.password_ = passwordLineEdit->text().latin1();
			params.port_ = atoi(portLineEdit->text().latin1());
			params.user_ = userLineEdit->text().latin1();
			
			db = TeDatabaseFactory::make(params);
			if(!db)
				return;
			
			if (db->showDatabases(hostLineEdit->text().latin1(), userLineEdit->text().latin1(),
							  passwordLineEdit->text().latin1(), dbNames, params.port_) == false)
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
			dbComboBox->setEnabled(true);
		}

}


void externalDBThemeWindow::previousTwoPushButton_clicked()
{
	remoteWidgetStack->raiseWidget(0);
}


void externalDBThemeWindow::nextTwoPushButton_clicked()
{
	if(!connectByParameters())
		return;

	initTreeView();

	themePropTable->setNumRows(0);

	remoteWidgetStack->raiseWidget(2);
}


void externalDBThemeWindow::externalListView_clicked(QListViewItem* themeItem )
{
	if(!themeItem)
		return;

	string status = themeItem->text(1).latin1();
	if (status == "")
		return;

	if(((QCheckListItem*)themeItem)->isOn())
	{
    std::string view_name =  std::string(themeItem->parent()->text(0).ascii());
    std::string theme_name = std::string(themeItem->text(0).ascii() );
    
		TeTheme* th = getTheme(view_name,theme_name);
		if(!th)
			return;

		TeAppTheme* theme = new TeAppTheme(th);
		
		bool addTheme = true;
		std::vector<TeAppTheme*>::iterator it = themeVec_.begin();
		while(it != themeVec_.end())
		{
			if((*it)->getTheme()->id() == th->id())
			{
				addTheme = false;
				break;
			}
			it++;
		}
		if(addTheme)
			themeVec_.push_back(theme);

		if(previewCheckBox->isChecked())
		{
			canvas_->plotOnPixmap0();
			canvas_->clearAll();	
			canvas_->setProjection(theme->getTheme()->getThemeProjection());
			canvas_->setWorld(theme->getTheme()->getThemeBox());
			
			map<int, RepMap> layerRepMap;
			
			TePlotObjects(theme, canvas_, layerRepMap, dynamic_cast<TeQtProgress*>(plugIngParams_->teprogressbase_ptr_));
			canvas_->copyPixmapToWindow();
		}
		getThemeProperties(th);
		return;
	}
	
  std::string view_name =  std::string(themeItem->parent()->text(0).ascii());
  std::string theme_name = std::string(themeItem->text(0).ascii() );  
  
	TeTheme* th = getTheme(view_name, theme_name);
	if(!th)
		return;

	std::vector<TeAppTheme*>::iterator it = themeVec_.begin();
	while(it != themeVec_.end())
	{
		if((*it)->getTheme()->id() == th->id())
		{
			delete (*it);
			themeVec_.erase(it);
			break;
		}

		it++;
	}
}


void externalDBThemeWindow::previousThreePushButton_clicked()
{
	externalListView->clear();
	clearDBAndThemeVector();
	canvas_->clearAll();
	databaseTable->clearSelection();
	remoteWidgetStack->raiseWidget(0);
}


void externalDBThemeWindow::okPushButton_clicked()
{
	if(themeVec_.size() == 0)
	{
		QString mess;
		mess = tr("There are not selected themes in the view tree!");
		QMessageBox::warning(this, tr("Warning"),mess);
		return;
	}

	QString mess;
	mess  = tr("Do you want to create ") + "  " + QString(Te2String(themeVec_.size()).c_str()) + "  ";
	mess += tr(" external theme(s) ? \n") + "\n";
	mess += tr(" The selected themes are: \n") + "\n";
	for(unsigned int i=0; i<themeVec_.size(); ++i)
		mess += QString(themeVec_[i]->getTheme()->name().c_str()) + "\n";
	if(QMessageBox::question(this, tr("Creating external themes"), mess, tr("Yes"), tr("No")) != 0)
		return;		

	TeWaitCursor wait;

	int dbId = TeDBConnectionsPool::instance().getDatabaseIdx(newDb_);

    if(dbId == -1)
    {
        TeDBConnectionsPool::instance().saveExternalDBConnection(currentDatabase_, newDb_);
        dbId = TeDBConnectionsPool::instance().getDatabaseIdx(newDb_);
    }

    std::vector<std::string> invalidThemeNames;

    for(unsigned int i = 0; i < themeVec_.size(); i++)
	{
		TeTheme* remoteTheme = dynamic_cast<TeTheme*>(themeVec_[i]->getTheme());

		std::string themeName = remoteTheme->name();

        if(themeNameExistsInView(plugIngParams_->getCurrentViewPtr(), themeName))
        {
            invalidThemeNames.push_back(themeName);
            continue;
        }

        TeExternalTheme* externalTheme	= dynamic_cast<TeExternalTheme*>
            (TeViewNodeFactory::make(TeEXTERNALTHEME));

        externalTheme->name(themeName);
        externalTheme->setRemoteTheme(remoteTheme);
		externalTheme->view(plugIngParams_->getCurrentViewPtr()->id());
        externalTheme->setSourceDatabase(currentDatabase_);

        externalTheme->minScale(themeVec_[i]->getTheme()->minScale());
		externalTheme->maxScale(themeVec_[i]->getTheme()->maxScale());
		externalTheme->setThemeBox(themeVec_[i]->getTheme()->box());
		externalTheme->visibleRep(themeVec_[i]->getTheme()->visibleRep());

		if(themeVec_[i]->getTheme()->legend().size() != 0)
		{
			TeLegendEntryVector& vecLeg = themeVec_[i]->getTheme()->legend();
			TeLegendEntryVector::iterator itVecLeg = vecLeg.begin();
			while (itVecLeg != vecLeg.end())
			{
				TeLegendEntry legEntry = *itVecLeg;
				externalTheme->legend(legEntry);
				itVecLeg++;
			}
	
			externalTheme->grouping() = themeVec_[i]->getTheme()->grouping();
		}

		//Insert the external theme metadata in the database
		if(!externalTheme->save())
		{
			wait.resetWaitCursor();
			QString msg = tr("Error inserting theme!") + "\n";
			QMessageBox::critical(this, tr("Error"), msg);
			delete externalTheme;
			return;
		}

		//Add the external theme in the theme list of the view
		plugIngParams_->getCurrentViewPtr()->add(externalTheme);
		TeAppTheme* themeExternal = new TeAppTheme(externalTheme);
		themeExternal->getTheme()->view(viewId_);
		themeExternal->getTheme()->visibleRep(externalTheme->getRemoteTheme()->visibleRep());
		
		//Put the new theme on the interface and set it as the current theme
		TeQtThemeItem* themeItem = new TeQtThemeItem(
			(QListViewItem*)(plugIngParams_->teqtviewslistview_ptr_->currentViewItem()),
			externalTheme->name().c_str(),themeExternal);
		themeItem->setOn(true);
		//checkWidgetEnabling();
		
		TeLegendEntryVector& legendVector = externalTheme->legend();
		if (legendVector.size() > 0)
		{
			//insert the grouping attribute name
			TeAttributeRep rep;
			string nattr;
			if(externalTheme->grouping().groupMode_ != TeNoGrouping)
			{
				rep = externalTheme->grouping().groupAttribute_;
				nattr = externalTheme->grouping().groupNormAttribute_;
			}
			string text = rep.name_;
			map<int, map<string, string> >::iterator it = currentDatabase_->mapThemeAlias().find(externalTheme->id());
			if(it != currentDatabase_->mapThemeAlias().end())
			{
				map<string, string>& m = it->second;
				map<string, string>::iterator tit = m.find(text);
				if(tit != m.end())
					text = tit->second;
				if(!(nattr.empty() || nattr == "NONE"))
				{
					tit = m.find(nattr);
					if(tit != m.end())
						text += "/" + tit->second;
					else
						text += "/" + nattr;
				}
			}
			new TeQtLegendTitleItem(themeItem, text.c_str());

			TeLegendEntryVector::iterator legIt;

			//insert the legend items
			for (legIt = legendVector.begin(); legIt != legendVector.end(); ++legIt)
			{
				TeLegendEntry& legendEntry = *legIt;
				string txt = legendEntry.label();
				new TeQtLegendItem(themeItem, txt.c_str(), &legendEntry);
			}
		}

		//Update view
		TeView* view = plugIngParams_->teqtviewslistview_ptr_->currentViewItem()->getView();

 		if(view->size() == 1)
        {
            TeProjection* viewProj = view->projection();
            TeProjection* layerProj = externalTheme->layer()->projection();

            if(!(*viewProj == *layerProj))
            {
                int id = viewProj->id();

                TeProjection* newViewProj = TeProjectionFactory::make(layerProj->params());

                newViewProj->id(id);

                view->projection(newViewProj);

                if(!currentDatabase_->updateProjection(newViewProj))
                {
			        wait.resetWaitCursor();
			        QString msg = tr("Error saving new view projection!") + "\n";
			        QMessageBox::critical(this, tr("Error"), msg);
                    currentDatabase_->deleteTheme(externalTheme->id());

                    view->projection(viewProj);

                    delete newViewProj;

			        return;
                }
                
                delete viewProj;
             }
        }

		wait.resetWaitCursor();
		QString msg = tr("The selected theme '") +  QString(themeExternal->getTheme()->name().c_str()) + tr("' was imported successfully!") + "\n";
		QMessageBox::information(this, tr("Success"), msg);
				
		externalThemeImportedSignal(themeExternal); //SIGNAL TO TexDATASOURCE INTERFACE
     }

     if(!invalidThemeNames.empty())
     {
         QString msg = tr("Could not create the themes. The names related, already exists as external themes:\n");
         std::vector<std::string>::iterator thNIt;

         for(thNIt = invalidThemeNames.begin(); thNIt != invalidThemeNames.end(); ++thNIt)
         {
             msg += QString("-") + (*thNIt).c_str();
         }

         QMessageBox::information(this, tr("Success"), msg);
     }
    

	wait.resetWaitCursor();
	return;
}


void externalDBThemeWindow::fillDatabaseTable()
{
	//remove the itens

    std::vector<int> dbIds = TeDBConnectionsPool::instance().getConnectionsIdVector();
    std::vector<int>::iterator dbIt;

	int pos = 0;

	databaseTable->setNumRows(0);
    databaseTable->setNumRows(dbIds.size());

    for(dbIt = dbIds.begin(); dbIt != dbIds.end(); ++dbIt)
	{
        TeDatabase* db = TeDBConnectionsPool::instance().getDatabase((*dbIt));

        if(db == NULL)
        {
            throw;
        }

		std::string dbms = db->dbmsName();		//dbPortal->getData("dbms_name");
		std::string host = db->host();			//dbPortal->getData("host_name");
		std::string name = db->databaseName();	//dbPortal->getData("database_name");
		std::string user = db->user();			//dbPortal->getData("user_name");
		std::string port = Te2String(db->portNumber());		//dbPortal->getData("port_number");

		databaseTable->setText(pos, 0, dbms.c_str());
		databaseTable->setText(pos, 1, host.c_str());
		databaseTable->setText(pos, 2, name.c_str());
		databaseTable->setText(pos, 3, user.c_str());
		databaseTable->setText(pos, 4, port.c_str());
		
		++pos;
	}
	
	databaseTable->adjustColumn(0);
	databaseTable->adjustColumn(1);
	databaseTable->adjustColumn(2);
	databaseTable->adjustColumn(3);
	databaseTable->adjustColumn(4);
	
	return;
}


bool externalDBThemeWindow::connectByPool()
{
	TeWaitCursor wait;
	clearDBAndThemeVector();

    std::vector<int> dbIds = TeDBConnectionsPool::instance().getConnectionsIdVector();
    int dbIdx = dbIds[databaseTable->currentRow()];

	newDb_ = TeDBConnectionsPool::instance().getDatabase(dbIdx);

	if(!newDb_ || !newDb_->isConnected())
	{
		wait.resetWaitCursor();
		QMessageBox::information(this, tr("Information"), tr("Error connecting database.\n Verify if the connection parameters are right!"));
		selectedDb_ = false;
		return false;
	}

	string DBver;
	if(needUpdateDB(newDb_, DBver))
	{
		if(newDb_->isConnected())
        {
            newDb_->close();
        }

        delete newDb_;
        newDb_ = NULL;

		wait.resetWaitCursor();
		QString msg = tr("It is not possible to connect to the database because it is not in the same terralib model version!") + "\n";
		QMessageBox::information(this, tr("Information"), msg);
		return false;
	}

	//verify if the database is the same of the current database
	if(currentDatabase_->getDatabaseDescription().compare(newDb_->getDatabaseDescription()) == 0)
	{
		wait.resetWaitCursor();
		QString msg = tr("It is not possible to connect to the same local database!") + "\n";
		QMessageBox::information(this, tr("Information"), msg);
		clearDBAndThemeVector();
		return false;
	}

    if(!newDb_->isConnected())
    {
        if(!newDb_->connect(newDb_->host(), newDb_->user(), newDb_->password(), newDb_->databaseName(), 
            newDb_->portNumber()))
        {
		    wait.resetWaitCursor();
		    QString msg = tr("Fail to connect to the external database!") + "\n";
		    QMessageBox::information(this, tr("Information"), msg);
		    newDb_ = NULL;
		    return false;
        }
    }

	// Get layers from the database
	
    if(newDb_->layerMap().empty())
    {
        if (!newDb_->loadLayerSet())
	    {
		    wait.resetWaitCursor();
		    QString msg = tr("Fail to get the layers from the database!") + "\n";
		    msg += newDb_->errorMessage().c_str();
		    QMessageBox::critical(this, tr("Error"), msg);
		    clearDBAndThemeVector();
		    return false;		
	    }
    }

    if(newDb_->viewMap().empty())
    {
        if (!newDb_->loadViewSet(newDb_->user()))
	    {	
		    wait.resetWaitCursor();
		    QString msg = tr("Fail to get the views from the database!") + "\n";
		    msg += newDb_->errorMessage().c_str();
		    QMessageBox::critical(this, tr("Error"), msg);
		    clearDBAndThemeVector();
		    return false;
	    }
    }

	return true;    
}


bool externalDBThemeWindow::connectByParameters()
{
	TeWaitCursor wait;
	externalDatabaseId_ = -1;
	clearDBAndThemeVector();
		
    std::string hostName = hostLineEdit->text().latin1(),
                userName = userLineEdit->text().latin1(),
                password = passwordLineEdit->text().latin1(),
                dirName = "";

	if (dbComboBox->count() != 0)
		dirName  = dbComboBox->currentText().latin1();
	
    std::string dbName = databaseNameLineEdit->text().latin1(),
                dbType = databaseTypeComboBox->currentText().latin1();
    
    int port = atoi(portLineEdit->text().latin1());
	
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
	else if (dbType == "SQL Server")
	{
		dirName = dbName;
	}
#endif

// Check the data provided by the user
	if (dbType == "Access" && dirName.empty() == true)
	{
		QString qs;
		qs = tr("Select the database to be connected!");
		wait.resetWaitCursor();
		QMessageBox::warning( this, tr("Warning"), qs);
		return false;
	}

	if (dbType != "Access" && userName.empty())
	{
		wait.resetWaitCursor();
		QMessageBox::warning(this, tr("Warning"), tr("Enter the user name!"));
		return false;
	}

	
	if (dbName.empty() && (dbType != "OracleSpatial" && dbType != "Oracle"))
	{
		wait.resetWaitCursor();
		QString mess;
		mess = tr("Select a database to be connected!");
		QMessageBox::warning(this, tr("Warning"),mess);
		return false;
	}

	if(dbType == "Firebird")
	{
		if(dirName.empty())
			dirName = dbName;
	}

	if(dbType == "OracleSpatial" || dbType == "Oracle")
		dirName = hostName;

    newDb_ = TeDBConnectionsPool::instance().getDatabase(dbType, dirName, hostName, userName, password, 
        port);

	string DBver;
	if(needUpdateDB(newDb_, DBver))
	{
		if(newDb_->isConnected())
        {
            newDb_->close();
        }

        delete newDb_;
        newDb_ = NULL;

		wait.resetWaitCursor();
		QString msg = tr("It is not possible to connect to the database because it is not in the same terralib model version!") + "\n";
		QMessageBox::information(this, tr("Information"), msg);
		return false;
	}

	//verify if the database is the same of the current database
	if(*currentDatabase_ == *newDb_)
	{
        if(newDb_->isConnected())
        {
            newDb_->close();
        }

        delete newDb_;
        newDb_ = NULL;

        wait.resetWaitCursor();
		QString msg = tr("It is not possible to connect to the same local database!") + "\n";
		QMessageBox::information(this, tr("Information"), msg);
		return false;
	}

	if (newDb_ == NULL)
	{
        wait.resetWaitCursor();

		QMessageBox::critical(this, tr("Error"),
		newDb_->errorMessage().c_str());
		return false;
	}
	
    // Get layers from the database
    if(newDb_->layerMap().empty())
    {
	    if (!newDb_->loadLayerSet())
	    {
            wait.resetWaitCursor();

            QString msg = tr("Fail to get the layers from the database!") + "\n";
		    msg += newDb_->errorMessage().c_str();
		    QMessageBox::critical(this, tr("Error"), msg);
		    newDb_->close();
		    delete newDb_;
		    newDb_ = 0;
		    return false;		
	    }
    }

    if(newDb_->viewMap().empty())
    {
	    if (!newDb_->loadViewSet(userLineEdit->text().latin1()))
	    {	
            wait.resetWaitCursor();

            QString msg = tr("Fail to get the views from the database!") + "\n";
		    msg += newDb_->errorMessage().c_str();
		    QMessageBox::critical(this, tr("Error"), msg);
		    newDb_->close();
		    delete newDb_;
		    newDb_ = 0;
		    return false;
	    }
    }

	wait.setWaitCursor();
	return true;
}


void externalDBThemeWindow::initTreeView()
{	
	externalListView->setRootIsDecorated(true);
	externalListView->clear();
	TeViewMap viewMap = newDb_->viewMap();
	TeViewMap::iterator itView = viewMap.begin();
	while (itView != viewMap.end())
	{
		QListViewItem* item = new QListViewItem(externalListView);
		item->setText(0, itView->second->name().c_str());
		item->setText(1, "");
		item->setPixmap(0, QPixmap::fromMimeSource("view.bmp"));
		//externalListView->setOpen(item, true);
		vector<TeViewNode*> viewNode = itView->second->themes();
		for (unsigned int i = 0; i < viewNode.size(); ++i)
		{
			if(viewNode[i]->type() != (int)TeTHEME)
				continue;

			QListViewItem* itemChild = new QCheckListItem(item,viewNode[i]->name().c_str(),QCheckListItem::CheckBox);
			item->insertItem(itemChild);
			string status = checkRemoteTheme(externalDatabaseId_, viewNode[i]->id());
			itemChild->setText(1, status.c_str());
			itemChild->setPixmap(0, QPixmap::fromMimeSource("theme.bmp"));
			//itemChild->setOpen(true);
		}

		++itView;		
	} 
}


std::string externalDBThemeWindow::checkRemoteTheme(const int& externalDatabaseId, const int& externalThemeId)
{
    string strSQL = "SELECT * FROM te_external_theme WHERE database_id = " + Te2String(externalDatabaseId);
    strSQL += " AND external_theme_id = " + Te2String(externalThemeId);

	TeDatabasePortal* dbPortal = currentDatabase_->getPortal();

    if(!dbPortal)
		return "";

    if(!dbPortal->query(strSQL) || !dbPortal->fetchRow())
    {
        dbPortal->freeResult();
        delete dbPortal;
		return "External";
	}

	dbPortal->freeResult();
    delete dbPortal;
    
    return "Imported";    
}


void externalDBThemeWindow::getThemeProperties(TeTheme* theme )
{
	if(!theme)
		return;
	
	vector<string> param;
	vector<string> val;
	string s;
	unsigned int i;

	std::string collectionTable = theme->collectionTable();
	TeLayer* layer = theme->layer();
	TeDatabasePortal* portal = layer->database()->getPortal();
	if(!portal)
		return;

	param.push_back(tr("Theme Name:").latin1());
	val.push_back(theme->name());
	param.push_back(tr("Infolayer:").latin1());
	val.push_back(layer->name());

	int rep = layer->geomRep();
	if((rep & TePOLYGONS) || (rep & TeLINES) || (rep & TePOINTS))
	{
		param.push_back(tr("Number of polygons:").latin1());
		if(layer->hasGeometry(TePOLYGONS))
		{
			string count = "SELECT COUNT(*) FROM " + layer->tableName(TePOLYGONS);
			count += ", " + collectionTable + " WHERE ";
			count += layer->tableName(TePOLYGONS) + ".object_id = ";
			count += collectionTable + ".c_object_id";
			portal->freeResult();
			if(portal->query(count))
			{
				if(portal->fetchRow())
				{
					count = portal->getData(0);			
					val.push_back(count);
				}
				else
					val.push_back("0");
			}
			else
				val.push_back("0");
		}
		else
			val.push_back("0");
	
		param.push_back(tr("Number of Lines:").latin1());
		if(layer->hasGeometry(TeLINES))
		{
			string count = "SELECT COUNT(*) FROM " + layer->tableName(TeLINES);
			count += ", " + collectionTable + " WHERE ";
			count += layer->tableName(TeLINES) + ".object_id = ";
			count += collectionTable + ".c_object_id";
			portal->freeResult();
			if(portal->query(count))
			{
				if(portal->fetchRow())
				{
					count = portal->getData(0);			
					val.push_back(count);
				}
				else
					val.push_back("0");
			}
			else
				val.push_back("0");
		}
		else
			val.push_back("0");
	
		param.push_back(tr("Number of Points:").latin1());
		if(layer->hasGeometry(TePOINTS))
		{
			string count = "SELECT COUNT(*) FROM " + layer->tableName(TePOINTS);
			count += ", " + collectionTable + " WHERE ";
			count += layer->tableName(TePOINTS) + ".object_id = ";
			count += collectionTable + ".c_object_id";
			portal->freeResult();
			if(portal->query(count))
			{
				if(portal->fetchRow())
				{
					count = portal->getData(0);			
					val.push_back(count);
				}
				else
					val.push_back("0");
			}
			else
				val.push_back("0");
		}
		else
			val.push_back("0");
	}
	if(rep&TeRASTER)
	{
	}

	param.push_back(tr("Collection:").latin1());
	if(theme->attributeRest().empty())
		val.push_back(tr("Without Restriction").latin1());
	else
		val.push_back(theme->attributeRest());
	
	themePropTable->setNumRows(param.size());
	for(i = 0; i < param.size(); ++i)
	{
		themePropTable->setText(i, 0, param[i].c_str());
		themePropTable->setText(i, 1, val[i].c_str());
	}
	themePropTable->adjustColumn(0);
	themePropTable->adjustColumn(1);
	
	delete portal;

}


TeTheme* externalDBThemeWindow::getTheme( std::string& viewName, std::string& themeName )
{
    TeViewMap& viewMap = newDb_->viewMap();
	TeViewMap::iterator itView = viewMap.begin();
	while ( itView != viewMap.end() )
	{
		if (TeConvertToUpperCase(itView->second->name()) == TeConvertToUpperCase(viewName))
		{
			vector<TeViewNode*>& viewNode = itView->second->themes();
			for (unsigned int i = 0; i < viewNode.size(); i ++)
			{
				//only get TeTheme type
				if (TeConvertToUpperCase(viewNode[i]->name()) == TeConvertToUpperCase(themeName) &&
					viewNode[i]->type()==TeTHEME)
				{
					return (TeTheme*)viewNode[i];
                    
				}
			}
		}
		++itView;
	}
	return 0;
}


void externalDBThemeWindow::databaseClickedVHeader( int row )
{
	if(row == -1)
	{
		selectedDb_ = false;
		return;
	}

	databaseTable->selectRow(row);
	passLineEdit->clear();
	selectedDb_ = true;
}


void externalDBThemeWindow::removePushButton_clicked()
{
	if(databaseTable->currentRow() == -1)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Database Entry not selected!"));
		return;
	}

    std::vector<int> dbIds = TeDBConnectionsPool::instance().getConnectionsIdVector();

    int idx_db = databaseTable->currentRow();

    int dbId = dbIds[idx_db];

    TeDatabase* db = TeDBConnectionsPool::instance().getDatabase(dbId);

    if(db == NULL)
    {
        QMessageBox::warning(this, tr("Warning"), tr("Error getting database status!"));
	    return;
    }

	//verifica se a conexao a ser deletada nao esta sendo usada por nenhum tema
	bool status = false;

    if(!getDatabaseStatus(db, status))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Error getting database status!"));
		return;
	}

	if(status)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Database Entry is in use by a remote theme!"));
		return;
	}

    TeDBConnectionsPool::instance().deleteExternalDBConnection(currentDatabase_, dbId);

	fillDatabaseTable();
}

//void externalDBThemeWindow::show()
//{
//	updateForm(plugIngParams_);
//}

bool externalDBThemeWindow::getDatabaseStatus(TeDatabase* remoteDb, bool & status )
{
	int dbId = TeDBConnectionsPool::instance().getDatabaseIdx(remoteDb);
	
	std::string sql = "SELECT * FROM te_external_theme WHERE database_id = " + Te2String(dbId);

    TeDatabasePortal* dbPortal = currentDatabase_->getPortal();

	if(!dbPortal)
		return false;

	if(!dbPortal->query(sql) )
	{
		dbPortal->freeResult();
		delete dbPortal;
		return false;
	} 

	if(dbPortal->fetchRow())
	{
		status = true;
	}

	dbPortal->freeResult();
	delete dbPortal;

    return true;
}

void externalDBThemeWindow::hideEvent(QHideEvent*)
{
    clearDBAndThemeVector();
}

bool externalDBThemeWindow::themeNameExistsInView(TeView* view, const std::string& themeName)
{
    std::vector<TeViewNode*> themes = view->themes();
    std::vector<TeViewNode*>::iterator it;

    for(it = themes.begin(); it != themes.end(); ++it)
    {
        if(((*it)->type() == TeEXTERNALTHEME) && ((*it)->name() == themeName))
        {
            return true;
        }
    }

    return false;    
}

void externalDBThemeWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;
	help_ = new Help(this, tr("Help"));
	QString helpFile = "et_index.html";
	help_->init(helpFile);
	help_->show();
}

QPopupMenu* externalDBThemeWindow::findThemeMenu()
{
	QMenuBar* mnu = plugIngParams_->qtmain_widget_ptr_->menuBar();

    int idx = -1;
    
    for(unsigned int i = 0; i < mnu->count() -1; i++)
    {
        idx = mnu->idAt(i);
		if(mnu->text(idx).isEmpty())
		{
			continue;
		}

		if(mnu->text(idx) == tr("&Theme"))
        {
            break;
        }
    }

    if(idx == -1)
    {
        return NULL;
    }

    return mnu->findItem(idx)->popup();
}




