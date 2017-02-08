#ifndef TDK_KMLMANAGER_H
#define TDK_KMLMANAGER_H


#include "kmldefines.h"
#include <map>
using namespace std;
#include <string>
using namespace std;
#include <vector>
using namespace std;

namespace tdk { 
	class KMLDocument; 
	class KMLStylesManager; 
	class KMLStyle; 
	class KMLImporter;
	class KMLReader;
	class KMLWriter;
	class KMLFeature;
} 
namespace TeOGC { class TeXMLEncoder; } 
namespace TeOGC { class TeXMLEncoderFactory; } 

namespace tdk {

class TDKKML_API KMLManager {
  public:
    KMLManager();

    ~KMLManager();

    //TDKKML_API
    map<string, KMLFeature*> getObjects();

    KMLStylesManager * getStylesManager() const;

    vector<const KMLStyle*> foundStyle(const string & styleId);

    void parse(const string & kmlFileName);

    void saveKML(const vector<KMLFeature*> & feats, const string & kml);

	map<string, KMLFeature*> getFeatures();

	bool hasMultiPolygon();


  protected:
    KMLStylesManager * _stylesMger;


  private:
    //No copy allowed.
    KMLManager(const KMLManager & source);

    //No copy allowed.
    KMLManager & operator=(const KMLManager & source);


  protected:
    map<string, KMLFeature*> _feats;

	bool _hasMultiPolygon;

};

} // namespace tdk
#endif
