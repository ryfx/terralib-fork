//Bibliotecas

#include "funcaux.h"

//#include <conio.h> 
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

#include <KnoxPluginWindow.h>

#include <TeGUIUtils.h> 
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeDatabaseUtils.h>
#include <TeWaitCursor.h>
#include <TeSTEFunctionsDB.h>
#include <TeDataTypes.h>

#include <TeSTElementSet.h>
#include <TeAppTheme.h>
#include <TeDatabaseFactory.h>

#include <help.h>
/*extern "C"
{
 #include <windbase_edb.h>
}*/


KnoxPluginWindow::KnoxPluginWindow(QWidget* parent):
	knox(parent, "KnoxPluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	localDb_ = false;
	control_ = false;

	criticRadiusLineEdit->setValidator(new QDoubleValidator(this));
	criticTimeLineEdit->setValidator(new QDoubleValidator(this));
	
}


KnoxPluginWindow::~KnoxPluginWindow()
{
	if(localDb_)
		delete localDb_;
}


void KnoxPluginWindow::temaComboBoxClickSlot(const QString&)
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



void KnoxPluginWindow::enabletimecolumnSlot()
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

void KnoxPluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/
	Help help(this, "help", false);
	help.init("TerraClusterKnox.htm");

	if(!help.erro_)
		help.exec();

}

//Executa o programa quando o botão "OK" é clicado
void KnoxPluginWindow::okButtonClickSlot()
{
  using namespace std;
  long numEvt, NP_Tempo, NP_Esp, IterPvalor, SaidaObservada, Vsaida;
  int n_perm;
  double dist_crit, temp_crit, Esperanca, Pvalor;
  vector<SIDStatEvETPoint> evts;
  QString str, saida = "";
  vector<string> attrs;
  vector<string> objIds;

  Output_text->setText("");

//Carrega as variaveis dist_crit e temp_crit da interface
  dist_crit = criticRadiusLineEdit->text().toDouble();
  temp_crit = criticTimeLineEdit->text().toDouble();

  //Obtém o número de Simulações Monte Carlo que o usuário deseja

  long NumSim = 99;
  if (_999RadioButton->isOn() ==  true)
     NumSim = 999;


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

 //if(selectimeCheckBox->isChecked())
 //   {

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

 /*   }

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
*/
   numEvt = coordx.size();
   n_perm = NumSim;
   evts.resize(numEvt);

	for(int i=0; i<numEvt; i++)
    {
		evts[i].ePos.x = coordx[i];
		evts[i].ePos.y = coordy[i];
		evts[i].eTime = coordt[i];
    }

    vector<bool> VizEspaco(numEvt*numEvt);  // Aloca vetor booleano de Vizinhos no espaco
    vector<bool> VizTempo(numEvt*numEvt);   // Aloca vetor booleano de Vizinhos no tempo
    vector<long> Perm(numEvt);              // Aloca vetor de permutações


         // Inicia Vetor Permutação

     for(int i=0; i<numEvt; i++)
       Perm[i] = i;     

     for(int i=0; i<numEvt; i++)
	   for(int j=0; j<numEvt; j++)         // Observação: i é vizinho de i
         {
		 //Calcula distancia de x,y de i e de j e compara com limiar
			   VizEspaco[i*numEvt+j] = (Dist(evts[i].ePos.x,evts[i].ePos.y,evts[j].ePos.x,evts[j].ePos.y) < dist_crit);
			   VizTempo[i*numEvt+j] = (fabs(evts[i].eTime-evts[j].eTime) < temp_crit);

         }
             
  NP_Esp = ContViz(VizEspaco,numEvt);
  NP_Tempo = ContViz(VizTempo,numEvt);
      Esperanca = 2*(float)NP_Esp*(float)NP_Tempo/(numEvt*(numEvt-1));
      SaidaObservada = DefineNumeroVizinhos(numEvt, Perm, VizEspaco, VizTempo);
       
      IterPvalor=0; 

	  TeProgress::instance()->reset();
	  TeProgress::instance()->setTotalSteps(n_perm);
	  TeProgress::instance()->setCaption(tr("Knox's Method").latin1());
	  TeProgress::instance()->setMessage(tr("Executing...").latin1());

      for(int i=0; i<n_perm; i++)        //Cria Permutacao
      {
         DefinePermutacaoAleatoria(numEvt, Perm);
         //Avalia vizinhos
         Vsaida = DefineNumeroVizinhos(numEvt, Perm, VizEspaco, VizTempo);
         if(Vsaida >= SaidaObservada)
           IterPvalor++;

		 TeProgress::instance()->setProgress(i);
		 if( TeProgress::instance()->wasCancelled())
		 {
			 Output_text->setText("Cancelled by the user...");
			 TeProgress::instance()->reset();
			return;
		 }
			 
      }

	  TeProgress::instance()->reset();


      Pvalor =((double)(IterPvalor+1)/(double)(n_perm+1));

      saida = tr("Knox's Method \n \n Num of Observations: ") + str.setNum(numEvt);
      saida += tr("\n Num of Space-Time Neibohrs (exp): ") + str.setNum(Esperanca);
      saida += tr("\n Num of Space-Time Neibohrs (obs): ") + str.setNum(SaidaObservada);
      saida += tr("\n P-Value: ") + str.setNum(Pvalor);
	  saida += tr("\n NP-Esp: ") + str.setNum(NP_Esp);
	  saida += tr("\n NP-Temp: ") + str.setNum(NP_Tempo);

      Output_text->setText(saida);

      return ;
}


void KnoxPluginWindow::table1ComboBoxClickSlot(const QString&)
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
void KnoxPluginWindow::updateForm(PluginParameters* pluginParams)
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
KnoxPluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}
