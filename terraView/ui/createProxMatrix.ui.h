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

#include "TeQuerierParams.h"
#include "TeDatabaseUtils.h"
#include <TeQtGrid.h>
#include <qmessagebox.h>
#include <display.h>
#include <qfiledialog.h>
#include <help.h>

void CreateProxMatrix::init()
{
	 help_ = 0;
	 //preencher a combo de temas da vista corrente
	 mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	 vector<TeGeomRep> reps;
	 reps.push_back (TePOLYGONS);
	 reps.push_back (TeLINES);
	 reps.push_back (TePOINTS);
	 reps.push_back (TeCELLS);
	 temaComboBox->clear();
	 	 
	 string themeName = "";
	 if(mainWindow_->currentTheme())
		themeName = mainWindow_->currentTheme()->getTheme()->name();
	 fillThemeCombo(reps, mainWindow_->currentView(), temaComboBox, themeName, true);

	  //preencher a combo de atributos
	 if(temaComboBox->count()<1)
	 {
		QMessageBox::warning(this, tr("Warning"),
		tr("There is no theme in the current view! "));
		 hide();
		 return;
	 }
	
	 string temaName =  string(temaComboBox->currentText().ascii());
	 curTheme_ = mainWindow_->currentView()->get(temaName);
	 
	 if(!curTheme_)
	     return;  //mensagem de erro!!!!

	 objIdComboBox->clear();
	 TeAttrTableVector tables;
	 curTheme_->getAttTables(tables, TeAttrStatic); 
	 //sequence number
	 string aux = tr(" sequential number (1,2,3...) ");
	 objIdComboBox->insertItem(aux.c_str());
	 for(unsigned int i=0; i<tables.size(); ++i)
	 {
	     TeAttributeList columns = tables[i].attributeList();
	     for(unsigned int j=0; j<columns.size(); ++j)
	     {
			string name = tables[i].name()+"."+columns[j].rep_.name_;
			objIdComboBox->insertItem(name.c_str());
		 }
	  }
	 objIdComboBox->setCurrentItem(0);	 
	 	 
	 //estratégia de construao default
	 AdjacenciaRadioButton->setChecked(true); 
	 OrdemTextLabel->setEnabled(false);
	 OrdemSpinBox->setEnabled(false);
	 
	 DistanciaRadioButton->setChecked(false);
	 DistanciaTextLabel->setEnabled(false);
	 DistanciaLineEdit->clear();
	 DistanciaLineEdit->setEnabled(false);
	 
	 VizinhoRadioButton->setChecked(false);
	 VizinhosTextLabel->setEnabled(false);
	 VizinhosLineEdit->clear();
	 VizinhosLineEdit->setEnabled(false);
		 
	 //estratégia de ponderação default
	 NaoPonderaRadioButton->setChecked(true);
	 PonderaQuadradoRadioButton->setChecked(false);
	 PonderaInversoRadioButton->setChecked(false);
	 
	 //salvar arquivos
	 SalvarArquivoCheckBox->setChecked(false);
	 fileNameLineEdit->clear();
	 fileNameLineEdit->setEnabled(false);
	 objIdComboBox->setEnabled(false);
	 pathLineEdit->clear();
	 pathLineEdit->setEnabled(false);
	 pathPushButton->setEnabled(false);

	//atributos
	db_= mainWindow_->currentDatabase();
	fileName_="";
	attrName_="";
	path_=""; 
	constStrategy_=TeAdjacencyStrategy;
	distance_=0.;
	num_neigs_=0;
	calculateDistance_=false;
	weightStrategy_=TeNoWeightsStrategy;
	norm_=true;
	show();
}

void  CreateProxMatrix::SalvarArquivoCheckBox_checked(bool status)
{
    if(status)
    {
		fileNameLineEdit->setEnabled(true);
		objIdComboBox->setEnabled(true);
		pathLineEdit->setEnabled(true);
		pathPushButton->setEnabled(true);
    }
    else
    {
		fileNameLineEdit->setEnabled(false);
		objIdComboBox->setEnabled(false);
		pathLineEdit->setEnabled(false);
		pathPushButton->setEnabled(false);
    }
    
}

