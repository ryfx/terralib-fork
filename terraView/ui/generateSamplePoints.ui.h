/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "TeBox.h"
#include "TeTheme.h"
#include "TeDatabase.h"
#include "TeUtils.h"
#include "TeWaitCursor.h"
#include "TeGUIUtils.h"
#include "TeQuerier.h"

#include "mtrand.h"

#include <string>

#include <assert.h>

#include <qmessagebox.h>
#include <qstring.h>

using std::string;

unsigned int  TeGeneratePointsInPolygon(const TePolygon& pol, const unsigned int npts, const double& res, vector<TeCoord2D>& result)
{
	MTRand_int32 iirand;
	MTRand drand;
	int nsegsv = (int)(pol.box().height()/res);
	if (nsegsv <=0 )
		return 0;

	unsigned int i, j, aux = result.size();
	TeCoordPairVect inters;	
	unsigned int nptsg = 0;
	while (nptsg < npts)
	{
		unsigned long val =  iirand();
		i = val%nsegsv;
		inters = TeGetIntersections(pol,pol.box().y1()+ i*res);
		if (inters.empty())
			continue;
		j = iirand()%inters.size();
		val = (unsigned long)(drand()*(inters[j].pt2.x_- inters[j].pt1.x_) + inters[j].pt1.x_);
		result.push_back(TeCoord2D(val,pol.box().y1()+ i*res));
		++nptsg;
	}
	return (result.size()-aux);
}

void GenerateSamplePoints::okPushButton_clicked()
{
	if (!db_)
	{
		reject();
	}
	
	if (outLayerName->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Inform a name to the new layer of points."));
		return;
	}

	string layerName = outLayerName->text().latin1();
	string errorMessage;
	bool changed;
	string newName = TeCheckName(layerName, changed, errorMessage); 
	if(changed)
	{
		QString mess = tr("The output layer name is invalid: ") + errorMessage.c_str();
		mess += "\n" + tr("Change current name and try again.");
		QMessageBox::warning(this, tr("Warning"), mess);
		outLayerName->setText(newName.c_str());
		return;
	}
	layerName = newName;
	if (db_->layerExist(layerName))
	{
		QMessageBox::warning(this,tr("Warning"),
		tr("Output layer name already exist, replace and try again!"));
		return;
	}
	newLayer_ = new TeLayer(layerName,db_);
	assert(newLayer_->id() > 0);

	bool res = false;
	TeWaitCursor wait;
	if (random_radioButton->isChecked())// random generation
	{
		res = generateRandomPoints();
	}
	else		// stratified
	{
		res = generateStratifiedClass();
	}
	wait.resetWaitCursor();
	if (!res)
	{
		db_->deleteLayer(newLayer_->id());
		newLayer_=0;
		return;
	}
	if(!newLayer_->database()->insertMetadata(newLayer_->tableName(TePOINTS),newLayer_->database()->getSpatialIdxColumn(TePOINTS), 0.0005,0.0005,newLayer_->box()))
	{
		QString msg = tr((db_->errorMessage()).c_str());
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}

	if(!newLayer_->database()->createSpatialIndex(newLayer_->tableName(TePOINTS),newLayer_->database()->getSpatialIdxColumn(TePOINTS), (TeSpatialIndexType)TeRTREE))
	{
		QString msg = tr((db_->errorMessage()).c_str());
		QMessageBox::warning(this, tr("Warning"), msg);
		return;
	}
	accept();
}

