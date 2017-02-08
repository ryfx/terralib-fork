#include <valarray>

#include <RogersonPluginWindow.h>
#include "algoritmos.h"
#include <curve.h>

#include <list>

#include <math.h>
#include <string.h>
#include <algorithm>
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


std::vector<SistVigEnt> PermTempo(std::vector<SistVigEnt> Dados, int nEvts);
bool operator<(const SistVigEnt &c, const SistVigEnt &d){
   return (c.t < d.t);
}


RogersonPluginWindow::RogersonPluginWindow(QWidget* parent):
	rogerson(parent, "RogersonPluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	RadiuslineEdit->setValidator(new QDoubleValidator(this));
	MedidaTempo->setValidator(new QDoubleValidator(this));
	Arl->setValidator(new QDoubleValidator(this));
	
	localDb_ = 0;
	control_ = false;
	gra_ = 0;
	ncols_ = 0;
	init();
}


RogersonPluginWindow::~RogersonPluginWindow()
{
	if(localDb_)
		delete localDb_;
}

/*Method: Validate the entry datas of the interface*/
void RogersonPluginWindow::init()
{
/* QDoubleValidator *v = new QDoubleValidator(this);
 v->setBottom(0.000000001);

 RadiuslineEdit->setValidator(v);
 EpsilonlineEdit->setValidator(v);
 TresholdlineEdit->setValidator(v);
 */
}

void RogersonPluginWindow::temaComboBoxClickSlot(const QString&)
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

void RogersonPluginWindow::Enable_timeSlot()
{
	if(timecheckBox->isChecked()){
		tableComboBox->setEnabled(true);
		timeComboBox->setEnabled(true);	
		timeUnitComboBox->setEnabled(false);
	}
	else{
		tableComboBox->setEnabled(false);
		timeComboBox->setEnabled(false);	
		timeUnitComboBox->setEnabled(true);
	}
}

void RogersonPluginWindow::CloseSlot()
{
	//if(localDb_)
	//	delete localDb_;
	//localDb_ = 0;
	QWidget::close();
}

void RogersonPluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/
	
	Help help(this, "help", false);
	help.init("TerraClusterRogerson.htm");

	if(!help.erro_)
		help.exec();

}

