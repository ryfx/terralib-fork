/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <HidroUtils.h>

#include <HidroOverlay.h>
#include <HidroOverlayClass.h>
#include <HidroOverlayRuleGroup.h>
#include <HidroOverlayRule.h>
#include <HidroCrossTable.h>

#include <HidroAttrString.h>
#include <HidroAttrInt.h>
#include <HidroAttrDouble.h>

#include <HidroLUTController.h>

#include <TeQtViewsListView.h>
#include <TeUtils.h>

#include <qcolordialog.h>
#include <qmessagebox.h>
#include <qcolordialog.h>
#include <qfiledialog.h>


void HidroClassWindow::init( PluginParameters * parameters )
{
    _parameters = parameters;

	_lutGroupController = NULL;
	_lutController = NULL;
	_overlayClassGroup = NULL;

	listLayersLUT();

	listLayersRaster();

	overlayWidgetStack->raiseWidget(5);
}


void HidroClassWindow::destroy()
{
    delete _lutGroupController;
	delete _lutController;
}

////////////////////////////////////////////////////////////////////////////////////
/////////////////////////Interface Class

void HidroClassWindow::classInputLayerComboBox_activated( const QString & value )
{
	if(value.isEmpty())
	{
		return;
	}

	std::string layerName = value.latin1();

	HidroUtils utils(_parameters->getCurrentDatabasePtr());

	TeLayer* layer = utils.getLayerByName(layerName);

	if(layer)
	{
		if(_lutController)
		{
			delete _lutController;
		}

		_lutController = new HidroLUTController(layer);

		if(!_lutController->createAttrLUTTable())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Error creating attr lut table."));
		}

		fillTableClassWithLutInfo();
	}
}


void HidroClassWindow::colorPushButton_clicked()
{
	QColor qc = QColorDialog::getColor(colorPixmapLabel->backgroundColor());

	colorPixmapLabel->setBackgroundColor(qc);
}


void HidroClassWindow::savePushButton_clicked()
{
	if(classInputLayerComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Layer with Raster Pallete representation not selected."));
		return;
	}

	if(nameLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Class Name not defined."));
		return;
	}

	if(!entryCheckBox->isChecked() && indexLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("LUT Entry not selected."));
		return;
	}

	std::string className = nameLineEdit->text().latin1();

	int r = colorPixmapLabel->backgroundColor().red();
	int g = colorPixmapLabel->backgroundColor().green();
	int b = colorPixmapLabel->backgroundColor().blue();

	bool setAsDummy = dummyCheckBox->isChecked();

	bool res = false;

	if(entryCheckBox->isChecked())
	{
		res = _lutController->addNewClass(className, r, g ,b, setAsDummy);
	}
	else
	{
		int classIndex = indexLineEdit->text().toInt();

		res = _lutController->changeClass(classIndex, className, r, g, b, setAsDummy); 
	}

	if(!res)
	{
		QMessageBox::warning(this, tr("Warning"), _lutController->getErrorMessage().c_str());
		return;
	}
	
	classInputLayerComboBox_activated(classInputLayerComboBox->currentText().latin1());
}


void HidroClassWindow::lutTable_clicked( int row, int, int, const QPoint & )
{
   	if(row == -1)
	{
		return;
	}

	entryCheckBox->setChecked(false);
	indexLineEdit->setText(lutTable->text(row, 0));

	std::string className = "";

	if(!lutTable->text(row, 1).isEmpty())
	{
		className = lutTable->text(row, 1).latin1();
	}

	nameLineEdit->setText(className.c_str());

	std::vector<HidroLutInfo> lutInfoVec;

	_lutController->getClassList(lutInfoVec);

	colorPixmapLabel->setBackgroundColor(QColor(atoi(lutInfoVec[row]._r.c_str()), atoi(lutInfoVec[row]._g.c_str()), atoi(lutInfoVec[row]._b.c_str())));

	dummyCheckBox->setChecked(false);
}


void HidroClassWindow::entryCheckBox_toggled( bool status )
{
    if(status)
	{
		indexLineEdit->setText("");
	}
}


void HidroClassWindow::saveClassPushButton_clicked()
{
	if(classInputLayerComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Layer not selected."));
		return;
	}

	std::string layerName = classInputLayerComboBox->currentText().latin1();

	QString s = QFileDialog::getSaveFileName("","*.txt",this,"Save as","File name");
	
	if (s.isEmpty())
	{
		return;
	}

	std::string fileName = s.latin1();
	fileName += ".txt";

	_lutController->exportLutInfoToFile(fileName);

	QMessageBox::information(this, tr("Information"), tr("File Saved."));   
}


