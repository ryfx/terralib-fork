//Bibliotecas

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

#include <OpgamPluginWindow.h>
#include <lgamma.h>

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
OpgamPluginWindow::OpgamPluginWindow(QWidget* parent):
	opgam(parent, "OpgamPluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	radiuslineEdit->setValidator(new QDoubleValidator(this));
	steplineEdit->setValidator(new QDoubleValidator(this));
	alphalineEdit->setValidator(new QDoubleValidator(this));

	localDb_ = false;
	control_ = false;
	//ncols_ = 0;
}


///Destrutor da classe, deleta a conexao com o banco de dados ao sair da interface.
OpgamPluginWindow::~OpgamPluginWindow()
{
	if(localDb_)
		delete localDb_;
}


///Responsavel pelo preenchimento do combo de tabelas  das quais pertencentes ao tema especificado.
void OpgamPluginWindow::temaComboBoxClickSlot(const QString&)
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

void OpgamPluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/
	
	Help help(this, "help", false);
	help.init("TerraClusterOpgam.htm");

	if(!help.erro_)
		help.exec();

}

///Funcao principal da interface. Executa o Metodo de Opgam quando o botão "OK" é clicado e apresenta resultados finais na interface.
void OpgamPluginWindow::okButtonClickSlot()
{
    double radius, step, alpha;            //Parâmetros de entrada

	QString str, saida = "";	       // String auxiliar e de saida
	vector<string> objIds;
	vector<double> coordx;			// Coordenadas x
	vector<double> coordy;			// Coordenadas y
	vector<int> casos, pop;			// Casos
	vector<string> attrs1;


	
	TeWaitCursor wait;

	//Carrega as variavies da interface
	radius = radiuslineEdit->text().toDouble();
	step = steplineEdit->text().toDouble();
	alpha = alphalineEdit->text().toDouble();

        

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


		//pegar os atributos de populacao e caso
		string valPop = "";
		//Carrega na variavel tipo instancia o valor da populacao para aquela linha
		obj.getPropertyValue(valPop, 0);
		//Salva no vetor de populacao o vetor carregado do banco
		pop.push_back (atof(valPop.c_str()));

		string valCase = "";
		obj.getPropertyValue(valCase, 1);
		casos.push_back(atof(valCase.c_str()));
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

  /////O método começa aqui!!!!

       // Imprime os resultados na interface
       saida = tr("Opgam's Method Results \n");


        int n=pop.size();
        int i, j, d, k, q, ngrx, ngry;
		vector<int> idtemp;
        double xmin=coordx[0], xmax=coordx[0];
        double ymin=coordy[0], ymax=coordy[0];
        double xpt, ypt, dpt;
        double poptot=0,obstot=0,tx=0 ;
        vector<double> esp;
        vector<int>idcluster;
		idtemp.resize(n);
		idcluster.resize(n);
		esp.resize(n);


       for (i=0; i<n; i++) {
	     idtemp[i] = 0;
		 idcluster[i] = 0;
         poptot += pop[i];
         obstot += casos[i];
       }

       tx = obstot/poptot;
       for (i=0; i<n; i++) {
         esp[i] = pop[i]*tx;
       }



/// Início dos cálculos
/// Encontrando ymin, xmin, ymax e xmax

       for (i=0; i<n; i++) {
         if (coordx[i]<xmin)
            xmin = coordx[i];
         if (coordx[i]>xmax)
            xmax = coordx[i];
         if (coordy[i]<ymin)
            ymin = coordy[i];
         if (coordy[i]>ymax)
            ymax = coordy[i];
      }

/// Cálculo do tamanho da grade em cada sentido
      ngrx = (int)((xmax-xmin)/(step)) + 1;
      ngry = (int)((ymax-ymin)/(step)) + 1;

/// Declarando outras variáveis
/// osum     : vetor inteiro com tamanho igual ao número de pontos na grade
///            para guardar o número observado de eventos no candidato à cluster.
/// esum     : vetor double com tamanho igual ao número de pontos na grade
///            para guardar o número esperado de eventos no candidato à cluster.
/// pval     : vetor double com tamanho igual ao número de pontos na grade
///            para guardar o valor p do candidato à cluster.

	  int teste=ngrx*ngry;
       vector<int> osum;
	   osum.resize(teste);
       vector<double> esum, pval;
	   esum.resize(teste);
	   pval.resize(teste);
	   for(i=0; i<teste; i++)
		   pval[i]=0.0;

/// Inicializando k e xpt
       k = 0;
       xpt = xmin; 

/// For para o número de pontos de x da grade 
/// A cada ponto de x, o ponto da grade em y inicia no mínimo
      for (i=0; i<ngrx; i++) {
        ypt = ymin;
/// For para o número de pontos de y na grade
/// A cada ponto de x e y, osum e esum são zerados
        for (j=0; j<ngry; j++) {
          osum[k]=0; 
		  esum[k]=0.0;
/// For para o número de áreas contidas nos dados
/// A cada área, é calculada a distância até ao ponto corrente da grade
           for (d=0; d<n; d++) {
	     dpt = sqrt(pow(coordx[d]-xpt,2.0) + pow(coordy[d]-ypt, 2.0));
	     idtemp[d] = 0;
/// Se a área está a uma distância menor que o raio, ela pertence ao candidato
/// à cluster e os eventos observados e esperados nessa área são somados aos
/// eventos observados e esperados do candidato à cluster. É assinalado no
/// vetor auxiliar de identificação que essa área pertence ao candidato à cluster.
	     if (dpt<radius) {
	       esum[k] += esp[d];
	       osum[k] += casos[d];
	       idtemp[d] = 1;
	     }
           }
/// Se o número de observados é igual a zero, o valor p é igual a 1
          if (osum[k]==0)
	    pval[k] = 1;
          else {
/// Se não, o valor p é calculado e é a área acima do número observado de
/// óbitos dentro do candidato à cluster, considerando uma densidade de Poisson
/// e parâmetro igual ao número esperado de óbitos dentro do candidato à cluster
            pval[k]=0.0;
          for (q=0; q<=osum[k]; q++)
            pval[k] += exp(q*log(esum[k]) - esum[k] - lgamma(q+1));

          pval[k] = 1-pval[k];
/// Se o valor p é menor que o nível de significância, esse candidato é um cluster.
/// É impresso id do ponto da malha relativo ao centro do cluster,
/// as coordenadas da malha, o número observado e esperado de óbitos e o valor p
	 if (alpha>pval[k]) {
//            saida += "\n" + str.setNum( xpt ) + " " + str.setNum( ypt ) + " " + str.setNum( osum[k] ) + " " + str.setNum( esum[k] ) + " " + str.setNum( pval[k] );


/// As áreas pertencentes ao cluster são assinaladas no vetor que identifica
/// áreas pertencentes à cluster, utilizando o vetor auxiliar que identificou
/// quais áreas pertencem ao cluster.
	  for (d=0; d<n; d++) {
	    if (idtemp[d]==1)
	      idcluster[d] = 1;
	  }
	}
      }
/// o valor k é incrementado de um e a coordenada y da malha é incrementado de step
      k++;
      ypt += step;
    }
/// a coordenada x da malha é incrementado de step
    xpt += step;
  }
/// A seguir são impressos o id e as coordenadas das áreas pertencentes à cluster
	  saida += tr("\n\nAreas which belong some clusters:");
  //printf("\nAreas pertencentes a clusters:\n");
  //printf("ID X Y \n");
  saida += "\n  ID              X                Y ";
  for (d=0; d<n; d++) {
    if (idcluster[d]==1)
      saida += "\n" + str.setNum( d+1 )+ "     " + str.setNum( coordx[d] )+ "     " + str.setNum( coordy[d] );
//      printf("%d %.2f %.2f \n", d+1, coordx[d], coordy[d]);
  }


    Output_text->setText(saida);
 return;
}

///Carrega tabela especificada pelo usuario
void OpgamPluginWindow::table1ComboBoxClickSlot(const QString&)
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
void OpgamPluginWindow::updateForm(PluginParameters* pluginParams)
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
void OpgamPluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}
