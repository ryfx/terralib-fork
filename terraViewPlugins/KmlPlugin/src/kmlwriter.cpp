
#include "kmlwriter.h"
#include "kmldocument.h"
#include "kmlfolder.h"
#include "kmlplacemark.h"
#include "kmlstyle.h"
#include "kmlstylemap.h"
#include "TeXMLEncoder.h"
#include "kmlobject.h"
#include "kmlfeature.h"
#include "TeGeometry.h"
#include "kmllinestyle.h"

string TeGeomToKML(const TeCoord2D& c)
{
	string p;

	p = Te2String(c.x(), 14) + "," + Te2String(c.y(), 14);

	return p;
}

string TeGeomToKML(const TeLinearRing& line)
{
	string l;
	TeLinearRing::iterator it;

	for(it = line.begin(); it != line.end(); ++it)
		l += TeGeomToKML(*it) + ",0 \n";

	return l;
}

string int2Hex(const unsigned int& num)
{
	string res;

	if(num == 0)
		res = "00";
	else if(num <= 255)
	{
		char buf[6];
		sprintf(buf, "%x", num);

		res = buf;
	}

	return res;
}

namespace tdk {

KMLWriter::KMLWriter() :
KMLAbstractVisitor()
{
}

KMLWriter::~KMLWriter() 
{
}

void KMLWriter::visit(KMLDocument & doc) 
{
	_xmlEncoder->addElement("Document");
	writeFeature(doc);

	int childs = doc.childrenCount();

	for(int i=0; i<childs; i++)
		((KMLObject*)doc.getChild(i))->accept(*this);

	_xmlEncoder->closeElement();
}

void KMLWriter::visit(KMLFolder & folder) 
{
	_xmlEncoder->addElement("Folder");
	writeFeature(folder);

	int childs = folder.childrenCount();

	for(int i=0; i<childs; i++)
		((KMLObject*)folder.getChild(i))->accept(*this);

	_xmlEncoder->closeElement();
}

void KMLWriter::visit(KMLPlacemark & mark) 
{
	_xmlEncoder->addElement("Placemark");
	writeFeature(mark);

	switch(mark.getGeometryType())
	{
		case KMLPlacemark::POINT:
			writeGeometry((const TePoint&)mark.getGeometry());
		break;

		case KMLPlacemark::LINE:
			writeGeometry((const TeLine2D&)mark.getGeometry());
		break;

		case KMLPlacemark::POLYGON:
			writeGeometry((const TePolygon&)mark.getGeometry());
		break;
	}

	_xmlEncoder->closeElement();
}

void KMLWriter::visit(KMLStyle & style) 
{
	_xmlEncoder->addElement("Style");
	_xmlEncoder->addAttribute("id", style.getId());

	KMLSubStyle* subS = style.getVisual(KMLSubStyle::POLYGON);

	if(subS != NULL)
		writeStyle(*subS);

	subS = style.getVisual(KMLSubStyle::LINE);

	if(subS != NULL)
		writeStyle(*subS);

	subS = style.getVisual(KMLSubStyle::ICON);

	if(subS != NULL)
		writeStyle(*subS);

	_xmlEncoder->closeElement();
}

void KMLWriter::visit(KMLStyleMap & style) 
{
	_xmlEncoder->addElement("StyleMap");
	
	if(!style.getId().empty())
		_xmlEncoder->addAttribute("id", style.getId());

	map<string, string> styles = style.getStyles();
	map<string, string>::iterator it;

	for(it = styles.begin(); it != styles.end(); ++it)
	{
		string key = it->first,
			styleUrl = it->second;

		if(!key.empty() && !styleUrl.empty())
		{
			_xmlEncoder->addElement("Pair");
			_xmlEncoder->addElement("key", key);
			_xmlEncoder->addElement("styleUrl", styleUrl);
			_xmlEncoder->closeElement();
		}
	}

	_xmlEncoder->closeElement();
}

void KMLWriter::setDocuments(const vector< KMLFeature*>& feats) 
{
  _feats = feats;
}

void KMLWriter::write(TeOGC::TeXMLEncoder & encoder) 
{
	_xmlEncoder = &encoder;

	if(!_xmlEncoder->createDocumentNS( "kml", "http://www.opengis.net/kml/2.2" ) )
	{
		_xmlEncoder->addElement( "kml" );
		_xmlEncoder->addNamespace( "", "http://www.opengis.net/kml/2.2" );
	}

	vector<KMLFeature*>::iterator it;

	for(it = _feats.begin(); it != _feats.end(); ++it)
		(*it)->accept(*this);
}

void KMLWriter::writeObject(const KMLObject & object) 
{
	if(!object.getId().empty())
		_xmlEncoder->addAttribute("id", object.getId());

	if(!object.getTargetId().empty())
		_xmlEncoder->addAttribute("targetId", object.getTargetId());
}

void KMLWriter::writeFeature(const KMLFeature & feature) 
{
	writeObject(feature);

	if(!feature.getName().empty())
		_xmlEncoder->addElement("name", feature.getName());

	string vis = (feature.getVisibility()) ? "1" : "0";
	_xmlEncoder->addElement("visibility", vis);

	vis = (feature.getOpen()) ? "1" : "0"; 
	_xmlEncoder->addElement("open", vis);

	if(!feature.getDescription().empty())
		_xmlEncoder->addElement("description", feature.getDescription());

	if(!feature.getStyleURL().empty())
		_xmlEncoder->addElement("styleUrl", feature.getStyleURL());
}

void KMLWriter::writeGeometry(const ::TePoint & geom) 
{
	string coords = TeGeomToKML(geom.location());

	_xmlEncoder->addElement("Point");
	_xmlEncoder->addElement("coordinates", coords);
	_xmlEncoder->closeElement();

}

void KMLWriter::writeGeometry(const ::TeLine2D & geom) 
{
	TeLinearRing l;
	l.copyElements(geom);

	string coords = TeGeomToKML(l);

	_xmlEncoder->addElement("LineString");
	_xmlEncoder->addElement("coordinates", coords);
	_xmlEncoder->closeElement();
}

void KMLWriter::writeGeometry(const ::TePolygon & geom) 
{
	_xmlEncoder->addElement("Polygon");

	for(unsigned int i=0; i<geom.size(); i++)
	{
		(i == 0) ?
			_xmlEncoder->addElement("outerBoundaryIs") :
			_xmlEncoder->addElement("innerBoundaryIs");

		string coords = TeGeomToKML(geom[i]);
		_xmlEncoder->addElement("LinearRing");
		_xmlEncoder->addElement("coordinates", coords);
		_xmlEncoder->closeElement();
		_xmlEncoder->closeElement();
	}
	_xmlEncoder->closeElement();
}

void KMLWriter::writeStyle(const KMLSubStyle & style) 
{
	switch(style.getStyleType())
	{
		case KMLSubStyle::POLYGON:
			_xmlEncoder->addElement("PolyStyle");
		break;
		
		case KMLSubStyle::LINE:
			_xmlEncoder->addElement("LineStyle");
		break;
		
		case KMLSubStyle::ICON:
			_xmlEncoder->addElement("IconStyle");
		break;

		default:
		break;
	}

	if(!style.getId().empty())
		_xmlEncoder->addAttribute("id", style.getId());

	TeColor c = style.getColor();
	string color = int2Hex(style.getAlpha()) +
		int2Hex(c.blue_) + int2Hex(c.green_) +
		int2Hex(c.red_);

	if(!color.empty())
		_xmlEncoder->addElement("color", color);

	_xmlEncoder->addElement("colorMode", 
		(style.getColorMode() == KMLSubStyle::NORMAL) ? "normal" : "random");

	switch(style.getStyleType())
	{
		case KMLSubStyle::POLYGON:
			_xmlEncoder->addElement("fill", (style.getAlpha() == 0) ? "0" : "1");
			_xmlEncoder->addElement("outline", "1");
		break;
		
		case KMLSubStyle::LINE:
			_xmlEncoder->addElement("width", Te2String(((const KMLLineStyle&)style).getWidth()));
		break;
		
		case KMLSubStyle::ICON:
		break;

		default:
		break;
	}

	_xmlEncoder->closeElement();
}


} // namespace tdk
