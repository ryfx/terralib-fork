#ifndef __SPATIALSCAN_PLUGIN_H_
#define __SPATIALSCAN_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

//Forward declarations
class SpatialScanPluginWindow;

class SPSCanPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	SPSCanPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~SPSCanPlugin();

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
	SpatialScanPluginWindow* win_;
};

#endif //__SPATIALSCAN_PLUGIN_H_