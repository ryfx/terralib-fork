/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <PluginParameters.h>
#include <DatabaseDialog.h>
#include <TeDatabaseUtils.h>
#include <TeLayerFunctions.h>
#include <TeWaitCursor.h>
#include <help.h>

//Qt include files
#include <qlistview.h>
#include <qdockwindow.h>
#include <qmessagebox.h>
#include <qmainwindow.h>

void CopyDatabaseWindow::init()
{
	help_ = NULL;
	_dbIn = NULL;
	_dbOut = NULL;

	QGridLayout* layout = new QGridLayout(fraConnect);

	databaseDialog = new DatabaseDialog(fraConnect, "");

	layout->addWidget(databaseDialog, 0, 0);

	databaseDialog->init(true, false);
	databaseDialog->show();

	feedbackTable->setNumCols(4);
	QStringList columnLabels;
	columnLabels.append(tr("Type"));
	columnLabels.append(tr("Object"));
	columnLabels.append(tr("Status"));
	columnLabels.append(tr("Error Message"));
	feedbackTable->setColumnLabels(columnLabels);
	feedbackTable->setNumRows(0);

	layersListView->setRootIsDecorated(true);
    layersListView->addColumn(tr("Layers"));
}

void CopyDatabaseWindow::destroy()
{
	if(_dbOut != NULL)
	{
		_dbOut->close();
		delete _dbOut;
		_dbOut = NULL;
	}
	if(help_ != NULL)
	{
		delete help_;
		help_ = NULL;
	}
}

void CopyDatabaseWindow::initFromPluginParameters()
{
}

void CopyDatabaseWindow::updateForm( PluginParameters * pluginParameters )
{
	feedbackTable->setNumRows(0);
	copyCurrentViewsRadioButton_clicked();
	pluginParameters_ = pluginParameters;

	TeDatabase* database = pluginParameters_->getCurrentDatabasePtr();
	if(database == NULL)
	{
		QMessageBox::critical(pluginParameters_->qtmain_widget_ptr_, tr("Error"), tr("There is no database connected!"));
		return;
	}

	databaseDialog->reset();

	_dbIn = database;

	copyDatabaseWidgetStack->raiseWidget(0);
}


void CopyDatabaseWindow::helpConnectPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("PluginCopiaBanco_janela1.htm");
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


void CopyDatabaseWindow::nextConnectPushButton_clicked()
{	
	if(_dbOut != NULL)
	{
		if(databaseDialog->isEqualTo(_dbOut) == false)
		{
			_dbOut->close();
			delete _dbOut;
						
			_dbOut = databaseDialog->getConnection(false, false);
			if(_dbOut == NULL)
			{
				return;
			}
		}
	}
	else
	{
		_dbOut = databaseDialog->getConnection(false, false);
		if(_dbOut == NULL)
		{
			return;
		}
	}

	copyDatabaseWidgetStack->raiseWidget(1);

	return;
}


void CopyDatabaseWindow::closeConnectPushButton_clicked()
{
	closeWindow();
}


void CopyDatabaseWindow::helpStatusPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("PluginCopiaBanco_janela4.htm");
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


void CopyDatabaseWindow::closeStatusPushButton_clicked()
{
	closeWindow();
}


void CopyDatabaseWindow::helpOperationPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("PluginCopiaBanco_janela2.htm");
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


void CopyDatabaseWindow::previousOperationPushButton_clicked()
{
	copyDatabaseWidgetStack->raiseWidget(0);
	/*if(_dbOut != NULL)
	{
		_dbOut->close();
		delete _dbOut;
		_dbOut = NULL;
	}*/
}


void CopyDatabaseWindow::nextOperationPushButton_clicked()
{
	layersListView->clear();
	
	if(copyDatabaseRadioButton->isOn())
	{
		copyDatabaseWidgetStack->raiseWidget(3);
		copyData();
	}
	else if(copyLayersRadioButton->isOn())
	{
		QListViewItem* item = new QListViewItem(layersListView);
		item->setText(0, tr("Layers"));
		item->setOpen(true);

		TeLayerMap& layerMap = _dbIn->layerMap();
		TeLayerMap::iterator itLayer = layerMap.begin();
		while(itLayer != layerMap.end())
		{
			QCheckListItem* layerItem = new QCheckListItem(item,  itLayer->second->name().c_str(), QCheckListItem::CheckBox);
			layerItem->setOn(true);

			++itLayer;
		}

		copyDatabaseWidgetStack->raiseWidget(2);
	}
}


void CopyDatabaseWindow::closeOperationPushButton_clicked()
{
	closeWindow();
}


void CopyDatabaseWindow::selectAllLayersPushButton_clicked()
{
	QListViewItemIterator itRoot(layersListView->firstChild());

	QListViewItem* root = itRoot.current();
	if(root != NULL)
	{
		QListViewItemIterator itItems(root->firstChild());
		QListViewItem* qItem = itItems.current();
		while(qItem != NULL)
		{
			QCheckListItem* qCheckItem =(QCheckListItem*)qItem;
			qCheckItem->setOn(true);
		
			++itItems;
			qItem = itItems.current();
		}
	}
}

