
#include "kmlreader.h"
#include "kmldocument.h"
#include "kmlfolder.h"
#include "kmlplacemark.h"
#include "kmlstyle.h"
#include "kmlstylemap.h"
#include "kmlpointstyle.h"
#include "kmllinestyle.h"
#include "kmlpolygonstyle.h"
#include "TeXMLDecoder.h"
#include "kmlstylesmanager.h"
#include "kmlobject.h"
#include "kmlfeature.h"
#include "kmlsubstyle.h"

//KML include files
#include <kmlobjectfactory.h>

//TeOGC include files
#include <TeXMLParserParams.h>
#include <TeXMLDecoderFactory.h>
#include <TeXMLException.h> 

//TerraLib include files
#include <TeVisual.h>
#include <TeGeometry.h>
#include <TeUtils.h>

//STL include files
#include <cstdlib>

using namespace TeOGC;

int hexToInt(const char * hex) 
{
	int hexInt;  
	sscanf( hex, "%x", &hexInt );  
	return hexInt;  
}

void hexaToTeColor(const string & hexaColor, int & r, int & g, int & b, int & a) 
{
	a = hexToInt(hexaColor.substr(0, 2).c_str());
	r = hexToInt(hexaColor.substr(2, 2).c_str());
	g = hexToInt(hexaColor.substr(4, 2).c_str());
	b = hexToInt(hexaColor.substr(6, 2).c_str());
}

tdk::KMLPointStyle::OffsetUnit getUnit(const string& sUnit)
{
	return (sUnit == "insetpixels") ? tdk::KMLPointStyle::INSETPIXELS :
		(sUnit == "fraction") ? tdk::KMLPointStyle::FRACTION :
			tdk::KMLPointStyle::PIXELS;
}

TeLinearRing strToRing(const string& strLine)
{
	// tuplas separadas por espacos
	vector<std::string> tupleVal;
	vector<std::string>::iterator it;
	TeSplitString( strLine, " ", tupleVal );
	TeLinearRing ring;

	// para cada tupla, pegar o comma
	for(it = tupleVal.begin(); it != tupleVal.end(); ++it)
	{
		string aux = *it;
		TeTrim(aux);

		if(aux.empty())
			continue;

		std::vector<std::string> commaVal;

		TeSplitString(aux, ",", commaVal );

		// tem soh um valor, coloque zero!
		if( commaVal.size() == 1 )
			commaVal.push_back( "0.0" );

		ring.add( TeCoord2D( atof( commaVal.at(0).c_str() ), atof( commaVal.at(1).c_str() ) ) );
	}

	return ring;
}

