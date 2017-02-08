/************************************************************************************
TerraView - visualization and exploration of geographical databases
using TerraLib.
Copyright � 2001-2007 INPE and Tecgraf/PUC-Rio.
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

#include <TeLayer.h>
#include <terraViewBase.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <TeWaitCursor.h>
#include <TeDatabase.h>
#include <TeQtGrid.h>
#include "TeDatabaseUtils.h"
#include "TeExternalTheme.h"


void QueryWindow::init()
{
	mainWindow_ = (TerraViewBase*)qApp->mainWidget();
    help_ = 0;

    andPushButton->setText("AND");
    orPushButton->setText("OR");
    notPushButton->setText("NOT");
    likePushButton->setText("LIKE");
    isNullPushButton->setText("IS NULL");
    isNotNullPushButton->setText("IS NOT NULL");
    undoPushButton->setText("&Undo");
    redoPushButton->setText("&Redo");
    delPushButton->setText("&Del");
}

void QueryWindow::setTheme(TeAppTheme *theme)
{
	TeTheme* th = (TeTheme*)theme->getTheme();
	if(!(th->type() == TeTHEME || th->type() == TeEXTERNALTHEME))
		return;

	unsigned int i;
	theme_ = theme;
	TeAttributeList attrList = ((TeTheme*)theme->getTheme())->sqlAttList();
	attributeComboBox->clear();
	for (i = 0; i < attrList.size(); ++i)
	{
		QString s(attrList[i].rep_.name_.c_str());
		switch (attrList[i].rep_.type_)
		{
			case TeREAL:
			case TeINT:
			case TeSTRING:
			case TeDATETIME:
				attributeComboBox->insertItem(s);
			break;

			default:
				break;
		}
	}
}


void QueryWindow::attributeComboBox_activated(const QString& attr)
{
	string sval = (attr + " ").latin1();

	insertText(sval);

	attributeValuesCheckBox->setChecked(false);
	attributeValuesComboBox->clear();
}

void QueryWindow::attributeValuesCheckBox_toggled(bool isChecked)
{
	TeWaitCursor wait;

	if (!isChecked)
		return;
	
	string attribute = attributeComboBox->currentText().latin1();

	string query = "SELECT DISTINCT ";
	query += attribute;
	TeTheme* th = theme_->getSourceTheme();
	if (th == 0)
		return;
	query += th->sqlFrom();
	query += " ORDER BY ";
	query += attribute;

	QString mess = query.c_str();

	attributeValuesComboBox->clear();

	TeDatabase* db = th->layer()->database();
	TeDatabasePortal *portal = db->getPortal();

	if (portal->query(query))
	{
		TeAttribute attr = portal->getAttribute(0);
		while(portal->fetchRow())
		{
			string s = portal->getData(0);
			if(s.empty() == false)
				attributeValuesComboBox->insertItem(s.c_str());
		}
	}
	delete portal;
}


void QueryWindow::attributeValuesComboBox_activated(const QString& value)
{
	bool isString = false;

	unsigned int i;
	TeAttributeList attrList = ((TeTheme*)theme_->getTheme())->sqlAttList();
	for (i = 0; i < attrList.size(); ++i)
	{
		QString s(attrList[i].rep_.name_.c_str());
		if (s == attributeComboBox->currentText())
		{
			if(attrList[i].rep_.type_ == TeSTRING)
				isString = true;
			break;
		}
	}

	string sval;
	if(isString)
		sval = ("'" + value + "' ").latin1();
	else
		sval = (value + " ").latin1();

	insertText(sval);
}

void QueryWindow::newQueryPushButton_clicked()
{
	TeWaitCursor wait;

	TeTheme* th = theme_->getSourceTheme();
	if (th == 0)
		return;
	TeDatabase* db = th->layer()->database();
	set<string> objectIdSet, uniqueIdSet;
	string sel, val, up;
	string whereClause = queryTextEdit->text().simplifyWhiteSpace().latin1();
	string WHERECLAUSE = TeConvertToUpperCase(whereClause);

	bool isAggregate = false;

	if (WHERECLAUSE.find("AVG(") != string::npos   ||
		WHERECLAUSE.find("AVG (") != string::npos  ||
		WHERECLAUSE.find("MIN(") != string::npos   ||
		WHERECLAUSE.find("MIN (") != string::npos  ||
		WHERECLAUSE.find("MAX(") != string::npos   ||
		WHERECLAUSE.find("MAX (") != string::npos  ||
		WHERECLAUSE.find("SUM(") != string::npos   ||
		WHERECLAUSE.find("SUM (") != string::npos  ||
		WHERECLAUSE.find("COUNT(") != string::npos ||
		WHERECLAUSE.find("COUNT (") != string::npos)
		isAggregate = true;

	if(isAggregate == false)
	{
		sel = "SELECT " + th->collectionTable() + "_aux.unique_id, ";
		sel += th->collectionTable() + ".c_object_id ";
		sel += th->sqlGridFrom() + " WHERE " + whereClause;

		TeDatabasePortal *portal = db->getPortal();
		if (portal->query(sel) == false)
		{
			wait.resetWaitCursor();
			string message = portal->errorMessage();
			QMessageBox::critical(this, tr("Error"), message.c_str());
			delete portal;
			return;
		}

		while (portal->fetchRow())
		{
			val = portal->getData(0);
			if(val.empty())
				objectIdSet.insert(portal->getData(1));
			else
				uniqueIdSet.insert(val);
		}
		delete portal;
	}
	else
	{
		sel = "SELECT MIN(" + th->collectionTable() + ".c_object_id)";
		sel += th->sqlGridFrom() + " GROUP BY " + th->collectionTable() + ".c_object_id HAVING " + whereClause;

		TeDatabasePortal *portal = db->getPortal();
		if (portal->query(sel) == false)
		{
			wait.resetWaitCursor();
			string message = portal->errorMessage();
			QMessageBox::critical(this, tr("Error"), message.c_str());
			delete portal;
			return;
		}

		while (portal->fetchRow())
			objectIdSet.insert(portal->getData(0));
		
		delete portal;
	}

	theme_->refineAttributeWhere(whereClause);
	up = "UPDATE te_theme_application SET refine_attribute_where = '" + db->escapeSequence(whereClause) + "'";
	up += " WHERE theme_id = " + Te2String(((TeTheme*)theme_->getTheme())->id());
	db->execute(up);
	mainWindow_->getGrid()->putColorOnGrid(uniqueIdSet, objectIdSet, "newQuery", whereClause);
	mainWindow_->queryChanged();
} 

void QueryWindow::addToQueryPushButton_clicked()
{
	TeWaitCursor wait;

	TeTheme *thr;
	TeDatabase* dbr;
	
	TeTheme *th = static_cast<TeTheme*>(theme_->getTheme());
	TeDatabase* db = th->layer()->database();

	if(theme_->getTheme()->getProductId() == TeEXTERNALTHEME)
	{
		thr = static_cast<TeExternalTheme*>(theme_->getTheme())->getRemoteTheme();
		dbr = thr->layer()->database();
	}
		
	set<string> objectIdSet, uniqueIdSet;
	string sel, val, up, rw;
	string whereClause = queryTextEdit->text().simplifyWhiteSpace().latin1();
	string WHERECLAUSE = TeConvertToUpperCase(whereClause);

	bool isAggregate = false;

	if (WHERECLAUSE.find("AVG(") != string::npos   ||
		WHERECLAUSE.find("AVG (") != string::npos  ||
		WHERECLAUSE.find("MIN(") != string::npos   ||
		WHERECLAUSE.find("MIN (") != string::npos  ||
		WHERECLAUSE.find("MAX(") != string::npos   ||
		WHERECLAUSE.find("MAX (") != string::npos  ||
		WHERECLAUSE.find("SUM(") != string::npos   ||
		WHERECLAUSE.find("SUM (") != string::npos  ||
		WHERECLAUSE.find("COUNT(") != string::npos ||
		WHERECLAUSE.find("COUNT (") != string::npos)
		isAggregate = true;

	if(theme_->getTheme()->getProductId() == TeTHEME)
	{
		if(isAggregate == false)
		{
			sel = "SELECT " + th->collectionTable() + "_aux.unique_id, ";
			sel += th->collectionTable() + ".c_object_id ";
			sel += th->sqlGridFrom() + " WHERE " + whereClause;
			sel += " AND (grid_status = 0 OR grid_status = 1" ;
			sel += " OR (grid_status is null AND (c_object_status = 0 OR c_object_status = 1)))" ;

			TeDatabasePortal *portal = db->getPortal();
			if (portal->query(sel) == false)
			{
				wait.resetWaitCursor();
				string message = portal->errorMessage();
				QMessageBox::critical(this, tr("Error"), message.c_str());
				delete portal;
				return;
			}
			while (portal->fetchRow())
			{
				val = portal->getData(0);
				if(val.empty())
					objectIdSet.insert(portal->getData(1));
				else
					uniqueIdSet.insert(val);
			}
			delete portal;
		}
		else
		{
			sel = "SELECT MIN(" + th->collectionTable() + ".c_object_id)";
			sel += th->sqlGridFrom();
			sel += " WHERE (grid_status = 0 OR grid_status = 1";
			sel += " OR (grid_status IS NULL AND (c_object_status = 0 OR c_object_status = 1)))" ;
			sel += " GROUP BY " + th->collectionTable() + ".c_object_id";
			sel += " HAVING " + whereClause;

			TeDatabasePortal *portal = db->getPortal();
			if (portal->query(sel) == false)
			{
				wait.resetWaitCursor();
				string message = portal->errorMessage();
				QMessageBox::critical(this, tr("Error"), message.c_str());
				delete portal;
				return;
			}

			while (portal->fetchRow())
				objectIdSet.insert(portal->getData(0));
			
			delete portal;
		}
	}
	else if(theme_->getTheme()->getProductId() == TeEXTERNALTHEME)
	{
		if(isAggregate == false)
		{
			sel = "SELECT " + thr->collectionTable() + "_aux.unique_id ";
			sel += thr->sqlGridFrom() + " WHERE " + whereClause;
			sel += " AND " + thr->collectionTable() + "_aux.unique_id NOT IN ";

			map<string, int>& itemMap = th->getItemStatusMap();
			map<string, int>::iterator mit;
			vector<string> idVec;
			for(mit=itemMap.begin(); mit!=itemMap.end(); ++mit)
			{
				if(mit->second & 2)
					idVec.push_back(mit->first);
			}
			std::vector<std::string>::iterator it_begin = idVec.begin();
			std::vector<std::string>::iterator it_end = idVec.end();
			vector<string> v = generateInClauses(it_begin, it_end, db, false);

			TeDatabasePortal *portal = dbr->getPortal();
			vector<string>::iterator it;
			string query;
			for(it=v.begin(); it!=v.end(); ++it)
			{
				query = sel + *it;
				if (portal->query(query) == false)
				{
					wait.resetWaitCursor();
					string message = portal->errorMessage();
					QMessageBox::critical(this, tr("Error"), message.c_str());
					delete portal;
					return;
				}
				while (portal->fetchRow())
				{
					val = portal->getData(0);
					uniqueIdSet.insert(val);
				}
				portal->freeResult();
			}
			if(v.empty())
			{
				sel = "SELECT " + thr->collectionTable() + "_aux.unique_id ";
				sel += thr->sqlGridFrom() + " WHERE " + whereClause;
				portal->freeResult();
				if (portal->query(sel) == false)
				{
					wait.resetWaitCursor();
					string message = portal->errorMessage();
					QMessageBox::critical(this, tr("Error"), message.c_str());
					delete portal;
					return;
				}
				while (portal->fetchRow())
				{
					val = portal->getData(0);
					uniqueIdSet.insert(val);
				}
			}
			
			//sel = "SELECT " + thr->collectionTable() + ".c_object_id ";
			//sel += thr->sqlGridFrom() + " WHERE " + whereClause;
			//sel += " AND " + thr->collectionTable() + ".c_object_id NOT IN ";

			//map<string, int>& objMap = th->getObjStatusMap();
			//idVec.clear();
			//for(mit=objMap.begin(); mit!=objMap.end(); ++mit)
			//{
			//	if(mit->second < 2)
			//		idVec.push_back(mit->first);
			//}
			//v = generateInClauses(idVec.begin(), idVec.end(), db);

			//portal->freeResult();
			//for(it=v.begin(); it!=v.end(); ++it)
			//{
			//	query = sel + *it;
			//	if (portal->query(query) == false)
			//	{
			//		wait.resetWaitCursor();
			//		string message = portal->errorMessage();
			//		QMessageBox::critical(this, tr("Error"), message.c_str());
			//		delete portal;
			//		return;
			//	}
			//	while (portal->fetchRow())
			//	{
			//		val = portal->getData(0);
			//		objectIdSet.insert(val);
			//	}
			//}
			//if(v.empty())
			//{
			//	sel = "SELECT " + thr->collectionTable() + ".c_object_id ";
			//	sel += thr->sqlGridFrom() + " WHERE " + whereClause;
			//	portal->freeResult();
			//	if (portal->query(sel) == false)
			//	{
			//		wait.resetWaitCursor();
			//		string message = portal->errorMessage();
			//		QMessageBox::critical(this, tr("Error"), message.c_str());
			//		delete portal;
			//		return;
			//	}
			//	while (portal->fetchRow())
			//	{
			//		val = portal->getData(0);
			//		objectIdSet.insert(val);
			//	}
			//}
			delete portal;
		}
		else
		{
			sel = "SELECT MIN(" + thr->collectionTable() + ".c_object_id) ";
			sel += thr->sqlGridFrom() + " WHERE " + thr->collectionTable() + ".c_object_id NOT IN ";

			map<string, int>& objMap = th->getObjStatusMap();
			map<string, int>::iterator mit;
			vector<string> idVec;
			for(mit=objMap.begin(); mit!=objMap.end(); ++mit)
			{
				if(mit->second & 2)
					idVec.push_back(mit->first);
			}
			std::vector<std::string>::iterator it_begin = idVec.begin();
			std::vector<std::string>::iterator it_end = idVec.end();
			vector<string> v = generateInClauses(it_begin, it_end, db);

			TeDatabasePortal *portal = db->getPortal();
			vector<string>::iterator it;
			string query;
			for(it=v.begin(); it!=v.end(); ++it)
			{
				query = sel + *it;
				query += " GROUP BY " + thr->collectionTable() + ".c_object_id";
				query += " HAVING " + whereClause;
				if (portal->query(query) == false)
				{
					wait.resetWaitCursor();
					string message = portal->errorMessage();
					QMessageBox::critical(this, tr("Error"), message.c_str());
					delete portal;
					return;
				}
				while (portal->fetchRow())
				{
					val = portal->getData(0);
					objectIdSet.insert(val);
				}
				portal->freeResult();
			}
			portal->freeResult();

			std::set<std::string>::iterator it1 = objectIdSet.begin();
			std::set<std::string>::iterator it2 = objectIdSet.end();
			v = generateInClauses(it1, it2, db);
			sel = "SELECT " + thr->collectionTable() + "_aux.unique_id ";
			sel += thr->sqlGridFrom() + " WHERE " + thr->collectionTable() + ".c_object_id IN ";
			for(it=v.begin(); it!=v.end(); ++it)
			{
				query = sel + *it;
				if (portal->query(query) == false)
				{
					wait.resetWaitCursor();
					string message = portal->errorMessage();
					QMessageBox::critical(this, tr("Error"), message.c_str());
					delete portal;
					return;
				}
				while (portal->fetchRow())
				{
					val = portal->getData(0);
					uniqueIdSet.insert(val);
				}
				portal->freeResult();
			}
			delete portal;
		}
	}

	if(objectIdSet.size() + uniqueIdSet.size() == 0)
		return;

	db = th->layer()->database();

	rw = theme_->refineAttributeWhere();
	if(rw.empty())
	{
		theme_->refineAttributeWhere(whereClause);
		up = "UPDATE te_theme_application SET refine_attribute_where = '" + db->escapeSequence(whereClause) + "'";
	}
	else
	{
		rw = "(" + rw + ") OR " + whereClause;
		theme_->refineAttributeWhere(rw);
		up = "UPDATE te_theme_application SET refine_attribute_where = '" + db->escapeSequence(rw) + "'";
	}
	up += " WHERE theme_id = " + Te2String(((TeTheme*)theme_->getTheme())->id());
	db->execute(up);

	mainWindow_->getGrid()->putColorOnGrid(uniqueIdSet, objectIdSet, "addQuery", whereClause);
	mainWindow_->queryChanged();
}


void QueryWindow::filterQueryPushButton_clicked()
{
	TeWaitCursor wait;

	TeTheme *thr;
	TeDatabase* dbr;
	
	TeTheme *th = static_cast<TeTheme*>(theme_->getTheme());
	TeDatabase* db = th->layer()->database();

	if(theme_->getTheme()->getProductId() == TeEXTERNALTHEME)
	{
		thr = static_cast<TeExternalTheme*>(theme_->getTheme())->getRemoteTheme();
		dbr = thr->layer()->database();
	}
		
	set<string> objectIdSet, uniqueIdSet;
	string	sel, val, up, rw;
	string whereClause = queryTextEdit->text().simplifyWhiteSpace().latin1();
	string WHERECLAUSE = TeConvertToUpperCase(whereClause);

	bool isAggregate = false;

	if (WHERECLAUSE.find("AVG(") != string::npos   ||
		WHERECLAUSE.find("AVG (") != string::npos  ||
		WHERECLAUSE.find("MIN(") != string::npos   ||
		WHERECLAUSE.find("MIN (") != string::npos  ||
		WHERECLAUSE.find("MAX(") != string::npos   ||
		WHERECLAUSE.find("MAX (") != string::npos  ||
		WHERECLAUSE.find("SUM(") != string::npos   ||
		WHERECLAUSE.find("SUM (") != string::npos  ||
		WHERECLAUSE.find("COUNT(") != string::npos ||
		WHERECLAUSE.find("COUNT (") != string::npos)
		isAggregate = true;

	if(theme_->getTheme()->getProductId() == TeTHEME)
	{
		if(isAggregate == false)
		{
			sel = "SELECT " + th->collectionTable() + "_aux.unique_id, ";
			sel += th->collectionTable() + ".c_object_id ";
			sel += th->sqlGridFrom() + " WHERE " + whereClause;
			sel += " AND (grid_status = 2 OR grid_status = 3";
			sel += " OR (grid_status IS NULL AND (c_object_status = 2 OR c_object_status = 3)))" ;

			TeDatabasePortal *portal = db->getPortal();
			if (portal->query(sel) == false)
			{
				wait.resetWaitCursor();
				string message = portal->errorMessage();
				QMessageBox::critical(this, tr("Error"), message.c_str());
				delete portal;
				return;
			}
			while (portal->fetchRow())
			{
				val = portal->getData(0);
				if(val.empty())
					objectIdSet.insert(portal->getData(1));
				else
					uniqueIdSet.insert(val);
			}
			delete portal;
		}
		else
		{

			sel = "SELECT MIN(" + th->collectionTable() + ".c_object_id)";
			sel += th->sqlGridFrom();
			sel += " WHERE (grid_status = 2 OR grid_status = 3";
			sel += " OR (grid_status is null AND (c_object_status = 2 OR c_object_status = 3)))" ;
			sel += " GROUP BY " + th->collectionTable() + ".c_object_id";
			sel += " HAVING " + whereClause;

			TeDatabasePortal *portal = db->getPortal();
			if (portal->query(sel) == false)
			{
				wait.resetWaitCursor();
				string message = portal->errorMessage();
				QMessageBox::critical(this, tr("Error"), message.c_str());
				delete portal;
				return;
			}

			while (portal->fetchRow())
				objectIdSet.insert(portal->getData(0));
			
			delete portal;
		}
	}
	else if(theme_->getTheme()->getProductId() == TeEXTERNALTHEME)
	{
		if(isAggregate == false)
		{
			sel = "SELECT " + thr->collectionTable() + "_aux.unique_id ";
			sel += thr->sqlGridFrom() + " WHERE " + whereClause;
			sel += " AND " + thr->collectionTable() + "_aux.unique_id IN ";

			map<string, int>& itemMap = th->getItemStatusMap();
			map<string, int>::iterator mit;
			vector<string> idVec;
			for(mit=itemMap.begin(); mit!=itemMap.end(); ++mit)
			{
				if(mit->second > 1)
					idVec.push_back(mit->first);
			}
			std::vector<std::string>::iterator it_begin = idVec.begin();
			std::vector<std::string>::iterator it_end = idVec.end();
			vector<string> v = generateInClauses(it_begin, it_end, db, false);

			TeDatabasePortal *portal = dbr->getPortal();
			vector<string>::iterator it;
			string query;
			for(it=v.begin(); it!=v.end(); ++it)
			{
				query = sel + *it;
				if (portal->query(query) == false)
				{
					wait.resetWaitCursor();
					string message = portal->errorMessage();
					QMessageBox::critical(this, tr("Error"), message.c_str());
					delete portal;
					return;
				}
				while (portal->fetchRow())
				{
					val = portal->getData(0);
					uniqueIdSet.insert(val);
				}
				portal->freeResult();
			}
			
			//sel = "SELECT " + thr->collectionTable() + ".c_object_id ";
			//sel += thr->sqlGridFrom() + " WHERE " + whereClause;
			//sel += " AND " + thr->collectionTable() + ".c_object_id IN ";

			//map<string, int>& objMap = th->getObjStatusMap();
			//idVec.clear();
			//for(mit=objMap.begin(); mit!=objMap.end(); ++mit)
			//{
			//	if(mit->second > 1)
			//		idVec.push_back(mit->first);
			//}
			//v = generateInClauses(idVec.begin(), idVec.end(), db);

			//portal->freeResult();
			//for(it=v.begin(); it!=v.end(); ++it)
			//{
			//	query = sel + *it;
			//	if (portal->query(query) == false)
			//	{
			//		wait.resetWaitCursor();
			//		string message = portal->errorMessage();
			//		QMessageBox::critical(this, tr("Error"), message.c_str());
			//		delete portal;
			//		return;
			//	}
			//	while (portal->fetchRow())
			//	{
			//		val = portal->getData(0);
			//		objectIdSet.insert(val);
			//	}
			//}
			delete portal;
		}
		else
		{
			sel = "SELECT MIN(" + thr->collectionTable() + ".c_object_id) ";
			sel += thr->sqlGridFrom() + " WHERE " + thr->collectionTable() + ".c_object_id IN ";

			map<string, int>& objMap = th->getObjStatusMap();
			map<string, int>::iterator mit;
			vector<string> idVec;
			for(mit=objMap.begin(); mit!=objMap.end(); ++mit)
			{
				if(mit->second > 1)
					idVec.push_back(mit->first);
			}
			std::vector<std::string>::iterator it_begin = idVec.begin();
			std::vector<std::string>::iterator it_end = idVec.end();
			vector<string> v = generateInClauses(it_begin, it_end, db);

			TeDatabasePortal *portal = db->getPortal();
			vector<string>::iterator it;
			string query;
			for(it=v.begin(); it!=v.end(); ++it)
			{
				query = sel + *it;
				query += " GROUP BY " + thr->collectionTable() + ".c_object_id";
				query += " HAVING " + whereClause;
				if (portal->query(query) == false)
				{
					wait.resetWaitCursor();
					string message = portal->errorMessage();
					QMessageBox::critical(this, tr("Error"), message.c_str());
					delete portal;
					return;
				}
				while (portal->fetchRow())
				{
					val = portal->getData(0);
					objectIdSet.insert(val);
				}
				portal->freeResult();
			}
			portal->freeResult();
			std::set<std::string>::iterator it1 = objectIdSet.begin();
			std::set<std::string>::iterator it2 = objectIdSet.end();
			v = generateInClauses(it1, it2, db);
			sel = "SELECT " + thr->collectionTable() + "_aux.unique_id ";
			sel += thr->sqlGridFrom() + " WHERE " + thr->collectionTable() + ".c_object_id IN ";
			for(it=v.begin(); it!=v.end(); ++it)
			{
				query = sel + *it;
				if (portal->query(query) == false)
				{
					wait.resetWaitCursor();
					string message = portal->errorMessage();
					QMessageBox::critical(this, tr("Error"), message.c_str());
					delete portal;
					return;
				}
				while (portal->fetchRow())
				{
					val = portal->getData(0);
					uniqueIdSet.insert(val);
				}
				portal->freeResult();
			}
			delete portal;
		}
	}

	if(objectIdSet.size() + uniqueIdSet.size() == 0)
		return;

	db = th->layer()->database();
	rw = theme_->refineAttributeWhere();
	if(rw.empty())
	{
		theme_->refineAttributeWhere(whereClause); // if concate with spatial queryes
		up = "UPDATE te_theme_application SET refine_attribute_where = '" + db->escapeSequence(whereClause) + "'";
	}
	else
	{
		rw = "(" + rw + ") AND " + whereClause;
		theme_->refineAttributeWhere(rw);
		up = "UPDATE te_theme_application SET refine_attribute_where = '" + db->escapeSequence(rw) + "'";
	}
	up += " WHERE theme_id = " + Te2String(((TeTheme*)theme_->getTheme())->id());
	db->execute(up);
	mainWindow_->getGrid()->putColorOnGrid(uniqueIdSet, objectIdSet, "filterQuery", whereClause);
	mainWindow_->queryChanged();
}

void QueryWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("query.htm");
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

void QueryWindow::insertText(string tx)
{
	int parag, index;

	queryTextEdit->getCursorPosition (&parag, &index );
	QString s = queryTextEdit->text(parag);

	if(index > 0)
	{
		if(s[index-1] != ' ')
		{
			if(!(tx == "'" || tx == "%"))
				tx.insert(0, " ");
		}
	}
	
	int pfim = queryTextEdit->paragraphLength(parag);

	if(queryTextEdit->hasSelectedText())
	{
		int paragFrom, indexFrom, paragTo, indexTo;
		queryTextEdit->getSelection(&paragFrom, &indexFrom, &paragTo, &indexTo);
		queryTextEdit->removeSelectedText();
		queryTextEdit->insertAt(tx.c_str(), paragFrom, indexFrom);
	}
	else if(pfim == index)
	{
		int	nparag = queryTextEdit->paragraphs();
		int	length = queryTextEdit->paragraphLength(nparag-1);
		parag = nparag - 1;
		index = length;
		queryTextEdit->insertAt(tx.c_str(), parag, index);
		queryTextEdit->setSelection (parag, index, parag, index+tx.length());
		queryTextEdit->removeSelectedText();
		queryTextEdit->undo();
	}
	else
		queryTextEdit->insertAt(tx.c_str(), parag, index);

//	int	nparag = queryTextEdit->paragraphs();
//	int	length = queryTextEdit->paragraphLength(nparag-1);
//	parag = nparag - 1;

//	queryTextEdit->setCursorPosition(parag, length);
}

void QueryWindow::plusPushButton_clicked()
{
	insertText("+ ");
}


void QueryWindow::minusPushButton_clicked()
{
	insertText("- ");
}

void QueryWindow::multiplyPushButton_clicked()
{
	insertText("* ");
}

void QueryWindow::dividePushButton_clicked()
{
	insertText("/ ");
}


void QueryWindow::openParPushButton_clicked()
{
	insertText("( ");
}


void QueryWindow::closeParPushButton_clicked()
{
	insertText(") ");
}


void QueryWindow::andPushButton_clicked()
{
	insertText("AND ");
}


void QueryWindow::orPushButton_clicked()
{
	insertText("OR ");
}


void QueryWindow::notPushButton_clicked()
{
	insertText("NOT ");
}


void QueryWindow::likePushButton_clicked()
{
	insertText("LIKE ");
}


void QueryWindow::plicPushButton_clicked()
{
	insertText("'");
}


void QueryWindow::percentPushButton_clicked()
{
	insertText("%");
}


void QueryWindow::greatPushButton_clicked()
{
	insertText("> ");
}


void QueryWindow::lessPushButton_clicked()
{
	insertText("< ");
}


void QueryWindow::lessEqualpushButton_clicked()
{
	insertText("<= ");
}


void QueryWindow::greatEqualPushButton_clicked()
{
	insertText(">= ");
}


void QueryWindow::differentPushButton_clicked()
{
	insertText("<> ");
}


void QueryWindow::equalPushButton_clicked()
{
	insertText("= ");
}


void QueryWindow::isNullPushButton_clicked()
{
	insertText("IS NULL ");
}


void QueryWindow::isNotNullPushButton_clicked()
{
	insertText("IS NOT NULL ");
}

void QueryWindow::redoPushButton_clicked()
{
	if(queryTextEdit->isRedoAvailable())
		queryTextEdit->redo();
}

void QueryWindow::undoPushButton_clicked()
{
	if(queryTextEdit->isUndoAvailable())
		queryTextEdit->undo();
}


void QueryWindow::delPushButton_clicked()
{
	queryTextEdit->del();
}


void QueryWindow::show()
{
	//TeTheme* theme = (TeTheme*)mainWindow_->currentTheme()->getTheme();
	//if(theme->type() == TeTHEME || theme->type() == TeEXTERNALTHEME)
		QDialog::show();
	//else
	//{
	//	QMessageBox::information(this, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
	//	reject();
	//	hide();
	//}
}
