#include "kmlinterpreter.h"

#include "kmldocument.h"
#include "kmlfolder.h"
#include "kmlplacemark.h"
#include "kmlstyle.h"
#include "kmlstylemap.h"
#include "kmlpointstyle.h"
#include "kmllinestyle.h"
#include "kmlpolygonstyle.h"

//TerraLib include files
#include <TeException.h>
#include "TeDatabase.h"

namespace tdk {

KMLInterpreter::KMLInterpreter() :
KMLAbstractVisitor()
{
	_count = 0;
}

KMLInterpreter::~KMLInterpreter()
{
	
}

TeTable KMLInterpreter::createAttrTable()
{
	// 0) Criando a AttributeList
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
	TeTable feature("attrTable", attrList, "feature_id", "feature_id", TeAttrStatic );

	return feature;
}

TeTable KMLInterpreter::createStyleTable()
{
	// 0) Criando a AttributeList	
	TeAttributeList attrList;
	TeAttribute attribute;

	// 0
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_id";
	attribute.rep_.numChar_ = 16;
	attribute.rep_.isPrimaryKey_ = true;
	attrList.push_back( attribute );

	// 1
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_alpha";
	attribute.rep_.numChar_ = 100;
	attribute.rep_.isPrimaryKey_ = false;
	attrList.push_back( attribute );

	//Color
	// 2
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_color_r";
	attribute.rep_.numChar_ = 3;
	attribute.rep_.isPrimaryKey_ = false;
	attrList.push_back( attribute );

	// 3
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_color_g";
	attribute.rep_.numChar_ = 3;
	attribute.rep_.isPrimaryKey_ = false;
	attrList.push_back( attribute );

	// 4
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_color_b";
	attribute.rep_.numChar_ = 3;
	attribute.rep_.isPrimaryKey_ = false;
	attrList.push_back( attribute );

	//Width
	// 5
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_width";
	attribute.rep_.numChar_ = 10;
	attribute.rep_.isPrimaryKey_ = false;
	attrList.push_back( attribute );

	//LineColor
	// 6
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_lineColor_r";
	attribute.rep_.numChar_ = 3;
	attribute.rep_.isPrimaryKey_ = false;
	attrList.push_back( attribute );

	// 7
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_lineColor_g";
	attribute.rep_.numChar_ = 3;
	attribute.rep_.isPrimaryKey_ = false;
	attrList.push_back( attribute );

	// 8
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_lineColor_b";
	attribute.rep_.numChar_ = 3;
	attribute.rep_.isPrimaryKey_ = false;
	attrList.push_back( attribute );

	//LineWidth
	// 9
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_lineWidth";
	attribute.rep_.numChar_ = 10;
	attribute.rep_.isPrimaryKey_ = false;
	attrList.push_back( attribute );

	//IconUrl
	// 10
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_iconUrl";
	attribute.rep_.numChar_ = 100;
	attribute.rep_.isPrimaryKey_ = false;
	attrList.push_back( attribute );

	//HotSpot X e Y
	// 11
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_iconHotSpotX";
	attribute.rep_.numChar_ = 5;
	attribute.rep_.isPrimaryKey_ = false;
	attrList.push_back( attribute );

	// 12
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_iconHotSpotY";
	attribute.rep_.numChar_ = 5;
	attribute.rep_.isPrimaryKey_ = false;
	attrList.push_back( attribute );

	//13
	attribute.rep_.type_ = TeSTRING;
	attribute.rep_.name_ = "feature_iconScale";
	attribute.rep_.numChar_ = 5;
	attribute.rep_.isPrimaryKey_ = false;
	attrList.push_back( attribute );
	
	// 4) Retornando
	TeTable styleTable("StyleTable", attrList, "feature_id", "feature_id", TeAttrStatic );

	return styleTable;
}

void KMLInterpreter::visit(KMLDocument & doc) 
{
	if(!doc.getVisibility())
		return;

	int childCount = doc.childrenCount();

	for(int i=0; i<childCount; i++)
		((KMLObject*)doc.getChild(i))->accept(*this);
}

void KMLInterpreter::visit(KMLFolder & folder) 
{
	if(!folder.getVisibility())
		return;

	int childCount = folder.childrenCount();

	for(int i=0; i<childCount; i++)
		((KMLObject*)folder.getChild(i))->accept(*this);
}

void KMLInterpreter::visit(KMLPlacemark & mark) 
{
	if(!mark.getVisibility())
		return;

	TeTableRow row;	
	string id = Te2String(_count++);	
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
	_placeMarks[id] = &mark;	
}

void KMLInterpreter::visit(KMLStyle & style)
{
	_styles.push_back(&style);
}

void KMLInterpreter::visit(KMLStyleMap & style)
{
	_styleMaps.push_back(&style);
}

void KMLInterpreter::interpret(const map<string, KMLFeature*> & feats, ::TeDatabase * db, TePointSet & pointSet, TeLineSet & lineSet, TePolygonSet & polygonSet, TeTable & attrTable, TeTable & styleTable)
{
	if(db == NULL || feats.empty())
		return;
	
	map<string, KMLFeature*>::const_iterator it;
	
	_attTable = createAttrTable();
	_styleTable = createStyleTable();

	TeDatum pDatum = TeDatumFactory::make("WGS84");
	TeProjection* proj = new TeLatLong(pDatum);
	
	for(it = feats.begin(); it != feats.end(); ++it)
		it->second->accept(*this);
	

	if(_points.empty() && _lines.empty() && _polygons.empty())
		throw TeException(UNKNOWN_ERROR_TYPE, "Error: No geometries found.");

	organizeInformation();

	pointSet = _points;
	lineSet = _lines;
	polygonSet = _polygons;
	attrTable = _attTable;
	styleTable = _styleTable;

}

void KMLInterpreter::organizeInformation()
{

	for(unsigned int i = 0; i < _placeMarks.size(); i++)
	{

		string markId = Te2String(i);
		string markStyleMapUrl = _placeMarks[Te2String(i)]->getStyleURL();
		string markStyleUrl = "";
		KMLStyle* markStyle = 0;
		TeTableRow styRow;


		if(_styleMaps.size() != 0)
		{
			for(unsigned int j = 0; j < _styleMaps.size(); j++)
			{
				string styleMapUrl = _styleMaps[j]->getStyles()[markStyleMapUrl];
				string styleMapKey = styleMapUrl.substr(0, 6);

				if(!styleMapKey.empty())
				{
					if(styleMapKey == "normal")
					{
						markStyleUrl = _styleMaps[j]->getStyles()[_placeMarks[Te2String(i)]->getStyleURL()].substr(6);
					}
					else
					{
						markStyleUrl = _styleMaps[j]->getStyles()[_placeMarks[Te2String(i)]->getStyleURL()].substr(9);
					}
					break;
				}			
				
			}
		}
		else
		{
			markStyleUrl = markStyleMapUrl;
		}

		for(unsigned int j = 0; j < _styles.size(); j++)
		{
			if(_styles[j]->getId() == markStyleUrl)
			{
				markStyle = _styles[j];
				break;
			}
		}

		if(_placeMarks[markId]->getGeometryType() == KMLPlacemark::POINT)
		{	
			KMLSubStyle* sty = 0;
			if(markStyle != 0)
				sty = markStyle->getVisual(KMLSubStyle/*::KMLStyleType*/::ICON);
			
			if(sty == 0)
			{
				styRow.push_back(Te2String(i));//0							
				styRow.push_back(""); //1
				styRow.push_back("255");//2
				styRow.push_back("255");//3
				styRow.push_back("255");//4
				styRow.push_back("1");//5
				styRow.push_back("255");//6
				styRow.push_back("255");//7
				styRow.push_back("255");//8
				styRow.push_back("1");//9
				styRow.push_back("");//10
				styRow.push_back("");//11
				styRow.push_back("");//12
				styRow.push_back("1");//13
			}
			else
			{
				styRow.push_back(Te2String(i));//0							
				int alpha = 100-(markStyle->getVisual(KMLSubStyle::ICON)->getAlpha()*100)/255;
				styRow.push_back(Te2String(alpha)); //1
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::ICON)->getColor().blue_));//2
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::ICON)->getColor().green_));//3
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::ICON)->getColor().red_));//4
				styRow.push_back(Te2String(((KMLLineStyle*)markStyle->getVisual(KMLSubStyle::ICON))->getWidth()));//5
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::ICON)->getColor().blue_));//6
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::ICON)->getColor().green_));//7
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::ICON)->getColor().red_));//8
				styRow.push_back("");//9
				styRow.push_back(((KMLPointStyle*)markStyle->getVisual(KMLSubStyle::ICON))->getIcon());//10
				styRow.push_back(Te2String((int)((KMLPointStyle*)markStyle->getVisual(KMLSubStyle::ICON))->getHotspot().x()));//11
				styRow.push_back(Te2String((int)((KMLPointStyle*)markStyle->getVisual(KMLSubStyle::ICON))->getHotspot().y()));//12
				styRow.push_back(Te2String((double)((KMLPointStyle*)markStyle->getVisual(KMLSubStyle::ICON))->getScale(), 2));//13
				
			}
			
		}	
		if(_placeMarks[markId]->getGeometryType() == KMLPlacemark::LINE)
		{
			
			KMLSubStyle* sty = 0;
			if(markStyle != 0)
				sty = markStyle->getVisual(KMLSubStyle::LINE);

			if(sty == 0)
			{
				styRow.push_back(Te2String(i));//0							
				styRow.push_back(""); //1
				styRow.push_back("255");//2
				styRow.push_back("255");//3
				styRow.push_back("255");//4
				styRow.push_back("1");//5
				styRow.push_back("0");//6
				styRow.push_back("0");//7
				styRow.push_back("0");//8
				styRow.push_back("5");//9
				styRow.push_back("");//10
				styRow.push_back("");//11
				styRow.push_back("");//12	
				styRow.push_back("");//13
			}
			else
			{			
				styRow.push_back(Te2String(i));//0	
				int alpha = 100-(markStyle->getVisual(KMLSubStyle::LINE)->getAlpha()*100)/255;
				styRow.push_back(Te2String(alpha)); //1
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::LINE)->getColor().blue_));//2
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::LINE)->getColor().green_));//3
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::LINE)->getColor().red_));//4
				styRow.push_back("");//5
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::LINE)->getColor().blue_));//6
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::LINE)->getColor().green_));//7
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::LINE)->getColor().red_));//8
				styRow.push_back(Te2String(((KMLLineStyle*)markStyle->getVisual(KMLSubStyle::LINE))->getWidth()));//9
				styRow.push_back("");//10
				styRow.push_back("");//11
				styRow.push_back("");//12
				styRow.push_back("");//13
			}
		}
		if(_placeMarks[markId]->getGeometryType() == KMLPlacemark::POLYGON)
		{

			KMLSubStyle* sty = 0;
			if(markStyle != 0)
				sty = markStyle->getVisual(KMLSubStyle::POLYGON);

			if(sty == 0)
			{
				styRow.push_back(Te2String(i));//0							
				styRow.push_back("100"); //1
				styRow.push_back("255");//2
				styRow.push_back("0");//3
				styRow.push_back("0");//4
				styRow.push_back("1");//5
				styRow.push_back("0");//6
				styRow.push_back("0");//7
				styRow.push_back("255");//8
				styRow.push_back("5");//9
				styRow.push_back("");//10
				styRow.push_back("");//11
				styRow.push_back("");//12	
				styRow.push_back("");//13
			}
			else
			{
				styRow.push_back(Te2String(i));//0		
				int alpha = 100-((markStyle->getVisual(KMLSubStyle::POLYGON)->getAlpha()*100)/255);
				styRow.push_back(Te2String(alpha)); //1 ALPHA
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::POLYGON)->getColor().blue_));//2
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::POLYGON)->getColor().green_));//3
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::POLYGON)->getColor().red_));//4
				styRow.push_back(Te2String(((KMLLineStyle*)markStyle->getVisual(KMLSubStyle::POLYGON))->getWidth()));//5
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::LINE)->getColor().blue_));//6
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::LINE)->getColor().green_));//7
				styRow.push_back(Te2String(markStyle->getVisual(KMLSubStyle::LINE)->getColor().red_));//8
				styRow.push_back(Te2String(((KMLLineStyle*)markStyle->getVisual(KMLSubStyle::LINE))->getWidth()));//9
				styRow.push_back("");//10
				styRow.push_back("");//11
				styRow.push_back("");//12
				styRow.push_back("");//13
			}
		}

		_styleTable.add(styRow);
		
	}

}

}
