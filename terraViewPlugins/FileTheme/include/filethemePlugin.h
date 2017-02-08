#ifndef __FILETHEME_PLUGIN_H_
#define __FILETHEME_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Forward declarations
class QAction;
class AttributesTable;

//Qt include files
#include <qobject.h>

class FileThemePlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	FileThemePlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~FileThemePlugin();

	/**	\brief Initializer method.
	 */
	void init();

	/**	\brief Finalizer method.
	 */
	void end();

	static void signalHandler( const PluginsSignal& sig, void* receiver);

protected slots:

	void showWindow();

protected:

	void signalHandler( const PluginsSignal& sig);

	void drawEventHandler(const PluginsSignal& x );

    void pointingEventHandler(const PluginsSignal& x );

	QAction* showWinAction_;

	AttributesTable* objPropsWindow_;

public:

    TeCommunicator<PluginsSignal> comm_;
};

#endif //__FILETHEME_PLUGIN_H_