bool GenerateSamplePoints::generateStratifiedClass()
{
	unsigned int npts_c = 0;
	if (strProp_radioButton->isChecked())
		npts_c = minPointPerClass->text().toInt();
	else
		npts_c = pointsPerClass->text().toInt();
	
	if (npts_c <= 0)
	{
		QString msg = tr("Enter with a valid (minimum) number of points per class  (>0).");
		QMessageBox::warning(this, tr("Warning"), msg);
		return false;
	}

	TeProjection* proj = 0;
	unsigned int totalpts = 0;
	TePointSet ps;

	if(classAttribute_ComboBox->currentText().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"),"Select a class attribute");
		classAttribute_ComboBox->setFocus();
		return false;
	}

	string table = classAttribute_ComboBox->currentText().section( '.', 0, 0 ).latin1();
	string column = classAttribute_ComboBox->currentText().section( '.', -1 ).latin1();

	QString themec = cclassThemes_ComboBox->currentText();
	const TeThemeMap& themeMap = db_->themeMap();
	TeThemeMap::const_iterator itthem = themeMap.begin();
	while (itthem != themeMap.end())
	{
		if (themec == (*itthem).second->name().c_str())
			break;
		++itthem;
	}
	assert(itthem != themeMap.end());
	TeTheme* th = (TeTheme*)itthem->second;
	proj = TeProjectionFactory::make(th->layer()->projection()->params());  // to clone layer projection
	newLayer_->setProjection(proj);

	TeGrouping& group = itthem->second->grouping();
	viewId_ = itthem->second->view();

	TeAttributeList themeAtt = th->sqlAttList();
	TeAttributeList::const_iterator itatt = themeAtt.begin();
	while (itatt != themeAtt.end())
	{
		if ((*itatt).rep_.name_ == column)
			break;
		++itatt;
	}
	assert(itatt !=  themeAtt.end());

	unsigned int numgroups = 0;
	vector<string> classes;
	TeLegendEntryVector legends;

	bool isLeg = false;
	if (useAttribute_RadioButton->isChecked())
	{
		// find out the classes available
		string sql = "SELECT DISTINCT " + column +  " FROM " + table;
		TeDatabasePortal* portal = db_->getPortal();
		if (!portal->query(sql))
		{
			delete portal;
			return false;
		}
		while (portal->fetchRow())
			classes.push_back(string(portal->getData(0)));
		delete portal;
		numgroups = classes.size();
	}
	else if (useLegendRadioButton->isChecked())
	{
		isLeg = true;
		legends = itthem->second->legend();
		numgroups = legends.size();
	}
	else
		return false;


	// attribute table for the sample points will include class information (second attribute)
	TeAttributeList attList;
	TeAttribute at;
	at.rep_.name_ = "object_id_";
	at.rep_.type_ = TeSTRING;
	at.rep_.isPrimaryKey_ = true;
	at.rep_.numChar_ = 16;
	attList.push_back(at);

	TeAttribute at2;
	at2.rep_.name_ = "classe";
	at.rep_.type_ = TeSTRING;
	at2.rep_.isPrimaryKey_ = false;
	at2.rep_.numChar_ = 100;
	attList.push_back(at2);

	TeTable newTable(newLayer_->name(),attList,"object_id_","object_id_");

	TeWaitCursor wait;
	TeTheme* theme = new TeTheme(*th);
	theme->id(0);
	theme->collectionAuxTable("");
	theme->collectionTable("");

	map<unsigned int, int> nptsPClass;
	unsigned int nn;
	string crit;

	if (strProp_radioButton->isChecked())
	{
		vector<string> aAtt;
		if (isLeg)
			aAtt.push_back(group.groupAttribute_.name_);
		else
			aAtt.push_back(classAttribute_ComboBox->currentText().latin1());

		TeGeomRep rep = (TeGeomRep)(itthem->second->visibleGeoRep() & ~TeTEXT);
		string geomTableName = th->layer()->tableName(rep);
		vector<double> vareas;
		unsigned int ng;
		for (ng=0; ng<numgroups; ++ng)			// for each class
		{
			if (isLeg)
			{
				crit = group.groupAttribute_.name_ + " >= " + legends[ng].from();
				crit += " AND " + group.groupAttribute_.name_ + " < " + legends[ng].to();
			}
			else
			{
				crit = (classAttribute_ComboBox->currentText() + " = ").latin1();
				if (itatt->rep_.type_ == TeSTRING)
					crit += "'" + classes[ng] + "'";
				else
					crit += classes[ng] ;
			}
			theme->attributeRest(crit);

			TeQuerierParams par(false, aAtt);
			par.setParams(theme);
			TeQuerier querier(par);
			if (!querier.loadInstances())
				continue;

			TeKeys objIds;
			TeSTInstance inst;
			while (querier.fetchInstance(inst))
			{
				objIds.push_back(inst.objectId());
			}	
			double val;
			if (db_->calculateArea(geomTableName,rep,objIds,val))
				vareas.push_back(val);
		}
		vector<double>::iterator pos = std::min_element(vareas.begin(),vareas.end());
		double minn = *pos;
		for (nn=0; nn<numgroups; ++nn)
			nptsPClass[nn] = (unsigned int)(vareas[nn]*npts_c/minn);
	}
	else
	{
		for (nn=0; nn<numgroups; ++nn)
			nptsPClass[nn] = npts_c;
	}


	MTRand_int32 irand((unsigned)time(0));

	theme->id(0);
	theme->collectionAuxTable("");
	theme->collectionTable("");

	if (TeProgress::instance())
		TeProgress::instance()->setTotalSteps(numgroups);
	clock_t	ti, tf;
	ti = clock();
	vector<TeCoord2D> res;
	for (nn=0; nn<numgroups; ++nn)			// for each class
	{
		npts_c = nptsPClass[nn];
		if (isLeg)
		{
			if (group.groupMode_ == TeUniqueValue)
			{
				if (group.groupAttribute_.type_ == TeSTRING)
					crit =  group.groupAttribute_.name_ + " = '" + legends[nn].from() + "'";
				else
					crit =  group.groupAttribute_.name_ + " = " + legends[nn].from();
			}
			else
			{
				crit = group.groupAttribute_.name_ + " >= " + legends[nn].from();
				crit += " AND " + group.groupAttribute_.name_ + " < " + legends[nn].to();
			}
		}
		else
		{
			crit = (classAttribute_ComboBox->currentText() + " = ").latin1();
			if (itatt->rep_.type_ == TeSTRING)
				crit += "'" + classes[nn] + "'";
			else
				crit += classes[nn] ;
		}
		theme->attributeRest(crit);

		TeQuerierParams par2(true, false);
		par2.setParams(theme);
		TeQuerier querier2(par2);
		if (!querier2.loadInstances())
			continue;

		// retrieve all the polygons associated to the class
		TePolygonSet polSet;
		TeSTInstance inst2;
		bool flag = false;
		do
		{
			flag = querier2.fetchInstance(inst2);
			if(!inst2.getGeometry(polSet))
				flag = false;
		}while (flag);
		unsigned int np = polSet.size();
		if (np < 1)
			continue;

		// distribute points uniformly per class
		unsigned int a1;
		unsigned int nptot = 0;
		for (a1=0; a1<npts_c; ++a1)
			nptot += TeGeneratePointsInPolygon(polSet[irand()%np],1,1,res);

		for (a1=0; a1<nptot; ++a1, ++totalpts)
		{
			TePoint pt(res[a1]);
			pt.objectId(Te2String(totalpts));
			ps.add(pt);
			TeTableRow row;
			row.push_back(Te2String(totalpts));
			if (isLeg)
				row.push_back(legends[nn].label());
			else
				row.push_back(classes[nn]);
			newTable.add(row);
		}
		res.clear();
		if(TeProgress::instance())
		{
			tf = clock();
			if (int((tf-ti)/CLOCKS_PER_SEC) > 3)
			{
				if (TeProgress::instance()->wasCancelled())
					break;
				TeProgress::instance()->setProgress(nn);
				ti = tf;
			}
		}
	}

	if (TeProgress::instance())
		TeProgress::instance()->reset();

	if (!newLayer_->addPoints(ps))
	{
		ps.clear();
		QMessageBox::critical(this, tr("Error"), tr("Error inserting points in the new layer."));
		reject();
		return false;
	}
	ps.clear();

	if (!newLayer_->saveAttributeTable(newTable))
	{
		newTable.clear();
		QMessageBox::critical(this, tr("Error"), tr("Error inserting the attributes of the points in layer."));
		return false;
	}	
	if (!db_->insertTableInfo(newLayer_->id(),newTable))
	{
		QMessageBox::critical( this, tr("Error"),
			tr("Fail to insert table information!"));
	}	
	newLayer_->addAttributeTable(newTable);
	newTable.clear();
	return true;
}

