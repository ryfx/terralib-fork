#ifndef __OPGAM_PLUGIN_H_
#define __OPGAM_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

class OpgamPluginWindow;

class OpgamPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	OpgamPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~OpgamPlugin();

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
	OpgamPluginWindow* win_;
};

#endif //__OPGAM_PLUGIN_H_