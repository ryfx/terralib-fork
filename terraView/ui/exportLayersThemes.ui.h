/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

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

#include "TeTable.h"
#include <TeQtLayerItem.h>
#include <TeDriverSHPDBF.h>
#include <TeDriverMIDMIF.h>
#include <TeDriverSPRING.h>
#include <TeDriverCSV.h>
#include <TeView.h>
#include <TeLayer.h>
#include <TeDataTypes.h>
#include <TeOGRExportImportFunctions.h>


void ExportLayersThemes::init(TeDatabase* currentDatabase, QString lastPath)
{
	database_ = currentDatabase;

	rbLayer->setChecked(true);
	brTheme->setEnabled(false);
	loadLayers();
	help_ = NULL;
	view_ = NULL;
	lastOpenDir_ = lastPath;
	fileNameLineEdit->setText(lastPath);
	fileNameLineEdit->setEnabled(true);
	fileNameLineEdit->setReadOnly(true);
	this->setCaption(tr("Vectorial Export..."));

}

void ExportLayersThemes::init( TeDatabase * currentDatabase, TeView * view, QString lastPath)
{
	database_ = currentDatabase;

	brTheme->setChecked(true);
	rbLayer->setEnabled(false);
	view_ = view;
	loadThemes();
	expPropsCheckBox->setDisabled(false);
	lastOpenDir_ = lastPath;
	fileNameLineEdit->setText(lastPath);
	expPropsCheckBox->setChecked(false);
	expPropsCheckBox->setEnabled(false);
	fileNameLineEdit->setEnabled(true);
	fileNameLineEdit->setReadOnly(true);
	this->setCaption(tr("Vectorial Export..."));
}


void ExportLayersThemes::exportButton_clicked()
{

    if (rbLayer->isChecked())
    {
		exportLayers();
    }
    else
    {
		exportThemes();
    }

}


void ExportLayersThemes::helpButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("MultiVectorialExport.htm");
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


void ExportLayersThemes::cancelButton_clicked()
{
    accept();
}


void ExportLayersThemes::goButton_clicked()
{

	if (!objListBox->hasFocus())
		return;

	if(objListBox->count() == 0)
		return;

	if(objListBox->currentText() == "")
		return;

	objInListBox->insertItem(objListBox->currentText()); 
	objListBox->removeItem(objListBox->currentItem());
	updateFileName(true);		
}


void ExportLayersThemes::backButton_clicked()
{

	if (!objInListBox->hasFocus())
		return;

	if(objInListBox->count() == 0)
		return;

	if (objInListBox->currentText() == "" )
		return;

	objListBox->insertItem(objInListBox->currentText()); 
	objInListBox->removeItem(objInListBox->currentItem());
	updateFileName(true);

}


void ExportLayersThemes::outputFileButton_clicked()
{
	unsigned int	size;
	QString			curPath;
	QString			fileName;
	QString			filter="cvs";
	std::string		fname;
	QFileDialog		oneFileDlg(this,"TeExport",true);
	int				index=0;

	if(sprRadioButton->isChecked() == true) index=0;
	else if(shpRadioButton->isChecked() == true) index=1;
	else if(mifRadioButton->isChecked() == true) index=2;
	else if(csvRadioButton->isChecked() == true) index=3;
	else if(gmlRadioButton->isChecked() == true) index=4;

	size=objInListBox->count();
	if(size == 1)
	{
		fileName=objInListBox->text(0);
		oneFileDlg.setFilters(QString("Spring File (*.spr);;" "Shape File (*.shp);;" "MapInfo File (*.mif);;" "Text File (*.csv);;" "Geography Markup Language(*.gml)"));
		oneFileDlg.setSelectedFilter(index);
		oneFileDlg.setSelection(fileName);
		oneFileDlg.setMode(QFileDialog::AnyFile);
		if(oneFileDlg.exec()==QFileDialog::Accepted)
		{
			//curPath=QFileDialog::getSaveFileName(fileName,"Spring File (*.spr);;" "Shape File (*.shp);;" "MapInfo File (*.mif);;" "Text File (*.csv);;" "Geography Markup Language(*.gml)",this,tr("Export File"),tr("Export"),&filter);
			curPath=oneFileDlg.selectedFile();
			if(curPath.isEmpty() == false)
			{
				filter=oneFileDlg.selectedFilter();
				fname=filter.latin1();
				if(fname.find("spr") != string::npos) sprRadioButton->setChecked(true);
				else if (fname.find("shp") != string::npos) shpRadioButton->setChecked(true);
				else if (fname.find("mif") != string::npos) mifRadioButton->setChecked(true);
				else if (fname.find("csv") != string::npos) csvRadioButton->setChecked(true);
				else if (fname.find("gml") != string::npos) gmlRadioButton->setChecked(true);

				fname= TeGetName(curPath.latin1());
				fileNameLineEdit->setText(fname.c_str());
				lastOpenDir_=TeGetPath(fname.c_str()).c_str();
				updateFileName(true);
			}else fileNameLineEdit->clear();
		}

	}
	else
	{
		curPath = fileNameLineEdit->text();
		QString s = QFileDialog::getExistingDirectory(curPath, this, tr("Save As"), tr("Path"));
		fname = TeGetName(s.latin1());
		if (fname.empty())		QMessageBox::warning(this, tr("Warning"), tr("Choose a valid path for the output file!"));
		else					fileNameLineEdit->setText(fname.c_str());
	}

}


