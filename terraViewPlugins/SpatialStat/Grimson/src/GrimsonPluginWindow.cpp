//Bibliotecas

#include <normal01.h>
#include <poisson.h>
#include <fatorial.h>

#include <math.h>
#include <valarray>

#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h> 
#include <qradiobutton.h>
#include <qmessagebox.h>
#include <qbuttongroup.h> 
#include <qtabwidget.h>
#include <qtextbrowser.h> 
#include <qspinbox.h> 
#include <qvalidator.h>
#include <qstring.h> 
#include <qlineedit.h> 

#include <GrimsonPluginWindow.h>
#include <curve.h>

#include <TeGUIUtils.h> 
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeDatabaseUtils.h>
#include <TeWaitCursor.h>
#include <TeSTEFunctionsDB.h>
#include <TeDataTypes.h>
#include <TeSTElementSet.h>
#include <TeProxMatrixSlicingStrategy.h>
#include <TeGeneralizedProxMatrix.h>
#include <TeDatabaseFactory.h>
#include <TeDatabaseFactoryParams.h>
#include <TeAppTheme.h>
#include <TeApplicationUtils.h>

#include <help.h>

///Metodo de inicializacao da interface. Inicializa as variaveis da interface.
GrimsonPluginWindow::GrimsonPluginWindow(QWidget* parent):
	grimson(parent, "GrimsonPluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	localDb_ = false;
	control_ = false;
	gra_ = 0;
	ncols_ = 0;

	cortelineEdit->setValidator(new QDoubleValidator(this));
}


///Destrutor da classe, deleta a conexao com o banco de dados ao sair da interface.
GrimsonPluginWindow::~GrimsonPluginWindow()
{
	if(localDb_)
		delete localDb_;
}


///Preenche o campo combobox com as matrizes de proximidade existentes
bool GrimsonPluginWindow::fillProxMatrixCombo(const QString& temaName)
 {
	 
	//Carrega o tema desejado
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
///Marca a Matriz de Proximidade selecionada como default
void GrimsonPluginWindow::setDefaultMatrix(const QString& temaName)
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
///Habilita ou desabilita o campo matriz de proximidade de acordo com a escolha do usuario.
void GrimsonPluginWindow::Enable_matrixSlot()
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


///Responsavel pelo preenchimento do combo de tabelas  das quais pertencentes ao tema especificado.
void GrimsonPluginWindow::temaComboBoxClickSlot(const QString&)
{
	//Carrega todas as tabelas de atributos do tema selecionado
	if(!fillTableCombo(localDb_->viewMap()[ViewId_], tableComboBox, themeComboBox))
	{
		//error message
		return;
	}

	table1ComboBoxClickSlot(tableComboBox->currentText());
	return;
}

void GrimsonPluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/
	Help help(this, "help", false);
	help.init("TerraClusterGrimson.htm");

	if(!help.erro_)
		help.exec();

}