void HidroClassWindow::updateLegPushButton_clicked()
{
	if(classInputLayerComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Layer not selected."));
		return;
	}

	std::string layerName = classInputLayerComboBox->currentText().latin1();

	TeDatabase* database = _parameters->getCurrentDatabasePtr();

	TeThemeMap& themeMap = database->themeMap();
	TeThemeMap::iterator itMap = themeMap.begin();

	while(itMap != themeMap.end())
	{
		if(itMap->second->type() == TeTHEME)
		{
			if(((TeTheme*)itMap->second)->layer()->name() == layerName)
			{
				TeTheme* theme = (TeTheme*)itMap->second;

				TeQtThemeItem* themeItem = _parameters->teqtviewslistview_ptr_->getThemeItem(theme->id());

				_lutController->updateThemeLegend(theme, themeItem);
			}
		}

		++itMap;
	}

	QMessageBox::information(this, tr("Information"), tr("Themes Legend updated."));
}

void HidroClassWindow::fillTableClassWithLutInfo()
{
	lutTable->setNumRows(0);

	std::vector<HidroLutInfo> lutInfoVec;

	_lutController->getClassList(lutInfoVec);

	lutTable->setNumRows(lutInfoVec.size());

	for(unsigned int i = 0; i < lutInfoVec.size(); ++i)
	{
		lutTable->setText(i, 0, lutInfoVec[i]._index.c_str());
		lutTable->setText(i, 1, lutInfoVec[i]._className.c_str());

		QColor color(atoi(lutInfoVec[i]._r.c_str()), atoi(lutInfoVec[i]._g.c_str()), atoi(lutInfoVec[i]._b.c_str()));
		QPixmap	pixmap(21, 16);
		pixmap.fill(color);

		lutTable->setPixmap(i, 2, pixmap); 
	}

	for(int i = 0; i < lutTable->numCols(); ++i)
	{
		lutTable->adjustColumn(i);
	}

	entryCheckBox->setChecked(false);
	indexLineEdit->setText("");
	nameLineEdit->setText("");
	colorPixmapLabel->setBackgroundColor(QColor(255, 255, 255));
	dummyCheckBox->setChecked(false);
}

////////////////////////////////////////////////////////////////////////////////////
/////////////////////////Interface Grouping

void HidroClassWindow::addGroupingPushButton_clicked()
{
	if(!_lutGroupController)
	{
		return;
	}

	if(groupingNameLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Group name not defined."));
		return;
	}

//group name
	std::string groupName = groupingNameLineEdit->text().latin1();

//color
	QColor color(255, 0, 0);
	QPixmap pixmap(21, 16);
	pixmap.fill(color);

	bool res = _lutGroupController->addGroup(groupName, 255, 0, 0);

	if(!res)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Group already exist."));
		return;
	}

	int row = groupingTable->numRows();

	groupingTable->setNumRows(row + 1);

	groupingTable->setText(row, 1, groupName.c_str());

	groupingTable->setPixmap(row, 0, pixmap);

	groupingNameLineEdit->setText("");
}

void HidroClassWindow::removeGroupingPushButton_clicked()
{
	if(!_lutGroupController)
	{
		return;
	}

	if(groupingTable->currentRow() == -1)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Group not selected."));
		return;
	}

	std::string groupName = groupingTable->text(groupingTable->currentRow(), 1).latin1();

	_lutGroupController->removeGroup(groupName);

	groupingTable->removeRow(groupingTable->currentRow());

	fillTableGroupWithLutInfo();
}

void HidroClassWindow::groupingPushButton_clicked()
{
//check parameters
	if(!_lutGroupController)
	{
		return;
	}

	if(!_lutGroupController->hasGrouping())
	{
		QMessageBox::warning(this, tr("Warning"), tr("None grouping was found."));
		return;
	}

	if(groupingOutputLayerLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Output layer name not defined."));
		return;
	}
	std::string layerName = groupingOutputLayerLineEdit->text().latin1();

	if(!_lutGroupController->generatingGrouping(layerName))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Error grouping raster."));
		return;
	}
	else
	{
		QMessageBox::warning(this, tr("Warning"), tr("Raster Grouping Done."));

		_parameters->updateTVInterface();

		listLayersLUT();
	}

	return;
}

void HidroClassWindow::groupingInputLayerComboBox_activated( const QString & value )
{
	if(value.isEmpty())
	{
		return;
	}

	std::string layerName = value.latin1();

	HidroUtils utils(_parameters->getCurrentDatabasePtr());

	TeLayer* layer = utils.getLayerByName(layerName);

	if(layer)
	{
		if(_lutGroupController)
		{
			delete _lutGroupController;
		}

		_lutGroupController = new HidroGroupLUTController(layer);

		fillTableGroupWithLutInfo();
	}
}

