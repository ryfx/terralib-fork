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

#include <terraViewBase.h>
#include <TeWaitCursor.h>
#include <display.h>


void TextRep::init()
{
	help_ = 0;

	unsigned int i;

	mainWindow_ = (TerraViewBase*)qApp->mainWidget();

	theme_ = mainWindow_->currentTheme();
	TeTheme* baseTheme = (TeTheme*)theme_->getTheme();
	if(baseTheme->type() != TeTHEME)
		return;

	layer_ = baseTheme->layer();
	db_ = layer_->database();
	canvas_ = mainWindow_->getCanvas();

	selectedAttributesListBox->clear();
	tableAttributesListBox->clear();
	TeAttributeList atr = baseTheme->sqlAttList();
	for(i=0; i<atr.size(); i++)
		tableAttributesListBox->insertItem(atr[i].rep_.name_.c_str());	

	TeRepresPointerVector repv;
	layer_->getRepresentation(TeTEXT, repv);

// replace operation disabled....
	if(repv.size() == 0)
	{
		createRadioButton->setEnabled(true);
		replaceRadioButton->setEnabled(false);
		selectRadioButton->setEnabled(false);
		deleteRadioButton->setEnabled(false);
		createRadioButton->setChecked(true);
		operationButtonGroup_clicked(0);
	}
	else
	{
		createRadioButton->setEnabled(true);
		replaceRadioButton->setEnabled(false);
		selectRadioButton->setEnabled(true);
		deleteRadioButton->setEnabled(true);

		if(theme_->textTable().empty())
		{
			createRadioButton->setChecked(true);
			operationButtonGroup_clicked(0);
		}
		else if(db_->tableExist(theme_->textTable()) == false)
		{
			createRadioButton->setChecked(true);
			operationButtonGroup_clicked(0);
		}
		else
		{
			selectRadioButton->setChecked(true);
			operationButtonGroup_clicked(1);
		}

//		layer_->getRepresentation(TeTEXT, repv);
		tableComboBox->clear();

		TeRepresPointerVector::iterator it;

		for(it = repv.begin(); it != repv.end(); ++it)
			tableComboBox->insertItem((*it)->tableName_.c_str());
	}
}


void TextRep::operationButtonGroup_clicked(int op)
{
	if (op == 0)		//create
	{
		addToSelectedPushButton->setEnabled(false);
		removeSelectedPushButton->setEnabled(false);
		tableNameLineEdit->setEnabled(true);
		tableAttributesListBox->setEnabled(true);
		tableAttributesListBox->clearSelection();
		selectedAttributesListBox->setEnabled(true);
		selectedAttributesListBox->clearSelection();
		tableTextLabel->setEnabled(false);
		tableAttributesTextLabel->setEnabled(true);
		funcComboBox->setEnabled(false);
		selectedAttributesTextLabel->setEnabled(true);
		tableNameTextLabel->setEnabled(true);
		tableComboBox->setEnabled(false);
		tableComboBox_activated("clear");
	}
	else if (op == 1)	//select
	{
		addToSelectedPushButton->setEnabled(false);
		removeSelectedPushButton->setEnabled(false);
		tableNameLineEdit->setEnabled(false);
		tableAttributesListBox->setEnabled(false);
		selectedAttributesListBox->setEnabled(false);
		tableTextLabel->setEnabled(true);
		tableAttributesTextLabel->setEnabled(false);
		selectedAttributesTextLabel->setEnabled(false);
		tableNameTextLabel->setEnabled(false);
		tableComboBox->setEnabled(true);
		tableComboBox_activated("init");
	}
	else if (op == 2)  //replace
	{
		addToSelectedPushButton->setEnabled(true);
		removeSelectedPushButton->setEnabled(true);
		tableNameLineEdit->setEnabled(false);
		tableTextLabel->setEnabled(true);
		tableAttributesTextLabel->setEnabled(true);
		selectedAttributesTextLabel->setEnabled(true);
		tableNameTextLabel->setEnabled(true);
		tableComboBox->setEnabled(true);
		tableComboBox_activated("init");	
	}
	else				//remove
	{
		addToSelectedPushButton->setEnabled(false);
		removeSelectedPushButton->setEnabled(false);
		tableNameLineEdit->setEnabled(false);
		tableAttributesListBox->setEnabled(false);
		selectedAttributesListBox->setEnabled(false);
		tableTextLabel->setEnabled(true);
		tableAttributesTextLabel->setEnabled(false);
		selectedAttributesTextLabel->setEnabled(false);
		tableNameTextLabel->setEnabled(false);
		tableComboBox->setEnabled(true);
		tableComboBox_activated("init");
	}
}


void TextRep::tableComboBox_activated(const QString& qs)
{
	int	i;

	if(qs == "clear")
		tableComboBox->clear();
	else if(qs == "init")
	{
		TeRepresPointerVector repv;
		layer_->getRepresentation(TeTEXT, repv);
		tableComboBox->clear();
		int i;
		for(i=0; i<int(repv.size()); i++)
			tableComboBox->insertItem(repv[i]->tableName_.c_str());

		if(tableComboBox->count())
		{
			string tt = theme_->textTable();
			if(tt.empty())
				tableComboBox->setCurrentItem(0);
			else
			{
				for(i=0; i<tableComboBox->count();i++)
				{
					string s = tableComboBox->text(i).latin1();
					if(tt == s)
					{
						tableComboBox->setCurrentItem(i);
						break;
					}
				}
			}
		}
	}

	tableAttributesListBox->clear();
	TeAttributeList atr = ((TeTheme*)theme_->getTheme())->sqlAttList();
	for(i=0; i<int(atr.size()); i++)
		tableAttributesListBox->insertItem(atr[i].rep_.name_.c_str());

	if(!(qs == "remove"))
	{
		selectedAttributesListBox->clear();
		if(tableComboBox->currentText().isEmpty())
			return;
		string	table = tableComboBox->currentText().latin1();
		
		TeRepresentation* trep = layer_->getRepresentation(TeTEXT, table);
		string descr = trep->description_;
		while(descr.size())
		{
			int f = descr.find(", ");
			if(f >= 0)
			{
				string s = descr;
				s = s.substr(0, f);
				selectedAttributesListBox->insertItem(s.c_str());
				descr = descr.substr(f+2);
			}
			else
			{
				selectedAttributesListBox->insertItem(descr.c_str());
				descr.clear();
			}
		}
	}

	i = 0;
	while(i < (int)selectedAttributesListBox->count())
	{
		string s = selectedAttributesListBox->text(i).latin1();
		int j = 0;
		while(j < (int)tableAttributesListBox->count())
		{
			string ss = tableAttributesListBox->text(j).latin1();
			if(s == ss)
			{
				tableAttributesListBox->removeItem(j);
				break;
			}
			j++;
		}
		i++;
	}
}


