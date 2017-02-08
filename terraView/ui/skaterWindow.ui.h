/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <TeGUIUtils.h>
#include <TeQtMethods.h>
#include <TeAppTheme.h>
#include <TeDataTypes.h>
#include <TeView.h>
#include <terraView.h>
#include <TeQtViewsListView.h>
#include <TeQtThemeItem.h>
#include <TeQtViewItem.h>
#include <TeQtGrid.h>
#include <graphic.h>
#include <TeColorUtils.h>

#include <qlistbox.h>
#include <qapplication.h>
#include <qmessagebox.h>

void SkaterWindow::init()
{
	help_ = 0;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	
	if (!setDefaultWindow())
  {
 		hide();
		reject();
		close();
  }
	skaCol_ = "";
}


void SkaterWindow::themeSeleceted()
{
	TeTheme* currTheme = mainWindow_->currentView()->get(string(dataThemeComboBox->currentText().ascii()));

	vector<TeAttrDataType> attrTypeVector;
	  
	attrTypeVector.push_back(TeREAL);
	attrTypeVector.push_back(TeINT);
	
	if (currTheme == NULL) 
	{
		QMessageBox::critical(this, tr("Error"), tr("Unable to load theme!"));
		return;
	}

	fillTableCombo(mainWindow_->currentView(), resultTableComboBox, dataThemeComboBox,"", true, TeAttrStatic);
	
	fillAttributeListBox(attrTypeVector, currTheme, themeAttributesListBox);

	selectedAttributesListBox->clear();

	if(populationRadioButton->isChecked()) 
	{
		fillTableCombo(mainWindow_->currentView(), dataTableComboBox, dataThemeComboBox,"");
		fillColumnCombo(TeREAL, currTheme, dataColumnComboBox, dataTableComboBox,"",true);
		fillColumnCombo(TeINT, currTheme, dataColumnComboBox, dataTableComboBox,"",false);
	}

	//fill combos by group objects
	groupTableComboBox->clear();
	groupColumnComboBox->clear();
	groupTableComboBox->insertItem(QString(""));
	groupColumnComboBox->insertItem(QString(""));
	fillTableCombo(mainWindow_->currentView(), groupTableComboBox, dataThemeComboBox,"", false);
	fillColumnCombo(TeSTRING, currTheme, groupColumnComboBox, groupTableComboBox,"",false);
	fillColumnCombo(TeINT, currTheme, groupColumnComboBox, groupTableComboBox,"",false);
}


bool SkaterWindow::setDefaultWindow()
{
	TeView* currView = mainWindow_->currentView();

	TeTheme* currTheme = 0;
	if (mainWindow_->currentTheme() != 0)
		currTheme = (TeTheme*)mainWindow_->currentTheme()->getTheme();	

	vector<TeAttrDataType> attrTypeVector;
	  
	attrTypeVector.push_back(TeREAL);
	attrTypeVector.push_back(TeINT);
	
	string curT;
	if (currTheme)
		curT = currTheme->name();
	if(!fillThemeCombo(TePOLYGONS, currView, dataThemeComboBox, curT))
	{
		QMessageBox::warning(this, tr("Warning"), tr("No viable themes in current view!"));
		return false;
	}

	popDataGroupBox->setEnabled(false);

	fillTableCombo(currView, resultTableComboBox, dataThemeComboBox,"", true, TeAttrStatic);

	valueLineEdit->setText("5");
	
	resultColumnLineEdit->setText("ska");

	currTheme = mainWindow_->currentView()->get(string(dataThemeComboBox->currentText().ascii()));

	fillAttributeListBox(attrTypeVector, currTheme, themeAttributesListBox);

	//fill combos by group objects
	groupTableComboBox->clear();
	groupColumnComboBox->clear();
	groupTableComboBox->insertItem(QString(""));
	groupColumnComboBox->insertItem(QString(""));
	fillTableCombo(mainWindow_->currentView(), groupTableComboBox, dataThemeComboBox,"", false);
	fillColumnCombo(TeSTRING, currTheme, groupColumnComboBox, groupTableComboBox,"",false);
	fillColumnCombo(TeINT, currTheme, groupColumnComboBox, groupTableComboBox,"",false);

	return true;
}


