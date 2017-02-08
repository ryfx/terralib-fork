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

#include <TeWaitCursor.h>
#include <TeQtViewsListView.h>
#include <TeQtThemeItem.h>
#include <TeQtProgress.h>
#include <TeExternalTheme.h>
//#include <TeDatabaseUtils.h>

#include <TeThemeFunctions.h>

void StatisticWindow::init()
{
	help_ = 0;

	unsigned int i;

	statisticTable->setColumnStretchable(1, true);
	statisticTable->adjustColumn(1);
	statisticTable->setLeftMargin(0);

	if((mainWindow_ = (TerraViewBase*)qApp->mainWidget()) == 0)
		return;
	
	TeAppTheme* appTheme = mainWindow_->currentTheme();
	TeAttributeList attrVec = ((TeTheme*)appTheme->getTheme())->sqlAttList();
	string fieldName = attrVec[mainWindow_->gridColumn()].rep_.name_;
	string caption = tr("Statistics").latin1();
	caption += " " + fieldName;
	setCaption(caption.c_str());

	summaryComboBox->clear();
    summaryComboBox->insertItem( tr( "NONE" ) );
    summaryComboBox->insertItem( tr( "Groups" ) );
	for(i=0; i<attrVec.size(); i++)
		summaryComboBox->insertItem(attrVec[i].rep_.name_.c_str());

	summaryComboBox->setCurrentItem(0);
	summary_ = summaryComboBox->currentText();
	objectsComboBox->setCurrentItem(0);
	input_ = objectsComboBox->currentItem();

	calculateStatistic(fieldName);
}



void StatisticWindow::summaryComboBox_activated( int )
{
	summary_ = summaryComboBox->currentText();

	TeAppTheme* curAppTheme = mainWindow_->currentTheme();
	TeAttributeList attrVec = ((TeTheme*)curAppTheme->getTheme())->sqlAttList();
	string field = attrVec[mainWindow_->gridColumn()].rep_.name_;

	calculateStatistic(field);
}


void StatisticWindow::objectsComboBox_activated( int )
{
	input_ = objectsComboBox->currentItem();
	if(input_ > 4)
		input_ += 2;

	TeAppTheme* appTheme = mainWindow_->currentTheme();
	TeAttributeList attrVec = ((TeTheme*)appTheme->getTheme())->sqlAttList();
	string field = attrVec[mainWindow_->gridColumn()].rep_.name_;

	calculateStatistic(field);
}


