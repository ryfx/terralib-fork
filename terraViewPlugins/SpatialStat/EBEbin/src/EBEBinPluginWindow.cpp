#include <EBEBinPluginWindow.h>

#include <vector>
//#include <math.h>

#include <TeGUIUtils.h> 
#include <TeDatabaseUtils.h>
#include <EBEbinomial.h>
#include <TeWaitCursor.h>
#include <TeDataTypes.h>
#include <TeTable.h>
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeSTEFunctionsDB.h>


//#include "TeAdoDB.h"
#include "TeUpdateDBVersion.h"
#include "TeQuerier.h"
#include "TeQuerierParams.h"
#include "TeSTElementSet.h"
#include "TeSTEFunctionsDB.h"


//#include <qtabwidget.h>
//#include <qspinbox.h> 
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h> 
#include <qradiobutton.h>
#include <qmessagebox.h>
#include <qbuttongroup.h> 
#include <qtextbrowser.h> 
#include <qvalidator.h>
#include <qstring.h> 
#include <qlineedit.h> 

#include <TeDatabaseFactory.h>
#include <TeDatabaseFactoryParams.h>
#include <TeAppTheme.h>

#include <time.h>
#include <iostream>

#include <help.h>

using namespace std;
using std::cout;



EBEbinPluginWindow::EBEbinPluginWindow(QWidget* parent):
	ebe(parent, "EBEbinPluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	radiuslineEdit->setValidator(new QDoubleValidator(this));
	
	localDb_ = false;
	control_ = false;
	init();
}


EBEbinPluginWindow::~EBEbinPluginWindow()
{
	if(localDb_)
		delete localDb_;
}

/*Method: Validate the entry datas of the interface*/
void EBEbinPluginWindow::init()
{
 QDoubleValidator *v = new QDoubleValidator(this);
 v->setBottom(0.0);

 radiuslineEdit->setValidator(v);

}


void EBEbinPluginWindow::Disable_advAttrSlot()
{
   if(globalradioButton->isOn())
	   advancedgroupBox->setEnabled(false);
   else
	   advancedgroupBox->setEnabled(true);
}

void EBEbinPluginWindow::temaComboBoxClickSlot(const QString&)
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

void EBEbinPluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/
	Help help(this, "help", false);
	help.init("TerraClusterEBEBin.htm");

	if(!help.erro_)
		help.exec();

}
    