void TextRep::addToSelectedPushButton_clicked()
{
	if(tableAttributesListBox->currentText().isEmpty())
		return;
	string s = tableAttributesListBox->currentText().latin1();
	selectedAttributesListBox->insertItem(s.c_str());
}


void TextRep::removeSelectedPushButton_clicked()
{
	if(selectedAttributesListBox->currentText().isEmpty())
		return;
	string s = selectedAttributesListBox->currentText().latin1();
	int item = selectedAttributesListBox->currentItem();
	selectedAttributesListBox->removeItem(item);

//	tableComboBox_activated("remove");
}


void TextRep::okPushButton_clicked()
{
	TeWaitCursor wait;
	unsigned int	n;
	vector<string> vn;

	TeTheme* baseTheme = (TeTheme*)theme_->getTheme();

	if(createRadioButton->isChecked())
	{
		if(tableNameLineEdit->text().isEmpty())
		{
			wait.resetWaitCursor();
			QMessageBox::warning(this, tr("Warning"),
				tr("Enter with the name of the representation table!"));
			return;
		}
		string tableName = tableNameLineEdit->text().latin1();
		string table = db_->getNewTableName(tableName);
		if(table != tableName)
		{
			wait.resetWaitCursor();
			QString msg = tr("There is already a table with this name or it contains invalid characters.");
			msg += "\n";
			msg += tr("The name will be changed automatically.");
			QMessageBox::warning(this, tr("Warning"), msg);
			wait.setWaitCursor();				
		}
		tableNameLineEdit->setText(table.c_str());

		if (db_->tableExist(table))
		{
			wait.resetWaitCursor();
			QMessageBox::warning(this, tr("Warning"),
				tr("There is already a table with this name in the database! Give another name."));
			return;
		}

		string descr;
		for(n=0; n<selectedAttributesListBox->count(); n++)
		{
			string cname = selectedAttributesListBox->text(n).latin1();
			size_t f = cname.find("All(");
			if(f != string::npos)
			{
				string s = cname;
				cname = s.substr(0, f) + s.substr(f+4);
				size_t ff = cname.find(")");
				if(ff != string::npos)
				{
					s = cname;
					cname = s.substr(0, ff) + s.substr(f+1);
				}
			}

			vn.push_back(cname);
			descr += cname + ", ";
		}
		if(descr.empty())
			vn.push_back("");

		if(descr.size() >= 2)
			descr = descr.substr(0, descr.size()-2);
		if(layer_->addGeometry(TeTEXT, table, descr) == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"),
				tr("Fail to create the text representation!"));
			return;
		}

		TeBox box = layer_->box();
		string uprep = "UPDATE te_representation SET";
		uprep += " lower_x = " + Te2String(box.x1_);
		uprep += ", lower_y = " + Te2String(box.y1_);
		uprep += ", upper_x = " + Te2String(box.x2_);
		uprep += ", upper_y = " + Te2String(box.y2_);
		uprep += " WHERE layer_id = " + Te2String(layer_->id());
		uprep += " AND geom_table = '" + table + "'";
		db_->execute(uprep);

		// create temporary table
		if(db_->tableExist("tmptxtable"))
			db_->execute("DROP TABLE tmptxtable");

		TeAttributeList atl;
		TeAttribute		at;

		at.rep_.name_ = "t_id";
		at.rep_.type_ = TeSTRING;
		at.rep_.numChar_ = 50;
		at.rep_.isPrimaryKey_ = true;
		atl.push_back(at);

		at.rep_.isPrimaryKey_ = false;
		at.rep_.name_ = "t_x";
		at.rep_.type_ = TeREAL;
		at.rep_.decimals_ = 9;
		atl.push_back(at);

		at.rep_.name_ = "t_y";
		atl.push_back(at);

		// calculate insertion coordinates
		TeBox dataBox = canvas_->getDataWorld();
		if(dataBox.isValid() == false)
		{
			canvas_->setProjection(mainWindow_->currentView()->projection());
			canvas_->setDataProjection(layer_->projection());
//			canvas_->setWorld(layer_->box());
			canvas_->setWorld(layer_->box(), mainWindow_->cvContentsW_, mainWindow_->cvContentsH_);
			dataBox = canvas_->getDataWorld();
		}
		TeCoord2D pc = dataBox.center();
		TeText tx(pc, "qP");
		TeVisual* visual = baseTheme->defaultLegend().visual(TeTEXT);
		TeColor cor = visual->color();
		int rgb = cor.blue_ | (cor.green_ << 8) | (cor.red_ << 16);
		string family = visual->family();
		bool bold = visual->bold();
		bool italic = visual->italic();
		int tsize = visual->size();
		double th;
		if(visual->fixedSize() == false)
			th = canvas_->mapVtoCW(tsize) * 1.8;
		else
		{
			QRect rect = canvas_->textRect (tx, *visual);
			th = canvas_->mapVtoCW(rect.height());
		}

		db_->createTable("tmptxtable", atl);

		string CT = baseTheme->collectionTable();
		string ins = "INSERT INTO tmptxtable (t_id, t_x, t_y) ";
		ins += "SELECT MIN(" + CT + ".c_object_id), MIN(" + CT + ".label_x), MIN(" + CT + ".label_y) + ((";
		int ff = vn[0].find("(");
		if(ff >= 0)
			ins += " 1 ";
		else
			ins += "COUNT(" + vn[0] + ")";
		for(n=1; n<vn.size(); n++)
		{
			ff = vn[n].find("(");
			if(ff >= 0)
				ins += " + 1 ";
			else
				ins += " + COUNT(" + vn[n] + ")";
		}
		ins += ") - 1) * " + Te2String(th) + " / 2" + baseTheme->sqlFrom();
		if(db_->dbmsName()=="Ado")			ins += " where " + CT + ".c_object_id <> ''";//;is not null";
		else								ins += " where " + CT + ".c_object_id is not null";

		ins += " GROUP BY " + CT + ".c_object_id";
		
		db_->execute(ins);

		string sqlFrom = baseTheme->sqlFrom();
		string scol = baseTheme->collectionTable() + ".c_object_id";
//		ff = sqlFrom.find(scol, string::npos);
		ff = sqlFrom.find(scol);
		while(ff >= 0)
		{
			sqlFrom.replace(ff, scol.size(), "tmptxtable.t_id");
//			ff = sqlFrom.find(scol, string::npos);
			ff = sqlFrom.find(scol);
		}

		string CTspace = baseTheme->collectionTable() + " ";
//		ff = sqlFrom.find(CTspace, string::npos);
		ff = sqlFrom.find(CTspace);
		while(ff >= 0)
		{
			sqlFrom.replace(ff, CTspace.size(), "tmptxtable ");
//			ff = sqlFrom.find(CTspace, string::npos);
			ff = sqlFrom.find(CTspace);
		}

		string CTpoint = baseTheme->collectionTable() + ".";
//		ff = sqlFrom.find(CTpoint, string::npos);
		ff = sqlFrom.find(CTpoint);
		while(ff >= 0)
		{
			sqlFrom.replace(ff, CTspace.size(), "tmptxtable.");
//			ff = sqlFrom.find(CTpoint, string::npos);
			ff = sqlFrom.find(CTpoint);
		}

		ff = vn[0].find("(");
		if(ff >= 0)
		{
			ins = "INSERT INTO " + table + " (object_id, x, y, text_value)";
			if (db_->dbmsName() == "Ado") // only ADO ACCESS
			{
				ins += " SELECT MIN(tmptxtable.t_id), MIN(tmptxtable.t_x), MIN(tmptxtable.t_y), " + vn[0] + sqlFrom;
				ins += " GROUP BY tmptxtable.t_id";
				ins += " ORDER BY tmptxtable.t_id, " + vn[0];
			}
			else
			{
				ins += " SELECT MIN(tmptxtable.t_id), MIN(tmptxtable.t_x), MIN(tmptxtable.t_y), " + vn[0] + " AS VN " + sqlFrom;
				ins += " GROUP BY tmptxtable.t_id";
				ins += " ORDER BY tmptxtable.t_id, VN";
			}
		}
		else
		{
			ins = "INSERT INTO " + table + " (object_id, x, y, text_value)";
			ins += " SELECT tmptxtable.t_id, tmptxtable.t_x, tmptxtable.t_y, " + vn[0] + sqlFrom;
			ins += " ORDER BY tmptxtable.t_id, " + vn[0];
		}

		db_->execute(ins);

		string up = "UPDATE (SELECT A.y, B.y FROM " + table + " as A, " + table + " as B";
		up += " WHERE A.geom_id = B.geom_id + 1 AND A.object_id = B.object_id";
		up += " ORDER BY A.object_id, A.geom_id)";
		up += " SET A.y = B.y - " + Te2String(th);

		if (db_->dbmsName() == "Ado") // only ADO ACCESS
			db_->execute(up);

		for(n=1; n<vn.size(); n++)
		{
			string gid;
			string sel = "SELECT MAX(geom_id) FROM " + table;
			TeDatabasePortal* portal = db_->getPortal();
			if(portal->query(sel) && portal->fetchRow())
				gid = portal->getData(0);
			else
			{
				delete portal;
				string dp = "DROP TABLE " + table;
				db_->execute(dp);
				wait.resetWaitCursor();
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to create the text representation!"));
				return;
			}
			delete portal;

			db_->execute("DELETE FROM tmptxtable");

			ins = "INSERT INTO tmptxtable (t_id, t_x, t_y)";
			if (db_->dbmsName() == "Ado") // only ADO ACCESS
			{
				ins += " SELECT MAX(object_id) , MAX(x), MIN(y) - " + Te2String(th);
				ins += " FROM " + table;
				ins += " GROUP BY object_id ORDER BY MAX(object_id)";
			}
			else
			{
				ins += " SELECT MAX(object_id) AS OID, MAX(x), MIN(y) - " + Te2String(th);
				ins += " FROM " + table;
				ins += " GROUP BY object_id ORDER BY OID";
			}

			db_->execute(ins);

			ff = vn[n].find("(");
			if(ff >= 0)
			{
				ins = "INSERT INTO " + table + " (object_id, x, y, text_value)";
				if (db_->dbmsName() == "Ado") // only ADO ACCESS
				{
					ins += " SELECT MIN(tmptxtable.t_id), MIN(tmptxtable.t_x), MIN(tmptxtable.t_y), " + vn[n] + sqlFrom;
					ins += " GROUP BY tmptxtable.t_id";
					ins += " ORDER BY tmptxtable.t_id, " + vn[n];
				}
				else
				{
					ins += " SELECT MIN(tmptxtable.t_id), MIN(tmptxtable.t_x), MIN(tmptxtable.t_y), " + vn[n] + " AS VN " + sqlFrom;
					ins += " GROUP BY tmptxtable.t_id";
					ins += " ORDER BY tmptxtable.t_id, VN";
				}
			}
			else
			{
				ins = "INSERT INTO " + table + " (object_id, x, y, text_value)";
				ins += " SELECT tmptxtable.t_id, tmptxtable.t_x, tmptxtable.t_y, " + vn[n] + sqlFrom;
				ins += " ORDER BY tmptxtable.t_id, " + vn[n];
			}

			db_->execute(ins);

			up = "UPDATE (SELECT A.y, B.y FROM " + table + " as A, " + table + " as B";
			up += " WHERE A.geom_id = B.geom_id + 1 AND A.object_id = B.object_id AND A.geom_id > " + gid;
			up += " ORDER BY A.object_id, A.geom_id)";
			up += " SET A.y = B.y - " + Te2String(th);

			if (db_->dbmsName() == "Ado") // only ADO ACCESS
			db_->execute(up);
		}

		// delete temporary table
		db_->deleteTable("tmptxtable");

		// update angle, height an alignments
		string popule = "UPDATE " + table + " SET angle = 0";
		popule += ", height = " + Te2String(th/1.8) + ", alignment_vert = 2, alignment_horiz = 2";
		if(db_->execute(popule) == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"),
				tr("Fail to generate the text representation!"));
			return;
		}

		string tvis = table + "_txvisual";
		atl.clear();

		at.rep_.name_ = "geom_id";
		at.rep_.type_ = TeINT;
		at.rep_.numChar_ = 0;
		at.rep_.isPrimaryKey_ = true;
		atl.push_back(at);

		at.rep_.isPrimaryKey_ = false;
		at.rep_.name_ = "dot_height";
		at.rep_.type_ = TeINT;
		atl.push_back(at);

		at.rep_.name_ = "fix_size";
		at.rep_.type_ = TeINT;
		atl.push_back(at);

		at.rep_.name_ = "color";
		at.rep_.type_ = TeINT;
		atl.push_back(at);

		at.rep_.name_ = "family";
		at.rep_.type_ = TeSTRING;
		at.rep_.numChar_ = 128;
		atl.push_back(at);

		at.rep_.name_ = "bold";
		at.rep_.type_ = TeINT;
		at.rep_.numChar_ = 0;
		atl.push_back(at);

		at.rep_.name_ = "italic";
		at.rep_.type_ = TeINT;
		atl.push_back(at);

		db_->createTable(tvis, atl);
		string fk = "fk_" + tvis;
		db_->createRelation(fk, tvis, "geom_id", table, "geom_id", true);
		ins = "INSERT INTO " + tvis + " (geom_id) SELECT geom_id FROM " + table;
		db_->execute(ins);

		popule = "UPDATE " + tvis;
		popule += " SET dot_height = " + Te2String(tsize);
		popule += ", fix_size = " + Te2String(visual->fixedSize());
		popule += ", color = " + Te2String(rgb);
		popule += ", family = '" + family + "'";
		popule += ", bold = " + Te2String(bold);
		popule += ", italic = " + Te2String(italic);
		if(db_->execute(popule) == false)
		{
			wait.resetWaitCursor();
			QMessageBox::critical(this, tr("Error"),
				tr("Fail to generate the visual table of the text!"));
			return;
		}

		theme_->textTable(table);
		string s = "UPDATE te_theme_application SET text_table = '" + table + "'";
		s += " WHERE theme_id = " + Te2String(baseTheme->id());
		db_->execute(s);
		wait.resetWaitCursor();
		int response = QMessageBox::question(this, tr("Question"),
			tr("The text representation was generated successfully!\nDo you want to redraw?"),
				 tr("Yes"), tr("No"));

		if (response == 0)
		{
			long vrep = baseTheme->visibleRep();
			vrep |= TeTEXT;
			baseTheme->visibleRep(vrep);
			mainWindow_->getDisplayWindow()->plotData();
		}
	}
	else if(selectRadioButton->isChecked())
	{
		string tab = theme_->textTable();
		if(tableComboBox->currentText().isEmpty())
		{
			wait.resetWaitCursor();
			QMessageBox::warning(this, tr("Warning"),
				tr("Select a representation table!"));
			return;
		}
		string table = tableComboBox->currentText().latin1();
		{
			if(!(tab == table))
			{
				theme_->textTable(table);
				string s = "UPDATE te_theme_application SET text_table = '" + table + "'";
				s += " WHERE theme_id = " + Te2String(baseTheme->id());
				db_->execute(s);
				wait.resetWaitCursor();
				int response = QMessageBox::question(this, tr("Question"),
					tr("The table was changed successfully!\nDo you want to redraw?"),
						 tr("Yes"), tr("No"));

				if (response == 0)
				{
					long vrep = baseTheme->visibleRep();
					vrep |= TeTEXT;
					baseTheme->visibleRep(vrep);
					mainWindow_->getDisplayWindow()->plotData();
				}
			}
			else
			{
				long vrep = baseTheme->visibleRep();
				if((vrep & TeTEXT) == 0)
				{
					vrep |= TeTEXT;
					baseTheme->visibleRep(vrep);
					mainWindow_->getDisplayWindow()->plotData();
				}
			}
		}
	}
	else if(deleteRadioButton->isChecked())
	{
		if(tableComboBox->currentText().isEmpty())
		{
			wait.resetWaitCursor();
			QMessageBox::warning(this, tr("Warning"),
				tr("Select a representation table!"));
			return;
		}
		string table = tableComboBox->currentText().latin1();
		string tabv = table + "_txvisual";
		string drop = "DROP TABLE " + tabv;
		db_->execute(drop);

		if(layer_->removeGeometry(TeTEXT, table))
		{
			theme_->textTable("");
			string s = "UPDATE te_theme_application SET text_table = null";
			s += " WHERE theme_id = " + Te2String(baseTheme->id());
			db_->execute(s);

			int v = baseTheme->visibleRep() & (TeTEXT ^ 0xffffffff);
			baseTheme->visibleRep(v);
			s = "UPDATE te_theme_application SET visible_rep = " + Te2String(v);
			s += " WHERE theme_id = " + Te2String(baseTheme->id());
			db_->execute(s);

			wait.resetWaitCursor();
			int response = QMessageBox::question(this, tr("Question"),
				tr("The table was removed successfully!\nDo you want to redraw?"),
					 tr("Yes"), tr("No"));

			if (response == 0)
				mainWindow_->getDisplayWindow()->plotData();
			else
			{
				tableComboBox->removeItem(tableComboBox->currentItem());
				selectedAttributesListBox->clear();
				tableComboBox_activated("init");
				return;
			}
		}
		else
		{
			wait.resetWaitCursor();
			QMessageBox::warning(this, tr("Warning"),
				tr("The text representation table could not be removed!\nCheck whether it is being used in another application."));
			return;
		}
	}

	string s = "UPDATE te_theme SET visible_rep = " + Te2String(baseTheme->visibleRep());
	s += " WHERE theme_id = " + Te2String(baseTheme->id());
	db_->execute(s);

	TeBox& bgBox = mainWindow_->getChangedBox();
	bgBox = TeBox();
	hide();
}