bool GenerateSamplePoints::generateStratifiedClassArea()
{
	return false;
}


bool GenerateSamplePoints::generateRandomPoints()
{
	TeProjection* proj = 0;
	unsigned int i;
	TePointSet ps;

	if (ntotalPoints->text().toInt() <= 0 )
	{
		QMessageBox::warning(this, tr("Warning"), tr("Enter with a valid number of points (>0)."));
		return false;
	}

	TeWaitCursor wait;
	TeBox bb;
	if (layerRadioButton->isChecked())
	{
		QString layerbb = layersComboBox->currentText();
		TeLayerMap& layerMap = db_->layerMap();
		TeLayerMap::iterator itlay = layerMap.begin();
		while (itlay != layerMap.end())
		{
			if (layerbb == (*itlay).second->name().c_str())
				break;
			++itlay;
		}
		assert(itlay != layerMap.end());
		bb = (*itlay).second->box();
		proj = (*itlay).second->projection();
		viewId_= 0;
	}
	else
	{
		QString themebb = themesComboBox->currentText();
		const TeThemeMap& themeMap = db_->themeMap();
		TeThemeMap::const_iterator itthem = themeMap.begin();
		while (itthem != themeMap.end())
		{
			if (themebb == (*itthem).second->name().c_str())
				break;
			++itthem;
		}
		assert(itthem != themeMap.end());
		bb = (*itthem).second->box();
		proj = ((TeTheme*)itthem->second)->layer()->projection();
		viewId_ = itthem->second->view();
	}

	newLayer_->setProjection(proj);
	unsigned int npts = ntotalPoints->text().toInt();
	vector<double> vx(npts);
	vector<double> vy(npts);

	MTRand drand;
	double val;
	double gain = bb.x2()-bb.x1();
	for (i=0; i<npts; ++i)
	{
		val = drand()*gain + bb.x1();
		vx[i] = val;
	}
	gain = bb.y2()-bb.y1();
	for (i=0; i<npts; ++i)
	{
		val = drand()*gain + bb.y1();;
		vy[i] = val;
	}
	
	for (i=0; i<npts; ++i)
	{
		TeCoord2D p = TeCoord2D(vx[i],vy[i]);
		TePoint pt(p);
		ps.add(pt);
	}

	if (ps.empty())
		return false;

	TeAttributeList attList;
	TeAttribute at;
	at.rep_.name_ = "object_id_";
	at.rep_.type_ = TeSTRING;
	at.rep_.isPrimaryKey_ = true;
	at.rep_.numChar_ = 16;
	attList.push_back(at);

	TeTable newTable(newLayer_->name(),attList,"object_id_","object_id_");

	for (unsigned int ii=0; ii<ps.size(); ++ii)
	{
		ps[ii].objectId(Te2String(ii));
		TeTableRow row;
		row.push_back(Te2String(ii));
		newTable.add(row);
	}

	if (!newLayer_->addPoints(ps))
	{
		ps.clear();
		db_->deleteLayer(newLayer_->id());
		QMessageBox::critical(this, tr("Error"), tr("Error inserting points in the new layer."));
		reject();
		return false;
	}
	ps.clear();

	if (!newLayer_->saveAttributeTable(newTable))
	{
		newTable.clear();
		QMessageBox::critical(this, tr("Error"), tr("Error inserting the attributes of the points in layer."));
		return false;
	}	
	if (!db_->insertTableInfo(newLayer_->id(),newTable))
	{
		QMessageBox::critical( this, tr("Error"),
			tr("Fail to insert table information!"));
	}	
	newLayer_->addAttributeTable(newTable);
	newTable.clear();
	return true;
}



