#ifndef __EXTERNALTHEME_PLUGIN_H_
#define __EXTERNALTHEME_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Qt include files
#include <qobject.h>

//Forward declarations
class QAction;

class ExtThemePlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	ExtThemePlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~ExtThemePlugin();

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

#endif //__EXTERNALTHEME_PLUGIN_H_
