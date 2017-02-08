#ifndef __CUZICK_PLUGIN_H_
#define __CUZICK_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

//Forward declarations
class CuzickPluginWindow;

class CuzickPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	CuzickPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~CuzickPlugin();

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
	CuzickPluginWindow* win_;
};

#endif //__CUZICK_PLUGIN_H_