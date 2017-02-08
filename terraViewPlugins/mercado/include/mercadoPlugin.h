#ifndef __MERCADO_PLUGIN_H_
#define __MERCADO_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>

//Forward declarations
class QAction;

class mercadoPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	mercadoPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~mercadoPlugin();

	/**	\brief Initializer method.
	 */
	void init();

	/**	\brief Finalizer method.
	 */
	void end();

protected slots:

	void showWindow();

protected:
	QAction* showWinAction_;
};

#endif //__MERCADO_PLUGIN_H_