void HidroClassWindow::groupingLutTable_clicked( int row, int col, int, const QPoint & )
{
	if(row == -1)
	{
		return;
	}

	if(col == 3)
	{
		std::string lutIdx = groupingLutTable->text(row, 1).latin1();
		std::string group = "";

		if(_lutGroupController->lutEntryHasGroup(lutIdx, group))
		{
			groupingLutTable->setPixmap(row, 3, QPixmap::fromMimeSource("checkFalse.bmp"));
			groupingLutTable->setText(row, 4, "");

			_lutGroupController->removeLutEntryGroup(lutIdx);
		}
		else
		{
			if(groupingTable->currentRow() == -1)
			{
				QMessageBox::warning(this, tr("Warning"), tr("Group not selected."));
				return;
			}

			std::string groupName = groupingTable->text(groupingTable->currentRow(), 1).latin1();

			groupingLutTable->setPixmap(row, 3, QPixmap::fromMimeSource("checkTrue.bmp"));
			groupingLutTable->setText(row, 4, groupName.c_str());

			_lutGroupController->setLutEntryGroup(lutIdx, groupName);
		}
	}
}


void HidroClassWindow::groupingTable_doubleClicked( int row, int col, int, const QPoint & )
{
    if(row == -1)
	{
		return;
	}

	if(col == 0)
	{
		QColor qc = QColorDialog::getColor();

		QPixmap pixmap(21, 16);
		pixmap.fill(qc);
		groupingTable->setPixmap(row, col, pixmap);

		std::string groupName = groupingTable->text(row, 1).latin1();

		bool res = _lutGroupController->changeGroupColor(groupName, qc.red(), qc.green(), qc.blue());

		if(!res)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Group already exist."));
			return;
		}
	}
}

void HidroClassWindow::fillTableGroupWithLutInfo()
{
	if(!_lutGroupController)
	{
		return;
	}

	int nClass = _lutGroupController->getLutInfo().size();

	groupingLutTable->setNumRows(0);

	groupingLutTable->setNumRows(nClass);


	int count = 0;

	for(int i = 0; i <= nClass; ++i)
	{
		std::map<std::string, HidroGroupLutInfo>::iterator it = _lutGroupController->getLutInfo().find(Te2String(i));

		if(it == _lutGroupController->getLutInfo().end())
		{
			continue;
		}

//color
		QColor color(atoi(it->second._r.c_str()), atoi(it->second._g.c_str()), atoi(it->second._b.c_str()));
		QPixmap pixmap(21, 16);
		pixmap.fill(color);
		groupingLutTable->setPixmap(count, 0, pixmap);

//index
		groupingLutTable->setText(count, 1, it->second._index.c_str());

//class
		groupingLutTable->setText(count, 2, it->second._className.c_str());

//group name
		if(it->second._hasGrouping)
		{
			groupingLutTable->setPixmap(count, 3, QPixmap::fromMimeSource("checkTrue.bmp"));
			groupingLutTable->setText(count, 4, it->second._groupingName.c_str());
		}
		else
		{
			groupingLutTable->setPixmap(count, 3, QPixmap::fromMimeSource("checkFalse.bmp"));
			groupingLutTable->setText(count, 4, "");
		}

		++count;
	}


	for(int i = 0; i < groupingLutTable->numCols(); ++i)
	{
		groupingLutTable->adjustColumn(i);
	}
}

////////////////////////////////////////////////////////////////////////////////////
/////////////////////////Interface Overlay

void HidroClassWindow::overlayColorPushButton_clicked()
{
	QColor qc = QColorDialog::getColor(overlayPixmapLabel->backgroundColor());

	overlayPixmapLabel->setBackgroundColor(qc);  
}


void HidroClassWindow::overlayAddPushButton_clicked()
{
	if(newClassLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Warning", "Class name not defined");
		return;
	}

	if(!_overlayClassGroup)
	{
		_overlayClassGroup = new HidroOverlayClassGroup();
	}

	int idxInt = _overlayClassGroup->getNewIndex();
	std::string className = newClassLineEdit->text().latin1();
	TeColor color(overlayPixmapLabel->backgroundColor().red(), 
		overlayPixmapLabel->backgroundColor().green(), overlayPixmapLabel->backgroundColor().blue());

	HidroOverlayClass* classItem = new HidroOverlayClass(idxInt, className);
	classItem->setClassColor(color);

	if(!_overlayClassGroup->addNewClass(classItem))
	{
		QMessageBox::warning(this, "Warning", "Class already exist");
		return;
	}

	int nRows = overlayNewClassTable->numRows();

	overlayNewClassTable->setNumRows(nRows + 1);

	std::string idx = Te2String(idxInt);

	overlayNewClassTable->setText(nRows, 0, idx.c_str());

	QPixmap	pixmap(21, 16);
	pixmap.fill(overlayPixmapLabel->backgroundColor());

	overlayNewClassTable->setPixmap(nRows, 1, pixmap); 

	overlayNewClassTable->setText(nRows, 2, className.c_str());

	for(int i = 0; i < overlayNewClassTable->numCols(); ++i)
	{
		overlayNewClassTable->adjustColumn(i);
	}

	newClassLineEdit->clear();
}


