#ifndef TDK_KMLREADER_H
#define TDK_KMLREADER_H


#include "kmldefines.h"
#include "kmlabstractvisitor.h"
#include <string>
using namespace std;
#include <map>
using namespace std;

namespace tdk { class KMLDocument; } 
namespace tdk { class KMLFolder; } 
namespace tdk { class KMLPlacemark; } 
namespace tdk { class KMLStyle; } 
namespace tdk { class KMLStyleMap; } 
namespace tdk { class KMLPointStyle; } 
namespace tdk { class KMLLineStyle; } 
namespace tdk { class KMLPolygonStyle; } 
namespace TeOGC { class TeXMLDecoder; } 
namespace tdk { class KMLStylesManager; } 
namespace tdk { class KMLObject; } 
namespace tdk { class KMLFeature; } 
namespace tdk { class KMLSubStyle; } 

namespace tdk {

class TDKKML_API KMLReader : public KMLAbstractVisitor 
{
  public:
    KMLReader();

    ~KMLReader();

    void visit(KMLDocument & doc);

    void visit(KMLFolder & folder);

    void visit(KMLPlacemark & mark);

    void visit(KMLStyle & style);

    void visit(KMLStyleMap & style);


  protected:
    void visit(KMLPointStyle & pStyle);

    void visit(KMLLineStyle & lStyle);

    void visit(KMLPolygonStyle & polStyle);


  public:
    void parse(TeOGC::TeXMLDecoder & xml);

    void parse(const string & xmlName);

    map<string, KMLFeature*>  getObjects() const;

    KMLStylesManager * getStylesManager() const;

	bool hasMultiPolygon();


  protected:
    void parseChild(const string & elem, KMLObject * parent);

    void parseFeature(KMLFeature & feature);

    void parseObject(KMLObject & obj);

    void parseSubstyle(KMLSubStyle & style);

    //Map with documents in a kml file.
    map<string, KMLFeature*> _feats;

    KMLStylesManager * _stylesManager;

    //XML decoder pointer.
    TeOGC::TeXMLDecoder * _decoder;

	bool _hasMultiPolygon;

	void getPoint(TeOGC::TeXMLDecoder * currentDecoder, KMLPlacemark & mark);
	void getLine(TeOGC::TeXMLDecoder * currentDecoder, KMLPlacemark & mark);
	void getLineString(TeOGC::TeXMLDecoder * currentDecoder, KMLPlacemark & mark);
	void getPolygon(TeOGC::TeXMLDecoder * currentDecoder, KMLPlacemark & mark);
	void getRepresentations(TeOGC::TeXMLDecoder * currentDecoder, KMLPlacemark & mark);

};

} // namespace tdk
#endif