///Funcao principal da interface. Executa o Metodo de Grimson quando o botão "OK" é clicado e apresenta resultados finais na interface.
void GrimsonPluginWindow::okButtonClickSlot()
{	
	
	//Parâmetros Auxiliares

	bool temporal=false;
	bool aux1, aux2;
	QString str, saida = "";					 // String auxiliar e de saida		
	std::vector<double> attrb1, attrb2;
	std::valarray<double> khat,upper,lower;
	std::vector<string> objIds;
	std::vector<double> h;
	vector<string> attrs1;

    //Criando o vetor de possíveis cores para o gráfico
	if( _colors.empty() ) {
	_colors.push_back(Qt::black);
	_colors.push_back(Qt::red);
	/*_colors.push_back(Qt::green);
	_colors.push_back(Qt::cyan);
	_colors.push_back(Qt::magenta);
	_colors.push_back(Qt::yellow);
	_colors.push_back(Qt::gray);*/
	}
	gra_= 0;
 
/* Variáveis principais utilizadas no Método de Grimson - Legenda

- pcorte -> ponte de corte tal que um objeto com valor de um determinado atributo
acima, abaixo ou igual a este ponto de corte será considerado um objeto indexado;
- nobj -> número de objetos no total;
- n -> número de objetos indexados de acordo com o ponto de corte;
- A -> número de pares de objetos adajcentes e que são objetos indexados;
- y -> número médio de objetos adjacentes por objeto;
- Var -> Variância de y;
- EA -> valor esperado do número de objetos adajcentes que
são considerados objetos indexados sob a hipótese nula de que
objetos de alto risco são distribuídos aleatoriamente;
- CR -> coeficiente de regularidade;
- CV -> coeficiente de variação;
- VarA -> Variância de A;
- pvalorP - p-valor assumindo a distribuição de Poisson(EA);
- pvalorN - p-valor assumindo a distribuição Normal(0,1); 

*/

	int ind=0, nobj=0, A = 0, viz = 0, n=0, regiaotrans;
	double pcorte, y, Vary = 0, EA;
	double CR, CV, VarA, par, pvalorP, pvalorN;

	vector<estrutura> Dados;
  
	TeWaitCursor wait;
	
	//Carrega as variavies da interface
	pcorte = cortelineEdit->text().toDouble();
	

	//Seleciona o tipo de análise (Espacial, Temporal)
	switch(this->tabuttonGroup->selectedId())	
	{
		case 0:
		  break;
		case 1:
			{
			temporal = true;
			break;
			}
	}
	
	

	//Obtém o tema, a tabela e os atributos selecionados na tela
	TeTheme* theme = plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1());
	int themeId = theme->id();
	if(!theme)
	{
		QMessageBox::critical(this, tr("Error"), tr("Not possible to load the Theme"));
		return;
	}
	string tableName = string(tableComboBox->currentText().ascii());
	if(tableName.empty())
	{
		QMessageBox::critical(this, tr("Error"), tr("Theme without Table"));
		return;
	}
	string dateAttr;
	string caseAttr =  string(caseComboBox->currentText().ascii());
	if(caseAttr.empty())
	{
		QMessageBox::critical(this, tr("Error"), tr("Empty data"));
		return;
	}

	//Pega do banco os atributos e geometrias que serão utilizados no método de Grimson
	vector<string> attrs;
	
	attrs.push_back(tableName+"."+caseAttr);

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
		//Armazena as instancias do querier
		objSet.insertSTInstance(obj);
		//Armazena os identificadores
		objIds.push_back(obj.objectId());
		estrutura aux;
		aux.regiao = ind;
		//Pega os atributos
		
		string valCase = "";
		obj.getPropertyValue(valCase, 0);
		aux.variavel = atof(valCase.c_str());
			
		switch(this->comparisonbuttonGroup->selectedId())	
			{
			case 0:
				{
				if(aux.variavel <= pcorte)
				  {
				   aux.highrisk = true;
				   n++;
				  }
				else
                  aux.highrisk = false;

				break;
				}
			case 1:
				{
				if(aux.variavel < pcorte)
			      {
				  aux.highrisk = true;
			      n++;
			      }
				else
                  aux.highrisk = false;

				break;
			    }
			case 2:
				{
				if(aux.variavel == pcorte)
			      {
				  aux.highrisk = true;
				  n++;
				  }
				else
                  aux.highrisk = false;

				break;
				}

			case 3:
				{
				if(aux.variavel > pcorte)
			      {
				  aux.highrisk = true;
				  n++;
				  }
				else
				  aux.highrisk = false;

				break;
				}

			case 4:
			   {
			   if(aux.variavel >= pcorte)
				 {
				 aux.highrisk = true;
				 n++;
				 }
			   else
				 aux.highrisk = false;

			   break;
			   }
			}

		Dados.push_back(aux);
		ind++;
	}

    nobj = ind;

    
	if(!temporal)
	{


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
		
	unsigned int i;
    TeSTElementSet::iterator it = objSet.begin();
	while(it != objSet.end())
	    {
		TeNeighboursMap neigs = proxMatrix->getMapNeighbours ((*it).objectId()); 
		TeNeighboursMap::iterator it2 = neigs.begin();
		for(i=0;i<objIds.size();++i)
		    {
			if((*it).objectId() == objIds[i])
			   {
			   regiaotrans = i;
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
				   Dados[regiaotrans].vizinhos.push_back(i);
				   if(Dados[regiaotrans].highrisk == true && Dados[i].highrisk == true)
                      A++;
                   
				   viz++;
 			 	   break;
				   }
				}
			  ++it2;
			}
		 ++it;
		}
	
	//Cálculo da Estatística A
	A /= 2.0;

	//Cálculo do número médio de objetos adjacentes por objeto
    y = viz/(double)nobj;

	//Cálculo da Variâcia de y  - Var(y)
    for(unsigned int i=0; i<Dados.size(); i++)
      Vary += (Dados[i].vizinhos.size()- y)*(Dados[i].vizinhos.size() - y);

    Vary /=(double)(nobj);

	}

	else
	{
	for(unsigned int k=1; k<Dados.size(); k++)
	  {
	  aux1 = Dados[k-1].highrisk;
	  aux2 = Dados[k].highrisk;
	  if(aux1==true && aux2==true)
		  A++;
	  }

	y =2*(nobj-1)/(double)nobj;
	Vary = (nobj-2)*(2-y)*(2-y) + 2*(1-y)*(1-y);
	Vary/=(double)(nobj);
	}

    //Cálculo do valor esperado de A sob a hipótese nula de que os objetos 
	//indexados são distribuídos aleatoriamente
	
	EA =y*n*(n-1)/(double)(2*(nobj-1));

	//Cálculo do coeficiente de regularidade
    CR = 1 + 2*(y-1)*(n-2)/(double)(nobj-2) + (nobj*y - 4*y + 2)*(n-2)*(n-3)/(double)(2*(nobj-2)*(nobj-3)) - EA;

    CR = CR*EA;

	//Cálculo do coeficiente de variação
    CV = fac(n,3)/(double)fac(nobj-1,2) - fac(n,4)/(double)fac(nobj-1,3);
    CV = CV*Vary;

    //Cálculo da Variância de A
	VarA = CR + CV;

	//Cálculo do P-Valor segundo a distribuição de Poisson
    pvalorP = 1 - AcPoisson(A,EA);

    //Cálculo do z-escore
	par = (A-EA)/sqrt(VarA);

	//Cálculo do P-Valor segundo a distribuição Normal
    pvalorN = 1-AcNormal(par);


    if(A>0)
	{
	// Imprime os resultados na interface
    saida = tr("Grimson's Method \n \n Number of Objects: ") + str.setNum( nobj );
	saida += tr("\n Number of Indexed Objects: ") + str.setNum(n);
	saida += tr("\n Average Number of Neighbors by Object: ") + str.setNum(y);
	saida += tr("\n Var(y): ") + str.setNum(Vary);
	saida += tr("\n Component of Regularity: ") + str.setNum(CR);
	saida += tr("\n Component of Change: ") + str.setNum(CV);
	saida += tr("\n Statistical A: ") + str.setNum(A);
    saida += tr("\n E(A): ") + str.setNum(EA);
	saida += tr("\n Var(A): ") + str.setNum(VarA);
    saida += tr("\n z-score: ") + str.setNum( par );
	saida += tr("\n Significance \n        normal: ") + str.setNum(pvalorN);
	saida += tr("\n        poisson: ") + str.setNum(pvalorP,5,4);
	


    Output_text->setText(saida);
    

   //Construção do gráfico
 
	valarray<double> eixoX(2*A);
	valarray<double> eixoYP(2*A), eixoYN(2*A);

 
    for(int index = 0; index < 2*A; index++)
	   {
       eixoX[index]=index;
   
       eixoYP[index]=1 - AcPoisson(eixoX[index],EA);
	   eixoYP[index]=eixoYP[index]*100;
    
       par = (eixoX[index]-EA)/sqrt(VarA);
       eixoYN[index]=1-AcNormal(par);
	   eixoYN[index]=eixoYN[index]*100;

	   }


	if( gra_ == NULL ) 
	  {
	  gra_ = new XYPlotGraphic(this, " P ");
	  gra_->plot->setTitleYAxis(tr("P-value(%)"));
	  gra_->plot->setTitleXAxis(tr("Adjacent high-risk pairs"));
	  }

	Curve *crv1 = new Curve(eixoYN,eixoX);
	Curve *crv2 = new Curve(eixoYP,eixoX);
	crv1->setColor(_colors[0]);
	crv2->setColor(_colors[1]);
	gra_->plot->append(crv1);
	gra_->plot->append(crv2);
	gra_->show();
	}

	else
	{
		// Imprime os resultados na interface
    saida = tr("Grimson's Method \n \n Number of Objects: ") + str.setNum( nobj );
	saida += tr("\n Number of Indexed Objects: ") + str.setNum(n);
	saida += tr("\n Average Number of Neighbors by Object: ") + str.setNum(y);
	saida += tr("\n Var(y): ") + str.setNum(Vary);
	saida += tr("\n Component of Regularity: ") + str.setNum(CR);
	saida += tr("\n Component of Change: ") + str.setNum(CV);
	saida += tr("\n Statistical A: ") + str.setNum(A);
    saida += tr("\n E(A): ") + str.setNum(EA);
	saida += tr("\n Var(A): ") + str.setNum(VarA);
    saida += tr("\n Significance \n        normal: ") + str.setNum(1.000);
	saida += tr("\n        poisson: ") + str.setNum(1.000);

	Output_text->setText(saida);
	}
	return;
}

