
#include <BesagPluginWindow.h>
#include <TeGUIUtils.h> 
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeDatabaseUtils.h>
#include <TeWaitCursor.h>
#include <TeSTEFunctionsDB.h>
#include <TeDataTypes.h>
#include <TeGeometryAlgorithms.h>
#include <TeSTElementSet.h>
#include <TeDatabaseFactory.h>
#include <TeDatabaseFactoryParams.h>
#include <TeAppTheme.h>


#undef min
#undef max

#include <besagfunc.h>
#include <vector>
#include <valarray>

#include <qpushbutton.h>
#include <qcheckbox.h> 
#include <qmessagebox.h>
#include <qbuttongroup.h> 
#include <qtabwidget.h>
#include <qvalidator.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qtextbrowser.h> 
#include <qspinbox.h> 

#include <help.h>

using namespace std;


BesagPluginWindow::BesagPluginWindow(QWidget* parent):
	besag(parent, "BesagPluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	localDb_ = false;
	control_ = false;

	pvaluelineEdit->setValidator(new QDoubleValidator(this));
	numCirclineEdit->setValidator(new QDoubleValidator(this));

	init();
}


BesagPluginWindow::~BesagPluginWindow()
{
	if(localDb_)
		delete localDb_;
}

/*Method: Validate the entry datas of the interface*/
void BesagPluginWindow::init()
{
 QDoubleValidator *v = new QDoubleValidator(this);
 v->setBottom(0.0);
 v->setTop(1.0);

 QIntValidator *vi = new QIntValidator(this);
 vi->setBottom(0);
 vi->setTop(11);

 pvaluelineEdit->setValidator(v);
 numCirclineEdit->setValidator(vi);
 
}


int BesagPluginWindow::temaComboBoxClickSlot(const QString&)
{
	//carregar todas as tabelas de atributos do tema selecionado
	if(!fillTableCombo(localDb_->viewMap()[ViewId_], tableComboBox, themeComboBox))
	{
		QMessageBox::critical( this, "TerraView", 
		tr( "Theme without attributes" ) ); 
		return(1);
	}

	//carregar todos os atributos da tabela do tableComboBox
	tableComboBoxClickSlot(tableComboBox->currentText());
	return(0);
}

void BesagPluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/
	Help help(this, "help", false);
	help.init("TerraClusterBesag.htm");

	if(!help.erro_)
		help.exec();

}

