#include <TeKmlTheme.h>

// TerraLib include files
#include <TeDatabase.h>

#include <kmlmanager.h>
#include <kmlinterpreter.h>
#include <kmlstylesmanager.h>

#include <kmlstyle.h>

bool 
TeKmlTheme::createKmlThemeTable(TeDatabase* db)
{
	if (!db)
		return false;

	if(db->tableExist("te_kml_theme"))
		return true;

	TeAttributeList attList;

	TeAttribute att1;
	att1.rep_.name_ = "theme_id";
	att1.rep_.isAutoNumber_ = false;
	att1.rep_.isPrimaryKey_ = true;
	att1.rep_.null_ = false;
	att1.rep_.type_ = TeINT;
	att1.rep_.numChar_ = 0;
	attList.push_back(att1);

	TeAttribute att2;
	att2.rep_.name_ = "file_name";
	att2.rep_.isAutoNumber_ = false;
	att2.rep_.isPrimaryKey_ = false;
	att2.rep_.null_ = false;
	att2.rep_.type_ = TeSTRING;
	att2.rep_.numChar_ = 255;
	attList.push_back(att2);

	TeAttribute att3;
	att3.rep_.name_ = "geom_rep";
	att3.rep_.isAutoNumber_ = false;
	att3.rep_.isPrimaryKey_ = false;
	att3.rep_.null_ = false;
	att3.rep_.type_ = TeINT;
	att3.rep_.numChar_ = 0;
	attList.push_back(att3);

	TeAttribute att4;
	att4.rep_.name_ = "proj_id";
	att4.rep_.isAutoNumber_ = false;
	att4.rep_.isPrimaryKey_ = false;
	att4.rep_.null_ = false;
	att4.rep_.type_ = TeINT;
	att4.rep_.numChar_ = 0;
	attList.push_back(att4);

	if (!db->createTable("te_kml_theme", attList)||
		!db->createRelation("fk_kml_theme_id", "te_kml_theme", "theme_id", "te_theme", "theme_id", true) ||
		!db->createRelation("fk_kml_theme_proj_id", "te_kml_theme", "proj_id", "te_projection", "projection_id", false))
		return false;
	return 1;
}

bool TeKmlTheme::getData(TePointSet & pointSet, TeLineSet & lineSet, TePolygonSet & polygonSet, TeTable & attrTable, TeTable & styleTable)
{

	if(_fileName.empty())
		return false; 

	tdk::KMLManager* mger = new tdk::KMLManager();

	try
	{			
		mger->parse(_fileName);		

		tdk::KMLInterpreter interpreter;
		interpreter.interpret(mger->getObjects(), _database, pointSet, lineSet, polygonSet, attrTable, styleTable);
				
	}
	catch(TeException&)
	{
		return false;
	}

	setPointSet(pointSet);
	setPolygonSet(polygonSet);
	setLineSet(lineSet);
	setAttrTable(attrTable);

	delete mger;

	return true;
}

TeKmlTheme::TeKmlTheme(const std::string& fileName, TeViewNode* parent, int view, int id)
: TeAbstractTheme(fileName, parent, view, id, (TeViewNodeType)TeKMLTHEME)
{	
	_fileName = fileName;
	_projection = 0;
	_database = 0;
}

TeKmlTheme::TeKmlTheme(const TeKmlTheme& other) : TeAbstractTheme(other)
{
	TeTable tableCopy = other.getAttrTable();

	std::string uniqueName = tableCopy.uniqueName();
	std::string linkName = tableCopy.linkName();

	this->_fileName = other._fileName;
	this->_attrTable = TeTable(tableCopy.name(), tableCopy.attributeList(), uniqueName, linkName);

	this->_projection = TeProjectionFactory::make(other._projection->params());
	this->_database = other._database;
	
}

TeKmlTheme::~TeKmlTheme()
{
	if(_projection != 0)
	{
		delete _projection;
	}
}

TeKmlTheme& TeKmlTheme::operator=(const TeKmlTheme& other)
{
	*(TeAbstractTheme*)this = other;

	TeTable tableCopy = other.getAttrTable();

	std::string uniqueName = tableCopy.uniqueName();
	std::string linkName = tableCopy.linkName();

	this->_fileName = other._fileName;
	this->_attrTable = TeTable(tableCopy.name(), tableCopy.attributeList(), uniqueName, linkName);

	this->_projection = TeProjectionFactory::make(other._projection->params());
	this->_database = other._database;

	return *this;
}

