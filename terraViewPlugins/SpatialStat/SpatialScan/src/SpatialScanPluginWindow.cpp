
#include <SpatialScanPluginWindow.h>
#include <TeGUIUtils.h> 
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeDatabaseUtils.h>
#include <cluster.h>
#include <TeWaitCursor.h>
#include <TeSTEFunctionsDB.h>
#include <TeDataTypes.h>

#include <qpushbutton.h>
#include <qcheckbox.h> 
#include <qmessagebox.h>
#include <qbuttongroup.h> 
#include <qtabwidget.h>
#include <qtextbrowser.h> 
#include <qspinbox.h> 

#include <TeSTElementSet.h>
#include <TeApplicationUtils.h>
#include <TeDatabaseFactory.h>
#include <TeDatabaseFactoryParams.h>

#include <TeDatabase.h>

#include <help.h>

SpatialScanPluginWindow::SpatialScanPluginWindow(QWidget* parent):
	spatialscan(parent, "SpatialScanPluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	localDb_ = false;
	control_ = false;
}


SpatialScanPluginWindow::~SpatialScanPluginWindow()
{
	if(localDb_)
		delete localDb_;
}

//Preenche o comboBox com as matrizes existentes
bool SpatialScanPluginWindow::fillProxMatrixCombo(const QString& temaName)
 {
    //Carrega o tema desejado
	TeTheme* curTheme_ =  plugIngParams_.getCurrentViewPtr()->get(temaName.latin1());
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
void SpatialScanPluginWindow::setDefaultMatrix(const QString& temaName)
{
	 //Carrega o tema sobre o qual a matriz criada
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

void SpatialScanPluginWindow::Enable_matrixSlot()
{
	bool control;
	if(matrixcheckBox->isChecked()){
		matrixComboBox->setEnabled(true);
		control = fillProxMatrixCombo(themeComboBox->currentText());
	}
	else
		matrixComboBox->setEnabled(false);
}

void SpatialScanPluginWindow::temaComboBoxClickSlot(const QString&)
{
	//carregar todas as tabelas de atributos do tema selecionado
	if(!fillTableCombo(plugIngParams_.getCurrentViewPtr(), tableComboBox, themeComboBox))
	{
		//error message
		return;
	}

	//carregar todos os atributos da tabela do tableComboBox
	tableComboBoxClickSlot(tableComboBox->currentText());
	return;
}
    
void SpatialScanPluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/
	
	Help help(this, "help", false);
	help.init("TerraClusterSpacialScan.htm");

	if(!help.erro_)
		help.exec();

}

void 
SpatialScanPluginWindow::scanButtonClickSlot()
{
	// Parametros para simulação
	int montecarlo;								// Interaçoes de Monte Carlo
	QString str, saida = "";					// String auxiliar e de saida
	std::vector<double> pop;					// Vetor de populacao
	std::vector<double> cases;					// Vetor de casos
	std::vector<double> coordx;					// Coordenadas x
	std::vector<double> coordy;					// Coordenadas x
	std::vector<std::string> objIds;
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
	std::vector<std::string> attrs;
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

    
	//----------------- chamar a funcao SCAN ------------------------------------//
	if(tabWidget->currentPageIndex()  ==  0)	// Metodo SCAN
	{
		Output_text->setText(tr(" Scan Cluster \n running..."));
	
		// Variáveis de Escopo
		TCluster scan_clu;						// Estrutura de Cluster
		double* geo[2];							// Coordenadas Geográficas
		
		Output_text->setText(" Scan Cluster \n running...");
		scan_clu.indices = new int[pop.size()];	// Aloca vetor com tamanho máximo	- FAZER TRATAMENTO DE ERRO

		geo[0] = &coordx[0];						// Atribuição dos elementos do vetor coordenada x	VERIFICAR CONVERSAO NA FUNCAO SCANCLUSTERBERNOULLI
		geo[1] = &coordy[0];						// Atribuição dos elementos do vetor coordenada y

		// Executa o Método SCAN
		if( ScanClusterBernoulli(scan_clu, pop.size(), &pop[0], &cases[0], geo, montecarlo , 
			(double) popspinBox->value()/100 ) )
		{
			QMessageBox::critical(this, tr("Error"), tr("Scan Memory Error"));
			return;
		}
		// Imprime os resultados na interface
		saida = tr("Spatial Scan Cluster \n \n Cluster size: ") + str.setNum( scan_clu.tamanho );
		saida += "\n Log(Lambda): " + str.setNum( scan_clu.veross );
		saida += tr("\n p-value: ") + str.setNum( scan_clu.pvalue );
		saida += tr("\n Areas (indexes): ");
		for(int c=0;c<scan_clu.tamanho;c++) 
		{
			result_.push_back(objIds[scan_clu.indices[c]]);
			saida += objIds[scan_clu.indices[c]].c_str();
			saida += "  ";
		}
	   
		Output_text->setText(saida);
	}
/* - - - - - - - - - - - - - - METODO dMST - - - - - - - - - - - - - - - - - - - - - - - */

	if(tabWidget->currentPageIndex()  ==  1)	// Metodo DMST
	{
		TCluster scan_clu;								// Estrutura de Cluster
		std::vector<TArea> vArea;						// Vetor de Areas
		std::vector<TAresta> vAresta;					// Vetor de Arestas
		TArea		auxArea;							// Auxiliar Area
		TAresta		auxAresta;							// Auxiliar Aresta
		bool doubly_flag =  doubly_checkBox->isOn();	// Verifica o flag doubly

		Output_text->setText(tr(" dMST Cluster \n running..."));
		scan_clu.indices = new int[pop.size()];	// Aloca vetor com tamanho máximo	- FAZER TRATAMENTO DE ERRO
		
		if(!theme->layer()->hasGeometry(TePOLYGONS))
		{
			QMessageBox::critical(this, tr("Error"), tr("The Layer has not indicated Geometry"));
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
		    QMessageBox::critical(this, tr("Error"), tr("Could not load the Proximity Matrix"));
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
			auxAresta.area1 = posArea1;
			//auxAresta.area1 = atoi((*it).objectId().c_str());
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
				auxAresta.area2 = posArea2;
				//auxAresta.area2 = atoi(objectId.c_str());
				vAresta.push_back(auxAresta);
				++it2;
			}
			++it;
		}

		//FALTA A CRIACAO DA ESTRUTURA DE VIZINHOS!!!!

		// Atribuição das informaçoes dos elementos de Area
		for(int c=0;c<(int)pop.size();c++){
			auxArea.pop    = pop[c];
			auxArea.casos  = cases[c];
			auxArea.indice = c;
			vArea.push_back(auxArea);
		}
		// Executa o Método dMST
		if(dMSTClusterBernoulli(scan_clu, vArea.size(), montecarlo, &vArea[0],
			vAresta.size(), &vAresta[0], maxspinBox->value(), minspinBox->value(), doubly_flag)  ){
			QMessageBox::critical(this, tr("Error"), tr("dMST Memory Error"));
			return;
		}

		// Imprime os resultados na interface
		saida = tr("Spatial dMST Cluster \n \n Cluster size: ") + str.setNum( scan_clu.tamanho );
		saida += "\n Log(Lambda): " + str.setNum( scan_clu.veross );
		saida += tr("\n p-value: ") + str.setNum( scan_clu.pvalue );
		saida += tr("\n Areas (indexes): ");
		for(int c=0;c<scan_clu.tamanho;c++) 
		{
			result_.push_back(objIds[scan_clu.indices[c]]);
			saida += objIds[scan_clu.indices[c]].c_str();
			saida += "  ";
		}
		
		Output_text->setText(saida);
	}

	//---------------- plotar o resultado no canvas
			
	//Collection Table
	string sql = " UPDATE "+ theme->collectionTable() +" SET ";
	sql += " c_object_status = 0 "; 
	if(!localDb_->execute(sql))
	{
		//error
		return;
	}

	std::vector<std::string> resultIndClause = generateInClauses(result_.begin(), result_.end(), localDb_, true);
	
	string base = " UPDATE "+ theme->collectionTable() +" SET ";
	base += " c_object_status = 1 "; 
	base += " WHERE c_object_id IN "; 
	for(unsigned int i=0; i<resultIndClause.size(); ++i)
	{
		sql = base + resultIndClause[i];
		if(!localDb_->execute(sql))
		{
			//error
			return;
		}
	}

	//Collection aux table
	sql = " UPDATE " + theme->collectionAuxTable();
	sql += " SET grid_status = 0 ";
	if(!localDb_->execute(sql))
	{
		//error
		return;
	}

	base = " UPDATE "+ theme->collectionAuxTable();
	base += " SET grid_status = 1 "; 
	base += " WHERE object_id IN "; 
	for(unsigned int i=0; i<resultIndClause.size(); ++i)
	{
		sql = base + resultIndClause[i];
		if(!localDb_->execute(sql))
		{
			//error
			return;
		}
	}

	plugIngParams_.updateTVInterface(plugIngParams_); 

	//Criar uma nova coluna na tabela selecionada do tipo TeREAL
    TeAttributeRep rep;
	rep.name_ = "Spatial_Scan";
	rep.type_ = TeINT;

	TeAttribute att;
	if(!localDb_->columnExist(tableName,rep.name_, att))
	{
		if(!localDb_->addColumn(tableName,rep))
		{
			//error
			return;
		}
	}

	

    TeTable table;
	if(!localDb_->loadTable(tableName,table))
	{
		//error
		return;
	}
	int obIdPos = table.attrLinkPosition();
	int SpScPos;
	for(unsigned int c=0;c<table.attributeList().size();++c)
	{
		if((table.attributeList()[c]).rep_.name_ == rep.name_)
		{
			SpScPos = c;
			break;
		}
	}

	for(unsigned int l=0; l<table.size();++l)
	{
		string ObjId = table(l,obIdPos);
		
//		table.setValue(l,SpScPos, string("0"));
		table(l,SpScPos) = string("0");
		//verificar se existe o id no vetor de saida do scan
		for(int c=0;c<result_.size(); ++c)
		{
			if(ObjId == result_[c])
			{
//				table.setValue(l,SpScPos, string("1"));
				table(l,SpScPos) = string("1");
				break;
			}
		}
	}

	localDb_->updateTable(table);

	plugIngParams_.updateTVInterface(plugIngParams_); 

	scanButton->setOn(false);

	/*wait.resetWaitCursor();
	QMessageBox::information(this, 
		tr("Information"), tr("Success!!!!!!!!!!!!!!"));*/

	
	return;
}

void 
SpatialScanPluginWindow::tableComboBoxClickSlot(const QString&)
{
	TeTheme* theme =  plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1()); 
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
void SpatialScanPluginWindow::CloseSlot()
{
	//if(localDb_)
	//	delete localDb_;
	QWidget::close();
}
//sera chamado toda vez que a form for chamada através do menu
void SpatialScanPluginWindow::updateForm(PluginParameters* pluginParams)
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

void 
SpatialScanPluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}











	