void 
BesagPluginWindow::scanButtonClickSlot()
{
	// Parametros para simulação
	int montecarlo, numCirc;					// Interaçoes de Monte Carlo
	double pvalor;
	QString str, saida = "";					// String auxiliar e de saida
	std::vector<double> pop;					// Vetor de populacao
	std::vector<double> cases;					// Vetor de casos
	std::vector<double> coordx;					// Coordenadas x
	std::vector<double> coordy;					// Coordenadas x
	std::vector<string> objIds;
	Output_text->clear();
	
    std::string layer_name = layerLineEdit->text().ascii();

	  //Testa se o Layer existe e o que fazer com ele
	  if( localDb_->layerExist( layer_name ) ) {
    
      int reposta = QMessageBox::question(this, "TerraView", 
        tr( "Do you want overright to the existing Layer?" ),tr("Yes"),tr("No"),0,0,1);    
	  //Se nao quiser sobrepor o Layer retorna
      if(reposta == 1)      
			return;    
	  //Caso contrario apaga o Layer e o sobrepoem
	  else{
		  int layerId = 0;
		  TeLayerMap::iterator it = localDb_->layerMap().begin();
		  TeLayerMap::iterator it_end = localDb_->layerMap().end();
		  while (it != it_end){
			  if(it->second->name() == layer_name){
					layerId = it->first;
					localDb_->deleteLayer(layerId);
					plugIngParams_.updateTVInterface(plugIngParams_); 
					break;
			  }
			++it;
		  }
	  }
  }

	TeWaitCursor wait;
	numCirc = numCirclineEdit->text().toInt();
	pvalor = pvaluelineEdit->text().toDouble();
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
	TeTheme* theme = plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1()); 
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
	}

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

    int tamanho = pop.size();
	int totcaso=0;

	vector< vector <double> > geo(tamanho), raios(tamanho);
	valarray<double> casos(tamanho), populacao(tamanho);

	for (int i=0;i<tamanho;i++){
		geo[i].resize(2);
		raios[i].resize(3);
		casos[i] = cases[i];
		populacao[i] = pop[i];
	}

	for (int i=0;i<(int)coordx.size();i++){
		geo[i][0]=coordx[i];
		geo[i][1]=coordy[i];
		totcaso+=cases[i];
	}
    
	// Executa o Método BESAGNEWELL
	if(!BesagNewell(tamanho,totcaso,geo,casos,populacao,pvalor,raios)){
			QMessageBox::critical(this, tr("Error"), tr("Besag&Newell Memory Error"));
			return;
		}
	
    //Imprime os resultados na interface
	saida = tr("Besag&Newell \n \n ");
	bool controle = false;
	vector<Elements> posicao(numCirc);
	for(int i=0;i<numCirc;i++){
		posicao[i].id = -1;
		posicao[i].dist = 999999999;
		posicao[i].pvalue = 999999999;
	}
	for(int i=0;i<tamanho;i++){
		if(raios[i][0] != 0){
			for(int j=0;j<numCirc;j++){
				if(posicao[j].id == -1){
					posicao[j].id = i;
					posicao[j].pvalue = raios[i][2];
					posicao[j].dist = raios[i][1];
				    break;
				}
				else if((j == (numCirc-1)) && (raios[i][2] < posicao[j].pvalue)){
					posicao[j].id = i;
					posicao[j].dist = raios[i][1];
					posicao[j].pvalue = raios[i][2];
				    break;
				}
			}
			sort(posicao.begin(),posicao.end());
            controle = true;
		}
	}
	if(!controle)
		saida += tr("No region has a high risk for p-value: ") + str.setNum(pvalor);
	else{
		/* Creating the new layer */
		TeLayer* newlayer = new TeLayer(layer_name, localDb_, localDb_->layerMap()[layer_]->projection());
     
		if( newlayer->id() <= 0 ) {
			QMessageBox::information( this, "TerraView",
			tr( "Unable to create the new layer" ) );
            return;
		}

		TePolygonSet ps;
		for(int i=0;i<numCirc;i++){
			if (posicao[i].id != -1){
				TePoint p(coordx[posicao[i].id],coordy[posicao[i].id]);
				TeLine2D circulo;
				TeGenerateCircle(p,posicao[i].dist,circulo,1000);
				TePolygon cir;
				cir.add(circulo);
				cir.objectId(Te2String(i));
				ps.add(cir);
			}
		}
		newlayer->addPolygons(ps);

		// create an attribute table
		TeAttributeList attList;
		TeAttribute at;
		at.rep_.type_ = TeSTRING;
		at.rep_.numChar_ = 16;
		at.rep_.name_ = "object_id";
		at.rep_.isPrimaryKey_ = true;
		attList.push_back(at);

		at.rep_.type_ = TeSTRING;
		at.rep_.numChar_ = 16;
		at.rep_.name_ = "src_object_id";
		at.rep_.isPrimaryKey_ = false;
		attList.push_back(at);

		// Create an attribute table associated to the new layer, called "TesteLayerAttr"
		TeTable attTable(layer_name+"Attr", attList, "object_id", "object_id");		// creates a table in memory
		if (!newlayer->createAttributeTable(attTable))
		{
			QMessageBox::information( this, "TerraView",
			tr( "Unable to create attribute table for the layer" ) );
            return;
		}

		TeTableRow row;
		for(int i=0;i<numCirc; ++i)
		{
			if(posicao[i].id != -1){
				row.push_back(Te2String(i));
				row.push_back(objIds[posicao[i].id]);
				attTable.add(row);
				row.clear();
			}
		}
		
		// Saves the table in the database
		if (!newlayer->saveAttributeTable( attTable ))
		{
			QMessageBox::information( this, "TerraView",
			tr( "Unable to safe in database the attribute table" ) );
            return;
		}
		
		TeTheme *newtheme = new TeTheme(layer_name,newlayer);
		plugIngParams_.getCurrentViewPtr()->add(newtheme);
		
		// Set a default visual for the geometries of the objects of the layer 
		// Polygons will be set with the blue color
		TeColor color;
		TeVisual polygonVisual(TePOLYGONS);
		color.init(0,0,0);
		polygonVisual.contourColor(color);
		polygonVisual.style(0);
    	newtheme->setVisualDefault(&polygonVisual, TePOLYGONS);

		// Set all of the geometrical representations to be visible
		int allRep = newlayer->geomRep();
		newtheme->visibleRep(allRep);
		newtheme->setAttTables(newlayer->attrTables());

		//Save the theme in the database
		if(!newtheme->save()){
			QMessageBox::information( this, "TerraView",
			tr( "Unable to create the new theme" ) );
            return;
		}

		if (!newtheme->buildCollection())
		{
			QMessageBox::information( this, "TerraView",
			tr( "Unable to create Collection" ) );
            return;
		}

		saida += tr("Circles: \n");
		for(int i=0;i<numCirc;i++){
			if(posicao[i].id != -1){
				saida += tr("ID Region centroid: ");
				saida += objIds[posicao[i].id].c_str();
				saida += tr("\nRadius: ") + str.setNum(posicao[i].dist);
				saida += tr("\nPvalue: ") + str.setNum(posicao[i].pvalue) + "\n \n";
			}
		}
	}

	Output_text->setText(saida);

	plugIngParams_.updateTVInterface(plugIngParams_); 
 		
	return;
}

