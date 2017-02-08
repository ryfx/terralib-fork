
#ifndef _KMLCHECKLISTITEM_H
#define _KMLCHECKLISTITEM_H

#include <qlistview.h>


namespace tdk { class KMLFeature; } 

using namespace tdk;

class  KMLCheckListItem : public QCheckListItem 
{
  public:
    KMLCheckListItem(QListViewItem * parent, KMLFeature * feature);

    KMLCheckListItem(QListView * list, KMLFeature * feature);

    ~KMLCheckListItem();

    void stateChange(bool state);


  protected:
    KMLFeature * _feature; //!< 

};
#endif