void StatisticWindow::calculateStatistic(string fieldname )
{
	TeWaitCursor wait;
	string resumo = summary_.latin1();

	statisticTable->setNumRows(0);
	statisticTable->setNumCols(0);

	TeAppTheme *currentAppTheme = mainWindow_->currentTheme();
	TeTheme* currentTheme = (TeTheme*)currentAppTheme->getTheme();
	TeQtThemeItem *currentThemeItem = mainWindow_->getViewsListView()->currentThemeItem();
	TeQtProgress *progress = mainWindow_->getProgress();
	vector<string>::iterator it_begin;
	vector<string>::iterator it_end;

	if(currentTheme->getProductId() == TeTHEME)
	{
		string	sel;
		string	TS = currentTheme->collectionTable();
		string	TSE = currentTheme->collectionAuxTable();
		TeDatabase *db = mainWindow_->currentDatabase();

		sel = "SELECT " + fieldname + ", " + TS + ".c_object_id" + currentTheme->sqlGridFrom();

		string isel;
		if(input_ == TeSelectedByPointing)
		{
			isel = " WHERE (grid_status = 1 OR grid_status = 3";
			isel += " OR (grid_status is null AND (c_object_status = 1 OR c_object_status = 3)))";
		}
		else if(input_ == TeNotSelectedByPointing)
		{
			isel = " WHERE (grid_status = 0 OR grid_status = 2";
			isel += " OR (grid_status is null AND (c_object_status = 0 OR c_object_status = 2)))";
		}
		else if(input_ == TeSelectedByQuery)
		{
			isel = " WHERE (grid_status = 2 OR grid_status = 3";
			isel += " OR (grid_status is null AND (c_object_status = 2 OR c_object_status = 3)))";
		}
		else if(input_ == TeNotSelectedByQuery)
		{
			isel = " WHERE (grid_status = 0 OR grid_status = 1";
			isel += " OR (grid_status is null AND (c_object_status = 0 OR c_object_status = 1)))";
		}
		else if(input_ == TeGrouped)
			isel =" WHERE c_legend_id <> 0";
		else if(input_ == TeNotGrouped)
			isel =" WHERE c_legend_id = 0";
		sel += isel;

		if(summary_ == tr("NONE"))
		{
			statisticTable->setNumRows(14);
			statisticTable->setNumCols(2);
			getValues(sel, fieldname, 0);
		}
		else if(summary_ == tr("Groups"))
		{
			int ngrupos = 0;
			string collection = currentTheme->collectionTable();
			string selLeg = "SELECT DISTINCT " + TS + ".c_legend_id " + currentTheme->sqlGridFrom();

			if(isel.empty() == false)
				isel.replace(0, strlen(" WHERE "), " AND ");

			selLeg += " WHERE " + TS + ".c_legend_id > 0" + isel;
			string conta = "SELECT COUNT(*) FROM (" + selLeg + ")";

			if((db->dbmsName() == "PostgreSQL") || (db->dbmsName() == "PostGIS"))
				conta += " AS r ";

			TeDatabasePortal *portal = db->getPortal();
			if(portal->query(conta))
			{
				if(portal->fetchRow())
					ngrupos = portal->getInt(0);
			}
			if(ngrupos == 0)
			{
				QMessageBox::warning(this, tr("Warning"),
					tr("There are no groups to calculate the statistics!"));
				delete portal;
				return;
			}

			portal->freeResult();
			vector<long> legIds;
			selLeg += " ORDER BY c_legend_id";
			if(portal->query(selLeg))
			{
				while(portal->fetchRow())
					legIds.push_back(portal->getInt(0));
			}

			map<int, int> legend2PixmapItem;
			string ss = "SELECT DISTINCT c_legend_id FROM " + currentTheme->collectionTable();
			ss += " WHERE c_legend_id > 0 ORDER BY c_legend_id";
			int	item = 0;
			portal->freeResult();
			if(portal->query(ss))
			{
				while(portal->fetchRow())
					legend2PixmapItem[portal->getInt(0)] = item++;
			}
			delete portal;

			int numLines = 19 * legIds.size();
			statisticTable->setNumRows(0);
			statisticTable->setNumRows(numLines);
			statisticTable->setNumCols(2);

			vector<const QPixmap*> pixs;
			vector<string> labels;
			string title;
			currentThemeItem->getLegends(pixs, labels, title);
			int f;
			if((f= title.find(":")) >= 0)
				title = title.substr(f+1);

			progress->setCaption(tr("Computing Statistics").latin1());
			progress->setLabelText(tr("Statistics is being computed. Please, wait!"));
			progress->setTotalSteps(pixs.size());
			unsigned int i = 0;
			while(i < pixs.size())
			{
				string ssel = sel;
				if(isel.empty())
					ssel += " WHERE " + TS + ".c_legend_id = " + Te2String(legIds[i]);
				else
					ssel += " AND " + TS + ".c_legend_id = " + Te2String(legIds[i]);

				int j = i * 19;
				statisticTable->setText(j, 0, tr("Grouped by"));
				statisticTable->setText(j, 1, title.c_str());
	//			statisticTable->setPixmap(j+1, 0, *(pixs[legend2PixmapItem[legIds[i]]]));
				statisticTable->setText(j+1, 0, tr("range"));
				statisticTable->setText(j+1, 1, labels[legend2PixmapItem[legIds[i]]].c_str());
				statisticTable->setText(j+2, 0, tr("Statistic of"));
				statisticTable->setText(j+2, 1, fieldname.c_str());
				getValues(ssel, fieldname, j+3);
				i++;
				if(progress->wasCancelled())
					break;
				progress->setProgress(i);
			}
			progress->reset();
		}
		else
		{
			string rsel = "SELECT DISTINCT " + resumo + currentTheme->sqlGridFrom() + isel + " ORDER BY " + resumo;

			vector<string> summaryVec;
			TeDatabasePortal *portal = db->getPortal();
			if(portal->query(rsel))
			{
				while(portal->fetchRow())
				{
					string s = db->escapeSequence(portal->getData(0));
					summaryVec.push_back(s);
				}
			}
			delete portal;

			if(summaryVec.size() == 0)
				return;

			int numLines = 19 * summaryVec.size();
			statisticTable->setNumRows(0);
			statisticTable->setNumRows(numLines);
			statisticTable->setNumCols(2);

			TeAttributeList attrVec = currentTheme->sqlAttList();
			int type;
			unsigned int i;
			for(i=0; i<attrVec.size(); i++)
			{
				if(attrVec[i].rep_.name_ == resumo)
				{
					type = attrVec[i].rep_.type_;
					break;
				}
			}

			progress->setCaption(tr("Computing Statistics").latin1());
			progress->setLabelText(tr("Statistics is being computed. Please, wait!"));
			progress->setTotalSteps(summaryVec.size());
			i = 0;
			while(i < summaryVec.size())
			{
				string ssel = sel;
				if(isel.empty())
				{
					if(summaryVec[i].empty() == true)
						ssel += " WHERE " + resumo + " is null";
					else
					{
						if(type == TeSTRING)
							ssel += " WHERE " + resumo + " = '" + summaryVec[i] + "'";
						else
							ssel += " WHERE " + resumo + " = " + summaryVec[i];
					}
				}
				else
				{
					if(summaryVec[i].empty() == true)
						ssel += " AND " + resumo + " is null";
					else
					{
						if(type == TeSTRING)
							ssel += " AND " + resumo + " = '" + summaryVec[i] + "'";
						else
							ssel += " AND " + resumo + " = " + summaryVec[i];
					}
				}

				int j = i * 19;
				statisticTable->setText(j, 0, tr("Grouped by"));
				statisticTable->setText(j, 1, summary_);
				statisticTable->setText(j+1, 0, summary_);
				statisticTable->setText(j+1, 1, summaryVec[i].c_str());
				statisticTable->setText(j+2, 0, tr("Statistic of"));
				statisticTable->setText(j+2, 1, fieldname.c_str());
				getValues(ssel, fieldname, j+3);
				i++;
				if(progress->wasCancelled())
					break;
				progress->setProgress(i);
			}
			progress->reset();
		}
	}
	else if(currentTheme->getProductId() == TeEXTERNALTHEME)
	{
		map<string, int>::iterator it;
		map<string, int>& legMap = currentTheme->getObjLegendMap();
		vector<string> idVec;
		string	sel;
		string	TS = ((TeExternalTheme*)currentTheme)->getRemoteTheme()->collectionTable();
		string	TSE = ((TeExternalTheme*)currentTheme)->getRemoteTheme()->collectionAuxTable();

		sel = "SELECT " + fieldname + ", " + TS + ".c_object_id" + ((TeExternalTheme*)currentTheme)->getRemoteTheme()->sqlGridFrom();
		sel += " WHERE " + TSE + ".unique_id IN ";

		idVec = getItems(currentTheme, input_);

		if(idVec.size() == 0)
		{
			QMessageBox::warning(this, tr("Warning"),
				tr("There are no objects to calculate the statistics!"));
			return;
		}

		TeDatabase* edb = ((TeExternalTheme*)currentTheme)->getRemoteTheme()->layer()->database();
		vector<string> inVec;

		if(summary_ == tr("NONE"))
		{
			it_begin = idVec.begin();
			it_end = idVec.end();
			inVec = generateInClauses(it_begin, it_end, edb, false);
			statisticTable->setNumRows(14);
			statisticTable->setNumCols(2);
			getValues(sel, inVec, fieldname, 0);
		}
		else if(summary_ == tr("Groups"))
		{
			map<int, vector<string> > mv;
			map<int, vector<string> >::iterator m;
			set<int> legIdSet;
			for(it=legMap.begin(); it!=legMap.end(); ++it)
			{
				legIdSet.insert(it->second);
				mv[it->second].push_back(it->first);
			}			

			int ngrupos = (int)legIdSet.size();
			if(ngrupos == 0)
			{
				QMessageBox::warning(this, tr("Warning"),
					tr("There are no groups to calculate the statistics!"));
				return;
			}

			string sa, sb;
			vector<string>::iterator a, b;
			map<int, vector<string> > fv, ffv;
			if(input_ == TeAll)
			{
				for(m=mv.begin(); m!=mv.end(); ++m)
					ffv[m->first] = getItems(currentTheme, m->second);
			}
			else
			{
				for(m=mv.begin(); m!=mv.end(); ++m)
					fv[m->first] = getItems(currentTheme, m->second);

				for(m=fv.begin(); m!=fv.end(); ++m)
				{
					for(a=m->second.begin(); a!=m->second.end(); ++a)
					{
						sa = *a;
						for(b=idVec.begin(); b!=idVec.end(); ++b)
						{
							sb = *b;
							if(sa == sb)
							{
								ffv[m->first].push_back(sa);
								break;
							}
						}
					}
				}
			}

			int numLines = 19 * ngrupos;
			statisticTable->setNumRows(0);
			statisticTable->setNumRows(numLines);
			statisticTable->setNumCols(2);

			vector<const QPixmap*> pixs;
			vector<string> labels;
			string title;
			currentThemeItem->getLegends(pixs, labels, title);
			int f;
			if((f= title.find(":")) >= 0)
				title = title.substr(f+1);

			progress->setCaption(tr("Computing Statistics").latin1());
			progress->setLabelText(tr("Statistics is being computed. Please, wait!"));
			progress->setTotalSteps(pixs.size());
			unsigned int i = 0;
			set<int>::iterator lit = legIdSet.begin();
			while(i < pixs.size())
			{
				if(lit == legIdSet.end())
					break;

				int j = i * 19;
				statisticTable->setText(j, 0, tr("Grouped by"));
				statisticTable->setText(j, 1, title.c_str());
	//			statisticTable->setPixmap(j+1, 0, *(pixs[legend2PixmapItem[legIds[i]]]));
				statisticTable->setText(j+1, 0, tr("range"));
				statisticTable->setText(j+1, 1, labels[i].c_str());
				statisticTable->setText(j+2, 0, tr("Statistic of"));
				statisticTable->setText(j+2, 1, fieldname.c_str());

				int legId = *lit;
				it_begin = ffv[legId].begin();
				it_end = ffv[legId].end();
				inVec = generateInClauses(it_begin, it_end, edb, false);

				getValues(sel, inVec, fieldname, j+3);
				i++;
				lit++;
				if(progress->wasCancelled())
					break;
				progress->setProgress(i);
			}
			progress->reset();
		}
		else
		{
			it_begin = idVec.begin();
			it_end = idVec.end();
			inVec = generateInClauses(it_begin, it_end, edb, false);

			vector<string> summaryVec = getSumaryVec(currentTheme, resumo, inVec);

			if(summaryVec.size() == 0)
				return;

			int numLines = 19 * summaryVec.size();
			statisticTable->setNumRows(0);
			statisticTable->setNumRows(numLines);
			statisticTable->setNumCols(2);

			TeAttributeList attrVec = currentTheme->sqlAttList();
			int type;
			unsigned int i;
			for(i=0; i<attrVec.size(); i++)
			{
				if(attrVec[i].rep_.name_ == resumo)
				{
					type = attrVec[i].rep_.type_;
					break;
				}
			}

			progress->setCaption(tr("Computing Statistics").latin1());
			progress->setLabelText(tr("Statistics is being computed. Please, wait!"));
			progress->setTotalSteps(summaryVec.size());
			string resumoVal;
			i = 0;
			while(i < summaryVec.size())
			{
				if(summaryVec[i].empty() == true)
					resumoVal = resumo + " is null";
				else
				{
					if(type == TeSTRING)
						resumoVal = resumo + " = '" + summaryVec[i] + "'";
					else
						resumoVal = resumo + " = " + summaryVec[i];
				}

				int j = i * 19;
				statisticTable->setText(j, 0, tr("Grouped by"));
				statisticTable->setText(j, 1, summary_);
				statisticTable->setText(j+1, 0, summary_);
				statisticTable->setText(j+1, 1, summaryVec[i].c_str());
				statisticTable->setText(j+2, 0, tr("Statistic of"));
				statisticTable->setText(j+2, 1, fieldname.c_str());
				getValues(sel, inVec, resumoVal, fieldname, j+3);
				i++;
				if(progress->wasCancelled())
					break;
				progress->setProgress(i);
			}
			progress->reset();
		}
	}

	statisticTable->adjustColumn(0);
	statisticTable->adjustColumn(1);
}

