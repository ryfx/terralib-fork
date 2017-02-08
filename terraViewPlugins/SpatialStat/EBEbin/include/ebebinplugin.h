#ifndef __EBEBIN_PLUGIN_H_
#define __EBEBIN_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

class EBEbinPluginWindow;

class EBEbinPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	EBEbinPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~EBEbinPlugin();

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
	EBEbinPluginWindow* win_;
};

#endif //__EBEBIN_PLUGIN_H_