void HidroClassWindow::overlayRemovePushButton_clicked()
{
	if(overlayNewClassTable->currentRow() == -1)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Class not selected."));
		return;
	}

	if(!_overlayClassGroup)
	{
		return;
	}

	std::string className = overlayNewClassTable->text(overlayNewClassTable->currentRow(), 2).latin1();

	_overlayClassGroup->removeClass(className);

	overlayNewClassTable->removeRow(overlayNewClassTable->currentRow());

	newClassLineEdit->clear();
}


void HidroClassWindow::overlayAddGroupPushButton_clicked()
{
	if(overlayNewClassLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Class not selected."));
		return;
	}

	if(groupNameLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Group name not defined."));
		return;
	}

	if(!_overlayClassGroup)
	{
		return;
	}

	std::string className = overlayNewClassLineEdit->text().latin1();

	HidroOverlayClass* classItem = _overlayClassGroup->getClass(className);

	if(!classItem)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Class not found."));
	}

	std::string newGroup = groupNameLineEdit->text().latin1();

	HidroOverlayRuleGroup* ruleGroup = new HidroOverlayRuleGroup(newGroup);

	if(!classItem->addNewRuleGroup(ruleGroup))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Rule Group already exist."));
		delete ruleGroup;
		return;
	}

	QListViewItem* item = new QListViewItem(overlayGroupListView);
	item->setText(0, newGroup.c_str());

	overlayGroupListView->insertItem(item);
}


void HidroClassWindow::overlayRemoveGroupPushButton_clicked()
{
	if(!_overlayClassGroup)
	{
		return;
	}

	std::string className = overlayNewClassLineEdit->text().latin1();

	HidroOverlayClass* classItem = _overlayClassGroup->getClass(className);

	if(!classItem)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Class not found."));
	}

	if(!overlayGroupListView->selectedItem())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Rule not selected."));
		return;
	}

	std::string ruleGroup = overlayGroupListView->selectedItem()->text(0).latin1();

	classItem->removeRuleGroup(ruleGroup);

	overlayGroupListView->removeItem(overlayGroupListView->selectedItem());
}


void HidroClassWindow::overlayRasterComboBox_activated( const QString & value )
{
	if(value.isEmpty())
	{
		return;
	}
}


void HidroClassWindow::overlayAddRulePushButton_clicked()
{
	if(groupLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Rule Group not selected."));
		return;
	}

	if(ruleNameLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Rule name not defined."));
		return;
	}

	if(!layerListBox->selectedItem())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Layer for this rule not defined."));
		return;
	}
	
	std::string op = getSelectedOperator();
	if(op.empty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Operator for this rule not defined."));
		return;
	}

	if(valueLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Value for this rule not defined."));
		return;
	}

	std::string className = overlayNewClassLineEdit->text().latin1();

	HidroOverlayClass* classItem = _overlayClassGroup->getClass(className);

	if(!classItem)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Class not found."));
	}

	std::string ruleGroupName = groupLineEdit->text().latin1();

	HidroOverlayRuleGroup* ruleGroup = classItem->getRuleGroup(ruleGroupName);

	if(!ruleGroup)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Rule Group not found."));
	}

	std::string ruleName = ruleNameLineEdit->text().latin1();

	HidroOverlayRule* rule = new HidroOverlayRule(ruleName);

	HidroUtils utils(_parameters->getCurrentDatabasePtr());

	TeLayer* layer = utils.getLayerByName(layerListBox->selectedItem()->text().latin1());

	rule->setLayer(layer);

