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

#include "TeTable.h"
#include "TeProgress.h"
#include "TeAsciiFile.h"
#include <TeQtLayerItem.h>

#include <TeDriverSHPDBF.h>
#include <TeDriverMIDMIF.h>
#include <TeDriverSPRING.h>
#include <TeDriverCSV.h>

void ExportWindow::init(TeLayer* curLayer)
{
		help_ = 0;
	curFormat_ = "spr";
	catSPRComboBox->setCurrentItem(0);
	attrComboBox->setEnabled(false);
	catObjLineEdit->setEnabled(true);
	catObjLineEdit->setText("Object");
	curLayer_ = curLayer;

	TeLayerMap& layerMap = db_->layerMap();
	if (layerMap.empty())
	{
	    QMessageBox::critical(this, tr("Error"),tr("The database has no layers!"));
	    return;
	}
	TeDatabasePortal* portal = db_->getPortal();
	string sql;
	
	TeLayerMap::iterator itlay = layerMap.begin();
	TeLayerMap::iterator itcur = layerMap.end();
	TeLayerMap::iterator itvec = layerMap.end();
	int i = 0, j = -1, f=-1;
	while ( itlay != layerMap.end() )
	{
		sql = "SELECT te_layer.name FROM (te_layer INNER JOIN te_representation ON te_layer.layer_id = te_representation.layer_id) INNER JOIN te_layer_table ON te_layer.layer_id = te_layer_table.layer_id WHERE te_layer.layer_id = ";
		sql += Te2String((*itlay).second->id());
		sql += " AND ((te_layer_table.attr_table_type=1 Or te_layer_table.attr_table_type=3) AND (te_representation.geom_type=1 Or te_representation.geom_type=2 Or te_representation.geom_type=4 Or te_representation.geom_type=8 Or te_representation.geom_type=256)) ";
		if (!portal->query(sql) || !portal->fetchRow())
		{
			portal->freeResult();
			++itlay;
			continue;
		}
		f = i;
		itvec = itlay;
		layerComboBox->insertItem((*itlay).second->name().c_str());
		if (curLayer_ && curLayer_->name() == (*itlay).second->name())
		{					// if there is a current layer 
			itcur = itlay;
			j = i;
		}
		++i;
		++itlay;
		portal->freeResult();
	}
	delete portal;
	if (i==0)
	{
	    QMessageBox::critical(this, tr("Error"),   
			tr("The database has no layers with static table and vectorial representation!"));
   		hide();
	}
	else
	{
		if (j>=0) // select current layer
		{
			layerComboBox->setCurrentItem(j);
			curLayer_ = (*itcur).second;
		}
		else	// select last layer in the combo
		{
			layerComboBox->setCurrentItem(f);
			curLayer_ = (*itvec).second;
		}

		tableComboBox->clear();
		TeAttrTableVector attrTables;
		curLayer_->getAttrTables(attrTables);
		TeAttrTableVector::iterator itattr = attrTables.begin();
		while (itattr != attrTables.end())
		{
			if ((*itattr).tableType() == TeAttrStatic || (*itattr).tableType() == TeAttrEvent || (*itattr).tableType() == TeFixedGeomDynAttr)
				tableComboBox->insertItem((*itattr).name().c_str());
			++itattr;
		}
		if (curLayer_->hasGeometry(TePOINTS))
			asciiRadioButton->setEnabled(true);
		else 
			asciiRadioButton->setEnabled(false);
	}
	string filename = string(lastOpenDir_.latin1()) + "/" + string(layerComboBox->currentText().latin1()) + ".spr"; 
	fileNameLineEdit->setText(filename.c_str());	
}


