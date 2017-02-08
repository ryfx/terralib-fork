#include "ip.h"

///Inclusoes necessarias no Plugin
#include <IpopPluginWindow.h>
#include <TeGUIUtils.h> 
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeDatabaseUtils.h>
#include <TeWaitCursor.h>
#include <TeSTEFunctionsDB.h>
#include <TeDataTypes.h>
#include <vector>
#include <valarray>


#include <TeUtils.h>
#include <TeSTElementSet.h>
#include "TeQtGrid.h"
#include "TeQtViewsListView.h"
#include "TeQtThemeItem.h"
#include "TeDatabaseUtils.h"
#include <TeDatabaseFactory.h>
#include <TeDatabaseFactoryParams.h>

#include <qpushbutton.h>
#include <qcheckbox.h> 
#include <qcombobox.h>
#include <qmessagebox.h>
#include <qbuttongroup.h> 
#include <qtabwidget.h>
#include <qtextbrowser.h> 
#include <qspinbox.h> 

#include <help.h>

///Metodo de inicializacao da interface. Inicializa as variaveis de interface.
IpopPluginWindow::IpopPluginWindow(QWidget* parent):
	ipop(parent, "IpopPluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	localDb_ = false;
	control_ = false;
}

///Destrutor da classe, deleta a conexao com o Banco de dados ao sair da interface
IpopPluginWindow::~IpopPluginWindow()
{
	if(localDb_)
		delete localDb_;
}
///Metodo para preencher o comboBox com as matrizes de vizinhanca existentes
bool IpopPluginWindow::fillProxMatrixCombo(const QString& temaName)
 {
	TeTheme* curTheme_ = plugIngParams_.getCurrentViewPtr()->get(temaName.latin1());

	//Testa se existe algum tema
     if(!curTheme_)
		return false;  //mensagem de erro!!!!

	 //Inicializa as variaveis
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
///Marca a matriz selecionada pelo usuario como default
void IpopPluginWindow::setDefaultMatrix(const QString& temaName)
{
 	 TeTheme* curTheme_ = plugIngParams_.getCurrentViewPtr()->get(temaName.latin1());

	 //Seleciona qual o index da matriz desejada
	 int index = matrixComboBox->currentItem();
	 if(index<0)
	 {
		hide();
		return;
	 }

	 //Encontra a matriz no mapa de matrizes
	 map<int, int>::iterator it = comboIndex_.find(index);
	 if(it==comboIndex_.end())
	 {
		hide();
		return;
	 }

	 int gpmId = it->second;
	       
     //Atribui todas matrizes sem ser como default
     string up =  " UPDATE te_gpm SET gpm_default = 0 WHERE theme_id1 = "+ Te2String(curTheme_ ->id());
     if(!localDb_->execute (up))
	 {
		hide();
		return;
	 }
	 //Atribui a matriz desejada como default
      up =  " UPDATE te_gpm SET gpm_default = 1 WHERE gpm_id = "+ Te2String(gpmId);
      if(!localDb_->execute (up))
	  {
		  hide();
          return;
	  }
      
	   return;
}
///Metodo responsavel pelo preenchimento do combo de tabelas com as tabelas pertencentes
///ao tema especificado
void IpopPluginWindow::temaComboBoxClickSlot(const QString&)
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
///Metodo para abilitar/desabilitar o combo das matrizes de acorodo com a escolha do 
///usuario.
void IpopPluginWindow::Enable_matrixSlot()
{
	bool control;
	//Se o usuario tiver selecionado o check-box abilita os combo de matrizes de vizinhaca e o preenche
	if(matrixcheckBox->isChecked()){
		matrixComboBox->setEnabled(true);
		string temaName =  string(themeComboBox->currentText().ascii());
		control = fillProxMatrixCombo(temaName.c_str());
	}
	//Se nao, desabilita o combo de matrizes de vizinhanca
	else
		matrixComboBox->setEnabled(false);
}

void IpopPluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/
	Help help(this, "help", false);
	help.init("TerraClusterIpop.htm");

	if(!help.erro_)
		help.exec();

}