///Carrega tabela especificada pelo usuario
void GrimsonPluginWindow::table1ComboBoxClickSlot(const QString&)
{
	TeTheme* theme = plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1());
	
	if(!theme)
		return;

	//Carrega os atributos (int ou double) da tabela selecionada
	vector<TeAttrDataType> attrType;
	attrType.push_back (TeINT);
	attrType.push_back (TeREAL);
	if(!fillColumnCombo(attrType, theme, caseComboBox, tableComboBox))
	{
		//error message
		return;
	}

	return;
}

///Ao ser inicializada a interface pelo menu do terraview, essa funcao e responsavel pela conexao do plugin com o banco de dados e devidas inicializacoes que devam ser feitas.
void GrimsonPluginWindow::updateForm(PluginParameters* pluginParams)
{
	plugIngParams_ = (*pluginParams);
	Output_text->clear();
	matrixcheckBox->setChecked(false);
	Enable_matrixSlot();

	if (plugIngParams_.getCurrentDatabasePtr() == NULL)
	{
		QMessageBox::critical(this, tr("Error"), tr("Empty data"));
				return;

	}
	//Fixa qual vista é carregada inicialmente na abertura do plugin
	if(!control_){
		ViewId_ = plugIngParams_.getCurrentViewPtr()->id();
		control_ = false;
	}

	//Checa se os ponteiros do pluginParams inclusive o database!!!!!! 
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
		hide();
		return;     
	}      
		
	if(!localDb_->isConnected())
	{
		QMessageBox::critical( this, "TerraView", 
		tr( "Unable to connect to database" ) );   
		hide();
		return;
	} 

	//load theme map, layer map, etc, etc
	localDb_->loadViewSet("");

	//carregar os temas da vista corrente e colcoar o tema corrente como default
	if(!fillThemeCombo(plugIngParams_.getCurrentViewPtr(), 
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

///Funcao responsavel por esconder a tela de interface do plugin
void GrimsonPluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}
