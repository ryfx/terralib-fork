#ifndef __DIGGLE_PLUGIN_H_
#define __DIGGLE_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

class DigglePluginWindow;

class DigglePlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	DigglePlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~DigglePlugin();

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
	DigglePluginWindow* win_;
};

#endif //__DIGGLE_PLUGIN_H_