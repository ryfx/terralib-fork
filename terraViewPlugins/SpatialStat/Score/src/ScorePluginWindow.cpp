//Bibliotecas

#include <Score.h>
#include <ScorePluginWindow.h>

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

///Metodo de inicializacao da interface. Inicializa as variaveis da interface.
ScorePluginWindow::ScorePluginWindow(QWidget* parent):
	score(parent, "ScorePluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	xlineEdit->setValidator(new QDoubleValidator(this));
	ylineEdit->setValidator(new QDoubleValidator(this));
	pondlineEdit->setValidator(new QDoubleValidator(this));

	localDb_ = false;
	control_ = false;
	ncols_ = 0;
}


///Destrutor da classe, deleta a conexao com o banco de dados ao sair da interface.
ScorePluginWindow::~ScorePluginWindow()
{
	if(localDb_)
		delete localDb_;
}


///Responsavel pelo preenchimento do combo de tabelas  das quais pertencentes ao tema especificado.
void ScorePluginWindow::temaComboBoxClickSlot(const QString&)
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

void ScorePluginWindow::Enable_pondSlot(const QString&)
{
	if(functionComboBox->currentText().toInt() == 0){
		pondlineEdit->setEnabled(true);
	}
	else{
		pondlineEdit->setEnabled(false);
	}
}


void ScorePluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/
	
	Help help(this, "help", false);
	help.init("TerraClusterScore.htm");

	if(!help.erro_)
		help.exec();

}

///Funcao principal da interface. Executa o Metodo de Score quando o botão "OK" é clicado e apresenta resultados finais na interface.
void ScorePluginWindow::okButtonClickSlot()
{	
	//Parâmetros Auxiliares
	double    coordx0, coordy0, Pvalor, Ust,pond=1.0,casostotal=0.0,poptotal=0.0;
    int i,tipo;
	QString str, saida = "";			// String auxiliar e de saida		
	std::vector<string> objIds;
	std::vector<double> coordx;			// Coordenadas x
	std::vector<double> coordy;			// Coordenadas y
	std::vector<double> casos, pop;			// Casos
	std::vector<double> h;
	vector<string> attrs1;

 
	TeWaitCursor wait;
	
	//Carrega as variavies da interface
	coordx0 = xlineEdit->text().toDouble();
	coordy0 = ylineEdit->text().toDouble();
	tipo = functionComboBox->currentText().toInt();
	//Enable_pondSlot();
	if (tipo== 0)
	{
		pond = pondlineEdit->text().toDouble();
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
		//Pega os atributos
				
		//pegar o centroide
		TeCoord2D coord;
		obj.centroid(coord);
		coordx.push_back(coord.x());
		coordy.push_back(coord.y());
		//pegar os atributos


		string valCase = "";
		obj.getPropertyValue(valCase, 1);
		casos.push_back(atof(valCase.c_str()));

		//pegar os atributos de populacao e caso
		string valPop = "";
		//Carrega na variavel tipo instancia o valor da populacao para aquela linha
		obj.getPropertyValue(valPop, 0);
		//Salva no vetor de populacao o vetor carregado do banco
		pop.push_back (atof(valPop.c_str()));


	}
	
	// Verifica se o numero de casos coincide com o número de areas
	if(objIds.size() != pop.size())
	{
		QMessageBox::critical(this, tr("Error"), tr("Population and/or Cases files are incompatible with the Number of Areas"));
		return;
	}
	//Verifica se o tamanho do vetor de casos coicide com o tamanho do vetor de populacao
	if(casos.size() != pop.size())
	{
		QMessageBox::critical(this, tr("Error"), tr("Population and Cases files are incompatible"));
		return;
	}


    int areas = coordx.size();

	vector <double> d(areas),esperados(areas);

	for (i=0;i<areas;i++){
        d[i] = sqrt(pow((coordx[i]-coordx0),2) + pow((coordy[i]-coordy0),2)); 
				// Distancias ate a fonte
        casostotal += casos[i];
        poptotal += pop[i];
	}

    for ( i=0;i<areas;i++){
        esperados[i]=pop[i]*casostotal/poptotal;
	}

    /// Calculo da estatistica de teste
    Ust=scorefunction(tipo,casos,esperados,d,pond);
    ///Calculo do p-valor
    Pvalor= 1-AcNormal(Ust);
	
	// Imprime os resultados na interface
    saida = tr("Score's Method \n \n Estatística Score: ") + str.setNum( Ust );
	saida += tr("\n P-Value: ") + str.setNum(Pvalor);
	

    Output_text->setText(saida);
    
	return;
}

///Carrega tabela especificada pelo usuario
void ScorePluginWindow::table1ComboBoxClickSlot(const QString&)
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

	if(!fillColumnCombo(attrType, theme, populationComboBox, tableComboBox))
	{
		//error message
		return;
	}

	return;
}

///Ao ser inicializada a interface pelo menu do terraview, essa funcao e responsavel pela conexao do plugin com o banco de dados e devidas inicializacoes que devam ser feitas.
void ScorePluginWindow::updateForm(PluginParameters* pluginParams)
{
	plugIngParams_ = (*pluginParams);
	Output_text->clear();
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
void ScorePluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}
