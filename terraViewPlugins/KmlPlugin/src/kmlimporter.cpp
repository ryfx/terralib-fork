
#include "kmlimporter.h"
#include "kmldocument.h"
#include "kmlfolder.h"
#include "kmlplacemark.h"
#include "kmlstyle.h"
#include "kmlstylemap.h"
#include "TeLayer.h"
#include "TeDatabase.h"

//TerraLib include files
#include <TeException.h>

//Using gettext i18n
//#include <libintl.h>
//#define _(X) gettext(X)

TeTable getAttrTable(const string& tabName)
{
	// 0) Criando a AttributeList
	// feature_id (int) -> feature_name (vc100) -> feature_description (vc1000) -> parent_id(int)
	TeAttributeList attrList;
	TeAttribute attribute;

	// 1) object_id
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_id";
	attribute.rep_.numChar_ = 16;
	attribute.rep_.isPrimaryKey_ = true;
	attrList.push_back( attribute );

	// 2) feature_name
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_name";
	attribute.rep_.numChar_ = 100; // esse deve estar bom
	attribute.rep_.isPrimaryKey_ = false;
	attrList.push_back( attribute );

	// 3) feature_description
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_description";
	attribute.rep_.numChar_ = 1000; // esse deve estar bom
	attribute.rep_.isPrimaryKey_ = false;
	attrList.push_back( attribute );

	// 4) Retornando
	TeTable feature(tabName, attrList, "feature_id", "feature_id", TeAttrStatic );

	return feature;
}

namespace tdk {

KMLImporter::KMLImporter() :
KMLAbstractVisitor()
{
	_count = 0;
}

KMLImporter::~KMLImporter() 
{
}

void KMLImporter::visit(KMLDocument & doc) 
{
	if(!doc.getVisibility())
		return;

	int childCount = doc.childrenCount();

	for(int i=0; i<childCount; i++)
		((KMLObject*)doc.getChild(i))->accept(*this);
}

void KMLImporter::visit(KMLFolder & folder) 
{
	if(!folder.getVisibility())
		return;

	int childCount = folder.childrenCount();

	for(int i=0; i<childCount; i++)
		((KMLObject*)folder.getChild(i))->accept(*this);
}

void KMLImporter::visit(KMLPlacemark & mark) 
{
	if(!mark.getVisibility())
		return;

	TeTableRow row;
	string id = Te2String(_count++);
	//(!mark.getId().empty()) ? mark.getId() : 
	row.push_back(id);
	row.push_back(mark.getName());
	row.push_back(mark.getDescription());

	switch(mark.getGeometryType())
	{
		case KMLPlacemark::POINT:
			_points.add((const TePoint&)mark.getGeometry());
			_points[_points.size()-1].objectId(id);
		break;

		case KMLPlacemark::LINE:
			_lines.add((const TeLine2D&)mark.getGeometry());
			_lines[_lines.size()-1].objectId(id);
		break;

		case KMLPlacemark::POLYGON:
			_polygons.add((const TePolygon&)mark.getGeometry());
			_polygons[_polygons.size()-1].objectId(id);
		break;
	}

	_attTable.add(row);
}

void KMLImporter::visit(KMLStyle & style) 
{
}

void KMLImporter::visit(KMLStyleMap & style) 
{
}

TeLayer* KMLImporter::import(const map<string, KMLFeature*> & feats, const string & layerName, ::TeDatabase * db) 
{
	if(db == NULL || feats.empty() || layerName.empty())
		return NULL;

	string attrTabName = layerName;
	int n=0;
	
	while(db->tableExist(attrTabName) || db->layerExist(attrTabName))
	{
		attrTabName += Te2String(n);
		n++;
	}

	if(n > 0)
		_attTable.name(attrTabName);

	_attTable = getAttrTable(attrTabName);

	TeDatum pDatum = TeDatumFactory::make("WGS84");
	TeProjection* proj = new TeLatLong(pDatum);
	
	if(!db->beginTransaction())
		throw TeException(UNKNOWN_ERROR_TYPE, "Error start transaction.");
	
	TeLayer* layer = new TeLayer(attrTabName, db, proj);

	map<string, KMLFeature*>::const_iterator it;

	for(it = feats.begin(); it != feats.end(); ++it)
		it->second->accept(*this);

	try
	{
		if(_points.empty() && _lines.empty() && _polygons.empty())
			throw TeException(UNKNOWN_ERROR_TYPE, "Error: No geometries found.");		
		
		if(!layer->createAttributeTable(_attTable))
			throw TeException(UNKNOWN_ERROR_TYPE, "Error inserting attribute table on layer.");

		if(!layer->saveAttributeTable(_attTable))
			throw TeException(UNKNOWN_ERROR_TYPE, "Error inserting attribute table on layer.");

		if(!_points.empty())
			if(!layer->addGeometrySet(_points))
				throw TeException(UNKNOWN_ERROR_TYPE, "Error adding points to layer.");

		if(!_lines.empty())
			if(!layer->addGeometrySet(_lines))
				throw TeException(UNKNOWN_ERROR_TYPE, "Error adding lines to layer.");

		if(!_polygons.empty())
			if(!layer->addGeometrySet(_polygons))
				throw TeException(UNKNOWN_ERROR_TYPE, "Error adding polygons to layer.");

		if(!db->commitTransaction())
			db->rollbackTransaction();
	}
	catch(TeException& e)
	{
		/*
		if(layer != NULL)
		{
			if(layer->id() >= 0)
				db->deleteLayer(layer->id());
			else
				delete layer;
		}
		if(db->tableExist(_attTable.name()))
			db->deleteTable(_attTable.name());
			
		*/

		db->rollbackTransaction();

		throw e;
	}

	return layer;
}


} // namespace tdk
