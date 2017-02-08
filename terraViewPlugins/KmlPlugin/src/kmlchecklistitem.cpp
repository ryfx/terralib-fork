#include "kmlchecklistitem.h"
#include "kmlfeature.h"

KMLCheckListItem::KMLCheckListItem(QListViewItem * parent, KMLFeature * feature) :
QCheckListItem(parent, feature->getName().c_str(), QCheckListItem::CheckBoxController),
_feature(feature)
{

}

KMLCheckListItem::KMLCheckListItem(QListView * list, KMLFeature * feature) :
QCheckListItem(list, feature->getName().c_str(), QCheckListItem::CheckBoxController),
_feature(feature)
{

}

KMLCheckListItem::~KMLCheckListItem(){

}

void KMLCheckListItem::stateChange(bool state) 
{

	ToggleState s = QCheckListItem::state();

	if(s == NoChange)
		state = true;

	QCheckListItem::stateChange(state);

	if(_feature != NULL)
		_feature->setVisibility(state);
}