void ExportWindow::exportButton_clicked()
{	
	if (!(curLayer_->hasGeometry(TePOINTS) 
		|| curLayer_->hasGeometry(TeLINES) 
		|| curLayer_->hasGeometry(TeCELLS) 
		|| curLayer_->hasGeometry(TePOLYGONS)))
	{
		QMessageBox::critical(this, tr("Error"),
			tr("The layer has no vectorial representation!"));
		return;
	}

	string fileName = fileNameLineEdit->text().latin1();
	if (fileName.empty())
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Select the name of the output file!"));
		return;
	}

	//Verify the name
	std::string baseName = TeGetBaseName(fileName.c_str());

	bool changed = false;
	std::string invalidChar = "";
	TeCheckName(baseName, changed, invalidChar);

	if(changed)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Output file name invalid!"));
		return;
	}

	//End

	QString ext;
	int fmt = formatButtonGroup->selectedId();
	QString fname = fileName.c_str();
	if (fmt == 2 )
	{
		fname = TeGetName(fileName.c_str()).c_str();
		if (curLayer_->hasGeometry(TePOLYGONS) || curLayer_->hasGeometry(TeCELLS))
			fname += "_pol.shp";
		if (curLayer_->hasGeometry(TeLINES))
			fname += "_lin.shp";
		if (curLayer_->hasGeometry(TeLINES))
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

	TeWaitCursor wait;
	bool res = false;
	if (fmt == 0)
	{
		string colName = "";
		int cat = catSPRComboBox->currentItem();
		if (cat < 2)
		{
			colName =  catObjLineEdit->text().latin1();
			if (colName.empty())
				colName = "Objeto";
		}
		else
			colName = attrComboBox->currentText().latin1();
		
		TeSpringModels sprCat = TeSpringModels(catSPRComboBox->currentItem());
		string fname = TeGetName(fileName.c_str());
		if(TeProgress::instance())
		{
			QString caption = tr("Exporting");
			TeProgress::instance()->setCaption(caption.latin1());
			QString msg = tr("Exporting vector to ASCII-SPRING format. Please, wait!");
			TeProgress::instance()->setMessage(msg.latin1());
		}
		res = TeExportSPR (curLayer_,TeGetName(fileName.c_str()), 
			string(tableComboBox->currentText().latin1()), sprCat, colName);
	}
	else if (fmt == 1)
	{
		if(TeProgress::instance())
		{
			QString caption = tr("Exporting");
			TeProgress::instance()->setCaption(caption.latin1());
			QString msg = tr("Exporting vector to MID/MIF format. Please, wait!");
			TeProgress::instance()->setMessage(msg.latin1());
		}
		res = TeExportMIF (curLayer_,TeGetName(fileName.c_str()),
			               string(tableComboBox->currentText().latin1()));
	}
	else if (fmt == 2)
	{
		if(TeProgress::instance())
		{
			QString caption = tr("Exporting");
			TeProgress::instance()->setCaption(caption.latin1());
			QString msg = tr("Exporting vector to Shapefile format. Please, wait!");
			TeProgress::instance()->setMessage(msg.latin1());
		}
		res = TeExportShapefile(curLayer_,TeGetName(fileName.c_str()),string(tableComboBox->currentText().latin1()));		
	}
	else if (fmt == 3)
	{
		if(TeProgress::instance())
		{
			QString caption = tr("Exporting");
			TeProgress::instance()->setCaption(caption.latin1());
			QString msg = tr("Exporting vector to ASCII format. Please, wait!");
			TeProgress::instance()->setMessage(msg.latin1());
		}
		string filename = fileName;
		res = TeExportLayerToCSV(curLayer_,filename,';');
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
		if (expPropsCheckBox->isChecked())
			exportProperties();
		QMessageBox::information(this, tr("Information"),
			tr("The data were exported successfully!"));
		QString curPath = fileNameLineEdit->text();
		int n = curPath.findRev('/');
		curPath.truncate(n);
		lastOpenDir_ = curPath;
		hide();
	}
	else
		QMessageBox::critical(this, tr("Error"), tr("Fail to export the data!"));
}

void ExportWindow::formatComboBox_activated( int fmt)
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


