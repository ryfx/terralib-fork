
#include <vector>
#include <valarray>

#include <TeGUIUtils.h> 
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeDatabaseUtils.h>
#include <TeWaitCursor.h>
#include <TeSTEFunctionsDB.h>
#include <TeDataTypes.h>

#include <math.h>
#include <string.h>
#include <algorithm>

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

#include <ranker.h>
#include <newmatap.h>

#include <Tango.h>
#include <TangoPluginWindow.h>

#include <TeSTElementSet.h>
#include <TeDatabaseFactory.h>
#include <TeDatabaseFactoryParams.h>
#include <TeAppTheme.h>

#include <help.h>

///Metodo de inicializacao da interface. Inicializa as variaveis de interface.
TangoPluginWindow::TangoPluginWindow(QWidget* parent):
	tango(parent, "TangoPluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	localDb_ = false;
	control_ = false;
}

///Destrutor da classe, deleta a conexao com o Banco de dados ao sair da interface
TangoPluginWindow::~TangoPluginWindow()
{
	if(localDb_)
		delete localDb_;
}

///Metodo responsavel pelo preenchimento do combo de tabelas com as tabelas pertencentes
///ao tema especificado


void TangoPluginWindow::temaComboBoxClickSlot(const QString&)
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

///Metodo para abilitar/desabilitar a opcao de analise estratificada de acordo com a escolha do 
///usuario.
void TangoPluginWindow::Enable_StrataSlot()
{
		//Testa se a opcao de analise estratificada esta marcada
		if(stratacheckBox->isChecked())
		{
			strataComboBox->setEnabled(true);
		}
		else
		{
			strataComboBox->setEnabled(false);
		}
}

void TangoPluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/
	
	Help help(this, "help", false);
	help.init("TerraClusterTango.htm");

	if(!help.erro_)
		help.exec();

}

///Metodo principal da interface. Execucao do algoritmo onde e executado o metodo de Tango,  
///responsavel por carregar todas as variaveis da interface e do banco de dados.
void TangoPluginWindow::RunButtonClickSlot()
{
	// Parametros para simulação
	int montecarlo, aux;					// Iteraçoes de Monte Carlo
	QString str, saida = "";				// String auxiliar e de saida
	vector<int> pop;					    // Vetor de populacao em risco
	vector<int> cases;					// Vetor de casos
	vector<int> strata;					// Vetor de numero de estrato
	vector<double> coordx;					// Coordenadas x
	vector<double> coordy;					// Coordenadas y
	vector<string> objIds;
	Output_text->clear();


	TeWaitCursor wait;

	switch(this->montecarlobuttonGroup->selectedId())	// Seleciona o numero de simulacoes de Monte Carlo
	{
		case 0:
			montecarlo = 99;
			break;
		case 1:
			montecarlo = 999;
			break;
	}

	//----------------- pegar o tema, a tabela e os atributos selecionados na tela
/*	TeView *view = localDb_->viewMap()[ViewId_];
	int themeId = (view->get(string(themeComboBox->currentText().ascii())))->id();
	TeTheme* theme = localDb_->themeMap()[themeId]; 
*/
	TeTheme* theme = plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1());
//	int themeId = theme->id();

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
	string popAttr = string(popComboBox->currentText().ascii());
	string casesAttr = string(casesComboBox->currentText().ascii());
	string strataAttr = string(strataComboBox->currentText().ascii());
	if(popAttr.empty() || casesAttr.empty() || strataAttr.empty())
	{
		QMessageBox::critical(this, tr("Error"), tr("Empty data"));
		return;
	}

	//----------------- pegar do banco os atributos e geometrias que serão utilizados na funcao
	vector<string> attrs;
	attrs.push_back (tableName+"."+popAttr);
	attrs.push_back (tableName+"."+casesAttr);
	attrs.push_back (tableName+"."+strataAttr);

	TeQuerierParams params(true, attrs);
	params.setParams(theme);
	
	TeQuerier querier(params);

	if(TeProgress::instance())
		{
			string caption = tr("Jacquez").latin1();
			TeProgress::instance()->setCaption(caption.c_str());
			string msg = tr("Running Jacquez's KNN Method. Please, wait!").latin1();
			TeProgress::instance()->setMessage(msg);
		}

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
		string valPop = "";
		obj.getPropertyValue(valPop, 0);
		pop.push_back (atof(valPop.c_str()));

		string valCase = "";
		obj.getPropertyValue(valCase, 1);
		cases.push_back (atof(valCase.c_str()));

		string valStrata = "";
		obj.getPropertyValue(valStrata, 0);
		strata.push_back (atof(valStrata.c_str()));
	}

	// Verifica se o numero de casos coincide com o número de areas
	if( (objIds.size() != pop.size()) || (objIds.size() != cases.size()))
	{
		QMessageBox::critical(this, tr("Error"), tr("Population and/or Cases files are incompatible with the Number of Areas"));
		return;
	}
	if((cases.size() != pop.size()) || (cases.size() != strata.size()) || (pop.size() != strata.size()))
	{
		QMessageBox::critical(this, tr("Error"), tr("Population and/or Cases ans Strata files are incompatible"));
		return;
	}

