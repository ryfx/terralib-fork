#include <valarray>
#include <vector>

#include <MantelPluginWindow.h>
#include <TeGUIUtils.h> 
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeDatabaseUtils.h>
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
#include <qlineedit.h>
#include <qstring.h>
#include <qcombobox.h>
#include <qradiobutton.h>

#include "Mantel.h"
#include <ranker.h>

#include <TeAppTheme.h>
#include <TeSTElementSet.h>
#include <TeDatabaseFactory.h>
#include <TeDatabaseFactoryParams.h>
#include <qvalidator.h>

///Metodo de inicializacao da interface. Inicializa as variaveis de interface.
MantelPluginWindow::MantelPluginWindow(QWidget* parent):
	mantel(parent, "MantelPluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	power1lineEdit->setValidator(new QDoubleValidator(this));
	power2lineEdit->setValidator(new QDoubleValidator(this));
	shift1lineEdit->setValidator(new QDoubleValidator(this));
	shift2lineEdit->setValidator(new QDoubleValidator(this));

	localDb_ = false;
	control_ = false;
}

///Destrutor da classe, deleta a conexao com o Banco de dados ao sair da interface
MantelPluginWindow::~MantelPluginWindow()
{
	if(localDb_)
		delete localDb_;
}

///Metodo responsavel pelo preenchimento do combo de tabelas com as tabelas pertencentes
///ao tema especificado
void MantelPluginWindow::temaComboBoxClickSlot(const QString&)
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

void MantelPluginWindow::EnableSpaceSlot()
{
	if(spaceRadioButton->isOn()) {
		power1lineEdit->setEnabled(true);
		shift1lineEdit->setEnabled(true);
	}
	else {
		power1lineEdit->setEnabled(false);
		shift1lineEdit->setEnabled(false);
	}

}

void MantelPluginWindow::EnableTimeSlot()
{
	if(timeRadioButton->isOn()) {
		power2lineEdit->setEnabled(true);
		shift2lineEdit->setEnabled(true);
	}
	else {
		power2lineEdit->setEnabled(false);
		shift2lineEdit->setEnabled(false);
	}

}

///Responsavel pelo preenchimento do combo de tabelas  pertencentes ao tema especificado.
void MantelPluginWindow::enabletimecolumnSlot()
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