void StatisticWindow::getValues(string& sel, string& fieldname, int offset )
{
	double sum = 0.;
	double sm2 = 0.;
	double sm3 = 0.;
	double sm4 = 0.;
	int	nulos = 0;
	vector<double> vd;
	set<string> oidSet;

	TeDatabase *db = mainWindow_->currentDatabase();
	TeDatabasePortal *portal = db->getPortal();
	if(portal->query(sel))
	{
		bool b = portal->fetchRow(0);
		while(b)
		{
			string s = portal->getData(0);
			string oid = portal->getData(1);
			oidSet.insert(oid);
			if(s.empty())
				nulos++;
			else
			{
				double d = atof(s.c_str());
				vd.push_back(d);
				sum += d;
				double dd = d * d;
				double ddd = dd * d;
				double dddd = ddd * d;
				sm2 += dd;
				sm3 += ddd;
				sm4 += dddd;
			}
			b = portal->fetchRow();
		}
	}
	delete portal;

	statisticCalc(vd, sum, sm2, sm3, sm4, nulos, oidSet, fieldname, offset);
}

void StatisticWindow::getValues(string& sel, vector<string>& inVec, string& fieldname, int offset)
{
	double sum = 0.;
	double sm2 = 0.;
	double sm3 = 0.;
	double sm4 = 0.;
	int	nulos = 0;
	vector<double> vd;
	set<string> oidSet;

	TeAppTheme *currentAppTheme = mainWindow_->currentTheme();
	TeTheme* currentTheme = (TeTheme*)currentAppTheme->getTheme();
	TeDatabase* db = ((TeExternalTheme*)currentTheme)->getRemoteTheme()->layer()->database();
	TeDatabasePortal* portal = db->getPortal();

	vector<string>::iterator it;
	for(it=inVec.begin(); it!=inVec.end(); ++it)
	{
		if((*it).empty() == false)
		{
			string query = sel + *it;
			if (portal->query(query) == false)
			{
				string message = portal->errorMessage();
				QMessageBox::critical(this, tr("Error"), message.c_str());
				delete portal;
				return;
			}
			while (portal->fetchRow())
			{
				string val = portal->getData(0);
				string oid = portal->getData(1);
				oidSet.insert(oid);
				if(val.empty())
					nulos++;
				else
				{
					double d = atof(val.c_str());
					vd.push_back(d);
					sum += d;
					double dd = d * d;
					double ddd = dd * d;
					double dddd = ddd * d;
					sm2 += dd;
					sm3 += ddd;
					sm4 += dddd;
				}
			}
			portal->freeResult();
		}
	}
	delete portal;

	statisticCalc(vd, sum, sm2, sm3, sm4, nulos, oidSet, fieldname, offset);
}

