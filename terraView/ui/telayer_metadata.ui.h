/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <TeDatabase.h>
#include <TeApplicationUtils.h>
#include <mediaDescription.h>

#include <help.h>

//QT include files
#include <qmessagebox.h>
#include <qdatetimeedit.h>
#include <qtextedit.h>

std::vector<TeLayer*> getSelectedLayers(QListBox* list, const QMap<QListBoxItem*, TeLayer*>& layers)
{
	std::vector<TeLayer*> result;
	QListBoxItem* item = list->firstItem();

	while(item != NULL)
	{
		QMap<QListBoxItem*, TeLayer*>::const_iterator it = layers.find(item);

		if(it != layers.end())
			result.push_back(it.data());

		item = item->next();
	}

	return result;
}

TeTable getTeTable(QTable* dataTable)
{
	TeTable res("te_layer_metadata"); 

	TeAttributeList attr;
	TeAttribute		at;

	at.rep_.type_ = TeINT;
	at.rep_.name_ = "layer_id";
	at.rep_.isPrimaryKey_ = true;
	attr.push_back(at);

	at.rep_.isPrimaryKey_ = false;

	QCheckTableItem* item = dynamic_cast<QCheckTableItem*>(dataTable->item(0, 0));
	if(item != NULL && item->isChecked())
	{
		at.rep_.type_ = TeSTRING;
		at.rep_.name_ = "name";
		at.rep_.numChar_ = 128;
		attr.push_back(at);
	}

	item = dynamic_cast<QCheckTableItem*>(dataTable->item(1, 0));
	if(item != NULL && item->isChecked())
	{
		at.rep_.type_ = TeSTRING;
		at.rep_.name_ = "author";
		at.rep_.numChar_ = 128;
		attr.push_back(at);
	}

	item = dynamic_cast<QCheckTableItem*>(dataTable->item(2, 0));
	if(item != NULL && item->isChecked())
	{
		at.rep_.type_ = TeSTRING;
		at.rep_.name_ = "source";
		at.rep_.numChar_ = 255;
		attr.push_back(at);
	}

	item = dynamic_cast<QCheckTableItem*>(dataTable->item(3, 0));
	if(item != NULL && item->isChecked())
	{
		at.rep_.type_ = TeSTRING;
		at.rep_.name_ = "quality";
		at.rep_.numChar_ = 128;
		attr.push_back(at);
	}

	item = dynamic_cast<QCheckTableItem*>(dataTable->item(4, 0));
	if(item != NULL && item->isChecked())
	{
		at.rep_.type_ = TeSTRING;
		at.rep_.name_ = "description";
		at.rep_.numChar_ = 255;
		attr.push_back(at);
	}

	item = dynamic_cast<QCheckTableItem*>(dataTable->item(5, 0));
	if(item != NULL && item->isChecked())
	{
		at.rep_.type_ = TeSTRING;
		at.rep_.name_ = "date_";
		at.rep_.numChar_ = 64;
		attr.push_back(at);
	}

	item = dynamic_cast<QCheckTableItem*>(dataTable->item(6, 0));
	if(item != NULL && item->isChecked())
	{
		at.rep_.type_ = TeSTRING;
		at.rep_.name_ = "hour_";
		at.rep_.numChar_ = 64;
		attr.push_back(at);
	}

	item = dynamic_cast<QCheckTableItem*>(dataTable->item(7, 0));
	if(item != NULL && item->isChecked())
	{
		at.rep_.type_ = TeINT;
		at.rep_.name_ = "transf";
		attr.push_back(at);
	}

	res.setAttributeList(attr);

	return res;
}

