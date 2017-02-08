#include <kmlexporter.h>

//KML include files
#include <kmldocument.h>
#include <kmlfolder.h>
#include <kmlplacemark.h>
#include <kmlstyle.h>
#include <kmlstylemap.h>
#include <kmlpointstyle.h>
#include <kmllinestyle.h>
#include <kmlpolygonstyle.h>
#include <kmlmanager.h>
#include <kmlchecklistitem.h>

//TerraLib include files
#include <TeTheme.h>
#include <TeDatabase.h>
#include <TeVectorRemap.h>

//Qt include files
#include <qlistview.h>
#include <qpixmap.h>

vector<KMLFeature*> theme2KML(TeTheme* theme, const string& descTable, const string& nameColumn,
							   const string& descColumn)
{
	vector<KMLFeature*> docs;

	TeDatabase* db = theme->layer()->database();

	if(db != NULL && theme != NULL)
	{
		//Get theme attributtes.  sel = descTable + "." + nameColumn + ", " + descTable + "." + descColumn,
		string sel;
		if(nameColumn == "" && descColumn == "")
			sel = "*";
		else if(nameColumn == "")
			sel = descTable + "." + descColumn;
		else if(descColumn == "")
			sel = descTable + "." + nameColumn;
		else
			sel = descTable + "." + nameColumn + ", " + descTable + "." + descColumn;
			
		string from = descTable;
		string whe = theme->attributeRest();
		string ord;

		TeAttrTableVector attrs;
		theme->getAttTables(attrs);

		if(!whe.empty())
		{
			from += "," + attrs[0].name();

			for(unsigned int i = 1; i < attrs.size(); i++)
			{
				from += "," + attrs[i].name();
				whe += " AND " + attrs[0].name() + "." + attrs[0].uniqueName() + "=" + 
					attrs[i].name() + "." + attrs[i].linkName();
			}
		}

		ord = attrs[0].name() + "." + attrs[0].uniqueName();

		std::string query;
		if(whe.empty())
			query = "select " + sel + " from " + from + " order by " + ord;
		else
			query = "select " + sel + " from " + from + " where " + whe + " order by " + ord;

		
		TeDatabasePortal* attPortal = db->getPortal();

		if(!attPortal->query(query))
		{
			docs.clear();
			return docs;
		}

		if(attPortal == NULL || !attPortal->fetchRow())
		{
			if(attPortal != NULL)
				delete attPortal;

			docs.clear();
			return docs;			
		}

		vector<string> name;
		vector<string> descript;

		do 
		{
			if(nameColumn == "" && descColumn == "")
			{
				name.push_back("");
				descript.push_back("");
			}
			else if(nameColumn == "")
			{
				name.push_back("");
				descript.push_back(attPortal->getData(0));
			}				
			else if(descColumn == "")
			{
				name.push_back(attPortal->getData(0));
				descript.push_back("");
			}				
			else
			{
				name.push_back(attPortal->getData(0));
				descript.push_back(attPortal->getData(1));
			}				
		}while(attPortal->fetchRow());

		attPortal->freeResult();
		delete attPortal;

		//Get theme geometries
		TeRepresPointerVector vec = theme->layer()->vectRepres();

		//main document
		tdk::KMLDocument* doc = new tdk::KMLDocument;
		doc->setName(theme->name());

		//main folder
		KMLFolder* folder = new KMLFolder;
		folder->setName(theme->layer()->name());

		TeRepresPointerVector::iterator it;
		vector<KMLPlacemark*> marks;

		TeDatum pDatum = TeDatumFactory::make("WGS84");
		TeProjection* kmlProj = new TeLatLong(pDatum);

		bool need2Reproj = !(*kmlProj == *theme->getThemeProjection());
		
		//create styles
		for(it = vec.begin(); it != vec.end(); ++it)
		{
			TeGeomRep rep = (*it)->geomRep_;
			string styleURL;

			TeVisual* defV = theme->defaultLegend().getVisualMap()[rep];
			TeVisual* selV = theme->pointingLegend().getVisualMap()[rep];

			if(rep == TePOLYGONS)
			{
				KMLPolygonStyle* s1 = new KMLPolygonStyle;
				int transp = convertTransparency2Alpha(defV->transparency());

				s1->setColor(defV->color());
				s1->setFill(transp != 0);
				s1->setColorMode(KMLSubStyle::NORMAL);
				s1->setAlpha(transp);

				KMLLineStyle* s2 = new KMLLineStyle;
				s2->setColor(defV->contourColor());
				s2->setColorMode(KMLSubStyle::NORMAL);
				s2->setWidth(defV->contourWidth());
				transp = convertTransparency2Alpha(defV->contourTransparency());
				s2->setAlpha(transp);

				KMLStyle* dStyle = new KMLStyle;
				dStyle->setId(doc->getName() + "_poly_def");
				dStyle->setVisual(KMLSubStyle::POLYGON, s1);
				dStyle->setVisual(KMLSubStyle::LINE, s2);

				KMLPolygonStyle* s3 = new KMLPolygonStyle;
				s3->setColor(selV->color());
				s3->setFill(selV->transparency() != 255);
				s3->setColorMode(KMLSubStyle::NORMAL);
				transp = convertTransparency2Alpha(selV->transparency());
				s3->setAlpha(transp);

				KMLLineStyle* s4 = new KMLLineStyle;
				s4->setColor(selV->contourColor());
				s4->setColorMode(KMLSubStyle::NORMAL);
				s4->setWidth(selV->contourWidth());
				transp = convertTransparency2Alpha(selV->contourTransparency());
				s4->setAlpha(transp);

				KMLStyle* sStyle = new KMLStyle;
				sStyle->setId(doc->getName() + "_poly_sel");
				sStyle->setVisual(KMLSubStyle::POLYGON, s3);
				sStyle->setVisual(KMLSubStyle::LINE, s4);

				KMLStyleMap* sMap = new KMLStyleMap;
				sMap->mapStyle("normal", "#" + dStyle->getId());
				sMap->mapStyle("highlight", "#" + sStyle->getId());
				sMap->setId(doc->getName() + "_poly");

				doc->addChild(dStyle);
				doc->addChild(sStyle);
				doc->addChild(sMap);

				styleURL = sMap->getId();
			}
			else if(rep == TeLINES)
			{
				KMLLineStyle* s1 = new KMLLineStyle;
				s1->setColor(defV->color());
				s1->setColorMode(KMLSubStyle::NORMAL);
				s1->setWidth(defV->width());
				int transp = convertTransparency2Alpha(defV->transparency());
				s1->setAlpha(transp);

				KMLStyle* dStyle = new KMLStyle;
				dStyle->setId(doc->getName() + "_line_def");
				dStyle->setVisual(KMLSubStyle::LINE, s1);

				KMLLineStyle* s2 = new KMLLineStyle;
				s2->setColor(selV->color());
				s2->setColorMode(KMLSubStyle::NORMAL);
				s2->setWidth(selV->width());
				transp = convertTransparency2Alpha(selV->transparency());
				s2->setAlpha(transp);

				KMLStyle* sStyle = new KMLStyle;
				sStyle->setId(doc->getName() + "_line_sel");
				sStyle->setVisual(KMLSubStyle::LINE, s2);

				KMLStyleMap* sMap = new KMLStyleMap;
				sMap->mapStyle("normal", "#" + dStyle->getId());
				sMap->mapStyle("highlight", "#" + sStyle->getId());
				sMap->setId(doc->getName() + "_line");

				doc->addChild(dStyle);
				doc->addChild(sStyle);
				doc->addChild(sMap);

				styleURL = sMap->getId();
			}
			else if(rep == TePOINTS)
			{
				KMLPointStyle* s1 = new KMLPointStyle;
				s1->setColor(defV->color());
				s1->setColorMode(KMLSubStyle::NORMAL);			
				s1->setAlpha(255);

				KMLStyle* dStyle = new KMLStyle;
				dStyle->setId(doc->getName() + "_point_def");
				dStyle->setVisual(KMLSubStyle::ICON, s1);

				KMLPointStyle* s2 = new KMLPointStyle;
				s2->setColor(selV->color());
				s2->setColorMode(KMLSubStyle::NORMAL);			
				s2->setAlpha(255);

				KMLStyle* sStyle = new KMLStyle;
				sStyle->setId(doc->getName() + "_point_sel");
				sStyle->setVisual(KMLSubStyle::ICON, s2);

				KMLStyleMap* sMap = new KMLStyleMap;
				sMap->mapStyle("normal", "#" + dStyle->getId());
				sMap->mapStyle("highlight", "#" + sStyle->getId());
				sMap->setId(doc->getName() + "_line");

				doc->addChild(dStyle);
				doc->addChild(sStyle);
				doc->addChild(sMap);

				styleURL = sMap->getId();
			}

			std::string query2 = "select * from " + theme->layer()->tableName(rep) + " p ";
			query2 += "inner join " + theme->collectionTable() + " c ON p.object_id = c.c_object_id ";
			query2 += "order by " + db->getSQLOrderBy(rep);

		//create placemarks
			TeDatabasePortal* gPortal = db->getPortal();

			if(!gPortal->query(query2))
			{
				docs.clear();
				return docs;
			}

			if(!gPortal->fetchRow())
			{
				docs.clear();
				return docs;
			}

			bool hasGeom = false;
			unsigned int descriptIdx = 0;

			do
			{
				tdk::KMLPlacemark* mark = new tdk::KMLPlacemark;

				if(rep == TePOLYGONS)
				{
					TePolygon poly;
					hasGeom = gPortal->fetchGeometry(poly); 
					
					if(need2Reproj)
						changeProjection(theme->getThemeProjection(), kmlProj, poly);
	
					mark->setGeometry(poly);
					mark->setId(poly.objectId());
				}
				else if(rep == TeLINES)
				{
					TeLine2D line;
					hasGeom = gPortal->fetchGeometry(line); 
	
					if(need2Reproj)
						changeProjection(theme->getThemeProjection(), kmlProj, line);

					mark->setGeometry(line);
					mark->setId(line.objectId());
				}
				if(rep == TePOINTS)
				{
					TePoint pt;
					hasGeom = gPortal->fetchGeometry(pt); 
						
					if(need2Reproj)
						changeProjection(theme->getThemeProjection(), kmlProj, pt);

					mark->setGeometry(pt);
					mark->setId(pt.objectId());
				}

				marks.push_back(mark);
				
				if(!styleURL.empty())
					mark->setStyleURL("#" + styleURL);

				if(!descript.empty() && descriptIdx < descript.size())
					mark->setDescription(descript[descriptIdx]);

				if(!name.empty() && descriptIdx < name.size())
					mark->setName(name[descriptIdx]);

				descriptIdx++;
			} while(hasGeom);

			gPortal->freeResult();
			delete gPortal;
		}

		vector<KMLPlacemark*>::iterator markIt;
		doc->addChild(folder);

		for(markIt = marks.begin(); markIt != marks.end(); ++markIt)
			folder->addChild(*markIt);

		docs.push_back(doc);
		
		delete kmlProj;
	}

	return docs;
}