void ExportWindow::catSPRComboBox_activated( int cat)
{
	if (!curLayer_)
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
		TeTable table;
		if (curLayer_->getAttrTablesByName(string(tableComboBox->currentText().latin1()),table))
		{
			TeAttributeList attrList = table.attributeList();
			unsigned int i;
			if (cat == 3)		// only numerical attributes
			{	
				attrTextLabel->setText(tr("Elevation:"));		
				for (i=0; i<attrList.size(); i++)
				{   
					TeAttribute at = attrList[i];
					if (at.rep_.type_ == (TeAttrDataType)TeREAL  || at.rep_.type_ == (TeAttrDataType)TeDOUBLE ||
					    at.rep_.type_ == (TeAttrDataType)TeFLOAT || at.rep_.type_ == (TeAttrDataType)TeINT)
					{
						QString	name(at.rep_.name_.c_str());
						attrComboBox->insertItem(name);
					}
				}
			}
			else
			{
				attrTextLabel->setText(tr("Class:"));		
				for (i=0; i<attrList.size(); i++)
				{   
					TeAttribute at = attrList[i];
					QString	name(at.rep_.name_.c_str());
					attrComboBox->insertItem(name);
				}
			}
		}
	}
}


void ExportWindow::outputFileButton_clicked()
{
	QString ext;
	int fmt = formatButtonGroup->selectedId();
	if (fmt == 0 )
		ext = ".spr";
	else if (fmt == 1)
		ext = ".mid";
	else if (fmt ==2)
		ext = ".shp";
	else
		ext = ".txt";

	QString filter = "*";
	filter += ext;
	QString curPath = fileNameLineEdit->text();
	if (!curPath.isEmpty())
	{
		int n = curPath.findRev('/');
		curPath.truncate(n);
	}
	QString s = QFileDialog::getSaveFileName( curPath,filter,this,tr("Save As"), tr("File Name"));
	string fname = TeGetName(s.latin1());
	if (fname.empty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Choose a valid name and path for the output file!"));
	}
	else
	{
		fname.append(ext.latin1());
		fileNameLineEdit->setText(fname.c_str());
	}
}


void ExportWindow::layerComboBox_activated( const QString &layerName )
{
	tableComboBox->clear();
	TeLayerMap& layerMap = db_->layerMap();
	if (layerMap.empty())
	{
	    QMessageBox::warning(this,  tr("Warning"),
			 tr("The database has no layers!"));
	    return;
	}
	curLayer_ = 0;
	TeLayerMap::iterator itlay = layerMap.begin();
	while ( itlay != layerMap.end() )
	{
		string name = (*itlay).second->name();
		if (name == string(layerName.latin1()))
		{
			curLayer_ = (*itlay).second;
			break;
		}
		++itlay;
	}
	if (!curLayer_)
		return;
	TeAttrTableVector attrTables;
	curLayer_->getAttrTables(attrTables,TeAttrStatic);
	TeAttrTableVector::iterator itattr = attrTables.begin();
	while (itattr != attrTables.end())
	{
		if ((*itattr).tableType() != TeAttrExternal)
			tableComboBox->insertItem((*itattr).name().c_str());
		++itattr;
	}
	if (curLayer_->hasGeometry(TePOINTS))
		asciiRadioButton->setEnabled(true);
	else 
		asciiRadioButton->setEnabled(false);
}


void ExportWindow::tableComboBox_activated( const QString& /* tableName */)
{
	int c = catSPRComboBox->currentItem();
	catSPRComboBox_activated(c);
}


