#include "DisolvePluginMainWindow.h"

#include <TeQtViewsListView.h>
#include <TeQtViewItem.h>
#include <TeQtThemeItem.h>
#include <TeImportRaster.h>
#include <TeQtDatabaseItem.h>

#include <TeDisolve.h>

#include <TeDatabaseFactory.h>

#include <qlineedit.h>
#include <qstring.h>
#include <qlistbox.h>
#include <qcombobox.h>
#include <qmessagebox.h>
#include <qcheckbox.h>
#include <qradiobutton.h>

#include <float.h>

DisolvePluginMainWindow::DisolvePluginMainWindow( PluginParameters* pp )
: DisolvePluginMainForm( pp->qtmain_widget_ptr_, "DisolvePluginMainWindow", 
  Qt::WType_TopLevel | Qt::WShowModal )
{
  plugin_params_ = pp;
    
  /* Inserting the tool button into TerraView toolbar */
  
  QPtrList< QToolBar > tv_tool_bars_list = 
    pp->qtmain_widget_ptr_->toolBars( Qt::DockTop );
    
  if( tv_tool_bars_list.count() > 0 ) {
    
    plugin_action_ptr_.reset( new QAction( tv_tool_bars_list.at( 0 ) ) );
    
    plugin_action_ptr_->setText( "Disolve Plugin" );
    plugin_action_ptr_->setMenuText( "Disolve Plugin" );
    plugin_action_ptr_->setToolTip( "Disolve Plugin" );
    plugin_action_ptr_->setIconSet( QIconSet( QPixmap::fromMimeSource( 
      "DisolvePluginIcon.bmp" ) ) );
    
    plugin_action_ptr_->addTo( tv_tool_bars_list.at( 0 ) );
    
    connect( plugin_action_ptr_.nakedPointer(), SIGNAL( activated() ), 
      this, SLOT( showPluginWindow() ) );
  }     
}


DisolvePluginMainWindow::~DisolvePluginMainWindow(){
}


void DisolvePluginMainWindow::showPluginWindow(){
	if(plugin_params_->current_database_ptr_ == 0 || plugin_params_->teqtviewslistview_ptr_->currentViewItem() == 0){
		QString msg = tr("Você deve selecionar, antes, uma vista!") + "\n";
		msg += tr("Selecione uma vista e tente novamente.");
		QMessageBox::information(this, tr("Information"), msg);
		return;
	}

	if(plugin_params_->current_view_ptr_->size() == 0){
		QString msg = tr("A vista selecionada deve conter pelo menos um tema com representação vetorial!") + "\n";
		msg += tr("Adicione um tema a vista e tente novamente.");
		QMessageBox::information(this, tr("Information"), msg);
		return;
	}

	cmbTema->clear();
	nameNewLayer->clear();

	TeQtViewItem* current_view_item = plugin_params_->teqtviewslistview_ptr_->currentViewItem();
	if( current_view_item != 0 ) {
		vector<QListViewItem*> view_items = current_view_item->getChildren();
		vector<QListViewItem*>::iterator it = view_items.begin();
		while( it != view_items.end()) {
			std::string theme_name( (*it)->text(0).ascii() );
			cmbTema->insertItem( QString( theme_name.c_str() ) );
			++it;
		}
	}
	show();
}

void DisolvePluginMainWindow::execute(){	
	
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

	QString nameTheme = cmbTema->currentText();
	TeTheme* theme = plugin_params_->current_view_ptr_->get(nameTheme.ascii());

	QString nameLayer = nameNewLayer->text();
	 
	TeLayer* newLayer = new TeLayer(nameLayer.ascii(), 
									plugin_params_->current_database_ptr_,
									plugin_params_->current_view_ptr_->projection());

	
	TeSelectedObjects selObj = TeAll;
	if(radioApontados->isOn())
		selObj = TeSelectedByPointing;
	else if(radioConsultados->isOn())
		selObj = TeSelectedByQuery;

	bool result = TeOpDisolve(newLayer, theme, selObj);

	QString mess;
	if(result){
		mess = tr("Operação Disolve realizada com sucesso!");
		QMessageBox::information(this, tr("Information"), mess);
	}
	else{
		mess = tr("Falha ao realizar a operação Disolve!");
		QMessageBox::warning(this, tr("Warning"), mess);
		plugin_params_->current_database_ptr_->deleteLayer(newLayer->id());
		hide();
	}
	plugin_params_->updateTVInterface(*plugin_params_);

}