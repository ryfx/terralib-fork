#ifndef TDK_KMLSTYLESMANAGER_H
#define TDK_KMLSTYLESMANAGER_H


//TDK KML include files
#include "kmldefines.h"

#include <vector>
using namespace std;
#include <string>
using namespace std;
#include <map>
using namespace std;

namespace tdk { class KMLStyle; } 
namespace tdk { class KMLStyleMap; } 

namespace tdk {

class TDKKML_API KMLStylesManager 
{
  public:
    KMLStylesManager();

    ~KMLStylesManager();

    void addStyle(KMLStyle * style);

    //Adds reference to KML style map object
    void addStyle(KMLStyleMap * styleMap);

    vector<const KMLStyle*> getStyles(const string & id);


  protected:
    //Defined styles.
    map<string, KMLStyle*> _styleMap;

    //Style Map.
    multimap<string, string> _styleMappings;


  private:
    //No copy allowed.
    KMLStylesManager(const KMLStylesManager & source);

    //No copy allowed.
    KMLStylesManager & operator=(const KMLStylesManager & source);

};

} // namespace tdk
#endif
