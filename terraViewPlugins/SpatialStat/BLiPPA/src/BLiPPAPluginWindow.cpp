//Bibliotecas

#include <math.h>

#include <list>
#include <valarray>
#include <qfiledialog.h>
#include <qapplication.h>
#include <qvalidator.h>
#include <LoadParams.h>
#include <algoritmos.h>
#include <sv.h>
#include <windows.h>
//#include <BLiPPAForm.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>


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

#include <BLiPPAPluginWindow.h>
#include <TeGUIUtils.h> 
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeDatabaseUtils.h>
#include <TeWaitCursor.h>
#include <TeSTEFunctionsDB.h>
#include <TeDataTypes.h>
#include <TeDatabaseFactoryParams.h>
#include <TeDatabaseFactory.h>
#include <TeAppTheme.h>

#include <help.h>


///Metodo de inicializacao da interface. Inicializa as variaveis da interface.
BLiPPAPluginWindow::BLiPPAPluginWindow(QWidget* parent):
	BLiPPA(parent, "BLiPPAPluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	OrigBandLineEdit->setValidator(new QDoubleValidator(this));
	GridLineEdit->setValidator(new QDoubleValidator(this));
	DestBandLineEdit->setValidator(new QDoubleValidator(this));
	RateLineEdit->setValidator(new QDoubleValidator(this));

	localDb_ = false;
	control_ = false;
	ncols_ = 0;
	init();
	
	

}


///Destrutor da classe, deleta a conexao com o banco de dados ao sair da interface.
BLiPPAPluginWindow::~BLiPPAPluginWindow()
{
	if(localDb_)
		delete localDb_;
}

/*Method: Validate the entry datas of the interface*/
void BLiPPAPluginWindow::init()
{
 QDoubleValidator *v = new QDoubleValidator(this);
 v->setBottom(0.0);
 
 QIntValidator *vi = new QIntValidator(this);
 vi->setBottom(1);
 
 DestBandLineEdit->setValidator(v);
// OrigBandLineEdit->setValidator(v);
 GridLineEdit->setValidator(vi);

}

void BLiPPAPluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/
	Help help(this, "help", false);
	help.init("TerraClusterBLiPPAP.htm");

	if(!help.erro_)
		help.exec();

}

