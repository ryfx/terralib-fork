#include <EBI.h>

#include <ImoranPluginWindow.h>
#include <TeGUIUtils.h> 
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeDatabaseUtils.h>
#include <TeWaitCursor.h>
#include <TeSTEFunctionsDB.h>
#include <TeDataTypes.h>
#include <vector>

#include <TeSTElementSet.h>
//#include <TeGeneralizedProxMatrix.h>
#include <TeApplicationUtils.h>
#include <TeDatabaseFactory.h>
#include <TeDatabaseFactoryParams.h>

#include <qpushbutton.h>
#include <qcheckbox.h> 
#include <qmessagebox.h>
#include <qbuttongroup.h> 
#include <qtabwidget.h>
#include <qtextbrowser.h> 
#include <qspinbox.h> 

#include <help.h>


IMoranPluginWindow::IMoranPluginWindow(QWidget* parent):
	imoran(parent, "IMoranPluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	localDb_ = false;
	control_ = false;
}


IMoranPluginWindow::~IMoranPluginWindow()
{
	if(localDb_)
		delete localDb_;
}

bool IMoranPluginWindow::fillProxMatrixCombo(const QString& temaName)
 {
  	TeTheme* curTheme_ = plugIngParams_.getCurrentViewPtr()->get(temaName.latin1());
  	 
     if(!curTheme_)
		return false;  //mensagem de erro!!!!

     comboIndex_.clear();
	 matrixComboBox->clear();
     
	 //Busca no banco de dados as matrizes existentes
     string sql = " SELECT gpm_id, gpm_default, theme_id1, construction_strategy, ";
     sql += "  const_max_distance, const_num_neighbours FROM te_gpm ";
     sql += "  WHERE  theme_id1 = "+ Te2String(curTheme_->id());
     sql += "  ORDER BY construction_strategy ";
     
     TeDatabasePortal* portal = localDb_->getPortal();
     if(!portal)
		return false; 
     
     if((!portal->query(sql)) || (!portal->fetchRow()))
     {
		QMessageBox::warning(this, tr("Warning"),
		tr("There is no proximity matrix associated to the selected theme!"));
		delete portal;
		return false;
     }
    int index=0; 
	//Preenche o combo com as matrizes
	do
     {
			int gpmId = portal->getInt(0);
			int gpmDef = portal->getInt(1);
			int constStr = portal->getInt(3);
			double dist = portal->getDouble(4);
			int nn = portal->getInt(5);
			
			string matrix;
			if(constStr == 0)  //adjacencia
			{
				matrix = tr(" Contiguity ").latin1();
			
			}
			else if(constStr == 1)  //distancia
			{
				matrix = string(tr(" Distance   ->   ").ascii())+ Te2String(dist, 6) + string(tr("  units ").ascii());
			}
			else if(constStr == 2)  //nn
			{
				matrix = string(tr(" Nearest neighbours   ->   ").ascii())+ Te2String(nn) + string(tr("  neighbours ").ascii());
			}
			
			if(gpmDef )
				matrix += string(tr("    (default )   ").ascii());
			
			matrixComboBox->insertItem(matrix.c_str());  
			comboIndex_[index]=gpmId;
			++index;
       } while (portal->fetchRow());
    
       delete portal;
	   return true;
}
//Marca a matriz selecionada como default
void IMoranPluginWindow::setDefaultMatrix(const QString& temaName)
{
  	TeTheme* curTheme_ = plugIngParams_.getCurrentViewPtr()->get(temaName.latin1());
	 //Seleciona qual o index da matriz desejada
	 int index = matrixComboBox->currentItem();
	 if(index<0)
	 {
		hide();
		return;
	 }

	 //Encontra a matriz
	 map<int, int>::iterator it = comboIndex_.find(index);
	 if(it==comboIndex_.end())
	 {
		hide();
		return;
	 }

	 int gpmId = it->second;
	       
     //setar todas matrizes sem ser como default
     string up =  " UPDATE te_gpm SET gpm_default = 0 WHERE theme_id1 = "+ Te2String(curTheme_ ->id());
     if(!localDb_->execute (up))
	 {
		hide();
		return;
	 }
	 //setar a matriz desejada como default
      up =  " UPDATE te_gpm SET gpm_default = 1 WHERE gpm_id = "+ Te2String(gpmId);
      if(!localDb_->execute (up))
	  {
		  hide();
          return;
	  }
      
	   return;
}
void IMoranPluginWindow::Enable_matrixSlot()
{
	bool control;
	if(matrixcheckBox->isChecked()){
		matrixComboBox->setEnabled(true);
		string temaName =  string(themeComboBox->currentText().ascii());
		control = fillProxMatrixCombo(temaName.c_str());
	}
	else
		matrixComboBox->setEnabled(false);
}