void ExportWindow::formatButtonGroup_clicked( int n)
{
    if (n != 0)
	    sprGroupBox->setEnabled(false);
	else
	    sprGroupBox->setEnabled(true);
	QString curFilename = fileNameLineEdit->text();
	if (!curFilename.isEmpty())
	{
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


void ExportWindow::exportProperties()
{
	if (!curLayer_)
		return;
	string fileName = fileNameLineEdit->text().latin1();
	if (fileName.empty())
		return;
	fileName = TeGetName(fileName.c_str()) + "_props.txt";

	TeDatabasePortal* portal = db_->getPortal();
	if (!portal)
		return;
	string line;
	try
	{
		TeAsciiFile properties(fileName,"w");
		if(db_->tableExist("te_layer_metadata"))
		{
			string selm = "SELECT * FROM te_layer_metadata WHERE layer_id = " + Te2String(curLayer_->id());
			if(portal->query(selm) && portal->fetchRow())
			{
				line = tr("Name").latin1();
				line += ": " + string(portal->getData(1));
				properties.writeString(line);
				properties.writeNewLine();

				line = tr("Author").latin1();
				line += ": " + string(portal->getData(2));
				properties.writeString(line);
				properties.writeNewLine();

				line = tr("Source").latin1();
				line += ": " + string(portal->getData(3));
				properties.writeString(line);
				properties.writeNewLine();			

				line = tr("Quality").latin1();
				line += ": " + string(portal->getData(4));
				properties.writeString(line);
				properties.writeNewLine();			

				line = tr("Description").latin1();
				line += ": " + string(portal->getData(5));
				properties.writeString(line);
				properties.writeNewLine();			

				line = tr("Date").latin1();
				line += ": " + string(portal->getData(6));
				properties.writeString(line);
				properties.writeNewLine();			

				line = tr("Hour").latin1();
				line += ": " + string(portal->getData(7));
				properties.writeString(line);
				properties.writeNewLine();			

				line = tr("Transf").latin1();
				line += ": " + string(portal->getData(8));
				properties.writeString(line);
				properties.writeNewLine();	
				properties.writeNewLine();	
			}
		}

		line = tr("Infolayer").latin1();
		line += ": " + curLayer_->name();
		properties.writeString(line);
		properties.writeNewLine();	
		
		line = tr("Infolayer Identifier").latin1();
		line += ": " + Te2String(curLayer_->id());
		properties.writeString(line);
		properties.writeNewLine();	
		properties.writeNewLine();

		int ndecFields = 3;
		if (curLayer_->projection()->name() == "NoProjection" ||
			curLayer_->projection()->name() == "LatLong")
			ndecFields = 10;

		line = tr("Initial Coordinate(X)").latin1();
		line += ": " + Te2String(curLayer_->box().x1_,ndecFields);
		properties.writeString(line);
		properties.writeNewLine();

		line = tr("Initial Coordinate(Y)").latin1();
		line += ": " + Te2String(curLayer_->box().y1_,ndecFields);
		properties.writeString(line);
		properties.writeNewLine();

		line = tr("Final Coordinate(X)").latin1();
		line += ": " + Te2String(curLayer_->box().x2_,ndecFields);
		properties.writeString(line);
		properties.writeNewLine();
		
		line = tr("Final Coordinate(Y)").latin1();
		line += ": " + Te2String(curLayer_->box().y2_,ndecFields);
		properties.writeString(line);
		properties.writeNewLine();
		properties.writeNewLine();	
			
		line = tr("Projection").latin1();
		line += ": " + curLayer_->projection()->name();
		properties.writeString(line);
		properties.writeNewLine();

		line = tr("Projection Identifier").latin1();
		line += ": " + Te2String(curLayer_->projection()->id());
		properties.writeString(line);
		properties.writeNewLine();

		if (curLayer_->projection()->name() != "NoProjection")
		{
			TeDatum datum = curLayer_->projection()->datum();
			line = tr("Datum").latin1();
			line += ": " + datum.name();
			properties.writeString(line);
			properties.writeNewLine();

			line = tr("Unit").latin1();
			line += ": " + curLayer_->projection()->units();
			properties.writeString(line);
			properties.writeNewLine();
			
			line = tr("Origin Longitude").latin1();
			line += ": " + Te2String(curLayer_->projection()->lon0()*TeCRD);
			properties.writeString(line);
			properties.writeNewLine();

			line = tr("Origin Latitude").latin1();
			line += ": " + Te2String(curLayer_->projection()->lat0()*TeCRD);
			properties.writeString(line);
			properties.writeNewLine();

			line = tr("Offset X").latin1();
			line += ": " + Te2String(curLayer_->projection()->offX());
			properties.writeString(line);
			properties.writeNewLine();

			line = tr("Offset Y").latin1();
			line += ": " + Te2String(curLayer_->projection()->offY());
			properties.writeString(line);
			properties.writeNewLine();

			line = tr("Standard Parallel 1").latin1();
			line += ": " + Te2String(curLayer_->projection()->stLat1()*TeCRD);
			properties.writeString(line);
			properties.writeNewLine();

			line = tr("Standard Parallel 2").latin1();
			line += ": " + Te2String(curLayer_->projection()->stLat2()*TeCRD);
			properties.writeString(line);
			properties.writeNewLine();

			line = tr("Scale").latin1();
			line += ": " + Te2String(curLayer_->projection()->scale());
			properties.writeString(line);
			properties.writeNewLine();

			line = tr("Hemisphere").latin1();
			line += ": ";
			if(curLayer_->projection()->hemisphere() == TeNORTH_HEM)
				line += string(tr("North").latin1());
			else
				line += string(tr("South").latin1());
			properties.writeString(line);
			properties.writeNewLine();
		}

		properties.writeNewLine();
		int rep = curLayer_->geomRep();
		if (rep&TePOLYGONS || rep&TeLINES || rep&TePOINTS || rep&TeCELLS)
		{
			string conta;
			if(curLayer_->hasGeometry(TePOLYGONS))
			{
				line = tr("Polygons number: ").latin1();
				conta = "SELECT COUNT(*) FROM " + curLayer_->tableName(TePOLYGONS);
				portal->freeResult();
				if(portal->query(conta) && portal->fetchRow())
				{
					line += portal->getData(0);
					properties.writeString(line);
					properties.writeNewLine();
				}
			}
			if(curLayer_->hasGeometry(TeLINES))
			{
				line = tr("Lines number: ").latin1();
				conta = "SELECT COUNT(*) FROM " + curLayer_->tableName(TeLINES);
				portal->freeResult();
				if(portal->query(conta) && portal->fetchRow())
				{
					line += portal->getData(0);
					properties.writeString(line);
					properties.writeNewLine();
				}
			}
		
			if(curLayer_->hasGeometry(TePOINTS))
			{
				line = tr("Points number: ").latin1();
				conta = "SELECT COUNT(*) FROM " + curLayer_->tableName(TePOINTS);
				portal->freeResult();
				if(portal->query(conta) && portal->fetchRow())
				{
					line += portal->getData(0);
					properties.writeString(line);
					properties.writeNewLine();
				}
			}
			if(curLayer_->hasGeometry(TeCELLS))
			{
				line = tr("Cells number: ").latin1();
				conta = "SELECT COUNT(*) FROM " + curLayer_->tableName(TeCELLS);
				portal->freeResult();

				if(portal->query(conta) && portal->fetchRow())
				{
					line += portal->getData(0);
					properties.writeString(line);
					properties.writeNewLine();
				}

				string res = "SELECT res_x, res_y FROM te_representation WHERE";
				res += " geom_table = '" + curLayer_->tableName(TeCELLS) + "'";
				portal->freeResult();
				if(portal->query(res) && portal->fetchRow())
				{
					line = tr("Horizontal resolution of the cells: ").latin1();
					line += portal->getData(0);
					properties.writeString(line);
					properties.writeNewLine();

					line = tr("Vertical resolution of the cells: ").latin1();
					line += portal->getData(1);
					properties.writeString(line);
					properties.writeNewLine();
					
				}
			}
		}
	}
	catch(...)
	{
	}
	delete portal;
}

void ExportWindow::helpButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("exportWindow.htm");
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


void ExportWindow::setParams( TeDatabase * db, QString & lastOpenDir )
{
	db_ = db;
	lastOpenDir_ = lastOpenDir;
}


QString ExportWindow::getLastOpenDir()
{
	return lastOpenDir_;
}
