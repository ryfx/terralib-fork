
#ifndef TDK_KMLSTRUCTUREITEM_H
#define TDK_KMLSTRUCTUREITEM_H

#include <vector>
using namespace std;

namespace tdk {

//Defines an item that can have a parent and chiildren. It may be used to contruct a hierarchy structure.
template<class T>
class KMLStructureItem {
  public:
    KMLStructureItem(KMLStructureItem<T> * parent = NULL);

    virtual ~KMLStructureItem();

    virtual void addChild(KMLStructureItem<T> * child);

    virtual void removeChild(KMLStructureItem<T> * child);

    KMLStructureItem<T>* getChild(unsigned int idx);

    KMLStructureItem<T>* parent();

    //Returns the number of children of the item.
    int childrenCount() const;


  protected:
    KMLStructureItem<T> * _parent;

    vector<KMLStructureItem<T>*> _children;

};
template<class T>
KMLStructureItem<T>::KMLStructureItem(KMLStructureItem<T> * parent) :
_parent(parent)
{
	_parent = parent;

	if(_parent != NULL)
		_parent->addChild(this);
}

template<class T>
KMLStructureItem<T>::~KMLStructureItem(){
	typename vector<KMLStructureItem<T>*>::iterator it;
	
	for(it = _children.begin(); it != _children.end(); ++it)
		delete *it;
}

template<class T>
void KMLStructureItem<T>::addChild(KMLStructureItem<T> * child) {
	_children.push_back(child);
	child->_parent = this;
}

template<class T>
void KMLStructureItem<T>::removeChild(KMLStructureItem<T> * child) {
	typename vector<KMLStructureItem<T>*>::iterator it;
	
	for(it == _children.begin(); it != _children.end(); ++it)
		if(*it == child)
			break;

	if(it != _children.end())
	{
		_children.erase(it);
		delete *it;
	}
}

template<class T>
KMLStructureItem<T>* KMLStructureItem<T>::getChild(unsigned int idx) 
{
	if(idx  >= _children.size() || idx < 0)
		return NULL;

	return _children[idx];
}

template<class T>
KMLStructureItem<T>* KMLStructureItem<T>::parent() 
{
	return _parent;
}

template<class T>
int KMLStructureItem<T>::childrenCount() const 
{
	return _children.size();
}


} // namespace tdk
#endif