void TextRep::generateInsertionPoint()
{
	// calculate t_x and t_y
	if (layer_->hasGeometry(TeCELLS))
	{
		string TCEL = layer_->tableName(TeCELLS);

		if (db_->dbmsName() == "MySQL")
		{
			string query = "SELECT t_id,";
			query += "(lower_x + (upper_x-lower_x)/2.) AS lbx,";
			query += "(lower_y + (upper_y-lower_y)/2.) AS lby";
			query += " FROM tmptxtable LEFT JOIN " + TCEL;
			query += " ON tmptxtable.t_id = " + TCEL + ".object_id";

			TeDatabasePortal *pt = db_->getPortal();
			if (pt->query(query) == false)
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to mount the collection table: label_x and label_y!"));
				delete pt;
				return;
			}

			bool bb = pt->fetchRow();
			string id,setClause,lbx,lby,upd;
			while(bb)
			{
				id  = pt->getData("t_id");
				lbx = pt->getData("lbx");
				lby = pt->getData("lby");

				upd = "UPDATE tmptxtable SET";					
				upd += " t_x = '" + lbx + "',"; 
				upd += " t_y = '" + lby + "'";
				upd += " WHERE t_id = '" + id + "'";

				if (db_->execute(upd) == false)
				{
					QMessageBox::critical(this, tr("Error"),
						tr("Fail to mount the collection table: label_x and label_y!"));
					delete pt;
					return;
				}
				bb = pt->fetchRow();
			}
			delete pt;			
		}

		else if(db_->dbmsName() == "Oracle")
		{
			string upd= "UPDATE tmptxtable SET ";
			upd += " t_x = (SELECT MAX(lower_x + (upper_x - lower_x)/2) ";
			upd += "FROM " + TCEL + " WHERE object_id = t_id) ";
			upd += "WHERE t_x IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to calculate label_x for cells!"));
				return;
			}
			
			//label_y
			//upd += " label_x = lower_x + (upper_x-lower_x)/2,";
			upd= "UPDATE tmptxtable SET ";
			upd += " t_y = (SELECT MAX(lower_y + (upper_y - lower_y)/2) ";
			upd += "FROM " + TCEL + " WHERE object_id = t_id) ";
			upd += "WHERE t_y IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to calculate label_y for cells!"));
				return;
			}

		}				
		
		else if(db_->dbmsName() == "OracleSpatial")
		{
			//t_x
			string upd= "UPDATE tmptxtable SET ";
			upd += " t_x = (SELECT MAX(SDO_GEOM.SDO_MIN_MBR_ORDINATE( ";
			upd += TCEL + ".spatial_data, 1) + (SDO_GEOM.SDO_MAX_MBR_ORDINATE( ";
			upd += TCEL + ".spatial_data, 1) -  SDO_GEOM.SDO_MIN_MBR_ORDINATE( ";
			upd += TCEL + ".spatial_data, 1))/2) ";
			upd += "FROM " + TCEL + " WHERE object_id = t_id) ";
			upd += "WHERE t_x IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to mount the collection table: label_x!"));
				return;
			}
			
			//t_y
			upd= "UPDATE tmptxtable SET ";
			upd += " t_y = (SELECT MAX(SDO_GEOM.SDO_MIN_MBR_ORDINATE( ";
			upd += TCEL + ".spatial_data, 2) + (SDO_GEOM.SDO_MAX_MBR_ORDINATE( ";
			upd += TCEL + ".spatial_data, 2) -  SDO_GEOM.SDO_MIN_MBR_ORDINATE( ";
			upd += TCEL + ".spatial_data, 2))/2) ";
			upd += "FROM " + TCEL + " WHERE object_id = t_id) ";
			upd += "WHERE t_y IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to mount the collection table: label_y!"));
				return;
			}
		}
		else if(db_->dbmsName() == "PostGIS") 
		{
			//t_x
			string upd= "UPDATE tmptxtable SET ";
			upd += " t_x = (SELECT MAX(xmin(spatial_data::box3d) + (xmax(spatial_data::box3d)";
			upd += " -  xmin(spatial_data::box3d)) / 2.0) ";
			upd += "FROM " + TCEL + " WHERE object_id = t_id) ";
			upd += "WHERE t_x IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::information(this, "Mensagem de Erro",
				"Erro ao montar tabela de coleção: t_x!");
				return;
			}
			
			//t_y
			upd= "UPDATE tmptxtable SET ";
			upd += " t_y = (SELECT MAX(ymin(spatial_data::box3d) + (ymax(spatial_data::box3d)";
			upd += " - ymin(spatial_data::box3d)) / 2.0) ";
			upd += "FROM " + TCEL + " WHERE object_id = t_id) ";
			upd += "WHERE t_y IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::information(this, "Mensagem de Erro",
				"Erro ao montar tabela de coleção: t_y!");
				return;
			}
		}
		else if(db_->dbmsName() == "PostgreSQL") 
		{
			//t_x
			string upd= "UPDATE tmptxtable SET ";
			upd += " t_x = (SELECT MAX(lower_x + (upper_x";
			upd += " -  lower_x) / 2.0) ";
			upd += "FROM " + TCEL + " WHERE object_id = t_id) ";
			upd += "WHERE t_x IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::information(this, "Mensagem de Erro",
				"Erro ao montar tabela de coleção: t_x!");
				return;
			}
			
			//t_y
			upd= "UPDATE tmptxtable SET ";
			upd += " t_y = (SELECT MAX(lower_y + (upper_y";
			upd += " - lower_y) / 2.0) ";
			upd += "FROM " + TCEL + " WHERE object_id = t_id) ";
			upd += "WHERE t_y IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::information(this, "Mensagem de Erro",
				"Erro ao montar tabela de coleção: t_y!");
				return;
			}
		}
		else
		{
			string sel = "SELECT t_x, t_y, lower_x, lower_y, upper_x, upper_y";
			sel += " FROM tmptxtable LEFT JOIN " + TCEL;
			sel += " ON tmptxtable.t_id = " + TCEL + ".object_id";
			string upd = "UPDATE (" + sel + ") SET";
			upd += " t_x = lower_x + (upper_x-lower_x)/2,";
			upd += " t_y = lower_y + (upper_y-lower_y)/2";
			if(db_->execute(upd) == false)
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to calculate label_x and label_y for cells!"));
				return;
			}
		}
	}
	if (layer_->hasGeometry(TePOLYGONS))
	{
		string TPOL = layer_->tableName(TePOLYGONS);

		if (db_->dbmsName() == "MySQL")
		{
			string extMax = "CREATE TEMPORARY TABLE maxTable";
			extMax += " SELECT object_id, max(ext_max) AS maximo";
			extMax += " FROM " + TPOL + " GROUP BY object_id";
			db_->execute(extMax);

			string qy = "SELECT t_id," ;
			qy += "(lower_x + (upper_x-lower_x)/2.) AS lbx,";
			qy += "(lower_y + (upper_y-lower_y)/2.) AS lby";
			qy += " FROM maxTable," + TPOL + ",tmptxtable";
			qy += " WHERE (t_x is null OR t_y is null)";
			qy += " AND maxTable.object_id =" + TPOL + ".object_id";
			qy += " AND maxTable.object_id = t_id";
			qy += " AND ext_max = maximo";

			TeDatabasePortal *pt = db_->getPortal();
			if (pt->query(qy) == false)
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to mount the collection table: label_x and label_y!"));
				delete pt;
				return;
			}

			bool bb = pt->fetchRow();
			string id,setClause,lbx,lby,upd;
			while(bb)
			{
				id  = pt->getData("t_id");
				lbx = pt->getData("lbx");
				lby = pt->getData("lby");

				upd = "UPDATE tmptxtable SET";					
				upd += " t_x = '" + lbx + "',"; 
				upd += " t_y = '" + lby + "'";
				upd += " WHERE t_id = '" + id + "'";

				if (db_->execute(upd) == false)
				{
					QMessageBox::critical(this, tr("Error"),
						tr("Fail to mount the collection table: label_x and label_y!"));
					delete pt;
					return;
				}
				bb = pt->fetchRow();
			}

			db_->execute("DROP TABLE maxTable");
			delete pt;			
		}

		else if(db_->dbmsName() == "Oracle")
		{
			string upd= "UPDATE tmptxtable SET ";
			upd += " t_x = (SELECT MAX(lower_x + (upper_x - lower_x)/2) ";
			upd += "FROM " + TPOL + " WHERE object_id = t_id) ";
			upd += "WHERE t_x IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to calculate label_x for polygons!"));
				return;
			}
			
			//label_y
			//upd += " label_x = lower_x + (upper_x-lower_x)/2,";
			upd= "UPDATE tmptxtable SET ";
			upd += " t_y = (SELECT MAX(lower_y + (upper_y - lower_y)/2) ";
			upd += "FROM " + TPOL + " WHERE object_id = t_id) ";
			upd += "WHERE t_y IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to calculate label_y for polygons!"));
				return;
			}

		}				
		
		else if(db_->dbmsName() == "OracleSpatial")
		{
			//t_x
			string upd= "UPDATE tmptxtable SET ";
			upd += " t_x = (SELECT MAX(SDO_GEOM.SDO_MIN_MBR_ORDINATE( ";
			upd += TPOL + ".spatial_data, 1) + (SDO_GEOM.SDO_MAX_MBR_ORDINATE( ";
			upd += TPOL + ".spatial_data, 1) -  SDO_GEOM.SDO_MIN_MBR_ORDINATE( ";
			upd += TPOL + ".spatial_data, 1))/2) ";
			upd += "FROM " + TPOL + " WHERE object_id = t_id) ";
			upd += "WHERE t_x IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to mount the collection table: label_x!"));
				return;
			}
			
			//t_y
			upd= "UPDATE tmptxtable SET ";
			upd += " t_y = (SELECT MAX(SDO_GEOM.SDO_MIN_MBR_ORDINATE( ";
			upd += TPOL + ".spatial_data, 2) + (SDO_GEOM.SDO_MAX_MBR_ORDINATE( ";
			upd += TPOL + ".spatial_data, 2) -  SDO_GEOM.SDO_MIN_MBR_ORDINATE( ";
			upd += TPOL + ".spatial_data, 2))/2) ";
			upd += "FROM " + TPOL + " WHERE object_id = t_id) ";
			upd += "WHERE t_y IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to mount the collection table: label_y!"));
				return;
			}
		}
		else if(db_->dbmsName() == "PostGIS") 
		{
			//t_x
			string upd= "UPDATE tmptxtable SET ";
			upd += " t_x = (SELECT MAX(xmin(spatial_data::box3d) + (xmax(spatial_data::box3d)";
			upd += " -  xmin(spatial_data::box3d)) / 2.0) ";
			upd += "FROM " + TPOL + " WHERE object_id = t_id) ";
			upd += "WHERE t_x IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::information(this, "Mensagem de Erro",
				"Erro ao montar tabela de coleção: t_x!");
				return;
			}
			
			//label_y
			upd= "UPDATE tmptxtable SET ";
			upd += " t_y = (SELECT MAX(ymin(spatial_data::box3d) + (ymax(spatial_data::box3d)";
			upd += " - ymin(spatial_data::box3d)) / 2.0) ";
			upd += "FROM " + TPOL + " WHERE object_id = t_id) ";
			upd += "WHERE t_y IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::information(this, "Mensagem de Erro",
				"Erro ao montar tabela de coleção: t_y!");
				return;
			}
		}
		else if(db_->dbmsName() == "PostgreSQL") 
		{
			//t_x
			string upd= "UPDATE tmptxtable SET ";
			upd += " t_x = (SELECT MAX(lower_x + (upper_x";
			upd += " -  lower_x) / 2.0) ";
			upd += "FROM " + TPOL + " WHERE object_id = t_id) ";
			upd += "WHERE t_x IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::information(this, "Mensagem de Erro",
				"Erro ao montar tabela de coleção: t_x!");
				return;
			}
			
			//label_y
			upd= "UPDATE tmptxtable SET ";
			upd += " t_y = (SELECT MAX(lower_y + (upper_y";
			upd += " - lower_y) / 2.0) ";
			upd += "FROM " + TPOL + " WHERE object_id = t_id) ";
			upd += "WHERE t_y IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::information(this, "Mensagem de Erro",
				"Erro ao montar tabela de coleção: t_y!");
				return;
			}
		}
		else
		{
			string sel = "SELECT t_x, t_y, lower_x, lower_y, upper_x, upper_y";
			sel += " FROM tmptxtable LEFT JOIN " + TPOL;
			sel += " ON tmptxtable.t_id = " + TPOL + ".object_id";
			sel += " WHERE t_x is null OR t_y is null";
			sel += " ORDER BY t_id ASC, ext_max ASC";
			string upd = "UPDATE (" + sel + ") SET";
			upd += " t_x = lower_x + (upper_x-lower_x)/2,";
			upd += " t_y = lower_y + (upper_y-lower_y)/2";
			if(db_->execute(upd) == false)
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to calculate label_x and label_y for polygons!"));
				return;
			}
		}
	}
	if (layer_->hasGeometry(TeLINES))
	{
		string TLIN = layer_->tableName(TeLINES);

		if (db_->dbmsName() == "MySQL")
		{
			string extMax = "CREATE TEMPORARY TABLE maxTable";
			extMax += " SELECT object_id, max(ext_max) AS maximo";
			extMax += " FROM " + TLIN + " GROUP BY object_id";
			db_->execute(extMax);

			string qy = "SELECT t_id," ;
			qy += "(lower_x + (upper_x-lower_x)/2.) AS lbx,";
			qy += "(lower_y + (upper_y-lower_y)/2.) AS lby";
			qy += " FROM maxTable," + TLIN + ",tmptxtable";
			qy += " WHERE (t_x is null OR t_y is null)";
			qy += " AND maxTable.object_id =" + TLIN + ".object_id";
			qy += " AND maxTable.object_id = t_id";
			qy += " AND ext_max = maximo";

			TeDatabasePortal *pt = db_->getPortal();
			if (pt->query(qy) == false)
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to mount the collection table: label_x and label_y!"));
				delete pt;
				return;
			}

			bool bb = pt->fetchRow();
			string id,setClause,lbx,lby,upd;
			while(bb)
			{
				id  = pt->getData("t_id");
				lbx = pt->getData("lbx");
				lby = pt->getData("lby");

				upd = "UPDATE tmptxtable SET";					
				upd += " t_x = '" + lbx + "',"; 
				upd += " t_y = '" + lby + "'";
				upd += " WHERE t_id = '" + id + "'";

				if (db_->execute(upd) == false)
				{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to mount the collection table: label_x and label_y!"));
					delete pt;
					return;
				}
				bb = pt->fetchRow();
			}

			db_->execute("DROP TABLE maxTable");
			delete pt;			
		}

		else if(db_->dbmsName() == "Oracle")
		{
			string upd= "UPDATE tmptxtable SET ";
			upd += " t_x = (SELECT MAX(lower_x + (upper_x - lower_x)/2) ";
			upd += "FROM " + TLIN + " WHERE object_id = t_id) ";
			upd += "WHERE t_x IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::critical(this, tr("Error"),
				tr ("Fail to calculate label_x for lines!"));
				return;
			}
			
			//label_y
			//upd += " label_x = lower_x + (upper_x-lower_x)/2,";
			upd= "UPDATE tmptxtable SET ";
			upd += " t_y = (SELECT MAX(lower_y + (upper_y - lower_y)/2) ";
			upd += "FROM " + TLIN + " WHERE object_id = t_id) ";
			upd += "WHERE t_y IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::critical(this, tr("Error"),
				tr ("Fail to calculate label_y for lines!"));
				return;
			}

		}
		
		else if(db_->dbmsName() == "OracleSpatial")
		{
			//t_x
			string upd= "UPDATE tmptxtable SET ";
			upd += " t_x = (SELECT MAX(SDO_GEOM.SDO_MIN_MBR_ORDINATE( ";
			upd += TLIN + ".spatial_data, 1) + (SDO_GEOM.SDO_MAX_MBR_ORDINATE( ";
			upd += TLIN + ".spatial_data, 1) -  SDO_GEOM.SDO_MIN_MBR_ORDINATE( ";
			upd += TLIN + ".spatial_data, 1))/2) ";
			upd += "FROM " + TLIN + " WHERE object_id = t_id) ";
			upd += "WHERE t_x IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to mount the collection table: label_x!"));
				return;
			}
			
			//t_y
			upd= "UPDATE tmptxtable SET ";
			upd += " t_y = (SELECT MAX(SDO_GEOM.SDO_MIN_MBR_ORDINATE( ";
			upd += TLIN + ".spatial_data, 2) + (SDO_GEOM.SDO_MAX_MBR_ORDINATE( ";
			upd += TLIN + ".spatial_data, 2) -  SDO_GEOM.SDO_MIN_MBR_ORDINATE( ";
			upd += TLIN + ".spatial_data, 2))/2) ";
			upd += "FROM " + TLIN + " WHERE object_id = t_id) ";
			upd += "WHERE t_y IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to mount the collection table: label_y!"));
				return;
			}
		}
		else if(db_->dbmsName() == "PostGIS") 
		{
			//t_x
			string upd= "UPDATE tmptxtable SET ";
			upd += " t_x = (SELECT MAX(xmin(spatial_data::box3d) + (xmax(spatial_data::box3d)";
			upd += " -  xmin(spatial_data::box3d)) / 2.0) ";
			upd += "FROM " + TLIN + " WHERE object_id = t_id) ";
			upd += "WHERE t_x IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::information(this, "Mensagem de Erro",
				"Erro ao montar tabela de coleção: t_x!");
				return;
			}
			
			//t_y
			upd= "UPDATE tmptxtable SET ";
			upd += " t_y = (SELECT MAX(ymin(spatial_data::box3d) + (ymax(spatial_data::box3d)";
			upd += " - ymin(spatial_data::box3d)) / 2.0) ";
			upd += "FROM " + TLIN + " WHERE object_id = t_id) ";
			upd += "WHERE t_y IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::information(this, "Mensagem de Erro",
				"Erro ao montar tabela de coleção: t_y!");
				return;
			}
		}
		else if(db_->dbmsName() == "PostgreSQL") 
		{
			//t_x
			string upd= "UPDATE tmptxtable SET ";
			upd += " t_x = (SELECT MAX(lower_x + (upper_x";
			upd += " -  lower_x) / 2.0) ";
			upd += "FROM " + TLIN + " WHERE object_id = t_id) ";
			upd += "WHERE t_x IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::information(this, "Mensagem de Erro",
				"Erro ao montar tabela de coleção: t_x!");
				return;
			}
			
			//t_y
			upd= "UPDATE tmptxtable SET ";
			upd += " t_y = (SELECT MAX(lower_y + (upper_y";
			upd += " - lower_y) / 2.0) ";
			upd += "FROM " + TLIN + " WHERE object_id = t_id) ";
			upd += "WHERE t_y IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::information(this, "Mensagem de Erro",
				"Erro ao montar tabela de coleção: t_y!");
				return;
			}
		}
		else
		{
			string sel = "SELECT t_x, t_y, lower_x, lower_y, upper_x, upper_y";
			sel += " FROM tmptxtable LEFT JOIN " + TLIN;
			sel += " ON tmptxtable.t_id = " + TLIN + ".object_id";
			sel += " WHERE t_x is null OR t_y is null";
			sel += " ORDER BY t_id ASC, ext_max ASC";
			string up = "UPDATE (" + sel + ") SET";
			up += " t_x = lower_x + (upper_x-lower_x)/2,";
			up += " t_y = lower_y + (upper_y-lower_y)/2";
			if(db_->execute(up) == false)
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to calculate label_x and label_y for lines!"));
				return;
			}
		}
	}
	if (layer_->hasGeometry(TePOINTS))
	{
		string TPOI = layer_->tableName(TePOINTS);

		if (db_->dbmsName() == "MySQL")
		{
			string query = "SELECT t_id,t_x, t_y, x, y";
			query += " FROM tmptxtable LEFT JOIN " + TPOI;
			query += " ON tmptxtable.t_id = " + TPOI + ".object_id";
			query += " WHERE t_x is null OR t_y is null";

			TeDatabasePortal *pt = db_->getPortal();
			if (pt->query(query) == false)
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to update the text tables for points!"));
				delete pt;
				return;
			}

			bool bb = pt->fetchRow();
			string id,setClause,lbx,lby,upd;
			while(bb)
			{
				id  = pt->getData("t_id");
				lbx = pt->getData("x");
				lby = pt->getData("y");

				upd = "UPDATE tmptxtable SET";					
				upd += " t_x = '" + lbx + "',"; 
				upd += " t_y = '" + lby + "'";
				upd += " WHERE t_id = '" + id + "'";

				if (db_->execute(upd) == false)
				{
					QMessageBox::critical(this, tr("Error"),
						tr("Fail to update the text tables for points!"));
					delete pt;
					return;
				}
				bb = pt->fetchRow();
			}
			delete pt;			
		}

		else if(db_->dbmsName() == "Oracle")
		{
			//label_x
			string upd= "UPDATE tmptxtable SET ";
			upd += " t_x = (SELECT MAX(x) ";
			upd += " FROM " + TPOI + " p WHERE object_id = t_id) ";
			upd += "WHERE t_x IS NULL";
			
			if(!db_->execute(upd))
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to calculate label_x for points!"));
				return;
			}

			//label_y
			upd= "UPDATE tmptxtable SET ";
			upd += " t_y = (SELECT MAX(y) ";
			upd += " FROM " + TPOI + " p WHERE object_id = t_id) ";
			upd += "WHERE t_y IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to calculate label_y for points!"));
				return;
			}
			
		}
		
		else if(db_->dbmsName() == "OracleSpatial") 
		{
			//t_x
			string upd= "UPDATE tmptxtable SET ";
			upd += " t_x = (SELECT MAX(p.spatial_data.SDO_POINT.X) ";
			upd += " FROM " + TPOI + " p WHERE object_id = t_id) ";
			upd += "WHERE t_x IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to mount the collection table: label_x!"));
				return;
			}

			//t_y
			upd= "UPDATE tmptxtable SET ";
			upd += " t_y = (SELECT MAX(p.spatial_data.SDO_POINT.Y) ";
			upd += " FROM " + TPOI + " p WHERE object_id = t_id) ";
			upd += "WHERE t_y IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to mount the collection table: label_y!"));
				return;
			}
		}
		else if(db_->dbmsName() == "PostGIS") 
		{
			//t_x
			string upd= "UPDATE tmptxtable SET ";
			upd += " t_x = (SELECT MAX(xmin(spatial_data::box3d) + (xmax(spatial_data::box3d)";
			upd += " -  xmin(spatial_data::box3d)) / 2.0) ";
			upd += "FROM " + TPOI + " WHERE object_id = t_id) ";
			upd += "WHERE t_x IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::information(this, "Mensagem de Erro",
				"Erro ao montar tabela de coleção: t_x!");
				return;
			}
			
			//t_y
			upd= "UPDATE tmptxtable SET ";
			upd += " t_y = (SELECT MAX(ymin(spatial_data::box3d) + (ymax(spatial_data::box3d)";
			upd += " - ymin(spatial_data::box3d)) / 2.0) ";
			upd += "FROM " + TPOI + " WHERE object_id = t_id) ";
			upd += "WHERE t_y IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::information(this, "Mensagem de Erro",
				"Erro ao montar tabela de coleção: t_y!");
				return;
			}
		}
		else if(db_->dbmsName() == "PostgreSQL") 
		{
			//t_x
			string upd= "UPDATE tmptxtable SET ";
			upd += " t_x = (SELECT MAX(lower_x + (upper_x";
			upd += " -  lower_x) / 2.0) ";
			upd += "FROM " + TPOI + " WHERE object_id = t_id) ";
			upd += "WHERE t_x IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::information(this, "Mensagem de Erro",
				"Erro ao montar tabela de coleção: t_x!");
				return;
			}
			
			//t_y
			upd= "UPDATE tmptxtable SET ";
			upd += " t_y = (SELECT MAX(lower_y + (upper_y";
			upd += " - lower_y) / 2.0) ";
			upd += "FROM " + TPOI + " WHERE object_id = t_id) ";
			upd += "WHERE t_y IS NULL";

			if(!db_->execute(upd))
			{
				QMessageBox::information(this, "Mensagem de Erro",
				"Erro ao montar tabela de coleção: t_y!");
				return;
			}
		}
		else
		{
			string sel = "SELECT t_x, t_y, x, y";
			sel += " FROM tmptxtable LEFT JOIN " + TPOI;
			sel += " ON tmptxtable.t_id = " + TPOI + ".object_id";
			sel += " WHERE t_x is null OR t_y is null";
			string up = "UPDATE (" + sel + ") SET";
			up += " t_x = x,";
			up += " t_y = y";
			if(db_->execute(up) == false)
			{
				QMessageBox::critical(this, tr("Error"),
					tr("Fail to calculate label_x and label_y for points!"));
				return;
			}
		}
	}
}