void StatisticWindow::getValues(string& sel, vector<string>& inVec, string& resumoVal, string& fieldname, int offset)
{
	double sum = 0.;
	double sm2 = 0.;
	double sm3 = 0.;
	double sm4 = 0.;
	int	nulos = 0;
	vector<double> vd;
	set<string> oidSet;

	TeAppTheme *currentAppTheme = mainWindow_->currentTheme();
	TeTheme* currentTheme = (TeTheme*)currentAppTheme->getTheme();
	TeDatabase* db = ((TeExternalTheme*)currentTheme)->getRemoteTheme()->layer()->database();
	TeDatabasePortal* portal = db->getPortal();

	vector<string>::iterator it;
	for(it=inVec.begin(); it!=inVec.end(); ++it)
	{
		if((*it).empty() == false)
		{
			string query = sel + *it + " AND " + resumoVal;
			if (portal->query(query) == false)
			{
				string message = portal->errorMessage();
				QMessageBox::critical(this, tr("Error"), message.c_str());
				delete portal;
				return;
			}
			while (portal->fetchRow())
			{
				string val = portal->getData(0);
				string oid = portal->getData(1);
				oidSet.insert(oid);
				if(val.empty())
					nulos++;
				else
				{
					double d = atof(val.c_str());
					vd.push_back(d);
					sum += d;
					double dd = d * d;
					double ddd = dd * d;
					double dddd = ddd * d;
					sm2 += dd;
					sm3 += ddd;
					sm4 += dddd;
				}
			}
			portal->freeResult();
		}
	}

	delete portal;

	statisticCalc(vd, sum, sm2, sm3, sm4, nulos, oidSet, fieldname, offset);
}