void CopyDatabaseWindow::unselectAllLayersPushButton_clicked()
{
	QListViewItemIterator itRoot(layersListView->firstChild());

	QListViewItem* root = itRoot.current();
	if(root != NULL)
	{
		QListViewItemIterator itItems(root->firstChild());
		QListViewItem* qItem = itItems.current();
		while(qItem != NULL)
		{
			QCheckListItem* qCheckItem =(QCheckListItem*)qItem;
			qCheckItem->setOn(false);
		
			++itItems;
			qItem = itItems.current();
		}
	}
}


void CopyDatabaseWindow::helpLayersPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("PluginCopiaBanco_janela3.htm");
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


void CopyDatabaseWindow::previousLayersPushButton_clicked()
{
	_setLayersToIgnore.clear();
	copyDatabaseWidgetStack->raiseWidget(1);	
}


void CopyDatabaseWindow::nextLayersPushButton_clicked()
{
	_setLayersToIgnore.clear();

	bool atLeastOne = false;

	QListViewItemIterator itRoot(layersListView->firstChild());

	QListViewItem* root = itRoot.current();
	if(root != NULL)
	{
		QListViewItemIterator itItems(root->firstChild());
		QListViewItem* qItem = itItems.current();
		while(qItem != NULL)
		{
			QCheckListItem* qCheckItem =(QCheckListItem*)qItem;
			if(qCheckItem->isOn() == false)
			{
				_setLayersToIgnore.insert(qCheckItem->text().latin1());
			}
			else
			{
				atLeastOne = true;
			}
		
			++itItems;
			qItem = itItems.current();
		}
	}

	if(atLeastOne == false)
	{
		QMessageBox::information(this, tr("Copy Database"), tr("At least one layer must be selected!"));
		return;
	}

	copyDatabaseWidgetStack->raiseWidget(3);
	copyData();
}


void CopyDatabaseWindow::closeLayersPushButton_clicked()
{
	closeWindow();
}


