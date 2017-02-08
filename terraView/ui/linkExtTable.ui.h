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

#include <qpopupmenu.h>
#include <qfiledialog.h>
#include <tableProp.h>
#include <TeDatabaseUtils.h>

void LinkExtTable::init()
{
	help_ = 0;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	TeAppTheme* curAppTheme = mainWindow_->currentTheme();
	TeTheme* curTheme = (TeTheme*)curAppTheme->getTheme();
	if(curTheme->type() != TeTHEME)
		return;

	grid_ = mainWindow_->getGrid();

	externalTable->setSelectionMode(QTable::NoSelection);

    connect(externalTable->horizontalHeader(), SIGNAL(pressed(int)),
	     this, SLOT(mousePressedOnHorizontalHeaderSlot(int)));
//    connect(externalTable, SIGNAL(clicked ( int, int, int, const QPoint&)),
//	     this, SLOT(contextMenu( int, int, int, const QPoint&)));
	connect(externalTable,SIGNAL(contextMenuRequested(int, int ,const QPoint&)),
		    this,SLOT(contextMenu(int, int ,const QPoint&)));
	connect(grid_,SIGNAL(linkTable(string, string, string)),
			this, SLOT(linkTable(string, string, string)));

}


bool LinkExtTable::init( TeAppTheme * theme )
{
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
	db_ = mainWindow_->currentDatabase();
	if(!db_)
		return false;
    theme_ = theme;
	layer_ = ((TeTheme*)theme_->getTheme())->layer();
	grid_ = mainWindow_->getGrid();

	string sel = "SELECT attr_table FROM te_layer_table WHERE layer_id is null";
	TeDatabasePortal* portal = db_->getPortal();

	externalTablesComboBox->clear();
	if(portal->query(sel))
	{
		while(portal->fetchRow())
			externalTablesComboBox->insertItem(portal->getData(0));
	}
	delete portal;
	if(externalTablesComboBox->count() == 0)
	{
		QMessageBox::warning(this, tr("Warning"),
			tr("There is no external table in the database! Please, import one."));
		return false;
	}
	
	externalTablesComboBox->setCurrentItem(0);
	externalTablesComboBox_activated(externalTablesComboBox->currentText());
	return true;
}


void LinkExtTable::externalTablesComboBox_activated( const QString & tab )
{
	TeWaitCursor wait;

	if(tab.isEmpty())
		return;

	string name = tab.latin1();

	externalTable->freePortal();
	externalTable->openTable(db_, name);
}


void LinkExtTable::mousePressedOnHorizontalHeaderSlot( int col)
{
	QString name = externalTablesComboBox->currentText();
	if(name.isEmpty())
		return;

	TeAttributeList& atl = externalTable->getPortal()->getAttributeList();

	string fname = atl[col].rep_.name_;
	string table = name.latin1();
	string t = "table:" + table + "-fieldName:" + fname;

	QTextDrag* tDrag = new QTextDrag(t.c_str(), this, "linkExtTable");
	tDrag->drag();
}

void LinkExtTable::contextMenu(int /* row */, int /* col */, const QPoint& p)
{
	QPopupMenu* popup = new QPopupMenu;

	popup->insertItem(QString(tr("&Properties...")), this, SLOT(popupPropertiesSlot()));
	popup->insertItem(QString(tr("Save ASC...")), this, SLOT(popupSaveAscSlot()));
	popup->move(p.x(), p.y());

	popup->exec();
}

void LinkExtTable::popupPropertiesSlot()
{
	TableProperties* prop = new TableProperties(this, "Table Properties", true);
	string table = externalTablesComboBox->currentText().latin1();
	prop->init(table);
	prop->exec();
	delete prop;
}

void LinkExtTable::popupSaveAscSlot()
{
	string	ss;
    QString s = QFileDialog::getSaveFileName(
                    "",
                    tr("File (*.txt)"),
                    this,
                    tr("save file dialog"),
                    tr("Choose a filename to save under"));

	if(s.isNull() || s.isEmpty())
		return;

	if(s.findRev(".txt", -1, false) == -1)
		s.append(".txt");

	string file = s.latin1();
	FILE* fp = fopen(file.c_str(), "r");
	if(fp)
	{
		fclose(fp);
		int response = QMessageBox::question(this, tr("File already exists"),
				 tr("Do you want to overwrite the file?"),
				 tr("Yes"), tr("No"));

		if (response != 0)
			return;
	}
	if((fp = fopen(file.c_str(), "w")) == 0)
	{
		QMessageBox::critical(this, tr("Error"), tr("The file cannot be opened!"));
		return;
	}

	string table = externalTablesComboBox->currentText().latin1();

	TeDatabasePortal* portal = db_->getPortal();
	string sel = "SELECT * FROM " + table;

	if(portal->query(sel))
	{
		unsigned int i;
		TeAttributeList& atl = portal->getAttributeList();
		ss.clear();
		for(i=0; i<atl.size(); i++)
			ss += "_" + atl[i].rep_.name_ + ";";
		ss.replace(ss.size()-1, 1, "\n");
		if(fwrite(ss.c_str(), sizeof( char ), ss.size(), fp) < ss.size())
		{
			fclose(fp);
			delete portal;
			QMessageBox::critical(this, tr("Error"), tr("Fail to save the table!"));
			return;
		}

		int nfields = portal->numFields();
		while(portal->fetchRow())
		{
			ss.clear();
			for(i=0; i<(unsigned int)nfields; i++)
			{
				ss += portal->getData(i);
				ss += ";";
			}
			ss.replace(ss.size()-1, 1, "\n");
			if(fwrite(ss.c_str(), sizeof( char ), ss.size(), fp) < ss.size())
			{
				fclose(fp);
				delete portal;
				QMessageBox::critical(this, tr("Error"), tr("Fail to save the table!"));
				return;
			}
		}
	}
	fclose(fp);
	delete portal;

	QMessageBox::information(this, tr("Save As txt"),
		tr("The save operation was executed successfully!"));

}


