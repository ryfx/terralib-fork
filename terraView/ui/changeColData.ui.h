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

#include <TeWaitCursor.h>

void ChangeColumnData::init()
{
	help_ = 0;
	int	i;
	mainWindow_= (TerraViewBase*)qApp->mainWidget();
	theme_ = mainWindow_->currentTheme();
	if(theme_->getTheme()->type() != TeTHEME)
		return;

	TeAttributeList attrVector = ((TeTheme*)theme_->getTheme())->sqlAttList();
	for(i = 0; i < (int)attrVector.size(); i++)
		operandColumnComboBox->insertItem(attrVector[i].rep_.name_.c_str());

	selectedColumnLineEdit->setText(attrVector[mainWindow_->gridColumn()].rep_.name_.c_str());
	modifyButtonGroup->setButton(0);
}


void ChangeColumnData::operandColumnComboBox_activated( int )
{
	string sval = operandColumnComboBox->currentText().latin1();
	sval += " ";
	int parag, index;
	expressionTextEdit->getCursorPosition (&parag, &index );
	if(expressionTextEdit->hasSelectedText())
	{
		int paragFrom, indexFrom, paragTo, indexTo;
		expressionTextEdit->getSelection(&paragFrom, &indexFrom, &paragTo, &indexTo);
		expressionTextEdit->removeSelectedText();
		expressionTextEdit->insertAt(sval.c_str(), paragFrom, indexFrom);
	}
	else
		expressionTextEdit->insertAt(sval.c_str(), parag, index);

	int	nparag = expressionTextEdit->paragraphs();
	int	length = expressionTextEdit->paragraphLength(nparag-1);
	expressionTextEdit->setCursorPosition(nparag-1, length);
}

void ChangeColumnData::addPushButton_clicked()
{
	string sval = "+ ";
	int parag, index;
	expressionTextEdit->getCursorPosition (&parag, &index );
	if(expressionTextEdit->hasSelectedText())
	{
		int paragFrom, indexFrom, paragTo, indexTo;
		expressionTextEdit->getSelection(&paragFrom, &indexFrom, &paragTo, &indexTo);
		expressionTextEdit->removeSelectedText();
		expressionTextEdit->insertAt(sval.c_str(), paragFrom, indexFrom);
	}
	else
		expressionTextEdit->insertAt(sval.c_str(), parag, index);

	int	nparag = expressionTextEdit->paragraphs();
	int	length = expressionTextEdit->paragraphLength(nparag-1);
	expressionTextEdit->setCursorPosition(nparag-1, length);
}

void ChangeColumnData::subtractionPushButton_clicked()
{
	string sval = "- ";
	int parag, index;
	expressionTextEdit->getCursorPosition (&parag, &index );
	if(expressionTextEdit->hasSelectedText())
	{
		int paragFrom, indexFrom, paragTo, indexTo;
		expressionTextEdit->getSelection(&paragFrom, &indexFrom, &paragTo, &indexTo);
		expressionTextEdit->removeSelectedText();
		expressionTextEdit->insertAt(sval.c_str(), paragFrom, indexFrom);
	}
	else
		expressionTextEdit->insertAt(sval.c_str(), parag, index);

	int	nparag = expressionTextEdit->paragraphs();
	int	length = expressionTextEdit->paragraphLength(nparag-1);
	expressionTextEdit->setCursorPosition(nparag-1, length);
}

void ChangeColumnData::multiplyPushButton_clicked()
{
	string sval = "* ";
	int parag, index;
	expressionTextEdit->getCursorPosition (&parag, &index );
	if(expressionTextEdit->hasSelectedText())
	{
		int paragFrom, indexFrom, paragTo, indexTo;
		expressionTextEdit->getSelection(&paragFrom, &indexFrom, &paragTo, &indexTo);
		expressionTextEdit->removeSelectedText();
		expressionTextEdit->insertAt(sval.c_str(), paragFrom, indexFrom);
	}
	else
		expressionTextEdit->insertAt(sval.c_str(), parag, index);

	int	nparag = expressionTextEdit->paragraphs();
	int	length = expressionTextEdit->paragraphLength(nparag-1);
	expressionTextEdit->setCursorPosition(nparag-1, length);
}

