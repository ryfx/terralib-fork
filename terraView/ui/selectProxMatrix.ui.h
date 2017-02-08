/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright © 2001-2007 INPE and Tecgraf/PUC-Rio.
This file is part of TerraView. TerraView is free software; you can redistribute it 
and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

You should have received a copy of the GNU General Public License
along with TerraView.
The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The software provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use of
this program and its documentation.
*************************************************************************************/

#include "TeQuerierParams.h"
#include "TeDatabaseUtils.h"
#include <TeQtGrid.h>
#include <qmessagebox.h>
#include <display.h>
#include <qfiledialog.h>
#include <help.h>

void SelectProxMatrix::init()
{
	 help_ = 0;
	 //preencher a combo de temas da vista corrente
	 mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	 db_= mainWindow_->currentDatabase();
	 if(!db_)
		 return;
	 vector<TeGeomRep> reps;
	 reps.push_back (TePOLYGONS);
	 reps.push_back (TeLINES);
	 reps.push_back (TePOINTS);
	 reps.push_back (TeCELLS);
	 temaComboBox->clear();
	 string themeName = "";
	 if(mainWindow_->currentTheme())
		themeName = ((TeTheme*)mainWindow_->currentTheme()->getTheme())->name();
	 fillThemeCombo(reps, mainWindow_->currentView(), temaComboBox, themeName, true);
	
	 //preencher a combo de atributos
	 if(temaComboBox->count()<1)
	 {
		QMessageBox::warning(this, tr("Warning"),
		tr("There is no theme in the current view!"));
		CancelPushButton_ativated();
		 hide();
		 return;
	 }
	
	 string temaName =  string(temaComboBox->currentText().ascii());
	 if (fillProxMatrixCombo(temaName.c_str()))
		show();
}

 bool SelectProxMatrix::fillProxMatrixCombo(const QString& temaName)
 {
     curTheme_ = mainWindow_->currentView()->get(string(temaName.ascii()));
	 
     if(!curTheme_)
		return false;  //mensagem de erro!!!!
     
     comboIndex_.clear();
	 matrixComboBox->clear();
     
     string sql = " SELECT gpm_id, gpm_default, theme_id1, construction_strategy, ";
     sql += "  const_max_distance, const_num_neighbours FROM te_gpm ";
     sql += "  WHERE  theme_id1 = "+ Te2String(curTheme_->id());
     sql += "  ORDER BY construction_strategy ";
     
     TeDatabasePortal* portal = db_->getPortal();
     if(!portal)
		return false; 
     
     if((!portal->query(sql)) || (!portal->fetchRow()))
     {
		QMessageBox::warning(this, tr("Warning"),
		tr("There is no proximity matrix associated to the selected theme!"));
		delete portal;
		return false;
     }
    int index=0; 
	do
     {
			int gpmId = portal->getInt(0);
			int gpmDef = portal->getInt(1);
			int constStr = portal->getInt(3);
			double dist = portal->getDouble(4);
			int nn = portal->getInt(5);
			
			string matrix;
			if(constStr == 0)  //adjacencia
			{
				matrix = tr(" Contiguity ").latin1();
			
			}
			else if(constStr == 1)  //distancia
			{
				matrix = string(tr(" Distance   ->   ").ascii())+ Te2String(dist, 6) + string(tr("  units ").ascii());
			}
			else if(constStr == 2)  //nn
			{
				matrix = string(tr(" Nearest neighbours   ->   ").ascii())+ Te2String(nn) + string(tr("  neighbours ").ascii());
			}
			
			if(gpmDef )
				matrix += string(tr("    (default )   ").ascii());
			
			matrixComboBox->insertItem(matrix.c_str());  
			comboIndex_[index]=gpmId;
			++index;
       } while (portal->fetchRow());
    
       delete portal;
	   return true;
}
 
  void SelectProxMatrix::SetPushButton_ativated( )
  {
     
	 if(temaComboBox->count()<1)
	 {
		 QMessageBox::warning(this, tr("Warning"),
		 tr("There is no theme in the current view!"));
		 hide();
		 return;
	 }
	  
	 int index = matrixComboBox->currentItem();
     if(index<0)
	 {
		hide();
		return;
	 }

	 map<int, int>::iterator it = comboIndex_.find(index);
	 if(it==comboIndex_.end())
	 {
		hide();
		return;
	 }

	 int gpmId = it->second;
	       
      //setar todos como default
     string up =  " UPDATE te_gpm SET gpm_default = 0 WHERE theme_id1 = "+ Te2String(curTheme_ ->id());
     if(!db_->execute (up))
	 {
		hide();
		return;
	 }
      up =  " UPDATE te_gpm SET gpm_default = 1 WHERE gpm_id = "+ Te2String(gpmId);
      if(!db_->execute (up))
	  {
		  hide();
          return;
	  }
      
       hide();
	   return;
   }
  
  void SelectProxMatrix::DeletePushButton_ativated( )
  {
     if(temaComboBox->count()<1)
	 {
		 QMessageBox::warning(this, tr("Warning"),
		 tr("There is no theme in the current view!"));
		 hide();
		 return;
	 }

	 int index = matrixComboBox->currentItem();
     if(index<0)
	 {
		hide();
		return;
	 }

	 map<int, int>::iterator it = comboIndex_.find(index);
	 if(it==comboIndex_.end())
	 {
		 hide();
		 return;
	 }

	 int gpmId = it->second;
      
      if(!deleteGPMTable(db_, curTheme_->id(), gpmId))
	  {
		 QMessageBox::warning(this, tr("Warning"),
		 tr("Fail to delete the proximity matrix!"));
		 hide();
		 return;
	  }
      
     hide();
	 return;
   }
  
  void SelectProxMatrix::CancelPushButton_ativated( )
  {
      hide();
  }


void SelectProxMatrix::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("matriz_proximidade.htm");
	if(help_->erro_ == false)
	{
		help_->show();
		help_->raise();
		help_->scrollToAnchor("selecionar_matriz");
	}
	else
	{
		delete help_;
		help_ = 0;
	}

}
