#ifndef __ORACLESPATIALADAPTER_PLUGIN_H_
#define __ORACLESPATIALADAPTER_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>


//Forward declarations
class QAction;

class oraSPAdaptPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	oraSPAdaptPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~oraSPAdaptPlugin();

	/**	\brief Initializer method.
	 */
	void init();

	/**	\brief Finalizer method.
	 */
	void end();

protected slots:

	void showWindow();

	QAction* showWinAction_;
};

#endif //__ORACLESPATIALADAPTER_PLUGIN_H_
