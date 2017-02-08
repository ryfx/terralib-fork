//Bibliotecas

#include <Diggle.h>
#include <DigglePluginWindow.h>

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
DigglePluginWindow::DigglePluginWindow(QWidget* parent):
	diggle(parent, "DigglePluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	xlineEdit->setValidator(new QDoubleValidator(this));
	ylineEdit->setValidator(new QDoubleValidator(this));
	
	localDb_ = false;
	control_ = false;
	ncols_ = 0;
}


///Destrutor da classe, deleta a conexao com o banco de dados ao sair da interface.
DigglePluginWindow::~DigglePluginWindow()
{
	if(localDb_)
		delete localDb_;
}


///Responsavel pelo preenchimento do combo de tabelas  das quais pertencentes ao tema especificado.
void DigglePluginWindow::temaComboBoxClickSlot(const QString&)
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

void DigglePluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/
	Help help(this, "help", false);
	help.init("TerraClusterDiggle.htm");

	if(!help.erro_)
		help.exec();

}
///Funcao principal da interface. Executa o Metodo de Diggle quando o bot�o "OK" � clicado e apresenta resultados finais na interface.
void DigglePluginWindow::okButtonClickSlot()
{	
	//Par�metros Auxiliares

	double coordx0, coordy0, betha, phi, Pvalor, T;
	int i,tipo; //inicializei o tipo apenas pra teste. Verificar como pedir isso
	int Npermut; // inicializei o Pvalor apenas pra teste. Verificar como pedir isso
	QString str, saida = "";			// String auxiliar e de saida		
	std::vector<string> objIds;
	std::vector<double> coordx;			// Coordenadas x
	std::vector<double> coordy;			// Coordenadas y
	std::vector<double> casos, controles, casoscontroles, h;
	std::vector<int> indca, indco;
	vector<string> attrs1;

 
	TeWaitCursor wait;
	
	
	//Obt�m o tema, a tabela e os atributos selecionados na tela
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

	//Pega do banco os atributos e geometrias que ser�o utilizados no m�todo de Diggle
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
		//Pega os atributos
				
		//pegar o centroide
		TeCoord2D coord;
		obj.centroid(coord);
		coordx.push_back(coord.x());
		coordy.push_back(coord.y());
		//pegar os atributos

		string valCase = "";
		obj.getPropertyValue(valCase, 0);
		casoscontroles.push_back(atof(valCase.c_str()));
	}
	
    int eventos = coordx.size();
	
	int ncasos = 0;
	int ncontroles = 0;

	for(i=0; i<eventos; i++)
	{
		if (casoscontroles[i] == 1)
		{
			casos.push_back(casoscontroles[i]);
			indca.push_back(i);
			ncasos++;
		}
		else
		{
			controles.push_back(casoscontroles[i]);
			indco.push_back(i);
			ncontroles++;
		}
	}

	double alfa=1.0, beta=1.0, D=0.0, L0=0.0, L1=0.0;
	int total, ind;
	coordx0 = 1.0, coordy0 = 0.0;
	
	total = ncasos + ncontroles;

	vector <double> coordx2(ncasos+ncontroles), coordy2(ncasos+ncontroles), casosx(ncasos), casosy(ncasos), controlesx(ncontroles), controlesy(ncontroles), d(total);

	coordx0 = xlineEdit->text().toDouble();
	coordy0 = ylineEdit->text().toDouble();
    
	for (i=0;i<ncasos;i++)
	{
		ind = indca[i];
		coordx2[i] = coordx[ind];
		coordy2[i] = coordy[ind];
	}
	for (i=0;i<ncontroles;i++)
	{
		ind = indco[i];
		coordx2[ncasos+i]= coordx[ind];
		coordy2[ncasos+i]= coordy[ind];
	}

	for (i=0;i<total;i++)
	     {
	      d[i] = sqrt(pow((coordx2[i]-coordx0),2) + pow((coordy2[i]-coordy0),2));  // Distancias ate a fonte
	      printf(" %lf\n",d[i]);
	} 


	/// Calcula a estatistica de teste
	D=diggletest(alfa,beta,d, ncasos, ncontroles,L1,L0);
	Pvalor=quiquadrado1gl(D);

	
	// Imprime os resultados na interface
	saida = tr("Diggle's Method \n \n Statistics D: ") + str.setNum( D );
	saida += tr("\n P-Value: ") + str.setNum(Pvalor);
	

	Output_text->setText(saida);
    
	return;
}


///Carrega tabela especificada pelo usuario
void DigglePluginWindow::table1ComboBoxClickSlot(const QString&)
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
void DigglePluginWindow::updateForm(PluginParameters* pluginParams)
{
	plugIngParams_ = (*pluginParams);
	Output_text->clear();
	Enable_matrixSlot();

	if (plugIngParams_.getCurrentDatabasePtr() == NULL)
	{
		QMessageBox::critical(this, tr("Error"), tr("Empty data"));
				return;

	}
	//Fixa qual vista � carregada inicialmente na abertura do plugin
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
void DigglePluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}