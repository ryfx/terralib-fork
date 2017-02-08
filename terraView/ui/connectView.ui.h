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

#include <qapplication.h>
#include <terraViewBase.h>
#include <display.h>


void ConnectView::init()
{
	help_ = 0;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	displayWindow_ = mainWindow_->getDisplayWindow();
	view_ = mainWindow_->currentView();
	db_ = mainWindow_->currentDatabase();
	int	viewId = view_->id();

	viewComboBox->clear();
	viewComboBox->setEditable(false);

	int item = 0;
	int connectedId = view_->connectedId();
	if(connectedId > 0)
	{
		TeViewMap& viewMap = db_->viewMap();
		TeViewMap::iterator it = viewMap.begin();
		while(it != viewMap.end())
		{
			if(it->second->id() != viewId)
				viewComboBox->insertItem(it->second->name().c_str());
			it++;
		}

		it = viewMap.begin();
		while(it != viewMap.end())
		{
			if(it->second->id() != viewId)
			{
				if(it->second->id() == connectedId)
				{
					viewComboBox->setCurrentItem(item);
					break;
				}
				item++;
			}
			it++;
		}
		connectCheckBox->setChecked(true);
		disconnectCheckBox->setChecked(false);
	}
	else
	{
		connectCheckBox->setChecked(false);
		disconnectCheckBox->setChecked(true);
	}
}



void ConnectView::connectCheckBox_clicked()
{
	int	viewId = view_->id();
	int	connectedId = view_->connectedId();

	viewComboBox->clear();

	TeViewMap& viewMap = db_->viewMap();
	TeViewMap::iterator it = viewMap.begin();
	while(it != viewMap.end())
	{
		if(it->second->id() != viewId)
			viewComboBox->insertItem(it->second->name().c_str());
		it++;
	}
	if(connectedId > 0)
	{
		int item = 0;
		it = viewMap.begin();
		while(it != viewMap.end())
		{
			if(it->second->id() != viewId)
			{
				if(it->second->id() == connectedId)
				{
					viewComboBox->setCurrentItem(item);
					break;
				}
				item++;
			}
			it++;
		}
	}
	connectCheckBox->setChecked(true);
	disconnectCheckBox->setChecked(false);
}


void ConnectView::disconnectCheckBox_clicked()
{
	viewComboBox->clear();
	connectCheckBox->setChecked(false);
	disconnectCheckBox->setChecked(true);
}


void ConnectView::okPushButton_clicked()
{
	int connectedId;

	if(connectCheckBox->isChecked())
	{
		TeViewMap& viewMap = db_->viewMap();
		TeViewMap::iterator it = viewMap.begin();
		while(it != viewMap.end())
		{
			if(it->second->name() == viewComboBox->currentText().latin1())
			{
				connectedId = it->first;
				view_->connectedId(connectedId);
				break;
			}
			it++;
		}
		displayWindow_->plotData();
	}
	else
	{
		view_->connectedId(0);
		displayWindow_->resetConnectedPixmap();
	}

	hide();
}


void ConnectView::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("connectView.htm");
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