vector<tdk::KMLFeature*> layer2KML(TeLayer* layer, const string& descTable, const string& nameColumn, const string& descColumn)
{
	
	vector<KMLFeature*> docs;

	TeDatabase* db = layer->database();

	if(db != NULL && layer != NULL)
	{
		//Get layer attributtes.  sel = descTable + "." + nameColumn + ", " + descTable + "." + descColumn,
		string sel;
		if(nameColumn == "" && descColumn == "")
			sel = "*";
		else if(nameColumn == "")
			sel = descTable + "." + descColumn;
		else if(descColumn == "")
			sel = descTable + "." + nameColumn;
		else
			sel = descTable + "." + nameColumn + ", " + descTable + "." + descColumn;
			
		string from = descTable;
		string ord;

		TeAttrTableVector attrs;
		layer->getAttrTables(attrs);		

		ord = attrs[0].name() + "." + attrs[0].uniqueName();

		std::string query;
		
		query = "select " + sel + " from " + from + " order by " + ord;
				
		TeDatabasePortal* attPortal = db->getPortal();

		if(!attPortal->query(query))
		{
			docs.clear();
			return docs;
		}

		if(attPortal == NULL || !attPortal->fetchRow())
		{
			if(attPortal != NULL)
				delete attPortal;

			docs.clear();
			return docs;			
		}

		vector<string> name;
		vector<string> descript;

		do 
		{
			if(nameColumn == "" && descColumn == "")
			{
				name.push_back("");
				descript.push_back("");
			}
			else if(nameColumn == "")
			{
				name.push_back("");
				descript.push_back(attPortal->getData(0));
			}				
			else if(descColumn == "")
			{
				name.push_back(attPortal->getData(0));
				descript.push_back("");
			}				
			else
			{
				name.push_back(attPortal->getData(0));
				descript.push_back(attPortal->getData(1));
			}				
		}while(attPortal->fetchRow());

		attPortal->freeResult();
		delete attPortal;

		//Get theme geometries
		TeRepresPointerVector vec = layer->vectRepres();

		//main document
		tdk::KMLDocument* doc = new tdk::KMLDocument;
		doc->setName(layer->name());

		//main folder
		KMLFolder* folder = new KMLFolder;
		folder->setName(layer->name());

		TeRepresPointerVector::iterator it;
		vector<KMLPlacemark*> marks;

		TeDatum pDatum = TeDatumFactory::make("WGS84");
		TeProjection* kmlProj = new TeLatLong(pDatum);

		bool need2Reproj = !(*kmlProj == *layer->projection());
		
		//create styles
		for(it = vec.begin(); it != vec.end(); ++it)
		{
			TeGeomRep rep = (*it)->geomRep_;
			string styleURL;

			if(rep == TePOLYGONS)
			{
				KMLPolygonStyle* s1 = new KMLPolygonStyle;
				int transp = 255;

				TeColor auxColor;
				auxColor.red_ = 255;
				auxColor.green_ = 0;
				auxColor.blue_ = 0;

				s1->setColor(auxColor);
				s1->setFill(true);
				s1->setColorMode(KMLSubStyle::NORMAL);
				s1->setAlpha(transp);

				auxColor.red_ = 0;
				auxColor.green_ = 0;
				auxColor.blue_ = 0;
				
				KMLLineStyle* s2 = new KMLLineStyle;
				s2->setColor(auxColor);
				s2->setColorMode(KMLSubStyle::NORMAL);
				s2->setWidth(1);
				transp = 255;
				s2->setAlpha(transp);

				KMLStyle* dStyle = new KMLStyle;
				dStyle->setId(doc->getName() + "_poly_def");
				dStyle->setVisual(KMLSubStyle::POLYGON, s1);
				dStyle->setVisual(KMLSubStyle::LINE, s2);

				auxColor.red_ = 0;
				auxColor.green_ = 0;
				auxColor.blue_ = 255;
				
				KMLPolygonStyle* s3 = new KMLPolygonStyle;
				s3->setColor(auxColor);
				s3->setFill(true);
				s3->setColorMode(KMLSubStyle::NORMAL);
				transp = 255;
				s3->setAlpha(transp);

				auxColor.red_ = 0;
				auxColor.green_ = 0;
				auxColor.blue_ = 0;

				KMLLineStyle* s4 = new KMLLineStyle;
				s4->setColor(auxColor);
				s4->setColorMode(KMLSubStyle::NORMAL);
				s4->setWidth(1);
				transp = 255;
				s4->setAlpha(transp);

				KMLStyle* sStyle = new KMLStyle;
				sStyle->setId(doc->getName() + "_poly_sel");
				sStyle->setVisual(KMLSubStyle::POLYGON, s3);
				sStyle->setVisual(KMLSubStyle::LINE, s4);

				KMLStyleMap* sMap = new KMLStyleMap;
				sMap->mapStyle("normal", "#" + dStyle->getId());
				sMap->mapStyle("highlight", "#" + sStyle->getId());
				sMap->setId(doc->getName() + "_poly");

				doc->addChild(dStyle);
				doc->addChild(sStyle);
				doc->addChild(sMap);

				styleURL = sMap->getId();
			}
			else if(rep == TeLINES)
			{

				TeColor auxColor;
				auxColor.red_ = 0;
				auxColor.green_ = 0;
				auxColor.blue_ = 0;

				KMLLineStyle* s1 = new KMLLineStyle;
				s1->setColor(auxColor);
				s1->setColorMode(KMLSubStyle::NORMAL);
				s1->setWidth(1);
				int transp = 255;
				s1->setAlpha(transp);

				KMLStyle* dStyle = new KMLStyle;
				dStyle->setId(doc->getName() + "_line_def");
				dStyle->setVisual(KMLSubStyle::LINE, s1);

				KMLLineStyle* s2 = new KMLLineStyle;
				s2->setColor(auxColor);
				s2->setColorMode(KMLSubStyle::NORMAL);
				s2->setWidth(1);
				transp = 255;
				s2->setAlpha(transp);

				KMLStyle* sStyle = new KMLStyle;
				sStyle->setId(doc->getName() + "_line_sel");
				sStyle->setVisual(KMLSubStyle::LINE, s2);

				KMLStyleMap* sMap = new KMLStyleMap;
				sMap->mapStyle("normal", "#" + dStyle->getId());
				sMap->mapStyle("highlight", "#" + sStyle->getId());
				sMap->setId(doc->getName() + "_line");

				doc->addChild(dStyle);
				doc->addChild(sStyle);
				doc->addChild(sMap);

				styleURL = sMap->getId();
			}
			else if(rep == TePOINTS)
			{
				TeColor auxColor;
				auxColor.red_ = 255;
				auxColor.green_ = 255;
				auxColor.blue_ = 0;

				KMLPointStyle* s1 = new KMLPointStyle;
				s1->setColor(auxColor);
				s1->setColorMode(KMLSubStyle::NORMAL);
				s1->setAlpha(255);

				KMLStyle* dStyle = new KMLStyle;
				dStyle->setId(doc->getName() + "_point_def");
				dStyle->setVisual(KMLSubStyle::ICON, s1);

				KMLPointStyle* s2 = new KMLPointStyle;
				s2->setColor(auxColor);
				s2->setColorMode(KMLSubStyle::NORMAL);
				s2->setAlpha(255);

				KMLStyle* sStyle = new KMLStyle;
				sStyle->setId(doc->getName() + "_point_sel");
				sStyle->setVisual(KMLSubStyle::ICON, s2);

				KMLStyleMap* sMap = new KMLStyleMap;
				sMap->mapStyle("normal", "#" + dStyle->getId());
				sMap->mapStyle("highlight", "#" + sStyle->getId());
				sMap->setId(doc->getName() + "_line");

				doc->addChild(dStyle);
				doc->addChild(sStyle);
				doc->addChild(sMap);

				styleURL = sMap->getId();
			}

			std::string query2 = "select * from " + layer->tableName(rep) + " p ";
			query2 += "order by " + db->getSQLOrderBy(rep);

		//create placemarks
			TeDatabasePortal* gPortal = db->getPortal();

			if(!gPortal->query(query2))
			{
				docs.clear();
				return docs;
			}

			if(!gPortal->fetchRow())
			{
				docs.clear();
				return docs;
			}

			bool hasGeom = false;
			unsigned int descriptIdx = 0;

			do
			{
				tdk::KMLPlacemark* mark = new tdk::KMLPlacemark;

				if(rep == TePOLYGONS)
				{
					TePolygon poly;
					hasGeom = gPortal->fetchGeometry(poly); 
					
					if(need2Reproj)
						changeProjection(layer->projection(), kmlProj, poly);
	
					mark->setGeometry(poly);
					mark->setId(poly.objectId());
				}
				else if(rep == TeLINES)
				{
					TeLine2D line;
					hasGeom = gPortal->fetchGeometry(line); 
	
					if(need2Reproj)
						changeProjection(layer->projection(), kmlProj, line);

					mark->setGeometry(line);
					mark->setId(line.objectId());
				}
				if(rep == TePOINTS)
				{
					TePoint pt;
					hasGeom = gPortal->fetchGeometry(pt); 
						
					if(need2Reproj)
						changeProjection(layer->projection(), kmlProj, pt);

					mark->setGeometry(pt);
					mark->setId(pt.objectId());
				}

				marks.push_back(mark);
				
				if(!styleURL.empty())
					mark->setStyleURL("#" + styleURL);

				if(!descript.empty() && descriptIdx < descript.size())
					mark->setDescription(descript[descriptIdx]);

				if(!name.empty() && descriptIdx < name.size())
					mark->setName(name[descriptIdx]);

				descriptIdx++;
			} while(hasGeom);

			gPortal->freeResult();
			delete gPortal;
		}

		vector<KMLPlacemark*>::iterator markIt;
		doc->addChild(folder);

		for(markIt = marks.begin(); markIt != marks.end(); ++markIt)
			folder->addChild(*markIt);

		docs.push_back(doc);
		
		delete kmlProj;
	}

	return docs;
}

