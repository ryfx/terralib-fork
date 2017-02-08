#ifndef __KNOX_PLUGIN_H_
#define __KNOX_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

//Forward declarations
class KnoxPluginWindow;

class KnoxPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	KnoxPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~KnoxPlugin();

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
	KnoxPluginWindow* win_;
};

#endif //__KNOX_PLUGIN_H_