TeTableRow getDataFromTable(QTable* table)
{
	TeTableRow res;
	res.push_back("-1");

	QCheckTableItem* item = dynamic_cast<QCheckTableItem*>(table->item(0, 0));
	if(item != NULL && item->isChecked())
		res.push_back((table->text(0, 1).isEmpty()) ? "" : table->text(0, 1).latin1());

	item = dynamic_cast<QCheckTableItem*>(table->item(1, 0));
	if(item != NULL && item->isChecked())
		res.push_back((table->text(1, 1).isEmpty()) ? "" : table->text(1, 1).latin1());

	item = dynamic_cast<QCheckTableItem*>(table->item(2, 0));
	if(item != NULL && item->isChecked())
		res.push_back((table->text(2, 1).isEmpty()) ? "" : table->text(2, 1).latin1());

	item = dynamic_cast<QCheckTableItem*>(table->item(3, 0));
	if(item != NULL && item->isChecked())
		res.push_back((table->text(3, 1).isEmpty()) ? "" : table->text(3, 1).latin1());

	item = dynamic_cast<QCheckTableItem*>(table->item(4, 0));
	if(item != NULL && item->isChecked())
		res.push_back((table->text(4, 1).isEmpty()) ? "" : table->text(4, 1).latin1());

	item = dynamic_cast<QCheckTableItem*>(table->item(5, 0));
	if(item != NULL && item->isChecked())
		res.push_back((table->text(5, 1).isEmpty()) ? "" : table->text(5, 1).latin1());
//	{
//		QDate date = ((QDateEdit*)table->cellWidget(5, 1))->date();
//
//		if(date.isValid())
//			res.push_back(date.toString(Qt::ISODate).latin1());
//	}

	item = dynamic_cast<QCheckTableItem*>(table->item(6, 0));
	if(item != NULL && item->isChecked())
		res.push_back((table->text(6, 1).isEmpty()) ? "" : table->text(6, 1).latin1());
//	{
//		QTime time = ((QTimeEdit*)table->cellWidget(6, 1))->time();
//
//		if(time.isValid())
//			res.push_back(time.toString().latin1());
//	}

	item = dynamic_cast<QCheckTableItem*>(table->item(7, 0));
	if(item != NULL && item->isChecked())
		res.push_back((table->text(7, 1).isEmpty()) ? "" : table->text(7, 1).latin1());

	return res;
}

void TelayerMetadata::all_CKB_toggled( bool status)
{
	for(int i=0; i<metadata_TBL_->numRows(); i++)
	{
		QCheckTableItem* item = dynamic_cast<QCheckTableItem*>(metadata_TBL_->item(i, 0));

		if(item != NULL)
			item->setChecked(status);
	}
}


void TelayerMetadata::execPushButton_pressed()
{
	metadata_TBL_->setCurrentCell(0, 0);

	if(((QCheckTableItem*)metadata_TBL_->item(7, 0))->isChecked())
	{
		QString v = metadata_TBL_->text(7, 1);

		if(!v.isEmpty())
		{
			bool ok;

			v.toLong(&ok);
			if(!ok)
			{
				QString msg = tr("transf should be numeric data!");
				msg += "\n" + tr("you typed") + " :" + v;
				QMessageBox::information(this, tr("Error"), msg);
				metadata_TBL_->clearCell(7, 1);
				return;
			}
		}
	}

	TeTable toInsert = getTeTable(metadata_TBL_);

	if(toInsert.attributeList().size() == 1)
	{
		QMessageBox::warning(this, tr("TerraView error"), tr("There's no metadata selected to update."));
		return;
	}
	
	TeTable toUpdate("te_layer_metadata");
	toUpdate.setAttributeList(toInsert.attributeList());

	TeDatabasePortal* portal = db_->getPortal();

	if(!portal->query("SELECT layer_id FROM te_layer_metadata"))
	{
		QMessageBox::warning(this, tr("TerraView error"), tr("Could not get layer metadata information."));
		delete portal;

		return;
	}

	std::set<int> layerIds;

	while(portal->fetchRow())
		layerIds.insert(portal->getInt(0));

	portal->freeResult();
	delete portal;

	std::vector<TeLayer*> layers = getSelectedLayers(selectLayers_LSB_, layers_);
	std::vector<TeLayer*>::iterator it;

	TeTableRow row = getDataFromTable(metadata_TBL_);

	for(it = layers.begin(); it != layers.end(); ++it)
	{
		int lId = (*it)->id();
		row[0] = Te2String(lId);

		(layerIds.find(lId) == layerIds.end()) ?
			toInsert.add(row) :
			toUpdate.add(row);
	}

	if(toInsert.size() > 0)
	{
		if(!db_->insertTable(toInsert))
		{
			QMessageBox::warning(this, tr("TerraView Error"), tr("Fail to insert data into te_layer_metadata table."));
			return;
		}
	}
	if(toUpdate.size() > 0)
	{
		if(!db_->updateTable(toUpdate))
		{
			QMessageBox::warning(this, tr("TerraView Error"), tr("Fail to update te_layer_metadata table."));
			return;
		}
	}

	accept();
}


void TelayerMetadata::previousPushButton_pressed()
{
	wizardStack_->raiseWidget(0);
}


void TelayerMetadata::closePushButton_pressed()
{
	reject();
}


void TelayerMetadata::helpPushButton_pressed()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("MetadadosMultiPlanos.htm");
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