void StatisticWindow::statisticCalc(vector<double>& vd, double sum, double sm2, double sm3, double sm4, int nulos, set<string>& oidSet, string& fieldname, int offset)
{
	double mean = 0.;

	if(vd.size() < 1)
	{
//		QMessageBox::warning(this, tr("Warning"),
//		tr("Insufficient data quantity\nto calculate the statistics!");
		return;
	}

	mean = sum / (double)vd.size();
	std::sort(vd.begin(), vd.end());

	double vmin = vd[0];
	double vmax = vd[vd.size()-1];

//-----> Calculate Median
	int index = vd.size()/2;

	double median;
	if ((vd.size()%2)==0) //list size is even 
		median = (vd[index] + vd[index-1])/2.;
	else
		median = vd[index];

//-----> Calculate QuartileLower
	double quartileLower;
	int meio = vd.size() / 2;
	if(vd.size()%2)
		meio++;
	index = meio / 2;
	if(meio%2)
		quartileLower = vd[index];
	else
		quartileLower = (vd[index] + vd[index-1])/2.;

//-----> Calculate QuartileUpper
	double quartileUpper;
	if(vd.size() > 1)
	{
		index += meio;
		if(meio%2)
			quartileUpper = vd[index];
		else
			quartileUpper = (vd[index] + vd[index-1])/2.;
	}
	else
		quartileUpper = quartileLower;

//-----> Calculate Variance
	double variance = (sm2/(double)vd.size()) - (mean*mean);
	
//-----> Calculate Standard Deviation
	double stdv = sqrt(variance);
	
//-----> Calculate Coeficiente of the Variation
	double cvar = stdv / mean;

//-----> Calculate the moment of the 3 order
	double m3 = sm3/(double)vd.size() - 3.*mean*(sm2/(double)vd.size()) + 2.*(mean*mean*mean);
		
//-----> Calculate the Skewness
	double skewness = m3 / (stdv*stdv*stdv);

//-----> Calculate the moment of the 4 order
	double m4 = (sm4/(double)vd.size()) - (4.*mean*(sm3/(double)vd.size())) + (6.*(mean*mean)*(sm2/(double)vd.size())) - (3.*(mean*mean*mean*mean));
		
//-----> Calculate the coeficient kurtose
	double kurtose = m4 / (stdv*stdv*stdv*stdv);

	statisticTable->horizontalHeader()->setLabel(0, tr("Description"));
	statisticTable->horizontalHeader()->setLabel(1, tr("Value"));

	string caption = tr("Statistic:").latin1();
	caption += " " + fieldname;
	setCaption(caption.c_str());
	statisticTable->setText(offset + 0, 0, tr("Number of Items"));
	int tot = vd.size()+nulos;
	statisticTable->setText(offset + 0, 1, Te2String(tot).c_str());
	statisticTable->setText(offset + 1, 0, tr("Number of nulls"));
	statisticTable->setText(offset + 1, 1, Te2String(nulos).c_str());
	statisticTable->setText(offset + 2, 0, tr("Number of Objects"));
	statisticTable->setText(offset + 2, 1, Te2String(oidSet.size()).c_str());
	statisticTable->setText(offset + 3, 0, tr("Minimum"));
	statisticTable->setText(offset + 3, 1, Te2String(vmin, 9).c_str());
	statisticTable->setText(offset + 4, 0, tr("Maximum"));
	statisticTable->setText(offset + 4, 1, Te2String(vmax, 9).c_str());
	statisticTable->setText(offset + 5, 0, tr("Mean"));
	statisticTable->setText(offset + 5, 1, Te2String(mean, 9).c_str());
	statisticTable->setText(offset + 6, 0, tr("Pattern Deviation"));
	statisticTable->setText(offset + 6, 1, Te2String(stdv, 9).c_str());
	statisticTable->setText(offset + 7, 0, tr("Median"));
	statisticTable->setText(offset + 7, 1, Te2String(median, 9).c_str());
	statisticTable->setText(offset + 8, 0, tr("Sum"));
	statisticTable->setText(offset + 8, 1, Te2String(sum, 9).c_str());
	statisticTable->setText(offset + 9, 0, tr("Variance"));
	statisticTable->setText(offset + 9, 1, Te2String(variance, 9).c_str());
	statisticTable->setText(offset + 10, 0, tr("Variance Coefficient"));
	statisticTable->setText(offset + 10, 1, Te2String(cvar, 9).c_str());
	statisticTable->setText(offset + 11, 0, tr("Inferior Quartil"));
	statisticTable->setText(offset + 11, 1, Te2String(quartileLower, 9).c_str());
	statisticTable->setText(offset + 12, 0, tr("Superior Quartil"));
	statisticTable->setText(offset + 12, 1, Te2String(quartileUpper, 9).c_str());
	statisticTable->setText(offset + 13, 0, tr("Skewness"));
	statisticTable->setText(offset + 14, 0, tr("Kurtose"));
	if(vd.size() > 1)
	{
		statisticTable->setText(offset + 13, 1, Te2String(skewness, 9).c_str());
		statisticTable->setText(offset + 14, 1, Te2String(kurtose, 9).c_str());
	}
}