///Metodo principal da interface. Execucao do algoritmo onde sao executadas o metodo Ipop, responsabel por
///carregar todas as variaveis da interface e do banco de dados.
void IpopPluginWindow::okButtonClickSlot()
{
	// Parametros para simulação
	int montecarlo;								// Interaçoes de Monte Carlo
	QString str, saida = "";					// String auxiliar e de saida
	std::vector<double> pop;					// Vetor de populacao
	std::vector<double> cases;					// Vetor de casos
	std::vector< vector <double> > M;           // Pesos	
	std::vector<double> d;
	Saida_IPOP saida_ipop;


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
	//Pega a vista selecionada no Terraview
	 TeTheme* theme = plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1());
	if(!theme)
	{
		QMessageBox::critical(this, tr("Error"), tr("Not possible to load the Theme"));
		return;
	}

	//Pega o nome da tabela selecionada onde estao os atributos a serem utilizados
	string tableName = string(tableComboBox->currentText().ascii());
	if(tableName.empty())
	{
		QMessageBox::critical(this, tr("Error"), tr("Theme without Table"));
		return;
	}
	//Pega o nome das colunas que contem os atribuitos de populacao e caso respectivamente
	string popAttr = string(populationComboBox->currentText().ascii());
	string caseAttr =  string(caseComboBox->currentText().ascii());
	if(popAttr.empty() || caseAttr.empty())
	{
		QMessageBox::critical(this, tr("Error"), tr("Empty data"));
		return;
	}

	//----------------- pegar do banco os atributos e geometrias que serão utilizados na funcao
	//Carrega os atributos para memoria
	vector<string> attrs;
	attrs.push_back (tableName+"."+popAttr);
	attrs.push_back (tableName+"."+caseAttr);

	//Atrubui na variavel params quais os paramentros do tema devem ser carregados para memoria
	TeQuerierParams params(true, attrs);
	params.setParams(theme);
	
	//Tras para memoria, atraves da variavel querier os parametros selecionado do tema escolhido
	TeQuerier querier(params);
	//Testa se carregou as instacias desejadas
	if(!querier.loadInstances())
	{
		QMessageBox::critical(this, tr("Error"), tr("Not possible to load data"));
		return;
	}

	//Cria uma variavel do tipo instancia, onde essa variavel e equivalente a cada linha das tabelas 
	//selecionadas, ou seja, a variavel vai receber por vez os valores de uma linha dos dados no banco
	TeSTInstance obj;
	TeSTElementSet objSet(theme);
	//Para todas as linhas da tabela (todas instancias do banco) faca
	while(querier.fetchInstance(obj))
	{
		//armazenar as instancias do querier
		objSet.insertSTInstance(obj);
		//armazenar os identificadores
		objIds.push_back(obj.objectId());
		
		//pegar os atributos de populacao e caso
		string valPop = "";
		//Carrega na variavel tipo instancia o valor da populacao para aquela linha
		obj.getPropertyValue(valPop, 0);
		//Salva no vetor de populacao o vetor carregado do banco
		pop.push_back (atof(valPop.c_str()));

		string valCase = "";
		//Carrega na variavel tipo instancia o valor da populacao para aquela linha
		obj.getPropertyValue(valCase, 1);
		//Salva no vetor de populacao o vetor carregado do banco
		cases.push_back (atof(valCase.c_str()));
	}

	// Verifica se o numero de casos coincide com o número de areas
	if(objIds.size() != pop.size())
	{
		QMessageBox::critical(this, tr("Error"), tr("Population and/or Cases files are incompatible with the Number of Areas"));
		return;
	}
	//Verifica se o tamanho do vetor de casos coicide com o tamanho do vetor de populacao
	if(cases.size() != pop.size())
	{
		QMessageBox::critical(this, tr("Error"), tr("Population and Cases files are incompatible"));
		return;
	}
		//Escreve na janela de saida a mensagem
		Output_text->setText(tr(" Ipop \n running..."));
		
		//pergunta se o tema selecionado possui poligonos
		if(!theme->layer()->hasGeometry(TePOLYGONS))
		{
			QMessageBox::critical(this, tr("Error"), tr("O Layer indicado nao possui Geometria"));
			return;
		}

		std::valarray<double> x(objIds.size()), n(objIds.size());
		unsigned int i;
		/*Abre os vetores n[i] número de casos na área i, x[i] tamanho da população na área i,
		  o número de areas "m" e a matriz Mij de pesos atribuidos a um par de casos
			sendo Mii = 2/d[i], para a mesma regiao;
				  Mij = 1/sqrt(d[i][dj], para regioes diferentes conectadas;
				  Mij = 0, para regioes nao conectadas */

		//Faz a conversao dos dados carregados para a forma da estrutura de entrada necessaria 
		//na funcao previamente existente
		M.resize(objIds.size());
		d.resize(objIds.size());
		for(i=0;i<objIds.size();i++){
			M[i].resize(objIds.size(),0.0);
			x[i] = pop[i];
			n[i] = cases[i];
		}
		//Cria os pesos para a matrix
		for(i=0;i<objIds.size();i++){
			d[i] = x[i]/x.sum();
			M[i][i] = 2.0/d[i];
		}

	//gerar a matriz de proximidade
	//load prox matrix or construct one
	TeGeneralizedProxMatrix<TeSTElementSet>*			proxMatrix=0;
	TeProxMatrixConstructionStrategy<TeSTElementSet>*	constStrategy=0;
	int themeId = theme->id();

	//-------------- Load prox matrix or create new
	//Testa se o usuario selecionou e existe alguma matriz
	if((matrixcheckBox->isChecked()) && (matrixComboBox->count() != 0)){
		//Define como default a matriz selecionada e a carrega para memoria
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
	    //Descobre qual a geometria do tema
		TeGeomRep geomRep = TeGEOMETRYNONE;
		if(theme->layer()->hasGeometry (TePOLYGONS))
			geomRep = TePOLYGONS;

		if(geomRep==TeGEOMETRYNONE)
		{
			QString msg = tr("The theme must have a geometric representation of polygons");
			QMessageBox::warning(this, tr("Warning"), msg);
			return;
		}
		
		//Abre uma barra de andamento de criacao da matriz na tela
		if(TeProgress::instance())
		{
			string caption = tr("Ipop").latin1();
			TeProgress::instance()->setCaption(caption.c_str());
			string msg = tr("Building the Proximity Matrix. Please, wait!").latin1();
			TeProgress::instance()->setMessage(msg);
		}
		
		//create matrix and save in the database
		//Escolhe qual a estrategia de construcao da matriz
		constStrategy = new TeProxMatrixLocalAdjacencyStrategy (&objSet, geomRep); 
		TeProxMatrixNoWeightsStrategy sw_no;
		TeProxMatrixNoSlicingStrategy ss_no;
		proxMatrix = new TeGeneralizedProxMatrix<TeSTElementSet>(constStrategy, &sw_no, &ss_no);
		
		//Constroi a matriz com a estrategia escolhida
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
			//cria no banco de dados uma tabela com os valores da matriz de proximidade
			if(!createGPMMetadataTable(theme->layer()->database()))
			{
				QMessageBox::warning(this, tr("Error"), 
					tr("It was not possible to save proximity matrix in database!"));
				savedMatrix = false;
			}
		}

		//Atribui o nome da tabela de matriz no banco
		string tableName = "te_neighbourhood_"+ Te2String(theme->id());
		string tempTable = tableName;
		int count = 1;
		//Testa se o banco ja possui matriz com mesmo nome
		while(theme->layer()->database()->tableExist(tempTable))
		{
			tempTable = tableName+"_"+Te2String(count);
			++count;
		}
        //Salva a matriz e grava a matriz no banco com o nome escolhido
		if(savedMatrix && (!createNeighbourhoodTable(theme->layer()->database(), tempTable)))
		{
			QMessageBox::warning(this, tr("Error"), 
					tr("It was not possible to save proximity matrix in database!"));
			savedMatrix = false;
		}

		//Acrescenta na tabela de controle de matrizes o nome da matriz criada
		proxMatrix->neighbourhood_table_=tempTable;


		//load object ids
		if(savedMatrix && (!insertGPM(theme->layer()->database(), proxMatrix, objIds)))
		{
			QMessageBox::warning(this, tr("Error"), 
					tr("It was not possible to save proximity matrix in database!"));
			savedMatrix = false;
		}
		//preenche o combo de matrizes de proximidades com as existente e a nova
		fillProxMatrixCombo(themeComboBox->currentText().ascii());
	}
	
	//FIM DA GERACAO DA MATRIX
		
	    //Transforma os dados da matriz de proximidade desejada no formato de entrada da funcao
		int posArea1, posArea2;
		//Para cada objeto carregado, faca
		TeSTElementSet::iterator it = objSet.begin();
		while(it != objSet.end())
		{
			//Carrega os vizinhos daquele objeto de acorodo com a matriz escolhida
			TeNeighboursMap neigs = proxMatrix->getMapNeighbours ((*it).objectId());
			//Para cada vizinho daquele objeto faca
			TeNeighboursMap::iterator it2 = neigs.begin();
			for(i=0;i<objIds.size();++i)
			{
				//Descobre qual a sua posicao relativa no vetor de identificadores
				if((*it).objectId() == objIds[i])
				{
					posArea1 = i;
					break;
				}
			}         
			//Para seus vizinhos faca
	    	while(it2 != neigs.end())
			{
				
				string objectId = it2->first;
				//Descobre qual a sua posicao relativa no vetor de identificadores
				for(i=0;i<objIds.size();++i)
				{
					if(objectId == objIds[i])
					{
						posArea2 = i;
						break;
					}
				}
				//A matriz de vizinhaca nas posicao (objeto,vizinho[i]) recebe o valor calculado para 
				//execucao da funcao
				M[posArea1][posArea2] = 1/sqrt(d[posArea1]*d[posArea2]);
				++it2;
			}
			++it;
		}

		// Executa o Método Ipop
		if(iFunction(n,x,M,d,n.size(),montecarlo,saida_ipop))
		{
			QMessageBox::critical(this, tr("Error"), tr("Ipop Memory Error"));
			return;
		}

		// Imprime os resultados na interface
		saida = tr("Ipop \n \n Areas: ") + str.setNum(n.size());
		saida += tr("\n cases: ") + str.setNum(n.sum());
		saida += tr("\n population: ") + str.setNum(x.sum(),15,0);
		saida += tr("\n\n Ipop: ") + str.setNum(saida_ipop.Ipop,10,7);
		saida += tr("\n\n Ipop': ") + str.setNum( saida_ipop.Ipopl,10,7);
		saida += tr("\n\n E[I]: ") + str.setNum( saida_ipop.esp,10,7);
		saida += tr("\n\n Assumption Randomization \n Variance: ") + str.setNum(saida_ipop.var,10,7 );
		saida += tr("\n Z-Score: ") + str.setNum( saida_ipop.Z,13,7 );
		saida += tr("\nSignificance: ") + str.setNum( saida_ipop.pvalor,10,7);
		saida += tr("\n\n Aproximation \n Variance: ") + str.setNum( saida_ipop.varA,10,7);
		saida += tr("\nSignificance: ") + str.setNum( saida_ipop.pMC,10,7);
		
		Output_text->setText(saida);

	return;
}
///Metodo responsavel pelo preenchimento do combo de atributos das tabelas pertencentes
///a tabela especificado
void IpopPluginWindow::tableComboBoxClickSlot(const QString&)
{
	TeTheme* theme = plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1());; 
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
///Metodo para encerramento da interface
void IpopPluginWindow::CloseSlot()
{
	//if(localDb_)
	//	delete localDb_;
	QWidget::close();
}
///Ao ser inicializada a interface pelo menu do terraview, esse metodo e responsavel
///pela conexao do plugin com banco de dados e devidas inicializacoes que devam ser feitas.
void IpopPluginWindow::updateForm(PluginParameters* pluginParams)
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
///Metodo responsavel por esconder a tela de interface do plugin
void IpopPluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}











	