void TextRep::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("textRep.htm");
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


void TextRep::funcComboBox_activated( const QString & )
{
	if(selectedAttributesListBox->currentText().isEmpty())
		return;
	string func = funcComboBox->currentText().latin1();
	string s = selectedAttributesListBox->currentText().latin1();
	int i = s.find("(");
	if (i >= 0)
	{
		s = s.substr(i+1);
		s = s.substr(0, s.size()-1);
	}
	string ss = func + "(" + s + ")";

	int item = selectedAttributesListBox->currentItem();
	selectedAttributesListBox->removeItem(item);
	selectedAttributesListBox->insertItem(ss.c_str(), item);
}


void TextRep::tableAttributesListBox_clicked( QListBoxItem * )
{
	funcComboBox->clear();
	funcComboBox->setEnabled(false);
	addToSelectedPushButton->setEnabled(true);
	removeSelectedPushButton->setEnabled(false);
	selectedAttributesListBox->clearSelection();
}


void TextRep::selectedAttributesListBox_clicked( QListBoxItem * )
{
	tableAttributesListBox->clearSelection();
	funcComboBox->clear();
	funcComboBox->setEnabled(true);
	funcComboBox->insertItem("All");
	funcComboBox->insertItem("MIN");
	funcComboBox->insertItem("MAX");
	funcComboBox->insertItem("COUNT");

	TeAttributeList atr = ((TeTheme*)theme_->getTheme())->sqlAttList();
	TeAttrDataType type;
	QString qs = selectedAttributesListBox->currentText();
	if(qs.isEmpty())
		return;
	string s = qs.latin1();
	string ss = s;
	int i = s.find("(");
	if (i > 0)
	{
		s = s.substr(i+1);
		s = s.substr(0, s.size()-1);
		ss = ss.substr(0, i);
	}
	for (unsigned j = 0; j < atr.size(); ++j)
	{
		if (atr[j].rep_.name_ == s)
		{
			type = atr[j].rep_.type_;
			break;
		}
	}
	if (type == TeREAL || type == TeINT)
	{
		funcComboBox->insertItem("AVG");
		funcComboBox->insertItem("SUM");
	}

	if(ss=="MIN" || ss=="MAX" || ss=="COUNT" || ss=="AVG" || ss=="SUM")
		funcComboBox->setCurrentText(ss.c_str());
	else
		funcComboBox->setCurrentText("All");

	addToSelectedPushButton->setEnabled(false);
	removeSelectedPushButton->setEnabled(true);
}


void TextRep::show()
{
	if(theme_->getTheme()->type() == TeTHEME)
		QDialog::show();
	else
	{
		QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
		reject();
		hide();
	}
}