void RogersonPluginWindow::okButtonClickSlot()
{
	Output_text->setText("");
	// Declaração de variáveis
	QString str, saida = "";			
	std::vector<double> coordx;			// Coordenadas x
	std::vector<double> coordy;			// Coordenadas y
	std::vector<double> coordt;			// Coordenadas t
	std::vector<string> objIds;
	int i, numPerm, j, nEvts, posQuantil,UniTempo;
    std::vector<double> SiLimiar, Si, Simax;
    double TempoC, arl, h0, maxIter, prec, k, alarmThreshold;
    long double RaioC;
    std::vector<SistVigEnt> Dados, DadosLimiar, DadoPerm;
	vector<string> attrs;
	valarray<double> Siaux;

	TeWaitCursor wait;
	/* Inicializa Variaveis e carrega as da interface*/
   numPerm=1000;
   nEvts = 0;
   RaioC = RadiuslineEdit->text().toDouble();
   TempoC = MedidaTempo->text().toDouble();
   arl = Arl->text().toDouble();
   k = 0.5;
   
	//create the vector of possible colors for the graphic
	if( _colors.empty() ) {
	_colors.push_back(Qt::black);
	_colors.push_back(Qt::red);
	/*_colors.push_back(Qt::green);
	_colors.push_back(Qt::cyan);
	_colors.push_back(Qt::magenta);
	_colors.push_back(Qt::yellow);
	_colors.push_back(Qt::gray);*/
	}	gra_= 0;

    //----------------- pegar o tema, a tabela e os atributos selecionados na tela
	//Carrega o tema com o primeiro conjunto de pontos
	TeTheme* theme = plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1()); 
	if(!theme)
	{
		QMessageBox::critical(this, tr("Error"), tr("Not possible to load the Theme"));
		return;
	}
	Output_text->setText(tr(" Surveillance System \n running..."));
	TeQuerierParams querierParams(true,false);
	querierParams.setParams(theme);
	//querierParams.setParams(theme, TeMONTH);
	TeQuerier  querier;//(querierParams);
    if(timecheckBox->isChecked())
		{
			//Carrega a tabela de atributos
			string tabelaName = string(tableComboBox->currentText().ascii());
			if(tabelaName.empty())
			{
				QMessageBox::critical(this, tr("Error"), tr("Theme without Table"));
				return;
			}

			//Carrega os atributos
			string TimeAttr = string(timeComboBox->currentText().ascii());
			if(TimeAttr.empty())
			{
				QMessageBox::critical(this, tr("Error"), tr("Empty data"));
				return;
			}
			
			//Carrega os atributos para memoria
			attrs.push_back (tabelaName+"."+TimeAttr);
			
			//----------------- pegar do banco os atributos e geometrias que serão utilizados na funcao
            querierParams.setParams(theme);
			querierParams.setFillParams(true,false,attrs);
			querier.refresh(querierParams);

			if(!querier.loadInstances())
			{
				QMessageBox::critical(this, tr("Error"), tr("Not possible to load data"));
				return;
			}

			TeSTInstance obj;
			TeSTElementSet objSet(theme);
			while(querier.fetchInstance(obj))
			{
				//armazenar as instancias do querier
				objSet.insertSTInstance(obj);
				//armazenar os identificadores
				objIds.push_back(obj.objectId());
				
				//pegar o centroide
				TeCoord2D coord;
				obj.centroid(coord);
				coordx.push_back(coord.x());
				coordy.push_back(coord.y());

				//pegar os atributos
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
		   //Escolhe o chrono de acordo com o selecionado na interface
			TeChronon Chrono;
		    int intchrono = timeUnitComboBox->currentItem();
			switch (intchrono)
			{
				case 0:
				{
					Chrono = TeHOUR;
					break;
				}
				case 1:
				{
					Chrono = TeDAY;
					break;
				}
				case 2:
				{
					Chrono = TeMONTH;
					break;
				}
				case 3:
				{
					Chrono = TeYEAR;
					break;
				}
				default:
				{
					cout << "ERRO" << endl;
					break;
				}
			}

			// -------- Criar um Querier com o tema selecionado e chrono
			querierParams.setParams(theme, Chrono); //associar o querier ao tema escolhido e montar os frames pelo chrono
			querier.refresh(querierParams); //Recarrega o querier com os novos parametros escolhidos

			// -------- Retorna o número de frames gerado baseado no chronon associado ao querier
			//ERRO AQUI SEMPRE RETORNA 0
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
					TeSTInstance obj;
					while(querier.fetchInstance(obj))
					{
						//armazenar as instancias do querier
						objSet.insertSTInstance(obj);
						//armazenar os identificadores
						objIds.push_back(obj.objectId());

						// pegar o ponto (x,y) correspondente
						TeCoord2D coord;
						obj.centroid(coord);
						coordx.push_back(coord.x());
						coordy.push_back(coord.y());
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

	SVEvent ev_d;
    SVEventLst ev;

	for(i=0;i<coordx.size();i++){
		ev_d.x = coordx[i];
		ev_d.y = coordy[i];
		ev_d.t = coordt[i];
		ev.push_back(ev_d);
	}
	ev.sort();
	nEvts = coordx.size();


//	if (EvLimiar < (nEvts/3))
//		EvLimiar = (nEvts/3);

	Dados.resize(nEvts);
	DadosLimiar.resize(nEvts);
	DadoPerm.resize(nEvts);

	Si.resize(nEvts,-1.0);
	SiLimiar.resize(nEvts,-1.0);
	Simax.resize(numPerm,-1.0);

	for (i=0;i<nEvts;i++){
		DadosLimiar[i].t = coordt[i];
		Dados[i].t = DadosLimiar[i].t;
		Dados[i].x = coordx[i];
		Dados[i].y = coordy[i];

	}

	

/*	for(i=0; i<numPerm; i++)
	{
    for (j=0; j<nEvts; j++)
	DadoPerm = PermTempo(DadosLimiar,nEvts);
    std::sort(DadoPerm.begin(),DadoPerm.end());
    SiLimiar = SistemadeVigilancia(ev,DadoPerm,nEvts,k,TempoC,RaioC);
      if (Simax[i] < SiLimiar[j])
         Simax[i] = SiLimiar[j];
  }

  std::sort(Simax.begin(),Simax.end());
  posQuantil = (1-alpha)*numPerm;
  alarmThreshold = Simax[posQuantil];
*/
  alarmThreshold = NRaphson(arl);

  bool wasCancelled = false;
  Si = SistemadeVigilancia(ev,Dados,nEvts,k,TempoC,RaioC, wasCancelled);

  if(wasCancelled)
  {
	Output_text->setText(tr("Canceled by the User..."));
	return;
  }  

  double tempoS, aux;
  int Ev;
  saida += tr("Threshold Alarm: ") + str.setNum( alarmThreshold );
  Siaux.resize(nEvts);
  for(i=0;i<nEvts;i++)
  {
    if(Si[i] > alarmThreshold)
    {
      tempoS = Dados[i].t;
      Ev = i;
      break;
    }
  }
 	
		//Imprime os resultados na interface
		
		for(i=0;i<nEvts;i++)
			saida += "\n " + str.setNum( Si[i] );
  	
	Output_text->setText(saida);

	if( gra_ == NULL ) {
	gra_ = new XYPlotGraphic(this, "Rogerson");
	gra_->plot->setTitleXAxis("Event Number");
	gra_->plot->setTitleYAxis("Cumulative Sum");
	}
	
	//set the curves that will be printed
	std::valarray<double> treshold(ev.size()),scale(ev.size());
	for(i=0;i<treshold.size();++i){
		treshold[i] = alarmThreshold;
		scale[i] =  i+1;
		aux = Si[i];
		Siaux[i] = aux;
	}
	Curve *lim = new Curve(treshold,scale);
	Curve *crv = new Curve(Siaux,scale);
	
	
	//choose the colors of the curves
	crv->setColor(_colors[0]);
	lim->setColor(_colors[1]);
	//if( (ncols_%7) == 0 ) ncols_ = 0;
	//plot the curves in the graphic window
	gra_->plot->append(lim);
	gra_->plot->append(crv);	
	gra_->show();

	return;
}


void RogersonPluginWindow::tableComboBoxClickSlot(const QString&)
{
	TeTheme* theme = plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1()); 
	if(!theme)
		return;

	//carregar os atributos (int ou double) da tabela selecionada
	vector<TeAttrDataType> attrType;
	attrType.push_back (TeINT);
	attrType.push_back (TeREAL);
	if(!fillColumnCombo(attrType, theme, timeComboBox, tableComboBox))
	{
		//error message
		return;
	}

	return;
}

//sera chamado toda vez que a form for chamada através do menu
void RogersonPluginWindow::updateForm(PluginParameters* pluginParams)
{
	plugIngParams_ = (*pluginParams);

 	if (plugIngParams_.getCurrentDatabasePtr() == NULL)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Data Base not selected"));
		return;
		
	}

	timecheckBox->setChecked(false);
	Enable_timeSlot();
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
	if(!fillThemeCombo(TePOINTS,localDb_->viewMap()[ViewId_], 
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

void RogersonPluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}

	