void ExportLayersThemes::exportLayers()
{
	if(objInListBox->count() == 0)
	{
		QMessageBox::information(this, tr("Information"),tr("There is no layer to be exported!"));
		return;
	}

	TeLayer * curLayer_;

	TeLayerMap lMap = database_->layerMap();
	TeLayerMap::iterator it;

	string pathName = fileNameLineEdit->text().latin1();
	string fileName = "";
	string tableName = "";
	int itensExported = 0;

	if (pathName.empty())
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Select the path of the output file!"));
		return;
	}

//////////////////////////////////////////////////////////////
    bool allExported = true;

	if(formatButtonGroup->selectedId() == 3)
		QMessageBox::information(this, tr("Information"),tr("Only the layers with point representation will be exported!"));

    for(unsigned int i=0; i<objInListBox->count(); i++)
    {

		for(it=lMap.begin(); it!=lMap.end(); ++it)
		{

			if(it->second->name() == objInListBox->item(i)->text().latin1())
			{
				//carregar o layer da lista
				curLayer_ = (TeLayer*)it->second;
				break;
			}
		}
		
		if(formatButtonGroup->selectedId() == 3 && !curLayer_->hasGeometry(TePOINTS))
			continue;

		TeAttrTableVector tables;
		curLayer_->getAttrTables(tables);
		if(tables.size()>0)
			tableName = tables[0].name();
		else
			tableName = "";

		fileName = pathName + "/" + tr("LAYER_").latin1() + objInListBox->item(i)->text().latin1();

		if(objInListBox->count() == 1)
		{
			fileName=fileNameLineEdit->text().latin1();
			fileName.erase(fileName.end() - 4,fileName.end());
		}

    	if (!(curLayer_->hasGeometry(TePOINTS) 
		|| curLayer_->hasGeometry(TeLINES) 
		|| curLayer_->hasGeometry(TeCELLS) 
		|| curLayer_->hasGeometry(TePOLYGONS)))
		{
			allExported = false;
		}


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


		TeWaitCursor wait;

		bool res = false;
		if (fmt == 0)
		{

			TeSpringModels sprCat = TeSpringModels(catSPRComboBox->currentItem());
			string fname = TeGetName(fileName.c_str());
			if(TeProgress::instance())
			{
				QString caption = tr("Exporting");
				TeProgress::instance()->setCaption(caption.latin1());
				QString msg = tr("Exporting vector to ASCII-SPRING format. Please, wait!");
				TeProgress::instance()->setMessage(msg.latin1());
			}
			res = TeExportSPR (curLayer_,TeGetName(fileName.c_str()), tableName, sprCat, "Objeto");

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

			res = TeExportMIF (curLayer_, TeGetName(fileName.c_str()), tableName);

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

			res = TeExportShapefile(curLayer_, TeGetName(fileName.c_str()), tableName);

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
			string filename = fileName + ".txt";
			res = TeExportLayerToCSV(curLayer_,filename,';');
			itensExported++;
		}else if(fmt == 4)
		{
			if(TeProgress::instance())
			{
				QString caption = tr("Exporting");
				TeProgress::instance()->setCaption(caption.latin1());
				QString msg = tr("Exporting vector to GML format. Please, wait!");
				TeProgress::instance()->setMessage(msg.latin1());
			}
			std::string filename=fileName + ".gml";
			res=TeExportToOGR(curLayer_,filename.c_str(), "GML");
			itensExported++;
		}
		else
		{
			wait.resetWaitCursor();
			QMessageBox::warning(this, tr("Warning"), tr("Select a format!"));
		}

		wait.resetWaitCursor();
		if(TeProgress::instance())
			TeProgress::instance()->reset();

		if (!res)
			allExported = false;
		else
			if (expPropsCheckBox->isChecked())
				exportProperties(curLayer_, fileName);

	}
		
	    
	if (allExported)
	{
		if(formatButtonGroup->selectedId() == 3 && itensExported == 0)
		{
			QMessageBox::information(this, tr("Information"), tr("There is no selected layer with point representation!"));
		}
		else
		{
			QMessageBox::information(this, tr("Information"), tr("The data were exported successfully!"));
		}

		lastOpenDir_ = fileNameLineEdit->text();
		hide();
	}
	else
		QMessageBox::critical(this, tr("Error"), tr("Fail to export the data!"));

	
}




