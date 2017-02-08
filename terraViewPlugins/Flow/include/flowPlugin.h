#ifndef __FLOW_PLUGIN_H_
#define __FLOW_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>

//Forward declarations
class QAction;

class FlowPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	FlowPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~FlowPlugin();

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

#endif //__FLOW_PLUGIN_H_
