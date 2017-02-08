#ifndef __MANTEL_PLUGIN_H_
#define __MANTEL_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

//Forward declarations
class MantelPluginWindow;

class MantelPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	MantelPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~MantelPlugin();

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
	MantelPluginWindow* win_;
};

#endif //__MANTEL_PLUGIN_H_