TeViewNode* TeKmlTheme::clone()
{
	TeKmlTheme* theme = new TeKmlTheme();
	*theme = *this;
	return theme;
}


void TeKmlTheme::setPolygonSet(const TePolygonSet & polygonSet)
{
	_polygonSet = polygonSet;
}

TePolygonSet TeKmlTheme::getPolygonSet()
{
	return _polygonSet;
}

void TeKmlTheme::setPointSet(const TePointSet & pointSet)
{
	_pointSet = pointSet;
}

TePointSet TeKmlTheme::getPointSet()
{
	return _pointSet;
}

void TeKmlTheme::setLineSet(const TeLineSet & lineSet)
{
	_lineSet = lineSet;
}

TeLineSet TeKmlTheme::getLineSet()
{
	return _lineSet;
}

void TeKmlTheme::setAttrTable(const TeTable & attrTable)
{
	_attrTable = attrTable;
}

TeTable TeKmlTheme::getAttrTable() const
{
	return _attrTable;
}

void TeKmlTheme::setFileName(const std::string & fileName)
{
	_fileName = fileName;
}

std::string TeKmlTheme::getFileName()
{
	return _fileName;
}

void TeKmlTheme::setLocalDatabase(TeDatabase* database)
{
	_database = database;
}

TeDatabase* TeKmlTheme::getLocalDatabase()
{
	return _database;
}

void TeKmlTheme::setThemeProjection(TeProjection* proj)
{
	if (_projection)
		delete _projection;
	_projection = proj; 
}

//! Returns a pointer to a projection that is the spatial reference for the objects of this theme
/*! Concrete classes should reimplement this method.*/
TeProjection* TeKmlTheme::getThemeProjection()
{
	return _projection;
}

//! Save the theme parameters
bool TeKmlTheme::save()
{
	//insert theme in database 
	if(id()==0)
	{
		if(!_database->insertTheme(this)) //updateThemeTable
		{
			_database->deleteTheme(this->id());
			return false;
		}
	}
	else
	{
		if(!_database->updateTheme(this)) //updateThemeTable
		{
			_database->deleteTheme(this->id());
			return false;
		}
	}
	return true;
		
}

//! Save the grouping parameters in memory when there is no chronon
bool TeKmlTheme::buildGrouping(const TeGrouping& g, TeSelectedObjects selectedObjects, vector<double>* dValuesVec)
{
	return true;
}

//! Save the grouping parameters in memory when there is chronon
bool TeKmlTheme::buildGrouping(const TeGrouping& g, TeChronon chr, vector<map<string, string> >& mapObjValVec)
{
	return true;
}


bool TeKmlTheme::saveGrouping(TeSelectedObjects /*selectedObjects = TeAll*/)
{
	return true;
}

//! Delete grouping
bool TeKmlTheme::deleteGrouping()
{
	return true;
}

//! Set the legend id for each object of the theme 
void TeKmlTheme::setLegendsForObjects()
{
	
}

//! Set the own legend id for each object of the theme 
void TeKmlTheme::setOwnLegendsForObjects()
{
	
}

/** @name Locate geometries
    Returns the geometry(ies) of the theme given coordinate
*/
//@{ 	
bool TeKmlTheme::locatePolygon(TeCoord2D& /*pt*/, TePolygon& /*polygon*/, const double& /*tol = 0.0*/)
{
	return true;
}

bool TeKmlTheme::locatePolygonSet(TeCoord2D& /*pt*/, double /*tol*/, TePolygonSet& /*polygons*/)
{
	return true;
}

bool TeKmlTheme::locateLine(TeCoord2D& /*pt*/, TeLine2D& /*line*/, const double& /*tol = 0.0*/)
{
	return true;
}

bool TeKmlTheme::locatePoint(TeCoord2D& /*pt*/, TePoint& /*point*/, const double& /*tol = 0.0*/)
{
	return true;
}

bool TeKmlTheme::locateCell(TeCoord2D& /*pt*/, TeCell& /*c*/, const double& /*tol = 0.0*/)
{
	return true;
}
//@}

//! Get the set of objects corresponding to the object selection criteria
set<string> TeKmlTheme::getObjects(TeSelectedObjects selectedObjects)
{
	set<string> obj;

	return obj;
}

//! Get the set of objects corresponding to the list of items
set<string> TeKmlTheme::getObjects(const vector<string>& itemVec)
{
	set<string> obj;

	return obj;
}