//check the raster layer data type
	if(layer->raster()->params().photometric_[0] == TeRasterParams::TePallete)
	{
		//check if the raster lut has attribute table
		HidroLUTController lutControl(layer);

		if(lutControl.hasAttrLUTTable())
		{
			HidroAttrString* attr = new HidroAttrString("value", valueLineEdit->text().latin1());
			rule->setRuleAttr(attr);
		}
		else
		{
			HidroAttrInt* attr = new HidroAttrInt("value", valueLineEdit->text().toInt());
			rule->setRuleAttr(attr);
		}

	}
	else
	{
		if(layer->raster()->params().dataType_[0] == TeDOUBLE)
		{
			HidroAttrDouble* attr = new HidroAttrDouble("value", valueLineEdit->text().toDouble());
			rule->setRuleAttr(attr);
		}
		else if(layer->raster()->params().dataType_[0] == TeUNSIGNEDCHAR)
		{
			HidroAttrInt* attr = new HidroAttrInt("value", valueLineEdit->text().toInt());
			rule->setRuleAttr(attr);
		}
	}

	rule->setComparisonOperator(rule->getRuleComparisonOp(op));
	
	if(!ruleGroup->addNewRule(rule))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Rule already exist."));
		delete rule;
		return;
	}

	overlayRuleListBox->insertItem(ruleName.c_str());

	layerListBox->clearSelection();
	clearSelectedOperators();
	valueListBox->clearSelection();
	valueLineEdit->clear();
}



void HidroClassWindow::overlayRemoveRulePushButton_clicked()
{
	if(!overlayRuleListBox->selectedItem())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Rule not selected."));
		return;
	}

	std::string className = overlayNewClassLineEdit->text().latin1();

	HidroOverlayClass* classItem = _overlayClassGroup->getClass(className);

	if(!classItem)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Class not found."));
	}

	std::string ruleGroupName = groupLineEdit->text().latin1();

	HidroOverlayRuleGroup* ruleGroup = classItem->getRuleGroup(ruleGroupName);

	if(!ruleGroup)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Rule Group not found."));
	}

	std::string ruleName = overlayRuleListBox->selectedItem()->text().latin1();

	ruleGroup->removeRule(ruleName);

	overlayRuleListBox->removeItem(overlayRuleListBox->currentItem());

	layerListBox->clearSelection();
	clearSelectedOperators();
	valueListBox->clearSelection();
	valueLineEdit->clear();
}


void HidroClassWindow::overlayPushButton_clicked()
{
	if(referenceRasterComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Reference raster not defined."));
		return;
	}

	if(overlayOutputLayerLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Output layer name not defined."));
		return;
	}

	if(!_overlayClassGroup)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Output classes not defined."));
		return;
	}

	HidroOverlay overlay(_parameters->getCurrentDatabasePtr());

	overlay.setClassGroup(_overlayClassGroup);

	HidroUtils utils(_parameters->getCurrentDatabasePtr());
	TeLayer* layer = utils.getLayerByName(referenceRasterComboBox->currentText().latin1());

	overlay.setReferenceRaster(layer->raster());

	TeRaster* outRaster = overlay.runOverlayOperation();

	if(!outRaster)
	{
		QMessageBox::warning(this, tr("Warning"), overlay.getErrorMessage().c_str());
		return;
	}

	if(!utils.saveOutputRaster(overlayOutputLayerLineEdit->text().latin1(), outRaster))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Error saving raster."));

		outRaster->clear();
		delete outRaster;
	}

	outRaster->clear();
	delete outRaster;


//create legend
	TeLayer* outLayer = utils.getLayerByName(overlayOutputLayerLineEdit->text().latin1());

	if(outLayer)
	{
		HidroLUTController lutControl(outLayer);

		if(lutControl.createAttrLUTTable())
		{
			std::map<std::string, HidroOverlayClass*> classMap = _overlayClassGroup->getClasses();

			std::map<std::string, HidroOverlayClass*>::iterator it = classMap.begin();

			while(it != classMap.end())
			{
				HidroOverlayClass* classItem = it->second;
				int r = classItem->getClassColor().red_;
				int g = classItem->getClassColor().green_;
				int b = classItem->getClassColor().blue_;

				lutControl.changeClass(classItem->getClassIdx(), classItem->getClassName(), r, g, b, false);

				++it;
			}
		}
	}

//update terraView
	_parameters->updateTVInterface();

	QMessageBox::information(this, tr("Information"), tr("Overlay Done."));
}

void HidroClassWindow::crossPushButton_clicked()
{
//check input parameters
	if(referenceRasterComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Reference raster not defined."));
		return;
	}

	if(overlayOutputLayerLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Output layer name not defined."));
		return;
	}

	if(overlayListBox->numRows() == 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Input Layers not defined."));
		return;
	}

	QMessageBox::information(this, tr("Information"), tr("Cross Table does not use rules defined by user."));  

	HidroUtils utils(_parameters->getCurrentDatabasePtr());
	
	TeLayer* layer = utils.getLayerByName(referenceRasterComboBox->currentText().latin1());

	std::vector<std::string> vecList;

	QListBoxItem* item = overlayListBox->firstItem();

	while(item)
	{
		vecList.push_back(item->text().latin1());

		item = item->next();
	}

//run cross table

	HidroCrossTable crossTable(_parameters->getCurrentDatabasePtr(), layer->raster());

	if(!crossTable.executeCrossTable(vecList))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Cross Table Error."));
		return;
	}

	TeRaster* outRaster = crossTable.getOutputRaster();

	if(!outRaster)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Cross Table Error."));
		return;
	}

	if(!utils.saveOutputRaster(overlayOutputLayerLineEdit->text().latin1(), outRaster))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Error saving raster."));
		return;
	}

