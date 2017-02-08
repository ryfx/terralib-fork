#ifndef TDK_KMLIMPORTER_H
#define TDK_KMLIMPORTER_H


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

class TDKKML_API KMLImporter : public KMLAbstractVisitor 
{
  public:
    KMLImporter();

    ~KMLImporter();

    void visit(KMLDocument & doc);

    void visit(KMLFolder & folder);

    void visit(KMLPlacemark & mark);

    void visit(KMLStyle & style);

    void visit(KMLStyleMap & style);

    TeLayer* import(const map<string, KMLFeature*> & docs, const string & layerName, ::TeDatabase * db);


  protected:
    ::TePointSet _points; //!< 

    ::TeLineSet _lines; //!< 

    ::TePolygonSet _polygons; //!< 

    ::TeTable _attTable;

    int _count; //!< 

};

} // namespace tdk
#endif