int convertTransparency2Alpha(int transparency)
{
	int alpha;
	if (transparency < 0)
		alpha = 255;
	else if (transparency > 100)
		alpha = 0;
	else
	{
		double t = (double)transparency / 100.;
		alpha = (int)(255. * (1. - t));
	}

	return alpha;
}

TeBox tetrahedronBBox(const TeCoord2D& pt0, const TeCoord2D& pt1, 
                const TeCoord2D& pt2, const TeCoord2D& pt3)
{
  double xmin, ymin, xmax, ymax;

  if (pt0.x() < pt1.x())
  {
    if (pt2.x() < pt3.x())
    {
      xmin = MIN(pt0.x(), pt2.x());
      xmax = MAX(pt1.x(), pt3.x());
    }else
    {
      xmin = MIN(pt0.x(), pt3.x());
      xmax = MAX(pt1.x(), pt2.x());
    }
  }else
  {
    if (pt2.x() < pt3.x())
    {
      xmin = MIN(pt1.x(), pt2.x());
      xmax = MAX(pt0.x(), pt3.x());
    }else
    {
      xmin = MIN(pt1.x(), pt3.x());
      xmax = MAX(pt0.x(), pt2.x());
    }
  }

  if (pt0.y() < pt1.y())
  {
    if (pt2.y() < pt3.y())
    {
      ymin = MIN(pt0.y(), pt2.y());
      ymax = MAX(pt1.y(), pt3.y());
    }else
    {
      ymin = MIN(pt0.y(), pt3.y());
      ymax = MAX(pt1.y(), pt2.y());
    }
  }else
  {
    if (pt2.y() < pt3.y())
    {
      ymin = MIN(pt1.y(), pt2.y());
      ymax = MAX(pt0.y(), pt3.y());
    }else
    {
      ymin = MIN(pt1.y(), pt3.y());
      ymax = MAX(pt0.y(), pt2.y());
    }
  }

  return TeBox(xmin, ymin, xmax, ymax);
}