void CreateProxMatrix::temaComboBox_activated( const QString& name)
{
    //preencher a combo de atributos
    objIdComboBox->clear();
    curTheme_ = mainWindow_->currentView()->get(string(name.ascii()));
	TeAttrTableVector tables;
	curTheme_ ->getAttTables(tables, TeAttrStatic); 
	//sequence number
	string aux = tr(" sequential number (1,2,3...) ");
	objIdComboBox->insertItem(aux.c_str());
	for(unsigned int i=0; i<tables.size(); ++i)
	{
	    TeAttributeList columns = tables[i].attributeList();
	    for(unsigned int j=0; j<columns.size(); ++j)
	    {
			string name = tables[i].name()+"."+columns[j].rep_.name_;
			objIdComboBox->insertItem(name.c_str());
		}
	}
	objIdComboBox->setCurrentItem(0);	 
 }

void CreateProxMatrix::AdjacenciaRadioButton_clicked()
{
       constStrategy_=TeAdjacencyStrategy;
      
       OrdemTextLabel->setEnabled(false);
       OrdemSpinBox->setEnabled(false);
	 
       DistanciaTextLabel->setEnabled(false);
       DistanciaLineEdit->setEnabled(false);
	 
       VizinhosTextLabel->setEnabled(false);
       VizinhosLineEdit->setEnabled(false);
}

void CreateProxMatrix::DistanciaRadioButton_clicked()
{
       constStrategy_=TeDistanceStrategy;
      
       OrdemTextLabel->setEnabled(false);
       OrdemSpinBox->setEnabled(false);
	 
       DistanciaLineEdit->setEnabled(true);
       DistanciaTextLabel->setEnabled(true);
 
       VizinhosTextLabel->setEnabled(false);
       VizinhosLineEdit->setEnabled(false);
}

void CreateProxMatrix::VizinhoRadioButton_clicked()
{
       constStrategy_=TeNearestNeighboursStrategy;
      
       OrdemTextLabel->setEnabled(false);
       OrdemSpinBox->setEnabled(false);
	 
       DistanciaLineEdit->setEnabled(false);
       DistanciaTextLabel->setEnabled(false);
	 
       VizinhosTextLabel->setEnabled(true);
       VizinhosLineEdit->setEnabled(true);
}

