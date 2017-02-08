#ifndef __GRIMSON_PLUGIN_H_
#define __GRIMSON_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

//Forward declarations 
class GrimsonPluginWindow;

class GrimsonPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	GrimsonPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~GrimsonPlugin();

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
	GrimsonPluginWindow* win_;
};

#endif //__GRIMSON_PLUGIN_H_