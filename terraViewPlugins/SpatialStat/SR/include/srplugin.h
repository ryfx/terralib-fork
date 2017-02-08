#ifndef __SR_PLUGIN_H_
#define __SR_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

//Forward declarations
class SRPluginWindow;

class SRPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	SRPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~SRPlugin();

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
	SRPluginWindow* win_;
};

#endif //__SR_PLUGIN_H_