void changeProjection(TeProjection* fromProj, TeProjection* toProj, TeBox& box)
{
	if (fromProj == NULL || toProj == NULL)
	{
		return;
	}

	if (*fromProj == *toProj)
	{
		return;
	}
	
	box = TeRemapBox(box, fromProj, toProj);

	box = tetrahedronBBox(box.lowerLeft(), TeCoord2D(box.x2(), box.y1()), 
	box.upperRight(), TeCoord2D(box.x1(), box.y2()));
}

void changeProjection(TeProjection* fromProj, TeProjection* toProj, 
                                                  TeCoord2D& coord)
{
    if (!(fromProj == NULL || toProj == NULL))
    {
        TeCoord2D c = coord;
        fromProj->setDestinationProjection(toProj);
        c = fromProj->PC2LL(c);
        c = toProj->LL2PC(c);
        
        coord.x_ = c.x();
        coord.y_ = c.y();
    }
}

void changeProjection(TeProjection* fromProj, TeProjection* toProj, 
                                                  TePoint& point)
{
    if (!(fromProj == NULL || toProj == NULL))
    {
        TeCoord2D c = point.location();
        fromProj->setDestinationProjection(toProj);
        c = fromProj->PC2LL(c);
        c = toProj->LL2PC(c);

        point.add(c);
    }
}