void ChangeColumnData::divisionPushButton_clicked()
{
	string sval = "/ ";
	int parag, index;
	expressionTextEdit->getCursorPosition (&parag, &index );
	if(expressionTextEdit->hasSelectedText())
	{
		int paragFrom, indexFrom, paragTo, indexTo;
		expressionTextEdit->getSelection(&paragFrom, &indexFrom, &paragTo, &indexTo);
		expressionTextEdit->removeSelectedText();
		expressionTextEdit->insertAt(sval.c_str(), paragFrom, indexFrom);
	}
	else
		expressionTextEdit->insertAt(sval.c_str(), parag, index);

	int	nparag = expressionTextEdit->paragraphs();
	int	length = expressionTextEdit->paragraphLength(nparag-1);
	expressionTextEdit->setCursorPosition(nparag-1, length);
}


void ChangeColumnData::okPushButton_clicked()
{
	TeWaitCursor wait;
	string erro;
	TeQtGrid* grid = mainWindow_->getGrid();
	TeDatabase* db = mainWindow_->currentDatabase();
	TeDatabasePortal *portal = db->getPortal();

	QButton* button = modifyButtonGroup->selected();

	TeTheme* curTheme = (TeTheme*)theme_->getTheme();

	string changingCol = selectedColumnLineEdit->text().latin1();
	TeTable changingTable = grid->findTable(changingCol);
	TeAttribute attr2 = (curTheme->sqlAttList())[mainWindow_->gridColumn()];
	string index = changingTable.name() + "." + changingTable.uniqueName();
	string exp = expressionTextEdit->text().simplifyWhiteSpace().latin1();
	string upperExp = TeConvertToUpperCase(exp);

	if(exp.empty())
	{
		wait.resetWaitCursor();
		QString error = tr("Enter with an expression for") + " " + changingCol.c_str();
		error += " " + tr("and try again!");
		QMessageBox::warning(this, tr("Warning"), error);
		delete portal;
		return;
	}

	string groupBy;
	if(upperExp.find("MIN(")!=string::npos || upperExp.find("MAX(")!=string::npos ||
	   upperExp.find("AVG(")!=string::npos || upperExp.find("SUM(")!=string::npos ||
	   upperExp.find("COUNT(")!=string::npos)
	{
		if(changingTable.tableType() != TeAttrStatic)
		{
			delete portal;
			wait.resetWaitCursor();
			QString error = tr("To use agregation, select column of a static table.");
			error += " " + tr("Select other column and try again!");
			QMessageBox::warning(this, tr("Warning"), error);
			return;
		}
		groupBy = " GROUP BY " + curTheme->collectionTable() + ".c_object_id";
	}

	//Test if the string is a valid sql expression
	string testExp, expression, where;
	if(upperExp.find(" WHERE ") != string::npos)
	{
		int f = upperExp.find(" WHERE ");
		where = upperExp.substr(f);
		expression = upperExp.substr(0, f);
		testExp = "SELECT " + expression + curTheme->sqlGridFrom() + where;
		if(button == pointedRadioButton)
			testExp += " AND (" + curTheme->collectionAuxTable() + ".grid_status = 1";
		else if(button == queriedRadioButton)
			testExp += " AND (" + curTheme->collectionAuxTable() + ".grid_status = 2";
		if(button != allRadioButton)
			testExp += " OR " + curTheme->collectionAuxTable() + ".grid_status = 3)";
	}
	else
	{
		expression = upperExp;
		testExp = "SELECT " + expression + curTheme->sqlGridFrom();
		if(button == pointedRadioButton)
			testExp += " WHERE " + curTheme->collectionAuxTable() + ".grid_status = 1";
		else if(button == queriedRadioButton)
			testExp += " WHERE " + curTheme->collectionAuxTable() + ".grid_status = 2";
		if(button != allRadioButton)
			testExp += " OR " + curTheme->collectionAuxTable() + ".grid_status = 3";
	}

	if(portal->query(testExp) == false)
	{
		erro = portal->errorMessage();
		wait.resetWaitCursor();
		QString error = tr("Error:") + " " + erro.c_str();
		QMessageBox::critical(this, tr("Error"), error);
		delete portal;
		return;
	}
	delete portal;

	if (db->dbmsName() == "Ado")
	{
		// find WHERE clause
		if(upperExp.find(" WHERE ") != string::npos)
		{
			string upd = "UPDATE (" + curTheme->sqlJoin() + ")";
			upd += " SET " + changingCol + " = ";
			if(exp.empty())
				upd += "null"; 
			else
				upd += exp;

			if(button == pointedRadioButton)
				upd += " AND (" + curTheme->collectionAuxTable() + ".grid_status = 1";
			else if(button == queriedRadioButton)
				upd += " AND (" + curTheme->collectionAuxTable() + ".grid_status = 2";
			if(button != allRadioButton)
				upd += " OR " + curTheme->collectionAuxTable() + ".grid_status = 3)";

			if(db->execute(upd) == false)
				erro = db->errorMessage();
		}
		else
		{
			map<string, bool> mapOut;
			map<string, bool> mapIn;

			string sel = "SELECT " + index + ", " + exp + curTheme->sqlGridFrom();

			if(button == pointedRadioButton)
				sel += " WHERE " + curTheme->collectionAuxTable() + ".grid_status = 1";
			else if(button == queriedRadioButton)
				sel += " WHERE " + curTheme->collectionAuxTable() + ".grid_status = 2";
			if(button != allRadioButton)
				sel += " OR " + curTheme->collectionAuxTable() + ".grid_status = 3";

			portal = db->getPortal();
			if(portal->query(sel))
			{
				string EXP = TeConvertToUpperCase(exp);
				if(EXP != "NULL")
				{
					while(portal->fetchRow())
					{
						string id = portal->getData(0);
						string s = portal->getData(1);
						if(s.empty())
							mapOut[id] = true;
						else
							mapIn[id] = true;
					}
				}
				else
				{
					while(portal->fetchRow())
					{
						string id = portal->getData(0);
						mapIn[id] = true;
					}
				}
			}
			else
				erro = portal->errorMessage();

			delete portal;

			if(erro.empty())
			{
				if(mapOut.size() <= 500)
				{
					string upd;
					if(mapOut.size() == 0)
					{
						upd = "UPDATE (" + curTheme->sqlGridJoin() + ")";
						upd += " SET " + changingCol + " = " + exp;
						if(button == pointedRadioButton)
							upd += " WHERE " + curTheme->collectionAuxTable() + ".grid_status = 1";
						else if(button == queriedRadioButton)
							upd += " WHERE " + curTheme->collectionAuxTable() + ".grid_status = 2";
						if(button != allRadioButton)
							upd += " OR " + curTheme->collectionAuxTable() + ".grid_status = 3";
					}
					else
					{
						string nupd = "UPDATE (" + curTheme->sqlGridJoin() + ")";
						nupd += " SET " + changingCol + " = null";
						nupd += " WHERE " + index + " IN (";
						map<string, bool>::iterator it = mapOut.begin();
						while(it != mapOut.end())
						{
							nupd += "'" + (*it).first + "', ";
							it++;
						}
						nupd =  nupd.substr(0, nupd.size()-2);
						nupd += ")";
						if(button == pointedRadioButton)
							nupd += " AND (" + curTheme->collectionAuxTable() + ".grid_status = 1";
						else if(button == queriedRadioButton)
							nupd += " AND (" + curTheme->collectionAuxTable() + ".grid_status = 2";
						if(button != allRadioButton)
							nupd += " OR " + curTheme->collectionAuxTable() + ".grid_status = 3)";

						if(db->execute(nupd) == false)
							erro = db->errorMessage();


						upd = "UPDATE (" + curTheme->sqlGridJoin() + ")";
						upd += " SET " + changingCol + " = " + exp;
						upd += " WHERE " + index + " NOT IN (";
						it = mapOut.begin();
						while(it != mapOut.end())
						{
							upd += "'" + (*it).first + "', ";
							it++;
						}
						upd = upd.substr(0, upd.size()-2);
						upd += ")";
						if(button == pointedRadioButton)
							upd += " AND (" + curTheme->collectionAuxTable() + ".grid_status = 1";
						else if(button == queriedRadioButton)
							upd += " AND (" + curTheme->collectionAuxTable() + ".grid_status = 2";
						if(button != allRadioButton)
							upd += " OR " + curTheme->collectionAuxTable() + ".grid_status = 3)";
					}
					if(db->execute(upd) == false)
						erro = db->errorMessage();
				}
				else
				{
					if(mapIn.size() == 0)
					{
						string upd = "UPDATE (" + curTheme->sqlGridJoin() + ")";
						upd += " SET " + changingCol + " = " + exp;

						if(button == pointedRadioButton)
							upd += " WHERE " + curTheme->collectionAuxTable() + ".grid_status = 1";
						else if(button == queriedRadioButton)
							upd += " WHERE " + curTheme->collectionAuxTable() + ".grid_status = 2";
						if(button != allRadioButton)
							upd += " OR " + curTheme->collectionAuxTable() + ".grid_status = 3";

						if(db->execute(upd) == false)
							erro = db->errorMessage();
					}
					else
					{
						map<string, bool>::iterator it = mapIn.begin();
						while(it != mapIn.end())
						{
							string upd = "UPDATE (" + curTheme->sqlGridJoin() + ")";
							upd += " SET " + changingCol + " = " + exp;
							upd += " WHERE " + index + " IN (";
							int i = 0;
							while(i < 200 && it != mapIn.end())
							{
								upd += "'" + (*it).first + "', ";
								it++;
								i++;
							}
							upd = upd.substr(0, upd.size()-2);
							upd += ")";
							if(button == pointedRadioButton)
								upd += " AND (" + curTheme->collectionAuxTable() + ".grid_status = 1";
							else if(button == queriedRadioButton)
								upd += " AND (" + curTheme->collectionAuxTable() + ".grid_status = 2";
							if(button != allRadioButton)
								upd += " OR " + curTheme->collectionAuxTable() + ".grid_status = 3)";

							if(db->execute(upd) == false)
								erro = db->errorMessage();
						}

						it = mapOut.begin();
						while(it != mapOut.end())
						{
							string upd = "UPDATE (" + curTheme->sqlGridJoin() + ")";
							upd += " SET " + changingCol + " = null";
							upd += " WHERE " + index + " IN (";
							int i = 0;
							while(i < 200 && it != mapOut.end())
							{
								upd += "'" + (*it).first + "', ";
								it++;
								i++;
							}
							upd = upd.substr(0, upd.size()-2);
							upd += ")";
							if(button == pointedRadioButton)
								upd += " AND (" + curTheme->collectionAuxTable() + ".grid_status = 1";
							else if(button == queriedRadioButton)
								upd += " AND (" + curTheme->collectionAuxTable() + ".grid_status = 2";
							if(button != allRadioButton)
								upd += " OR " + curTheme->collectionAuxTable() + ".grid_status = 3)";

							if(db->execute(upd) == false)
								erro = db->errorMessage();
						}
					}
				}
			}
		}

		if(groupBy.empty())
		{
			if(erro.empty() == false)
			{
				wait.resetWaitCursor();
				QString error = tr("Fail to update the column!") + "\n";
				error += erro.c_str();
				QMessageBox::critical(this, tr("Error"), error);
				return;
			}
		}
		else
		{
			string sel = "SELECT MIN(" + curTheme->collectionTable() + ".c_object_id) AS id, ";
			sel += expression + " AS val" + curTheme->sqlGridFrom();
			if(where.empty() == false)
			{
				sel += where;
				if(button == pointedRadioButton)
					sel += " AND (" + curTheme->collectionAuxTable() + ".grid_status = 1";
				else if(button == queriedRadioButton)
					sel += " AND (" + curTheme->collectionAuxTable() + ".grid_status = 2";
				if(button != allRadioButton)
					sel += " OR " + curTheme->collectionAuxTable() + ".grid_status = 3)";
			}
			else
			{
				if(button == pointedRadioButton)
					sel += " WHERE " + curTheme->collectionAuxTable() + ".grid_status = 1";
				else if(button == queriedRadioButton)
					sel += " WHERE " + curTheme->collectionAuxTable() + ".grid_status = 2";
				if(button != allRadioButton)
					sel += " OR " + curTheme->collectionAuxTable() + ".grid_status = 3";
			}
			sel += groupBy;

			// create temporary table
			string ttable;
			string user = db->user();
			if(user.empty())
				ttable = "Temp" + Te2String(curTheme->id());
			else
				ttable = db->user() + "_Temp";

			string drop = "DROP TABLE " + ttable;
			if(db->tableExist(ttable))
				db->execute(drop);

			TeAttributeList atl;
			TeAttribute		at;

			at.rep_.name_ = "id";
			at.rep_.type_ = TeSTRING;
			at.rep_.numChar_ = 50;
			at.rep_.isPrimaryKey_ = true;
			atl.push_back(at);

			attr2.rep_.name_ = "val";
			atl.push_back(attr2);

			if(!db->createTable(ttable, atl))
			{
				wait.resetWaitCursor();
				QString error = tr("Fail to update the column!") + "\n";
				error += erro.c_str();
				QMessageBox::critical(this, tr("Error"), error);
				return;
			}

			string ins = "INSERT INTO " + ttable + " (id, val) " + sel;

			if(db->execute(ins))
			{
				string up = "UPDATE (SELECT " + changingCol + ", " + ttable + ".val";
				up += " FROM " + changingTable.name() + ", " + ttable + " WHERE ";
				up += index + " = " + ttable + ".id) SET ";
				up += changingCol + " = " + ttable + ".val";
				if(db->execute(up) == false)
				{
					wait.resetWaitCursor();
					db->execute(drop);
					QString error = tr("Fail to update the column!") + "\n";
					error += db->errorMessage().c_str();
					QMessageBox::critical(this, tr("Error"), error);
					return;
				}
			}
			else // not all cells are success calculated
			{
				map<string, bool> mapOut;
				map<string, bool> mapIn;

				portal = db->getPortal();
				if(portal->query(sel))
				{
					while(portal->fetchRow())
					{
						string id = portal->getData(0);
						string s = portal->getData(1);
						if(s.empty())
							mapOut[id] = true;
						else
							mapIn[id] = true;
					}
				}
				else
				{
					delete portal;
					wait.resetWaitCursor();
					db->execute(drop);
					QString error = tr("Fail to insert temporary values!") + "\n";
					error = db->errorMessage().c_str();
					QMessageBox::critical(this, tr("Error"), error);
					return;
				}

				delete portal;

				string ins = "INSERT INTO " + ttable + " (id, val) " + sel;
				string a;
				if(ins.find(" WHERE ") == string::npos)
					a = " WHERE " + curTheme->collectionTable() + ".c_object_id IN (";
				else
				{
					ins.insert(ins.find(" WHERE ") + strlen(" WHERE "), "(", 1);
					a = ") AND " + curTheme->collectionTable() + ".c_object_id IN (";
				}

				string INS = ins;
				map<string, bool>::iterator it = mapIn.begin();
				while(it != mapIn.end())
				{
					int i = 0;
					while(i < 200 && it != mapIn.end())
					{
						a += "'" + (*it).first + "', ";
						it++;
						i++;
					}
					a = a.substr(0, a.size()-2);
					a += ")";

					ins = INS;
					size_t f = ins.find(" GROUP BY ");
					if(f != string::npos)
						ins.insert(f, a.c_str(), a.size());

					if(db->execute(ins) == false)
					{
						wait.resetWaitCursor();
						db->execute(drop);
						QString error = tr("Fail to insert temporary values!") + "\n";
						error = db->errorMessage().c_str();
						QMessageBox::critical(this, tr("Error"), error);
						return;
					}
				}

				string up = "UPDATE (SELECT " + changingCol + ", " + ttable + ".val";
				up += " FROM " + changingTable.name() + ", " + ttable + " WHERE ";
				up += index + " = " + ttable + ".id) SET ";
				up += changingCol + " = " + ttable + ".val";
				if(db->execute(up) == false)
				{
					wait.resetWaitCursor();
					db->execute(drop);
					QString error = tr("Fail to update the column!") + "\n";
					error += db->errorMessage().c_str();
					QMessageBox::critical(this, tr("Error"), error);
					return;
				}

				it = mapOut.begin();
				while(it != mapOut.end())
				{
					up = "UPDATE " + changingTable.name() + " SET " + changingCol;
					up += " = null WHERE " + index + " IN (";

					int i = 0;
					string b;
					while(i < 200 && it != mapOut.end())
					{
						b += "'" + (*it).first + "', ";
						it++;
						i++;
					}
					b = b.substr(0, b.size()-2);
					b += ")";

					up += b;
					if(db->execute(up) == false)
					{
						wait.resetWaitCursor();
						db->execute(drop);
						QString error = tr("Fail to update the column!") + "\n";
						error += db->errorMessage().c_str();
						QMessageBox::critical(this, tr("Error"), error);
						return;
					}
				}
			}

			db->execute(drop);
		}
	} // end of change column data for an "Access - Ado" database
//	else if ((db->dbmsName()=="OracleAdo") || (db->dbmsName()=="OracleSpatial"))
	else
	{
		// Create a temporary table
		string tempTable = "tempTable" + Te2String(curTheme->id());
		string dropTable = "DROP TABLE " + tempTable;
		if(db->tableExist(tempTable))
			db->execute(dropTable);

		string insertTable;
		
		TeAttributeList attList;
		TeAttribute attr1;
		attr1.rep_.name_ = "object_id_";
		attr1.rep_.type_ = TeSTRING;
		attr1.rep_.numChar_ = 50;
		attList.push_back (attr1);

		attList.push_back (attr2);
		
		if(!db->createTable(tempTable, attList))
		{
			wait.resetWaitCursor();
			QString error = tr("Fail to change column!") + "\n";
			error += db->errorMessage().c_str();
			QMessageBox::critical(this, tr("Error"), error);
			return;
		}

		if (groupBy.empty() == true)
		{
			insertTable = "INSERT INTO " + tempTable;
			insertTable += " SELECT DISTINCT " + index + " AS ID, ";
			insertTable += expression + " AS RESULT " + curTheme->sqlGridFrom();
		}
		else
		{
			insertTable = "INSERT INTO " + tempTable + " SELECT ";
			insertTable += "MIN(" + curTheme->collectionTable() + ".c_object_id) AS ID, ";
			insertTable += expression + " AS RESULT " + curTheme->sqlFrom();
		}

		if(where.empty() == false)
		{
			insertTable += where;	
			if (groupBy.empty() == false)
			{
				if(button == pointedRadioButton)
					insertTable += " AND (" + curTheme->collectionTable() + ".c_object_status = 1";
				else if(button == queriedRadioButton)
					insertTable += " AND (" + curTheme->collectionTable() + ".c_object_status = 2";
				if(button != allRadioButton)
					insertTable += " OR " + curTheme->collectionTable() + ".c_object_status = 3)";
			}
			else
			{
				if(button == pointedRadioButton)
					insertTable += " AND (" + curTheme->collectionAuxTable() + ".grid_status = 1";
				else if(button == queriedRadioButton)
					insertTable += " AND (" + curTheme->collectionAuxTable() + ".grid_status = 2";
				if(button != allRadioButton)
					insertTable += " OR " + curTheme->collectionAuxTable() + ".grid_status = 3)";
			}
		}
		else
		{
			if (groupBy.empty() == false)
			{
				if(button == pointedRadioButton)
					insertTable += " WHERE " + curTheme->collectionTable() + ".c_object_status = 1";
				else if(button == queriedRadioButton)
					insertTable += " WHERE " + curTheme->collectionTable() + ".c_object_status = 2";
				if(button != allRadioButton)
					insertTable += " OR " + curTheme->collectionTable() + ".c_object_status = 3";

				insertTable += " "+ groupBy;
			}
			else
			{
				if(button == pointedRadioButton)
					insertTable += " WHERE " + curTheme->collectionAuxTable() + ".grid_status = 1";
				else if(button == queriedRadioButton)
					insertTable += " WHERE " + curTheme->collectionAuxTable() + ".grid_status = 2";
				if(button != allRadioButton)
					insertTable += " OR " + curTheme->collectionAuxTable() + ".grid_status = 3";
			}
		}

		if(db->execute(insertTable) == false)
		{
			wait.resetWaitCursor();
			db->execute(dropTable);
			QString error = tr("Fail to create a temporary table!") + "\n";
			error += db->errorMessage().c_str();
			QMessageBox::critical(this, tr("Error"), error);
			return;
		}

		string ins = "INSERT INTO " + tempTable + " SELECT ";
		ins += index + ", " + changingCol + " FROM " + changingTable.name();
		ins += " WHERE " + index + " NOT IN (SELECT object_id_ FROM " + tempTable + ")";
		if(db->execute(ins) == false)
		{
			wait.resetWaitCursor();
			db->execute(dropTable);
			QString error = tr("Fail to insert values in the tempTable!") + "\n";
			error += db->errorMessage().c_str();
			QMessageBox::critical(this, tr("Error"), error);
			return;
		}

		string update = "UPDATE " + changingTable.name() + " SET ";
		update += changingCol + " = (SELECT ";
		update += tempTable + "."+ attr2.rep_.name_ +" FROM " + tempTable;
		update += " WHERE " + index + " = " + tempTable + ".object_id_)";
		if(db->execute(update) == false)
		{
			wait.resetWaitCursor();
			db->execute(dropTable);
			QString error = tr("Fail to update the column!") + "\n";
			error += db->errorMessage().c_str();
			QMessageBox::critical(this, tr("Error"), error);
			return;
		}

		db->execute(dropTable);
		int col = mainWindow_->gridColumn();
		grid->arrangeGridToPortalRowVector(col);


	}
//	else
//	{
//		// Create a temporary table
//		string tempTable = "tempTable" + Te2String(theme_->id());
//		string dropTable = "DROP TABLE " + tempTable;
//		if(db->tableExist(tempTable))
//			db->execute(dropTable);
//
//		string createTable;
//		if (groupBy.empty() == true)
//		{
//			createTable = "CREATE TABLE " + tempTable;
//			createTable += " SELECT DISTINCT " + changingTable.name() + "." + index + " AS ID, ";
//			createTable += expression + " AS RESULT " + theme_->sqlGridFrom();
//		}
//		else
//		{
//			createTable = "CREATE TABLE " + tempTable + " SELECT ";
//			createTable += "MIN(" + theme_->collectionTable() + ".c_object_id) AS ID, ";
//			createTable += expression + " AS RESULT " + theme_->sqlFrom();
//		}
//
//		if(where.empty() == false)
//		{
//			createTable += where;	
//			if (groupBy.empty() == false)
//			{
//				if(button == pointedRadioButton)
//					createTable += " AND (" + theme_->collectionTable() + ".c_object_status = 1";
//				else if(button == queriedRadioButton)
//					createTable += " AND (" + theme_->collectionTable() + ".c_object_status = 2";
//				if(button != allRadioButton)
//					createTable += " OR " + theme_->collectionTable() + ".c_object_status = 3)";
//			}
//			else
//			{
//				if(button == pointedRadioButton)
//					createTable += " AND (" + theme_->collectionAuxTable() + ".grid_status = 1";
//				else if(button == queriedRadioButton)
//					createTable += " AND (" + theme_->collectionAuxTable() + ".grid_status = 2";
//				if(button != allRadioButton)
//					createTable += " OR " + theme_->collectionAuxTable() + ".grid_status = 3)";
//			}
//		}
//		else
//		{
//			if (groupBy.empty() == false)
//			{
//				if(button == pointedRadioButton)
//					createTable += " WHERE " + theme_->collectionTable() + ".c_object_status = 1";
//				else if(button == queriedRadioButton)
//					createTable += " WHERE " + theme_->collectionTable() + ".c_object_status = 2";
//				if(button != allRadioButton)
//					createTable += " OR " + theme_->collectionTable() + ".c_object_status = 3";
//			}
//			else
//			{
//				if(button == pointedRadioButton)
//					createTable += " WHERE " + theme_->collectionAuxTable() + ".grid_status = 1";
//				else if(button == queriedRadioButton)
//					createTable += " WHERE " + theme_->collectionAuxTable() + ".grid_status = 2";
//				if(button != allRadioButton)
//					createTable += " OR " + theme_->collectionAuxTable() + ".grid_status = 3";
//			}
//		}
//
//		if(db->execute(createTable) == false)
//		{
//			wait.resetWaitCursor();
//			QString error = tr("Fail to create a temporary table!") + "\n";
//			error += db->errorMessage().c_str();
//			QMessageBox::critical(this, tr("Error"), error);
//			return;
//		}
//
//		string ins = "INSERT INTO " + tempTable + " (ID, RESULT) SELECT ";
//		ins += index + ", " + changingCol + " FROM " + changingTable.name();
//		ins += " WHERE " + index + " NOT IN (SELECT ID FROM " + tempTable + ")";
//		if(db->execute(ins) == false)
//		{
//			wait.resetWaitCursor();
//			db->execute(dropTable);
//			QString error = tr("Fail to insert values in the tempTable!") + "\n";
//			error += db->errorMessage().c_str();
//			QMessageBox::critical(this, tr("Error"), error);
//			return;
//		}
//
//		string update = "UPDATE " + changingTable.name() + " SET ";
//		update += changingTable.name() + "." + changingCol + " = (SELECT ";
//		update += tempTable + ".RESULT FROM " + tempTable;
//		update += " WHERE " + changingTable.name() + "." + index + " = " + tempTable + ".ID)";
//		if(db->execute(update) == false)
//		{
//			wait.resetWaitCursor();
//			db->execute(dropTable);
//			QString error = tr("Fail to update the column!") + "\n";
//			error += db->errorMessage().c_str();
//			QMessageBox::critical(this, tr("Error"), error);
//			return;
//		}
//
//		db->execute(dropTable);
//		int col = mainWindow_->gridColumn();
//		grid->arrangeGridToPortalRowVector(col);
//	
//	} //end database types


	grid->refresh();
	grid->adjustColumns();
	wait.resetWaitCursor();
	QMessageBox::information(this, tr("Information"), tr("Column updated successfully!"));	
	hide();
}


void ChangeColumnData::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("changeColData.htm");
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



void ChangeColumnData::show()
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
