//Bibliotecas

#include "funcoes.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

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

#include <kNNPluginWindow.h>

#include <TeGUIUtils.h> 
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeDatabaseUtils.h>
#include <TeWaitCursor.h>
#include <TeSTEFunctionsDB.h>
#include <TeDataTypes.h>

#include <TeSTElementSet.h>
#include <TeDatabaseFactory.h>
#include <TeAppTheme.h>

#include <help.h>

//#include <help.h>

///Metodo de inicializacao da interface. Inicializa as variaveis da interface.
kNNPluginWindow::kNNPluginWindow(QWidget* parent):
	kNN(parent, "kNNPluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	localDb_ = false;
	control_ = false;

	kNNLineEdit->setValidator(new QDoubleValidator(this));

//	help_= 0;
}



///Destrutor da classe, deleta a conexao com o banco de dados ao sair da interface.
kNNPluginWindow::~kNNPluginWindow()
{
	if(localDb_)
		delete localDb_;
}

///Funcao responsavel pelo preeenchimento do combo de temas com as tabelas pertencentes ao tema especificado. 
void kNNPluginWindow::temaComboBoxClickSlot(const QString&)
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


///Responsavel pelo preenchimento do combo de tabelas  das quais pertencentes ao tema especificado.
void kNNPluginWindow::enabletimecolumnSlot()
{
	if(selectimeCheckBox->isChecked())
	{
		timeComboBox->setEnabled(false);
		tableComboBox->setEnabled(true);
		columnComboBox->setEnabled(true);
	
	}
	else
	{
		timeComboBox->setEnabled(true);
		tableComboBox->setEnabled(false);
		columnComboBox->setEnabled(false);
	}
}

void kNNPluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/
	Help help(this, "help", false);
	help.init("TerraClusterKNN.htm");

	if(!help.erro_)
		help.exec();

}

