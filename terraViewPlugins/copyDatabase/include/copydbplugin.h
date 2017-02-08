#ifndef __COPYDATABASE_PLUGIN_H_
#define __COPYDATABASE_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>

//Forward declarations
class QAction;

class CopyDBPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	CopyDBPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~CopyDBPlugin();

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
};

#endif //__COPYDATABASE_PLUGIN_H_