void changeProjection(TeProjection* fromProj, TeProjection* toProj, 
                                                  TePointSet& pointSet)
{
	TePointSet::iterator it;
	for(it = pointSet.begin(); it != pointSet.end(); ++it)
	{
		changeProjection(fromProj, toProj, (*it));
	}
	TeBox box = pointSet.box();
	changeProjection(fromProj, toProj, box);
	pointSet.setBox(box);
}

void changeProjection(TeProjection* fromProj, TeProjection* toProj, 
                                                  TeLine2D& line)
{
    TeLine2D::iterator it;

    for(it = line.begin(); it != line.end(); ++it)
    {
        changeProjection(fromProj, toProj, (*it));		
    }
	TeBox box = line.box();
	changeProjection(fromProj, toProj, box);
	line.setBox(box);
}

void changeProjection(TeProjection* fromProj, TeProjection* toProj, 
                                                  TeLineSet& lineSet)
{
	TeLineSet::iterator it;
	for(it = lineSet.begin(); it != lineSet.end(); ++it)
	{
		changeProjection(fromProj, toProj, (*it));		
	}
	TeBox box = lineSet.box();
	changeProjection(fromProj, toProj, box);
	lineSet.setBox(box);
}

void changeProjection(TeProjection* fromProj, TeProjection* toProj, 
                                                  TePolygon& poly)
{
    TePolygon::iterator it;

    for(it = poly.begin(); it != poly.end(); ++it)
    {
        changeProjection(fromProj, toProj, (*it));		
    }
	TeBox box = poly.box();
	changeProjection(fromProj, toProj, box);
	poly.setBox(box);
}

void changeProjection(TeProjection* fromProj, TeProjection* toProj, 
                                                  TePolygonSet& polygonSet)
{
	TePolygonSet::iterator it;
	for(it = polygonSet.begin(); it != polygonSet.end(); ++it)
	{
		changeProjection(fromProj, toProj, (*it));		
	}
	TeBox box = polygonSet.box();
	changeProjection(fromProj, toProj, box);
	polygonSet.setBox(box);
}