///Funcao principal da interface. Executa o Metodo k-NN de Jacquez quando o botão "OK" é clicado e apresenta resultados finais na interface.
void kNNPluginWindow::okButtonClickSlot()
{
 
  using namespace std;
  int Erro, i, nareas;
  double k, Dist, pvalDist, pvalBonfJk, pvalSimesJk, pvalBonfDJk, pvalSimesDJk;
  QString str, saida = "";
  vector<string> attrs;
  vector<string> objIds;
  
  
//Carrega a variavel k 
  k = kNNLineEdit->text().toDouble();

  //Obtém o número de Simulações Monte Carlo que o usuário deseja

  long NumSim = 99;
  if (_999RadioButton->isOn() ==  true)
     NumSim = 999;


 vector<double> coordx, coordy, coordt;
 vector<int> Jk0, DJk;
 vector<double> pvalJk, pvalDJk;


 //----------------- pegar o tema, a tabela e os atributos selecionados na tela
	TeTheme* theme =  plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1()); 
	if(!theme)
	{
		QMessageBox::critical(this, tr("Error"), tr("Not possible to load the Theme"));
		return;
	}

 TeQuerierParams params(true, false);
 params.setParams(theme);
 TeQuerier querier;

 if(selectimeCheckBox->isChecked())
    {

    //Carrega a tabela de atributos
    string tabelaName = string(tableComboBox->currentText().ascii());
    if(tabelaName.empty())
      {
      QMessageBox::critical(this, tr("Error"), tr("Theme without Table"));
      return;
      }

    //Carrega os atributos
    string TimeAttr = string(columnComboBox->currentText().ascii());
    if(TimeAttr.empty())
      {
      QMessageBox::critical(this, tr("Error"), tr("Empty data"));
      return;
      }

    //Carrega os atributos para memoria
    attrs.push_back (tabelaName+"."+TimeAttr);

    //----------------- pegar do banco os atributos e geometrias que serão utilizados na funcao
	params.setFillParams(true, false, attrs);

	querier.refresh(params);
    
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


      //carregar os pontos do tema
      TeCoord2D p;
	  obj.centroid(p);
      
      coordx.push_back(p.x());
      coordy.push_back(p.y());

      string valTime = "";
      obj.getPropertyValue(valTime, 0);
      coordt.push_back(atof(valTime.c_str()));
      }

    // Verifica se o numero de casos coincide com o número de areas
    if(objIds.size() != coordx.size())
      {
      QMessageBox::critical(this, tr("Error"), tr("Not all of the points have (x,y) coordinates"));
      return;
      }
    if(coordx.size() != coordt.size())
      {
      QMessageBox::critical(this, tr("Error"), tr("Not all of the points have time coordinate"));
      return;
      }

    }

 else
    {
    int intchrono = timeComboBox->currentItem();
    TeChronon Chrono;
    switch (intchrono)
       {
       case 0:
        Chrono =TeHOUR;
       break;
       case 1:
        Chrono=TeDAY;
       break;
       case 2:
        Chrono= TeMONTH;
       break;
       case 3:
        Chrono= TeYEAR;
       break;
       default:
        cout << "ERRO" << endl;
      break;
      }



 // -------- Criar um Querier com os parametros acima
    params.setParams(theme, Chrono); //associar o querier ao tema contraPessoa e montar os frames por mês

	querier.refresh(params); //cria o querier

  // -------- Retorna o número de frames gerado baseado no chronon associado ao querier
    int numTimeFrames = querier.getNumTimeFrames();
	if (numTimeFrames == 0){
				QMessageBox::critical(this, tr("Error"), tr("The Table doesn't have time specification"));
				return;
			}

 // -------- Para cada frame retornar todos os eventos temporais que estão nesse frame
    TeSTElementSet objSet(theme);


    for(int frame = 0; frame < numTimeFrames; ++frame)
       {
       if(querier.loadInstances(frame))
         {
         // para cada evento ou instancia temporal
	 TeSTInstance sti;
	 while(querier.fetchInstance(sti))
       {
		//armazenar as instancias do querier
		objSet.insertSTInstance(sti);
		//armazenar os identificadores
		objIds.push_back(sti.objectId());
	    // pegar o ponto (x,y) correspondente
	    TeCoord2D p;
	    sti.centroid(p);
    
	    coordx.push_back(p.x());
	    coordy.push_back(p.y());

	    coordt.push_back(frame);
            }
         }
       }

    if(objIds.size() != coordx.size())
       {
       QMessageBox::critical(this, tr("Error"), tr("Not all of the points have (x,y) coordinates"));
       return;
       }
    if(coordx.size() != coordt.size())
       {
       QMessageBox::critical(this, tr("Error"), tr("Not all of the points have time coordinate"));
       return;
       }
    }

	if(k>=coordx.size() || k<=0)
	  {
	  Erro = 1;		//O numero de vizinhos deve ser menor do que o numero de pontos.
      QMessageBox::critical(this, tr("Error"), tr("The number of neighbors must be smaller than the number of cases and bigger than 0"));
      return;
	  }

    Output_text->setText(tr("k-Nearest Neighbor Method \n running...\n \n"));
    jacqueztest(coordx, coordy, coordt, k, NumSim, Jk0, DJk, pvalJk, pvalBonfJk, pvalSimesJk);

      Erro = 0;

	  //********************** Como criar uma tabela externa e associá-la ao tema

 	// 1) Verificar se a tabela já existe
	string tableName = "kNN_Method";
	if (localDb_->tableExist(tableName))
	{
		//mensagem de erro para escolher outro nome da tabela
		//ou pode apagar a tabela existente
		localDb_->deleteTable("kNN_Method");
	}
	
	// 2) Criar o esquema da tabela em memória
	TeAttributeList attList;	
	
	TeAttribute at;
	at.rep_.name_= "k";	
	at.rep_.type_ = TeINT;	
	at.rep_.isPrimaryKey_ = true;
	attList.push_back(at); 

	at.rep_.name_= "Jk";	
	at.rep_.type_ = TeINT;	
	at.rep_.isPrimaryKey_ = false;
	attList.push_back(at); 

	at.rep_.name_= "Upper_tail_Pk";	
	at.rep_.type_ = TeREAL;	
	attList.push_back(at); 

	at.rep_.name_= "DJk";	
	at.rep_.type_ = TeINT;	
	attList.push_back(at); 

		
	// 3) Criar a tabela no banco
	if (!localDb_->createTable(tableName, attList))
	{
		///menagem de erro
	}		
		
	// 4) Inserir os dados em memória e inserir no banco (de 100 em 100 registros em memória vc chama o insertTable
	TeTable table(tableName, attList, "k", "", TeAttrExternal);	
	table.relatedTableId(12);  //o identificador da tabela estática do tema que essa tabela externa irá se linkar
	table.relatedAttribute("ttttt"); //o nome do atributo da tabela estática que essa tabela externa irá se linkar


	for(i = 0; i < k; i++)
	{
		TeTableRow row1;	
		row1.push_back(Te2String(i+1));
		row1.push_back(Te2String(Jk0[i]));
		row1.push_back(Te2String(pvalJk[i]));
		row1.push_back(Te2String(DJk[i]));
		table.add(row1);
	}

	// 5) Grava tudo pro banco
	if (!localDb_->insertTable(table))				// save the table into the database
	{
		//menagem de erro
	}
	
	// 6) Inserir as informações no banco sobre a tabela 
	localDb_->insertTableInfo(-1, table); //No caso de criar uma tabela estatica colocar no lugar de -1 o ID do Layer


	// 7) Associar essa tabela estática(que está toda dentro do banco) ao tema
	localDb_->insertThemeTable(theme, table); 


        saida =  tr("k-NN Jacquez Results       \n\n");
        saida += tr("Total number of cases = ") + str.setNum(coordx.size());
        saida += tr("\n\n    Combined P-value: \n\n");
	    //saida += tr("Statistical Distance Test statistic  = ")+ str.setNum(Dist)+"\n";
	    saida += tr("Number of Monte Carlo simulations    = ")+ str.setNum(NumSim)+"\n";
	    //saida += tr("P-value from Monte Carlo simulations = ")+ str.setNum(pvalDist)+"\n\n";
	    saida += tr("Bonferroni P-value (J)               = ")+ str.setNum(pvalBonfJk)+"\n";
	    saida += tr("     Simes P-value (J)               = ")+ str.setNum(pvalSimesJk)+"\n\n";
	  

	  saida += tr("Other results are stored in external table 'kNN_Method'");
	  saida += tr("To see this external table go to 'Views/Themes' and click on the theme(that you have used) with the right button. Then click on 'Link External' Table \n\n");