void IMoranPluginWindow::temaComboBoxClickSlot(const QString&)
{
	//carregar todas as tabelas de atributos do tema selecionado
	if(!fillTableCombo(localDb_->viewMap()[ViewId_], tableComboBox, themeComboBox))
	{
		//error message
		return;
	}

	//carregar todos os atributos da tabela do tableComboBox
	tableComboBoxClickSlot(tableComboBox->currentText());
	return;
}

void IMoranPluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/
	Help help(this, "help", false);
	help.init("TerraClusterIMoran.htm");

	if(!help.erro_)
		help.exec();

}
    
void IMoranPluginWindow::okButtonClickSlot()
{
	// Parametros para simulação
	int montecarlo;								// Interaçoes de Monte Carlo
	QString str, saida = "";					// String auxiliar e de saida
	std::vector<double> pop;					// Vetor de populacao
	std::vector<double> cases;					// Vetor de casos
	std::vector<double> coordx;					// Coordenadas x
	std::vector<double> coordy;					// Coordenadas y
	std::vector< vector <double> > vizin;       // Vizinhanca	

	std::vector<string> objIds;
	Output_text->clear();
	
	TeWaitCursor wait;
	switch(this->montecarlobuttonGroup->selectedId())	// Seleciona o numero de simulações de Monte Carlo
	{
		case 0:
			montecarlo = 99;
			break;
		case 1:
			montecarlo = 999;
			break;
		case 2:
			montecarlo = 9999;
			break;
	}

	//----------------- pegar o tema, a tabela e os atributos selecionados na tela
	TeTheme* theme = plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1()); 
	if(!theme)
	{
		QMessageBox::critical(this, tr("Error"), tr("Not possible to load the Theme"));
		return;
	}
	int themeId = theme->id();
	string tableName = string(tableComboBox->currentText().ascii());
	if(tableName.empty())
	{
		QMessageBox::critical(this, tr("Error"), tr("Theme without Table"));
		return;
	}
	string popAttr = string(populationComboBox->currentText().ascii());
	string caseAttr =  string(caseComboBox->currentText().ascii());
	if(popAttr.empty() || caseAttr.empty())
	{
		QMessageBox::critical(this, tr("Error"), tr("Empty data"));
		return;
	}

	//----------------- pegar do banco os atributos e geometrias que serão utilizados na funcao
	vector<string> attrs;
	attrs.push_back (tableName+"."+popAttr);
	attrs.push_back (tableName+"."+caseAttr);

	TeQuerierParams params(true, attrs);
	params.setParams(theme);
	
	TeQuerier querier(params);
	if(!querier.loadInstances())
	{
		QMessageBox::critical(this, tr("Error"), tr("Not possible to load data"));
		return;
	}

	TeSTInstance obj;
	TeSTElementSet objSet(theme);
	while(querier.fetchInstance(obj))
	{
		//armazenar as instancias do querier
		objSet.insertSTInstance(obj);
		//armazenar os identificadores
		objIds.push_back(obj.objectId());
		
		//pegar o centroide
		TeCoord2D coord;
		obj.centroid(coord);
		coordx.push_back(coord.x());
		coordy.push_back(coord.y());

		//pegar os atributos
		string valPop = "";
		obj.getPropertyValue(valPop, 0);
		pop.push_back (atof(valPop.c_str()));

		string valCase = "";
		obj.getPropertyValue(valCase, 1);
		cases.push_back (atof(valCase.c_str()));
	}

	// Verifica se o numero de casos coincide com o número de areas
	if(objIds.size() != pop.size())
	{
		QMessageBox::critical(this, tr("Error"), tr("Population and/or Cases files are incompatible with the Number of Areas"));
		return;
	}
	if(cases.size() != pop.size())
	{
		QMessageBox::critical(this, tr("Error"), tr("Population and Cases files are incompatible"));
		return;
	}
	
		Output_text->setText(" EBI Moran \n running...");
		
		if(!theme->layer()->hasGeometry(TePOLYGONS))
		{
			QMessageBox::critical(this, tr("Error"), tr("O Layer indicado nao possui Geometria"));
			return;
		}


	//gerar a matriz de proximidade
	//load prox matrix or construct one
	TeGeneralizedProxMatrix<TeSTElementSet>*			proxMatrix=0;
	TeProxMatrixConstructionStrategy<TeSTElementSet>*	constStrategy=0;

	//-------------- Load prox matrix or create new
	//Testa se o usuario selecionou e existe alguma matriz
	if((matrixcheckBox->isChecked()) && (matrixComboBox->count() != 0)){
		setDefaultMatrix(themeComboBox->currentText().ascii());
		if(!loadDefaultGPM(localDb_,themeId,proxMatrix))
		{
		    QMessageBox::critical(this, tr("Error"), tr("Nao foi possivel carregar a Matriz de Proximidade"));
			return;
		}
	}
	//Se nao cria a matrix de proximidade
	else
	   {
		TeGeomRep geomRep = TeGEOMETRYNONE;
		if(theme->layer()->hasGeometry (TePOLYGONS))
			geomRep = TePOLYGONS;

		if(geomRep==TeGEOMETRYNONE)
		{
			QString msg = tr("The theme must have a geometric representation of polygons");
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}
		
		if(TeProgress::instance())
		{
			string caption = tr("Ipop").latin1();
			TeProgress::instance()->setCaption(caption.c_str());
			string msg = tr("Building the Proximity Matrix. Please, wait!").latin1();
			TeProgress::instance()->setMessage(msg);
		}
		
		//create matrix and save in the database
		constStrategy = new TeProxMatrixLocalAdjacencyStrategy (&objSet, geomRep); 
		TeProxMatrixNoWeightsStrategy sw_no;
		TeProxMatrixNoSlicingStrategy ss_no;
		proxMatrix = new TeGeneralizedProxMatrix<TeSTElementSet>(constStrategy, &sw_no, &ss_no);
		
		if (!proxMatrix->constructMatrix())
		{
			QMessageBox::critical(this, tr("Error"), tr("Fail building the proximity matrix!"));
			if(proxMatrix)
				delete proxMatrix;
			return;
		}

		//save in the database
		bool savedMatrix = true;
		if(!theme->layer()->database()->tableExist("te_gpm"))
		{
			if(!createGPMMetadataTable(theme->layer()->database()))
			{
				QMessageBox::warning(this, tr("Error"), 
					tr("It was not possible to save proximity matrix in database!"));
				savedMatrix = false;
			}
		}

		string tableName = "te_neighbourhood_"+ Te2String(theme->id());
		string tempTable = tableName;
		int count = 1;
		while(theme->layer()->database()->tableExist(tempTable))
		{
			tempTable = tableName+"_"+Te2String(count);
			++count;
		}

		if(savedMatrix && (!createNeighbourhoodTable(theme->layer()->database(), tempTable)))
		{
			QMessageBox::warning(this, tr("Error"), 
					tr("It was not possible to save proximity matrix in database!"));
			savedMatrix = false;
		}

		proxMatrix->neighbourhood_table_=tempTable;


		//load object ids
		if(savedMatrix && (!insertGPM(theme->layer()->database(), proxMatrix, objIds)))
		{
			QMessageBox::warning(this, tr("Error"), 
					tr("It was not possible to save proximity matrix in database!"));
			savedMatrix = false;
		}
		fillProxMatrixCombo(themeComboBox->currentText().ascii());
	}
	
	//FIM DA GERACAO DA MATRIX
		
		int posArea1, posArea2;
		unsigned int i;
		std::vector<double> aux(3,1.0);
        TeSTElementSet::iterator it = objSet.begin();
		while(it != objSet.end()){
			TeNeighboursMap neigs = proxMatrix->getMapNeighbours ((*it).objectId()); 
			TeNeighboursMap::iterator it2 = neigs.begin();
			for(i=0;i<objIds.size();++i)
			{
				if((*it).objectId() == objIds[i])
				{
					posArea1 = i;
					break;
				}
			}         
	    	while(it2 != neigs.end())
			{
				
				string objectId = it2->first;
				for(i=0;i<objIds.size();++i)
				{
					if(objectId == objIds[i])
					{
						posArea2 = i;
						break;
					}
				}
				aux[0] = posArea1;
				aux[1] = posArea2;
				vizin.push_back(aux);
				++it2;
			}
			++it;
		}

		if(weightcheckBox->isChecked())
		{
			double dist,x1,x2,y1,y2;
			for(i=0;i<vizin.size();i++)
			{
				x1 = coordx[(int)vizin[i][0]];
				y1 = coordy[(int)vizin[i][0]];
				x2 = coordx[(int)vizin[i][1]];
				y2 = coordy[(int)vizin[i][1]];
				dist = sqrt(pow((x1-x2),2.0)+pow((y1-y2),2.0));
				vizin[i][2] = 1/(pow((1+dist),2.0));
			}
		}
		
		Saida_EBI saida_ebi;
		std::valarray<double> popaux(pop.size()), casesaux(cases.size());
		for(i=0;i<pop.size();i++){
			popaux[i] = pop[i];
			casesaux[i] = cases[i];
		}

		// Executa o Método IMoran
		if(EBI(vizin,popaux,casesaux,saida_ebi,montecarlo))
		{
			QMessageBox::critical(this, tr("Error"), tr("IMoran Memory Error"));
			return;
		}

		// Imprime os resultados na interface
		saida = tr("I Moran \n \n Moran I: ") + str.setNum( saida_ebi.EBI );
