#include "UnionPluginMainWindow.h"

#include <TeQtViewsListView.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <TeQtLayerItem.h>

#include <TeGeoOpOverlayUnion.h>

#include <vector>
#include <qlineedit.h>
#include <qstring.h>
#include <qlistbox.h>
#include <qcombobox.h>
#include <qmessagebox.h>
#include <qcheckbox.h>
#include <qradiobutton.h>

UnionPluginMainWindow::UnionPluginMainWindow( PluginParameters* pp )
: UnionPluginMainForm( pp->qtmain_widget_ptr_, "UnionPluginMainWindow", 
  Qt::WType_TopLevel | Qt::WShowModal )
{
  plugin_params_ = pp;
  
  /* Inserting the tool button into TerraView toolbar */
  
  QPtrList< QToolBar > tv_tool_bars_list = 
    pp->qtmain_widget_ptr_->toolBars( Qt::DockTop );
    
  if( tv_tool_bars_list.count() > 0 ) {
    
    plugin_action_ptr_.reset( new QAction( tv_tool_bars_list.at( 0 ) ) );
    
    plugin_action_ptr_->setText( "Union Plugin" );
    plugin_action_ptr_->setMenuText( "Union Plugin" );
    plugin_action_ptr_->setToolTip( "Union Plugin" );
    plugin_action_ptr_->addTo( tv_tool_bars_list.at( 0 ) );
    
    connect( plugin_action_ptr_.nakedPointer(), SIGNAL( activated() ), 
      this, SLOT( showPluginWindow() ) );
  }

}


UnionPluginMainWindow::~UnionPluginMainWindow(){
}


void UnionPluginMainWindow::showPluginWindow(){
	
	if(plugin_params_->current_database_ptr_ == 0 || plugin_params_->teqtviewslistview_ptr_->currentViewItem() == 0){
		QString msg = tr("Você deve selecionar, antes, uma vista!") + "\n";
		msg += tr("Selecione uma vista e tente novamente.");
		QMessageBox::information(this, tr("Information"), msg);
		return;
	}

	
	if(plugin_params_->current_view_ptr_->size() == 0){
		QString msg = tr("A vista selecionada deve conter dois ou mais temas com representação vetorial!") + "\n";
		msg += tr("Adicione outros temas a vista e tente novamente.");
		QMessageBox::information(this, tr("Information"), msg);
		return;
	}

	cmbTemaA->clear();
	cmbTemaB->clear();
	nameNewLayer->clear();

	int qtdThemes = 0;
	TeQtViewItem* current_view_item = plugin_params_->teqtviewslistview_ptr_->currentViewItem();
	if( current_view_item != 0 ) {
		vector<QListViewItem*> view_items = current_view_item->getChildren();
		vector<QListViewItem*>::iterator it = view_items.begin();
		std::string theme_name( (*it)->text(0).ascii() );
		cmbTemaA->insertItem( QString( theme_name.c_str() ) );
		qtdThemes++;
		it++;
		while( it != view_items.end()) {
			qtdThemes++;
			std::string theme_name( (*it)->text(0).ascii() );
			cmbTemaA->insertItem( QString( theme_name.c_str() ) );
			cmbTemaB->insertItem( QString( theme_name.c_str() ) );
			++it;
		}
	}
	if(qtdThemes < 2){
		QString msg = tr("A vista selecionada deve conter dois ou mais temas com representação vetorial!") + "\n";
		msg += tr("Adicione outros temas a vista e tente novamente.");
		QMessageBox::information(this, tr("Information"), msg);
		return;
	}else show();
}

void UnionPluginMainWindow::execute(){	
	
	string layerName = nameNewLayer->text().latin1();
	if(layerName == ""){
		QString msg = tr("Digite o nome do plano de saída!");
		QMessageBox::information(this, tr("Information"), msg);
		return;
	}

	string errorMessage;
	bool changed;
	string newName = TeCheckName(layerName, changed, errorMessage); 
	if(changed){
		QString mess = tr("O nome do plano de saída é inválido: ") + errorMessage.c_str();
		mess += "\n" + tr("Verifique na interface uma sugestão para um novo nome válido.");
		QMessageBox::warning(this, tr("Warning"), mess);
		nameNewLayer->setText(newName.c_str());
		return;
	}

	layerName = newName;
	if (plugin_params_->current_database_ptr_->layerExist(layerName)){
		QString msg = tr("O nome do plano de saída já existe!") + "\n";
		msg += "Escolha outro nome e tente novamente.";
		QMessageBox::warning(this,tr("Warning"), msg);
		return;
	}



	QString nameThemeA = cmbTemaA->currentText();
	QString nameThemeB = cmbTemaB->currentText();
	
	TeTheme* themeA = plugin_params_->current_view_ptr_->get(nameThemeA.ascii());
	TeTheme* themeB = plugin_params_->current_view_ptr_->get(nameThemeB.ascii());

	QString nameLayer = nameNewLayer->text();
	 
	TeLayer* newLayer = new TeLayer(nameLayer.ascii(), 
									plugin_params_->current_database_ptr_,
									plugin_params_->current_view_ptr_->projection());

	TeSelectedObjects selOb = TeAll;
	if(radioApontadosA->isOn())
		selOb = TeSelectedByPointing;
	else if(radioConsultadosA->isOn())
		selOb = TeSelectedByQuery;

	TeSelectedObjects selObTrim = TeAll;
	if(radioApontadosB->isOn())
		selObTrim = TeSelectedByPointing;
	else if(radioConsultadosB->isOn())
		selObTrim = TeSelectedByQuery;


	bool result = TeGeoOpOverlayUnion(newLayer, themeA, themeB, selOb, selObTrim, 0);

	QString mess;
	if(result){
		mess = tr("Operação Union realizada com sucesso!\n");
		QMessageBox::information(this, tr("Information"), mess);
	}
	else{
		mess = tr("Falha ao realizar a operação Union!");
		QMessageBox::warning(this, tr("Warning"), mess);
		plugin_params_->current_database_ptr_->deleteLayer(newLayer->id());
		hide();
	}
	plugin_params_->updateTVInterface(*plugin_params_);

}

void UnionPluginMainWindow::cmbTemaA_activated(){
	cmbTemaB->clear();
	nameNewLayer->clear();
	QString s = cmbTemaA->currentText();
	TeQtViewItem* current_view_item = plugin_params_->teqtviewslistview_ptr_->currentViewItem();
	if( current_view_item != 0 ) {
		vector<QListViewItem*> view_items = current_view_item->getChildren();
		vector<QListViewItem*>::iterator it = view_items.begin();

		while( it != view_items.end()) {
			std::string theme_name( (*it)->text(0).ascii() );
			if( theme_name.c_str() != s)
				cmbTemaB->insertItem( QString( theme_name.c_str() ) );
			++it;
		}
	}
}

void UnionPluginMainWindow::closeEvent( QCloseEvent* e ){ 
	e->accept();
}