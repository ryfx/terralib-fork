#include <math.h>
#include <valarray>
#include <list>

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

#include <SRPluginWindow.h>
#include <curve.h>
#include <sv.h>

#include <TeGUIUtils.h> 
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeDatabaseUtils.h>
#include <TeWaitCursor.h>
#include <TeSTEFunctionsDB.h>
#include <TeDataTypes.h>
#include <TerraViewBase.h>

#include <TeSTElementSet.h>
#include <TeDatabaseFactory.h>
#include <TeDatabaseFactoryParams.h>
#include <TeAppTheme.h>
#include <TeGeometryAlgorithms.h>
#include <TeQtThemeItem.h>
#include <TeQtViewsListView.h>
#include <TeAppTheme.h>
#include <TeQtViewItem.h>

#include <help.h>

using namespace std;

SRPluginWindow::SRPluginWindow(QWidget* parent):
	sr(parent, "SRPluginWindow", Qt::WType_TopLevel | Qt::WShowModal )
{
	RadiuslineEdit->setValidator(new QDoubleValidator(this));
	EpsilonlineEdit->setValidator(new QDoubleValidator(this));
	TresholdlineEdit->setValidator(new QDoubleValidator(this));

	localDb_ = 0;
	control_ = false;
	gra_ = 0;
	ncols_ = 0;

	_tvBase = dynamic_cast<TerraViewBase*>(parent);

	init();
}


SRPluginWindow::~SRPluginWindow()
{
	if(localDb_)
		delete localDb_;
}

/*Method: Validate the entry datas of the interface*/
void SRPluginWindow::init()
{
 QDoubleValidator *v = new QDoubleValidator(this);
 v->setBottom(0.000000001);

 RadiuslineEdit->setValidator(v);
 EpsilonlineEdit->setValidator(v);
 TresholdlineEdit->setValidator(v);
 
}
void SRPluginWindow::temaComboBoxClickSlot(const QString&)
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

void SRPluginWindow::Enable_timeSlot()
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

void SRPluginWindow::CloseSlot()
{
	//if(localDb_)
	//	delete localDb_;
	//localDb_ = 0;
	QWidget::close();
}

void SRPluginWindow::helpButtonSlot()
{
	/*if(1==1)
	{
	QMessageBox::critical(this, tr("Alert"), tr("The explanatory manual can be accessed at C:/Arquivos de programas/terraview3.2.0"));
      return;
	}*/
	
	Help help(this, "help", false);
	help.init("TerraClusterSR.htm");

	if(!help.erro_)
		help.exec();

}

