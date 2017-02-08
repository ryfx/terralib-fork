#ifndef __ROGERSON_PLUGIN_H_
#define __ROGERSON_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

//Forward declarations
class RogersonPluginWindow;

class RogersonPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	RogersonPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~RogersonPlugin();

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
	RogersonPluginWindow* win_;
};

#endif //__ROGERSON_PLUGIN_H_