///Metodo principal da interface. Execucao do algoritmo onde e executado o metodo de Mantel,  
///responsavel por carregar todas as variaveis da interface e do banco de dados.
void MantelPluginWindow::RunButtonClickSlot()
{
    using namespace std;
	
	// Parametros para simulação
	int montecarlo;								// Iteraçoes de Monte Carlo
	QString str, saida = "";					// String auxiliar e de saida
	std::vector<double> coordt;					// Vetor de tempos
	std::vector<double> coordx;					// Coordenadas x
	std::vector<double> coordy;					// Coordenadas y
    double shift_S, power_S, shift_T, power_T;
	bool transform1, transform2;
	vector<string> attrs;
    std::vector<string> objIds;
	Output_text->clear();


	TeWaitCursor wait;
	//Carrega as variavies da interface
	transform1 = spaceRadioButton->isOn();
	transform2 = timeRadioButton->isOn();
	power_S = power1lineEdit->text().toDouble();
	shift_S = shift1lineEdit->text().toDouble();
	power_T = power2lineEdit->text().toDouble();
	shift_T = shift2lineEdit->text().toDouble();


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
	TeView *view = localDb_->viewMap()[ViewId_];
	//int themeId = (view->get(string(themeComboBox->currentText().ascii())))->id();
	//TeTheme* theme = localDb_->themeMap()[themeId]; 
	TeTheme* theme = plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1()); 
	if(!theme)
	{
		QMessageBox::critical(this, tr("Error"), tr("Not possible to load the Theme"));
		return;
	}

 //TeInitQuerierStrategies();
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


	//----------------- Teste de Mantel para Dados Transformados ------------------------------------//
	if( transform1 || transform2 )	// Dados transformados
	{
		Output_text->setText(tr(" Mantel Test (transformed data) \n running..."));
	
        int i, size = coordx.size();	    
	    double pvalue, Zobs;
		std::valarray<double> ZZ(montecarlo+1);
        vector< vector <double> > geo(size);

	    // Redimensiona o vetor geo 
	    for (i=0; i<size; i++){
		     geo[i].resize(2, 0.0);
	    }

	    for (i=0; i<size; i++){
		     geo[i][0]=coordx[i];
		     geo[i][1]=coordy[i];
	    }

	    if( transform1 == FALSE )	
		  {
	       power_S = 1;
	       shift_S = 0;
		  }
		else if ( transform2 == FALSE )
		  {
	       power_T = 1;
	       shift_T = 0;
		  }

		// Executa o Teste de Mantel com dados transformados
		if( Mantel_transform(geo, coordt, montecarlo, shift_S, power_S, shift_T, power_T, ZZ, pvalue, Zobs) )
		{
			QMessageBox::critical(this, tr("Error"), tr("Memory Error"));
			return;
		}

        vector<double> ZZZ(montecarlo+1);
        for (i=0; i<=montecarlo; i++) 
             ZZZ[i] = ZZ[i]; 

		// Imprime os resultados na interface
		saida = tr("Mantel Test (transformed data) \n \n Number of cases analized: ") + str.setNum( size );
		saida += tr("\n Transformations that you specified: \n (S + %f)^%f") + str.setNum(shift_S) + str.setNum(power_S);
		saida += "\n (T + %f)^%f" + str.setNum(shift_T) + str.setNum(power_T);
		saida += tr("\n Number of Monte Carlo iterations: ") + str.setNum( montecarlo );
		saida += tr("\n test statistic Z: ") + str.setNum( Zobs );
		saida += tr("\n pvalue: ") + str.setNum( pvalue );
		saida += tr("\n \n Some percentiles for Z distribution: \n \n percentile 0.5%: ") + str.setNum( quantile(ZZZ, 0.005) );
		saida += tr("\n percentile 2.5%: ") + str.setNum( quantile(ZZZ, 0.025) );
		saida += tr("\n percentile 5%: ") + str.setNum( quantile(ZZZ, 0.05) );
		saida += tr("\n percentile 10%: ") + str.setNum( quantile(ZZZ, 0.1) );
		saida += tr("\n percentile 90%: ") + str.setNum( quantile(ZZZ, 0.9) );
		saida += tr("\n percentile 95%: ") + str.setNum( quantile(ZZZ, 0.95) );
		saida += tr("\n percentile 97.5%: ") + str.setNum( quantile(ZZZ, 0.975) );
		saida += tr("\n percentile 99.5%: ") + str.setNum( quantile(ZZZ, 0.995) );
	   
		Output_text->setText(saida);
	}