void SRPluginWindow::okButtonClickSlot()
{
	// Parametros para simulação
	unsigned int i;
	QString str, saida = "";			
	std::vector<double> coordx;			// Coordenadas x
	std::vector<double> coordy;			// Coordenadas y
	std::vector<double> coordt;			// Coordenadas t
	std::vector<string> objIds;
	double raio, eps, limiar;
	vector<string> attrs;

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
					break;
				}
				++it;
			}
		}
	}
	
	TeWaitCursor wait;
	//Carrega as variavies da interface
	raio = RadiuslineEdit->text().toDouble();
	eps =  EpsilonlineEdit->text().toDouble();
	limiar =  TresholdlineEdit->text().toDouble();

	//create the vector of possible colors for the graphic
	if( _colors.empty() ) {
		_colors.push_back(Qt::black);
		_colors.push_back(Qt::red);
		/*_colors.push_back(Qt::green);
		_colors.push_back(Qt::cyan);
		_colors.push_back(Qt::magenta);
		_colors.push_back(Qt::yellow);
		_colors.push_back(Qt::gray);*/
	}
	gra_= 0;

    //----------------- pegar o tema, a tabela e os atributos selecionados na tela
	//Carrega o tema com o primeiro conjunto de pontos
	TeTheme* theme = plugIngParams_.getCurrentViewPtr()->get(themeComboBox->currentText().latin1()); 
	
	if(!theme){
		QMessageBox::critical(this, tr("Error"), tr("Not possible to load the Theme"));
		return;
	}
	Output_text->setText(tr(" Surveillance System \n running..."));

	TeQuerierParams querierParams(true,false);
	querierParams.setParams(theme);
	//querierParams.setParams(theme, TeMONTH);
	TeQuerier querier(querierParams);
    if(timecheckBox->isChecked()){
		//Carrega a tabela de atributos
		string tabelaName = string(tableComboBox->currentText().ascii());
		if(tabelaName.empty()){
				QMessageBox::critical(this, tr("Error"), tr("Theme without Table"));
				return;
		}

		//Carrega os atributos
		string TimeAttr = string(timeComboBox->currentText().ascii());
		if(TimeAttr.empty()){
			QMessageBox::critical(this, tr("Error"), tr("Empty data"));
			return;
		}
			
		//Carrega os atributos para memoria
		attrs.push_back (tabelaName+"."+TimeAttr);
		
		//----------------- pegar do banco os atributos e geometrias que serão utilizados na funcao
        querierParams.setParams(theme);
		querierParams.setFillParams(true,false,attrs);
		querier.refresh(querierParams);

		if(!querier.loadInstances()){
			QMessageBox::critical(this, tr("Error"), tr("Not possible to load data"));
			return;
		}

		TeSTInstance obj;
		TeSTElementSet objSet(theme);
		while(querier.fetchInstance(obj)){
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
		if(objIds.size() != coordx.size()){
			QMessageBox::critical(this, tr("Error"), tr("Not all of the points have (x,y) coordinates"));
			return;
		}
		if(coordx.size() != coordt.size()){
			QMessageBox::critical(this, tr("Error"), tr("Not all of the points have time coordinate"));
			return;
		}
	}
	else{
		//Escolhe o chrono de acordo com o selecionado na interface
		TeChronon Chrono;
		int intchrono = timeUnitComboBox->currentItem();
		switch (intchrono){
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

		int numTimeFrames = querier.getNumTimeFrames();
		if (numTimeFrames == 0){
			QMessageBox::critical(this, tr("Error"), tr("The Table doesn't have time specification"));
			return;
		}
   	    // -------- Para cada frame retornar todos os eventos temporais que estão nesse frame
		TeSTElementSet objSet(theme);
		for(int frame = 0; frame < numTimeFrames; ++frame){
			if(querier.loadInstances(frame)){
				// para cada evento ou instancia temporal
				TeSTInstance obj;
				while(querier.fetchInstance(obj)){
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
		if(objIds.size() != coordx.size()){
			QMessageBox::critical(this, tr("Error"), tr("Not all of the points have (x,y) coordinates"));
			return;
		}
		if(coordx.size() != coordt.size()){
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
	
	std::valarray<double> R;

	SistemadeVigilancia(ev,raio,eps,R);
	bool controle = false;
	for(i=0;i<R.size();i++){
		if(R[i]>limiar){
			controle = true;
			break;
		}
	}

	unsigned int cont = 0;
	SVEventLst::iterator it = ev.begin();
	while((cont < i) &&  (it != ev.end())){
		++it;
		++cont;
	}
	
	unsigned int num = cont;	
	CalculaLambda(ev,raio,eps,R,num);
	SVEventLst::iterator it2 = ev.begin();
	unsigned int h=0;
	while((h < num) &&  (it2 != ev.end())){
		++it2;
		++h;
	}

	
	Elements posicao;
	posicao.x = -1;
	posicao.y = -1;
	posicao.dist = raio;

	if(controle){
		posicao.x = (*it2).x;
		posicao.y = (*it2).y;
		posicao.id = (*it2).t;

		/* Creating the new layer */
		TeLayer* newlayer = new TeLayer(layer_name, localDb_, localDb_->layerMap()[layer_]->projection());

		if( newlayer->id() <= 0 ) {
			QMessageBox::information( this, "TerraView",
			tr( "Unable to create the new layer" ) );
            return;
		}

		TePolygonSet ps;
		if ((posicao.x != -1) && (posicao.y != -1)){
			TePoint p(posicao.x,posicao.y);
			TeLine2D circulo;
			TeGenerateCircle(p,posicao.dist,circulo,1000);
			TePolygon cir;
			cir.add(circulo);
			cir.objectId(Te2String(1));
			ps.add(cir);
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
		if(posicao.id != -1){
			row.push_back(Te2String(1));
			row.push_back(objIds[posicao.id]);
			attTable.add(row);
			row.clear();
		}
		
		
		// Saves the table in the database
		if (!newlayer->saveAttributeTable( attTable )){
			QMessageBox::information( this, "TerraView",
			tr( "Unable to safe in database the attribute table" ) );
            return;
		}
		
		TeTheme *newtheme = new TeTheme(layer_name,newlayer);
		newtheme->view(plugIngParams_.getCurrentViewPtr()->id());		
		
		
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
		
		_tvBase->popupDatabaseRefreshSlot();

		//Imprime os resultados na interface
		saida += tr("Surveillance System \n \n Ring the alarm:");
		saida += "\nX: " + str.setNum( (*it).x ) + "   Y: " + str.setNum( (*it).y ) + "   T: " + str.setNum( (*it).t );
		saida += tr("\n \n Center of Cluster:");
		saida += "\nX: " + str.setNum( (*it2).x ) + "   Y: " + str.setNum( (*it2).y ) + "   T: " + str.setNum( (*it2).t );
		//saida += "\n X: " + str.setNum( coordx[i] ) + " Y: " + str.setNum( coordy[i] ) + " T: " + str.setNum( coordt[i]);
	}
	else
	{
		saida = tr("Surveillance System \n \n Event that Ring the alarm:");
		saida += tr("\n No events ring the alarm");
	}
	Output_text->setText(saida);
	
	if( gra_ == NULL ) {
	gra_ = new XYPlotGraphic(this, "SR");
	gra_->plot->setTitleXAxis("Event Number");
	}
	gra_->plot->setTitleYAxis("R");
	
	//set the curves that will be printed
	std::valarray<double> treshold(ev.size()),scale(ev.size());
	for(i=0;i<treshold.size();++i){
		treshold[i] = limiar;
		scale[i] =  i+1;
	}
	Curve *crv = new Curve(R,scale);
	Curve *lim = new Curve(treshold,scale);
	
	//choose the colors of the curves
	crv->setColor(_colors[0]);
	lim->setColor(_colors[1]);
	if( (ncols_%7) == 0 ) ncols_ = 0;
	//plot the curves in the graphic window
	gra_->plot->append(crv);
	gra_->plot->append(lim);
	gra_->show();

	//plugIngParams_.updateTVInterface(plugIngParams_); 

	return;
}

void SRPluginWindow::tableComboBoxClickSlot(const QString&)
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
void SRPluginWindow::updateForm(PluginParameters* pluginParams)
{
	plugIngParams_ = (*pluginParams);
	
	//Inicializa a interface
	Output_text->clear();

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

void SRPluginWindow::hideEvent(QHideEvent* )
{
	if(localDb_)
		delete localDb_;
	localDb_ = 0;
}











	