void StatisticWindow::savePushButton_clicked()
{
	FILE *fp;
	QString qa, qb;
	string	a, b, c;
//	char	buf[500];
	int	i, j;
	int n = statisticTable->numRows();

    QString s = QFileDialog::getSaveFileName(
                    "",
                    tr("Text (*.txt)"),
                    this,
                    tr("Save File Dialog"),
                    tr("Select a filename to save under"));
	if(s.isNull() || s.isEmpty())
		return;

	if(s.findRev(".txt", -1, false) == -1)
		s.append(".txt");

	string file = s.latin1();
	if((fp = fopen(file.c_str(), "w")) == 0)
	{
		QMessageBox::critical(this, tr("Error"),
			tr("Fail to open the file") + " \"" + file.c_str() + "\" !\n" + tr("The statistics could not be saved."));
		return;
	}

	string title = caption().latin1();
	title += "\n\n";
	if((i = title.find(":")) >= 0)
		title.replace(i, 1, " de");

	fwrite(title.c_str(), sizeof( char ), title.size(), fp);

	for(i=0; i<n; i++)
	{
		a.clear();
		b.clear();
		qa = statisticTable->text(i, 0);
		qb = statisticTable->text(i, 1);
		if(qa.isEmpty() == false)
			a = qa.latin1();
		if(qb.isEmpty() == false)
			b = qb.latin1();

		if(a.empty() == false)
			a += ":";

		if(a.empty() && b.empty())
		{
			fwrite("---------------------------------------------------\n\n", sizeof( char ), strlen("---------------------------------------------------\n\n"), fp);
			continue;
		}

		int d = 32 - a.size();
		if(d%8)
		{
			a += "\t";
			d -= (d%8);
		}

		int dd = d/8;
		for(j=0; j<dd; j++)
			a += "\t";
		c = a + b + "\n";

		fwrite(c.c_str(), sizeof( char ), c.size(), fp);
	}
	if(fp)
		fclose(fp);

	QMessageBox::information(this, tr("Information"), 
		tr("The statistics was saved in the file") + " \"" + file.c_str() + "\"");
}

