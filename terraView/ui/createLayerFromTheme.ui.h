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

#include "TeDatabase.h"
#include "TeAppTheme.h"
#include "TeLayerFunctions.h"
#include "TeExternalTheme.h"
#include "projection.h"
#include <qapplication.h>
#include <qmessagebox.h>
#include <qbutton.h>
#include <qstring.h>
//#include <TeDatabaseUtils.h>

#include <TeThemeFunctions.h>

void CreateLayerFromTheme::init(TeQtThemeItem* themeItem)
{
	help_ = 0;
	theme_ = themeItem->getAppTheme();
	if (!theme_)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Select a theme!"));
		return;
	}
	TeLayer* layer = 0;
	if(((TeTheme*)theme_->getTheme())->getProductId() == TeTHEME)
		layer = ((TeTheme*)theme_->getTheme())->layer();
	else if(((TeTheme*)theme_->getTheme())->getProductId() == TeEXTERNALTHEME)
		layer = ((TeExternalTheme*)(theme_->getTheme()))->getRemoteTheme()->layer();
	else
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		reject();
		return;
	}

	if (!layer)
	{
		QMessageBox::information(this, tr("Information"), tr("Can not access the layer associated to the theme."));	
		reject();
		return;
	}

	newLayer_ = 0;
	proj_ = 0;
	projNameLineEdit->clear();
	viewProjRadioButton->setChecked(true);
	string newLayerName = ((TeTheme*)theme_->getTheme())->name();
	int n = 1;
	while (true)
	{
		// Check whether there is a layer with this name in the database
		bool flag = false;
		TeLayerMap& layerMap = db_->layerMap();
		TeLayerMap::iterator it;
		for (it = layerMap.begin(); it != layerMap.end(); ++it)
		{
			if (TeStringCompare(it->second->name(),newLayerName))
			{
				flag = true;
				break;
			}
		}
		if (flag)
		{
			newLayerName = ((TeTheme*)theme_->getTheme())->name() + "_" + Te2String(n);
			n++;
		}
		else
			break;
	}
	layerNameLineEdit->setText(newLayerName.c_str());
	
	if (layer->projection()->name() == "NoProjection")
	{
		proj_ = new TeNoProjection();
		projButtonGroup->setEnabled(false);
		layerProjRadioButton->setChecked(true);
		string pname = proj_->name() + "/" + proj_->datum().name();
		projNameLineEdit->setText(pname.c_str());
		projPushButton->setEnabled(false);
	}
	else
	{
		projButtonGroup->setEnabled(true);
		viewProjRadioButton->setChecked(true);
		projButtonGroup_clicked(0);
		projPushButton->setEnabled(false);
		projPushButton->setEnabled(true);
	}
	TeGeomRep rep = static_cast<TeGeomRep>(layer->geomRep());
	if (!(rep & TePOLYGONS || rep & TeLINES || rep & TePOINTS || rep & TeCELLS))
		objectsButtonGroup->setEnabled(false);
	else
		objectsButtonGroup->setEnabled(true);
	objectsButtonGroup->setButton(0);
}


void CreateLayerFromTheme::projButtonGroup_clicked( int i )
{
	if (!theme_)
		return;
	if (proj_)
	{
		delete proj_;
		proj_ = 0;
	}

	TeLayer* layer;
	if(((TeTheme*)theme_->getTheme())->getProductId() == TeTHEME)
		layer = ((TeTheme*)theme_->getTheme())->layer();
	else if(((TeTheme*)theme_->getTheme())->getProductId() == TeEXTERNALTHEME)
		layer = ((TeExternalTheme*)(theme_->getTheme()))->getRemoteTheme()->layer();

	if (i==0)  // view projection
	{
		int vid = ((TeTheme*)theme_->getTheme())->view();
		TeView* view_ = 0;
		TeViewMap& viewMap = db_->viewMap();
		TeViewMap::iterator it;
		for (it = viewMap.begin(); it != viewMap.end(); ++it)
		{
			if (it->second->id() == vid)
			{
				view_ = it->second;
				break;
			}
		}
		proj_ = TeProjectionFactory::make(view_->projection()->params());
	}
	else if (i==1)	// layer projection
	{
		proj_ = TeProjectionFactory::make(layer->projection()->params());
	}
	else
	{
		proj_ = TeProjectionFactory::make(layer->projection()->params());
		projPushButton_clicked();
	}
	string pname = proj_->name() + "/" + proj_->datum().name();
	projNameLineEdit->setText(pname.c_str());
}


