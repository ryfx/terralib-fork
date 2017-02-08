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

#include <TeQtLayerItem.h>
#include "TeProgress.h"
#include <qstatusbar.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <TeDriverSHPDBF.h>
#include <TeDriverCSV.h>
#include <TeDriverMIDMIF.h>
#include <TeDriverSPRING.h>
#include <TeDriverBNA.h>
#include <TeOGRExportImportFunctions.h>
#include <time.h>
#include <TeGDriverFactory.h>


void ImportWindow::init()
{
	help_ = 0;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	db_ = mainWindow_->currentDatabase();
	projectionLineEdit->setEnabled(false);
	openFileLineEdit->setEnabled(false);
	projection_ = 0;
	autoLinkRadioButton->setEnabled(false);
	autoLinkRadioButton->setChecked(true);
	columnLinkRadioButton->setEnabled(false);
	linkColumnComboBox->setEnabled(false);

	openFileLineEdit->setText(mainWindow_->lastOpenDir_ + "/...");	
	openFileLineEdit->setEnabled(false);	
	noMessage_ = false;
	loadInitialPostgresEncoding();
}


void ImportWindow::openFilePushButton_clicked()
{
	bool enableAutoLink=true;

	projectionPushButton->setEnabled(true);
	if (projection_)
	{
		delete projection_;
		projection_ = 0;
	}

	QStringList qfileNames = QFileDialog::getOpenFileNames(tr("Shape Files (*.shp *.SHP);;MIF Files (*.mif *.MIF);;SPRING-GEO Files (*.geo *.GEO);;Atlas GIS BNA Files (*.bna *.BNA);;Geography Markup Language Files (*.gml)"), 
        mainWindow_->lastOpenDir_, this, 0, tr("Open Import File"));
	if (qfileNames.isEmpty())
		return;

    QStringList::iterator fileit;
    QString auxFileName,
            auxLayerName;
    
    int i = 0;

	if(qfileNames.size() > 1 || qfileNames.size() <= 0)
	{
		linkTypeButtonGroup->setEnabled(false);
	}
	else 
	{
		linkTypeButtonGroup->setEnabled(true);
	}

	bool enableColumnLink = true;

    for(fileit = qfileNames.begin(); fileit != qfileNames.end(); ++fileit)
    {
        QString qfileName = (*fileit);
	    char* filename = strdup(qfileName);
        if(i == 0)
        {
            mainWindow_->lastOpenDir_ = TeGetPath(filename).c_str();
        }
        i++;

	    string filePrefix = TeGetName( filename );
	    string fileExt = TeGetExtension( filename );
	    string keyName;
	    string layerName = TeGetBaseName(filename);
	    TeAttributeList attList;
	    linkColumnComboBox->clear();

	    if (fileExt == "shp" || fileExt == "SHP")
	    {
//		    linkTypeButtonGroup->setEnabled(true);

		    string ftest = filePrefix + ".dbf";
		    if (access(ftest.c_str(),04) == -1)
		    {
			    ftest = filePrefix + ".DBF";
			    if (access(ftest.c_str(),04) == -1)
			    {
				    QString mess = tr("Shapefile data is incomplete. File \'");
				    mess += ftest.c_str();
				    mess += tr("\' can not be found!");
				    QMessageBox::critical(this, tr("Error"), mess);
				    return;
			    }
		    }
		    ftest = filePrefix + ".shx";
		    if (access(ftest.c_str(),04) == -1)
		    {
			    ftest = filePrefix + ".SHX";
			    if (access(ftest.c_str(),04) == -1)
			    {
				    QString mess = tr("Shapefile data is incomplete. File \'");
				    mess += ftest.c_str();
				    mess += tr("\' can not be found!");
				    QMessageBox::critical(this, tr("Error"), mess);
				    return;
			    }
		    }

            auxLayerName += QString(layerName.c_str());
            auxFileName += filename;

            if(i < (int)qfileNames.size())
            {
                auxLayerName += ", ";
                auxFileName += ", ";
            }
            
		    try
		    {
			    TeShapefileDriver shpF(filename);
			    shpF.getDataAttributesList(attList);
			    projection_ = shpF.getDataProjection();
			    if (projection_)
			    {
				    string projName = projection_->name() + " / " + (projection_->datum()).name();
				    projectionLineEdit->setText(projName.c_str());
			    }
		    }
		    catch(...)
		    {
			    layerNameLineEdit->clear();
			    openFileLineEdit->clear();
			    QMessageBox::critical(this, tr("Error"),
				    tr("Fail to read the list of attributes of the DBF file!"));
			    return;
		    }
	    }
	    else if (fileExt == "mif" || fileExt == "MIF")
	    {
		    linkTypeButtonGroup->setEnabled(true);

		    string ftest = filePrefix + ".mid";
		    if (access(ftest.c_str(),04) == -1)
		    {
			    ftest = filePrefix + ".MID";
			    if (access(ftest.c_str(),04) == -1)
			    {
				    QString mess = tr("MID/MIF data is incomplete. File \'");
				    mess += ftest.c_str();
				    mess += tr("\' can not be found!");
				    QMessageBox::critical(this, tr("Error"), mess);
				    return;
			    }
		    }

            auxLayerName += QString(layerName.c_str());
            auxFileName += filename;

            if(i < (int)qfileNames.size())
            {
                auxLayerName += ", ";
                auxFileName += ", ";
            }

		    try
		    {
			    TeReadMIFAttributeList(filename,attList);
			    projection_ = TeReadMIFProjection(filename);
			    if (!projection_)
				    projection_ = new TeNoProjection();
			    string projName = projection_->name() + " / " + (projection_->datum()).name();
			    projectionLineEdit->setText(projName.c_str());
		    }
		    catch(...)
		    {
			    layerNameLineEdit->clear();
			    openFileLineEdit->clear();
			    QMessageBox::critical( this, tr("Error"),
				    tr("Fail to read the list of attributes of the MIF file!"));
			    return;
		    }
	    }
	    else if (TeStringCompare(fileExt,"geo"))
	    {
		    linkTypeButtonGroup->setEnabled(true);
			enableColumnLink = false;

		    string ftest = filePrefix + ".tab";
		    if (access(ftest.c_str(),04) == -1)
		    {
			    ftest = filePrefix + ".TAB";
			    if (access(ftest.c_str(),04) == -1)
			    {
				    QString mess = tr("GEO/TAB data is incomplete. File \'");
				    mess += ftest.c_str();
				    mess += tr("\' can not be found!");
				    QMessageBox::critical(this, tr("Error"), mess);
				    return;
			    }
		    }
    		
            auxLayerName += QString(layerName.c_str());
            auxFileName += filename;

            if(i < (int)qfileNames.size())
            {
                auxLayerName += ", ";
                auxFileName += ", ";
            }

            string tabFileName = filePrefix + ".tab";
		    try
		    {
			    TeReadGeoAttributeList(tabFileName, attList, keyName);
			    linkColumnComboBox->insertItem(keyName.c_str());
			    projection_ = new TeNoProjection();
			    if (projection_)
			    {
				    string projName = projection_->name() + " / " + (projection_->datum()).name();
				    projectionLineEdit->setText(projName.c_str());
			    }
		    }
		    catch(...)
		    {
			    layerNameLineEdit->clear();
			    openFileLineEdit->clear();
			    QMessageBox::critical( this, tr("Error"),
				    tr("Fail to read the list of attributes of the SPRING-GEO file!"));
			    return;
		    }
	    }
	    else if(fileExt == "bna" || fileExt == "BNA")
	    {
		    string ftest = filePrefix + ".bna";
		    if(access(ftest.c_str(),04) == -1)
		    {
			    ftest = filePrefix + ".BNA";
			    if (access(ftest.c_str(),04) == -1)
			    {
				    QString mess = tr("BNA data is incomplete. File \'");
				    mess += ftest.c_str();
				    mess += tr("\' can not be found!");
				    QMessageBox::critical(this, tr("Error"), mess);
				    return;
			    }
		    }		

            auxLayerName += QString(layerName.c_str());
            auxFileName += filename;

            if(i < (int)qfileNames.size())
            {
                auxLayerName += ", ";
                auxFileName += ", ";
            }

		    try
		    {
			    TeReadBNAAttributeList(filename, attList);
			    //projectionLineEdit->setText(tr("Unknown..."));
			    projection_ = new TeNoProjection();
			    if(projection_)
			    {
				    string projName = projection_->name() + " / " + (projection_->datum()).name();
				    projectionLineEdit->setText(projName.c_str());
			    }
		    }
		    catch(...)
		    {
			    layerNameLineEdit->clear();
			    openFileLineEdit->clear();
			    QMessageBox::critical(this, tr("Error"),
				    tr("Fail to read the list of attributes of the BNA file!"));
			    return;
		    }
	    }else if(TeConvertToUpperCase(fileExt) == "GML")
		{
			enableAutoLink=false;
			string ftest = filePrefix + ".gml";
		    if(access(ftest.c_str(),04) == -1)
		    {
			    ftest = filePrefix + ".GML";
			    if (access(ftest.c_str(),04) == -1)
			    {
				    QString mess = tr("GML data is incomplete. File \'");
				    mess += ftest.c_str();
				    mess += tr("\' can not be found!");
				    QMessageBox::critical(this, tr("Error"), mess);
				    return;
			    }
		    }		

            auxLayerName += QString(layerName.c_str());
            auxFileName += filename;

            if(i < (int)qfileNames.size())
            {
                auxLayerName += ", ";
                auxFileName += ", ";
            }
			try
			{
				if(readGMLSettings(filename,attList) == false)
				{
					QMessageBox::critical(this, tr("Error"), tr("Fail to read the list of attributes of the GML file!"));
					return;
				}
			} catch(...)
		    {
			    layerNameLineEdit->clear();
			    openFileLineEdit->clear();
			    QMessageBox::critical(this, tr("Error"),
				    tr("Fail to read the list of attributes of the GML file!"));
			    return;
		    }

		}else
	    {
		    linkTypeButtonGroup->setEnabled(true);

		    layerNameLineEdit->clear();
		    openFileLineEdit->clear();
		    QMessageBox::critical(this, tr("Error"), tr("This file type is not supported!"));
		    return;
	    }

	    linkColumnComboBox->clear();
		if(TeRenameInvalidFieldName(attList))
		{
			QMessageBox::information(this,tr("Information"),tr("There are fields with invalid names! The system will suggest new names for them! Please check the new name"));
		}
	    TeAttributeList::iterator it = attList.begin();
	    while ( it != attList.end() )
	    {
		    QString s((*it).rep_.name_.c_str());
		    switch ((*it).rep_.type_)
		    {
			    case TeREAL:
			    case TeINT:
			    case TeSTRING:
			    case TeCHARACTER:
				    linkColumnComboBox->insertItem(s);
	 			    break;
			    default:
			    break;
		    }
		    ++it;
			

	    }
	
		linkColumnComboBox->setCurrentItem( 0 );
		autoLinkRadioButton->setEnabled(enableAutoLink);
		columnLinkRadioButton->setEnabled(enableColumnLink & enableAutoLink);
		linkColumnComboBox->setEnabled(false);
    }   

	layerNameLineEdit->setText(auxLayerName);
	openFileLineEdit->clear();
	openFileLineEdit->insert(auxFileName);	
}