/* - - - - - - - - - - - - - - Tango's MEET Test - - - - - - - - - - - - - - - - - - - - - - - */

	// **** teste estratificado *****
	if(stratacheckBox->isChecked())
	  {
	   Output_text->setText(tr(" Tango's MEET Test \n running..."));
	
       int i, nk, lambda_opt, size = cases.size();	    
	   double pvalue, Pmin;
	   std::valarray <int> pop1(size);			// Vetor de populacao em risco
	   std::valarray <int> cases1(size);	    	// Vetor de casos
	   std::valarray <int> strata1(size);		// Vetor de numero de estrato de cada area
    
	   for (i=0; i<size; i++){
	        pop1[i] = pop[i];
	        cases1[i] = cases[i];
	        strata1[i] = strata[i];
	   }
	
	   std::valarray<double> ZZ(montecarlo+1);
       ColumnVector ss(size);    


//       valarray<int> regid; 
//       Matrix geo(size,size);
//       geo = 0.0;

       std::vector< vector<double> > geo(size);
       for(i=0; i<size; i++)
	       geo[i].resize(2, 0.0);

       for (i=0; i<size; i++){
             geo[i][0]=coordx[i];
             geo[i][1]=coordy[i];
       }


/*       vector<char> geocode(size);
       for (i=0; i<size; i++)
            geocode[i] = objIds[i];
*/
	   // Executa o metodo MEET de Toshiro Tango 

	   if(convertcheckBox->isChecked())
	     {
	       aux = Meet1(montecarlo, strata1, pop1, cases1, geo, objIds, ZZ, pvalue, lambda_opt, Pmin, ss, nk);
		 }
	   else
		 {
	       aux = Meet2(montecarlo, strata1, pop1, cases1, geo, objIds, ZZ, pvalue, lambda_opt, Pmin, ss, nk);
		 }

	   if (aux != 0)
		  {
	   	   QMessageBox::critical(this, tr("Error"), tr("Memory Error"));
		   return;
		  }

	   vector<double> ZZZ(montecarlo+1);
       for (i=0; i<=montecarlo; i++) 
            ZZZ[i] = ZZ[i]; 


	   // Imprime os resultados na interface
	   saida = tr("Tango's MEET Test (stratified) \n \n Number of regions: ") + str.setNum( size );
	   saida += tr("\n Number of strata: ") + str.setNum( nk );
	   saida += tr("\n Number of Monte Carlo iterations: ") + str.setNum( montecarlo );
	   saida += tr("\n optimal lambda: ") + str.setNum( lambda_opt );
	   saida += "\n Pmin: " + str.setNum( Pmin );
	   saida += tr("\n pvalue: ") + str.setNum( pvalue );
	   saida += tr("\n \n Some percentiles for Z's distribution: \n \n percentile 0.5%: ") + str.setNum( quantile(ZZZ, 0.005) );
	   saida += tr("\n percentile 2.5%: ") + str.setNum( quantile(ZZZ, 0.025) );
	   saida += tr("\n percentile 5%: ") + str.setNum( quantile(ZZZ, 0.05) );
	   saida += tr("\n percentile 10%: ") + str.setNum( quantile(ZZZ, 0.1) );
	   saida += tr("\n percentile 90%: ") + str.setNum( quantile(ZZZ, 0.9) );
	   saida += tr("\n percentile 95%: ") + str.setNum( quantile(ZZZ, 0.95) );
	   saida += tr("\n percentile 97.5%: ") + str.setNum( quantile(ZZZ, 0.975) );
	   saida += tr("\n percentile 99.5%: ") + str.setNum( quantile(ZZZ, 0.995) );
       saida += "\n";

	   for(i=0;i<size;i++){
//	  	   saida += str.setNum( objIds[i] );
//		   saida += "   ";
		   saida += str.setNum( ss[i] );
           saida += "\n";
       }

	   Output_text->setText(saida);
	}

    // ***** teste nao estratificado *******
	else
	{
	   Output_text->setText(tr(" Tango's MEET1 Test \n running..."));
	
       int i, lambda_opt, size = cases.size();	    
	   double pvalue, Pmin;
       valarray <int> pop1(size);			// Vetor de populacao em risco
	   valarray <int> cases1(size);	    	// Vetor de casos
	
	   for (i=0; i<size; i++){
	        pop1[i] = pop[i];
	        cases1[i] = cases[i];
	   }
	
	   valarray<double> ZZ(montecarlo+1);
       ColumnVector ss(size);    


//       valarray<int> regid; 
/*       Matrix geo(size,size);
       geo = 0.0;

	   for (i=0; i<size; i++){
	        geo[i][0]=coordx[i];
	        geo[i][1]=coordy[i];
	   }
 */

       std::vector< vector<double> > geo(size);
       for(i=0; i<size; i++)
	       geo[i].resize(2, 0.0);

       for (i=0; i<size; i++){
             geo[i][0]=coordx[i];
             geo[i][1]=coordy[i];
       }


/*       vector<char> geocode(size);
       for (i=0; i<size; i++)
            geocode[i] = objIds[i];
*/
	   // Executa o metodo MEET de Toshiro Tango 

	   if(convertcheckBox->isChecked())
	     {
	       aux = Meet3(montecarlo, pop1, cases1, geo, objIds, ZZ, pvalue, lambda_opt, Pmin, ss);
		 }
	   else
		 {
	       aux = Meet4(montecarlo, pop1, cases1, geo, objIds, ZZ, pvalue, lambda_opt, Pmin, ss);
		 }

       if (aux != 0)
		  {
	   	   QMessageBox::critical(this, tr("Error"), tr("Memory Error"));
		   return;
		  }
       vector<double> ZZZ(montecarlo+1);
       for (i=0; i<=montecarlo; i++) 
            ZZZ[i] = ZZ[i]; 


	   // Imprime os resultados na interface
	   saida = tr("Tango's MEET Test \n \n Number of regions: ") + str.setNum( size );
	   saida += tr("\n Number of Monte Carlo iterations: ") + str.setNum( montecarlo );
	   saida += tr("\n optimal lambda: ") + str.setNum( lambda_opt );
	   saida += "\n Pmin: " + str.setNum( Pmin );
	   saida += tr("\n pvalue: ") + str.setNum( pvalue );
	   saida += tr("\n \n Some percentiles for Z's distribution: \n \n percentile 0.5%: ") + str.setNum( quantile(ZZZ, 0.005) );
	   saida += tr("\n percentile 2.5%: ") + str.setNum( quantile(ZZZ, 0.025) );
	   saida += tr("\n percentile 5%: ") + str.setNum( quantile(ZZZ, 0.05) );
	   saida += tr("\n percentile 10%: ") + str.setNum( quantile(ZZZ, 0.1) );
	   saida += tr("\n percentile 90%: ") + str.setNum( quantile(ZZZ, 0.9) );
	   saida += tr("\n percentile 95%: ") + str.setNum( quantile(ZZZ, 0.95) );
	   saida += tr("\n percentile 97.5%: ") + str.setNum( quantile(ZZZ, 0.975) );
	   saida += tr("\n percentile 99.5%: ") + str.setNum( quantile(ZZZ, 0.995) );
       saida += "\n";

	   for(i=0;i<size;i++){
//	  	   saida += str.setNum( objIds[i] );
//		   saida += "   ";
		   saida += str.setNum( ss[i] );
           saida += "\n";
       }

	   Output_text->setText(saida);
	}
	return;
}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