void ExportLayersThemes::exportThemes()
{

	if(objInListBox->count() == 0)
	{
		QMessageBox::information(this, tr("Information"),tr("There is no theme to be exported!"));
		return;
	}

    bool allExported = true;
	TeThemeMap tMap = database_->themeMap();
	TeThemeMap::iterator it;
	TeAttrTableVector tables;
	int itensExported = 0;

	TeTheme * curTheme_;
	string pathName = fileNameLineEdit->text().latin1();
	string fileName = "";
	string tableName = "";
	TeLayer * tl;
	
	if (pathName.empty())
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("Select the path of the output file!"));
		return;
	}
	
	if(formatButtonGroup->selectedId() == 3)
		QMessageBox::information(this, tr("Information"),tr("Only the themes with point representation will be exported!"));

	for(unsigned int i=0; i<objInListBox->count(); i++)
    {

		for(it=tMap.begin(); it!=tMap.end(); ++it)
		{
			if(it->second->type() != TeTHEME && it->second->type() != TeEXTERNALTHEME)
				continue;

			if(it->second->name() == objInListBox->item(i)->text().latin1() && it->second->view() == view_->id())
			{
				curTheme_ = (TeTheme*)it->second;
				tl = curTheme_->layer();
				tl->getAttrTables(tables);

				if(tables.size()>0)
					tableName = tables[0].name();
				else
					tableName = "";

				break;
			}
		}
			
		if(formatButtonGroup->selectedId() == 3 && !tl->hasGeometry(TePOINTS))
			continue;

		fileName = pathName + "/" + tr("THEME_").latin1() + objInListBox->item(i)->text().latin1();

		if(objInListBox->count() == 1)
		{
			fileName=fileNameLineEdit->text().latin1();
			fileName.erase(fileName.end() - 4,fileName.end());
		}

		QString fname = fileName.c_str();

		TeTheme* curBaseTheme = curTheme_;

		fname = fileName.c_str();

		TeWaitCursor wait;
		bool res=false;

		if (formatButtonGroup->selectedId() == 0)
		{

			string colName =  "Objeto";

			if(TeProgress::instance())
			{
				QString caption = tr("Exporting");
				TeProgress::instance()->setCaption(caption.latin1());
				QString msg = tr("Exporting vector to ASCII-SPRING format. Please, wait!");
				TeProgress::instance()->setMessage(msg.latin1());
			}
			res = TeExportThemeToSPRING(curBaseTheme, TeSpringModels(catSPRComboBox->currentItem()), colName, TeAll,fname.latin1());

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
			res = TeExportThemeToMIF(curBaseTheme, TeAll, fname.latin1());
		
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
			
			res = TeExportThemeToShapefile(curBaseTheme, TeAll, fname.latin1());
		
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
			res = TeExportThemeToCSV(curBaseTheme, TeAll, fname.latin1(),';');
			itensExported++;
			
		}else if(formatButtonGroup->selectedId() == 4)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Functionality isn't implemented yet!"));
			return;
		}
		else
		{
			wait.resetWaitCursor();
			QMessageBox::warning(this, tr("Warning"), tr("Select a format!"));
		}

		wait.resetWaitCursor();
		if(TeProgress::instance())
			TeProgress::instance()->reset();

		if(!res)
			allExported = false;
		
	}
    
	if (allExported)
	{
		if(formatButtonGroup->selectedId() == 3 && itensExported == 0)
		{
			QMessageBox::information(this, tr("Information"), tr("There is no selected theme with point representation!"));
		}
		else
		{
			QMessageBox::information(this,tr("Information"),tr("Themes were successfully exported!"));
		}
		
		lastOpenDir_ = fileNameLineEdit->text();
		hide();
	}
	else
	{
		QMessageBox::critical(this, tr("Error"), tr("Fail to export the data!"));
		reject();
	}
		
}