void CreateLayerFromTheme::projPushButton_clicked()
{
	ProjectionWindow *projectionWindow = new ProjectionWindow(this, "projection", true);
	if (proj_)
		projectionWindow->loadProjection(proj_);
	if (projectionWindow->exec() == QDialog::Accepted)
	{
		if (proj_)
			delete proj_;
		proj_ = projectionWindow->getProjection();
		string pname = proj_->name() + "/" + proj_->datum().name();
		projNameLineEdit->setText(pname.c_str());
	}
	delete projectionWindow;
}


TeLayer* CreateLayerFromTheme::getLayer()
{
    return newLayer_;
}


void CreateLayerFromTheme::okPushButton_clicked()
{
	if (!theme_)
		return;

	TeTheme* theme = (TeTheme*)theme_->getTheme();
	// Creating a new layer
	// Check layer name validity
	string layerName = layerNameLineEdit->text().latin1();
	if(layerName.empty())
	{
		QMessageBox::warning(this,tr("Warning"),
		tr("Enter output layer name and try again!"));
		return;
	}
	string errorMessage;
	bool changed;
	string newName = TeCheckName(layerName, changed, errorMessage); 
	if(changed)
	{
		QString mess = tr("The output layer name is invalid: ") + errorMessage.c_str();
		mess += "\n" + tr("Change current name and try again.");
		QMessageBox::warning(this, tr("Warning"), mess);
		layerNameLineEdit->setText(newName.c_str());
		return;
	}
	layerName = newName;
	if (db_->layerExist(layerName))
	{
		QMessageBox::warning(this,tr("Warning"),
		tr("Output layer name already exist, replace and try again!"));
		return;
	}

	int selobj = objectsButtonGroup->selectedId();
	if(((TeTheme*)theme_->getTheme())->getProductId() == TeTHEME)
	{
		if (!(theme->layer()->hasGeometry(TeRASTER) || theme->layer()->hasGeometry(TeRASTERFILE)))
		{
			// First check if selection will result in an object
			string input; 
			if(selobj == 1) // queried
			{
				input = "(" + theme->collectionTable() + ".c_object_status = 2";
				input += " OR " + theme->collectionTable() + ".c_object_status = 3)";
			}
			else if(selobj == 2) // pointed
			{
				input = "(" + theme->collectionTable() + ".c_object_status = 1";
				input += " OR " + theme->collectionTable() + ".c_object_status = 3)";
			}
			else if(selobj == 3) // not queried
			{
				input = "(" + theme->collectionTable() + ".c_object_status <> 2";
				input += " AND " + theme->collectionTable() + ".c_object_status <> 3)";
			}
			else if(selobj == 4) // not pointed
			{
				input = "(" + theme->collectionTable() + ".c_object_status <> 1";
				input += " AND " + theme->collectionTable() + ".c_object_status <> 3)";
			}
			else if(selobj == 5) // pointed and queried
			{
				input = "(" + theme->collectionTable() + ".c_object_status = 3)";
			}
			else if(selobj == 6) // pointed or queried
			{
				input = "(" + theme->collectionTable() + ".c_object_status = 2";
				input += " OR " + theme->collectionTable() + ".c_object_status = 1";
				input += " OR " + theme->collectionTable() + ".c_object_status = 3)";
			}

			// Initally test if there is any object selected
		
			string sql = "SELECT c_object_id FROM " + theme->collectionTable();
			if (!input.empty())
				sql += " WHERE " + input;
			TeDatabasePortal* portal = db_->getPortal();
			if (!portal->query(sql))
			{
				delete portal;
				QString msg = tr("Fail to create a layer from the theme") + " \"" + theme->name().c_str() + "\" !";
				QMessageBox::critical(this, tr("Error"), msg);
				reject();
				return;
			}
			if (!portal->fetchRow())
			{
				delete portal;
				QButton* selB = objectsButtonGroup->selected();
				QString msg = tr("No objects will be selected:") + " " + selB->text();
				QMessageBox::warning(this, tr("Warning"), msg);
				objectsButtonGroup->setButton(0);
				return;
			}
			delete portal;
		}
	}
	else if(((TeTheme*)theme_->getTheme())->getProductId() == TeEXTERNALTHEME)
	{
		if (!(theme->layer()->hasGeometry(TeRASTER) || theme->layer()->hasGeometry(TeRASTERFILE)))
		{
			// First check if selection will result in an object
			string input; 
			TeTheme* tm = (TeTheme*)theme_->getTheme();

			int sel;
			if(selobj == 1) // queried
				sel = TeSelectedByQuery;
			else if(selobj == 2) // pointed
				sel = TeSelectedByPointing;
			else if(selobj == 3) // not queried
				sel = TeNotSelectedByQuery;
			else if(selobj == 4) // not pointed
				sel = TeNotSelectedByPointing;
			else if(selobj == 5) // pointed and queried
				sel = TeSelectedByPointingAndQuery;
			else if(selobj == 6) // pointed or queried
				sel = TeSelectedByPointingOrQuery;

			vector<string> itemVec = getItems(tm, sel);
			if(itemVec.empty())
			{
				QButton* selB = objectsButtonGroup->selected();
				QString msg = tr("No objects will be selected:") + " " + selB->text();
				QMessageBox::warning(this, tr("Warning"), msg);
				objectsButtonGroup->setButton(0);
				return;
			}
		}
	}

	newLayer_ = new TeLayer(layerName,db_,proj_);
	if (newLayer_->id() <= 0)
	{
		QString msg = tr("Fail to create a layer from the theme") + " \"" + theme->name().c_str() + "\" !";
		QMessageBox::critical(this, tr("Error"), msg);
		return;
	}
	map<string,string> txttabs;
	bool res;

	if (theme_->getTheme()->type() == TeEXTERNALTHEME)
		res = TeCopyExternThemeToLayer((TeTheme*)theme_->getTheme(), newLayer_, selobj, &txttabs);
	else if (theme_->getTheme()->type() == TeTHEME)
	{
		res = TeCopyThemeToLayer((TeTheme*)theme_->getTheme(), newLayer_, selobj, &txttabs);
		if (txttabs.size() > 0)		// copy the extensions to text tables
		{
			string sql;
			string ttable;
			TeDatabasePortal* portal = db_->getPortal();
			map<string,string>::iterator it = txttabs.begin();
			while (it != txttabs.end())
			{
				ttable = (*it).first;
				sql = "SELECT * FROM " + (*it).first + "_txvisual";
				if (portal->query(sql)) 
				{
					string tableName = (*it).second+"_txvisual";
					TeAttributeList& attrList = portal->getAttributeList();
					if (!db_->createTable(tableName, attrList))
						continue;
					string fields = "geom_id, dot_height, fix_size, color, family, bold, italic ";
					string selg = "SELECT " + (*it).first+ "." + fields + "FROM ";
					selg += (*it).first + "," + (*it).first + "_txvisual";
					selg += " WHERE " + (*it).first + "_txvisual.geom_id = " + (*it).first + ".geom_id";
					string popule = "INSERT INTO " + (*it).second+"_txvisual" + " ( " + fields + ") " + selg;
					db_->execute(popule);
				}
				portal->freeResult();
				++it;
			}
			delete portal;
			txttabs.clear();
		}
	}
	if (!res)
	{
		db_->deleteLayer(newLayer_->id());
		QString msg = tr("Fail to create a layer from the theme") + " \"" + theme->name().c_str() + "\" !";
		QMessageBox::critical(this, tr("Error"), msg);
		reject();
	}
	else
		accept();
}


void CreateLayerFromTheme::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("createLayerFromTheme.htm");
	if(help_->erro_ == false)
	{
		help_->show();
		help_->raise();
	}
	else
	{
		delete help_;
		help_ = 0;
	}
}


void CreateLayerFromTheme::reject()
{
	hide();
}


void CreateLayerFromTheme::setParams( TeDatabase * db )
{
	db_ = db;
}
