#ifndef __BLIPPA_PLUGIN_H_
#define __BLIPPA_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>
#include <qaction.h>

//Forward declarations
class BLiPPAPluginWindow;

class BLiPPAPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	BLiPPAPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~BLiPPAPlugin();

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
	BLiPPAPluginWindow* win_;
};

#endif //__BLIPPA_PLUGIN_H_