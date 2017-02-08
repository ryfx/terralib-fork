#ifndef __WMSCLIENT_PLUGIN_H_
#define __WMSCLIENT_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>

//Forward declarations
class QAction;
class TeWMSConfigurer;

class WMSClientPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	WMSClientPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~WMSClientPlugin();

	/**	\brief Initializer method.
	 */
	void init();

	/**	\brief Finalizer method.
	 */
	void end();

protected slots:

	void showWindow();

protected:

	void initialize();

	QAction* showWinAction_;
	TeCommunicator<PluginsSignal> wmsPluginSignalReceiver_;
	TeWMSConfigurer* wmsConfigurer_;
};

#endif //__WMSCLIENT_PLUGIN_H_