void CreateProxMatrix::OKPushButton_clicked()
{
    //tema selecionado
	string temaName =  string(temaComboBox->currentText().ascii());
    curTheme_ = mainWindow_->currentView()->get(temaName);

	//verify the geometry representation
	int numRepres = 0;
	TeGeomRep geomRep = (TeGeomRep)curTheme_->visibleRep();
	if ((curTheme_->layer()->hasGeometry (TePOLYGONS)) && ((geomRep & TePOLYGONS) == TePOLYGONS))
	{
		++numRepres;
		geomRep = TePOLYGONS;
	}
	if ((curTheme_->layer()->hasGeometry (TeLINES)) && ((geomRep & TeLINES) == TeLINES))
	{
		++numRepres;
		geomRep = TeLINES;
	}
	if ((curTheme_->layer()->hasGeometry (TePOINTS)) && ((geomRep & TePOINTS) == TePOINTS))
	{
		++numRepres;
		geomRep = TePOINTS;
	}
	if ((curTheme_->layer()->hasGeometry (TeCELLS)) && ((geomRep & TeCELLS) == TeCELLS))
	{
		++numRepres;
		geomRep = TeCELLS;
	}

	if(numRepres>1)
	{
		QMessageBox::warning(this, tr("Warning"),
		tr("Select only a visible geometrical representation to the selected theme!"));
		return;
	}

	//arquivo de saída
    map<string, string>  mapIds; //quando for associar um object_id a um outro atributo
	vector<string>		 vecIds; //quando for associar o object_id a ordem de entrada no vetor
	bool saveFile = SalvarArquivoCheckBox->isChecked();
	bool seqNumber = false;
	if(saveFile)
	{
		fileName_= string(fileNameLineEdit->text().ascii());
		attrName_= string(objIdComboBox->currentText().ascii());
		path_= string(pathLineEdit->text().ascii());
		if(fileName_.empty() || attrName_.empty() || path_.empty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Enter the file name that will be created and its path!"));
			return;
		}
		
		//verify if the attribute is sequence number
		if(objIdComboBox->currentItem()==0)
			seqNumber = true;
		
		if(!seqNumber)
		{
			//sql to verify if the attribute is unique
			int pos = attrName_.find(".");
			string t = attrName_.substr(0, pos); 
			string test = " SELECT COUNT("+ attrName_ +")";
			test += " FROM "+ t;
			test += " GROUP BY "+ attrName_;
			test += " HAVING COUNT("+ attrName_ +")>1";

			TeDatabasePortal* portal = db_->getPortal();
			if(!portal || !portal->query(test) || portal->fetchRow())
			{
				QMessageBox::warning(this, tr("Error"),
				tr("The chosen attribute to identify each object must be unique!"));
				if(portal)
					delete portal;
				return;
			}
			delete portal;
		}
		else //sequential number
		{
			//fill the mapId
			string geomTable = curTheme_->layer()->vectRepres()[0]->tableName_;
			string test = " SELECT object_id ";
			test += " FROM "+ geomTable;
			test += " ORDER BY geom_id, object_id ";
			
			TeDatabasePortal* portal = db_->getPortal();
			if(!portal || !portal->query(test)) 
			{
				QMessageBox::warning(this, tr("Error"),
				tr("The chosen attribute to identify each object must be unique!"));
				if(portal)
					delete portal;
				return;
			}
			vecIds.clear();
			int cont = 1;
			while(portal->fetchRow())
			{
				string objId = string(portal->getData(0));
				if(mapIds.find(objId) == mapIds.end())
				{
					mapIds[objId]=Te2String(cont);
					vecIds.push_back (objId);
					++cont;
				}
			}
			delete portal;
		}
	}		

	//ponderação
	norm_ = NormCheckBox->isChecked(); 
	TeProxMatrixWeightsStrategy* sw;
	if(NaoPonderaRadioButton->isOn())
	{
		weightStrategy_ = TeNoWeightsStrategy;
		sw = new TeProxMatrixNoWeightsStrategy(norm_);
	}
	else if (PonderaQuadradoRadioButton->isOn())
	{
		weightStrategy_ = TeSquaredInverseDistStrategy;
		sw = new TeProxMatrixSquaredInverseDistanceStrategy(1.0,1.0,1.0,1.0,norm_);
	}
	else 
	{
		weightStrategy_ = TeInverseDistanceStrategy;
		sw = new TeProxMatrixInverseDistanceStrategy(1.0,1.0,1.0,1.0,norm_);
	}
	
	//-------------------- construção
	//attributes
	vector<string> attributes;
	if(saveFile && !seqNumber)
		attributes.push_back (attrName_);

	//calculate distance?
	calculateDistance_ = CalcularDistCheckBox->isChecked(); 
	string dist = string(DistanciaLineEdit->text().ascii());
	string num_neig = string(VizinhosLineEdit->text().ascii());

	if((constStrategy_==TeDistanceStrategy) && dist.empty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Define the distance!"));
		return;
	}

	if((constStrategy_==TeNearestNeighboursStrategy) && num_neig.empty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Define the number of neighbours!"));
		return;
	}
	
	distance_ = atof(dist.c_str());
	num_neigs_ = atoi(num_neig.c_str());
	vector<string> objIds;
	
	if(TeProgress::instance())
	{
		string caption = tr("Creating proximity matrix").latin1();
		TeProgress::instance()->setCaption(caption.c_str());
    }

	if((constStrategy_==TeAdjacencyStrategy) || (constStrategy_==TeDistanceStrategy))
    {
		TeProxMatrixConstructionStrategy<TeSTElementSet>* sc;
		//verify if the theme has polygons or points
		if((constStrategy_==TeAdjacencyStrategy) && (geomRep != TePOLYGONS) && (geomRep != TeCELLS))
		{
			QMessageBox::warning(this, tr("Warning"), tr("The theme must have polygons or cells to apply contiguity strategy!"));
			return;
		}

		//verify if it exists a contiguity matrix for this theme
		if(constStrategy_==TeAdjacencyStrategy)
		{
			string sql = " SELECT * FROM te_gpm WHERE theme_id1 = "+ Te2String(curTheme_->id());
			sql += " AND construction_strategy = 0 "; 
			TeDatabasePortal* portal = db_->getPortal();
			if(!portal)
				return;

			if(portal->query(sql) && portal->fetchRow())
			{
				QString msg = tr("The contiguity matrix already exists for this theme!\n");
				msg += tr("Do you want to continue?");
				int response = QMessageBox::question(this, tr("Question"), msg, tr("Yes"), tr("No"));
				if (response != 0)
				{
					delete portal;
					return;	
				}
			}
			delete portal;
		}

		//load objects from database
		TeQuerierParams params(true, attributes);
		params.setParams(curTheme_);
		TeQuerier querier(params);
		
		if(!querier.loadInstances())
		{
			QMessageBox::warning(this, tr("Error"), tr("Error loading objects from database!"));
			return;
		}

		//progress bar
		int step = 0;
		if(TeProgress::instance())
		{
			string msg = tr("Loading theme... Please, wait!").latin1();
			TeProgress::instance()->setMessage(msg);
			TeProgress::instance()->setTotalSteps(querier.numElemInstances());
		}
		
		TeSTElementSet objSet(curTheme_);
		TeSTInstance  sto;
		while(querier.fetchInstance(sto))
		{
			objSet.insertSTInstance (sto);
			objIds.push_back(sto.objectId());
			if(saveFile && !seqNumber)
			{
				string attrId;
				sto.getPropertyValue(attrId, 0); //get the first attribute as object Id
				mapIds[sto.objectId()]=attrId;
			}
			
			//progress
			if(TeProgress::instance())
			{
				if (TeProgress::instance()->wasCancelled())
				{
					TeProgress::instance()->reset();
					return;
				}
				else
					TeProgress::instance()->setProgress(step);
			}	
			++step;
		}
		TeProgress::instance()->reset();

		//create strategy
		if(constStrategy_==TeAdjacencyStrategy) 
			sc= new TeProxMatrixLocalAdjacencyStrategy (&objSet, geomRep, calculateDistance_);
		else
			sc= new TeProxMatrixLocalDistanceStrategy<TeSTElementSet> (&objSet, geomRep, distance_);

		//create porx matrix
		TeGeneralizedProxMatrix<TeSTElementSet> proxMatrix(sc,  sw); 

		//progress bar
		if(TeProgress::instance())
		{
			string msg = tr("Building the Proximity Matrix... Please, wait!").latin1();
			TeProgress::instance()->setMessage(msg);
		}

		//build the matrix
		if(!proxMatrix.constructMatrix())
		{
			QMessageBox::warning(this, tr("Error"), tr("Error building proximity matrix!"));
			return;
		}

		//save matrix - files
		if(saveFile)
		{
			string file = path_+"/"+fileName_;
			if(!seqNumber)
			{
				if(!proxMatrix.saveTextFile(file, &mapIds))
						QMessageBox::warning(this, tr("Warning"), tr("It was not possible to create TXT file!"));
							
				if((constStrategy_!=TeDistanceStrategy) && (!proxMatrix.saveGALFile(file, &mapIds)))
					QMessageBox::warning(this, tr("Warning"), tr("It was not possible to create GAL file!"));

				if((constStrategy_!=TeAdjacencyStrategy) && (!proxMatrix.saveGWTFile(file, &mapIds)))
					QMessageBox::warning(this, tr("Warning"), tr("It was not possible to create GWT file!"));
			}
			else
			{
				if(!proxMatrix.saveTextFile(file, &vecIds))
					QMessageBox::warning(this, tr("Warning"), tr("It was not possible to create TXT file!"));
							
				if((constStrategy_!=TeDistanceStrategy) && (!proxMatrix.saveGALFile(file, &vecIds)))
					QMessageBox::warning(this, tr("Warning"), tr("It was not possible to create GAL file!"));

				if((constStrategy_!=TeAdjacencyStrategy) && (!proxMatrix.saveGWTFile(file, &vecIds)))
					QMessageBox::warning(this, tr("Warning"), tr("It was not possible to create GWT file!"));
			}
		}

		//save matrix - database
		if(!db_->tableExist("te_gpm"))
		{
			if(!createGPMMetadataTable(db_))
			{
				QMessageBox::warning(this, tr("Error"), tr("Error creating proximity matrix table!"));
				return;
			}
		}

		string tableName = "te_neighbourhood_"+ Te2String(curTheme_->id());
		string tempTable = tableName;
		int count = 1;
		while(db_->tableExist(tempTable))
		{
			tempTable = tableName+"_"+Te2String(count);
			++count;
		}

		if(!createNeighbourhoodTable(db_, tempTable))
		{
			QMessageBox::warning(this, tr("Error"), tr("Error creating neighbourhood table!"));
			return;
		}

		proxMatrix.neighbourhood_table_=tempTable;

		//progress bar
		if(TeProgress::instance())
		{
			string msg = tr("Saving Proximity Matrix... Please, wait!").latin1();
			TeProgress::instance()->setMessage(msg);
		}

		if(!insertGPM(db_, &proxMatrix, objIds))
		{
			QMessageBox::warning(this, tr("Error"), tr("Error saving proximity matrix!"));
			return;

		}
	}
	
	else //TeNearestNeighboursStrategy
	{
		TeProxMatrixConstructionStrategy<TeSTEventSet>* sc;

		//verificar se o tema é de pontos
		if(!curTheme_->layer()->hasGeometry(TePOINTS))
		{
			QMessageBox::warning(this, tr("Warning"), tr("The theme must have points to apply nearest neighbours strategy!"));
			return;
		}

		TeQuerierParams params(true, attributes);
		params.setParams(curTheme_);
		TeQuerier querier(params);

		if(!querier.loadInstances())
		{
			QMessageBox::warning(this, tr("Error"), tr("Error loading objects from database!"));
			return;
		}

		//progress bar
		int step = 0;
		if(TeProgress::instance())
		{
			string msg = tr("Loading theme... Please, wait!").latin1();
			TeProgress::instance()->setMessage(msg);
			TeProgress::instance()->setTotalSteps(querier.numElemInstances());
		}
		
		TeSTEventSet objSet(curTheme_);
		TeSTInstance  sto;
		while(querier.fetchInstance(sto))
		{
			TeCoord2D coord;
			sto.centroid(coord); 
			TeSTEvent ev(sto.objectId(), TePoint(coord), -1);
			objSet.insertSTInstance (ev);
			objIds.push_back(sto.objectId());
			if(saveFile && !seqNumber)
			{
				string attrId;
				sto.getPropertyValue(attrId, 0); //get the first attribute as object Id
				mapIds[sto.objectId()]=attrId;
			}

			//progress
			if(TeProgress::instance())
			{
				if (TeProgress::instance()->wasCancelled())
				{
					TeProgress::instance()->reset();
					return;
				}
				else
					TeProgress::instance()->setProgress(step);
			}	
			++step;
		}
		TeProgress::instance()->reset();

		if(!objSet.buildKdTree(num_neigs_))
		{
			QMessageBox::warning(this, tr("Error"), tr("Error building neighbourhood tree!"));
			return;
		}

		sc= new TeProxMatrixNearestNeighbourStrategy (&objSet, num_neigs_);

		//create porx matrix
		TeGeneralizedProxMatrix<TeSTEventSet> proxMatrix(sc,  sw); 
		
		//progress bar
		if(TeProgress::instance())
		{
			string msg = tr("Building the Proximity Matrix... Please, wait!").latin1();
			TeProgress::instance()->setMessage(msg);
		}
		
		//build the matrix
		if(!proxMatrix.constructMatrix())
		{
			QMessageBox::warning(this, tr("Error"), tr("Error building proximity matrix!"));
			return;
		}

		//save matrix - files
		if(saveFile)
		{
			string file = path_+"/"+fileName_;
			if(!seqNumber)
			{
				if(!proxMatrix.saveTextFile(file, &mapIds))
					QMessageBox::warning(this, tr("Warning"), tr("It was not possible to create TXT file!"));
							
				if((constStrategy_!=TeDistanceStrategy) && (!proxMatrix.saveGALFile(file, &mapIds)))
					QMessageBox::warning(this, tr("Warning"), tr("It was not possible to create GAL file!"));

				if((constStrategy_!=TeAdjacencyStrategy) && (!proxMatrix.saveGWTFile(file, &mapIds)))
					QMessageBox::warning(this, tr("Warning"), tr("It was not possible to create GWT file!"));
			}
			else
			{
				if(!proxMatrix.saveTextFile(file, &vecIds))
					QMessageBox::warning(this, tr("Warning"), tr("It was not possible to create TXT file!"));
							
				if((constStrategy_!=TeDistanceStrategy) && (!proxMatrix.saveGALFile(file, &vecIds)))
					QMessageBox::warning(this, tr("Warning"), tr("It was not possible to create GAL file!"));

				if((constStrategy_!=TeAdjacencyStrategy) && (!proxMatrix.saveGWTFile(file, &vecIds)))
					QMessageBox::warning(this, tr("Warning"), tr("It was not possible to create GWT file!"));
			}
		}

		//save matrix - database
		if(!db_->tableExist("te_gpm"))
		{
			if(!createGPMMetadataTable(db_))
			{
				QMessageBox::warning(this, tr("Error"), tr("Error creating proximity matrix table!"));
				return;
			}
		}

		string tableName = "te_neighbourhood_"+ Te2String(curTheme_->id());
		string tempTable = tableName;
		int count = 1;
		while(db_->tableExist(tempTable))
		{
			tempTable = tableName+"_"+Te2String(count);
			++count;
		}

		if(!createNeighbourhoodTable(db_, tempTable))
		{
			QMessageBox::warning(this, tr("Error"), tr("Error creating neighbourhood table!"));
			return;
		}

		//progress bar
		if(TeProgress::instance())
		{
			string msg = tr("Saving Proximity Matrix... Please, wait!").latin1();
			TeProgress::instance()->setMessage(msg);
		}

		proxMatrix.neighbourhood_table_=tempTable;
		if(!insertGPM(db_, &proxMatrix, objIds))
		{
			QMessageBox::warning(this, tr("Error"), tr("Error saving proximity matrix!"));
			return;

		}
	}

	QMessageBox::information(this, tr("Information"), tr("The proximity matrix was created successfully!"));
	hide();
}