/* - - - - - - - - - - - - - - Teste de Mantel para Dados Originais - - - - - - - - - - - - - - - - - - - - - - - */
	else	  // Dados originais
	{
		Output_text->setText(tr(" Mantel Test \n running..."));
	
        int i, size = coordx.size();	    
	    double pvalue, Zobs;
		std::valarray<double> ZZ(montecarlo+1);
        vector< vector <double> > geo(size);

	    // Redimensiona o vetor geo 
	    for (i=0; i<size; i++){
		     geo[i].resize(2, 0.0);
	    }

	    for (i=0; i<size; i++){
		     geo[i][0]=coordx[i];
		     geo[i][1]=coordy[i];
	    }

		// Executa o Teste de Mantel com dados originais
		if( Mantel(geo, coordt, montecarlo, ZZ, pvalue, Zobs) )
		{
			QMessageBox::critical(this, tr("Error"), tr("Memory Error"));
			return;
		}

        vector<double> ZZZ(montecarlo+1);
        for (i=0; i<=montecarlo; i++) 
             ZZZ[i] = ZZ[i]; 


		// Imprime os resultados na interface
		saida = tr("Mantel Test (original data) \n \n Number of cases analized: ") + str.setNum( size );
		saida += tr("\n Number of Monte Carlo iterations: ") + str.setNum( montecarlo );
		saida += tr("\n test statistic Z: ") + str.setNum( Zobs );
		saida += tr("\n pvalue: ") + str.setNum( pvalue );
		saida += tr("\n \n Some percentiles for Z distribution: \n \n percentile 0.5%: ") + str.setNum( quantile(ZZZ, 0.005) );
		saida += tr("\n percentile 2.5%: ") + str.setNum( quantile(ZZZ, 0.025) );
		saida += tr("\n percentile 5%: ") + str.setNum( quantile(ZZZ, 0.05) );
		saida += tr("\n percentile 10%: ") + str.setNum( quantile(ZZZ, 0.1) );
		saida += tr("\n percentile 90%: ") + str.setNum( quantile(ZZZ, 0.9) );
		saida += tr("\n percentile 95%: ") + str.setNum( quantile(ZZZ, 0.95) );
		saida += tr("\n percentile 97.5%: ") + str.setNum( quantile(ZZZ, 0.975) );
		saida += tr("\n percentile 99.5%: ") + str.setNum( quantile(ZZZ, 0.995) );

		Output_text->setText(saida);
	}

	return;
}


///Metodo responsavel pelo preenchimento do combo de atributos das tabelas pertencentes
///a tabela especificado
void MantelPluginWindow::tableComboBoxClickSlot(const QString&)
{
	//selecionar o tema selecionado na combo
	string aux = string(themeComboBox->currentText().ascii());
	//Carrega a vista pelo Id carregado inicialmente
	TeView* view = localDb_->viewMap()[ViewId_];
	TeTheme* theme = plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1()); 
	//TeTheme* themeaux = view->get(aux);
	//int themeId = themeaux->id();
	//TeTheme* theme = localDb_->themeMap()[themeId]; 
	
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
///Slot para fechar a interface
void MantelPluginWindow::CloseSlot()
{
	//if(localDb_)
	//	delete localDb_;
	QWidget::close();
}
///Ao ser inicializada a interface pelo menu do terraview, esse metodo e responsavel
///pela conexao do plugin com banco de dados e devidas inicializacoes que devam ser feitas.
void MantelPluginWindow::updateForm(PluginParameters* pluginParams)
{
	plugIngParams_ = (*pluginParams);
	
	//Inicializa a interface
	Output_text->clear();

	if (plugIngParams_.getCurrentDatabasePtr() == NULL)
	{
		QMessageBox::warning(this, tr("Error"), tr("Data Base not selected"));
		exit(0);

	}
	//Fixa qual vista é carregada inicialmente na abertura do plugin
	if(!control_){
		if (plugIngParams_.getCurrentViewPtr() == NULL)
	    { 
			QMessageBox::warning(this, tr("Error"), tr("View not selected"));
			exit(0);
     	}
		ViewId_ = plugIngParams_.getCurrentViewPtr()->id();
		control_ = false;
	}
    if (plugIngParams_.getCurrentThemeAppPtr() == NULL)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Theme not selected"));
		exit(0);
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

	//Vetor de representações
	std::vector<TeGeomRep> reps;
	reps.push_back(TePOLYGONS);
	reps.push_back(TePOINTS);

	//carregar os temas da vista corrente e colcoar o tema corrente como default
	//if(!fillThemeCombo(TePOLYGONS,localDb_->viewMap()[ViewId_], 
	//	themeComboBox, pluginParams->getCurrentThemeAppPtr()->getTheme()->name()))
	if(!fillThemeCombo(reps,localDb_->viewMap()[ViewId_], 
		themeComboBox, pluginParams->getCurrentThemeAppPtr()->getTheme()->name()))
	{
		//error message
		return;
	}                

	//carregar todos os atributos da tabela do tableComboBox
	temaComboBoxClickSlot(QString(""));
	return;
}

///Metodo responsavel por esconder a tela de interface do plugin
void MantelPluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}
