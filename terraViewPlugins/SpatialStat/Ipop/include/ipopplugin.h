#ifndef __IPOP_PLUGIN_H_
#define __IPOP_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

//Forward declarations
class IpopPluginWindow;

class IpopPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	IpopPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~IpopPlugin();

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
	IpopPluginWindow* win_;
};

#endif //__IPOP_PLUGIN_H_