void SkaterWindow::tableSelected()
{
	TeTheme* currTheme = mainWindow_->currentView()->get(string(dataThemeComboBox->currentText().ascii()));

	if (currTheme == NULL) 
	{
		QMessageBox::critical(this, tr("Error"), tr("Unable to load theme!"));
		return;
	}

	fillColumnCombo(TeREAL, currTheme, dataColumnComboBox, dataTableComboBox,"");
	fillColumnCombo(TeINT, currTheme, dataColumnComboBox, dataTableComboBox,"",false);
}

void SkaterWindow::groupTableSelected()
{
	TeTheme* currTheme = mainWindow_->currentView()->get(string(dataThemeComboBox->currentText().ascii()));

	if (currTheme == NULL) 
	{
		QMessageBox::critical(this, tr("Error"), tr("Unable to load theme!"));
		return;
	}

	//fill combos by group objects
	groupColumnComboBox->clear();
	groupColumnComboBox->insertItem(QString(""));
	fillColumnCombo(TeSTRING, currTheme, groupColumnComboBox, groupTableComboBox,"",false);
	fillColumnCombo(TeINT, currTheme, groupColumnComboBox, groupTableComboBox,"",false);
}


void SkaterWindow::attributeAdded()
{
	if(themeAttributesListBox->count() > 0)	
		sendCurrentAttribute(themeAttributesListBox, selectedAttributesListBox);	
}


void SkaterWindow::attributeDeleted()
{
	if(selectedAttributesListBox->count() > 0)
		sendCurrentAttribute(selectedAttributesListBox, themeAttributesListBox);
}


int SkaterWindow::sendCurrentAttribute(QListBox* fromListBox, QListBox* toListBox)
{
	int index = fromListBox->currentItem();

	if (index == -1)							//Nenhum item selecionado
	{													
		fromListBox->setCurrentItem(0);	//Seleciona o primeiro
		index = 0;
	}

	string itemAtual = fromListBox->text(index);
	
	toListBox->insertItem(itemAtual.c_str());
	
	fromListBox->removeItem(index);

	toListBox->sort();

	return index;
}


bool SkaterWindow::getSelectedAttributes(vector<string> &Attr)
{
	QString attribute;
	int elem = selectedAttributesListBox->count();
	int cont = 1;

	if(elem > 0)
	{
		selectedAttributesListBox->setCurrentItem(0);
	
		while(cont <= elem)
		{
			attribute = selectedAttributesListBox->currentText();
			Attr.push_back(string(attribute.ascii()));
			selectedAttributesListBox->setCurrentItem(selectedAttributesListBox->currentItem()+1);
			cont++;
		}
		return true;
	}
	else
	{
		return false;
	}
}


/** Para executar o metodo eh necessario obter a matriz de vizinhaca e um stoset
 * com os atributos selecionados na interface **/