//create legend

	TeLayer* outLayer = utils.getLayerByName(overlayOutputLayerLineEdit->text().latin1());

	if(outLayer)
	{
		HidroLUTController lutControl(outLayer);

		if(lutControl.createAttrLUTTable())
		{
			std::map<std::string, HidroOverlayClass*> classMap = crossTable.getClassGroup()->getClasses();

			std::map<std::string, HidroOverlayClass*>::iterator it = classMap.begin();

			while(it != classMap.end())
			{
				HidroOverlayClass* classItem = it->second;
				int r = classItem->getClassColor().red_;
				int g = classItem->getClassColor().green_;
				int b = classItem->getClassColor().blue_;

				lutControl.changeClass(classItem->getClassIdx(), classItem->getClassName(), r, g, b, false);

				++it;
			}
		}
	}

//update terraView
	_parameters->updateTVInterface();

	QMessageBox::information(this, tr("Information"), tr("Cross Table Done."));  
}

void HidroClassWindow::overlayNewClassTable_pressed( int row, int, int, const QPoint & )
{
	if(row == -1)
	{
		return;
	}

	std::string className = overlayNewClassTable->text(row, 2).latin1();
	overlayNewClassLineEdit->setText(className.c_str());

	updateSelectedClass(className);
}


void HidroClassWindow::overlayGroupListView_clicked( QListViewItem * item )
{
	if(!item)
	{
		return;
	}

	std::string groupName = item->text(0).latin1();
	groupLineEdit->setText(groupName.c_str());	

	updateSelectedRuleGroup(groupName);
}


void HidroClassWindow::overlayRuleListBox_clicked( QListBoxItem * item )
{
	if(!item)
	{
		return;
	}

	std::string ruleName = item->text();

	updateSelectedRule(ruleName);
}

void HidroClassWindow::radioButton1_clicked()
{
	overlayWidgetStack->raiseWidget(0);
}


void HidroClassWindow::radioButton2_clicked()
{
	overlayWidgetStack->raiseWidget(1);
}


void HidroClassWindow::radioButton3_clicked()
{
	overlayWidgetStack->raiseWidget(2);
}


void HidroClassWindow::radioButton4_clicked()
{
	overlayWidgetStack->raiseWidget(3);
}


void HidroClassWindow::radioButton5_clicked()
{
	overlayWidgetStack->raiseWidget(4);
}


void HidroClassWindow::addLayerPushButton_clicked()
{
	QListBoxItem* item = dbListBox->firstItem();

	std::vector<int> removeVec;

	while(item)
	{
		if(item->isSelected())
		{
			overlayListBox->insertItem(item->text());

			removeVec.push_back(dbListBox->index(item));
		}
		item = item->next();
	}

	for(unsigned int i = 0; i < removeVec.size(); ++i)
	{
		dbListBox->removeItem(removeVec[i]);
	}

//update layer list
	layerListBox->clear();

	item = overlayListBox->firstItem();

	while(item)
	{
		layerListBox->insertItem(item->text());

		item = item->next();
	}
}


void HidroClassWindow::removeLayerPushButton_clicked()
{
	QListBoxItem* item = overlayListBox->firstItem();

	std::vector<int> removeVec;

	while(item)
	{
		if(item->isSelected())
		{
			dbListBox->insertItem(item->text());

			removeVec.push_back(overlayListBox->index(item));
		}
		item = item->next();
	}

	for(unsigned int i = 0; i < removeVec.size(); ++i)
	{
		overlayListBox->removeItem(removeVec[i]);
	}

//update layer list
	layerListBox->clear();

	item = overlayListBox->firstItem();

	while(item)
	{
		layerListBox->insertItem(item->text());

		item = item->next();
	}
}

////////////////////////////////////////////////////////////////////////////////////
/////////////////////////Common functions

/** \brief HidroClassWindow listLayers function.
    * Used to list all layer names from current database
	* that has TeRASTER representation and has photometric
	* equal to Pallete, inside a combo box in interface.
    */
