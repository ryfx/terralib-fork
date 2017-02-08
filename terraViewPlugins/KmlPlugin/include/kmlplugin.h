#ifndef __KMLPLUGIN_PLUGIN_H_
#define __KMLPLUGIN_PLUGIN_H_

#include <TViewAbstractPlugin.h>

//Forward declarations
class QAction;
class KmlInformationBalloon;

//Qt include files
#include <qobject.h>


class KmlPlugin: public QObject, public TViewAbstractPlugin
{
	Q_OBJECT

public:
	/**	\brief Constructor.
	 *	\param params Plugin parameters.
	 */
	KmlPlugin(PluginParameters* params);

	/**	\brief Destructor.
	 */
	~KmlPlugin();

	/**	\brief Initializer method.
	 */
	void init();

	/**	\brief Finalizer method.
	 */
	void end();

	static void signalHandler( const PluginsSignal& sig, void* receiver);


protected:

	QAction* _importAction;

	QAction* _exportAction;

	QAction* _kmlThemeAction;

	map<std::string, QImage> _images;

	KmlInformationBalloon* _balloon;
	
	void signalHandler( const PluginsSignal& sig);

	void drawEventHandler(const PluginsSignal& x );

    void pointingEventHandler(const PluginsSignal& x );

	void showThemePopup(const PluginsSignal& x );

	void setThemeIcon(const PluginsSignal& x );

	bool hasHtml(const std::string & description);

	void callBrowser();

	void createTempFile(std::string code);

protected slots:

	void showImportWindow();

	void showExportWindow();

	void showKmlThemeWindow();

public:

    TeCommunicator<PluginsSignal> comm_;

};

#endif //__KMLPLUGIN_PLUGIN_H_