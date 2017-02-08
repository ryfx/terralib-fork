#ifndef __BESAG_PLUGIN_H_
#define __BESAG_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>

//Forward declarations
class QAction;
class BesagPluginWindow;

class BesagePlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	BesagePlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~BesagePlugin();

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
	BesagPluginWindow* win_;
};

#endif //__BESAG_PLUGIN_H_