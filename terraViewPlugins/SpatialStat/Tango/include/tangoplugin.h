#ifndef __TANGO_PLUGIN_H_
#define __TANGO_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

class TangoPluginWindow;

class TangoPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	TangoPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~TangoPlugin();

	/**	\brief Initializer method.
	 */
	void init();

	/**	\brief Finalizer method.
	 */
	void end();

protected slots:

	/**	\brief Slot used to present a window.
	 */
	void showWindow();

protected:

	QAction* showWinAction_;
	TangoPluginWindow* win_;
};

#endif //__TANGO_PLUGIN_H_