void GenerateSamplePoints::init()
{
	help_ = 0;
	db_ = 0;
	newLayer_ = 0;
	ntotalPoints->setText("30");
	pointsPerClass->setText("30");
	minPointPerClass->setText("30");
	cclassThemes_ComboBox->clear();
	viewId_ = 0;
}

TeLayer * GenerateSamplePoints::getNewLayer()
{
	return newLayer_;
}

void GenerateSamplePoints::type_buttonGroup_clicked( int bt )
{
	if (bt == 0)
	{
		randomGeneration_groupBox->setEnabled(true);
		stratifiedGeneration_groupBox->setEnabled(false);
	}
	else
	{
		randomGeneration_groupBox->setEnabled(false);
		stratifiedGeneration_groupBox->setEnabled(true);
		stratMode_buttonGroup_clicked(0);
		cclassThemes_ComboBox_activated(cclassThemes_ComboBox->currentText());
	}
}


void GenerateSamplePoints::bb_buttonGroup_clicked( int bt )
{
	if (bt == 0)
	{
		layersComboBox->setEnabled(true);
		themesComboBox->setEnabled(false);
	}
	else
	{
		layersComboBox->setEnabled(false);
		themesComboBox->setEnabled(true);
	}
}


void GenerateSamplePoints::class_buttonGroup_clicked( int bt )
{
	if (bt == 0)
		classAttribute_ComboBox->setEnabled(true);
	else
		classAttribute_ComboBox->setEnabled(false);
}

