#ifndef __IMORAN_PLUGIN_H_
#define __IMORAN_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

//Forward declarations
class IMoranPluginWindow;

class IMoranPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	IMoranPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~IMoranPlugin();

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
	IMoranPluginWindow* win_;
};

#endif //__IMORAN_PLUGIN_H_