void TelayerMetadata::addLayer_BTN__pressed()
{
	if(inputLayers_LSB_->count() == 0)
		return;

	QListBoxItem* item = inputLayers_LSB_->firstItem();
	QListBoxItem* item2 = item->next();

	if(item->isSelected())
	{
		inputLayers_LSB_->takeItem(item);
		selectLayers_LSB_->insertItem(item);
		selectLayers_LSB_->setSelected(item, false);
	}

	while(item2 != NULL)
	{
		item = item2;
		item2 = item->next();

		if(item->isSelected())
		{
			inputLayers_LSB_->takeItem(item);
			selectLayers_LSB_->insertItem(item);
			selectLayers_LSB_->setSelected(item, false);
		}
	}

	if(selectLayers_LSB_->numRows() > 0)
	{
		selectLayers_LSB_->sort();
		nextPushButton->setEnabled(true);
	}
}


void TelayerMetadata::addAllLayers_BTN__pressed()
{
	if(inputLayers_LSB_->count() == 0)
		return;

	QListBoxItem* item = inputLayers_LSB_->firstItem();
	QListBoxItem* item2 = item->next();

	inputLayers_LSB_->takeItem(item);
	selectLayers_LSB_->insertItem(item);
	selectLayers_LSB_->setSelected(item, false);

	while(item2 != NULL)
	{
		item = item2;
		item2 = item->next();
		inputLayers_LSB_->takeItem(item);
		selectLayers_LSB_->insertItem(item);
		selectLayers_LSB_->setSelected(item, false);
	}

	nextPushButton->setEnabled(true);
}


void TelayerMetadata::search_BTN__pressed()
{

}


void TelayerMetadata::removeAllLayers_BTN__pressed()
{
	if(selectLayers_LSB_->count() == 0)
		return;

	QListBoxItem* item = selectLayers_LSB_->firstItem();
	QListBoxItem* item2 = item->next();

	selectLayers_LSB_->takeItem(item);
	inputLayers_LSB_->insertItem(item);
	inputLayers_LSB_->setSelected(item, false);

	while(item2 != NULL)
	{
		item = item2;
		item2 = item->next();
		selectLayers_LSB_->takeItem(item);
		inputLayers_LSB_->insertItem(item);
		inputLayers_LSB_->setSelected(item, false);
	}

	nextPushButton->setEnabled(false);
}


void TelayerMetadata::removeLayer_BTN__pressed()
{
	if(selectLayers_LSB_->count() == 0)
		return;

	QListBoxItem* item = selectLayers_LSB_->firstItem();
	QListBoxItem* item2 = item->next();

	if(item->isSelected())
	{
		selectLayers_LSB_->takeItem(item);
		inputLayers_LSB_->insertItem(item);
		inputLayers_LSB_->setSelected(item, false);
	}

	while(item2 != NULL)
	{
		item = item2;
		item2 = item->next();

		if(item->isSelected())
		{
			selectLayers_LSB_->takeItem(item);
			inputLayers_LSB_->insertItem(item);
			inputLayers_LSB_->setSelected(item, false);
		}
	}

	if(selectLayers_LSB_->numRows() == 0)
		nextPushButton->setEnabled(false);

	inputLayers_LSB_->sort();
}


void TelayerMetadata::init( TeDatabase * db )
{
	db_ = db;
	help_ = 0;

	if(!db_->tableExist("te_layer_metadata"))
		createTeLayerMetadata(db);

	TeLayerMap lmap = db_->layerMap();
	TeLayerMap::iterator it;
                                       
	for(it = lmap.begin(); it != lmap.end(); ++it)
	{
		TeLayer* layer = it->second;
		QListBoxText* item = new QListBoxText(inputLayers_LSB_, layer->name().c_str());
		layers_[item] = layer;
	}

	TeTable metadata("te_layer_metadata");
	db_->selectTable(metadata.name(), "", metadata);

	for(unsigned int i = 0; i < metadata.size(); i++)
		layerMetadata_[atoi(metadata[i][0].c_str())] = metadata[i];

	inputLayers_LSB_->setSelectionMode(QListBox::Multi);
	selectLayers_LSB_->setSelectionMode(QListBox::Single);

	search_BTN_->hide();

	metadata_TBL_->setItem(0, 0, new QCheckTableItem(metadata_TBL_, tr("Name" )));
	metadata_TBL_->setItem(1, 0, new QCheckTableItem(metadata_TBL_, tr("Author" )));
	metadata_TBL_->setItem(2, 0, new QCheckTableItem(metadata_TBL_, tr("Source" )));
	metadata_TBL_->setItem(3, 0, new QCheckTableItem(metadata_TBL_, tr("Quality" )));
	metadata_TBL_->setItem(4, 0, new QCheckTableItem(metadata_TBL_, tr("Description" )));
	metadata_TBL_->setItem(5, 0, new QCheckTableItem(metadata_TBL_, tr("Date" )));
	metadata_TBL_->setItem(6, 0, new QCheckTableItem(metadata_TBL_, tr("Hour" )));
	metadata_TBL_->setItem(7, 0, new QCheckTableItem(metadata_TBL_, tr("Transf" )));

//	metadata_TBL_->setCellWidget(5, 1, new QDateEdit(metadata_TBL_));
//	metadata_TBL_->setCellWidget(6, 1, new QTimeEdit(metadata_TBL_));

	metadata_TBL_->setColumnStretchable(0, true);
	metadata_TBL_->setColumnStretchable(1, true);
}