/*Method: Run the system */
void BLiPPAPluginWindow::okButtonClickSlot()
{
	Output_text->setText("BLiPPA...");
	
	TeProgress::instance()->reset();
	TeProgress::instance()->setTotalSteps(3);
	TeProgress::instance()->setCaption("BLiPPA");
	TeProgress::instance()->setMessage(tr("Initializing the function...").latin1());
	TeProgress::instance()->setProgress(1);

  QString saida = "", str;
  TeWaitCursor wait;
  int control, sim, ngrid;
  double h1, h2, Obs, Exp, pvalue, Var, rate;
  std::vector<double> xp, yp, x1, y1, x2, y2, coordx1, coordx2, coordy1, coordy2;
  std::vector<int> i1, i2;
  std::vector<string> objIds1, objIds2;
 
  
  //Set the input data of the interface in objects
  h2 = DestBandLineEdit->text().toDouble();
  h1 = OrigBandLineEdit->text().toDouble();
  ngrid = GridLineEdit->text().toInt();
  rate = RateLineEdit->text().toDouble();

	
  //----------------- pegar o tema, a tabela e os atributos selecionados na tela
	TeView *view = localDb_->viewMap()[ViewId_];
	TeTheme* theme1 =  plugIngParams_.getCurrentViewPtr()->get(themeComboBox1->currentText().latin1());;
//	int themeId1 = (view->get(string(themeComboBox1->currentText().ascii())))->id();
	//Carrega o tema com o primeiro conjunto de pontos
//	TeTheme* theme1 = localDb_->themeMap()[themeId1]; 

	TeTheme* theme2 =  plugIngParams_.getCurrentViewPtr()->get(themeComboBox2->currentText().latin1());;
//	int themeId2 = (view->get(string(themeComboBox2->currentText().ascii())))->id();
	//Carrega o tema com o segundo conjunto de pontos
//	TeTheme* theme2 = localDb_->themeMap()[themeId2];

	TeTheme* theme3 =  plugIngParams_.getCurrentViewPtr()->get(themeComboBox3->currentText().latin1());;
//	int themeId3 = (view->get(string(themeComboBox3->currentText().ascii())))->id();
	//Carrega o tema com o segundo conjunto de pontos
//	TeTheme* theme3 = localDb_->themeMap()[themeId3];

	if(!theme1 || !theme2 || !theme3)
	{
		QMessageBox::critical(this, tr("Error"), tr("Not possible to load themes"));
		wait.resetWaitCursor();
		TeProgress::instance()->reset();
		return;
	}


	LoadParams(theme1, coordx1, coordy1, objIds1);
	LoadParams(theme2, coordx2, coordy2, objIds2);
	
	TeProgress::instance()->setTotalSteps(2);
	if(TeProgress::instance()->wasCancelled())
	{
		Output_text->setText("Canceled by the user...");
		wait.resetWaitCursor();
		TeProgress::instance()->reset();
		return;	
	}
	
	for(unsigned int i = 0; i < objIds1.size(); i++)
		for(unsigned int j = 0; j < objIds2.size(); j++)
			if(objIds1[i] == objIds2[j])
			{
				x1.push_back(coordx1[i]);
				y1.push_back(coordy1[i]);				
				x2.push_back(coordx2[j]);				
				y2.push_back(coordy2[j]);
				break;
			}

	TeQuerierParams params;
	params.setFillParams(true,false);

	params.setParams(theme3);
	
	TeQuerier querier(params);
	if(!querier.loadInstances())
	{
		//QMessageBox::critical(this, tr("Error"), tr("Not possible to load data"));
		wait.resetWaitCursor();
		TeProgress::instance()->reset();
		return;
	}

	

	TeSTInstance obj;
	querier.fetchInstance(obj);

    //Get geometries
    if(!obj.hasPolygons())
    {
		QMessageBox::critical(this, tr("Error"), tr("Not valid polygon theme"));
		wait.resetWaitCursor();
		TeProgress::instance()->reset();
		return;
	}
	else
	{
		TePolygonSet polSet;
        obj.getGeometry(polSet);

		if(polSet.size() > 1 || polSet[0].size() > 1)
		{
			QMessageBox::critical(this, tr("Error"), tr("Invalid polygon geometry"));
			wait.resetWaitCursor();
			TeProgress::instance()->reset();
			return;
		}
		else
		{
				for(unsigned int k = 0; k < polSet[0][0].size(); k++)
				{
					xp.push_back(polSet[0][0][k].x());
					yp.push_back(polSet[0][0][k].y());
				}
		}
	}

	TeBox box = theme3->getThemeBox();

  if( h1*h2*ngrid*rate == 0.0)
  {
    wait.resetWaitCursor();
    QMessageBox::critical(this, tr("Error"), tr("Unknown parameter"));
	TeProgress::instance()->reset();
    return;
  }

  	//Seleciona o numero de simulacoes para envelope
	switch(this->MonteCarloTestButtonGroup->selectedId())	// Seleciona o numero de simulações de Monte Carlo
	{
		case 0:
			sim = -1;
			break;
		case 1:
			sim = 0;
			break;
		case 2:
			sim = 99;
			break;
		case 3:
			sim = 999;
			break;
	}

	bool MaxRegion = Max_Leg_checkBox->isChecked();

	TeProgress::instance()->setProgress(3);

	if(TeProgress::instance()->wasCancelled())
	{
		Output_text->setText("Canceled by the user...");
		wait.resetWaitCursor();
		TeProgress::instance()->reset();
		return;	
	}

	TeProgress::instance()->reset();
  
  //Search for expection erros during the function

  if(sim >= 0)
  {
	  bool wasCancelled = false;
	  ScoreStat(x1, y1, x2, y2, h1, h2, Obs, Exp, Var, i1, i2,	wasCancelled);
		
	if(wasCancelled)
	{
		Output_text->setText("Canceled by the user...");
		wait.resetWaitCursor();
		TeProgress::instance()->reset();
		return;
	}

				// Imprime os resultados na interface
				saida = tr("Origin-Destination Score Test \n \n");
				saida += tr("Observed Value for Score Test Statistic: ") + str.setNum( Obs );
				saida += tr("\nExpected Value for Score Test Statistic: ") + str.setNum( Exp );
				saida += tr("\nBarton-David-based approximation variance: ") + str.setNum( Var );
				saida += tr("\nBarton-David-based approximation P-Value: ") + str.setNum( 1.0 - AcNormal((Obs-Exp)/sqrt(Var)) );

		if(sim > 0)
		{
			bool wasCancelled = false;
			ScoreTest(i1, i2, x1.size(), sim, Obs, pvalue, wasCancelled);

			if(wasCancelled)
			{
				Output_text->setText("Canceled by the user...");
				wait.resetWaitCursor();
				TeProgress::instance()->reset();
				return;
			}

			saida += tr("\nMonte Carlo Test P-Value: ") + str.setNum( pvalue );
		}

		Output_text->setText(saida);
  }
 control = ConditionalKernel((int) ngrid, (int) ngrid, h1, h2, xp, box.x1(), box.x2(),
							yp, box.y1(), box.y2(), x2, y2, x1, y1, rate, MaxRegion);

}



//sera chamado toda vez que a form for chamada através do menu
void 
BLiPPAPluginWindow::updateForm(PluginParameters* pluginParams)
{
	plugIngParams_ = (*pluginParams);
	init();

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

	//carregar os temas da vista corrente e colcoar o tema corrente como default
	if(!fillThemeCombo(TePOINTS, localDb_->viewMap()[ViewId_], 
		themeComboBox1, pluginParams->getCurrentThemeAppPtr()->getTheme()->name()))
	{
		//error message
		return;
	}       

	if(!fillThemeCombo(TePOINTS, localDb_->viewMap()[ViewId_], 
		themeComboBox2, pluginParams->getCurrentThemeAppPtr()->getTheme()->name()))
	{
		//error message
		return;
	}

	if(!fillThemeCombo(TePOLYGONS, localDb_->viewMap()[ViewId_], 
		themeComboBox3, pluginParams->getCurrentThemeAppPtr()->getTheme()->name()))
	{
		//error message
		return;
	} 


	//carregar todos os atributos da tabela do tableComboBox
	//tema1ComboBoxClickSlot(QString(""));
	return;
}
void 
BLiPPAPluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}
