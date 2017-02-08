#ifndef __POSTGISADAPTER_PLUGIN_H_
#define __POSTGISADAPTER_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>

class PostGISAdapter: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	PostGISAdapter(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~PostGISAdapter();

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

#endif //__POSTGISADAPTER_PLUGIN_H_