void 
BesagPluginWindow::tableComboBoxClickSlot(const QString&)
{
	//selecionar o tema selecionado na combo
	TeTheme* theme = plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1()); 
	if(!theme)
		return;

	//carregar os atributos (int ou double) da tabela selecionada
	vector<TeAttrDataType> attrType;
	attrType.push_back (TeINT);
	attrType.push_back (TeREAL);
	if(!fillColumnCombo(attrType, theme, populationComboBox, tableComboBox))
	{
		QMessageBox::critical( this, "TerraView", 
		tr( "Unable to fill population ComboBox" ) );   
		return;
	}

	if(!fillColumnCombo(attrType, theme, caseComboBox, tableComboBox))
	{
		QMessageBox::critical( this, "TerraView", 
		tr( "Unable to fill theme case ComboBox" ) );   
		return;
	}

	return;
}
void BesagPluginWindow::CloseSlot()
{
	//if(localDb_)
	//	delete localDb_;
	QWidget::close();
}
//sera chamado toda vez que a form for chamada através do menu
void BesagPluginWindow::updateForm(PluginParameters* pluginParams)
{
	plugIngParams_ = (*pluginParams);
	
	//Inicializa a interface
	Output_text->clear();

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
		layer_ =  plugIngParams_.getCurrentLayerPtr()->id();
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
	if(!fillThemeCombo(TePOLYGONS,localDb_->viewMap()[ViewId_], 
		themeComboBox, pluginParams->getCurrentThemeAppPtr()->getTheme()->name()))
	{
		QMessageBox::critical( this, "TerraView", 
		tr( "Unable to fill theme ComboBox" ) );   
		return;
	}                

	//carregar todos os atributos da tabela do tableComboBox
	int controle = 0;
	controle = temaComboBoxClickSlot(QString(""));
	if (controle == 0)
	    show();
	return;
}

void 
BesagPluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}











	
