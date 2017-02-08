#ifndef __WFSCLIENT_PLUGIN_H_
#define __WFSCLIENT_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>

//Forward declarations
class QAction;
struct WFSParams;

class WFSPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	WFSPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~WFSPlugin();

	/**	\brief Initializer method.
	 */
	void init();

	/**	\brief Finalizer method.
	 */
	void end();

protected slots:

	void showCreateThemeWindow();

	void createTheme(const WFSParams&);

protected:

	void initOWSPlatform();

	QAction* createTheme_;
	TeCommunicator<PluginsSignal> wmsPluginSignalReceiver_;
};

#endif //__WFSCLIENT_PLUGIN_H_