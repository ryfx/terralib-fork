#ifndef __FILLCELL_PLUGIN_H_
#define __FILLCELL_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>

class FillCellPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	FillCellPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~FillCellPlugin();

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

#endif //__FILLCELL_PLUGIN_H_