void GenerateSamplePoints::cclassThemes_ComboBox_activated( const QString & themeName )
{
	const TeThemeMap themeMap = db_->themeMap();
	TeThemeMap::const_iterator itthem = themeMap.begin();
	while (itthem != themeMap.end())
	{
		if (themeName == (*itthem).second->name().c_str())
			break;
		++itthem;
	}
	if(itthem != themeMap.end())
	{
		TeGrouping& g = itthem->second->grouping();
		if (g.groupMode_ == TeNoGrouping)
			useLegendRadioButton->setEnabled(false);
		else
			useLegendRadioButton->setEnabled(true);

		vector<TeAttrDataType> attypes;
		attypes.push_back(TeSTRING);
		attypes.push_back(TeINT);
		attypes.push_back(TeUNSIGNEDINT);
		if (!fillColumnCombo(attypes, (TeTheme*)itthem->second, classAttribute_ComboBox, true))
		{
			QMessageBox::warning( this, tr("Error"),
				tr("Theme doesn't have integer or string columns to represent classes."));
		}
	}
}

void GenerateSamplePoints::stratMode_buttonGroup_clicked( int bt )
{
	if (bt == 0)
	{
		pointsPerClass->setEnabled(true);
		minPointPerClass->setEnabled(false);
	}
	else
	{
		pointsPerClass->setEnabled(false);
		minPointPerClass->setEnabled(true);
	}
}

void GenerateSamplePoints::setParams( TeDatabase * appDatabase, TeTheme* curTheme, TeLayer* curLayer)
{
	db_ = appDatabase;
	type_buttonGroup_clicked(0);
	bb_buttonGroup_clicked(0);

	unsigned int curr = 0, aux = 0;
	TeLayerMap& layerMap = db_->layerMap();
	TeLayerMap::iterator itlay = layerMap.begin();
	while ( itlay != layerMap.end() )
	{
		TeLayer* ll = (*itlay).second;
		layersComboBox->insertItem(ll->name().c_str());
		if (curLayer && curLayer->name() == ll->name())
			curr = aux;
		++aux;
		++itlay;
	}
	layersComboBox->setCurrentItem(curr);

	if (aux == 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("You should have at least one layer in your database."));
		return;
	}

	curr = 0; aux = 0;
	unsigned int ccurr = 0, caux=0;
	const TeThemeMap themeMap = db_->themeMap();
	TeThemeMap::const_iterator itthem = themeMap.begin();
	while ( itthem != themeMap.end() )
	{
		if ((*itthem).second->type()!= TeTHEME)
		{
			++itthem;
			continue;
		}
		themesComboBox->insertItem((*itthem).second->name().c_str());
		if (curTheme && curTheme->name() == (*itthem).second->name())
			curr = aux;

		// only themes with area representations can be used in stratified sample generation
		if ((*itthem).second->visibleRep() & TePOLYGONS || (*itthem).second->visibleRep() & TeCELLS)
		{
			cclassThemes_ComboBox->insertItem((*itthem).second->name().c_str());
			if (curTheme && curTheme->name() == (*itthem).second->name())
				ccurr = caux;
			++caux;
		}
        ++aux;
		++itthem;
	}
	themesComboBox->setCurrentItem(curr);
	cclassThemes_ComboBox->setCurrentItem(ccurr);
}


void GenerateSamplePoints::helpPushButton_clicked()
{
	if(help_)
		delete help_;

	help_ = new Help(this, "help", false);
	help_->init("generateSamplePoints.htm");
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


int GenerateSamplePoints::getViewId()
{
	return viewId_;
}
