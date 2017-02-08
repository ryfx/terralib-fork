#ifndef __GEODMA_PLUGIN_H_
#define __GEODMA_PLUGIN_H_

#include <TViewAbstractPlugin.h>

// Qt include files
#include <qobject.h>

// Internal include files
#include <geodma_debug.h>

class geodma_plugin: public QObject, public TViewAbstractPlugin
{
  Q_OBJECT
  private:

  public:
    /** \brief Constructor.
    *	\param params Plugin parameters.
    */
    geodma_plugin(PluginParameters* params);

    /** \brief Destructor.
    */
    ~geodma_plugin();

    /** \brief Initializer method.
    */
    void init();

    /** \brief Finalizer method.
    */
    void end();
  
  protected slots:
    void execute_plugin();
};

#endif //__GEODMA_PLUGIN_H_