void SkaterWindow::execute()
{
	vector<double> het;
	vector<double> groups;
	
	TeSkaterMethod ska;
  
	//Vetor de atributos do listbox
	vector<string> attrs;
	
	if(!getSelectedAttributes(attrs))
	{
		QMessageBox::warning(this, tr("Error"), tr("No attributes selected."));
		return;
	}
  
	ska.setAttrVector(attrs);
	string groupTable = string(groupTableComboBox->currentText().ascii());
	string groupCol = string(groupColumnComboBox->currentText().ascii());
	if(!groupTable.empty() && !groupCol.empty())
        ska.setAggrAttr(groupTable+"."+groupCol);
  
	//Check de populacao
	if(populationRadioButton->isChecked())
	{
		string pop_table = string(dataTableComboBox->currentText().ascii());
		string pop_col = string(dataColumnComboBox->currentText().ascii());

		if(pop_col == "")
		{
			QMessageBox::warning(this, tr("Error"), tr("No population column selected."));
			return;
		}
		
		ska.setHasPop(true, (pop_table + "." + pop_col));
	}
	else
		ska.setHasPop(false, "");

	 //Nome da coluna, nome da tabela de resultados
	string tableName = string(resultTableComboBox->currentText().ascii());
	string res_col = string(resultColumnLineEdit->text().ascii());
	
	if(tableName.empty() || res_col.empty())
	{
		QMessageBox::warning(this, tr("Error"), tr("Please name the result column and table."));
		return;
	}
	skaCol_ = tableName + "." + res_col;
	
	ska.setResName(res_col, tableName);
	
	//Nome do tema + vista corrente
	string themeName = string(dataThemeComboBox->currentText().ascii());
	ska.setTheme(themeName, mainWindow_->currentView());

	// points to the desired Theme

	TeTheme* tempTheme = 0;
	TeAppTheme* tempAppTheme = 0;
	TeQtViewItem* curViewItem = mainWindow_->getViewsListView()->currentViewItem();
	vector<TeQtThemeItem*> themeItemVec = curViewItem->getThemeItemVec();
	for (unsigned int i=0; i<themeItemVec.size(); ++i)
	{
		TeTheme* theme = (TeTheme*)(themeItemVec[i]->getAppTheme())->getTheme();
		if (theme->name() == themeName)
		{
			tempTheme = theme;
			tempAppTheme = themeItemVec[i]->getAppTheme();
			break;
		}
	}

	//Opcao + tamPop + numCluster -- define TeSkaterFunctions.h
	int par_value = (int)atof(valueLineEdit->text().ascii());

	if(par_value <= 0)
	{
		QMessageBox::warning(this, tr("Error"), tr("The 'value' parameter should have a positive number."));
		return;
	}
	
	if(populationRadioButton->isChecked())		
		ska.setType(1, par_value, 0);	
	else
		ska.setType(0, 0, par_value);

	if (TeProgress::instance())
	{	 
		TeProgress::instance()->reset();
		QString label = tr("Skater");
		QString msg = tr("Partitioning region. Please wait...");
		TeProgress::instance()->setCaption(label.latin1());
		TeProgress::instance()->setMessage(msg.latin1());
	}

	mainWindow_->getGrid()->clear();
	if (ska.apply(het))
	{
		TeAttributeList attList = tempTheme->sqlAttList();
		string colName = TeGetExtension(skaCol_.c_str());
		TeAttributeList::const_iterator it = attList.begin();
		while (it != attList.end())
		{
			if (it->rep_.name_ == colName || it->rep_.name_ == skaCol_)
			{
				skaCol_ = it->rep_.name_;
				break;
			}
			++it;
		}
		if (it != attList.begin() && it != attList.end())
		{
			TeGrouping groupingParams;
			groupingParams.groupMode_ = TeUniqueValue;
			groupingParams.groupAttribute_ = it->rep_;
			tempTheme->resetGrouping();
			tempAppTheme->groupColor("240;255;255;0-60;255;255;1-120;255;255;2-300;255;255;3-0;255;255;4");
			tempTheme->buildGrouping(groupingParams);

			TeColor cor(0,0,255);
			vector<ColorBar> colorBar;
			ColorBar c1;
			c1.color(cor);
			c1.distance_ = 0;
			colorBar.push_back(c1);

			cor.red_ = 255;
			cor.green_ = 255;
			cor.blue_ = 0;
			c1.color(cor);
			c1.distance_ = 1;
			colorBar.push_back(c1);

			cor.red_ = 0;
			cor.green_ = 255;
			cor.blue_ = 0;
			c1.color(cor);
			c1.distance_ = 2;
			colorBar.push_back(c1);

			cor.red_ = 255;
			cor.green_ = 0;
			cor.blue_ = 255;
			c1.color(cor);
			c1.distance_ = 3;
			colorBar.push_back(c1);

			cor.red_ = 255;
			cor.green_ = 0;
			cor.blue_ = 0;
			c1.color(cor);
			c1.distance_ = 4;
			colorBar.push_back(c1);

			vector<TeColor> colorVec;
			colorVec = getColors(colorBar,tempTheme->grouping().groupNumSlices_);

			TeLegendEntryVector& legVec = tempTheme->legend();
			unsigned int nlegs = legVec.size();
			unsigned int i;
			for(i = 0; i < nlegs; ++i)
			{
				string vv = legVec[i].label();
				TeColor	cor = colorVec[i];
				if(tempTheme->visibleRep() & TePOINTS)
				{
					TeVisual* v = tempTheme->defaultLegend().visual(TePOINTS);	
					v->color(cor);
					legVec[i].setVisual(v->copy(), TePOINTS); 
				}
				if(tempTheme->visibleRep() & TeLINES)
				{
					TeVisual* v = tempTheme->defaultLegend().visual(TeLINES);	
					v->color(cor);
					legVec[i].setVisual(v->copy(), TeLINES); 
				}
				if((tempTheme->visibleRep() & TePOLYGONS) || (tempTheme->visibleRep() & TeCELLS))
				{
					TeVisual* v = tempTheme->defaultLegend().visual(TePOLYGONS);	
					v->color(cor);
					legVec[i].setVisual(v->copy(), TePOLYGONS); 
				}
			}
			tempTheme->saveGrouping();
			tempTheme->saveLegendInCollection();
			string update = "UPDATE te_theme_application SET ";
			update += "grouping_color='" + tempAppTheme->groupColor() + "'";
			update += " WHERE theme_id = " + Te2String(tempTheme->id());
			mainWindow_->currentDatabase()->execute(update);
		}
		// ---------------------------------------------- set new theme tree  
		TeQtViewsListView* viewsListView = mainWindow_->getViewsListView();
		TeQtThemeItem* themeItem = viewsListView->getThemeItem(tempAppTheme);
		viewsListView->selectItem ((QListViewItem*) themeItem);
		mainWindow_->updateThemeItemInterface(themeItem);
		
		// ---------------------------------------------- grid 
		if(mainWindow_->getGrid()->getPortal() == 0)
			mainWindow_->getGrid()->init(tempAppTheme);
		mainWindow_->getGrid()->goToLastColumn();
		
		for(unsigned int i=1; i<= het.size(); ++i)
			groups.push_back(i+1);

		mainWindow_->getGraphicWindow();
		mainWindow_->getGraphicWindow()->setSkaterValues(groups, het);
		mainWindow_->getGraphicWindow()->applyGraphic();
		mainWindow_->getGraphicWindow()->show();
		mainWindow_->tileWindowsAction_activated();
		mainWindow_->resetAction_activated();
		accept();
	}
	else
	{
		if(mainWindow_->getGrid()->getPortal() == 0)
			mainWindow_->getGrid()->init(tempAppTheme);
		mainWindow_->resetAction_activated();
		skaCol_ = "";
		if (TeProgress::instance())
			TeProgress::instance()->reset();
		reject();
	}
}


