#ifndef TDK_VISITRECEIVER_H
#define TDK_VISITRECEIVER_H


namespace tdk {

template<class Visitor>
class VisitReceiver 
{
  public:
    virtual ~VisitReceiver();

    virtual void accept(Visitor & v) = 0;


  protected:
    VisitReceiver();


  private:
    VisitReceiver(const VisitReceiver<Visitor> & source);

    VisitReceiver<Visitor> & operator=(const VisitReceiver<Visitor> & source);

};
template<class Visitor>
VisitReceiver<Visitor>::~VisitReceiver() 
{
}

template<class Visitor>
VisitReceiver<Visitor>::VisitReceiver() 
{
}


} // namespace tdk
#endif
