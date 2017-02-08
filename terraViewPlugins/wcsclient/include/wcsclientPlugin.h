#ifndef __WCSCLIENT_PLUGIN_H_
#define __WCSCLIENT_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>

//Forward declarations
class QAction;
//class TeWMSConfigurer;

class WCSClientPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	WCSClientPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~WCSClientPlugin();

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
};

#endif //__WCSCLIENT_PLUGIN_H_