void SkaterWindow::cancelPushButton_clicked()
{
	close();
}


void SkaterWindow::groupsRadioButton_clicked()
{
	if(populationRadioButton->isChecked())
	{
		groupsRadioButton->setChecked(true);
		populationRadioButton->setChecked(false);
		popDataGroupBox->setEnabled(false);
		dataTableComboBox->clear();
		dataColumnComboBox->clear();
	}
	else
	{
		groupsRadioButton->setChecked(true);
	}
}


void SkaterWindow::populationRadioButton_clicked()
{
	vector<TeAttrDataType> attrTypeVector;
	  
	attrTypeVector.push_back(TeREAL);
	attrTypeVector.push_back(TeINT);
	
	if(groupsRadioButton->isChecked())
	{
		groupsRadioButton->setChecked(false);
		populationRadioButton->setChecked(true);
		popDataGroupBox->setEnabled(true);
		fillTableCombo(mainWindow_->currentView(), dataTableComboBox, dataThemeComboBox,"");

		TeTheme* currTheme = mainWindow_->currentView()->get(string(dataThemeComboBox->currentText().ascii()));
		
		if (currTheme == NULL) 
		{
			QMessageBox::critical(this, tr("Error"), tr("Unable to load theme!"));
			return;
		}

		fillColumnCombo(TeREAL, currTheme, dataColumnComboBox, dataTableComboBox,"");
    fillColumnCombo(TeINT, currTheme, dataColumnComboBox, dataTableComboBox,"",false);
	}
	else
	{
		populationRadioButton->setChecked(true);
	}
}


void SkaterWindow::fillAttributeListBox(vector<TeAttrDataType> &aType, TeTheme *theme, QListBox *attListBox) 
{
	
	TeAttributeList columns;
	TeAttrTableVector atvec;
  theme->getAttTables(atvec);

	attListBox->clear();
	
	unsigned int t;
	
	for (t = 0; t < atvec.size(); t++) 
	{
		TeTable ttable = atvec[t];
		columns = ttable.attributeList();		
		string tableName = ttable.name();
		
		int count = 0;
	
		for(unsigned int i = 0; i < columns.size(); i++)
		{
			int type = columns[i].rep_.type_;			
			string colName = columns[i].rep_.name_;
			
			for(unsigned int j = 0; j < aType.size(); j++)
			{
				if(type == aType[j]) 
				{
					string attribute = tableName + "." + colName;
					attListBox->insertItem(attribute.c_str());
					count++;
				}
			}
		}
	}
	
	attListBox->sort();
	attListBox->setCurrentItem(0);
}


void SkaterWindow::okPushButton_clicked()
{
	execute();
}


void SkaterWindow::close()
{
	QWidget::close();
}


void SkaterWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("skaterWindow.htm");
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