void CreateProxMatrix::show()
{
	if(isShown())
		return;

	DisplayWindow *displayWindow = mainWindow_->getDisplayWindow();
	TeQtGrid *ptrGrid = mainWindow_->getGrid();

	connect((const QObject*)displayWindow,SIGNAL(distanceMeterSignal(TeCoord2D, TeCoord2D)),
			this, SLOT(putDistanceMeterSlot(TeCoord2D, TeCoord2D)));

	connect((const QObject*)ptrGrid,SIGNAL(gridChangedSignal()),
	this, SLOT(gridChangedSlot()));

	connect((const QObject*)mainWindow_,SIGNAL(dbChanged(QString)),
	this, SLOT(dbChangedSlot(QString)));
	
	connect((const QObject*)mainWindow_,SIGNAL(layerChanged(int)),
	this, SLOT(layerChangedSlot(int)));
	
	connect((const QObject*)mainWindow_,SIGNAL(themeChanged(int)),
	this, SLOT(themeChangedSlot(int)));
	
	connect((const QObject*)mainWindow_,SIGNAL(viewChanged(int)),
	this, SLOT(viewChangedSlot(int)));

	QDialog::show();
}


void CreateProxMatrix::hide()
{
	if(isHidden())
		return;

	disconnect(this);
	QDialog::hide();
}


