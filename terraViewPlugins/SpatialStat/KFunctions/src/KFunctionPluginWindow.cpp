
///Inclusoes necessarias no Plugin
#include <curve.h>

#include <KFunctionPluginWindow.h>
#include <envelope.h>

#include <math.h>
//#include <valarray>
#include <vector>

#include <LoadParams.h>
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
#include <qfiledialog.h>
#include <qapplication.h>

#include <TeGUIUtils.h> 
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeDatabaseUtils.h>
#include <TeWaitCursor.h>
#include <TeSTEFunctionsDB.h>
#include <TeDataTypes.h>

#include <TeSTElementSet.h>
#include <TeDatabaseFactory.h>
#include <TeDatabaseFactoryParams.h>
#include <TeAppTheme.h>

#include <help.h>

using namespace std;

///Metodo de inicializacao da interface. Inicializa as variaveis e chama a funcao
///init() para inicializar os campos da interface
KFunctionPluginWindow::KFunctionPluginWindow(QWidget* parent):
	kfunction(parent, "KFunctionPluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	localDb_ = 0;
	control_ = false;
	gra_ = 0;
	ncols_ = 0;

	maxRadiusLineEdit->setValidator(new QDoubleValidator(this));
	numRadiusLineEdit->setValidator(new QDoubleValidator(this));

	init();
}


///Destrutor da classe, deleta a conexao com o Banco de dados ao sair da interface
KFunctionPluginWindow::~KFunctionPluginWindow()
{
	if(localDb_)
		delete localDb_;
}

///Valida as entradas da interface
void KFunctionPluginWindow::init()
{
 ///Cria um validadores para os campos da interface de double e inteiros
 QDoubleValidator *v = new QDoubleValidator(this);
 QIntValidator *vi = new QIntValidator(this);
 ///Atribui valores minimos para os validadores
 vi->setBottom(1);
 v->setBottom(0.0);

 ///Inicializa os campos com os validadores criados 
 numRadiusLineEdit->setValidator(vi);
 maxRadiusLineEdit->setValidator(v);
 
}

///Metodo para abilitar/desabilitar o combo do primeiro tema de acordo com a escolha do 
///usuario.
void KFunctionPluginWindow::Enable_Theme1Slot()
{
	//Desabilita as funcoes relativas a funcao k bivariada
	IndepenRadioButton->setChecked(true);
	LabelRadioButton->setChecked(true);
	themeComboBox2->setEnabled(false);

	//Inicializa o combo com as funcoes k existentes
	InitializefunctionComboBox();
}

///Metodo para abilitar/desabilitar o combo do segundo tema de acordo com a escolha do 
///usuario.
void KFunctionPluginWindow::Enable_Theme2Slot()
{
	    //Habilita o tema relativo a funcao k bivariada
		themeComboBox2->setEnabled(true);

		//carregar os temas da vista corrente e colocar o tema corrente como default
		if(!fillThemeCombo(TePOINTS, localDb_->viewMap()[ViewId_], 
			themeComboBox2, plugIngParams_.getCurrentThemeAppPtr()->getTheme()->name()))
		{
			//error message
			return;
		}        

		IndepenRadioButton->setChecked(true);
		LabelRadioButton->setChecked(true);
		//Inicializa o combo com as funcoes k bivariada existentes
		InitializefunctionComboBox();
}

///Preenche o functionComboBox com as funcoes K disponiveis para o metodo desejado
void KFunctionPluginWindow::InitializefunctionComboBox()
{
	int i, tam;
	vector<QString> combo;
	//Inicializa o vetor funcoes que se aplicam a todas as funcoes k
    combo.push_back("K-Stoyan");

	//Inicializa o combo com as funcoes K
	tam = functionComboBox->count();
	for(i=0;i<tam;++i)
		functionComboBox->removeItem(0);
	
	//Testa se o botao univariado esta selecionado
	if(univariateRadioButton->isOn())
	{
		//Insere as funcoes relativas a funcao k 
		combo.push_back("K-Ripley");
		for(i=0;i<(int)combo.size();i++)
			functionComboBox->insertItem(combo[i],i);
		functionComboBox->setCurrentItem(1);
	}

	else  // Insere as funcoes relativas a funcao k bivariada
	{
		//Insere as funcoes relativas a funcao k12
		combo.push_back("K-Ripley");
		for(i=0;i<(int)combo.size();i++)
			functionComboBox->insertItem(combo[i],i);
	   	functionComboBox->setCurrentItem(1);
    }
}

void KFunctionPluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/
	Help help(this, "help", false);
	help.init("TerraClusterKFunction.htm");

	if(!help.erro_)
		help.exec();

}

///Metodo principal da interface. Execucao do algoritmo onde sao executadas as funcoes K e
///carregadas todas as variaveis da interface e do banco de dados.
void KFunctionPluginWindow::kfunctionButtonClickSlot()
{
	// Parametros para simulação
	unsigned int i;
	int sim, funcId;
	bool uni, ind, lab, L; 
	QString str, saida = "";			
	std::valarray<double> khat,upper,lower;
	std::vector<double> coordx1,coordx2;			// Coordenadas x
	std::vector<double> coordy1,coordy2;			// Coordenadas y
	std::vector<string> objIds;
	std::vector<double> h;
	double raioMax, numRaio;

	TeWaitCursor wait;
	//Carrega as variavies da interface
	L = LFunctionCheckBox->isChecked();
	uni = univariateRadioButton->isOn();
	ind = IndepenRadioButton->isOn();
	lab = LabelRadioButton->isOn();
	raioMax = maxRadiusLineEdit->text().toDouble();
	numRaio = numRadiusLineEdit->text().toDouble();
	funcId = functionComboBox->currentItem();

	
  if( raioMax*numRaio == 0.0)
  {
    wait.resetWaitCursor();
    QMessageBox::critical(this, tr("Error"), tr("Unknown parameter"));
    return;
  }

	//create the vector of possible colors for the graphic
	if( _colors.empty() ) {
	_colors.push_back(Qt::black);
	_colors.push_back(Qt::red);
	}
	gra_= 0;

    //Cria o vetor de distancias
	double varR = raioMax/numRaio;
	h.resize(numRaio+1, 0.0);
	for(i=1; i<numRaio+1; i++)
		h[i] = i*varR;


	//cria os vetores de envelope e da funcao K
	khat.resize(numRaio+1, 0.0);
	upper.resize(numRaio+1, 0.0);
	lower.resize(numRaio+1, 0.0);


	//Seleciona o numero de simulacoes para envelope
	switch(this->numSimulationsButtonGroup->selectedId())	// Seleciona o numero de simulações de Monte Carlo
	{
		case 0:
			sim = 99;
			break;
		case 1:
			sim = 999;
			break;
	}
	

	//------------------pegar o tema, a tabela e os atributos selecionados na tela

	//Pega a vista selecionada no Terraview
	TeTheme* theme1 = plugIngParams_.getCurrentViewPtr()->get(themeComboBox1->currentText().latin1());

	if(!theme1)
	{
		QMessageBox::critical(this, tr("Error"), tr("Not possible to load the Theme"));
		return;
	}

	//Se a selecao for bivariada
	if(ind)
	{
		    //Pega o id do segundo tema necessario para utilizacao da funcao K bivariada
			TeTheme* theme2 = plugIngParams_.getCurrentViewPtr()->get(themeComboBox2->currentText().latin1());

			//Carrega do banco de dados os parametros para memoria
			LoadParams(theme1,coordx1,coordy1);
			LoadParams(theme2,coordx2,coordy2);

			//Encontra o box do conjunto de pontos
			TeBox box = BoxBiVariate(theme1,theme2);

			//Escolhe qual funcao K disponivel sera utilizada de acordo com a escolha do usuario
			switch(funcId)
			{
				case 0:
					{
						KStoyan12(box,coordx1.size(),coordx2.size(),numRaio,raioMax,coordx1,coordy1,coordx2,coordy2,h,khat);
						envelope12(box,sim,coordx1.size(),coordx2.size(),numRaio,raioMax,coordx1,coordy1,coordx2,coordy2,h,funcId,upper,lower);
						break;
					}
				case 1:
					{
						KRipley12(box,coordx1.size(),coordx2.size(),numRaio,raioMax,coordx1,coordy1,coordx2,coordy2,h,khat);
						envelope12(box,sim,coordx1.size(),coordx2.size(),numRaio,raioMax,coordx1,coordy1,coordx2,coordy2,h,funcId,upper,lower);
						break;
					}
			}
	}
	else
	{
		//Se a selecao for univariada
		if(uni)
		{
				//Carrega do banco de dados os parametros para memoria
				LoadParams(theme1,coordx1,coordy1);

				//Encontra o box do conjunto de pontos
				TeBox box = theme1->getThemeBox();

				//Escolhe qual funcao K disponivel sera utilizada de acordo com a escolha do usuario
				switch(funcId)
				{
					case 0:
						{
							KStoyan(box,coordx1.size(),numRaio,raioMax,coordx1,coordy1,h,khat);
							envelope(box,sim,coordx1.size(),numRaio,raioMax,funcId,h,upper,lower);
							break;
						}
					case 1:
						{
							KRipley(box,coordx1.size(),numRaio,raioMax,coordx1,coordy1,h,khat);
							envelope(box,sim,coordx1.size(),numRaio,raioMax,funcId,h,upper,lower);
							break;
						}
				}
		}

		// Se a seleção for de rotulação aleatória
		else
		{
				//Pega o id do segundo tema necessario para utilizacao da funcao K bivariada
				TeTheme* theme2 = plugIngParams_.getCurrentViewPtr()->get(themeComboBox2->currentText().latin1());

				//Carrega do banco de dados os parametros para memoria
				LoadParams(theme1,coordx1,coordy1);
				LoadParams(theme2,coordx2,coordy2);

				//Encontra o box do conjunto de pontos
				TeBox box = BoxBiVariate(theme1,theme2);

				//Escolhe qual funcao K disponivel sera utilizada de acordo com a escolha do usuario
				switch(funcId)
				{
					case 0:
						{
							Klabel_Stoyan(box, coordx1.size(), coordx2.size(), numRaio, raioMax, coordx1, coordy1, 
										  coordx2, coordy2, h, khat);
							EnvLabel_Stoyan(box, sim, coordx1.size(), coordx2.size(), numRaio, raioMax, coordx1, coordy1, 
											coordx2, coordy2, h, upper, lower);
							break;
						}
					case 1:
						{
							Klabel_Ripley(box, coordx1.size(), coordx2.size(), numRaio, raioMax, coordx1, coordy1, 
										  coordx2, coordy2, h, khat);
							EnvLabel_Ripley(box, sim, coordx1.size(), coordx2.size(), numRaio, raioMax, coordx1, coordy1, 
											coordx2, coordy2, h, upper, lower);
							break;
						}
				}

		}
	}
    
	//Caso esteja habilitado na interface faz a conversao dos resultados para funcao L
	if(L)
	{
		for(i=0;i<khat.size();i++)
		{
			khat[i] = sqrt(khat[i]/TePI) - h[i];
			lower[i] = sqrt(lower[i]/TePI) - h[i];
			upper[i] = sqrt(upper[i]/TePI) - h[i];
		}
	}
	
	//Testa se a variavel do tipo grafico ja foi criada 
	if( gra_ == NULL ) {
	//Escolhe o titulo do grafico e os das coordenadas X e Y
	gra_ = new XYPlotGraphic(this, "K_gra");
	gra_->plot->setTitleXAxis("Distance");
	}
	if (L)
		gra_->plot->setTitleYAxis("L");
	else
		gra_->plot->setTitleYAxis("K");
	
	//Cria um vetor com a escala do eixo X do grafico
	std::valarray<double> hr(h.size());
	for(i=0; i<h.size(); i++)
		hr[i] = h[i];


	//Cria as curvas que serao impressas 
	Curve *envup = new Curve(upper,hr);
	Curve *crv = new Curve(khat,hr);
	Curve *envlo = new Curve(lower,hr);

	//choose the colors of the curves
	envup->setColor(_colors[1]);
	crv->setColor(_colors[0]);
	envlo->setColor(_colors[1]);

	//plot the curves in the graphic window
	gra_->plot->append(envup);
	gra_->plot->append(crv);
	gra_->plot->append(envlo);
	gra_->show();

	return;
}

///Ao ser inicializada a interface pelo menu do terraview, esse metodo e responsavel
///pela conexao do plugin com banco de dados e devidas inicializacoes que devam ser feitas.
void KFunctionPluginWindow::updateForm(PluginParameters* pluginParams)
{

	plugIngParams_ = (*pluginParams);
	//Inicializa o combo com as funcoes K existentes
	InitializefunctionComboBox();
 	if (plugIngParams_.getCurrentDatabasePtr() == NULL)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Data Base not selected"));
		return;
		
	}
	//Fixa qual vista é carregada inicialmente na abertura do plugin
	if(!control_){
		if (plugIngParams_.getCurrentViewPtr() == NULL)
	    { 
			QMessageBox::warning(this, tr("Warning"), tr("View not selected"));
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
		hide();
		return;     
	}      
	//Testa se o bando de dados esta conectado	
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

	//carregar todos os atributos da tabela do tableComboBox
	//tema1ComboBoxClickSlot(QString(""));
	show();
	return;
}

///Metodo responsavel por esconder a tela de interface do plugin
void KFunctionPluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}