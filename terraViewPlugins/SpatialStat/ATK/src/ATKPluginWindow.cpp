//Bibliotecas

#include "Event.h"
#include "ScanInt.h"
#include "ClusterAlg.h"
#include "ProspectiveAnalysis.h"
#include "Clustering.h"

#include <time.h>

//#include <conio> 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <valarray>
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

#include <ATKPluginWindow.h>

#include <TeGUIUtils.h> 
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeDatabaseUtils.h>
#include <TeWaitCursor.h>
#include <TeSTEFunctionsDB.h>
#include <TeDataTypes.h>
#include <TeDatabase.h>

#include <TeSTElementSet.h>
#include <TeDatabaseFactory.h>
#include <TeDatabaseFactoryParams.h>
#include <TeAppTheme.h>

#include <help.h>

ATKPluginWindow::ATKPluginWindow(QWidget* parent):
	ATK(parent, "ATKPluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	localDb_ = false;
	control_ = false;

	percSpaceLineEdit->setValidator(new QDoubleValidator(this));
	percTimeLineEdit->setValidator(new QDoubleValidator(this));
	percPopLineEdit->setValidator(new QDoubleValidator(this));
	minPopLineEdit->setValidator(new QDoubleValidator(this));
	numClustersLineEdit->setValidator(new QDoubleValidator(this));
	
}


ATKPluginWindow::~ATKPluginWindow()
{
	if(localDb_)
		delete localDb_;
}


void ATKPluginWindow::temaComboBoxClickSlot(const QString&)
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



void ATKPluginWindow::enabletimecolumnSlot()
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


void ATKPluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	
	  QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/

	Help help(this, "help", false);
	help.init("TerraClusterATK.htm");

	if(!help.erro_)
		help.exec();

}