//! Get the set of items corresponding to the object selection criteria
vector<string> TeKmlTheme::getItemVector(TeSelectedObjects selectedObjects)
{
	vector<string> itemVector;

	return itemVector;
}

//! Get the set of items corresponding to the set of objects
vector<string> TeKmlTheme::getItemVector(const set<string>& oidSet)
{
	vector<string> itemVector;

	return itemVector;
}

//! Get the number of objects acessible by this theme
unsigned int TeKmlTheme::getNumberOfObjects()
{
	return 0;
}

//! Save (insert or update) the theme metadata in the database
bool TeKmlTheme::saveMetadata(TeDatabase* database)
{
	if(!database)
		return false;

	//insert theme in database 
	if(id()==0)
	{
		if(!database->insertTheme(this)) //updateThemeTable
		{
			database->deleteTheme(this->id());
			return false;
		}
	}

	if (!_projection)
		_projection = new TeNoProjection();
	if (_projection->id() <= 0)
		database->insertProjection(_projection);

	// remove the metadata currently stored
	string strSQL;
	strSQL = "DELETE FROM te_kml_theme WHERE theme_id = ";
 	strSQL += Te2String(id());
    database->execute(strSQL);

	// insert the current metadata
	strSQL  = "INSERT INTO te_kml_theme (theme_id, file_name, geom_rep, proj_id) VALUES (";
	strSQL += Te2String(id());
	strSQL += ", '";
	strSQL += _fileName;
	strSQL += "', ";
	strSQL += Te2String(visibleRep_);
	strSQL += ", ";
	strSQL += Te2String(_projection->id());
	strSQL += ")";

	if(!database->execute(strSQL))
	{
		database->deleteTheme(this->id());
		return false;
	}
	return true;
}

//! Load the theme metadata from database
bool TeKmlTheme::loadMetadata(TeDatabase* database)
{	
	if(!database)
		return false;

	//second: load specific theme information from te_file_theme table
	TeDatabasePortal* portal = database->getPortal();
	if(!portal)
		return false;

	//table: theme_id, file_name, geom_rep
	std::string sql = " SELECT te_kml_theme.*, ";	
	sql += " te_projection.* ";						 
	sql += " FROM te_kml_theme, te_projection ";
	sql += " WHERE te_kml_theme.proj_id = te_projection.projection_id ";
	sql += " AND theme_id = "+ Te2String(id());

	if(!portal->query(sql) || !portal->fetchRow())
	{
		delete portal;
		return false;
	}

	_fileName = portal->getData(1);
	TeAbstractTheme::visibleRep(portal->getInt(2));

	/*
	FILE* fp = fopen(fileName_.c_str(), "r");
	if(!fp)
		return false;
	else
		fclose(fp);
	*/
	
	delete _projection;
	portal->getProjection(&_projection, 4);

	delete portal;

	setLocalDatabase(database);

	return true;
}

//! Erase the theme metadata in the database
bool TeKmlTheme::eraseMetadata(TeDatabase* database)
{
	if (!database)
		return false;

	int proid = -1;
	string strSQL = "SELECT proj_id FROM te_kml_theme WHERE theme_id = "+ Te2String(this->id());
	TeDatabasePortal* portal = database->getPortal();
	if (portal->query(strSQL) && portal->fetchRow())
		proid = portal->getInt(0);
	delete portal;

	strSQL  = " DELETE FROM te_kml_theme ";
    strSQL += " WHERE theme_id = "+ Te2String(this->id());
	
	if (!database->execute(strSQL))
		return false;

	if (proid > 0)
	{
		strSQL = "DELETE FROM te_projection WHERE projection_id = " + Te2String(proid);
		database->execute(strSQL);
	}
	return true;
}


bool TeKmlTheme::beforeCopyThemeTo(TeAbstractTheme* absThemeCopy, TeDatabase* outputDatabase)
{
	if(TeAbstractTheme::beforeCopyThemeTo(absThemeCopy, outputDatabase) == false)
	{
		return false;
	}

	TeKmlTheme* themeCopy = dynamic_cast<TeKmlTheme*>(absThemeCopy);
	if(themeCopy == 0)
	{
		return false;
	}

	themeCopy->setLocalDatabase(outputDatabase);

	if(themeCopy->createKmlThemeTable(outputDatabase) == false)
	{
		return false;
	}
	
	return true;
}