//		saida += "\n E[I]: " + str.setNum( saida_ebi.esperanca);
//		saida += "\n\n Assumption Normal \n Variance: " + str.setNum( saida_ebi.varN );
//		saida += "\n Z-Score: " + str.setNum( saida_ebi.ZN );
//		saida += "\n\n Assumption Randomization \n Variance: " + str.setNum( saida_ebi.varR );
//		saida += "\n Z-Score: " + str.setNum( saida_ebi.ZR );
		saida += tr("\n\n Simulation \n p-value: ") + str.setNum( saida_ebi.pvalor );
		
		Output_text->setText(saida);

	return;
}

void IMoranPluginWindow::tableComboBoxClickSlot(const QString&)
{
	TeTheme* theme = plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1()); 
	if(!theme)
		return;

	//carregar os atributos (int ou double) da tabela selecionada
	vector<TeAttrDataType> attrType;
	attrType.push_back (TeINT);
	attrType.push_back (TeREAL);
	if(!fillColumnCombo(attrType, theme, populationComboBox, tableComboBox))
	{
		//error message
		return;
	}

	if(!fillColumnCombo(attrType, theme, caseComboBox, tableComboBox))
	{
		//error message
		return;
	}

	return;
}

void IMoranPluginWindow::CloseSlot()
{
	//if(localDb_)
	//	delete localDb_;
	QWidget::close();
}
//sera chamado toda vez que a form for chamada através do menu
void IMoranPluginWindow::updateForm(PluginParameters* pluginParams)
{
	plugIngParams_ = (*pluginParams);

	//Inicializa a interface
	Output_text->clear();
	matrixcheckBox->setChecked(false);
	Enable_matrixSlot();



	if (plugIngParams_.getCurrentDatabasePtr() == NULL)
	{
		QMessageBox::warning(this, tr("Error"), tr("Data Base not selected"));
		return;
	}
	//Fixa qual vista é carregada inicialmente na abertura do plugin
	if(!control_){
		if (plugIngParams_.getCurrentViewPtr() == NULL)
	    { 
			QMessageBox::warning(this, tr("Error"), tr("View not selected"));
			return;
     	}
		ViewId_ = plugIngParams_.getCurrentViewPtr()->id();
		control_ = false;
	}
    if (plugIngParams_.getCurrentThemeAppPtr() == NULL)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Theme not selected"));
		return;
	}
	//checar se os ponteiros do pluginParams inclusive o database!!!!!! 
	if(localDb_)
		delete localDb_;

	/* Creating a local database connection - Windows DLLs workaround */
	TeDatabaseFactoryParams dbf_params;
	dbf_params.database_ = plugIngParams_.getCurrentDatabasePtr()->databaseName();
	dbf_params.dbms_name_ = plugIngParams_.getCurrentDatabasePtr()->dbmsName();
	dbf_params.host_ = plugIngParams_.getCurrentDatabasePtr()->host();
	dbf_params.password_ = plugIngParams_.getCurrentDatabasePtr()->password();
	dbf_params.port_ = plugIngParams_.getCurrentDatabasePtr()->portNumber();
	dbf_params.user_ = plugIngParams_.getCurrentDatabasePtr()->user();
	    
	//localDb_ = .reset(TeDatabaseFactory::make(dbf_params ));
	localDb_ = TeDatabaseFactory::make(dbf_params);
	//if(!localDb_.isActive() ) 
	if(!localDb_)
	{
		QMessageBox::critical( this, "TerraView", 
		tr( "Unable to create output database connection" ) );    
		return;     
	}      
		
	if(!localDb_->isConnected())
	{
		QMessageBox::critical( this, "TerraView", 
		tr( "Unable to connect to database" ) );   
		return;
	} 

	//load theme map, layer map, etc, etc
	localDb_->loadViewSet("");

	//carregar os temas da vista corrente e colcoar o tema corrente como default
	if(!fillThemeCombo(TePOLYGONS,pluginParams->getCurrentViewPtr(), 
		themeComboBox, pluginParams->getCurrentThemeAppPtr()->getTheme()->name()))
	{
		QMessageBox::critical( this, "TerraView", 
		tr( "Unable to fill theme ComboBox" ) );   
		return;
	}                

	//carregar todos os atributos da tabela do tableComboBox
	temaComboBoxClickSlot(QString(""));
	show();
	return;
}

void IMoranPluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}











	