void HidroClassWindow::listLayersLUT()
{
	if(!_parameters->getCurrentDatabasePtr())
	{
		return;
	}

//clear all layers names inside the combo box
    groupingInputLayerComboBox->clear();
	groupingInputLayerComboBox->insertItem("");

	classInputLayerComboBox->clear();
	classInputLayerComboBox->insertItem("");

	
	HidroUtils utils(_parameters->getCurrentDatabasePtr());

	std::vector<std::string> layerNames = utils.listLayersLUT();

	for(unsigned int i = 0; i < layerNames.size(); ++i)
	{
		groupingInputLayerComboBox->insertItem(layerNames[i].c_str());
		classInputLayerComboBox->insertItem(layerNames[i].c_str());
	}
}


void HidroClassWindow::listLayersRaster()
{
	if(!_parameters->getCurrentDatabasePtr())
	{
		return;
	}

//clear all layers names inside the combo box
    dbListBox->clear();
		
	referenceRasterComboBox->clear();
	referenceRasterComboBox->insertItem("");

	HidroUtils utils(_parameters->getCurrentDatabasePtr());

	std::vector<std::string> layerNames = utils.listLayers(true,false);

	for(unsigned int i = 0; i < layerNames.size(); ++i)
	{
		dbListBox->insertItem(layerNames[i].c_str());
		referenceRasterComboBox->insertItem(layerNames[i].c_str());
	}
}

void HidroClassWindow::updateSelectedClass(string name)
{
	if(name.empty())
	{
		return;
	}

	if(!_overlayClassGroup)
	{
		return;
	}

//get selected class item
	HidroOverlayClass* classItem = _overlayClassGroup->getClass(name);

	if(!classItem)
	{
		return;
	}

//update class item information
	newClassLineEdit->setText(classItem->getClassName().c_str());
	
	QColor color(classItem->getClassColor().red_,
		classItem->getClassColor().green_,
		classItem->getClassColor().blue_);

	overlayPixmapLabel->setBackgroundColor(color);

//update rule groups from this class
	overlayGroupListView->clear();

	std::map<std::string, HidroOverlayRuleGroup*> ruleGroupMap = classItem->getRuleGroups();

	std::map<std::string, HidroOverlayRuleGroup*>::iterator it = ruleGroupMap.begin();

	while(it != ruleGroupMap.end())
	{
		std::string name = it->first;

		QListViewItem* item = new QListViewItem(overlayGroupListView);
		item->setText(0, name.c_str());

		overlayGroupListView->insertItem(item);

		++it;
	}

//clear rule information
	classRuleLineEdit->setText(name.c_str());
	overlayRuleListBox->clear();
	groupLineEdit->clear();

	layerListBox->clearSelection();
	clearSelectedOperators();
	valueListBox->clearSelection();
	valueLineEdit->clear();
}

void HidroClassWindow::updateSelectedRuleGroup(string name)
{
	if(!_overlayClassGroup)
	{
		return;
	}

//get selected class item
	std::string className = overlayNewClassLineEdit->text().latin1();
	HidroOverlayClass* classItem = _overlayClassGroup->getClass(className);

	if(!classItem)
	{
		return;
	}

//get selected rule group item
	HidroOverlayRuleGroup* ruleGroupItem = classItem->getRuleGroup(name);

	if(!ruleGroupItem)
	{
		return;
	}

//clear rule information
	overlayRuleListBox->clear();
	layerListBox->clearSelection();
	clearSelectedOperators();
	valueListBox->clearSelection();
	valueLineEdit->clear();

//update rules from this rule group
	std::map<std::string, HidroOverlayRule*> ruleMap = ruleGroupItem->getRuleGroup();

	std::map<std::string, HidroOverlayRule*>::iterator it = ruleMap.begin();

	while(it != ruleMap.end())
	{
		std::string name = it->first;
		
		overlayRuleListBox->insertItem(name.c_str());
		
		++it;
	}
}