void ImportWindow::projectionPushButton_clicked()
{
	QString f = openFileLineEdit->text();
    string fileExt = TeGetExtension(f.latin1());

    if (fileExt.empty())
	{
		QMessageBox::warning( this, tr("Warning"), tr("There's no file selected!"));
		return;
	}

    ProjectionWindow *projectionWindow = new ProjectionWindow(this, "projection", true);
	if (projection_)
		projectionWindow->loadProjection(projection_);
	if (projectionWindow->exec() == QDialog::Accepted)
	{
		projection_ = projectionWindow->getProjection();
		string mess = projection_->name() + "/" + projection_->datum().name();
		projectionLineEdit->setText(mess.c_str());
	}
	delete projectionWindow;

}

void ImportWindow::okPushButton_clicked()
{
	QString f = openFileLineEdit->text();
	if (f.isEmpty()) 
	{
		QMessageBox::warning( this, tr("Warning"), tr("Enter with a file name!"));
		return;
	}

    if(layerNameLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this,tr("Warning"),
		tr("Enter output layer name and try again!"));
		return;
	}

	for(int iCont=0;iCont<linkColumnComboBox->count();iCont++)
	{
			std::string			errorMessage;
			 bool				changed = false;
			TeCheckName(linkColumnComboBox->text(iCont).latin1(), changed, errorMessage);
			if(changed)
			{
				/* Mudança devido a falta de tradução genérica na terralib
				QString mess = "Invalid column name " + linkColumnComboBox->text(iCont);
				mess += " : ";
				mess += errorMessage.c_str();
				QMessageBox::warning( this, tr("Warning"),mess);
				*/
				QString mess = "Invalid column name " + linkColumnComboBox->text(iCont);
				QMessageBox::warning( this, tr("Warning"),mess);

				return;
			}
	}

    QStringList fileNames = QStringList::split(", ", openFileLineEdit->text());
    QStringList layerNames = QStringList::split(", ", layerNameLineEdit->text());
    QStringList errorLayerNames;
    QStringList errorLayers;

    bool isMosaic = (layerNames.size() == 1) && (fileNames.size() > 1);

    if(isMosaic)
    {
        return;
    }
	setClientEncoding();
    for(unsigned int i = 0; i < fileNames.size(); i++)
    {
        string layerName = layerNames[i].latin1();

        string errorMessage;
	    bool changed = false;
	    string newName = TeCheckName(layerName, changed, errorMessage); 
	    if(changed)
	    {
		    /* Mudança devido a falta de tradução genérica na terralib
			QString mess = " - " + QString(layerName.c_str()) + ": "; 
            mess += errorMessage.c_str();
            errorLayerNames.push_back(mess);
			*/
			QString mess = " - " + QString(layerName.c_str()); 
            errorLayerNames.push_back(mess);
            continue;
	    }

	    if (db_->layerExist(layerName))
	    {
		    QString mess = " - " + QString(layerName.c_str()) + ": "; 
            mess += tr("Output layer name already exist, replace and try again!");
            errorLayerNames.push_back(mess);
            continue;
	    }

        bool status = true;
	    TeBox box;
	    if (!projection_)
	    {
		    QMessageBox::warning( this, tr("Warning"),
			    tr("Enter with the data projection!"));
			restoreClientEncoding();
		    return;
	    }

        const char *filename = fileNames[i].latin1();
	    string fileExt = TeGetExtension (filename);
	    string linkName;

        if(fileNames.size() <= 1)
        {
	        if (!TeStringCompare(fileExt,"geo"))
	        {
		        if (columnLinkRadioButton->isChecked())
		        {
			        QString k = linkColumnComboBox->currentText();
			        if (!k.isEmpty())
				        linkName = k.latin1();
			        QString msg = tr("The column") + " \"" + linkName.c_str() + "\" ";
			        msg += tr("will link the table of attributes with the table of geometries!\nDo you confirm?");
			        int response = QMessageBox::question(this, tr("Question"), msg, tr("Yes"), tr("No"));
			        if (response != 0)
			        {
				        noMessage_ = true;
				        linkName = "";
				        autoLinkRadioButton->toggle();
				        linkTypeButtonGroup_clicked(0);
						restoreClientEncoding();
				        return;	
			        }
		        }
		        else
		        {
			        linkName = "";
			        if (!noMessage_)
			        {
				        QString msg = tr("A column named \"object_id_<n>\" will be generated to link the table of attributes with the table of geometries!\nDo you confirm?");
				        int response = QMessageBox::question(this, tr("Question"), msg, tr("Yes"), tr("No"));
				        if (response != 0)
				        {
					        linkTypeButtonGroup_clicked(1);
							restoreClientEncoding();
					        return;	
				        }
			        }
		        }
	        }
        }

	    importedLayer_ = new TeLayer(layerName,db_,projection_);
	    
        if (!importedLayer_ || importedLayer_->id() <= 0)
	    {
            QString error = " - " + QString(layerName.c_str()) + tr(": Fail to create a new layer in the database.");
            errorLayers.push_back(error);
		    delete importedLayer_;
		    importedLayer_ = 0;
		    continue;
	    }

	    TeWaitCursor wait;
	    clock_t	ti=0, tf=0;
	    try
	    {
		    if(TeProgress::instance())
		    {
			    QString caption = tr("Import");
			    TeProgress::instance()->setCaption(caption.latin1());
		    }
		    ti = clock();
		    if (TeStringCompare(fileExt,"shp"))
		    {
			    if(TeProgress::instance())
			    {
				    QString msg = tr("Importing the shapefile. Please, wait!");
				    TeProgress::instance()->setMessage(msg.latin1());
			    }

				status = TeImportShape (importedLayer_,filename,"",linkName,100,(db_->dbmsName() == "Ado" ? false : true));
		    }
		    else if (TeStringCompare(fileExt,"mif"))
		    {
			    if(TeProgress::instance())
			    {
				    QString msg = tr("Importing the MIF file. Please, wait!");
				    TeProgress::instance()->setMessage(msg.latin1());
			    }
			    status = TeImportMIF (importedLayer_,filename,"",linkName);
		    }
		    else if (TeStringCompare(fileExt,"geo"))
		    {
			    if(TeProgress::instance())
			    {
				    QString msg = tr("Importing the SPRING-GEO file. Please, wait!");
				    TeProgress::instance()->setMessage(msg.latin1());
			    }
			    status = TeImportGEO (importedLayer_,filename);
		    }
		    else if(TeStringCompare(fileExt,"bna"))
		    {
			    if(TeProgress::instance())
			    {
				    QString msg = tr("Importing the BNA file. Please, wait!");
				    TeProgress::instance()->setMessage(msg.latin1());
			    }

			    TeAttributeList attList;

			    status = TeImportBNA(importedLayer_, filename, "" , attList, 60, linkName);
		    }else if(TeStringCompare(fileExt,"gml"))
			{

				if(TeProgress::instance())
			    {
				    QString msg = tr("Importing the GML file. Please, wait!");
				    TeProgress::instance()->setMessage(msg.latin1());
			    }
				status=TeImportOGR(importedLayer_,filename);
			}
		    else 
		    {
			    wait.resetWaitCursor();
			    QMessageBox::critical( this, tr("Error"), tr("This file type is not supported!"));
				restoreClientEncoding();
			    return;
		    }
		    tf = clock(); 
		    if(TeProgress::instance())
			    TeProgress::instance()->reset();
	    }
	    catch (TeException& ex)
	    {
		    if(TeProgress::instance())
			    TeProgress::instance()->reset();
		    wait.resetWaitCursor();
            QString msg = " - " + QString(layerName.c_str()) + tr(": Fail to import the layer.");
		    msg += ex.message().c_str();
            errorLayers.push_back(msg);
		    status = false;
	    }
	    catch(...)
	    {
		    if(TeProgress::instance())
			    TeProgress::instance()->reset();
		    wait.resetWaitCursor();
            QString msg = " - " + QString(layerName.c_str()) + tr(": Fail to import the layer.");
            errorLayers.push_back(msg);
		    status = false;
	    }
	    projectionLineEdit->setText("");
	    if (status)
	    {
		    TeQtDatabasesListView *databasesListView = mainWindow_->getDatabasesListView();
		    TeQtDatabaseItem *databaseItem = databasesListView->currentDatabaseItem();

		    TeQtLayerItem *layerItem = new TeQtLayerItem((QListViewItem*)databaseItem, importedLayer_->name().c_str(), importedLayer_);
		    layerItem->setEnabled(true);
		    if (databasesListView->isOpen(layerItem->parent()) == false)
			    databasesListView->setOpen(layerItem->parent(),true);
		    mainWindow_->checkWidgetEnabling();
		    double mTime = (tf-ti)/CLOCKS_PER_SEC;
		    QString myValue;
		    myValue.sprintf("%.3f",mTime);
		    QString ttime = tr("Time to import (in seconds): ");
		    ttime += myValue;
		    mainWindow_->statusBar()->message(ttime);
	    }
	    else
	    {
		    if (importedLayer_->id() > 0)
		    {
			    db_->deleteLayer(importedLayer_->id());
			    importedLayer_ = 0;
		    }
		    if (importedLayer_)
			    delete importedLayer_;
		    importedLayer_ = 0;
		    if(TeProgress::instance())
			    TeProgress::instance()->reset();
            
            QString mess = " - " + QString(layerName.c_str()) + tr(": Fail to import the layer ");

            errorLayers.push_back(mess);
	    }
    }

    bool errorNames = errorLayerNames.size() > 0,
         errorImports = errorLayers.size() > 0;

    QStringList::iterator errorsIt;

    QString errorMsg;

    if(errorNames)
    {
        errorMsg = tr("The following layer names are invalid: ");
        
        for(errorsIt = errorLayerNames.begin(); errorsIt != errorLayerNames.end(); ++errorsIt)
        {
            errorMsg += "\n" + (*errorsIt);
        }
    }
	restoreClientEncoding();
    if(errorImports)
    {
        errorMsg += tr("\nCould not import the following layer: ");
        
        for(errorsIt = errorLayers.begin(); errorsIt != errorLayers.end(); ++errorsIt)
        {
            errorMsg += "\n" + (*errorsIt);
        }
    }

    if(!errorMsg.isEmpty())
    {
        QMessageBox::warning( this, tr("Warning"), errorMsg);
    }

    if(!(errorNames || errorImports))
    {
        if(fileNames.size() == 1)
        {
            accept();
        }
        else
        {
            reject();
        }
    }
}


