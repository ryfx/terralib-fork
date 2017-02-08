#ifndef __ATK_PLUGIN_H_
#define __ATK_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

//Forward declarations
class ATKPluginWindow;

class ATKPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	ATKPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~ATKPlugin();

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
	ATKPluginWindow* win_;
};

#endif //__ATK_PLUGIN_H_