void HidroClassWindow::updateSelectedRule(string name)
{
	if(!_overlayClassGroup)
	{
		return;
	}

//get selected class item
	std::string className = overlayNewClassLineEdit->text().latin1();
	HidroOverlayClass* classItem = _overlayClassGroup->getClass(className);

	if(!classItem)
	{
		return;
	}

//get selected rule group item
	std::string ruleGroupName = groupLineEdit->text().latin1();
	HidroOverlayRuleGroup* ruleGroupItem = classItem->getRuleGroup(ruleGroupName);

	if(!ruleGroupItem)
	{
		return;
	}

//get selected rule item
	HidroOverlayRule* rule = ruleGroupItem->getRule(name);

	if(!rule)
	{
		return;
	}

//clear rule information
	layerListBox->clearSelection();
	clearSelectedOperators();
	valueListBox->clearSelection();
	valueLineEdit->clear();


//update rule 
	std::string layerName = rule->getLayer()->name();

	layerListBox->clearSelection();

	QListBoxItem* item = layerListBox->firstItem();

	while(item)
	{
		if(item->text().latin1() == layerName)
		{
			layerListBox->setSelected(item, true);
			break;
		}

		item = item->next();
	}

//check the raster layer data type
	if(rule->getLayer()->raster()->params().photometric_[0] == TeRasterParams::TePallete)
	{
		//check if the raster lut has attribute table
		HidroLUTController lutControl(rule->getLayer());

		if(lutControl.hasAttrLUTTable())
		{
			HidroAttrString* attr = (HidroAttrString*)rule->getRuleAttr();
			valueLineEdit->setText(attr->getAttrValue().c_str());
		}
		else
		{
			HidroAttrInt* attr = (HidroAttrInt*)rule->getRuleAttr();
			std::string valStr = Te2String(attr->getAttrValue());
			valueLineEdit->setText(valStr.c_str());
		}

	}
	else
	{
		if(rule->getLayer()->raster()->params().dataType_[0] == TeDOUBLE)
		{
			HidroAttrDouble* attr = (HidroAttrDouble*)rule->getRuleAttr();
			std::string valStr = Te2String(attr->getAttrValue());
			valueLineEdit->setText(valStr.c_str());
		}
		else if(rule->getLayer()->raster()->params().dataType_[0] == TeUNSIGNEDCHAR)
		{
			HidroAttrInt* attr = (HidroAttrInt*)rule->getRuleAttr();
			std::string valStr = Te2String(attr->getAttrValue());
			valueLineEdit->setText(valStr.c_str());
		}
	}

	setSelectedOperator(rule->getRuleStrComparisonOp(rule->getComparisonOperator()));
}

string HidroClassWindow::getSelectedOperator()
{
	std::string op = "";

	if(equalRadioButton->isChecked())
	{
		op = "==";
	}
	else if(diffRadioButton->isChecked())
	{
		op = "<>";
	}
	else if(moreRadioButton->isChecked())
	{
		op = ">";
	}
	else if(lessRadioButton->isChecked())
	{
		op = "<";
	}
	else if(moreEqualRadioButton->isChecked())
	{
		op = ">=";
	}
	else if(lessEqualRadioButton->isChecked())
	{
		op = "<=";
	}
    
	return op;
}


void HidroClassWindow::setSelectedOperator( string op )
{
	clearSelectedOperators();

	if(op == "==")
	{
		equalRadioButton->setChecked(true);
	}
	else if(op == "<>")
	{
		diffRadioButton->setChecked(true);
	}
	else if(op == ">")
	{
		moreRadioButton->setChecked(true);
	}
	else if(op == "<")
	{
		lessRadioButton->setChecked(true);
	}
	else if(op == ">=")
	{
		moreEqualRadioButton->setChecked(true);
	}
	else if(op == "<=")
	{
		lessEqualRadioButton->setChecked(true);
	}
}


void HidroClassWindow::clearSelectedOperators()
{
	equalRadioButton->setChecked(false);
	diffRadioButton->setChecked(false);
	moreRadioButton->setChecked(false);
	lessRadioButton->setChecked(false);
	moreEqualRadioButton->setChecked(false);
	lessEqualRadioButton->setChecked(false);
}


void HidroClassWindow::groupSuggestPushButton_clicked()
{
	if(overlayNewClassLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Class not selected."));
		return;
	}

	if(!_overlayClassGroup)
	{
		return;
	}

	std::string className = overlayNewClassLineEdit->text().latin1();

	HidroOverlayClass* classItem = _overlayClassGroup->getClass(className);

	if(!classItem)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Class not found."));
	}

	std::string newGroup = classItem->getNewGroupName();

	groupNameLineEdit->setText(newGroup.c_str());
}


void HidroClassWindow::ruleSuggestPushButton_clicked()
{
	if(groupLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Rule Group not defined."));
		return;
	}

	std::string className = overlayNewClassLineEdit->text().latin1();

	HidroOverlayClass* classItem = _overlayClassGroup->getClass(className);

	if(!classItem)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Class not found."));
	}

	std::string ruleGroupName = groupLineEdit->text().latin1();

	HidroOverlayRuleGroup* ruleGroup = classItem->getRuleGroup(ruleGroupName);

	if(!ruleGroup)
	{
		QMessageBox::warning(this, tr("Warning"), tr("Rule Group not found."));
	}

	std::string ruleName = ruleGroup->getNewRuleName();

	ruleNameLineEdit->setText(ruleName.c_str());
}