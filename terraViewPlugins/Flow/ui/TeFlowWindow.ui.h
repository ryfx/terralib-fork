/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <PluginBase.h>

#include <TeDatabase.h>
#include <TeAppTheme.h>

//QT include files
#include <qmainwindow.h>
#include <qmessagebox.h>
	
void TeFlowWindow::generatePushButton_clicked()
{
	if(!plug_pars_ptr_->getCurrentDatabasePtr())
	{
		QMessageBox::critical(plug_pars_ptr_->qtmain_widget_ptr_, tr("Error"), tr("There is no database connected!"));
		return;
	}

	if(!plug_pars_ptr_->getCurrentLayerPtr())
	{
		QMessageBox::warning(plug_pars_ptr_->qtmain_widget_ptr_, tr("Warning"),tr("Error, layer not found!"));
		return;
	}

	if(!plug_pars_ptr_->getCurrentThemeAppPtr())
	{
		QMessageBox::warning(plug_pars_ptr_->qtmain_widget_ptr_, tr("Warning"), tr("Error, theme not found!"));
		return;
	}

	generate_ = new FlowDiagramGeneration(this, "generateWindow", true);
	generate_->init(plug_pars_ptr_);
    generate_->exec();
	delete generate_;
}


void TeFlowWindow::networkPushButton_clicked()
{
	if(!plug_pars_ptr_->getCurrentDatabasePtr())
	{
		QMessageBox::warning(plug_pars_ptr_->qtmain_widget_ptr_, tr("Warning"), tr("Error, database not found!"));
		return;
	}

	if(!plug_pars_ptr_->getCurrentLayerPtr())
	{
		QMessageBox::warning(plug_pars_ptr_->qtmain_widget_ptr_, tr("Warning"),tr("Error, layer not found!"));
		return;
	}

	if(!plug_pars_ptr_->getCurrentThemeAppPtr())
	{
		QMessageBox::warning(plug_pars_ptr_->qtmain_widget_ptr_, tr("Warning"), tr("Error, theme not found!"));
		return;
	}

	classify_ = new FlowClassify(this, "classifyWindow", true);
	if(classify_->init(plug_pars_ptr_) == true)
	{
  		classify_->setLayout(true);
		classify_->exec();
	}
	delete classify_;
}


void TeFlowWindow::classifyPushButton_clicked()
{
	if(!plug_pars_ptr_->getCurrentDatabasePtr())
	{
		QMessageBox::warning(plug_pars_ptr_->qtmain_widget_ptr_, tr("Warning"), tr("Error, database not found!"));
		return;
	}

	if(!plug_pars_ptr_->getCurrentLayerPtr())
	{
		QMessageBox::warning(plug_pars_ptr_->qtmain_widget_ptr_, tr("Warning"),tr("Error, layer not found!"));
		return;
	}

	if(!plug_pars_ptr_->getCurrentThemeAppPtr())
	{
		QMessageBox::warning(plug_pars_ptr_->qtmain_widget_ptr_, tr("Warning"), tr("Error, theme not found!"));
		return;
	}

	classify_ = new FlowClassify(this, "classifyWindow", true);
	classify_->init(plug_pars_ptr_);
   	classify_->setLayout(false);
	classify_->exec();
	delete classify_;	
}

void TeFlowWindow::merPushButton_clicked()
{

}
	

void TeFlowWindow::init(PluginParameters * plug_pars_ptr)
{
    plug_pars_ptr_ = plug_pars_ptr;

	databaseLineEdit->setText("");
	layerLineEdit->setText("");
	themeLineEdit->setText("");

	databaseLineEdit->setText(plug_pars_ptr_->getCurrentDatabasePtr()->databaseName().c_str());
	layerLineEdit->setText(plug_pars_ptr_->getCurrentLayerPtr()->name().c_str());
	themeLineEdit->setText(plug_pars_ptr_->getCurrentThemeAppPtr()->getTheme()->name().c_str());
}


void TeFlowWindow::destroy()
{
	
}


bool TeFlowWindow::updateInterface( PluginParameters * plug_pars_ptr )
{
	plug_pars_ptr_ = plug_pars_ptr;

	classify_->updateInterface(plug_pars_ptr_);
	generate_->updateInterface(plug_pars_ptr_);
	
	return true;
}




