#ifndef __JACQUEZ_KNN_PLUGIN_H_
#define __JACQUEZ_KNN_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

//Forward declarations
class kNNPluginWindow;

class KNNPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	KNNPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~KNNPlugin();

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
	kNNPluginWindow* win_;
};

#endif //__JACQUEZ_KNN_PLUGIN_H_