///Metodo responsavel pelo preenchimento do combo de atributos das tabelas pertencentes
///a tabela especificado
void TangoPluginWindow::tableComboBoxClickSlot(const QString&)
{
/*	//selecionar o tema selecionado na combo
	string aux = string(themeComboBox->currentText().ascii());
	//Carrega a vista pelo Id carregado inicialmente
	TeView* view = localDb_->viewMap()[ViewId_];
	TeTheme* themeaux = view->get(aux);
	int themeId = themeaux->id();
	TeTheme* theme = localDb_->themeMap()[themeId]; 
*/	

	TeTheme* theme = plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1());

	if(!theme)
		return;

	//carregar os atributos (int ou double) da tabela selecionada
	vector<TeAttrDataType> attrType;
	attrType.push_back (TeINT);
	attrType.push_back (TeREAL);
	if(!fillColumnCombo(attrType, theme, popComboBox, tableComboBox))
	{
		//error message
		return;
	}

	if(!fillColumnCombo(attrType, theme, casesComboBox, tableComboBox))
	{
		//error message
		return;
	}

	if(!fillColumnCombo(attrType, theme, strataComboBox, tableComboBox))
	{
		//error message
		return;
	}

	return;
}
///Slot para fechar a interface
void TangoPluginWindow::CloseSlot()
{
	//if(localDb_)
	//	delete localDb_;
	QWidget::close();
}
///Ao ser inicializada a interface pelo menu do terraview, esse metodo e responsavel
///pela conexao do plugin com banco de dados e devidas inicializacoes que devam ser feitas.
void TangoPluginWindow::updateForm(PluginParameters* pluginParams)
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

	//carregar os temas da vista corrente e colocar o tema corrente como default

	/*	if(!fillThemeCombo(TePOLYGONS,localDb_->viewMap()[ViewId_], 
		themeComboBox, pluginParams->getCurrentThemeAppPtr()->name()))
	{
		//error message
		return;
	}                
*/

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

///Metodo responsavel por esconder a tela de interface do plugin
void TangoPluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}