void StatisticWindow::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("statistic.htm");
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

vector<string> StatisticWindow::getSumaryVec(TeTheme* theme, string resumo, vector<string> inVec)
{
	string	TS = ((TeExternalTheme*)theme)->getRemoteTheme()->collectionTable();
	string	TSE = ((TeExternalTheme*)theme)->getRemoteTheme()->collectionAuxTable();
	TeDatabase* db = ((TeExternalTheme*)theme)->getRemoteTheme()->layer()->database();
	TeDatabasePortal* portal = db->getPortal();
	vector<string> summaryVec;

	string sel = "SELECT DISTINCT " + resumo + theme->sqlGridFrom();
	sel += " WHERE " + TSE + ".unique_id IN ";

	set<string> idSet;
	vector<string>::iterator it;
	for(it=inVec.begin(); it!=inVec.end(); ++it)
	{
		if((*it).empty() == false)
		{
			string query = sel + *it + " ORDER BY " + resumo;
			if (portal->query(query) == false)
			{
				string message = portal->errorMessage();
				QMessageBox::critical(this, tr("Error"), message.c_str());
				delete portal;
				return summaryVec;
			}
			while (portal->fetchRow())
			{
				string s = db->escapeSequence(portal->getData(0));
				idSet.insert(s);
			}
			portal->freeResult();
		}
	}
	delete portal;

	set<string>::iterator sit;
	for(sit=idSet.begin(); sit!=idSet.end(); ++sit)
		summaryVec.push_back(*sit);
	return summaryVec;
}





