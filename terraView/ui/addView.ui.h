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

#include <qmessagebox.h>
#include <TeQtViewItem.h>
#include <TeDatabase.h>


void AddView::init()
{
	help_ = 0;
	mainWindow_= (TerraViewBase*)qApp->mainWidget();
	currentDatabaseLineEdit->setText(mainWindow_->currentDatabase()->databaseName().c_str());
}


void AddView::okPushButton_clicked()
{
	TeDatabase *currentDatabase = mainWindow_->currentDatabase();

	string viewName = viewNameLineEdit->text().latin1();
	if (viewName.empty() == true)
	{
		QMessageBox::warning(this,
		tr("Warning"), tr("Give the name of the new view!"));
		return;
	}

	TeViewMap& viewMap = currentDatabase->viewMap();
	TeViewMap::iterator it;
	for (it = viewMap.begin(); it != viewMap.end(); ++it)
	{
		if (it->second->name() == viewName)
		{
			QMessageBox::warning(this,
			tr("Warning"),
			tr("There is already a view with this name in the database!"));
			return;
		}
	}

	//Add a new view to the database and insert it on the map of Views
	TeView *view = new TeView (viewName, currentDatabase->user());  
	new TeQtViewItem((QListView*)(mainWindow_->getViewsListView()), view->name().c_str(), view);

	//Generate a dummy LatLong projection
	TeDatum dSAD69 = TeDatumFactory::make("SAD69");
	TeLatLong *projection = new TeLatLong(dSAD69);
	view->projection(projection);

	TeBox b;
	view->setCurrentBox(b);
	view->setCurrentTheme(-1);
	currentDatabase->insertView(view);
	TeProject* tvProj=0;
	if(currentDatabase->projectMap().empty() == false)
	{
		TeProjectMap& pm = currentDatabase->projectMap();
		TeProjectMap::iterator it = pm.begin();
		while (it != pm.end())
		{
			if (it->second->name() == "TV_Project")
			{
				tvProj = it->second;
				tvProj->addView(view->id());
				break;
			}
			++it;
		}
	}
	if (tvProj)
		currentDatabase->insertProjectViewRel(tvProj->id(), view->id());
	mainWindow_->checkWidgetEnabling();
	hide();
}


void AddView::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("addView.htm");
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