void TelayerMetadata::nextPushButton_pressed()
{
	wizardStack_->raiseWidget(1);
}


void TelayerMetadata::selectLayers_LSB__pressed( QListBoxItem * item)
{
	if(item != NULL && item->isSelected())
	{
		QMap<QListBoxItem*, TeLayer*>::iterator it = layers_.find(item);

		if(it != layers_.end())
		{
			TeLayer* layer = it.data();

			QMap<int, TeTableRow>::iterator lIt = layerMetadata_.find(layer->id());

			if(lIt != layerMetadata_.end())
			{
				TeTableRow row = lIt.data();

				metadata_TBL_->setText(0, 1, row[1].c_str());
				metadata_TBL_->setText(1, 1, row[2].c_str());
				metadata_TBL_->setText(2, 1, row[3].c_str());
				metadata_TBL_->setText(3, 1, row[4].c_str());
				metadata_TBL_->setText(4, 1, row[5].c_str());
				metadata_TBL_->setText(5, 1, row[6].c_str());
				metadata_TBL_->setText(6, 1, row[7].c_str());

//				TeTime date(row[6], TeSECOND, "YYYYsMMsDD", "-");
//				QDate d(date.year(), date.month(), date.day());
//				((QDateEdit*)metadata_TBL_->cellWidget(5, 1))->setDate(d);
//
//				TeTime hour(row[7], TeSECOND, "HHsmmsSS");
//				QTime t = QTime(hour.hour(), hour.minute(), hour.second());
//				((QTimeEdit*)metadata_TBL_->cellWidget(6, 1))->setTime(t);

				metadata_TBL_->setText(7, 1, row[8].c_str());
			}
		}
	}
	else
	{
		metadata_TBL_->setText(0, 1, "");
		metadata_TBL_->setText(1, 1, "");
		metadata_TBL_->setText(2, 1, "");
		metadata_TBL_->setText(3, 1, "");
		metadata_TBL_->setText(4, 1, "");
		metadata_TBL_->setText(5, 1, "");
		metadata_TBL_->setText(6, 1, "");

		//TeTime date(row[6], TeSECOND, "YYYYsMMsDD", "-");
		//QDate d(date.year(), date.month(), date.day());
//		((QDateEdit*)metadata_TBL_->cellWidget(5, 1))->setDate(QDate());

		//TeTime hour(row[7], TeSECOND, "HHsmmsSS");
		//QTime t = QTime(hour.hour(), hour.minute(), hour.second());
//		((QTimeEdit*)metadata_TBL_->cellWidget(6, 1))->setTime(QTime());

		metadata_TBL_->setText(7, 1, "");
	}
}


void TelayerMetadata::metadataTBL_doubleClicked( int row, int col, int, const QPoint & )
{
	if((row == 2 || row == 4) && col == 1)
	{
		MediaDescription* editWidget = new MediaDescription(this, "Media description", true);
		if(row == 2)
		{
			QString cap = tr("Source");
			editWidget->setCaption(cap);
		}
		else
		{
			QString cap = tr("Description");
			editWidget->setCaption(cap);
		}

		QString val = metadata_TBL_->text(row, col);
		editWidget->descriptionTextEdit->setText(val);
		editWidget->exec();

		QString newVal = editWidget->descriptionTextEdit->text();

		delete editWidget;

		if(newVal.length() > 255)
		{
			QMessageBox::warning(this,
			tr("Warning"),
			tr("The text is longer than 255 characters!\nIt will be truncated!"));
			string aux = newVal.latin1();
			newVal = aux.substr(0, 255).c_str();
		}
		if(newVal.isEmpty())
			newVal = " ";

		metadata_TBL_->setText(row, col, newVal);
	}
}
