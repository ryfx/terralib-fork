#ifndef __KFUNCTIONS_PLUGIN_H_
#define __KFUNCTIONS_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

//Forward declarations
class KFunctionPluginWindow;

class KFuncPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	KFuncPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~KFuncPlugin();

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
	KFunctionPluginWindow* win_;
};

#endif //__KFUNCTIONS_PLUGIN_H_