void EBEbinPluginWindow::scanButtonClickSlot()
{
	// Parametros para simulação
	unsigned int i, j, c;
    int aux, tamanho;
	std::vector<double> pop;					// Vetor de populacao
	std::vector<double> cases;					// Vetor de casos
	vector<double> coordx;					// Coordenadas x
	vector<double> coordy;					// Coordenadas y
	std::vector<string> objIds;
	QString saida = "", str;

	double raio, Dij; //, limiar;	
	TeWaitCursor wait;
	Output_text->clear();


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
	string popAttr = string(populationComboBox->currentText().ascii());
	string caseAttr =  string(caseComboBox->currentText().ascii());

	if(popAttr.empty() || caseAttr.empty())
	{
		QMessageBox::critical(this, tr("Error"), tr("Empty data"));
		return;
	}

	//----------------- pegar do banco os atributos e geometrias que serão utilizados na funcao
	vector<string> attrs;
	attrs.push_back (tableName+"."+popAttr);
	attrs.push_back (tableName+"."+caseAttr);

	TeQuerierParams params(false, attrs);
	params.setParams(theme);
	
	TeQuerier querier(params);
	if(!querier.loadInstances())
	{
		QMessageBox::critical(this, tr("Error"), tr("Not possible to load data"));
		return;
	}

	TeSTInstance obj;
	TeSTElementSet objSet(theme, querier.getAttrList());

	//vector<double> estimativas(objIds.size());
	vector<double> estimativas;


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
		string valPop = "";
		obj.getPropertyValue(valPop, 0);
		pop.push_back (atof(valPop.c_str()));

		string valCase = "";
		obj.getPropertyValue(valCase, 1);
		cases.push_back (atof(valCase.c_str()));

		estimativas.push_back(99.99);        
	}

	tamanho = objIds.size();


	// Verifica se o numero de casos coincide com o número de areas
	if(objIds.size() != pop.size())
	{
		QMessageBox::critical(this, tr("Error"), tr("Population and/or Cases files are incompatible with the Number of Areas"));
		return;
	}
	if(cases.size() != pop.size())
	{
		QMessageBox::critical(this, tr("Error"), tr("Population and Cases files are incompatible"));
		return;
	}

	

	/*--------------  Executa o estimador Bayesiano empirico local  ---------------------------*/
	if(localradioButton->isOn())
	{

	  string dist = string(radiuslineEdit->text().ascii());
	  if(dist.empty()){
	     QMessageBox::warning(this, tr("Warning"), tr("Define the distance!"));
	     return;
	  }
	  else
	     raio = radiuslineEdit->text().toDouble();

	  std::vector< std::vector <double> > vizin(tamanho);   	
	  for(i=0; i<objIds.size(); i++)
	      vizin[i].resize(objIds.size(),0.0);


	  if(ConvertCheckBox->isChecked())
	    {
		  //gerar a matriz de proximidade
          std::vector< vector<double> > geo(tamanho);
          for(i=0; i<tamanho; i++)
	          geo[i].resize(2, 0.0);

          for (i=0; i<tamanho; i++){
               geo[i][0]=coordx[i];
               geo[i][1]=coordy[i];
          }
		  geodeticdistance(geo, vizin, raio);
		}
	  else
	    {
		  //gerar a matriz de proximidade
		  for (i=0; i<tamanho; i++) {
		      for (j=0; j<i; j++) {
				   Dij = sqrt( (coordx[i]-coordx[j])*(coordx[i]-coordx[j]) +
                               (coordy[i]-coordy[j])*(coordy[i]-coordy[j]) );

                   // preenche a matriz de vizinhanca
                   if(Dij < raio)
                      vizin[i][j] = 1.0;
                   else  
                      vizin[i][j] = 0.0;

                   vizin[j][i] = vizin[i][j];
		      }
              vizin[i][i] = 1.0;
	      }
		}

	  // chamar a funcao EBE BINOMIAL local //
	  aux = EBElocal(pop, cases, vizin, estimativas);

	  if (aux != 0)
	      {
	        QMessageBox::critical(this, tr("Error"), tr("Memory Error"));
	        return;
	      }

	  // Imprime os resultados na interface
        saida = tr("Local empirical Bayes estimator: \n") ;
		saida += tr("\n Number of Areas: ") + str.setNum(tamanho);
		saida += tr("\n\n radius: ") + str.setNum( raio );
		saida += tr("\n\n Area: \n");
		saida += tr("\n Index   Estimate \n");
		for(c=0; c<tamanho; c++) 
		{
      		    result_.push_back( objIds[c] );
			    saida += objIds[c].c_str();
			    saida += "     ";
                saida += str.setNum( estimativas[c] );
                saida += "\n";
		}
	   
		Output_text->setText(saida);
	}

	else    // Executa o estimador Bayesiano empirico global  //
	{
	    aux = EBEglobal(pop,cases,estimativas);

        if (aux != 0)
	       {
	        QMessageBox::critical(this, tr("Error"), tr("Memory Error"));
	        return;
	       }

		   // Imprime os resultados na interface
		   saida = tr("Global empirical Bayes estimator: \n") ;
		saida += tr("\n Number of Areas: ") + str.setNum(tamanho);
		saida += tr("\n\n Area: \n");
		saida += tr("\n Index   Estimate \n");
		for(c=0; c<tamanho; c++) 
		{
      		    result_.push_back( objIds[c] );
			    saida += objIds[c].c_str();
			    saida += "     ";
                saida += str.setNum( estimativas[c] );
                saida += "\n";
		}
	   
		Output_text->setText(saida);
	}

	return;
}


void EBEbinPluginWindow::tableComboBoxClickSlot(const QString&)
{
	TeTheme* theme = plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1());

	if(!theme)
		return;

	//carregar os atributos (int ou double) da tabela selecionada
	vector<TeAttrDataType> attrType;
	attrType.push_back (TeINT);
	attrType.push_back (TeREAL);
	if(!fillColumnCombo(attrType, theme, populationComboBox, tableComboBox))
	{
		//error message
		return;
	}

	if(!fillColumnCombo(attrType, theme, caseComboBox, tableComboBox))
	{
		//error message
		return;
	}

	return;
}

//sera chamado toda vez que a form for chamada através do menu
void EBEbinPluginWindow::updateForm(PluginParameters* pluginParams)
{
	plugIngParams_ = (*pluginParams);
	
//	Output_text->clear();
	
	if (plugIngParams_.getCurrentDatabasePtr() == NULL)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Data Base not selected"));
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

	if(!fillThemeCombo(plugIngParams_.getCurrentViewPtr(), 
		themeComboBox, pluginParams->getCurrentThemeAppPtr()->getTheme()->name()))
	{
		//error message
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
void EBEbinPluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}