/*for(i = 0; i < k; i++)
	{
	saida += str.setNum(i+1)+"\t" +str.setNum(Jk0[i]) +"\t" +str.setNum(pvalJk[i])+"\t" +str.setNum(DJk[i]) +"\n";
	}*/

 

    Output_text->setText(saida);
    
	
 
      return ;
}


///Carrega tabela especificada pelo usuario
void kNNPluginWindow::table1ComboBoxClickSlot(const QString&)
{
	TeTheme* theme =  plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1());; 
	if(!theme)
		return;

	//carregar os atributos (int ou double) da tabela selecionada
	vector<TeAttrDataType> attrType;
	attrType.push_back (TeINT);
	attrType.push_back (TeREAL);
	if(!fillColumnCombo(attrType, theme, columnComboBox, tableComboBox))
	{
		//error message
		return;
	}
	return;

}


///Ao ser inicializada a interface pelo menu do terraview, essa funcao e responsavel pela conexao do plugin com o banco de dados e devidas inicializacoes que devam ser feitas.
void kNNPluginWindow::updateForm(PluginParameters* pluginParams)
{
	plugIngParams_ = (*pluginParams);

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

	//carregar os temas da vista corrente e colocar o tema corrente como default
	if(!fillThemeCombo(TePOINTS,pluginParams->getCurrentViewPtr(), 
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
void kNNPluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}

/*void PluginWindow::helpPushButton_clicked()
{
	if(help_)
	 delete help_;
	help_= new Help(this, "help", false);
	help_->init("kNNWindow.htm");
	if(help_->erro_ == false)
	{
	 help_->show();
	 help_->raise();
	}
	else
	{
	delete help_;
	help_=0;
	}
}*/
