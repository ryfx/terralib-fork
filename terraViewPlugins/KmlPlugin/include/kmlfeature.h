#ifndef TDK_KMLFEATURE_H
#define TDK_KMLFEATURE_H


#include "kmlobject.h"
#include "kmldefines.h"
#include <string>
using namespace std;
#include <vector>
using namespace std;

namespace tdk {

class TDKKML_API KMLFeature : public KMLObject 
{
  protected:
    KMLFeature(const KMLType & objType, KMLObject * parent);

    KMLFeature(const KMLFeature & source);


  public:
    virtual ~KMLFeature();


  protected:
    KMLFeature & operator=(const KMLFeature & source);


  public:
    const string getName() const;

    void setName(const string & value);

    const bool getVisibility() const;

    void setVisibility(const bool & value);

    const bool getOpen() const;

    void setOpen(const bool & value);

    const string getAuthor() const;

    void setAuthor(const string & value);

    const string getLink() const;

    void setLink(const string & value);

    const string getStyleURL() const;

    void setStyleURL(const string & value);

    virtual void getChildFeatures(const KMLType & featureType, vector<KMLObject*> & features);

    /** @brief 
         */
        const string getDescription() const;

    /** @brief 
         */
        void setDescription(const string & value);


  protected:
    //Feature name.
    string _name;

    bool _visibility;

    bool _open;

    string _author;

    string _link;

    string _styleURL;

    string _description; //!< The feature description.    

};

} // namespace tdk
#endif
