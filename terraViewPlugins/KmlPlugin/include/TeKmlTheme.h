#ifndef  __TEKMLTHEME_H
#define  __TEKMLTHEME_H


// TerraLib include files
#include <TeAbstractTheme.h>
#include <TeTable.h>

#include <TeProjection.h>

// STL include files
#include <string>
#include <map>

#define TeKMLTHEME 10

// Forward declarations
//class TeDatabase;
//class TePolygonSet;
//class TePointSet;
//class TeLineSet;

class TeKmlTheme : public TeAbstractTheme
{

protected:

	std::string			_fileName;		// location of the file	
	TeDatabase*			_database;		// a pointer to the database that stores the theme
	TeProjection*		_projection;
	TePolygonSet _polygonSet;
	TePointSet _pointSet;
	TeLineSet _lineSet;
	TeTable _attrTable;

public:

	TeKmlTheme(const std::string& fileName = "", TeViewNode* parent = 0, int view = 0, int id = 0);

	//! Copy constructor
	TeKmlTheme (const TeKmlTheme& other);

	//! Destructor
	virtual ~TeKmlTheme ();

	//! Assignment operator
	TeKmlTheme& operator= (const TeKmlTheme& other); 

	//! Clones the object
	virtual TeViewNode* clone();



	void setPolygonSet(const TePolygonSet & polygonSet);
	TePolygonSet getPolygonSet();

	void setPointSet(const TePointSet & pointSet);
	TePointSet getPointSet();

	void setLineSet(const TeLineSet & lineSet);
	TeLineSet getLineSet();

	void setAttrTable(const TeTable & attrTable);
	TeTable getAttrTable() const;

	void setFileName(const std::string & _themeName);

	std::string getFileName();

	void setLocalDatabase(TeDatabase* database);

	TeDatabase* getLocalDatabase();

	bool getData(TePointSet & pointSet, TeLineSet & lineSet, TePolygonSet & polygonSet, TeTable & attrTable, TeTable & styleTable);

	void setThemeProjection(TeProjection* proj);

	//! Returns a pointer to a projection that is the spatial reference for the objects of this theme
	/*! Concrete classes should reimplement this method.*/
	virtual TeProjection* getThemeProjection();

	//! Save the theme parameters
	virtual bool save();
	
	//! Build the grouping and associate each object to its group  
	virtual bool saveGrouping(TeSelectedObjects selectedObjects = TeAll);

	//! Save the grouping parameters in memory when there is no chronon
	virtual bool buildGrouping(const TeGrouping& g, TeSelectedObjects selectedObjects = TeAll, vector<double>* dValuesVec = 0);

	//! Save the grouping parameters in memory when there is chronon
	virtual bool buildGrouping(const TeGrouping& g, TeChronon chr, vector<map<string, string> >& mapObjValVec);

	//! Delete grouping
	virtual bool deleteGrouping(); 

	//! Set the legend id for each object of the theme 
	virtual void setLegendsForObjects();

	//! Set the own legend id for each object of the theme 
	virtual void setOwnLegendsForObjects();

	/** @name Locate geometries
	    Returns the geometry(ies) of the theme given coordinate
	*/
	//@{ 	
	virtual bool locatePolygon		(TeCoord2D &pt, TePolygon &polygon, const double& tol = 0.0);
	virtual bool locatePolygonSet   (TeCoord2D &pt, double tol, TePolygonSet &polygons);
	virtual bool locateLine		(TeCoord2D &pt, TeLine2D &line, const double& tol = 0.0);
	virtual bool locatePoint	(TeCoord2D &pt, TePoint &point, const double& tol = 0.0);
	virtual bool locateCell		(TeCoord2D &pt, TeCell &c, const double& tol = 0.0);
	//@}

	//! Get the set of objects corresponding to the object selection criteria
	virtual set<string> getObjects(TeSelectedObjects selectedObjects = TeAll);

	//! Get the set of objects corresponding to the list of items
	virtual set<string> getObjects(const vector<string>& itemVec);

	//! Get the set of items corresponding to the object selection criteria
	virtual vector<string> getItemVector(TeSelectedObjects selectedObjects);

	//! Get the set of items corresponding to the set of objects
	virtual vector<string> getItemVector(const set<string>& oidSet);

	//! Get the number of objects acessible by this theme
	virtual unsigned int getNumberOfObjects(); 

	//! Save (insert or update) the theme metadata in the database
	virtual bool saveMetadata(TeDatabase* );	
	
	//! Load the theme metadata from database
	virtual bool loadMetadata(TeDatabase* );

// --- Methods responsible to the persistence of this kind of theme

	//! An static method to create a metadata table to store information about this type of theme
	static bool createKmlThemeTable(TeDatabase* db);

	//! Load the basic information about the data set from the file
	bool retrieveMetadataFromFile();

	//! Load the objects from the file and fills the set of elements
	bool retrieveDataFromFile();

	//! Releases the internal data set
	void clearData(); 	

//--- Methods that reimplement the interface defined by the abstract theme

protected:

	//! Erase the theme metadata in the database
	virtual bool eraseMetadata(TeDatabase* );

	//! Should be reimplemented to set all the parameters and pre-requisites necessaries to copy a theme to the given database. This method will be called before the theme is saved in the database.
	virtual bool beforeCopyThemeTo(TeAbstractTheme* absThemeCopy, TeDatabase* outputDatabase);	
};

//!  This class implements a factory to create remote theme objects. 
/*!  
	
*/
class TeKmlThemeFactory : public TeViewNodeFactory
{
public:
	//! Constructor 
	TeKmlThemeFactory() : TeViewNodeFactory((int)TeKMLTHEME)
	{}

	//! Created theme objects 
	TeViewNode* build(TeViewNodeParams* params)
	{	
		TeViewNodeParams auxParams = *params;
		return new TeKmlTheme(auxParams.name_, auxParams.myParent_, auxParams.viewId_, auxParams.id_);	
	}
	
	TeViewNode* build()
	{
		return new TeKmlTheme();
	}
};

namespace 
{
  static TeKmlThemeFactory kmlThemeFactory;
}; 



#endif //__TEKMLTHEME_H