namespace tdk {

KMLReader::KMLReader() :
KMLAbstractVisitor()
{
	_stylesManager = new KMLStylesManager();
	_hasMultiPolygon = false;
}

KMLReader::~KMLReader() 
{
}

void KMLReader::visit(KMLDocument & doc) 
{
	parseObject(doc);

	if(_decoder->setFirstChildAsCurrent())
	{
		do
		{
			parseFeature(doc);
		} while(_decoder->setNextSiblingAsCurrent());

		_decoder->closeElement();
	}

	//if(doc.parent() == NULL)
	//	_docs[doc.getName()] = &doc;
}

void KMLReader::visit(KMLFolder & folder) 
{
	parseObject(folder);

	if(_decoder->setFirstChildAsCurrent())
	{
		do
		{
			parseFeature(folder);
		} while(_decoder->setNextSiblingAsCurrent());

		_decoder->closeElement();
	}
}

void KMLReader::visit(KMLPlacemark & mark) 
{
	parseObject(mark);

	if(_decoder->setFirstChildAsCurrent())
	{
		do
		{
			string localName = _decoder->getLocalName();

			if(localName == "MultiGeometry")
			{
				_hasMultiPolygon = true;
				return;
			}
			else if(localName == "Point")
			{
				getPoint(_decoder, mark);
			}
			else if(localName == "Line")
			{
				getLine(_decoder, mark);
			}
			else if(localName == "LineString")
			{
				getLineString(_decoder, mark);
			}			
			else if(localName == "Polygon")
			{
				getPolygon(_decoder, mark);
			}
			else
				parseFeature(mark);
		} while(_decoder->setNextSiblingAsCurrent());

		_decoder->closeElement();
	}
}

bool KMLReader::hasMultiPolygon()
{
	return _hasMultiPolygon;
}

void KMLReader::visit(KMLStyle & style) 
{
	style.setId("#"+_decoder->getAttribute("id"));

	//Parse IconStyle
	if(_decoder->setCurrentElement("IconStyle"))
	{
		KMLPointStyle* s = new KMLPointStyle();
		s->setId(_decoder->getAttribute("id"));
		visit(*s);
		style.setVisual(KMLSubStyle::ICON, s);
	}

	//Parse LineStyle
	if(_decoder->setCurrentElement("LineStyle"))
	{
		KMLLineStyle* s = new KMLLineStyle();
		s->setId(_decoder->getAttribute("id"));
		visit(*s);
		style.setVisual(KMLSubStyle::LINE, s);
	}

	//Parse PolygonStyle
	if(_decoder->setCurrentElement("PolyStyle"))
	{
		KMLPolygonStyle* s = new KMLPolygonStyle();
		s->setId(_decoder->getAttribute("id"));
		visit(*s);
		style.setVisual(KMLSubStyle::POLYGON, s);
	}

	_stylesManager->addStyle(&style);
}

void KMLReader::visit(KMLStyleMap & style) 
{
	string styleId =  _decoder->getAttribute("id");

	if(!styleId.empty())
		style.setId("#" + styleId);

	if(_decoder->setFirstChildAsCurrent())
	{
		do
		{
			string localName = _decoder->getLocalName();

			if(localName == "Pair")
			{
				string key = _decoder->getChildValue("key"),
						url =_decoder->getChildValue("styleUrl");

				if(!key.empty() && !url.empty())
					style.mapStyle(style.getId(), key+url);
			}
		} while(_decoder->setNextSiblingAsCurrent());

		_decoder->closeElement();
	}

	_stylesManager->addStyle(&style);
}

void KMLReader::visit(KMLPointStyle & pStyle) 
{
	parseSubstyle(pStyle);

	//Parse scale
	string value = _decoder->getChildValue("scale");

	if(!value.empty())
		pStyle.setScale(atof(value.c_str()));

	//Parse heading
	value = _decoder->getChildValue("heading");

	if(!value.empty())
		pStyle.setHeading(atof(value.c_str()));

	//Parse Icon
	if(_decoder->setCurrentElement("Icon"))
	{
		value = _decoder->getChildValue("href");
		pStyle.setIcon(value);
		_decoder->closeElement();
	}

	//Parse hotSpot
	if(_decoder->setCurrentElement("hotSpot"))
	{
		string sx = _decoder->getAttribute("x"),
			sy = _decoder->getAttribute("y");

		if(!sx.empty() && !sy.empty())
		{
			double x = atof(sx.c_str()),
				y = atof(sy.c_str());

			TeCoord2D location(x, y);

			pStyle.setHotspot(location);
		}

		KMLPointStyle::OffsetUnit xUnit = getUnit(_decoder->getAttribute("xunit")),
				yUnit = getUnit(_decoder->getAttribute("yunit"));

		pStyle.setHotspotUnit(xUnit, yUnit);

		_decoder->closeElement();
	}

	_decoder->closeElement();
}

void KMLReader::visit(KMLLineStyle & lStyle) 
{
	parseSubstyle(lStyle);

	string value = _decoder->getChildValue("width");

	if(!value.empty())
	{
		lStyle.setWidth(atoi(value.c_str()));
	}

	_decoder->closeElement();
}

void KMLReader::visit(KMLPolygonStyle & polStyle) 
{
	parseSubstyle(polStyle);

	//Parse fill
	string value = _decoder->getChildValue("fill");
	polStyle.setFill(value == "1" || value == "true");

	//Parse outline
	value = _decoder->getChildValue("outline");
	polStyle.setOutline(value == "1" || value == "true");

	_decoder->closeElement();
}

void KMLReader::parse(TeOGC::TeXMLDecoder & xml) 
{
	if(!xml.setFirstChildAsCurrent())
		throw;

	parseChild(xml.getElementName(), NULL);
}

void KMLReader::parse(const string & xmlName) 
{
	TeXMLParserParams p;
	_decoder = TeXMLDecoderFactory::make(p);
	_decoder->setDocumentFromFile(xmlName);

	parse(*_decoder);
}

map<string, KMLFeature*> KMLReader::getObjects() const 
{
  return _feats;
}

KMLStylesManager * KMLReader::getStylesManager() const 
{
  return _stylesManager;
}

void KMLReader::parseChild(const string & elem, KMLObject * parent) 
{
	KMLObjsParams p(elem, parent);
	KMLObject* obj = KMLObjectFactory::make(&p);

	if(obj == NULL)
		return;

	try
	{
		obj->accept(*this);
	}
	catch(...)
	{
		delete obj;
	}
}

void KMLReader::parseFeature(KMLFeature & feature) 
{
	string localName = _decoder->getLocalName(),
			value;

	if( localName == "name" )
	{
		_decoder->getElementValue(value);
		feature.setName(value);
	}
	else if( localName == "open" )
	{
		_decoder->getElementValue(value);
		feature.setOpen(value == "1" || value == "true" );
	}
	else if( localName == "styleUrl" )
	{
		_decoder->getElementValue(value);
		feature.setStyleURL(value);
	}
	else if( localName == "visibility" )
	{
		_decoder->getElementValue(value);
		feature.setVisibility(value == "1" || value == "true");
	}
	else if( localName == "description" )
	{
		// aqui pode ter CDATA!
		string description;
		_decoder->getElementValue(description);

		std::vector<std::string> cdata;
		_decoder->getCDATASections(cdata);

		//Trecho inserido para não haver repetição de informação
		for( unsigned i = 0; i < cdata.size(); ++i )
		{
			if(cdata[i] != description)
				description += cdata[i];
		}
			

		feature.setDescription(description);
	}
	else
	{
		parseChild(localName, &feature);
	}

	if(feature.parent() == 0)
		_feats[feature.getName()] = &feature;
}

void KMLReader::parseObject(KMLObject & obj) 
{
	obj.setId(_decoder->getAttribute("id"));
	obj.setTargetId(_decoder->getAttribute("targetId"));
}

void KMLReader::parseSubstyle(KMLSubStyle & style) 
{
	//Parse color
	string value = _decoder->getChildValue("color");

	if(!value.empty())
	{
		int r, g, b, a;

		hexaToTeColor(value, r, g, b, a);
		style.setColor(TeColor(r, g, b));
		style.setAlpha(a);
	}

	//Parse colorMode
	value = _decoder->getChildValue("colorMode");
	KMLSubStyle::KMLColorMode mode = (value == "normal") ? KMLSubStyle::NORMAL : KMLSubStyle::RANDOM;

	style.setColorMode(mode);
}

void KMLReader::getPoint(TeOGC::TeXMLDecoder * currentDecoder, KMLPlacemark & mark)
{
	string coords = currentDecoder->getChildValue("coordinates");

	if(!coords.empty())
	{
		std::vector<std::string> commaVal;
		TeSplitString(coords, ",", commaVal);

		if(commaVal.size() < 2)
			return;

		TePoint coord(atof(commaVal.at(0).c_str()), atof(commaVal.at(1).c_str()));
		mark.setGeometry(coord);
	}
}

void KMLReader::getLine(TeOGC::TeXMLDecoder * currentDecoder, KMLPlacemark & mark)
{
	if(currentDecoder->setCurrentElement("LineString"))
	{
		string value = currentDecoder->getChildValue("coordinates");

		if(!value.empty())
		{
			TeLine2D line = strToRing(value);

			if(!line.empty())
				mark.setGeometry(line);
		}
	}
}

void KMLReader::getLineString(TeOGC::TeXMLDecoder * currentDecoder, KMLPlacemark & mark)
{
	string value = currentDecoder->getChildValue("coordinates");

	if(!value.empty())
	{
		TeLine2D line = strToRing(value);

		if(!line.empty())
			mark.setGeometry(line);
	}
}

void KMLReader::getPolygon(TeOGC::TeXMLDecoder * currentDecoder, KMLPlacemark & mark)
{
	TePolygon poly;
	vector<TeLinearRing> holes;

	if(currentDecoder->setFirstChildAsCurrent())
	{
		do
		{
			string localName = currentDecoder->getLocalName();

			if(localName == "outerBoundaryIs")
			{
				if(currentDecoder->setCurrentElement("LinearRing"))
				{
					string value = currentDecoder->getChildValue("coordinates");
					TeLinearRing ring = strToRing(value);

					if(!ring.empty())
						poly.add(ring);

					currentDecoder->closeElement();
				}
			}
			else if(localName == "innerBoundaryIs")
				if(currentDecoder->setCurrentElement("LinearRing"))
				{
					string value = currentDecoder->getChildValue("coordinates");
					TeLinearRing ring = strToRing(value);

					if(!ring.empty())
						holes.push_back(ring);

					currentDecoder->closeElement();
				}
		} while(currentDecoder->setNextSiblingAsCurrent());

		currentDecoder->closeElement();
	}

	vector<TeLinearRing>::iterator it;

	for(it = holes.begin(); it != holes.end(); it++)
		poly.add(*it);

	mark.setGeometry(poly);	
}

} // namespace tdk