void CopyDatabaseWindow::copyData()
{
	TeWaitCursor wait;
	wait.setWaitCursor();


	bool copyDatabase = copyDatabaseRadioButton->isOn();

	std::set<int> setViewIds;
	std::vector<std::string> vecViewThemeNames;
	TeDatabasePortal* portal = _dbIn->getPortal();

	std::string sql = "SELECT te_view.view_id, te_view.name, te_view.user_name, te_theme.name FROM te_view LEFT JOIN te_theme ON te_view.view_id = te_theme.view_id";
	if(copyCurrentViewsRadioButton->isChecked())
	{
		sql += " WHERE user_name = '" + _dbIn->user() + "'";
	}
	if(!portal->query(sql))
	{
		delete portal;
		return;
	}

	while(portal->fetchRow())
	{
		setViewIds.insert(portal->getInt(0));
		std::string viewName(portal->getData(1));
		std::string view_Theme = viewName + "/";
		view_Theme += portal->getData(3);
		vecViewThemeNames.push_back(view_Theme);
	}
	
	delete portal;
	portal = NULL;

	TeLayerMap& layerMap = _dbIn->layerMap();
	TeViewMap& viewMap = _dbIn->viewMap();	
	unsigned int row = 0;

	feedbackTable->setNumRows(0);
	feedbackTable->setNumRows(layerMap.size() + vecViewThemeNames.size());

	copyStepTextLabel->setText("1/2");
	copyStepNameTextLabel->setText(tr("Copying Layers"));

//preenche a tabela de feedback com os layers
	row = 0;
	TeLayerMap::iterator itLayer = layerMap.begin();
	while(itLayer != layerMap.end())
	{
		std::string layerName = itLayer->second->name();
		feedbackTable->setText(row, 0, tr("Layer"));
		feedbackTable->setText(row, 1, layerName.c_str());		

		if(_setLayersToIgnore.find(layerName) == _setLayersToIgnore.end())
		{
			feedbackTable->setText(row, 2, tr("Waiting"));
		}
		else
		{
			feedbackTable->setText(row, 2, tr("Not Selected"));
		}
		
		++itLayer;
		++row;
	}

//preenche a tabela de feedback com os temas
	for(unsigned int i = 0; i < vecViewThemeNames.size(); ++i, ++row)
	{		
		feedbackTable->setText(row, 0, tr("View / Theme"));
		feedbackTable->setText(row, 1, vecViewThemeNames[i].c_str());
		feedbackTable->setText(row, 2, tr("Waiting"));
	}

	unsigned int errors = 0;

	TeProgressBase* progressBase = TeProgress::instance(); 
	if(TeProgress::instance())
	{
		TeProgress::setProgressInterf(NULL);
		//TeProgress::instance()->setCaption("Copy Database");
	}

//copia os layers
	itLayer = layerMap.begin();
	row = 0;
	while(itLayer != layerMap.end())
	{
		/*if(TeProgress::instance())
		{
			std::string msg = "Copying Layer " + itLayer->second->name();
			TeProgress::instance()->setMessage(msg);
			if(TeProgress::instance()->wasCancelled())
			{
				break;
			}
		}*/
		
		feedbackTable->ensureCellVisible(row, 0);

		std::string layerName = itLayer->second->name();
		if(_setLayersToIgnore.find(layerName) != _setLayersToIgnore.end())
		{
			++itLayer;
			++row;
			continue;
		}

		feedbackTable->setText(row, 2, tr("Copying"));

		TeLayer* toLayer = new TeLayer(itLayer->second->name(), _dbOut, itLayer->second->projection());

		std::map<string, string> tables;
		if(TeCopyLayerToLayer(itLayer->second, toLayer, &tables))
		{
			feedbackTable->setText(row, 2, tr("Copied with Success"));
		}
		else
		{
			feedbackTable->setText(row, 2, tr("Error Copying"));
			feedbackTable->setText(row, 3, _dbOut->errorMessage().c_str());
			++errors;
		}

		++itLayer;
		++row;
	}

	copyStepTextLabel->setText("2/2");
	copyStepNameTextLabel->setText(tr("Copying Views and Themes"));
	
	std::set<int>::iterator it;
	for(it = setViewIds.begin(); it != setViewIds.end(); ++it)
	{
		/*if(TeProgress::instance())
		{
			if(TeProgress::instance()->wasCancelled())
			{
				break;
			}
		}*/

		int viewId = *it;
		TeView* view = NULL;

		bool remove = false;
		TeViewMap::iterator itView = viewMap.find(viewId);
		if(itView == viewMap.end())
		{
			view = new TeView("", "", viewId);
			if(!_dbIn->loadView(view))
			{
				return;
			}
			remove = true;
		}
		else
		{
			view = itView->second;
		}

        std::string destinationUser = view->user();
		if(changeUserNameCheckBox->isChecked())
		{
			destinationUser = _dbOut->user();
		}

		std::set<int> setThemeIds;
		std::vector<TeViewNode*>& vecThemes = view->themes();
		for(unsigned int j = 0; j < vecThemes.size(); ++j)
		{
			TeViewNode* node = vecThemes[j];
			if(copyDatabase == true)
			{
				setThemeIds.insert(node->id());
				continue;
			}

			if(node->type() != TeTHEME)
			{
				continue;
			}

			TeTheme* theme = dynamic_cast<TeTheme*>(node);
			if(theme == NULL)
			{
				continue;
			}

			std::string layerName = theme->layer()->name();
			if(_setLayersToIgnore.find(layerName) == _setLayersToIgnore.end())
			{
				setThemeIds.insert(node->id());
			}
		}

		if(copyDatabase == false && setThemeIds.empty())
		{
			feedbackTable->setText(row, 2, tr("No themes to copy"));
			continue;
		}

		std::vector<bool> vecResult;
		if(TeCopyView(view, _dbIn, view->name(), destinationUser, setThemeIds, _dbOut, vecResult))
		{
			for(unsigned int i = 0; i < vecResult.size(); ++i, ++row)
			{
				if(vecResult[i])
				{
					feedbackTable->setText(row, 2, tr("Copied with Success"));
				}
				else
				{
					++errors;
					feedbackTable->setText(row, 2, tr("Error Copying: ") + QString(_dbOut->errorMessage().c_str()));
					//feedbackTable->setText(row, 3, _dbOut->errorMessage().c_str());
				}
			}
		}

		if(remove == true)
		{
			TeViewMap::iterator viewIt = viewMap.find(viewId);
			if(viewIt != viewMap.end())
			{
				if(viewIt->second)
				{
					delete viewIt->second;
				}
			}
			viewMap.erase(viewIt);
		}
	}

	if(progressBase != 0)
	{
		TeProgress::setProgressInterf(progressBase);
	}

	_dbOut->clear();
	_dbOut->close();
	delete _dbOut;
	_dbOut = NULL;

	wait.resetWaitCursor();

	copyStepTextLabel->setText("");
	copyStepNameTextLabel->setText(tr("The copy has finished!"));

	QString qMessage = tr("The copy has finished!");
	qMessage += "\n";
	qMessage += tr("Number of errors - ") + Te2String(errors).c_str();

	QMessageBox::information(this, tr("Copy Database"), qMessage);
}

void CopyDatabaseWindow::closeWindow()
{
	if(_dbOut != NULL)
	{
		_dbOut->close();
		delete _dbOut;
		_dbOut = NULL;
	}

	close();
}

void CopyDatabaseWindow::copyCurrentViewsRadioButton_clicked()
{
	if(copyCurrentViewsRadioButton->isChecked())
	{
		changeUserNameCheckBox->setEnabled(true);
		changeUserNameCheckBox->setChecked(true);
	}
}

void CopyDatabaseWindow::copyAllViewsRadioButton_clicked()
{
	if(copyAllViewsRadioButton->isChecked())
	{
		changeUserNameCheckBox->setEnabled(false);
		changeUserNameCheckBox->setChecked(false);
	}
}