//Executa o programa quando o botão "OK" é clicado
void ATKPluginWindow::okButtonClickSlot()
{
  using namespace std;
  
  int numCluster, testType, minPop;
  double percSpace,percTime,percPop, vthreshold;
  short evtype;

  CEventSet* arEv;

  vector<int> eventos;
  vector<double> espX, varX, padZi, somi;

  vector<int> n_eventos, n_eventos_espaco, n_eventos_tempo;
  vector<double> veros, centroX, centroY, raio, Tinicial, Tfinal;

//Carrega as variaveis numCluster, minPop, percSpace, percTime e percPop da interface
	numCluster = numClustersLineEdit->text().toInt();;
	minPop =     minPopLineEdit->text().toInt();;
	percSpace =  percSpaceLineEdit->text().toDouble();
	percTime =   percTimeLineEdit->text().toDouble();
	percPop =    percPopLineEdit->text().toDouble();


   if((2*numCluster)%2!= 0 || numCluster<=0)
      {
      QMessageBox::critical(this, tr("Error"), tr("The number of clusters must be a natural number bigger than 0"));
      return;
      }

  
   if(percSpace <=0)
      {
      QMessageBox::critical(this, tr("Error"), tr("Perc Space must be bigger than 0"));
      return;
      }

   if(percTime <=0)
      {
      QMessageBox::critical(this, tr("Error"), tr("Perc Time must be bigger than 0"));
      return;
      }

   if(percPop <=0)
      {
      QMessageBox::critical(this, tr("Error"), tr("Perc Pop must be bigger than 0"));
      return;
      }

  TeWaitCursor wait;
  

 //Obtém o número de permutações de tempo para calcular o p-valor
  long numSimul = 99;
  if (_100RadioButton->isOn() ==  true)
     numSimul = 999;

  //Obtém o tipo de análise que o usuário deseja (0 - prospectivo, 1 - identificador espaço/tempo)
  testType = 0;
  if (_identifierRadioButton->isOn() ==  true)
     testType = 1;


  /*long numEvt, NP_Tempo, NP_Esp, IterPvalor, SaidaObservada, Vsaida;
  int n_perm;
  double Esperanca, Pvalor;  */
  QString str, saida = "";
  vector<string> attrs;
  vector<string> objIds;


 vector<double> coordx, coordy, coordt;


 //----------------- pegar o tema, a tabela e os atributos selecionados na tela
	//Carrega o tema com o primeiro conjunto de pontos
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


// Aqui estão os comandos do ATK

    if(minPop <=0 || minPop > coordx.size())
      {
      QMessageBox::critical(this, tr("Error"), tr("Perc Space deve ser maior que 0 e menor que N"));
      return;
      }

    
   if(numCluster > coordx.size())
      {
      QMessageBox::critical(this, tr("Error"), tr("The number of clusters must be smaller than the number of observations"));
      return;
      }


    if (testType == 0)
       {
       evtype = EV_STPC;
       arEv = new CSortedAEventSet(coordx,coordy, coordt, evtype);
       }
    else if (testType == 1)
       {
       evtype = EV_STP;
       arEv = new CArrayEventSet(coordx,coordy, coordt, evtype);
       }


    CGridEvent gEv(arEv);

 
    CSTZone*            zoneObj;
    CScanZoneGenerator* zoneGen;
    CTestStatistics*   stTest;
    CProspectiveAnalysis* pAn = NULL;
    long maxPop;
    double spRadius;
    double tiRadius;

    maxPop = (percPop < 1.0) ? arEv->Size()*percPop : percPop;
    spRadius = (percSpace < 1.0) ? arEv->LinearSize()*percSpace : percSpace;
    tiRadius = (percTime < 1.0) ? arEv->TimeInterval()*percTime : percTime;


	saida += tr(" Output -   ATK    Results \n\n");


	// 1) Verificar se a tabela já existe
	string tableName = "ATK_Method_Prospective";
	if (localDb_->tableExist(tableName))
	{
		//mensagem de erro para escolher outro nome da tabela
		//ou pode apagar a tabela existente
		localDb_->deleteTable("ATK_Method_Prospective");
	}


    if (testType == 0)
        {
        zoneObj = new CSTZoneCov(maxPop, minPop);
        pAn = new CProspectiveAnalysis((CSortedAEventSet *)arEv, spRadius, tiRadius);
	    vthreshold = pAn->AlarmThreshold(0.1, arEv->Size()/5, 2, 200, 0.001);

        saida += tr("Threshold = ")+ str.setNum(vthreshold)+"\n";



	zoneGen = new CScanAlarmGenerator(&gEv, arEv, pAn, spRadius, tiRadius, minPop,
				zoneObj, eventos, espX, varX, padZi, somi);

       


    //********************** Como criar uma tabela externa e associá-la ao tema

	// 2) Criar o esquema da tabela em memória
	TeAttributeList attList;	
	
	TeAttribute at;
	at.rep_.name_= "Eventos";	
	at.rep_.type_ = TeINT;	
	at.rep_.isPrimaryKey_ = true;
	attList.push_back(at); 

	at.rep_.name_= "Esp";	
	at.rep_.type_ = TeREAL;	
	at.rep_.isPrimaryKey_ = false;
	attList.push_back(at); 

	at.rep_.name_= "Var";	
	at.rep_.type_ = TeREAL;	
	attList.push_back(at); 

	at.rep_.name_= "Zi";	
	at.rep_.type_ = TeREAL;	
	attList.push_back(at); 


		
	// 3) Criar a tabela no banco
	if (!localDb_->createTable(tableName, attList))
	{
		///mensagem de erro
	}		
		
	// 4) Inserir os dados em memória e inserir no banco (de 100 em 100 registros em memória vc chama o insertTable
	TeTable table(tableName, attList, "Eventos", "", TeAttrExternal);	
	table.relatedTableId(12);  //o identificador da tabela estática do tema que essa tabela externa irá se linkar
	table.relatedAttribute("tttt"); //o nome do atributo da tabela estática que essa tabela externa irá se linkar


	for(unsigned int i = 0; i < eventos.size(); i++)
	{
		TeTableRow row1;	
		row1.push_back(Te2String(eventos[i]));
		row1.push_back(Te2String(espX[i]));
		row1.push_back(Te2String(varX[i]));
		row1.push_back(Te2String(padZi[i]));
		table.add(row1);

		QString alarmMsg = tr("Alarm sounded in the event");
		
     //Verifica se alarme soou
		if (somi[i] > vthreshold) 
			saida += alarmMsg.latin1() + str.setNum(n_eventos[i])+"\n\n";
	     
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

  


	stTest = new CSTCovSum();

	}

    else if (testType == 1)
        {
        zoneObj = new CSTZoneCount(maxPop, minPop);
	zoneGen = new CScanSTIGenerator(&gEv, arEv, spRadius, tiRadius, minPop, zoneObj);

	stTest = new CSTInteractionScore(arEv->Size());

 	}


    CIdSTClusterRecalcTest testAlg(zoneGen,stTest);
    testAlg.DataLikelihood(numCluster, n_eventos, n_eventos_espaco, n_eventos_tempo, veros, centroX, centroY, raio, Tinicial, Tfinal);

	/*saida += "Veros         NEvts  NEvts_Esp    NEvts_T      X           Y          Raio    T_Ini     T_Fin \n\n";
    for(int i=0; i< veros.size(); i++)
      {
      saida += str.setNum(fabs(veros[i]))+ "\t\t" + str.setNum(n_eventos[i])+"\t" +str.setNum(n_eventos_espaco[i]) +"\t" +str.setNum(n_eventos_tempo[i])+"\t" +str.setNum(centroX[i]) +"\t" +str.setNum(centroY[i]) +"\t"+ str.setNum(raio[i]) +"\t" + str.setNum(Tinicial[i]) +"\t"+str.setNum(Tfinal[i]) + "\n\n";
      }*/


    //********************** Como criar uma tabela externa e associá-la ao tema

 	// 1) Verificar se a tabela já existe
	string tableName2 = "ATK_Method_FinalResult";
	if (localDb_->tableExist(tableName2))
	{
		//mensagem de erro para escolher outro nome da tabela
		//ou pode apagar a tabela existente
		localDb_->deleteTable("ATK_Method_FinalResult");
	}
	
	// 2) Criar o esquema da tabela em memória
	TeAttributeList attList;	
	
	TeAttribute at;
	at.rep_.name_= "Cluster";	
	at.rep_.type_ = TeINT;	
	at.rep_.isPrimaryKey_ = true;
	attList.push_back(at); 

	at.rep_.name_= "Verossimilhança";	
	at.rep_.type_ = TeREAL;	
	at.rep_.isPrimaryKey_ = false;
	attList.push_back(at); 

	
	at.rep_.name_= "Num Evts";	
	at.rep_.type_ = TeINT;	
	attList.push_back(at); 

	at.rep_.name_= "Num Evts Espaço";	
	at.rep_.type_ = TeINT;	
	attList.push_back(at); 

	at.rep_.name_= "Num Evts Tempo";	
	at.rep_.type_ = TeINT;	
	attList.push_back(at); 

	at.rep_.name_= "Centro X";	
	at.rep_.type_ = TeREAL;	
	attList.push_back(at); 

	at.rep_.name_= "Centro Y";	
	at.rep_.type_ = TeREAL;	
	attList.push_back(at); 

	at.rep_.name_= "Raio";	
	at.rep_.type_ = TeREAL;	
	attList.push_back(at);

	at.rep_.name_= "Tempo Inicial";	
	at.rep_.type_ = TeREAL;	
	attList.push_back(at); 

	at.rep_.name_= "Tempo Final";	
	at.rep_.type_ = TeREAL;	
	attList.push_back(at); 

		
	// 3) Criar a tabela no banco
	if (!localDb_->createTable(tableName2, attList))
	{
		///mensagem de erro
	}		
		
	// 4) Inserir os dados em memória e inserir no banco (de 100 em 100 registros em memória vc chama o insertTable
	TeTable table(tableName2, attList, "Cluster", "", TeAttrExternal);	
	table.relatedTableId(12);  //o identificador da tabela estática do tema que essa tabela externa irá se linkar
	table.relatedAttribute("ttttttttt"); //o nome do atributo da tabela estática que essa tabela externa irá se linkar


	for(unsigned int i = 0; i < veros.size(); i++)
	{
		TeTableRow row1;	
		row1.push_back(Te2String(i+1));
		row1.push_back(Te2String(fabs(veros[i])));
		row1.push_back(Te2String(n_eventos[i]));
		row1.push_back(Te2String(n_eventos_espaco[i]));
		row1.push_back(Te2String(n_eventos_tempo[i]));
		row1.push_back(Te2String(centroX[i]));
		row1.push_back(Te2String(centroY[i]));
		row1.push_back(Te2String(raio[i]));
		row1.push_back(Te2String(Tinicial[i]));
		row1.push_back(Te2String(Tfinal[i]));
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


    double pValue = testAlg.PValue(numSimul);

    saida += tr("P-Value= ") +str.setNum(pValue)+"\n\n";

	saida += tr("Other results are stored in external table /n");
	saida += tr("To see this external table go to 'Views/Themes' and click on the theme(that you have used) with the right button. Then click on 'Link External' Table \n\n");
	

    Output_text->setText(saida);

    mdelete((void **)&zoneObj);
    mdelete((void **)&zoneGen);
    mdelete((void **)&stTest);
    mdelete((void **)&pAn);
    mdelete((void **)&arEv);

      return ;
}


void ATKPluginWindow::table1ComboBoxClickSlot(const QString&)
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


//Será chamado toda vez que a form for chamada através do menu
void ATKPluginWindow::updateForm(PluginParameters* pluginParams)
{
	plugIngParams_ = (*pluginParams);

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

	//carregar os temas da vista corrente e colcoar o tema corrente como default
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

void 
ATKPluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}
