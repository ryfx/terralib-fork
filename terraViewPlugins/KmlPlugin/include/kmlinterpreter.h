#ifndef KMLINTERPRETER_H
#define KMLINTERPRETER_H

#include "kmldefines.h"
#include "kmlabstractvisitor.h"
#include <map>
using namespace std;
#include <string>
using namespace std;
#include "TeGeometry.h"
#include "TeTable.h"

namespace tdk { 
	class KMLDocument; 
	class KMLFolder;
	class KMLPlacemark;
	class KMLStyle; 
	class KMLStyleMap; 
	class KMLFeature;
}

class TeLayer;
class TeDatabase;

namespace tdk {

class TDKKML_API KMLInterpreter : public KMLAbstractVisitor 
{
  public:
    KMLInterpreter();

    ~KMLInterpreter();

    void visit(KMLDocument & doc);

    void visit(KMLFolder & folder);

    void visit(KMLPlacemark & mark);

    void visit(KMLStyle & style);

    void visit(KMLStyleMap & style);

    void interpret(const map<string, KMLFeature*> & feats, ::TeDatabase * db, TePointSet & pointSet, TeLineSet & lineSet, TePolygonSet & polygonSet, TeTable & attrTable, TeTable & styleTable);

	//TeTable getAttrTable(const string& tabName);

	//TeTable getStyleTable();

protected:

	TeTable createStyleTable();

	TeTable createAttrTable();

	void organizeInformation();

protected:

	::TePointSet _points; //!< 

    ::TeLineSet _lines; //!< 

    ::TePolygonSet _polygons; //!< 

    ::TeTable _attTable;

	::TeTable _styleTable;

	std::vector<KMLStyle*> _styles;

	std::vector<KMLStyleMap*> _styleMaps;

	std::map<std::string, KMLPlacemark*> _placeMarks;

    int _count; //!< 
	
};

} // namespace tdk
#endif