void CreateProxMatrix::putDistanceMeterSlot( TeCoord2D pa, TeCoord2D pb)
{
	if(DistanciaLineEdit->isEnabled() == false)
		return;

	DisplayWindow *displayWindow = mainWindow_->getDisplayWindow();
    TeQtCanvas *canvas = displayWindow->getCanvas();
	TeProjection *cproj = canvas->projection();

	TeProjection *dproj = curTheme_->layer()->projection();

	if(cproj && dproj && !(*cproj == *dproj))
	{
		cproj->setDestinationProjection(dproj);
		pa = cproj->PC2LL (pa);
		pa = dproj->LL2PC (pa);
		pb = cproj->PC2LL (pb);
		pb = dproj->LL2PC (pb);
	}
	double radius = sqrt((pa.x()-pb.x()) * (pa.x()-pb.x()) + (pa.y()-pb.y()) * (pa.y()-pb.y()));
	
	string s = Te2String(radius, 2);	
	DistanciaLineEdit->setText(s.c_str());
}

void CreateProxMatrix::gridChangedSlot()
{
	hide();
}
void CreateProxMatrix::dbChangedSlot(QString databaseName)
{
	if(databaseName.isNull() || databaseName.isEmpty())
		return;

	string dname = databaseName.latin1();
	string name = curTheme_->layer()->database()->databaseName();

	if(dname == name)
		hide();
}


void CreateProxMatrix::layerChangedSlot(int)
{
	hide();
}


void CreateProxMatrix::themeChangedSlot(int)
{
	hide();
}


void CreateProxMatrix::viewChangedSlot(int)
{
	hide();
}

void CreateProxMatrix::pathPushButton_clicked()
{
	QString dirName = QFileDialog::getExistingDirectory(
		QString::null,
		this,
		tr("Get Existing Directory"),
		tr("Select a directory"),
		true);
	
	if (!dirName.isEmpty())
		pathLineEdit->setText(dirName);
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select a directory!"));
		return;
	}
}



void CreateProxMatrix::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("matriz_proximidade.htm");
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