void LinkExtTable::linkTable(string dragTableName, string dragField, string dropField)
{
	unsigned int i, index;
	TeAttrTableVector attrTableVector;
	((TeTheme*)theme_->getTheme())->getAttTables(attrTableVector);

	if(dragField.empty() || dropField.empty() || dragTableName.empty())
	{
		QMessageBox::critical(this, "Error",
			tr("It is not possible to accomplish the linking operation due to parameters missing!"));
		return;
	}

	// Get the drop table corresponding to the dropField
	index = theme_->getTableIndexFromField(dropField);
	TeTable& dropTable = attrTableVector[index];
	
	// Check if the drop field name is composed with its table name;
	// in positive case remove the table name
	unsigned int idx = dropField.rfind(".");
	if (idx != string::npos)
		dropField = dropField.substr(idx+1);

	TeAttributeList attrList;
	TeAttrDataType dragAttrType, dropAttrType;

	// Get the type of the field of the drag table
	db_->getAttributeList(dragTableName, attrList);
	for (i = 0; i < attrList.size(); ++i)
	{
		if(TeConvertToLowerCase(attrList[i].rep_.name_) == TeConvertToLowerCase(dragField))
		{
			dragAttrType = attrList[i].rep_.type_;
			break;
		}
	}

	// Get the type of the field of the drop table
	attrList.clear();
	db_->getAttributeList(dropTable.name(), attrList);
	for (i = 0; i < attrList.size(); ++i)
	{
		if(TeConvertToLowerCase(attrList[i].rep_.name_) == TeConvertToLowerCase(dropField))
		{
			dropAttrType = attrList[i].rep_.type_;
			break;
		}
	}

	if (dragAttrType != dropAttrType)
	{
		QMessageBox::warning(this, tr("Warning"), 
			tr("The linking operation cannot be accomplished due to the incompatible types of the fields!"));
		return;
	}

	// Check if there is a link that already exists between the drag and drop tables
	for (i = 0; i < attrTableVector.size(); ++i)
	{
		TeTable table = attrTableVector[i];

		if (table.tableType() == TeAttrExternal && 
			table.name() == dragTableName &&
			table.linkName() == dragField &&
			table.relatedTableId() == dropTable.id() &&
			table.relatedAttribute() == dropField)
		{
			QMessageBox::warning(this, tr("Warning"),
						tr("The link between the tables already exists! Try another link."));
			return;
		}
	}

	// Add the table with the new link to the theme tables list

	// Get the fields from the drag(external) table
	string sql = "SELECT table_id, unique_id FROM te_layer_table";
	sql += " WHERE attr_table = '" + dragTableName + "'";
	TeDatabasePortal *portal = db_->getPortal();
	int tableId;
	string uniqueId;
	if (portal->query(sql) && portal->fetchRow())
	{
		tableId = portal->getInt(0);
		uniqueId = portal->getData(1);
	}
	delete portal;

	attrList.clear();
	db_->getAttributeList(dragTableName, attrList);

	TeTable newTable(dragTableName, attrList, uniqueId);
	newTable.setId(tableId);
	newTable.setTableType(TeAttrExternal, dropTable.id(), dropField);
	newTable.setLinkName(dragField);
	newTable.relatedTableName(dropTable.name());

	// Insert the table into the database, where its order will also be set 
	db_->insertThemeTable((TeTheme*)theme_->getTheme(), newTable);
	((TeTheme*)theme_->getTheme())->addThemeTable(newTable);

	grid_->clear();
	if (!((TeTheme*)theme_->getTheme())->createCollectionAuxTable() || !((TeTheme*)theme_->getTheme())->populateCollectionAux())
	{
		QMessageBox::information(this, tr("Error"),
		tr("Fail to mount the auxiliary table of the collection!"));
		return;
	}
	grid_->init(theme_);
	externalTable->deletePortal();
	hide();
}

void LinkExtTable::closeEvent( QCloseEvent * )
{
	externalTable->deletePortal();
	hide();
}


void LinkExtTable::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("linkExtTable.htm");
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


void LinkExtTable::show()
{
	TeAppTheme* curAppTheme = mainWindow_->currentTheme();
	TeTheme* curTheme = (TeTheme*)curAppTheme->getTheme();
	if(curTheme->type() == TeTHEME)
		QDialog::show();
	else
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		reject();
		hide();
	}
}