void ImportWindow::linkTypeButtonGroup_clicked(int n)
{
    if (n != 1)
		linkColumnComboBox->setEnabled(false);
	else
		linkColumnComboBox->setEnabled(true);
}


TeLayer* ImportWindow::getLayer()
{
	return importedLayer_;
}


void ImportWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("importWindow.htm");
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





void ImportWindow::loadInitialPostgresEncoding()
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


void ImportWindow::setClientEncoding()
{
	std::string		encoding;

	if(db_ == 0) return;
	if((db_->dbmsName() != "PostgreSQL") && (db_->dbmsName() != "PostGIS")) return; // only Postgres and Postgis

	if(!cmbEncoding->currentText().isEmpty()) encoding=cmbEncoding->currentText().latin1();
	_clientEncoding=db_->getClientEncoding().c_str();

	if(!encoding.empty())
	{
		db_->setClientEncoding(encoding);
	}
}


void ImportWindow::restoreClientEncoding()
{
	std::string  encode;
	if(db_ == 0) return;
	if((db_->dbmsName() != "PostgreSQL") && (db_->dbmsName() != "PostGIS")) return; // only Postgres and Postgis
	if(!_clientEncoding.isEmpty()) encode=_clientEncoding.latin1();
	
	
	if(!encode.empty())
	{
		db_->setClientEncoding(encode);
	}
}




bool ImportWindow::readGMLSettings( const QString & fileName, TeAttributeList & list )
{
	TeGDriverParams			params("OGR",fileName.latin1());
	TeGeoDataDriver			*driver;

	autoLinkRadioButton->setChecked(true);
	if((driver=TeGDriverFactory::make(&params)) !=0)
	{
		if((projection_=driver->getDataProjection()) == 0 )	projection_ = new TeNoProjection();
		if(projection_)
			    {
				    string projName = projection_->name() + " / " + (projection_->datum()).name();
				    projectionLineEdit->setText(projName.c_str());
			    }
		driver->getDataAttributesList(list);
	} else return false;
	if(driver !=0 ) delete driver;
	return true;
}