void ExportLayersThemes::loadThemes()
{
	objInListBox->clear();
	objListBox->clear();

	if(!view_)
		return;
	
	TeTheme* theme;

	TeThemeMap tMap = database_->themeMap();
	TeThemeMap::iterator it;

	for(it=tMap.begin(); it!=tMap.end(); ++it)
	{
		if(it->second->view() == view_->id())
		{
			if(it->second->type() != TeTHEME && it->second->type() != TeEXTERNALTHEME)
				continue;

			theme = (TeTheme*)it->second;

			TeLayer * tl = theme->layer();
			
			if(!tl->hasGeometry(TeRASTER))
			{
				if(theme->visibility())
					objInListBox->insertItem(theme->name().c_str());
				else
					objListBox->insertItem(theme->name().c_str());

			}
		}
	}
	updateFileName(false);	
}

void ExportLayersThemes::loadLayers()
{
	TeLayerMap& lm = database_->layerMap();
	TeLayerMap::iterator layerIt = lm.begin();
	while(layerIt != lm.end())
	{
		if(!layerIt->second->hasGeometry(TeRASTER))
			objListBox->insertItem(layerIt->second->name().c_str());				

		layerIt++;
	}

}


void ExportLayersThemes::catSPRComboBox_activated( int )
{

}


void ExportLayersThemes::exportProperties(TeLayer * curLayer_, std::string fileName)
{
	if (!curLayer_)
		return;
	
	if (fileName.empty())
		return;

	fileName = TeGetName(fileName.c_str()) + "_props.txt";

	TeDatabasePortal* portal = database_->getPortal();

	if (!portal)
		return;

	string line;

	try
	{
		TeAsciiFile properties(fileName,"w");
		if(database_->tableExist("te_layer_metadata"))
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
				line = tr("Polygons number: ").latin1();
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
				line = tr("Lines number: ").latin1();
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

void ExportLayersThemes::sprRadioButton_toggled( bool )
{
		catSPRComboBox->setEnabled(true);
		catSPRComboBox->setCurrentItem(0);
		updateFileName(false);
}


void ExportLayersThemes::mifRadioButton_toggled( bool )
{
		catSPRComboBox->setEnabled(false);
		updateFileName(false);
}


void ExportLayersThemes::shpRadioButton_toggled( bool )
{
		catSPRComboBox->setEnabled(false);
		updateFileName(false);
}

QString ExportLayersThemes::getLastOpenDir()
{
	QString path;

	path=TeGetPath(lastOpenDir_.latin1()).c_str();
	return path;
}


void ExportLayersThemes::csvRadioButton_clicked()
{
	catSPRComboBox->setEnabled(false);
	updateFileName(false);
}


void ExportLayersThemes::updateFileName(bool changePath)
{
	outputFileButton->setText(tr("Path..."));
	if(objInListBox->count() ==1 ) 
	{
			std::string fileName=lastOpenDir_ + "/" + objInListBox->text(0).latin1();
			if(sprRadioButton->isChecked() == true) fileName +=".spr";
			else if(mifRadioButton->isChecked() == true) fileName +=".mif";
			else if(shpRadioButton->isChecked() == true) fileName +=".shp";
			else if(csvRadioButton->isChecked() == true) fileName +=".csv";
			else if(gmlRadioButton->isChecked() == true) fileName +=".gml";
			fileNameLineEdit->setText(fileName.c_str());
			outputFileButton->setText(tr("File..."));
	}
	else if(changePath == true ) fileNameLineEdit->setText(lastOpenDir_);
}

void ExportLayersThemes::gmlRadioButton_clicked()
{
	catSPRComboBox->setEnabled(false);
	updateFileName(false);
}
