#ifndef __BITHELL_PLUGIN_H_
#define __BITHELL_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

//Forward declarations
class BithellPluginWindow;

class BithellPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	BithellPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~BithellPlugin();

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
	BithellPluginWindow* win_;
};

#endif //__BITHELL_PLUGIN_H_